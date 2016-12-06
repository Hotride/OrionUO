/***********************************************************************************
**
** Container.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "Container.h"
#include "OrionUO.h"
#include "OrionWindow.h"
#include "Managers/ConfigManager.h"
#include "Game objects/GameWorld.h"
//----------------------------------------------------------------------------------
deque<CContainerStackItem> g_ContainerStack;
uint g_CheckContainerStackTimer = 0;
CContainerRect g_ContainerRect;
CUseItemActions g_UseItemActions;
//----------------------------------------------------------------------------------
void CUseItemActions::Add(const uint &serial)
{
	for (deque<uint>::iterator i = m_List.begin(); i != m_List.end(); i++)
	{
		if (*i == serial)
			return;
	}

	m_List.push_back(serial);
}
//----------------------------------------------------------------------------------
void CUseItemActions::Process()
{
	if (m_List.size() && m_Timer <= g_Ticks)
	{
		uint serial = m_List.front();
		m_List.pop_front();

		if (g_World->FindWorldObject(serial) != NULL)
		{
			if (serial < 0x40000000) //NPC
				g_Orion.PaperdollReq(serial);
			else //item
				g_Orion.DoubleClick(serial);

			m_Timer = g_Ticks + 1000;
		}
	}
}
//----------------------------------------------------------------------------------
CContainerStackItem::CContainerStackItem(uint serial, short x, short y, short minimizedX, short minimizedY, bool minimized, bool lockMoving)
: m_Serial(serial), m_X(x), m_Y(y), m_MinimizedX(minimizedX), m_MinimizedY(minimizedY),
m_Minimized(minimized), m_LockMoving(lockMoving)
{
}
//----------------------------------------------------------------------------------
CContainerStackItem::~CContainerStackItem()
{
}
//----------------------------------------------------------------------------------
CONTAINER_OFFSET g_ContainerOffset[CONTAINERS_COUNT] =
{
	//Gump   OpenSnd  CloseSnd  X   Y   Width Height
	{ 0x0009, 0x0000, 0x0000, { 20, 85, 124, 196 } }, //corpse
	{ 0x003C, 0x0048, 0x0058, { 44, 65, 186, 159 } },
	{ 0x003D, 0x0048, 0x0058, { 29, 34, 137, 128 } },
	{ 0x003E, 0x002F, 0x002E, { 33, 36, 142, 148 } },
	{ 0x003F, 0x004F, 0x0058, { 19, 47, 182, 123 } },
	{ 0x0040, 0x004F, 0x0058, { 44, 65, 102, 60 } },
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
	{ 0x092E, 0x0000, 0x0000, { 1, 13, 260, 199 } } //backgammon game
};
//----------------------------------------------------------------------------------
CContainerRect::CContainerRect()
: m_X(100), m_Y(100)
{
}
//----------------------------------------------------------------------------------
/*!
Функция вычисления смещения для текущей картинки
@param [__in] gumpID Индекс картинки
@return 
*/
void CContainerRect::Calculate(ushort gumpID)
{
	//!Указатель на текстуру
	CGLTexture *tex = g_Orion.ExecuteGump(gumpID);

	//!Если текстура есть в памяти
	if (tex != NULL)
	{
		//!Если выключено смещение - открываем гамп в правом верхнем углу клиента
		if (!g_ConfigManager.OffsetInterfaceWindows)
		{
			m_X = g_OrionWindow.Size.Width - tex->Width;
			m_Y = 0;
		}
		else //!Или вычисляем смещение и открываем в результируемых координатах
		{
			int passed = 0;

			//!Цикл в 4 итерации и пока не будут валидные координаты
			IFOR(i, 0, 4 && !passed)
			{
				//!Пора изменять смещение по оси Y и обнулять по оси X
				if (m_X + tex->Width + CONTAINERS_RECT_STEP > g_OrionWindow.Size.Width)
				{
					m_X = CONTAINERS_RECT_DEFAULT_POS;

					//!Если смещение по оси Y достигло максимума - выставим стандартное значение
					if (m_Y + tex->Height + CONTAINERS_RECT_LINESTEP > g_OrionWindow.Size.Height)
						m_Y = CONTAINERS_RECT_DEFAULT_POS;
					else
						m_Y += CONTAINERS_RECT_LINESTEP;
				}
				//!Пора изменять смещение по оси X и обнулять по оси Y
				else if (m_Y + tex->Height + CONTAINERS_RECT_STEP > g_OrionWindow.Size.Height)
				{
					//!Если смещение по оси X достигло максимума - выставим стандартное значение
					if (m_X + tex->Width + CONTAINERS_RECT_LINESTEP > g_OrionWindow.Size.Width)
						m_X = CONTAINERS_RECT_DEFAULT_POS;
					else
						m_X += CONTAINERS_RECT_LINESTEP;

					m_Y = CONTAINERS_RECT_DEFAULT_POS;
				}
				else //!Все отлично, пропускаем дальше
					passed = i + 1;
			}

			if (!passed) //!Смещение не вычислено. Выставим значение по-умолчанию
				MakeDefault();
			else if (passed == 1) //!Прошло с 1 раза, можно изменить смещение
			{
				m_X += CONTAINERS_RECT_STEP;
				m_Y += CONTAINERS_RECT_STEP;
			}
		}
	}
}
//----------------------------------------------------------------------------------
