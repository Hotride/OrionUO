/***********************************************************************************
**
** GUISkillGroup.h
**
** Компонента для группы скиллов
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUISKILLGROUP_H
#define GUISKILLGROUP_H
//----------------------------------------------------------------------------------
class CGUISkillGroup : public CBaseGUI
{
	//!Свернута или развернута группа
protected:
    bool m_Minimized = false;
public:
    bool GetMinimized() { return m_Minimized; };
    void SetMinimized(bool val);

public:
	CGUISkillGroup(int serial, int minimizeSerial, class CSkillGroupObject *group, int x, int y);
	virtual ~CGUISkillGroup();

	//!Компоненты управления
	CGUIButton *m_Minimizer{ NULL };
	CGUITextEntry *m_Name{ NULL };

	void UpdateDataPositions();

	virtual WISP_GEOMETRY::CSize GetSize();

	virtual void PrepareTextures();

	virtual bool EntryPointerHere();

	virtual void Draw(bool checktrans = false);
	virtual bool Select();

	//!Получить ссылку на выбранную компоненту или на себя
	CBaseGUI *SelectedItem();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
