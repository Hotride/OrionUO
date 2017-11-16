// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GumpScreenCreateCharacter.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGumpScreenCreateCharacter::CGumpScreenCreateCharacter()
: CGump(GT_NONE, 0, 0, 0)
{
	m_NoMove = true;
	m_NoClose = true;
}
//----------------------------------------------------------------------------------
CGumpScreenCreateCharacter::~CGumpScreenCreateCharacter()
{
}
//----------------------------------------------------------------------------------
void CGumpScreenCreateCharacter::UpdateContent()
{
	WISPFUN_DEBUG("c114_f1");
	Clear();

	Add(new CGUIGumppicTiled(0x0E14, 0, 0, 640, 480));
	Add(new CGUIGumppic(0x157C, 0, 0));
	Add(new CGUIGumppic(0x15A0, 0, 4));
	Add(new CGUIButton(ID_CCS_QUIT, 0x1589, 0x158A, 0x158B, 555, 4));
	Add(new CGUIButton(ID_CCS_ARROW_PREV, 0x15A1, 0x15A2, 0x15A3, 586, 445));
	Add(new CGUIButton(ID_CCS_ARROW_NEXT, 0x15A4, 0x15A5, 0x15A6, 610, 445));
	Add(new CGUIResizepic(0, 0x0E10, 82, 125, 151, 310));
	Add(new CGUIGumppic(0x0709, 280, 53));
	Add(new CGUIGumppic(0x070A, 240, 73));
	Add(new CGUIGumppicTiled(0x070B, 248, 73, 215, 0));
	Add(new CGUIGumppic(0x070C, 463, 73));

	CGUITextEntry *entry = (CGUITextEntry*)Add(new CGUITextEntry(ID_CCS_NAME_FIELD, 1, 1, 1, 257, 65, 300, false, 5, TS_LEFT, 0, 32));
	entry->CheckOnSerial = true;
	g_EntryPointer = &entry->m_Entry;
	g_EntryPointer->SetText(g_CreateCharacterScreen.Name);

	Add(new CGUIGumppic(0x0708, 238, 98));
	Add(new CGUIResizepic(0, 0x0E10, 475, 125, 151, 310));

	const ushort toneTextColorRange[2] = { 0x0481, 0x0021 };



	int facialHairCount = g_CreateCharacterManager.GetCurrentFacialHairCount();

	CGUIComboBox *combo = (CGUIComboBox*)Add(new CGUIComboBox(ID_CCS_FACIAL_HAIR_STYLE, 0x0BB8, false, 0x0BB8, 97, 199, 120, facialHairCount, false));
	combo->SelectedIndex = g_CreateCharacterManager.BeardStyle;
	combo->Visible = (!g_CreateCharacterManager.Female && g_CreateCharacterManager.Race != RT_ELF);

	CGUIText *text = new CGUIText(0x0481, 3, -12);
	text->CreateTextureA(9, "Facial Hair Style");
	combo->SetText(text);

	IFOR(i, 0, facialHairCount)
		combo->Add(new CGUIComboboxText(0x0381, 9, g_CreateCharacterManager.GetBeard((uchar)i).Name));

	combo->RecalculateWidth();



	int hairCount = g_CreateCharacterManager.GetCurrentHairCount();

	combo = (CGUIComboBox*)Add(new CGUIComboBox(ID_CCS_HAIR_STYLE, 0x0BB8, false, 0x0BB8, 97, 154, 120, hairCount, false));
	combo->SelectedIndex = g_CreateCharacterManager.HairStyle;

	text = new CGUIText(0x0481, 3, -12);
	text->CreateTextureA(9, "Hair Style");
	combo->SetText(text);

	IFOR(i, 0, hairCount)
		combo->Add(new CGUIComboboxText(0x0381, 9, g_CreateCharacterManager.GetHair((uchar)i).Name));

	combo->RecalculateWidth();



	puchar huesData = (puchar)g_ColorManager.GetHuesRangePointer() + 32 + 4;
	int colorOffsetDivider = sizeof(HUES_GROUP) - 4;

	if (g_CreateCharacterScreen.ColorSelection == 0)
	{
		entry = (CGUITextEntry*)Add(new CGUITextEntry(ID_CCS_SKIN_TONE, toneTextColorRange[0], toneTextColorRange[1], toneTextColorRange[1], 490, 140, 0, false, 9));
		entry->m_Entry.SetText("Skin Tone");
		entry->CheckOnSerial = true;
		entry->ReadOnly = true;

		ushort color = g_CreateCharacterManager.SkinTone - 1;

		int colorIndex = (color + ((color + (color << 2)) << 1)) << 3;
		colorIndex += (colorIndex / colorOffsetDivider) << 2;
		color = *(pushort)(huesData + colorIndex);

		uint clr = g_ColorManager.Color16To32(color);

		Add(new CGUIColoredPolygone(ID_CCS_SKIN_TONE, color, 490, 154, 120, 25, clr));



		entry = (CGUITextEntry*)Add(new CGUITextEntry(ID_CCS_SHIRT_COLOR, toneTextColorRange[0], toneTextColorRange[1], toneTextColorRange[1], 490, 185, 0, false, 9));

		if (g_CreateCharacterManager.Race == RT_GARGOYLE)
			entry->m_Entry.SetText("Robe Color");
		else
			entry->m_Entry.SetText("Shirt Color");

		entry->CheckOnSerial = true;
		entry->ReadOnly = true;

		color = g_CreateCharacterManager.ShirtColor - 1;

		colorIndex = (color + ((color + (color << 2)) << 1)) << 3;
		colorIndex += (colorIndex / colorOffsetDivider) << 2;
		color = *(pushort)(huesData + colorIndex);

		clr = g_ColorManager.Color16To32(color);

		Add(new CGUIColoredPolygone(ID_CCS_SHIRT_COLOR, color, 490, 199, 120, 25, clr));



		if (g_CreateCharacterManager.Race != RT_GARGOYLE)
		{
			entry = (CGUITextEntry*)Add(new CGUITextEntry(ID_CCS_SKIRT_OR_PANTS_COLOR, toneTextColorRange[0], toneTextColorRange[1], toneTextColorRange[1], 490, 230, 0, false, 9));
			entry->CheckOnSerial = true;
			entry->ReadOnly = true;

			if (g_CreateCharacterManager.Female)
				entry->m_Entry.SetText("Skirt Color");
			else
				entry->m_Entry.SetText("Pants Color");

			color = g_CreateCharacterManager.PantsColor - 1;

			colorIndex = (color + ((color + (color << 2)) << 1)) << 3;
			colorIndex += (colorIndex / colorOffsetDivider) << 2;
			color = *(pushort)(huesData + colorIndex);

			clr = g_ColorManager.Color16To32(color);

			Add(new CGUIColoredPolygone(ID_CCS_SKIRT_OR_PANTS_COLOR, color, 490, 244, 120, 25, clr));
		}



		entry = (CGUITextEntry*)Add(new CGUITextEntry(ID_CCS_HAIR_COLOR, toneTextColorRange[0], toneTextColorRange[1], toneTextColorRange[1], 490, 275, 0, false, 9));
		entry->m_Entry.SetText("Hair Color");
		entry->CheckOnSerial = true;
		entry->ReadOnly = true;

		color = g_CreateCharacterManager.HairColor - 1;

		colorIndex = (color + ((color + (color << 2)) << 1)) << 3;
		colorIndex += (colorIndex / colorOffsetDivider) << 2;
		color = *(pushort)(huesData + colorIndex);

		clr = g_ColorManager.Color16To32(color);

		Add(new CGUIColoredPolygone(ID_CCS_HAIR_COLOR, color, 490, 289, 120, 25, clr));



		if (!g_CreateCharacterManager.Female && g_CreateCharacterManager.Race != RT_ELF)
		{
			entry = (CGUITextEntry*)Add(new CGUITextEntry(ID_CCS_FACIAL_HAIR_COLOR, toneTextColorRange[0], toneTextColorRange[1], toneTextColorRange[1], 490, 320, 0, false, 9));
			entry->m_Entry.SetText("Facial Hair Color");
			entry->CheckOnSerial = true;
			entry->ReadOnly = true;

			color = g_CreateCharacterManager.BeardColor - 1;

			colorIndex = (color + ((color + (color << 2)) << 1)) << 3;
			colorIndex += (colorIndex / colorOffsetDivider) << 2;
			color = *(pushort)(huesData + colorIndex);

			clr = g_ColorManager.Color16To32(color);

			Add(new CGUIColoredPolygone(ID_CCS_FACIAL_HAIR_COLOR, color, 490, 333, 120, 25, clr));
		}
	}
	else
	{
		if (g_CreateCharacterScreen.ColorSelection == CCSID_SKIN_TONE)
		{
			pushort colorPtr = g_CreateCharacterManager.GetSkinTonePtr();
			int maxX = 8;
			int polygoneWidth = 15;
			int maxY = 8;
			int polygoneHeight = 35;

			if (g_CreateCharacterManager.Race == RT_ELF)
			{
				maxX = 4;
				polygoneWidth = 30;
			}
			else if (g_CreateCharacterManager.Race == RT_GARGOYLE)
			{
				maxX = 4;
				polygoneWidth = 30;
				maxY = 7;
				polygoneHeight = 40;
			}

			IFOR(y, 0, maxY)
			{
				IFOR(x, 0, maxX)
				{
					ushort startColor = *colorPtr++;

					int colorIndex = (startColor + ((startColor + (startColor << 2)) << 1)) << 3;
					colorIndex += (colorIndex / colorOffsetDivider) << 2;
					ushort color = *(pushort)(huesData + colorIndex);

					uint clr = g_ColorManager.Color16To32(color);

					CGUIColoredPolygone *polygone = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_CCS_COLOR_RANGE + ((int)x * maxY + (int)y), startColor, 491 + ((int)x * polygoneWidth), 138 + ((int)y * polygoneHeight), polygoneWidth, polygoneHeight, clr));
					polygone->DrawDot = true;
				}
			}
		}
		else if (g_CreateCharacterScreen.ColorSelection == CCSID_SHIRT_COLOR || g_CreateCharacterScreen.ColorSelection == CCSID_SKIRT_OR_PANTS_COLOR)
		{
			IFOR(y, 1, 48)
			{
				ushort startColor = (1 << (y % 4)) * 100 + 1 + ((ushort)y / 4);

				IFOR(x, 0, 20)
				{
					int colorIndex = (startColor + ((startColor + (startColor << 2)) << 1)) << 3;
					colorIndex += (colorIndex / colorOffsetDivider) << 2;
					ushort color = *(pushort)(huesData + colorIndex);

					uint clr = g_ColorManager.Color16To32(color);

					CGUIColoredPolygone *polygone = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_CCS_COLOR_RANGE + ((int)y * 20 + (int)x), startColor, 492 + ((int)x * 6), 148 + ((int)y * 5), 6, 5, clr));
					polygone->DrawDot = true;

					startColor += 5;
				}
			}
		}
		else if (g_CreateCharacterScreen.ColorSelection == CCSID_HAIR_COLOR || g_CreateCharacterScreen.ColorSelection == CCSID_FACIAL_HAIR_COLOR)
		{
			pushort colorPtr = g_CreateCharacterManager.GetHairColorPtr();
			int maxY = 8;
			int polygoneHeight = 35;

			if (g_CreateCharacterManager.Race == RT_ELF)
			{
				maxY = 9;
				polygoneHeight = 31;
			}
			else if (g_CreateCharacterManager.Race == RT_GARGOYLE)
			{
				maxY = 3;
				polygoneHeight = 93;
			}

			IFOR(y, 0, maxY)
			{
				IFOR(x, 0, 6)
				{
					ushort startColor = *colorPtr++;

					int colorIndex = (startColor + ((startColor + (startColor << 2)) << 1)) << 3;
					colorIndex += (colorIndex / colorOffsetDivider) << 2;
					ushort color = *(pushort)(huesData + colorIndex);

					uint clr = g_ColorManager.Color16To32(color);

					CGUIColoredPolygone *polygone = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_CCS_COLOR_RANGE + ((int)x * maxY + (int)y), startColor, 490 + ((int)x * 20), 140 + ((int)y * polygoneHeight), 20, polygoneHeight, clr));
					polygone->DrawDot = true;
				}
			}
		}
	}

	Add(new CGUIShader(&g_ColorizerShader, true));

	ushort gumpID = g_CreateCharacterManager.GetBodyGump();

	if (gumpID)
	{
		CGUIGumppic *gumppic = (CGUIGumppic*)Add(new CGUIGumppic(gumpID, 238, 98));
		gumppic->Color = g_CreateCharacterManager.SkinTone;
	}

	gumpID = g_CreateCharacterManager.GetBootsGump();

	if (gumpID)
		Add(new CGUIGumppic(gumpID, 238, 98));

	gumpID = g_CreateCharacterManager.GetPantsGump();

	if (gumpID)
	{
		CGUIGumppic *gumppic = (CGUIGumppic*)Add(new CGUIGumppic(gumpID, 238, 98));
		gumppic->Color = g_CreateCharacterManager.PantsColor;
	}

	gumpID = g_CreateCharacterManager.GetShirtGump();

	if (gumpID)
	{
		CGUIGumppic *gumppic = (CGUIGumppic*)Add(new CGUIGumppic(gumpID, 238, 98));
		gumppic->Color = g_CreateCharacterManager.ShirtColor;
	}

	if (g_CreateCharacterManager.HairStyle)
	{
		CGUIGumppic *gumppic = (CGUIGumppic*)Add(new CGUIGumppic(g_CreateCharacterManager.GetHair(g_CreateCharacterManager.HairStyle).GumpID, 238, 98));
		gumppic->Color = g_CreateCharacterManager.HairColor;
	}

	if (!g_CreateCharacterManager.Female && g_CreateCharacterManager.BeardStyle && g_CreateCharacterManager.Race != RT_ELF)
	{
		CGUIGumppic *gumppic = (CGUIGumppic*)Add(new CGUIGumppic(g_CreateCharacterManager.GetBeard(g_CreateCharacterManager.BeardStyle).GumpID, 238, 98));
		gumppic->Color = g_CreateCharacterManager.BeardColor;
	}

	Add(new CGUIShader(NULL, false));

	if (g_PacketManager.ClientVersion < CV_4011D)
	{
		if (g_CreateCharacterManager.Female)
			Add(new CGUIButton(ID_CCS_MALE_BUTTON, 0x070D, 0x070E, 0x070F, 310, 408));
		else
			Add(new CGUIButton(ID_CCS_FEMALE_BUTTON, 0x0710, 0x0711, 0x0712, 310, 408));
	}
	else
	{
		Add(new CGUIGroup(1));

		CGUIRadio *radio = (CGUIRadio*)Add(new CGUIRadio(ID_CCS_MALE_BUTTON, 0x0768, 0x0767, 0x0767, 425, 435));
		radio->Checked = !g_CreateCharacterManager.Female;
		radio = (CGUIRadio*)Add(new CGUIRadio(ID_CCS_FEMALE_BUTTON, 0x0768, 0x0767, 0x0767, 425, 455));
		radio->Checked = g_CreateCharacterManager.Female;

		Add(new CGUIGroup(0));

		Add(new CGUIButton(ID_CCS_MALE_BUTTON, 0x0710, 0x0711, 0x0712, 445, 435));
		Add(new CGUIButton(ID_CCS_FEMALE_BUTTON, 0x070D, 0x070E, 0x070F, 445, 455));

		Add(new CGUIGroup(2));

		radio = (CGUIRadio*)Add(new CGUIRadio(ID_CCS_HUMAN_RACE_BUTTON, 0x0768, 0x0767, 0x0767, 180, 435));
		radio->Checked = (g_CreateCharacterManager.Race == RT_HUMAN);
		radio = (CGUIRadio*)Add(new CGUIRadio(ID_CCS_ELF_RACE_BUTTON, 0x0768, 0x0767, 0x0767, 180, 455));
		radio->Checked = (g_CreateCharacterManager.Race == RT_ELF);

		Add(new CGUIButton(ID_CCS_HUMAN_RACE_BUTTON, 0x0702, 0x0703, 0x0704, 200, 435));
		Add(new CGUIButton(ID_CCS_ELF_RACE_BUTTON, 0x0705, 0x0706, 0x0707, 200, 455));

		if (g_PacketManager.ClientVersion >= CV_60144)
		{
			radio = (CGUIRadio*)Add(new CGUIRadio(ID_CCS_GARGOYLE_RACE_BUTTON, 0x0768, 0x0767, 0x0767, 60, 435));
			radio->Checked = (g_CreateCharacterManager.Race == RT_GARGOYLE);

			Add(new CGUIButton(ID_CCS_GARGOYLE_RACE_BUTTON, 0x07D3, 0x07D4, 0x07D5, 80, 435));
		}

		Add(new CGUIGroup(0));
	}
}
//----------------------------------------------------------------------------------
void CGumpScreenCreateCharacter::InitToolTip()
{
	WISPFUN_DEBUG("c114_f2");
	if (!g_ConfigManager.UseToolTips || g_SelectedObject.Object == NULL)
		return;

	uint id = g_SelectedObject.Serial;
	
	switch (id)
	{
		case ID_CCS_QUIT:
		{
			g_ToolTip.Set(L"Quit Ultima Online", 80);
			break;
		}
		case ID_CCS_ARROW_NEXT:
		{
			g_ToolTip.Set(L"Accept this character");
			break;
		}
		case ID_CCS_ARROW_PREV:
		{
			g_ToolTip.Set(L"Back to character generation screen", 150);
			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------------
void CGumpScreenCreateCharacter::GUMP_BUTTON_EVENT_C
{
	WISPFUN_DEBUG("c114_f3");
	if (serial == ID_CCS_QUIT) //x button
		g_CreateCharacterScreen.CreateSmoothAction(CCreateCharacterScreen::ID_SMOOTH_CCS_QUIT);
	else if (serial == ID_CCS_ARROW_PREV) //< button
		g_CreateCharacterScreen.CreateSmoothAction(CCreateCharacterScreen::ID_SMOOTH_CCS_GO_SCREEN_CHARACTER);
	else if (serial == ID_CCS_ARROW_NEXT) //> button
	{
		if (g_EntryPointer->Length() < 2)
			g_CreateCharacterScreen.CreateSmoothAction(CCreateCharacterScreen::ID_SMOOTH_CCS_GO_SCREEN_CONNECT);
		else
			g_CreateCharacterScreen.CreateSmoothAction(CCreateCharacterScreen::ID_SMOOTH_CCS_GO_SCREEN_SELECT_TOWN);
	}
	else if (serial == ID_CCS_FEMALE_BUTTON)
	{
		g_CreateCharacterManager.Female = true;
		m_WantUpdateContent = true;
		g_CreateCharacterScreen.ColorSelection = 0;
	}
	else if (serial == ID_CCS_MALE_BUTTON)
	{
		g_CreateCharacterManager.Female = false;
		m_WantUpdateContent = true;
		g_CreateCharacterScreen.ColorSelection = 0;
	}
	else if (serial == ID_CCS_HUMAN_RACE_BUTTON)
	{
		g_CreateCharacterManager.Race = RT_HUMAN;
		m_WantUpdateContent = true;
		g_CreateCharacterScreen.ColorSelection = 0;
	}
	else if (serial == ID_CCS_ELF_RACE_BUTTON)
	{
		g_CreateCharacterManager.Race = RT_ELF;
		m_WantUpdateContent = true;
		g_CreateCharacterScreen.ColorSelection = 0;
	}
	else if (serial == ID_CCS_GARGOYLE_RACE_BUTTON)
	{
		g_CreateCharacterManager.Race = RT_GARGOYLE;
		m_WantUpdateContent = true;
		g_CreateCharacterScreen.ColorSelection = 0;
	}
}
//----------------------------------------------------------------------------------
void CGumpScreenCreateCharacter::GUMP_RADIO_EVENT_C
{
	WISPFUN_DEBUG("c114_f4");
	if (state)
	{
		if (serial == ID_CCS_MALE_BUTTON)
			g_CreateCharacterManager.Female = false;
		else if (serial == ID_CCS_FEMALE_BUTTON)
			g_CreateCharacterManager.Female = true;
		else if (serial == ID_CCS_HUMAN_RACE_BUTTON)
			g_CreateCharacterManager.Race = RT_HUMAN;
		else if (serial == ID_CCS_ELF_RACE_BUTTON)
			g_CreateCharacterManager.Race = RT_ELF;
		else if (serial == ID_CCS_GARGOYLE_RACE_BUTTON)
			g_CreateCharacterManager.Race = RT_GARGOYLE;

		m_WantUpdateContent = true;
		g_CreateCharacterScreen.ColorSelection = 0;
	}
}
//----------------------------------------------------------------------------------
void CGumpScreenCreateCharacter::GUMP_TEXT_ENTRY_EVENT_C
{
	WISPFUN_DEBUG("c114_f5");
	if (g_CreateCharacterScreen.ColorSelection == 0)
	{
		if (serial == ID_CCS_SKIN_TONE)
			g_CreateCharacterScreen.ColorSelection = CCSID_SKIN_TONE;
		else if (serial == ID_CCS_SHIRT_COLOR)
			g_CreateCharacterScreen.ColorSelection = CCSID_SHIRT_COLOR;
		else if (serial == ID_CCS_SKIRT_OR_PANTS_COLOR)
			g_CreateCharacterScreen.ColorSelection = CCSID_SKIRT_OR_PANTS_COLOR;
		else if (serial == ID_CCS_HAIR_COLOR)
			g_CreateCharacterScreen.ColorSelection = CCSID_HAIR_COLOR;
		else if (!g_CreateCharacterManager.Female && serial == ID_CCS_FACIAL_HAIR_COLOR)
			g_CreateCharacterScreen.ColorSelection = CCSID_FACIAL_HAIR_COLOR;

		m_WantUpdateContent = true;
	}
	else
	{
		if (serial >= ID_CCS_COLOR_RANGE)
		{
			ushort color = g_SelectedObject.Object->Color + 1;

			if (g_CreateCharacterScreen.ColorSelection == CCSID_SKIN_TONE)
			{
				if (g_CreateCharacterManager.Race == RT_HUMAN)
				{
					if (color < 0x03EA)
						color = 0x03EA;
					else if (color > 0x0422)
						color = 0x0422;
				}

				g_CreateCharacterManager.SkinTone = color;
			}
			else if (g_CreateCharacterScreen.ColorSelection == CCSID_SHIRT_COLOR || g_CreateCharacterScreen.ColorSelection == CCSID_SKIRT_OR_PANTS_COLOR)
			{
				if (g_CreateCharacterScreen.ColorSelection == CCSID_SHIRT_COLOR)
					g_CreateCharacterManager.ShirtColor = color;
				else
					g_CreateCharacterManager.PantsColor = color;
			}
			else if (g_CreateCharacterScreen.ColorSelection == CCSID_HAIR_COLOR || g_CreateCharacterScreen.ColorSelection == CCSID_FACIAL_HAIR_COLOR)
			{
				if (g_CreateCharacterManager.Race == RT_HUMAN)
				{
					if (color < 0x044E)
						color = 0x044E;
					else if (color > 0x047C)
						color = 0x047C;
				}

				if (g_CreateCharacterScreen.ColorSelection == CCSID_HAIR_COLOR)
					g_CreateCharacterManager.HairColor = color;
				else
					g_CreateCharacterManager.BeardColor = color;
			}

			g_CreateCharacterScreen.ColorSelection = 0;
			m_WantUpdateContent = true;
		}
	}
}
//----------------------------------------------------------------------------------
void CGumpScreenCreateCharacter::GUMP_COMBOBOX_SELECTION_EVENT_C
{
	WISPFUN_DEBUG("c114_f6");
	if (serial >= ID_CCS_HAIR_STYLE)
	{
		if (serial >= ID_CCS_FACIAL_HAIR_STYLE)
		{
			uchar index = serial - ID_CCS_FACIAL_HAIR_STYLE;

			if (index < g_CreateCharacterManager.GetCurrentFacialHairCount())
				g_CreateCharacterManager.BeardStyle = (uchar)index;

			m_WantUpdateContent = true;
		}
		else
		{
			uchar index = serial - ID_CCS_HAIR_STYLE;

			if (index < g_CreateCharacterManager.GetCurrentHairCount())
				g_CreateCharacterManager.HairStyle = index;

			m_WantUpdateContent = true;
		}
	}
}
//----------------------------------------------------------------------------------
