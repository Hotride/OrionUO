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
		FrameRedraw = false;
		FrameCreated = false;

		return;
	}

	DWORD index = (DWORD)this;

	//Нажата ли кнопка в окне?
	bool IsPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index && g_LastSelectedGump == index);

	//Может ли быть подсвечен элемент?
	int CanSelectedButton = ((g_LastSelectedGump == index) ? g_LastSelectedObject : 0);

	//Может ли быть нажат элемент?
	int CanPressedButton = 0;
	if (IsPressed && g_LastObjectLeftMouseDown == g_LastSelectedObject)
		CanPressedButton = g_LastObjectLeftMouseDown;

	glNewList((GLuint)index, GL_COMPILE);

		UO->DrawGump(0x085C, 0, posX, posY); //Gump
		
		WORD gumpID = 0x0845 + (int)(CanSelectedButton == ID_GD_SCROLL); //Scroll button / selected
		UO->DrawGump(gumpID, 0, posX + m_ScrollPos, posY + 16);
		
		if (TextEntry != NULL) //Текстовое поле существует, отрисуем его
			TextEntry->DrawA(1, 0x0386, posX + 29, posY + 42);
		
		gumpID = 0x081A; //Button Okay
		if (CanPressedButton == ID_GD_OKAY)
			gumpID = 0x081B; //Button Okay (down)
		else if (CanSelectedButton == ID_GD_OKAY)
			gumpID = 0x081C; //Button Okay selected
		UO->DrawGump(gumpID, 0, posX + 102, posY + 37);

	glEndList();

	FrameRedraw = true;
	FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpDrag::Draw(bool &mode)
{
	DWORD index = (DWORD)this;

	TGameItem *selobj = World->FindWorldItem(Serial);
	if (selobj == NULL)
		return 0; //Объект, к которому привязан ползунок - исчезsel
	
	//Для быстрого доступа
	int posX = X;
	int posY = Y;
	
	//Нажата ли кнопка в окне?
	bool IsPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index && g_LastSelectedGump == index);

	//Может ли быть подсвечен элемент?
	int CanSelectedButton = ((g_LastSelectedGump == index) ? g_LastSelectedObject : 0);

	//Может ли быть нажат элемент?
	int CanPressedButton = 0;
	if (IsPressed && g_LastObjectLeftMouseDown == g_LastSelectedObject)
		CanPressedButton = g_LastObjectLeftMouseDown;

	//Если схватили ползунок - вычислим его позицию и значение в текстовом поле
	if (mode && IsPressed && g_LastObjectLeftMouseDown == ID_GD_SCROLL)
	{
		int CurrentX = g_MouseX - posX - 5; //Текущая позиция ползунка

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
		char text_amount[20] = {0};
		sprintf(text_amount, "%d", count);

		TextEntry->SetText(text_amount);

		GenerateFrame(posX, posY);
		FrameRedraw = false;
	}

	//Если окошко захвачено для перемещения - вычислим оффсеты
	if (mode && g_LeftMouseDown && g_LastGumpLeftMouseDown == index && !g_LastObjectLeftMouseDown)
	{
		posX += g_MouseX - g_DroppedLeftMouseX;
		posY += g_MouseY - g_DroppedLeftMouseY;
		
		if (mode)
			GenerateFrame(posX, posY);
	}
	else if (mode)
	{
		if (IsPressed || CanSelectedButton)
			GenerateFrame(posX, posY);
		else if (FrameRedraw)
		{
			GenerateFrame(posX, posY);
			FrameRedraw = false;
		}
	}

	if (mode) //Отрисовка
	{
		if (!FrameCreated)
			GenerateFrame(posX, posY);

		glCallList((GLuint)index);
	}
	else //Выбор объектов
	{
		int LSG = 0;

		//Если выбран основной гамп - меняем глобальный указатель на выбранный гамп на него
		if (UO->GumpPixelsInXY(0x085C, posX, posY))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		
			if (UO->GumpPixelsInXY(0x0845, posX + (int)m_ScrollPos, posY + 16))
				LSG = ID_GD_SCROLL; //Scroll button
			else if (UO->GumpPixelsInXY(0x081A, posX + 102, posY + 37))
				LSG = ID_GD_OKAY; //Button Okay
			else
			{
				//Текстовое поле
				RECT rc = {0, 0, 60, 16};
				POINT p = {g_MouseX - (posX + 28), g_MouseY - (posY + 40)};
				if (PtInRect(&rc, p))
					LSG = ID_GD_TEXT_FIELD;
			}

			if (LSG != 0)
				g_LastSelectedObject = LSG; //Если что-то нашлось - выбираем
		}

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
		int x = g_MouseX - (X + 28);
		int y = g_MouseY - (Y + 40);

		TextEntry->OnClick(this, 1, false, x, y);
	}
}
//----------------------------------------------------------------------------
void TGumpDrag::OnCharPress(WPARAM &wparam, LPARAM &lparam)
{
	if (wparam == VK_RETURN || wparam == VK_BACK || wparam == VK_ESCAPE || EntryPointer == NULL)
		return; //Ignore no print keys

	TGameItem *item = World->FindWorldItem(Serial);
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

				GenerateFrame(X, Y);
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

				GenerateFrame(X, Y);

				break;
			}
			case VK_END:
			{
				EntryPointer->SetPos(EntryPointer->Length());

				if (m_StartText)
					m_StartText = false;

				GenerateFrame(X, Y);

				break;
			}
			case VK_LEFT:
			{
				EntryPointer->AddPos(-1);

				if (m_StartText)
					m_StartText = false;

				GenerateFrame(X, Y);

				break;
			}
			case VK_RIGHT:
			{
				EntryPointer->AddPos(1);

				if (m_StartText)
					m_StartText = false;

				GenerateFrame(X, Y);

				break;
			}
			case VK_BACK:
			{
				EntryPointer->Remove(true);

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

				GenerateFrame(X, Y);

				break;
			}
			case VK_DELETE:
			{
				EntryPointer->Remove(false);

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

				GenerateFrame(X, Y);

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
	if (ObjectInHand != NULL)
		return;

	if (TextEntry->Length())
	{
		int count = atoi(TextEntry->c_str());
		if (count)
		{
			TGameItem *obj = World->FindWorldItem(Serial);

			if (obj != NULL)
				UO->PickupItem(obj, count);
		}
	}

	GumpManager->RemoveGump(this);
}
//----------------------------------------------------------------------------