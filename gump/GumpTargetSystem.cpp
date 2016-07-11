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
void TGumpTargetSystem::GenerateFrame()
{
	if (!g_DrawMode || ConfigManager.DisableNewTargetSystem || !NewTargetSystem.Serial)
	{
		m_FrameRedraw = false;
		m_FrameCreated = false;

		return;
	}

	CalculateGumpState();

	glNewList((GLuint)this, GL_COMPILE);

		TGameObject *obj = World->FindWorldObject(NewTargetSystem.Serial);
		if (obj != NULL)
		{
			//Вычисляем цвет статусбара
			WORD color = 0;
			TGameCharacter *character = NULL;
			
			if (obj->NPC)
			{
				character = obj->GameCharacterPtr();
				NOTORIETY_TYPE noto = (NOTORIETY_TYPE)character->Notoriety;

				color = ConfigManager.GetColorByNotoriety(noto);

				if (noto == NT_CRIMINAL || noto == NT_SOMEONE_GRAY)
					color = 0;
			}

			ColorizerShader->Use();

			//Гамп статус бара
			UO->DrawGump(0x0804, color, 0, 0);

			UnuseShader();

			m_OldName = obj->GetName();

			if (!obj->NPC && !m_OldName.length())
			{
				STATIC_TILES *st = obj->StaticGroupObjectPtr()->GetStaticData();

				m_OldName = ClilocManager->Cliloc(g_Language)->GetA(102000 + obj->Graphic, st->Name);
			}

			FontManager->DrawA(1, m_OldName.c_str(), 0x0386, 16, 14, 150, TS_LEFT, UOFONT_FIXED);

			//Hits
			UO->DrawGump(0x0805, 0, 34, 38);
				
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

					UO->DrawGump(gumpid, 0, 34, 38, per, 0);
				}
			}
		}
		else //Серенький статус
		{
			ColorizerShader->Use();

			//Гамп статус бара
			UO->DrawGump(0x0804, 0x0386, 0, 0);

			//Hits
			UO->DrawGump(0x0805, 0x0386, 34, 38);
				
			UnuseShader();

			FontManager->DrawA(1, m_OldName.c_str(), 0x0386, 16, 14, 150, TS_LEFT, UOFONT_FIXED);
		}

	glEndList();
	
	m_FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpTargetSystem::Draw(bool &mode)
{
	if (m_Serial != NewTargetSystem.Serial)
		m_Serial = NewTargetSystem.Serial;

	if (ConfigManager.DisableNewTargetSystem || !NewTargetSystem.Serial)
		return 0;

	DWORD index = (DWORD)this;

	CalculateGumpState();

	//Если гамп захватили и (может быть) двигают
	if (g_GumpMovingOffsetX && Target.IsTargeting())
		g_GeneratedMouseDown = true;
	else if (g_GumpMovingOffsetY && Target.IsTargeting())
		g_GeneratedMouseDown = true;

	if (mode) //Отрисовка
	{
		//Если список отображения не был сделан - сделаем его
		if (!m_FrameCreated)
			GenerateFrame();
		else if (m_FrameRedraw)
		{
			GenerateFrame();
			m_FrameRedraw = false;
		}

		glTranslatef(g_GumpTranslateX, g_GumpTranslateY, 0.0f);

		//Рисуем заранее заготовленный список отображения
		glCallList((GLuint)index);

		DrawLocker();

		glTranslatef(-g_GumpTranslateX, -g_GumpTranslateY, 0.0f);
	}
	else //Выбор объектов
	{
		int LSG = 0;

		if (UO->GumpPixelsInXY(0x0804, (int)g_GumpTranslateX, (int)g_GumpTranslateY))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
			g_StatusbarUnderMouse = index;
		}

		if (g_ShowGumpLocker && UO->PolygonePixelsInXY((int)g_GumpTranslateX, (int)g_GumpTranslateY, 10, 14))
			LSG = ID_GSB_LOCK_MOVING;

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
		m_LockMoving = !m_LockMoving;
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