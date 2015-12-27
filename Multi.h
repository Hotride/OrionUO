/****************************************************************************
**
** Multi.h
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
#ifndef MultiH
#define MultiH
//---------------------------------------------------------------------------
class TMultiObject : public TRenderWorldObject
{
private:
	DWORD m_MultiFlags;
	DWORD m_ObjectFlags;

public:
	TMultiObject(WORD graphic, short x, short y, char z, DWORD flags);
	virtual ~TMultiObject();

	SETGET(DWORD, MultiFlags);
	SETGET(DWORD, ObjectFlags);

	virtual int Draw(bool &mode, int &drawX, int &drawY, DWORD &ticks);

	bool IsBackground() {return (m_ObjectFlags & 0x00000001);}
	bool IsWeapon() {return (m_ObjectFlags & 0x00000002);}
	bool IsTransparent() {return (m_ObjectFlags & 0x00000004);}
	bool IsTranslucent() {return (m_ObjectFlags & 0x00000008);}
	bool IsWall() {return (m_ObjectFlags & 0x00000010);}
	bool IsDamaging() {return (m_ObjectFlags & 0x00000020);}
	bool IsImpassable() {return (m_ObjectFlags & 0x00000040);}
	bool IsWet() {return (m_ObjectFlags & 0x00000080);}
	bool IsUnknown() {return (m_ObjectFlags & 0x00000100);}
	bool IsSurface() {return (m_ObjectFlags & 0x00000200);}
	bool IsBridge() {return (m_ObjectFlags & 0x00000400);}
	bool IsStackable() {return (m_ObjectFlags & 0x00000800);}
	bool IsWindow() {return (m_ObjectFlags & 0x00001000);}
	bool IsNoShoot() {return (m_ObjectFlags & 0x00002000);}
	bool IsPrefixA() {return (m_ObjectFlags & 0x00004000);}
	bool IsPrefixAn() {return (m_ObjectFlags & 0x00008000);}
	bool IsInternal() {return (m_ObjectFlags & 0x00010000);}
	bool IsFoliage() {return (m_ObjectFlags & 0x00020000);}
	bool IsPartialHue() {return (m_ObjectFlags & 0x00040000);}
	bool IsUnknown1() {return (m_ObjectFlags & 0x00080000);}
	bool IsMap() {return (m_ObjectFlags & 0x00100000);}
	bool IsContainer() {return (m_ObjectFlags & 0x00200000);}
	bool IsWearable() {return (m_ObjectFlags & 0x00400000);}
	bool IsLightSource() {return (m_ObjectFlags & 0x00800000);}
	bool IsAnimated() {return (m_ObjectFlags & 0x01000000);}
	bool IsNoDiagonal() {return (m_ObjectFlags & 0x02000000);}
	bool IsUnknown2() {return (m_ObjectFlags & 0x04000000);}
	bool IsArmor() {return (m_ObjectFlags & 0x08000000);}
	bool IsRoof() {return (m_ObjectFlags & 0x10000000);}
	bool IsDoor() {return (m_ObjectFlags & 0x20000000);}
	bool IsStairBack() {return (m_ObjectFlags & 0x40000000);}
	bool IsStairRight() {return (m_ObjectFlags & 0x80000000);}

	bool IsMultiObject() {return true;}
};
//---------------------------------------------------------------------------
class TMulti : public TBaseQueueItem
{
private:
	short m_X;
	short m_Y;
	short m_MinX;
	short m_MinY;
	short m_MaxX;
	short m_MaxY;

public:
	TMulti(short x, short y);
	virtual ~TMulti();
	
	SETGET(short, X);
	SETGET(short, Y);
	SETGET(short, MinX);
	SETGET(short, MinY);
	SETGET(short, MaxX);
	SETGET(short, MaxY);
};
//---------------------------------------------------------------------------
#endif