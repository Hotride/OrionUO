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
	//!Индекс действия, которое необходимо совершить после окончания плавного перехода затемненного состояния экрана
	BYTE m_SmoothScreenAction;

public:
	TBaseScreen() :m_SmoothScreenAction(0) {}
	virtual ~TBaseScreen() {}

	/*!
	Инициализация
	@return 
	*/
	virtual void Init() {}

	/*!
	Инициализация всплывающих подсказок
	@return 
	*/
	virtual void InitPopupHelp() {}

	/*!
	Отрисовка/выбор объектов
	@param [__in] mode true - отрисовка, false - выбор
	@return При выборе объектов - идентификатор выбранного объекта
	*/
	virtual int Render(__in bool mode) {return 0;}

	/*!
	Вычисление смещения объектов в окне скроллбокса
	@param [__out] currentLine Текущая строка
	@param [__in] visibleLines Количество видимых строк
	@param [__in] maxY Максимальное значение Y координаты
	@param [__in] currentY Текущее значение Y координаты
	@return Координата Y скроллера
	*/
	virtual int CalculateScrollerAndTextPosition(__out int &currentLine, __in int &visibleLines, __in int &maxY, __in int currentY);

	/*!
	Вычисление смещения скроллера
	@param [__in] currentLine Текущая строка
	@param [__in] visibleLines Количество видимых строк
	@param [__in] maxY Максимальное значение Y координаты
	@return Координата Y скроллера
	*/
	virtual int CalculateScrollerY(__in int &currentLine, __in int &visibleLines, __in int &maxY);

	/*!
	Создание плавного затемнения экрана
	@param [__in] action Идентификатор действия
	@return 
	*/
	virtual void CreateSmoothAction(__in BYTE action);

	/*!
	Обработка события после перехода
	@param [__in_opt] action Идентификатор действия
	@return 
	*/
	virtual void ProcessSmoothAction(__in_opt BYTE action = 0xFF) {}

	/*!
	Вычисление состояния перехода
	@return Индекс состояния
	*/
	virtual int DrawSmoothMonitor();

	/*!
	Наложение эффекта перехода
	@return 
	*/
	virtual void DrawSmoothMonitorEffect();



	/*!
	Нажатие левой кнопки мыши
	@return 
	*/
	virtual void OnLeftMouseDown() {}

	/*!
	Отпускание левой кнопки мыши
	@return 
	*/
	virtual void OnLeftMouseUp() {}

	/*!
	Нажатие правой кнопки мыши
	@return 
	*/
	virtual void OnRightMouseDown() {}

	/*!
	Отпускание правой кнопки мыши
	@return 
	*/
	virtual void OnRightMouseUp() {}

	/*!
	Двойной клик левой кнопкой мыши
	@return true при успешной обработке
	*/
	virtual bool OnLeftMouseDoubleClick() {return false;}

	/*!
	Двойной клик правой кнопкой мыши
	@return true при успешной обработке
	*/
	virtual bool OnRightMouseDoubleClick() {return false;}

	/*!
	Обработка средней кнопки (колесика) мыши
	@param [__in] state Состояние колесика
	@return 
	*/
	virtual void OnMouseWheel(__in MOUSE_WHEEL_STATE state) {}

	/*!
	Обработка нажатия клавиши
	@param [__in] wparam не подписанный параметр
	@param [__in] lparam не подписанный параметр
	@return 
	*/
	virtual void OnCharPress(__in WPARAM wparam, __in LPARAM lparam) {}

	/*!
	Обработка нажатия клавиши
	@param [__in] wparam не подписанный параметр
	@param [__in] lparam не подписанный параметр
	@return
	*/
	virtual void OnKeyPress(WPARAM wparam, LPARAM lparam) {}
};
//---------------------------------------------------------------------------
//!Указатель на текущий экран
extern TBaseScreen *CurrentScreen;
//---------------------------------------------------------------------------
#endif