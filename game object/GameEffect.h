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
//Игровой эффект
class TGameEffect : public TRenderWorldObject
{
//private:
protected:
	EFFECT_TYPE m_EffectType; //Тип эффекта

	DWORD m_DestSerial; //Серийник назначения
	WORD m_DestX; //Координата назначения по оси X
	WORD m_DestY; //Координата назначения по оси Y
	char m_DestZ; //Координата назначения по оси Z

	BYTE m_Speed; //Скорость
	DWORD m_Duration; //Время жизни
	bool m_FixedDirection; //Изменять направление картинки при полете эффекта?
	bool m_Explode; //Взрывать по достижении цели?
	DWORD m_RenderMode; //Режим отображения эффекта

	int m_AnimIndex; //Индекс картинки
	int m_Increment; //Смещение от базового индекса
	DWORD m_LastChangeFrameTime; //Последнее время изменения картинки
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

	virtual int Draw(bool &mode, int &drawX, int &drawY, DWORD &ticks);

	//Получить текущий индекс картинки
	WORD GetCurrentGraphic();

	//Вычислить текущий индекс картинки
	WORD CalculateCurrentGraphic();

	//Применение режима отображения
	void ApplyRenderMode();

	//Отмена режима отображения
	void RemoveRenderMode();

	bool IsEffectObject() {return true;}

	virtual void Update() {}
};
//--------------------------------------------------------------------------
//Эффект перемещения предмета
class TGameEffectDrag : public TGameEffect
{
//private:
protected:
	int m_OffsetX; //Смещение по оси X
	int m_OffsetY; //Смещение по оси Y
public:
	TGameEffectDrag();
	virtual ~TGameEffectDrag();

	SETGET(int, OffsetX);
	SETGET(int, OffsetY);

	virtual void Update();
};
//--------------------------------------------------------------------------
//Движущийся эффект
class TGameEffectMoving : public TGameEffectDrag
{
//private:
protected:
	float m_Angle;
	int m_OffsetZ;
public:
	TGameEffectMoving();
	virtual ~TGameEffectMoving();

	SETGET(float, Angle);
	SETGET(int, OffsetZ);

	virtual void Update();
};
//---------------------------------------------------------------------------
#endif