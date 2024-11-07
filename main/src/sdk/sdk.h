#pragma once
#include "../utils/logger.h"
#include "./screen/screen.h"
#include <windows.h>
#include <tlhelp32.h>
#include <thread>
#include <chrono>

class SDK {
public:
	SDK(Logger logger);
	Screen metrics;
private:
	DWORD processId;
	Logger logger;
	HANDLE processHandle;
};