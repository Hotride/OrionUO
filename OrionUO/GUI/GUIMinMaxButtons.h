/***********************************************************************************
**
** GUIMinMaxButtons.h
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
	SETGET(int, BaseScrollStep);
	SETGET(int, ScrollStep);
	SETGET(uint, LastScrollTime);
	SETGET(int, DefaultTextOffset);

private:
	CGLTextTexture m_Text;
	int m_ScrollMode;

public:
	CGUIMinMaxButtons(const uint &serial, const ushort &graphic, const int &x, const int &y, const int &minValue, const int &maxValue, const int &value);
	virtual ~CGUIMinMaxButtons();

	virtual bool IsPressedOuthit() { return true; }

	virtual WISP_GEOMETRY::CSize GetSize() { return WISP_GEOMETRY::CSize(36, 18); }

	virtual void Scroll(const uint &delay);

	virtual void OnClick();

	void UpdateText();

	void SetTextParameters(const bool &haveText, const SLIDER_TEXT_POSITION &textPosition, const uchar &font, const ushort &color, const bool &unicode, const int &textWidth = 0, const TEXT_ALIGN_TYPE &align = TS_LEFT, const ushort &textFlags = 0);

	virtual void PrepareTextures();

	virtual void Draw(const bool &checktrans = false);
	virtual bool Select();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
