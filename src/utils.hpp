#pragma once

#include <string>

namespace Utils {
    std::wstring CStringToWString( const char* cstring );
    void EraseSubString( std::string& string, const std::string& subString );
    std::wstring StringtoWString( std::string const& string );
} // namespace Utils
