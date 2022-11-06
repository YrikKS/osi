//
// Created by kurya on 06.11.2022.
//

#include "BufferImpl.h"

using namespace ProxyServer;

void BufferImpl::readRequest(char *buf) {
    _buf += std::string(buf);
    if (_statusClient == StatusHttp::WRITE_REQUEST_HEADING) {
        int posEndHeading = 0;
        if (ParserImpl::findEndHeading(_buf, &posEndHeading) == ResultPars::END_HEADING) {
            _requestHeading = _buf.substr(0, posEndHeading);
            _resultParseHeading = ParserImpl::parsingHeading(_requestHeading);

            if (_resultParseHeading->getType() == GET_REQUEST) {
//                _statusClient = StatusHttp::READ_RESPONSE;
                _isReadyToSend = true;
                _isEndSend = true;
            } else {
                _isReadyToSend = true;
                _statusClient = StatusHttp::WRITE_REQUEST_BODY;
                _lengthBody = _resultParseHeading->getContentLength();
                _lengthBody -= _buf.size() - _requestHeading.size();

                if (_lengthBody <= 0) {
//                    _statusClient = StatusHttp::READ_RESPONSE;
                    _isEndSend = true;
                }
            }
            _isReadyConnectHttpServer = true;
        }
    } else if (_statusClient == StatusHttp::WRITE_REQUEST_BODY) {
        _isReadyToSend = true;
        _lengthBody -= std::string(buf).size();
        if (_lengthBody <= 0) {
//            _statusClient = StatusHttp::READ_RESPONSE;
            _isEndSend = true;
        }
    } else if (_statusHttpServer == StatusHttp::WRITE_RESPONSE) {
        int posEnd = 0;
        _isReadyToSend = true;
        if (ParserImpl::findEndBody(_buf, &posEnd) == ResultPars::END_BODY) {
            _isEndSend = true;
        }
    }
    // TODO parse RESPONSE RESULT heading
}

void BufferImpl::readResponse(char *buf) {
    _buf += std::string(buf);
}

const char *BufferImpl::sendBuf() {
    if (_buf.size() > BUF_SIZE) {
        _sendingString = _buf.substr(0, BUF_SIZE);
    } else {
        _sendingString = _buf;
    }
    return _sendingString.c_str();
}

void BufferImpl::proofSend(const char *buf) {
    _buf.erase(0, std::string(buf).size());
    if (_buf.empty() && _isEndSend) {
        if (_statusHttpServer == StatusHttp::READ_REQUEST) {
            _statusClient = StatusHttp::READ_RESPONSE;
            _statusHttpServer = StatusHttp::WRITE_RESPONSE;
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

StatusHttp BufferImpl::getStatusBuf() {
    return _statusClient;
}

void BufferImpl::setStatusBuf(StatusHttp statusHttp) {
    _statusClient = statusHttp;
}

bool BufferImpl::isReadyToSend() {
    return _isReadyToSend;
}

BufferImpl::~BufferImpl() {

}

bool BufferImpl::isSendEnd() {
    return _isEndSend;
}

ResultParseHeading BufferImpl::getParseResult() {
    return *_resultParseHeading;
}
