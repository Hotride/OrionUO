/***********************************************************************************
**
** PartyObject.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "PartyObject.h"
#include "Game objects/GameWorld.h"
//----------------------------------------------------------------------------------
CPartyObject::CPartyObject()
: m_Serial(0), m_Hits(0), m_MaxHits(0), m_Mana(0), m_MaxMana(0), m_Stam(0),
m_MaxStam(0), m_Notoriety(NT_NONE), m_CanChangeName(false)
{
}
//----------------------------------------------------------------------------------
//Get ordinary class property customized
#define GET_PARTY_BODY(type, name) \
type CPartyObject::Get##name() \
{ \
	type result = (type)0; \
	if (m_Serial) \
	{ \
		CGameCharacter *gc = g_World->FindWorldCharacter(m_Serial); \
		if (gc != NULL) \
			result = (type)gc-> ##name; \
	} \
	return result; \
}
//----------------------------------------------------------------------------------
GET_PARTY_BODY(short, Hits)
GET_PARTY_BODY(short, MaxHits)
GET_PARTY_BODY(short, Mana)
GET_PARTY_BODY(short, MaxMana)
GET_PARTY_BODY(short, Stam)
GET_PARTY_BODY(short, MaxStam)
GET_PARTY_BODY(NOTORIETY_TYPE, Notoriety)
GET_PARTY_BODY(bool, CanChangeName)
//----------------------------------------------------------------------------------
string CPartyObject::GetName(const int &index)
{
	if (m_Serial)
	{
		CGameCharacter *gc = g_World->FindWorldCharacter(m_Serial);

		if (gc != NULL)
			return gc->Name;
	}

	char buf[10] = {0};
	sprintf_s(buf, "[%i]", index);

	return string(buf);
}
//----------------------------------------------------------------------------------
