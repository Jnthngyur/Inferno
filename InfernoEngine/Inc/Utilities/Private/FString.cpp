#include "FString.h"

#include <ostream>
#include <codecvt>

#define DEFAULT_RESERVE_SIZE 5

FString SubstringHelper(const char* string, uint32_t startIndex, uint32_t endIndex);

FString::FString(uint32_t capacity)
{
	m_uiCapacity = capacity;
	m_uiSize = 0;
	m_pString = new char[capacity + 1];
	m_pString[0] = '\0';
}
FString::FString(const char character, uint32_t capacity)
{
	m_uiCapacity = capacity < 1 ? 1 : capacity;
	m_uiSize = 1;
	m_pString = new char[m_uiCapacity + 1];
	m_pString[0] = character;
	m_pString[1] = '\0';
}
FString::FString(const char* string, uint32_t capacity)
{
	m_uiSize = std::strlen(string);
	m_uiCapacity = capacity < m_uiSize ? m_uiSize : capacity;
	m_pString = new char[m_uiCapacity + 1];
	memcpy(m_pString, string, m_uiSize + 1);
}
FString::FString(const wchar_t* string, uint32_t capacity)
{
	m_uiSize = std::wcslen(string);
	m_uiCapacity = capacity < m_uiSize ? m_uiSize : capacity;
	m_pString = new char[m_uiCapacity + 1];
	memcpy(m_pString, std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(string).c_str(), m_uiSize + 1);
}
FString::FString(const std::string& string, uint32_t capacity)
{
	m_uiSize = string.length();
	m_uiCapacity = capacity < m_uiSize ? m_uiSize : capacity;
	m_pString = new char[m_uiCapacity + 1];
	memcpy(m_pString, string.c_str(), m_uiSize + 1);
}
FString::FString(const std::wstring& string, uint32_t capacity)
{
	m_uiSize = string.length();
	m_uiCapacity = capacity < m_uiSize ? m_uiSize : capacity;
	m_pString = new char[m_uiCapacity + 1];
	memcpy(m_pString, std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(string).c_str(), m_uiSize + 1);
}
FString::FString(const FString& string, uint32_t capacity)
{
	m_uiSize = string.Length();
	m_uiCapacity = capacity < m_uiSize ? m_uiSize : capacity;
	m_pString = new char[m_uiCapacity + 1];
	memcpy(m_pString, string.m_pString, m_uiSize + 1);
}
FString::~FString()
{
	if (m_pString)
		delete[] m_pString;
}

FString& FString::operator=(const char character)
{
	if (m_uiCapacity < 1)
		Reserve(1);

	m_uiSize = 1;

	m_pString[0] = character;
	m_pString[1] = '\0';

	return *this;
}
FString& FString::operator=(const char* string)
{
	m_uiSize = strlen(string);

	if (m_uiCapacity < m_uiSize)
		Reserve(GetSizeToFit(m_uiSize));

	memcpy(m_pString, string, m_uiSize + 1);

	return *this;
}
FString& FString::operator=(const wchar_t* string)
{
	m_uiSize = wcslen(string);

	if (m_uiCapacity < m_uiSize)
		Reserve(GetSizeToFit(m_uiSize));

	memcpy(m_pString, std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(string).c_str(), m_uiSize + 1);

	return *this;
}
FString& FString::operator=(const std::string& string)
{
	m_uiSize = string.length();

	if (m_uiCapacity < m_uiSize)
		Reserve(GetSizeToFit(m_uiSize));

	memcpy(m_pString, string.c_str(), m_uiSize + 1);

	return *this;
}
FString& FString::operator=(const std::wstring& string)
{
	m_uiSize = string.length();

	if (m_uiCapacity < m_uiSize)
		Reserve(GetSizeToFit(m_uiSize));

	memcpy(m_pString, std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(string).c_str(), m_uiSize + 1);

	return *this;
}
FString& FString::operator=(const FString& string)
{
	if (&string == this)
		return *this;

	m_uiSize = string.Length();

	if (m_uiCapacity < m_uiSize)
		Reserve(GetSizeToFit(m_uiSize));

	memcpy(m_pString, string.m_pString, m_uiSize + 1);

	return *this;
}
		 
