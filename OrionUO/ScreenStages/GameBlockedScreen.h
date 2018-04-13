/***********************************************************************************
**
** GameBlockedScreen.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GAMEBLOCKEDSCREEN_H
#define GAMEBLOCKEDSCREEN_H
//----------------------------------------------------------------------------------
class CGameBlockedScreen : public CBaseScreen
{
public:
	//!Код блокировки
	uchar Code = 0;

	CGump m_GameBlockedScreenGump;

	CGameBlockedScreen();
	~CGameBlockedScreen();


	/*!
	Инициализация
	@return
	*/
	virtual void Init();

	/*!
	Отрисовка/выбор объектов
	@param [__in] mode true - отрисовка, false - выбор
	@return При выборе объектов - идентификатор выбранного объекта
	*/
	virtual void Render(const bool &mode);



	virtual void OnLeftMouseButtonDown();
	virtual void OnLeftMouseButtonUp();
	virtual void OnCharPress(const WPARAM &wParam, const LPARAM &lParam);
	virtual void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam);
};
//----------------------------------------------------------------------------------
extern CGameBlockedScreen g_GameBlockedScreen;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
