/****************************************************************************
**
** CharacterListGump.h
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
#ifndef CharacterListScreenH
#define CharacterListScreenH
//---------------------------------------------------------------------------
class TCharacterListScreen : public TBaseScreen
{
private:
	//!Объект текста
	TTextTexture m_Text;

	//!Идентификаторы событий для плавного перехода
	static const BYTE ID_SMOOTH_CLS_QUIT = 1;
	static const BYTE ID_SMOOTH_CLS_CONNECT = 2;
	static const BYTE ID_SMOOTH_CLS_SELECT_CHARACTER = 3;
	static const BYTE ID_SMOOTH_CLS_GO_SCREEN_PROFESSION_SELECT = 4;
	static const BYTE ID_SMOOTH_CLS_GO_SCREEN_DELETE = 5;

	//!Идентификаторы кнопок
	static const int ID_CS_QUIT = 1;
	static const int ID_CS_ARROW_PREV = 2;
	static const int ID_CS_ARROW_NEXT = 3;
	static const int ID_CS_NEW = 4;
	static const int ID_CS_DELETE = 5;
	static const int ID_CS_CHARACTERS = 6;
public:
	TCharacterListScreen();
	virtual ~TCharacterListScreen();

	/*!
	Инициализация
	@return 
	*/
	void Init();

	/*!
	Обработка события после перехода
	@param [__in_opt] action Идентификатор действия
	@return 
	*/
	void ProcessSmoothAction(__in_opt BYTE action = 0xFF);

	/*!
	Инициализация всплывающих подсказок
	@return 
	*/
	void InitToolTip();

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
	Двойной клик левой кнопкой мыши
	@return true при успешной обработке
	*/
	bool OnLeftMouseDoubleClick();
};
//---------------------------------------------------------------------------
extern TCharacterListScreen *CharacterListScreen;
//---------------------------------------------------------------------------
#endif