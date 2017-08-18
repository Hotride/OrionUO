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
//Класс для работы с плавным переключением экрана
class CScreenEffectManager
{
	SETGET(SCREEN_EFFECT_MODE, Mode, SEM_NONE);
	SETGET(SCREEN_EFFECT_TYPE, Type, SET_TO_BLACK);
	SETGET(bool, Enabled, false);
	SETGET(GLfloat, ColorR, 0.0f);
	SETGET(GLfloat, ColorG, 0.0f);
	SETGET(GLfloat, ColorB, 0.0f);
	SETGET(GLfloat, Alpha, 1.0f);
	SETGET(GLfloat, Step, 0.03f);

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
