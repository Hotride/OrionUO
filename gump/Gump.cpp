/****************************************************************************
**
** Gump.cpp
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
//---------------------------------------------------------------------------
#include "stdafx.h"

TGump *g_ResizedGump = NULL;
//---------------------------------------------------------------------------
TGump::TGump(GUMP_TYPE type, DWORD serial, short x, short y)
: TRenderObject(serial, 0, 0, x, y), m_GumpType(type), m_ID(0), m_MinimizedX(0),
m_MinimizedY(0), m_NoClose(false), m_NoMove(false), m_Minimized(false),
m_FrameCreated(false), m_FrameRedraw(false), m_Blocked(false), m_LockMoving(false)
{
}
//---------------------------------------------------------------------------
TGump::~TGump()
{
	//Если это гамп, блокирующий игровое окно
	if (m_Blocked)
	{
		//Уменьшаем счетчик блокирующих гампов
		g_GrayMenuCount--;

		//Если таких гампов больше нет - восстанавливаем игровой экран
		if (g_GrayMenuCount <= 0)
		{
			g_GrayMenuCount = 0;
			g_GameState = GS_GAME;
			CurrentScreen = GameScreen;
		}
	}
}
//---------------------------------------------------------------------------
void TGump::CalculateGumpState()
{
	DWORD index = (DWORD)this;

	g_GumpPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index && g_LastSelectedGump == index);
	g_GumpPressedScroller = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index);
	g_GumpSelectElement = ((g_LastSelectedGump == index) ? g_LastSelectedObject : 0);
	g_GumpPressedElement = ((g_GumpPressed && g_LastObjectLeftMouseDown == g_LastSelectedObject) ? g_LastObjectLeftMouseDown : 0);

	if (g_LastObjectType == SOT_TEXT_OBJECT)
		g_GumpSelectElement = false;

	if (CanBeMoved() && g_LeftMouseDown && g_LastGumpLeftMouseDown == index && !g_LastObjectLeftMouseDown)
	{
		g_GumpMovingOffsetX = g_MouseX - g_DroppedLeftMouseX;
		g_GumpMovingOffsetY = g_MouseY - g_DroppedLeftMouseY;
	}
	else
	{
		g_GumpMovingOffsetX = 0;
		g_GumpMovingOffsetY = 0;
	}

	if (m_Minimized)
	{
		g_GumpTranslateX = (float)(m_MinimizedX + g_GumpMovingOffsetX);
		g_GumpTranslateY = (float)(m_MinimizedY + g_GumpMovingOffsetY);
	}
	else
	{
		g_GumpTranslateX = (float)(m_X + g_GumpMovingOffsetX);
		g_GumpTranslateY = (float)(m_Y + g_GumpMovingOffsetY);
	}
}
//---------------------------------------------------------------------------
void TGump::DrawLocker(int &x, int &y)
{
	if (g_ShowGumpLocker)
	{
		g_GL.OldTexture = 0;
		g_GL.Draw(g_TextureGumpState[m_LockMoving], x, y, 10, 14);
	}
}
//---------------------------------------------------------------------------
void TGump::GetBaseProperties(DWORD &serial, WORD &graphic, WORD &color, WORD &x, WORD &y)
{
	serial = m_Serial;
	graphic = m_Graphic;
	color = m_Color;
	x = m_X;
	y = m_Y;
}
//---------------------------------------------------------------------------
bool TGump::CanBeMoved()
{
	bool result = true;

	if (ConfigManager.LockGumpsMoving)
		result = !m_LockMoving;

	return result;
}
//---------------------------------------------------------------------------
int TGump::CalculateScrollerAndTextPosition(int &currentLine, int &visibleLines, int &maxY, int currentY)
{
	//Без смещения
	int scrollerY = 0;

	//Если не выходим за рамки ограничения отрисовки
	if (currentY < maxY)
	{
		//Если скроллер смещен
		if (currentY > 0)
		{
			//Вычисляем процент, на сколько его сместили
			float per = (currentY / (float)maxY) * 100.0f;

			//Вычисляем значение текущей отображаемой линии
			int jsize = (int)((visibleLines * per) / 100.0f);

			//Корректируем значения (при необходимости)
			if (jsize < 1)
				jsize = 1;
			else if (jsize > visibleLines)
				jsize = visibleLines;

			//Выставляем текущее значение линии
			currentLine = jsize;

			//Вычисляем положение скроллера	
			if (jsize >= visibleLines) //Максимальное - если достигли конца текста/объектов
				scrollerY = maxY;
			else //Или где-то в заданных пределах
				scrollerY = (int)((maxY * per) / 100.0f);
		}
		else //Обнуляем все, если скроллер не передвигали
		{
			scrollerY = 0;
			currentLine = 0;
		}
	}
	else //Вышли за допустимые пределы, выставляем все по максимуму
	{
		currentLine = visibleLines;
		scrollerY = maxY;
	}

	//Возвращаем позицию скроллера
	return scrollerY;
}
//---------------------------------------------------------------------------
int TGump::CalculateScrollerY(int &currentLine, int &visibleLines, int &maxY)
{
	//Действия, аналогичные коду выше при смещении скроллера
	float per = (currentLine / (float)visibleLines) * 100.0f;

	int jsize = (int)((visibleLines * per) / 100.0f);

	if (jsize < 1)
		jsize = 1;
	else if (jsize > visibleLines)
		jsize = visibleLines;

	int scrollerY = 0;

	if (jsize >= visibleLines)
		scrollerY = maxY;
	else
		scrollerY = (int)((maxY * per) / 100.0f);

	return scrollerY;
}
//---------------------------------------------------------------------------
void TGump::UpdateFrame()
{
	//Если метод рендеринга не рисование - отмечаем, что гамп нужно будет перерисовать перед отображением
	if (!g_DrawMode)
	{
		m_FrameRedraw = false;
		m_FrameCreated = false;
	}
	else
	{
		//Перерисовка мрэймов в зависимости от типа гампа (свернут/стандартное отображение)
		if (m_Minimized && m_GumpType != GT_MINIMAP)
			GenerateFrame(m_MinimizedX, m_MinimizedY);
		else
			GenerateFrame(X, Y);
	}
}
//---------------------------------------------------------------------------
void TGump::SendGumpResponse(int index)
{
	//Ответ на гамп
	TPacketGumpResponse packet(this, index);
	packet.Send();
	packet.Free();

	//Удаляем использованный гамп
	GumpManager->RemoveGump(this);
}
//---------------------------------------------------------------------------
void TGump::SendMenuResponse(int index)
{
	//Ответ на меню
	TPacketMenuResponse packet(this, index);
	packet.Send();

	//Удаляем использованный гамп
	GumpManager->RemoveGump(this);
}
//---------------------------------------------------------------------------
void TGump::SendTradingResponse(int code)
{
	//Ответ на трэйд окно
	TPacketTradeResponse packet(this, code);
	if (code == 1) //Закрываем окно
		GumpManager->RemoveGump(this);

	packet.Send();
}
//---------------------------------------------------------------------------
void TGump::SendTextEntryDialogResponse(bool mode)
{
	TGumpTextEntryDialog *gted = (TGumpTextEntryDialog*)this;

	if (gted->TextEntry != NULL) //Непредвиденная ошибка при отсутствии поля ввода текста в гампе
	{
		//Отправляем ответ на ентри диалог
		TPacketTextEntryDialogResponse packet(gted, mode);
		packet.Send();
		packet.Free();
	}

	//Удаляем использованный гамп
	GumpManager->RemoveGump(this);
}
//---------------------------------------------------------------------------
void TGump::SendRenameRequest()
{
	TGumpStatusbar *gsb = (TGumpStatusbar*)this;

	if (gsb->TextEntry->Length()) //Если в поле для ввода текста что-то есть
	{
		//Отправляем запрос на изменение имени
		TPacketRenameRequest packet(gsb->Serial, string(gsb->TextEntry->c_str()));
		packet.Send();
	}
}
//---------------------------------------------------------------------------
void TGump::SendSellList()
{
	WORD size = 9;
	WORD count = 0;

	TGumpSellObject *go = (TGumpSellObject*)m_Items;

	while (go != NULL)
	{
		if (go->SelectedCount)
		{
			size += 6;
			count++;
		}

		go = (TGumpSellObject*)go->m_Next;
	}

	PBYTE buf = new BYTE[size];
	*buf = 0x9F;
	pack16(buf + 1, size);
	pack32(buf + 3, m_Serial);
	pack16(buf + 7, count);
	
	PBYTE ptr = buf + 9;

	go = (TGumpSellObject*)m_Items;

	while (go != NULL)
	{
		if (go->SelectedCount)
		{
			pack32(ptr, go->Serial);
			ptr += 4;
			pack16(ptr, go->SelectedCount);
			ptr += 2;
		}

		go = (TGumpSellObject*)go->m_Next;
	}

	UO->Send(buf, size);

	delete buf;
}
//---------------------------------------------------------------------------
void TGump::SendBuyList()
{
	TGameCharacter *vendor = World->FindWorldCharacter(m_Serial);

	if (vendor == NULL)
		return;

	WORD size = 8;
	WORD count = 0;

	int Layers[2] = { OL_BUY_RESTOCK, OL_BUY };

	IFOR(i, 0, 2)
	{
		TGameItem *box = vendor->FindLayer(Layers[i]);

		if (box != NULL)
		{
			box = (TGameItem*)box->m_Items;

			while (box != NULL)
			{
				TShopItem *si = box->ShopItem;

				if (si != NULL && si->Count > 0)
				{
					size += 7;
					count++;
				}

				box = (TGameItem*)box->m_Next;
			}
		}
	}
	
	PBYTE buf = new BYTE[size];
	*buf = 0x3B;
	pack16(buf + 1, size);
	pack32(buf + 3, GetSerial());

	if (count)
	{
		buf[7] = 0x02;
	
		PBYTE ptr = buf + 8;
	
		IFOR(i, 0, 2)
		{
			TGameItem *box = vendor->FindLayer(Layers[i]);

			if (box != NULL)
			{
				box = (TGameItem*)box->m_Items;

				while (box != NULL)
				{
					TShopItem *si = box->ShopItem;

					if (si != NULL && si->Count > 0)
					{
						*ptr = 0x1A;
						ptr++;
						pack32(ptr, box->Serial);
						ptr += 4;
						pack16(ptr, si->Count);
						ptr += 2;
					}

					box = (TGameItem*)box->m_Next;
				}
			}
		}
	}
	else buf[7] = 0x00;

	UO->Send(buf, size);

	delete buf;
}
//---------------------------------------------------------------------------
void TGump::SendTipRequest(BYTE flag)
{
	//Отправляем запрос диалога Tip/Updates
	TPacketTipRequest packet((WORD)Serial, flag);
	packet.Send();

	//Удаляем использованный гамп
	GumpManager->RemoveGump(this);
}
//---------------------------------------------------------------------------