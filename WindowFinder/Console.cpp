#include "stdafx.h"
#include "typedef.h"
#include "structs.h"
#include "functions.h"
#include "Console.h"

Console::Console(String ConsoleName, uint16_t SizeX, uint16_t SizeY) noexcept
	:	m_sConsoleName(ConsoleName), m_uSizeX(SizeX), m_uSizeY(SizeY),
		m_bCloseConsole(false), m_pManager(nullptr),m_dwPID(0), m_hConsoleWindow(nullptr)
{
}

uint32_t Console::SetupConsole() noexcept
{
	TCHAR szConsoleName[MAX_PATH];
	RECT rect;

	//Get Console Info
	if (!(m_hConsoleWindow = GetConsoleWindow())) {
		std::wcout << "Error - Could not get console window handle" << std::endl;
		return EXIT_FAILURE;
	}
	
	//Get PID
	if (!(m_dwPID = GetCurrentProcessId())) {
		std::wcout << "Error - Could not get current process id" << std::endl;
		return EXIT_FAILURE;
	}

	//Set Console Mode and 
	SetConsoleMode();
	AllowSetForegroundWindow(GetCurrentProcessId());

	//Set Console Privilege
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &m_hToken)) {
		SetPrivilege(m_hToken, SE_DEBUG_NAME, TRUE);
		CloseHandle(m_hToken);
	}

	//Set Console Name
	GetConsoleTitle(szConsoleName, MAX_PATH);
	if (!m_sConsoleName.empty())
		SetConsoleTitle(m_sConsoleName.c_str());

	//Get Console Info
	if (m_uSizeX > 0 && m_uSizeY > 0) {
		GetWindowRect(m_hConsoleWindow, &rect);
		MoveWindow(m_hConsoleWindow, rect.left, rect.top, m_uSizeX, m_uSizeY, TRUE);
	}

	//Setup Windows Manager
	m_pManager = new WindowManager();
	if(!m_pManager->Setup())
		std::wcout << "Error - Could not setup Window Manager " << std::endl;

	return EXIT_SUCCESS;
}

void Console::ShowUserInterface() const noexcept
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

uint32_t Console::GetUserInput() const noexcept
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
	std::vector<WINDOW_INFO> WindowsFound = m_pManager->FindWindow(sInput);

	if (WindowsFound.size() == 1)  {
		m_pManager->PrintWindowInfo(WindowsFound[0]);
		m_pManager->SetForeground(WindowsFound[0]);
		m_pManager->SetWindow(WindowsFound[0]);
	}
	else {
		m_pManager->SetWindow({});
		if (WindowsFound.size() == 0) {
			std::wcout << "Error - Window was not found" << std::endl << std::endl;
		}
		else {
			m_pManager->PrintWindowListInfo(WindowsFound);
			std::wcout << "Error - " << WindowsFound.size() << " windows were found" << std::endl << std::endl;
		}
	}
	return uint32_t();
}

uint32_t Console::PollEvents() noexcept
{
	std::wcout << "[F1] Find Another Window" << std::endl;
	std::wcout << "[F2] Terminate Process" << std::endl;
	std::wcout << "[F3] Exit";
	while (TRUE) {
		if (GetAsyncKeyState(VK_F1)) {
			//Clear console output
			ClearConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE));

			//Release key
			KeyUp(VK_F1);

			//Erase leftover char
			KeyDown(VK_BACK); KeyUp(VK_BACK);

			break;
		}
		if (GetAsyncKeyState(VK_F2)) {
			//Clear console output
			ClearConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE));

			//Release key
			KeyUp(VK_F2);

			// Check if process if valid
			if (!m_pManager->GetWindow().dwPID) break;

			//Attach handle to process
			HANDLE hProcess = OpenProcess(
				PROCESS_TERMINATE,
				FALSE,
				m_pManager->GetWindow().dwPID
			);

			//Check if handle is valid
			if (!hProcess) break;
			TerminateProcess(hProcess, 0);
			CloseHandle(hProcess);

			break;
		}
		if (GetAsyncKeyState(VK_F3)) {
			//Release key
			KeyUp(VK_F3);

			//Quit Window Manager and console
			m_pManager->SetQuitStatus(true);
			m_bCloseConsole = true;

			break;
		}
	}
	return uint32_t();
}

bool Console::ShouldQuit() const noexcept
{
	return m_bCloseConsole;
}

Console::~Console()
{
	m_pManager->SetQuitStatus(true);
	m_pManager->~WindowManager();
}
