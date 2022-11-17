//
// Created by kurya on 06.11.2022.
//

#include <cstring>
#include "BufferImpl.h"

using namespace ProxyServer;

void BufferImpl::readFromSocket(BinaryString *binaryString) {
    _buf.add(*binaryString);
    if (_statusClient == StatusHttp::WRITE_REQUEST_HEADING) {
        wrightRequestHeading(binaryString);
    } else if (_statusClient == StatusHttp::WRITE_REQUEST_BODY) {
        wrightRequestBody(binaryString);
    } else if (_statusHttpServer == StatusHttp::WRITE_RESPONSE_HEADING) {
        wrightResponseHeading(binaryString);
    } else if (_statusHttpServer == StatusHttp::WRITE_RESPONSE_BODY) {
        wrightResponseBody(binaryString);
    }
// TODO parse RESPONSE RESULT heading
}

void BufferImpl::wrightRequestHeading(BinaryString *binaryString) {
    int posEndHeading = 0;
    if (ParserImpl::findEndHeading(_buf.toSting(), &posEndHeading) == ResultPars::END_HEADING) {
        _requestHeading = _buf.subBinaryString(0, posEndHeading).toSting(); // так как не бинарные ресурсы
//        std::cout << _requestHeading << std::endl;
        parsHead();
        if (_cash->isElementInCash(_requestHeading)) {
            std::cout << "data get from cash" << std::endl;
            _isDataGetCash = true;
            _cashElement = _cash->findResponseInCash(_requestHeading);
            _cashElement->addCountUsers();
            _buf.deleteData();
//            _buf.mallocNeedSize(_cashElement->getCash()->getMallocedSize());
            _statusClient = StatusHttp::READ_RESPONSE;
            return;
        } else {
//            std::cout << "NOT data get from cash" << std::endl;
        }

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
}

void BufferImpl::wrightRequestBody(BinaryString *binaryString) {
    _isReadyToSend = true;
    _lengthBody -= binaryString->getLength();
    if (_lengthBody <= 0) {
        _isEndSend = true;
    }
}

void BufferImpl::wrightResponseHeading(BinaryString *binaryString) {
    int posEndHeading = 0;
    if (ParserImpl::findEndHeading(_buf.toSting(), &posEndHeading) == ResultPars::END_HEADING) {
        std::string responseHead = _buf.subBinaryString(0, posEndHeading).toSting();
        ResultParseHeading resultParseHeading = ParserImpl::parsingResponseHeading(responseHead);

        if (isCashingData(responseHead.size(), resultParseHeading) &&
            !_cash->isElementInCash(_requestHeading)) {
            _cashElement = _cash->addStringToCash(_requestHeading);
            if (_cashElement != NULL) {
                _isAddDataToCash = true;
                std::cout << "malloced == " << resultParseHeading.getContentLength() + responseHead.size() << std::endl;
                _cashElement->getCash()->mallocNeedSize(resultParseHeading.getContentLength() + responseHead.size());
                _cashElement->getCash()->copyData(_buf);
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
            _lengthBody -= _buf.getLength() - responseHead.size();
            if (_lengthBody <= 0) {
                _isEndSend = true;
                if (_isAddDataToCash) {
                    _cashElement->setIsCashEnd(true);
                }
            }
        } else {
            int posEnd = 0;
            if (ParserImpl::findEndBody(_buf.toSting(), &posEnd) == ResultPars::END_BODY) {
                _isEndSend = true;
                if (_isAddDataToCash) {
                    std::cout << "!!maybe error" << std::endl;
//                    _cashElement->setIsCashEnd(true);
                }
            }
        }
    }
}

void BufferImpl::wrightResponseBody(BinaryString *binaryString) {
    int posEnd = 0;
    _isReadyToSend = true;
    std::cout << "read from server = " << binaryString->getLength() << std::endl;
    if (_isAddDataToCash) {
        _cashElement->getCash()->addToMallocedBuf(*binaryString);
    }
    if (_isHaveContentLengthresponse) {
        _lengthBody -= binaryString->getLength();
        if (_lengthBody <= 0) {
            _isEndSend = true;
            if (_isAddDataToCash) {
                _cashElement->setIsCashEnd(true);
            }
            LOG_EVENT("end body response read");
        }
    } else {
        if (ParserImpl::findEndBody(_buf.toSting(), &posEnd) == ResultPars::END_BODY) {
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
            (_buf.getLength() + resultParseHeading.getContentLength()) < SIZE_EACH_CASH_ELEMENT) &&
        _resultParseHeading->getType() == TypeRequestAndResponse::GET_REQUEST &&
        resultParseHeading.isHaveContentLength())
        return true;
    return false;
}

void BufferImpl::sendBuf(BinaryString *binaryString) {
    if (_isDataGetCash) {
//        std::cout << _cashElement->getCash()->getLength() << " vs " << _countByteReadFromCash << std::endl;
        if (_cashElement->getCash()->getLength() > _countByteReadFromCash) {
            if (_cashElement->getCash()->getLength() >= BUF_SIZE - 1) {
//                binaryString->setNewDataNotMalloc(*_cashElement->getCash(), _countByteReadFromCash,
//                                                  _countByteReadFromCash + BUF_SIZE - 1);

                binaryString->setNewDataNotMalloc(*_cashElement->getCash(), 0,
                                                   BUF_SIZE - 1);
                std::cout << "second == " << binaryString->getLength() << std::endl;
            } else {
                binaryString->setNewDataNotMalloc(*_cashElement->getCash(), 0,
                                                  _cashElement->getCash()->getLength());
                std::cout << "second == " << binaryString->getLength() << std::endl;
            }
        }
//        std::cout << "second == " << binaryString->getLength() << std::endl;
    } else {
        if (_buf.getLength() >= BUF_SIZE - 1) {
            binaryString->setNewDataNotMalloc(_buf, 0, BUF_SIZE - 1);
            std::cout << "main == " << binaryString->getLength() << std::endl;
        } else {
            binaryString->setNewDataNotMalloc(_buf, 0, _buf.getLength()); // TODO: check
            std::cout << "main == " << binaryString->getLength() << std::endl;
//        binaryString->copyData(_buf);
        }
//        std::cout << "main == " << binaryString->getLength() << std::endl;
    }
}

void BufferImpl::proofSend(BinaryString *binaryString) {
    if (_isDataGetCash && !error) {
        _countByteReadFromCash += binaryString->getLength();

        if (_cashElement->isCashEnd()) {
            _isEndSend = true;
            _statusClient = StatusHttp::END_WORK;
            return;
        } else if (!_cashElement->isIsServerConnected()) {
            error = true;
        }
        return;
    }
    _buf = _buf.subBinaryString(binaryString->getLength(), _buf.getLength());
    if (_buf.getLength() <= 0 && !_isEndSend) {
        _isReadyToSend = false;
    }

    if (_buf.getLength() <= 0 && _isEndSend) {
        if (_statusClient == READ_RESPONSE && error) { // TODO подумать как иначе
            _statusClient = StatusHttp::END_WORK;
            return;
        }
        if (_statusClient == READ_RESPONSE && _isDataGetCash) { // TODO подумать как иначе
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

void BufferImpl::setStatusServer(StatusHttp statusHttp) {
    _statusHttpServer = statusHttp;
}

bool BufferImpl::isReadyToSend() {
    if (!_isReadyToSend && _isDataGetCash && _cashElement->getCash()->getLength() > _countByteReadFromCash) {
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
