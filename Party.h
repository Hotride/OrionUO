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
class TPartyObject
{
private:
	DWORD m_Serial;

public:
	TPartyObject();
	~TPartyObject() {}

	SETGET(DWORD, Serial)
	GET_PARTY(short, Hits)
	GET_PARTY(short, MaxHits)
	GET_PARTY(short, Mana)
	GET_PARTY(short, MaxMana)
	GET_PARTY(short, Stam)
	GET_PARTY(short, MaxStam)
	GET_PARTY(NOTORIETY_TYPE, Notoriety)
	GET_PARTY(bool, CanChangeName)

	string GetName(int index);
};
//---------------------------------------------------------------------------
class TParty
{
private:
	DWORD m_Leader;
	DWORD m_Inviter;
	bool m_CanLoot;

public:
	TParty();
	~TParty();

	TPartyObject Member[10];
	
	SETGET(DWORD, Leader)
	SETGET(DWORD, Inviter)
	SETGET(bool, CanLoot)

	void ParsePacketData(PBYTE buf, int &size);

	bool Contains(DWORD serial);
	void Clear();
};
//---------------------------------------------------------------------------
extern TParty Party;
//---------------------------------------------------------------------------
#endif