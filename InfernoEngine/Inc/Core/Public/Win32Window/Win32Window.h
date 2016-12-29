// Win32Window.h
// Author: Jonathan Gyurkovics
// Purpose: This is a wrapper around the Windows Class functions to handle creating and upkeeping a window

#ifndef _WIN32WINDOW_H
#define _WIN32WINDOW_H

#include "Globals.h"
#include "IWndProc.h"

class Win32Window
{
private:
	enum {
		MOVE_WITH_PARENT = 1 << 0,
		STAY_WITHIN_PARENT = 1 << 1,
		CENTER_WINDOW = 1 << 2,
	};

private:
	HWND			m_hWnd;				// Windows Handle Pointer
	HINSTANCE		m_hInstance;		// Windows Instance Pointer
	const wchar_t*	m_wsName;			// Name String
	uint16_t		m_uiXPos;			// Position of the Window's X
	uint16_t		m_uiYPos;			// Position of the Window's Y
	uint16_t		m_uiWidth;			// Width of the Window
	uint16_t		m_uiHeight;			// Height of the Window
	DWORD			m_dStyle;			// Original style of the Window
	WindowMode		m_eWindowMode;		// Current Window Mode

	Win32Window*	m_pParent;			// Pointer to the parent Window
	Win32Window*	m_pChild;			// Pointer to the first child Window
	Win32Window*	m_pSibling;			// Pointer to the first sibling Window

	uint8_t			m_uiFlags;			// An 8-bit int to hold flags for the window

	// Delete the Copy Constructor and Operator
	Win32Window(const Win32Window&) = delete;
	Win32Window& operator=(const Win32Window&) = delete;

	// Private Functions
	// ------------------------------------

	// IN: Win32Window* child - Pointer to a child Window to Add
	// OUT: bool - If the child was added successfully
	//
	// Adds a Window as a child to this window
	bool AddChild(Win32Window* child);
	// IN: const Win32Window* child - Pointer to a child Window to Remove
	// OUT: bool - If the child was found and removed
	//
	// Searches this Window's children and attempts to remove the passed in child
	bool RemoveChild(const Win32Window* child);
	// IN: uint16_t dX - Change in the x Pos
	//     uint16_t dY - Change in the y Pos
	// OUT: void
	//
	// Updates the windows position if it is set to move with the parent window
	void ParentMoved(int16_t dX, int16_t dY);
	// IN: uint16_t& x - X Position being moved to
	//     uint16_t& y - Y Position being moved to
	// OUT: void
	//
	// Updates the x,y position if needed to stay within the parent window
	void ContainInParent(uint16_t& x, uint16_t& y);
	// IN: void
	// OUT: void
	//
	// Updates the Window if any of its information was changed
	void UpdateWindowState();

public:
	// Ctor/Dtor
	// ------------------------------------
	Win32Window(uint16_t width = 640, uint16_t height = 480, const wchar_t* name = L"");
	~Win32Window();

	// Public Functions
	// ------------------------------------

	// IN: IWndProc* object - Pointer to a IWndProc class
	//     LPCWSTR IconName - String name of the Icon **Optional**
	//     LPCWSTR SmallIconName - String name of the Small Icon **Optional**
	// OUT: bool - States if whether or not the initialization of the Window was successful
	//
	// Initializes the Window Object
	bool Initialize(const IWndProc* object, const LPCWSTR IconName = nullptr, const LPCWSTR SmallIconName = nullptr);
	// IN: void
	// OUT: void
	//
	// Destorys the Window
	void DestoryWindow();
	// IN: void
	// OUT: void
	//
	// Processes any messages that may have been sent to this window and then informs all children and siblings to do so as well
	void ProcessMessages();

	// Accessors
	// ------------------------------------
	const HWND GetHWnd() const;
	uint16_t GetWindowWidth() const;
	uint16_t GetWindowHeight() const;
	uint16_t GetMaxWidth() const;
	uint16_t GetMaxHieght() const;
	POINT	 GetPosition() const;
	uint32_t GetXPos() const;
	uint32_t GetYPos() const;
	WindowMode GetWindowMode() const;
	DWORD	 GetWindowStyle() const;
	RECT	 GetWindowRect() const;

	// Mutators
	// ------------------------------------
	void SetWindowMode(WindowMode mode);
	void SetWindowStyle(DWORD style);
	void SetPosition(int16_t x, int16_t y);
	void SetSize(uint16_t width, uint16_t height);
	void SetParent(Win32Window* parent);

	// Flag Accessors
	// ------------------------------------
	bool IsFollowingParent() const;
	bool CanLeaveParent() const;
	bool IsCenterWindow() const;

	// Flag Mutators
	// ------------------------------------
	void FollowParent(bool follow);
	void StayInParent(bool stayInParent);
	void CenterWindow(bool center);
};

#endif