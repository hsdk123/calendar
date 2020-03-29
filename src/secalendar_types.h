#pragma once
#include <string>
#include <sstream>

using tchar = wchar_t;
#define STEXT(quote) L##quote 

using tstring = std::basic_string<tchar>;
using tstringstream = std::basic_stringstream<tchar>;
using tifstream = std::basic_ifstream<tchar>;
using tostringstream = std::basic_ostringstream<tchar>;