FString& FString::operator+=(const char character)
{
	int32_t addLocation = m_uiSize;
	m_uiSize++;

	if (m_uiCapacity < m_uiSize)
		Reserve(GetSizeToFit(m_uiSize));

	m_pString[addLocation] = character;
	m_pString[addLocation + 1] = '\0';

	return *this;
}
FString& FString::operator+=(const char* string)
{
	int32_t addLocation = m_uiSize, stringlen = strlen(string);
	m_uiSize += stringlen;

	if (m_uiCapacity < m_uiSize)
		Reserve(GetSizeToFit(m_uiSize));

	memcpy(&m_pString[addLocation], string, stringlen + 1);

	return *this;
}
FString& FString::operator+=(const wchar_t* string)
{
	int32_t addLocation = m_uiSize, stringlen = wcslen(string);
	m_uiSize += stringlen;

	if (m_uiCapacity < m_uiSize)
		Reserve(GetSizeToFit(m_uiSize));

	memcpy(&m_pString[addLocation], std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(string).c_str(), stringlen + 1);

	return *this;
}
FString& FString::operator+=(const std::string& string)
{
	int32_t addLocation = m_uiSize;
	m_uiSize += string.length();

	if (m_uiCapacity < m_uiSize)
		Reserve(GetSizeToFit(m_uiSize));

	memcpy(&m_pString[addLocation], string.c_str(), string.length() + 1);

	return *this;
}
FString& FString::operator+=(const std::wstring& string)
{
	int32_t addLocation = m_uiSize;
	m_uiSize += string.length();

	if (m_uiCapacity < m_uiSize)
		Reserve(GetSizeToFit(m_uiSize));

	memcpy(&m_pString[addLocation], std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(string).c_str(), string.length() + 1);

	return *this;
}
FString& FString::operator+=(const FString& string)
{
	int32_t addLocation = m_uiSize;
	m_uiSize += string.Length();

	if (m_uiCapacity < m_uiSize)
		Reserve(GetSizeToFit(m_uiSize));

	memcpy(&m_pString[addLocation], string.m_pString, string.Length() + 1);

	return *this;
}

FString FString::operator+(const char character) const
{
	FString newString(*this, m_uiSize + 1);
	newString += character;

	return newString;
}
FString FString::operator+(const char* string) const
{
	uint32_t length = strlen(string);
	FString newString(*this, m_uiSize + length);
	newString += string;

	return newString;
}
FString FString::operator+(const wchar_t* string) const
{
	uint32_t length = wcslen(string);
	FString newString(*this, m_uiSize + length);
	newString += string;

	return newString;
}
FString FString::operator+(const std::string& string) const
{
	FString newString(*this, m_uiSize + string.length());
	newString += string;

	return newString;
}
FString FString::operator+(const std::wstring& string) const
{
	FString newString(*this, m_uiSize + string.length());
	newString += string;

	return newString;
}
FString FString::operator+(const FString& string) const
{
	FString newString(*this, m_uiSize + string.Length());
	newString += string;

	return newString;
}

bool FString::operator==(const char* string) const { return strcmp(m_pString, string) == 0; }
bool FString::operator==(const wchar_t* string) const { return strcmp(m_pString, std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(string).c_str()) == 0; }
bool FString::operator==(const std::string& string) const { return strcmp(m_pString, string.c_str()) == 0; }
bool FString::operator==(const std::wstring& string) const { return strcmp(m_pString, std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(string).c_str()) == 0; }
bool FString::operator==(const FString& string) const { return strcmp(m_pString, string.m_pString) == 0; }
	 
bool FString::operator!=(const char* string) const { return !operator==(string); }
bool FString::operator!=(const wchar_t* string) const { return !operator==(string); }
bool FString::operator!=(const std::string& string) const { return !operator==(string); }
bool FString::operator!=(const std::wstring& string) const { return !operator==(string); }
bool FString::operator!=(const FString& string) const { return !operator==(string); }
	 
bool FString::operator>(const char* string) const { return strcmp(m_pString, string) > 0; }
bool FString::operator>(const wchar_t* string) const { return strcmp(m_pString, std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(string).c_str()) > 0; }
bool FString::operator>(const std::string& string) const { return strcmp(m_pString, string.c_str()) > 0; }
bool FString::operator>(const std::wstring& string) const { return strcmp(m_pString, std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(string).c_str()) > 0; }
bool FString::operator>(const FString& string) const { return strcmp(m_pString, string.m_pString) > 0; }
	 
