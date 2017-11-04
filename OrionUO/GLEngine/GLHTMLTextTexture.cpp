// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GLHTMLTextTexture.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGLHTMLTextTexture::CGLHTMLTextTexture()
: CGLTextTexture()
{
}
//----------------------------------------------------------------------------------
CGLHTMLTextTexture::~CGLHTMLTextTexture()
{
	WISPFUN_DEBUG("c31_f1");
	m_WebLinkRect.clear();
}
//----------------------------------------------------------------------------------
ushort CGLHTMLTextTexture::WebLinkUnderMouse(int x, int y)
{
	WISPFUN_DEBUG("c31_f2");
	x = g_MouseManager.Position.X - x;
	y = g_MouseManager.Position.Y - y;

	for (std::deque<WEB_LINK_RECT>::iterator it = m_WebLinkRect.begin(); it != m_WebLinkRect.end(); ++it)
	{
		if (y >= (*it).StartY && y < (*it).StartY + (*it).EndY)
		{
			if (x >= (*it).StartX && x < (*it).StartX + (*it).EndX)
				return it->LinkID;
		}
	}

	return 0;
}
//----------------------------------------------------------------------------------