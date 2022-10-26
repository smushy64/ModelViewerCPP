#include "utils.hpp"
#include "alias.hpp"
#include "debug.hpp"

#include <locale>
#include <codecvt>

std::wstring Utils::CStringToWString( const char* cstring ) {
    isize strLen = mbstowcs( nullptr, cstring, 0 );
    if( strLen == -1 ) {
        LOG_ERROR("Failed to convert const char* to std::wstring. Invalid source str!");
        return std::wstring();
    }

    std::wstring wstring( strLen, 0 );
    mbstowcs( &wstring[0], cstring, wstring.size() );

    return wstring;
}

void Utils::EraseSubString( std::string& string, const std::string& subString ) {
    usize position = string.find(subString);
    if( position != std::string::npos ) {
        string.erase( position, subString.length() );
    }
}

std::wstring Utils::StringtoWString( const std::string& string ) {
    isize strLen = mbstowcs( nullptr, &string[0], 0 );
    if( strLen == -1 ) {
        LOG_ERROR("Failed to convert std::string to std::wstring. Invalid source str!");
        return std::wstring();
    }

    std::wstring wstring( strLen, 0 );
    mbstowcs( &wstring[0], &string[0], wstring.size() );

    return wstring;
}

std::string Utils::WStringToString( const std::wstring& wstring ) {
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;
    return converter.to_bytes( wstring );
}
