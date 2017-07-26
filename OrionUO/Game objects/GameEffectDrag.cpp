/***********************************************************************************
**
** GameEffectDrag.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GameEffectDrag.h"
#include "../Managers/EffectManager.h"
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
