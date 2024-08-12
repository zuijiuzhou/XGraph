#pragma once
#include <string>


namespace stdutils{
    std::wstring gbkToUnicode(const std::string& str);

    std::string unicodeToGbk(const std::wstring& str);

    std::wstring utf8ToUnicode(const std::string& str);

    std::string unicodeToUtf8(const std::wstring& str);

    std::string gbkToUtf8(const std::string& str);

    std::string utf8ToGbk(const std::string& str);
}