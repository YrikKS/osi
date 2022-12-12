//
// Created by kurya on 04.11.2022.
//

#ifndef LAB31PROXY_LOGGER_H
#define LAB31PROXY_LOGGER_H

#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>
#include <pthread.h>

#define TIME_NOW now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec
#define LOG_EVENT(event) Logger::getInstance().logEvent(event)
#define LOG_ERROR_WITH_ERRNO(event) Logger::getInstance().logError(event + std::string(strerror(errno)))
#define LOG_ERROR(event) Logger::getInstance().logError(event)
#define LOGGER_CLOSE Logger::getInstance().closeFile()

class Logger {
public:
    static Logger &getInstance() {
//        pthread_mutex_lock(&Logger::mutexForData);
        static Logger instance;
//        pthread_mutex_unlock(&mutexForData);
        return instance;
    }

    void openFile() {
        loggerFile.open("log.txt");
    }

    void logEvent(std::string str) {
//        pthread_mutex_lock(&mutexForData);
        std::time_t t = std::time(0);   // get time now
        std::tm *now = std::localtime(&t);
        loggerFile << "time: " << TIME_NOW << "   event --->    " << str << std::endl;
//        pthread_mutex_unlock(&mutexForData);
    }

    void logError(std::string error) {
//        pthread_mutex_lock(&mutexForData);
        std::time_t t = std::time(0);   // get time now
        std::tm *now = std::localtime(&t);
        loggerFile << "time: " << TIME_NOW << "   ERROR --->    " << error << std::endl;
//        pthread_mutex_unlock(&mutexForData);
    }

    void closeFile() {
        loggerFile.close();
    }

//    static pthread_mutex_t mutexForData;
private:
    Logger() {
//        pthread_mutex_init(&mutexForData, NULL);
        openFile();
    }

    ~Logger() { //???
        loggerFile.close();
//        pthread_mutex_destroy(&mutexForData);
    }

    std::ofstream loggerFile;
};


#endif //LAB31PROXY_LOGGER_H
