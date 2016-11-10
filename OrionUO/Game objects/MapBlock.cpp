/***********************************************************************************
**
** MapBlock.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "MapBlock.h"
#include "../Managers/FileManager.h"
#include "../Managers/MapManager.h"
#include "../MulStruct.h"
//----------------------------------------------------------------------------------
CMapBlock::CMapBlock(const uint &index)
: CBaseQueueItem(), m_Index(index), m_LastAccessTime(GetTickCount()), m_X(0), m_Y(0)
{
	//Обнуляем блок
	IFOR(i, 0, 8)
	{
		IFOR(j, 0, 8)
			Block[i][j] = NULL;
	}
}
//----------------------------------------------------------------------------------
CMapBlock::~CMapBlock()
{
	//При удалении блока очищаем список отрисовки блока и удаляем элементы
	IFOR(i, 0, 8)
	{
		IFOR(j, 0, 8)
		{
			CMapObject *obj = Block[i][j];

			if (obj != NULL)
			{
				CRenderWorldObject *render = GetRender(i, j);

				while (render != NULL)
				{
					CRenderWorldObject *next = render->m_NextXY;
					render->m_PrevXY = NULL;
					render->m_NextXY = NULL;
					render = next;
				}

				delete obj;
				Block[i][j] = NULL;
			}
		}
	}

	m_Items = NULL;
}
//----------------------------------------------------------------------------------
bool CMapBlock::HasNoExternalData()
{
	IFOR(x, 0, 8)
	{
		IFOR(y, 0, 8)
		{
			for (CRenderWorldObject *obj = GetRender(x, y); obj != NULL; obj = obj->m_NextXY)
			{
				if (!obj->IsLandObject() && !obj->IsStaticObject())
					return false;
			}
		}
	}

	return true;
}
//----------------------------------------------------------------------------------
ushort CMapBlock::GetRadarColor(const int &x, const int &y)
{
	ushort color = 0;
	CRenderWorldObject *obj = Block[x][y];

	//Получаем указатель на первый элемент списка
	while (obj != NULL && obj->m_PrevXY != NULL)
		obj = obj->m_PrevXY;

	//Пройдемся по всему списку и запомним последний встретившийся ИД предмета
	while (obj != NULL)
	{
		switch (obj->RenderType)
		{
			case ROT_LAND_OBJECT:
			case ROT_STATIC_OBJECT:
			case ROT_MULTI_OBJECT:
			{
				color = obj->Graphic;

				break;
			}
			default:
				break;
		}

		obj = obj->m_NextXY;
	}

	//Вернем последнее найденное
	return color;
}
//----------------------------------------------------------------------------------
void CMapBlock::CreateLandTextureRect()
{
	//Подкорректируем индекс карты
	int map = g_MapManager->GetActualMap();

	//И пройдемся по всем позициям ландшафта блока
	IFOR(x, 0, 8)
	{
		IFOR(y, 0, 8)
		{
			//Указатель на землю
			CLandObject *obj = GetLand(x, y);

			if (obj != NULL)
			{
				int tileX = obj->X;
				int tileY = obj->Y;
				char tileZ1 = obj->Z;

				//Если это тайл воды с отсутствующей текстурой или все Z-координаты равны - укажем что это тайл из артов
				if (obj->IsStretched || !TestStretched(tileX, tileY, tileZ1, map, true))
				{
					obj->IsStretched = false;

					obj->MinZ = tileZ1;
				}
				else //Или же - текстура
				{
					obj->IsStretched = true;

					obj->UpdateZ(GetLandZ(tileX, tileY + 1, map), GetLandZ(tileX + 1, tileY + 1, map), GetLandZ(tileX + 1, tileY, map));

					CVector vec[3][3][4];

					IFOR(i, -1, 2)
					{
						int curX = tileX + i;
						int curI = i + 1;

						IFOR(j, -1, 2)
						{
							int curY = tileY + j;
							int curJ = j + 1;

							char currentZ = GetLandZ(curX, curY, map);
							char leftZ = GetLandZ(curX, curY + 1, map);
							char rightZ = GetLandZ(curX + 1, curY, map);
							char bottomZ = GetLandZ(curX + 1, curY + 1, map);

							if (currentZ == leftZ && currentZ == rightZ && currentZ == bottomZ)
							{
								IFOR(k, 0, 4)
									vec[curI][curJ][k].Link(0.0, 0.0, 1.0);
							}
							else
							{
								vec[curI][curJ][0].Link(-22.0, 22.0, (currentZ - rightZ) * 4);
								vec[curI][curJ][0].Merge(-22.0, -22.0, (leftZ - currentZ) * 4);
								vec[curI][curJ][0].Normalize();
								
								vec[curI][curJ][1].Link(22.0, 22.0, (rightZ - bottomZ) * 4);
								vec[curI][curJ][1].Merge(-22.0, 22.0, (currentZ - rightZ) * 4);
								vec[curI][curJ][1].Normalize();
								
								vec[curI][curJ][2].Link(22.0, -22.0, (bottomZ - leftZ) * 4);
								vec[curI][curJ][2].Merge(22.0, 22.0, (rightZ - bottomZ) * 4);
								vec[curI][curJ][2].Normalize();
								
								vec[curI][curJ][3].Link(-22.0, -22.0, (leftZ - currentZ) * 4);
								vec[curI][curJ][3].Merge(22.0, -22.0, (bottomZ - leftZ) * 4);
								vec[curI][curJ][3].Normalize();
							}
						}
					}

					int i = 1;
					int j = 1;

					obj->m_Normals[0].Link(vec[i - 1][j - 1][2]);
					obj->m_Normals[0].Add(vec[i - 1][j][1]);
					obj->m_Normals[0].Add(vec[i][j - 1][3]);
					obj->m_Normals[0].Add(vec[i][j][0]);
					obj->m_Normals[0].Normalize();

					obj->m_Normals[1].Link(vec[i][j - 1][2]);
					obj->m_Normals[1].Add(vec[i][j][1]);
					obj->m_Normals[1].Add(vec[i + 1][j - 1][3]);
					obj->m_Normals[1].Add(vec[i + 1][j][0]);
					obj->m_Normals[1].Normalize();

					obj->m_Normals[2].Link(vec[i][j][2]);
					obj->m_Normals[2].Add(vec[i][j + 1][1]);
					obj->m_Normals[2].Add(vec[i + 1][j][3]);
					obj->m_Normals[2].Add(vec[i + 1][j + 1][0]);
					obj->m_Normals[2].Normalize();

					obj->m_Normals[3].Link(vec[i - 1][j][2]);
					obj->m_Normals[3].Add(vec[i - 1][j + 1][1]);
					obj->m_Normals[3].Add(vec[i][j][3]);
					obj->m_Normals[3].Add(vec[i][j + 1][0]);
					obj->m_Normals[3].Normalize();
				}

				AddRender(obj, x, y);
			}
		}
	}
}
//----------------------------------------------------------------------------------
bool CMapBlock::TestStretched(const int &x, const int &y, const char &z, const int &map, const bool &recurse)
{
	bool result = false;

	IFOR(i, -1, 2 && !result)
	{
		IFOR(j, -1, 2 && !result)
		{
			if (recurse)
				result = TestStretched(x + i, y + j, z, map, false);
			else
			{
				char testZ = GetLandZ(x + i, y + j, map);

				result = (testZ != z && testZ != -125);
			}
		}
	}

	return result;
}
//----------------------------------------------------------------------------------
char CMapBlock::GetLandZ(const int &x, const int &y, const int &map)
{
	CIndexMap *blockIndex = g_MapManager->GetIndex(map, x / 8, y / 8);

	//Проверки актуальности данных
	if (blockIndex == NULL || blockIndex->MapAddress == 0)
		return -125;

	int mX = x % 8;
	int mY = y % 8;

	return ((PMAP_BLOCK)blockIndex->MapAddress)->Cells[mY * 8 + mX].Z;
}
//----------------------------------------------------------------------------------
CLandObject *CMapBlock::GetLand(const int &x, const int &y)
{
	CMapObject *obj = Block[x][y];

	//Пройдемся по MapObject'ам блока
	while (obj != NULL)
	{
		//Если земля - можно прервать поиск
		if (obj->IsLandObject())
			break;

		obj = (CMapObject*)obj->m_Next;
	}

	return (CLandObject*)obj;
}
//----------------------------------------------------------------------------------
char CMapBlock::GetRenderZ(CRenderWorldObject *item)
{
	//Получаем Z-координату для рендера
	char z = item->Z;

	CLandObject *land = item->LandObjectPtr();

	//Исключения для земли, если это тайл текстуры
	if (land != NULL && land->IsStretched)
		z = (char)land->Serial;

	return z;
}
//----------------------------------------------------------------------------------
void CMapBlock::AddRender(CRenderWorldObject *item, const int &x, const int &y)
{
	item->RemoveRender();

	CRenderWorldObject *obj = Block[x][y];

	if (obj == item)
	{
		if (obj->m_Prev != NULL)
			obj = (CRenderWorldObject*)obj->m_Prev;
		else if (obj->m_Next != NULL)
			obj = (CRenderWorldObject*)obj->m_Next;
		else
			return;
	}

	while (obj != NULL && obj->m_PrevXY != NULL)
		obj = obj->m_PrevXY;

	char inZ = GetRenderZ(item);
	BYTE inRQI = item->RenderQueueIndex;
	int inSumZ = inZ + inRQI;

	while (obj != NULL /*&& obj->m_NextDraw != NULL*/)
	{
		char objZ = GetRenderZ(obj);
		BYTE objRQI = obj->RenderQueueIndex;
		int objSumZ = objZ + objRQI;

		bool condition = false;

		if (inSumZ < objSumZ)
			condition = true;
		else if (inSumZ == objSumZ)
		{
			if (!inRQI && objRQI)
				condition = true;
			else if (inRQI < objRQI)
				condition = true;
		}

		if (condition)
		{
			if (obj->m_PrevXY != NULL)
			{
				obj = obj->m_PrevXY;
				break;
			}
			else
			{
				obj->m_PrevXY = item;
				item->m_NextXY = obj;
				item->m_PrevXY = NULL;

				return;
			}
		}

		if (obj->m_NextXY == NULL)
			break;

		obj = obj->m_NextXY;
	}

	CRenderWorldObject *next = NULL;
	
	if (obj != NULL)
	{
		next = obj->m_NextXY;
		obj->m_NextXY = item;
	}
	
	item->m_PrevXY = obj;
	item->m_NextXY = next;

	if (next != NULL)
		next->m_PrevXY = item;
}
//----------------------------------------------------------------------------------
CRenderWorldObject *CMapBlock::GetRender(const int &x, const int &y)
{
	CRenderWorldObject *obj = Block[x][y];

	//Найдем указатель на первый элемент списка рендера
	while (obj != NULL && obj->m_PrevXY != NULL)
		obj = obj->m_PrevXY;

	return obj;
}
//----------------------------------------------------------------------------------
CMapObject *CMapBlock::AddObject(CMapObject *obj, const int &x, const int &y)
{
	if (Block[x][y] != NULL)
	{
		CMapObject *item = Block[x][y];

		while (item != NULL)
		{
			if (!item->IsLandObject() && item->Z > obj->Z)
			{
				if (item->m_Prev != NULL)
				{
					item = (CMapObject*)item->m_Prev;
					break;
				}

				m_Items = obj;
				obj->m_Prev = NULL;
				obj->m_Next = item;
				item->m_Prev = obj;

				AddRender(obj, x, y);

				return obj;
			}
			else if (item->m_Next == NULL)
				break;

			item = (CMapObject*)item->m_Next;
		}
		
		CMapObject *next = (CMapObject*)item->m_Next;
		
		item->m_Next = obj;
		obj->m_Next = next;
		obj->m_Prev = item;
		
		if (next != NULL)
			next->m_Prev = obj;

		AddRender(obj, x, y);
	}
	else
	{
		Block[x][y] = obj;
		obj->m_Next = NULL;
		obj->m_Prev = NULL;
		obj->m_NextXY = NULL;
		obj->m_PrevXY = NULL;
	}

	return obj;
}
//----------------------------------------------------------------------------------
