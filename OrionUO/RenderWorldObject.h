/***********************************************************************************
**
** RenderWorldObject.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef RENDERWORLDOBJECT_H
#define RENDERWORLDOBJECT_H
//----------------------------------------------------------------------------------
//Класс для работы с объектами рендера
class CRenderWorldObject : public CRenderObject
{
	SETGETE(char, Z, 0, OnChangeZ);
	SETGET(short, PriorityZ, 0);
	SETGET(RENDER_OBJECT_TYPE, RenderType, ROT_GAME_OBJECT);
#if UO_RENDER_LIST_SORT == 1
	SETGET(uchar, CurrentRenderIndex, 0);
	SETGET(uchar, UseInRender, 0);
#endif
	SETGET(bool, NoDrawTile, false);

public:
	CRenderWorldObject(const RENDER_OBJECT_TYPE &renderType, const uint &serial, const ushort &graphic, const ushort &color, const int &x, const int &y, const char &z);
	virtual ~CRenderWorldObject();

	//Ссылка на компонентный состав текстуры
	uchar m_DrawTextureColor[4];

	bool ProcessAlpha(const int &maxAlpha);

	bool RemovedFromRender();

	//Добавить текст в контейнер
	virtual void AddText(CTextData *td) {}

	//Проверить, может ли текст быть прозрачным
	virtual bool TextCanBeTransparent(CRenderTextObject *text) { return false; }

	//Проверка прозрачности (для круга прозрачности)
	virtual bool TranparentTest(const int &playerZ) { return false; }

	//Ссылки на предыдущий и следующий элементы в очереди рендера
	CRenderWorldObject *m_NextXY{ NULL };
	CRenderWorldObject *m_PrevXY{ NULL };

	//Получить указатель на объект ландшафта в данных координатах
	class CLandObject *GetLand();

	//Получить индекс света объекта
	virtual ushort GetLightID() { return 0; }

	//Отрисовать объект
	virtual void Draw(const int &x, const int &y) { }

	//Выбрать объект
	virtual void Select(const int &x, const int &y) { }

	virtual void UpdateGraphicBySeason() {}

	//Удалить объект из очереди рендера
	void RemoveRender();

	//Стандартные состояния по флагам из тайлдаты (вердаты)
	virtual bool IsBackground() { return false; }
	virtual bool IsWeapon() { return false; }
	virtual bool IsTransparent() { return false; }
	virtual bool IsTranslucent() { return false; }
	virtual bool IsWall() { return false; }
	virtual bool IsDamaging() { return false; }
	virtual bool IsImpassable() { return false; }
	virtual bool IsWet() { return false; }
	virtual bool IsUnknown() { return false; }
	virtual bool IsSurface() { return false; }
	virtual bool IsBridge() { return false; }
	virtual bool IsStackable() { return false; }
	virtual bool IsWindow() { return false; }
	virtual bool IsNoShoot() { return false; }
	virtual bool IsPrefixA() { return false; }
	virtual bool IsPrefixAn() { return false; }
	virtual bool IsInternal() { return false; }
	virtual bool IsFoliage() { return false; }
	virtual bool IsPartialHue() { return false; }
	virtual bool IsUnknown1() { return false; }
	virtual bool IsMap() { return false; }
	virtual bool IsContainer() { return false; }
	virtual bool IsWearable() { return false; }
	virtual bool IsLightSource() { return false; }
	virtual bool IsAnimated() { return false; }
	virtual bool IsNoDiagonal() { return false; }
	virtual bool IsUnknown2() { return false; }
	virtual bool IsArmor() { return false; }
	virtual bool IsRoof() { return false; }
	virtual bool IsDoor() { return false; }
	virtual bool IsStairBack() { return false; }
	virtual bool IsStairRight() { return false; }

	virtual bool IsWorldObject() { return true; }

	//Если это объект группы статики
	virtual bool IsStaticGroupObject() { return false; }

	//Идентификация объектов
	virtual bool IsLandObject() { return false; }
	virtual bool IsStaticObject() { return false; }
	virtual bool IsMultiObject() { return false; }
	virtual bool IsEffectObject() { return false; }

	virtual class CRenderStaticObject *StaticGroupObjectPtr() { return NULL; }
	virtual class CLandObject *LandObjectPtr() { return NULL; }
	virtual class CGameCharacter *GameCharacterPtr() { return NULL; }

	virtual void UpdateRealDrawCoordinates();
	virtual void UpdateDrawCoordinates();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
