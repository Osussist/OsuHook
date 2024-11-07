#include "sdk.h"

std::string wcharToChar(const wchar_t* wchar) {
	int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wchar, -1, nullptr, 0, nullptr, nullptr);
	std::string charStr(bufferSize, 0);
	WideCharToMultiByte(CP_UTF8, 0, wchar, -1, &charStr[0], bufferSize, nullptr, nullptr);
	return charStr;
}

DWORD getProcessIdByName(const std::string& processName) {
    DWORD processId = 0;
    while (processId == 0) {
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        PROCESSENTRY32 entry;
        entry.dwSize = sizeof(entry);
        if (Process32First(snapshot, &entry)) {
            do {
                if (wcharToChar(entry.szExeFile) == processName) {
                    processId = entry.th32ProcessID;
                    break;
                }
            } while (Process32Next(snapshot, &entry));
        }
        CloseHandle(snapshot);

        if (processId == 0) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    return processId;
}

SDK::SDK(Logger sdkLogger)
	: logger(sdkLogger),
	processId(getProcessIdByName("osu!.exe")),
	processHandle(OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId)),
	metrics(sdkLogger, processHandle) {
    logger.debug(__FUNCTION__, "Found process ID: " + std::to_string(processId));
	logger.debug(__FUNCTION__, "Found process handle: " + std::to_string(reinterpret_cast<int>(processHandle)));
	logger.info(__FUNCTION__, "SDK initialized");
}