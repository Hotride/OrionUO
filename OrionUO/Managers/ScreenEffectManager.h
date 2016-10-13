/***********************************************************************************
**
** ScreenEffectManager.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef SCREENEFFECTMANAGER_H
#define SCREENEFFECTMANAGER_H
//----------------------------------------------------------------------------------
#include "../Globals.h"
#include "../EnumList.h"
//----------------------------------------------------------------------------------
//Класс для работы с плавным переключением экрана
class CScreenEffectManager
{
	SETGET(SCREEN_EFFECT_MODE, Mode);
	SETGET(SCREEN_EFFECT_TYPE, Type);
	SETGET(bool, Enabled);
	SETGET(GLfloat, ColorR);
	SETGET(GLfloat, ColorG);
	SETGET(GLfloat, ColorB);
	SETGET(GLfloat, Alpha);
	SETGET(GLfloat, Step);

public:
	CScreenEffectManager();
	virtual ~CScreenEffectManager();

	int Process();
	void Draw();
	bool Use(const SCREEN_EFFECT_MODE &mode, const SCREEN_EFFECT_TYPE &type = SET_TO_BLACK, const bool &ignoreEnabled = false);
	bool UseSunrise();
	bool UseSunset();
};
//----------------------------------------------------------------------------------
extern CScreenEffectManager g_ScreenEffectManager;
//----------------------------------------------------------------------------------
#endif
