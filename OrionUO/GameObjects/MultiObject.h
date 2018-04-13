/***********************************************************************************
**
** MultiObject.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef MULTIOBJECT_H
#define MULTIOBJECT_H
//----------------------------------------------------------------------------------
//Multi component object
class CMultiObject : public CRenderStaticObject
{
public:
	//!This object is temp object on mouse (when you placing a house)
	bool OnTarget = false;

	//!Original object's graphic
	ushort OriginalGraphic = 0;

	//!Object's state for custom houses buildings
	int State = 0;

	/*!
	Constructor
	@param [__in] graphic Multi object's graphic
	@param [__in] x Coordinate X in the world
	@param [__in] y Coordinate Y in the world
	@param [__in] z Coordinate Z in the world
	@param [__in] flags Multi object flags
	@return
	*/
	CMultiObject(const ushort &graphic, const short &x, const short &y, const char &z, const uint &flags);

	/*!
	Destructor
	@return
	*/
	virtual ~CMultiObject();

	/*!
	Update draw graphic by current season
	@return
	*/
	virtual void UpdateGraphicBySeason();

	/*!
	Draw object in the world
	@param [__in] x Coordinate X on screen
	@param [__in] y Coordinate Y on screen
	@return
	*/
	virtual void Draw(const int &x, const int &y);

	/*!
	Select object in the world
	@param [__in] x Coordinate X on screen
	@param [__in] y Coordinate Y on screen
	@return
	*/
	virtual void Select(const int &x, const int &y);

	/*!
	Check on multi component object
	@return Always true
	*/
	bool IsMultiObject() { return true; }

	/*!
	Check on custom house multi component object
	@return Always false
	*/
	virtual bool IsCustomHouseMulti() { return false; }
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
