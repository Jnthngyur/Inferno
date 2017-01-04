// Console.h
// Author: Jonathan Gyurkovics
// Purpose: A warpper around the default console

#ifndef _CONSOLE_H
#define _CONSOLE_H

#include "Globals.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// Forward Declarations
class FString;

struct Coords
{
	int16_t x;
	int16_t y;

	Coords() : x(0), y(0) {}
	Coords(int16_t x, int16_t y) : x(x), y(y) {}
	Coords(const Coords& coords) : x(coords.x), y(coords.y) {}
	Coords(const COORD& coords) : x(coords.X), y(coords.Y) {}
};

// Colors - can be used as Foreground or Background
enum ConsoleColor
{
	Black = 0,
	DarkBlue = FOREGROUND_BLUE,
	DarkGreen = FOREGROUND_GREEN,
	DarkCyan = FOREGROUND_GREEN | FOREGROUND_BLUE,
	DarkRed = FOREGROUND_RED,
	DarkMagenta = FOREGROUND_RED | FOREGROUND_BLUE,
	DarkYellow = FOREGROUND_RED | FOREGROUND_GREEN,
	Gray = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	DarkGray = FOREGROUND_INTENSITY,
	Blue = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
	Green = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
	Cyan = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
	Red = FOREGROUND_RED | FOREGROUND_INTENSITY,
	Magenta = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
	Yellow = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
	White = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
};

static uint16_t s_usDefaultColors;

class Console
{
	HWND m_hConsoleHandle;
	HANDLE m_hOutHandle;

	Console();
	~Console();

	CONSOLE_SCREEN_BUFFER_INFO GetScreenBufferInfo();
	CONSOLE_SCREEN_BUFFER_INFO GetScreenBufferInfo() const;
	CONSOLE_CURSOR_INFO GetCursorInfo();
	CONSOLE_CURSOR_INFO GetCursorInfo() const;

public:
	static Console* GetConsole();

	bool Initialize(const char* name);
	bool Initialize(const wchar_t* name);
	bool Initialize(const FString& name);
	bool Destory();

	void Clear();
	void ResetColors();
	void CursorVisible(bool visible);
	void Lock(bool lock);

	// Accessors
	int16_t GetCursorX() const;
	int16_t GetCursorY() const;
	Coords GetCursorPosition() const;
	int16_t GetWindowHeight() const;
	int16_t GetWindowWidth() const;
	Coords GetWindowDimensions() const;
	int16_t GetTextColor() const;
	int16_t GetBackgroundColor() const;

	// Mutators
	void SetCursorPosition(uint16_t x, uint16_t y);
	void SetWindowDimensions(uint16_t x, uint16_t y);
	void SetTextColor(ConsoleColor color);
	void SetBackgroundColor(ConsoleColor color);
};

#endif