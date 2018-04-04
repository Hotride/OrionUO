/***********************************************************************************
**
** GameEffectDrag.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GAMEEFFECTDRAG_H
#define GAMEEFFECTDRAG_H
//----------------------------------------------------------------------------------
//!Moving item effect
class CGameEffectDrag : public CGameEffect
{
	//!Offset on screen X
	SETGET(int, OffsetX, 0);

	//!Offset on screen Y
	SETGET(int, OffsetY, 0);

	//!Last move time stamp
	SETGET(uint, LastMoveTime, 0);

	//!Delay for moving
	SETGET(uchar, MoveDelay, 20);

public:
	/*!
	Constructor
	@return
	*/
	CGameEffectDrag();

	/*!
	Destructor
	@return
	*/
	virtual ~CGameEffectDrag();

	/*!
	Update effect data
	@param [__in] parent Parent object
	@return
	*/
	virtual void Update(class CGameObject *parent);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
