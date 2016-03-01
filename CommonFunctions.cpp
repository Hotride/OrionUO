/****************************************************************************
**
** CommonFunctions.cpp
**
** Copyright (C) February 2016 Hotride
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
*****************************************************************************
*/
//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
const char *GetReagentName(WORD ID)
{
	switch (ID)
	{
	case 0x0F7A: return "Black pearl";
	case 0x0F7B: return "Bloodmoss";
	case 0x0F84: return "Garlic";
	case 0x0F85: return "Ginseng";
	case 0x0F86: return "Mandrake root";
	case 0x0F88: return "Nightshade";
	case 0x0F8C: return "Sulfurous ash";
	case 0x0F8D: return "Spiders silk";
	default: break;
	}

	return "";
}
//--------------------------------------------------------------------------
int CalculateSphereOffset(int max, int current, int maxValue, float divizor)
{
	if (max > 0)
	{
		max = (int)((current / divizor) * (float)max);

		max = (maxValue * max) / 100;

		if (max < 0)
			max = 0;
	}

	return max;
}
//--------------------------------------------------------------------------
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
//---------------------------------------------------------------------------
void TileOffsetOnMonitorToXY(int &ofsX, int ofsY, int &x, int &y)
{
	if (!ofsX)
		x = y = ofsY / 2;
	else if (!ofsY)
	{
		x = ofsX / 2;
		y = -x;
	}
	else //if (ofsX && ofsY)
	{
		int absX = abs(ofsX);
		int absY = abs(ofsY);
		x = ofsX;

		if (ofsY > ofsX)
		{
			if (ofsX < 0 && ofsY < 0)
				y = absX - absY;
			else if (ofsX > 0 && ofsY > 0)
				y = absY - absX;
		}
		else if (ofsX > ofsY)
		{
			if (ofsX < 0 && ofsY < 0)
				y = -(absY - absX);
			else if (ofsX > 0 && ofsY > 0)
				y = -(absX - absY);
		}

		if (!y && ofsY != ofsX)
		{
			if (ofsY < 0)
				y = -(absX + absY);
			else
				y = absX + absY;
		}

		y /= 2;
		x += y;
	}
}
//---------------------------------------------------------------------------
void UnuseShader()
{
	glUseProgramObjectARB(0);
	ShaderTexture = 0;
	ShaderColorTable = 0;
	ShaderDrawMode = 0;
	CurrentShader = NULL;
}
//---------------------------------------------------------------------------
string FilePath(string fName)
{
	return g_DirectoryPath + "\\" + fName;
}
//---------------------------------------------------------------------------
string ToString(wstring wstr)
{
	int size = wstr.length();
	string str = "";

	if (size > 0)
	{
		char *text = new char[size + 1];
		int len = uucode2str(wstr.c_str(), size, &text[0], size);
		text[size] = 0;
		str = text;
		delete text;
	}

	return str;
}
//---------------------------------------------------------------------------
wstring ToWString(string str)
{
	int size = str.length();
	wstring wstr = L"";

	if (size)
	{
		wchar_t *wbuf = new wchar_t[size + 1];
		MultiByteToWideChar(GetACP(), 0, str.c_str(), size, &wbuf[0], size);
		wbuf[size] = 0;
		wstr = wbuf;
		delete wbuf;
	}

	return wstr;
}
//---------------------------------------------------------------------------
int gumpuucode2str(const wchar_t* wstr, int wlength, LPSTR receiver, int maxsize)
{
	if (!wlength || wlength < -1 || maxsize < 1)
		return 0;

	int msize = ((wlength < 0) ? (maxsize * 2 + 2) : (wlength * 2 + 2));
	PBYTE buf = new BYTE[msize];
	memcpy(buf, wstr, msize - 2);
	int len = 0;
	PBYTE p = buf + 1;

	while (len < maxsize)
	{
		if (wlength < 0)
		if (!*(p - 1) && !*p)
		{
			len++;
			break;
		}
		else;

		if (len >= wlength)
			break;

		BYTE c = *(p - 1);
		*(p - 1) = *p;
		*p = c;
		len++;
		p += 2;
	}

	len = WideCharToMultiByte(GetACP(), 0, (LPCWSTR)buf, len, receiver, maxsize, 0, 0);
	delete buf;

	if (len < maxsize && len && receiver[len - 1])
		receiver[len++] = 0;

	return len;
}
//---------------------------------------------------------------------------
int uucode2str(const wchar_t* wstr, int wlength, LPSTR receiver, int maxsize)
{
	if (!wlength || wlength < -1 || maxsize < 1)
		return 0;

	int msize = ((wlength < 0) ? (maxsize * 2 + 2) : (wlength * 2 + 2));
	int len = lstrlenW(wstr);
	len = WideCharToMultiByte(GetACP(), 0, wstr, len, receiver, maxsize, 0, 0);

	if (len < maxsize && len && receiver[len - 1])
		receiver[len++] = 0;

	return len;
}
//--------------------------------------------------------------------------
int str2uucode(const char* str, int length, wchar_t* wreceiver, int wmaxsize)
{
	if (!length || length < -1 || wmaxsize < 1 || !wreceiver)
		return 0;

	int len = 0, i;
	wreceiver[0] = 0;
	len = MultiByteToWideChar(GetACP(), 0, str, length, wreceiver, wmaxsize);
	PBYTE p = (PBYTE)wreceiver + 1;

	for (i = 0; i < len && (*(p - 1) || *p) && i < wmaxsize; i++, p += 2)
	{
		BYTE c = *(p - 1);
		*(p - 1) = *p;
		*p = c;
	}

	if (i < wmaxsize && i && wreceiver[i - 1])
		wreceiver[i] = 0;

	return i;
}
//---------------------------------------------------------------------------