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
	Correct animation index by group
	@param [__in] graphic Body graphic
	@param [__in] group Animation group
	@param [__inout] animation Animation index
	@return 
	*/
	void CorrectAnimationGroup(const ushort &graphic, const ANIMATION_GROUPS &group, uchar &animation);

public:
	/*!
	Constructor
	@param [__in] serial Character's serial
	@return
	*/
	CGameCharacter(const uint &serial);

	/*!
	Destructor
	@return
	*/
	virtual ~CGameCharacter();

	//!Damage text container
	CTextContainer m_DamageTextControl{ CTextContainer(10) };

	//!Steps stack
	deque<CWalkData> m_Steps;

	//!Texture for hurrent hitpoints value
	CGLTextTexture m_HitsTexture{ CGLTextTexture() };

	/*!
	Update text coordinates
	@return
	*/
	virtual void UpdateTextCoordinates();

	/*!
	Update hitpoints text texture
	@param [__in] hits Current hitpoints
	@return
	*/
	void UpdateHitsTexture(const uchar &hits);

	/*!
	Process gargoyle animations
	@param [__in] animGroup Animation group
	@return
	*/
	void ProcessGargoyleAnims(int &animGroup);

	/*!
	Сидит ли персонаж
	@return Индекс объекта из таблицы, на котором он восседает
	*/
	/*!
	Sitting/staying character state
	@return Sitting table index+1, 0 if not sitting
	*/
	int IsSitting();

	/*!
	Draw character in the world
	@param [__in] x Place on screen coordinate X
	@param [__in] y Place on screen coordinate Y
	@return
	*/
	virtual void Draw(const int &x, const int &y);

	/*!
	Select character in the world
	@param [__in] x Place on screen coordinate X
	@param [__in] y Place on screen coordinate Y
	@return
	*/
	virtual void Select(const int &x, const int &y);

	/*!
	Update graphic event
	@param [__in_opt] direction Direction of character
	@return
	*/
	void OnGraphicChange(int direction = 0);

	/*!
	Reset animation group index
	@param [__in] val New animation group index
	@return
	*/
	void ResetAnimationGroup(const uchar &val);

	/*!
	Reset animation group index to random fidget
	@return
	*/
	void SetRandomFidgetAnimation();

	/*!
	Set animation data
	@param [__in] id Animation group
	@param [__in_opt] interval Frame delay
	@param [__in_opt] frameCount Frame count
	@param [__in_opt] repeatCount Repeat animation count
	@param [__in_opt] repeat Is repeated
	@param [__in_opt] frameDirection Frame direction (forwarf/backward)
	@return
	*/
	void SetAnimation(const uchar &id, const uchar &interval = 0, const uchar &frameCount = 0, const uchar &repeatCount = 0, const bool &repeat = false, const bool &frameDirection = false);

	/*!
	Get mount animation index
	@return Graphic
	*/
	ushort GetMountAnimation();

	/*!
	Get current animation index
	@param [__in_opt] checkGraphic Current graphic
	@return Animation group index
	*/
	uchar GetAnimationGroup(ushort checkGraphic = 0);

	/*!
	Correct animation index
	@param [__in] group Animation group
	@param [__out] animation Animation index
	@return
	*/
	void GetAnimationGroup(const ANIMATION_GROUPS &group, uchar &animation);

	/*!
	Staying character state
	@return true if character is staying
	*/
	bool Staying() { return m_AnimationGroup == 0xFF && m_Steps.empty(); }

	/*!
	Check for the possibility of changing the direction of the character when driving in a seated position
	@param [__in] group Animation group
	@return true if direction can be changed
	*/
	bool TestStepNoChangeDirection(const uchar &group);

	/*!
	Character walking state
	@return true if walking
	*/
	virtual bool Walking() { return (m_LastStepTime > (uint)(g_Ticks - WALKING_DELAY)); }

	/*!
	Check for animation frame changing
	@return true if don't need iterate frames
	*/
	virtual bool NoIterateAnimIndex() { return ((m_LastStepTime > (uint)(g_Ticks - WALKING_DELAY)) && m_Steps.empty()); }

	/*!
	Update character animation state and world position
	@param [__out] dir Direction
	@param [__in_opt] canChange Can change private fields/stacks
	@return
	*/
	void UpdateAnimationInfo(uchar &dir, const bool &canChange = false);

	/*!
	Check on humanoid
	@return true if character is humanoid
	*/
	bool IsHuman() { return (IN_RANGE(m_Graphic, 0x0190, 0x0193) || IN_RANGE(m_Graphic, 0x00B7, 0x00BA) || IN_RANGE(m_Graphic, 0x025D, 0x0260) || IN_RANGE(m_Graphic, 0x029A, 0x029B) || IN_RANGE(m_Graphic, 0x02B6, 0x02B7) || (m_Graphic == 0x03DB) || (m_Graphic == 0x03DF) || (m_Graphic == 0x03E2)); }

	/*!
	Check on dead
	@return true if graphic is ghost
	*/
	bool Dead() { return (IN_RANGE(m_Graphic, 0x0192, 0x0193) || IN_RANGE(m_Graphic, 0x025F, 0x0260) || IN_RANGE(m_Graphic, 0x02B6, 0x02B7)); }

	/*!
	Get character pointer
	@return Always self
	*/
	virtual CGameCharacter *GameCharacterPtr() { return this; }

	/*!
	Find secure trade box object
	@return Item pointer or NULL if box is not found
	*/
	virtual CGameItem *FindSecureTradeBox();
 };
 //----------------------------------------------------------------------------------
#endif
 //----------------------------------------------------------------------------------
