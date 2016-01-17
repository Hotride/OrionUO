/****************************************************************************
**
** ColorManager.h
**
** Copyright (C) September 2015 Hotride
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
#ifndef ColorManagerH
#define ColorManagerH
//---------------------------------------------------------------------------
struct INT_HUES
{
	GLint Palette[32];
};
//---------------------------------------------------------------------------
class TColorManager : public IColorManager
{
private:
	PHUES_GROUP m_HuesRange;
	INT_HUES *m_HuesInt;
	int m_HuesCount;

	WORD m_Radarcol[0x10000];

	PALETTE_BLOCK m_Palette[256];

public:
	TColorManager();
	~TColorManager();

	int GetHuesCount() const {return m_HuesCount;}
	void SetHuesBlock(int Index, PVERDATA_HUES_GROUP group);

	void CreateHuesPalette();
	void SendColorsToShader(WORD &color);

	WORD Color32To16(DWORD &C);
	DWORD Color16To32(WORD &C);
	WORD ConvertToGray(WORD &C);

	DWORD GetRadarColor(WORD &C);
	DWORD GetPolygoneColor(WORD C, WORD Color);
	DWORD GetUnicodeFontColor(WORD &C, WORD Color);
	DWORD GetColor(WORD &C, WORD Color);
	DWORD GetPartialHueColor(WORD &C, WORD Color);
};

extern TColorManager *ColorManager;
//---------------------------------------------------------------------------
#endif