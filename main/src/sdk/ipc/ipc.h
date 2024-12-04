#pragma once
#include "../../utils/translate.h"
#include "../../utils/logger.h"
#include <windows.h>
#include <psapi.h>
#include <metahost.h>
#include <comutil.h>
#include <atlbase.h>
#include <atlcom.h>
#include <atlconv.h>
#include <mscoree.h>
#include <comdef.h>
#pragma comment(lib, "mscoree.lib")
#pragma comment(lib, "comsuppw.lib")

struct ClientData {
	bool Buffering;
	int AudioTime;
	int ReplayTime;
	int SpectatingID;
	int Score;
	int Mode;
	bool SkipCalculations;
	std::string BeatmapChecksum;
	int BeatmapId;
	int DimLevel;
	int MenuTime;
	bool AudioPlaying;
	int LLastAction;
	int LNextScoreSync;
	bool LReplayMode;
	bool LReplayStreaming;
	int LReplayFrame;
	bool LReplayToEnd;
};

struct CallbackData {
	std::wstring assemblyPath;
	std::wstring typeName;
	std::wstring methodName;
	std::wstring methodArgs;
	DWORD* pReturnValue;
};

class IPC {
public:
	IPC() : logger(0, false) { return; }
	IPC(Logger sdkLogger, HANDLE processHandle);

	ClientData get_client_data();
private:
	Logger logger;
	static ICLRMetaHost* pMetaHost;
	static ICLRRuntimeInfo* pRuntimeInfo;
	static ICLRRuntimeHost* pRuntimeHost;
	static std::string assemblyPath;
};
