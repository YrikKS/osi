//
// Created by kurya on 06.11.2022.
//

#include <cstring>
#include "BufferImpl.h"

using namespace ProxyServer;

void BufferImpl::readFromSocket(std::string *binaryString) {
//    std::cout << "add buf" << std::endl;
//    std::cout.flush();
//    _buf->resize(_buf->length() + (binaryString)->length());
//    _buf->insert(_buf->length(), *binaryString);
//    std::memcmp((void*) _buf->c_str(), )
//    *_buf += (*binaryString)->c_str();
    _buf->append(*binaryString);
//    (*_buf) += (*binaryString);
//    (binaryString)->resize(BUF_SIZE - 1);
//    std::memcpy((void *) (binaryString)->c_str(), _buf->c_str(), BUF_SIZE - 1);
//    std::cout << *_buf << std::endl;
    if (_statusClient == StatusHttp::WRITE_REQUEST_HEADING) {
        wrightRequestHeading(binaryString);
    } else if (_statusClient == StatusHttp::WRITE_REQUEST_BODY) {
        wrightRequestBody(binaryString);
    } else if (_statusHttpServer == StatusHttp::WRITE_RESPONSE_HEADING) {
        wrightResponseHeading(binaryString);
    } else if (_statusHttpServer == StatusHttp::WRITE_RESPONSE_BODY) {
        wrightResponseBody(binaryString);
    }
//    std::cout << "add buf end" << std::endl;
//    std::cout.flush();
// TODO parse RESPONSE RESULT heading
}

void BufferImpl::wrightRequestHeading(std::string *binaryString) {
    int posEndHeading = 0;
    if (ParserImpl::findEndHeading(*_buf, &posEndHeading) == ResultPars::END_HEADING) {
        _requestHeading = _buf->substr(0, posEndHeading); // так как не бинарные ресурсы
//        std::cout << _requestHeading << std::endl;
        parsHead();
        if (_cash->isElementInCash(_requestHeading)) {
            std::cout << "data get from cash" << std::endl;
            _isDataGetCash = true;
            _cashElement = _cash->findResponseInCash(_requestHeading);
            _cashElement->addCountUsers();
            _buf->clear();
//            _buf.mallocNeedSize(_cashElement->getCash()->getMallocedSize());
            _statusClient = StatusHttp::READ_RESPONSE;
            return;
        }

        _isReadyConnectHttpServer = true;
        std::cout << "ready connect to server" << std::endl;
        if (_resultParseHeading->getType() == GET_REQUEST) {
            _isReadyToSend = true;
            _isEndSend = true;
        } else {
            _isReadyToSend = true;
            _statusClient = StatusHttp::WRITE_REQUEST_BODY;
            _lengthBody = _resultParseHeading->getContentLength();
            _lengthBody -= _buf->length() - _requestHeading.size();
            if (_lengthBody <= 0) {
                _isEndSend = true;
            }
        }
    }
}

void BufferImpl::wrightRequestBody(std::string *binaryString) {
    _isReadyToSend = true;
    _lengthBody -= (binaryString)->length();
    if (_lengthBody <= 0) {
        _isEndSend = true;
    }
}

void BufferImpl::wrightResponseHeading(std::string *binaryString) {
    int posEndHeading = 0;
    if (ParserImpl::findEndHeading(*_buf, &posEndHeading) == ResultPars::END_HEADING) {
        std::string responseHead = _buf->substr(0, posEndHeading);
        ResultParseHeading resultParseHeading = ParserImpl::parsingResponseHeading(responseHead);

        if (isCashingData(responseHead.length(), resultParseHeading) &&
            !_cash->isElementInCash(_requestHeading)) {
            _cashElement = _cash->addStringToCash(_requestHeading);
            if (_cashElement != NULL) {
                _isAddDataToCash = true;
//                malloced = 829151 752 // 829151 232
                std::cout << "malloced == " << resultParseHeading.getContentLength() + responseHead.size() << std::endl;
//                _cashElement->getCash()->resize(resultParseHeading.getContentLength() + responseHead.size());
                _cashElement->getCash()->append(*_buf);
                _cashElement->setIsCashEnd(false);
                _cashElement->setIsServerConnect(true);
            } else {
                _isAddDataToCash = false;
            }
        }

        _isReadyToSend = true;
        _statusHttpServer = StatusHttp::WRITE_RESPONSE_BODY;
        _isHaveContentLengthresponse = resultParseHeading.isHaveContentLength();

        if (_isHaveContentLengthresponse) {
            _lengthBody = resultParseHeading.getContentLength();
            _lengthBody -= _buf->length() - responseHead.size();
            if (_lengthBody <= 0) {
                _isEndSend = true;
                if (_isAddDataToCash) {
                    _cashElement->setIsCashEnd(true);
                }
            }
        } else {
            int posEnd = 0;
            if (ParserImpl::findEndBody(*_buf, &posEnd) == ResultPars::END_BODY) {
                _isEndSend = true;
                if (_isAddDataToCash) {
                    std::cout << "!!maybe error" << std::endl;
//                    _cashElement->setIsCashEnd(true);
                }
            }
        }
    }
}

void BufferImpl::wrightResponseBody(std::string *binaryString) {
    int posEnd = 0;
    _isReadyToSend = true;
    std::cout << "read from server = " << (binaryString)->length() << std::endl;
    if (_isAddDataToCash) {
        *_cashElement->getCash() += (*binaryString);
    }
    if (_isHaveContentLengthresponse) {
        _lengthBody -= (binaryString)->length();
        if (_lengthBody <= 0) {
            _isEndSend = true;
            if (_isAddDataToCash) {
                _cashElement->setIsCashEnd(true);
            }
            LOG_EVENT("end body response read");
        }
    } else {
        if (ParserImpl::findEndBody(*_buf, &posEnd) == ResultPars::END_BODY) {
            if (_isAddDataToCash) {
                std::cout << "!!maybe Error" << std::endl;
//                _cashElement->setIsCashEnd(true);
            }
            _isEndSend = true;
            LOG_EVENT("end body response read");
        }
    }
}

