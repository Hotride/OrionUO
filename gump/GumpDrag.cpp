/****************************************************************************
**
** DragGump.cpp
**
** Copyright (C) September 2015 Hotride
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
//---------------------------------------------------------------------------
TGumpDrag::TGumpDrag(DWORD serial, short x, short y)
: TGump(GT_DRAG, serial, x, y), m_ScrollPos(122), m_StartText(true)
{
	int count = 0;
	TGameItem *selobj = World->FindWorldItem(serial);
	if (selobj == NULL)
		count = selobj->Count;

	TextEntry = new TEntryText(count, 0, 0, true);
}
//---------------------------------------------------------------------------
TGumpDrag::~TGumpDrag()
{
	if (TextEntry != NULL)
	{
		delete TextEntry;
		TextEntry = NULL;
	}
}
//---------------------------------------------------------------------------
void TGumpDrag::PrepareTextures()
{
	UO->ExecuteGump(0x085C);
	UO->ExecuteGumpPart(0x0845, 2);
	UO->ExecuteButton(0x081A);
}
//---------------------------------------------------------------------------
void TGumpDrag::GenerateFrame(int posX, int posY)
{
	if (!g_DrawMode)
	{
		m_FrameRedraw = false;
		m_FrameCreated = false;

		return;
	}

	CalculateGumpState();

	if (TextEntry != NULL && TextEntry->Changed) //Текстовое поле существует и было изменено, отрисуем его для формирования текстуры
		TextEntry->DrawA(1, 0x0386, 29, 42);

	glNewList((GLuint)this, GL_COMPILE);

		UO->DrawGump(0x085C, 0, 0, 0); //Gump
		
		WORD gumpID = 0x0845 + (int)(g_GumpSelectElement == ID_GD_SCROLL); //Scroll button / selected
		UO->DrawGump(gumpID, 0, m_ScrollPos, 16);
		
		if (TextEntry != NULL) //Текстовое поле существует, отрисуем его
			TextEntry->DrawA(1, 0x0386, 29, 42);
		
		gumpID = 0x081A; //Button Okay
		if (g_GumpPressedElement == ID_GD_OKAY)
			gumpID = 0x081B; //Button Okay (down)
		else if (g_GumpSelectElement == ID_GD_OKAY)
			gumpID = 0x081C; //Button Okay selected
		UO->DrawGump(gumpID, 0, 102, 37);

	glEndList();

	m_FrameRedraw = true;
	m_FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpDrag::Draw(bool &mode)
{
	TGameItem *selobj = World->FindWorldItem(Serial);
	if (selobj == NULL)
		return 0; //Объект, к которому привязан ползунок - исчезsel

	DWORD index = (DWORD)this;

	CalculateGumpState();

	//Если схватили ползунок - вычислим его позицию и значение в текстовом поле
	if (mode && g_GumpPressed && g_LastObjectLeftMouseDown == ID_GD_SCROLL)
	{
		int CurrentX = g_MouseX - (int)g_GumpTranslateX - 5; //Текущая позиция ползунка

		if (CurrentX < 29)
			CurrentX = 29; //Выход за допустимый предел, корректируем на минимум
		else if (CurrentX > 122)
			CurrentX = 122; //Выход за допустимый предел, корректируем на максимум

		m_ScrollPos = CurrentX; //Устанавливаем значение ползунка

		int count = selobj->Count; //Количество предметов в стеке

		if (CurrentX > 29) //Позиция ползунка больше минимума
		{
			if (CurrentX < 122) //Позиция ползунка меньше максимума, вычисляем
			{
				float ValPer = ((CurrentX - 29.0f) / 93.0f) * 100.0f;
				if (ValPer == 0.0f)
					count = 0;
				else
					count = (int)(count * ValPer) / 100;
			}
		}
		else
			count = 0; //Позиция равна минимуму, выставляем количество 0

		//Конвертирование текста в строку и запись в текстовое поле гампа
		TextEntry->SetText(std::to_string(count));

		m_FrameCreated = false;
	}

	if (mode) //Отрисовка
	{
		if (!m_FrameCreated || g_GumpSelectElement) // || g_GumpPressed
			GenerateFrame(0, 0);
		else if (m_FrameRedraw)
		{
			GenerateFrame(0, 0);
			m_FrameRedraw = false;
		}

		glTranslatef(g_GumpTranslateX, g_GumpTranslateY, 0.0f);

		glCallList((GLuint)index);

		glTranslatef(-g_GumpTranslateX, -g_GumpTranslateY, 0.0f);
	}
	else //Выбор объектов
	{
		int oldMouseX = g_MouseX;
		int oldMouseY = g_MouseY;
		g_MouseX -= (int)g_GumpTranslateX;
		g_MouseY -= (int)g_GumpTranslateY;

		int LSG = 0;

		//Если выбран основной гамп - меняем глобальный указатель на выбранный гамп на него
		if (UO->GumpPixelsInXY(0x085C, 0, 0))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		
			if (UO->GumpPixelsInXY(0x0845, m_ScrollPos, 16))
				LSG = ID_GD_SCROLL; //Scroll button
			else if (UO->GumpPixelsInXY(0x081A, 102, 37))
				LSG = ID_GD_OKAY; //Button Okay
			else if (UO->PolygonePixelsInXY(28, 40, 60, 16))
				LSG = ID_GD_TEXT_FIELD; //Текстовое поле

			if (LSG != 0)
				g_LastSelectedObject = LSG; //Если что-то нашлось - выбираем
		}

		g_MouseX = oldMouseX;
		g_MouseY = oldMouseY;

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpDrag::OnLeftMouseUp()
{
	//Нажатый объект - не последний выбранный под курсором...валим
	if (g_LastObjectLeftMouseDown != g_LastSelectedObject)
		return;

	if (g_LastObjectLeftMouseDown == ID_GD_OKAY) //Button Okay
		OnOkayPressed();
	else if (g_LastObjectLeftMouseDown == ID_GD_TEXT_FIELD) //Выбрали текстовое поле
	{
		int x = g_MouseX - (m_X + 28);
		int y = g_MouseY - (m_Y + 40);

		TextEntry->OnClick(this, 1, false, x, y);
	}
}
//----------------------------------------------------------------------------
void TGumpDrag::OnCharPress(WPARAM &wparam, LPARAM &lparam)
{
	if (wparam == VK_RETURN || wparam == VK_BACK || wparam == VK_ESCAPE || EntryPointer == NULL)
		return; //Ignore no print keys

	TGameItem *item = World->FindWorldItem(m_Serial);
	if (item != NULL)
	{
		if (wparam >= '0' && wparam <= '9')
		{
			if (m_StartText)
			{
				EntryPointer->Clear();
				m_StartText = false;
			}

			if (EntryPointer->Insert(wparam))
			{
				int val = atoi(EntryPointer->c_str());

				float ValPer = (val / item->Count) * 100.0f;

				if (ValPer == 0.0f)
					val = 0;
				else
					ValPer = (ValPer * 93.0f) / 100.0f;

				m_ScrollPos = 29 + (int)ValPer;

				m_FrameCreated = false;
			}
		}
	}
}
//----------------------------------------------------------------------------
void TGumpDrag::OnKeyPress(WPARAM &wparam, LPARAM &lparam)
{
	TGameItem *item = World->FindWorldItem(Serial);

	if (item != NULL)
	{
		switch (wparam)
		{
			case VK_RETURN:
			{
				OnOkayPressed();

				if (ConfigManager.GetConsoleNeedEnter())
					EntryPointer = NULL;
				else
					EntryPointer = GameConsole;

				break;
			}
			case VK_HOME:
			{
				EntryPointer->SetPos(0);

				if (m_StartText)
					m_StartText = false;

				m_FrameCreated = false;

				break;
			}
			case VK_END:
			{
				EntryPointer->SetPos(EntryPointer->Length());

				if (m_StartText)
					m_StartText = false;

				m_FrameCreated = false;

				break;
			}
			case VK_LEFT:
			{
				EntryPointer->AddPos(-1);

				if (m_StartText)
					m_StartText = false;

				m_FrameCreated = false;

				break;
			}
			case VK_RIGHT:
			{
				EntryPointer->AddPos(1);

				if (m_StartText)
					m_StartText = false;

				m_FrameCreated = false;

				break;
			}
			case VK_DELETE:
			case VK_BACK:
			{
				EntryPointer->Remove(wparam == VK_BACK);

				if (EntryPointer->Length())
				{
					int val = atoi(EntryPointer->c_str());
					float ValPer = (val / item->Count) * 100.0f;

					if (ValPer == 0.0f)
						ValPer = 0.0f;
					else
						ValPer = (ValPer * 93.0f) / 100.0f;

					m_ScrollPos = 29 + (int)ValPer;
				}
				else
					m_ScrollPos = 29;

				if (m_StartText)
					m_StartText = false;

				m_FrameCreated = false;

				break;
			}
			default:
				break;
		}
	}
}
//----------------------------------------------------------------------------
void TGumpDrag::OnOkayPressed()
{
	if (ObjectInHand == NULL)
	{
		if (TextEntry->Length())
		{
			int count = atoi(TextEntry->c_str());

			if (count)
			{
				TGameItem *obj = World->FindWorldItem(m_Serial);

				if (obj != NULL)
					UO->PickupItem(obj, count);
			}
		}

		GumpManager->RemoveGump(this);
	}
}
//----------------------------------------------------------------------------