// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** CustomHouseObjects.cpp
**
** Copyright (C) September 2017 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
bool CCustomHouseObjectWall::Parse(const char *text)
{
	WISPFUN_DEBUG("");
	bool result = sscanf(text,
		"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
		&Category, &Style, &TID, &South1, &South2, &South3, &Corner, &East1, &East2, &East3, &Post, &WindowS, &AltWindowS, &WindowE, &AltWindowE, &SecondAltWindowS, &SecondAltWindowE, &FeatureMask) >= 17;

	if (result)
	{
		m_Graphics[0] = South1;
		m_Graphics[1] = South2;
		m_Graphics[2] = South3;
		m_Graphics[3] = Corner;
		m_Graphics[4] = East1;
		m_Graphics[5] = East2;
		m_Graphics[6] = East3;
		m_Graphics[7] = Post;

		memcpy(&m_WindowGraphics[0], &m_Graphics[0], sizeof(m_WindowGraphics));

		if (!AltWindowE && WindowE)
		{
			AltWindowE = WindowE;
			WindowE = 0;
		}

		if (WindowS)
			m_WindowGraphics[0] = WindowS;

		if (AltWindowS)
			m_WindowGraphics[1] = AltWindowS;

		if (SecondAltWindowS)
			m_WindowGraphics[2] = SecondAltWindowS;

		if (WindowE)
			m_WindowGraphics[4] = WindowE;

		if (AltWindowE)
			m_WindowGraphics[5] = AltWindowE;

		if (SecondAltWindowE)
			m_WindowGraphics[6] = SecondAltWindowE;
	}

	return result;
}
//----------------------------------------------------------------------------------
int CCustomHouseObjectWall::Contains(const ushort &graphic) const
{
	IFOR(i, 0, GRAPHICS_COUNT)
	{
		if (m_Graphics[i] == graphic || m_WindowGraphics[i] == graphic)
			return (int)i;
	}

	return -1;
}
//----------------------------------------------------------------------------------
bool CCustomHouseObjectFloor::Parse(const char *text)
{
	WISPFUN_DEBUG("");
	bool result = sscanf(text,
		"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
		&Category, &F1, &F2, &F3, &F4, &F5, &F6, &F7, &F8, &F9, &F10, &F11, &F12, &F13, &F14, &F15, &F16, &FeatureMask) >= 17;

	if (result)
	{
		m_Graphics[0] = F1;
		m_Graphics[1] = F2;
		m_Graphics[2] = F3;
		m_Graphics[3] = F4;
		m_Graphics[4] = F5;
		m_Graphics[5] = F6;
		m_Graphics[6] = F7;
		m_Graphics[7] = F8;
		m_Graphics[8] = F9;
		m_Graphics[9] = F10;
		m_Graphics[10] = F11;
		m_Graphics[11] = F12;
		m_Graphics[12] = F13;
		m_Graphics[13] = F14;
		m_Graphics[14] = F15;
		m_Graphics[15] = F16;
	}

	return result;
}
//----------------------------------------------------------------------------------
int CCustomHouseObjectFloor::Contains(const ushort &graphic) const
{
	IFOR(i, 0, GRAPHICS_COUNT)
	{
		if (m_Graphics[i] == graphic)
			return (int)i;
	}

	return -1;
}
//----------------------------------------------------------------------------------
bool CCustomHouseObjectDoor::Parse(const char *text)
{
	WISPFUN_DEBUG("");
	bool result = sscanf(text,
		"%d %d %d %d %d %d %d %d %d %d",
		&Category, &Piece1, &Piece2, &Piece3, &Piece4, &Piece5, &Piece6, &Piece7, &Piece8, &FeatureMask) >= 9;

	if (result)
	{
		m_Graphics[0] = Piece1;
		m_Graphics[1] = Piece2;
		m_Graphics[2] = Piece3;
		m_Graphics[3] = Piece4;
		m_Graphics[4] = Piece5;
		m_Graphics[5] = Piece6;
		m_Graphics[6] = Piece7;
		m_Graphics[7] = Piece8;
	}

	return result;
}
//----------------------------------------------------------------------------------
int CCustomHouseObjectDoor::Contains(const ushort &graphic) const
{
	IFOR(i, 0, GRAPHICS_COUNT)
	{
		if (m_Graphics[i] == graphic)
			return (int)i;
	}

	return -1;
}
//----------------------------------------------------------------------------------
bool CCustomHouseObjectMisc::Parse(const char *text)
{
	WISPFUN_DEBUG("");
	bool result = sscanf(text,
		"%d %d %d %d %d %d %d %d %d %d %d %d",
		&Category, &Style, &TID, &Piece1, &Piece2, &Piece3, &Piece4, &Piece5, &Piece6, &Piece7, &Piece8, &FeatureMask) >= 11;

	if (result)
	{
		m_Graphics[0] = Piece1;
		m_Graphics[1] = Piece2;
		m_Graphics[2] = Piece3;
		m_Graphics[3] = Piece4;
		m_Graphics[4] = Piece5;
		m_Graphics[5] = Piece6;
		m_Graphics[6] = Piece7;
		m_Graphics[7] = Piece8;
	}

	return result;
}
//----------------------------------------------------------------------------------
int CCustomHouseObjectMisc::Contains(const ushort &graphic) const
{
	IFOR(i, 0, GRAPHICS_COUNT)
	{
		if (m_Graphics[i] == graphic)
			return (int)i;
	}

	return -1;
}
//----------------------------------------------------------------------------------
bool CCustomHouseObjectStair::Parse(const char *text)
{
	WISPFUN_DEBUG("");
	bool result = sscanf(text,
		"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
		&Category, &Block, &North, &East, &South, &West, &Squared1, &Squared2, &Rounded1, &Rounded2, &MultiNorth, &MultiEast, &MultiSouth, &MultiWest, &FeatureMask) >= 14;

	if (result)
	{
		m_Graphics[0] = (MultiNorth ? Squared1 : 0);
		m_Graphics[1] = (MultiEast ? Squared2 : 0);
		m_Graphics[2] = (MultiSouth ? Rounded1 : 0);
		m_Graphics[3] = (MultiWest ? Rounded2 : 0);
		m_Graphics[4] = Block;
		m_Graphics[5] = North;
		m_Graphics[6] = East;
		m_Graphics[7] = South;
		m_Graphics[8] = West;
	}

	return result;
}
//----------------------------------------------------------------------------------
int CCustomHouseObjectStair::Contains(const ushort &graphic) const
{
	IFOR(i, 0, GRAPHICS_COUNT)
	{
		if (m_Graphics[i] == graphic)
			return (int)i;
	}

	return -1;
}
//----------------------------------------------------------------------------------
bool CCustomHouseObjectTeleport::Parse(const char *text)
{
	WISPFUN_DEBUG("");
	bool result = sscanf(text,
		"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
		&Category, &F1, &F2, &F3, &F4, &F5, &F6, &F7, &F8, &F9, &F10, &F11, &F12, &F13, &F14, &F15, &F16, &FeatureMask) >= 17;

	if (result)
	{
		m_Graphics[0] = F1;
		m_Graphics[1] = F2;
		m_Graphics[2] = F3;
		m_Graphics[3] = F4;
		m_Graphics[4] = F5;
		m_Graphics[5] = F6;
		m_Graphics[6] = F7;
		m_Graphics[7] = F8;
		m_Graphics[8] = F9;
		m_Graphics[9] = F10;
		m_Graphics[10] = F11;
		m_Graphics[11] = F12;
		m_Graphics[12] = F13;
		m_Graphics[13] = F14;
		m_Graphics[14] = F15;
		m_Graphics[15] = F16;
	}

	return result;
}
//----------------------------------------------------------------------------------
int CCustomHouseObjectTeleport::Contains(const ushort &graphic) const
{
	IFOR(i, 0, GRAPHICS_COUNT)
	{
		if (m_Graphics[i] == graphic)
			return (int)i;
	}

	return -1;
}
//----------------------------------------------------------------------------------
bool CCustomHouseObjectRoof::Parse(const char *text)
{
	WISPFUN_DEBUG("");
	bool result = sscanf(text,
		"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
		&Category, &Style, &TID, &North, &East, &South, &West, &NSCrosspiece, &EWCrosspiece, &NDent, &SDent, &WDent, &NTPiece, &ETPiece, &STPiece, &WTPiece, &XPiece, &Extra, &Piece, &FeatureMask) >= 19;

	if (result)
	{
		m_Graphics[0] = North;
		m_Graphics[1] = East;
		m_Graphics[2] = South;
		m_Graphics[3] = West;
		m_Graphics[4] = NSCrosspiece;
		m_Graphics[5] = EWCrosspiece;
		m_Graphics[6] = NDent;
		m_Graphics[7] = SDent;
		m_Graphics[8] = WDent;
		m_Graphics[9] = NTPiece;
		m_Graphics[10] = ETPiece;
		m_Graphics[11] = STPiece;
		m_Graphics[12] = WTPiece;
		m_Graphics[13] = XPiece;
		m_Graphics[14] = Extra;
		m_Graphics[15] = Piece;
	}

	return result;
}
//----------------------------------------------------------------------------------
int CCustomHouseObjectRoof::Contains(const ushort &graphic) const
{
	IFOR(i, 0, GRAPHICS_COUNT)
	{
		if (m_Graphics[i] == graphic)
			return (int)i;
	}

	return -1;
}
//----------------------------------------------------------------------------------
bool CCustomHouseObjectPlaceInfo::Parse(const char *text)
{
	WISPFUN_DEBUG("");
	char buf[50] = { 0 };

	bool result = sscanf(text,
		"%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
		&buf[0], &Graphic, &Top, &Bottom, &AdjUN, &AdjLN, &AdjUE, &AdjLE, &AdjUS, &AdjLS, &AdjUW, &AdjLW, &DirectSupports, &CanGoW, &CanGoN, &CanGoNWS) >= 16;

	if (result)
		m_Graphics[0] = Graphic;

	return result;
}
//----------------------------------------------------------------------------------
int CCustomHouseObjectPlaceInfo::Contains(const ushort &graphic) const
{
	IFOR(i, 0, GRAPHICS_COUNT)
	{
		if (m_Graphics[i] == graphic)
			return (int)i;
	}

	return -1;
}
//----------------------------------------------------------------------------------
