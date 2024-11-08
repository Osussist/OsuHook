#include <windows.h>
#include <string>
#include <thread>
#include <iostream>
#include <functional>

class FileSystemWatcher {
public:
    using Callback = std::function<void(const std::wstring&)>;

    FileSystemWatcher(const std::wstring& directory, Callback callback)
        : directory_(directory), callback_(callback), stop_flag_(false) {}

    ~FileSystemWatcher() {
        stop();
    }

    void SetupWatcher() {
        watcher_thread_ = std::thread(&FileSystemWatcher::watch, this);
    }

    void stop() {
        stop_flag_ = true;
        if (watcher_thread_.joinable()) {
            watcher_thread_.join();
        }
    }

private:
    void watch() {
        HANDLE hDir = CreateFileW(
            directory_.c_str(),
            FILE_LIST_DIRECTORY,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            NULL,
            OPEN_EXISTING,
            FILE_FLAG_BACKUP_SEMANTICS,
            NULL);

        if (hDir == INVALID_HANDLE_VALUE) {
            std::wcerr << L"Failed to open directory: " << directory_ << std::endl;
            return;
        }

        BYTE buffer[1024];
        DWORD bytesReturned;
        while (!stop_flag_) {
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
                        callback_(directory_ + L"\\" + fileName);
                    }

                    offset += notify->NextEntryOffset;
                } while (notify->NextEntryOffset != 0);
            }
        }

        CloseHandle(hDir);
    }

    std::wstring directory_;
    Callback callback_;
    bool stop_flag_;
    std::thread watcher_thread_;
};