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
public:
	SCREEN_EFFECT_MODE Mode = SEM_NONE;
	SCREEN_EFFECT_TYPE Type = SET_TO_BLACK;
	bool Enabled = false;
	GLfloat ColorR = 0.0f;
	GLfloat ColorG = 0.0f;
	GLfloat ColorB = 0.0f;
	GLfloat Alpha = 1.0f;
	GLfloat Step = 0.03f;

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
