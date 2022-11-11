#pragma once
class WindowManager
{
public:
	WindowManager();
	bool Setup();

	std::vector<WINDOW_INFO> FindWindow(String WindowName);
	HWND SetForegroundWindow(WINDOW_INFO& Window);

	void PrintWindowInfo(WINDOW_INFO& Window);
	void PrintWindowListInfo(std::vector<WINDOW_INFO>& List);

	void SetQuitStatus(bool QuitStatus);
	bool ShouldQuit();

	~WindowManager();

	WindowManager(const WindowManager&) = default;
	WindowManager& operator=(const WindowManager&) = default;
private:
	uint32_t BackgroundThread();

private:
	bool bQuitManager, bQuitBackgroundTread;
	std::vector<WINDOW_INFO> Buffer1, Buffer2;
	std::vector<WINDOW_INFO>* WindowList;
	std::vector<WINDOW_INFO> WindowsFound;
	std::mutex mutex;
	std::thread threadObject;
};

