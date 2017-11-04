/***********************************************************************************
**
** RenderStaticObject.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef RENDERSTATICOBJECT_H
#define RENDERSTATICOBJECT_H
//----------------------------------------------------------------------------------
//Класс обобщенного объекта группы статики
class CRenderStaticObject : public CMapObject
{
	SETGET(char, FoliageTransparentIndex, -1);
	SETGET(char, CanBeTransparent, 0);
	SETGET(bool, Vegetation, false);

	SETGET(ushort, RenderGraphic, 0);
	SETGET(ushort, RenderColor, 0);

protected:
	//Указатель на структуру данных тайлдаты
	STATIC_TILES *m_TiledataPtr{ NULL };

public:
	CRenderStaticObject(const RENDER_OBJECT_TYPE &renderType, const uint &serial, const ushort &graphic, const ushort &color, const short &x, const short &y, const char &z);
	virtual ~CRenderStaticObject();

	CTextContainer *m_TextControl{ NULL }; //Ссылка на контейнер для текста

	virtual void UpdateTextCoordinates();
	virtual void FixTextCoordinates();

	static bool IsNoDrawTile(const ushort &graphic);

	//Отрисовать объект
	virtual void Draw(const int &x, const int &y);

	//Выбрать объект
	virtual void Select(const int &x, const int &y);

	//Добавить текст в контейнер
	virtual void AddText(CTextData *td);

	//Проверка прозрачности (для круга прозрачности)
	virtual bool TranparentTest(const int &playerZPlus5);

	//Проверка, может ли текст быть прозрачным
	bool TextCanBeTransparent(CRenderTextObject *text);

	STATIC_TILES *GetStaticData() { return m_TiledataPtr; }

	//Получить высоту объекта (по данным из тайлдаты)
	uchar GetStaticHeight() { return m_TiledataPtr->Height; }

	//Получить индекс света
	ushort GetLightID() { return m_TiledataPtr->Layer/*LightIndex*/; } //m_TiledataPtr->Layer
	
	//Стандартные состояния по флагам из тайлдаты (вердаты)
	bool IsBackground() {return (m_TiledataPtr->Flags & 0x00000001);}
	bool IsWeapon() {return (m_TiledataPtr->Flags & 0x00000002);}
	bool IsTransparent() {return (m_TiledataPtr->Flags & 0x00000004);}
	bool IsTranslucent() {return (m_TiledataPtr->Flags & 0x00000008);}
	bool IsWall() {return (m_TiledataPtr->Flags & 0x00000010);}
	bool IsDamaging() {return (m_TiledataPtr->Flags & 0x00000020);}
	bool IsImpassable() {return (m_TiledataPtr->Flags & 0x00000040);}
	bool IsWet() {return (m_TiledataPtr->Flags & 0x00000080);}
	bool IsUnknown() {return (m_TiledataPtr->Flags & 0x00000100);}
	bool IsSurface() {return (m_TiledataPtr->Flags & 0x00000200);}
	bool IsBridge() {return (m_TiledataPtr->Flags & 0x00000400);}
	bool IsStackable() {return (m_TiledataPtr->Flags & 0x00000800);}
	bool IsWindow() {return (m_TiledataPtr->Flags & 0x00001000);}
	bool IsNoShoot() {return (m_TiledataPtr->Flags & 0x00002000);}
	bool IsPrefixA() {return (m_TiledataPtr->Flags & 0x00004000);}
	bool IsPrefixAn() {return (m_TiledataPtr->Flags & 0x00008000);}
	bool IsInternal() {return (m_TiledataPtr->Flags & 0x00010000);}
	bool IsFoliage() {return (m_TiledataPtr->Flags & 0x00020000);}
	bool IsPartialHue() {return (m_TiledataPtr->Flags & 0x00040000);}
	bool IsUnknown1() {return (m_TiledataPtr->Flags & 0x00080000);}
	bool IsMap() {return (m_TiledataPtr->Flags & 0x00100000);}
	bool IsContainer() {return (m_TiledataPtr->Flags & 0x00200000);}
	bool IsWearable() {return (m_TiledataPtr->Flags & 0x00400000);}
	bool IsLightSource() {return (m_TiledataPtr->Flags & 0x00800000);}
	bool IsAnimated() {return (m_TiledataPtr->Flags & 0x01000000);}
	bool IsNoDiagonal() {return (m_TiledataPtr->Flags & 0x02000000);}
	bool IsUnknown2() {return (m_TiledataPtr->Flags & 0x04000000);}
	bool IsArmor() {return (m_TiledataPtr->Flags & 0x08000000);}
	bool IsRoof() {return (m_TiledataPtr->Flags & 0x10000000);}
	bool IsDoor() {return (m_TiledataPtr->Flags & 0x20000000);}
	bool IsStairBack() {return (m_TiledataPtr->Flags & 0x40000000);}
	bool IsStairRight() {return (m_TiledataPtr->Flags & 0x80000000);}

	//Это объект группы объектов статики
	bool IsStaticGroupObject() {return true;}

	virtual CRenderStaticObject *StaticGroupObjectPtr() { return this; }
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
