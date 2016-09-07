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
#include "Globals.h"
#include "Gumps/Gump.h"
#include "GUI/BaseGUI.h"
#include "SelectedObject.h"
//----------------------------------------------------------------------------------
class CPressedObject
{
	SETGET(uint, LeftSerial);
	SETGET(uint, RightSerial);
	SETGET(uint, MidSerial);

private:
	CRenderObject *m_LeftObject;
	CGump *m_LeftGump;

	CRenderObject *m_RightObject;
	CGump *m_RightGump;

	CRenderObject *m_MidObject;
	CGump *m_MidGump;

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

	CRenderObject *LeftObject() const { return m_LeftObject; }
	CGump *LeftGump() const { return m_LeftGump; }

	CRenderObject *RightObject() const { return m_RightObject; }
	CGump *RightGump() const { return m_RightGump; }

	CRenderObject *MidObject() const { return m_RightObject; }
	CGump *MidGump() const { return m_RightGump; }
};
//----------------------------------------------------------------------------------
extern CPressedObject g_PressedObject;
//----------------------------------------------------------------------------------
#endif
