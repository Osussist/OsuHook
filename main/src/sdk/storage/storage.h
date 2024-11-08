#pragma once
#include "../../utils/logger.h"
#include <osu!parser/Parser.hpp>
#include "../../utils/translate.h"
#include "../../utils/crypto.h"
#include "../../utils/watcher.h"
#include <memory>
#include <psapi.h>
#include <list>

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
    static std::list<std::pair<std::string, std::string>> cachedBeatmaps;
    static void on_beatmap_import(const std::wstring& beatmapPath);
};
