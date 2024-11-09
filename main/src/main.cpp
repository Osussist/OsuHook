#include "./utils/logger.h"
#include "./sdk/sdk.h"
#include <iostream>
#include <exception>
#include <csignal>

Logger* globalLogger = nullptr;
std::atomic<bool> keepRunning(true);

void CustomTerminateHandler() {
    if (globalLogger) {
        globalLogger->error(__FUNCTION__, "Unhandled exception occurred. Terminating program.");
    }
    std::abort();
}

void SignalHandler(int signal) {
    keepRunning = false;
}

#ifdef _DEBUG
    #define LOG_TYPE LOG_TYPE_DEBUG
#else
    #define LOG_TYPE LOG_TYPE_INFO
#endif

int main() {
    Logger logger(LOG_TYPE, true);
    globalLogger = &logger;

    std::set_terminate(CustomTerminateHandler);

    try {
        SDK sdk(logger);

        std::signal(SIGINT, SignalHandler);
        std::signal(SIGTERM, SignalHandler);

        while (keepRunning) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    catch (const std::exception& e) {
        logger.error(__FUNCTION__, std::string("Exception caught: ") + e.what());
        return EXIT_FAILURE;
    }
    catch (...) {
        logger.error(__FUNCTION__, "Unknown exception caught");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}