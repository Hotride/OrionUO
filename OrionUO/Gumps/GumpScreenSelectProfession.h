/***********************************************************************************
**
** GumpScreenSelectProfession.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPSCREENSELECTPROFESSION_H
#define GUMPSCREENSELECTPROFESSION_H
//----------------------------------------------------------------------------------
class CGumpScreenSelectProfession : public CGump
{
private:
	//!Идентификаторы кнопок
	static const int ID_SPS_QUIT = 1;
	static const int ID_SPS_ARROW_PREV = 2;
	static const int ID_SPS_ARROW_NEXT = 3;
	static const int ID_SPS_ARROW_BACK_PROFESSION = 4;
	static const int ID_SPS_LABEL_BACK_PROFESSION = 5;
	static const int ID_SPS_HTMLGUMP = 6;
	static const int ID_SPS_STATS_SPHERE = 10;
	static const int ID_SPS_SKILLS_SPHERE = 20;
	static const int ID_SPS_SKILLS_FILED = 30;
	static const int ID_SPS_LABEL = 50;
	static const int ID_SPS_SKILLS_LIST = 100;

	void UpdateContentOld();
	void UpdateContentNew();

	void ShuffleStats(int id, int maxSum, int maxVal);
	void ShuffleSkills(int id);

	CGUISlider *m_StatsSliders[3];
	CGUISlider *m_SkillsSliders[4];

public:
	CGumpScreenSelectProfession();
	virtual ~CGumpScreenSelectProfession();

	virtual void UpdateContent();

	virtual void InitToolTip();



	GUMP_BUTTON_EVENT_H;
	GUMP_TEXT_ENTRY_EVENT_H { OnButton(serial); }
	GUMP_SLIDER_CLICK_EVENT_H;
	GUMP_SLIDER_MOVE_EVENT_H;
 };
 //----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
