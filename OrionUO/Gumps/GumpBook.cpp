// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GumpBook.cpp
**
** Copyright (C) September 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGumpBook::CGumpBook(uint serial, short x, short y, short pageCount, bool writable, bool unicode)
: CGump(GT_BOOK, serial, x, y), PageCount(pageCount), Writable(writable),
Unicode(unicode)
{
	WISPFUN_DEBUG("c87_f1");
	m_ChangedPage = new bool[pageCount + 1];
	m_PageDataReceived = new bool[pageCount + 1];
	Page = 0;
	Draw2Page = true;

	Add(new CGUIPage(-1));
	Add(new CGUIGumppic(0x01FE, 0, 0)); //Body
	m_PrevPage = (CGUIButton*)Add(new CGUIButton(ID_GB_BUTTON_PREV, 0x01FF, 0x01FF, 0x01FF, 0, 0));
	m_PrevPage->Visible = (Page != 0);
	m_NextPage = (CGUIButton*)Add(new CGUIButton(ID_GB_BUTTON_NEXT, 0x0200, 0x0200, 0x0200, 356, 0));
	m_NextPage->Visible = (Page + 2 <= PageCount);

	Add(new CGUIPage(0));
	CGUIText *text = (CGUIText*)Add(new CGUIText(0x0386, 78, 32));
	g_FontManager.UnusePartialHue = true;
	text->CreateTextureA(9, "Title");
	g_FontManager.UnusePartialHue = false;

	uchar entryFont = 1;

	if (!Unicode)
		entryFont = 4;

	Add(new CGUIHitBox(ID_GB_TEXT_AREA_TITLE, 41, 65, 150, (Unicode ? 22 : 44)));

	m_EntryTitle = (CGUITextEntry*)Add(new CGUITextEntry(ID_GB_TEXT_AREA_TITLE, 0, 0, 0, 41, 65, 150, Unicode, entryFont));
	m_EntryTitle->ReadOnly = !Writable;
	m_EntryTitle->CheckOnSerial = true;

	text = (CGUIText*)Add(new CGUIText(0x0386, 88, 134));
	g_FontManager.UnusePartialHue = true;
	text->CreateTextureA(9, "by");
	g_FontManager.UnusePartialHue = false;

	Add(new CGUIHitBox(ID_GB_TEXT_AREA_AUTHOR, 41, 160, 150, 22));

	m_EntryAuthor = (CGUITextEntry*)Add(new CGUITextEntry(ID_GB_TEXT_AREA_AUTHOR, 0, 0, 0, 41, 160, 150, Unicode, entryFont));
	m_EntryAuthor->ReadOnly = !Writable;
	m_EntryAuthor->CheckOnSerial = true;

	ushort textColor = 0x0012;

	if (Unicode)
		textColor = 0x001B;

	for (int i = 0; i <= PageCount; i++)
	{
		m_ChangedPage[i] = false;
		m_PageDataReceived[i] = false;

		if (i)
		{
			Add(new CGUIPage(i));
			CGUIHitBox *box = (CGUIHitBox*)Add(new CGUIHitBox(ID_GB_TEXT_AREA_PAGE_LEFT, 38, 34, 160, 166));
			box->MoveOnDrag = true;

			CGUITextEntry *entry = (CGUITextEntry*)Add(new CGUITextEntry(ID_GB_TEXT_AREA_PAGE_LEFT, textColor, textColor, textColor, 38, 34, 0, Unicode, entryFont));
			entry->m_Entry.Width = 166;
			entry->ReadOnly = !Writable;
			entry->CheckOnSerial = true;
			entry->MoveOnDrag = true;

			text = (CGUIText*)Add(new CGUIText(0x0386, 112, 202));
			text->CreateTextureA(9, std::to_string(i));
		}

		i++;

		if (i <= PageCount)
		{
			m_ChangedPage[i] = false;
			m_PageDataReceived[i] = false;

			Add(new CGUIPage(i));
			CGUIHitBox *box = (CGUIHitBox*)Add(new CGUIHitBox(ID_GB_TEXT_AREA_PAGE_RIGHT, 224, 34, 160, 166));
			box->MoveOnDrag = true;

			CGUITextEntry *entry = (CGUITextEntry*)Add(new CGUITextEntry(ID_GB_TEXT_AREA_PAGE_RIGHT, textColor, textColor, textColor, 224, 34, 0, Unicode, entryFont));
			entry->m_Entry.Width = 166;
			entry->ReadOnly = !Writable;
			entry->CheckOnSerial = true;
			entry->MoveOnDrag = true;

			text = (CGUIText*)Add(new CGUIText(0x0386, 299, 202));
			text->CreateTextureA(9, std::to_string(i));
		}
	}
}
//----------------------------------------------------------------------------------
CGumpBook::~CGumpBook()
{
	WISPFUN_DEBUG("c87_f2");

	RELEASE_POINTER(m_ChangedPage);
	RELEASE_POINTER(m_PageDataReceived);
}
//----------------------------------------------------------------------------------
void CGumpBook::PrepareContent()
{
	WISPFUN_DEBUG("c87_f2.1");
	/*if (!m_PageDataReceived[Page])
	{
		CPacketBookPageDataRequest(Serial, Page).Send();
		m_PageDataReceived[Page] = true;
	}

	if (Page + 1 <= PageCount && !m_PageDataReceived[Page + 1])
	{
		CPacketBookPageDataRequest(Serial, Page + 1).Send();
		m_PageDataReceived[Page + 1] = true;
	}*/
}
//----------------------------------------------------------------------------------
CGUITextEntry *CGumpBook::GetEntry(int page)
{
	WISPFUN_DEBUG("c87_f3");
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
void CGumpBook::SetPageData(int page, const wstring &data)
{
	WISPFUN_DEBUG("c87_f4");
	CGUITextEntry *entry = GetEntry(page);
	m_PageDataReceived[page] = true;

	if (entry != NULL)
		entry->m_Entry.SetText(data);
}
//----------------------------------------------------------------------------------
void CGumpBook::ChangePage(int newPage, bool playSound)
 {
	WISPFUN_DEBUG("c87_f5");
	IFOR(i, 0, 2)
	{
		if (Page + i >= PageCount)
			break;

		if (m_ChangedPage[Page + i])
		{
			m_ChangedPage[Page + i] = false;
			CPacketBookPageData(this, Page + (int)i).Send();
		}
	}

	if (playSound && Page != newPage)
		g_Orion.PlaySoundEffect(0x0055);

	Page = newPage;

	m_PrevPage->Visible = (Page != 0);
	m_NextPage->Visible = (Page + 2 <= PageCount);


	if (EntryPointerHere())
	{
		if (g_ConfigManager.GetConsoleNeedEnter())
			g_EntryPointer = NULL;
		else
			g_EntryPointer = &g_GameConsole;
	}
}
//----------------------------------------------------------------------------------
void CGumpBook::DelayedClick(CRenderObject *obj)
{
	WISPFUN_DEBUG("c87_f6");
	if (obj != NULL)
	{
		ChangePage(g_ClickObject.Page);
		WantRedraw = true;
	}
}
//----------------------------------------------------------------------------------
void CGumpBook::GUMP_BUTTON_EVENT_C
{
	WISPFUN_DEBUG("c87_f7");
	if (!g_ClickObject.Enabled)
	{
		int newPage = -1;

		if (serial == ID_GB_BUTTON_PREV) //Prev
		{
			if (Page > 0) //Если не было запроса на клик
			{
				newPage = Page - 2;

				if (newPage < 0)
					newPage = 0;
			}
		}
		else if (serial == ID_GB_BUTTON_NEXT) //Next
		{
			if (Page < PageCount) //Если не было запроса на клик
			{
				newPage = Page + 2;

				if (newPage > PageCount)
					newPage = PageCount - 1;
			}
		}

		if (newPage > -1)
		{
			g_ClickObject.Init(g_PressedObject.LeftObject, this);
			g_ClickObject.Timer = g_Ticks + g_MouseManager.DoubleClickDelay;
			g_ClickObject.Page = newPage;
		}
	}
}
//----------------------------------------------------------------------------------
bool CGumpBook::OnLeftMouseButtonDoubleClick()
{
	WISPFUN_DEBUG("c87_f8");
	if (g_PressedObject.LeftSerial == ID_GB_BUTTON_PREV) //Prev
	{
		//Был нажат уголок "Назад", при даблклике устанавливаем 1 страницу
		ChangePage(0);

		//Перерисуем гамп
		WantRedraw = true;

		return true;
	}
	else if (g_PressedObject.LeftSerial == ID_GB_BUTTON_NEXT) //Next
	{
		//Был нажат уголок "Вперед", при даблклике устанавливаем последнюю страницу
		int page = PageCount;

		if (PageCount % 2)
			page--;

		ChangePage(page);

		//Перерисуем гамп
		WantRedraw = true;

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------------
void CGumpBook::InsertInContent(const WPARAM &wparam, bool isCharPress)
{
	WISPFUN_DEBUG("c87_f9");
	int page = Page;

	if (page >= 0 && page <= PageCount)
	{
		bool isSecondEntry = false;
		CGUITextEntry *entry = GetEntry(page);

		if (entry == NULL)
			return;

		if (g_EntryPointer != &entry->m_Entry)
		{
			entry = GetEntry(page + 1);

			if (entry != NULL && page < PageCount - 1 && g_EntryPointer == &entry->m_Entry)
			{
				isSecondEntry = true;
				page++;
			}
			else
				return;
		}

		if (isCharPress)
		{
			if (!Unicode && wparam >= 0x0100)
				return;

			if (g_EntryPointer->Insert((wchar_t)wparam))
			{
				int linesCount = 0;
				int maxLinesCount = 8;

				if (!Unicode)
					linesCount = g_EntryPointer->GetLinesCountA(4);
				else
				{
					linesCount = g_EntryPointer->GetLinesCountW(1);
					maxLinesCount = 10;
				}

				if (linesCount > maxLinesCount)
				{
					int newPage = page + 1;

					bool pageLimitExceeded = false;
					if (newPage > PageCount)
						pageLimitExceeded = true;

					int current = g_EntryPointer->Pos();

					//if we have to paste last line from text entry on the next page and flip back
					bool goBack = true;

					//get info with last line of text on current page
					PMULTILINES_FONT_INFO info = Unicode ? g_FontManager.GetInfoW(1, g_EntryPointer->GetTextW().c_str(), g_EntryPointer->Length(), TS_LEFT, 0, 166)
						: g_FontManager.GetInfoA(4, g_EntryPointer->GetTextA().c_str(), g_EntryPointer->Length(), TS_LEFT, 0, 166);

					bool addNewLine = false;
					while (info != NULL)
					{
						PMULTILINES_FONT_INFO next = info->m_Next;
						if (next != NULL)
						{
							if (next->m_Next == NULL && next->Data.size() > 0 && info->Data.size() == 0)
								addNewLine = true;
							info->Data.clear();
							delete info;
							info = next;
						}
						else
							break;
					}

					m_ChangedPage[page] = true;

					//determine if we're staying on a new page or going back to the current
					if (g_EntryPointer->Pos() >= info->CharStart)
						goBack = false;

					//remove characters which do not fit on current page
					if (info->CharCount == 0)
						g_EntryPointer->RemoveSequence(g_EntryPointer->Length() - 1, 1);
					else
					{
						int start = info->CharStart;
						int count = info->CharCount;
						if (addNewLine)
						{
							start -= 1;
							count += 1;
						}
						g_EntryPointer->RemoveSequence(start, count);
					}

					if (pageLimitExceeded)
						return;

					//go to the next page and set position for text entry there
					if (newPage % 2 == 0)
						ChangePage(newPage, !goBack);
					SetPagePos(0, newPage);

					//insert data on the next page
					if (info->Data.size() == 0 || addNewLine)
						InsertInContent('\n');
					IFOR(i, 0, info->Data.size())
						InsertInContent(info->Data[i].item);


					if (goBack)
					{
						//go back to initial position on your current page
						m_ChangedPage[page + 1] = true;
						ChangePage(page % 2 == 0 ? page : page - 1, false);
						SetPagePos(current, page);
					}
					else
						g_EntryPointer->SetPos(0, this);
				}	
				else
					m_ChangedPage[page] = true;

				WantRedraw = true;

			}
		}
		else
		{
			if (g_EntryPointer->Pos() == 0)
			{
				int previousPage = page - 2;
				if (previousPage < 0 )
					previousPage = 0;	

				if (page % 2 == 0)
					ChangePage(previousPage);

				SetPagePos(-1, page - 1);
			}	
			m_ChangedPage[page] = true;
			WantRedraw = true;
		}
		//page
	}
}
//----------------------------------------------------------------------------------
void CGumpBook::OnCharPress(const WPARAM &wParam, const LPARAM &lParam)
{
	WISPFUN_DEBUG("c87_f10");
	if (!Writable)
		return;

	if (g_EntryPointer == &m_EntryTitle->m_Entry || g_EntryPointer == &m_EntryAuthor->m_Entry)
	{
		if (g_EntryPointer->Insert((wchar_t)wParam))
		{
			if (Unicode)
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

			WantRedraw = true;
		}
	}
	else
		InsertInContent(wParam);
}
//----------------------------------------------------------------------------------
void CGumpBook::OnKeyDown(const WPARAM &wParam, const LPARAM &lParam)
{
	WISPFUN_DEBUG("c87_f11");
	if (!Writable)
		return;

	switch (wParam)
	{
		case VK_RETURN:
		{
			if (g_EntryPointer != &m_EntryTitle->m_Entry && g_EntryPointer != &m_EntryAuthor->m_Entry)
			{
				InsertInContent(L'\n');
				WantRedraw = true;
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
void CGumpBook::SetPagePos(int val, int page)
{
	//safety
	if (page < 0)
		page = 0;
	if (page > PageCount)
		page = PageCount;

	//set position of caret
	CGUITextEntry *newEntry = GetEntry(page);
	g_EntryPointer = &newEntry->m_Entry;

	if (val == -1)
		val = g_EntryPointer->Length();
	g_EntryPointer->SetPos(val, this);
		
}
//----------------------------------------------------------------------------------
void CGumpBook::PasteClipboardData(wstring &data)
{
	IFOR(i, 0, (int)data.length())
		InsertInContent(data[i]);
}
//----------------------------------------------------------------------------------