bool FString::operator>=(const char* string) const { return strcmp(m_pString, string) >= 0; }
bool FString::operator>=(const wchar_t* string) const { return strcmp(m_pString, std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(string).c_str()) >= 0; }
bool FString::operator>=(const std::string& string) const { return strcmp(m_pString, string.c_str()) >= 0; }
bool FString::operator>=(const std::wstring& string) const { return strcmp(m_pString, std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(string).c_str()) >= 0; }
bool FString::operator>=(const FString& string) const { return strcmp(m_pString, string.m_pString) >= 0; }
	 
bool FString::operator<(const char* string) const { return strcmp(m_pString, string) < 0; }
bool FString::operator<(const wchar_t* string) const { return strcmp(m_pString, std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(string).c_str()) < 0; }
bool FString::operator<(const std::string& string) const { return strcmp(m_pString, string.c_str()) < 0; }
bool FString::operator<(const std::wstring& string) const { return strcmp(m_pString, std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(string).c_str()) < 0; }
bool FString::operator<(const FString& string) const { return strcmp(m_pString, string.m_pString) < 0; }
	 
bool FString::operator<=(const char* string) const { return strcmp(m_pString, string) <= 0; }
bool FString::operator<=(const wchar_t* string) const { return strcmp(m_pString, std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(string).c_str()) <= 0; }
bool FString::operator<=(const std::string& string) const { return strcmp(m_pString, string.c_str()) <= 0; }
bool FString::operator<=(const std::wstring& string) const { return strcmp(m_pString, std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(string).c_str()) <= 0; }
bool FString::operator<=(const FString& string) const { return strcmp(m_pString, string.m_pString) <= 0; }

char FString::operator[](int32_t index) const
{
	int32_t size = (int32_t)m_uiSize;	// To eliminate the sign mismatch warning
	while (index < 0) index += size;
	while (index >= size) index -= size;

	return m_pString[index];
}
// IN: char* - Formated string to represent which part of the string to return. Use "x:y" where x and y are
//             optional but : is required and they can be negative to wrap around the string. Not including
//             x states you want to start a the beginning of the string and not including y states you want
//             to go to the end of the string. EX. ":-1" returns the whole string but the last character.
// OUT: FString - Substring requested
//
// Returns the substring for the indices requested
FString FString::operator[](const char* indices) const
{
	if (indices == nullptr || indices[0] == '\0')
		return FString();

	auto splitIndices = ::Split(indices, ':');
	
	if (splitIndices.size() == 1)
		return operator[](atoi(splitIndices[0].c_str()));

	int32_t size = (int32_t)m_uiSize;	// To eliminate the sign mismatch warning
	int32_t start = (splitIndices[0].m_uiSize) ? atoi(splitIndices[0].c_str()) : 0;
	int32_t end	  = (splitIndices[1].m_uiSize) ? atoi(splitIndices[1].c_str()) : m_uiSize - 1;

	while (start < 0) start += size;
	while (start >= size) start -= size;
	while (end < 0) end += size;
	while (end >= size) end -= size;

	return Substring(start, end);
}
// IN: uint32_t - The start index
//     uint32_t - The end index
// OUT: FString - The requested substring
//
// Returns the requested substring if the bounds are applicable. Otherwise, returns an empty string
FString FString::Substring(uint32_t startIndex, uint32_t endIndex) const
{
	if (startIndex > m_uiSize)
		return FString();

	if (endIndex >= m_uiSize)
		endIndex = m_uiSize - 1;

	if (startIndex > endIndex)
	{
		uint32_t subSize = (startIndex - endIndex) + 1;
		FString newString(subSize);	// + 1 needed to ensure string size is correct
		newString.m_uiSize = subSize;
		for (uint32_t i = startIndex, index = 0; index < subSize; i--)
			newString.m_pString[index++] = m_pString[i];

		newString.m_pString[subSize] = '\0';

		return newString;
	}
	
	uint32_t subSize = (endIndex - startIndex) + 1;
	FString newString(subSize);	// + 1 needed to ensure string size is correct
	newString.m_uiSize = subSize;
	for (uint32_t i = startIndex, index = 0; i <= endIndex; i++)
		newString.m_pString[index++] = m_pString[i];

	newString.m_pString[subSize] = '\0';

	return newString;
}

