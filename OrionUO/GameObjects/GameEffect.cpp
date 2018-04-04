// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GameEffect.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGameEffect::CGameEffect()
: CRenderWorldObject(ROT_EFFECT, 0, 0, 0, 0, 0, 0)
{
}
//----------------------------------------------------------------------------------
CGameEffect::~CGameEffect()
{
}
//----------------------------------------------------------------------------------
/*!
Отрисовать эффект
@param [__in] mode Режим рисования. true - рисование, false - выбор объектов
@param [__in] drawX Экранная координата X объекта
@param [__in] drawY Экранная координата Y объекта
@param [__in] ticks Таймер рендера
@return При выборе объектов возвращает выбранный элемент
*/
void CGameEffect::Draw(const int &x, const int &y)
{
	WISPFUN_DEBUG("c16_f1");
#if UO_DEBUG_INFO!=0
	g_RenderedObjectsCountInGameWindow++;
#endif

	ushort objGraphic = GetCurrentGraphic();

	ApplyRenderMode();

	if (m_EffectType == EF_MOVING)
	{
		CGameEffectMoving *moving = (CGameEffectMoving*)this;

		int drawEffectX = x + moving->OffsetX;
		int drawEffectY = y + moving->OffsetY + moving->OffsetZ;

		if (moving->FixedDirection)
			g_Orion.DrawStaticArt(objGraphic, m_Color, drawEffectX, drawEffectY);
		else
			g_Orion.DrawStaticArtRotated(objGraphic, m_Color, drawEffectX, drawEffectY, moving->Angle);
	}
	else if (m_EffectType == EF_DRAG)
	{
		CGameEffectDrag *dragEffect = (CGameEffectDrag*)this;

		g_Orion.DrawStaticArt(m_Graphic, m_Color, x - dragEffect->OffsetX, y - dragEffect->OffsetY);
	}
	else
		g_Orion.DrawStaticArt(objGraphic, m_Color, x, y);

	RemoveRenderMode();
}
//----------------------------------------------------------------------------------
void CGameEffect::Update(CGameObject *parent)
{
	WISPFUN_DEBUG("c16_f2");
	if (m_EffectType != EF_MOVING)
	{
		if (m_Duration < g_Ticks)
		{
			if (parent != NULL)
				parent->RemoveEffect(this);
			else
				g_EffectManager.RemoveEffect(this);
		}
		else if (m_LastChangeFrameTime < g_Ticks)
		{
			m_LastChangeFrameTime = g_Ticks + m_Speed;

			if (m_EffectType == EF_LIGHTING)
			{
				m_AnimIndex++;

				if (m_AnimIndex >= 10)
				{
					if (parent != NULL)
						parent->RemoveEffect(this);
					else
						g_EffectManager.RemoveEffect(this);
				}
			}
			else
				CalculateCurrentGraphic();
		}
	}
	else if (m_LastChangeFrameTime < g_Ticks)
	{
		m_LastChangeFrameTime = g_Ticks + m_Speed;

		CalculateCurrentGraphic();
	}
}
//----------------------------------------------------------------------------------
/*!
Вычислить текущий индекс картинки
@return Индекс картинки
*/
ushort CGameEffect::CalculateCurrentGraphic()
{
	WISPFUN_DEBUG("c16_f3");
	uint addressAnimData = (uint)g_FileManager.m_AnimdataMul.Start;

	if (addressAnimData)
	{
		uint addr = (m_Graphic * 68) + 4 * ((m_Graphic / 8) + 1);
		PANIM_DATA pad = (PANIM_DATA)(addressAnimData + addr);

		if (m_AnimIndex < (int)pad->FrameCount)
		{
			m_Increment = pad->FrameData[m_AnimIndex];
			m_AnimIndex++;
		}

		if (m_AnimIndex >= (int)pad->FrameCount)
			m_AnimIndex = 0;
	}

	return m_Graphic + m_Increment;
}
//----------------------------------------------------------------------------------
/*!
Получить текущий индекс картинки
@return Индекс картинки
*/
ushort CGameEffect::GetCurrentGraphic()
{
	WISPFUN_DEBUG("c16_f4");
	return m_Graphic + m_Increment;
}
//----------------------------------------------------------------------------------
/*!
Применение режима отображения
@return 
*/
void CGameEffect::ApplyRenderMode()
{
	WISPFUN_DEBUG("c16_f5");
	switch (m_RenderMode)
	{
		case 1: //ok
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_ZERO, GL_SRC_COLOR);
			break;
		}
		case 2: //ok
		case 3: //ok
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			break;
		}
		case 4: //?
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
			break;
		}
		case 5: //?
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
			break;
		}
		case 6: //ok
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
			glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------------
/*!
Отмена режима отображения
@return 
*/
void CGameEffect::RemoveRenderMode()
{
	WISPFUN_DEBUG("c16_f6");
	switch (m_RenderMode)
	{
		case 1: //ok
		case 2: //ok
		case 3: //ok
		{
			glDisable(GL_BLEND);
			break;
		}
		case 4:
		{
			glDisable(GL_BLEND);
			break;
		}
		case 5:
		{
			glDisable(GL_BLEND);
			break;
		}
		case 6: //ok
		{
			glDisable(GL_BLEND);
			glBlendEquation(GL_FUNC_ADD);
			break;
		}
		default:
			break;
	}
}
//---------------------------------------------------------------------------