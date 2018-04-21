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
class CGumpSkills : public CGumpBaseScroll
{
private:
	static constexpr int ID_GS_SHOW_REAL = 1;
	static constexpr int ID_GS_SHOW_CAP = 2;
	static constexpr int ID_GS_BUTTON_NEW_GROUP = 3;
	static constexpr int ID_GS_LOCK_MOVING = 4;
	static constexpr int ID_GS_GROUP_MINIMIZE = 10000;
	static constexpr int ID_GS_GROUP = 11000;
	static constexpr int ID_GS_SKILL_BUTTON = 12000;
	static constexpr int ID_GS_SKILL = 13000;
	static constexpr int ID_GS_SKILL_STATE = 14000;

	bool m_ShowReal{ false };
	bool m_ShowCap{ false };

	//Получить группу над курсором
	class CSkillGroupObject *GetGroupUnderCursor(int &index);

	CGUIGumppic *m_BottomLine{ NULL };
	CGUIGumppic *m_Comment{ NULL };
	CGUIButton *m_CreateGroup{ NULL };
	CGUIText *m_SkillSum{ NULL };
	CGUICheckbox *m_CheckboxShowReal{ NULL };
	CGUICheckbox *m_CheckboxShowCap{ NULL };

	void UpdateGroupPositions();

	void SetGroupTextFromEntry();

protected:
	virtual void CalculateGumpState();

	virtual void UpdateHeight();

public:
	CGumpSkills(short x, short y, bool minimized, int height);
	virtual ~CGumpSkills();

	void UpdateGroupText();

	CGUISkillGroup *GetSkillGroup(int index);
	CGUISkillItem *GetSkill(int index);

	void UpdateSkillValue(int index);
	void UpdateSkillValues();

	void UpdateSkillsSum();
	
	void Init();

	virtual void InitToolTip();

	virtual void PrepareContent();

	GUMP_BUTTON_EVENT_H;
	GUMP_CHECKBOX_EVENT_H;
	GUMP_TEXT_ENTRY_EVENT_H;
	
	virtual void OnLeftMouseButtonUp();
	virtual bool OnLeftMouseButtonDoubleClick();
	virtual void OnCharPress(const WPARAM &wParam, const LPARAM &lParam);
	virtual void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
