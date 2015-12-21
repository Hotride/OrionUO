/****************************************************************************
**
** RenderObject.cpp
**
** Copyright (C) September 2015 Hotride
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
*****************************************************************************
*/
//---------------------------------------------------------------------------
#include "stdafx.h"

TRenderWorldObject *g_SelectedObject = NULL;
TRenderWorldObject *g_RenderList = NULL;
//---------------------------------------------------------------------------
//---------------------------------TRenderObject-----------------------------
//---------------------------------------------------------------------------
TRenderObject::TRenderObject(DWORD serial, WORD graphic, WORD color, short x, short y)
: TBaseQueueItem(), m_Graphic(graphic), m_Color(color), m_X(x), m_Y(y),
m_Serial(serial)
{
}
//---------------------------------------------------------------------------
TRenderObject::~TRenderObject()
{
}
//---------------------------------------------------------------------------
//-------------------------------TRenderWorldObject--------------------------
//---------------------------------------------------------------------------
TRenderWorldObject::TRenderWorldObject(RENDER_OBJECT_TYPE renderType, DWORD serial, WORD graphic, WORD color, short x, short y, char z)
: TRenderObject(serial, graphic, color, x, y), m_RenderType(renderType),
m_Z(z), m_NextXY(NULL), m_PrevXY(NULL), m_RenderQueueIndex(0)
//m_NextDraw(NULL), m_PrevDraw(NULL)
{
	if (renderType != ROT_LAND_OBJECT && renderType != ROT_EFFECT)
		m_TextControl = new TTextContainer(3);
	else
		m_TextControl = NULL;
}
//---------------------------------------------------------------------------
TRenderWorldObject::~TRenderWorldObject()
{
	if (m_TextControl != NULL)
	{
		delete m_TextControl;
		m_TextControl = NULL;
	}

	RemoveRender();
}
//---------------------------------------------------------------------------
void TRenderWorldObject::AddText(TTextData *msg)
{
	if (m_TextControl != NULL)
	{
		m_TextControl->Add(msg);

		UO->AddJournalMessage(msg, "You see: ");
	}
}
//---------------------------------------------------------------------------
int TRenderWorldObject::GetTextOffsetX(TTextData *text)
{
	return text->m_Texture.Width / 2;
}
//---------------------------------------------------------------------------
int TRenderWorldObject::GetTextOffsetY(TTextData *text)
{
	int offset = 0;

	TTextData *td = m_TextControl->m_Head;

	while (td != NULL)
	{
		offset += td->m_Texture.Height;
		
		if (text == td)
			break;

		td = td->m_Prev;
	}

	return offset;
}
//---------------------------------------------------------------------------
bool TRenderWorldObject::CanBeTransparent(TRenderTextObject *text)
{
	bool result = true;

	TTextData *td = m_TextControl->m_Head;

	while (td != NULL)
	{
		if (text == td)
		{
			result = false;
			break;
		}

		td = td->m_Prev;
	}

	return result;
}
//---------------------------------------------------------------------------
void TRenderWorldObject::RemoveRender()
{
	if (m_PrevXY != NULL)
		m_PrevXY->m_NextXY = m_NextXY;

	if (m_NextXY != NULL)
		m_NextXY->m_PrevXY = m_PrevXY;

	m_NextXY = NULL;
	m_PrevXY = NULL;

	/*if (m_PrevDraw != NULL)
		m_PrevDraw->m_NextDraw = m_NextDraw;
	else
		g_RenderList = m_NextDraw;

	if (m_NextDraw != NULL)
		m_NextDraw->m_PrevDraw = m_PrevDraw;

	m_NextDraw = NULL;
	m_PrevDraw = NULL;*/
}
//---------------------------------------------------------------------------
TLandObject *TRenderWorldObject::GetLand()
{
	TRenderWorldObject *land = this;

	while (land != NULL && !land->IsLandObject())
		land = land->m_NextXY;

	return (TLandObject*)land;
}
//---------------------------------------------------------------------------