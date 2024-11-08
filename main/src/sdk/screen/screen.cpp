#include "screen.h"

HWND Screen::windowHandle = nullptr;

std::string WcharToChar(const wchar_t* wchar) {
	int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wchar, -1, nullptr, 0, nullptr, nullptr);
	std::string charStr(bufferSize, 0);
	WideCharToMultiByte(CP_UTF8, 0, wchar, -1, &charStr[0], bufferSize, nullptr, nullptr);
	return charStr;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	WindowData* data = reinterpret_cast<WindowData*>(lParam);
	DWORD windowPID = 0;
	GetWindowThreadProcessId(hwnd, &windowPID);

	if (windowPID == data->processID && IsWindowVisible(hwnd)) {
		data->windowHandle = hwnd;
		return FALSE;
	}
	return TRUE;
}

HWND FindWindowFromProcess(HANDLE processHandle) {
	DWORD processID = GetProcessId(processHandle);
	WindowData data = { processID, NULL };

	EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&data));
	return data.windowHandle;
}

Screen::Screen(Logger sdkLogger, HANDLE processHandle) : logger(sdkLogger) {
	Screen::windowHandle = FindWindowFromProcess(processHandle);
	logger.debug(__FUNCTION__, "Found window handle: " + std::to_string(reinterpret_cast<int>(Screen::windowHandle)));
	logger.info(__FUNCTION__, "Screen successfully initialized");
}

template <typename T>
Vector2<T> Screen::get_client_offset() {
	RECT clientRect;
	GetClientRect(Screen::windowHandle, &clientRect);
	return Vector2<T>(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
}

template <typename T>
Vector2<T> Screen::get_host_size() {
	return Vector2<T>(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
}

std::string Screen::get_client_title() {
	wchar_t title[256];
	GetWindowText(Screen::windowHandle, title, sizeof(title));
	return WcharToChar(title);
}

std::string Screen::get_current_window_title() {
	wchar_t title[256];
	GetWindowText(GetForegroundWindow(), title, sizeof(title));
	return WcharToChar(title);
}