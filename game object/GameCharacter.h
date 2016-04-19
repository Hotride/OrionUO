/****************************************************************************
**
** GameObject.h
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
#ifndef GameCharacterH
#define GameCharacterH
//--------------------------------------------------------------------------
//!Игровой персонаж
class TGameCharacter: public TGameObject
{
protected:
	//!Текущее значение ХП
	short m_Hits;

	//!Максимальное значение ХП
	short m_MaxHits;

	//!Текущее значение маны
	short m_Mana;

	//!Максимальное значение маны
	short m_MaxMana;

	//!Текущее значение стамины
	short m_Stam;

	//!Максимальное значение стамины
	short m_MaxStam;

	//!Пол
	bool m_Sex;

	//!Расса
	BYTE m_Race;

	//!Направление
	BYTE m_Direction;

	//!Злобность
	BYTE m_Notoriety;

	//!Возможность изменить имя в статус баре
	bool m_CanChangeName;

	//!Группа анимации
	BYTE m_AnimationGroup;

	//!Задержка смены кадров
	BYTE m_AnimationInterval;

	//!Количество кадров
	BYTE m_AnimationFrameCount;

	//!Метод воспроизведения анимации
	BYTE m_AnimationRepeatMode;

	//!Повторять анимацию?
	bool m_AnimationRepeat;

	//!Направление проигрывания анимации
	bool m_AnimationDirection;

	//!Если анимация пришла с сервера
	bool m_AnimationFromServer;

	//!Время последнего проигрывания звука для хотьбы
	DWORD m_LastStepSoundTime;

	//!Время для установки случайной анимации
	DWORD m_TimeToRandomFidget;

	//!Смещение картинки по оси X
	int m_OffsetX;

	//!Смещение картинки по оси Y
	int m_OffsetY;

	//!Смещение картинки по оси Z
	int m_OffsetZ;

	//!Время последнего шага
	DWORD m_LastStepTime;

	//!Задержка после шага (для корректного отображения анимации хотьбы)
	short m_AfterStepDelay;

	//!Смещение для звука шага
	BYTE m_StepSoundOffset;
	
	/*!
	Проверка, шаг ли это или телепорт (определяет телепорт на 1 тайл по направлению движения как шаг)
	@param [__inout] cx Текущая координата X
	@param [__inout] cy Текущая координата Y
	@param [__in] x Новая координата X
	@param [__in] y Новая координата Y
	@param [__in] dir Направление персонажа
	@return Результат выполнения шаг/телепорт
	*/
	bool IsCorrectStep(__inout WORD &cx, __inout WORD &cy, __in WORD &x, __in WORD &y, __in BYTE &dir);

	/*!
	Скорректировать отношение индексов групп анимаций
	@param [__in] graphic Индекс картинки
	@param [__in] group Группа анимаций
	@param [__inout] animation Индекс анимации в группе
	@return 
	*/
	void CorrectAnimationGroup(__in WORD &graphic, __in ANIMATION_GROUPS &group, __inout BYTE &animation);

