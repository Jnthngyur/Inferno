#include "Win32Window/Win32Window.h"
#include "AssertHelper.h"

// A global WndProc that is the default for the application
LRESULT CALLBACK GlobalWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LONG_PTR ObjPtr = GetWindowLongPtr(hWnd, 0);

	if (0 == ObjPtr) {
		return(DefWindowProc(hWnd, uMsg, wParam, lParam));
	}
	else {
		return reinterpret_cast<IWndProc*>(ObjPtr)->WndProc(hWnd, uMsg, wParam, lParam);
	}
}

Win32Window::Win32Window(const uint16_t width, const uint16_t height, const wchar_t* name)
{
	m_uiWidth = width;
	m_uiHeight = height;
	m_wsName = name;

	m_hWnd = nullptr;
	m_hInstance = nullptr;
	m_uiXPos = 0;
	m_uiYPos = 0;
	m_dStyle = WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
	m_eWindowMode = WindowMode::Windowed;

	m_pParent = nullptr;
	m_pChild = nullptr;
	m_pSibling = nullptr;

	m_uiFlags = 0;
}

Win32Window::~Win32Window()
{
	if (m_hWnd)
		DestoryWindow();
}

// Private Functions
// ------------------------------------

// IN: const Win32Window* child - Pointer to a child Window to Add
// OUT: bool - If the child was added successfully
//
// Adds a Window as a child to this window
bool Win32Window::AddChild(Win32Window* child)
{
	// First check if we have any children
	if (m_pChild == nullptr)
	{
		m_pChild = child;
		return true;
	}

	// Loop through our children to make sure we are not adding the same child twice
	Win32Window* curr = m_pChild;
	while (curr != nullptr) {
		if (curr == child)
			return false;

		curr = curr->m_pSibling;
	}

	// If the child was not found we can add it
	curr->m_pSibling = child;

	return true;
}

// IN: const Win32Window* child - Pointer to a child window
// OUT: bool - If the child was found and removed
//
// Searches this Window's children and attempts to remove the passed in child
bool Win32Window::RemoveChild(const Win32Window* child)
{
	// Make sure we have children first
	if (m_pChild == nullptr)
		return false;

	// See if our first child is the one to remove
	if (m_pChild == child)
	{
		m_pChild = m_pChild->m_pSibling;
		return true;
	}

	// Loop through our children to try to find the child to remove
	Win32Window* curr = m_pChild;
	while (curr->m_pSibling != nullptr) {
		if (curr->m_pSibling == child)
		{
			curr->m_pSibling = child->m_pSibling;
			return true;
		}

		curr = curr->m_pSibling;
	}

	// If the child was not found
	return false;
}

// IN: int16_t dX - Change in the x Pos
//     int16_t dY - Change in the y Pos
// OUT: void
//
// Updates the windows position if it is set to move with the parent window
void Win32Window::ParentMoved(int16_t dX, int16_t dY)
{
	if (m_uiFlags & MOVE_WITH_PARENT && !(m_uiFlags & STAY_WITHIN_PARENT))
	{
		// Move our position based on our parents movement
		SetPosition(m_uiXPos + dX, m_uiYPos + dY);

		if (m_pChild)
			m_pChild->ParentMoved(dX, dY);
	}

	if (m_pSibling)
		m_pSibling->ParentMoved(dX, dY);
}

// IN: uint16_t& x - X Position being moved to
//     uint16_t& y - Y Position being moved to
// OUT: void
//
// Updates the x,y position if needed to stay within the parent window
void Win32Window::ContainInParent(uint16_t& x, uint16_t& y)
{
	// Get the top left pixel position of the parent window
	POINT ParentPos = m_pParent->GetPosition();
	// Get the size of the Window Style used by the child
	RECT ParentRect, WindowRect = { 0, 0, 0, 0 };

	ParentRect = m_pParent->GetWindowRect();
	AdjustWindowRect(&WindowRect, m_dStyle, FALSE);

	RECT newRect = { x + WindowRect.left, y + WindowRect.top, x + m_uiWidth + WindowRect.right, y + m_uiHeight + WindowRect.bottom };

	// If our width is equal to or greater than our parents just set our position to the left-most pixel of our parent
	if (m_uiWidth >= m_pParent->GetWindowWidth()) {
		x = uint16_t(ParentPos.x);
	}
	else {
		if (newRect.left < ParentRect.left)
			x = uint16_t(ParentRect.left);
		else if (newRect.right > ParentRect.right)
			x = uint16_t((ParentRect.right - WindowRect.right) - m_uiWidth);
	}

	// If our height is equal to or greater than our parents just set our position to the top-most pixel of our parent
	if (m_uiHeight >= m_pParent->GetWindowHeight()) {
		y = uint16_t(ParentPos.y);
	}
	else {
		if (newRect.top < ParentRect.top)
			y = uint16_t(ParentRect.top);
		else if (newRect.bottom > ParentRect.bottom)
			y = uint16_t((ParentRect.bottom - WindowRect.bottom) - m_uiHeight);
	}
}

