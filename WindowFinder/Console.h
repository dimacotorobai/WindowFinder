#pragma once
#include "WindowManager.h"

class Console
{
public:
	Console(String ConsoleName, uint16_t SizeX = 0, uint16_t SizeY = 0) noexcept;
	uint32_t SetupConsole() noexcept;

	void ShowUserInterface() const noexcept;
	uint32_t GetUserInput() const noexcept;
	uint32_t PollEvents() noexcept;

	bool ShouldQuit() const noexcept;
	~Console();

private:
	bool m_bCloseConsole;
	String m_sConsoleName;
	uint16_t m_uSizeX, m_uSizeY;
	HWND m_hConsoleWindow;
	DWORD m_dwPID;
	HANDLE m_hToken;
	WindowManager *m_pManager;
};

