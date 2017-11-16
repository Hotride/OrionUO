// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GumpScreenSelectProfession.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGumpScreenSelectProfession::CGumpScreenSelectProfession()
: CGump(GT_NONE, 0, 0, 0)
{
	WISPFUN_DEBUG("c117_f1");
	m_NoMove = true;
	m_NoClose = true;

	IFOR(i, 0, 3)
	{
		m_StatsSliders[i] = NULL;
		m_SkillsSliders[i] = NULL;
	}

	m_SkillsSliders[3] = NULL;
}
//----------------------------------------------------------------------------------
CGumpScreenSelectProfession::~CGumpScreenSelectProfession()
{
}
//----------------------------------------------------------------------------------
void CGumpScreenSelectProfession::UpdateContent()
{
	WISPFUN_DEBUG("c117_f2");
	Clear();

	if (g_ProfessionManager.Selected == NULL)
	{
		g_OrionWindow.ShowMessage("No items in profession manager!", "Profession error!");
		return;
	}

	if (g_PacketManager.ClientVersion >= CV_308Z)
		UpdateContentNew();
	else
		UpdateContentOld();
}
//----------------------------------------------------------------------------------
void CGumpScreenSelectProfession::UpdateContentOld()
{
	WISPFUN_DEBUG("c117_f3");
	CBaseProfession *obj = g_ProfessionManager.Selected;

	if (obj == NULL)
	{
		g_OrionWindow.ShowMessage("No items in profession manager!", "Profession error!");
		return;
	}

	IFOR(i, 0, 3)
	{
		m_StatsSliders[i] = NULL;
		m_SkillsSliders[i] = NULL;
	}

	m_SkillsSliders[3] = NULL;

	Add(new CGUIGumppicTiled(0x0E14, 0, 0, 640, 480));
	Add(new CGUIGumppic(0x157C, 0, 0));
	Add(new CGUIGumppic(0x15A0, 0, 4));
	Add(new CGUIButton(ID_SPS_QUIT, 0x1589, 0x158A, 0x158B, 555, 4));
	Add(new CGUIButton(ID_SPS_ARROW_PREV, 0x15A1, 0x15A2, 0x15A3, 586, 445));
	Add(new CGUIResizepic(0, 0x0A28, 80, 80, 546, 352));
	Add(new CGUIGumppic(0x058B, 145, 57));
	Add(new CGUIGumppic(0x0589, 222, 44));
	Add(new CGUIButton(ID_SPS_ARROW_BACK_PROFESSION, 0x119C, 0x119D, 0x119E, 200, 356));

	CGUIHTMLGump *htmlGump = new CGUIHTMLGump(ID_SPS_HTMLGUMP, 0x0BB8, 120, 137, 221, 214, true, true);
	Add(htmlGump);

	if (!g_SelectProfessionScreen.SkillSelection)
	{
		//!Используем обработку HTML-тэгов при создании текстуры текста
		g_FontManager.SetUseHTML(true);

		CGUIText *text = new CGUIText(0, 3, 3);
		text->CreateTextureW(1, obj->Description, 30, 195);
		htmlGump->Add(text);

		//!Выключаем обработку HTML-тэгов
		g_FontManager.SetUseHTML(false);
	}
	else
	{
		int yPtr = 4; // 20;

		IFOR(i, 0, g_SkillsManager.Count)
		{
			CSkill *skill = g_SkillsManager.Get(g_SkillsManager.GetSortedIndex((uint)i));

			if (skill == NULL)
				continue;

			CGUITextEntry *entry = new CGUITextEntry(ID_SPS_SKILLS_LIST + (int)i, 1, 0x0035, 0x0035, 3, yPtr, 0, false, 9);
			entry->m_Entry.SetText(skill->Name);
			entry->m_Entry.CreateTextureA(9, skill->Name, 1, 0, TS_LEFT, 0);
			entry->CheckOnSerial = true;
			entry->ReadOnly = true;

			htmlGump->Add(new CGUIHitBox(ID_SPS_SKILLS_LIST + (int)i, 3, yPtr, 195, entry->m_Entry.m_Texture.Height));
			htmlGump->Add(entry);

			yPtr += entry->m_Entry.m_Texture.Height;
		}
	}

	htmlGump->CalculateDataSize();

	if (obj->Type == PT_CATEGORY) //category
	{
		Add(new CGUIButton(ID_SPS_LABEL_BACK_PROFESSION, obj->Gump, obj->Gump, obj->Gump + 1, 231, 53));

		int offsY = 0;

		int index = 0;

		QFOR(child, obj->m_Items, CBaseProfession*)
		{
			Add(new CGUIGumppic(0x0589, 500, 100 + offsY));

			Add(new CGUIButton(ID_SPS_LABEL + index, child->Gump, child->Gump, child->Gump + 1, 509, 109 + offsY));

			CGUIText *text = (CGUIText*)Add(new CGUIText(0, 350, 135 + offsY));
			text->CreateTextureW(2, ToWString(child->Name), 30, 185, TS_LEFT, UOFONT_SOLID);

			offsY += 79;

			index++;
		}
	}
	else if (obj->Type == PT_PROFESSION) //profession
	{
		Add(new CGUIButton(ID_SPS_LABEL_BACK_PROFESSION, obj->Gump, obj->Gump, obj->Gump + 1, 231, 53));

		const float SphereListWidth = 95.0f;
		float ValPer = 0.0f;

		CProfession *profession = (CProfession*)obj;
		int statVal[3] = { profession->Str, profession->Dex, profession->Int };

		const string statName[3] = { "Strength", "Dexterity", "Intelligence" };

		int yPtr = 136;

		IFOR(i, 0, 3)
		{
			CGUIText *text = new CGUIText(1, 360, yPtr);
			text->CreateTextureA(1, statName[i]);
			Add(text);

			m_StatsSliders[i] = (CGUISlider*)Add(new CGUISlider(ID_SPS_STATS_SPHERE + (int)i, 0x00D8, 0x00D8, 0x00D8, 0x00D5, true, false, 496, yPtr, 95, 10, 45, statVal[i]));
			m_StatsSliders[i]->DefaultTextOffset = -10;
			m_StatsSliders[i]->SetTextParameters(true, STP_LEFT, 1, 1, false);

			yPtr += 30;
		}

		if (profession->DescriptionIndex >= 0)
		{
			yPtr = 260;

			IFOR(i, 0, 3)
			{
				int skillID = profession->GetSkillIndex((int)i);

				if (skillID >= g_SkillsManager.Count)
					skillID = 0;

				CGUIText *text = new CGUIText(1, 360, yPtr);

				CSkill *skill = g_SkillsManager.Get(skillID);

				if (skill != NULL)
					text->CreateTextureA(1, skill->Name, 90, TS_LEFT, UOFONT_FIXED);

				Add(text);

				yPtr += 32;
			}
		}
		else //advanced
		{
			yPtr = 256;

			IFOR(i, 0, 3)
			{
				Add(new CGUIResizepic(ID_SPS_SKILLS_FILED + (int)i, 0x0BB8, 350, yPtr, 105, 25));

				int skillID = profession->GetSkillIndex((uint)i);

				CGUITextEntry *entry = (CGUITextEntry*)Add(new CGUITextEntry(ID_SPS_SKILLS_FILED + (int)i, 0x0386, 0, 0x0021, 354, yPtr + 5, 90, false, 9, TS_LEFT, UOFONT_FIXED));

				CSkill *skill = g_SkillsManager.Get(skillID);

				if (skillID >= g_SkillsManager.Count || skill == NULL)
					entry->m_Entry.SetText("Click here");
				else
					entry->m_Entry.SetText(skill->Name);

				entry->CheckOnSerial = true;
				entry->ReadOnly = true;

				yPtr += 32;
			}
		}

		IFOR(i, 0, 3)
		{
			m_SkillsSliders[i] = (CGUISlider*)Add(new CGUISlider(ID_SPS_SKILLS_SPHERE + (int)i, 0x00D8, 0x00D8, 0x00D8, 0x00D5, true, false, 496, 258 + ((int)i * 32), 95, 0, 50, profession->GetSkillValue((int)i)));
			m_SkillsSliders[i]->DefaultTextOffset = -10;
			m_SkillsSliders[i]->SetTextParameters(true, STP_LEFT, 1, 1, false);
		}

		Add(new CGUIButton(ID_SPS_ARROW_NEXT, 0x15A4, 0x15A5, 0x15A6, 610, 445));
	}
}
//----------------------------------------------------------------------------------
void CGumpScreenSelectProfession::UpdateContentNew()
{
	WISPFUN_DEBUG("c117_f4");
	CBaseProfession *obj = g_ProfessionManager.Selected;

	Add(new CGUIGumppicTiled(0x0E14, 0, 0, 640, 480));
	Add(new CGUIGumppic(0x157C, 0, 0));
	Add(new CGUIGumppic(0x15A0, 0, 4));
	Add(new CGUIButton(ID_SPS_QUIT, 0x1589, 0x158A, 0x158B, 555, 4));
	Add(new CGUIButton(ID_SPS_ARROW_PREV, 0x15A1, 0x15A2, 0x15A3, 586, 445));
	Add(new CGUIResizepic(0, 0x0A28, 100, 80, 470, 372));
	Add(new CGUIGumppic(0x058B, 213, 57));
	Add(new CGUIGumppic(0x0589, 290, 44));

	string str = g_ClilocManager.Cliloc(g_Language)->GetA(3000326, false, "Choose a Trade for Your Character");
	CGUIText *text = new CGUIText(0x0386, 120, 126);
	text->CreateTextureA(2, str, 432, TS_CENTER);
	Add(text);

	Add(new CGUIGumppic(obj->Gump, 299, 53));

	if (obj->Type == PT_CATEGORY) //category
	{
		int offsX = 0;
		int offsY = 0;
		int index = 0;

		QFOR(child, obj->m_Items, CBaseProfession*)
		{
			Add(new CGUIResizepic(ID_SPS_LABEL + index, 0x0BB8, 145 + offsX, 168 + offsY, 175, 34));

			CGUITextEntry *entry = (CGUITextEntry*)Add(new CGUITextEntry(ID_SPS_LABEL + index, 0, 0, 0, 151 + offsX, 174 + offsY, 185, true, 2, TS_LEFT, UOFONT_FIXED | UOFONT_SOLID));
			entry->m_Entry.SetText(child->Name);
			entry->m_Entry.PrepareToDrawW(2, 0, TS_LEFT, UOFONT_FIXED | UOFONT_SOLID);
			entry->CheckOnSerial = true;
			entry->ReadOnly = true;

			Add(new CGUIButton(ID_SPS_LABEL + index, child->Gump, child->Gump, child->Gump + 1, 265 + offsX, 155 + offsY));

			if (offsX)
			{
				offsX = 0;
				offsY += 70;
			}
			else
				offsX = 195;

			index++;
		}
	}
	else if (obj->Type == PT_PROFESSION) //profession
	{
		CProfession *profession = (CProfession*)obj;
		int statVal[3] = { profession->Str, profession->Dex, profession->Int };
		const string statName[3] = { "Strength", "Dexterity", "Intelligence" };

		int yPtr = 171;

		//Stats
		IFOR(i, 0, 3)
		{
			text = (CGUIText*)Add(new CGUIText(1, 160, yPtr));
			text->CreateTextureA(1, statName[i]);

			m_StatsSliders[i] = (CGUISlider*)Add(new CGUISlider(ID_SPS_STATS_SPHERE + (int)i, 0x00D8, 0x00D8, 0x00D8, 0x00D5, true, false, 160, yPtr + 20, 95, 10, 50, statVal[i]));
			m_StatsSliders[i]->SetTextParameters(true, STP_RIGHT, 1, 1, false);

			yPtr += 80;
		}

		yPtr = 171;

		//Skills

		if (g_SelectProfessionScreen.SkillSelection)
		{
			CGUIHTMLGump *htmlGump = (CGUIHTMLGump*)Add(new CGUIHTMLGump(ID_SPS_HTMLGUMP, 0x0BB8, 320, 168, 197, 215, true, true));

			yPtr = 4;

			IFOR(i, 0, g_SkillsManager.Count)
			{
				CSkill *skill = g_SkillsManager.Get(g_SkillsManager.GetSortedIndex((uint)i));

				if (skill == NULL)
					continue;

				CGUITextEntry *entry = new CGUITextEntry(ID_SPS_SKILLS_LIST + (int)i, 1, 0x0035, 0x0035, 2, yPtr, 0, false, 9);
				entry->m_Entry.SetText(skill->Name);
				entry->m_Entry.PrepareToDrawA(9, 1);
				entry->CheckOnSerial = true;
				entry->ReadOnly = true;

				htmlGump->Add(new CGUIHitBox(ID_SPS_SKILLS_LIST + (int)i, 2, yPtr, 190, entry->m_Entry.m_Texture.Height));
				htmlGump->Add(entry);

				yPtr += entry->m_Entry.m_Texture.Height;
			}

			htmlGump->CalculateDataSize();
		}
		else
		{
			int skillsCount = 3;
			int skillStep = 80;

			if (g_PacketManager.ClientVersion >= CV_70160)
			{
				yPtr -= 12;
				skillStep = 70;
				skillsCount++;
			}

			IFOR(i, 0, skillsCount)
			{
				Add(new CGUIResizepic(ID_SPS_SKILLS_FILED + (int)i, 0x0BB8, 340, yPtr, 175, 25));

				int skillID = profession->GetSkillIndex((int)i);

				CGUITextEntry *entry = (CGUITextEntry*)Add(new CGUITextEntry(ID_SPS_SKILLS_FILED + (int)i, 0x0386, 0, 0x0021, 346, yPtr + 5, 90, false, 9, TS_LEFT, UOFONT_FIXED));

				CSkill *skill = g_SkillsManager.Get(skillID);

				if (skillID >= g_SkillsManager.Count || skill == NULL)
					entry->m_Entry.SetText("Click here");
				else
					entry->m_Entry.SetText(skill->Name);

				entry->CheckOnSerial = true;
				entry->ReadOnly = true;

				m_SkillsSliders[i] = (CGUISlider*)Add(new CGUISlider(ID_SPS_SKILLS_SPHERE + (int)i, 0x00D8, 0x00D8, 0x00D8, 0x00D5, true, false, 340, yPtr + 30, 95, 0, 50, profession->GetSkillValue((int)i)));
				m_SkillsSliders[i]->DefaultTextOffset = 60;
				m_SkillsSliders[i]->SetTextParameters(true, STP_RIGHT, 1, 1, false);

				yPtr += skillStep;
			}
		}

		Add(new CGUIButton(ID_SPS_ARROW_NEXT, 0x15A4, 0x15A5, 0x15A6, 610, 445));
	}
}
//----------------------------------------------------------------------------------
void CGumpScreenSelectProfession::InitToolTip()
{
	WISPFUN_DEBUG("c117_f5");
	if (!g_ConfigManager.UseToolTips)
		return;

	uint id = g_SelectedObject.Serial;

	switch (id)
	{
		case ID_SPS_QUIT:
		{
			g_ToolTip.Set(L"Quit Ultima Online", 80);
			break;
		}
		case ID_SPS_ARROW_NEXT:
		{
			g_ToolTip.Set(L"Next screen");
			break;
		}
		case ID_SPS_ARROW_PREV:
		{
			g_ToolTip.Set(L"Preveous screen");
			break;
		}
		case ID_SPS_ARROW_BACK_PROFESSION:
		case ID_SPS_LABEL_BACK_PROFESSION:
		{
			g_ToolTip.Set(L"Back to select profession category", 150);
			break;
		}
		default:
			break;
	}

	if (id >= ID_SPS_LABEL && id < ID_SPS_SKILLS_LIST)
	{
		CBaseProfession *obj = g_ProfessionManager.Selected;

		int index = 0;

		QFOR(child, obj->m_Items, CBaseProfession*)
		{
			if (id == ID_SPS_LABEL + index)
			{
				if (child->DescriptionClilocID)
					g_ToolTip.Set(child->DescriptionClilocID, "Description", 350, false);

				break;
			}

			index++;
		}
	}
}
//----------------------------------------------------------------------------------
void CGumpScreenSelectProfession::GUMP_BUTTON_EVENT_C
{
	WISPFUN_DEBUG("c117_f6");
	CBaseProfession *obj = g_ProfessionManager.Selected;
	CProfession *profession = (CProfession*)obj;

	if (serial == ID_SPS_QUIT) //x button
		g_SelectProfessionScreen.CreateSmoothAction(CSelectProfessionScreen::ID_SMOOTH_SPS_QUIT);
	else if (serial == ID_SPS_ARROW_PREV) //< button
	{
		if (g_PacketManager.ClientVersion >= CV_308Z && g_ProfessionManager.Selected->Type == PT_PROFESSION && g_ProfessionManager.Selected->DescriptionIndex == -1 /*Advanced*/)
		{
			g_ProfessionManager.Selected = g_ProfessionManager.GetParent(g_ProfessionManager.Selected);
			g_SelectProfessionScreen.SkillSelection = 0;

			return;
		}
		else
			g_SelectProfessionScreen.CreateSmoothAction(CSelectProfessionScreen::ID_SMOOTH_SPS_GO_SCREEN_CHARACTER);
	}
	else if (serial == ID_SPS_ARROW_NEXT) //> button
	{
		if (obj->Type == PT_PROFESSION)
		{
			bool passed = true;

			if (profession->DescriptionIndex == -1) //Advanced
			{
				int skillsCount = 3;

				if (g_PacketManager.ClientVersion >= CV_70160)
					skillsCount++;

				IFOR(i, 0, skillsCount)
				{
					IFOR(j, 0, skillsCount)
					{
						if (i != j)
						{
							if (profession->GetSkillIndex((int)i) == 0xFF || profession->GetSkillIndex((int)i) == profession->GetSkillIndex((int)j))
							{
								passed = false;

								g_SelectProfessionScreen.CreateSmoothAction(CSelectProfessionScreen::ID_SMOOTH_SPS_GO_SCREEN_GAME_CONNECT);

								break;
							}
						}
					}
				}
			}

			if (passed)
				g_SelectProfessionScreen.CreateSmoothAction(CSelectProfessionScreen::ID_SMOOTH_SPS_GO_SCREEN_CREATE);
		}
	}
	else if (serial == ID_SPS_ARROW_BACK_PROFESSION || serial == ID_SPS_LABEL_BACK_PROFESSION) //Arrow < or General Label gump
	{
		g_ProfessionManager.Selected = g_ProfessionManager.GetParent(g_ProfessionManager.Selected);
		g_SelectProfessionScreen.SkillSelection = 0;

		return;
	}
	else if (obj->Type == PT_CATEGORY)
	{
		int index = 0;

		QFOR(child, obj->m_Items, CBaseProfession*)
		{
			if (serial == ID_SPS_LABEL + index)
			{
				g_ProfessionManager.Selected = child;
				g_SelectProfessionScreen.SkillSelection = 0;

				if (g_PacketManager.ClientVersion >= CV_308Z && child->Type == PT_PROFESSION && child->DescriptionIndex != -1)
					g_SelectProfessionScreen.CreateSmoothAction(CSelectProfessionScreen::ID_SMOOTH_SPS_GO_SCREEN_CREATE);

				return;
			}

			index++;
		}
	}
	else if (obj->Type == PT_PROFESSION)
	{
		if (serial >= ID_SPS_SKILLS_LIST)
		{
			g_SelectProfessionScreen.SkillSelection = g_SelectProfessionScreen.SkillSelection - 1;;
			int index = serial - ID_SPS_SKILLS_LIST;
			index = g_SkillsManager.GetSortedIndex(index);

			profession->SetSkillIndex(g_SelectProfessionScreen.SkillSelection, index);

			g_SelectProfessionScreen.SkillSelection = 0;
		}
		else
		{
			int skillsCount = 3;

			if (g_PacketManager.ClientVersion >= CV_70160)
				skillsCount++;

			IFOR(i, 0, skillsCount)
			{
				if (serial == ID_SPS_SKILLS_FILED + (int)i)
				{
					g_SelectProfessionScreen.SkillSelection = (int)i + 1;

					break;
				}
			}
		}
	}
}
//----------------------------------------------------------------------------------
void CGumpScreenSelectProfession::GUMP_SLIDER_CLICK_EVENT_C
{
	WISPFUN_DEBUG("c117_f7");
	OnSliderMove(serial);
}
//----------------------------------------------------------------------------------
void CGumpScreenSelectProfession::GUMP_SLIDER_MOVE_EVENT_C
{
	WISPFUN_DEBUG("c117_f8");
	int skillsCount = 3;

	if (g_PacketManager.ClientVersion >= CV_70160)
		skillsCount++;

	//Stats
	if (serial >= ID_SPS_STATS_SPHERE && (int)serial < ID_SPS_STATS_SPHERE + skillsCount)
	{
		if (g_PacketManager.ClientVersion >= CV_308Z)
		{
			if (g_PacketManager.ClientVersion >= CV_70160)
				ShuffleStats(serial - ID_SPS_STATS_SPHERE, 90, 60);
			else
				ShuffleStats(serial - ID_SPS_STATS_SPHERE, 80, 60);
		}
		else
			ShuffleStats(serial - ID_SPS_STATS_SPHERE, 65, 45);
	}

	//Skills
	if (serial >= ID_SPS_SKILLS_SPHERE && (int)serial < ID_SPS_SKILLS_SPHERE + skillsCount)
		ShuffleSkills(serial - ID_SPS_SKILLS_SPHERE);
}
//----------------------------------------------------------------------------------
void CGumpScreenSelectProfession::ShuffleStats(const int &id, const int &maxSum, const int &maxVal)
{
	WISPFUN_DEBUG("c117_f9");
	CProfession *profession = (CProfession*)g_ProfessionManager.Selected;
	int stats[3] = { m_StatsSliders[0]->Value, m_StatsSliders[1]->Value, m_StatsSliders[2]->Value };

	int used_stat = id;
	int others_stat[2] = { 0 };

	others_stat[0] = 0;
	if (others_stat[0] == used_stat)
		others_stat[0]++;

	others_stat[1] = others_stat[0] + 1;
	if (others_stat[1] == used_stat)
	{
		others_stat[1]++;

		if (others_stat[1] == 3)
			others_stat[1] = 0;
	}

	stats[used_stat] = m_StatsSliders[id]->Value;

	int stat_sum = maxSum - (stats[0] + stats[1] + stats[2]);

	if (stat_sum > 0) //stat will decrease
	{
		while (stat_sum > 0)
		{
			if (stats[others_stat[0]] < maxVal)
			{
				stat_sum--;
				stats[others_stat[0]]++;
			}
			else if (stats[others_stat[1]] < maxVal)
			{
				stat_sum--;
				stats[others_stat[1]]++;
			}
			else
				break;
		}
	}
	else if (stat_sum < 0) //stat will increase
	{
		while (stat_sum < 0)
		{
			if (stats[others_stat[0]] > 10)
			{
				stat_sum++;
				stats[others_stat[0]]--;
			}
			else if (stats[others_stat[1]] > 10)
			{
				stat_sum++;
				stats[others_stat[1]]--;
			}
			else
				break;
		}
	}

	IFOR(i, 0, 3)
	{
		m_StatsSliders[i]->Value = stats[i];
		m_StatsSliders[i]->CalculateOffset();
		m_StatsSliders[i]->UpdateText();
	}

	profession->Str = stats[0];
	profession->Dex = stats[1];
	profession->Int = stats[2];
}
//----------------------------------------------------------------------------------
void CGumpScreenSelectProfession::ShuffleSkills(const int &id)
{
	WISPFUN_DEBUG("c117_f10");
	CProfession *profession = (CProfession*)g_ProfessionManager.Selected;
	int skills[4] = { m_SkillsSliders[0]->Value, m_SkillsSliders[1]->Value, m_SkillsSliders[2]->Value, 0 };

	int used_skill = id;
	int others_skills[3] = { 0 };

	others_skills[0] = 0;

	if (others_skills[0] == used_skill)
		others_skills[0]++;

	others_skills[1] = others_skills[0] + 1;

	if (others_skills[1] == used_skill)
		others_skills[1]++;

	others_skills[2] = others_skills[1] + 1;

	if (others_skills[2] == used_skill)
	{
		others_skills[2]++;

		if (others_skills[2] == 5)
			others_skills[2] = 0;
	}

	int skillsCount = 3;
	bool use4Skill = false;

	if (g_PacketManager.ClientVersion >= CV_70160)
	{
		use4Skill = true;
		skillsCount++;
		skills[3] = m_SkillsSliders[3]->Value;
	}

	skills[used_skill] = m_SkillsSliders[id]->Value;

	int skills_sum = 100 - (skills[0] + skills[1] + skills[2] + skills[3]);

	if (skills_sum > 0) //skill will decrease
	{
		while (skills_sum > 0)
		{
			if (skills[others_skills[0]] < 50)
			{
				skills_sum--;
				skills[others_skills[0]]++;
			}
			else if (skills[others_skills[1]] < 50)
			{
				skills_sum--;
				skills[others_skills[1]]++;
			}
			else if (use4Skill && skills[others_skills[2]] < 50)
			{
				skills_sum--;
				skills[others_skills[2]]++;
			}
			else
				break;
		}
	}
	else if (skills_sum < 0) //skill will increase
	{
		while (skills_sum < 0)
		{
			if (skills[others_skills[0]] > 0)
			{
				skills_sum++;
				skills[others_skills[0]]--;
			}
			else if (skills[others_skills[1]] > 0)
			{
				skills_sum++;
				skills[others_skills[1]]--;
			}
			else if (use4Skill && skills[others_skills[2]] > 0)
			{
				skills_sum++;
				skills[others_skills[2]]--;
			}
			else
				break;
		}
	}

	IFOR(i, 0, skillsCount)
	{
		m_SkillsSliders[i]->Value = skills[i];
		m_SkillsSliders[i]->CalculateOffset();
		m_SkillsSliders[i]->UpdateText();
	}

	profession->SetSkillValue(0, skills[0]);
	profession->SetSkillValue(1, skills[1]);
	profession->SetSkillValue(2, skills[2]);
}
//----------------------------------------------------------------------------------
