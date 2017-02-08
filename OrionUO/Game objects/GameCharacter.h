/***********************************************************************************
**
** GameCharacter.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GAMECHARACTER_H
#define GAMECHARACTER_H
//----------------------------------------------------------------------------------
#include "GameObject.h"
#include "../Walker/WalkStack.h"
#include "../GLEngine/GLTextTexture.h"
//----------------------------------------------------------------------------------
//!Игровой персонаж
class CGameCharacter: public CGameObject
{
	SETGET(short, Hits);
	SETGET(short, MaxHits);
	SETGET(short, MaxMana);
	SETGET(short, MaxStam);
	SETGET(short, Mana);
	SETGET(short, Stam);
	SETGET(bool, Female);
	SETGET(RACE_TYPE, Race);
	SETGET(uchar, Direction);
	SETGET(uchar, Notoriety);
	SETGET(bool, CanChangeName);
	SETGET(uchar, AnimationInterval);
	SETGET(uchar, AnimationFrameCount);
	SETGET(uchar, AnimationRepeatMode);
	SETGET(uchar, AnimationGroup);
	SETGET(bool, AnimationRepeat);
	SETGET(bool, AnimationDirection);
	SETGET(bool, AnimationFromServer);
	SETGET(uint, LastStepSoundTime);
	SETGET(uint, TimeToRandomFidget);
	SETGET(uchar, StepSoundOffset);

	SETGET(int, OffsetX);
	SETGET(int, OffsetY);
	SETGET(int, OffsetZ);
	SETGET(uint, LastStepTime);
	SETGET(string, PaperdollText);
	SETGET(uchar, HitsPercent);

	SETGET(bool, Deleted);

protected:
	/*!
	Проверка, шаг ли это или телепорт (определяет телепорт на 1 тайл по направлению движения как шаг)
	@param [__inout] cx Текущая координата X
	@param [__inout] cy Текущая координата Y
	@param [__in] x Новая координата X
	@param [__in] y Новая координата Y
	@param [__in] dir Направление персонажа
	@return Результат выполнения шаг/телепорт
	*/
	bool IsCorrectStep(short &cx, short &cy, short &x, short &y, const uchar &dir);

	/*!
	Скорректировать отношение индексов групп анимаций
	@param [__in] graphic Индекс картинки
	@param [__in] group Группа анимаций
	@param [__inout] animation Индекс анимации в группе
	@return 
	*/
	void CorrectAnimationGroup(const ushort &graphic, const ANIMATION_GROUPS &group, uchar &animation);

public:
	CGameCharacter(const uint &serial = 0);
	virtual ~CGameCharacter();

	//Ссылка на контейнер для текста урона
	CTextContainer m_DamageTextControl;

	//!Ссылка на счетчик шагов
	CWalkStack m_WalkStack;

	CGLTextTexture m_HitsTexture;

	void UpdateHitsTexture(const uchar &hits);

	/*!
	Сидит ли персонаж
	@return Индекс объекта из таблицы, на котором он восседает
	*/
	int IsSitting();

	virtual void Draw(const int &x, const int &y);
	virtual void Select(const int &x, const int &y);

	/*!
	Обновить информацию о поле персонажа, обновление гампов
	@param [__in_opt] direction Направление персонажа
	@return 
	*/
	void OnGraphicChange(int direction = 0);

	/*!
	Установка группы анимации
	@param [__in] val Новое значение группы анимации
	@return 
	*/
	void ResetAnimationGroup(const uchar &val);

	/*!
	Установка случайной анимации (при длительном простое)
	@return 
	*/
	void SetRandomFidgetAnimation();

	/*!
	Установка анимации от сервера
	@param [__in] id Группа анимаци
	@param [__in_opt] interval Задержка между кадрами
	@param [__in_opt] frameCount Количество кадлов анимации
	@param [__in_opt] repeatCount Количество повторов анимации
	@param [__in_opt] repeat Зациклено или нет
	@param [__out_opt] frameDirection Направление прокрутки кадров (вперед/назад)
	@return 
	*/
	void SetAnimation(const uchar &id, const uchar &interval = 0, const uchar &frameCount = 0, const uchar &repeatCount = 0, const bool &repeat = false, const bool &frameDirection = false);

	/*!
	Получить индекс картинки для вычисления картинки анимации
	@return Индекс картинки персонажа
	*/
	ushort GetMountAnimation();

	/*!
	Получить текущую группу анимации
	@param [__in_opt] graphic Индекс картинки персонажа
	@return Индекс группы анимации
	*/
	uchar GetAnimationGroup(ushort graphic = 0);

	/*!
	Скорректировать отношение анимаций
	@param [__in] group Группа анимации
	@param [__inout] animation Индекс группы анимации
	@return 
	*/
	void GetAnimationGroup(const ANIMATION_GROUPS &group, uchar &animation);

	/*!
	Состояние, если персонаж не движется
	@return true - стоит, false - в движении
	*/
	bool Staying() { return m_AnimationGroup == 0xFF && m_WalkStack.Empty(); }

	/*!
	Проверка на возможность изменения направления персонажа при движении в сидячем положении
	@param [__in] group Индекс группы анимации
	@return Можно изменять направление или нет
	*/
	bool TestStepNoChangeDirection(const uchar &group);

	/*!
	Если персонаж идет (или только что закончил передвигаться)
	@return true - в движении, false - нет
	*/
	bool Walking() { return (m_LastStepTime > (uint)(g_Ticks - WALKING_DELAY));	}

	/*!
	не подписанная функция
	@param [__inout] dir не подписанный параметр
	@param [__in] canChange Можно ли изменять состояние стека хотьбы или нет
	@return 
	*/
	void UpdateAnimationInfo(uchar &dir, const bool &canChange = false);

	/*!
	Проверка изменения координат, телепорт ли это
	@param [__in] x Новая координата X
	@param [__in] y Новая координата Y
	@param [__in] dir Новое направление персонажа
	@return true - телепорт, false - шаг
	*/
	bool IsTeleportAction(short &x, short &y, const uchar &dir);

	/*!
	Проверка на человекоподобного персонажа
	@return Человекоподобное или нет
	*/
	bool IsHuman() { return (IN_RANGE(m_Graphic, 0x0190, 0x0193) || IN_RANGE(m_Graphic, 0x025D, 0x025E) || IN_RANGE(m_Graphic, 0x029A, 0x029B) || (m_Graphic == 0x03DB) || (m_Graphic == 0x03DF) || (m_Graphic == 0x03E2)); }

	/*!
	Жив или мертв объект
	@return Мертв или нет
	*/
	bool Dead() { return ((m_Graphic == 0x0192) || (m_Graphic == 0x0193)); }

	virtual CGameCharacter *GameCharacterPtr() { return this; }
 };
 //----------------------------------------------------------------------------------
#endif
 //----------------------------------------------------------------------------------
