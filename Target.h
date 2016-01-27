/****************************************************************************
**
** Target.h
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
#ifndef TargetH
#define TargetH
//--------------------------------------------------------------------------
class TTarget
{
private:
	BYTE m_Type;
	BYTE m_CursorType;
	DWORD m_CursorID;

	bool m_Targeting;
	
	BYTE m_Data[19];
	BYTE m_LastData[19];

	WORD m_MultiGraphic;

	TMulti *m_Multi;
	
	void AddMultiObject(TMultiObject *obj);
public:
	TTarget();
	~TTarget() {}
	
	SETGET(BYTE, Type);
	SETGET(BYTE, CursorType);
	SETGET(WORD, MultiGraphic);
	
	void SetData(PBYTE buf, int size);
	void SetMultiData(PBYTE buf, int size);

	bool IsTargeting() const {return m_Targeting;}

	void Reset();

	void SendTargetObject(DWORD Serial);
	void SendTargetTile(WORD tileID, WORD X, WORD Y, char Z);
	void SendCancelTarget();
	void SendLastTarget();
	
	void SendTarget();
	
	void LoadMulti(int x, int y, char z);
	void UnloadMulti();
	TMulti *GetMultiAtXY(short x, short y);
};
//--------------------------------------------------------------------------
extern TTarget Target;
//--------------------------------------------------------------------------
#endif