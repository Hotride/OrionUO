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
//!Base class for Custom House object info
class CCustomHouseObject
{
	//!Category for object
	SETGET(int, Category, 0);

	//!Flag mask to enable/disable objects (in some expansions)
	SETGET(int, FeatureMask, 0);

public:
	/*!
	Constructor
	@return
	*/
	CCustomHouseObject() {}

	/*!
	Destructor
	@return
	*/
	virtual ~CCustomHouseObject() {}

	/*!
	Parse text data to class fields
	@param [__in] text Data for parsing
	@return
	*/
	virtual bool Parse(const char *text) { return false; }
};
//----------------------------------------------------------------------------------
//!Wall object for CH
class CCustomHouseObjectWall : public CCustomHouseObject
{
	//!Position in category
	SETGET(int, Style, 0);

	//!ClilocID for name of category (for tooltips)
	SETGET(int, TID, 0);

	//!Item ID (direction: south 1)
	SETGET(int, South1, 0);

	//!Item ID (direction: south 2)
	SETGET(int, South2, 0);

	//!Item ID (direction: south 3)
	SETGET(int, South3, 0);

	//!Item ID (corner)
	SETGET(int, Corner, 0);

	//!Item ID (direction: east 1)
	SETGET(int, East1, 0);

	//!Item ID (direction: east 2)
	SETGET(int, East2, 0);

	//!Item ID (direction: east 3)
	SETGET(int, East3, 0);

	//!Item ID (post)
	SETGET(int, Post, 0);

	//!Item ID with window (south)
	SETGET(int, WindowS, 0);

	//!Item ID with window (south), first, alternative
	SETGET(int, AltWindowS, 0);

	//!Item ID with window (east)
	SETGET(int, WindowE, 0);

	//!Item ID with window (east), first, alternative
	SETGET(int, AltWindowE, 0);

	//!Item ID with window (south), second, alternative
	SETGET(int, SecondAltWindowS, 0);

	//!Item ID with window (east), second, alternative
	SETGET(int, SecondAltWindowE, 0);

public:
	/*!
	Constructor
	@return
	*/
	CCustomHouseObjectWall() : CCustomHouseObject() { memset(&m_Graphics[0], 0, sizeof(m_Graphics)); memset(&m_WindowGraphics[0], 0, sizeof(m_WindowGraphics)); }

	/*!
	Destructor
	@return
	*/
	virtual ~CCustomHouseObjectWall() {}

	/*!
	Parse text data to class fields
	@param [__in] text Data for parsing
	@return
	*/
	virtual bool Parse(const char *text);

	/*!
	Search graphic in list
	@param [__in] graphic Graphic to search
	@return index if it found or -1 if not found
	*/
	int Contains(const ushort &graphic) const;

	//!Maximum number of graphics count
	static const int GRAPHICS_COUNT = 8;

	//!Graphics list
	ushort m_Graphics[GRAPHICS_COUNT];

	//!Graphics list with replaced windowed walls
	ushort m_WindowGraphics[GRAPHICS_COUNT];
};
//----------------------------------------------------------------------------------
//!Category of wall objects
class CCustomHouseObjectWallCategory
{
	//!Index of category
	SETGET(int, Index, 0);

public:
	/*!
	Constructor
	@return
	*/
	CCustomHouseObjectWallCategory() {}

	/*!
	Destructor
	@return
	*/
	virtual ~CCustomHouseObjectWallCategory() {}

	//!Wall objects list
	vector<CCustomHouseObjectWall> m_Items;
};
//----------------------------------------------------------------------------------
//!Floor objects for CH
class CCustomHouseObjectFloor : public CCustomHouseObject
{
	//!Floor 1 graphic
	SETGET(int, F1, 0);

	//!Floor 2 graphic
	SETGET(int, F2, 0);

	//!Floor 3 graphic
	SETGET(int, F3, 0);

	//!Floor 4 graphic
	SETGET(int, F4, 0);

	//!Floor 5 graphic
	SETGET(int, F5, 0);

	//!Floor 6 graphic
	SETGET(int, F6, 0);

	//!Floor 7 graphic
	SETGET(int, F7, 0);

