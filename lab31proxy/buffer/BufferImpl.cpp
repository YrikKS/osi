//
// Created by kurya on 06.11.2022.
//

#include <cstring>
#include "BufferImpl.h"

using namespace ProxyServer;

void BufferImpl::readRequest(char *buf) {
    _buf += std::string(buf);
    if (_statusClient == StatusHttp::WRITE_REQUEST_HEADING) {
        wrightRequestHeading(buf);
    } else if (_statusClient == StatusHttp::WRITE_REQUEST_BODY) {
        wrightRequestBody(buf);
    } else if (_statusHttpServer == StatusHttp::WRITE_RESPONSE_HEADING) {
        wrightResponseHeading(buf);
    } else if (_statusHttpServer == StatusHttp::WRITE_RESPONSE_BODY) {
        wrightResponseBody(buf);
    }
}

void BufferImpl::wrightRequestHeading(char *buf) {
    int posEndHeading = 0;
    if (ParserImpl::findEndHeading(_buf, &posEndHeading) == ResultPars::END_HEADING) {
        _requestHeading = _buf.substr(0, posEndHeading);
        parsHead();
        if (_resultParseHeading->getType() == TypeRequestAndResponse::GET_REQUEST) {
            if (checkCash())
                return;
        }

        _isReadyConnectHttpServer = true;
        if (_resultParseHeading->getType() == GET_REQUEST) {
            _isReadyToSend = true;
            _isEndSend = true;
        } else {
            _isReadyToSend = true;
            _statusClient = StatusHttp::WRITE_REQUEST_BODY;
            _lengthBody = _resultParseHeading->getContentLength();
            _lengthBody -= _buf.size() - _requestHeading.size();
            std::cout << "_lengthBody " << _lengthBody << std::endl;
            if (_lengthBody <= 0) {
                _isEndSend = true;
            }
        }
    }
}

void BufferImpl::wrightRequestBody(char *buf) {
    _isReadyToSend = true;
    _lengthBody -= std::strlen(buf);
//    std::cout << "_lengthBody " << _lengthBody << std::endl;
    if (_lengthBody <= 0) {
        _isEndSend = true;
    }
}

void BufferImpl::wrightResponseHeading(char *buf) {
    int posEndHeading = 0;
    if (ParserImpl::findEndHeading(_buf, &posEndHeading) == ResultPars::END_HEADING) {
        std::string responseHead = _buf.substr(0, posEndHeading);
        ResultParseHeading resultParseHeading = ParserImpl::parsingResponseHeading(responseHead);
        if (isCashingData(resultParseHeading)) {
            _isWrightDataToCash = true;
            LOG_EVENT("Add response to cash");
            if (_cashElement != NULL) {
                _cashElement = _cash->addStringToCash(_requestHeading);
                *_cashElement->getCash() += _buf;
            }
        }
        _isReadyToSend = true;
        _statusHttpServer = StatusHttp::WRITE_RESPONSE_BODY;
        _isHaveContentLengthresponse = resultParseHeading.isHaveContentLength();
        if (_isHaveContentLengthresponse) {
            _lengthBody = resultParseHeading.getContentLength();
            _lengthBody -= _buf.size() - responseHead.size();
            std::cout << "_lengthBody " << _lengthBody << std::endl;
            if (_lengthBody <= 0) {
                _isEndSend = true;
            }
        }

        int posEnd = 0;
        if (ParserImpl::findEndBody(_buf, &posEnd) == ResultPars::END_BODY) {
            _isEndSend = true;
        }
    }
}

void BufferImpl::wrightResponseBody(char *buf) {
    int posEnd = 0;
    _isReadyToSend = true;
    if (_isHaveContentLengthresponse) {
        _lengthBody -= std::strlen(buf);
        if (_lengthBody <= 0) {
            _isEndSend = true;
            if (_cashElement != NULL) {
                _cashElement->setIsCashEnd();
            }
            LOG_EVENT("end body response read");
        }
    }
    if (ParserImpl::findEndBody(_buf, &posEnd) == ResultPars::END_BODY) {
        _isEndSend = true;
        if (_cashElement != NULL) {
            _cashElement->setIsCashEnd();
        }
        LOG_EVENT("end body response read");
    }
    if (_isWrightDataToCash) {
        *(_cashElement->getCash()) += buf;
    }
}

bool BufferImpl::isCashingData(ResultParseHeading resultParseHeading) {
    if (!resultParseHeading.isResponseWithError() && (
            (_buf.size() + resultParseHeading.getContentLength()) < SIZE_EACH_CASH_ELEMENT) &&
        _resultParseHeading->getType() == TypeRequestAndResponse::GET_REQUEST &&
        resultParseHeading.isHaveContentLength())
        return true;
    return false;
}

void BufferImpl::readResponse(char *buf) {
    _buf += std::string(buf);
}

const char *BufferImpl::sendBuf() {
    if (_buf.size() >= BUF_SIZE - 1) {
        _sendingString = _buf.substr(0, BUF_SIZE - 1);
    } else {
        _sendingString = _buf;
    }
    return _sendingString.c_str();
}

void BufferImpl::proofSend(const char *buf) {
    _buf = _buf.erase(0, std::strlen(buf));
    if (_buf.empty() && !_isEndSend) {
        _isReadyToSend = false;
    }

    if (_buf.empty() && _isEndSend) {
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
        std::string str = _cashElement->getCash()->substr(bytesReadFromCash);
        bytesReadFromCash += str.size();
        _buf += str;
        _isReadyToSend = true;
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
        _resultParseHeading = ParserImpl::parsingHeading(_requestHeading);
    } catch (ParseException ex) {
        std::cerr << ex.what() << std::endl;
        _buf.clear();
        _buf += "incorrect heading\r\n";
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
        _isGetDataFromCash = true;
        _buf.clear();
        _buf = *_cashElement->getCash();
        _statusClient = StatusHttp::READ_RESPONSE;
        if (_cashElement->isCashEnd()) {
            _isEndSend = true;
        } else {
            _isEndSend = false;
            bytesReadFromCash = _cashElement->getCash()->size();
        }
        _isReadyToSend = true;
        return true;
    }
    return false;
}

//void BufferImpl::checkErrorLogout() {
//    if(_isWrightDataToCash && !_isEndSend && _statusHttpServer == StatusHttp::END_WORK) {
//        _cash->deleteCashElement(_cashElement);
//        _isGetDataFromCash = false;
//    }
//}
