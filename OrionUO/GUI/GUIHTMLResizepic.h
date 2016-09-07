/***********************************************************************************
**
** GUIHTMLResizepic.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIHTMLRESIZEPIC_H
#define GUIHTMLRESIZEPIC_H
//----------------------------------------------------------------------------------
#include "GUIResizepic.h"
//----------------------------------------------------------------------------------
class CGUIHTMLResizepic : public CGUIResizepic
{
private:
	class CGUIHTMLGump *m_HTMLGump;

public:
	CGUIHTMLResizepic(class CGUIHTMLGump *htmlGump, const uint &serial, const ushort &graphic, const int &x, const int &y, const int &width, const int &height);
	virtual ~CGUIHTMLResizepic();

	void Scroll(const bool &up, const uint &delay);

	virtual bool IsControlHTML() { return true; }
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
