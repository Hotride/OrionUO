/***********************************************************************************
**
** GumpContainer.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPCONTAINER_H
#define GUMPCONTAINER_H
//----------------------------------------------------------------------------------
#include "Gump.h"
#include "../TextEngine/TextRenderer.h"
//----------------------------------------------------------------------------------
class CGumpContainer : public CGump
{
	SETGET(bool, IsGameBoard);

private:
	uint m_CorpseEyesTicks;
	uchar m_CorpseEyesOffset;

	static const uint ID_GC_LOCK_MOVING = 0xFFFFFFFE;
	static const uint ID_GC_MINIMIZE = 0xFFFFFFFF;
	
	CTextRenderer m_TextRenderer;

	CGUIGumppic *m_CorpseEyes;
	CGUIDataBox *m_DataBox;

protected:
	virtual void CalculateGumpState();

public:
	CGumpContainer(uint serial, uint id, short x, short y);
	virtual ~CGumpContainer();

	CGUIGumppic *m_BodyGump;
	
	CTextRenderer *GetTextRenderer() { return &m_TextRenderer; }

	virtual void PrepareTextures();

	virtual void PrepareContent();

	virtual void UpdateContent();

	virtual void InitToolTip();

	virtual void Draw();
	virtual CRenderObject *Select();

	GUMP_BUTTON_EVENT_H;

	virtual void OnLeftMouseButtonUp();
	virtual bool OnLeftMouseButtonDoubleClick();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
