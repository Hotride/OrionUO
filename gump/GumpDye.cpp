/****************************************************************************
**
** GumpSelectColor.cpp
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
//----------------------------------------------------------------------------
TGumpDye::TGumpDye(DWORD serial, short x, short y, WORD graphic)
: TGumpSelectColor(serial, x, y, SCGS_OPT_POPUP_TEXT)
{
	m_NoClose = true;
	m_GumpType = GT_DYE;
	m_Graphic = graphic;
}
//----------------------------------------------------------------------------
TGumpDye::~TGumpDye()
{
}
//----------------------------------------------------------------------------
void TGumpDye::GenerateFrame(int posX, int posY)
{
	if (!g_DrawMode)
	{
		m_FrameRedraw = false;
		m_FrameCreated = false;

		return;
	}

	CalculateGumpState();

	glNewList((GLuint)this, GL_COMPILE);

		TGumpSelectColor::GenerateFrame(posX, posY);

		WORD startColor = m_ColorRef + 2;
		WORD color = 0;

		IFOR(y, 0, 10 && !color)
		{
			IFOR(x, 0, 20)
			{
				if (m_SelectedIndex == ID_GSC_COLORS + (x * 30 + y))
				{
					color = startColor;
					break;
				}

				startColor += 5;
			}
		}

		ColorizerShader->Use();

		UO->DrawStaticArtInContainer(0x0FAB, color, 200, 58);

		UnuseShader();

	glEndList();

	m_FrameRedraw = true;
	m_FrameCreated = true;
}
//----------------------------------------------------------------------------
void TGumpDye::OnSelectColor(WORD &color)
{
	TPacketDyeDataResponse packet(m_Serial, m_Graphic, color);
	packet.Send();

	GumpManager->RemoveGump(this);
}
//----------------------------------------------------------------------------