	//!Floor 8 graphic
	SETGET(int, F8, 0);

	//!Floor 9 graphic
	SETGET(int, F9, 0);

	//!Floor 10 graphic
	SETGET(int, F10, 0);

	//!Floor 11 graphic
	SETGET(int, F11, 0);

	//!Floor 12 graphic
	SETGET(int, F12, 0);

	//!Floor 13 graphic
	SETGET(int, F13, 0);

	//!Floor 14 graphic
	SETGET(int, F14, 0);

	//!Floor 15 graphic
	SETGET(int, F15, 0);

	//!Floor 16 graphic
	SETGET(int, F16, 0);

public:
	/*!
	Constructor
	@return
	*/
	CCustomHouseObjectFloor() : CCustomHouseObject() { memset(&m_Graphics[0], 0, sizeof(m_Graphics)); }

	/*!
	Destructor
	@return
	*/
	virtual ~CCustomHouseObjectFloor() {}

	/*!
	Parse text data to class fields
	@param [__in] text Data for parsing
	@return
	*/
	virtual bool Parse(const char *text);

	/*!
	Search graphic in list
	@param [__in] graphic Graphic to search
	@return index if it found or -1 if not found
	*/
	int Contains(const ushort &graphic) const;

	//!Maximum number of graphics count
	static const int GRAPHICS_COUNT = 16;

	//!Graphics list
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
	//!Door 1 graphic
	SETGET(int, Piece1, 0);

	//!Door 2 graphic
	SETGET(int, Piece2, 0);

	//!Door 3 graphic
	SETGET(int, Piece3, 0);

	//!Door 4 graphic
	SETGET(int, Piece4, 0);

	//!Door 5 graphic
	SETGET(int, Piece5, 0);

	//!Door 6 graphic
	SETGET(int, Piece6, 0);

	//!Door 7 graphic
	SETGET(int, Piece7, 0);

	//!Door 8 graphic
	SETGET(int, Piece8, 0);
	
public:
	/*!
	Constructor
	@return
	*/
	CCustomHouseObjectDoor() : CCustomHouseObject() { memset(&m_Graphics[0], 0, sizeof(m_Graphics)); }

	/*!
	Destructor
	@return
	*/
	virtual ~CCustomHouseObjectDoor() {}

	/*!
	Parse text data to class fields
	@param [__in] text Data for parsing
	@return
	*/
	virtual bool Parse(const char *text);

	/*!
	Search graphic in list
	@param [__in] graphic Graphic to search
	@return index if it found or -1 if not found
	*/
	int Contains(const ushort &graphic) const;

	//!Maximum number of graphics count
	static const int GRAPHICS_COUNT = 8;

	//!Graphics list
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
	//!Position in category
	SETGET(int, Style, 0);

	//!ClilocID for name of category (for tooltips)
	SETGET(int, TID, 0);

	//!Misc 1 graphic
	SETGET(int, Piece1, 0);

	//!Misc 2 graphic
	SETGET(int, Piece2, 0);

	//!Misc 3 graphic
	SETGET(int, Piece3, 0);

	//!Misc 4 graphic
	SETGET(int, Piece4, 0);

	//!Misc 5 graphic
	SETGET(int, Piece5, 0);

	//!Misc 6 graphic
	SETGET(int, Piece6, 0);

	//!Misc 7 graphic
	SETGET(int, Piece7, 0);

	//!Misc 8 graphic
	SETGET(int, Piece8, 0);
	
public:
	/*!
	Constructor
	@return
	*/
	CCustomHouseObjectMisc() : CCustomHouseObject() { memset(&m_Graphics[0], 0, sizeof(m_Graphics)); }

	/*!
	Destructor
	@return
	*/
	virtual ~CCustomHouseObjectMisc() {}

	/*!
	Parse text data to class fields
	@param [__in] text Data for parsing
	@return
	*/
	virtual bool Parse(const char *text);

	/*!
	Search graphic in list
	@param [__in] graphic Graphic to search
	@return index if it found or -1 if not found
	*/
	int Contains(const ushort &graphic) const;

	//!Maximum number of graphics count
	static const int GRAPHICS_COUNT = 8;

