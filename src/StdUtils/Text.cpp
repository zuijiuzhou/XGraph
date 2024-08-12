#include "Text.h"
#include <codecvt>
#include <locale>

namespace stdutils
{
    namespace
    {
        class codecvt_gbk : public std::codecvt_byname<wchar_t, char, std::mbstate_t>
        {
        public:
            codecvt_gbk()
#ifdef _WIN32
                : codecvt_byname("zh_CN")
#else
                : codecvt_byname("zh_CN.GBK")
#endif
            {
            }
        };
        std::wstring_convert<codecvt_gbk> s_GbkConvert;
        std::wstring_convert<std::codecvt_utf8<wchar_t>> s_Utf8Convert;
    }

    std::wstring gbkToUnicode(const std::string &str)
    {
        return s_GbkConvert.from_bytes(str);
    }
    std::string unicodeToGbk(const std::wstring &str)
    {
        return s_GbkConvert.to_bytes(str);
    }
    std::wstring utf8ToUnicode(const std::string &str)
    {
        return s_Utf8Convert.from_bytes(str);
    }
    std::string unicodeToUtf8(const std::wstring &str)
    {
        return s_Utf8Convert.to_bytes(str);
    }
    std::string gbkToUtf8(const std::string &str)
    {
        return s_Utf8Convert.to_bytes(s_GbkConvert.from_bytes(str));
    }
    std::string utf8ToGbk(const std::string &str)
    {
        return s_GbkConvert.to_bytes(s_Utf8Convert.from_bytes(str));
    }
}