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
	SETGET(uint, RightSerial, 0);
	SETGET(uint, MidSerial, 0);

private:
	CRenderObject *m_LeftObject{ NULL };
	CGump *m_LeftGump{ NULL };

	CRenderObject *m_RightObject{ NULL };
	CGump *m_RightGump{ NULL };

	CRenderObject *m_MidObject{ NULL };
	CGump *m_MidGump{ NULL };

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