	//!Graphics list
	ushort m_Graphics[GRAPHICS_COUNT];
};
//----------------------------------------------------------------------------------
class CCustomHouseObjectMiscCategory
{
	//!Index of category
	SETGET(int, Index, 0);

public:
	/*!
	Constructor
	@return
	*/
	CCustomHouseObjectMiscCategory() {}

	/*!
	Destructor
	@return
	*/
	virtual ~CCustomHouseObjectMiscCategory() {}

	//!Misc objects list
	vector<CCustomHouseObjectMisc> m_Items;
};
//----------------------------------------------------------------------------------
class CCustomHouseObjectStair : public CCustomHouseObject
{
	//!Stair block (quard) graphic
	SETGET(int, Block, 0);

	//!North direction stair graphic
	SETGET(int, North, 0);

	//!East direction stair graphic
	SETGET(int, East, 0);

	//!South direction stair graphic
	SETGET(int, South, 0);

	//!West direction stair graphic
	SETGET(int, West, 0);

	//!Squared 1 stair graphic
	SETGET(int, Squared1, 0);

	//!Squared 2 stair graphic
	SETGET(int, Squared2, 0);

	//!Rounded 1 stair graphic
	SETGET(int, Rounded1, 0);

	//!Rounded 2 stair graphic
	SETGET(int, Rounded2, 0);

	//!Graphic for packet if used Squared1
	SETGET(int, MultiNorth, 0);

	//!Graphic for packet if used Squared2
	SETGET(int, MultiEast, 0);

	//!Graphic for packet if used Rounded1
	SETGET(int, MultiSouth, 0);

	//!Graphic for packet if used Rounded2
	SETGET(int, MultiWest, 0);

public:
	/*!
	Constructor
	@return
	*/
	CCustomHouseObjectStair() : CCustomHouseObject() { memset(&m_Graphics[0], 0, sizeof(m_Graphics)); }

	/*!
	Destructor
	@return
	*/
	virtual ~CCustomHouseObjectStair() {}

	/*!
	Parse text data to class fields
	@param [__in] text Data for parsing
	@return
	*/
	virtual bool Parse(const char *text);

	/*!
	Search graphic in list
	@param [__in] graphic Graphic to search
	@return index if it found or -1 if not found
	*/
	int Contains(const ushort &graphic) const;

	//!Maximum number of graphics count
	static const int GRAPHICS_COUNT = 9;

	//!Graphics list
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
	//!Teleport 1 graphic
	SETGET(int, F1, 0);

	//!Teleport 2 graphic
	SETGET(int, F2, 0);

	//!Teleport 3 graphic
	SETGET(int, F3, 0);

	//!Teleport 4 graphic
	SETGET(int, F4, 0);

	//!Teleport 5 graphic
	SETGET(int, F5, 0);

	//!Teleport 6 graphic
	SETGET(int, F6, 0);

	//!Teleport 7 graphic
	SETGET(int, F7, 0);

	//!Teleport 8 graphic
	SETGET(int, F8, 0);

	//!Teleport 9 graphic
	SETGET(int, F9, 0);

	//!Teleport 10 graphic
	SETGET(int, F10, 0);

	//!Teleport 11 graphic
	SETGET(int, F11, 0);

	//!Teleport 12 graphic
	SETGET(int, F12, 0);

	//!Teleport 13 graphic
	SETGET(int, F13, 0);

	//!Teleport 14 graphic
	SETGET(int, F14, 0);

	//!Teleport 15 graphic
	SETGET(int, F15, 0);

	//!Teleport 16 graphic
	SETGET(int, F16, 0);

public:
	/*!
	Constructor
	@return
	*/
	CCustomHouseObjectTeleport() : CCustomHouseObject() { memset(&m_Graphics[0], 0, sizeof(m_Graphics)); }

	/*!
	Destructor
	@return
	*/
	virtual ~CCustomHouseObjectTeleport() {}

	/*!
	Parse text data to class fields
	@param [__in] text Data for parsing
	@return
	*/
	virtual bool Parse(const char *text);

