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
public:
    //!Category for object
    int Category = 0;

    //!Flag mask to enable/disable objects (in some expansions)
    int FeatureMask = 0;

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
public:
    //!Position in category
    int Style = 0;

    //!ClilocID for name of category (for tooltips)
    int TID = 0;

    //!Item ID (direction: south 1)
    int South1 = 0;

    //!Item ID (direction: south 2)
    int South2 = 0;

    //!Item ID (direction: south 3)
    int South3 = 0;

    //!Item ID (corner)
    int Corner = 0;

    //!Item ID (direction: east 1)
    int East1 = 0;

    //!Item ID (direction: east 2)
    int East2 = 0;

    //!Item ID (direction: east 3)
    int East3 = 0;

    //!Item ID (post)
    int Post = 0;

    //!Item ID with window (south)
    int WindowS = 0;

    //!Item ID with window (south), first, alternative
    int AltWindowS = 0;

    //!Item ID with window (east)
    int WindowE = 0;

    //!Item ID with window (east), first, alternative
    int AltWindowE = 0;

    //!Item ID with window (south), second, alternative
    int SecondAltWindowS = 0;

    //!Item ID with window (east), second, alternative
    int SecondAltWindowE = 0;

    /*!
	Constructor
	@return
	*/
    CCustomHouseObjectWall()
        : CCustomHouseObject()
    {
        memset(&m_Graphics[0], 0, sizeof(m_Graphics));
        memset(&m_WindowGraphics[0], 0, sizeof(m_WindowGraphics));
    }

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
    int Contains(ushort graphic) const;

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
public:
    //!Index of category
    int Index = 0;

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
    int F1 = 0;

    //!Floor 2 graphic
    int F2 = 0;

    //!Floor 3 graphic
    int F3 = 0;

    //!Floor 4 graphic
    int F4 = 0;

    //!Floor 5 graphic
    int F5 = 0;

    //!Floor 6 graphic
    int F6 = 0;

    //!Floor 7 graphic
    int F7 = 0;

    //!Floor 8 graphic
    int F8 = 0;

    //!Floor 9 graphic
    int F9 = 0;

    //!Floor 10 graphic
    int F10 = 0;

    //!Floor 11 graphic
    int F11 = 0;

    //!Floor 12 graphic
    int F12 = 0;

    //!Floor 13 graphic
    int F13 = 0;

    //!Floor 14 graphic
    int F14 = 0;

    //!Floor 15 graphic
    int F15 = 0;

    //!Floor 16 graphic
    int F16 = 0;

public:
    /*!
	Constructor
	@return
	*/
    CCustomHouseObjectFloor()
        : CCustomHouseObject()
    {
        memset(&m_Graphics[0], 0, sizeof(m_Graphics));
    }

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
    int Contains(ushort graphic) const;

    //!Maximum number of graphics count
    static const int GRAPHICS_COUNT = 16;

    //!Graphics list
    ushort m_Graphics[GRAPHICS_COUNT];
};
//----------------------------------------------------------------------------------
/*class CCustomHouseObjectFloorCategory
{
	int Index = 0;

public:
	CCustomHouseObjectFloorCategory() {}
	virtual ~CCustomHouseObjectFloorCategory() {}

	vector<CCustomHouseObjectFloor> m_Items;
};*/
//----------------------------------------------------------------------------------
class CCustomHouseObjectDoor : public CCustomHouseObject
{
public:
    //!Door 1 graphic
    int Piece1 = 0;

    //!Door 2 graphic
    int Piece2 = 0;

    //!Door 3 graphic
    int Piece3 = 0;

    //!Door 4 graphic
    int Piece4 = 0;

    //!Door 5 graphic
    int Piece5 = 0;

    //!Door 6 graphic
    int Piece6 = 0;

    //!Door 7 graphic
    int Piece7 = 0;

    //!Door 8 graphic
    int Piece8 = 0;

    /*!
	Constructor
	@return
	*/
    CCustomHouseObjectDoor()
        : CCustomHouseObject()
    {
        memset(&m_Graphics[0], 0, sizeof(m_Graphics));
    }

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
    int Contains(ushort graphic) const;

