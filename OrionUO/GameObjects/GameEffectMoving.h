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
public:
	//!Effect's angle
	float Angle = 0.0f;

	//!Offset in the world Z
	int OffsetZ = 0;

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
