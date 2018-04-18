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
class CGumpRacialAbility : public CGump
{
private:
	static const int ID_GS_LOCK_MOVING = 1;

public:
	CGumpRacialAbility(int serial, int x, int y);
	virtual ~CGumpRacialAbility();

	static void OnAbilityUse(ushort index);

	virtual void InitToolTip();

	GUMP_BUTTON_EVENT_H;

	virtual bool OnLeftMouseButtonDoubleClick();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
