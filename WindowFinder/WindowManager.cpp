#include "stdafx.h"
#include "typedef.h"
#include "structs.h"
#include "functions.h"
#include "WindowManager.h"

WindowManager::WindowManager()
	: WindowList(&Buffer1), bQuitManager(false), bQuitBackgroundTread(false)
{
	Buffer1.reserve(500);
	Buffer2.reserve(500);
	WindowsFound.reserve(500);

	WindowList = &Buffer1;
}

bool WindowManager::Setup()
{
	threadObject = new std::thread(&WindowManager::BackgroundThread, this);
	return true;
}

std::vector<WINDOW_INFO> WindowManager::FindWindow(String PartialString)
{
	//Clear found windows buffer
	WindowsFound.clear();

	//Enum Window List and find matching windows
	mutex.lock();
	for (const auto& window : *WindowList)
	{
		String WindowName(window.szWindowName);
		if (!WindowName.find(PartialString))
		{
			WindowsFound.push_back(window);
		}
	}
	mutex.unlock();

	return WindowsFound;
}

HWND WindowManager::SetForegroundWindow(WINDOW_INFO& Window)
{
	//Check if window is minimized
	if (IsIconic(Window.hWnd))
		ShowWindow(Window.hWnd, SW_RESTORE);
	else
		ShowWindow(Window.hWnd, SW_SHOW);

	//Set foreground window
	::SetForegroundWindow(Window.hWnd);

	return Window.hWnd;
}

void WindowManager::PrintWindowInfo(WINDOW_INFO& Window)
{
	ConsolePrintWindowInfo(Window);
	std::wcout << std::endl;
}

void WindowManager::PrintWindowListInfo(std::vector<WINDOW_INFO>& List)
{
	for (const auto& window : List)
	{
		ConsolePrintWindowInfo(window);
		std::wcout << std::endl;
	}
}

std::mutex& WindowManager::GetMutex()
{
	return mutex;
}

void WindowManager::SetQuitStatus(bool QuitStatus)
{
	bQuitBackgroundTread = QuitStatus;
	bQuitManager = QuitStatus;
}

bool WindowManager::ShouldQuit()
{
	return bQuitManager;
}

WindowManager::~WindowManager()
{
	bQuitBackgroundTread = true;
	threadObject->join();
}

uint32_t WindowManager::BackgroundThread()
{
	while (!bQuitBackgroundTread)
	{
		if (WindowList == &Buffer1)
		{
			Buffer2.clear();
			EnumarateOSWindows(Buffer2);
			
			mutex.lock();
			WindowList = &Buffer2;
			mutex.unlock();
		}
		else if (WindowList == &Buffer2)
		{
			Buffer1.clear();
			EnumarateOSWindows(Buffer1);

			mutex.lock();
			WindowList = &Buffer1;
			mutex.unlock();
		}
		else
		{
			std::cout << "Windows Manager: Error - WindowList does not point to any buffer" << std::endl;
			return EXIT_FAILURE;
		}

		Sleep(2500);
	}

	return EXIT_SUCCESS;
}
