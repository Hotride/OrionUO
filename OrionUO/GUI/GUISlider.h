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
class CGUISlider : public CBaseGUI
{
	//!ИД картинки в выбранном состоянии
	SETGET(ushort, GraphicSelected, 0);

	//!ИД картинки в зажатом состоянии
	SETGET(ushort, GraphicPressed, 0);

	//!ИД картинки фона
	SETGET(ushort, BackgroundGraphic, 0);

	//!Сборный фон
	SETGET(bool, CompositeBackground, false);

	//!Вертикальный или горизонтальный
	SETGET(bool, Vertical, false);

	//!Длина
	SETGET(int, Lenght, 0);

	//!Смещение в процентах
	SETGET(float, Percents, 0.0f);

	//!Смещение
	SETGET(int, Offset, 0);

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

	//!Текст в юникоде
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

	//!Шаг скроллера
	SETGET(int, ScrollStep, 15);

	//!Время последнего скроллинга
	SETGET(uint, LastScrollTime, 0);

	//!Стандартное смещение текста
	SETGET(int, DefaultTextOffset, 2);

private:
	//!Текстура текста
	CGLTextTexture m_Text{ CGLTextTexture() };

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
