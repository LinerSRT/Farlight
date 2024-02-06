#include <Windows.h>


static bool IsBadReadPtr(void *p) {
    MEMORY_BASIC_INFORMATION mbi;

    if (VirtualQuery(p, &mbi, sizeof(mbi))) {
        constexpr DWORD mask = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
        bool b = !(mbi.Protect & mask);
        if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS))
            b = true;

        return b;
    }

    return true;
};