    //!Maximum number of graphics count
    static const int GRAPHICS_COUNT = 8;

    //!Graphics list
    ushort m_Graphics[GRAPHICS_COUNT];
};
//----------------------------------------------------------------------------------
/*class CCustomHouseObjectDoorCategory
{
	int Index = 0;

public:
	CCustomHouseObjectDoorCategory() {}
	virtual ~CCustomHouseObjectDoorCategory() {}

	vector<CCustomHouseObjectDoor> m_Items;
};*/
//----------------------------------------------------------------------------------
class CCustomHouseObjectMisc : public CCustomHouseObject
{
public:
    //!Position in category
    int Style = 0;

    //!ClilocID for name of category (for tooltips)
    int TID = 0;

    //!Misc 1 graphic
    int Piece1 = 0;

    //!Misc 2 graphic
    int Piece2 = 0;

    //!Misc 3 graphic
    int Piece3 = 0;

    //!Misc 4 graphic
    int Piece4 = 0;

    //!Misc 5 graphic
    int Piece5 = 0;

    //!Misc 6 graphic
    int Piece6 = 0;

    //!Misc 7 graphic
    int Piece7 = 0;

    //!Misc 8 graphic
    int Piece8 = 0;

    /*!
	Constructor
	@return
	*/
    CCustomHouseObjectMisc()
        : CCustomHouseObject()
    {
        memset(&m_Graphics[0], 0, sizeof(m_Graphics));
    }

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
    int Contains(ushort graphic) const;

    //!Maximum number of graphics count
    static const int GRAPHICS_COUNT = 8;

    //!Graphics list
    ushort m_Graphics[GRAPHICS_COUNT];
};
//----------------------------------------------------------------------------------
class CCustomHouseObjectMiscCategory
{
public:
    //!Index of category
    int Index = 0;

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
public:
    //!Stair block (quard) graphic
    int Block = 0;

    //!North direction stair graphic
    int North = 0;

    //!East direction stair graphic
    int East = 0;

    //!South direction stair graphic
    int South = 0;

    //!West direction stair graphic
    int West = 0;

    //!Squared 1 stair graphic
    int Squared1 = 0;

    //!Squared 2 stair graphic
    int Squared2 = 0;

    //!Rounded 1 stair graphic
    int Rounded1 = 0;

    //!Rounded 2 stair graphic
    int Rounded2 = 0;

    //!Graphic for packet if used Squared1
    int MultiNorth = 0;

    //!Graphic for packet if used Squared2
    int MultiEast = 0;

    //!Graphic for packet if used Rounded1
    int MultiSouth = 0;

    //!Graphic for packet if used Rounded2
    int MultiWest = 0;

    /*!
	Constructor
	@return
	*/
    CCustomHouseObjectStair()
        : CCustomHouseObject()
    {
        memset(&m_Graphics[0], 0, sizeof(m_Graphics));
    }

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
    int Contains(ushort graphic) const;

    //!Maximum number of graphics count
    static const int GRAPHICS_COUNT = 9;

    //!Graphics list
    ushort m_Graphics[GRAPHICS_COUNT];
};
//----------------------------------------------------------------------------------
/*class CCustomHouseObjectStairCategory
{
	int Index = 0;

public:
	CCustomHouseObjectStairCategory() {}
	virtual ~CCustomHouseObjectStairCategory() {}

	vector<CCustomHouseObjectStair> m_Items;
};*/
//----------------------------------------------------------------------------------
class CCustomHouseObjectTeleport : public CCustomHouseObject
{
    //!Teleport 1 graphic
    int F1 = 0;

    //!Teleport 2 graphic
    int F2 = 0;

    //!Teleport 3 graphic
    int F3 = 0;

    //!Teleport 4 graphic
    int F4 = 0;

    //!Teleport 5 graphic
    int F5 = 0;

    //!Teleport 6 graphic
    int F6 = 0;

    //!Teleport 7 graphic
    int F7 = 0;

