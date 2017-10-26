/***********************************************************************************
**
** TextData.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CTextData::CTextData()
: CRenderTextObject()
{
	WISPFUN_DEBUG("c174_f1");
}
//---------------------------------------------------------------------------
CTextData::CTextData(CTextData *obj)
: m_Unicode(obj->Unicode), m_Type(obj->Type), m_Font(obj->Font), m_Timer(obj->Timer),
m_Alpha(obj->Alpha)
{
	WISPFUN_DEBUG("c174_f2");
	m_RealDrawX = obj->RealDrawX;
	m_RealDrawY = obj->RealDrawY;
	m_Color = obj->Color;
	m_Text = obj->GetText();
	m_UnicodeText = obj->GetUnicodeText();
}
//---------------------------------------------------------------------------
CTextData::~CTextData()
{
	WISPFUN_DEBUG("c174_f3");
	m_Texture.Clear();
	m_Owner = NULL;
}
//---------------------------------------------------------------------------
bool CTextData::CanBeDrawedInJournalGump()
{
	WISPFUN_DEBUG("c174_f4");
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
void CTextData::GenerateTexture(const int &maxWidth, const ushort &flags, const  TEXT_ALIGN_TYPE &align, const uchar &cell, int font)
{
	WISPFUN_DEBUG("c174_f5");
	if (m_Unicode)
	{
		if (font == -1)
			font = m_Font;

		g_FontManager.GenerateW((uchar)font, m_Texture, m_UnicodeText, m_Color, cell, maxWidth, align, flags);
	}
	else
		g_FontManager.GenerateA((uchar)m_Font, m_Texture, m_Text, m_Color, maxWidth, align, flags);

	if (!m_Texture.Empty())
	{
		if (g_ConfigManager.ScaleSpeechDelay)
			m_Timer += (((4000 * m_Texture.LinesCount) * g_ConfigManager.SpeechDelay) / 100);
		else
		{
			uint delay = ((__int64)((__int64)5497558140000 * g_ConfigManager.SpeechDelay) >> 32) >> 5;
			m_Timer += (uint)((delay >> 31) + delay);
		}
	}
}
//----------------------------------------------------------------------------------
