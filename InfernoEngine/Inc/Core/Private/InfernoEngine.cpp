#include "InfernoEngine.h"
#include "Win32Window\Win32Window.h"

InfernoEngine::InfernoEngine()
{
	for (int i = 0; i < MAX_WINDOWS_COUNT; i++)
		m_pWindows[i] = 0;

	ZeroMemory(&m_msg, sizeof(MSG));
	m_iNumWindows = 0;
	m_bRunning = true;
}

InfernoEngine::~InfernoEngine() {}

uint16_t InfernoEngine::Run()
{
	while (m_bRunning)
	{
		for (int i = 0; i < m_iNumWindows; i++)
		{
			m_pWindows[i]->ProcessMessages();
		}
	}

	return 0;
}

// IN: HWND - Handle to the topmost window to be added
// OUT: bool - If the window was successfully added
//
// Adds a window to the window list. Note that only 3 windows can be added and they should be the top most, all children
// of those windows will be updated without needing to be added.
bool InfernoEngine::AddWindow(Win32Window* window)
{
	if (m_iNumWindows >= MAX_WINDOWS_COUNT)
		return false;

	m_pWindows[m_iNumWindows++] = window;
	return true;
}
// IN: HWND - Handle to window to remove
// OUT: bool - If the window was successfully found and removed
//
// Removes a window from the window list.
bool InfernoEngine::RemoveWindows(Win32Window* window)
{
	uint16_t WindowSlot = 0;
	while (WindowSlot < MAX_WINDOWS_COUNT && m_pWindows[WindowSlot++] != window);

	if (WindowSlot == MAX_WINDOWS_COUNT)
		return false;

	for (uint16_t i = WindowSlot; i < MAX_WINDOWS_COUNT - 1; i++)
		m_pWindows[i] = m_pWindows[i + 1];

	m_pWindows[MAX_WINDOWS_COUNT - 1] = 0;
	m_iNumWindows--;

	return true;
}

LRESULT InfernoEngine::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_QUIT:
	case WM_CLOSE:
		m_bRunning = false;
		PostQuitMessage(0);
		break;
	case WM_SIZE:
	{
		LONG_PTR WindowPtr = GetWindowLongPtr(hWnd, sizeof(Win32Window*));
		reinterpret_cast<Win32Window*>(WindowPtr)->SetSize(LOWORD(lParam), HIWORD(lParam));
		break;
	}
	case WM_MOVE:
	{
		LONG_PTR WindowPtr = GetWindowLongPtr(hWnd, sizeof(Win32Window*));
		reinterpret_cast<Win32Window*>(WindowPtr)->SetPosition(LOWORD(lParam), HIWORD(lParam));
		break;
	}
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}