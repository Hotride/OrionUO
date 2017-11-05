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
	virtual ~CCustomHouseObject() {}

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
	SETGET(int, East1, 0);
	SETGET(int, East2, 0);
	SETGET(int, East3, 0);
	SETGET(int, Post, 0);
	SETGET(int, WindowS, 0);
	SETGET(int, AltWindowS, 0);
	SETGET(int, WindowE, 0);
	SETGET(int, AltWindowE, 0);
	SETGET(int, SecondAltWindowS, 0);
	SETGET(int, SecondAltWindowE, 0);

public:
	CCustomHouseObjectWall() : CCustomHouseObject() { memset(&m_Graphics[0], 0, sizeof(m_Graphics)); memset(&m_WindowGraphics[0], 0, sizeof(m_WindowGraphics)); }
	virtual ~CCustomHouseObjectWall() {}

	virtual bool Parse(const char *text);

	int Contains(const ushort &graphic) const;

	static const int GRAPHICS_COUNT = 8;

	ushort m_Graphics[GRAPHICS_COUNT];
	ushort m_WindowGraphics[GRAPHICS_COUNT];
};
//----------------------------------------------------------------------------------
class CCustomHouseObjectWallCategory
{
	SETGET(int, Index, 0);

public:
	CCustomHouseObjectWallCategory() {}
	virtual ~CCustomHouseObjectWallCategory() {}

	vector<CCustomHouseObjectWall> m_Items;
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
	CCustomHouseObjectFloor() : CCustomHouseObject() { memset(&m_Graphics[0], 0, sizeof(m_Graphics)); }
	virtual ~CCustomHouseObjectFloor() {}

	virtual bool Parse(const char *text);

	int Contains(const ushort &graphic) const;

	static const int GRAPHICS_COUNT = 16;

	ushort m_Graphics[GRAPHICS_COUNT];
};
//----------------------------------------------------------------------------------
/*class CCustomHouseObjectFloorCategory
{
	SETGET(int, Index, 0);

public:
	CCustomHouseObjectFloorCategory() {}
	virtual ~CCustomHouseObjectFloorCategory() {}

	vector<CCustomHouseObjectFloor> m_Items;
};*/
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
	CCustomHouseObjectDoor() : CCustomHouseObject() { memset(&m_Graphics[0], 0, sizeof(m_Graphics)); }
	virtual ~CCustomHouseObjectDoor() {}

	virtual bool Parse(const char *text);

	int Contains(const ushort &graphic) const;

	static const int GRAPHICS_COUNT = 8;

	ushort m_Graphics[GRAPHICS_COUNT];
};
//----------------------------------------------------------------------------------
/*class CCustomHouseObjectDoorCategory
{
	SETGET(int, Index, 0);

public:
	CCustomHouseObjectDoorCategory() {}
	virtual ~CCustomHouseObjectDoorCategory() {}

	vector<CCustomHouseObjectDoor> m_Items;
};*/
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
	CCustomHouseObjectMisc() : CCustomHouseObject() { memset(&m_Graphics[0], 0, sizeof(m_Graphics)); }
	virtual ~CCustomHouseObjectMisc() {}

	virtual bool Parse(const char *text);

	int Contains(const ushort &graphic) const;

	static const int GRAPHICS_COUNT = 8;

	ushort m_Graphics[GRAPHICS_COUNT];
};
//----------------------------------------------------------------------------------
class CCustomHouseObjectMiscCategory
{
	SETGET(int, Index, 0);

public:
	CCustomHouseObjectMiscCategory() {}
	virtual ~CCustomHouseObjectMiscCategory() {}

	vector<CCustomHouseObjectMisc> m_Items;
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
	CCustomHouseObjectStair() : CCustomHouseObject() { memset(&m_Graphics[0], 0, sizeof(m_Graphics)); }
	virtual ~CCustomHouseObjectStair() {}

	virtual bool Parse(const char *text);

	int Contains(const ushort &graphic) const;

	static const int GRAPHICS_COUNT = 9;

	ushort m_Graphics[GRAPHICS_COUNT];
};
//----------------------------------------------------------------------------------
/*class CCustomHouseObjectStairCategory
{
	SETGET(int, Index, 0);

public:
	CCustomHouseObjectStairCategory() {}
	virtual ~CCustomHouseObjectStairCategory() {}

	vector<CCustomHouseObjectStair> m_Items;
};*/
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
	CCustomHouseObjectTeleport() : CCustomHouseObject() { memset(&m_Graphics[0], 0, sizeof(m_Graphics)); }
	virtual ~CCustomHouseObjectTeleport() {}

	virtual bool Parse(const char *text);

	int Contains(const ushort &graphic) const;

	static const int GRAPHICS_COUNT = 16;

	ushort m_Graphics[GRAPHICS_COUNT];
};
//----------------------------------------------------------------------------------
/*class CCustomHouseObjectTeleportCategory
{
	SETGET(int, Index, 0);

public:
	CCustomHouseObjectTeleportCategory() {}
	virtual ~CCustomHouseObjectTeleportCategory() {}

	vector<CCustomHouseObjectTeleport> m_Items;
};*/
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
	CCustomHouseObjectRoof() : CCustomHouseObject() { memset(&m_Graphics[0], 0, sizeof(m_Graphics)); }
	virtual ~CCustomHouseObjectRoof() {}

	virtual bool Parse(const char *text);

	int Contains(const ushort &graphic) const;

	static const int GRAPHICS_COUNT = 16;

	ushort m_Graphics[GRAPHICS_COUNT];
};
//----------------------------------------------------------------------------------
class CCustomHouseObjectRoofCategory
{
	SETGET(int, Index, 0);

public:
	CCustomHouseObjectRoofCategory() {}
	virtual ~CCustomHouseObjectRoofCategory() {}

	vector<CCustomHouseObjectRoof> m_Items;
};
//----------------------------------------------------------------------------------
class CCustomHouseObjectPlaceInfo
{
	SETGET(int, Graphic, 0);
	SETGET(int, Top, 0);
	SETGET(int, Bottom, 0);
	SETGET(int, AdjUN, 0);
	SETGET(int, AdjLN, 0);
	SETGET(int, AdjUE, 0);
	SETGET(int, AdjLE, 0);
	SETGET(int, AdjUS, 0);
	SETGET(int, AdjLS, 0);
	SETGET(int, AdjUW, 0);
	SETGET(int, AdjLW, 0);
	SETGET(int, DirectSupports, 0);
	SETGET(int, CanGoW, 0);
	SETGET(int, CanGoN, 0);
	SETGET(int, CanGoNWS, 0);

	SETGET(int, FeatureMask, 0);

public:
	CCustomHouseObjectPlaceInfo() { memset(&m_Graphics[0], 0, sizeof(m_Graphics)); }
	virtual ~CCustomHouseObjectPlaceInfo() {}

	bool Parse(const char *text);

	int Contains(const ushort &graphic) const;

	static const int GRAPHICS_COUNT = 1;

	ushort m_Graphics[GRAPHICS_COUNT];
};
//----------------------------------------------------------------------------------
#endif //CUSTOMHOUSEOBJECTS_H
//----------------------------------------------------------------------------------
