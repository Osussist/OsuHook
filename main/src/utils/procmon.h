#pragma once
#include <windows.h>
#include <thread>
#include <atomic>
#include <chrono>

class ProcessMonitor {
public:
	ProcessMonitor(HANDLE processHandle, int checkIntervalMs = 1000)
		: _processHandle(processHandle), _checkIntervalMs(checkIntervalMs), _isMonitoring(false) {}

	~ProcessMonitor() {
		StopMonitoring();
	}

	ProcessMonitor(ProcessMonitor&& other) noexcept
		: _processHandle(other._processHandle), _checkIntervalMs(other._checkIntervalMs), _isMonitoring(other._isMonitoring.load()) {
		other._isMonitoring = false;
		if (other._monitorThread.joinable()) {
			other._monitorThread.join();
		}
		_monitorThread = std::move(other._monitorThread);
	}

	ProcessMonitor& operator=(ProcessMonitor&& other) noexcept {
		if (this != &other) {
			StopMonitoring();
			_processHandle = other._processHandle;
			_checkIntervalMs = other._checkIntervalMs;
			_isMonitoring = other._isMonitoring.load();
			other._isMonitoring = false;
			if (other._monitorThread.joinable()) {
				other._monitorThread.join();
			}
			_monitorThread = std::move(other._monitorThread);
		}
		return *this;
	}

	void StartMonitoring() {
		_isMonitoring = true;
		_monitorThread = std::thread([this]() {
			while (_isMonitoring) {
				DWORD exitCode;
				if (!GetExitCodeProcess(_processHandle, &exitCode) || exitCode != STILL_ACTIVE) {
					ExitProcess(0);
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(_checkIntervalMs));
			}
		});
	}

	void StopMonitoring() {
		_isMonitoring = false;
		if (_monitorThread.joinable()) {
			_monitorThread.join();
		}
	}

private:
	HANDLE _processHandle;
	int _checkIntervalMs;
	std::atomic<bool> _isMonitoring;
	std::thread _monitorThread;
};
