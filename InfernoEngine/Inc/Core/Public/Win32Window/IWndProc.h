// IWndProc.h
// Author: Jonathan Gyurkovics
// Purpose: A simple interface for the WndProc function that can be inhereted by other classes to use with a window

#ifndef _IWNDPROC_H
#define _IWNDPROC_H

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

struct __declspec(novtable) IWndProc
{
	virtual LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
	virtual void BeforeRegisterWindowClass(WNDCLASSEX &_wc) const {}
};

#endif