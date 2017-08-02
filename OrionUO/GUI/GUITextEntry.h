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
class CGUITextEntry : public CBaseGUI
{
	//!Цвет текста в выбранном состоянии
	SETGET(ushort, ColorSelected, 0);

	//!Цвет текста в состоянии фокуса
	SETGET(ushort, ColorFocused, 0);

	//!Текст в юникоде
	SETGET(bool, Unicode, false);

	//!Шрифт
	SETGET(uchar, Font, 0);

	//!Ориентация текста
	SETGET(TEXT_ALIGN_TYPE, Align, TS_LEFT);

	//!Флаги текста
	SETGET(ushort, TextFlags, 0);

	//!Производить выбор объекта по серийнику может быть выбран фоном, хит-боксом и т.п.)
	SETGET(bool, CheckOnSerial, false);

	//!Только для чтения
	SETGET(bool, ReadOnly, false);

	//!Флаг фокуса
	SETGET(bool, Focused, false);

	//!Флаг использования глобального цвета
	SETGET(bool, UseGlobalColor, false);

	//!R-компонента глобального OGL цвета
	SETGET(uchar, GlobalColorR, 0);

	//!G-компонента глобального OGL цвета
	SETGET(uchar, GlobalColorG, 0);

	//!B-компонента глобального OGL цвета
	SETGET(uchar, GlobalColorB, 0);

	//!Альфа-канал глобального OGL цвета
	SETGET(uchar, GlobalColorA, 0);

	//!R-компонента глобального OGL цвета когда компонента выбрана
	SETGET(uchar, GlobalColorSelectedR, 0);

	//!G-компонента глобального OGL цвета когда компонента выбрана
	SETGET(uchar, GlobalColorSelectedG, 0);

	//!B-компонента глобального OGL цвета когда компонента выбрана
	SETGET(uchar, GlobalColorSelectedB, 0);

	//!Альфа-канал глобального OGL цвета когда компонента выбрана
	SETGET(uchar, GlobalColorSelectedA, 0);

	//!R-компонента глобального OGL цвета когда компонента находится в фокусе
	SETGET(uchar, GlobalColorFocusedR, 0);

	//!G-компонента глобального OGL цвета когда компонента находится в фокусе
	SETGET(uchar, GlobalColorFocusedG, 0);

	//!B-компонента глобального OGL цвета когда компонента находится в фокусе
	SETGET(uchar, GlobalColorFocusedB, 0);

	//!Альфа-канал глобального OGL цвета когда компонента находится в фокусе
	SETGET(uchar, GlobalColorFocusedA, 0);

	//!Смещение текста когда компонента находится в фокусе
	SETGET(char, FocusedOffsetY, 0);

public:
	CGUITextEntry(const uint &serial, const ushort &color, const ushort &colorSelected, const ushort &colorFocused, const int &x, const int &y, const int &maxWidth = 0, const bool &unicode = true, const uchar &font = 0, const TEXT_ALIGN_TYPE &align = TS_LEFT, const ushort &textFlags = 0, const int &maxLength = 0);
	virtual ~CGUITextEntry();

	//!Объект класса для ввода текста
	class CEntryText m_Entry;

	virtual void PrepareTextures();

	virtual bool EntryPointerHere();

	virtual WISP_GEOMETRY::CSize GetSize();

	//!Установить глобальный OGL цвет перед отрисовкой текста
	void SetGlobalColor(const bool &use, const uint &color, const uint &selected, const uint &focused);

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
