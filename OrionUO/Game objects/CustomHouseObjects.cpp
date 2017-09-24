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
	return sscanf(text,
		"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
		&m_Category, &m_Style, &m_TID, &m_South1, &m_South2, &m_South3, &m_Corner, &m_Eats1, &m_Eats2, &m_Eats3, &m_Post, &m_WindowS, &m_AltWindowS, &m_WindowE, &m_AltWindowE, &m_SecondAltWindowS, &m_SecondAltWindowE, &m_FeatureMask) >= 17;
}
//----------------------------------------------------------------------------------
bool CCustomHouseObjectFloor::Parse(const char *text)
{
	WISPFUN_DEBUG("");
	return sscanf(text,
		"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
		&m_Category, &m_F1, &m_F2, &m_F3, &m_F4, &m_F5, &m_F6, &m_F7, &m_F8, &m_F9, &m_F10, &m_F11, &m_F12, &m_F13, &m_F14, &m_F15, &m_F16, &m_FeatureMask) >= 17;
}
//----------------------------------------------------------------------------------
bool CCustomHouseObjectDoor::Parse(const char *text)
{
	WISPFUN_DEBUG("");
	return sscanf(text,
		"%d %d %d %d %d %d %d %d %d %d",
		&m_Category, &m_Piece1, &m_Piece2, &m_Piece3, &m_Piece4, &m_Piece5, &m_Piece6, &m_Piece7, &m_Piece8, &m_FeatureMask) >= 9;
}
//----------------------------------------------------------------------------------
bool CCustomHouseObjectMisc::Parse(const char *text)
{
	WISPFUN_DEBUG("");
	return sscanf(text,
		"%d %d %d %d %d %d %d %d %d %d %d %d",
		&m_Category, &m_Style, &m_TID, &m_Piece1, &m_Piece2, &m_Piece3, &m_Piece4, &m_Piece5, &m_Piece6, &m_Piece7, &m_Piece8, &m_FeatureMask) >= 11;
}
//----------------------------------------------------------------------------------
bool CCustomHouseObjectStair::Parse(const char *text)
{
	WISPFUN_DEBUG("");
	return sscanf(text,
		"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
		&m_Category, &m_Block, &m_North, &m_East, &m_South, &m_West, &m_Squared1, &m_Squared2, &m_Rounded1, &m_Rounded2, &m_MultiNorth, &m_MultiEast, &m_MultiSouth, &m_MultiWest, &m_FeatureMask) >= 14;
}
//----------------------------------------------------------------------------------
bool CCustomHouseObjectTeleport::Parse(const char *text)
{
	WISPFUN_DEBUG("");
	return sscanf(text,
		"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
		&m_Category, &m_F1, &m_F2, &m_F3, &m_F4, &m_F5, &m_F6, &m_F7, &m_F8, &m_F9, &m_F10, &m_F11, &m_F12, &m_F13, &m_F14, &m_F15, &m_F16, &m_FeatureMask) >= 17;
}
//----------------------------------------------------------------------------------
bool CCustomHouseObjectRoof::Parse(const char *text)
{
	WISPFUN_DEBUG("");
	return sscanf(text,
		"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
		&m_Category, &m_Style, &m_TID, &m_North, &m_East, &m_South, &m_West, &m_NSCrosspiece, &m_EWCrosspiece, &m_NDent, &m_SDent, &m_WDent, &m_NTPiece, &m_ETPiece, &m_STPiece, &m_WTPiece, &m_XPiece, &m_Extra, &m_Piece, &m_FeatureMask) >= 19;
}
//----------------------------------------------------------------------------------
