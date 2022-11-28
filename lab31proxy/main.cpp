#include <iostream>
#include "server/ServerImpl.h"
#include "logger/Logger.h"

//#include "cash/CashImpl.h"
//#include "buffer/BufferImpl.h"

int main(int argc, char *argv[]) {
    LOG_EVENT("start program");
    try {
        ProxyServer::ServerImpl *server = new ProxyServer::ServerImpl();
        server->startServer();
    } catch (std::exception &ex) {
        std::cout << ex.what() << std::endl;
        LOG_ERROR("close app by exception");
    }
    LOG_EVENT("close app");
    LOGGER_CLOSE;


//    ProxyServer::Cash* cash = new ProxyServer::CashImpl();
//    ProxyServer::CashElement* cashElement = cash->addStringToCash("1");
//    cashElement->getCash()->add(ProxyServer::BinaryString("qweAsd", 6));
//    cashElement->getCash()->add(ProxyServer::BinaryString("123\0 456", 8));
//
//
//    ProxyServer::CashElement* cashElement2 = cash->addStringToCash("2");
//    cashElement2->getCash()->add(ProxyServer::BinaryString("qwerty", 6));
//    cashElement2->getCash()->add(ProxyServer::BinaryString("qwertyyu", 8));
//
//    ProxyServer::CashElement* cashElement3 = cash->addStringToCash("3");
//    cashElement3->getCash()->add(ProxyServer::BinaryString("asdfgh", 6));
//    cashElement3->getCash()->add(ProxyServer::BinaryString("zxcvbnm,", 8));
//
//    cash->findResponseInCash("1")->getCash()->printer();
//    cash->findResponseInCash("2")->getCash()->printer();
//    cash->findResponseInCash("3")->getCash()->printer();

//    ProxyServer::BufferImpl buffer = ProxyServer::BufferImpl(new ProxyServer::CashImpl());
//    char string[1024] = "GET http://detectportal.firefox.com/canonical.html HTTP/1.1\n"
//                        "Host: detectportal.firefox.com\n"
//                        "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:106.0) Gecko/20100101 Firefox/106.0\n"
//                        "Accept: */*\n"
//                        "Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3\n"
//                        "Accept-Encoding: gzip, deflate\n"
//                        "Cache-Control: no-cache\n"
//                        "Pragma: no-cache\n"
//                        "Connection: keep-alive\r\n\r\n";
//    ProxyServer::BinaryString binaryString(string, std::strlen(string));
//    buffer.readFromSocket(&binaryString);
//    std::cout << "buffer.isReadyConnectHttpServer() = " << buffer.isReadyConnectHttpServer() << std::endl;
//    std::cout << "buffer.isReadyToSend() = " << buffer.isReadyToSend() << std::endl;
//    std::cout << "buffer.isSendEnd() = " << buffer.isSendEnd() << std::endl;
//    std::cout << "buffer.getStatusHttpServer() = " << buffer.getStatusHttpServer() << std::endl;
//    std::cout << "buffer.getStatusClient() = " << buffer.getStatusClient() << std::endl;
//
//    char string2[1024] = {0};
//    ProxyServer::BinaryString binaryString2(string2, 1024);
//    buffer.sendBuf(&binaryString2);
//    std::cout << binaryString2.toSting() << std::endl;
//    buffer.proofSend(&binaryString2);
//
//    std::cout << "buffer.isReadyConnectHttpServer() = " << buffer.isReadyConnectHttpServer() << std::endl;
//    std::cout << "buffer.isReadyToSend() = " << buffer.isReadyToSend() << std::endl;
//    std::cout << "buffer.isSendEnd() = " << buffer.isSendEnd() << std::endl;
//    std::cout << "buffer.getStatusHttpServer() = " << buffer.getStatusHttpServer() << std::endl;
//    std::cout << "buffer.getStatusClient() = " << buffer.getStatusClient() << std::endl;
    return 0;
}
