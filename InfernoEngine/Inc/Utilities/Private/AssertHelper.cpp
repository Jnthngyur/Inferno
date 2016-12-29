#include "AssertHelper.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <ctime>

namespace Inferno
{
	// IN: const char* message - message to display
	// OUT: void
	//
	// Creates a message box with the sent in string as well as writes the message to the debug log
	void Alert(const char* message)
	{
		Print(message);
		Print("\n");

		char title[128];
		GetWindowTextA(GetActiveWindow(), title, 128);
		MessageBoxA(GetActiveWindow(), message, title, MB_OK | MB_ICONEXCLAMATION);
	}
	// IN: const wchar_t* message - message to display
	// OUT: void
	//
	// Creates a message box with the sent in string as well as writes the message to the debug log
	void Alert(const wchar_t* message)
	{
		Print(message);
		Print("\n");

		wchar_t title[128];
		GetWindowTextW(GetActiveWindow(), title, 128);
		MessageBoxW(GetActiveWindow(), message, title, MB_OK | MB_ICONEXCLAMATION);
	}

	// IN: bool expression - expression to evaluate, true will trigger the assert
	//     const char* message - message to display
	// OUT: bool - State of the expression
	//
	// Creates a message box with the sent in string as well as writes the message to the debug log
	// If in Debug, a break will be triggered
	bool Assert(bool expression, const char* message)
	{
		if (expression == false)
		{
			Alert(message);
#if defined(DEBUG) || defined(_DEBUG)
			DebugBreak();
#endif
		}

		return expression;
	}
	// IN: bool expression - expression to evaluate, true will trigger the assert
	//     const wchar_t* message - message to display
	// OUT: bool - State of the expression
	//
	// Creates a message box with the sent in string as well as writes the message to the debug log
	// If in Debug, a break will be triggered
	bool Assert(bool expression, const wchar_t* message)
	{
		if (expression == true)
		{
			Alert(message);
#if defined(DEBUG) || defined(_DEBUG)
			DebugBreak();
#endif
		}

		return expression;
	}

	// IN: const char* message - message to print
	// OUT: void
	//
	// Prints the message to the debug log and will print to the debug console if in Debug
	void Print(const char* message)
	{
#if defined(DEBUG) || defined(_DEBUG)
		std::cout << message;
#endif
		//debug.Print(message);
	}
	// IN: const wchar_t* message - message to print
	// OUT: void
	//
	// Prints the message to the debug log and will print to the debug console if in Debug
	void Print(const wchar_t* message)
	{
#if defined(DEBUG) || defined(_DEBUG)
		std::cout << message;
#endif
		//debug.Print(Inferno::ToAsciiString(message));
	}
}