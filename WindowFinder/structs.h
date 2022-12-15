#pragma once
typedef struct {
	DWORD dwPID;
	TCHAR szProcessName[MAX_PATH];
	TCHAR szWindowName[MAX_PATH];
	HWND hWnd;
	SYSTEMTIME tCreationTime;
}WINDOW_INFO;