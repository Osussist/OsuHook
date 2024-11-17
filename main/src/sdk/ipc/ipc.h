#pragma once
#include "../../utils/logger.h"

class IPC {
public:
	IPC() : logger(0, false) { return; }
	IPC(HANDLE processHandle);
private:
	Logger logger;
};