/****************************************************************************
**
** GumpTargetSystem.cpp
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
TGumpTargetSystem::TGumpTargetSystem(DWORD serial, short x, short y)
: TGump(GT_TARGET_SYSTEM, serial, x, y), m_OldName("")
{
}
//---------------------------------------------------------------------------
TGumpTargetSystem::~TGumpTargetSystem()
{
	NewTargetSystem.GumpX = m_X;
	NewTargetSystem.GumpY = m_Y;
}
//---------------------------------------------------------------------------
void TGumpTargetSystem::PrepareTextures()
{
	UO->ExecuteGumpPart(0x0802, 8);
}
//---------------------------------------------------------------------------
void TGumpTargetSystem::GenerateFrame(int posX, int posY)
{
	if (!g_DrawMode || ConfigManager.DisableNewTargetSystem || !NewTargetSystem.Serial)
	{
		FrameRedraw = false;
		FrameCreated = false;

		return;
	}

	DWORD index = (DWORD)this;

	//Нажата ли кнопка в окне?
	bool IsPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index && g_LastSelectedGump == index);
	
	//Может ли быть нажат элемент?
	int CanPressedButton = 0;
	if (IsPressed && g_LastObjectLeftMouseDown == g_LastSelectedObject)
		CanPressedButton = g_LastObjectLeftMouseDown;

	glNewList((GLuint)index, GL_COMPILE);

		TGameObject *obj = World->FindWorldObject(NewTargetSystem.Serial);
		if (obj != NULL)
		{
			//Вычисляем цвет статусбара
			WORD color = 0;
			TGameCharacter *character = NULL;
			
			if (obj->NPC)
			{
				character = (TGameCharacter*)obj;
				NOTORIETY_TYPE noto = (NOTORIETY_TYPE)character->Notoriety;

				color = ConfigManager.GetColorByNotoriety(noto);

				if (noto == NT_CRIMINAL || noto == NT_SOMEONE_GRAY)
					color = 0;
			}

			ColorizerShader->Use();

			//Гамп статус бара
			UO->DrawGump(0x0804, color, posX, posY);

			UnuseShader();

			m_OldName = obj->GetName();

			if (!obj->NPC && !m_OldName.length())
			{
				STATIC_TILES *st = ((TRenderStaticObject*)obj)->GetStaticData();

				m_OldName = ClilocManager->Cliloc(g_Language)->GetA(102000 + obj->Graphic, st->Name);
			}

			FontManager->DrawA(1, m_OldName.c_str(), 0x0386, posX + 16, posY + 14, 150, TS_LEFT, UOFONT_FIXED);

			//Hits
			UO->DrawGump(0x0805, 0, posX + 34, posY + 38);
				
			if (character != NULL)
			{
				int per = CalculatePercents(character->MaxHits, character->Hits, 109);

				if (per > 0)
				{
					WORD gumpid = 0x0806; //Character status line (blue)
					if (obj->Poisoned())
						gumpid = 0x0808; //Character status line (green)
					else if (obj->YellowHits())
						gumpid = 0x0809; //Character status line (yellow)

					UO->DrawGump(gumpid, 0, posX + 34, posY + 38, per, 0);
				}
			}
		}
		else //Серенький статус
		{
			ColorizerShader->Use();

			//Гамп статус бара
			UO->DrawGump(0x0804, 0x0386, posX, posY);

			//Hits
			UO->DrawGump(0x0805, 0x0386, posX + 34, posY + 38);
				
			UnuseShader();

			FontManager->DrawA(1, m_OldName.c_str(), 0x0386, posX + 16, posY + 14, 150, TS_LEFT, UOFONT_FIXED);
		}

	glEndList();
	FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpTargetSystem::Draw(bool &mode)
{
	if (m_Serial != NewTargetSystem.Serial)
		m_Serial = NewTargetSystem.Serial;

	if (ConfigManager.DisableNewTargetSystem || !NewTargetSystem.Serial)
		return 0;

	DWORD index = (DWORD)this;

	//Для быстрого доступа
	int posX = m_X;
	int posY = m_Y;

	//Нажата ли кнопка в окне?
	bool IsPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index && g_LastSelectedGump == index);

	//Может ли быть нажат элемент?
	int CanPressedButton = 0;
	if (IsPressed && g_LastObjectLeftMouseDown == g_LastSelectedObject)
		CanPressedButton = g_LastObjectLeftMouseDown;

	//Если гамп захватили и (может быть) двигают
	if (CanBeMoved() && g_LeftMouseDown && g_LastGumpLeftMouseDown == index && !g_LastObjectLeftMouseDown)
	{
		int offsetX = g_MouseX - g_DroppedLeftMouseX;
		if (offsetX && Target.IsTargeting())
			g_GeneratedMouseDown = true;

		int offsetY = g_MouseY - g_DroppedLeftMouseY;
		if (offsetY && Target.IsTargeting())
			g_GeneratedMouseDown = true;

		posX += offsetX;
		posY += offsetY;

		//Окно может (и, скорее всего, должно) быть перерисовано
		if (mode)
			GenerateFrame(posX, posY);
	}
	else if (mode)
	{
		if (CanPressedButton != 0)
			GenerateFrame(posX, posY);
		else if (FrameRedraw)
		{
			GenerateFrame(posX, posY);
			FrameRedraw = false;
		}
	}

	if (mode) //Отрисовка
	{
		//Если список отображения не был сделан - сделаем его
		if (!FrameCreated)
			GenerateFrame(posX, posY);

		//Рисуем заранее заготовленный список отображения
		glCallList((GLuint)index);

		DrawLocker(posX, posY);
	}
	else //Выбор объектов
	{
		int LSG = 0;

		if (UO->GumpPixelsInXY(0x0804, posX, posY))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
			g_StatusbarUnderMouse = index;
		}

		if (g_ShowGumpLocker && UO->PolygonePixelsInXY(posX, posY, 10, 14))
		{
			g_LastSelectedObject = LSG = ID_GSB_LOCK_MOVING;
			g_LastSelectedGump = index;
		}

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpTargetSystem::OnLeftMouseDown()
{
	if (g_GeneratedMouseDown)
		return;

	if (!g_LastObjectLeftMouseDown)
	{
		//Проверим, может быть есть таргет, который нужно повесить на данного чара
		if (Target.IsTargeting())
		{
			Target.SendTargetObject(NewTargetSystem.Serial);
			g_CancelDoubleClick = true;
		}
	}
}
//----------------------------------------------------------------------------
void TGumpTargetSystem::OnLeftMouseUp()
{
	if (g_GeneratedMouseDown)
		return;

	if (g_LastObjectLeftMouseDown == ID_GSB_LOCK_MOVING)
	{
		LockMoving = !LockMoving;
		g_CancelDoubleClick = true;
	}
	else if (!g_LastObjectLeftMouseDown)
	{
		//Проверим, может быть есть таргет, который нужно повесить на данного чара
		/*if (Target.IsTargeting())
			Target.SendTargetObject(NewTargetSystem.Serial);*/
	}
}
//----------------------------------------------------------------------------
bool TGumpTargetSystem::OnLeftMouseDoubleClick()
{
	if (g_GeneratedMouseDown)
		return false;

	DWORD serial = NewTargetSystem.Serial;

	if (serial != g_PlayerSerial)
	{
		if (g_Player->Warmode)
		{
			if (serial < 0x40000000)
				UO->Attack(serial); //Если в вармоде - атакуем
		}
		else
			UO->DoubleClick(serial); //Или используем предмет

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------