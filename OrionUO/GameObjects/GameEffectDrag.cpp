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
	if (m_LastMoveTime > g_Ticks)
		return;

	m_OffsetX += 8;
	m_OffsetY += 8;

	m_LastMoveTime = g_Ticks + m_MoveDelay;

	if (m_Duration < g_Ticks)
		g_EffectManager.RemoveEffect(this);
}
//----------------------------------------------------------------------------------
