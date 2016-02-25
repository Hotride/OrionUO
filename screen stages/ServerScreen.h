/****************************************************************************
**
** ServerGump.h
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
#ifndef ServerScreenH
#define ServerScreenH
//---------------------------------------------------------------------------
class TServerScreen : public TBaseScreen
{
private:
	//Объекты текста
	TTextTexture m_Text[2];

	//Идентификаторы событий для плавного перехода
	static const BYTE ID_SMOOTH_SS_QUIT = 1;
	static const BYTE ID_SMOOTH_SS_SELECT_SERVER = 2;
	static const BYTE ID_SMOOTH_SS_GO_SCREEN_MAIN = 3;

	//Идентификаторы кнопок
	static const int ID_SS_QUIT = 1;
	static const int ID_SS_ARROW_PREV = 2;
	static const int ID_SS_ARROW_NEXT = 3;
	static const int ID_SS_TIME_ZONE = 4;
	static const int ID_SS_FULL = 5;
	static const int ID_SS_CONNECTION = 6;
	static const int ID_SS_EARTH = 7;
	static const int ID_SS_SCROLLBAR_UP = 8;
	static const int ID_SS_SCROLLBAR_DOWN = 9;
	static const int ID_SS_SCROLLBAR = 10;
	static const int ID_SS_SCROLLBAR_BACKGROUND = 11;
	static const int ID_SS_SERVER_LIST = 20;

	//Получить высоту списка серверов
	int GetServerListHeight();

	//Пролистать список
	void ListingList(bool direction, int divizor = 1);

	//Последнее время изменения положения информации в скроллбоксе
	DWORD m_LastScrollChangeTime;

	//Смещение в пикселях отображаемой информации в скроллбоксе
	int m_PixelOffset;

	//Временная переменная для идентификации выбранного сервера
	int m_SelectionServerTempValue;

	//Обработка события после плавного затемнения экрана
	void ProcessSmoothAction(BYTE action = 0xFF);
public:
	TServerScreen();
	virtual ~TServerScreen();

	//Инициализация
	void Init();

	//Инициализация тултипа
	void InitPopupHelp();

	//Рисование экрана
	int Render(bool mode);
	
	//События
	void OnLeftMouseDown();
	void OnLeftMouseUp();
	void OnMouseWheel(MOUSE_WHEEL_STATE state);
	void OnKeyPress(WPARAM wparam, LPARAM lparam);
};
//---------------------------------------------------------------------------
extern TServerScreen *ServerScreen;
//---------------------------------------------------------------------------
#endif