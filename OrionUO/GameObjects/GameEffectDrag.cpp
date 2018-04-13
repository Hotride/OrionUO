// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GameEffectDrag.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGameEffectDrag::CGameEffectDrag()
: CGameEffect()
{
}
//----------------------------------------------------------------------------------
CGameEffectDrag::~CGameEffectDrag()
{
}
//----------------------------------------------------------------------------------
/*!
Обновить эффект
@return
*/
void CGameEffectDrag::Update(CGameObject *parent)
{
	WISPFUN_DEBUG("c17_f1");
	if (LastMoveTime > g_Ticks)
		return;

	OffsetX += 8;
	OffsetY += 8;

	LastMoveTime = g_Ticks + MoveDelay;

	if (Duration < g_Ticks)
		g_EffectManager.RemoveEffect(this);
}
//----------------------------------------------------------------------------------
