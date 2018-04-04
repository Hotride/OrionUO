/***********************************************************************************
**
** ServerScreen.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef SERVERSCREEN_H
#define SERVERSCREEN_H
//----------------------------------------------------------------------------------
class CServerScreen : public CBaseScreen
{
	//!Временная переменная для идентификации выбранного сервера
	SETGET(int, SelectionServerTempValue, 0);

private:
	CGumpScreenServer m_ServerGump;

public:
	CServerScreen();
	virtual ~CServerScreen();

	//!Идентификаторы событий для плавного перехода
	static const uchar ID_SMOOTH_SS_QUIT = 1;
	static const uchar ID_SMOOTH_SS_SELECT_SERVER = 2;
	static const uchar ID_SMOOTH_SS_GO_SCREEN_MAIN = 3;

	/*!
	Инициализация
	@return 
	*/
	void Init();

	/*!
	Обработка события после плавного затемнения экрана
	@param [__in_opt] action Идентификатор действия
	@return 
	*/
	void ProcessSmoothAction(uchar action = 0xFF);
	


	virtual void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam);
};
//----------------------------------------------------------------------------------
extern CServerScreen g_ServerScreen;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
