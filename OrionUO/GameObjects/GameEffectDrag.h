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
public:
    //!Offset on screen X
    int OffsetX = 0;

    //!Offset on screen Y
    int OffsetY = 0;

    //!Last move time stamp
    uint LastMoveTime = 0;

    //!Delay for moving
    uchar MoveDelay = 20;

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
