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
//!Game character class
class CGameCharacter: public CGameObject
{
	//!Hitpoints
	SETGET(short, Hits, 0);

	//!Max hitpoints
	SETGET(short, MaxHits, 0);

	//!Mana
	SETGET(short, Mana, 0);

	//!Max mana
	SETGET(short, MaxMana, 0);

	//!Stamina
	SETGET(short, Stam, 0);

	//!Max stamina
	SETGET(short, MaxStam, 0);

	//!Is female
	SETGET(bool, Female, false);

	//!Character's race
	SETGET(RACE_TYPE, Race, RT_HUMAN);

	//!Current direction
	SETGET(uchar, Direction, 0);

	//!Character's notoriety
	SETGET(uchar, Notoriety, 0);

	//!Player can change the name for this character
	SETGET(bool, CanChangeName, false);

	//!Interval between animation playback
	SETGET(uchar, AnimationInterval, 0);

	//!Current animation frame count
	SETGET(uchar, AnimationFrameCount, 0);

	//!Current animation repeat mode
	SETGET(uchar, AnimationRepeatMode, 1);

	//!Current animation group
	SETGET(uchar, AnimationGroup, 0xFF);

	//!Current animation is repeat
	SETGET(bool, AnimationRepeat, false);

	//!Current animation direction (front or back)
	SETGET(bool, AnimationDirection, false);

	//!Current animation called from the server
	SETGET(bool, AnimationFromServer, false);

	//!Last step sound time stamp
	SETGET(uint, LastStepSoundTime, 0);

	//!Time stamp to fidget animation
	SETGET(uint, TimeToRandomFidget, 0);

	//!Offset to step sound
	SETGET(uchar, StepSoundOffset, 0);

	//!Sprite offset by X coordinate on the tile
	SETGET(int, OffsetX, 0);

	//!Sprite offset by Y coordinate on the tile
	SETGET(int, OffsetY, 0);

	//!Sprite offset by Z coordinate on the tile
	SETGET(int, OffsetZ, 0);

	//!Last step time stamp
	SETGET(uint, LastStepTime, 0);

	//!Character's title
	SETGET(string, Title, "");

	//!Percent of hits
	SETGET(uchar, HitsPercent, 0);

protected:
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

	//!Damage text container
	CTextContainer m_DamageTextControl{ CTextContainer(10) };

	//!Steps stack
	deque<CWalkData> m_Steps;

	//!Texture for hurrent hitpoints value
	CGLTextTexture m_HitsTexture{ CGLTextTexture() };

	virtual void UpdateTextCoordinates();

	void UpdateHitsTexture(const uchar &hits);

	//Обработка особенностей анимаций гаргулей.
	void ProcessGargoyleAnims(int &animGroup);

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
	uchar GetAnimationGroup(ushort checkGraphic = 0);

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
	bool Staying() { return m_AnimationGroup == 0xFF && m_Steps.empty(); }

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
	virtual bool Walking() { return (m_LastStepTime > (uint)(g_Ticks - WALKING_DELAY)); }

	virtual bool NoIterateAnimIndex() { return ((m_LastStepTime > (uint)(g_Ticks - WALKING_DELAY)) && m_Steps.empty()); }

	/*!
	не подписанная функция
	@param [__inout] dir не подписанный параметр
	@param [__in] canChange Можно ли изменять состояние стека хотьбы или нет
	@return 
	*/
	void UpdateAnimationInfo(uchar &dir, const bool &canChange = false);

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

	virtual CGameItem *FindSecureTradeBox();
 };
 //----------------------------------------------------------------------------------
#endif
 //----------------------------------------------------------------------------------
