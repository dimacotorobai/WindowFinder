#pragma once
BOOL GetProcessNameById(DWORD dwPID, TCHAR* szProcessName);

BOOL CALLBACK EnumarateWindowFunction(HWND hWnd, LPARAM lParam);

BOOL EnumarateOSWindows(std::vector<WINDOW_INFO>& WindowList);

DWORD GetProcessCreationTime(DWORD dwPID, LPSYSTEMTIME lpBuffer);

int SetConsoleMode();

BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);

BOOL ConsolePrintWindowInfo(const WINDOW_INFO& window);

void ClearConsoleOutput(HANDLE hConsole);

void KeyDown(BYTE vk);

void KeyUp(BYTE vk);