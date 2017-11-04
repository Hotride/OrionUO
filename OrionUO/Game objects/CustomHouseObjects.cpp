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
		&m_Category, &m_Style, &m_TID, &m_South1, &m_South2, &m_South3, &m_Corner, &m_East1, &m_East2, &m_East3, &m_Post, &m_WindowS, &m_AltWindowS, &m_WindowE, &m_AltWindowE, &m_SecondAltWindowS, &m_SecondAltWindowE, &m_FeatureMask) >= 17;

	if (result)
	{
		m_Graphics[0] = m_South1;
		m_Graphics[1] = m_South2;
		m_Graphics[2] = m_South3;
		m_Graphics[3] = m_Corner;
		m_Graphics[4] = m_East1;
		m_Graphics[5] = m_East2;
		m_Graphics[6] = m_East3;
		m_Graphics[7] = m_Post;

		memcpy(&m_WindowGraphics[0], &m_Graphics[0], sizeof(m_WindowGraphics));

		if (!m_AltWindowE && m_WindowE)
		{
			m_AltWindowE = m_WindowE;
			m_WindowE = 0;
		}

		if (m_WindowS)
			m_WindowGraphics[0] = m_WindowS;

		if (m_AltWindowS)
			m_WindowGraphics[1] = m_AltWindowS;

		if (m_SecondAltWindowS)
			m_WindowGraphics[2] = m_SecondAltWindowS;

		if (m_WindowE)
			m_WindowGraphics[4] = m_WindowE;

		if (m_AltWindowE)
			m_WindowGraphics[5] = m_AltWindowE;

		if (m_SecondAltWindowE)
			m_WindowGraphics[6] = m_SecondAltWindowE;
	}

	return result;
}
//----------------------------------------------------------------------------------
int CCustomHouseObjectWall::Contains(const ushort &graphic) const
{
	IFOR(i, 0, GRAPHICS_COUNT)
	{
		if (m_Graphics[i] == graphic || m_WindowGraphics[i] == graphic)
			return i;
	}

	return -1;
}
//----------------------------------------------------------------------------------
bool CCustomHouseObjectFloor::Parse(const char *text)
{
	WISPFUN_DEBUG("");
	bool result = sscanf(text,
		"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
		&m_Category, &m_F1, &m_F2, &m_F3, &m_F4, &m_F5, &m_F6, &m_F7, &m_F8, &m_F9, &m_F10, &m_F11, &m_F12, &m_F13, &m_F14, &m_F15, &m_F16, &m_FeatureMask) >= 17;

	if (result)
	{
		m_Graphics[0] = m_F1;
		m_Graphics[1] = m_F2;
		m_Graphics[2] = m_F3;
		m_Graphics[3] = m_F4;
		m_Graphics[4] = m_F5;
		m_Graphics[5] = m_F6;
		m_Graphics[6] = m_F7;
		m_Graphics[7] = m_F8;
		m_Graphics[8] = m_F9;
		m_Graphics[9] = m_F10;
		m_Graphics[10] = m_F11;
		m_Graphics[11] = m_F12;
		m_Graphics[12] = m_F13;
		m_Graphics[13] = m_F14;
		m_Graphics[14] = m_F15;
		m_Graphics[15] = m_F16;
	}

	return result;
}
//----------------------------------------------------------------------------------
int CCustomHouseObjectFloor::Contains(const ushort &graphic) const
{
	IFOR(i, 0, GRAPHICS_COUNT)
	{
		if (m_Graphics[i] == graphic)
			return i;
	}

	return -1;
}
//----------------------------------------------------------------------------------
bool CCustomHouseObjectDoor::Parse(const char *text)
{
	WISPFUN_DEBUG("");
	bool result = sscanf(text,
		"%d %d %d %d %d %d %d %d %d %d",
		&m_Category, &m_Piece1, &m_Piece2, &m_Piece3, &m_Piece4, &m_Piece5, &m_Piece6, &m_Piece7, &m_Piece8, &m_FeatureMask) >= 9;

	if (result)
	{
		m_Graphics[0] = m_Piece1;
		m_Graphics[1] = m_Piece2;
		m_Graphics[2] = m_Piece3;
		m_Graphics[3] = m_Piece4;
		m_Graphics[4] = m_Piece5;
		m_Graphics[5] = m_Piece6;
		m_Graphics[6] = m_Piece7;
		m_Graphics[7] = m_Piece8;
	}

	return result;
}
//----------------------------------------------------------------------------------
int CCustomHouseObjectDoor::Contains(const ushort &graphic) const
{
	IFOR(i, 0, GRAPHICS_COUNT)
	{
		if (m_Graphics[i] == graphic)
			return i;
	}

	return -1;
}
//----------------------------------------------------------------------------------
bool CCustomHouseObjectMisc::Parse(const char *text)
{
	WISPFUN_DEBUG("");
	bool result = sscanf(text,
		"%d %d %d %d %d %d %d %d %d %d %d %d",
		&m_Category, &m_Style, &m_TID, &m_Piece1, &m_Piece2, &m_Piece3, &m_Piece4, &m_Piece5, &m_Piece6, &m_Piece7, &m_Piece8, &m_FeatureMask) >= 11;

	if (result)
	{
		m_Graphics[0] = m_Piece1;
		m_Graphics[1] = m_Piece2;
		m_Graphics[2] = m_Piece3;
		m_Graphics[3] = m_Piece4;
		m_Graphics[4] = m_Piece5;
		m_Graphics[5] = m_Piece6;
		m_Graphics[6] = m_Piece7;
		m_Graphics[7] = m_Piece8;
	}

	return result;
}
//----------------------------------------------------------------------------------
int CCustomHouseObjectMisc::Contains(const ushort &graphic) const
{
	IFOR(i, 0, GRAPHICS_COUNT)
	{
		if (m_Graphics[i] == graphic)
			return i;
	}

	return -1;
}
//----------------------------------------------------------------------------------
bool CCustomHouseObjectStair::Parse(const char *text)
{
	WISPFUN_DEBUG("");
	bool result = sscanf(text,
		"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
		&m_Category, &m_Block, &m_North, &m_East, &m_South, &m_West, &m_Squared1, &m_Squared2, &m_Rounded1, &m_Rounded2, &m_MultiNorth, &m_MultiEast, &m_MultiSouth, &m_MultiWest, &m_FeatureMask) >= 14;

	if (result)
	{
		m_Graphics[0] = (m_MultiNorth ? m_Squared1 : 0);
		m_Graphics[1] = (m_MultiEast ? m_Squared2 : 0);
		m_Graphics[2] = (m_MultiSouth ? m_Rounded1 : 0);
		m_Graphics[3] = (m_MultiWest ? m_Rounded2 : 0);
		m_Graphics[4] = m_Block;
		m_Graphics[5] = m_North;
		m_Graphics[6] = m_East;
		m_Graphics[7] = m_South;
		m_Graphics[8] = m_West;
	}

	return result;
}
//----------------------------------------------------------------------------------
int CCustomHouseObjectStair::Contains(const ushort &graphic) const
{
	IFOR(i, 0, GRAPHICS_COUNT)
	{
		if (m_Graphics[i] == graphic)
			return i;
	}

	return -1;
}
//----------------------------------------------------------------------------------
bool CCustomHouseObjectTeleport::Parse(const char *text)
{
	WISPFUN_DEBUG("");
	bool result = sscanf(text,
		"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
		&m_Category, &m_F1, &m_F2, &m_F3, &m_F4, &m_F5, &m_F6, &m_F7, &m_F8, &m_F9, &m_F10, &m_F11, &m_F12, &m_F13, &m_F14, &m_F15, &m_F16, &m_FeatureMask) >= 17;

	if (result)
	{
		m_Graphics[0] = m_F1;
		m_Graphics[1] = m_F2;
		m_Graphics[2] = m_F3;
		m_Graphics[3] = m_F4;
		m_Graphics[4] = m_F5;
		m_Graphics[5] = m_F6;
		m_Graphics[6] = m_F7;
		m_Graphics[7] = m_F8;
		m_Graphics[8] = m_F9;
		m_Graphics[9] = m_F10;
		m_Graphics[10] = m_F11;
		m_Graphics[11] = m_F12;
		m_Graphics[12] = m_F13;
		m_Graphics[13] = m_F14;
		m_Graphics[14] = m_F15;
		m_Graphics[15] = m_F16;
	}

	return result;
}
//----------------------------------------------------------------------------------
int CCustomHouseObjectTeleport::Contains(const ushort &graphic) const
{
	IFOR(i, 0, GRAPHICS_COUNT)
	{
		if (m_Graphics[i] == graphic)
			return i;
	}

	return -1;
}
//----------------------------------------------------------------------------------
bool CCustomHouseObjectRoof::Parse(const char *text)
{
	WISPFUN_DEBUG("");
	bool result = sscanf(text,
		"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
		&m_Category, &m_Style, &m_TID, &m_North, &m_East, &m_South, &m_West, &m_NSCrosspiece, &m_EWCrosspiece, &m_NDent, &m_SDent, &m_WDent, &m_NTPiece, &m_ETPiece, &m_STPiece, &m_WTPiece, &m_XPiece, &m_Extra, &m_Piece, &m_FeatureMask) >= 19;

	if (result)
	{
		m_Graphics[0] = m_North;
		m_Graphics[1] = m_East;
		m_Graphics[2] = m_South;
		m_Graphics[3] = m_West;
		m_Graphics[4] = m_NSCrosspiece;
		m_Graphics[5] = m_EWCrosspiece;
		m_Graphics[6] = m_NDent;
		m_Graphics[7] = m_SDent;
		m_Graphics[8] = m_WDent;
		m_Graphics[9] = m_NTPiece;
		m_Graphics[10] = m_ETPiece;
		m_Graphics[11] = m_STPiece;
		m_Graphics[12] = m_WTPiece;
		m_Graphics[13] = m_XPiece;
		m_Graphics[14] = m_Extra;
		m_Graphics[15] = m_Piece;
	}

	return result;
}
//----------------------------------------------------------------------------------
int CCustomHouseObjectRoof::Contains(const ushort &graphic) const
{
	IFOR(i, 0, GRAPHICS_COUNT)
	{
		if (m_Graphics[i] == graphic)
			return i;
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
		&buf[0], &m_Graphic, &m_Top, &m_Bottom, &m_AdjUN, &m_AdjLN, &m_AdjUE, &m_AdjLE, &m_AdjUS, &m_AdjLS, &m_AdjUW, &m_AdjLW, &m_DirectSupports, &m_CanGoW, &m_CanGoN, &m_CanGoNWS) >= 16;

	if (result)
		m_Graphics[0] = m_Graphic;

	return result;
}
//----------------------------------------------------------------------------------
int CCustomHouseObjectPlaceInfo::Contains(const ushort &graphic) const
{
	IFOR(i, 0, GRAPHICS_COUNT)
	{
		if (m_Graphics[i] == graphic)
			return i;
	}

	return -1;
}
//----------------------------------------------------------------------------------
