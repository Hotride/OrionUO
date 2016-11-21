/***********************************************************************************
**
** GumpScreenConnection.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPSCREENCONNECTION_H
#define GUMPSCREENCONNECTION_H
//----------------------------------------------------------------------------------
#include "Gump.h"
//----------------------------------------------------------------------------------
class CGumpScreenConnection : public CGump
{
private:
	//!Идентификаторы кнопок
	static const int ID_CS_OK = 1;
	static const int ID_CS_CANCEL = 2;

	void CreateText(const int &x, const int &y, string str, const uchar &font);

public:
	CGumpScreenConnection();
	virtual ~CGumpScreenConnection();

	virtual void UpdateContent();



	GUMP_BUTTON_EVENT_H;
 };
 //----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
