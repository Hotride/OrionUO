/****************************************************************************
**
** BaseScreen.h
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
#ifndef BaseScreenH
#define BaseScreenH
//---------------------------------------------------------------------------
//Базовый класс стадий окна клиента
class TBaseScreen
{
protected:
	BYTE m_SmoothScreenAction;
public:
	TBaseScreen() :m_SmoothScreenAction(0) {}
	virtual ~TBaseScreen() {}

	//Инициализация
	virtual void Init() {}

	//Инициализация тултипа
	virtual void InitTooltip() {}

	//Отрисовка/выбор объектов
	virtual int Render(bool mode) {return 0;}

	//Вычисление смещения объектов в окне скроллбокса и скроллера
	virtual int CalculateScrollerAndTextPosition(int &currentLine, int &visibleLines, int &maxY, int currentY);
	virtual int CalculateScrollerY(int &currentLine, int &visibleLines, int &maxY);

	//Эвенты
	//Нажатие левой кнопки мыши
	virtual void OnLeftMouseDown() {}
	//Отпускание левой кнопки мыши
	virtual void OnLeftMouseUp() {}
	//Нажатие правой кнопки мыши
	virtual void OnRightMouseDown() {}
	//Отпускание правой кнопки мыши
	virtual void OnRightMouseUp() {}
	//Двойной клик левой кнопкой мыши
	virtual bool OnLeftMouseDoubleClick() {return false;}
	//Двойной клик правой кнопкой мыши
	virtual bool OnRightMouseDoubleClick() {return false;}
	//Обработка средней кнопки (колесика) мыши
	virtual void OnMouseWheel(MOUSE_WHEEL_STATE state) {}
	//Обработка нажатия клавишь
	virtual void OnCharPress(WPARAM wparam, LPARAM lparam) {}
	virtual void OnKeyPress(WPARAM wparam, LPARAM lparam) {}
};
//---------------------------------------------------------------------------
extern TBaseScreen *CurrentScreen; //Указатель на текущий экран
//---------------------------------------------------------------------------
#endif