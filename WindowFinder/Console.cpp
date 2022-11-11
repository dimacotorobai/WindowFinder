#include "stdafx.h"
#include "typedef.h"
#include "structs.h"
#include "functions.h"
#include "Console.h"

Console::Console(String ConsoleName, uint16_t SizeX, uint16_t SizeY) 
	:	sConsoleName(ConsoleName), uSizeX(SizeX), uSizeY(SizeY),
		bCloseConsole(false), manager(nullptr),dwPID(0), hWnd(nullptr)
{
}

uint32_t Console::SetupConsole()
{
	TCHAR szConsoleName[MAX_PATH];
	RECT rect;

	//Get Console Info
	if (!(hWnd = GetConsoleWindow()))
	{
		std::wcout << "Error - Could not get console window handle" << std::endl;
		return EXIT_FAILURE;
	}
	
	//Get PID
	if (!(dwPID = GetCurrentProcessId()))
	{
		std::wcout << "Error - Could not get current process id" << std::endl;
		return EXIT_FAILURE;
	}

	//Set Console Mode and 
	SetConsoleMode();
	AllowSetForegroundWindow(GetCurrentProcessId());

	//Set Console Name
	GetConsoleTitle(szConsoleName, MAX_PATH);
	if (!sConsoleName.empty())
		SetConsoleTitle(sConsoleName.c_str());

	//Get Console Info
	if (uSizeX > 0 && uSizeY > 0)
	{
		GetWindowRect(hWnd, &rect);
		MoveWindow(hWnd, rect.left, rect.top, uSizeX, uSizeY, TRUE);
	}

	//Setup Windows Manager
	manager = new WindowManager();
	if(!manager->Setup())
	{
		std::wcout << "Error - Could not setup Window Manager " << std::endl;
	}

	return EXIT_SUCCESS;
}

void Console::Display()
{
	//Front Menu UI
	std::wcout << R"(////////////////////////////////////////////////////////////////////////////////////////////////////////////////////)" << std::endl;
	std::wcout << R"(////////////////////////////////////////////////////////////////////////////////////////////////////////////////////)" << std::endl;
	std::wcout << R"(////////////////////  __        ___           _                 _____ _           _             ////////////////////)" << std::endl;
	std::wcout << R"(////////////////////  \ \      / (_)_ __   __| | _____      __ |  ___(_)_ __   __| | ___ _ __   ////////////////////)" << std::endl;
	std::wcout << R"(////////////////////   \ \ /\ / /| | '_ \ / _` |/ _ \ \ /\ / / | |_  | | '_ \ / _` |/ _ \ '__|  ////////////////////)" << std::endl;
	std::wcout << R"(////////////////////    \ V  V / | | | | | (_| | (_) \ V  V /  |  _| | | | | | (_| |  __/ |     ////////////////////)" << std::endl;
	std::wcout << R"(////////////////////     \_/\_/  |_|_| |_|\__,_|\___/ \_/\_/   |_|   |_|_| |_|\__,_|\___|_|     ////////////////////)" << std::endl;
	std::wcout << R"(////////////////////                                                                            ////////////////////)" << std::endl;
	std::wcout << R"(////////////////////////////////////////////////////////////////////////////////////////////////////////////////////)" << std::endl;
	std::wcout << R"(////////////////////////////////////////////////////////////////////////////////////////////////////////////////////)" << std::endl;
	std::wcout << std::endl;

	return;
}

uint32_t Console::GetUserInput()
{
	//Variables
	String sInput, sWindowName;
	TCHAR szInputBuffer[MAX_PATH];

	//Take user input
	std::wcout << "Input a substring: ";
	std::wcin.getline(szInputBuffer, MAX_PATH);
	std::wcout << std::endl;
	sInput.assign(szInputBuffer);

	//User confirmation(STDIN is weird AF)
	std::wcout << "Searching for \"" << sInput << "\"\n" << std::endl;

	//Find Window
	std::vector<WINDOW_INFO> WindowsFound = manager->FindWindow(sInput);

	if (WindowsFound.size() == 1) 
	{
		manager->PrintWindowInfo(WindowsFound[0]);
		manager->SetForegroundWindow(WindowsFound[0]);
	}
	else
	{
		if (WindowsFound.size() == 0) {
			std::wcout << "Error - Window was not found" << std::endl << std::endl;
		}
		else {
			manager->PrintWindowListInfo(WindowsFound);
			std::wcout << "Error - " << WindowsFound.size() << " windows were found" << std::endl << std::endl;
		}
	}
	return uint32_t();
}

uint32_t Console::PollEvents()
{
	std::wcout << "[F1] Find Another Window" << std::endl;
	std::wcout << "[F2] Exit";
	while (TRUE) {
		if (GetAsyncKeyState(VK_F1)) {
			//Release key
			KeyUp(VK_F1);

			//Erase leftover char
			KeyDown(VK_BACK);
			KeyUp(VK_BACK);

			//Clear console output
			ClearConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE));
			break;
		}
		if (GetAsyncKeyState(VK_F2)) {
			//Release key
			KeyUp(VK_F2);

			//Quit Window Manager and console
			manager->SetQuitStatus(true);
			bCloseConsole = true;
			break;
		}
	}
	return uint32_t();
}

bool Console::ShouldQuit()
{
	return bCloseConsole;
}

Console::~Console()
{
	manager->SetQuitStatus(true);
	manager->~WindowManager();
}
