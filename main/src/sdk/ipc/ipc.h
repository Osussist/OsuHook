#pragma once
#include "../../utils/logger.h"

class IPC {
public:
	IPC() : logger(0, false) { return; }
	IPC(Logger sdkLogger, HANDLE processHandle);

	ClientData get_client_data();
private:
	Logger logger;
};

struct ClientData {
	static bool Buffering;
	static int AudioTime;
    static int ReplayTime;
    static int SpectatingID;
    static int Score;
    static int Mode;
    static bool SkipCalculations;
    static std::string BeatmapChecksum;
    static int BeatmapId;
    static int DimLevel;
    static int MenuTime;
    static bool AudioPlaying;
    static int LLastAction;
    static int LNextScoreSync;
    static bool LReplayMode;
    static bool LReplayStreaming;
    static int LReplayFrame;
    static bool LReplayToEnd;
};