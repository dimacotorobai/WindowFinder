#pragma once
BOOL GetProcessNameById(DWORD dwPID, TCHAR* szProcessName);

BOOL CALLBACK EnumarateWindowFunction(HWND hWnd, LPARAM lParam);

BOOL EnumarateOSWindows(std::vector<WINDOW_INFO>& WindowList);

int SetConsoleMode();

BOOL ConsolePrintWindowInfo(const WINDOW_INFO& window);

void KeyDown(BYTE vk);

void KeyUp(BYTE vk);