// IN: uint32_t - The size to reserve the string
// OUT: void
//
// Attempts to reserve the size requested, unless the requested size is smaller then the
// current capacity, in which case this will do nothing
void FString::Reserve(uint32_t size)
{
	if (m_uiCapacity < size)
	{
		m_uiCapacity = size;
		char* temp = new char[m_uiCapacity + 1];
		
		uint32_t i = 0;
		for (; m_pString[i]; i++)
			temp[i] = m_pString[i];
		
		temp[i] = '\0';
		delete[] m_pString;
		m_pString = temp;
	}
}
// IN: uint32_t - The size to reserve the string
// OUT: void
//
// Resizes the string to the requested size. This will trim the string if the string is
// shrunk down below the current size
void FString::Resize(uint32_t size)
{
	if (m_uiCapacity == size)
		return;

	m_uiCapacity = size;

	if (m_uiSize > m_uiCapacity)
		m_uiSize = m_uiCapacity;

	char* temp = new char[m_uiCapacity + 1];
	for (uint32_t i = 0; i < m_uiSize; i++)
		temp[i] = m_pString[i];

	temp[m_uiSize] = '\0';
	delete[] m_pString;
	m_pString = temp;
}
// IN: void
// OUT: void
//
// Clears the string
void FString::Clear()
{
	m_pString[0] = '\0';
	m_uiSize = 0;
}
// IN: void
// OUT: FString - Copy of this string
//
// Creates a copy of this string
FString FString::Copy() const
{
	return FString(*this);
}
// IN: char - Delimiter
// OUT: std::vector - List of strings
//
// Splits the string based on the delimiter and returns the list of strings.
std::vector<FString> FString::Split(char delimiter) const
{
	std::vector<FString> stringList;
	stringList.reserve(DEFAULT_RESERVE_SIZE);

	uint32_t index = 0, stringStart = 0;
	for (; index <= m_uiSize; index++)
	{
		if (m_pString[index] == delimiter)
		{
			stringList.push_back(m_pString[stringStart] == delimiter ? FString() : Substring(stringStart, index - 1));
			stringStart = index + 1;
		}
	}

	stringList.push_back(stringStart == m_uiSize ? FString() : Substring(stringStart, index - 1));

	return stringList;
}
// IN: char* - Delimiter
// OUT: std::vector - List of strings
//
// Splits the string based on the delimiter and returns the list of strings.
std::vector<FString> FString::Split(const char* delimiter) const
{
	std::vector<FString> stringList;
	stringList.reserve(DEFAULT_RESERVE_SIZE);
	uint32_t delimLength = strlen(delimiter);

	uint32_t index = 0, stringStart = 0;
	for (; index <= m_uiSize; index++)
	{
		if (memcmp(&m_pString[index], delimiter, delimLength) == 0)
		{
			stringList.push_back(memcmp(&m_pString[stringStart], delimiter, delimLength) == 0 ? FString() : Substring(stringStart, index - 1));
			index += delimLength - 1;
			stringStart = index + 1;
		}
	}

	stringList.push_back(stringStart == m_uiSize ? FString() : Substring(stringStart, index - 1));

	return stringList;
}
// IN: void
// OUT: char* - Pointer to the string
//
// Returns the pointer to the start of the string
const char* FString::c_str() const
{
	return m_pString;
}
// IN: void
// OUT: std::wstring - Wide String of FString
//
// Returns a wide string version of this current string
std::wstring FString::GetWideString() const
{
	return std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().from_bytes(m_pString);
}

// Find Functions
int32_t FString::Find(const char character) const
{
	for (uint32_t i = 0; i <= m_uiSize; i++)
		if (m_pString[i] == character)
			return i;

	return -1;
}
int32_t FString::Find(const char* string) const
{
	uint32_t length = strlen(string);
	uint32_t searchLength = (m_uiSize + 1) - length;

	for (uint32_t i = 0; i < searchLength; i++)
		if (memcmp(&m_pString[i], string, length) == 0)
			return i;

	return -1;
}
int32_t FString::Find(const wchar_t* string) const
{
	std::string str = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(string);
	uint32_t searchLength = (m_uiSize + 1) - str.length();

	for (uint32_t i = 0; i < searchLength; i++)
		if (memcmp(&m_pString[i], str.c_str(), str.length()) == 0)
			return i;

	return -1;
}
int32_t FString::Find(const std::string& string) const
{
	uint32_t searchLength = (m_uiSize + 1) - string.length();

	for (uint32_t i = 0; i < searchLength; i++)
		if (memcmp(&m_pString[i], string.c_str(), string.length()) == 0)
			return i;

	return -1;
}
int32_t FString::Find(const std::wstring& string) const
{
	std::string str = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(string);
	uint32_t searchLength = (m_uiSize + 1) - str.length();

	for (uint32_t i = 0; i < searchLength; i++)
		if (memcmp(&m_pString[i], str.c_str(), str.length()) == 0)
			return i;

	return -1;
}
int32_t FString::Find(const FString& string) const
{
	uint32_t searchLength = (m_uiSize + 1) - string.Length();

	for (uint32_t i = 0; i < searchLength; i++)
		if (memcmp(&m_pString[i], m_pString, string.Length()) == 0)
			return i;

	return -1;
}
	 
