#include "stdafx.h"
#include "typedef.h"
#include "structs.h"
#include "functions.h"

BOOL GetProcessNameById(DWORD dwPID, TCHAR* szProcessName)
{
	if (HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0))
	{
		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(pe32);

		if (Process32First(hSnapshot, &pe32))
		{
			do
			{
				if (pe32.th32ProcessID == dwPID)
				{
					lstrcpy(szProcessName, pe32.szExeFile);
					break;
				}
			} while (Process32Next(hSnapshot, &pe32));
		}
		CloseHandle(hSnapshot);
	}
	return TRUE;
}

BOOL CALLBACK EnumarateWindowFunction(HWND hWnd, LPARAM lParam)
{
	WINDOW_INFO tempBuffer = { 0 };
	std::vector<WINDOW_INFO>& WindowList = *(reinterpret_cast<std::vector<WINDOW_INFO>*>(lParam));

	if (hWnd)
	{
		tempBuffer.hWnd = hWnd;

		GetWindowThreadProcessId(hWnd, &tempBuffer.dwPID);
		if (tempBuffer.dwPID)
		{
			GetWindowText(hWnd, tempBuffer.szWindowName, MAX_PATH);
			GetProcessCreationTime(tempBuffer.dwPID, &tempBuffer.tCreationTime);

			if (lstrlen(tempBuffer.szWindowName))
			{
				GetProcessNameById(tempBuffer.dwPID, tempBuffer.szProcessName);
				WindowList.push_back(tempBuffer);
			}
		}
	}
	return TRUE;
}

BOOL EnumarateOSWindows(std::vector<WINDOW_INFO>& WindowList)
{
	EnumWindows(EnumarateWindowFunction, reinterpret_cast<LPARAM>(&WindowList));
	return TRUE;
}

DWORD GetProcessCreationTime(DWORD dwPID, LPSYSTEMTIME lpBuffer) {
	HANDLE					hProcess;
	FILETIME				creationTime, discard;
	LPFILETIME				_ = &discard;
	SYSTEMTIME				utcSystemTime;
	TIME_ZONE_INFORMATION	tzi;

	//Zero Buffer
	memset(lpBuffer, 0x0, sizeof(*lpBuffer));

	//Get handle to process
	hProcess = OpenProcess(
		PROCESS_QUERY_INFORMATION | PROCESS_QUERY_LIMITED_INFORMATION,
		FALSE,
		dwPID
	);

	//Skip if OpenProcess Failed
	if (!hProcess) return GetLastError();

	GetProcessTimes(hProcess, &creationTime, _, _, _);
	FileTimeToSystemTime(&creationTime, &utcSystemTime);

	GetTimeZoneInformation(&tzi);
	SystemTimeToTzSpecificLocalTime(&tzi, &utcSystemTime, lpBuffer);

	CloseHandle(hProcess);
	return 0;
}

int SetConsoleMode()
{
#ifdef UNICODE
	//Setting console for UTF16 characters
	return _setmode(_fileno(stdout), _O_WTEXT);
#else
	//Setting console output for ASCII characters
	return TRUE;
#endif // UNICODE
}

BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege) {
	LUID luid;
	BOOL bRet = FALSE;

	if (LookupPrivilegeValue(NULL, lpszPrivilege, &luid)) {
		TOKEN_PRIVILEGES tp;

		tp.PrivilegeCount = 1;
		tp.Privileges[0].Luid = luid;
		tp.Privileges[0].Attributes = (bEnablePrivilege) ? SE_PRIVILEGE_ENABLED : 0;

		if (AdjustTokenPrivileges(hToken, FALSE, &tp, NULL, (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL))
			bRet = (GetLastError() == ERROR_SUCCESS);
	}
	return bRet;
}

BOOL ConsolePrintWindowInfo(const WINDOW_INFO& window)
{
	std::wcout << "Window Name:  " << window.szWindowName << std::endl;
	std::wcout << "|-----------> Window HWND:  0x" << std::hex << window.hWnd << std::endl;
	std::wcout << "|-----------> Process Name: " << window.szProcessName << std::endl;
	std::wcout << "|-----------> Process ID:   0x" << window.dwPID << std::dec << std::endl;
	std::wcout << "|-----------> Creation Time: "
		<< window.tCreationTime.wYear << "-"
		<< window.tCreationTime.wMonth << "-"
		<< window.tCreationTime.wDay << "  "
		<< window.tCreationTime.wHour << ":"
		<< window.tCreationTime.wMinute << ":"
		<< window.tCreationTime.wSecond << std::endl;
	return TRUE;
}

void ClearConsoleOutput(HANDLE hConsole)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	SMALL_RECT scrollRect;
	COORD scrollTarget;
	CHAR_INFO fill;

	// Get the number of character cells in the current buffer.
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
	{
		return;
	}

	// Scroll the rectangle of the entire buffer.
	scrollRect.Left = 0;
	scrollRect.Top = 0;
	scrollRect.Right = csbi.dwSize.X;
	scrollRect.Bottom = csbi.dwSize.Y;

	// Scroll it upwards off the top of the buffer with a magnitude of the entire height.
	scrollTarget.X = 0;
	scrollTarget.Y = (SHORT)(0 - csbi.dwSize.Y);

	// Fill with empty spaces with the buffer's default text attribute.
	fill.Char.UnicodeChar = TEXT(' ');
	fill.Attributes = csbi.wAttributes;

	// Do the scroll
	ScrollConsoleScreenBuffer(hConsole, &scrollRect, NULL, scrollTarget, &fill);

	// Move the cursor to the top left corner too.
	csbi.dwCursorPosition.X = 0;
	csbi.dwCursorPosition.Y = 0;

	SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition);
}

void KeyDown(BYTE vk)
{
	return keybd_event(
		vk,
		MapVirtualKey(vk, MAPVK_VK_TO_VSC),
		KEYEVENTF_EXTENDEDKEY | 0,
		0
	);
}

void KeyUp(BYTE vk)
{
	return keybd_event(
		vk,
		MapVirtualKey(vk, MAPVK_VK_TO_VSC),
		KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
		0
	);
}