bool BufferImpl::isCashingData(int sizeHeading, ResultParseHeading resultParseHeading) {
    if (!resultParseHeading.isResponseWithError() && (
            (_buf->length() + resultParseHeading.getContentLength()) < SIZE_EACH_CASH_ELEMENT) &&
        _resultParseHeading->getType() == TypeRequestAndResponse::GET_REQUEST &&
        resultParseHeading.isHaveContentLength())
        return true;
    return false;
}

void BufferImpl::sendBuf(std::string *binaryString) {
    if (_isDataGetCash) {
//        std::cout << _cashElement->getCash()->getLength() << " vs " << _countByteReadFromCash << std::endl;
        if (_cashElement->getCash()->length() > _countByteReadFromCash) {
            if (_cashElement->getCash()->length() >= _countByteReadFromCash + BUF_SIZE - 1) {
                (binaryString)->resize(BUF_SIZE - 1);
                std::memcpy((void *) (binaryString)->c_str(), _cashElement->getCash()->c_str() + _countByteReadFromCash, BUF_SIZE - 1);
//                (*binaryString)->setNewDataNotMallocWithPtr(_cashElement->getCash(), _countByteReadFromCash,
//                                                         _countByteReadFromCash + BUF_SIZE - 1);
//
////                binaryString->setNewDataNotMalloc(*_cashElement->getCash(), 0,
////                                                   BUF_SIZE - 1);
////                std::cout << "second == " << binaryString->getLength() << std::endl;
            } else {
                (binaryString)->resize(_cashElement->getCash()->length());
                std::memcpy((void *) (binaryString)->c_str(), _cashElement->getCash()->c_str() + _countByteReadFromCash, _cashElement->getCash()->length());
//                binaryString->setNewDataNotMallocWithPtr(_cashElement->getCash(), _countByteReadFromCash,
//                                                         _cashElement->getCash()->getLength());
//                std::cout << "second == " << binaryString->getLength() << std::endl;
            }
        }
//        std::cout << "second == " << binaryString->getLength() << std::endl;
    } else {
        if (_buf->length() >= BUF_SIZE - 1) {
            (binaryString)->resize(BUF_SIZE - 1);
            std::memcpy((void *) (binaryString)->c_str(), _buf->c_str(), BUF_SIZE - 1);
//            std::cout << "main == " << binaryString->getLength() << std::endl;
        } else {
            (binaryString)->resize(_buf->length());
            std::memcpy((void *) (binaryString)->c_str(), _buf->c_str(), _buf->length());
//            binaryString->setNewDataNotMalloc(_buf, 0, _buf.getLength()); // TODO: check
//            std::cout << "main == " << binaryString->getLength() << std::endl;
//        binaryString->copyData(_buf);
        }
//        std::cout << "main == " << binaryString->getLength() << std::endl;
    }
}

void BufferImpl::proofSend(std::string *binaryString) {
    if (_isDataGetCash) { // TODO: error rework !error
        _countByteReadFromCash += (binaryString)->length();
        // 71 ws 72
        std::cout << _cashElement->getCash()->length() << " ws " << _countByteReadFromCash << std::endl;
        if (_cashElement->getCash()->length() == _countByteReadFromCash) {
            _isReadyToSend = false;
        }
        if (_cashElement->isCashEnd()) {
            _isEndSend = true;
            _statusClient = StatusHttp::END_WORK;
            return;
        } else if (!_cashElement->isIsServerConnected()) {
            error = true;
        }
        return;
    }

    _buf->erase(0, (binaryString)->length());
//    _buf.shiftDataNotMalloc(_buf, binaryString->getLength());
    if (_buf->empty() && !_isEndSend) {
        _isReadyToSend = false;
    }


    if (_buf->empty() && _isEndSend) {
        if (_statusClient == READ_RESPONSE && error) { // TODO подумать как иначе
            _statusClient = StatusHttp::END_WORK;
            return;
        }
//        if (_statusClient == READ_RESPONSE && _isDataGetCash) { // TODO подумать как иначе
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

void BufferImpl::setStatusServer(StatusHttp statusHttp) {
    _statusHttpServer = statusHttp;
}

bool BufferImpl::isReadyToSend() {
    if (!_isReadyToSend && _isDataGetCash && _cashElement->getCash()->length() > _countByteReadFromCash) {
        _isReadyToSend = true;
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
        _buf->clear();
        char data[] = "incorrect heading\r\n";
        *_buf += data;
        _statusClient = StatusHttp::READ_RESPONSE;
        _isReadyToSend = true;
        _isEndSend = true;
        error = true;
        throw ParseException("incorrect heading");
    }
}

bool BufferImpl::isIsAddDataToCash() {
    return _isAddDataToCash;
}

bool BufferImpl::isIsDataGetCash() {
    return _isDataGetCash;
}

CashElement *BufferImpl::getCashElement() {
    return _cashElement;
}

void BufferImpl::setStatusClient(StatusHttp statusHttp) {
    _statusClient = statusHttp;
}

bool BufferImpl::isIsClientConnect() {
    return _isClientConnect;
}

void BufferImpl::setIsClientConnect(bool isClientConnect) {
    _isClientConnect = isClientConnect;
}

bool BufferImpl::isIsServerConnect() {
    return _isServerConnect;
}

void BufferImpl::setIsServerConnect(bool isServerConnect) {
    _isServerConnect = isServerConnect;
}



// socket reuse_add
