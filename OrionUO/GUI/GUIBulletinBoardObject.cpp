// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIBulletinBoardObject.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIBulletinBoardObject::CGUIBulletinBoardObject(const uint &serial, const int &x, const int &y, const wstring &text)
: CBaseGUI(GOT_BB_OBJECT, serial, 0, 0, x, y), Text(text)
{
	WISPFUN_DEBUG("c43_f1");
	MoveOnDrag = true;

	if (g_PacketManager.GetClientVersion() >= CV_305D)
		g_FontManager.GenerateW(1, m_Texture, text, 0);
	else
		g_FontManager.GenerateA(9, m_Texture, ToString(text), 0x0386);
}
//----------------------------------------------------------------------------------
CGUIBulletinBoardObject::~CGUIBulletinBoardObject()
{
	WISPFUN_DEBUG("c43_f2");
	m_Texture.Clear();
}
//----------------------------------------------------------------------------------
void CGUIBulletinBoardObject::PrepareTextures()
{
	WISPFUN_DEBUG("c43_f5");
	g_Orion.ExecuteGump(0x1523);
}
//----------------------------------------------------------------------------------
void CGUIBulletinBoardObject::Draw(const bool &checktrans)
{
	WISPFUN_DEBUG("c43_f3");
	CGLTexture *th = g_Orion.ExecuteGump(0x1523);

	if (th != NULL)
		th->Draw(m_X, m_Y, checktrans);

	m_Texture.Draw(m_X + 23, m_Y + 1);
}
//----------------------------------------------------------------------------------
bool CGUIBulletinBoardObject::Select()
{
	WISPFUN_DEBUG("c43_f4");
	int x = g_MouseManager.Position.X - m_X;
	int y = g_MouseManager.Position.Y - m_Y;

	return (x >= 0 && y >= 0 && x < 230 && y < 18);
}
//----------------------------------------------------------------------------------
