// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** ImageBounds.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CImageBounds g_PlayerRect(0, 0, 0, 0);
//----------------------------------------------------------------------------------
CImageBounds::CImageBounds(const int &x, const int &y, const int &width, const int &height)
: m_X(x), m_Y(y), m_Width(width), m_Height(height)
{
}
//----------------------------------------------------------------------------------
CImageBounds::~CImageBounds()
{
}
//----------------------------------------------------------------------------------
bool CImageBounds::InRect(const CImageBounds &ib)
{
	WISPFUN_DEBUG("c188_f1");
	bool inRect = false;

	if (m_X < ib.X)
	{
		if (ib.X < m_X + m_Width)
			inRect = true;
	}
	else
	{
		if (m_X < ib.X + ib.Width)
			inRect = true;
	}

	if (inRect)
	{
		if (m_Y < ib.Y)
		{
			if (ib.Y < m_Y + m_Height)
				inRect = true;
			else
				inRect = false;
		}
		else
		{
			if (m_Y < ib.Y + ib.Height)
				inRect = true;
			else
				inRect = false;
		}
	}

	return inRect;
}
//----------------------------------------------------------------------------------
CTextImageBounds::CTextImageBounds(const int &x, const int &y, const int &width, const int &height, CRenderTextObject *text)
: CImageBounds(x, y, width, height), m_Text(text)
{
}
//----------------------------------------------------------------------------------
CTextImageBounds::CTextImageBounds(CTextData *text)
: CImageBounds(text->RealDrawX, text->RealDrawY, text->m_Texture.Width, text->m_Texture.Height), m_Text(text)
{
}
//----------------------------------------------------------------------------------
CTextImageBounds::~CTextImageBounds()
{
	m_Text = NULL;
}
//----------------------------------------------------------------------------------
