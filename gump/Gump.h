/****************************************************************************
**
** Gump.h
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
#ifndef GumpH
#define GumpH
//---------------------------------------------------------------------------
//!Базовый класс гампов
class TGump : public TRenderObject
{
protected:
	//!Тип гампа
	GUMP_TYPE m_GumpType;

	//!ИД гампа
	DWORD m_ID;

	//!Координаты гампа в минимизированном состоянии
	int m_MinimizedX;
	int m_MinimizedY;

	//!Флаг невозможности закрыть гамп правой кнопкой мышки
	bool m_NoClose;

	//!Флаг невозможности перемещать гамп
	bool m_NoMove;

	//!Гамп минимизирован
	bool m_Minimized;

	//!Окно гампа отрисовано и инструкции запомнены в видеопамяти
	bool m_FrameCreated;

	//!Необходимость перерисовать гамп
	bool m_FrameRedraw;

	//!Это блокирующий гамп
	bool m_Blocked;

	//!Гамп заблокирован от случайных перемещений и закрытий
	bool m_LockMoving;

	/*!
	Вычислить состояние гампа и его элементов для отрисовки
	@return 
	*/
	virtual void CalculateGumpState();
public:
	TGump(GUMP_TYPE type, DWORD serial, short x, short y);
	virtual ~TGump();

	SETGET(GUMP_TYPE, GumpType);
	SETGET(DWORD, ID);
	SETGETEX(int, MinimizedX);
	SETGETEX(int, MinimizedY);
	SETGET(bool, NoMove);
	SETGET(bool, NoClose);
	SETGET(bool, Minimized);
	SETGET(bool, FrameCreated);
	SETGET(bool, FrameRedraw);
	SETGET(bool, Blocked);
	SETGET(bool, LockMoving);

	/*!
	Получить объект рендера текста гампа
	@return 
	*/
	virtual TTextRenderer *GetTextRenderer() { return NULL; }

	/*!
	Получить основные характеристики гампа
	@param [__out] serial Серийник
	@param [__out] graphic Графика
	@param [__out] color Цвет
	@param [__out] x Координата X
	@param [__out] y Координата Y
	@return 
	*/
	virtual void GetBaseProperties(__out DWORD &serial, __out WORD &graphic, __out WORD &color, __out WORD &x, __out WORD &y);

	/*!
	Изменить высоту гампа
	@return 
	*/
	virtual void ChangeHeight() {}

	/*!
	Перерисовать гамп
	@return 
	*/
	virtual void GenerateFrame() {}

	/*!
	Может ли гамп быть отображен
	@return 
	*/
	virtual bool CanBeDisplayed() {return true;}

	/*!
	Может ли гамп быть подвинут
	@return 
	*/
	bool CanBeMoved();

	/*!
	Отрисовать замочек гампа
	@return 
	*/
	void DrawLocker();

	/*!
	Отрисовать/выбрать объекты гампа
	@param [__in] mode true - рисование, false - выбор объектов
	@return 
	*/
	virtual int Draw(__in bool &mode) { return 0; }

	virtual void OnToolTip() {}

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
	virtual void OnMouseWheel(MOUSE_WHEEL_STATE &state) {}

	/*!
	Обработка нажатия клавиши
	@param [__in] wparam не подписанный параметр
	@param [__in] lparam не подписанный параметр
	@return
	*/
	virtual void OnCharPress(WPARAM &wparam, LPARAM &lparam) {}

	/*!
	Обработка нажатия клавиши
	@param [__in] wparam не подписанный параметр
	@param [__in] lparam не подписанный параметр
	@return
	*/
	virtual void OnKeyPress(WPARAM &wparam, LPARAM &lparam) {}

	virtual bool EntryPointerHere() {return false;}

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
	Отправить ответ гампа
	@param [__in] index Индекс кнопки
	@return 
	*/
	void SendGumpResponse(__in int index);

	/*!
	Отправить ответ меню
	@param [__in] index Индекс предмета меню
	@return 
	*/
	void SendMenuResponse(__in int index);

	/*!
	Отправить изменение состояния чекбокса трэйд окна
	@param [__in] code Состояние чекбокса
	@return 
	*/
	void SendTradingResponse(__in int code);

	/*!
	Отправить ответ ентри диалога
	@param [__in] mode Состояние
	@return 
	*/
	void SendTextEntryDialogResponse(__in bool mode);

	/*!
	Отправить запрос изменения имени
	@return 
	*/
	void SendRenameRequest();

	/*!
	Отправить запрос окна продажи
	@return 
	*/
	void SendSellList();

	/*!
	Отправить запрос окна покупки
	@return 
	*/
	void SendBuyList();

	/*!
	Отправить запрос тип окошка
	@param [__in] flag Флаг тип окна
	@return 
	*/
	void SendTipRequest(__in BYTE flag);
 };
//---------------------------------------------------------------------------
//!Ссылка на гамп, который в данный момент изменяет высоту
extern TGump *g_ResizedGump;
//---------------------------------------------------------------------------
#endif