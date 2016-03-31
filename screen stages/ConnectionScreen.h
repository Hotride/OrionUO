/****************************************************************************
**
** ConnectionScreen.h
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
#ifndef ConnectionScreenH
#define ConnectionScreenH
//---------------------------------------------------------------------------
class TConnectionScreen : public TBaseScreen
{
private:
	//!Флаг поровала подключения
	bool m_ConnectionFailed;

	//!Флаг подключения
	bool m_Connected;

	//!Флаг завершения работы экрана
	bool m_Completed;

	//!Код ошибки
	int m_ErrorCode;

	//!Тип экрана
	CONNECTION_SCREEN_TYPE m_Type;

	//!Объекты текста
	TTextTexture m_Text[32];

	//!Идентификаторы событий для плавного перехода
	static const BYTE ID_SMOOTH_CS_GO_SCREEN_MAIN = 1;
	static const BYTE ID_SMOOTH_CS_GO_SCREEN_CHARACTER = 2;
	static const BYTE ID_SMOOTH_CS_GO_SCREEN_PROFESSION = 3;
	static const BYTE ID_SMOOTH_CS_SEND_DELETE = 4;

	//!Идентификаторы кнопок
	static const int ID_CS_OK = 1;
	static const int ID_CS_CANCEL = 2;

public:
	TConnectionScreen();
	~TConnectionScreen();
	
	SETGET(bool, ConnectionFailed);
	SETGET(bool, Connected);
	SETGET(bool, Completed);
	SETGET(int, ErrorCode);
	SETGET(CONNECTION_SCREEN_TYPE, Type);

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
	void ProcessSmoothAction(__in_opt BYTE action = 0xFF);

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
	void OnKeyPress(__in WPARAM wparam, __in LPARAM lparam);

};
//---------------------------------------------------------------------------
extern TConnectionScreen *ConnectionScreen;
//---------------------------------------------------------------------------
#endif