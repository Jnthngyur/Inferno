// AssertHelper.h
// Author: Jonathan Gyurkovics
// Purpose: This allows the use of several debugging tools such as Asserts, Alerts, and Logging

#ifndef _ASSERTHELPER_H
#define _ASSERTHELPER_H

#define ALERT(message)				Inferno::Alert(message)
#define ASSERT(expression, message) Inferno::Assert(expression, message)
#define PRINT(message)				Inferno::Print(message)

namespace Inferno
{
	// IN: const char* message - message to display
	// OUT: void
	//
	// Creates a message box with the sent in string as well as writes the message to the debug log
	void Alert(const char* message);
	// IN: const wchar_t* message - message to display
	// OUT: void
	//
	// Creates a message box with the sent in string as well as writes the message to the debug log
	void Alert(const wchar_t* message);

	// IN: bool expression - expression to evaluate, true will trigger the assert
	//     const char* message - message to display
	// OUT: bool - State of the expression
	//
	// Creates a message box with the sent in string as well as writes the message to the debug log
	// If in Debug, a break will be triggered
	bool Assert(bool expression, const char* message);
	// IN: bool expression - expression to evaluate, true will trigger the assert
	//     const wchar_t* message - message to display
	// OUT: bool - State of the expression
	//
	// Creates a message box with the sent in string as well as writes the message to the debug log
	// If in Debug, a break will be triggered
	bool Assert(bool expression, const wchar_t* message);

	// IN: const char* message - message to print
	// OUT: void
	//
	// Prints the message to the debug log and will print to the debug console if in Debug
	void Print(const char* message);
	// IN: const wchar_t* message - message to print
	// OUT: void
	//
	// Prints the message to the debug log and will print to the debug console if in Debug
	void Print(const wchar_t* message);
}

#endif