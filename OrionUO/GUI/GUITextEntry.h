/***********************************************************************************
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
#include "BaseGUI.h"
#include "../GLEngine/GLTextTexture.h"
#include "../TextEngine/EntryText.h"
//----------------------------------------------------------------------------------
class CGUITextEntry : public CBaseGUI
{
	//!Цвет текста в выбранном состоянии
	SETGET(ushort, ColorSelected);

	//!Цвет текста в состоянии фокуса
	SETGET(ushort, ColorFocused);

	//!Текст в юникоде
	SETGET(bool, Unicode);

	//!Шрифт
	SETGET(uchar, Font);

	//!Ориентация текста
	SETGET(TEXT_ALIGN_TYPE, Align);

	//!Флаги текста
	SETGET(ushort, TextFlags);

	//!Производить выбор объекта по серийнику может быть выбран фоном, хит-боксом и т.п.)
	SETGET(bool, CheckOnSerial);

	//!Только для чтения
	SETGET(bool, ReadOnly);

	//!Флаг фокуса
	SETGET(bool, Focused);

	//!Флаг использования глобального цвета
	SETGET(bool, UseGlobalColor);

	//!R-компонента глобального OGL цвета когда компонента выбрана
	SETGET(uchar, GlobalColorSelectedR);

	//!G-компонента глобального OGL цвета когда компонента выбрана
	SETGET(uchar, GlobalColorSelectedG);

	//!B-компонента глобального OGL цвета когда компонента выбрана
	SETGET(uchar, GlobalColorSelectedB);

	//!Альфа-канал глобального OGL цвета когда компонента выбрана
	SETGET(uchar, GlobalColorSelectedA);

	//!R-компонента глобального OGL цвета когда компонента находится в фокусе
	SETGET(uchar, GlobalColorFocusedR);

	//!G-компонента глобального OGL цвета когда компонента находится в фокусе
	SETGET(uchar, GlobalColorFocusedG);

	//!B-компонента глобального OGL цвета когда компонента находится в фокусе
	SETGET(uchar, GlobalColorFocusedB);

	//!Альфа-канал глобального OGL цвета когда компонента находится в фокусе
	SETGET(uchar, GlobalColorFocusedA);

	//!Смещение текста когда компонента находится в фокусе
	SETGET(char, FocusedOffsetY);

public:
	CGUITextEntry(const uint &serial, const ushort &color, const ushort &colorSelected, const ushort &colorFocused, const int &x, const int &y, const int &maxWidth = 0, const bool &unicode = true, const uchar &font = 0, const TEXT_ALIGN_TYPE &align = TS_LEFT, const ushort &textFlags = 0, const int &maxLength = 0);
	virtual ~CGUITextEntry();

	//!Объект класса для ввода текста
	class CEntryText m_Entry;

	virtual bool EntryPointerHere();

	virtual WISP_GEOMETRY::CSize GetSize();

	//!Установить глобальный OGL цвет перед отрисовкой текста
	void SetGlobalColor(const bool &use, const uint &selected = 0, const uint &focused = 0);

	//!Нажатие на компоненту
	void OnClick(CGump *gump, const int &x, const int &y);

	virtual void OnMouseEnter();
	virtual void OnMouseExit();

	virtual void Draw(const bool &checktrans = false);
	virtual bool Select();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
