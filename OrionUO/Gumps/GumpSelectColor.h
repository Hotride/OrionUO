/***********************************************************************************
**
** GumpSelectColor.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPSELECTCOLOR_H
#define GUMPSELECTCOLOR_H
//----------------------------------------------------------------------------------
#include "Gump.h"
//----------------------------------------------------------------------------------
class CGumpSelectColor : public CGump
{
protected:
	static const int ID_GSC_BUTTON_OKAY = 1;
	static const int ID_GSC_SLIDER = 2;
	static const int ID_GSC_COLORS = 10;

	SELECT_COLOR_GUMP_STATE m_State;
	uchar m_ColorRef;
	int m_SelectedIndex;

	CGUISlider *m_Slider;
	CGUIDataBox *m_DataBox;

public:
	CGumpSelectColor(uint serial, short x, short y, SELECT_COLOR_GUMP_STATE state);
	virtual ~CGumpSelectColor();

	virtual void UpdateContent();

	GUMP_BUTTON_EVENT_H;
	GUMP_SLIDER_CLICK_EVENT_H;
	GUMP_SLIDER_MOVE_EVENT_H;

	virtual void OnSelectColor(const ushort &color);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
