#include "sdk.h"

IPC SDK::ipc = IPC();
Screen SDK::screen = Screen();
Storage SDK::storage = Storage();
Memory SDK::memory = Memory();
DWORD SDK::processId = 0;
HANDLE SDK::processHandle = nullptr;

DWORD GetProcessIdByName(const wchar_t* processName) {
	DWORD processID = 0;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	while (processID == 0) {
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
		if (processID == 0) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		}
	}
	return processID;
}

SDK::SDK(Logger sdkLogger): logger(sdkLogger), processMonitor(nullptr), watcher(L"C:\\path\\to\\directory", SDK::storage.on_beatmap_import, logger) {
	logger.info(__FUNCTION__, "Initializing SDK");
	SDK::processId = GetProcessIdByName(L"osu!.exe");
	logger.debug(__FUNCTION__, "Found process ID: " + std::to_string(processId));
	SDK::processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	logger.debug(__FUNCTION__, "Found process handle: " + std::to_string(reinterpret_cast<int>(processHandle)));
	logger.info(__FUNCTION__, "Initializing managers");
	SDK::storage = Storage(logger, processHandle);
	SDK::memory = Memory(logger, processHandle);
	// SDK::ipc = IPC(logger, processHandle); <- This shit does not work YET, im gonna fix it... I pinky promise :)
	SDK::screen = Screen(logger, processHandle);
	logger.info(__FUNCTION__, "Initializing process monitor");
	SDK::processMonitor = ProcessMonitor(processHandle);
	processMonitor.StartMonitoring();
	SDK::watcher = FileSystemWatcher(Translate::CharToWchar(Storage::songsDirectory.c_str()), SDK::storage.on_beatmap_import, logger);
	watcher.StartWatcher();
	logger.info(__FUNCTION__, "SDK initialized");
}