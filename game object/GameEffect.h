/****************************************************************************
**
** GameEffect.h
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
#ifndef GameEffectH
#define GameEffectH
//--------------------------------------------------------------------------
class TGameEffect : public TRenderWorldObject
{
//private:
protected:
	EFFECT_TYPE m_EffectType;
	DWORD m_DestSerial;
	WORD m_DestX;
	WORD m_DestY;
	char m_DestZ;
	BYTE m_Speed;
	DWORD m_Duration;
	bool m_FixedDirection;
	bool m_Explode;
	DWORD m_RenderMode;

	int m_AnimIndex;
	int m_Increment;
	DWORD m_LastChangeFrameTime;
public:
	TGameEffect();
	virtual ~TGameEffect();

	SETGET(EFFECT_TYPE, EffectType);
	SETGET(DWORD, DestSerial);
	SETGET(WORD, DestX);
	SETGET(WORD, DestY);
	SETGET(char, DestZ);
	SETGET(BYTE, Speed);
	SETGET(DWORD, Duration);
	SETGET(bool, FixedDirection);
	SETGET(bool, Explode);
	SETGET(DWORD, RenderMode);
	
	SETGET(int, AnimIndex);
	SETGET(int, Increment);
	SETGET(DWORD, LastChangeFrameTime);

	virtual int Draw(bool &mode, RENDER_LIST_DATA &data, DWORD &ticks);

	WORD GetCurrentGraphic();
	WORD CalculateCurrentGraphic();
	void ApplyRenderMode();

	bool IsEffectObject() {return true;}
};
//--------------------------------------------------------------------------
class TGameEffectMoving : public TGameEffect
{
private:
	int m_DiffX;
	int m_DiffY;
	int m_DiffZ;
	double m_CosA;
	double m_SinA;
	int m_OffsetX;
	int m_OffsetY;
	int m_OffsetZ;
	int m_Step;
	int m_Distance;
public:
	TGameEffectMoving();
	virtual ~TGameEffectMoving();

	SETGET(int, DiffX);
	SETGET(int, DiffY);
	SETGET(int, DiffZ);
	SETGET(double, CosA);
	SETGET(double, SinA);
	SETGET(int, OffsetX);
	SETGET(int, OffsetY);
	SETGET(int, OffsetZ);
	SETGET(int, Step);
	SETGET(int, Distance);
	
	void Init();
	void Update();
};
//--------------------------------------------------------------------------
class TGameEffectDrag : public TGameEffect
{
private:
	int m_OffsetX;
	int m_OffsetY;
public:
	TGameEffectDrag();
	virtual ~TGameEffectDrag();

	SETGETEX(int, OffsetX);
	SETGETEX(int, OffsetY);
};
//---------------------------------------------------------------------------
#endif