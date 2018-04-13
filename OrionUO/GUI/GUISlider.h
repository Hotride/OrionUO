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
public:
	//!ИД картинки в выбранном состоянии
	ushort GraphicSelected = 0;

	//!ИД картинки в зажатом состоянии
	ushort GraphicPressed = 0;

	//!ИД картинки фона
	ushort BackgroundGraphic = 0;

	//!Сборный фон
	bool CompositeBackground = false;

	//!Вертикальный или горизонтальный
	bool Vertical = false;

	//!Длина
	int Length = 0;

	//!Смещение в процентах
	float Percents = 0.0f;

	//!Смещение
	int Offset = 0;

	//!Минимальное значение
	int MinValue = 0;

	//!Максимальное значение
	int MaxValue = 0;

	//!Текущее значение
	int Value = 0;

	//!Имеет текст
	bool HaveText = false;

	//!Позиция текста
	SLIDER_TEXT_POSITION TextPosition = STP_RIGHT;

	//!Шрифт текста
	uchar Font = 0;

	//!Цвет текста
	ushort TextColor = 0;

	//!Текст в юникоде
	bool Unicode = true;

	//!Ширина текста
	int TextWidth = 0;

	//!Ориентация текста
	TEXT_ALIGN_TYPE Align = TS_LEFT;

	//!Флаги текста
	ushort TextFlags = 0;

	//!Координата текста по оси X
	int TextX = 0;

	//!Координата текста по оси Y
	int TextY = 0;

	//!Шаг скроллера
	int ScrollStep = 15;

	//!Время последнего скроллинга
	uint LastScrollTime = 0;

	//!Стандартное смещение текста
	int DefaultTextOffset = 2;

private:
	//!Текстура текста
	CGLTextTexture Text{ CGLTextTexture() };

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
