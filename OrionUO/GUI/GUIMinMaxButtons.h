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
	SETGET(int, MinValue);

	//!Максимальное значение
	SETGET(int, MaxValue);

	//!Текущее значение
	SETGET(int, Value);

	//!Имеет текст
	SETGET(bool, HaveText);

	//!Позиция текста
	SETGET(SLIDER_TEXT_POSITION, TextPosition);

	//!Шрифт текста
	SETGET(uchar, Font);

	//!Цвет текста
	SETGET(ushort, TextColor);

	//!Юникод текст
	SETGET(bool, Unicode);

	//!Ширина текста
	SETGET(int, TextWidth);

	//!Ориентация текста
	SETGET(TEXT_ALIGN_TYPE, Align);

	//!Флаги текста
	SETGET(ushort, TextFlags);

	//!Координата текста по оси X
	SETGET(int, TextX);

	//!Координата текста по оси Y
	SETGET(int, TextY);

	//!Базовый шаг смещения
	SETGET(int, BaseScrollStep);

	//!Текущий шаг смещения
	SETGET(int, ScrollStep);

	//!Время последней итерации прокрутки
	SETGET(uint, LastScrollTime);

	//!Стандартное смещение текста
	SETGET(int, DefaultTextOffset);

private:
	//!Текстура текста
	CGLTextTexture m_Text;

	//!Метод прокрутки (NoScroll/Max/Min)
	int m_ScrollMode;

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
