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
public:
	short Str = 0;
	short Int = 0;
	short Dex = 0;
	uchar LockStr = 0;
	uchar LockInt = 0;
	uchar LockDex = 0;
	ushort MaxWeight = 0;
	ushort Weight = 0;
	short Armor = 0;
	uint Gold = 0;
	bool Warmode = 0;
	ushort StatsCap = 0;
	uchar Followers = 0;
	uchar MaxFollowers = 5;
	short FireResistance = 0;
	short ColdResistance = 0;
	short PoisonResistance = 0;
	short EnergyResistance = 0;
	short MaxPhysicalResistance = 0;
	short MaxFireResistance = 0;
	short MaxColdResistance = 0;
	short MaxPoisonResistance = 0;
	short MaxEnergyResistance = 0;
	short Luck = 0;
	short MinDamage = 0;
	short MaxDamage = 0;
	uint TithingPoints = 0;
	short DefenceChance = 0;
	short MaxDefenceChance = 0;
	short AttackChance = 0;
	short WeaponSpeed = 0;
	short WeaponDamage = 0;
	short LowerRegCost = 0;
	short SpellDamage = 0;
	short CastRecovery = 0;
	short CastSpeed = 0;
	short LowerManaCost = 0;

	short OldX = 0;
	short OldY = 0;
	char OldZ = 0;

	/*!
	Constructor
	@param [__in] serial Player's serial
	@return
	*/
	CPlayer(int serial);

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
	virtual bool Walking() { return (LastStepTime > (uint)(g_Ticks - PLAYER_WALKING_DELAY)); }

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
