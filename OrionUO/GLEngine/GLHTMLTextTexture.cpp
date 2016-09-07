/***********************************************************************************
**
** GLHTMLTextTexture.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GLHTMLTextTexture.h"
#include "../Managers/MouseManager.h"
//----------------------------------------------------------------------------------
CGLHTMLTextTexture::CGLHTMLTextTexture()
: CGLTextTexture()
{
}
//----------------------------------------------------------------------------------
CGLHTMLTextTexture::~CGLHTMLTextTexture()
{
	m_WebLinkRect.clear();
}
//----------------------------------------------------------------------------------
ushort CGLHTMLTextTexture::WebLinkUnderMouse(int x, int y)
{
	x = g_MouseManager.Position.X - x;
	y = g_MouseManager.Position.Y - y;

	for (std::deque<WEB_LINK_RECT>::iterator it = m_WebLinkRect.begin(); it != m_WebLinkRect.end(); it++)
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