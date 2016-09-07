/***********************************************************************************
**
** GumpSpell.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPSPELL_H
#define GUMPSPELL_H
//----------------------------------------------------------------------------------
#include "Gump.h"
//----------------------------------------------------------------------------------
class CGumpSpell : public CGump
{
private:
	static const int ID_GS_LOCK_MOVING = 1;

public:
	CGumpSpell(uint serial, short x, short y, ushort graphic);
	virtual ~CGumpSpell();

	virtual void UpdateContent();

	virtual void OnButton(const uint &serial);
	virtual bool OnLeftMouseButtonDoubleClick();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
