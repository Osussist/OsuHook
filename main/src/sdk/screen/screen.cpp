#include "screen.h"

HWND FindWindowFromProcessId(DWORD processId) {
	HWND windowHandle = NULL;
	EnumWindows([](HWND windowHandle, LPARAM lParam) -> BOOL {
		DWORD windowProcessId;
		GetWindowThreadProcessId(windowHandle, &windowProcessId);
		if (windowProcessId == lParam) {
			*reinterpret_cast<HWND*>(lParam) = windowHandle;
			return FALSE;
		}
		return TRUE;
		}, reinterpret_cast<LPARAM>(&windowHandle));
	return windowHandle;
}

Screen::Screen(Logger sdkLogger, HANDLE processHandle) : logger(sdkLogger) {
	Screen::windowHandle = FindWindowFromProcessId(GetProcessId(processHandle));
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

std::wstring Screen::get_client_title() {
	wchar_t title[256];
	GetWindowText(Screen::windowHandle, title, sizeof(title));
	return std::wstring(title);
}

std::wstring Screen::get_current_window_title() {
	wchar_t title[256];
	GetWindowText(GetForegroundWindow(), title, sizeof(title));
	return std::wstring(title);
}