    //!Teleport 8 graphic
    int F8 = 0;

    //!Teleport 9 graphic
    int F9 = 0;

    //!Teleport 10 graphic
    int F10 = 0;

    //!Teleport 11 graphic
    int F11 = 0;

    //!Teleport 12 graphic
    int F12 = 0;

    //!Teleport 13 graphic
    int F13 = 0;

    //!Teleport 14 graphic
    int F14 = 0;

    //!Teleport 15 graphic
    int F15 = 0;

    //!Teleport 16 graphic
    int F16 = 0;

public:
    /*!
	Constructor
	@return
	*/
    CCustomHouseObjectTeleport()
        : CCustomHouseObject()
    {
        memset(&m_Graphics[0], 0, sizeof(m_Graphics));
    }

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
    int Contains(ushort graphic) const;

    //!Maximum number of graphics count
    static const int GRAPHICS_COUNT = 16;

    //!Graphics list
    ushort m_Graphics[GRAPHICS_COUNT];
};
//----------------------------------------------------------------------------------
/*class CCustomHouseObjectTeleportCategory
{
	int Index = 0;

public:
	CCustomHouseObjectTeleportCategory() {}
	virtual ~CCustomHouseObjectTeleportCategory() {}

	vector<CCustomHouseObjectTeleport> m_Items;
};*/
//----------------------------------------------------------------------------------
class CCustomHouseObjectRoof : public CCustomHouseObject
{
public:
    //!Position in category
    int Style = 0;

    //!ClilocID for name of category (for tooltips)
    int TID = 0;

    //!North direction roof graphic
    int North = 0;

    //!East direction roof graphic
    int East = 0;

    //!South direction roof graphic
    int South = 0;

    //!West direction roof graphic
    int West = 0;

    int NSCrosspiece = 0;
    int EWCrosspiece = 0;
    int NDent = 0;
    int SDent = 0;
    int WDent = 0;
    int NTPiece = 0;
    int ETPiece = 0;
    int STPiece = 0;
    int WTPiece = 0;
    int XPiece = 0;
    int Extra = 0;
    int Piece = 0;

    /*!
	Constructor
	@return
	*/
    CCustomHouseObjectRoof()
        : CCustomHouseObject()
    {
        memset(&m_Graphics[0], 0, sizeof(m_Graphics));
    }

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
    int Contains(ushort graphic) const;

    //!Maximum number of graphics count
    static const int GRAPHICS_COUNT = 16;

    //!Graphics list
    ushort m_Graphics[GRAPHICS_COUNT];
};
//----------------------------------------------------------------------------------
class CCustomHouseObjectRoofCategory
{
public:
    //!Index of category
    int Index = 0;

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
public:
    //!Object graphic
    int Graphic = 0;

    //!Check top
    int Top = 0;

    //!Check bottom
    int Bottom = 0;

    //!Adjustment upper north
    int AdjUN = 0;

    //!Adjustment lower north
    int AdjLN = 0;

    //!Adjustment upper east
    int AdjUE = 0;

    //!Adjustment lower east
    int AdjLE = 0;

    //!Adjustment upper south
    int AdjUS = 0;

    //!Adjustment lower south
    int AdjLS = 0;

    //!Adjustment upper west
    int AdjUW = 0;

    //!Adjustment lower west
    int AdjLW = 0;

    //!Check corner with near tiles
    int DirectSupports = 0;

    //!Can move (to/from?) west
    int CanGoW = 0;

    //!Can move (to/from?) north
    int CanGoN = 0;

    //!Can move (to/from?) north/west/south
    int CanGoNWS = 0;

    //!Flag mask to enable/disable objects (in some expansions)
    int FeatureMask = 0;

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
    int Contains(ushort graphic) const;

    //!Maximum number of graphics count
    static const int GRAPHICS_COUNT = 1;

    //!Graphics list
    ushort m_Graphics[GRAPHICS_COUNT];
};
//----------------------------------------------------------------------------------
#endif //CUSTOMHOUSEOBJECTS_H
//----------------------------------------------------------------------------------
