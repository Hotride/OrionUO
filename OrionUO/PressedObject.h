/***********************************************************************************
**
** PressedObject.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef PRESSEDOBJECT_H
#define PRESSEDOBJECT_H
//----------------------------------------------------------------------------------
class CPressedObject
{
	SETGET(uint, LeftSerial, 0);
	SETGETP(CRenderObject*, LeftObject, NULL);
	SETGETP(CGump*, LeftGump, NULL);

	SETGET(uint, RightSerial, 0);
	SETGETP(CRenderObject*, RightObject, NULL);
	SETGETP(CGump*, RightGump, NULL);

	SETGET(uint, MidSerial, 0);
	SETGETP(CRenderObject*, MidObject, NULL);
	SETGETP(CGump*, MidGump, NULL);

public:
	CPressedObject();
	virtual ~CPressedObject();

	void ClearAll();

	void Clear(CRenderObject *obj);

	void ClearLeft();
	void ClearRight();
	void ClearMid();

	void Init(const CPressedObject &obj);

	void InitLeft(CRenderObject *obj, CGump *gump = NULL);
	void InitLeft(const CSelectedObject &obj);

	void InitRight(CRenderObject *obj, CGump *gump = NULL);
	void InitRight(const CSelectedObject &obj);

	void InitMid(CRenderObject *obj, CGump *gump = NULL);
	void InitMid(const CSelectedObject &obj);

	bool TestMoveOnDrag();
};
//----------------------------------------------------------------------------------
extern CPressedObject g_PressedObject;
//----------------------------------------------------------------------------------
#endif
