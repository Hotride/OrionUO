/***********************************************************************************
**
** GumpScreenCharacterList.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPSCREENCHARACTERLIST_H
#define GUMPSCREENCHARACTERLIST_H
//----------------------------------------------------------------------------------
#include "Gump.h"
//----------------------------------------------------------------------------------
class CGumpScreenCharacterList : public CGump
{
private:
	//!Идентификаторы кнопок
	static const int ID_CS_QUIT = 1;
	static const int ID_CS_ARROW_PREV = 2;
	static const int ID_CS_ARROW_NEXT = 3;
	static const int ID_CS_NEW = 4;
	static const int ID_CS_DELETE = 5;
	static const int ID_CS_CHARACTERS = 6;

public:
	CGumpScreenCharacterList();
	virtual ~CGumpScreenCharacterList();

	virtual void UpdateContent();

	virtual void InitToolTip();



	virtual void OnButton(const uint &serial);

	virtual void OnTextEntry(const uint &serial);

	virtual bool OnLeftMouseButtonDoubleClick();
 };
 //----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
