/***********************************************************************************
**
** GameEffectMoving.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GAMEEFFECTMOVING_H
#define GAMEEFFECTMOVING_H
//----------------------------------------------------------------------------------
//!Moving effect
class CGameEffectMoving : public CGameEffectDrag
{
	//!Effect's angle
	SETGET(float, Angle, 0.0f);

	//!Offset in the world Z
	SETGET(int, OffsetZ, 0);

public:
	/*!
	Constructor
	@return
	*/
	CGameEffectMoving();

	/*!
	Destructor
	@return
	*/
	virtual ~CGameEffectMoving();

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
