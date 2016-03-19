/****************************************************************************
**
** DebugScreen.h
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
#ifndef DebugScreenH
#define DebugScreenH
//--------------------------------------------------------------------------
class TEffect : public TRenderObject
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
	char m_Z;

	int m_AnimIndex;
	int m_Increment;
	DWORD m_LastChangeFrameTime;
public:
	TEffect();
	virtual ~TEffect();

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
	SETGET(char, Z);

	SETGET(int, AnimIndex);
	SETGET(int, Increment);
	SETGET(DWORD, LastChangeFrameTime);

	virtual int Draw(bool &mode, int &drawX, int &drawY, DWORD &ticks);

	WORD GetCurrentGraphic();
	WORD CalculateCurrentGraphic();
	void ApplyRenderMode();

	bool IsEffectObject() { return true; }
};
//--------------------------------------------------------------------------
class TEffectMoving : public TEffect
{
private:
	float m_Angle;
	int m_OffsetX;
	int m_OffsetY;
	int m_OffsetZ;
public:
	TEffectMoving();
	virtual ~TEffectMoving();

	SETGET(float, Angle);
	SETGET(int, OffsetX);
	SETGET(int, OffsetY);
	SETGET(int, OffsetZ);

	void Update();
};
//---------------------------------------------------------------------------
class TDebugScreen : public TBaseScreen
{
private:
	static const BYTE ID_SMOOTH_DS_QUIT = 1;
	static const BYTE ID_SMOOTH_DS_GO_SCREEN_MAIN = 2;

	static const int ID_DS_QUIT = 1;
	static const int ID_DS_GO_SCREEN_MAIN = 2;

	TEffectMoving *effect;
	void ProcessSmoothAction(BYTE action = 0xFF);
public:
	TDebugScreen();
	virtual ~TDebugScreen();

	void Init();

	void InitPopupHelp();

	int Render(bool mode);

	void OnLeftMouseUp();
	void OnKeyPress(WPARAM wparam, LPARAM lparam);
};
//---------------------------------------------------------------------------
extern TDebugScreen *DebugScreen;
//---------------------------------------------------------------------------
#endif