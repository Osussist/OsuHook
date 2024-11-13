#include "storage.h"

std::string Storage::baseDirectory = "";
std::string Storage::songsDirectory = "";
Parser::Database Storage::database(Storage::baseDirectory);
std::unordered_map<std::string, std::string> Storage::cachedBeatmaps;

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
	logger.info(__FUNCTION__, "Storage initialized successfully");
}

Parser::Beatmap Storage::get_beatmap(const std::string beatmapHash) {
	auto it = cachedBeatmaps.find(beatmapHash);
	if (it != cachedBeatmaps.end()) {
		logger.debug(__FUNCTION__, "Beatmap with hash " + beatmapHash + " found in cache");
		return Parser::Beatmap(it->second);
	}

	logger.debug(__FUNCTION__, "Beatmap with hash " + beatmapHash + " not found in cache");
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

void Storage::update_database() {
    Storage::database.~Database();
	Parser::Database database(Storage::baseDirectory + "\\osu!.db");
    logger.info(__FUNCTION__, "Database updated");
}

void Storage::on_beatmap_import(const std::wstring& beatmapPath, Logger logger) {
	if (beatmapPath.find(L".osu") == std::string::npos)
		return;

	std::string convertedPath = Translate::WcharToChar(beatmapPath.c_str());
	std::string beatmapHash = Translate::CharArrayToString(Crypto::GetMD5FromFile(convertedPath));
	logger.info(__FUNCTION__, "Caching beatmap with hash " + beatmapHash);
	cachedBeatmaps[beatmapHash] = convertedPath;
}