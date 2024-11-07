#pragma once
#include <iostream>
#include <direct.h>
#include <windows.h>
#include <string>
#include <fstream>

#define DISABLED 0
#define LOG_TYPE_ERROR 1
#define LOG_TYPE_WARNING 2
#define LOG_TYPE_INFO 3
#define LOG_TYPE_DEBUG 4

class Logger {
public:
    Logger(const int logLevel, const bool logToFile);

    void info(const std::string& function, const std::string& message);
    void error(const std::string& function, const std::string& message);
    void warning(const std::string& function, const std::string& message);
    void debug(const std::string& function, const std::string& message);
    void separator();
private:
	static int logLevel;
    static bool logToFile;
    static std::ofstream logFile;
};