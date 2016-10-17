//----------------------------------------------------------------------------------
#include "WispGlobal.h"
//----------------------------------------------------------------------------------
int CalculatePercents(int max, int current, int maxValue)
{
	if (max > 0)
	{
		max = (current * 100) / max;

		if (max > 100)
			max = 100;

		if (max > 1)
			max = (maxValue * max) / 100;
	}

	return max;
}
//----------------------------------------------------------------------------------
string EncodeUTF8(const wstring &wstr)
{
	int size = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	string result = "";

	if (size > 0)
	{
		result.resize(size + 1);
		WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &result[0], size, NULL, NULL);
		result.resize(size); // result[size] = 0;
	}

	return result;
}
//----------------------------------------------------------------------------------
wstring DecodeUTF8(const string &str)
{
	int size = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	wstring result = L"";

	if (size > 0)
	{
		result.resize(size + 1);
		MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &result[0], size);
		result.resize(size); // result[size] = 0;
	}

	return result;
}
//----------------------------------------------------------------------------------
string ToString(const wstring &wstr)
{
	string str = "";
	int size = wstr.length();
	int newSize = ::WideCharToMultiByte(GetACP(), 0, wstr.c_str(), size, NULL, 0, NULL, NULL);

	if (newSize > 0)
	{
		str.resize(newSize + 1);
		::WideCharToMultiByte(GetACP(), 0, wstr.c_str(), size, &str[0], newSize, NULL, NULL);
		str.resize(newSize); // str[newSize] = 0;
	}

	return str;
}
//----------------------------------------------------------------------------------
wstring ToWString(const string &str)
{
	int size = str.length();
	wstring wstr = L"";

	if (size > 0)
	{
		wstr.resize(size + 1);
		MultiByteToWideChar(GetACP(), 0, str.c_str(), size, &wstr[0], size);
		wstr.resize(size); // wstr[size] = 0;
	}

	return wstr;
}
//----------------------------------------------------------------------------------
string ToLowerA(string str)
{
	if (str.length())
		_strlwr(&str[0]);

	return str.c_str();
}
//----------------------------------------------------------------------------------
string ToUpperA(string str)
{
	if (str.length())
		_strupr(&str[0]);

	return str.c_str();
}
//----------------------------------------------------------------------------------
wstring ToLowerW(wstring str)
{
	if (str.length())
		_wcslwr(&str[0]);

	return str.c_str();
}
//----------------------------------------------------------------------------------
wstring ToUpperW(wstring str)
{
	if (str.length())
		_wcsupr(&str[0]);

	return str.c_str();
}
//----------------------------------------------------------------------------------
bool ToBool(const string &str)
{
	string data = ToLowerA(str);

	const int countOfTrue = 3;
	const string m_TrueValues[countOfTrue] = { "on", "yes", "true" };
	bool result = false;

	IFOR(i, 0, countOfTrue && !result)
		result = (data == m_TrueValues[i]);

	return result;
}
//----------------------------------------------------------------------------------
#if DEBUGGIND_OUTPUT == 1
void DebugMsg(const char *format, ...)
{
	va_list arg;
	va_start(arg, format);

	char buf[512] = { 0 };
	vsprintf_s(buf, format, arg);

	OutputDebugStringA(buf);

	va_end(arg);
}
//----------------------------------------------------------------------------------
void DebugMsg(const wchar_t *format, ...)
{
	va_list arg;
	va_start(arg, format);

	wchar_t buf[512] = { 0 };
	vswprintf_s(buf, format, arg);

	OutputDebugStringW(buf);

	va_end(arg);
}
//----------------------------------------------------------------------------------
void DebugDump(puchar data, const int &size)
{
	int num_lines = size / 16;

	if (size % 16 != 0)
		num_lines++;

	for (int line = 0; line < num_lines; line++)
	{
		int row = 0;

		DebugMsg("%04x: ", line * 16);

		for (row = 0; row < 16; row++)
		{
			if (line * 16 + row < size)
				DebugMsg("%02x ", data[line * 16 + row]);
			else
				DebugMsg("-- ");
		}

		DebugMsg(": ");

		char buf[17] = { 0 };

		for (row = 0; row < 16; row++)
		{
			if (line * 16 + row < size)
				buf[row] = (isprint(data[line * 16 + row]) ? data[line * 16 + row] : '.');
		}

		DebugMsg(buf);
		DebugMsg("\n");
	}
}
#endif
//----------------------------------------------------------------------------------
