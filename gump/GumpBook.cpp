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
	TextEntryAuthor = new TEntryText();
	TextEntryTitle = new TEntryText();

	IFOR(i, 0, 2)
		TextEntry[i] = new TBookEntryText();
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

	IFOR(i, 0, 2)
	{
		if (TextEntry[i] != NULL)
		{
			delete TextEntry[i];
			TextEntry[i] = NULL;
		}
	}
}
//----------------------------------------------------------------------------
void TGumpBook::InitTextTextures()
{
	FontManager->GenerateA(6, m_TextTitle, "TITLE", 0);
	FontManager->GenerateA(6, m_TextBy, "by", 0);
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

	CalculateGumpState();

	glNewList((GLuint)this, GL_COMPILE);

		UO->DrawGump(0x01FE, 0, 0, 0); //Body

		m_TextTitle.Draw(60, 220);
		m_TextBy.Draw(60, 420);

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
		if (!m_FrameCreated || g_GumpPressedElement || g_GumpMovingOffsetX || g_GumpMovingOffsetY)
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
			LSG = 0;
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

	//Сохраним текущую страницу в буффер
	int lastPage = m_Page;

	if (g_LastObjectLeftMouseDown == ID_GB_BUTTON_PREV) //Prev
	{
		//Ткнули по уголку "назад"

		if (!g_ClickObjectReq && m_Page > 0) //Если не было запроса на клик
		{
			//Активируем запрос на клик
			g_ClickObjectReq = true;

			//Инициализируем клик-буффер
			g_ClickObject.Init(COT_GUMP);
			g_ClickObject.GumpType = GumpType;
			g_ClickObject.Serial = Serial;
			g_ClickObject.GumpID = ID;
			g_ClickObject.GumpButtonID = m_Page - 1;

			//Задаем время до выполнения
			g_ClickObject.Timer = GetTickCount() + DCLICK_DELAY;
		}
	}
	else if (g_LastObjectLeftMouseDown == ID_GB_BUTTON_NEXT) //Next
	{
		//Ткнули по уголку "назад"

		if (!g_ClickObjectReq && m_Page < m_PageCount) //Если не было запроса на клик
		{
			//Активируем запрос на клик
			g_ClickObjectReq = true;

			//Инициализируем клик-буффер
			g_ClickObject.Init(COT_GUMP);
			g_ClickObject.GumpType = GumpType;
			g_ClickObject.Serial = Serial;
			g_ClickObject.GumpID = ID;
			g_ClickObject.GumpButtonID = m_Page + 1;

			//Задаем время до выполнения
			g_ClickObject.Timer = GetTickCount() + DCLICK_DELAY;
		}
	}

	//Если была изменена страница - перерисуем гамп
	if (lastPage != m_Page)
		m_FrameCreated = false;
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
		m_Page = 1;

		//Перерисуем гамп
		m_FrameCreated = false;

		return true;
	}
	else if (g_LastObjectLeftMouseDown == ID_GB_BUTTON_NEXT) //Next
	{
		//Был нажат уголок "Вперед", при даблклике устанавливаем последнюю страницу
		m_Page = m_PageCount;

		//Перерисуем гамп
		m_FrameCreated = false;

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------