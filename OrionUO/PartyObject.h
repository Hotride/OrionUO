/***********************************************************************************
**
** PartyObject.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef PARTYOBJECT_H
#define PARTYOBJECT_H
//----------------------------------------------------------------------------------
#include "Globals.h"
//---------------------------------------------------------------------------
//!Get ordinary class property customized
#define GET_PARTY(type, name) \
	protected: \
	type m_##name; \
	public: \
	type Get##name(); \
	__declspec(property(get = Get##name)) type name
//----------------------------------------------------------------------------------
//Объект группы
class CPartyObject
{
	SETGET(uint, Serial);

	GET_PARTY(short, Hits);
	GET_PARTY(short, MaxHits);
	GET_PARTY(short, Mana);
	GET_PARTY(short, MaxMana);
	GET_PARTY(short, Stam);
	GET_PARTY(short, MaxStam);
	GET_PARTY(NOTORIETY_TYPE, Notoriety);
	GET_PARTY(bool, CanChangeName);

public:
	CPartyObject();
	~CPartyObject() {}

	string GetName(const int &index);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
