// Inferno.cpp : Defines the entry point for the application.
//

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "resource.h"

#include "InfernoEngine.h"
#include "Win32Window/Win32Window.h"
#include "Globals.h"

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	InfernoEngine engine;

	Win32Window *Window, *Child;
	Window = new Win32Window(1024, 768, L"Parent");
	Window->Initialize(&engine);

	Child = new Win32Window(640, 480, L"Child");
	Child->Initialize(&engine);
	Child->SetParent(Window);
	Child->FollowParent(true);

	engine.AddWindow(Window);
	
	engine.Run();

	return 0;
}