/***********************************************************************************
**
** GUIMinMaxButtons.h
**
** Компонента для отображения блока кнопок MinMax
**
** Copyright (C) September 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIMINMAXBUTTONS_H
#define GUIMINMAXBUTTONS_H
//----------------------------------------------------------------------------------
#include "BaseGUI.h"
#include "../GLEngine/GLTextTexture.h"
//----------------------------------------------------------------------------------
class CGUIMinMaxButtons : public CBaseGUI
{
	//!Минимальное значение
	SETGET(int, MinValue, 0);

	//!Максимальное значение
	SETGET(int, MaxValue, 0);

	//!Текущее значение
	SETGET(int, Value, 0);

	//!Имеет текст
	SETGET(bool, HaveText, false);

	//!Позиция текста
	SETGET(SLIDER_TEXT_POSITION, TextPosition, STP_RIGHT);

	//!Шрифт текста
	SETGET(uchar, Font, 0);

	//!Цвет текста
	SETGET(ushort, TextColor, 0);

	//!Юникод текст
	SETGET(bool, Unicode, true);

	//!Ширина текста
	SETGET(int, TextWidth, 0);

	//!Ориентация текста
	SETGET(TEXT_ALIGN_TYPE, Align, TS_LEFT);

	//!Флаги текста
	SETGET(ushort, TextFlags, 0);

	//!Координата текста по оси X
	SETGET(int, TextX, 0);

	//!Координата текста по оси Y
	SETGET(int, TextY, 0);

	//!Базовый шаг смещения
	SETGET(int, BaseScrollStep, 1);

	//!Текущий шаг смещения
	SETGET(int, ScrollStep, 1);

	//!Время последней итерации прокрутки
	SETGET(uint, LastScrollTime, 0);

	//!Стандартное смещение текста
	SETGET(int, DefaultTextOffset, 2);

private:
	//!Текстура текста
	CGLTextTexture m_Text{ CGLTextTexture() };

	//!Метод прокрутки (NoScroll/Max/Min)
	int m_ScrollMode{ 0 };

public:
	CGUIMinMaxButtons(const uint &serial, const ushort &graphic, const int &x, const int &y, const int &minValue, const int &maxValue, const int &value);
	virtual ~CGUIMinMaxButtons();

	virtual bool IsPressedOuthit() { return true; }

	virtual WISP_GEOMETRY::CSize GetSize() { return WISP_GEOMETRY::CSize(36, 18); }

	//Прокрутка
	virtual void Scroll(const uint &delay);

	//Нажатие
	virtual void OnClick();

	//!Обновить текст
	void UpdateText();

	//!Установить параметры текста
	void SetTextParameters(const bool &haveText, const SLIDER_TEXT_POSITION &textPosition, const uchar &font, const ushort &color, const bool &unicode, const int &textWidth = 0, const TEXT_ALIGN_TYPE &align = TS_LEFT, const ushort &textFlags = 0);

	virtual void PrepareTextures();

	virtual void Draw(const bool &checktrans = false);
	virtual bool Select();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
