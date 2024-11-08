#include "storage.h"

std::string Storage::baseDirectory = "";
std::string Storage::songsDirectory = "";
Parser::Database Storage::database(Storage::baseDirectory);


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

Parser::Beatmap Storage::get_beatmap(std::string beatmapHash) {
	for (const Parser::BeatmapEntry& checkedBeatmap : database.Beatmaps)
	{
		for (const Parser::BeatmapEntry& checkedBeatmap : database.Beatmaps) {
			if (checkedBeatmap.BeatmapHash == beatmapHash) {
				Parser::Beatmap beatmap = Parser::Beatmap(checkedBeatmap.BeatmapPath);
				return beatmap;
			}
		}
		logger.warning(__FUNCTION__, "Beatmap with hash " + beatmapHash + " not found in database");
		return Parser::Beatmap(""); // TODO: Implement cached beatmaps
	}
}