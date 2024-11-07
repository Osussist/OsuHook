#include "storage.h"
#include <psapi.h>

std::string Storage::baseDirectory = "";
std::string Storage::songsDirectory = "";
std::unique_ptr<Parser::Database> Storage::database = nullptr;

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
	Storage::database = std::make_unique<Parser::Database>(Storage::baseDirectory + "\\osu!.db");
	logger.debug(__FUNCTION__, "Database initialized");
	logger.info(__FUNCTION__, "Storage initialized successfully");
}
