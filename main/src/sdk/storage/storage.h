#pragma once
#include "../../utils/logger.h"
#include <osu!parser/Parser.hpp>
#include <memory>
#include <psapi.h>
#include <list>
#include <vector>

class Storage {
public:
    Storage() : logger(0, false) { return; }
    Storage(Logger sdkLogger, HANDLE processHandle);
    
	Parser::Beatmap get_beatmap(std::string beatmapChecksum);
	void update_database();

    static std::string baseDirectory;
    static std::string songsDirectory;
    static Parser::Database database;
private:
    Logger logger;
	static std::list<std::vector<std::string, Parser::Beatmap>> cachedBeatmaps;
};
