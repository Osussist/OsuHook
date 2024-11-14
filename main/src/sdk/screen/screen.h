#pragma once
#include "../../utils/logger.h"
#include "../objects/vector.h"
#include "../../utils/translate.h"
#include <thread>
#include <chrono>

struct WindowData {
    DWORD processID;
    HWND windowHandle;
};

class Screen {
public:
	Screen() : logger(0, false) { return; }
	Screen(Logger sdkLogger, HANDLE processHandle);

	template <typename T>
	Vector2<T> get_client_offset();
	template <typename T>
	Vector2<T> get_host_size();
	std::string get_client_title();
	std::string get_current_window_title();
private:
	Logger logger;
	static HWND windowHandle;
};