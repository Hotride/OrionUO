/***********************************************************************************
**
** GumpRacialAbilitiesBook.cpp
**
** Copyright (C) December 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GumpRacialAbilitiesBook.h"
#include "../Game objects/GamePlayer.h"
#include "../PressedObject.h"
#include "../ClickObject.h"
#include "../OrionUO.h"
#include "../Managers/MouseManager.h"
#include "../Managers/GumpManager.h"
#include "../Managers/ClilocManager.h"
#include "../Managers/PacketManager.h"
#include "../OrionWindow.h"
#include "../ToolTip.h"
//----------------------------------------------------------------------------------
CGumpRacialAbilitiesBook::CGumpRacialAbilitiesBook(const int &x, const int &y)
: CGump(GT_RACIAL_ABILITIES_BOOK, 0, x, y), m_DictionaryPagesCount(6), m_PagesCount(37),
m_AbilityCount(MAX_ABILITIES_COUNT), m_PrevPage(NULL), m_NextPage(NULL)
{
	m_Draw2Page = true;

	if (g_PacketManager.ClientVersion < CV_7000)
	{
		if (g_PacketManager.ClientVersion >= CV_500A)
			m_AbilityCount = 29;
		else
		{
			m_AbilityCount = 13;
			m_DictionaryPagesCount = 2;
		}
	}

	m_PagesCount = m_DictionaryPagesCount + (m_AbilityCount * 2);
}
//----------------------------------------------------------------------------------
CGumpRacialAbilitiesBook::~CGumpRacialAbilitiesBook()
{
}
//----------------------------------------------------------------------------------
void CGumpRacialAbilitiesBook::InitToolTip()
{
	if (m_Minimized)
	{
		g_ToolTip.Set(L"Double click to maximize book gump", g_SelectedObject.Object());
		return;
	}

	uint serial = g_SelectedObject.Serial;

	//if (m_Page >= m_DictionaryPagesCount && serial >= ID_GCB_ICON)
	//	g_ToolTip.Set(g_ClilocManager.Cliloc(g_Language)->GetW(1061693 + (serial - ID_GCB_ICON)), g_SelectedObject.Object(), 150);
}
//----------------------------------------------------------------------------
void CGumpRacialAbilitiesBook::UpdateContent()
{
	m_PrevPage = NULL;
	m_NextPage = NULL;

	Clear();

	Add(new CGUIPage(-1));

	if (m_Minimized)
	{
		Add(new CGUIGumppic(0x2B27, 0, 0));

		return;
	}

	Add(new CGUIGumppic(0x2B29, 0, 0));

	Add(new CGUIHitBox(ID_GCB_BUTTON_MINIMIZE, 6, 100, 16, 16, true));



	Add(new CGUIPage(-1));

	m_PrevPage = (CGUIButton*)Add(new CGUIButton(ID_GCB_BUTTON_PREV, 0x08BB, 0x08BB, 0x08BB, 50, 8));
	m_PrevPage->Visible = (m_Page != 0);
	m_NextPage = (CGUIButton*)Add(new CGUIButton(ID_GCB_BUTTON_NEXT, 0x08BC, 0x08BC, 0x08BC, 321, 8));
	m_NextPage->Visible = (m_Page + 2 < m_PagesCount);
}
//----------------------------------------------------------------------------
void CGumpRacialAbilitiesBook::GUMP_BUTTON_EVENT_C
{
	int newPage = -1;

	if (serial == ID_GCB_BUTTON_PREV)
	{
		if (m_Page > 0)
		{
			newPage = m_Page - 2;

			if (newPage < 0)
				newPage = 0;
		}
	}
	else if (serial == ID_GCB_BUTTON_NEXT)
	{
		if (m_Page < m_PagesCount)
		{
			newPage = m_Page + 2;

			if (newPage >= m_PagesCount)
				newPage = m_PagesCount - 1;
		}
	}
	else if (serial == ID_GCB_BUTTON_MINIMIZE)
	{
		m_Minimized = true;
		m_WantUpdateContent = true;
	}
	else if (serial == ID_GCB_LOCK_MOVING)
		m_LockMoving = !m_LockMoving;
	else if (serial >= ID_GCB_ICON)
	{
		if (m_Page < m_DictionaryPagesCount) //List of spells
			newPage = m_DictionaryPagesCount + ((serial - ID_GCB_ICON) * 2);
	}

	if (newPage > -1 && !g_ClickObject.Enabled)
	{
		if (newPage % 2)
			newPage--;

		g_ClickObject.Init(g_PressedObject.LeftObject(), this);
		g_ClickObject.Timer = g_Ticks + g_MouseManager.DoubleClickDelay;
		g_ClickObject.Page = newPage;
	}
}
//----------------------------------------------------------------------------
bool CGumpRacialAbilitiesBook::OnLeftMouseButtonDoubleClick()
{
	bool result = false;

	if (m_Minimized)
	{
		m_Minimized = false;
		m_WantUpdateContent = true;

		result = true;
	}
	else
	{
		if (g_PressedObject.LeftSerial == ID_GCB_BUTTON_PREV)
		{
			ChangePage(0);

			m_WantRedraw = true;

			result = true;
		}
		else if (g_PressedObject.LeftSerial == ID_GCB_BUTTON_NEXT)
		{
			int newPage = m_PagesCount - 1;

			if (newPage % 2)
				newPage--;

			ChangePage(newPage);

			m_WantRedraw = true;

			result = true;
		}
	}

	return result;
}
//----------------------------------------------------------------------------------
void CGumpRacialAbilitiesBook::DelayedClick(CRenderObject *obj)
{
	if (obj != NULL)
	{
		ChangePage(g_ClickObject.Page);
		m_WantRedraw = true;
	}
}
//----------------------------------------------------------------------------------
void CGumpRacialAbilitiesBook::ChangePage(int newPage)
{
	m_Page = newPage;

	m_PrevPage->Visible = (m_Page != 0);
	m_NextPage->Visible = (m_Page + 2 < m_PagesCount);

	g_Orion.PlaySoundEffect(0x0055);
}
//----------------------------------------------------------------------------------
