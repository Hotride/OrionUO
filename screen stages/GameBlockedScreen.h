/****************************************************************************
**
** GameBlockedGump.h
**
** Copyright (C) September 2015 Hotride
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
*****************************************************************************
*/
//---------------------------------------------------------------------------
#ifndef GameBlockedScreenH
#define GameBlockedScreenH
//---------------------------------------------------------------------------
class TGameBlockedScreen : public TBaseScreen
{
private:
	//!Код блокировки
	BYTE m_Code;

public:
	TGameBlockedScreen();
	~TGameBlockedScreen();

	SETGET(BYTE, Code);

	/*!
	Инициализация
	@return
	*/
	void Init();

	/*!
	Отрисовка/выбор объектов
	@param [__in] mode true - отрисовка, false - выбор
	@return При выборе объектов - идентификатор выбранного объекта
	*/
	int Render(__in bool mode);



	/*!
	Нажатие левой кнопки мыши
	@return 
	*/
	void OnLeftMouseDown();

	/*!
	Отпускание левой кнопки мыши
	@return 
	*/
	void OnLeftMouseUp();

	/*!
	Обработка нажатия клавиши
	@param [__in] wparam не подписанный параметр
	@param [__in] lparam не подписанный параметр
	@return 
	*/
	void OnCharPress(__in WPARAM wparam, __in LPARAM lparam);

	/*!
	Обработка нажатия клавиши
	@param [__in] wparam не подписанный параметр
	@param [__in] lparam не подписанный параметр
	@return 
	*/
	void OnKeyPress(__in WPARAM wparam, __in LPARAM lparam);

};
//---------------------------------------------------------------------------
extern TGameBlockedScreen *GameBlockedScreen;
//---------------------------------------------------------------------------
#endif