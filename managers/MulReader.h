/****************************************************************************
**
** MulReader.h
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
#ifndef MulReaderH
#define MulReaderH
//---------------------------------------------------------------------------
class TMulReader
{
private:
public:
	TMulReader() {}
	~TMulReader() {}
	
	TTextureObject *ReadGump(DWORD Address, DWORD Size, WORD Width, WORD Height);
	TTextureObject *ReadArt(WORD ID, DWORD Address, DWORD Size);
	TTextureObject *ReadTexture(WORD ID, DWORD Address, DWORD Size);
	TTextureObject *ReadLight(WORD ID, DWORD Address, DWORD Size, WORD Width, WORD Height);
	
	GLuint ReadColoredGump(DWORD Address, DWORD Size, WORD Color, TTextureObject *pth, bool partialHue);
	GLuint ReadColoredArt(WORD ID, DWORD Address, DWORD Size, WORD Color, TTextureObject *pth, bool partialHue);
	GLuint ReadColoredTexture(DWORD Address, DWORD Size, WORD Color, TTextureObject *pth);
	GLuint ReadColoredLight(DWORD Address, DWORD Size, WORD Color, TTextureObject *pth);
	
	bool GumpPixelsInXY(DWORD Address, DWORD Size, WORD Width, WORD Height, int CheckX, int CheckY);
	bool ArtPixelsInXY(WORD ID, DWORD Address, DWORD Size, WORD Width, WORD Height, int CheckX, int CheckY);
};
//---------------------------------------------------------------------------
extern TMulReader MulReader;
//---------------------------------------------------------------------------
#endif