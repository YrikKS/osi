//
// Created by kurya on 06.11.2022.
//

#include <cstring>
#include "BufferImpl.h"

using namespace ProxyServer;

void BufferImpl::readFromSocket(BinaryString *binaryString) {
//    std::cout << "buf = " << buf << std::endl;
    _buf.add(*binaryString);
    if (_statusClient == StatusHttp::WRITE_REQUEST_HEADING) {
        int posEndHeading = 0;
        if (ParserImpl::findEndHeading(_buf.toSting(), &posEndHeading) == ResultPars::END_HEADING) {
            _requestHeading = _buf.subBinaryString(0, posEndHeading).toSting(); // так как не бинарные ресурсы
//            std::cout << _requestHeading << std::endl;
            parsHead();
            if (_resultParseHeading->getType() == TypeRequestAndResponse::GET_REQUEST) {
                if (checkCash())
                    return;
            }

//            parsHead();
            _isReadyConnectHttpServer = true;
            if (_resultParseHeading->getType() == GET_REQUEST) {
                _isReadyToSend = true;
                _isEndSend = true;
            } else {
                _isReadyToSend = true;
                _statusClient = StatusHttp::WRITE_REQUEST_BODY;
                _lengthBody = _resultParseHeading->getContentLength();
                _lengthBody -= _buf.getLength() - _requestHeading.size();
//                std::cout << "_lengthBody " << _lengthBody << std::endl;
                if (_lengthBody <= 0) {
                    _isEndSend = true;
                }
            }
        }
    } else if (_statusClient == StatusHttp::WRITE_REQUEST_BODY) {
        _isReadyToSend = true;
        _lengthBody -= binaryString->getLength();
//        std::cout << "_lengthBody " << _lengthBody << std::endl;
        if (_lengthBody <= 0) {
            _isEndSend = true;
        }
    } else if (_statusHttpServer == StatusHttp::WRITE_RESPONSE_HEADING) {
//        std::cout << "try read body response" << std::endl;
        std::cout.flush();
        int posEndHeading = 0;
        if (ParserImpl::findEndHeading(_buf.toSting(), &posEndHeading) == ResultPars::END_HEADING) {
            std::string responseHead = _buf.subBinaryString(0, posEndHeading).toSting();
//            std::cout << "parsing heading!" << std::endl;
            ResultParseHeading resultParseHeading = ParserImpl::parsingResponseHeading(responseHead);
//            std::cout << "parsing heading!" << std::endl;
            if (isCashingData(resultParseHeading)) {
                _isWrightDataToCash = true;
                LOG_EVENT("Add response to cash");
                _cashElement = _cash->addStringToCash(_requestHeading);
                _cashElement->getCash()->add(_buf); // TODO заменить
            }

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
                if (_cashElement != NULL) {
                    _cashElement->setIsCashEnd();
                }
                LOG_EVENT("end body response read");
            }
        } else {
            if (ParserImpl::findEndBody(_buf.toSting(), &posEnd) == ResultPars::END_BODY) {
                _isEndSend = true;
                if (_cashElement != NULL) {
                    _cashElement->setIsCashEnd();
                }
                LOG_EVENT("end body response read");
            }
        }
        if (_isWrightDataToCash) {
            (_cashElement->getCash())->add(*binaryString);
        }
    }
// TODO parse RESPONSE RESULT heading
}

bool BufferImpl::isCashingData(ResultParseHeading resultParseHeading) {
    if (!resultParseHeading.isResponseWithError() && (
            (_buf.getLength() + resultParseHeading.getContentLength()) < SIZE_EACH_CASH_ELEMENT) &&
        _resultParseHeading->getType() == TypeRequestAndResponse::GET_REQUEST &&
        resultParseHeading.isHaveContentLength())
        return true;
    return false;
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
        if (_statusClient == READ_RESPONSE && _isGetDataFromCash) { // TODO подумать как иначе
            _statusClient = StatusHttp::END_WORK;
            return;
        }

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
    if (_isGetDataFromCash && !_isEndSend) {
        BinaryString str = _cashElement->getCash()->subBinaryString(bytesReadFromCash,
                                                                    _cashElement->getCash()->getLength());
        if (str.getLength() > 0) {
            bytesReadFromCash += str.getLength();
            _buf.add(str);
            _isReadyToSend = true;
        }
        if (_cashElement->isCashEnd()) {
            _isEndSend = true;
        }
    }
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

bool BufferImpl::checkCash() {
    _cashElement = _cash->findResponseInCash(_requestHeading);
    if (_cashElement != NULL) {
        std::cout << "find in cash " << std::endl;
        _isGetDataFromCash = true;
        _buf.deleteData();
//        _buf.clearData();
        _buf.copyAndCreateData(*_cashElement->getCash());
        _statusClient = StatusHttp::READ_RESPONSE;
        if (_cashElement->isCashEnd()) {
            _isEndSend = true;
        } else {
            _isEndSend = false;
            bytesReadFromCash = _cashElement->getCash()->getLength();
        }
        _isReadyToSend = true;
        return true;
    }
    return false;
}

// socket reuse_add
