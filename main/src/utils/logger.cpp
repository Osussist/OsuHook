#include "logger.h"

std::ofstream Logger::logFile;
bool Logger::logToFile = false;
int Logger::logLevel = 0;

std::wstring stringToWideString(const std::string& str) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

Logger::Logger(const int logLevel, const bool fileLogging) {
    Logger::logToFile = fileLogging;
	Logger::logLevel = logLevel;
    if (fileLogging) {
        char buffer[FILENAME_MAX];
        auto temp = _getcwd(buffer, FILENAME_MAX);
        std::string exePath(buffer);
        Logger::logFile = std::ofstream(exePath + "\\log.txt", std::ios_base::app);
    }
    Logger::info(__FUNCTION__, "Logger initialized successfully");
}

void Logger::info(const std::string& function, const std::string& message) {
    if (Logger::logLevel < LOG_TYPE_INFO)
		return;

    std::cout << "[INFO] " << function << " | " << message << std::endl;
    if (Logger::logToFile) {
        Logger::logFile << "[INFO] " << function << " | " << message << std::endl;
    }
}

void Logger::error(const std::string& function, const std::string& message) {
    if (Logger::logLevel < LOG_TYPE_ERROR)
		return;

    std::cout << "[ERROR] " << function << " | " << message << std::endl;
    if (Logger::logToFile) {
        Logger::logFile << "[ERROR] " << function << " | " << message << std::endl;
    }
}

void Logger::warning(const std::string& function, const std::string& message) {
    if (Logger::logLevel < LOG_TYPE_WARNING)
        return;

    std::cout << "[WARNING] " << function << " | " << message << std::endl;
    if (Logger::logToFile) {
        Logger::logFile << "[WARNING] " << function << " | " << message << std::endl;
    }
}

void Logger::debug(const std::string& function, const std::string& message) {
	if (Logger::logLevel < LOG_TYPE_DEBUG)
		return;

    std::cout << "[DEBUG] " << function << " | " << message << std::endl;
    if (Logger::logToFile) {
        Logger::logFile << "[DEBUG] " << function << " | " << message << std::endl;
    }
}

void Logger::separator() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    std::cout << std::string(width, '-') << std::endl;
    std::cout << std::endl;
    if (Logger::logToFile) {
        Logger::logFile << std::string(width, '-') << std::endl;
        Logger::logFile << std::endl;
    }
}