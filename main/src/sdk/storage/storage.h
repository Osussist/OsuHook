#pragma once
#include "../../utils/logger.h"
#include <osu!parser/Parser.hpp>
#include <memory>

class Storage {
public:
    Storage() : logger(0, false) { return; }
    Storage(Logger sdkLogger, HANDLE processHandle);

    static std::string baseDirectory;
    static std::string songsDirectory;
    static std::unique_ptr<Parser::Database> database;
private:
    Logger logger;
};
