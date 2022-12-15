#include "stdafx.h"
#include "typedef.h"
#include "structs.h"
#include "functions.h"
#include "WindowManager.h"

WindowManager::WindowManager() noexcept
	: m_pWindowList(&m_Buffer1), m_bQuitManager(false), m_bQuitBackgroundTread(false)
{
	m_Buffer1.reserve(500);
	m_Buffer2.reserve(500);
	m_WindowsFound.reserve(500);

	m_pWindowList = &m_Buffer1;
}

bool WindowManager::Setup() noexcept
{
	m_ThreadObject = std::move(std::thread{ &WindowManager::BackgroundThread, this });
	return true;
}

std::vector<WINDOW_INFO> WindowManager::FindWindow(String PartialString) noexcept
{
	//Clear found windows buffer
	m_WindowsFound.clear();

	//Enum Window List and find matching windows
	m_Mutex.lock();
	for (const auto& window : *m_pWindowList)
	{
		if (String(window.szWindowName).find(PartialString) != std::string::npos)
		{
			m_WindowsFound.push_back(window);
		}
	}
	m_Mutex.unlock();

	return m_WindowsFound;
}

HWND WindowManager::SetForeground(WINDOW_INFO& Window) const noexcept
{
	//Check if window is minimized
	if (IsIconic(Window.hWnd))
		PostMessage(Window.hWnd,WM_SYSCOMMAND, SC_RESTORE, 0);
	else
		ShowWindow(Window.hWnd, SW_SHOW);

	//Set foreground window
	Sleep(100);
	SetForegroundWindow(Window.hWnd);

	return Window.hWnd;
}

void WindowManager::PrintWindowInfo(WINDOW_INFO& Window) const noexcept
{
	ConsolePrintWindowInfo(Window);
	std::wcout << std::endl;
}

void WindowManager::PrintWindowListInfo(std::vector<WINDOW_INFO>& List) const noexcept
{
	for (const auto& window : List)
	{
		ConsolePrintWindowInfo(window);
		std::wcout << std::endl;
	}
}

void WindowManager::SetWindow(const WINDOW_INFO& Window) noexcept
{
	m_Window = WINDOW_INFO(Window);
}

const WINDOW_INFO& WindowManager::GetWindow() const noexcept
{
	return m_Window;
}

void WindowManager::SetQuitStatus(bool QuitStatus) noexcept
{
	m_bQuitBackgroundTread = QuitStatus;
	m_bQuitManager = QuitStatus;
}

bool WindowManager::ShouldQuit() const noexcept
{
	return m_bQuitManager;
}

WindowManager::~WindowManager()
{
	m_bQuitBackgroundTread = true;
	m_ThreadObject.join();
}

uint32_t WindowManager::BackgroundThread() noexcept
{
	//Fill both buffers
	EnumarateOSWindows(m_Buffer1);
	m_Buffer2 = m_Buffer1;

	while (!m_bQuitBackgroundTread)
	{
		if (m_pWindowList == &m_Buffer1)
		{
			m_Buffer2.clear();
			EnumarateOSWindows(m_Buffer2);
			
			m_Mutex.lock();
			m_pWindowList = &m_Buffer2;
			m_Mutex.unlock();
		}
		else if (m_pWindowList == &m_Buffer2)
		{
			m_Buffer1.clear();
			EnumarateOSWindows(m_Buffer1);

			m_Mutex.lock();
			m_pWindowList = &m_Buffer1;
			m_Mutex.unlock();
		}
		else
		{
			std::wcout << "Windows Manager: Error - WindowList does not point to any buffer" << std::endl;
			return EXIT_FAILURE;
		}

		Sleep(2500);
	}

	return EXIT_SUCCESS;
}
