/***********************************************************************************
**
** GamePlayer.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GAMEPLAYER_H
#define GAMEPLAYER_H
//----------------------------------------------------------------------------------
//!Класс игрока
class CPlayer: public CGameCharacter
{
	SETGET(short, Str, 0);
	SETGET(short, Int, 0);
	SETGET(short, Dex, 0);
	SETGET(uchar, LockStr, 0);
	SETGET(uchar, LockInt, 0);
	SETGET(uchar, LockDex, 0);
	SETGET(ushort, MaxWeight, 0);
	SETGET(ushort, Weight, 0);
	SETGET(short, Armor, 0);
	SETGET(uint, Gold, 0);
	SETGET(bool, Warmode, 0);
	SETGET(ushort, StatsCap, 0);
	SETGET(uchar, Followers, 0);
	SETGET(uchar, MaxFollowers, 5);
	SETGET(short, FireResistance, 0);
	SETGET(short, ColdResistance, 0);
	SETGET(short, PoisonResistance, 0);
	SETGET(short, EnergyResistance, 0);
	SETGET(short, MaxPhysicalResistance, 0);
	SETGET(short, MaxFireResistance, 0);
	SETGET(short, MaxColdResistance, 0);
	SETGET(short, MaxPoisonResistance, 0);
	SETGET(short, MaxEnergyResistance, 0);
	SETGET(short, Luck, 0);
	SETGET(short, MinDamage, 0);
	SETGET(short, MaxDamage, 0);
	SETGET(uint, TithingPoints, 0);
	SETGET(short, DefenceChance, 0);
	SETGET(short, MaxDefenceChance, 0);
	SETGET(short, AttackChance, 0);
	SETGET(short, WeaponSpeed, 0);
	SETGET(short, WeaponDamage, 0);
	SETGET(short, LowerRegCost, 0);
	SETGET(short, SpellDamage, 0);
	SETGET(short, CastRecovery, 0);
	SETGET(short, CastSpeed, 0);
	SETGET(short, LowerManaCost, 0);

	SETGET(short, OldX, 0);
	SETGET(short, OldY, 0);
	SETGET(char, OldZ, 0);

public:
	/*!
	Constructor
	@param [__in] serial Player's serial
	@return
	*/
	CPlayer(const uint &serial);

	/*!
	Destructor
	@return
	*/
	virtual ~CPlayer();

	//!Fast Walk Stack
	CFastWalkStack m_FastWalkStack;

	/*!
	Close bank gump
	@return
	*/
	void CloseBank();

	/*!
	Search bandage in backpack
	@return Item pointer or NULL if bandage is not found
	*/
	class CGameItem *FindBandage();

	/*!
	Check on player
	@return Always true
	*/
	bool IsPlayer() {return true;}

	/*!
	Player walking state
	@return true if player if walking
	*/
	virtual bool Walking() { return (m_LastStepTime > (uint)(g_Ticks - PLAYER_WALKING_DELAY)); }

	/*!
	No iterate animation frames
	@return Always false for player
	*/
	virtual bool NoIterateAnimIndex() { return false; }

	/*!
	Update current abilities
	@return
	*/
	void UpdateAbilities();
};
//----------------------------------------------------------------------------------
//!Ссылка на игрока
extern CPlayer *g_Player;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
