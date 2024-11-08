#include "sdk.h"

Screen SDK::screen = Screen();
Storage SDK::storage = Storage();
DWORD SDK::processId = 0;
HANDLE SDK::processHandle = nullptr;

DWORD GetProcessIdByName(const wchar_t* processName) {
    DWORD processID = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 processEntry;
        processEntry.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(snapshot, &processEntry)) {
            do {
                if (_wcsicmp(processEntry.szExeFile, processName) == 0) {
                    processID = processEntry.th32ProcessID;
                    break;
                }
            } while (Process32Next(snapshot, &processEntry));
        }
        CloseHandle(snapshot);
    }
    return processID;
}

SDK::SDK(Logger sdkLogger): logger(sdkLogger) {
	logger.info(__FUNCTION__, "Initializing SDK");
	SDK::processId = GetProcessIdByName(L"osu!.exe");
    logger.debug(__FUNCTION__, "Found process ID: " + std::to_string(processId));
    SDK::processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	logger.debug(__FUNCTION__, "Found process handle: " + std::to_string(reinterpret_cast<int>(processHandle)));
	logger.info(__FUNCTION__, "Initializing managers");
    SDK::screen = Screen(logger, processHandle);
    SDK::storage = Storage(logger, processHandle);
    logger.info(__FUNCTION__, "SDK initialized");
}