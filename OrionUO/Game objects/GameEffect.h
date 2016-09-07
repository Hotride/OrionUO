/***********************************************************************************
**
** GameEffect.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GAMEEFFECT_H
#define GAMEEFFECT_H
//----------------------------------------------------------------------------------
#include "../RenderWorldObject.h"
//----------------------------------------------------------------------------------
//!Игровой эффект
class CGameEffect : public CRenderWorldObject
{
	//!Тип эффекта
	SETGET(EFFECT_TYPE, EffectType);
	//!Серийник назначения
	SETGET(uint, DestSerial);
	//!Координата назначения по оси X
	SETGET(ushort, DestX);
	//!Координата назначения по оси Y
	SETGET(ushort, DestY);
	//!Координата назначения по оси Z
	SETGET(char, DestZ);
	//!Скорость
	SETGET(uchar, Speed);
	//!Время жизни
	SETGET(uint, Duration);
	//!Изменять направление картинки при полете эффекта?
	SETGET(bool, FixedDirection);
	//!Взрывать по достижении цели?
	SETGET(bool, Explode);
	//!Режим отображения эффекта
	SETGET(uint, RenderMode);

	//!Индекс картинки
	SETGET(int, AnimIndex);
	//!Смещение от базового индекса
	SETGET(int, Increment);
	//!Последнее время изменения картинки
	SETGET(uint, LastChangeFrameTime);

public:
	CGameEffect();
	virtual ~CGameEffect();

	/*!
	Отрисовать эффект
	@param [__in] mode Режим рисования. true - рисование, false - выбор объектов
	@param [__in] drawX Экранная координата X объекта
	@param [__in] drawY Экранная координата Y объекта
	@param [__in] ticks Таймер рендера
	@return При выборе объектов возвращает выбранный элемент
	*/
	virtual void Draw(const int &x, const int &y);

	/*!
	Получить текущий индекс картинки
	@return Индекс картинки
	*/
	ushort GetCurrentGraphic();

	/*!
	Вычислить текущий индекс картинки
	@return Индекс картинки
	*/
	ushort CalculateCurrentGraphic();

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
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
