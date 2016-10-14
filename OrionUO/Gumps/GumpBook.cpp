/***********************************************************************************
**
** GumpBook.cpp
**
** Copyright (C) September 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GumpBook.h"
#include "../Network/Packets.h"
#include "../Managers/ConfigManager.h"
#include "../Managers/FontsManager.h"
#include "../Managers/MouseManager.h"
#include "../TextEngine/GameConsole.h"
#include "../OrionUO.h"
#include "../ClickObject.h"
#include "../PressedObject.h"
//----------------------------------------------------------------------------------
CGumpBook::CGumpBook(uint serial, short x, short y, short pageCount, bool writable, bool unicode)
: CGump(GT_BOOK, serial, x, y), m_PageCount(pageCount), m_Writable(writable),
m_Unicode(unicode)
{
	m_ChangedPage = new bool[pageCount + 1];
	m_Page = 0;
	m_Draw2Page = true;

	Add(new CGUIPage(-1));
	Add(new CGUIGumppic(0x01FE, 0, 0)); //Body
	m_PrevPage = (CGUIButton*)Add(new CGUIButton(ID_GB_BUTTON_PREV, 0x01FF, 0x01FF, 0x01FF, 0, 0));
	m_PrevPage->Visible = (m_Page != 0);
	m_NextPage = (CGUIButton*)Add(new CGUIButton(ID_GB_BUTTON_NEXT, 0x0200, 0x0200, 0x0200, 356, 0));
	m_NextPage->Visible = (m_Page + 2 <= m_PageCount);

	Add(new CGUIPage(0));
	CGUIText *text = (CGUIText*)Add(new CGUIText(0x0386, 78, 32));
	g_FontManager.UnusePartialHue = true;
	text->CreateTextureA(4, "TITLE");
	g_FontManager.UnusePartialHue = false;

	uchar entryFont = 0;

	if (!m_Unicode)
		entryFont = 4;

	m_EntryTitle = (CGUITextEntry*)Add(new CGUITextEntry(ID_GB_TEXT_AREA_TITLE, 0, 0, 0, 41, 65, 150, m_Unicode, entryFont));
	m_EntryTitle->ReadOnly = !m_Writable;
	m_EntryTitle->CheckOnSerial = true;

	Add(new CGUIHitBox(ID_GB_TEXT_AREA_TITLE, 41, 65, 150, (m_Unicode ? 22 : 44)));

	text = (CGUIText*)Add(new CGUIText(0x0386, 88, 134));
	g_FontManager.UnusePartialHue = true;
	text->CreateTextureA(4, "by");
	g_FontManager.UnusePartialHue = false;

	m_EntryAuthor = (CGUITextEntry*)Add(new CGUITextEntry(ID_GB_TEXT_AREA_AUTHOR, 0, 0, 0, 41, 160, 150, m_Unicode, entryFont));
	m_EntryAuthor->ReadOnly = !m_Writable;
	m_EntryAuthor->CheckOnSerial = true;

	Add(new CGUIHitBox(ID_GB_TEXT_AREA_AUTHOR, 41, 160, 150, 22));

	for (int i = 0; i <= m_PageCount; i++)
	{
		m_ChangedPage[i] = false;

		if (i)
		{
			Add(new CGUIPage(i));
			CGUITextEntry *entry = (CGUITextEntry*)Add(new CGUITextEntry(ID_GB_TEXT_AREA_PAGE_LEFT, 0x0012, 0x0012, 0x0012, 38, 34, 150, m_Unicode, entryFont));
			entry->m_Entry.Width = 166;
			entry->ReadOnly = !m_Writable;
			entry->CheckOnSerial = true;
			entry->MoveOnDrag = true;

			CGUIHitBox *box = (CGUIHitBox*)Add(new CGUIHitBox(ID_GB_TEXT_AREA_PAGE_LEFT, 38, 34, 160, 166));
			box->MoveOnDrag = true;

			text = (CGUIText*)Add(new CGUIText(0x0386, 112, 202));
			text->CreateTextureA(9, std::to_string(i));
		}

		i++;

		if (i <= m_PageCount)
		{
			Add(new CGUIPage(i));
			CGUITextEntry *entry = (CGUITextEntry*)Add(new CGUITextEntry(ID_GB_TEXT_AREA_PAGE_RIGHT, 0x0012, 0x0012, 0x0012, 224, 34, 150, m_Unicode, entryFont));
			entry->m_Entry.Width = 166;
			entry->ReadOnly = !m_Writable;
			entry->CheckOnSerial = true;
			entry->MoveOnDrag = true;

			CGUIHitBox *box = (CGUIHitBox*)Add(new CGUIHitBox(ID_GB_TEXT_AREA_PAGE_RIGHT, 224, 34, 160, 166));
			box->MoveOnDrag = true;

			text = (CGUIText*)Add(new CGUIText(0x0386, 299, 202));
			text->CreateTextureA(9, std::to_string(i));
		}
	}
}
//----------------------------------------------------------------------------------
CGumpBook::~CGumpBook()
{
	if (m_ChangedPage != NULL)
	{
		delete m_ChangedPage;
		m_ChangedPage = NULL;
	}
}
//----------------------------------------------------------------------------------
CGUITextEntry *CGumpBook::GetEntry(const int &page)
{
	int currentPage = -1;

	QFOR(item, m_Items, CBaseGUI*)
	{
		if (item->Type == GOT_PAGE)
			currentPage = ((CGUIPage*)item)->Index;
		else if (item->Type == GOT_TEXTENTRY && currentPage == page)
			return (CGUITextEntry*)item;
	}

	return NULL;
}
//----------------------------------------------------------------------------------
void CGumpBook::SetPageData(const int &page, const string &data)
{
	CGUITextEntry *entry = GetEntry(page);

	if (entry != NULL)
		entry->m_Entry.SetText(data);
}
//----------------------------------------------------------------------------------
void CGumpBook::SetPageData(const int &page, const wstring &data)
{
	CGUITextEntry *entry = GetEntry(page);

	if (entry != NULL)
		entry->m_Entry.SetText(data);
}
//----------------------------------------------------------------------------------
void CGumpBook::ChangePage(int newPage)
{
	IFOR(i, 0, 2)
	{
		if (m_Page + i >= m_PageCount)
			break;

		if (m_ChangedPage[m_Page + i])
		{
			m_ChangedPage[m_Page + i] = false;
			CPacketBookPageData(this, m_Page + i).Send();
		}
	}

	m_Page = newPage;

	m_PrevPage->Visible = (m_Page != 0);
	m_NextPage->Visible = (m_Page + 2 <= m_PageCount);

	g_Orion.PlaySoundEffect(0x0055);

	if (EntryPointerHere())
	{
		if (g_ConfigManager.ConsoleNeedEnter)
			g_EntryPointer = NULL;
		else
			g_EntryPointer = &g_GameConsole;
	}
}
//----------------------------------------------------------------------------------
void CGumpBook::DelayedClick(CRenderObject *obj)
{
	if (obj != NULL)
	{
		ChangePage(g_ClickObject.Page);
		m_WantRedraw = true;
	}
}
//----------------------------------------------------------------------------------
void CGumpBook::GUMP_BUTTON_EVENT_C
{
	if (!g_ClickObject.Enabled)
	{
		int newPage = -1;

		if (serial == ID_GB_BUTTON_PREV) //Prev
		{
			if (m_Page > 0) //Если не было запроса на клик
			{
				newPage = m_Page - 2;

				if (newPage < 0)
					newPage = 0;
			}
		}
		else if (serial == ID_GB_BUTTON_NEXT) //Next
		{
			if (m_Page < m_PageCount) //Если не было запроса на клик
			{
				newPage = m_Page + 2;

				if (newPage > m_PageCount)
					newPage = m_PageCount - 1;
			}
		}

		if (newPage > -1)
		{
			g_ClickObject.Init(g_PressedObject.LeftObject(), this);
			g_ClickObject.Timer = g_Ticks + g_MouseManager.DoubleClickDelay;
			g_ClickObject.Page = newPage;
		}
	}
}
//----------------------------------------------------------------------------------
bool CGumpBook::OnLeftMouseButtonDoubleClick()
{
	if (g_PressedObject.LeftSerial == ID_GB_BUTTON_PREV) //Prev
	{
		//Был нажат уголок "Назад", при даблклике устанавливаем 1 страницу
		ChangePage(0);

		//Перерисуем гамп
		m_WantRedraw = true;

		return true;
	}
	else if (g_PressedObject.LeftSerial == ID_GB_BUTTON_NEXT) //Next
	{
		//Был нажат уголок "Вперед", при даблклике устанавливаем последнюю страницу
		int page = m_PageCount;

		if (m_PageCount % 2)
			page--;

		ChangePage(page);

		//Перерисуем гамп
		m_WantRedraw = true;

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------------
void CGumpBook::InsertInContent(const WPARAM &wparam, const bool &isCharPress)
{
	int page = m_Page;

	if (page >= 0 && page < m_PageCount)
	{
		bool isSecondEntry = false;
		CGUITextEntry *entry = GetEntry(page);

		if (entry == NULL)
			return;

		if (g_EntryPointer != &entry->m_Entry)
		{
			entry = GetEntry(page + 1);

			if (entry != NULL && page < m_PageCount - 1 && g_EntryPointer == &entry->m_Entry)
			{
				isSecondEntry = true;
				page++;
			}
			else
				return;
		}

		if (isCharPress)
		{
			if (g_EntryPointer->Insert(wparam))
			{
				uchar font = 0;

				if (!m_Unicode)
					font = 4;

				if (g_EntryPointer->GetLinesCountW(font) > 8)
					g_EntryPointer->Remove(true);
				else
					m_ChangedPage[page] = true;

				m_WantRedraw = true;
			}
		}
		else
		{
			m_ChangedPage[page] = true;
			m_WantRedraw = true;
		}
		//page
	}
}
//----------------------------------------------------------------------------------
void CGumpBook::OnCharPress(const WPARAM &wParam, const LPARAM &lParam)
{
	if (!m_Writable)
		return;

	if (g_EntryPointer == &m_EntryTitle->m_Entry || g_EntryPointer == &m_EntryAuthor->m_Entry)
	{
		if (g_EntryPointer->Insert(wParam))
		{
			if (m_Unicode)
			{
				if (g_EntryPointer->GetLinesCountW(0) > 1)
					g_EntryPointer->Remove(true);
			}
			else
			{
				int count = 1;

				if (g_EntryPointer == &m_EntryTitle->m_Entry)
					count++;

				if (g_EntryPointer->GetLinesCountA(4) > count)
					g_EntryPointer->Remove(true);
			}

			m_WantRedraw = true;
		}
	}
	else
		InsertInContent(wParam);
}
//----------------------------------------------------------------------------------
void CGumpBook::OnKeyDown(const WPARAM &wParam, const LPARAM &lParam)
{
	if (!m_Writable)
		return;

	switch (wParam)
	{
		case VK_RETURN:
		{
			if (g_EntryPointer != &m_EntryTitle->m_Entry && g_EntryPointer != &m_EntryAuthor->m_Entry)
			{
				InsertInContent(L'\n');
				m_FrameCreated = false;
			}

			break;
		}
		case VK_HOME:
		case VK_END:
		case VK_LEFT:
		case VK_RIGHT:
		case VK_BACK:
		case VK_DELETE:
		{
			g_EntryPointer->OnKey(this, wParam);
			InsertInContent(wParam, false);
			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------------
