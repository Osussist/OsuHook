#pragma once
#include "../../utils/logger.h"
#include <osu!parser/Parser.hpp>
#include <memory>
#include <psapi.h>

class Storage {
public:
    Storage() : logger(0, false) { return; }
    Storage(Logger sdkLogger, HANDLE processHandle);
    
	Parser::Beatmap get_beatmap(std::string beatmapChecksum);

    static std::string baseDirectory;
    static std::string songsDirectory;
    static Parser::Database database;
private:
    Logger logger;
};
