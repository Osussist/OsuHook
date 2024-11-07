#include "sdk.h"

Screen SDK::screen = Screen();
Storage SDK::storage = Storage();
DWORD SDK::processId = 0;
HANDLE SDK::processHandle = nullptr;

std::string WcharToChar(const wchar_t* wchar) {
	int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wchar, -1, nullptr, 0, nullptr, nullptr);
	std::string charStr(bufferSize, 0);
	WideCharToMultiByte(CP_UTF8, 0, wchar, -1, &charStr[0], bufferSize, nullptr, nullptr);
	return charStr;
}

std::wstring CharToWChar(const char* charStr) {
	int bufferSize = MultiByteToWideChar(CP_UTF8, 0, charStr, -1, nullptr, 0);
	std::wstring wcharStr(bufferSize, 0);
	MultiByteToWideChar(CP_UTF8, 0, charStr, -1, &wcharStr[0], bufferSize);
	return wcharStr;
}

DWORD GetProcessIdByName(const std::string& rawName) {
    std::wstring processName = CharToWChar(rawName.c_str());
    PROCESSENTRY32 processInfo;
    processInfo.dwSize = sizeof(processInfo);

    DWORD processId = 0;
    while (processId == 0) {
        HANDLE processSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
        if (processSnapshot == INVALID_HANDLE_VALUE) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }

        if (Process32First(processSnapshot, &processInfo)) {
            do {
                if (!processName.compare(processInfo.szExeFile)) {
                    processId = processInfo.th32ProcessID;
                    break;
                }
            } while (Process32Next(processSnapshot, &processInfo));
        }

        CloseHandle(processSnapshot);

        if (processId == 0) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    return processId;
}


SDK::SDK(Logger sdkLogger): logger(sdkLogger) {
	logger.info(__FUNCTION__, "Initializing SDK");
	SDK::processId = GetProcessIdByName("osu!.exe");
    logger.debug(__FUNCTION__, "Found process ID: " + std::to_string(processId));
    SDK::processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	logger.debug(__FUNCTION__, "Found process handle: " + std::to_string(reinterpret_cast<int>(processHandle)));
	logger.info(__FUNCTION__, "Initializing managers");
    SDK::screen = Screen(logger, processHandle);
    SDK::storage = Storage(logger, processHandle);
    logger.info(__FUNCTION__, "SDK initialized");
}