public:
	TGameCharacter(DWORD serial = 0);
	virtual ~TGameCharacter();

	//Ссылка на контейнер для текста урона
	TTextContainer *m_DamageTextControl;

	//!Ссылка на счетчик шагов
	TWalkStack m_WalkStack;

	//!Текстура текста папердолла
	TTextTexture m_PaperdollTextTexture;

	SETGETEX(short, Hits);
	SETGETEX(short, MaxHits);
	SETGETEX(short, MaxMana);
	SETGETEX(short, MaxStam);
	SETGETEX(short, Mana);
	SETGETEX(short, Stam);
	SETGET(bool, Sex);
	SETGET(BYTE, Race);
	SETGET(BYTE, Direction);
	SETGET(BYTE, Notoriety);
	SETGET(bool, CanChangeName);
	SETGET(short, AfterStepDelay);
	SETGET(BYTE, AnimationInterval);
	SETGET(BYTE, AnimationFrameCount);
	SETGET(BYTE, AnimationRepeatMode);
	SETGET(bool, AnimationRepeat);
	SETGET(bool, AnimationDirection);
	SETGET(bool, AnimationFromServer);
	SETGET(DWORD, LastStepSoundTime);
	SETGET(DWORD, TimeToRandomFidget);
	SETGET(BYTE, StepSoundOffset);

	SETGETEX(int, OffsetX);
	SETGETEX(int, OffsetY);
	SETGETEX(int, OffsetZ);
	SETGET(DWORD, LastStepTime);

	/*!
	Сидит ли персонаж
	@return Индекс объекта из таблицы, на котором он восседает
	*/
	int IsSitting();

	/*!
	Отрисовать персонажа
	@param [__in] mode Режим рисования. true - рисование, false - выбор объектов
	@param [__in] drawX Экранная координата X объекта
	@param [__in] drawY Экранная координата Y объекта
	@param [__in] ticks Таймер рендера
	@return При выборе объектов возвращает выбранный элемент
	*/
	virtual int Draw(__in bool &mode, __out int &drawX, __in int &drawY, __in DWORD &ticks);

	/*!
	Обновить информацию о поле персонажа, обновление гампов
	@param [__in_opt] direction Направление персонажа
	@return 
	*/
	void OnGraphicChange(__in_opt int direction = 0);

	/*!
	Сгенерировать текстуру текста папердолла
	@param [__in] val Текст папердолла
	@return 
	*/
	void GeneratePaperdollText(__in string val);

	/*!
	Установка группы анимации
	@param [__in] val Новое значение группы анимации
	@return 
	*/
	void SetAnimationGroup(__in BYTE val);

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
	void SetAnimation(__in BYTE id, __in_opt BYTE interval = 0, __in_opt BYTE frameCount = 0, __in_opt BYTE repeatCount = 0, __in_opt bool repeat = false, __out_opt bool frameDirection = false);

	/*!
	Получить индекс картинки для вычисления картинки анимации
	@return Индекс картинки персонажа
	*/
	WORD GetMountAnimation();

	/*!
	Получить текущую группу анимации
	@param [__in_opt] graphic Индекс картинки персонажа
	@return Индекс группы анимации
	*/
	BYTE GetAnimationGroup(__in_opt WORD graphic = 0);

	/*!
	Скорректировать отношение анимаций
	@param [__in] group Группа анимации
	@param [__inout] animation Индекс группы анимации
	@return 
	*/
	void GetAnimationGroup(__in ANIMATION_GROUPS group, __inout BYTE &animation);

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
	bool TestStepNoChangeDirection(__in BYTE group);

	/*!
	Если персонаж идет (или только что закончил передвигаться)
	@return true - в движении, false - нет
	*/
	bool Walking() {
		return (m_LastStepTime > (DWORD)(GetTickCount() - m_AfterStepDelay));
	}

	/*!
	не подписанная функция
	@param [__inout] dir не подписанный параметр
	@param [__in] canChange Можно ли изменять состояние стека хотьбы или нет
	@return 
	*/
	void UpdateAnimationInfo(__inout BYTE &dir, __in bool canChange = false);

	/*!
	Проверка изменения координат, телепорт ли это
	@param [__in] x Новая координата X
	@param [__in] y Новая координата Y
	@param [__in] dir Новое направление персонажа
	@return true - телепорт, false - шаг
	*/
	bool IsTeleportAction(__in WORD &x, __in WORD &y, __in BYTE &dir);

	/*!
	Проверка на человекоподобного персонажа
	@return Человекоподобное или нет
	*/
	bool IsHuman() { return (((m_Graphic >= 0x0190) && (m_Graphic <= 0x0193)) || (m_Graphic == 0x03DB) || (m_Graphic == 0x03DF) || (m_Graphic == 0x03E2)); }

	/*!
	Жив или мертв объект
	@return Мертв или нет
	*/
	bool Dead() { return ((m_Graphic == 0x0192) || (m_Graphic == 0x0193)); }

	/*!
	Найти объект на указанном слое
	@param [__in] layer Номер слоя
	@return Ссылка на найденный объект или NULL
	*/
	TGameItem *FindLayer(__in int layer);
 };
//---------------------------------------------------------------------------
#endif