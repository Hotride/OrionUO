/****************************************************************************
**
** ConnectionScreen.cpp
**
** Copyright (C) September 2015 Hotride
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
*****************************************************************************
*/
//---------------------------------------------------------------------------
#include "stdafx.h"

TBaseScreen *CurrentScreen = NULL;
//---------------------------------------------------------------------------
void TBaseScreen::CreateSmoothAction(BYTE action)
{
	if (g_UseSmoothMonitor)
	{
		m_SmoothScreenAction = action;
		g_SmoothMonitorMode = SMOOTH_MONITOR_SUNSET;
		g_SmoothMonitorColor = 1.0f;
	}
	else
		ProcessSmoothAction(action);
}
//---------------------------------------------------------------------------
int TBaseScreen::CalculateScrollerAndTextPosition(int &currentLine, int &visibleLines, int &maxY, int currentY)
{
	//Без смещения
	int scrollerY = 0;

	//Если не выходим за рамки ограничения отрисовки
	if (currentY < maxY)
	{
		//Если скроллер смещен
		if (currentY > 0)
		{
			//Вычисляем процент, на сколько его сместили
			float per = (currentY / (float)maxY) * 100.0f;

			//Вычисляем значение текущей отображаемой линии
			int jsize = (int)((visibleLines * per) / 100.0f);

			//Корректируем значения (при необходимости)
			if (jsize < 1)
				jsize = 1;
			else if (jsize > visibleLines)
				jsize = visibleLines;

			//Выставляем текущее значение линии
			currentLine = jsize;

			//Вычисляем положение скроллера
			if (jsize >= visibleLines) //Максимальное - если достигли конца текста/объектов
				scrollerY = maxY;
			else //Или где-то в заданных пределах
				scrollerY = (int)((maxY * per) / 100.0f);
		}
		else //Обнуляем все, если скроллер не передвигали
		{
			scrollerY = 0;
			currentLine = 0;
		}
	}
	else //Вышли за допустимые пределы, выставляем все по максимуму
	{
		currentLine = visibleLines;
		scrollerY = maxY;
	}

	//Возвращаем позицию скроллера
	return scrollerY;
}
//---------------------------------------------------------------------------
int TBaseScreen::CalculateScrollerY(int &currentLine, int &visibleLines, int &maxY)
{
	//Действия, аналогичные коду выше при смещении скроллера
	float per = (currentLine / (float)visibleLines) * 100.0f;

	int jsize = (int)((visibleLines * per) / 100.0f);

	if (jsize < 1)
		jsize = 1;
	else if (jsize > visibleLines)
		jsize = visibleLines;

	int scrollerY = 0;

	if (jsize >= visibleLines)
		scrollerY = maxY;
	else
		scrollerY = (int)((maxY * per) / 100.0f);

	return scrollerY;
}
//---------------------------------------------------------------------------