// IN: void
// OUT: void
//
// Updates the window if any of its information was changed
void Win32Window::UpdateWindowState()
{
	if (!m_hWnd)
		return;

	RECT ClientRect = { 0, 0, m_uiWidth, m_uiHeight };
	RECT WindowRect = { 0, 0, 0, 0 };

	// Adjust our windows rect based on the set flags
	AdjustWindowRect(&WindowRect, m_dStyle, FALSE);

	int windowWidth = (WindowRect.right - WindowRect.left) + ClientRect.right;
	int windowHeight = (WindowRect.bottom - WindowRect.top) + ClientRect.bottom;
	int left = 0;
	int top = 0;

	// If we want to center the window, calculate the top left pixel for the window, otherwise let Windows decide where to put it
	if (m_uiFlags & CENTER_WINDOW)
	{
		// Grabs the desktop dimensions
		RECT desktop;
		::GetWindowRect(GetDesktopWindow(), &desktop);

		left = (desktop.right - windowWidth) >> 1;
		top = (desktop.bottom - windowHeight) >> 1;
	}

	MoveWindow(m_hWnd, left, top, windowWidth, windowHeight, true);
}

// Public Functions
// ------------------------------------

// IN: IWndProc* object - Pointer to a IWndProc class
//     LPCWSTR IconName - String name of the Icon **Optional**
//     LPCWSTR SmallIconName - String name of the Small Icon **Optional**
// OUT: bool - States if whether or not the initialization of the Window was successful
//
// Initializes the Window Object
bool Win32Window::Initialize(const IWndProc* object, const LPCWSTR IconName, const LPCWSTR SmallIconName)
{
	// Create our window class
	WNDCLASSEX wcex;
	memset(&wcex, 0, sizeof(wcex));

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = GlobalWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(&GlobalWndProc) + sizeof(this);
	wcex.hInstance = 0;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = m_wsName;
	wcex.hIcon = (IconName != nullptr) ? LoadIcon(NULL, IconName) : LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = (SmallIconName != nullptr) ? LoadIcon(NULL, SmallIconName) : LoadIcon(NULL, IDI_APPLICATION);

	if (object)
		object->BeforeRegisterWindowClass(wcex);

	if (ASSERT(!RegisterClassEx(&wcex), L"Failed to register the Windows Class!"))
	{
		// If the window class fails to register
		return false;
	}

	RECT ClientRect = { 0, 0, m_uiWidth, m_uiHeight };
	RECT WindowRect = { 0, 0, 0, 0 };

	// Adjust our windows rect based on the set flags
	AdjustWindowRect(&WindowRect, m_dStyle, FALSE);

	int windowWidth = (WindowRect.right - WindowRect.left) + ClientRect.right;
	int windowHeight = (WindowRect.bottom - WindowRect.top) + ClientRect.bottom;
	int left = 0;
	int top = 0;

	// If we want to center the window, calculate the top left pixel for the window, otherwise let Windows decide where to put it
	if (m_uiFlags & CENTER_WINDOW)
	{
		// Grabs the desktop dimensions
		RECT desktop;
		::GetWindowRect(GetDesktopWindow(), &desktop);

		left = (desktop.right - windowWidth) >> 1;
		top = (desktop.bottom - windowHeight) >> 1;
	}

	m_hWnd = CreateWindow(m_wsName, m_wsName, m_dStyle,
		left, top, windowWidth, windowHeight, NULL, NULL, NULL, NULL);

	if (!m_hWnd)
		return false;

	RECT WinRect;
	::GetWindowRect(m_hWnd, &WinRect);
	m_uiXPos = uint16_t(WinRect.left - WindowRect.left);
	m_uiYPos = uint16_t(WinRect.top - WindowRect.top);

	SetWindowLongPtr(m_hWnd, 0, reinterpret_cast<LONG_PTR>(object));
	SetWindowLongPtr(m_hWnd, sizeof(this), reinterpret_cast<LONG_PTR>(this));
	m_hInstance = wcex.hInstance;

	ShowWindow(m_hWnd, SW_SHOW);
	SetFocus(m_hWnd);

	return true;
}
// IN: void
// OUT: void
//
// Destorys the Window
void Win32Window::DestoryWindow()
{
	if (m_hWnd && !ASSERT(!DestroyWindow(m_hWnd), "Failed to dstroy the Window!"))
		m_hWnd = nullptr;

	if (m_hInstance && !ASSERT(!UnregisterClass(m_wsName, m_hInstance), "Failed to unregister the Window Class!"))
		m_hInstance = nullptr;

	if (m_pParent)
		SetParent(nullptr);
}
// IN: void
// OUT: void
//
// Processes any messages that may have been sent to this window and then informs all children and siblings to do so as well
void Win32Window::ProcessMessages()
{
	MSG msg;
	while (PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (m_pSibling)
		m_pSibling->ProcessMessages();

	if (m_pChild)
		m_pChild->ProcessMessages();
}

// Accessors
// ------------------------------------
const HWND Win32Window::GetHWnd() const { return m_hWnd; }
uint16_t Win32Window::GetWindowWidth() const { return m_uiWidth; }
uint16_t Win32Window::GetWindowHeight() const { return m_uiHeight; }
uint16_t Win32Window::GetMaxWidth() const { return GetSystemMetrics(SM_CXSCREEN); }
uint16_t Win32Window::GetMaxHieght() const { return GetSystemMetrics(SM_CYSCREEN); }
POINT	 Win32Window::GetPosition() const
{
	RECT WndRect;
	GetClientRect(m_hWnd, &WndRect);

	return{ WndRect.left, WndRect.top };
}
uint32_t Win32Window::GetXPos() const
{
	RECT WndRect;
	GetClientRect(m_hWnd, &WndRect);

	return WndRect.left;
}
uint32_t Win32Window::GetYPos() const
{
	RECT WndRect;
	GetClientRect(m_hWnd, &WndRect);

	return WndRect.top;
}
WindowMode Win32Window::GetWindowMode() const { return m_eWindowMode; }
DWORD Win32Window::GetWindowStyle() const { return m_dStyle; }
RECT Win32Window::GetWindowRect() const
{
	RECT ClientRect;
	RECT WindowRect = { 0, 0, 0, 0 };
	// Adjust our windows rect based on the set flags
	AdjustWindowRect(&WindowRect, m_dStyle, FALSE);

	::GetWindowRect(m_hWnd, &ClientRect);

	ClientRect.left -= WindowRect.left;
	ClientRect.right -= WindowRect.right;
	ClientRect.top -= WindowRect.top;
	ClientRect.bottom -= WindowRect.bottom;

	return ClientRect;
}

// Mutators
// ------------------------------------
void Win32Window::SetWindowMode(WindowMode mode)
{
	m_eWindowMode = mode;

	if (!m_hWnd)
		return;

	switch (mode)
	{
	case WindowMode::Windowed:
	{
		m_dStyle = WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
		SetWindowLong(m_hWnd, GWL_STYLE, m_dStyle);

		// Grabs the desktop dimensions
		RECT desktop;
		::GetWindowRect(GetDesktopWindow(), &desktop);

		// Set up a default window Rect
		RECT ClientRect = { 0, 0, m_uiWidth, m_uiHeight };
		RECT WindowRect = { 0, 0, 0, 0 };
		// Adjust our windows rect based on the set flags
		AdjustWindowRect(&WindowRect, m_dStyle, FALSE);

		int windowWidth = (WindowRect.right - WindowRect.left) + ClientRect.right;
		int windowHeight = (WindowRect.bottom - WindowRect.top) + ClientRect.bottom;
		int left = m_uiXPos;
		int top = m_uiYPos;

		if (m_uiFlags & CENTER_WINDOW)
		{
			left = (desktop.right - windowWidth) >> 1;
			top = (desktop.bottom - windowHeight) >> 1;
		}

		if (SetWindowPos(m_hWnd, HWND_NOTOPMOST, left, top, windowWidth, windowHeight, SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW) == 0)
			PRINT("Error in setting window to windowed!");
	}
	break;
	case WindowMode::Borderless:
		m_dStyle = WS_POPUP;
		SetWindowLong(m_hWnd, GWL_STYLE, m_dStyle);

		if (SetWindowPos(m_hWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW) == 0)
			PRINT("Error in setting window to borderless!");

		break;
	case WindowMode::Fullscreen:
		m_dStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
		SetWindowLong(m_hWnd, GWL_STYLE, m_dStyle);

		int width = GetSystemMetrics(SM_CXSCREEN);
		int height = GetSystemMetrics(SM_CYSCREEN);

		if (SetWindowPos(m_hWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW) == 0)
			PRINT("Error in setting window to fullscreen!");

		break;
	}
}
void Win32Window::SetWindowStyle(DWORD style)
{
	if (m_hWnd)
		return;

	m_dStyle = style;
}
void Win32Window::SetPosition(int16_t x, int16_t y)
{
	int16_t dX = x - m_uiXPos;
	int16_t dY = y - m_uiYPos;

	m_uiXPos = x;
	m_uiYPos = y;

	RECT WindowRect = { 0, 0, 0, 0 };
	AdjustWindowRect(&WindowRect, m_dStyle, FALSE);

	int WindowWidth = (WindowRect.right - WindowRect.left) + m_uiWidth;
	int windowHeight = (WindowRect.bottom - WindowRect.top) + m_uiHeight;
	int left = x + WindowRect.left;
	int top = y + WindowRect.top;

	MoveWindow(m_hWnd, left, top, WindowWidth, windowHeight, FALSE);

	if (m_pChild)
		m_pChild->ParentMoved(dX, dY);
}
void Win32Window::SetSize(uint16_t width, uint16_t height)
{
	// Grab our currnet Window Rect
	RECT WindowRect;
	::GetWindowRect(m_hWnd, &WindowRect);

	// Check against our current size and if they are the same return
	if (WindowRect.left - WindowRect.right == m_uiWidth && WindowRect.top - WindowRect.bottom == m_uiHeight)
		return;

	m_uiWidth = width;
	m_uiHeight = height;

	UpdateWindowState();
}
void Win32Window::SetParent(Win32Window* parent)
{
	if (m_pParent)
		m_pParent->RemoveChild(this);

	m_pParent = parent;

	if (m_pParent)
	{
		m_pParent->AddChild(this);

		if (m_uiFlags & STAY_WITHIN_PARENT)
		{
			::SetParent(m_hWnd, m_pParent->GetHWnd());
			SetPosition(m_uiXPos, m_uiYPos);
		}
	}
	else
		::SetParent(m_hWnd, nullptr);
}

// Flag Accessors
// ------------------------------------
bool Win32Window::IsFollowingParent() const { return (m_uiFlags & MOVE_WITH_PARENT ? true : false); }
bool Win32Window::CanLeaveParent() const { return (m_uiFlags & STAY_WITHIN_PARENT ? false : true); }
bool Win32Window::IsCenterWindow() const { return (m_uiFlags & CENTER_WINDOW ? true : false); }

// Flag Mutators
// ------------------------------------
void Win32Window::FollowParent(bool follow) { follow ? m_uiFlags |= MOVE_WITH_PARENT : m_uiFlags &= ~(MOVE_WITH_PARENT); }
void Win32Window::StayInParent(bool stayInParent) { stayInParent ? m_uiFlags |= STAY_WITHIN_PARENT : m_uiFlags &= ~(STAY_WITHIN_PARENT); }
void Win32Window::CenterWindow(bool center) { center ? m_uiFlags |= CENTER_WINDOW : m_uiFlags &= ~(CENTER_WINDOW); }