﻿/***********************************************************************************
**
** GUITextEntry.h
**
** Компонента для отображения поля для ввода текста
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUITEXTENTRY_H
#define GUITEXTENTRY_H
//----------------------------------------------------------------------------------
class CGUITextEntry : public CBaseGUI
{
public:
	//!Цвет текста в выбранном состоянии
	ushort ColorSelected = 0;

	//!Цвет текста в состоянии фокуса
	ushort ColorFocused = 0;

	//!Текст в юникоде
	bool Unicode = false;

	//!Шрифт
	uchar Font = 0;

	//!Ориентация текста
	TEXT_ALIGN_TYPE Align = TS_LEFT;

	//!Флаги текста
	ushort TextFlags = 0;

	//!Производить выбор объекта по серийнику может быть выбран фоном, хит-боксом и т.п.)
	bool CheckOnSerial = false;

	//!Только для чтения
	bool ReadOnly = false;

	//!Флаг фокуса
	bool Focused = false;

	//!Флаг использования глобального цвета
	bool UseGlobalColor = false;

	//!R-компонента глобального OGL цвета
	uchar GlobalColorR = 0;

	//!G-компонента глобального OGL цвета
	uchar GlobalColorG = 0;

	//!B-компонента глобального OGL цвета
	uchar GlobalColorB = 0;

	//!Альфа-канал глобального OGL цвета
	uchar GlobalColorA = 0;

	//!R-компонента глобального OGL цвета когда компонента выбрана
	uchar GlobalColorSelectedR = 0;

	//!G-компонента глобального OGL цвета когда компонента выбрана
	uchar GlobalColorSelectedG = 0;

	//!B-компонента глобального OGL цвета когда компонента выбрана
	uchar GlobalColorSelectedB = 0;

	//!Альфа-канал глобального OGL цвета когда компонента выбрана
	uchar GlobalColorSelectedA = 0;

	//!R-компонента глобального OGL цвета когда компонента находится в фокусе
	uchar GlobalColorFocusedR = 0;

	//!G-компонента глобального OGL цвета когда компонента находится в фокусе
	uchar GlobalColorFocusedG = 0;

	//!B-компонента глобального OGL цвета когда компонента находится в фокусе
	uchar GlobalColorFocusedB = 0;

	//!Альфа-канал глобального OGL цвета когда компонента находится в фокусе
	uchar GlobalColorFocusedA = 0;

	//!Смещение текста когда компонента находится в фокусе
	char FocusedOffsetY = 0;

	CGUITextEntry(int serial, ushort color, ushort colorSelected, ushort colorFocused, int x, int y, int maxWidth = 0, bool unicode = true, uchar font = 0, TEXT_ALIGN_TYPE align = TS_LEFT, ushort textFlags = 0, int maxLength = 0);
	virtual ~CGUITextEntry();

	//!Объект класса для ввода текста
	class CEntryText m_Entry;

	virtual void PrepareTextures();

	virtual bool EntryPointerHere();

	virtual WISP_GEOMETRY::CSize GetSize();

	//!Установить глобальный OGL цвет перед отрисовкой текста
	void SetGlobalColor(bool use, int color, int selected, int focused);

	//!Нажатие на компоненту
	void OnClick(CGump *gump, int x, int y);

	virtual void OnMouseEnter();
	virtual void OnMouseExit();

	virtual void Draw(bool checktrans = false);
	virtual bool Select();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
