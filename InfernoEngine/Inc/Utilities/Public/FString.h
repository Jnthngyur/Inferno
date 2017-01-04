// FString
// Author: Jonathan Gyurkovics
// Purpose: Handles strings and string based functions

#ifndef _FSTRING_H
#define _FSTRING_H

#include "Globals.h"

#include <string>
#include <vector>

class FString
{
private:
	char* m_pString;		// The pointer to the start of the string
	uint32_t m_uiSize;		// The current size of the string minus the null terminator
	uint32_t m_uiCapacity;	// The current capacity of the string minus the null terminator

	// Helper functions
	uint32_t GetSizeToFit(uint32_t size) const;
	bool FindAndReplace(const char* find, uint32_t findLength, const char* replace, uint32_t replaceLength);

public:
	FString(uint32_t capacity = 0);
	FString(const char character, uint32_t capacity = 0);
	FString(const char* string, uint32_t capacity = 0);	
	FString(const wchar_t* string, uint32_t capacity = 0);
	FString(const std::string& string, uint32_t capacity = 0);
	FString(const std::wstring& string, uint32_t capacity = 0);
	FString(const FString& string, uint32_t capacity = 0);
	~FString();

	FString& operator=(const char character);
	FString& operator=(const char* string);
	FString& operator=(const wchar_t* string);
	FString& operator=(const std::string& string);
	FString& operator=(const std::wstring& string);
	FString& operator=(const FString& string);

	FString& operator+=(const char character);
	FString& operator+=(const char* string);
	FString& operator+=(const wchar_t* string);
	FString& operator+=(const std::string& string);
	FString& operator+=(const std::wstring& string);
	FString& operator+=(const FString& string);

	FString operator+(const char character) const;
	FString operator+(const char* string) const;
	FString operator+(const wchar_t* string) const;
	FString operator+(const std::string& string) const;
	FString operator+(const std::wstring& string) const;
	FString operator+(const FString& string) const;

	bool operator==(const char* string) const;
	bool operator==(const wchar_t* string) const;
	bool operator==(const std::string& string) const;
	bool operator==(const std::wstring& string) const;
	bool operator==(const FString& string) const;

	bool operator!=(const char* string) const;
	bool operator!=(const wchar_t* string) const;
	bool operator!=(const std::string& string) const;
	bool operator!=(const std::wstring& string) const;
	bool operator!=(const FString& string) const;

	bool operator>(const char* string) const;
	bool operator>(const wchar_t* string) const;
	bool operator>(const std::string& string) const;
	bool operator>(const std::wstring& string) const;
	bool operator>(const FString& string) const;

	bool operator>=(const char* string) const;
	bool operator>=(const wchar_t* string) const;
	bool operator>=(const std::string& string) const;
	bool operator>=(const std::wstring& string) const;
	bool operator>=(const FString& string) const;

	bool operator<(const char* string) const;
	bool operator<(const wchar_t* string) const;
	bool operator<(const std::string& string) const;
	bool operator<(const std::wstring& string) const;
	bool operator<(const FString& string) const;

	bool operator<=(const char* string) const;
	bool operator<=(const wchar_t* string) const;
	bool operator<=(const std::string& string) const;
	bool operator<=(const std::wstring& string) const;
	bool operator<=(const FString& string) const;

	char operator[](int32_t index) const;
	// IN: char* - Formated string to represent which part of the string to return. Use "x:y" where x and y are
	//             optional but : is required and they can be negative to wrap around the string. Not including
	//             x states you want to start a the beginning of the string and not including y states you want
	//             to go to the end of the string. EX. ":-1" returns the whole string but the last character.
	// OUT: FString - Substring requested
	//
	// Returns the substring for the indices requested
	FString operator[](const char* indices) const;
	// IN: uint32_t - The start index
	//     uint32_t - The end index
	// OUT: FString - The requested substring
	//
	// Returns the requested substring if the bounds are applicable. Otherwise, returns an empty string
	FString Substring(uint32_t startIndex, uint32_t endIndex) const;

	// IN: uint32_t - The size to reserve the string
	// OUT: void
	//
	// Attempts to reserve the size requested, unless the requested size is smaller then the
	// current capacity, in which case this will do nothing
	void Reserve(uint32_t size);
	// IN: uint32_t - The size to reserve the string
	// OUT: void
	//
	// Resizes the string to the requested size. This will trim the string if the string is
	// shrunk down below the current size
	void Resize(uint32_t size);
	// IN: void
	// OUT: void
	//
	// Clears the string
	void Clear();
	// IN: void
	// OUT: FString - Copy of this string
	//
	// Creates a copy of this string
	FString Copy() const;
	// IN: char - Delimiter
	// OUT: std::vector - List of strings
	//
	// Splits the string based on the delimiter and returns the list of strings.
	std::vector<FString> Split(char delimiter) const;
	// IN: char* - Delimiter
	// OUT: std::vector - List of strings
	//
	// Splits the string based on the delimiter and returns the list of strings.
	std::vector<FString> Split(const char* delimiter) const;
	// IN: void
	// OUT: char* - Pointer to the string
	//
	// Returns the pointer to the start of the string
	const char* c_str() const;
	// IN: void
	// OUT: std::wstring - Wide String of FString
	//
	// Returns a wide string version of this current string
	std::wstring GetWideString() const;

