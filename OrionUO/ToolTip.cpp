/***********************************************************************************
**
** ToolTip.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "ToolTip.h"
#include "GLEngine/GLEngine.h"
#include "Managers/ConfigManager.h"
#include "Managers/ClilocManager.h"
#include "Managers/MouseManager.h"
#include "Managers/FontsManager.h"
#include "RenderObject.h"
//----------------------------------------------------------------------------------
CToolTip g_ToolTip;
//----------------------------------------------------------------------------------
CToolTip::CToolTip()
: m_Data(L""), m_ClilocID(0), m_MaxWidth(0), m_Position(), m_Use(false), m_Timer(0),
m_Object(NULL)
{
}
//----------------------------------------------------------------------------------
CToolTip::~CToolTip()
{
	Reset();
}
//----------------------------------------------------------------------------------
void CToolTip::Reset()
{
	Texture.Clear();
	m_Object = NULL;
}
//----------------------------------------------------------------------------------
void CToolTip::Set(wstring str, CRenderObject *object, int maxWidth, int x, int y)
{
	m_Use = !(m_Timer > g_Ticks);

	if (object == m_Object || object == NULL) //Уже забиндено или нет объекта для бинда
		return;

	m_Timer = g_Ticks + g_ConfigManager.ToolTipsDelay;
	m_Object = object;
	m_Use = false;
	m_Data = str;
	m_ClilocID = 0;
	m_MaxWidth = maxWidth;
	m_Position.X = x;
	m_Position.Y = y;

	Texture.Clear();

	g_FontManager.GenerateW((uchar)g_ConfigManager.ToolTipsTextFont, Texture, m_Data.c_str(), g_ConfigManager.ToolTipsTextColor, 5, m_MaxWidth, TS_CENTER, UOFONT_BLACK_BORDER);
}
//----------------------------------------------------------------------------------
void CToolTip::Set(uint clilocID, string str, CRenderObject *object, int maxWidth, int x, int y)
{
	Set(g_ClilocManager.Cliloc(g_Language)->GetW(clilocID, str), object, maxWidth, x, y);

	m_ClilocID = clilocID;
}
//----------------------------------------------------------------------------------
void CToolTip::Draw(const int &cursorWidth, const int &cursorHeight)
{
	if (!m_Use /*|| !g_ConfigManager.UseToolTips*/)
		return;

	if (!m_MaxWidth)
		m_MaxWidth = g_FontManager.GetWidthW((uchar)g_ConfigManager.ToolTipsTextFont, m_Data.c_str(), m_Data.length());

	if (Texture.Empty())
		g_FontManager.GenerateW((uchar)g_ConfigManager.ToolTipsTextFont, Texture, m_Data.c_str(), g_ConfigManager.ToolTipsTextColor, 30, m_MaxWidth, TS_CENTER, UOFONT_BLACK_BORDER);

	if (!Texture.Empty())
	{
		int x = m_Position.X;
		int y = m_Position.Y;

		if (!x)
			x = g_MouseManager.Position.X - (Texture.Width + 8);

		if (!y)
			y = g_MouseManager.Position.Y - (Texture.Height + 8);

		if (y < 0)
		{
			y = m_Position.Y;
			y = g_MouseManager.Position.Y + cursorHeight;
		}
		
		if (x < 0)
		{
			x = m_Position.X;
			x = g_MouseManager.Position.X + cursorWidth;
		}
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glColor4f(0.0f, 0.0f, 0.0f, 0.5f);

		g_GL.DrawPolygone(x, y, Texture.Width + 8, Texture.Height + 8);

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		
		glDisable(GL_BLEND);

		g_GL.Draw(Texture.Texture, x + 6, y + 4, Texture.Width, Texture.Height);
	}

	m_Use = false;
}
//----------------------------------------------------------------------------------
