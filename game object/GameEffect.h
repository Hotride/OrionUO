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
//!Игровой эффект
class TGameEffect : public TRenderWorldObject
{
protected:
	//!Тип эффекта
	EFFECT_TYPE m_EffectType;

	//!Серийник назначения
	DWORD m_DestSerial;

	//!Координата назначения по оси X
	WORD m_DestX;

	//!Координата назначения по оси Y
	WORD m_DestY;

	//!Координата назначения по оси Z
	char m_DestZ;

	//!Скорость
	BYTE m_Speed;

	//!Время жизни
	DWORD m_Duration;

	//!Изменять направление картинки при полете эффекта?
	bool m_FixedDirection;

	//!Взрывать по достижении цели?
	bool m_Explode;

	//!Режим отображения эффекта
	DWORD m_RenderMode;

	//!Индекс картинки
	int m_AnimIndex;

	//!Смещение от базового индекса
	int m_Increment;

	//!Последнее время изменения картинки
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

	/*!
	Отрисовать эффект
	@param [__in] mode Режим рисования. true - рисование, false - выбор объектов
	@param [__in] drawX Экранная координата X объекта
	@param [__in] drawY Экранная координата Y объекта
	@param [__in] ticks Таймер рендера
	@return При выборе объектов возвращает выбранный элемент
	*/
	virtual int Draw(__in bool &mode, __in int &drawX, __in int &drawY, __in DWORD &ticks);

	/*!
	Получить текущий индекс картинки
	@return Индекс картинки
	*/
	WORD GetCurrentGraphic();

	/*!
	Вычислить текущий индекс картинки
	@return Индекс картинки
	*/
	WORD CalculateCurrentGraphic();

	/*!
	Применение режима отображения
	@return 
	*/
	void ApplyRenderMode();

	/*!
	Отмена режима отображения
	@return 
	*/
	void RemoveRenderMode();

	/*!
	Проверка, эффект ли это
	@return Эффект или нет
	*/
	bool IsEffectObject() {return true;}

	/*!
	Обновить эффект
	@return 
	*/
	virtual void Update() {}
};
//--------------------------------------------------------------------------
//Эффект перемещения предмета
class TGameEffectDrag : public TGameEffect
{
protected:
	//!Смещение по оси X
	int m_OffsetX;

	//!Смещение по оси Y
	int m_OffsetY;
public:
	TGameEffectDrag();
	virtual ~TGameEffectDrag();

	SETGET(int, OffsetX);
	SETGET(int, OffsetY);

	/*!
	Обновить эффект
	@return
	*/
	virtual void Update();
};
//--------------------------------------------------------------------------
//Движущийся эффект
class TGameEffectMoving : public TGameEffectDrag
{
protected:
	//!Угол поворота эффекта
	float m_Angle;

	//!Смещение по оси Z
	int m_OffsetZ;
public:
	TGameEffectMoving();
	virtual ~TGameEffectMoving();

	SETGET(float, Angle);
	SETGET(int, OffsetZ);

	/*!
	Обновить эффект
	@return
	*/
	virtual void Update();
};
//---------------------------------------------------------------------------
#endif