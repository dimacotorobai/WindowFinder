#pragma once
class WindowManager
{
public:
	WindowManager() noexcept;
	bool Setup() noexcept;

	std::vector<WINDOW_INFO> FindWindow(String WindowName) noexcept;
	HWND SetForeground(WINDOW_INFO& Window) const noexcept;

	void PrintWindowInfo(WINDOW_INFO& Window) const noexcept;
	void PrintWindowListInfo(std::vector<WINDOW_INFO>& List) const noexcept;

	void SetWindow(const WINDOW_INFO& Window) noexcept;
	const WINDOW_INFO& GetWindow() const noexcept;

	void SetQuitStatus(bool QuitStatus) noexcept;
	bool ShouldQuit() const noexcept;

	~WindowManager();

	WindowManager(const WindowManager&) = default;
	WindowManager& operator=(const WindowManager&) = default;

private:
	uint32_t BackgroundThread() noexcept;

private:
	bool		m_bQuitManager, m_bQuitBackgroundTread;
	std::mutex  m_Mutex;
	std::thread	m_ThreadObject;
	WINDOW_INFO	m_Window;
	std::vector<WINDOW_INFO>  m_Buffer1, m_Buffer2, m_WindowsFound;
	std::vector<WINDOW_INFO>* m_pWindowList;
};

