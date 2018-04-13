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
public:
	uint CorpseSerial = 0;
	uint ObjectSerial = 0;
	uchar Direction = 0;
	bool Running = false;

	CCorpse() {}
	CCorpse(const uint &corpseSerial, const uint &objectSerial, const uchar &direction, const bool &running)
		: CorpseSerial(corpseSerial), ObjectSerial(objectSerial), Direction(direction), Running(running) {}
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
