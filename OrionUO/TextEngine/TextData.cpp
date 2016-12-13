/***********************************************************************************
**
** TextData.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "TextData.h"
#include "../Managers/FontsManager.h"
#include "../Managers/ConfigManager.h"
//----------------------------------------------------------------------------------
CTextData::CTextData()
: CRenderTextObject(), m_Unicode(false), m_Type(TT_CLIENT), m_Font(0), m_Timer(0),
m_DrawX(0), m_DrawY(0), m_Alpha(0xFF)
{
	m_Text = "";
	m_UnicodeText = L"";
}
//---------------------------------------------------------------------------
CTextData::CTextData(CTextData *obj)
: m_Unicode(obj->Unicode), m_Type(obj->Type), m_Font(obj->Font), m_Timer(obj->Timer),
m_DrawX(obj->DrawX), m_DrawY(obj->DrawY), m_Alpha(obj->Alpha)
{
	m_Color = obj->Color;
	m_Text = obj->GetText();
	m_UnicodeText = obj->GetUnicodeText();
}
//---------------------------------------------------------------------------
CTextData::~CTextData()
{
	m_Texture.Clear();
}
//---------------------------------------------------------------------------
bool CTextData::CanBeDrawedInJournalGump()
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
void CTextData::GenerateTexture(const int &maxWidth, const ushort &flags, const  TEXT_ALIGN_TYPE &align, const uchar &cell, int font)
{
	if (m_Unicode)
	{
		if (font == -1)
			font = m_Font;

		g_FontManager.GenerateW((uchar)font, m_Texture, m_UnicodeText.c_str(), m_Color, cell, maxWidth, align, flags);
	}
	else
		g_FontManager.GenerateA((uchar)m_Font, m_Texture, m_Text.c_str(), m_Color, maxWidth, align, flags);

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