bool FString::FindAndReplace(const char* find, const char* replace)
{
	return FindAndReplace(find, strlen(find), replace, strlen(replace));
}
bool FString::FindAndReplace(const char* find, const wchar_t* replace)
{
	auto str = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(replace);
	return FindAndReplace(find, strlen(find), str.c_str(), str.length());
}
bool FString::FindAndReplace(const char* find, const std::string& replace)
{
	return FindAndReplace(find, strlen(find), replace.c_str(), replace.length());
}
bool FString::FindAndReplace(const char* find, const std::wstring& replace)
{
	auto str = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(replace);
	return FindAndReplace(find, strlen(find), str.c_str(), str.length());
}
bool FString::FindAndReplace(const char* find, const FString& replace)
{
	return FindAndReplace(find, strlen(find), replace.c_str(), replace.Length());
}
bool FString::FindAndReplace(const wchar_t* find, const char* replace)
{
	auto str = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(find);
	return FindAndReplace(str.c_str(), str.length(), replace, strlen(replace));
}
bool FString::FindAndReplace(const wchar_t* find, const wchar_t* replace)
{
	auto str = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(find);
	auto rstr = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(replace);
	return FindAndReplace(str.c_str(), str.length(), rstr.c_str(), rstr.length());
}
bool FString::FindAndReplace(const wchar_t* find, const std::string& replace)
{
	auto str = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(find);
	return FindAndReplace(str.c_str(), str.length(), replace.c_str(), replace.length());
}
bool FString::FindAndReplace(const wchar_t* find, const std::wstring& replace)
{
	auto str = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(find);
	auto rstr = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(replace);
	return FindAndReplace(str.c_str(), str.length(), rstr.c_str(), rstr.length());
}
bool FString::FindAndReplace(const wchar_t* find, const FString& replace)
{
	auto str = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(find);
	return FindAndReplace(str.c_str(), str.length(), replace.c_str(), replace.Length());
}
bool FString::FindAndReplace(const std::string& find, const char* replace)
{
	return FindAndReplace(find.c_str(), find.length(), replace, strlen(replace));
}
bool FString::FindAndReplace(const std::string& find, const wchar_t* replace)
{
	auto str = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(replace);
	return FindAndReplace(find.c_str(), find.length(), str.c_str(), str.length());
}
bool FString::FindAndReplace(const std::string& find, const std::string& replace)
{
	return FindAndReplace(find.c_str(), find.length(), replace.c_str(), replace.length());
}
bool FString::FindAndReplace(const std::string& find, const std::wstring& replace)
{
	auto str = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(replace);
	return FindAndReplace(find.c_str(), find.length(), str.c_str(), str.length());
}
bool FString::FindAndReplace(const std::string& find, const FString& replace)
{
	return FindAndReplace(find.c_str(), find.length(), replace.c_str(), replace.Length());
}
bool FString::FindAndReplace(const std::wstring& find, const char* replace)
{
	auto str = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(find);
	return FindAndReplace(str.c_str(), str.length(), replace, strlen(replace));
}
bool FString::FindAndReplace(const std::wstring& find, const wchar_t* replace)
{
	auto str = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(find);
	auto rstr = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(replace);
	return FindAndReplace(str.c_str(), str.length(), rstr.c_str(), rstr.length());
}
bool FString::FindAndReplace(const std::wstring& find, const std::string& replace)
{
	auto str = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(find);
	return FindAndReplace(str.c_str(), str.length(), replace.c_str(), replace.length());
}
bool FString::FindAndReplace(const std::wstring& find, const std::wstring& replace)
{
	auto str = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(find);
	auto rstr = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(replace);
	return FindAndReplace(str.c_str(), str.length(), rstr.c_str(), rstr.length());
}
bool FString::FindAndReplace(const std::wstring& find, const FString& replace)
{
	auto str = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(find);
	return FindAndReplace(str.c_str(), str.length(), replace.c_str(), replace.Length());
}
bool FString::FindAndReplace(const FString& find, const char* replace)
{
	return FindAndReplace(find.c_str(), find.Length(), replace, strlen(replace));
}
bool FString::FindAndReplace(const FString& find, const wchar_t* replace)
{
	auto str = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(replace);
	return FindAndReplace(find.c_str(), find.Length(), str.c_str(), str.length());
}
bool FString::FindAndReplace(const FString& find, const std::string& replace)
{
	return FindAndReplace(find.c_str(), find.Length(), replace.c_str(), replace.length());
}
bool FString::FindAndReplace(const FString& find, const std::wstring& replace)
{
	auto str = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(replace);
	return FindAndReplace(find.c_str(), find.Length(), str.c_str(), str.length());
}
bool FString::FindAndReplace(const FString& find, const FString& replace)
{
	return FindAndReplace(find.c_str(), find.Length(), replace.c_str(), replace.Length());
}

