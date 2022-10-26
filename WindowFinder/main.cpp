#include "stdafx.h"
#include "typedef.h"
#include "structs.h"
#include "functions.h"
#include "controls.h"

//Global Variables
const TCHAR *szWindowName = TEXT("Window Finder");

int main(int argc, char* argv[])
{	
	//Set the name of app
	SetConsoleTitle(szWindowName);

	//Set the console output character set
	SetConsoleMode();

	//Allow app to set foreground
	AllowSetForegroundWindow(GetCurrentProcessId());

	setvbuf(stdin, NULL, _IONBF, 0);

	//Main Loop
	while (TRUE) {
		//Variables
		String sInput, sWindowName;
		TCHAR szInputBuffer[120];
		std::vector<WINDOW_INFO> WindowList;
		std::vector<WINDOW_INFO*> WindowsFound;

		//Front Menu UI
		std::wcout << R"(/////////////////////////////////////////////////////////////////////////////)" << std::endl;
		std::wcout << R"(// __        ___           _                 _____ _           _           //)" << std::endl;
		std::wcout << R"(// \ \      / (_)_ __   __| | _____      __ |  ___(_)_ __   __| | ___ _ __ //)" << std::endl;
		std::wcout << R"(//  \ \ /\ / /| | '_ \ / _` |/ _ \ \ /\ / / | |_  | | '_ \ / _` |/ _ \ '__|//)" << std::endl;
		std::wcout << R"(//   \ V  V / | | | | | (_| | (_) \ V  V /  |  _| | | | | | (_| |  __/ |   //)" << std::endl;
		std::wcout << R"(//    \_/\_/  |_|_| |_|\__,_|\___/ \_/\_/   |_|   |_|_| |_|\__,_|\___|_|   //)" << std::endl;
		std::wcout << R"(/////////////////////////////////////////////////////////////////////////////)" << std::endl;
		std::wcout << std::endl;

		//Take user input
		std::wcout << "Input a substring: ";
		std::wcin.getline(szInputBuffer, 120);
		std::wcout << std::endl;
		sInput.assign(szInputBuffer);

		//User confirmation(STDIN is weird AF)
		std::wcout << "Searching for \"" << sInput << "\"\n" << std::endl;

		//Enumarate all windows
		EnumarateOSWindows(WindowList);

		//Search window list and store
		for (const WINDOW_INFO& window : WindowList) {
			sWindowName.assign(window.szWindowName);
			if (sWindowName.find(sInput) != String::npos) {
				WindowsFound.push_back((WINDOW_INFO*)&window);
			}
		}

		if (WindowsFound.size() == 1) {
			ShowWindow(WindowsFound[0]->hWnd, SW_NORMAL);
			SetForegroundWindow(WindowsFound[0]->hWnd);
			ConsolePrintWindowInfo(*(WindowsFound[0]));
			std::wcout << std::endl;
		}
		else
		{
			if (WindowsFound.size() == 0) {
				std::wcout << "Error - Window was not found" << std::endl << std::endl;
			}
			else {
				for (int i = 0; i < WindowsFound.size(); i++)
				{
					ConsolePrintWindowInfo(*(WindowsFound[i]));
					std::wcout << std::endl;
				}

				std::wcout << "Error - " << WindowsFound.size() << " windows were found" << std::endl << std::endl;
			}
		}

		std::wcout << "[F1] Find Another Window" << std::endl;
		std::wcout << "[F2] Exit" << std::endl << std::endl;
		while (TRUE) {
			if (GetAsyncKeyState(VK_F1)) {
				KeyUp(VK_F1);
				break;
			}
			if (GetAsyncKeyState(VK_F2)) {
				KeyUp(VK_F2);
				return EXIT_SUCCESS;
			}
		}

		system("cls");
	}
}