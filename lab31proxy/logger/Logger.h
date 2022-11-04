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

#define TIME_NOW now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec
#define LOG_EVENT(event) Logger::getInstance().logEvent(event)
#define LOG_ERROR(event) Logger::getInstance().logError(event)
#define LOGGER_CLOSE Logger::getInstance().closeFile()

class Logger {
public:
    static Logger &getInstance() {
        static Logger instance;
        return instance;
    }

    void openFile() {
        loggerFile.open("log.txt");
    }

    void logEvent(std::string str) {
        std::time_t t = std::time(0);   // get time now
        std::tm* now = std::localtime(&t);
        loggerFile << "time: " << TIME_NOW << "   event --->    "  << str << std::endl;
    }

    void logError(std::string error) {
        std::time_t t = std::time(0);   // get time now
        std::tm* now = std::localtime(&t);
        loggerFile << "time: " << TIME_NOW << "   ERROR --->    "  << error << std::endl;
    }

    void closeFile() {
        loggerFile.close();
    }

private:
    Logger() {
        openFile();
    }

    ~Logger() { //???
        loggerFile.close();
    }

    std::ofstream loggerFile;
};


#endif //LAB31PROXY_LOGGER_H
