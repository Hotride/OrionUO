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
#include "GameEffect.h"
//----------------------------------------------------------------------------------
//Эффект перемещения предмета
class CGameEffectDrag : public CGameEffect
{
	SETGET(int, OffsetX);
	SETGET(int, OffsetY);

	SETGET(uint, LastMoveTime);
	SETGET(uchar, MoveDelay);

public:
	CGameEffectDrag();
	virtual ~CGameEffectDrag();

	/*!
	Обновить эффект
	@return
	*/
	virtual void Update(class CGameObject *parent);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
