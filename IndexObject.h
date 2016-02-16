/****************************************************************************
**
** IndexObject.h
**
** Copyright (C) January 2016 Hotride
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
#ifndef IndexObjectH
#define IndexObjectH
//---------------------------------------------------------------------------
class TIndexObject
{
private:
	DWORD m_Address;
	DWORD m_Size;
	DWORD m_LastAccessTime;
	short m_Width;
	short m_Height;
public:
	TIndexObject();
	virtual ~TIndexObject();

	SETGET(DWORD, Address);
	SETGET(DWORD, Size);
	SETGET(DWORD, LastAccessTime);
	SETGET(short, Width);
	SETGET(short, Height);

	TTextureObject *Texture;
};
//---------------------------------------------------------------------------
class TIndexObjectStatic : public TIndexObject
{
private:
	WORD m_Index;
	char m_Increment;
	char m_AnimIndex;
	DWORD m_NextChange;
	WORD m_LightColor;
public:
	TIndexObjectStatic();

	SETGET(WORD, Index);
	SETGET(char, Increment);
	SETGET(char, AnimIndex);
	SETGET(DWORD, NextChange);
	SETGET(WORD, LightColor);
};
//---------------------------------------------------------------------------

class TIndexSound
{
private:
	DWORD m_Address;
	DWORD m_Size;
	DWORD m_Timer;
	DWORD m_LastAccessTime;
public:
	TIndexSound();
	virtual ~TIndexSound();

	SETGET(DWORD, Address);
	SETGET(DWORD, Size);
	SETGET(DWORD, Timer);
	SETGET(DWORD, LastAccessTime);
	
	//Mix_Chunk *Sound;

	WaveFile waveFile;
	HSTREAM hStream;
};
//---------------------------------------------------------------------------
class TIndexAnimation
{
private:
	DWORD m_Address;
	DWORD m_Offset;
	WORD m_Graphic;
public:
	TIndexAnimation();
	virtual ~TIndexAnimation();

	TTextureAnimation *Group;

	SETGET(DWORD, Address);
	SETGET(DWORD, Offset);
	SETGET(WORD, Graphic);
};
//---------------------------------------------------------------------------
class TIndexMulti
{
private:
	DWORD m_Address;
	DWORD m_Size;
	WORD m_Count;
public:
	TIndexMulti();
	virtual ~TIndexMulti();

	SETGET(DWORD, Address);
	SETGET(DWORD, Size);
	SETGET(WORD, Count);
};
//---------------------------------------------------------------------------
#endif