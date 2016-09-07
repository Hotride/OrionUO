/***********************************************************************************
**
** DebugScreen.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef DEBUGSCREEN_H
#define DEBUGSCREEN_H
//----------------------------------------------------------------------------------
#include "BaseScreen.h"
//----------------------------------------------------------------------------------
class CDebugScreen : public CBaseScreen
{
private:
	static const uchar ID_SMOOTH_DS_QUIT = 1;
	static const uchar ID_SMOOTH_DS_GO_SCREEN_MAIN = 2;

	static const int ID_DS_QUIT = 1;
	static const int ID_DS_GO_SCREEN_MAIN = 2;

	static const bool DISPLAY_MAP_TEST = true;
	static const bool DISPLAY_SHADOW_TEST = false;

	void ProcessSmoothAction(uchar action = 0xFF);

	/*TGameItem *m_Chair;
	TGameCharacter *m_Player;
	TGameItem *m_Mount;
	TGameItem *m_Cloak;

	TGumpMap *m_Map1;
	TGumpMap *m_Map2;
	TGumpMap *m_Map3;*/

public:
	CDebugScreen();
	virtual ~CDebugScreen();

	void Init();

	void InitToolTip();

	void Render(const bool &mode);

	virtual void OnLeftMouseButtonUp();
	virtual void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam);
};
//----------------------------------------------------------------------------------
extern CDebugScreen g_DebugScreen;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
