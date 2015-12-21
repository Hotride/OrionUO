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
	NoClose = true;
	GumpType = GT_DYE;
	Graphic = graphic;
}
//----------------------------------------------------------------------------
TGumpDye::~TGumpDye()
{
}
//----------------------------------------------------------------------------
void TGumpDye::OnSelectColor(WORD &color)
{
	TPacketDyeDataResponse packet(Serial, Graphic, color);
	packet.Send();

	GumpManager->RemoveGump(this);
}
//----------------------------------------------------------------------------