	/*!
	Search graphic in list
	@param [__in] graphic Graphic to search
	@return index if it found or -1 if not found
	*/
	int Contains(const ushort &graphic) const;

	//!Maximum number of graphics count
	static const int GRAPHICS_COUNT = 16;

	//!Graphics list
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
	//!Position in category
	SETGET(int, Style, 0);

	//!ClilocID for name of category (for tooltips)
	SETGET(int, TID, 0);

	//!North direction roof graphic
	SETGET(int, North, 0);

	//!East direction roof graphic
	SETGET(int, East, 0);

	//!South direction roof graphic
	SETGET(int, South, 0);

	//!West direction roof graphic
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
	/*!
	Constructor
	@return
	*/
	CCustomHouseObjectRoof() : CCustomHouseObject() { memset(&m_Graphics[0], 0, sizeof(m_Graphics)); }

	/*!
	Destructor
	@return
	*/
	virtual ~CCustomHouseObjectRoof() {}

	/*!
	Parse text data to class fields
	@param [__in] text Data for parsing
	@return
	*/
	virtual bool Parse(const char *text);

	/*!
	Search graphic in list
	@param [__in] graphic Graphic to search
	@return index if it found or -1 if not found
	*/
	int Contains(const ushort &graphic) const;

	//!Maximum number of graphics count
	static const int GRAPHICS_COUNT = 16;

	//!Graphics list
	ushort m_Graphics[GRAPHICS_COUNT];
};
//----------------------------------------------------------------------------------
class CCustomHouseObjectRoofCategory
{
	//!Index of category
	SETGET(int, Index, 0);

public:
	/*!
	Constructor
	@return
	*/
	CCustomHouseObjectRoofCategory() {}

	/*!
	Destructor
	@return
	*/
	virtual ~CCustomHouseObjectRoofCategory() {}

	//!Roof objects list
	vector<CCustomHouseObjectRoof> m_Items;
};
//----------------------------------------------------------------------------------
class CCustomHouseObjectPlaceInfo
{
	//!Object graphic
	SETGET(int, Graphic, 0);

	//!Check top
	SETGET(int, Top, 0);

	//!Check bottom
	SETGET(int, Bottom, 0);

	//!Adjustment upper north
	SETGET(int, AdjUN, 0);

	//!Adjustment lower north
	SETGET(int, AdjLN, 0);

	//!Adjustment upper east
	SETGET(int, AdjUE, 0);

	//!Adjustment lower east
	SETGET(int, AdjLE, 0);

	//!Adjustment upper south
	SETGET(int, AdjUS, 0);

	//!Adjustment lower south
	SETGET(int, AdjLS, 0);

	//!Adjustment upper west
	SETGET(int, AdjUW, 0);

	//!Adjustment lower west
	SETGET(int, AdjLW, 0);

	//!Check corner with near tiles
	SETGET(int, DirectSupports, 0);

	//!Can move (to/from?) west
	SETGET(int, CanGoW, 0);

	//!Can move (to/from?) north
	SETGET(int, CanGoN, 0);

	//!Can move (to/from?) north/west/south
	SETGET(int, CanGoNWS, 0);

	//!Flag mask to enable/disable objects (in some expansions)
	SETGET(int, FeatureMask, 0);

public:
	/*!
	Constructor
	@return
	*/
	CCustomHouseObjectPlaceInfo() { memset(&m_Graphics[0], 0, sizeof(m_Graphics)); }

	/*!
	Destructor
	@return
	*/
	virtual ~CCustomHouseObjectPlaceInfo() {}

	/*!
	Parse text data to class fields
	@param [__in] text Data for parsing
	@return
	*/
	bool Parse(const char *text);

	/*!
	Search graphic in list
	@param [__in] graphic Graphic to search
	@return index if it found or -1 if not found
	*/
	int Contains(const ushort &graphic) const;

	//!Maximum number of graphics count
	static const int GRAPHICS_COUNT = 1;

	//!Graphics list
	ushort m_Graphics[GRAPHICS_COUNT];
};
//----------------------------------------------------------------------------------
#endif //CUSTOMHOUSEOBJECTS_H
//----------------------------------------------------------------------------------
