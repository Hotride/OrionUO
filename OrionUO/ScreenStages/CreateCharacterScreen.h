﻿/***********************************************************************************
**
** CreateCharacterScreen.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef CREATECHARACTERSCREEN_H
#define CREATECHARACTERSCREEN_H
//----------------------------------------------------------------------------------
class CCreateCharacterScreen : public CBaseScreen
{
	//!Выбранный стиль прически/бороды
protected:
    int m_StyleSelection = 0;
public:
    int GetStyleSelection() { return m_StyleSelection; };
    void SetStyleSelection(int val);

	//!Выбранный цвет тела/одежды/волосеного покрова
protected:
    int m_ColorSelection = 0;
public:
    int GetColorSelection() { return m_ColorSelection; };
    void SetColorSelection(int val);
	string Name = "";

private:
	CGumpScreenCreateCharacter m_CreateCharacterGump;

public:
	CCreateCharacterScreen();
	virtual ~CCreateCharacterScreen();

	//!Идентификаторы событий для плавного перехода
	static const uchar ID_SMOOTH_CCS_QUIT = 1;
	static const uchar ID_SMOOTH_CCS_GO_SCREEN_CHARACTER = 2;
	static const uchar ID_SMOOTH_CCS_GO_SCREEN_CONNECT = 3;
	static const uchar ID_SMOOTH_CCS_GO_SCREEN_SELECT_TOWN = 4;

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



	virtual void OnLeftMouseButtonDown();

	/*!
	Обработка нажатия клавиши
	@param [__in] wparam не подписанный параметр
	@param [__in] lparam не подписанный параметр
	@return 
	*/
	void OnCharPress(const WPARAM &wParam, const LPARAM &lParam);

	/*!
	Обработка нажатия клавиши
	@param [__in] wparam не подписанный параметр
	@param [__in] lparam не подписанный параметр
	@return 
	*/
	void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam);
};
//----------------------------------------------------------------------------------
extern CCreateCharacterScreen g_CreateCharacterScreen;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
