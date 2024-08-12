#include "Environment.h"

#ifdef _WIN32
#include <Windows.h>
#endif

namespace stdutils
{
    std::string getApplicationDir()
    {
#ifdef _WIN32
        HMODULE hModule = GetModuleHandle(NULL);
        char path[MAX_PATH];
        GetModuleFileNameA(hModule, path, MAX_PATH);
        std::string fullPath(path);
        size_t pos = fullPath.find_last_of("\\/");
        return fullPath.substr(0, pos);
#else
    return {};
#endif
    }
}