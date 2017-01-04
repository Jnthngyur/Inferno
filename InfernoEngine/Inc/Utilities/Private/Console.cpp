#include "Console.h"

//#include <iostream>
//#include <streambuf>
//#include <fstream>
#include <ios>
#include <io.h>
#include <fcntl.h>
#include <codecvt>
#include "FString.h"
#include "AssertHelper.h"

//std::streambuf *CinBuffer, *CoutBuffer, *CerrBuffer;
//std::fstream ConsoleInput, ConsoleOutput, ConsoleError;

Console::Console()
{
	m_hConsoleHandle = nullptr;
	m_hOutHandle = nullptr;
}

Console::~Console()
{
	if (m_hConsoleHandle)
		Destory();
}

/*static*/ Console* Console::GetConsole()
{
	static Console s_Console;
	return &s_Console;
}

bool Console::Initialize(const wchar_t* name)
{ 
	if (m_hConsoleHandle)
		return false;

	ASSERT(name != L"", "Warning: Console Name is trying to be set as an empty string!");

	AllocConsole();
	m_hConsoleHandle = GetConsoleWindow();
	m_hOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTitleW(name);

	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(m_hOutHandle, &info);
	s_usDefaultColors = info.wAttributes & 0xFF;

	/*CinBuffer = std::cin.rdbuf();
	CoutBuffer = std::cout.rdbuf();
	CerrBuffer = std::cerr.rdbuf();
	ConsoleInput.open("CONIN$", std::ios::in);
	ConsoleOutput.open("CONOUT$", std::ios::out);
	ConsoleError.open("CONOUT$", std::ios::out);
	std::cin.rdbuf(ConsoleInput.rdbuf());
	std::cout.rdbuf(ConsoleOutput.rdbuf());
	std::cerr.rdbuf(ConsoleError.rdbuf());*/

	int hConHandle;
	long lStdHandle;
	FILE *fp;

	// Redirect STDOUT to the console
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);

	// Redirect STDIN to the console
	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "r");
	*stdin = *fp;
	setvbuf(stdin, NULL, _IONBF, 0);

	// Redirect STDERR to the console
	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stderr = *fp;
	setvbuf(stderr, NULL, _IONBF, 0);

	// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
	// point to console as well
	std::ios::sync_with_stdio();

	return true;
}
bool Console::Initialize(const FString& name) { return Initialize(name.c_str()); }
bool Console::Initialize(const char* name)
{
	if (m_hConsoleHandle)
		return false;

	ASSERT(name != "", "Warning: Console Name is trying to be set as an empty string!");

	AllocConsole();
	m_hConsoleHandle = GetConsoleWindow();
	m_hOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTitleA(name);

	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(m_hOutHandle, &info);
	s_usDefaultColors = info.wAttributes & 0xFF;

	/*CinBuffer = std::cin.rdbuf();
	CoutBuffer = std::cout.rdbuf();
	CerrBuffer = std::cerr.rdbuf();
	ConsoleInput.open("CONIN$", std::ios::in);
	ConsoleOutput.open("CONOUT$", std::ios::out);
	ConsoleError.open("CONOUT$", std::ios::out);
	std::cin.rdbuf(ConsoleInput.rdbuf());
	std::cout.rdbuf(ConsoleOutput.rdbuf());
	std::cerr.rdbuf(ConsoleError.rdbuf());*/

	int hConHandle;
	long lStdHandle;
	FILE *fp;

	// Redirect STDOUT to the console
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);

	// Redirect STDIN to the console
	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "r");
	*stdin = *fp;
	setvbuf(stdin, NULL, _IONBF, 0);

	// Redirect STDERR to the console
	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stderr = *fp;
	setvbuf(stderr, NULL, _IONBF, 0);

	// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
	// point to console as well
	std::ios::sync_with_stdio();

	return true;
}
bool Console::Destory()
{
	if (!m_hConsoleHandle)
		return false;

	FreeConsole();
	m_hConsoleHandle = nullptr;
	m_hOutHandle = nullptr;

	/*ConsoleInput.close();
	ConsoleOutput.close();
	ConsoleError.close();
	std::cin.rdbuf(CinBuffer);
	std::cout.rdbuf(CoutBuffer);
	std::cerr.rdbuf(CerrBuffer);
	CinBuffer = NULL;
	CoutBuffer = NULL;
	CerrBuffer = NULL;*/

	// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
	// no longer point to the console
	std::ios::sync_with_stdio(false);

	return true;
}

