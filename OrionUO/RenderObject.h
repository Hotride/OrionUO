/***********************************************************************************
**
** RenderObject.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H
//----------------------------------------------------------------------------------
//Класс для работы с объектами рендера
class CRenderObject : public CBaseQueueItem
{
	SETGET(uint, Serial, 0);
	SETGET(ushort, Graphic, 0);
	SETGET(ushort, Color, 0);
	SETGETE(int, X, 0, OnChangeX);
	SETGETE(int, Y, 0, OnChangeY);
	SETGET(int, DrawX, 0);
	SETGET(int, DrawY, 0);

public:
	CRenderObject(const uint &serial, const ushort &graphic, const ushort &color, const short &x, const short &y);
	virtual ~CRenderObject();

	//Подготовка текстур
	virtual void PrepareTextures() {}

	//Отрисовка
	//virtual int Draw(bool &mode) {return 0;}

	virtual void OnMouseEnter() {}
	virtual void OnMouseExit() {}

	virtual bool IsGUI() { return false; }
	virtual bool IsText() { return false; }
	virtual bool IsWorldObject() { return false; }
	virtual bool IsGameObject() { return false; }

	virtual void UpdateDrawCoordinates() {}
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
