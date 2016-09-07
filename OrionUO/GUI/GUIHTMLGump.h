/***********************************************************************************
**
** GUIHTMLGump.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIHTMLGUMP_H
#define GUIHTMLGUMP_H
//----------------------------------------------------------------------------------
#include "GUIPolygonal.h"
#include "GUIHTMLButton.h"
#include "GUIHTMLHitBox.h"
#include "GUIHTMLSlider.h"
#include "GUIHTMLResizepic.h"
#include "GUIScissor.h"
//----------------------------------------------------------------------------------
class CGUIHTMLGump : public CGUIPolygonal
{
	SETGET(bool, HaveBackground);
	SETGET(bool, HaveScrollbar);
	SETGET(WISP_GEOMETRY::CSize, DataSize);
	SETGET(WISP_GEOMETRY::CPoint2Di, DataOffset);
	SETGET(WISP_GEOMETRY::CPoint2Di, CurrentOffset);
	SETGET(WISP_GEOMETRY::CPoint2Di, AvailableOffset);

private:
	void CalculateDataSize(CBaseGUI *item, int &startX, int &startY, int &endX, int &endY);

public:
	CGUIHTMLGump(const uint &serial, const ushort &graphic, const int &x, const int &y, const int &width, const int &height, const bool &haveBackground, const bool &haveScrollbar);
	virtual ~CGUIHTMLGump();

	CGUIHTMLResizepic *m_Background;
	CGUIHTMLButton *m_ButtonUp;
	CGUIHTMLButton *m_ButtonDown;
	CGUIHTMLHitBox *m_HitBoxLeft;
	CGUIHTMLHitBox *m_HitBoxRight;
	CGUIHTMLSlider *m_Slider;
	CGUIScissor *m_Scissor;

	void Initalize(bool menu = false);

	void UpdateHeight(const int &height);

	void ResetDataOffset();
	void CalculateDataSize();

	virtual bool EntryPointerHere();

	virtual bool Select();

	virtual void Scroll(const bool &up, const uint &delay);

	virtual bool IsHTMLGump() { return true; }
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
