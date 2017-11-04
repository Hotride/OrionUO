// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** WalkData.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
void CWalkData::GetOffset(float &x, float &y, float &steps)
{
	WISPFUN_DEBUG("c178_f1");
	float step_NESW_D = 44.0f / steps; //NW NE SW SE
	float step_NESW = 22.0f / steps; //N E S W

	int checkX = 22;
	int checkY = 22;

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
			checkX = 44;
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
			checkY = 44;
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
			checkX = 44;
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
			checkY = 44;
			break;
		}
		default:
			break;
	}

	int valueX = (int)x;

	if (abs(valueX) > checkX)
	{
		if (valueX < 0)
			x = -(float)checkX;
		else
			x = (float)checkX;
	}

	int valueY = (int)y;

	if (abs(valueY) > checkY)
	{
		if (valueY < 0)
			y = -(float)checkY;
		else
			y = (float)checkY;
	}
}
//----------------------------------------------------------------------------------
