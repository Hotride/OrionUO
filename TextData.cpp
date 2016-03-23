/****************************************************************************
**
** TextData.cpp
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

TTextContainer *SystemChat = NULL;
TJournal *Journal = NULL;
TTextRenderer *WorldTextRenderer = NULL;
TRenderTextObject *g_SelectedTextObject = NULL;;
//---------------------------------------------------------------------------
//-----------------------------TRenderTextObject----------------------------
//---------------------------------------------------------------------------
TRenderTextObject::TRenderTextObject()
: m_NextDraw(NULL), m_PrevDraw(NULL), m_DrawX(0), m_DrawY(0), m_Transparent(false)
{
}
//---------------------------------------------------------------------------
TRenderTextObject::~TRenderTextObject()
{
	Unlink();
}
//---------------------------------------------------------------------------
void TRenderTextObject::Unlink()
{
	if (m_NextDraw != NULL)
		m_NextDraw->m_PrevDraw = m_PrevDraw;

	if (m_PrevDraw != NULL)
		m_PrevDraw->m_NextDraw = m_NextDraw;
	
	m_NextDraw = NULL;
	m_PrevDraw = NULL;
}
//---------------------------------------------------------------------------
void TRenderTextObject::ToTop()
{
	TRenderTextObject *obj = this;

	while (obj != NULL)
	{
		if (obj->m_PrevDraw == NULL)
			break;

		obj = obj->m_PrevDraw;
	}

	TTextRenderer *tr = (TTextRenderer*)obj;
	tr->ToTop(this);
}
//---------------------------------------------------------------------------
//-----------------------------TRenderTextObject----------------------------
//---------------------------------------------------------------------------
TTextRenderer::TTextRenderer()
: m_Items(this)
{
}
//---------------------------------------------------------------------------
TTextRenderer::~TTextRenderer()
{
	m_Items = NULL;
}
//---------------------------------------------------------------------------
TRenderTextObject *TTextRenderer::Add(TRenderTextObject *obj)
{
	if (obj != NULL)
	{
		TRenderTextObject *item = m_Items;

		if (item != NULL)
		{
			if (item->m_NextDraw != NULL)
			{
				TRenderTextObject *next = item->m_NextDraw;

				item->m_NextDraw = obj;
				obj->m_PrevDraw = item;
				obj->m_NextDraw = next;
				next->m_PrevDraw = obj;
			}
			else
			{
				item->m_NextDraw = obj;
				obj->m_PrevDraw = item;
				obj->m_NextDraw = NULL;
			}
		}
	}

	return obj;
}
//---------------------------------------------------------------------------
void TTextRenderer::ToTop(TRenderTextObject *obj)
{
	obj->Unlink();

	TRenderTextObject *next = m_Items->m_NextDraw;

	m_Items->m_NextDraw = obj;
	obj->m_PrevDraw = m_Items;
	obj->m_NextDraw = next;

	if (next != NULL)
		next->m_PrevDraw = obj;
}
//---------------------------------------------------------------------------
bool TTextRenderer::InRect(TTextImageBounds &rect, TRenderWorldObject *rwo)
{
	bool result = false;

	for (std::deque<TTextImageBounds>::iterator it = m_TextRect.begin(); it != m_TextRect.end(); it++)
	{
		if ((*it).InRect(rect))
		{
			if (rwo == NULL || (rwo != NULL && rwo->TextCanBeTransparent((*it).Text)))
			{
				result = true;
				break;
			}
		}
	}

	return result;
}
//---------------------------------------------------------------------------
//--------------------------------TTextData---------------------------------
//---------------------------------------------------------------------------
TTextData::TTextData()
: TRenderTextObject(), m_Unicode(false), m_Type(TT_CLIENT), m_Font(0), m_Color(0),
m_Timer(0), m_Next(NULL), m_Prev(NULL)
{
	m_Text = "";
	m_UnicodeText = L"";

	m_Texture.Init();
}
//---------------------------------------------------------------------------
TTextData::TTextData(TTextData *obj)
: m_Unicode(obj->Unicode), m_Type(obj->Type), m_Font(obj->Font),
m_Color(obj->Color), m_Timer(obj->Timer), m_Next(NULL), m_Prev(NULL)
{
	m_Text = obj->GetText();
	m_UnicodeText = obj->GetUnicodeText();
	
	m_Texture.Init();
}
//---------------------------------------------------------------------------
TTextData::~TTextData()
{
	m_Texture.Clear();
}
//---------------------------------------------------------------------------
bool TTextData::CanBeDrawedInJournalGump()
{
	bool result = true;

	switch (m_Type)
	{
		case TT_SYSTEM:
			result = g_JournalShowSystem;
			break;
		case TT_OBJECT:
			result = g_JournalShowObjects;
			break;
		case TT_CLIENT:
			result = g_JournalShowClient;
			break;
		default:
			break;
	}

	return result;
}
//---------------------------------------------------------------------------
void TTextData::GenerateTexture(int MaxWidth, WORD flags, TEXT_ALIGN_TYPE align, BYTE cell, int font)
{
	if (m_Unicode)
	{
		if (font == -1)
			font = m_Font;

		FontManager->GenerateW((BYTE)font, m_Texture, m_UnicodeText.c_str(), m_Color, cell, MaxWidth, align, flags);
	}
	else
		FontManager->GenerateA((BYTE)m_Font, m_Texture, m_Text.c_str(), m_Color, MaxWidth, align, flags);

	if (!m_Texture.Empty())
		m_Timer += (m_Texture.Lines * (ConfigManager.SpeechDelay * 20));
}
//---------------------------------------------------------------------------
//-----------------------------TTextContainer-------------------------------
//---------------------------------------------------------------------------
TTextContainer::TTextContainer(int max_size)
: m_MaxSize(max_size), m_Size(0), m_Head(NULL), m_Top(NULL)
{
}
//---------------------------------------------------------------------------
TTextContainer::~TTextContainer()
{
	Clear();
}
//---------------------------------------------------------------------------
void TTextContainer::Clear()
{
	TTextData *obj = m_Head;

	while (obj != NULL)
	{
		TTextData *prev = obj->m_Prev;
		delete obj;
		obj = prev;
	}

	m_Top = NULL;
	m_Head = NULL;

	m_Size = 0;
}
//---------------------------------------------------------------------------
void TTextContainer::Add(TTextData *obj)
{
	if (m_Head != NULL)
	{
		m_Head->m_Next = obj;
		obj->m_Prev = m_Head;
		obj->m_Next = NULL;
		m_Head = obj;

		if (m_Size >= m_MaxSize)
		{
			TTextData *top = m_Top->m_Next;

			delete m_Top;

			m_Top = top;
			m_Top->m_Prev = NULL;
		}
		else
			m_Size++;
	}
	else
	{
		obj->m_Next = NULL;
		obj->m_Prev = NULL;
		m_Top = obj;
		m_Head = obj;

		m_Size = 1;
	}
}
//---------------------------------------------------------------------------
void TTextContainer::DrawSystemChat(int &x, int &y, int &height)
{
	DWORD ticks = GetTickCount();
	int offset = (y + height) - 41;

	TTextData *td = SystemChat->m_Head;

	while (td != NULL && offset >= y)
	{
		TTextTexture &tth = td->m_Texture;

		offset -= tth.Height;

		if (td->Timer >= ticks)
			tth.Draw(x, offset);

		td = td->m_Prev;
	}
}
//---------------------------------------------------------------------------
//--------------------------------TJournal----------------------------------
//---------------------------------------------------------------------------
TJournal::TJournal(int max_size)
: m_MaxSize(max_size), m_Size(0), m_Height(0), m_SystemHeight(0),
m_ObjectsHeight(0), m_ClientHeight(0), m_Head(NULL), m_Top(NULL),
m_SelectionIndex(0)
{
}
//---------------------------------------------------------------------------
TJournal::~TJournal()
{
	Clear();
}
//---------------------------------------------------------------------------
void TJournal::Clear()
{
	TTextData *obj = m_Head;

	while (obj != NULL)
	{
		TTextData *prev = obj->m_Prev;
		delete obj;
		obj = prev;
	}

	m_Top = NULL;
	m_Head = NULL;

	m_Size = 0;
	m_SelectionIndex = 0;

	m_Height = 0;
	m_SystemHeight = 0;
	m_ObjectsHeight = 0;
	m_ClientHeight = 0;
}
//---------------------------------------------------------------------------
void TJournal::Add(TTextData *obj)
{
	if (m_Head != NULL)
	{
		m_Head->m_Next = obj;
		obj->m_Prev = m_Head;
		obj->m_Next = NULL;
		m_Head = obj;

		if (m_Size >= m_MaxSize)
		{
			TTextData *top = m_Top->m_Next;

			m_Height -= m_Top->m_Texture.Height;

			if (m_Top->Type == TT_SYSTEM)
				m_SystemHeight -= m_Top->m_Texture.Height;
			else if (m_Top->Type == TT_OBJECT)
				m_ObjectsHeight -= m_Top->m_Texture.Height;
			else if (m_Top->Type == TT_CLIENT)
				m_ClientHeight -= m_Top->m_Texture.Height;

			delete m_Top;

			m_Top = top;
			m_Top->m_Prev = NULL;
		}
		else
			m_Size++;
		
		if (m_SelectionIndex && m_SelectionIndex < m_Size)
			m_SelectionIndex++;

		m_Height += obj->m_Texture.Height;

		if (obj->Type == TT_SYSTEM)
			m_SystemHeight += obj->m_Texture.Height;
		else if (obj->Type == TT_OBJECT)
			m_ObjectsHeight += obj->m_Texture.Height;
		else if (obj->Type == TT_CLIENT)
			m_ClientHeight += obj->m_Texture.Height;
	}
	else
	{
		obj->m_Next = NULL;
		obj->m_Prev = NULL;
		m_Top = obj;
		m_Head = obj;

		m_Size = 1;
		if (m_SelectionIndex)
			m_SelectionIndex = 0;

		m_Height = obj->m_Texture.Height;

		if (obj->Type == TT_SYSTEM)
			m_SystemHeight = obj->m_Texture.Height;
		else if (obj->Type == TT_OBJECT)
			m_ObjectsHeight = obj->m_Texture.Height;
		else if (obj->Type == TT_CLIENT)
			m_ClientHeight = obj->m_Texture.Height;
	}
}
//---------------------------------------------------------------------------