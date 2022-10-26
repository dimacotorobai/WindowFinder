#pragma once
#ifdef UNICODE
typedef std::wstring String;
#elif _MBCS
typedef std::string String;
#endif // UNICODE
