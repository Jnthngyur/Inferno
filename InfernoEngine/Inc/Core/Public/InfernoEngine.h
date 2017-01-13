// Main Engine which handles the main update loop and all systems

#ifndef _INFERNOENGINE_H
#define _INFERNOENGINE_H

#include "Win32Window\IWndProc.h"
#include "Globals.h"

#define MAX_WINDOWS_COUNT 3

class Win32Window;

class InfernoEngine : public IWndProc
{
private:
	MSG m_msg;
	Win32Window* m_pWindows[MAX_WINDOWS_COUNT];
	int m_iNumWindows;
	bool m_bRunning;

	virtual LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

public:
	InfernoEngine();
	~InfernoEngine();

	bool Initialize();

	uint16_t Run();

	// IN: HWND - Handle to the topmost window to be added
	// OUT: bool - If the window was successfully added
	//
	// Adds a window to the window list. Note that only 3 windows can be added and they should be the top most, all children
	// of those windows will be updated without needing to be added.
	bool AddWindow(Win32Window* window);
	// IN: HWND - Handle to window to remove
	// OUT: bool - If the window was successfully found and removed
	//
	// Removes a window from the window list.
	bool RemoveWindows(Win32Window* window);
};

#endif