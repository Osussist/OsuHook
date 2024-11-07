#pragma once
#include "../../utils/logger.h"
#include <osu!parser/Parser.hpp>

class Storage {
public:
	Storage(Logger sdkLogger, HANDLE processHandle);
private:
	Logger logger;
	HANDLE processHandle;
};