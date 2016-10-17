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
	uchar dir = m_Direction + 1;
	
	float step_NESW_D = 44.0f / steps; //NW NE SW SE
	float step_NESW = 22.0f / steps; //N E S W

	switch (dir & 7)
	{
		case 0: //NW
		{
			x = 0.0f;
			y *= -step_NESW_D;
			break;
		}
		case 1: //N
		{
			x *= step_NESW;
			y *= -step_NESW;
			break;
		}
		case 2: //NE
		{
			x *= step_NESW_D;
			y = 0.0f;
			break;
		}
		case 3: //E
		{
			x *= step_NESW;
			y *= step_NESW;
			break;
		}
		case 4: //SE
		{
			x = 0.0f;
			y *= step_NESW_D;
			break;
		}
		case 5: //S
		{
			x *= -step_NESW;
			y *= step_NESW;
			break;
		}
		case 6: //SW
		{
			x *= -step_NESW_D;
			y = 0.0f;
			break;
		}
		case 7: //W
		{
			x *= -step_NESW;
			y *= -step_NESW;
			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------------
