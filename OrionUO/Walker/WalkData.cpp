/***********************************************************************************
**
** WalkData.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "WalkData.h"
//----------------------------------------------------------------------------------
void CWalkData::GetOffset(float &x, float &y, float &steps)
{
	float step_NESW_D = 44.0f / steps; //NW NE SW SE
	float step_NESW = 22.0f / steps; //N E S W

	switch (m_Direction & 7)
	{
		case 0: //W
		{
			x *= step_NESW;
			y *= -step_NESW;
			break;
		}
		case 1: //NW
		{
			x *= step_NESW_D;
			y = 0.0f;
			break;
		}
		case 2: //N
		{
			x *= step_NESW;
			y *= step_NESW;
			break;
		}
		case 3: //NE
		{
			x = 0.0f;
			y *= step_NESW_D;
			break;
		}
		case 4: //E
		{
			x *= -step_NESW;
			y *= step_NESW;
			break;
		}
		case 5: //SE
		{
			x *= -step_NESW_D;
			y = 0.0f;
			break;
		}
		case 6: //S
		{
			x *= -step_NESW;
			y *= -step_NESW;
			break;
		}
		case 7: //SW
		{
			x = 0.0f;
			y *= -step_NESW_D;
			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------------
