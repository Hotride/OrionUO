/****************************************************************************
**
** Party.h
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
#ifndef PartyH
#define PartyH
//---------------------------------------------------------------------------
//Объект группы
class TPartyObject
{
private:
	//Серийник объекта
	DWORD m_Serial;

public:
	TPartyObject();
	~TPartyObject() {}

	SETGET(DWORD, Serial);
	GET_PARTY(short, Hits);
	GET_PARTY(short, MaxHits);
	GET_PARTY(short, Mana);
	GET_PARTY(short, MaxMana);
	GET_PARTY(short, Stam);
	GET_PARTY(short, MaxStam);
	GET_PARTY(NOTORIETY_TYPE, Notoriety);
	GET_PARTY(bool, CanChangeName);

	string GetName(int index);
};
//---------------------------------------------------------------------------
class TParty
{
private:
	//Серийник лидера пати
	DWORD m_Leader;

	//Серийник пригласившего в пати
	DWORD m_Inviter;

	//Может ли группа лутать труп игрока
	bool m_CanLoot;

public:
	TParty();
	~TParty();

	//Члены группы
	TPartyObject Member[10];
	
	SETGET(DWORD, Leader);
	SETGET(DWORD, Inviter);
	SETGET(bool, CanLoot);

	//Обработка пакетов пати
	void ParsePacketData(PBYTE buf, int &size);

	//Содержит ли пати игрока с данным серийником
	bool Contains(DWORD serial);

	//Очистить пати
	void Clear();
};
//---------------------------------------------------------------------------
extern TParty Party;
//---------------------------------------------------------------------------
#endif