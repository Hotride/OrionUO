/***********************************************************************************
**
** GumpSkills.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPSKILLS_H
#define GUMPSKILLS_H
//----------------------------------------------------------------------------------
#include "GumpBaseScroll.h"
//----------------------------------------------------------------------------------
class CGumpSkills : public CGumpBaseScroll
{
private:
	static const int ID_GS_SHOW_REAL = 1;
	static const int ID_GS_SHOW_CAP = 2;
	static const int ID_GS_BUTTON_NEW_GROUP = 3;
	static const int ID_GS_LOCK_MOVING = 4;
	static const int ID_GS_GROUP_MINIMIZE = 10000;
	static const int ID_GS_GROUP = 11000;
	static const int ID_GS_SKILL_BUTTON = 12000;
	static const int ID_GS_SKILL = 13000;
	static const int ID_GS_SKILL_STATE = 14000;

	bool m_ShowReal;
	bool m_ShowCap;

	//Получить группу над курсором
	class CSkillGroupObject *GetGroupUnderCursor(int startIndex);

	CGUIGumppic *m_BottomLine;
	CGUIGumppic *m_Comment;
	CGUIButton *m_CreateGroup;
	CGUIText *m_SkillSum;
	CGUIDataBox *m_DataBox;

	void UpdateGroupPositions();

protected:
	virtual void UpdateHeight();

public:
	CGumpSkills(uint serial, short x, short y, bool minimized, int height = 250);
	virtual ~CGumpSkills();

	CGUISkillGroup *GetSkillGroup(const int &index);
	CGUISkillItem *GetSkill(const int &index);

	void UpdateSkillValue(const int &index);
	void UpdateSkillValues();

	void UpdateSkillsSum();
	
	void Init();

	virtual void PrepareContent();
	virtual void UpdateContent();

	GUMP_BUTTON_EVENT_H;
	GUMP_CHECKBOX_EVENT_H;
	
	void OnLeftMouseDown();
	virtual bool OnLeftMouseButtonDoubleClick();
	virtual void OnCharPress(const WPARAM &wParam, const LPARAM &lParam);
	virtual void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
