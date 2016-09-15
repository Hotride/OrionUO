/***********************************************************************************
**
** GumpScreenCreateCharacter.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GumpScreenCreateCharacter.h"
#include "../Screen stages/CreateCharacterScreen.h"
#include "../OrionUO.h"
#include "../Managers/ConfigManager.h"
#include "../Managers/CreateCharacterManager.h"
#include "../Managers/ColorManager.h"
#include "../Managers/ConnectionManager.h"
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
	Clear();

	Add(new CGUIGumppicTiled(0x0588, 0, 0, 640, 480));
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



	CGUIComboBox *combo = (CGUIComboBox*)Add(new CGUIComboBox(ID_CCS_FACIAL_HAIR_STYLE, 0x0BB8, false, 0x0BB8, 97, 199, 120, 8, false));
	combo->SelectedIndex = g_CreateCharacterManager.BeardStyle;
	combo->Visible = !g_CreateCharacterManager.Sex;

	CGUIText *text = new CGUIText(0x0481, 3, -12);
	text->CreateTextureA(9, "Facial Hair Style");
	combo->SetText(text);

	IFOR(i, 0, 8)
		combo->Add(new CGUIComboboxText(0x0381, 9, g_CreateCharacterManager.GetBeard(i).Name));

	combo->RecalculateWidth();



	int hairCount = 10 + (int)g_CreateCharacterManager.Sex;

	combo = (CGUIComboBox*)Add(new CGUIComboBox(ID_CCS_HAIR_STYLE, 0x0BB8, false, 0x0BB8, 97, 154, 120, hairCount, false));
	combo->SelectedIndex = g_CreateCharacterManager.HairStyle;

	text = new CGUIText(0x0481, 3, -12);
	text->CreateTextureA(9, "Hair Style");
	combo->SetText(text);

	IFOR(i, 0, hairCount)
		combo->Add(new CGUIComboboxText(0x0381, 9, g_CreateCharacterManager.GetHair(i).Name));

	combo->RecalculateWidth();



	puchar huesData = (puchar)g_ColorManager.GetHuesRangePointer() + 32 + 4;
	int colorOffsetDivider = sizeof(HUES_GROUP) - 4;

	if (g_CreateCharacterScreen.ColorSelection == 0)
	{
		entry = (CGUITextEntry*)Add(new CGUITextEntry(ID_CCS_SKIN_TONE, toneTextColorRange[0], toneTextColorRange[1], toneTextColorRange[1], 490, 140, 0, false, 9));
		entry->m_Entry.SetText("Skin Tone");
		entry->CheckOnSerial = true;
		entry->ReadOnly = true;

		ushort color = g_CreateCharacterManager.SkinTone;

		if (color < 0x03EA)
			color = 0x03EA;
		else if (color > 0x0422)
			color = 0x0422;

		int colorIndex = (color + ((color + (color << 2)) << 1)) << 3;
		colorIndex += (colorIndex / colorOffsetDivider) << 2;
		color = *(pushort)(huesData + colorIndex);

		uint clr = g_ColorManager.Color16To32(color);

		Add(new CGUIColoredPolygone(ID_CCS_SKIN_TONE, color, 490, 154, 120, 25, clr));



		entry = (CGUITextEntry*)Add(new CGUITextEntry(ID_CCS_SHIRT_COLOR, toneTextColorRange[0], toneTextColorRange[1], toneTextColorRange[1], 490, 185, 0, false, 9));
		entry->m_Entry.SetText("Shirt Color");
		entry->CheckOnSerial = true;
		entry->ReadOnly = true;

		color = g_CreateCharacterManager.GetShirtColor();
		//if (color < 0x044E) color = 0x044E;
		//else if (color > 0x04AD) color = 0x04AD;

		colorIndex = (color + ((color + (color << 2)) << 1)) << 3;
		colorIndex += (colorIndex / colorOffsetDivider) << 2;
		color = *(pushort)(huesData + colorIndex);

		clr = g_ColorManager.Color16To32(color);

		Add(new CGUIColoredPolygone(ID_CCS_SHIRT_COLOR, color, 490, 199, 120, 25, clr));



		entry = (CGUITextEntry*)Add(new CGUITextEntry(ID_CCS_SKIRT_OR_PANTS_COLOR, toneTextColorRange[0], toneTextColorRange[1], toneTextColorRange[1], 490, 230, 0, false, 9));
		entry->CheckOnSerial = true;
		entry->ReadOnly = true;

		if (g_CreateCharacterManager.Sex)
			entry->m_Entry.SetText("Skirt Color");
		else
			entry->m_Entry.SetText("Pants Color");

		color = g_CreateCharacterManager.PantsColor;
		//if (color < 0x044E) color = 0x044E;
		//else if (color > 0x04AD) color = 0x04AD;

		colorIndex = (color + ((color + (color << 2)) << 1)) << 3;
		colorIndex += (colorIndex / colorOffsetDivider) << 2;
		color = *(pushort)(huesData + colorIndex);

		clr = g_ColorManager.Color16To32(color);

		Add(new CGUIColoredPolygone(ID_CCS_SKIRT_OR_PANTS_COLOR, color, 490, 244, 120, 25, clr));



		entry = (CGUITextEntry*)Add(new CGUITextEntry(ID_CCS_HAIR_COLOR, toneTextColorRange[0], toneTextColorRange[1], toneTextColorRange[1], 490, 275, 0, false, 9));
		entry->m_Entry.SetText("Hair Color");
		entry->CheckOnSerial = true;
		entry->ReadOnly = true;

		color = g_CreateCharacterManager.HairColor;

		if (color < 0x044E)
			color = 0x044E;
		else if (color > 0x04AD)
			color = 0x04AD;

		colorIndex = (color + ((color + (color << 2)) << 1)) << 3;
		colorIndex += (colorIndex / colorOffsetDivider) << 2;
		color = *(pushort)(huesData + colorIndex);

		clr = g_ColorManager.Color16To32(color);

		Add(new CGUIColoredPolygone(ID_CCS_HAIR_COLOR, color, 490, 289, 120, 25, clr));



		if (!g_CreateCharacterManager.Sex)
		{
			entry = (CGUITextEntry*)Add(new CGUITextEntry(ID_CCS_FACIAL_HAIR_COLOR, toneTextColorRange[0], toneTextColorRange[1], toneTextColorRange[1], 490, 320, 0, false, 9));
			entry->m_Entry.SetText("Facial Hair Color");
			entry->CheckOnSerial = true;
			entry->ReadOnly = true;

			color = g_CreateCharacterManager.BeardColor;

			if (color < 0x044E)
				color = 0x044E;
			else if (color > 0x04AD)
				color = 0x04AD;

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
			IFOR(y, 0, 8)
			{
				ushort startColor = 0x03E9 + y;

				IFOR(x, 0, 8)
				{
					int colorIndex = (startColor + ((startColor + (startColor << 2)) << 1)) << 3;
					colorIndex += (colorIndex / colorOffsetDivider) << 2;
					ushort color = *(pushort)(huesData + colorIndex);

					uint clr = g_ColorManager.Color16To32(color);

					CGUIColoredPolygone *polygone = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_CCS_COLOR_RANGE + (x * 8 + y), startColor, 491 + (x * 15), 138 + (y * 35), 15, 35, clr));
					polygone->DrawDot = true;

					if (x == 6)
						startColor = 0x03E9 + ((x + 1) * 8);
					else
						startColor += 8;
				}
			}
		}
		else if (g_CreateCharacterScreen.ColorSelection == CCSID_SHIRT_COLOR || g_CreateCharacterScreen.ColorSelection == CCSID_SKIRT_OR_PANTS_COLOR)
		{
			IFOR(y, 1, 48)
			{
				ushort startColor = (1 << (y % 4)) * 100 + 1 + (y / 4);

				IFOR(x, 0, 20)
				{
					int colorIndex = (startColor + ((startColor + (startColor << 2)) << 1)) << 3;
					colorIndex += (colorIndex / colorOffsetDivider) << 2;
					ushort color = *(pushort)(huesData + colorIndex);

					uint clr = g_ColorManager.Color16To32(color);

					CGUIColoredPolygone *polygone = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_CCS_COLOR_RANGE + (y * 20 + x), startColor, 492 + (x * 6), 148 + (y * 5), 6, 5, clr));
					polygone->DrawDot = true;

					startColor += 5;
				}
			}
		}
		else if (g_CreateCharacterScreen.ColorSelection == CCSID_HAIR_COLOR || g_CreateCharacterScreen.ColorSelection == CCSID_FACIAL_HAIR_COLOR)
		{
			IFOR(y, 0, 8)
			{
				ushort startColor = 0x44D + y;

				IFOR(x, 0, 6)
				{
					int colorIndex = (startColor + ((startColor + (startColor << 2)) << 1)) << 3;
					colorIndex += (colorIndex / colorOffsetDivider) << 2;
					ushort color = *(pushort)(huesData + colorIndex);

					uint clr = g_ColorManager.Color16To32(color);

					CGUIColoredPolygone *polygone = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_CCS_COLOR_RANGE + (x * 8 + y), startColor, 490 + (x * 20), 140 + (y * 35), 20, 35, clr));
					polygone->DrawDot = true;

					if (x == 6)
						startColor = 0x03E9 + ((x + 1) * 8);
					else
						startColor += 8;
				}
			}
		}
	}

	Add(new CGUIShader(g_ColorizerShader, true));

	if (g_CreateCharacterManager.Sex)
	{
		CGUIGumppic *gumppic = (CGUIGumppic*)Add(new CGUIGumppic(0x0760, 238, 98));
		gumppic->Color = g_CreateCharacterManager.SkinTone;

		gumppic = (CGUIGumppic*)Add(new CGUIGumppic(0x0714, 238, 98));
		gumppic->Color = g_CreateCharacterManager.ShirtColor;

		gumppic = (CGUIGumppic*)Add(new CGUIGumppic(0x0764, 238, 98));
		gumppic->Color = g_CreateCharacterManager.PantsColor;

		if (g_CreateCharacterManager.HairStyle)
		{
			gumppic = (CGUIGumppic*)Add(new CGUIGumppic(g_CreateCharacterManager.GetHair(g_CreateCharacterManager.HairStyle).GumpID, 238, 98));
			gumppic->Color = g_CreateCharacterManager.HairColor;
		}
	}
	else
	{
		CGUIGumppic *gumppic = (CGUIGumppic*)Add(new CGUIGumppic(0x0761, 238, 98));
		gumppic->Color = g_CreateCharacterManager.SkinTone;

		gumppic = (CGUIGumppic*)Add(new CGUIGumppic(0x0739, 238, 98));
		gumppic->Color = g_CreateCharacterManager.ShirtColor;

		gumppic = (CGUIGumppic*)Add(new CGUIGumppic(0x0738, 238, 98));
		gumppic->Color = g_CreateCharacterManager.PantsColor;

		Add(new CGUIGumppic(0x0762, 238, 98));

		if (g_CreateCharacterManager.HairStyle)
		{
			gumppic = (CGUIGumppic*)Add(new CGUIGumppic(g_CreateCharacterManager.GetHair(g_CreateCharacterManager.HairStyle).GumpID, 238, 98));
			gumppic->Color = g_CreateCharacterManager.HairColor;
		}

		if (g_CreateCharacterManager.BeardStyle)
		{
			gumppic = (CGUIGumppic*)Add(new CGUIGumppic(g_CreateCharacterManager.GetBeard(g_CreateCharacterManager.BeardStyle).GumpID, 238, 98));
			gumppic->Color = g_CreateCharacterManager.BeardColor;
		}
	}

	Add(new CGUIShader(NULL, false));

	if (g_ConnectionManager.ClientVersion < CV_4011D)
	{
		if (g_CreateCharacterManager.Sex)
			Add(new CGUIButton(ID_CCS_FEMALE_BUTTON, 0x070D, 0x070E, 0x070F, 310, 408));
		else
			Add(new CGUIButton(ID_CCS_MALE_BUTTON, 0x0710, 0x0711, 0x0712, 310, 408));
	}
	else
	{
	}
}
//----------------------------------------------------------------------------------
void CGumpScreenCreateCharacter::InitToolTip()
{
	if (!g_ConfigManager.UseToolTips || g_SelectedObject.Object() == NULL)
		return;

	uint id = g_SelectedObject.Serial;
	
	switch (id)
	{
		case ID_CCS_QUIT:
		{
			g_ToolTip.Set(L"Quit Ultima Online", g_SelectedObject.Object(), 80);
			break;
		}
		case ID_CCS_ARROW_NEXT:
		{
			g_ToolTip.Set(L"Accept this character", g_SelectedObject.Object());
			break;
		}
		case ID_CCS_ARROW_PREV:
		{
			g_ToolTip.Set(L"Back to character generation screen", g_SelectedObject.Object(), 150);
			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------------
void CGumpScreenCreateCharacter::OnButton(const uint &serial)
{
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
		g_CreateCharacterManager.Sex = false;
		m_WantUpdateContent = true;
	}
	else if (serial == ID_CCS_MALE_BUTTON)
	{
		g_CreateCharacterManager.Sex = true;
		m_WantUpdateContent = true;
	}
}
//----------------------------------------------------------------------------------
void CGumpScreenCreateCharacter::OnTextEntry(const uint &serial)
{
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
		else if (!g_CreateCharacterManager.Sex && serial == ID_CCS_FACIAL_HAIR_COLOR)
			g_CreateCharacterScreen.ColorSelection = CCSID_FACIAL_HAIR_COLOR;

		m_WantUpdateContent = true;
	}
	else
	{
		if (serial >= ID_CCS_COLOR_RANGE)
		{
			ushort color = (ushort)(serial - ID_CCS_COLOR_RANGE);

			if (g_CreateCharacterScreen.ColorSelection == CCSID_SKIN_TONE)
			{
				color = 0x03EA + color;

				if (color > 0x0422)
					color = 0x0422;

				g_CreateCharacterManager.SkinTone = color;
			}
			else if (g_CreateCharacterScreen.ColorSelection == CCSID_SHIRT_COLOR || g_CreateCharacterScreen.ColorSelection == CCSID_SKIRT_OR_PANTS_COLOR)
			{
				color = g_SelectedObject.Object()->Color;
				if (g_CreateCharacterScreen.ColorSelection == CCSID_SHIRT_COLOR)
					g_CreateCharacterManager.ShirtColor = color;
				else
					g_CreateCharacterManager.PantsColor = color;
			}
			else if (g_CreateCharacterScreen.ColorSelection == CCSID_HAIR_COLOR || g_CreateCharacterScreen.ColorSelection == CCSID_FACIAL_HAIR_COLOR)
			{
				if (g_CreateCharacterScreen.ColorSelection == CCSID_HAIR_COLOR)
					g_CreateCharacterManager.HairColor = 0x044D + color;
				else
					g_CreateCharacterManager.BeardColor = 0x044D + color;
			}

			g_CreateCharacterScreen.ColorSelection = 0;
			m_WantUpdateContent = true;
		}
	}
}
//----------------------------------------------------------------------------------
void CGumpScreenCreateCharacter::GUMP_COMBOBOX_SELECTION_EVENT_C
{
	if (serial >= ID_CCS_HAIR_STYLE)
	{
		if (serial >= ID_CCS_FACIAL_HAIR_STYLE)
		{
			uchar index = serial - ID_CCS_FACIAL_HAIR_STYLE;

			if (index < 8)
				g_CreateCharacterManager.BeardStyle = (uchar)index;

			m_WantUpdateContent = true;
		}
		else
		{
			int count = 10 + (int)g_CreateCharacterManager.Sex;

			uchar index = serial - ID_CCS_HAIR_STYLE;

			if (index < count)
				g_CreateCharacterManager.HairStyle = index;

			m_WantUpdateContent = true;
		}
	}
}
//----------------------------------------------------------------------------------
