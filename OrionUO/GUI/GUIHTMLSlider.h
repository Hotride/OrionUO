/***********************************************************************************
**
** GUIHTMLSlider.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIHTMLSLIDER_H
#define GUIHTMLSLIDER_H
//----------------------------------------------------------------------------------
#include "GUISlider.h"
//----------------------------------------------------------------------------------
class CGUIHTMLSlider : public CGUISlider
{
private:
	class CGUIHTMLGump *m_HTMLGump;

public:
	CGUIHTMLSlider(class CGUIHTMLGump *htmlGump, const uint &serial, const ushort &graphic, const ushort &graphicSelected, const ushort &graphicPressed, const ushort &backgroundGraphic, const bool &compositeBackground, const bool &vertical, const int &x, const int &y, const int &lenght, const int &minValue, const int &maxValue, const int &value);
	virtual ~CGUIHTMLSlider();

	virtual void CalculateOffset();

	virtual bool IsControlHTML() { return true; }
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
