/****************************************************************************
**
** RenderObject.h
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
#ifndef RenderObjectH
#define RenderObjectH
//---------------------------------------------------------------------------
class TLandObject;
//---------------------------------------------------------------------------
class TRenderObject : public TBaseQueueItem
{
private:
	DWORD m_Serial;
	WORD m_Graphic;
	WORD m_Color;
	short m_X;
	short m_Y;
public:
	TRenderObject(DWORD serial, WORD graphic, WORD color, short x, short y);
	virtual ~TRenderObject();

	SETGET(DWORD, Serial)
	SETGET(WORD, Graphic)
	SETGET(WORD, Color)
	SETGETEX(short, X)
	SETGETEX(short, Y)

	virtual void PrepareTextures() {}

	virtual int Draw(bool &mode) {return 0;}
};
//---------------------------------------------------------------------------
class TRenderWorldObject : public TRenderObject
{
private:
	char m_Z;
	BYTE m_RenderQueueIndex;
	RENDER_OBJECT_TYPE m_RenderType;
public:
	TRenderWorldObject(RENDER_OBJECT_TYPE renderType, DWORD serial, WORD graphic, WORD color, short x, short y, char z);
	virtual ~TRenderWorldObject();
	
	TTextContainer *m_TextControl; //Ссылка на контейнер для текста
	
	//Добавить текст в контейнер
	virtual void AddText(TTextData *td);
	
	int GetTextOffsetX(TTextData *text);
	int GetTextOffsetY(TTextData *text);
	bool CanBeTransparent(TRenderTextObject *text);

	SETGETEX(char, Z)
	SETGETEX(BYTE, RenderQueueIndex)
	SETGET(RENDER_OBJECT_TYPE, RenderType)

	TRenderWorldObject *m_NextXY;
	TRenderWorldObject *m_PrevXY;

	//TRenderWorldObject *m_NextDraw;
	//TRenderWorldObject *m_PrevDraw;

	TLandObject *GetLand();

	//bool InRenderList() {return (m_NextDraw != NULL || m_PrevDraw != NULL);}

	void RemoveRender();

	virtual bool IsBackground() {return false;}
	virtual bool IsWeapon() {return false;}
	virtual bool IsTransparent() {return false;}
	virtual bool IsTranslucent() {return false;}
	virtual bool IsWall() {return false;}
	virtual bool IsDamaging() {return false;}
	virtual bool IsImpassable() {return false;}
	virtual bool IsWet() {return false;}
	virtual bool IsUnknown() {return false;}
	virtual bool IsSurface() {return false;}
	virtual bool IsBridge() {return false;}
	virtual bool IsStackable() {return false;}
	virtual bool IsWindow() {return false;}
	virtual bool IsNoShoot() {return false;}
	virtual bool IsPrefixA() {return false;}
	virtual bool IsPrefixAn() {return false;}
	virtual bool IsInternal() {return false;}
	virtual bool IsFoliage() {return false;}
	virtual bool IsPartialHue() {return false;}
	virtual bool IsUnknown1() {return false;}
	virtual bool IsMap() {return false;}
	virtual bool IsContainer() {return false;}
	virtual bool IsWearable() {return false;}
	virtual bool IsLightSource() {return false;}
	virtual bool IsAnimated() {return false;}
	virtual bool IsNoDiagonal() {return false;}
	virtual bool IsUnknown2() {return false;}
	virtual bool IsArmor() {return false;}
	virtual bool IsRoof() {return false;}
	virtual bool IsDoor() {return false;}
	virtual bool IsStairBack() {return false;}
	virtual bool IsStairRight() {return false;}

	virtual bool IsLandObject() {return false;}
	virtual bool IsStaticObject() {return false;}
	virtual bool IsGameObject() {return false;}
	virtual bool IsMultiObject() {return false;}
	virtual bool IsEffectObject() {return false;}
};
//---------------------------------------------------------------------------
extern TRenderWorldObject *g_SelectedObject;
extern TRenderWorldObject *g_RenderList;
//---------------------------------------------------------------------------
#endif