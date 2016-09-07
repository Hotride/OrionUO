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
#include "Globals.h"
#include "BaseQueue.h"
//----------------------------------------------------------------------------------
//Класс для работы с объектами рендера
class CRenderObject : public CBaseQueueItem
{
	SETGET(uint, Serial);
	SETGET(ushort, Graphic);
	SETGET(ushort, Color);
	SETGET(short, X);
	SETGET(short, Y);

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
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
