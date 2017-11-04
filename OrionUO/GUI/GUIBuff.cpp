// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIButton.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIBuff::CGUIBuff(const ushort &graphic, const uint &timer, const wstring &text)
: CGUIDrawObject(GOT_BUFF, 0, graphic, 0, 0, 0), m_Timer(timer), m_Text(text)
{
}
//----------------------------------------------------------------------------------
CGUIBuff::~CGUIBuff()
{
}
//----------------------------------------------------------------------------------
void CGUIBuff::Draw(const bool &checktrans)
{
	WISPFUN_DEBUG("c42_f1");
	CGLTexture *th = g_Orion.ExecuteGump(GetDrawGraphic());

	if (th != NULL)
	{
		glColor4ub(0xFF, 0xFF, 0xFF, m_Alpha);
		th->Draw(m_X, m_Y, checktrans);
	}
}
//----------------------------------------------------------------------------------
