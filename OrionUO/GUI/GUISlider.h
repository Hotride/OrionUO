/***********************************************************************************
**
** GUISlider.h
**
** Компонента для отображения скроллера
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUISLIDER_H
#define GUISLIDER_H
//----------------------------------------------------------------------------------
#include "BaseGUI.h"
#include "../GLEngine/GLTextTexture.h"
//----------------------------------------------------------------------------------
class CGUISlider : public CBaseGUI
{
	//!ИД картинки в выбранном состоянии
	SETGET(ushort, GraphicSelected);

	//!ИД картинки в зажатом состоянии
	SETGET(ushort, GraphicPressed);

	//!ИД картинки фона
	SETGET(ushort, BackgroundGraphic);

	//!Сборный фон
	SETGET(bool, CompositeBackground);

	//!Вертикальный или горизонтальный
	SETGET(bool, Vertical);

	//!Длина
	SETGET(int, Lenght);

	//!Смещение в процентах
	SETGET(int, Percents);

	//!Смещение
	SETGET(int, Offset);

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

	//!Текст в юникоде
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

	//!Шаг скроллера
	SETGET(int, ScrollStep);

	//!Время последнего скроллинга
	SETGET(uint, LastScrollTime);

	//!Стандартное смещение текста
	SETGET(int, DefaultTextOffset);

private:
	//!Текстура текста
	CGLTextTexture m_Text;

public:
	CGUISlider(const uint &serial, const ushort &graphic, const ushort &graphicSelected, const ushort &graphicPressed, const ushort &backgroundGraphic, const bool &compositeBackground, const bool &vertical, const int &x, const int &y, const int &lenght, const int &minValue, const int &maxValue, const int &value);
	virtual ~CGUISlider();

	virtual bool IsPressedOuthit() { return true; }

	virtual WISP_GEOMETRY::CSize GetSize();

	//!Скроллинг
	virtual void OnScroll(const bool &up, const uint &delay);

	//!Нажатие на компоненту
	virtual void OnClick(const int &x, const int &y);

	//!Обновить текст
	void UpdateText();

	//!Пересчитать смещения
	virtual void CalculateOffset();

	//!Установить параметры текста
	void SetTextParameters(const bool &haveText, const SLIDER_TEXT_POSITION &textPosition, const uchar &font, const ushort &color, const bool &unicode, const int &textWidth = 0, const TEXT_ALIGN_TYPE &align = TS_LEFT, const ushort &textFlags = 0);

	virtual void PrepareTextures();

	virtual ushort GetDrawGraphic();

	virtual void Draw(const bool &checktrans = false);
	virtual bool Select();

	virtual void OnMouseEnter();
	virtual void OnMouseExit();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
