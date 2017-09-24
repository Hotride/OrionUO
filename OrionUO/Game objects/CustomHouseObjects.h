/***********************************************************************************
**
** CustomHouseObjects.h
**
** Copyright (C) September 2017 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef CUSTOMHOUSEOBJECTS_H
#define CUSTOMHOUSEOBJECTS_H
//----------------------------------------------------------------------------------
class CCustomHouseObject
{
	SETGET(int, Category, 0);
	SETGET(int, FeatureMask, 0);

public:
	CCustomHouseObject() {}
	~CCustomHouseObject() {}

	virtual bool Parse(const char *text) { return false; }
};
//----------------------------------------------------------------------------------
class CCustomHouseObjectWall : public CCustomHouseObject
{
	SETGET(int, Style, 0);
	SETGET(int, TID, 0);
	SETGET(int, South1, 0);
	SETGET(int, South2, 0);
	SETGET(int, South3, 0);
	SETGET(int, Corner, 0);
	SETGET(int, Eats1, 0);
	SETGET(int, Eats2, 0);
	SETGET(int, Eats3, 0);
	SETGET(int, Post, 0);
	SETGET(int, WindowS, 0);
	SETGET(int, AltWindowS, 0);
	SETGET(int, WindowE, 0);
	SETGET(int, AltWindowE, 0);
	SETGET(int, SecondAltWindowS, 0);
	SETGET(int, SecondAltWindowE, 0);

public:
	CCustomHouseObjectWall() : CCustomHouseObject() {}
	~CCustomHouseObjectWall() {}

	virtual bool Parse(const char *text);
};
//----------------------------------------------------------------------------------
class CCustomHouseObjectFloor : public CCustomHouseObject
{
	SETGET(int, F1, 0);
	SETGET(int, F2, 0);
	SETGET(int, F3, 0);
	SETGET(int, F4, 0);
	SETGET(int, F5, 0);
	SETGET(int, F6, 0);
	SETGET(int, F7, 0);
	SETGET(int, F8, 0);
	SETGET(int, F9, 0);
	SETGET(int, F10, 0);
	SETGET(int, F11, 0);
	SETGET(int, F12, 0);
	SETGET(int, F13, 0);
	SETGET(int, F14, 0);
	SETGET(int, F15, 0);
	SETGET(int, F16, 0);

public:
	CCustomHouseObjectFloor() : CCustomHouseObject() {}
	~CCustomHouseObjectFloor() {}

	virtual bool Parse(const char *text);
};
//----------------------------------------------------------------------------------
class CCustomHouseObjectDoor : public CCustomHouseObject
{
	SETGET(int, Piece1, 0);
	SETGET(int, Piece2, 0);
	SETGET(int, Piece3, 0);
	SETGET(int, Piece4, 0);
	SETGET(int, Piece5, 0);
	SETGET(int, Piece6, 0);
	SETGET(int, Piece7, 0);
	SETGET(int, Piece8, 0);
	
public:
	CCustomHouseObjectDoor() : CCustomHouseObject() {}
	~CCustomHouseObjectDoor() {}

	virtual bool Parse(const char *text);
};
//----------------------------------------------------------------------------------
class CCustomHouseObjectMisc : public CCustomHouseObject
{
	SETGET(int, Style, 0);
	SETGET(int, TID, 0);
	SETGET(int, Piece1, 0);
	SETGET(int, Piece2, 0);
	SETGET(int, Piece3, 0);
	SETGET(int, Piece4, 0);
	SETGET(int, Piece5, 0);
	SETGET(int, Piece6, 0);
	SETGET(int, Piece7, 0);
	SETGET(int, Piece8, 0);
	
public:
	CCustomHouseObjectMisc() : CCustomHouseObject() {}
	~CCustomHouseObjectMisc() {}

	virtual bool Parse(const char *text);
};
//----------------------------------------------------------------------------------
class CCustomHouseObjectStair : public CCustomHouseObject
{
	SETGET(int, Block, 0);
	SETGET(int, North, 0);
	SETGET(int, East, 0);
	SETGET(int, South, 0);
	SETGET(int, West, 0);
	SETGET(int, Squared1, 0);
	SETGET(int, Squared2, 0);
	SETGET(int, Rounded1, 0);
	SETGET(int, Rounded2, 0);
	SETGET(int, MultiNorth, 0);
	SETGET(int, MultiEast, 0);
	SETGET(int, MultiSouth, 0);
	SETGET(int, MultiWest, 0);

public:
	CCustomHouseObjectStair() : CCustomHouseObject() {}
	~CCustomHouseObjectStair() {}

	virtual bool Parse(const char *text);
};
//----------------------------------------------------------------------------------
class CCustomHouseObjectTeleport : public CCustomHouseObject
{
	SETGET(int, F1, 0);
	SETGET(int, F2, 0);
	SETGET(int, F3, 0);
	SETGET(int, F4, 0);
	SETGET(int, F5, 0);
	SETGET(int, F6, 0);
	SETGET(int, F7, 0);
	SETGET(int, F8, 0);
	SETGET(int, F9, 0);
	SETGET(int, F10, 0);
	SETGET(int, F11, 0);
	SETGET(int, F12, 0);
	SETGET(int, F13, 0);
	SETGET(int, F14, 0);
	SETGET(int, F15, 0);
	SETGET(int, F16, 0);

public:
	CCustomHouseObjectTeleport() : CCustomHouseObject() {}
	~CCustomHouseObjectTeleport() {}

	virtual bool Parse(const char *text);
};
//----------------------------------------------------------------------------------
class CCustomHouseObjectRoof : public CCustomHouseObject
{
	SETGET(int, Style, 0);
	SETGET(int, TID, 0);
	SETGET(int, North, 0);
	SETGET(int, East, 0);
	SETGET(int, South, 0);
	SETGET(int, West, 0);
	SETGET(int, NSCrosspiece, 0);
	SETGET(int, EWCrosspiece, 0);
	SETGET(int, NDent, 0);
	SETGET(int, SDent, 0);
	SETGET(int, WDent, 0);
	SETGET(int, NTPiece, 0);
	SETGET(int, ETPiece, 0);
	SETGET(int, STPiece, 0);
	SETGET(int, WTPiece, 0);
	SETGET(int, XPiece, 0);
	SETGET(int, Extra, 0);
	SETGET(int, Piece, 0);

public:
	CCustomHouseObjectRoof() : CCustomHouseObject() {}
	~CCustomHouseObjectRoof() {}

	virtual bool Parse(const char *text);
};
//----------------------------------------------------------------------------------
#endif //CUSTOMHOUSEOBJECTS_H
//----------------------------------------------------------------------------------
