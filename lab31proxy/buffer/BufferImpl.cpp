//
// Created by kurya on 06.11.2022.
//

#include <cstring>
#include "BufferImpl.h"

using namespace ProxyServer;

void BufferImpl::readFromSocket(BinaryString *binaryString) {
    _buf.add(*binaryString);
    if (_statusClient == StatusHttp::WRITE_REQUEST_HEADING) {
        int posEndHeading = 0;
        if (ParserImpl::findEndHeading(_buf.toSting(), &posEndHeading) == ResultPars::END_HEADING) {
            _requestHeading = _buf.subBinaryString(0, posEndHeading).toSting(); // так как не бинарные ресурсы
            parsHead();

            _isReadyConnectHttpServer = true;
            if (_resultParseHeading->getType() == GET_REQUEST) {
                _isReadyToSend = true;
                _isEndSend = true;
            } else {
                _isReadyToSend = true;
                _statusClient = StatusHttp::WRITE_REQUEST_BODY;
                _lengthBody = _resultParseHeading->getContentLength();
                _lengthBody -= _buf.getLength() - _requestHeading.size();
                if (_lengthBody <= 0) {
                    _isEndSend = true;
                }
            }
        }
    } else if (_statusClient == StatusHttp::WRITE_REQUEST_BODY) {
        _isReadyToSend = true;
        _lengthBody -= binaryString->getLength();
        if (_lengthBody <= 0) {
            _isEndSend = true;
        }
    } else if (_statusHttpServer == StatusHttp::WRITE_RESPONSE_HEADING) {
        int posEndHeading = 0;
        if (ParserImpl::findEndHeading(_buf.toSting(), &posEndHeading) == ResultPars::END_HEADING) {
            std::string responseHead = _buf.subBinaryString(0, posEndHeading).toSting();
            ResultParseHeading resultParseHeading = ParserImpl::parsingResponseHeading(responseHead);


            _isReadyToSend = true;
            _statusHttpServer = StatusHttp::WRITE_RESPONSE_BODY;
            _isHaveContentLengthresponse = resultParseHeading.isHaveContentLength();
            if (_isHaveContentLengthresponse) {
                _lengthBody = resultParseHeading.getContentLength();
                _lengthBody -= _buf.getLength() - responseHead.size();

//                std::cout << "_lengthBody " << _lengthBody << std::endl;
                if (_lengthBody <= 0) {
                    _isEndSend = true;
                }
            } else {
                int posEnd = 0;
                if (ParserImpl::findEndBody(_buf.toSting(), &posEnd) == ResultPars::END_BODY) {
                    _isEndSend = true;
                }
            }
        }
    } else if (_statusHttpServer == StatusHttp::WRITE_RESPONSE_BODY) {
        int posEnd = 0;
        _isReadyToSend = true;
        if (_isHaveContentLengthresponse) {
            _lengthBody -= binaryString->getLength();
            if (_lengthBody <= 0) {
                _isEndSend = true;
                LOG_EVENT("end body response read");
            }
        } else {
            if (ParserImpl::findEndBody(_buf.toSting(), &posEnd) == ResultPars::END_BODY) {
                _isEndSend = true;
                LOG_EVENT("end body response read");
            }
        }
    }
// TODO parse RESPONSE RESULT heading
}

//void BufferImpl::readResponse(char *buf) {
//    _buf += std::string(buf);
//}

void BufferImpl::sendBuf(BinaryString *binaryString) {
    if (_buf.getLength() >= BUF_SIZE - 1) {
        binaryString->copyData(_buf.subBinaryString(0, BUF_SIZE - 1));
    } else {
        binaryString->copyData(_buf);
    }
//    return _sendingString.c_str();
}

void BufferImpl::proofSend(BinaryString *binaryString) {
    _buf = _buf.subBinaryString(binaryString->getLength(), _buf.getLength());
//    std::cout << _buf.getLength() << std::endl;
    if (_buf.getLength() <= 0 && !_isEndSend) {
        _isReadyToSend = false;
    }

    if (_buf.getLength() <= 0 && _isEndSend) {
//        _lengthBody = 0;
//        std::cout << "!! end BODY" << std::endl;
        if (_statusClient == READ_RESPONSE && error) { // TODO подумать как иначе
            _statusClient = StatusHttp::END_WORK;
            return;
        }
//        if (_statusClient == READ_RESPONSE && _isGetDataFromCash) { // TODO подумать как иначе
//            _statusClient = StatusHttp::END_WORK;
//            return;
//        }

        if (_statusHttpServer == StatusHttp::READ_REQUEST) {
            _statusClient = StatusHttp::READ_RESPONSE;
            _statusHttpServer = StatusHttp::WRITE_RESPONSE_HEADING;
        } else if (_statusClient == StatusHttp::READ_RESPONSE) {
            _statusClient = StatusHttp::END_WORK;
            _statusHttpServer = StatusHttp::END_WORK;
        }
        _isReadyToSend = false;
        _isEndSend = false;
    }
}

bool BufferImpl::isReadyConnectHttpServer() {
    if (_isReadyConnectHttpServer) {
        _isReadyConnectHttpServer = false;
        return true;
    }
    return false;
}

StatusHttp BufferImpl::getStatusClient() {
    return _statusClient;
}

void BufferImpl::setStatusBuf(StatusHttp statusHttp) {
    _statusHttpServer = statusHttp;
}

bool BufferImpl::isReadyToSend() {
    return _isReadyToSend;
}

BufferImpl::~BufferImpl() {
    LOG_EVENT("delete buffer");
    if (_resultParseHeading != NULL) {
        delete _resultParseHeading;
    }
}

bool BufferImpl::isSendEnd() {
    return _isEndSend;
}

ResultParseHeading BufferImpl::getParseResult() {
    return *_resultParseHeading;
}

StatusHttp BufferImpl::getStatusHttpServer() {
    return _statusHttpServer;
}

BufferImpl::BufferImpl(Cash *cash) {
    _cash = cash;
}

void BufferImpl::parsHead() {
    try {
//        std::cout << "START normal parse" << std::endl;
        _resultParseHeading = ParserImpl::parsingHeading(_requestHeading);
//        std::cout << "normal parse" << std::endl;
    } catch (ParseException ex) {
        std::cerr << ex.what() << std::endl;
//        _buf.deleteData();
        _buf.clearData();
        char data[] = "incorrect heading\r\n";
        _buf.add(BinaryString(data, std::strlen(data)));
        _statusClient = StatusHttp::READ_RESPONSE;
        _isReadyToSend = true;
        _isEndSend = true;
        error = true;
        throw ParseException("incorrect heading");
    }
}


// socket reuse_add
