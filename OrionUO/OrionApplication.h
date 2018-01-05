/***********************************************************************************
**
** OrionApplication.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef ORIONAPPLICATION_H
#define ORIONAPPLICATION_H
//----------------------------------------------------------------------------------
class COrionApplication : public WISP_APPLICATION::CApplication
{
	SETGET(uint, NextRenderTime, 0);
	SETGET(uint, NextUpdateTime, 0);

protected:
	virtual void OnMainLoop();

public:
	COrionApplication() : WISP_APPLICATION::CApplication() {}
	virtual ~COrionApplication() {}
};
//----------------------------------------------------------------------------------
extern COrionApplication g_App;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