// Compare Functions
uint16_t FString::Compare(const char* string) const { return strcmp(m_pString, string); }
uint16_t FString::Compare(const wchar_t* string) const { return strcmp(m_pString, std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(string).c_str()); }
uint16_t FString::Compare(const std::string& string) const { return strcmp(m_pString, string.c_str()); }
uint16_t FString::Compare(const std::wstring& string) const { return strcmp(m_pString, std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(string).c_str()); }
uint16_t FString::Compare(const FString& string) const { return strcmp(m_pString, string.m_pString); }

// Accessors
uint32_t FString::Length() const { return m_uiSize; }
uint32_t FString::Capacity() const { return m_uiCapacity; }

// Output friend function
std::ostream& operator<<(std::ostream& out, const FString& string)
{
	out << string.m_pString;
	return out;
}

// Helper Functions
// ------------------------------------------------------------------------------
uint32_t FString::GetSizeToFit(uint32_t size) const
{
	uint32_t newCap = m_uiCapacity;

	if (newCap == 0)
		newCap = 1;

	while (newCap < size) newCap *= 2;
	return newCap;
}
bool FString::FindAndReplace(const char* find, uint32_t findLength, const char* replace, uint32_t replaceLength)
{
	uint32_t startIndex = (uint32_t)Find(find);

	if (startIndex == UINT32_MAX)
		return false;

	m_uiSize = (m_uiSize - findLength) + replaceLength;

	Reserve(m_uiSize);

	memcpy(&m_pString[startIndex + replaceLength], &m_pString[startIndex + findLength], m_uiSize - (replaceLength + startIndex) + 1);
	memcpy(&m_pString[startIndex], replace, replaceLength);

	return true;
}

// Additional operators for right hand side operations
// ------------------------------------------------------------------------------
FString operator+(const char character, const FString& fstring) { return FString(character, fstring.Length() + 1) + fstring; }
FString operator+(const char* string, const FString& fstring) { return FString(string, fstring.Length() + strlen(string)) + fstring; }
FString operator+(const wchar_t* string, const FString& fstring) { return FString(string, fstring.Length() + wcslen(string)) + fstring; }
FString operator+(const std::string& string, const FString& fstring) { return FString(string, fstring.Length() + string.length()) + fstring; }
FString operator+(const std::wstring& string, const FString& fstring) { return FString(string, fstring.Length() + string.length()) + fstring; }

bool operator==(const char* string, const FString& fstring) { return fstring == string; }
bool operator==(const wchar_t* string, const FString& fstring) { return fstring == string; }
bool operator==(const std::string& string, const FString& fstring) { return fstring == string; }
bool operator==(const std::wstring& string, const FString& fstring) { return fstring == string; }

bool operator!=(const char* string, const FString& fstring) { return fstring != string; }
bool operator!=(const wchar_t* string, const FString& fstring) { return fstring != string; }
bool operator!=(const std::string& string, const FString& fstring) { return fstring != string; }
bool operator!=(const std::wstring& string, const FString& fstring) { return fstring != string; }

bool operator>(const char* string, const FString& fstring) { return fstring < string; }
bool operator>(const wchar_t* string, const FString& fstring) { return fstring < string; }
bool operator>(const std::string& string, const FString& fstring) { return fstring < string; }
bool operator>(const std::wstring& string, const FString& fstring) { return fstring < string; }

