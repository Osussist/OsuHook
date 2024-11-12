#pragma once
#include "../../utils/logger.h"
#include <FileWatcher.h>
#include <osu!parser/Parser.hpp>
#include "../../utils/translate.h"
#include "../../utils/crypto.h"
#include <unordered_map>
#include <memory>
#include <psapi.h>
#include <list>

class Storage {
public:
    Storage() : logger(0, false) { return; }
    Storage(Logger sdkLogger, HANDLE processHandle);
    
	Parser::Beatmap get_beatmap(const std::string beatmapHash);
	void update_database();

    static std::string baseDirectory;
    static std::string songsDirectory;
    static Parser::Database database;
private:
    Logger logger;
	static std::unordered_map<std::string, std::string> cachedBeatmaps;
    static void on_beatmap_import(const std::wstring& beatmapPath, Logger logger);
};
