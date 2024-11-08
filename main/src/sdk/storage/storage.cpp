#include "storage.h"

std::string Storage::baseDirectory = "";
std::string Storage::songsDirectory = "";
Parser::Database Storage::database(Storage::baseDirectory);
std::list<std::pair<std::string, std::string>> Storage::cachedBeatmaps;

std::string GetHandlePath(HANDLE processHandle) {
	char buffer[MAX_PATH];
	GetModuleFileNameExA(processHandle, NULL, buffer, MAX_PATH);
	std::string path = buffer;
	return path.substr(0, path.find_last_of("\\/"));
}

Storage::Storage(Logger sdkLogger, HANDLE processHandle) : logger(sdkLogger) {
	logger.info(__FUNCTION__, "Initializing storage");
	Storage::baseDirectory = GetHandlePath(processHandle);
	logger.debug(__FUNCTION__, "Base directory: " + Storage::baseDirectory);
	Storage::songsDirectory = Storage::baseDirectory + "\\Songs";
	logger.debug(__FUNCTION__, "Songs directory: " + Storage::songsDirectory);
	Parser::Database database(Storage::baseDirectory + "\\osu!.db");
	logger.debug(__FUNCTION__, "Database initialized");
	logger.debug(__FUNCTION__, "Cached beatmaps initialized");
	logger.info(__FUNCTION__, "Storage initialized successfully");
}

Parser::Beatmap Storage::get_beatmap(std::string beatmapHash) {
	if (cachedBeatmaps.size() > 0) {
        for (const auto& cachedBeatmap : cachedBeatmaps) {
			if (cachedBeatmap.first == beatmapHash) {
				logger.debug(__FUNCTION__, "Beatmap with hash " + beatmapHash + " found in cache");
				return Parser::Beatmap(cachedBeatmap.second);
			}
        }
	}

	logger.debug(__FUNCTION__, "Beatmap with hash " + beatmapHash + " not found in cache");
	for (const Parser::BeatmapEntry& checkedBeatmap : database.Beatmaps)
	{
		for (const Parser::BeatmapEntry& checkedBeatmap : database.Beatmaps) {
			if (checkedBeatmap.BeatmapHash == beatmapHash) {
				Parser::Beatmap beatmap = Parser::Beatmap(checkedBeatmap.BeatmapPath);
				logger.debug(__FUNCTION__, "Beatmap with hash " + beatmapHash + " found in database");
				return beatmap;
			}
		}
		logger.debug(__FUNCTION__, "Beatmap with hash " + beatmapHash + " not found in database");
		return Parser::Beatmap("");
	}
}

void Storage::update_database() {
    Storage::database.~Database();
	Parser::Database database(Storage::baseDirectory + "\\osu!.db");
    logger.info(__FUNCTION__, "Database updated");
}

void Storage::on_beatmap_import(std::wstring beatmapPath) {
    std::string convertedPath = Translate::WcharToChar(beatmapPath.c_str());
	std::string beatmapHash = Translate::CharArrayToString(Crypto::get_md5_from_file(convertedPath));
	logger.debug(__FUNCTION__, "Beatmap imported at path: " + convertedPath);
	Storage::cachedBeatmaps.push_back(std::make_pair(beatmapHash, convertedPath));
	logger.info(__FUNCTION__, "Beatmap cached successfully");
}