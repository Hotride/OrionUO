/****************************************************************************
**
** GameScreen.h
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
#ifndef GameScreenH
#define GameScreenH
//---------------------------------------------------------------------------
//Структура с данными о параметрах и возможностях игрового окна
struct RENDER_VARIABLES_FOR_GAME_WINDOW
{
	//Положение окна
	int GameWindowPosX;
	int GameWindowPosY;

	//Габариты окна
	int GameWindowSizeX;
	int GameWindowSizeY;

	//Центр окна
	int GameWindowCenterX;
	int GameWindowCenterY;

	//Дистанции отображения
	int RealMinRangeX;
	int RealMaxRangeX;
	int RealMinRangeY;
	int RealMaxRangeY;

	//Индексы блоков для отображения
	int MinBlockX;
	int MinBlockY;
	int MaxBlockX;
	int MaxBlockY;

	//Координаты пикселей для отображения
	int MinPixelsX;
	int MaxPixelsX;
	int MinPixelsY;
	int MaxPixelsY;

	//Координаты игрока
	int PlayerX;
	int PlayerY;
	int PlayerZ;
};
//---------------------------------------------------------------------------
//Структура данных о источнике света
struct LIGHT_DATA
{
	//Индекс света
	BYTE ID;

	//Цвет света
	WORD Color;

	//Экранные координаты
	int DrawX;
	int DrawY;
};
//---------------------------------------------------------------------------
class TGameScreen : public TBaseScreen
{
private:
	//Структура с данными о параметрах и возможностях игрового окна
	RENDER_VARIABLES_FOR_GAME_WINDOW m_RenderBounds;

	//Возможность перемещения игрового окна
	bool m_GameWindowMoving;

	//Возможность изменения размера игрового окна
	bool m_GameWindowResizing;

	//Список источников света
	LIGHT_DATA m_Light[MAX_LIGHT_SOURCES];

	//Количество источников света
	int m_LightCount;

	//Использовать ли освещение при перерисовке текущего кадра
	bool m_UseLight;

	//Максимальное значение отображения по Z координате
	int m_MaxDrawZ;

	//Вычисление значений Z координаты для рендера
	int GetMaxDrawZ(bool &noDrawRoof, char &maxGroundZ);

	//Обработка сообщений мыши (стягивание статуса. драг-гампа и т.п.)
	void CheckMouseEvents(bool &charSelected);

	//Указатель на элемент текста
	TRenderTextObject *m_GameWindowText;

	//Вычисление параметров отображаемого текста
	void CalculateGameWindowText(bool &mode);

	//Отображение игрового окна
	void DrawGameWindow(bool &mode);

	//Отображение источников света
	void DrawGameWindowLight();

	//Отображение текста над объектами мира
	void DrawGameWindowText(bool &mode);
public:
	TGameScreen();
	virtual ~TGameScreen();

	//Обработка события после плавного затемнения экрана
	void ProcessSmoothAction(BYTE action = 0xFF);

	//Идентификаторы событий для плавного перехода
	static const BYTE ID_SMOOTH_GS_LOGOUT = 1;

	//Вычисление параметров игрового окна
	void CalculateGameWindow();

	//Восстановить размеры отображаемой области игрового окна
	void RestoreGameWindowPort() {g_GL.ViewPort(m_RenderBounds.GameWindowPosX, m_RenderBounds.GameWindowPosY, m_RenderBounds.GameWindowSizeX, m_RenderBounds.GameWindowSizeY);}

	//Инициализация
	void Init();

	//Инициализация тултипа
	void InitTooltip();

	//Функция добавления источника света
	void AddLight(TRenderWorldObject *rwo, TRenderWorldObject *lightObject, int x, int y);

	//Рисование экрана
	int Render(bool mode);

	SETGET(bool, UseLight);

	//События
	void OnLeftMouseDown();
	void OnLeftMouseUp();
	void OnRightMouseDown();
	void OnRightMouseUp();
	bool OnLeftMouseDoubleClick();
	bool OnRightMouseDoubleClick();
	void OnMouseWheel(MOUSE_WHEEL_STATE state);
	void OnCharPress(WPARAM wparam, LPARAM lparam);
	void OnKeyPress(WPARAM wparam, LPARAM lparam);
};
//---------------------------------------------------------------------------
extern TGameScreen *GameScreen;
//---------------------------------------------------------------------------
#endif