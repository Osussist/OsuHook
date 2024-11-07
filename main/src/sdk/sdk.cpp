#include "sdk.h"

SDK::SDK(Logger sdkLogger) : logger(sdkLogger) {
	logger.info(__FUNCTION__, "SDK initialized");
}