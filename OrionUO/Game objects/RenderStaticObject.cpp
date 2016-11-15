/***********************************************************************************
**
** RenderStaticObject.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "RenderStaticObject.h"
#include "../OrionUO.h"
//----------------------------------------------------------------------------------
CRenderStaticObject::CRenderStaticObject(const RENDER_OBJECT_TYPE &renderType, const uint &serial, const ushort &graphic, const ushort &color, const short &x, const short &y, const char &z)
: CMapObject(renderType, serial, graphic, color, x, y, z), m_FoliageTransparentIndex(-1),
m_Vegetation(false)
{
	if (graphic >= g_Orion.StaticDataCount)
		m_TiledataPtr = &g_Orion.m_StaticData[(graphic - 0x4000) / 32].Tiles[(graphic - 0x4000) % 32];
	else
		m_TiledataPtr = &g_Orion.m_StaticData[graphic / 32].Tiles[graphic % 32];

	if (m_TiledataPtr->Height > 5)
		m_CanBeTransparent = 1;
	else if (IsRoof() || (IsSurface() && IsBackground()) || IsWall())
		m_CanBeTransparent = 1;
	else if (m_TiledataPtr->Height == 5 && IsSurface() && !IsBackground())
		m_CanBeTransparent = 1;
	else
		m_CanBeTransparent = 0;

	if (renderType == ROT_GAME_OBJECT)
	{
		m_TextControl = new CTextContainer(3);

		if (IsSurface() || (IsBackground() && IsUnknown2()))
			m_CanBeTransparent |= 0x10;
	}
	else
	{
		m_TextControl = new CTextContainer(1);

		if (IsSurface() || (IsBackground() && IsUnknown2()) || IsRoof())
			m_CanBeTransparent |= 0x10;
	}
}
//---------------------------------------------------------------------------
CRenderStaticObject::~CRenderStaticObject()
{
	if (m_TextControl != NULL)
	{
		delete m_TextControl;
		m_TextControl = NULL;
	}
}
//---------------------------------------------------------------------------
void CRenderStaticObject::AddText(CTextData *msg)
{
	if (m_TextControl != NULL)
	{
		m_TextControl->Add(msg);

		g_Orion.AddJournalMessage(msg, "You see: ");
	}
}
//---------------------------------------------------------------------------
int CRenderStaticObject::GetTextOffsetX(CTextData *text)
{
	return text->m_Texture.Width / 2;
}
//---------------------------------------------------------------------------
int CRenderStaticObject::GetTextOffsetY(CTextData *text)
{
	int offset = 0;

	CTextData *td = (CTextData*)m_TextControl->Last();

	while (td != NULL)
	{
		offset += td->m_Texture.Height;
		
		if (text == td)
			break;

		td = (CTextData*)td->m_Prev;
	}

	return offset;
}
//---------------------------------------------------------------------------
bool CRenderStaticObject::TextCanBeTransparent(CRenderTextObject *text)
{
	bool result = true;

	QFOR(item, m_TextControl->m_Items, CTextData*)
	{
		if (text == item)
		{
			result = false;
			break;
		}
	}

	return result;
}
//---------------------------------------------------------------------------
bool CRenderStaticObject::TranparentTest(int &playerZPlus5)
{
	bool result = true;

	if (m_Z <= playerZPlus5 - m_TiledataPtr->Height)
		result = false;
	else if (playerZPlus5 < m_Z && !(m_CanBeTransparent & 0xF))
		result = false;

	return result;
}
//----------------------------------------------------------------------------------
