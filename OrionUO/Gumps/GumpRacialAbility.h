/***********************************************************************************
**
** GumpRacialAbility.h
**
** Copyright (C) January 2017 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPRACIALABILITY_H
#define GUMPRACIALABILITY_H
//----------------------------------------------------------------------------------
#include "Gump.h"
//----------------------------------------------------------------------------------
class CGumpRacialAbility : public CGump
{
private:
	static const int ID_GS_LOCK_MOVING = 1;

public:
	CGumpRacialAbility(const uint &serial, const int &x, const int &y);
	virtual ~CGumpRacialAbility();

	static void OnAbilityUse(const ushort &index);

	virtual void InitToolTip();

	GUMP_BUTTON_EVENT_H;

	virtual bool OnLeftMouseButtonDoubleClick();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
