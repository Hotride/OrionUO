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
m_Unicode(unicode), m_Page(2)
{
	TextEntryAuthor = new TEntryText();
	TextEntryTitle = new TEntryText();

	TextEntry = new TBookEntryText[pageCount];
	m_PageIndexText = new TTextTexture[pageCount];

	IFOR(i, 0, pageCount)
		FontManager->GenerateA(9, m_PageIndexText[i], std::to_string(i).c_str(), 0x0386);
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

	if (m_PageIndexText != NULL)
	{
		delete[] m_PageIndexText;
		m_PageIndexText = NULL;
	}
}
//----------------------------------------------------------------------------
void TGumpBook::InitTextTextures()
{
	FontManager->GenerateA(4, m_TextTitle, "TITLE", 1);
	FontManager->GenerateA(4, m_TextBy, "by", 1);
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
			m_TextTitle.Draw(78, 32);

			TextEntryTitle->DrawA(4, 0, 41, 63);

			m_TextBy.Draw(88, 132);

			TextEntryAuthor->DrawA(4, 0, 41, 158);

			TextEntry[1].DrawA(4, 0, 224, 32);
			m_PageIndexText[1].Draw(299, 202);
		}
		else
		{
			TextEntry[m_Page].DrawA(4, 0, 38, 32);
			m_PageIndexText[m_Page].Draw(112, 202);

			if (m_Page + 1 < m_PageCount)
			{
				TextEntry[m_Page + 1].DrawA(4, 0, 224, 32);
				m_PageIndexText[m_Page + 1].Draw(299, 202);
			}
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
		{
			g_LastSelectedGump = (DWORD)this;

			if (m_Page && UO->GumpPixelsInXY(0x01FF, 0, 0))
				LSG = ID_GB_BUTTON_PREV; //Last page
			else if (m_Page + 2 < m_PageCount && UO->GumpPixelsInXY(0x0200, 356, 0))
				LSG = ID_GB_BUTTON_NEXT; //Next page

			if (!m_Page)
			{
			}
			else
			{
			}
		}

		g_MouseX = oldMouseX;
		g_MouseY = oldMouseY;

		if (LSG != 0)
			g_LastSelectedObject = LSG; //Если что-то нашлось - выбираем

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpBook::OnLeftMouseUp()
{
	//Проверим валидность переменных
	if (g_LastObjectLeftMouseDown != g_LastSelectedObject || !g_LastObjectLeftMouseDown || Minimized || !g_LastSelectedGump)
		return;

	int newPage = -1;

	if (g_LastObjectLeftMouseDown == ID_GB_BUTTON_PREV) //Prev
	{
		//Ткнули по уголку "назад"

		if (!g_ClickObjectReq && m_Page > 0) //Если не было запроса на клик
		{
			newPage = m_Page - 2;

			if (newPage < 0)
				newPage = 0;
		}
	}
	else if (g_LastObjectLeftMouseDown == ID_GB_BUTTON_NEXT) //Next
	{
		//Ткнули по уголку "назад"

		if (!g_ClickObjectReq && m_Page < m_PageCount) //Если не было запроса на клик
		{
			newPage = m_Page + 2;

			if (newPage >= m_PageCount)
				newPage = m_PageCount - 1;
		}
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