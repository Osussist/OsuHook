#include "utils/logger.h"

#ifdef _DEBUG
	#define LOG_TYPE LOG_TYPE_DEBUG
#else
	#define LOG_TYPE LOG_TYPE_INFO
#endif

int main() {
    Logger logger(LOG_TYPE, true);
}