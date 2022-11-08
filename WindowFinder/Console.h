#pragma once
#include "WindowManager.h"

class Console
{
public:
	Console(String ConsoleName, uint16_t SizeX = 0, uint16_t SizeY = 0);
	uint32_t SetupConsole();
	void Display();
	uint32_t GetUserInput();
	uint32_t PollEvents();
	bool ShouldQuit();

	~Console();

private:
	bool bCloseConsole;
	String sConsoleName;
	uint16_t uSizeX, uSizeY;
	HWND hWnd;
	DWORD dwPID;
	WindowManager *manager;
};

