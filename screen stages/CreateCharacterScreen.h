/****************************************************************************
**
** CreateCharacterGump.h
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
#ifndef CreateCharacterScreenH
#define CreateCharacterScreenH
//---------------------------------------------------------------------------
class TCreateCharacterScreen : public TBaseScreen
{
private:
	//Идентификаторы событий для плавного перехода
	static const BYTE ID_SMOOTH_CCS_QUIT = 1;
	static const BYTE ID_SMOOTH_CCS_GO_SCREEN_CHARACTER = 2;
	static const BYTE ID_SMOOTH_CCS_GO_SCREEN_CONNECT = 3;
	static const BYTE ID_SMOOTH_CCS_GO_SCREEN_SELECT_TOWN = 4;

	//Идентификаторы кнопок
	static const int ID_CCS_QUIT = 1;
	static const int ID_CCS_ARROW_PREV = 2;
	static const int ID_CCS_ARROW_NEXT = 3;
	static const int ID_CCS_NAME_FIELD = 4;
	static const int ID_CCS_MALE_BUTTON = 5;
	static const int ID_CCS_FEMALE_BUTTON = 6;
	static const int ID_CCS_HUMAN_RACE_BUTTON = 7;
	static const int ID_CCS_ELVEN_RACE_BUTTON = 8;
	static const int ID_CCS_GARGOYLE_RACE_BUTTON = 9;

public:
	TCreateCharacterScreen();
	virtual ~TCreateCharacterScreen();

	//Инициализация
	void Init();

	//Обработка события после плавного затемнения экрана
	void ProcessSmoothAction(BYTE action = 0xFF);

	//Инициализация тултипа
	void InitPopupHelp();

	//Рисование экрана
	int Render(bool mode);

	//События
	void OnLeftMouseDown();
	void OnLeftMouseUp();
	void OnCharPress(WPARAM wparam, LPARAM lparam);
	void OnKeyPress(WPARAM wparam, LPARAM lparam);
};
//---------------------------------------------------------------------------
extern TCreateCharacterScreen *CreateCharacterScreen;
//---------------------------------------------------------------------------
#endif