/****************************************************************************
**
** CreateCharacterSelectionTownGump.h
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
#ifndef SelectTownScreenH
#define SelectTownScreenH
//---------------------------------------------------------------------------
class TSelectTownScreen : public TBaseScreen
{
private:
	//Смещение в пикселях отображаемой информации в скроллбоксе
	int m_PixelOffset;

	//Последнее время изменения положения информации в скроллбоксе
	DWORD m_LastScrollChangeTime;

	//Идентификаторы событий для плавного перехода
	static const BYTE ID_SMOOTH_STS_QUIT = 1;
	static const BYTE ID_SMOOTH_STS_GO_SCREEN_CHARACTER = 2;
	static const BYTE ID_SMOOTH_STS_GO_SCREEN_GAME_CONNECT = 3;

	//Пролистать список
	void ListingList(bool direction, int divizor = 1);

	//Идентификаторы кнопок
	static const int ID_STS_QUIT = 1;
	static const int ID_STS_ARROW_PREV = 2;
	static const int ID_STS_ARROW_NEXT = 3;
	static const int ID_STS_SCROLLBAR_UP = 4;
	static const int ID_STS_SCROLLBAR_DOWN = 5;
	static const int ID_STS_SCROLLBAR = 6;
	static const int ID_STS_SCROLLBAR_BACKGROUND = 7;
	static const int ID_STS_TOWN = 10;

	//Получить высоту скроллбокса
	int GetScrollBoxHeight();

	//Отобразить информацию о городе
	void DrawTownInfo(int x, int y);

	//Список точек для отображения кнопок городов
	static POINT m_TownButtonText[9];

public:
	TSelectTownScreen();
	~TSelectTownScreen();

	//Указатель на выбранный город
	TCityItem *m_City;

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
	void OnMouseWheel(MOUSE_WHEEL_STATE state);
};
//---------------------------------------------------------------------------
extern TSelectTownScreen *SelectTownScreen;
//---------------------------------------------------------------------------
#endif