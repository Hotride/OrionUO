/***********************************************************************************
**
** GUISlider.h
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
	SETGET(ushort, GraphicSelected);
	SETGET(ushort, GraphicPressed);
	SETGET(ushort, BackgroundGraphic);
	SETGET(bool, CompositeBackground);
	SETGET(bool, Vertical);
	SETGET(int, Lenght);
	SETGET(int, Percents);
	SETGET(int, Offset);
	SETGET(int, MinValue);
	SETGET(int, MaxValue);
	SETGET(int, Value);
	SETGET(bool, HaveText);
	SETGET(SLIDER_TEXT_POSITION, TextPosition);
	SETGET(uchar, Font);
	SETGET(ushort, TextColor);
	SETGET(bool, Unicode);
	SETGET(int, TextWidth);
	SETGET(TEXT_ALIGN_TYPE, Align);
	SETGET(ushort, TextFlags);
	SETGET(int, TextX);
	SETGET(int, TextY);
	SETGET(int, ScrollStep);
	SETGET(uint, LastScrollTime);
	SETGET(int, DefaultTextOffset);

private:
	CGLTextTexture m_Text;

public:
	CGUISlider(const uint &serial, const ushort &graphic, const ushort &graphicSelected, const ushort &graphicPressed, const ushort &backgroundGraphic, const bool &compositeBackground, const bool &vertical, const int &x, const int &y, const int &lenght, const int &minValue, const int &maxValue, const int &value);
	virtual ~CGUISlider();

	virtual bool IsPressedOuthit() { return true; }

	virtual WISP_GEOMETRY::CSize GetSize();

	virtual void OnScroll(const bool &up, const uint &delay);

	virtual void OnClick(const int &x, const int &y);

	void UpdateText();

	virtual void CalculateOffset();

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
