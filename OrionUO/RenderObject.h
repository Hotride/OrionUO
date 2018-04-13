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
public:
	uint Serial = 0;
	ushort Graphic = 0;
	ushort Color = 0;
	SETGETE(int, X, 0);
	SETGETE(int, Y, 0);
	int RealDrawX = 0;
	int RealDrawY = 0;
	int DrawX = 0;
	int DrawY = 0;
	bool Changed = true;

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

	virtual void UpdateRealDrawCoordinates() {}
	virtual void UpdateDrawCoordinates() {}
	virtual void UpdateTextCoordinates() {}
	virtual void FixTextCoordinates() {}
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