	// Find Functions
	int32_t Find(const char character) const;
	int32_t Find(const char* string) const;
	int32_t Find(const wchar_t* string) const;
	int32_t Find(const std::string& string) const;
	int32_t Find(const std::wstring& string) const;
	int32_t Find(const FString& string) const;

	bool FindAndReplace(const char* find, const char* replace);
	bool FindAndReplace(const char* find, const wchar_t* replace);
	bool FindAndReplace(const char* find, const std::string& replace);
	bool FindAndReplace(const char* find, const std::wstring& replace);
	bool FindAndReplace(const char* find, const FString& replace);
	bool FindAndReplace(const wchar_t* find, const char* replace);
	bool FindAndReplace(const wchar_t* find, const wchar_t* replace);
	bool FindAndReplace(const wchar_t* find, const std::string& replace);
	bool FindAndReplace(const wchar_t* find, const std::wstring& replace);
	bool FindAndReplace(const wchar_t* find, const FString& replace);
	bool FindAndReplace(const std::string& find, const char* replace);
	bool FindAndReplace(const std::string& find, const wchar_t* replace);
	bool FindAndReplace(const std::string& find, const std::string& replace);
	bool FindAndReplace(const std::string& find, const std::wstring& replace);
	bool FindAndReplace(const std::string& find, const FString& replace);
	bool FindAndReplace(const std::wstring& find, const char* replace);
	bool FindAndReplace(const std::wstring& find, const wchar_t* replace);
	bool FindAndReplace(const std::wstring& find, const std::string& replace);
	bool FindAndReplace(const std::wstring& find, const std::wstring& replace);
	bool FindAndReplace(const std::wstring& find, const FString& replace);
	bool FindAndReplace(const FString& find, const char* replace);
	bool FindAndReplace(const FString& find, const wchar_t* replace);
	bool FindAndReplace(const FString& find, const std::string& replace);
	bool FindAndReplace(const FString& find, const std::wstring& replace);
	bool FindAndReplace(const FString& find, const FString& replace);

	// Compare Functions
	uint16_t Compare(const char* string) const;
	uint16_t Compare(const wchar_t* string) const;
	uint16_t Compare(const std::string& string) const;
	uint16_t Compare(const std::wstring& string) const;
	uint16_t Compare(const FString& string) const;

	// Accessors
	uint32_t Length() const;
	uint32_t Capacity() const;

	// Output friend function
	friend std::ostream& operator<<(std::ostream&, const FString&);
	
	// Other friend functions
	friend FString SubstringHelper(const char*, uint32_t, uint32_t);
};

// Additional operators for right hand side operations
FString operator+(const char character, const FString& fstring);
FString operator+(const char* string, const FString& fstring);
FString operator+(const wchar_t* string, const FString& fstring);
FString operator+(const std::string& string, const FString& fstring);
FString operator+(const std::wstring& string, const FString& fstring);

bool operator==(const char* string, const FString& fstring);
bool operator==(const wchar_t* string, const FString& fstring);
bool operator==(const std::string& string, const FString& fstring);
bool operator==(const std::wstring& string, const FString& fstring);

bool operator!=(const char* string, const FString& fstring);
bool operator!=(const wchar_t* string, const FString& fstring);
bool operator!=(const std::string& string, const FString& fstring);
bool operator!=(const std::wstring& string, const FString& fstring);

bool operator>(const char* string, const FString& fstring);
bool operator>(const wchar_t* string, const FString& fstring);
bool operator>(const std::string& string, const FString& fstring);
bool operator>(const std::wstring& string, const FString& fstring);

bool operator>=(const char* string, const FString& fstring);
bool operator>=(const wchar_t* string, const FString& fstring);
bool operator>=(const std::string& string, const FString& fstring);
bool operator>=(const std::wstring& string, const FString& fstring);

bool operator<(const char* string, const FString& fstring);
bool operator<(const wchar_t* string, const FString& fstring);
bool operator<(const std::string& string, const FString& fstring);
bool operator<(const std::wstring& string, const FString& fstring);

bool operator<=(const char* string, const FString& fstring);
bool operator<=(const wchar_t* string, const FString& fstring);
bool operator<=(const std::string& string, const FString& fstring);
bool operator<=(const std::wstring& string, const FString& fstring);

// Helper functions for general use
std::vector<FString> Split(const char* string, char delimiter);
std::vector<FString> Split(const char* string, const char* delimiter);
std::vector<FString> Split(const wchar_t* string, char delimiter);
std::vector<FString> Split(const wchar_t* string, const char* delimiter);
std::vector<FString> Split(const std::string& string, char delimiter);
std::vector<FString> Split(const std::string& string, const char* delimiter);
std::vector<FString> Split(const std::wstring& string, char delimiter);
std::vector<FString> Split(const std::wstring& string, const char* delimiter);
std::vector<FString> Split(const FString& string, char delimiter);
std::vector<FString> Split(const FString& string, const char* delimiter);

#endif