#include <windows.h>
#include <sys/stat.h>
#include <tchar.h>
#include <string>
#include <algorithm>

using namespace std;

bool directory_delete(const char *pathname) {
    string str(pathname);
    if (!str.empty()) {
        while (*str.rbegin() == '\\' || *str.rbegin() == '/') {
            str.erase(str.size() - 1);
        }
    }
    replace(str.begin(), str.end(), '/', '\\');

    struct stat sb;
    if (stat((char *) str.c_str(), &sb) == 0 &&
        S_ISDIR(sb.st_mode)) {
        HANDLE hFind;
        WIN32_FIND_DATA FindFileData;

        TCHAR DirPath[MAX_PATH];
        TCHAR FileName[MAX_PATH];

        _tcscpy(DirPath, (char *) str.c_str());
        _tcscat(DirPath, "\\*");
        _tcscpy(FileName, (char *) str.c_str());
        _tcscat(FileName, "\\");

        hFind = FindFirstFile(DirPath, &FindFileData);
        if (hFind == INVALID_HANDLE_VALUE) return false;
        _tcscpy(DirPath, FileName);

        bool bSearch = true;
        while (bSearch) {
            if (FindNextFile(hFind, &FindFileData)) {
                if (!(_tcscmp(FindFileData.cFileName, ".") &&
                      _tcscmp(FindFileData.cFileName, "..")))
                    continue;
                _tcscat(FileName, FindFileData.cFileName);
                if ((FindFileData.dwFileAttributes &
                     FILE_ATTRIBUTE_DIRECTORY)) {
                    if (!directory_delete(FileName)) {
                        FindClose(hFind);
                        return false;
                    }
                    RemoveDirectory(FileName);
                    _tcscpy(FileName, DirPath);
                } else {
                    if (FindFileData.dwFileAttributes &
                        FILE_ATTRIBUTE_READONLY)
                        _chmod(FileName, _S_IWRITE);

                    if (!DeleteFile(FileName)) {
                        FindClose(hFind);
                        return false;
                    }
                    _tcscpy(FileName, DirPath);
                }
            } else {
                if (GetLastError() == ERROR_NO_MORE_FILES)
                    bSearch = false;
                else {
                    FindClose(hFind);
                    return false;
                }
            }
        }
        FindClose(hFind);
        return (RemoveDirectory((char *) str.c_str()) == true);
    } else {
        return false;
    }
}