#include <windows.h>
#include <string>
#include <thread>
#include <iostream>
#include <functional>
#include <atomic>

class FileSystemWatcher {
public:
    using Callback = std::function<void(const std::wstring&, Logger logger)>;

    FileSystemWatcher(const std::wstring& directory, Callback callback, Logger givenLogger)
        : _directory(directory), _callback(callback), logger(givenLogger), _stop_flag(false) {}

    ~FileSystemWatcher() {
        StopWatcher();
    }

    FileSystemWatcher(FileSystemWatcher&& other) noexcept
        : _directory(std::move(other._directory)), _callback(std::move(other._callback)), 
        logger(std::move(other.logger)), _stop_flag(other._stop_flag.load()) {
        other._stop_flag.store(true);
        if (other._watcher_thread.joinable()) {
            other._watcher_thread.join();
        }
        _watcher_thread = std::move(other._watcher_thread);
    }

    FileSystemWatcher& operator=(FileSystemWatcher&& other) noexcept {
        if (this != &other) {
            StopWatcher();
            _directory = std::move(other._directory);
            _callback = std::move(other._callback);
            logger = std::move(other.logger);
            _stop_flag.store(other._stop_flag.load());
            other._stop_flag.store(true);
            if (other._watcher_thread.joinable()) {
                other._watcher_thread.join();
            }
            _watcher_thread = std::move(other._watcher_thread);
        }
        return *this;
    }

    void StartWatcher() {
        _watcher_thread = std::thread([this]() {
            _stop_flag.store(false);
            watch();
        });
    }

    void StopWatcher() {
        _stop_flag.store(true);
        if (_watcher_thread.joinable()) {
            _watcher_thread.join();
        }
    }

private:
    void watch() {
        logger.debug(__FUNCTION__, "Attempting to open directory: " + Translate::WcharToChar(_directory.c_str()));
        HANDLE hDir = CreateFileW(
            _directory.c_str(),
            FILE_LIST_DIRECTORY,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            NULL,
            OPEN_EXISTING,
            FILE_FLAG_BACKUP_SEMANTICS,
            NULL);

        if (hDir == INVALID_HANDLE_VALUE) {
            logger.error(__FUNCTION__, "Failed to open directory: " + std::to_string(GetLastError()));
            return;
        }

        logger.debug(__FUNCTION__, "Successfully opened directory: " + Translate::WcharToChar(_directory.c_str()));
        const DWORD bufferSize = 1024 * 10;
        BYTE buffer[bufferSize];
        DWORD bytesReturned;
        while (!_stop_flag.load()) {
            BOOL success = ReadDirectoryChangesW(
                hDir,
                &buffer,
                sizeof(buffer),
                TRUE, // Monitor subdirectories
                FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE,
                &bytesReturned,
                NULL,
                NULL);

            if (success) {
                FILE_NOTIFY_INFORMATION* notify;
                size_t offset = 0;
                do {
                    notify = (FILE_NOTIFY_INFORMATION*)((BYTE*)buffer + offset);
                    std::wstring fileName(notify->FileName, notify->FileNameLength / sizeof(WCHAR));

                    if (notify->Action == FILE_ACTION_ADDED || notify->Action == FILE_ACTION_MODIFIED) {
                        logger.debug(__FUNCTION__, "File added or modified: " + Translate::WcharToChar(fileName.c_str()));
                        _callback(_directory + L"\\" + fileName, logger);
                    }

                    offset += notify->NextEntryOffset;
                } while (notify->NextEntryOffset != 0);
            }
        }

        CloseHandle(hDir);
    }

    Logger logger;
    std::wstring _directory;
    Callback _callback;
    std::atomic<bool> _stop_flag;
    std::thread _watcher_thread;
};