bool operator>=(const char* string, const FString& fstring) { return fstring <= string; }
bool operator>=(const wchar_t* string, const FString& fstring) { return fstring <= string; }
bool operator>=(const std::string& string, const FString& fstring) { return fstring <= string; }
bool operator>=(const std::wstring& string, const FString& fstring) { return fstring <= string; }

bool operator<(const char* string, const FString& fstring) { return fstring > string; }
bool operator<(const wchar_t* string, const FString& fstring) { return fstring > string; }
bool operator<(const std::string& string, const FString& fstring) { return fstring > string; }
bool operator<(const std::wstring& string, const FString& fstring) { return fstring > string; }

bool operator<=(const char* string, const FString& fstring) { return fstring >= string; }
bool operator<=(const wchar_t* string, const FString& fstring) { return fstring >= string; }
bool operator<=(const std::string& string, const FString& fstring) { return fstring >= string; }
bool operator<=(const std::wstring& string, const FString& fstring) { return fstring >= string; }

FString SubstringHelper(const char* string, uint32_t startIndex, uint32_t endIndex)
{
	if (startIndex > endIndex)
		return FString();

	uint32_t subSize = (endIndex - startIndex) + 1;
	FString newString(subSize);
	newString.m_uiSize = subSize;

	for (uint32_t index = startIndex, i = 0; i < subSize;)
		newString.m_pString[i++] = string[index++];

	return newString;
}

std::vector<FString> SplitHelper(const char* string, uint32_t strLen, char delimiter)
{
	std::vector<FString> stringList;
	stringList.reserve(DEFAULT_RESERVE_SIZE);

	uint32_t index = 0, stringStart = 0;
	for (; index <= strLen; index++)
	{
		if (string[index] == delimiter)
		{
			stringList.push_back(string[stringStart] == delimiter ? FString() : SubstringHelper(string, stringStart, index - 1));
			stringStart = ++index;
		}
	}

	stringList.push_back(stringStart == strLen ? FString() : SubstringHelper(string, stringStart, index - 1));

	return stringList;
}
std::vector<FString> SplitHelper(const char* string, uint32_t strLen, const char* delimiter, uint32_t delLen)
{
	std::vector<FString> stringList;
	stringList.reserve(DEFAULT_RESERVE_SIZE);

	uint32_t index = 0, stringStart = 0;
	for (; index <= strLen; index++)
	{
		if (memcmp(&string[index], delimiter, delLen) == 0)
		{
			stringList.push_back(memcmp(&string[stringStart], delimiter, delLen) == 0 ? FString() : SubstringHelper(string, stringStart, index - 1));
			index += delLen;
			stringStart = index;
		}
	}

	stringList.push_back(stringStart == strLen ? FString() : SubstringHelper(string, stringStart, index - 1));

	return stringList;
}

// Helper functions for general use
std::vector<FString> Split(const char* string, char delimiter)
{
	return SplitHelper(string, strlen(string), delimiter);
}
std::vector<FString> Split(const char* string, const char* delimiter)
{
	return SplitHelper(string, strlen(string), delimiter, strlen(delimiter));
}
std::vector<FString> Split(const wchar_t* string, char delimiter)
{
	auto str = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(string);
	return SplitHelper(str.c_str(), str.length(), delimiter);
}
std::vector<FString> Split(const wchar_t* string, const char* delimiter)
{
	auto str = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(string);
	return SplitHelper(str.c_str(), str.length(), delimiter, strlen(delimiter));
}
std::vector<FString> Split(const std::string& string, char delimiter)
{
	return SplitHelper(string.c_str(), string.length(), delimiter);
}
std::vector<FString> Split(const std::string& string, const char* delimiter)
{
	return SplitHelper(string.c_str(), string.length(), delimiter, strlen(delimiter));
}
std::vector<FString> Split(const std::wstring& string, char delimiter)
{
	auto str = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(string);
	return SplitHelper(str.c_str(), str.length(), delimiter);
}
std::vector<FString> Split(const std::wstring& string, const char* delimiter)
{
	auto str = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(string);
	return SplitHelper(str.c_str(), str.length(), delimiter, strlen(delimiter));
}
std::vector<FString> Split(const FString& string, char delimiter)
{
	return SplitHelper(string.c_str(), string.Length(), delimiter);
}
std::vector<FString> Split(const FString& string, const char* delimiter)
{
	return SplitHelper(string.c_str(), string.Length(), delimiter, strlen(delimiter));
}