void Console::Clear() { system("CLS"); }
void Console::ResetColors()
{
	SetConsoleTextAttribute(m_hOutHandle, s_usDefaultColors);
}
void Console::CursorVisible(bool visible)
{
	CONSOLE_CURSOR_INFO info;
	GetConsoleCursorInfo(m_hOutHandle, &info);
	info.bVisible = visible ? TRUE : FALSE;
	SetConsoleCursorInfo(m_hOutHandle, &info);
}
void Console::Lock(bool lock)
{
	LockWindowUpdate(lock ? m_hConsoleHandle : nullptr);
}

// Accessors
int16_t Console::GetCursorX() const { return GetScreenBufferInfo().dwCursorPosition.X; }
int16_t Console::GetCursorY() const { return GetScreenBufferInfo().dwCursorPosition.Y; }
Coords Console::GetCursorPosition() const { return GetScreenBufferInfo().dwCursorPosition; }
int16_t Console::GetWindowHeight() const
{
	SMALL_RECT rect = GetScreenBufferInfo().srWindow;
	return (rect.Bottom - rect.Top) + 1;
}
int16_t Console::GetWindowWidth() const
{
	SMALL_RECT rect = GetScreenBufferInfo().srWindow;
	return (rect.Right - rect.Left) + 1;
}
Coords Console::GetWindowDimensions() const
{ 
	SMALL_RECT rect = GetScreenBufferInfo().srWindow;
	return Coords((rect.Right - rect.Left) + 1, (rect.Bottom - rect.Top) + 1);
}
int16_t Console::GetTextColor() const { return GetScreenBufferInfo().wAttributes & 0x0F; }
int16_t Console::GetBackgroundColor() const { return (GetScreenBufferInfo().wAttributes & 0xF0) >> 4; }

// Mutators
void Console::SetCursorPosition(uint16_t x, uint16_t y)
{
	CONSOLE_SCREEN_BUFFER_INFO info = GetScreenBufferInfo();
	SetConsoleCursorPosition(m_hOutHandle, { min(x, info.dwSize.X - 1), min(y, info.dwSize.Y - 1) });
}
void Console::SetWindowDimensions(uint16_t x, uint16_t y)
{
	COORD max = GetLargestConsoleWindowSize(m_hOutHandle);
	SMALL_RECT dim = { 0, 0, ((x < 1 || x > max.X) ? max.X : x) - 1, ((y < 1 || y > max.Y) ? max.Y : y) - 1 };
	SetConsoleWindowInfo(m_hOutHandle, TRUE, &dim);
}
void Console::SetTextColor(ConsoleColor color)
{
	SetConsoleTextAttribute(m_hOutHandle, GetScreenBufferInfo().wAttributes & 0xF0 | color);
}
void Console::SetBackgroundColor(ConsoleColor color)
{
	SetConsoleTextAttribute(m_hOutHandle, GetScreenBufferInfo().wAttributes & 0x0F | color << 4);
}

// Helper Functions
//-------------------------------------------------------
CONSOLE_SCREEN_BUFFER_INFO Console::GetScreenBufferInfo()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(m_hOutHandle, &info);
	return info;
}
CONSOLE_SCREEN_BUFFER_INFO Console::GetScreenBufferInfo() const
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(m_hOutHandle, &info);
	return info;
}
CONSOLE_CURSOR_INFO Console::GetCursorInfo()
{
	CONSOLE_CURSOR_INFO info;
	GetConsoleCursorInfo(m_hOutHandle, &info);
	return info;
}
CONSOLE_CURSOR_INFO Console::GetCursorInfo() const
{
	CONSOLE_CURSOR_INFO info;
	GetConsoleCursorInfo(m_hOutHandle, &info);
	return info;
}