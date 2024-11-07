#pragma once
#include "../utils/logger.h"
#include "./screen/screen.h"
#include "./storage/storage.h"
#include <tlhelp32.h>
#include <thread>
#include <chrono>

class SDK {
public:
	SDK(Logger logger);
	static Screen screen;
	static Storage storage;
	static HANDLE processHandle;
private:
	Logger logger;
	static DWORD processId;
};