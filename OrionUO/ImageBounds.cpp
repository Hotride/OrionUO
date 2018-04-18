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
CImageBounds::CImageBounds(int x, int y, int width, int height)
: X(x), Y(y), Width(width), Height(height)
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

	if (X < ib.X)
	{
		if (ib.X < X + Width)
			inRect = true;
	}
	else
	{
		if (X < ib.X + ib.Width)
			inRect = true;
	}

	if (inRect)
	{
		if (Y < ib.Y)
		{
			if (ib.Y < Y + Height)
				inRect = true;
			else
				inRect = false;
		}
		else
		{
			if (Y < ib.Y + ib.Height)
				inRect = true;
			else
				inRect = false;
		}
	}

	return inRect;
}
//----------------------------------------------------------------------------------
CTextImageBounds::CTextImageBounds(int x, int y, int width, int height, CRenderTextObject *text)
: CImageBounds(x, y, width, height), Text(text)
{
}
//----------------------------------------------------------------------------------
CTextImageBounds::CTextImageBounds(CTextData *text)
: CImageBounds(text->RealDrawX, text->RealDrawY, text->m_Texture.Width, text->m_Texture.Height), Text(text)
{
}
//----------------------------------------------------------------------------------
CTextImageBounds::~CTextImageBounds()
{
	Text = NULL;
}
//----------------------------------------------------------------------------------
