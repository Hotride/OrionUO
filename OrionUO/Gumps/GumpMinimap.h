/***********************************************************************************
**
** GumpMinimap.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPMINIMAP_H
#define GUMPMINIMAP_H
//----------------------------------------------------------------------------------
#include "Gump.h"
//----------------------------------------------------------------------------------
class CGumpMinimap : public CGump
{
	SETGET(ushort, LastX);
	SETGET(ushort, LastY);

private:
	static const int ID_GMM_LOCK_MOVING = 1;

	uchar m_Count;

	CGLTexture m_Texture;

	CGUIGumppic *m_Body;
	CGUIDataBox *m_DataBox;

	void GenerateMap();

protected:
	virtual void CalculateGumpState();

public:
	CGumpMinimap(uint serial, short x, short y, bool minimized);
	virtual ~CGumpMinimap();

	virtual void PrepareContent();

	virtual void UpdateContent();

	GUMP_BUTTON_EVENT_H;
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
