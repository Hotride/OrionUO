/***********************************************************************************
**
** CorpseManager.h
**
** Copyright (C) August 2017 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef CORPSEMANAGER_H
#define CORPSEMANAGER_H
//----------------------------------------------------------------------------------
class CCorpse
{
	SETGET(uint, CorpseSerial, 0);
	SETGET(uint, ObjectSerial, 0);
	SETGET(uchar, Direction, 0);
	SETGET(bool, Running, false);

public:
	CCorpse() {}
	CCorpse(const uint &corpseSerial, const uint &objectSerial, const uchar &direction, const bool &running)
		: m_CorpseSerial(corpseSerial), m_ObjectSerial(objectSerial), m_Direction(direction), m_Running(running) {}
	~CCorpse() {}
};
//----------------------------------------------------------------------------------
//!Класс менеджера трупов
class CCorpseManager
{
private:
	deque<CCorpse> m_List;

public:
	CCorpseManager() {}
	~CCorpseManager() {}

	void Add(const CCorpse &corpse);

	void Remove(const uint &corpseSerial, const uint &objectSerial);

	bool InList(const uint &corpseSerial, const uint &objectSerial);

	CGameObject *GetCorpseObject(const uint &serial);

	void Clear() { m_List.clear(); }
};
//----------------------------------------------------------------------------------
//!Менеджер трупов
extern CCorpseManager g_CorpseManager;
//----------------------------------------------------------------------------------
#endif //CORPSEMANAGER_H
//----------------------------------------------------------------------------------
