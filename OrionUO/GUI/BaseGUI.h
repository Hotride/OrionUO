/***********************************************************************************
**
** BaseGUI.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef BASEGUI_H
#define BASEGUI_H
//----------------------------------------------------------------------------------
#include "../Globals.h"
#include "../RenderObject.h"
#include "../GLEngine/GLEngine.h"
//----------------------------------------------------------------------------------
class CBaseGUI : public CRenderObject
{
	SETGET(GUMP_OBJECT_TYPE, Type);
	SETGET(bool, MoveOnDrag);
	SETGET(bool, PartialHue);
	SETGET(bool, CheckPolygone);
	SETGET(bool, Enabled);
	SETGET(bool, Visible);
	SETGET(bool, SelectOnly);

public:
	CBaseGUI(const GUMP_OBJECT_TYPE &type, const uint &serial, const ushort &graphic, const ushort &color, const int &x, const int &y);
	virtual ~CBaseGUI();

	/*
	GOT_SCOPE,				//!Рамка (лепится из 4 кусков)
	GOT_SLIDER,				//!Ползунок
	GOT_HTMLGUMP,			//!ХТМЛ гамп
	GOT_XFMHTMLGUMP,		//!ХТМЛ гамп из клилока (цветной там же)
	GOT_XFMHTMLTOKEN,		//
	*/

	virtual bool EntryPointerHere() { return false; }

	virtual void SetShaderMode() { }
	virtual ushort GetDrawGraphic() { return m_Graphic; }

	virtual void Draw(const bool &checktrans = false) {}
	virtual bool Select() { return false; }

	virtual WISP_GEOMETRY::CSize GetSize() { return WISP_GEOMETRY::CSize(); }

	virtual bool IsGUI() { return true; }

	virtual bool IsHTMLGump() { return false; }
	virtual bool IsControlHTML() { return false; }
	virtual bool IsPressedOuthit() { return false; }
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
