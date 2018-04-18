﻿// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** Container.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CContainerRect g_ContainerRect;
vector<CContainerOffset> g_ContainerOffset;
//----------------------------------------------------------------------------------
/*CONTAINER_OFFSET g_ContainerOffset[CONTAINERS_COUNT] =
{
	//Gump   OpenSnd  CloseSnd  X   Y   Width Height
	{ 0x0009, 0x0000, 0x0000, { 20, 85, 124, 196 } }, //corpse
	{ 0x003C, 0x0048, 0x0058, { 44, 65, 186, 159 } },
	{ 0x003D, 0x0048, 0x0058, { 29, 34, 137, 128 } },
	{ 0x003E, 0x002F, 0x002E, { 33, 36, 142, 148 } },
	{ 0x003F, 0x004F, 0x0058, { 19, 47, 182, 123 } },
	{ 0x0040, 0x002D, 0x002C, { 16, 51, 150, 140 } },
	{ 0x0041, 0x004F, 0x0058, { 35, 38, 145, 116 } },
	{ 0x0042, 0x002D, 0x002C, { 18, 105, 162, 178 } },
	{ 0x0043, 0x002D, 0x002C, { 16, 51, 181, 124 } },
	{ 0x0044, 0x002D, 0x002C, { 20, 10, 170, 100 } },
	{ 0x0048, 0x002F, 0x002E, { 16, 10, 154, 94 } },
	{ 0x0049, 0x002D, 0x002C, { 18, 105, 162, 178 } },
	{ 0x004A, 0x002D, 0x002C, { 18, 105, 162, 178 } },
	{ 0x004B, 0x002D, 0x002C, { 16, 51, 184, 124 } },
	{ 0x004C, 0x002D, 0x002C, { 46, 74, 196, 184 } },
	{ 0x004D, 0x002F, 0x002E, { 76, 12, 140, 68 } },
	{ 0x004E, 0x002D, 0x002C, { 24, 96, 140, 152 } }, //bugged
	{ 0x004F, 0x002D, 0x002C, { 24, 96, 140, 152 } }, //bugged
	{ 0x0051, 0x002F, 0x002E, { 16, 10, 154, 94 } },
	{ 0x091A, 0x0000, 0x0000, { 1, 13, 260, 199 } }, //game board
	{ 0x092E, 0x0000, 0x0000, { 1, 13, 260, 199 } }, //backgammon game
	{ 0x0104, 0x002F, 0x002E, { 0, 20, 168, 115 } },
	{ 0x0105, 0x002F, 0x002E, { 0, 20, 168, 115 } },
	{ 0x0106, 0x002F, 0x002E, { 0, 20, 168, 115 } },
	{ 0x0107, 0x002F, 0x002E, { 0, 20, 168, 115 } },
	{ 0x0108, 0x004F, 0x0058, { 0, 35, 150, 105 } },
	{ 0x0109, 0x002F, 0x002E, { 0, 20, 175, 105 } },
	{ 0x010A, 0x002F, 0x002E, { 0, 20, 175, 105 } },
	{ 0x010B, 0x002F, 0x002E, { 0, 20, 175, 105 } },
	{ 0x010C, 0x002F, 0x002E, { 0, 20, 168, 115 } },
	{ 0x010D, 0x002F, 0x002E, { 0, 20, 168, 115 } },
	{ 0x010E, 0x002F, 0x002E, { 0, 20, 168, 115 } }
};*/
//----------------------------------------------------------------------------------
/*!
Функция вычисления смещения для текущей картинки
@param [__in] gumpID Индекс картинки
@return 
*/
void CContainerRect::Calculate(ushort gumpID)
{
	WISPFUN_DEBUG("c187_f1");
	//!Указатель на текстуру
	CGLTexture *tex = g_Orion.ExecuteGump(gumpID);

	//!Если текстура есть в памяти
	if (tex != NULL)
	{
		//!Если выключено смещение - открываем гамп в правом верхнем углу клиента
		if (!g_ConfigManager.OffsetInterfaceWindows)
		{
			X = DefaultX;
			Y = DefaultY;
		}
		else //!Или вычисляем смещение и открываем в результируемых координатах
		{
			int passed = 0;

			//!Цикл в 4 итерации и пока не будут валидные координаты
			IFOR(i, 0, 4 && !passed)
			{
				//!Пора изменять смещение по оси Y и обнулять по оси X
				if (X + tex->Width + CONTAINERS_RECT_STEP > g_OrionWindow.GetSize().Width)
				{
					X = CONTAINERS_RECT_DEFAULT_POS;

					//!Если смещение по оси Y достигло максимума - выставим стандартное значение
					if (Y + tex->Height + CONTAINERS_RECT_LINESTEP > g_OrionWindow.GetSize().Height)
						Y = CONTAINERS_RECT_DEFAULT_POS;
					else
						Y += CONTAINERS_RECT_LINESTEP;
				}
				//!Пора изменять смещение по оси X и обнулять по оси Y
				else if (Y + tex->Height + CONTAINERS_RECT_STEP > g_OrionWindow.GetSize().Height)
				{
					//!Если смещение по оси X достигло максимума - выставим стандартное значение
					if (X + tex->Width + CONTAINERS_RECT_LINESTEP > g_OrionWindow.GetSize().Width)
						X = CONTAINERS_RECT_DEFAULT_POS;
					else
						X += CONTAINERS_RECT_LINESTEP;

					Y = CONTAINERS_RECT_DEFAULT_POS;
				}
				else //!Все отлично, пропускаем дальше
					passed = (int)i + 1;
			}

			if (!passed) //!Смещение не вычислено. Выставим значение по-умолчанию
				MakeDefault();
			else if (passed == 1) //!Прошло с 1 раза, можно изменить смещение
			{
				X += CONTAINERS_RECT_STEP;
				Y += CONTAINERS_RECT_STEP;
			}
		}
	}
}
//----------------------------------------------------------------------------------
