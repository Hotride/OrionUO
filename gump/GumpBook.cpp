/****************************************************************************
**
** GumpBook.cpp
**
** Copyright (C) October 2015 Hotride
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
*****************************************************************************
*/
//----------------------------------------------------------------------------
#include "stdafx.h"

TTextTexture TGumpBook::m_TextTitle;
TTextTexture TGumpBook::m_TextBy;
//----------------------------------------------------------------------------
TGumpBook::TGumpBook(DWORD serial, short x, short y, short pageCount, bool writable, bool unicode)
: TGump(GT_BOOK, serial, x, y), m_PageCount(pageCount), m_Writable(writable),
m_Unicode(unicode), m_Page(0)
{
	TextEntryAuthor = new TEntryText(0, 150);
	TextEntryTitle = new TEntryText(0, 150);

	TextEntry = new TEntryText[pageCount];
	m_ChangedPage = new bool[pageCount];
	m_PageIndexText = new TTextTexture[pageCount];

	IFOR(i, 0, pageCount)
	{
		FontManager->GenerateA(9, m_PageIndexText[i], std::to_string(i).c_str(), 0x0386);
		TextEntry[i].Width = 166;
		m_ChangedPage[i] = false;
	}
}
//----------------------------------------------------------------------------
TGumpBook::~TGumpBook()
{
	if (TextEntryAuthor != NULL)
	{
		delete TextEntryAuthor;
		TextEntryAuthor = NULL;
	}

	if (TextEntryTitle != NULL)
	{
		delete TextEntryTitle;
		TextEntryTitle = NULL;
	}

	if (TextEntry != NULL)
	{
		delete[] TextEntry;
		TextEntry = NULL;
	}

	if (m_ChangedPage != NULL)
	{
		delete m_ChangedPage;
		m_ChangedPage = NULL;
	}

	if (m_PageIndexText != NULL)
	{
		delete[] m_PageIndexText;
		m_PageIndexText = NULL;
	}
}
//----------------------------------------------------------------------------
void TGumpBook::InitTextTextures()
{
	FontManager->UnusePartialHue = true;
	FontManager->GenerateA(4, m_TextTitle, "TITLE", 0x0386);
	FontManager->GenerateA(4, m_TextBy, "by", 0x0386);
	FontManager->UnusePartialHue = false;
}
//----------------------------------------------------------------------------
void TGumpBook::ReleaseTextTextures()
{
	m_TextTitle.Clear();
	m_TextBy.Clear();
}
//----------------------------------------------------------------------------
void TGumpBook::PrepareTextures()
{
	UO->ExecuteGumpPart(0x01FE, 3);
}
//----------------------------------------------------------------------------
bool TGumpBook::EntryPointerHere()
{
	bool result = (EntryPointer == TextEntryAuthor || EntryPointer == TextEntryTitle);

	if (!result && TextEntry != NULL)
	{
		IFOR(i, 0, m_PageCount && !result)
			result = (EntryPointer == &TextEntry[i]);
	}

	return result;
}
//---------------------------------------------------------------------------
void TGumpBook::GenerateFrame()
{
	if (!g_DrawMode)
	{
		m_FrameRedraw = false;
		m_FrameCreated = false;

		return;
	}

	glNewList((GLuint)this, GL_COMPILE);

		UO->DrawGump(0x01FE, 0, 0, 0); //Body

		if (m_Page)
			UO->DrawGump(0x01FF, 0, 0, 0); //Last page

		if (m_Page + 2 < m_PageCount)
			UO->DrawGump(0x0200, 0, 356, 0); //Next page

		if (!m_Page)
		{
			//Название книги
			m_TextTitle.Draw(78, 32);
			if (m_Unicode)
				TextEntryTitle->DrawW(0, 0, 41, 65);
			else
				TextEntryTitle->DrawA(4, 0, 41, 65);

			//Автор
			m_TextBy.Draw(88, 134);
			if (m_Unicode)
				TextEntryAuthor->DrawW(0, 0, 41, 160);
			else
				TextEntryAuthor->DrawA(4, 0, 41, 160);

			//Правая страница
			FontManager->UnusePartialHue = true;
			if (m_Unicode)
				TextEntry[1].DrawW(0, 0x0012, 224, 34);
			else
				TextEntry[1].DrawA(4, 0x0012, 224, 34);
			m_PageIndexText[1].Draw(299, 202);
			FontManager->UnusePartialHue = false;
		}
		else
		{
			FontManager->UnusePartialHue = true;
			//Левая страница
			if (m_Unicode)
				TextEntry[m_Page].DrawW(0, 0x0012, 38, 34);
			else
				TextEntry[m_Page].DrawA(4, 0x0012, 38, 34);
			m_PageIndexText[m_Page].Draw(112, 202);

			if (m_Page + 1 < m_PageCount)
			{
				//Правая страница
				if (m_Unicode)
					TextEntry[m_Page + 1].DrawW(0, 0x0012, 224, 34);
				else
					TextEntry[m_Page + 1].DrawA(4, 0x0012, 224, 34);
				m_PageIndexText[m_Page + 1].Draw(299, 202);
			}
			FontManager->UnusePartialHue = false;
		}

	glEndList();

	m_FrameCreated = true;
	m_FrameRedraw = true;
}
//----------------------------------------------------------------------------
int TGumpBook::Draw(bool &mode)
{
	DWORD index = (DWORD)this;

	CalculateGumpState();

	if (mode) //Отрисовка
	{
		if (!m_FrameCreated || g_GumpMovingOffsetX || g_GumpMovingOffsetY)
			GenerateFrame();
		else if (m_FrameRedraw)
		{
			GenerateFrame();
			m_FrameRedraw = false;
		}

		glTranslatef(g_GumpTranslateX, g_GumpTranslateY, 0.0f);

		glCallList((GLuint)this);

		glTranslatef(-g_GumpTranslateX, -g_GumpTranslateY, 0.0f);
	}
	else //Выбор объектов
	{
		int oldMouseX = g_MouseX;
		int oldMouseY = g_MouseY;
		g_MouseX -= (int)g_GumpTranslateX;
		g_MouseY -= (int)g_GumpTranslateY;

		int LSG = 0;

		if (UO->GumpPixelsInXY(0x01FE, 0, 0))
			g_LastSelectedGump = index;

		if (m_Page && UO->GumpPixelsInXY(0x01FF, 0, 0))
			LSG = ID_GB_BUTTON_PREV; //Last page
		else if (m_Page + 2 < m_PageCount && UO->GumpPixelsInXY(0x0200, 356, 0))
			LSG = ID_GB_BUTTON_NEXT; //Next page
		else if (!m_Page)
		{
			if (UO->PolygonePixelsInXY(41, 65, 150, (m_Unicode ? 22 : 44)))
				LSG = ID_GB_TEXT_AREA_TITLE; //Text title
			else if (UO->PolygonePixelsInXY(41, 160, 150, 22))
				LSG = ID_GB_TEXT_AREA_AUTHOR; //Text author
			else if (UO->PolygonePixelsInXY(224, 34, 160, 166))
				LSG = ID_GB_TEXT_AREA_PAGE_RIGHT; //Text right area
		}
		else
		{
			if (UO->PolygonePixelsInXY(38, 34, 160, 166))
				LSG = ID_GB_TEXT_AREA_PAGE_LEFT; //Text left area
			else if (UO->PolygonePixelsInXY(224, 34, 160, 166))
				LSG = ID_GB_TEXT_AREA_PAGE_RIGHT; //Text right area
		}

		g_MouseX = oldMouseX;
		g_MouseY = oldMouseY;

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpBook::OnLeftMouseUp()
{
	//Проверим валидность переменных
	if (g_LastObjectLeftMouseDown != g_LastSelectedObject || !g_LastObjectLeftMouseDown || !g_LastSelectedGump)
		return;

	int newPage = -1;

	if (g_LastObjectLeftMouseDown == ID_GB_BUTTON_PREV) //Prev
	{
		if (!g_ClickObjectReq && m_Page > 0) //Если не было запроса на клик
		{
			newPage = m_Page - 2;

			if (newPage < 0)
				newPage = 0;
		}
	}
	else if (g_LastObjectLeftMouseDown == ID_GB_BUTTON_NEXT) //Next
	{
		if (!g_ClickObjectReq && m_Page < m_PageCount) //Если не было запроса на клик
		{
			newPage = m_Page + 2;

			if (newPage >= m_PageCount)
				newPage = m_PageCount - 1;
		}
	}
	else if (m_Writable)
	{
		int x = g_MouseX - m_X;
		int y = g_MouseY - m_Y;

		TEntryText *entry = NULL;

		if (g_LastObjectLeftMouseDown == ID_GB_TEXT_AREA_AUTHOR)
		{
			entry = TextEntryAuthor;
			x -= 41;
			y -= 160;
		}
		else if (g_LastObjectLeftMouseDown == ID_GB_TEXT_AREA_TITLE)
		{
			entry = TextEntryTitle;
			x -= 41;
			y -= 65;
		}
		else if (g_LastObjectLeftMouseDown == ID_GB_TEXT_AREA_PAGE_LEFT)
		{
			entry = &TextEntry[m_Page];
			x -= 38;
			y -= 34;
		}
		else if (g_LastObjectLeftMouseDown == ID_GB_TEXT_AREA_PAGE_RIGHT)
		{
			entry = &TextEntry[m_Page + 1];
			x -= 224;
			y -= 34;
		}

		if (entry != NULL)
			entry->OnClick(this, (m_Unicode ? 0 : 4), m_Unicode, x, y);
	}

	if (newPage > -1)
	{
		//Активируем запрос на клик
		g_ClickObjectReq = true;

		//Инициализируем клик-буффер
		g_ClickObject.Init(COT_GUMP);
		g_ClickObject.GumpType = GumpType;
		g_ClickObject.Serial = Serial;
		g_ClickObject.GumpID = ID;
		g_ClickObject.GumpButtonID = newPage;

		//TEST~>>
		m_Page = newPage;
		m_FrameCreated = false;
		g_ClickObjectReq = false;
		//TEST~<<

		//Задаем время до выполнения
		g_ClickObject.Timer = GetTickCount() + DCLICK_DELAY;
	}
}
//----------------------------------------------------------------------------
bool TGumpBook::OnLeftMouseDoubleClick()
{
	//Если ничего выбрано не было - уходим от сюда
	if (!g_LastSelectedGump)
		return false;

	if (g_LastObjectLeftMouseDown == ID_GB_BUTTON_PREV) //Prev
	{
		//Был нажат уголок "Назад", при даблклике устанавливаем 1 страницу
		m_Page = 0;

		//Перерисуем гамп
		m_FrameCreated = false;

		return true;
	}
	else if (g_LastObjectLeftMouseDown == ID_GB_BUTTON_NEXT) //Next
	{
		//Был нажат уголок "Вперед", при даблклике устанавливаем последнюю страницу
		if (m_PageCount % 2)
			m_Page = m_PageCount - 2;
		else
			m_Page = m_PageCount - 1;

		//Перерисуем гамп
		m_FrameCreated = false;

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void TGumpBook::InsertInContent(const WPARAM &wparam)
{
	int page = m_Page;

	if (page >= 0 && page < m_PageCount)
	{
		bool isSecondEntry = false;

		if (EntryPointer != &TextEntry[page])
		{
			if (page < m_PageCount - 1 && EntryPointer == &TextEntry[page + 1])
			{
				isSecondEntry = true;
				page++;
			}
			else
				return;
		}

		if (EntryPointer->Insert(wparam))
		{
			if (m_Unicode)
			{
				if (EntryPointer->GetLinesCountW(0) > 8)
					EntryPointer->Remove(true);
				else
					m_ChangedPage[page] = true;
			}
			else
			{
				if (EntryPointer->GetLinesCountA(4) > 8)
					EntryPointer->Remove(true);
				else
					m_ChangedPage[page] = true;
			}

			m_FrameCreated = false;
		}
		//page
	}
}
//----------------------------------------------------------------------------
void TGumpBook::OnCharPress(WPARAM &wparam, LPARAM &lparam)
{
	if (wparam == VK_RETURN || wparam == VK_BACK || wparam == VK_ESCAPE || EntryPointer == NULL || !m_Writable)
		return; //Ignore no print keys

	if (EntryPointer == TextEntryTitle || EntryPointer == TextEntryAuthor)
	{
		if (EntryPointer->Insert(wparam))
		{
			if (m_Unicode)
			{
				if (EntryPointer->GetLinesCountW(0) > 1)
					EntryPointer->Remove(true);
			}
			else
			{
				int count = 1;

				if (EntryPointer == TextEntryTitle)
					count++;

				if (EntryPointer->GetLinesCountA(4) > count)
					EntryPointer->Remove(true);
			}

			m_FrameCreated = false;
		}
	}
	else
		InsertInContent(wparam);
}
//----------------------------------------------------------------------------
void TGumpBook::OnKeyPress(WPARAM &wparam, LPARAM &lparam)
{
	if (!m_Writable)
		return;

	switch (wparam)
	{
		case VK_RETURN:
		{
			if (EntryPointer != TextEntryTitle && EntryPointer != TextEntryAuthor)
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
			EntryPointer->OnKey(this, wparam);
			m_FrameCreated = false;
			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------