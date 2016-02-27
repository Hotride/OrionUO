/****************************************************************************
**
** OptionsGump.cpp
**
** Copyright (C) September 2015 Hotride
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
*****************************************************************************
*/
//----------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------
const WORD g_OptionsTextColor = 0;
//----------------------------------------------------------------------------
TTextTexture TGumpOptions::m_TexturePage1[8];	//Sound and Music
TTextTexture TGumpOptions::m_TexturePage2[8];	//Pop-up Help
TTextTexture TGumpOptions::m_TexturePage3[6];	//Language
TTextTexture TGumpOptions::m_TexturePage4[19];	//Chat
TTextTexture TGumpOptions::m_TexturePage5[7];	//Macro Options
TTextTexture TGumpOptions::m_TexturePage6[13];	//Interface
TTextTexture TGumpOptions::m_TexturePage7[18];	//Display
TTextTexture TGumpOptions::m_TexturePage8[9];	//Reputation System
TTextTexture TGumpOptions::m_TexturePage9[9];	//Miscellaneous
TTextTexture TGumpOptions::m_TexturePage10[2];	//Filter Options
//----------------------------------------------------------------------------
TGumpOptions::TGumpOptions(DWORD serial, short x, short y)
: TGump(GT_OPTIONS, serial, x, y), m_Page(0), m_LastChangeMacroTime(0),
m_MacroSelection(0), m_MacroElement(0), m_MacroListOffset(0), m_MacroListCount(0),
m_MacroListOffsetYStart(0), m_MacroListOffsetYEnd(0), m_MacroListNameOffset(0),
m_MacroPointer(NULL), m_MacroObjectPointer(NULL)
{
	TextEntryGameSizeX = new TEntryText(2000, 0, 0, true);
	TextEntryGameSizeY = new TEntryText(2000, 0, 0, true);
	TextEntryMacro = new TEntryText();
}
//----------------------------------------------------------------------------
TGumpOptions::~TGumpOptions()
{
	if (TextEntryGameSizeX != NULL)
	{
		delete TextEntryGameSizeX;
		TextEntryGameSizeX = NULL;
	}

	if (TextEntryGameSizeY != NULL)
	{
		delete TextEntryGameSizeY;
		TextEntryGameSizeY = NULL;
	}

	if (TextEntryMacro != NULL)
	{
		delete TextEntryMacro;
		TextEntryMacro = NULL;
	}
}
//----------------------------------------------------------------------------
void TGumpOptions::Init()
{
	OptionsMacroManager->LoadFromMacro();

	m_MacroPointer = (TMacro*)OptionsMacroManager->m_Items;
	m_MacroObjectPointer = (TMacroObject*)m_MacroPointer->m_Items;

	TextEntryMacro->SetText(m_HotkeyText[m_MacroPointer->Key & 0xFF]);
}
//----------------------------------------------------------------------------
bool TGumpOptions::EntryPointerHere()
{
	return (EntryPointer == TextEntryGameSizeX || EntryPointer == TextEntryGameSizeY || EntryPointer == TextEntryMacro || OptionsMacroManager->EntryPointerHere());
}
//----------------------------------------------------------------------------
void TGumpOptions::PrepareTextures()
{
	UO->ExecuteResizepic(0x00C8);
	UO->ExecuteResizepic(0x0A28);
	UO->ExecuteResizepic(0x0BB8);
	UO->ExecuteButton(0x0867);
	UO->ExecuteButton(0x099C);
	UO->ExecuteButton(0x099F);
	UO->ExecuteButton(0x09A2);
	UO->ExecuteButton(0x09A5);
	UO->ExecuteGumpPart(0x09B5, 5);
	UO->ExecuteGumpPart(0x0983, 9);
	UO->ExecuteGumpPart(0x00D0, 42);
}
//----------------------------------------------------------------------------
void TGumpOptions::InitTextTextures()
{
	//Sound and Music
	wstring str = L"Sound and Music";
	FontManager->GenerateW(0, m_TexturePage1[0], str.c_str(), g_OptionsTextColor, 30, 460, TS_CENTER);

	str = L"These settings affect the sound and music you will hear while playing Ultima Online.";
	FontManager->GenerateW(0, m_TexturePage1[1], str.c_str(), g_OptionsTextColor, 30, 500);

	str = L"Sound on/off";
	FontManager->GenerateW(0, m_TexturePage1[2], str.c_str(), g_OptionsTextColor);

	str = L"Sound volume";
	FontManager->GenerateW(0, m_TexturePage1[3], str.c_str(), g_OptionsTextColor);

	str = L"Music on/off";
	FontManager->GenerateW(0, m_TexturePage1[4], str.c_str(), g_OptionsTextColor);

	str = L"Music volume";
	FontManager->GenerateW(0, m_TexturePage1[5], str.c_str(), g_OptionsTextColor);

	str = L"Play footstep sounds";
	FontManager->GenerateW(0, m_TexturePage1[6], str.c_str(), g_OptionsTextColor);

	str = L"Play combat music";
	FontManager->GenerateW(0, m_TexturePage1[7], str.c_str(), g_OptionsTextColor);



	//Pop-up Help
	str = L"Pop-up Help";
	FontManager->GenerateW(0, m_TexturePage2[0], str.c_str(), g_OptionsTextColor, 30, 460, TS_CENTER);

	str = L"These settings configure the behavior of the pop-up help.";
	FontManager->GenerateW(0, m_TexturePage2[1], str.c_str(), g_OptionsTextColor);

	str = L"Use Pop-up Help";
	FontManager->GenerateW(0, m_TexturePage2[2], str.c_str(), g_OptionsTextColor);

	str = L"Delay befor Help pops up";
	FontManager->GenerateW(0, m_TexturePage2[3], str.c_str(), g_OptionsTextColor);

	str = L"Text Color";
	FontManager->GenerateW(0, m_TexturePage2[4], str.c_str(), g_OptionsTextColor);

	str = L"Help text font";
	FontManager->GenerateW(0, m_TexturePage2[5], str.c_str(), g_OptionsTextColor);

	str = L"Make window Sticky";
	FontManager->GenerateW(0, m_TexturePage2[6], str.c_str(), g_OptionsTextColor);

	str = L"Make text Persistant";
	FontManager->GenerateW(0, m_TexturePage2[7], str.c_str(), g_OptionsTextColor);



	//Language
	str = L"Language";
	FontManager->GenerateW(0, m_TexturePage3[0], str.c_str(), g_OptionsTextColor, 30, 460, TS_CENTER);

	str = L"The language you use when playing UO is obtained from your Operating System settings.";
	FontManager->GenerateW(0, m_TexturePage3[1], str.c_str(), g_OptionsTextColor, 30, 480);

	str = L"Use Tool-tips";
	FontManager->GenerateW(0, m_TexturePage3[2], str.c_str(), g_OptionsTextColor);

	str = L"Delay befor Tool-tip appears";
	FontManager->GenerateW(0, m_TexturePage3[3], str.c_str(), g_OptionsTextColor);

	str = L"Color of Tool-tip text";
	FontManager->GenerateW(0, m_TexturePage3[4], str.c_str(), g_OptionsTextColor);

	str = L"Font for Tool-tips";
	FontManager->GenerateW(0, m_TexturePage3[5], str.c_str(), g_OptionsTextColor);



	//Chat
	str = L"Chat";
	FontManager->GenerateW(0, m_TexturePage4[0], str.c_str(), g_OptionsTextColor, 30, 460, TS_CENTER);

	str = L"These settings affect the interface display for the chat system.";
	FontManager->GenerateW(0, m_TexturePage4[1], str.c_str(), g_OptionsTextColor);

	str = L"Input text color";
	FontManager->GenerateW(0, m_TexturePage4[2], str.c_str(), g_OptionsTextColor);

	str = L"Menu option color";
	FontManager->GenerateW(0, m_TexturePage4[3], str.c_str(), g_OptionsTextColor);

	str = L"Player color in member list";
	FontManager->GenerateW(0, m_TexturePage4[4], str.c_str(), g_OptionsTextColor);

	str = L"Chat text color";
	FontManager->GenerateW(0, m_TexturePage4[5], str.c_str(), g_OptionsTextColor);

	str = L"Player name without speaking privileges";
	FontManager->GenerateW(0, m_TexturePage4[6], str.c_str(), g_OptionsTextColor, 30, 160);

	str = L"Muted text color";
	FontManager->GenerateW(0, m_TexturePage4[7], str.c_str(), g_OptionsTextColor);

	str = L"Channel moderator name";
	FontManager->GenerateW(0, m_TexturePage4[8], str.c_str(), g_OptionsTextColor);

	str = L"Channel moderator text";
	FontManager->GenerateW(0, m_TexturePage4[9], str.c_str(), g_OptionsTextColor);

	str = L"My name's color";
	FontManager->GenerateW(0, m_TexturePage4[10], str.c_str(), g_OptionsTextColor);

	str = L"My text color";
	FontManager->GenerateW(0, m_TexturePage4[11], str.c_str(), g_OptionsTextColor);

	str = L"System message color";
	FontManager->GenerateW(0, m_TexturePage4[12], str.c_str(), g_OptionsTextColor);

	str = L"Text Output Background Color";
	FontManager->GenerateW(0, m_TexturePage4[13], str.c_str(), g_OptionsTextColor);

	str = L"Text Input Background Color";
	FontManager->GenerateW(0, m_TexturePage4[14], str.c_str(), g_OptionsTextColor);

	str = L"User List Background Color";
	FontManager->GenerateW(0, m_TexturePage4[15], str.c_str(), g_OptionsTextColor);

	str = L"Conference List Background Color";
	FontManager->GenerateW(0, m_TexturePage4[16], str.c_str(), g_OptionsTextColor);

	str = L"Command List Background Color";
	FontManager->GenerateW(0, m_TexturePage4[17], str.c_str(), g_OptionsTextColor);

	str = L"Chat font";
	FontManager->GenerateW(0, m_TexturePage4[18], str.c_str(), g_OptionsTextColor);



	//Macro Options
	str = L"Macro Options";
	FontManager->GenerateW(0, m_TexturePage5[0], str.c_str(), g_OptionsTextColor, 30, 460, TS_CENTER);
	
	str = L"Keystroke";
	FontManager->GenerateW(0, m_TexturePage5[1], str.c_str(), g_OptionsTextColor);
	
	str = L"Key";
	FontManager->GenerateW(0, m_TexturePage5[2], str.c_str(), g_OptionsTextColor);
	
	str = L"Shift";
	FontManager->GenerateW(0, m_TexturePage5[3], str.c_str(), g_OptionsTextColor);
	
	str = L"Alt";
	FontManager->GenerateW(0, m_TexturePage5[4], str.c_str(), g_OptionsTextColor);
	
	str = L"Ctrl";
	FontManager->GenerateW(0, m_TexturePage5[5], str.c_str(), g_OptionsTextColor);
	
	str = L"Actions";
	FontManager->GenerateW(0, m_TexturePage5[6], str.c_str(), g_OptionsTextColor);




	//Interface
	str = L"Interface";
	FontManager->GenerateW(0, m_TexturePage6[0], str.c_str(), g_OptionsTextColor, 30, 460, TS_CENTER);

	str = L"These options affect your interface.";
	FontManager->GenerateW(0, m_TexturePage6[1], str.c_str(), g_OptionsTextColor);

	str = L"Enable pathfinding with double-right-click";
	FontManager->GenerateW(0, m_TexturePage6[2], str.c_str(), g_OptionsTextColor);

	str = L"Hold down TAB key for combat, instead of tapping it to toggle combat mode";
	FontManager->GenerateW(0, m_TexturePage6[3], str.c_str(), g_OptionsTextColor, 30, 450);

	str = L"Offset interface windows rather than perfectly stacking them";
	FontManager->GenerateW(0, m_TexturePage6[4], str.c_str(), g_OptionsTextColor);

	str = L"Automatically arrange minimized windows";
	FontManager->GenerateW(0, m_TexturePage6[5], str.c_str(), g_OptionsTextColor);

	str = L"Your character will always run if this is checked";
	FontManager->GenerateW(0, m_TexturePage6[6], str.c_str(), g_OptionsTextColor);

	str = L"Disable the Menu Bar";
	FontManager->GenerateW(0, m_TexturePage6[7], str.c_str(), g_OptionsTextColor);

	str = L"Gray out of range objects";
	FontManager->GenerateW(0, m_TexturePage6[8], str.c_str(), g_OptionsTextColor);

	str = L"Disable New Target System";
	FontManager->GenerateW(0, m_TexturePage6[9], str.c_str(), g_OptionsTextColor);

	str = L"Object Handles";
	FontManager->GenerateW(0, m_TexturePage6[10], str.c_str(), g_OptionsTextColor);
	
	str = L"Reduce FPS when Window is Unactive";
	FontManager->GenerateW(0, m_TexturePage6[11], str.c_str(), g_OptionsTextColor);

	str = L"Display Item Properties Icon";
	FontManager->GenerateW(0, m_TexturePage6[12], str.c_str(), g_OptionsTextColor);



	//Display
	str = L"Display";
	FontManager->GenerateW(0, m_TexturePage7[0], str.c_str(), g_OptionsTextColor, 30, 460, TS_CENTER);

	str = L"These options affect your display, and adjusting some of them may improve your graphics performance.";
	FontManager->GenerateW(0, m_TexturePage7[1], str.c_str(), g_OptionsTextColor, 30, 500);

	str = L"Game Play Window Size";
	FontManager->GenerateW(0, m_TexturePage7[2], str.c_str(), g_OptionsTextColor);

	str = L"Adjust how long speech remains on screen";
	FontManager->GenerateW(0, m_TexturePage7[3], str.c_str(), g_OptionsTextColor);

	str = L"Scale speech duration based on length";
	FontManager->GenerateW(0, m_TexturePage7[4], str.c_str(), g_OptionsTextColor);

	str = L"Speech Color";
	FontManager->GenerateW(0, m_TexturePage7[5], str.c_str(), g_OptionsTextColor);

	str = L"Emote Color";
	FontManager->GenerateW(0, m_TexturePage7[6], str.c_str(), g_OptionsTextColor);

	str = L"Party Message Color";
	FontManager->GenerateW(0, m_TexturePage7[7], str.c_str(), g_OptionsTextColor);
	
	str = L"Guild Message Color";
	FontManager->GenerateW(0, m_TexturePage7[8], str.c_str(), g_OptionsTextColor);

	str = L"Alliance Message Color";
	FontManager->GenerateW(0, m_TexturePage7[9], str.c_str(), g_OptionsTextColor);
	
	str = L"Ignore Guild Messages";
	FontManager->GenerateW(0, m_TexturePage7[10], str.c_str(), g_OptionsTextColor);

	str = L"Ignore Alliance Messages";
	FontManager->GenerateW(0, m_TexturePage7[11], str.c_str(), g_OptionsTextColor);

	str = L"Dark Nights";
	FontManager->GenerateW(0, m_TexturePage7[12], str.c_str(), g_OptionsTextColor);

	str = L"Colored Lighting";
	FontManager->GenerateW(0, m_TexturePage7[13], str.c_str(), g_OptionsTextColor);

	str = L"Standard characters animation delay";
	FontManager->GenerateW(0, m_TexturePage7[14], str.c_str(), g_OptionsTextColor);

	str = L"Standard items animation delay";
	FontManager->GenerateW(0, m_TexturePage7[15], str.c_str(), g_OptionsTextColor);

	str = L"Lock game window resizing";
	FontManager->GenerateW(0, m_TexturePage7[16], str.c_str(), g_OptionsTextColor);

	str = L"Lock gumps moving";
	FontManager->GenerateW(0, m_TexturePage7[17], str.c_str(), g_OptionsTextColor);
	


	//Reputation System
	str = L"Reputation System";
	FontManager->GenerateW(0, m_TexturePage8[0], str.c_str(), g_OptionsTextColor, 30, 460, TS_CENTER);

	str = L"These settings affect the reputation system, which is Ultima Online's system for controlling antisocial behavior.";
	FontManager->GenerateW(0, m_TexturePage8[1], str.c_str(), g_OptionsTextColor, 30, 500);

	str = L"Innocent highlight color";
	FontManager->GenerateW(0, m_TexturePage8[2], str.c_str(), g_OptionsTextColor);

	str = L"Friendly guilds highlight color";
	FontManager->GenerateW(0, m_TexturePage8[3], str.c_str(), g_OptionsTextColor);

	str = L"Someone that can be attacked color";
	FontManager->GenerateW(0, m_TexturePage8[4], str.c_str(), g_OptionsTextColor);

	str = L"Criminal highlight color";
	FontManager->GenerateW(0, m_TexturePage8[5], str.c_str(), g_OptionsTextColor);

	str = L"Enemy guildmembers color";
	FontManager->GenerateW(0, m_TexturePage8[6], str.c_str(), g_OptionsTextColor);

	str = L"Murderer highlight color";
	FontManager->GenerateW(0, m_TexturePage8[7], str.c_str(), g_OptionsTextColor);

	str = L"Query before performing criminal actions";
	FontManager->GenerateW(0, m_TexturePage8[8], str.c_str(), g_OptionsTextColor);



	//Miscellaneous
	str = L"Miscellaneous";
	FontManager->GenerateW(0, m_TexturePage9[0], str.c_str(), g_OptionsTextColor, 30, 460, TS_CENTER);

	str = L"Miscellaneous options.";
	FontManager->GenerateW(0, m_TexturePage9[1], str.c_str(), g_OptionsTextColor);

	str = L"Show Names of Approaching Players";
	FontManager->GenerateW(0, m_TexturePage9[2], str.c_str(), g_OptionsTextColor);

	str = L"Use circle of transparency";
	FontManager->GenerateW(0, m_TexturePage9[3], str.c_str(), g_OptionsTextColor);

	str = L"Set radius of transparency circle";
	FontManager->GenerateW(0, m_TexturePage9[4], str.c_str(), g_OptionsTextColor);

	str = L"Inform me when my skills improve this much (in tenths)";
	FontManager->GenerateW(0, m_TexturePage9[5], str.c_str(), g_OptionsTextColor);

	str = L"Inform me of increases in strength, dexterity, and intelligence.";
	FontManager->GenerateW(0, m_TexturePage9[6], str.c_str(), g_OptionsTextColor);

	str = L"Console need press 'Enter' to activate it.";
	FontManager->GenerateW(0, m_TexturePage9[7], str.c_str(), g_OptionsTextColor);

	str = L"Set the font for speech";
	FontManager->GenerateW(0, m_TexturePage9[8], str.c_str(), g_OptionsTextColor);



	//Filter Options

}
//----------------------------------------------------------------------------
void TGumpOptions::ReleaseTextTextures()
{
	IFOR(i, 0, 8)
		m_TexturePage1[i].Clear();

	IFOR(i, 0, 8)
		m_TexturePage2[i].Clear();

	IFOR(i, 0, 6)
		m_TexturePage3[i].Clear();

	IFOR(i, 0, 19)
		m_TexturePage4[i].Clear();

	IFOR(i, 0, 7)
		m_TexturePage5[i].Clear();

	IFOR(i, 0, 13)
		m_TexturePage6[i].Clear();

	IFOR(i, 0, 18)
		m_TexturePage7[i].Clear();

	IFOR(i, 0, 9)
		m_TexturePage8[i].Clear();

	IFOR(i, 0, 9)
		m_TexturePage9[i].Clear();

	//IFOR(i, 0, 2)
		//m_TexturePage10[i].Clear();
}
//---------------------------------------------------------------------------
void TGumpOptions::GenerateFrame(int posX, int posY)
{
	if (!g_DrawMode)
	{
		FrameRedraw = false;
		FrameCreated = false;

		return;
	}

	DWORD index = (DWORD)this;

	//Нажата ли кнопка в окне?
	bool IsPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index && g_LastSelectedGump == index);

	//Может ли быть подсвечен элемент?
	int CanSelectedButton = ((g_LastSelectedGump == index) ? g_LastSelectedObject : 0);

	//Может ли быть нажат элемент?
	int CanPressedButton = 0;
	if (IsPressed && g_LastObjectLeftMouseDown == g_LastSelectedObject)
		CanPressedButton = g_LastObjectLeftMouseDown;

	glNewList((GLuint)index, GL_COMPILE);
	
		UO->DrawResizepicGump(0x0A28, posX + 40, posY, 550, 450); //Body



		//Left page buttons

		//Sound and Music
		WORD gumpID = 0x00DA;
		if (!m_Page)
			gumpID--;
		UO->DrawGump(gumpID, 0, posX, posY + 45);

		//Pop-up Help
		gumpID = 0x00DC;
		if (m_Page == 1)
			gumpID--;
		UO->DrawGump(gumpID, 0, posX, posY + 111);

		//Language
		if (ConnectionManager.ClientVersion < CV_400B)
		{
			gumpID = 0x00DE;
			if (m_Page == 2)
				gumpID--;
			UO->DrawGump(gumpID, 0, posX, posY + 177);
		}

		//Chat
		gumpID = 0x00E0;
		if (m_Page == 3)
			gumpID--;
		UO->DrawGump(gumpID, 0, posX, posY + 243);

		//Macro Options
		gumpID = 0x00ED;
		if (m_Page == 4)
			gumpID--;
		UO->DrawGump(gumpID, 0, posX, posY + 309);
		


		//Right page buttons

		//Interface
		gumpID = 0x00E2;
		if (m_Page == 5)
			gumpID--;
		UO->DrawGump(gumpID, 0, posX + 576, posY + 45);

		//Display
		gumpID = 0x00E4;
		if (m_Page == 6)
			gumpID--;
		UO->DrawGump(gumpID, 0, posX + 576, posY + 111);

		//Reputation System
		gumpID = 0x00E6;
		if (m_Page == 7)
			gumpID--;
		UO->DrawGump(gumpID, 0, posX + 576, posY + 177);

		//Miscellaneous
		gumpID = 0x00E8;
		if (m_Page == 8)
			gumpID--;
		UO->DrawGump(gumpID, 0, posX + 576, posY + 243);

		//Filter Options
		gumpID = 0x00EB;
		if (m_Page == 9)
			gumpID--;
		UO->DrawGump(gumpID, 0, posX + 576, posY + 309);
		


		if (CanPressedButton == ID_GO_CANCEL)
			gumpID = 0x00F1; //Cancel button (down)
		else if (CanSelectedButton == ID_GO_CANCEL)
			gumpID = 0x00F2; //Cancel button selected
		else
			gumpID = 0x00F3; //Cancel button
		UO->DrawGump(gumpID, 0, posX + 154, posY + 405);
		
		if (CanPressedButton == ID_GO_APPLY)
			gumpID = 0x00F0; //Apply button (down)
		else if (CanSelectedButton == ID_GO_APPLY)
			gumpID = 0x00EE; //Apply button selected
		else
			gumpID = 0x00EF; //Apply button
		UO->DrawGump(gumpID, 0, posX + 248, posY + 405);
		
		if (CanPressedButton == ID_GO_DEFAULT)
			gumpID = 0x00F4; //Default button (down)
		else if (CanSelectedButton == ID_GO_DEFAULT)
			gumpID = 0x00F5; //Default button selected
		else
			gumpID = 0x00F6; //Default button
		UO->DrawGump(gumpID, 0, posX + 346, posY + 405);
		
		if (CanPressedButton == ID_GO_OKAY)
			gumpID = 0x00F8; //Okay button (down)
		else if (CanSelectedButton == ID_GO_OKAY)
			gumpID = 0x00F7; //Okay button selected
		else
			gumpID = 0x00F9; //Okay button
		UO->DrawGump(gumpID, 0, posX + 443, posY + 405);

		bool renderMode = true;

		switch (m_Page)
		{
			case 0:
			{
				DrawPage1(renderMode, index, IsPressed, CanSelectedButton, CanPressedButton, posX, posY); //Sound and Music
				break;
			}
			case 1:
			{
				DrawPage2(renderMode, index, IsPressed, CanSelectedButton, CanPressedButton, posX, posY); //Pop-up Help
				break;
			}
			case 2:
			{
				if (ConnectionManager.ClientVersion < CV_400B)
					DrawPage3(renderMode, index, IsPressed, CanSelectedButton, CanPressedButton, posX, posY); //Language
				break;
			}
			case 3:
			{
				DrawPage4(renderMode, index, IsPressed, CanSelectedButton, CanPressedButton, posX, posY); //Chat
				break;
			}
			case 4:
			{
				DrawPage5(renderMode, index, IsPressed, CanSelectedButton, CanPressedButton, posX, posY); //Macro Options
				break;
			}
			case 5:
			{
				DrawPage6(renderMode, index, IsPressed, CanSelectedButton, CanPressedButton, posX, posY); //Interface
				break;
			}
			case 6:
			{
				DrawPage7(renderMode, index, IsPressed, CanSelectedButton, CanPressedButton, posX, posY); //Display
				break;
			}
			case 7:
			{
				DrawPage8(renderMode, index, IsPressed, CanSelectedButton, CanPressedButton, posX, posY); //Reputation System
				break;
			}
			case 8:
			{
				DrawPage9(renderMode, index, IsPressed, CanSelectedButton, CanPressedButton, posX, posY); //Miscellaneous
				break;
			}
			case 9:
			{
				DrawPage10(renderMode, index, IsPressed, CanSelectedButton, CanPressedButton, posX, posY); //Filter Options
				break;
			}
			default:
				break;
		}

	glEndList();

	FrameRedraw = true;
	FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpOptions::Draw(bool &mode)
{
	DWORD index = (DWORD)this;

	int posX = X;
	int posY = Y;

	//Нажата ли кнопка в окне?
	bool IsPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index && g_LastSelectedGump == index);

	//Может ли быть подсвечен элемент?
	int CanSelectedButton = ((g_LastSelectedGump == index) ? g_LastSelectedObject : 0);

	//Может ли быть нажат элемент?
	int CanPressedButton = 0;
	if (IsPressed && g_LastObjectLeftMouseDown == g_LastSelectedObject)
		CanPressedButton = g_LastObjectLeftMouseDown;

	DWORD ticks = GetTickCount();
	if (m_LastChangeMacroTime < ticks)
	{
		if (CanPressedButton == ID_GO_P5_BUTTON_PREVEOUS) //Preveous button
		{
			if (m_MacroPointer->m_Prev != NULL)
			{
				m_MacroPointer = (TMacro*)m_MacroPointer->m_Prev;
				m_MacroObjectPointer = (TMacroObject*)m_MacroPointer->m_Items;

				TextEntryMacro->SetText(m_HotkeyText[m_MacroPointer->Key & 0xFF]);
			}

			m_LastChangeMacroTime = ticks + CHANGE_MACRO_DELAY;
		}
		else if (CanPressedButton == ID_GO_P5_BUTTON_NEXT) //Next button
		{
			if (m_MacroPointer->m_Next != NULL)
			{
				m_MacroPointer = (TMacro*)m_MacroPointer->m_Next;
				m_MacroObjectPointer = (TMacroObject*)m_MacroPointer->m_Items;

				TextEntryMacro->SetText(m_HotkeyText[m_MacroPointer->Key & 0xFF]);
			}

			m_LastChangeMacroTime = ticks + CHANGE_MACRO_DELAY;
		}
		else if (m_MacroSelection)
		{
			if (m_MacroListOffset && g_MouseY < posY + m_MacroListOffsetYStart)
			{
				m_MacroListOffset -= m_MacroListMaxCount;
				m_LastChangeMacroTime = ticks + CHANGE_MACRO_LIST_DELAY;
			}
			else if ((m_MacroListOffset + m_MacroListMaxCount < m_MacroListCount) && g_MouseY > posY + m_MacroListOffsetYEnd)
			{
				m_MacroListOffset += m_MacroListMaxCount;
				m_LastChangeMacroTime = ticks + CHANGE_MACRO_LIST_DELAY;
			}
		}
	}

	if (g_LeftMouseDown && g_LastGumpLeftMouseDown == index && !g_LastObjectLeftMouseDown)
	{
		posX += (g_MouseX - g_DroppedLeftMouseX);
		posY += (g_MouseY - g_DroppedLeftMouseY);
		
		if (mode)
			GenerateFrame(posX, posY);
	}
	else if (mode)
	{
		if (IsPressed || CanSelectedButton)
			GenerateFrame(posX, posY);
		else if (FrameRedraw)
		{
			GenerateFrame(posX, posY);
			FrameRedraw = false;
		}
	}

	if (mode)
	{
		if (!FrameCreated)
			GenerateFrame(posX, posY);

		glCallList((GLuint)index);
	}
	else
	{
		if (UO->ResizepicPixelsInXY(0x0A28, posX + 40, posY, 550, 450)) //Body
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}



		//Left page buttons

		//Sound and Music
		WORD gumpID = 0x00DA;
		if (!m_Page)
			gumpID--;
		if (UO->GumpPixelsInXY(gumpID, posX, posY + 45))
		{
			g_LastSelectedObject = ID_GO_PAGE_0;
			g_LastSelectedGump = index;
		}

		//Pop-up Help
		gumpID = 0x00DC;
		if (m_Page == 1)
			gumpID--;
		if (UO->GumpPixelsInXY(gumpID, posX, posY + 111))
		{
			g_LastSelectedObject = ID_GO_PAGE_1;
			g_LastSelectedGump = index;
		}

		//Language
		if (ConnectionManager.ClientVersion < CV_400B)
		{
			gumpID = 0x00DE;
			if (m_Page == 2)
				gumpID--;
			if (UO->GumpPixelsInXY(gumpID, posX, posY + 177))
			{
				g_LastSelectedObject = ID_GO_PAGE_2;
				g_LastSelectedGump = index;
			}
		}

		//Chat
		gumpID = 0x00E0;
		if (m_Page == 3)
			gumpID--;
		if (UO->GumpPixelsInXY(gumpID, posX, posY + 243))
		{
			g_LastSelectedObject = ID_GO_PAGE_3;
			g_LastSelectedGump = index;
		}

		//Macro Options
		gumpID = 0x00ED;
		if (m_Page == 4)
			gumpID--;
		if (UO->GumpPixelsInXY(gumpID, posX, posY + 309))
		{
			g_LastSelectedObject = ID_GO_PAGE_4;
			g_LastSelectedGump = index;
		}


		
		//Right page buttons

		//Interface
		gumpID = 0x00E2;
		if (m_Page == 5)
			gumpID--;
		if (UO->GumpPixelsInXY(gumpID, posX + 576, posY + 45))
		{
			g_LastSelectedObject = ID_GO_PAGE_5;
			g_LastSelectedGump = index;
		}

		//Display
		gumpID = 0x00E4;
		if (m_Page == 6)
			gumpID--;
		if (UO->GumpPixelsInXY(gumpID, posX + 576, posY + 111))
		{
			g_LastSelectedObject = ID_GO_PAGE_6;
			g_LastSelectedGump = index;
		}

		//Reputation System
		gumpID = 0x00E6;
		if (m_Page == 7)
			gumpID--;
		if (UO->GumpPixelsInXY(gumpID, posX + 576, posY + 177))
		{
			g_LastSelectedObject = ID_GO_PAGE_7;
			g_LastSelectedGump = index;
		}

		//Miscellaneous
		gumpID = 0x00E8;
		if (m_Page == 8)
			gumpID--;
		if (UO->GumpPixelsInXY(gumpID, posX + 576, posY + 243))
		{
			g_LastSelectedObject = ID_GO_PAGE_8;
			g_LastSelectedGump = index;
		}

		//Filter Options
		gumpID = 0x00EB;
		if (m_Page == 9)
			gumpID--;
		if (UO->GumpPixelsInXY(gumpID, posX + 576, posY + 309))
		{
			g_LastSelectedObject = ID_GO_PAGE_9;
			g_LastSelectedGump = index;
		}
		

		 //Cancel button
		if (UO->GumpPixelsInXY(0x00F3, posX + 154, posY + 405))
		{
			g_LastSelectedObject = ID_GO_CANCEL;
			g_LastSelectedGump = index;
		}

		 //Apply button
		if (UO->GumpPixelsInXY(0x00EF, posX + 248, posY + 405))
		{
			g_LastSelectedObject = ID_GO_APPLY;
			g_LastSelectedGump = index;
		}

		 //Default button
		if (UO->GumpPixelsInXY(0x00F6, posX + 346, posY + 405))
		{
			g_LastSelectedObject = ID_GO_DEFAULT;
			g_LastSelectedGump = index;
		}

		 //Okay button
		if (UO->GumpPixelsInXY(0x00F9, posX + 443, posY + 405))
		{
			g_LastSelectedObject = ID_GO_OKAY;
			g_LastSelectedGump = index;
		}
	}

	if (!mode)
	{
		switch (m_Page)
		{
			case 0:
				return DrawPage1(mode, index, IsPressed, CanSelectedButton, CanPressedButton, posX, posY); //Sound and Music
			case 1:
				return DrawPage2(mode, index, IsPressed, CanSelectedButton, CanPressedButton, posX, posY); //Pop-up Help
			case 2:
				if (ConnectionManager.ClientVersion < CV_400B)
					return DrawPage3(mode, index, IsPressed, CanSelectedButton, CanPressedButton, posX, posY); //Language
			case 3:
				return DrawPage4(mode, index, IsPressed, CanSelectedButton, CanPressedButton, posX, posY); //Chat
			case 4:
				return DrawPage5(mode, index, IsPressed, CanSelectedButton, CanPressedButton, posX, posY); //Macro Options
			case 5:
				return DrawPage6(mode, index, IsPressed, CanSelectedButton, CanPressedButton, posX, posY); //Interface
			case 6:
				return DrawPage7(mode, index, IsPressed, CanSelectedButton, CanPressedButton, posX, posY); //Display
			case 7:
				return DrawPage8(mode, index, IsPressed, CanSelectedButton, CanPressedButton, posX, posY); //Reputation System
			case 8:
				return DrawPage9(mode, index, IsPressed, CanSelectedButton, CanPressedButton, posX, posY); //Miscellaneous
			case 9:
				return DrawPage10(mode, index, IsPressed, CanSelectedButton, CanPressedButton, posX, posY); //Filter Options
			default:
				break;
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
int TGumpOptions::DrawPage1(bool &mode, DWORD &index, bool &IsPressed, int &CanSelectedButton, int &CanPressedButton, int &posX, int &posY)
{
	//Sound and Music
	if (mode)
	{
		
		if (IsPressed)
		{
			if (g_LastObjectLeftMouseDown == ID_GO_P1_SOUND_VOLUME) //Sound volume slider pressed
			{
				int currentX = g_MouseX - posX - 3; //Текущая позиция ползунка

				if (currentX < 64)
					currentX = 64; //Выход за допустимый предел, корректируем на минимум
				else if (currentX > 152)
					currentX = 152; //Выход за допустимый предел, корректируем на максимум
			
				int count = g_OptionsConfig.SoundVolume; //Количество предметов в стеке
				if (currentX > 64) //Позиция ползунка больше минимума
				{
					if (currentX < 152) //Позиция ползунка меньше максимума, вычисляем
					{
						float ValPer = ((currentX - 64.0f) / g_SphereListWidth) * 100.0f;

						if (ValPer == 0.0f)
							count = 0;
						else
							count = (int)(255 * ValPer) / 100;
					}
					else
						count = 255; //Позиция равна максимуму, выставляем значение 255
				}
				else
					count = 0; //Позиция равна минимуму, выставляем значение 0

				g_OptionsConfig.SoundVolume = count;
			}
			else if (g_LastObjectLeftMouseDown == ID_GO_P1_MUSIC_VOLUME) //Music volume slider pressed
			{
				int currentX = g_MouseX - posX - 3; //Текущая позиция ползунка

				if (currentX < 64)
					currentX = 64; //Выход за допустимый предел, корректируем на минимум
				else if (currentX > 152)
					currentX = 152; //Выход за допустимый предел, корректируем на максимум
			
				int count = g_OptionsConfig.MusicVolume; //Количество предметов в стеке
				if (currentX > 64) //Позиция ползунка больше минимума
				{
					if (currentX < 152) //Позиция ползунка меньше максимума, вычисляем
					{
						float ValPer = ((currentX - 64.0f) / g_SphereListWidth) * 100.0f;

						if (ValPer == 0.0f)
							count = 0;
						else
							count = (int)(255.0f * ValPer) / 100;
					}
					else
						count = 255; //Позиция равна максимуму, выставляем значение 255
				}
				else
					count = 0; //Позиция равна минимуму, выставляем значение 0

				g_OptionsConfig.MusicVolume = count;
			}
		}

		//UO->DrawUnicodeFont(0, L"Sound and Music", g_OptionsTextColor, posX + 254, posY + 22);
		m_TexturePage1[0].Draw(posX + 84, posY + 22);

		//UO->DrawUnicodeFontFixed(0, L"Sound and Music", 1, posX + 240, posY + 42, 100);
		//UO->DrawUnicodeFontCropped(0, L"Sound and Music", 1, posX + 240, posY + 62, 60);

		//UO->DrawUnicodeFont(0, L"These settings affect the sound and music you will hear while playing Ultima Online.", g_OptionsTextColor, posX + 64, posY + 44, 480.0f);
		m_TexturePage1[1].Draw(posX + 64, posY + 44);

		posY += 90;
		UO->DrawGump(0x00D2 + (int)g_OptionsConfig.Sound, 0, posX + 64, posY);
		//UO->DrawUnicodeFont(0, L"Sound on/off", g_OptionsTextColor, posX + 86, posY);
		m_TexturePage1[2].Draw(posX + 86, posY);

		//UO->DrawUnicodeFont(0, L"Sound volume", g_OptionsTextColor, posX + 64, posY + 22);
		m_TexturePage1[3].Draw(posX + 64, posY + 22);
		
		posY += 43;
		UO->DrawSphereGump(g_OptionsConfig.SoundVolume, 255.0f, posX, posY);

		/*UO->DrawGump(0x00D5, 0, posX + 64, posY); //Sphere line gump start
		UO->DrawGump(0x00D7, 0, posX + 152, posY); //Sphere line gump end
		UO->DrawGump(0x00D6, 0, posX + 77, posY, 75, 0); //Sphere line gump body

		float ValPer = (g_OptionsConfig.SoundVolume / 255.0f) * 100.0f;
		ValPer = (g_SphereListWidth * ValPer) / 100.0f;
		if (ValPer < 0.0f)
			ValPer = 0.0f;
		UO->DrawGump(0x00D8, 0, posX + 64 + (int)ValPer, posY); //Sphere gump*/

		wchar_t text[20] = {0};
		swprintf(text, L"%d", g_OptionsConfig.SoundVolume);
		FontManager->DrawW(0, text, g_OptionsTextColor, posX + 176, posY - 3);
		


		posY += 18;
		UO->DrawGump(0x00D2 + (int)g_OptionsConfig.Music, 0, posX + 64, posY);
		//UO->DrawUnicodeFont(0, L"Music on/off", g_OptionsTextColor, posX + 88, posY);
		m_TexturePage1[4].Draw(posX + 88, posY);

		//UO->DrawUnicodeFont(0, L"Music volume", g_OptionsTextColor, posX + 64, posY + 22);
		m_TexturePage1[5].Draw(posX + 64, posY + 22);
		
		posY += 43;
		UO->DrawSphereGump(g_OptionsConfig.MusicVolume, 255.0f, posX, posY);

		/*UO->DrawGump(0x00D5, 0, posX + 64, posY); //Sphere line gump start
		UO->DrawGump(0x00D7, 0, posX + 152, posY); //Sphere line gump end
		UO->DrawGump(0x00D6, 0, posX + 77, posY, 75, 0); //Sphere line gump body

		ValPer = (g_OptionsConfig.MusicVolume / 255.0f) * 100.0f;
		ValPer = (g_SphereListWidth * ValPer) / 100.0f;
		if (ValPer < 0)
			ValPer = 0;
		UO->DrawGump(0x00D8, 0, posX + 64 + (int)ValPer, posY); //Sphere gump*/
		
		swprintf(text, L"%d", g_OptionsConfig.MusicVolume);
		FontManager->DrawW(0, text, g_OptionsTextColor, posX + 176, posY - 3);

		
		posY += 18;
		UO->DrawGump(0x00D2 + (int)g_OptionsConfig.FootstepsSound, 0, posX + 64, posY);
		//UO->DrawUnicodeFont(0, L"Play footstep sounds", g_OptionsTextColor, posX + 86, posY);
		m_TexturePage1[6].Draw(posX + 86, posY);
		
		posY += 20;
		UO->DrawGump(0x00D2 + (int)g_OptionsConfig.GetCombatMusic(), 0, posX + 64, posY);
		//UO->DrawUnicodeFont(0, L"Play combat music", g_OptionsTextColor, posX + 86, posY);
		m_TexturePage1[7].Draw(posX + 88, posY);
	}
	else
	{
		if (g_LastSelectedGump != index)
			return 0;

		int LSG = 0;
		
		if (UO->GumpPixelsInXY(0x00D2, posX + 64, posY + 90))
			LSG = ID_GO_P1_SOUND_ON_OFF; //Sound on/off
		else if (UO->GumpPixelsInXY(0x00D2, posX + 64, posY + 151))
			LSG = ID_GO_P1_MUSIC_ON_OFF; //Music on/off
		else if (UO->GumpPixelsInXY(0x00D2, posX + 64, posY + 212))
			LSG = ID_GO_P1_PLAY_FOOTSTEP_SOUNDS; //Play footstep sounds
		else if (UO->GumpPixelsInXY(0x00D2, posX + 64, posY + 232))
			LSG = ID_GO_P1_PLAY_COMBAT_MUSIC; //Play combat music
		else
		{
			RECT rc = {0, 0, (int)g_SphereListWidth + 10, 16};
			POINT p = {g_MouseX - (posX + 64), g_MouseY - (posY + 130)};
			if (PtInRect(&rc, p))
			LSG = ID_GO_P1_SOUND_VOLUME; //Sound volume

			p.y = g_MouseY - (posY + 192);
			if (PtInRect(&rc, p))
			LSG = ID_GO_P1_MUSIC_VOLUME; //Music volume
		}
		
		if (LSG != 0)
			g_LastSelectedObject = LSG;

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
int TGumpOptions::DrawPage2(bool &mode, DWORD &index, bool &IsPressed, int &CanSelectedButton, int &CanPressedButton, int &posX, int &posY)
{
	//Pop-up Help
	if (mode)
	{

		if (IsPressed)
		{
			if (g_LastObjectLeftMouseDown == ID_GO_P2_DELAY_BEFORE_POPUPS) //Delay befor Help pops up slider pressed
			{
				int currentX = g_MouseX - posX - 3; //Текущая позиция ползунка

				if (currentX < 64)
					currentX = 64; //Выход за допустимый предел, корректируем на минимум
				else if (currentX > 152)
					currentX = 152; //Выход за допустимый предел, корректируем на максимум
			
				int count = g_OptionsConfig.PopupHelpDelay; //Количество предметов в стеке
				if (currentX > 64) //Позиция ползунка больше минимума
				{
					if (currentX < 152) //Позиция ползунка меньше максимума, вычисляем
					{
						float ValPer = ((currentX - 64.0f) / g_SphereListWidth) * 100.0f;

						if (ValPer == 0.0f)
							count = 0;
						else
							count = (int)(5000 * ValPer) / 100;
					}
					else
						count = 5000; //Позиция равна максимуму, выставляем значение 255
				}
				else
					count = 0; //Позиция равна минимуму, выставляем значение 0

				g_OptionsConfig.PopupHelpDelay = count;
			}
		}

		//UO->DrawUnicodeFont(0, L"Pop-up Help", g_OptionsTextColor, posX + 274, posY + 22);
		m_TexturePage2[0].Draw(posX + 84, posY + 22);
		
		//UO->DrawUnicodeFont(0, L"These settings configure the behavior of the pop-up help.", g_OptionsTextColor, posX + 64, posY + 44, 480.0f);
		m_TexturePage2[1].Draw(posX + 64, posY + 44);
		
		posY += 90;
		UO->DrawGump(0x00D2 + (int)g_OptionsConfig.PopupHelpEnabled, 0, posX + 64, posY);
		//UO->DrawUnicodeFont(0, L"Use Pop-up Help", g_OptionsTextColor, posX + 86, posY);
		m_TexturePage2[2].Draw(posX + 86, posY);

		//UO->DrawUnicodeFont(0, L"Delay befor Help pops up", g_OptionsTextColor, posX + 64, posY + 22);
		m_TexturePage2[3].Draw(posX + 64, posY + 22);
		
		posY += 43;
		UO->DrawSphereGump(g_OptionsConfig.PopupHelpDelay, 5000.0f, posX, posY);

		/*UO->DrawGump(0x00D5, 0, posX + 64, posY); //Sphere line gump start
		UO->DrawGump(0x00D7, 0, posX + 152, posY); //Sphere line gump end
		UO->DrawGump(0x00D6, 0, posX + 77, posY, 75, 0); //Sphere line gump body

		float ValPer = (g_OptionsConfig.PopupHelpDelay / 5000.0f) * 100.0f;
		ValPer = (g_SphereListWidth * ValPer) / 100.0f;
		if (ValPer < 0.0f)
			ValPer = 0.0f;
		UO->DrawGump(0x00D8, 0, posX + 64 + (int)ValPer, posY); //Sphere gump*/

		wchar_t text[20] = {0};
		swprintf(text, L"%d", g_OptionsConfig.PopupHelpDelay);
		FontManager->DrawW(0, text, g_OptionsTextColor, posX + 176, posY - 3);
		


		posY += 18;
		UO->DrawGump(0x00D4, 0, posX + 64, posY);
		DWORD clr = 0xFF7F7F7F;
		if (g_OptionsConfig.PopupHelpTextColor != 0xFFFF)
			clr = ColorManager->GetPolygoneColor(5, g_OptionsConfig.PopupHelpTextColor);
		g_GL.DrawPolygone(clr, posX + 67.0f, posY + 3.0f, 13.0f, 14.0f);
		//UO->DrawUnicodeFont(0, L"Text Color", g_OptionsTextColor, posX + 88, posY);
		m_TexturePage2[4].Draw(posX + 88, posY);
		
		posY += 22;
		UO->DrawGump(0x00D0, 0, posX + 64, posY);
		//UO->DrawUnicodeFont(0, L"Help text font", g_OptionsTextColor, posX + 64, posY + 22);
		m_TexturePage2[5].Draw(posX + 88, posY);
		
		posY += 24;
		UO->DrawGump(0x00D2 + (int)g_OptionsConfig.PopupHelpSticky, 0, posX + 64, posY);
		//UO->DrawUnicodeFont(0, L"Make window Sticky", g_OptionsTextColor, posX + 86, posY);
		m_TexturePage2[6].Draw(posX + 86, posY);
		
		posY += 20;
		UO->DrawGump(0x00D2 + (int)g_OptionsConfig.PopupHelpPersistant, 0, posX + 64, posY);
		//UO->DrawUnicodeFont(0, L"Make text Persistant", g_OptionsTextColor, posX + 86, posY);
		m_TexturePage2[7].Draw(posX + 86, posY);
	}
	else
	{
		if (g_LastSelectedGump != index)
			return 0;

		int LSG = 0;
		
		if (UO->GumpPixelsInXY(0x00D2, posX + 64, posY + 90))
			LSG = ID_GO_P2_USE_POPUP; //Use Pop-up Help
		else if (UO->GumpPixelsInXY(0x00D2, posX + 64, posY + 151))
			LSG = ID_GO_P2_TEXT_COLOR; //Text Color
		else if (UO->GumpPixelsInXY(0x00D0, posX + 64, posY + 173))
			LSG = ID_GO_P2_TEXT_FONT; //Help text font
		else if (UO->GumpPixelsInXY(0x00D2, posX + 64, posY + 197))
			LSG = ID_GO_P2_MAKE_WINDOW_STICKY; //Make window Sticky
		else if (UO->GumpPixelsInXY(0x00D2, posX + 64, posY + 217))
			LSG = ID_GO_P2_MAKE_TEXT_PERTISTANT; //Make text Persistant
		else
		{
			RECT rc = {0, 0, (int)g_SphereListWidth + 10, 16};
			POINT p = {g_MouseX - (posX + 64), g_MouseY - (posY + 130)};
			if (PtInRect(&rc, p))
				LSG = ID_GO_P2_DELAY_BEFORE_POPUPS; //Delay befor Help pops up
		}

		if (LSG != 0)
			g_LastSelectedObject = LSG;

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
int TGumpOptions::DrawPage3(bool &mode, DWORD &index, bool &IsPressed, int &CanSelectedButton, int &CanPressedButton, int &posX, int &posY)
{
	//Language
	if (mode)
	{

		if (IsPressed)
		{
			if (g_LastObjectLeftMouseDown == ID_GO_P3_DELAY_BEFORE_TOOLTIP) //Delay befor Tool-tip appears slider pressed
			{
				int currentX = g_MouseX - posX - 3; //Текущая позиция ползунка

				if (currentX < 64)
					currentX = 64; //Выход за допустимый предел, корректируем на минимум
				else if (currentX > 152)
					currentX = 152; //Выход за допустимый предел, корректируем на максимум
			
				WORD count = g_OptionsConfig.ToolTipsDelay; //Количество предметов в стеке
				if (currentX > 64) //Позиция ползунка больше минимума
				{
					if (currentX < 152) //Позиция ползунка меньше максимума, вычисляем
					{
						float ValPer = ((currentX - 64.0f) / g_SphereListWidth) * 100.0f;

						if (ValPer == 0.0f)
							count = 0;
						else
							count = (int)(5000.0f * ValPer) / 100;
					}
					else
						count = 5000; //Позиция равна максимуму, выставляем значение 255
				}
				else
					count = 0; //Позиция равна минимуму, выставляем значение 0

				g_OptionsConfig.ToolTipsDelay = count;
			}
		}

		//UO->DrawUnicodeFont(0, L"Language", g_OptionsTextColor, posX + 280, posY + 22);
		m_TexturePage3[0].Draw(posX + 84, posY + 22);
		
		//UO->DrawUnicodeFont(0, L"The language you use when playing UO is obtained from your Operating System settings.", g_OptionsTextColor, posX + 64, posY + 44, 480.0f);
		m_TexturePage3[1].Draw(posX + 64, posY + 44);

		posY += 90;
		UO->DrawGump(0x00D2 + (int)g_OptionsConfig.UseToolTips, 0, posX + 64, posY);
		//UO->DrawUnicodeFont(0, L"Use Tool-tips", g_OptionsTextColor, posX + 86, posY);
		m_TexturePage3[2].Draw(posX + 86, posY);

		//UO->DrawUnicodeFont(0, L"Delay befor Tool-tip appears", g_OptionsTextColor, posX + 64, posY + 22);
		m_TexturePage3[3].Draw(posX + 64, posY + 22);
		
		posY += 43;
		UO->DrawGump(0x00D5, 0, posX + 64, posY); //Sphere line gump start
		UO->DrawGump(0x00D7, 0, posX + 152, posY); //Sphere line gump end
		UO->DrawGump(0x00D6, 0, posX + 77, posY, 75, 0); //Sphere line gump body

		float ValPer = (g_OptionsConfig.ToolTipsDelay / 5000.0f) * 100.0f;
		ValPer = (g_SphereListWidth * ValPer) / 100.0f;
		if (ValPer < 0.0f)
			ValPer = 0.0f;
		UO->DrawGump(0x00D8, 0, posX + 64 + (int)ValPer, posY); //Sphere gump

		wchar_t text[20] = {0};
		swprintf(text, L"%d", g_OptionsConfig.ToolTipsDelay);
		FontManager->DrawW(0, text, g_OptionsTextColor, posX + 176, posY - 3);
		


		posY += 18;
		UO->DrawGump(0x00D4, 0, posX + 64, posY);
		DWORD clr = 0xFF7F7F7F;
		if (g_OptionsConfig.ToolTipsTextColor != 0xFFFF)
			clr = ColorManager->GetPolygoneColor(5, g_OptionsConfig.ToolTipsTextColor);
		g_GL.DrawPolygone(clr, posX + 67.0f, posY + 3.0f, 13.0f, 14.0f);
		//UO->DrawUnicodeFont(0, L"Color of Tool-tip text", g_OptionsTextColor, posX + 88, posY);
		m_TexturePage3[4].Draw(posX + 88, posY);
		
		posY += 22;
		UO->DrawGump(0x00D0, 0, posX + 64, posY);
		//UO->DrawUnicodeFont(0, L"Font for Tool-tips", g_OptionsTextColor, posX + 64, posY + 22);
		m_TexturePage3[5].Draw(posX + 88, posY);
	}
	else
	{
		if (g_LastSelectedGump != index)
			return 0;

		int LSG = 0;
		
		if (UO->GumpPixelsInXY(0x00D2, posX + 64, posY + 90))
			LSG = ID_GO_P3_USE_TOOLTIP; //Use Tool-tips
		else if (UO->GumpPixelsInXY(0x00D2, posX + 64, posY + 151))
			LSG = ID_GO_P3_TEXT_COLOR; //Color of Tool-tip text
		else if (UO->GumpPixelsInXY(0x00D0, posX + 64, posY + 173))
			LSG = ID_GO_P3_TEXT_FONT; //Font for Tool-tips
		else
		{
			RECT rc = {0, 0, (int)g_SphereListWidth + 10, 16};
			POINT p = {g_MouseX - (posX + 64), g_MouseY - (posY + 130)};
			if (PtInRect(&rc, p))
				LSG = ID_GO_P3_DELAY_BEFORE_TOOLTIP; //Delay befor Tool-tip appears
		}

		if (LSG != 0)
			g_LastSelectedObject = LSG;

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
int TGumpOptions::DrawPage4(bool &mode, DWORD &index, bool &IsPressed, int &CanSelectedButton, int &CanPressedButton, int &posX, int &posY)
{
	//Chat

	const int step = 19;

	if (mode)
	{
		//UO->DrawUnicodeFont(0, L"Chat", g_OptionsTextColor, posX + 292, posY + 22);
		m_TexturePage4[0].Draw(posX + 84, posY + 22);
		
		//UO->DrawUnicodeFont(0, L"These settings affect the interface display for the chat system.", g_OptionsTextColor, posX + 64, posY + 44, 480.0f);
		m_TexturePage4[1].Draw(posX + 64, posY + 44);
		

		posY += 90;
		
		int posYStart = posY;

		UO->DrawGump(0x00D4, 0, posX + 64, posY);
		DWORD clr = 0xFF7F7F7F;
		if (g_OptionsConfig.ChatColorInputText != 0xFFFF)
			clr = ColorManager->GetPolygoneColor(5, g_OptionsConfig.ChatColorInputText);
		g_GL.DrawPolygone(clr, posX + 67.0f, posY + 3.0f, 13.0f, 14.0f);
		//UO->DrawUnicodeFont(0, L"Input text color", g_OptionsTextColor, posX + 88, posY);
		m_TexturePage4[2].Draw(posX + 88, posY);
		
		posY += step;
		UO->DrawGump(0x00D4, 0, posX + 64, posY);
		clr = 0xFF7F7F7F;
		if (g_OptionsConfig.ChatColorMenuOption != 0xFFFF)
			clr = ColorManager->GetPolygoneColor(5, g_OptionsConfig.ChatColorMenuOption);
		g_GL.DrawPolygone(clr, posX + 67.0f, posY + 3.0f, 13.0f, 14.0f);
		//UO->DrawUnicodeFont(0, L"Menu option color", g_OptionsTextColor, posX + 88, posY);
		m_TexturePage4[3].Draw(posX + 88, posY);
		
		posY += step;
		UO->DrawGump(0x00D4, 0, posX + 64, posY);
		clr = 0xFF7F7F7F;
		if (g_OptionsConfig.ChatColorPlayerInMemberList != 0xFFFF)
			clr = ColorManager->GetPolygoneColor(5, g_OptionsConfig.ChatColorPlayerInMemberList);
		g_GL.DrawPolygone(clr, posX + 67.0f, posY + 3.0f, 13.0f, 14.0f);
		//UO->DrawUnicodeFont(0, L"Player color in member list", g_OptionsTextColor, posX + 88, posY);
		m_TexturePage4[3].Draw(posX + 88, posY);
		
		posY += step;
		UO->DrawGump(0x00D4, 0, posX + 64, posY);
		clr = 0xFF7F7F7F;
		if (g_OptionsConfig.ChatColorText != 0xFFFF)
			clr = ColorManager->GetPolygoneColor(5, g_OptionsConfig.ChatColorText);
		g_GL.DrawPolygone(clr, posX + 67.0f, posY + 3.0f, 13.0f, 14.0f);
		//UO->DrawUnicodeFont(0, L"Chat text color", g_OptionsTextColor, posX + 88, posY);
		m_TexturePage4[5].Draw(posX + 88, posY);
		
		posY += step;
		UO->DrawGump(0x00D4, 0, posX + 64, posY);
		clr = 0xFF7F7F7F;
		if (g_OptionsConfig.ChatColorPlayerNameWithout != 0xFFFF)
			clr = ColorManager->GetPolygoneColor(5, g_OptionsConfig.ChatColorPlayerNameWithout);
		g_GL.DrawPolygone(clr, posX + 67.0f, posY + 3.0f, 13.0f, 14.0f);
		//UO->DrawUnicodeFont(0, L"Player name without speaking privileges", g_OptionsTextColor, posX + 88, posY, 140);
		m_TexturePage4[6].Draw(posX + 88, posY);
		
		posY += step + 16;
		UO->DrawGump(0x00D4, 0, posX + 64, posY);
		clr = 0xFF7F7F7F;
		if (g_OptionsConfig.ChatColorMuted != 0xFFFF)
			clr = ColorManager->GetPolygoneColor(5, g_OptionsConfig.ChatColorMuted);
		g_GL.DrawPolygone(clr, posX + 67.0f, posY + 3.0f, 13.0f, 14.0f);
		//UO->DrawUnicodeFont(0, L"Muted text color", g_OptionsTextColor, posX + 88, posY);
		m_TexturePage4[7].Draw(posX + 88, posY);
		
		posY += step;
		UO->DrawGump(0x00D4, 0, posX + 64, posY);
		clr = 0xFF7F7F7F;
		if (g_OptionsConfig.ChatColorChannelModeratorName != 0xFFFF)
			clr = ColorManager->GetPolygoneColor(5, g_OptionsConfig.ChatColorChannelModeratorName);
		g_GL.DrawPolygone(clr, posX + 67.0f, posY + 3.0f, 13.0f, 14.0f);
		//UO->DrawUnicodeFont(0, L"Channel moderator name", g_OptionsTextColor, posX + 88, posY);
		m_TexturePage4[8].Draw(posX + 88, posY);
		
		posY += step;
		UO->DrawGump(0x00D4, 0, posX + 64, posY);
		clr = 0xFF7F7F7F;
		if (g_OptionsConfig.ChatColorChannelModeratorText != 0xFFFF)
			clr = ColorManager->GetPolygoneColor(5, g_OptionsConfig.ChatColorChannelModeratorText);
		g_GL.DrawPolygone(clr, posX + 67.0f, posY + 3.0f, 13.0f, 14.0f);
		//UO->DrawUnicodeFont(0, L"Channel moderator text", g_OptionsTextColor, posX + 88, posY);
		m_TexturePage4[9].Draw(posX + 88, posY);
		
		posY += step;
		UO->DrawGump(0x00D4, 0, posX + 64, posY);
		clr = 0xFF7F7F7F;
		if (g_OptionsConfig.ChatColorMyName != 0xFFFF)
			clr = ColorManager->GetPolygoneColor(5, g_OptionsConfig.ChatColorMyName);
		g_GL.DrawPolygone(clr, posX + 67.0f, posY + 3.0f, 13.0f, 14.0f);
		//UO->DrawUnicodeFont(0, L"My name's color", g_OptionsTextColor, posX + 88, posY);
		m_TexturePage4[10].Draw(posX + 88, posY);
		
		posY += step;
		UO->DrawGump(0x00D4, 0, posX + 64, posY);
		clr = 0xFF7F7F7F;
		if (g_OptionsConfig.ChatColorMyText != 0xFFFF)
			clr = ColorManager->GetPolygoneColor(5, g_OptionsConfig.ChatColorMyText);
		g_GL.DrawPolygone(clr, posX + 67.0f, posY + 3.0f, 13.0f, 14.0f);
		//UO->DrawUnicodeFont(0, L"My text color", g_OptionsTextColor, posX + 88, posY);
		m_TexturePage4[11].Draw(posX + 88, posY);
		
		posY += step;
		UO->DrawGump(0x00D4, 0, posX + 64, posY);
		clr = 0xFF7F7F7F;
		if (g_OptionsConfig.ChatColorSystemMessage != 0xFFFF)
			clr = ColorManager->GetPolygoneColor(5, g_OptionsConfig.ChatColorSystemMessage);
		g_GL.DrawPolygone(clr, posX + 67.0f, posY + 3.0f, 13.0f, 14.0f);
		//UO->DrawUnicodeFont(0, L"System message color", g_OptionsTextColor, posX + 88, posY);
		m_TexturePage4[12].Draw(posX + 88, posY);

		

		posY = posYStart;
		UO->DrawGump(0x00D4, 0, posX + 300, posY);
		clr = 0xFF7F7F7F;
		if (g_OptionsConfig.ChatColorBGOutputText != 0xFFFF)
			clr = ColorManager->GetPolygoneColor(5, g_OptionsConfig.ChatColorBGOutputText);
		g_GL.DrawPolygone(clr, posX + 303.0f, posY + 3.0f, 13.0f, 14.0f);
		//UO->DrawUnicodeFont(0, L"Text Output Background Color", g_OptionsTextColor, posX + 324, posY);
		m_TexturePage4[13].Draw(posX + 324, posY);

		posY += step;
		UO->DrawGump(0x00D4, 0, posX + 300, posY);
		clr = 0xFF7F7F7F;
		if (g_OptionsConfig.ChatColorBGInputText != 0xFFFF)
			clr = ColorManager->GetPolygoneColor(5, g_OptionsConfig.ChatColorBGInputText);
		g_GL.DrawPolygone(clr, posX + 303.0f, posY + 3.0f, 13.0f, 14.0f);
		//UO->DrawUnicodeFont(0, L"Text Input Background Color", g_OptionsTextColor, posX + 324, posY);
		m_TexturePage4[14].Draw(posX + 324, posY);

		posY += step;
		UO->DrawGump(0x00D4, 0, posX + 300, posY);
		clr = 0xFF7F7F7F;
		if (g_OptionsConfig.ChatColorBGUserList != 0xFFFF)
			clr = ColorManager->GetPolygoneColor(5, g_OptionsConfig.ChatColorBGUserList);
		g_GL.DrawPolygone(clr, posX + 303.0f, posY + 3.0f, 13.0f, 14.0f);
		//UO->DrawUnicodeFont(0, L"User List Background Color", g_OptionsTextColor, posX + 324, posY);
		m_TexturePage4[15].Draw(posX + 324, posY);

		posY += step;
		UO->DrawGump(0x00D4, 0, posX + 300, posY);
		clr = 0xFF7F7F7F;
		if (g_OptionsConfig.ChatColorBGConfList != 0xFFFF)
			clr = ColorManager->GetPolygoneColor(5, g_OptionsConfig.ChatColorBGConfList);
		g_GL.DrawPolygone(clr, posX + 303.0f, posY + 3.0f, 13.0f, 14.0f);
		//UO->DrawUnicodeFont(0, L"Conference List Background Color", g_OptionsTextColor, posX + 324, posY);
		m_TexturePage4[16].Draw(posX + 324, posY);

		posY += step;
		UO->DrawGump(0x00D4, 0, posX + 300, posY);
		clr = 0xFF7F7F7F;
		if (g_OptionsConfig.ChatColorBGCommandList != 0xFFFF)
			clr = ColorManager->GetPolygoneColor(5, g_OptionsConfig.ChatColorBGCommandList);
		g_GL.DrawPolygone(clr, posX + 303.0f, posY + 3.0f, 13.0f, 14.0f);
		//UO->DrawUnicodeFont(0, L"Command List Background Color", g_OptionsTextColor, posX + 324, posY);
		m_TexturePage4[17].Draw(posX + 324, posY);

		
		posY += 26;
		UO->DrawGump(0x00D0, 0, posX + 300, posY);
		//UO->DrawUnicodeFont(0, L"Chat font", g_OptionsTextColor, posX + 300, posY);
		m_TexturePage4[18].Draw(posX + 324, posY);
	}
	else
	{
		if (g_LastSelectedGump != index)
			return 0;

		int LSG = 0;
		
		int posXStart = posX;
		posX += 64;

		if (UO->GumpPixelsInXY(0x00D2, posX, posY + 90))
			LSG = ID_GO_P4_TEXT_COLOR; //Input text color
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 109))
			LSG = ID_GO_P4_TEXT_COLOR + 1; //Menu option color
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 128))
			LSG = ID_GO_P4_TEXT_COLOR + 2; //Player color in member list
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 147))
			LSG = ID_GO_P4_TEXT_COLOR + 3; //Chat text color
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 166))
			LSG = ID_GO_P4_TEXT_COLOR + 4; //Player name without speaking privileges
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 201))
			LSG = ID_GO_P4_TEXT_COLOR + 5; //Muted text color
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 220))
			LSG = ID_GO_P4_TEXT_COLOR + 6; //Channel moderator name
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 239))
			LSG = ID_GO_P4_TEXT_COLOR + 7; //Channel moderator text
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 258))
			LSG = ID_GO_P4_TEXT_COLOR + 8; //My name's color
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 277))
			LSG = ID_GO_P4_TEXT_COLOR + 9; //My text color
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 296))
			LSG = ID_GO_P4_TEXT_COLOR + 10; //System message color
		else
		{
			posX = posXStart + 300;
			
			if (UO->GumpPixelsInXY(0x00D2, posX, posY + 90))
				LSG = ID_GO_P4_TEXT_COLOR + 11; //Text Output Background Color
			else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 109))
				LSG = ID_GO_P4_TEXT_COLOR + 12; //Text Input Background Color
			else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 128))
				LSG = ID_GO_P4_TEXT_COLOR + 13; //User List Background Color
			else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 147))
				LSG = ID_GO_P4_TEXT_COLOR + 14; //Conference List Background Color
			else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 166))
				LSG = ID_GO_P4_TEXT_COLOR + 15; //Command List Background Color
			else if (UO->GumpPixelsInXY(0x00D0, posX, posY + 192))
				LSG = ID_GO_P4_TEXT_FONT; //Chat font
		}

		if (LSG != 0)
			g_LastSelectedObject = LSG;

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
int TGumpOptions::DrawPage5(bool &mode, DWORD &index, bool &IsPressed, int &CanSelectedButton, int &CanPressedButton, int &posX, int &posY)
{
	const int maxMacroDraw = 7;

	int captionY = posY + 22;
	posY = captionY + 38;
	posX += 30;

	//Macro Options
	if (mode)
	{
		bool alt = false;
		bool ctrl = false;
		bool shift = false;
		WORD key = 0;

		if (m_MacroPointer != NULL)
		{
			alt = m_MacroPointer->Alt;
			ctrl = m_MacroPointer->Ctrl;
			shift = m_MacroPointer->Shift;
			key = m_MacroPointer->Key;
		}

		m_TexturePage5[0].Draw(posX + 84, captionY); //"Macro Options"
		
		//Add button
		WORD gumpID = 0x099C;
		if (CanPressedButton == ID_GO_P5_BUTTON_ADD)
			gumpID++;
		else if (CanSelectedButton == ID_GO_P5_BUTTON_ADD)
			gumpID = 0x099E;

		UO->DrawGump(gumpID, 0, posX + 152, posY);
		
		//Delete button
		if (CanPressedButton == ID_GO_P5_BUTTON_DELETE)
			gumpID = 0x09A0;
		else
			gumpID = 0x099F + (int)((CanSelectedButton == ID_GO_P5_BUTTON_DELETE) ? 2 : 0);

		UO->DrawGump(gumpID, 0, posX + 205, posY);
		
		//Preveous button
		if (CanPressedButton == ID_GO_P5_BUTTON_PREVEOUS)
			gumpID = 0x09A3;
		else
			gumpID = 0x09A2 + (int)((CanSelectedButton == ID_GO_P5_BUTTON_PREVEOUS) ? 2 : 0);

		UO->DrawGump(gumpID, 0, posX + 273, posY);
		
		//Next button
		if (CanPressedButton == ID_GO_P5_BUTTON_NEXT)
			gumpID = 0x09A6;
		else
			gumpID = 0x09A5 + (int)((CanSelectedButton == ID_GO_P5_BUTTON_NEXT) ? 2 : 0);

		UO->DrawGump(gumpID, 0, posX + 357, posY);
		
		m_TexturePage5[1].Draw(posX + 134, posY + 22); //"Keystroke"
		
		//KeyBox
		UO->DrawGump(0x098B, 0, posX + 133, posY + 52);
		TextEntryMacro->DrawA(1, 0x0386, posX + 137, posY + 57);
		
		m_TexturePage5[2].Draw(posX + 200, posY + 51); //"Key"
		
		//Shift checkbox
		if (shift)
			gumpID = 0x0869;
		else
			gumpID = 0x0867 + (int)(CanSelectedButton == ID_GO_P5_BUTTON_SHIFT);

		UO->DrawGump(gumpID, 0, posX + 248, posY + 19);
		
		m_TexturePage5[3].Draw(posX + 280, posY + 22); //"Shift"
		
		//Alt checkbox
		if (alt)
			gumpID = 0x0869;
		else
			gumpID = 0x0867 + (int)(CanSelectedButton == ID_GO_P5_BUTTON_ALT);

		UO->DrawGump(gumpID, 0, posX + 248, posY + 47);
		
		m_TexturePage5[4].Draw(posX + 280, posY + 51); //"Alt"
		
		//Ctrl checkbox
		if (ctrl)
			gumpID = 0x0869;
		else
			gumpID = 0x0867 + (int)(CanSelectedButton == ID_GO_P5_BUTTON_CTRL);

		UO->DrawGump(gumpID, 0, posX + 248, posY + 75);
		
		m_TexturePage5[5].Draw(posX + 280, posY + 80); //"Ctrl"
		
		posX += 134;

		m_TexturePage5[6].Draw(posX, posY + 103); //"Actions"
		
		int boxPosY = posY + 127;
		int arrowPosY = posY + 133;
		bool drawEmptyBox = true;
		
		TMacroObject *obj = m_MacroObjectPointer;
		if (obj != NULL)
		{
			if (obj->m_Prev != NULL)
				UO->DrawGump(0x0983 + (int)(CanSelectedButton == ID_GO_P5_BUTTON_UP), 0, posX + 292, posY + 113);
			
			int macroCount = 0;

			while (obj != NULL && macroCount < maxMacroDraw)
			{
				UO->DrawGump(0x098D, 0, posX, boxPosY);
				FontManager->DrawA(1, TMacro::GetActionName(obj->Code), 0x0386, posX + 4, boxPosY + 5, 98, TS_CENTER, UOFONT_FIXED);
				UO->DrawGump(0x0985, 0, posX + 94, arrowPosY);

				char hasMenu = obj->HasSubMenu;
				if (hasMenu)
				{
					UO->DrawGump(0x098E, 0, posX + 112, boxPosY);

					if (hasMenu == 1) //Нужна стрелочка вниз
					{
						UO->DrawGump(0x0985, 0, posX + 274, arrowPosY);
						
						FontManager->DrawA(1, TMacro::GetAction(obj->SubCode), 0x0386, posX + 118, boxPosY + 5, 150, TS_LEFT, UOFONT_FIXED);
					}
					else
						((TMacroObjectEntry*)obj)->TextEntry->DrawA(1, 0x0386, posX + 118, boxPosY + 5, TS_LEFT, UOFONT_FIXED);
				}

				boxPosY += 26;
				arrowPosY += 26;
				macroCount++;

				if (obj->m_Next == NULL)
					break;

				obj = (TMacroObject*)obj->m_Next;
			}
			
			if (macroCount >= maxMacroDraw)
			{
				drawEmptyBox = false;
				UO->DrawGump(0x0985 + (int)(CanSelectedButton == ID_GO_P5_BUTTON_DOWN), 0, posX + 292, boxPosY);
			}
			else if (obj->Code == MC_NONE)
				drawEmptyBox = false;
		}

		if (drawEmptyBox)
		{
			UO->DrawGump(0x098D, 0, posX, boxPosY);
			UO->DrawGump(0x0985, 0, posX + 94, arrowPosY);
		}

		if (m_MacroSelection)
		{
			bool isAction = false;
			if (m_MacroSelection == 0x20000000) //Action
			{
				posX += 116;
				isAction = true;
			}

			int textIndex = m_MacroListOffset;

			posY -= 60;
			posY += m_MacroListOffsetYStart;
				
			//Top
			UO->DrawGump(0x09B5, 0, posX - 5, posY - 11);

			if (textIndex > 0)
				UO->DrawGump(0x0983, 0, posX + 85, posY - 8);
				
			int ofs = 0;

			int count = m_MacroListMaxCount;

			if (count > m_MacroListCount)
				count = m_MacroListCount;

			IFOR(i, 0, count)
			{
				gumpID = 0x09B6 + ofs;
				ofs = (ofs + 1) % 3;

				int itemPosY = posY + (i * 15);

				UO->DrawGump(gumpID, 0, posX, itemPosY);
				
				int textNameIndex = textIndex + m_MacroListNameOffset;

				if (textIndex < m_MacroListCount)
				{
					if (CanSelectedButton >= ID_GO_P5_SELECTION)
					{
						if (textNameIndex == (CanSelectedButton - ID_GO_P5_SELECTION))
						{
							g_GL.DrawPolygone(0x7F7F7F7F, posX + 4.0f, (float)itemPosY, 150.0f, 14.0f);
						}
					}

					if (isAction) //Action
					{
						if (textNameIndex < 209)
						{
							FontManager->DrawA(1, TMacro::GetAction(textNameIndex), 0x0386, posX + 4, itemPosY);
							textIndex++;
						}
					}
					else //Action name
					{
						if (textNameIndex < 60)
						{
							FontManager->DrawA(1, TMacro::GetActionName(textNameIndex), 0x0386, posX + 4, itemPosY);
							textIndex++;
						}
					}
				}
			}

			int offsBottomY = (m_MacroListOffsetYEnd - m_MacroListOffsetYStart);

			//Bottom
			UO->DrawGump(0x09B9, 0, posX - 5, posY + offsBottomY);

			if (m_MacroListOffset + m_MacroListMaxCount < m_MacroListCount)
				UO->DrawGump(0x0985, 0, posX + 85, posY + offsBottomY - 2);
		}
	}
	else
	{
		if (g_LastSelectedGump != index)
			return 0;

		int LSG = 0;
		
		if (UO->GumpPixelsInXY(0x099C, posX + 152, posY))
			LSG = ID_GO_P5_BUTTON_ADD; //Add
		else if (UO->GumpPixelsInXY(0x099F, posX + 205, posY))
			LSG = ID_GO_P5_BUTTON_DELETE; //Delete
		else if (UO->GumpPixelsInXY(0x09A2, posX + 273, posY))
			LSG = ID_GO_P5_BUTTON_PREVEOUS; //Preveous
		else if (UO->GumpPixelsInXY(0x09A5, posX + 357, posY))
			LSG = ID_GO_P5_BUTTON_NEXT; //Next
		else if (UO->GumpPixelsInXY(0x098B, posX + 133, posY + 52))
			LSG = ID_GO_P5_KEY_BOX; //Key box
		else if (UO->GumpPixelsInXY(0x0867, posX + 248, posY + 19))
			LSG = ID_GO_P5_BUTTON_SHIFT; //Shift
		else if (UO->GumpPixelsInXY(0x0867, posX + 248, posY + 47))
			LSG = ID_GO_P5_BUTTON_ALT; //Shift
		else if (UO->GumpPixelsInXY(0x0867, posX + 248, posY + 75))
			LSG = ID_GO_P5_BUTTON_CTRL; //Shift

		posX += 134;

		int boxPosY = posY + 127;
		int arrowPosY = posY + 133;
		bool drawEmptyBox = true;
		
		TMacroObject *obj = m_MacroObjectPointer;
		if (obj != NULL)
		{
			if (obj->m_Prev != NULL && UO->GumpPixelsInXY(0x0983, posX + 292, posY + 113))
				LSG = ID_GO_P5_BUTTON_UP; //UP arrow
			
			int macroCount = 0;

			while (obj != NULL && macroCount < maxMacroDraw)
			{
				if (UO->GumpPixelsInXY(0x098D, posX, boxPosY))
					LSG = ID_GO_P5_LEFT_BOX + macroCount; //Left macro box

				char hasMenu = obj->HasSubMenu;
				if (hasMenu)
				{
					if (UO->GumpPixelsInXY(0x098E, posX + 112, boxPosY))
						LSG = ID_GO_P5_RIGHT_BOX + macroCount; //Right macro box
				}

				boxPosY += 26;
				arrowPosY += 26;
				macroCount++;
					
				if (obj->m_Next == NULL)
					break;

				obj = (TMacroObject*)obj->m_Next;
			}
			
			if (macroCount >= maxMacroDraw)
			{
				drawEmptyBox = false;

				if (UO->GumpPixelsInXY(0x0985, posX + 292, boxPosY))
					LSG = ID_GO_P5_BUTTON_DOWN; //Down arrow
			}
			else if (obj->Code == MC_NONE)
				drawEmptyBox = false;

			if (macroCount >= maxMacroDraw || obj->Code == MC_NONE)
				drawEmptyBox = false;
		}

		if (drawEmptyBox)
		{
			if (UO->GumpPixelsInXY(0x098D, posX, boxPosY))
				LSG = ID_GO_P5_EMPTY_BOX; //Empty macro box
		}

		if (m_MacroSelection)
		{
			bool isAction = false;
			if (m_MacroSelection == 0x20000000) //Action
			{
				posX += 116;
				isAction = true;
			}

			int textIndex = m_MacroListOffset;

			posY -= 60;
			posY += m_MacroListOffsetYStart;
				
			int ofs = 0;

			int count = m_MacroListMaxCount;

			if (count > m_MacroListCount)
				count = m_MacroListCount;

			IFOR(i, 0, count)
			{
				if (UO->GumpPixelsInXY(0x09B6 + ofs, posX, posY + (i * 15)))
					LSG = ID_GO_P5_SELECTION + textIndex + m_MacroListNameOffset;
					
				ofs = (ofs + 1) % 3;
				textIndex++;
			}
		}

		if (LSG != 0)
			g_LastSelectedObject = LSG;

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
int TGumpOptions::DrawPage6(bool &mode, DWORD &index, bool &IsPressed, int &CanSelectedButton, int &CanPressedButton, int &posX, int &posY)
{
	//Interface
	if (mode)
	{
		//UO->DrawUnicodeFont(0, L"Interface", g_OptionsTextColor, posX + 276, posY + 22);
		m_TexturePage6[0].Draw(posX + 84, posY + 22);
		
		//UO->DrawUnicodeFont(0, L"These options affect your interface.", g_OptionsTextColor, posX + 64, posY + 44, 480.0f);
		m_TexturePage6[1].Draw(posX + 64, posY + 44);
		
		posY += 90;
		UO->DrawGump(0x00D2 + (int)g_OptionsConfig.EnablePathfind, 0, posX + 64, posY);
		//UO->DrawUnicodeFont(0, L"Enable pathfinding with double-right-click", g_OptionsTextColor, posX + 86, posY);
		m_TexturePage6[2].Draw(posX + 86, posY);
		
		posY += 20;
		UO->DrawGump(0x00D2 + (int)g_OptionsConfig.HoldTabForCombat, 0, posX + 64, posY);
		//UO->DrawUnicodeFont(0, L"Hold down TAB key for combat, instead of tapping it to toggle combat mode", g_OptionsTextColor, posX + 86, posY, 450);
		m_TexturePage6[3].Draw(posX + 86, posY);
		
		posY += 36;
		UO->DrawGump(0x00D2 + (int)g_OptionsConfig.OffsetInterfaceWindows, 0, posX + 64, posY);
		//UO->DrawUnicodeFont(0, L"Offset interface windows rather than perfectly stacking them", g_OptionsTextColor, posX + 86, posY);
		m_TexturePage6[4].Draw(posX + 86, posY);
		
		posY += 20;
		UO->DrawGump(0x00D2 + (int)g_OptionsConfig.AutoArrange, 0, posX + 64, posY);
		//UO->DrawUnicodeFont(0, L"Automatically arrange minimized windows", g_OptionsTextColor, posX + 86, posY);
		m_TexturePage6[5].Draw(posX + 86, posY);
		
		posY += 20;
		UO->DrawGump(0x00D2 + (int)g_OptionsConfig.AlwaysRun, 0, posX + 64, posY);
		//UO->DrawUnicodeFont(0, L"Your character will always run if this is checked", g_OptionsTextColor, posX + 86, posY);
		m_TexturePage6[6].Draw(posX + 86, posY);
		
		posY += 20;
		UO->DrawGump(0x00D2 + (int)g_OptionsConfig.DisableMenubar, 0, posX + 64, posY);
		//UO->DrawUnicodeFont(0, L"Disable the Menu Bar", g_OptionsTextColor, posX + 86, posY);
		m_TexturePage6[7].Draw(posX + 86, posY);

		posY += 20;
		UO->DrawGump(0x00D2 + (int)g_OptionsConfig.GrayOutOfRangeObjects, 0, posX + 64, posY);
		//UO->DrawUnicodeFont(0, L"Gray out of range objects", g_OptionsTextColor, posX + 86, posY);
		m_TexturePage6[8].Draw(posX + 86, posY);

		posY += 20;
		UO->DrawGump(0x00D2 + (int)g_OptionsConfig.DisableNewTargetSystem, 0, posX + 64, posY);
		//UO->DrawUnicodeFont(0, L"Disable New Target System", g_OptionsTextColor, posX + 86, posY);
		m_TexturePage6[9].Draw(posX + 86, posY);

		posY += 20;
		UO->DrawGump(0x00D2 + (int)g_OptionsConfig.ObjectHandles, 0, posX + 64, posY);
		//UO->DrawUnicodeFont(0, L"Object Handles", g_OptionsTextColor, posX + 86, posY);
		m_TexturePage6[10].Draw(posX + 86, posY);
		
		posY += 20;
		UO->DrawGump(0x00D2 + (int)g_OptionsConfig.GetReduceFPSUnactiveWindow(), 0, posX + 64, posY);
		//UO->DrawUnicodeFont(0, L"Reduce FPS when Window is Unactive", g_OptionsTextColor, posX + 86, posY);
		m_TexturePage6[11].Draw(posX + 86, posY);

		posY += 20;
		UO->DrawGump(0x00D2 + (int)g_OptionsConfig.ItemPropertiesIcon, 0, posX + 64, posY);
		//UO->DrawUnicodeFont(0, L"Display Item Properties Icon", g_OptionsTextColor, posX + 86, posY);
		m_TexturePage6[12].Draw(posX + 86, posY);
	}
	else
	{
		if (g_LastSelectedGump != index)
			return 0;

		int LSG = 0;
		
		posX += 64;

		if (UO->GumpPixelsInXY(0x00D2, posX, posY + 90))
			LSG = ID_GO_P6_ENABLE_PATHFINDING; //Enable pathfinding with double-right-click
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 110))
			LSG = ID_GO_P6_HOLD_TAB_FOR_COMBAT; //Hold down TAB key for combat, instead of tapping it to toggle combat mode
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 146))
			LSG = ID_GO_P6_OFFSET_INTERFACE_WINDOWS; //Offset interface windows rather than perfectly stacking them
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 166))
			LSG = ID_GO_P6_AUTO_ARRANGE_MINIMIZED_WINDOWS; //Automatically arrange minimized windows
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 186))
			LSG = ID_GO_P6_ALWAYS_RUN; //Your character will always run if this is checked
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 206))
			LSG = ID_GO_P6_DISABLE_MENUBAR; //Disable the Menu Bar
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 226))
			LSG = ID_GO_P6_GRAY_OUT_OF_RANGE_OBJECTS; //Gray out of range objects
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 246))
			LSG = ID_GO_P6_DISABLE_NEW_TARGET_SYSTEM; //Disable New Target System
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 266))
			LSG = ID_GO_P6_OBJECT_HANDLES; //Object Handles
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 286))
			LSG = ID_GO_P6_REDUCE_FPS_UNACTIVE_WINDOW; //Reduce FPS when Window is Unactive
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 306))
			LSG = ID_GO_P6_DISPLAY_ITEM_PROPERTIES_ICON; //Display Item Properties Icon

		if (LSG != 0)
			g_LastSelectedObject = LSG;

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
int TGumpOptions::DrawPage7(bool &mode, DWORD &index, bool &IsPressed, int &CanSelectedButton, int &CanPressedButton, int &posX, int &posY)
{
	//Display
	if (mode)
	{
		if (IsPressed)
		{
			if (g_LastObjectLeftMouseDown == ID_GO_P7_AJUST_LONG_SPEECH) //Adjust how long speech remains on screen slider pressed
			{
				int currentX = g_MouseX - posX - 3; //Текущая позиция ползунка

				if (currentX < 64)
					currentX = 64; //Выход за допустимый предел, корректируем на минимум
				else if (currentX > 152)
					currentX = 152; //Выход за допустимый предел, корректируем на максимум
			
				int count = g_OptionsConfig.SpeechDelay; //Количество предметов в стеке
				if (currentX > 64) //Позиция ползунка больше минимума
				{
					if (currentX < 152) //Позиция ползунка меньше максимума, вычисляем
					{
						float ValPer = ((currentX - 64.0f) / g_SphereListWidth) * 100.0f;

						if (ValPer == 0.0f)
							count = 0;
						else
							count = (int)(999.0f * ValPer) / 100;

						if (count < 10)
							count = 10;
					}
					else
						count = 999; //Позиция равна максимуму, выставляем значение 255
				}
				else
					count = 10; //Позиция равна минимуму, выставляем значение 0

				g_OptionsConfig.SpeechDelay = count;
			}
		}

		//UO->DrawUnicodeFont(0, L"Display", g_OptionsTextColor, posX + 276, posY + 22);
		m_TexturePage7[0].Draw(posX + 84, posY + 22);
		
		//UO->DrawUnicodeFont(0, L"These options affect your display, and adjusting some of them may improve your graphics performance.", g_OptionsTextColor, posX + 64, posY + 44, 480.0f);
		m_TexturePage7[1].Draw(posX + 64, posY + 44);
		
		posY += 90;
		//UO->DrawUnicodeFont(0, L"Game Play Window Size", g_OptionsTextColor, posX + 64, posY);
		m_TexturePage7[2].Draw(posX + 64, posY);
		
		posY += 22;
		UO->DrawResizepicGump(0x0BB8, posX + 64, posY, 60, 22); //X field
		FontManager->DrawW(0, L"x", g_OptionsTextColor, posX + 126, posY);
		UO->DrawResizepicGump(0x0BB8, posX + 139, posY, 60, 22); //Y field
		
		posY += 2;

		wchar_t text[20] = {0};

		TextEntryGameSizeX->DrawW(0, g_OptionsTextColor, posX + 68, posY);

		TextEntryGameSizeY->DrawW(0, g_OptionsTextColor, posX + 143, posY);

		//UO->DrawUnicodeFont(0, L"Adjust how long speech remains on screen", g_OptionsTextColor, posX + 64, posY + 26);
		m_TexturePage7[3].Draw(posX + 64, posY + 26);
		
		posY += 46;
		UO->DrawGump(0x00D5, 0, posX + 64, posY); //Sphere line gump start
		UO->DrawGump(0x00D7, 0, posX + 152, posY); //Sphere line gump end
		UO->DrawGump(0x00D6, 0, posX + 77, posY, 75, 0); //Sphere line gump body

		float ValPer = (g_OptionsConfig.SpeechDelay / 999.0f) * 100.0f;
		ValPer = (g_SphereListWidth * ValPer) / 100.0f;
		if (ValPer < 0.0f)
			ValPer = 0.0f;
		UO->DrawGump(0x00D8, 0, posX + 64 + (int)ValPer, posY); //Sphere gump

		swprintf(text, L"%d", g_OptionsConfig.SpeechDelay);
		FontManager->DrawW(0, text, g_OptionsTextColor, posX + 176, posY - 3);
		
		posY += 22;
		UO->DrawGump(0x00D2 + (int)g_OptionsConfig.GetScaleSpeechDelay(), 0, posX + 64, posY);
		//UO->DrawUnicodeFont(0, L"Scale speech duration based on length", g_OptionsTextColor, posX + 86, posY);
		m_TexturePage7[4].Draw(posX + 86, posY);
		
		int tempY = posY;

		posY += 22;
		UO->DrawGump(0x00D4, 0, posX + 64, posY);
		g_GL.DrawPolygone(ColorManager->GetPolygoneColor(5, g_OptionsConfig.SpeechColor), posX + 67.0f, posY + 3.0f, 13.0f, 14.0f);
		//UO->DrawUnicodeFont(0, L"Speech color", g_OptionsTextColor, posX + 86, posY);
		m_TexturePage7[5].Draw(posX + 86, posY);
		
		posY += 19;
		UO->DrawGump(0x00D4, 0, posX + 64, posY);
		g_GL.DrawPolygone(ColorManager->GetPolygoneColor(5, g_OptionsConfig.EmoteColor), posX + 67.0f, posY + 3.0f, 13.0f, 14.0f);
		//UO->DrawUnicodeFont(0, L"Emote color", g_OptionsTextColor, posX + 86, posY);
		m_TexturePage7[6].Draw(posX + 86, posY);
		
		posY += 19;
		UO->DrawGump(0x00D4, 0, posX + 64, posY);
		g_GL.DrawPolygone(ColorManager->GetPolygoneColor(5, g_OptionsConfig.PartyMessageColor), posX + 67.0f, posY + 3.0f, 13.0f, 14.0f);
		//UO->DrawUnicodeFont(0, L"Party Message color", g_OptionsTextColor, posX + 86, posY);
		m_TexturePage7[7].Draw(posX + 86, posY);
		
		posY += 22;
		UO->DrawGump(0x00D2 + (int)g_OptionsConfig.DarkNights, 0, posX + 64, posY);
		//UO->DrawUnicodeFont(0, L"Dark Nights", g_OptionsTextColor, posX + 86, posY);
		m_TexturePage7[12].Draw(posX + 86, posY);

		posY += 20;
		UO->DrawGump(0x00D2 + (int)g_OptionsConfig.ColoredLighting, 0, posX + 64, posY);
		//UO->DrawUnicodeFont(0, L"Colored Lighting", g_OptionsTextColor, posX + 86, posY);
		m_TexturePage7[13].Draw(posX + 86, posY);
		
		posY += 20;
		UO->DrawGump(0x00D2 + (int)g_OptionsConfig.StandartCharactersAnimationDelay, 0, posX + 64, posY);
		//UO->DrawUnicodeFont(0, L"Standard characters animation delay", g_OptionsTextColor, posX + 86, posY);
		m_TexturePage7[14].Draw(posX + 86, posY);
		
		posY += 20;
		UO->DrawGump(0x00D2 + (int)g_OptionsConfig.StandartItemsAnimationDelay, 0, posX + 64, posY);
		//UO->DrawUnicodeFont(0, L"Standard items animation delay", g_OptionsTextColor, posX + 86, posY);
		m_TexturePage7[15].Draw(posX + 86, posY);

		posY += 20;
		UO->DrawGump(0x00D2 + (int)g_OptionsConfig.LockResizingGameWindow, 0, posX + 64, posY);
		//UO->DrawUnicodeFont(0, L"Lock game window resizing", g_OptionsTextColor, posX + 86, posY);
		m_TexturePage7[16].Draw(posX + 86, posY);

		posY += 20;
		UO->DrawGump(0x00D2 + (int)g_OptionsConfig.LockGumpsMoving, 0, posX + 64, posY);
		//UO->DrawUnicodeFont(0, L"Lock gumps moving", g_OptionsTextColor, posX + 86, posY);
		m_TexturePage7[17].Draw(posX + 86, posY);

		if (ConnectionManager.ClientVersion >= CV_6000)
		{
			posY = tempY;
			posX += 290;
		
			posY += 22;
			UO->DrawGump(0x00D4, 0, posX + 64, posY);
			g_GL.DrawPolygone(ColorManager->GetPolygoneColor(5, g_OptionsConfig.GuildMessageColor), posX + 67.0f, posY + 3.0f, 13.0f, 14.0f);
			//UO->DrawUnicodeFont(0, L"Guild Message Color", g_OptionsTextColor, posX + 86, posY);
			m_TexturePage7[8].Draw(posX + 86, posY);
		
			posY += 19;
			UO->DrawGump(0x00D4, 0, posX + 64, posY);
			g_GL.DrawPolygone(ColorManager->GetPolygoneColor(5, g_OptionsConfig.AllianceMessageColor), posX + 67.0f, posY + 3.0f, 13.0f, 14.0f);
			//UO->DrawUnicodeFont(0, L"Alliance Message Color", g_OptionsTextColor, posX + 86, posY);
			m_TexturePage7[9].Draw(posX + 86, posY);
			
			posY += 22;
			UO->DrawGump(0x00D2 + (int)g_OptionsConfig.IgnoreGuildMessage, 0, posX + 64, posY);
			//UO->DrawUnicodeFont(0, L"Ignore Guild Messages", g_OptionsTextColor, posX + 86, posY);
			m_TexturePage7[10].Draw(posX + 86, posY);

			posY += 20;
			UO->DrawGump(0x00D2 + (int)g_OptionsConfig.IgnoreAllianceMessage, 0, posX + 64, posY);
			//UO->DrawUnicodeFont(0, L"Ignore Alliance Messages", g_OptionsTextColor, posX + 86, posY);
			m_TexturePage7[11].Draw(posX + 86, posY);
		}
	}
	else
	{
		if (g_LastSelectedGump != index)
			return 0;

		int LSG = 0;
		
		posX += 64;

		if (UO->GumpPixelsInXY(0x00D2, posX, posY + 182))
			LSG = ID_GO_P7_SCALE_SPEECH_DURATION; //Scale speech duration based on length
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 204))
			LSG = ID_GO_P7_SPEECH_COLOR; //Speech color
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 223))
			LSG = ID_GO_P7_EMOTE_COLOR; //Emote color
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 242))
			LSG = ID_GO_P7_PARTY_MESSAGE_COLOR; //Party Message color
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 264))
			LSG = ID_GO_P7_DARK_NIGHTS; //Dark Nights
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 284))
			LSG = ID_GO_P7_COLORED_LIGHTING; //Colored Lighting
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 304))
			LSG = ID_GO_P7_CHARACTERS_ANIMATION_DELAY; //Standard characters animation delay
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 324))
			LSG = ID_GO_P7_ITEMS_ANIMATION_DELAY; //Standard items animation delay
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 344))
			LSG = ID_GO_P7_LOCK_GAME_WINDOW_RESIZING; //Lock game window resizing
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 364))
			LSG = ID_GO_P7_LOCK_GUMP_MOVING; //Lock gump moving
		else
		{
			RECT rc = {0, 0, (int)g_SphereListWidth + 10, 16};
			POINT p = {g_MouseX - posX, g_MouseY - (posY + 160)};
			if (PtInRect(&rc, p))
				LSG = ID_GO_P7_AJUST_LONG_SPEECH; //Adjust how long speech remains on screen
			else
			{
				RECT rcf = {0, 0, 60, 22};
				POINT pf = {g_MouseX - posX, g_MouseY - (posY + 112)};
				if (PtInRect(&rcf, pf))
					LSG = ID_GO_P7_GAME_WINDOW_SIZE_X; //Game Play Window Size X

				pf.x = (g_MouseX - (posX + 75));
				if (PtInRect(&rcf, pf))
					LSG = ID_GO_P7_GAME_WINDOW_SIZE_Y; //Game Play Window Size Y
			}
		}

		if (!LSG && ConnectionManager.ClientVersion >= CV_6000)
		{
			posX += 290;

			if (UO->GumpPixelsInXY(0x00D2, posX, posY + 204))
				LSG = ID_GO_P7_GUILD_MESSAGE_COLOR; //Guild Message Color
			else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 223))
				LSG = ID_GO_P7_ALLIANCE_MESSAGE_COLOR; //Alliance Message Color
			else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 245))
				LSG = ID_GO_P7_IGNORE_GUILD_MESSAGE; //Ignore Guild Messages
			else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 265))
				LSG = ID_GO_P7_IGNORE_ALLIANCE_MESSAGE; //Ignore Alliance Messages
		}

		if (LSG != 0)
			g_LastSelectedObject = LSG;

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
int TGumpOptions::DrawPage8(bool &mode, DWORD &index, bool &IsPressed, int &CanSelectedButton, int &CanPressedButton, int &posX, int &posY)
{
	//Reputation System

	const BYTE step = 19;

	if (mode)
	{
		//UO->DrawUnicodeFont(0, L"Reputation System", g_OptionsTextColor, posX + 246, posY + 22);
		m_TexturePage8[0].Draw(posX + 84, posY + 22);
		
		//UO->DrawUnicodeFont(0, L"These settings affect the reputation system, which is Ultima Online's system for controlling antisocial behavior.", g_OptionsTextColor, posX + 64, posY + 44, 480.0f);
		m_TexturePage8[1].Draw(posX + 64, posY + 44);
		
		posY += 90;
		UO->DrawGump(0x00D4, 0, posX + 64, posY);
		g_GL.DrawPolygone(ColorManager->GetPolygoneColor(5, g_OptionsConfig.InnocentColor), posX + 67.0f, posY + 3.0f, 13.0f, 14.0f);
		//UO->DrawUnicodeFont(0, L"Innocent highlight color", g_OptionsTextColor, posX + 88, posY);
		m_TexturePage8[2].Draw(posX + 88, posY);
		
		posY += step;
		UO->DrawGump(0x00D4, 0, posX + 64, posY);
		g_GL.DrawPolygone(ColorManager->GetPolygoneColor(5, g_OptionsConfig.FriendlyColor), posX + 67.0f, posY + 3.0f, 13.0f, 14.0f);
		//UO->DrawUnicodeFont(0, L"Friendly guilds highlight color", g_OptionsTextColor, posX + 88, posY);
		m_TexturePage8[3].Draw(posX + 88, posY);
		
		posY += step;
		UO->DrawGump(0x00D4, 0, posX + 64, posY);
		g_GL.DrawPolygone(ColorManager->GetPolygoneColor(5, g_OptionsConfig.SomeoneColor), posX + 67.0f, posY + 3.0f, 13.0f, 14.0f);
		//UO->DrawUnicodeFont(0, L"Someone that can be attacked color", g_OptionsTextColor, posX + 88, posY);
		m_TexturePage8[4].Draw(posX + 88, posY);
		
		posY += step;
		UO->DrawGump(0x00D4, 0, posX + 64, posY);
		g_GL.DrawPolygone(ColorManager->GetPolygoneColor(5, g_OptionsConfig.CriminalColor), posX + 67.0f, posY + 3.0f, 13.0f, 14.0f);
		//UO->DrawUnicodeFont(0, L"Criminal highlight color", g_OptionsTextColor, posX + 88, posY);
		m_TexturePage8[5].Draw(posX + 88, posY);
		
		posY += step;
		UO->DrawGump(0x00D4, 0, posX + 64, posY);
		g_GL.DrawPolygone(ColorManager->GetPolygoneColor(5, g_OptionsConfig.EnemyColor), posX + 67.0f, posY + 3.0f, 13.0f, 14.0f);
		//UO->DrawUnicodeFont(0, L"Enemy guildmembers color", g_OptionsTextColor, posX + 88, posY);
		m_TexturePage8[6].Draw(posX + 88, posY);
		
		posY += step;
		UO->DrawGump(0x00D4, 0, posX + 64, posY);
		g_GL.DrawPolygone(ColorManager->GetPolygoneColor(5, g_OptionsConfig.MurdererColor), posX + 67.0f, posY + 3.0f, 13.0f, 14.0f);
		//UO->DrawUnicodeFont(0, L"Murderer highlight color", g_OptionsTextColor, posX + 88, posY);
		m_TexturePage8[7].Draw(posX + 88, posY);
		
		posY += step;
		UO->DrawGump(0x00D2 + (int)g_OptionsConfig.CriminalActionsQuery, 0, posX + 64, posY);
		//UO->DrawUnicodeFont(0, L"Query before performing criminal actions", g_OptionsTextColor, posX + 88, posY);
		m_TexturePage8[8].Draw(posX + 88, posY);
	}
	else
	{
		if (g_LastSelectedGump != index)
			return 0;

		int LSG = 0;
		
		posX += 64;

		if (UO->GumpPixelsInXY(0x00D2, posX, posY + 90))
			LSG = ID_GO_P8_INNOCENT_COLOR; //Innocent highlight color
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 109))
			LSG = ID_GO_P8_FRIENDLY_COLOR; //Friendly guilds highlight color
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 128))
			LSG = ID_GO_P8_SOMEONE_COLOR; //Someone that can be attacked color
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 147))
			LSG = ID_GO_P8_CRIMINAL_COLOR; //Criminal highlight color
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 166))
			LSG = ID_GO_P8_ENEMY_COLOR; //Enemy guildmembers color
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 185))
			LSG = ID_GO_P8_MURDERER_COLOR; //Murderer highlight color
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 204))
			LSG = ID_GO_P8_QUERY_CRIMINAL_ACTIONS; //Query before performing criminal actions

		if (LSG != 0)
			g_LastSelectedObject = LSG;

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
int TGumpOptions::DrawPage9(bool &mode, DWORD &index, bool &IsPressed, int &CanSelectedButton, int &CanPressedButton, int &posX, int &posY)
{
	//Miscellaneous
	if (mode)
	{
		
		if (IsPressed)
		{
			if (g_LastObjectLeftMouseDown == ID_GO_P9_TRANSPARENCY_RADIUS) //Set radius of transparency circle slider pressed
			{
				int currentX = g_MouseX - posX - 3; //Текущая позиция ползунка

				if (currentX < 64)
					currentX = 64; //Выход за допустимый предел, корректируем на минимум
				else if (currentX > 152)
					currentX = 152; //Выход за допустимый предел, корректируем на максимум
			
				int count = g_OptionsConfig.CircleTransRadius; //Количество предметов в стеке
				if (currentX > 64) //Позиция ползунка больше минимума
				{
					if (currentX < 152) //Позиция ползунка меньше максимума, вычисляем
					{
						float ValPer = ((currentX - 64.0f) / g_SphereListWidth) * 100.0f;

						if (ValPer == 0.0f)
							count = 0;
						else
							count = (int)(200.0f * ValPer) / 100;
					}
					else
						count = 200; //Позиция равна максимуму, выставляем значение 255
				}
				else
					count = 0; //Позиция равна минимуму, выставляем значение 0

				g_OptionsConfig.CircleTransRadius = count;
			}
			else if (g_LastObjectLeftMouseDown == ID_GO_P9_INFORM_SKILLS) //Inform me when my skills improve this much (in tenths) slider pressed
			{
				int currentX = g_MouseX - posX - 3; //Текущая позиция ползунка

				if (currentX < 64)
					currentX = 64; //Выход за допустимый предел, корректируем на минимум
				else if (currentX > 152)
					currentX = 152; //Выход за допустимый предел, корректируем на максимум
			
				int count = g_OptionsConfig.SkillReport; //Количество предметов в стеке
				if (currentX > 64) //Позиция ползунка больше минимума
				{
					if (currentX < 152) //Позиция ползунка меньше максимума, вычисляем
					{
						float ValPer = ((currentX - 64.0f) / g_SphereListWidth) * 100.0f;

						if (ValPer == 0.0f)
							count = 0;
						else
							count = (int)(100.0f * ValPer) / 100;
					}
					else
						count = 100; //Позиция равна максимуму, выставляем значение 255
				}
				else
					count = 0; //Позиция равна минимуму, выставляем значение 0

				g_OptionsConfig.SkillReport = count;
			}
		}

		//UO->DrawUnicodeFont(0, L"Miscellaneous", g_OptionsTextColor, posX + 266, posY + 22);
		m_TexturePage9[0].Draw(posX + 84, posY + 22);
		
		//UO->DrawUnicodeFont(0, L"Miscellaneous options.", g_OptionsTextColor, posX + 64, posY + 44);
		m_TexturePage9[1].Draw(posX + 64, posY + 44);
		
		int posYStart = posY;

		posY += 90;
		UO->DrawGump(0x00D2 + (int)g_OptionsConfig.ShowIncomingNames, 0, posX + 64, posY);
		//UO->DrawUnicodeFont(0, L"Show Names of Approaching Players", g_OptionsTextColor, posX + 88, posY);
		m_TexturePage9[2].Draw(posX + 88, posY);

		posY += 20;
		UO->DrawGump(0x00D2 + (int)g_OptionsConfig.UseCircleTrans, 0, posX + 64, posY);
		//UO->DrawUnicodeFont(0, L"Use circle of transparency", g_OptionsTextColor, posX + 88, posY);
		m_TexturePage9[3].Draw(posX + 88, posY);
		
		//UO->DrawUnicodeFont(0, L"Set radius of transparency circle", g_OptionsTextColor, posX + 64, posY + 22);
		m_TexturePage9[4].Draw(posX + 64, posY + 22);
		
		posY += 43;
		UO->DrawGump(0x00D5, 0, posX + 64, posY); //Sphere line gump start
		UO->DrawGump(0x00D7, 0, posX + 152, posY); //Sphere line gump end
		UO->DrawGump(0x00D6, 0, posX + 77, posY, 75, 0); //Sphere line gump body

		float ValPer = (g_OptionsConfig.CircleTransRadius / 200.0f) * 100.0f;
		ValPer = (g_SphereListWidth * ValPer) / 100.0f;
		if (ValPer < 0.0f)
			ValPer = 0.0f;
		UO->DrawGump(0x00D8, 0, posX + 64 + (int)ValPer, posY); //Sphere gump

		wchar_t text[20] = {0};
		swprintf(text, L"%d", g_OptionsConfig.CircleTransRadius);
		FontManager->DrawW(0, text, g_OptionsTextColor, posX + 176, posY - 3);
		


		//UO->DrawUnicodeFont(0, L"Inform me when my skills improve this much (in tenths)", g_OptionsTextColor, posX + 64, posY + 22);
		m_TexturePage9[5].Draw(posX + 64, posY + 22);
		
		posY += 43;
		UO->DrawGump(0x00D5, 0, posX + 64, posY); //Sphere line gump start
		UO->DrawGump(0x00D7, 0, posX + 152, posY); //Sphere line gump end
		UO->DrawGump(0x00D6, 0, posX + 77, posY, 75, 0); //Sphere line gump body
		
		ValPer = (g_OptionsConfig.SkillReport / 100.0f) * 100.0f;
		ValPer = (g_SphereListWidth * ValPer) / 100.0f;
		if (ValPer < 0)
			ValPer = 0;
		UO->DrawGump(0x00D8, 0, posX + 64 + (int)ValPer, posY); //Sphere gump

		swprintf(text, L"%d", g_OptionsConfig.SkillReport);
		FontManager->DrawW(0, text, g_OptionsTextColor, posX + 176, posY - 3);
		
		

		posY += 20;
		UO->DrawGump(0x00D2 + (int)g_OptionsConfig.StatReport, 0, posX + 64, posY);
		//UO->DrawUnicodeFont(0, L"Inform me of increases in strength, dexterity, and intelligence.", g_OptionsTextColor, posX + 88, posY);
		m_TexturePage9[6].Draw(posX + 88, posY);

		posY += 20;
		UO->DrawGump(0x00D2 + (int)g_OptionsConfig.GetConsoleNeedEnter(), 0, posX + 64, posY);
		//UO->DrawUnicodeFont(0, L"Console need press 'Enter' to activate it.", g_OptionsTextColor, posX + 88, posY);
		m_TexturePage9[7].Draw(posX + 88, posY);
		



		posY += 26;
		UO->DrawGump(0x00D0, 0, posX + 64, posY);
		//UO->DrawUnicodeFont(0, L"Set the font for speech", g_OptionsTextColor, posX + 64, posY + 22);
		m_TexturePage9[8].Draw(posX + 88, posY);
	}
	else
	{
		if (g_LastSelectedGump != index)
			return 0;

		int LSG = 0;
		
		posX += 64;

		if (UO->GumpPixelsInXY(0x00D2, posX, posY + 90))
			LSG = ID_GO_P9_SHOW_APPROACHING_NAMES; //Show Names of Approaching Players
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 110))
			LSG = ID_GO_P9_USE_CIRCLE_OF_TRANSPARENCY; //Use circle of transparency
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 216))
			LSG = ID_GO_P9_INFORM_STATS; //Inform me of increases in strength, dexterity, and intelligence.
		else if (UO->GumpPixelsInXY(0x00D2, posX, posY + 236))
			LSG = ID_GO_P9_CONSOLE_ENTER; //Console need press 'Enter' to activate it.
		else if (UO->GumpPixelsInXY(0x00D0, posX, posY + 262))
			LSG = ID_GO_P9_SPEECH_FONT; //Set the font for speech
		else
		{
			RECT rc = {0, 0, (int)g_SphereListWidth + 10, 16};
			POINT p = {g_MouseX - posX, g_MouseY - (posY + 145)};
			if (PtInRect(&rc, p))
				LSG = ID_GO_P9_TRANSPARENCY_RADIUS; //Set radius of transparency circle

			p.y = g_MouseY - (posY + 188);
			if (PtInRect(&rc, p))
				LSG = ID_GO_P9_INFORM_SKILLS; //Inform me when my skills improve this much (in tenths)
		}

		if (LSG != 0)
			g_LastSelectedObject = LSG;

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
int TGumpOptions::DrawPage10(bool &mode, DWORD &index, bool &IsPressed, int &CanSelectedButton, int &CanPressedButton, int &posX, int &posY)
{
	//Filter Options
	if (mode)
	{
	}
	else
	{
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpOptions::OnLeftMouseDown()
{
	if (g_LastSelectedObject == ID_GO_P5_BUTTON_PREVEOUS) //Preveous button
	{
		if (m_MacroPointer->m_Prev != NULL)
		{
			m_MacroPointer = (TMacro*)m_MacroPointer->m_Prev;
			m_MacroObjectPointer = (TMacroObject*)m_MacroPointer->m_Items;

			TextEntryMacro->SetText(m_HotkeyText[m_MacroPointer->Key & 0xFF]);
		}

		m_LastChangeMacroTime = GetTickCount() + CHANGE_MACRO_DELAY;
	}
	else if (g_LastSelectedObject == ID_GO_P5_BUTTON_NEXT) //Next button
	{
		if (m_MacroPointer->m_Next != NULL)
		{
			m_MacroPointer = (TMacro*)m_MacroPointer->m_Next;
			m_MacroObjectPointer = (TMacroObject*)m_MacroPointer->m_Items;

			TextEntryMacro->SetText(m_HotkeyText[m_MacroPointer->Key & 0xFF]);
		}

		m_LastChangeMacroTime = GetTickCount() + CHANGE_MACRO_DELAY;
	}
	else if (g_LastSelectedObject >= ID_GO_P5_LEFT_BOX && g_LastSelectedObject < ID_GO_P5_RIGHT_BOX) //Left action box
	{
		m_MacroElement = g_LastSelectedObject - ID_GO_P5_LEFT_BOX;
		m_MacroSelection = 0x10000000;
		m_MacroListOffset = 0;
		m_MacroListCount = 60;
		m_MacroListNameOffset = 0;
		m_MacroListOffsetYStart = 92;
		m_MacroListOffsetYEnd = 392;
	}
	else if (g_LastSelectedObject >= ID_GO_P5_RIGHT_BOX && g_LastSelectedObject < ID_GO_P5_EMPTY_BOX) //Right action box
	{
		m_MacroElement = g_LastSelectedObject - ID_GO_P5_RIGHT_BOX;
		
		TMacroObject *obj = m_MacroObjectPointer;
					
		IFOR(i, 0, (int)m_MacroElement)
		{
			if (obj->m_Next == NULL)
				break;

			obj = (TMacroObject*)obj->m_Next;
		}
		
		m_MacroListCount = 0;
		m_MacroListOffset = 0;
		m_MacroListNameOffset = 0;

		TMacro::GetBoundByCode(obj->Code, m_MacroListCount, m_MacroListNameOffset);

		if (m_MacroListCount)
		{
			m_MacroSelection = 0x20000000;

			if (m_MacroListCount >= m_MacroListMaxCount)
			{
				m_MacroListOffsetYStart = 92;
				m_MacroListOffsetYEnd = 392;
			}
			else
			{
				int startY = 187 + (m_MacroElement * 26) + 26;

				int heightWindow = m_MacroListCount * 15;

				if (heightWindow + startY > 392)
				{
					m_MacroListOffsetYStart = 392 - heightWindow;
					m_MacroListOffsetYEnd = 392;
				}
				else
				{
					m_MacroListOffsetYStart = startY;
					m_MacroListOffsetYEnd = startY + heightWindow;
				}
			}
		}
		else
		{
			m_MacroSelection = 0;
			m_MacroListOffsetYStart = 0;
			m_MacroListOffsetYEnd = 0;

			if (obj->HasSubMenu == 2)
			{
				int x = g_MouseX - (X + 110);
				int y = 0; //g_MouseY - (Y + 57);

				((TMacroObjectEntry*)obj)->TextEntry->OnClick(this, 1, false, x, y, TS_LEFT, UOFONT_FIXED);
			}
		}
	}
	else if (g_LastSelectedObject == ID_GO_P5_EMPTY_BOX) //Empty left action box
	{
		m_MacroSelection = 0x40000000;
		m_MacroListOffset = 0;
		m_MacroListCount = 60;
		m_MacroListNameOffset = 0;
		m_MacroListOffsetYStart = 92;
		m_MacroListOffsetYEnd = 392;
	}
}
//----------------------------------------------------------------------------
void TGumpOptions::OnLeftMouseUp()
{
	if (g_LastObjectLeftMouseDown != g_LastSelectedObject)
	{
		if (m_Page == 4 && g_LastSelectedObject >= ID_GO_P5_SELECTION)
			{}
		else
		{
			if (m_MacroSelection != 0)
				m_MacroSelection = 0;

			return;
		}
	}

	if (g_LastObjectLeftMouseDown == ID_GO_PAGE_0)
		m_Page = 0;
	else if (g_LastObjectLeftMouseDown == ID_GO_PAGE_1)
		m_Page = 1;
	else if (g_LastObjectLeftMouseDown == ID_GO_PAGE_2)
		m_Page = 2;
	else if (g_LastObjectLeftMouseDown == ID_GO_PAGE_3)
		m_Page = 3;
	else if (g_LastObjectLeftMouseDown == ID_GO_PAGE_4)
		m_Page = 4;
	else if (g_LastObjectLeftMouseDown == ID_GO_PAGE_5)
		m_Page = 5;
	else if (g_LastObjectLeftMouseDown == ID_GO_PAGE_6)
	{
		m_Page = 6;

		char buf[20] = {0};
		sprintf(buf, "%d", g_GameWindowWidth);
		TextEntryGameSizeX->SetText(buf);

		sprintf(buf, "%d", g_GameWindowHeight);
		TextEntryGameSizeY->SetText(buf);
	}
	else if (g_LastObjectLeftMouseDown == ID_GO_PAGE_7)
		m_Page = 7;
	else if (g_LastObjectLeftMouseDown == ID_GO_PAGE_8)
		m_Page = 8;
	else if (g_LastObjectLeftMouseDown == ID_GO_PAGE_9)
		m_Page = 9;
	else if (g_LastObjectLeftMouseDown == ID_GO_CANCEL) //Cancel
	{
		GumpManager->CloseGump(g_PlayerSerial, 0, GT_OPTIONS);

		OptionsMacroManager->Clear();
	}
	else if (g_LastObjectLeftMouseDown == ID_GO_APPLY) //Apply
		ApplyPageChanges();
	else if (g_LastObjectLeftMouseDown == ID_GO_DEFAULT) //Default
	{
		switch (m_Page)
		{
			case 0:
			{
				g_OptionsConfig.DefaultPage1();
				break;
			}
			case 1:
			{
				g_OptionsConfig.DefaultPage2();
				break;
			}
			case 2:
			{
				g_OptionsConfig.DefaultPage3();
				break;
			}
			case 3:
			{
				g_OptionsConfig.DefaultPage4();
				break;
			}
			case 5:
			{
				g_OptionsConfig.DefaultPage6();
				break;
			}
			case 6:
			{
				g_OptionsConfig.DefaultPage7();
				break;
			}
			case 7:
			{
				g_OptionsConfig.DefaultPage8();
				break;
			}
			case 8:
			{
				g_OptionsConfig.DefaultPage9();
				break;
			}
			default:
				break;
		}
	}
	else if (g_LastObjectLeftMouseDown == ID_GO_OKAY) //Okay
	{
		/*IFOR(i, 0, 10)
		{
			m_Page = i;
			ApplyPageChanges();
		}*/
		ApplyPageChanges();
		GumpManager->CloseGump(g_PlayerSerial, 0, GT_OPTIONS);

		OptionsMacroManager->Clear();

		return;
	}
	else
	{
		switch (m_Page)
		{
			case 0: //Sound and Music
			{
				if (g_LastObjectLeftMouseDown == ID_GO_P1_SOUND_ON_OFF) //Sound on/off
					g_OptionsConfig.Sound = !g_OptionsConfig.Sound;
				else if (g_LastObjectLeftMouseDown == ID_GO_P1_MUSIC_ON_OFF) //Music on/off
					g_OptionsConfig.Music = !g_OptionsConfig.Music;
				else if (g_LastObjectLeftMouseDown == ID_GO_P1_PLAY_FOOTSTEP_SOUNDS) //Play footstep sounds
					g_OptionsConfig.FootstepsSound = !g_OptionsConfig.FootstepsSound;
				else if (g_LastObjectLeftMouseDown == ID_GO_P1_PLAY_COMBAT_MUSIC) //Play combat music
					g_OptionsConfig.CombatMusic = !g_OptionsConfig.CombatMusic;

				break;
			}
			case 1: //Pop-up Help
			{
				if (g_LastObjectLeftMouseDown == ID_GO_P2_USE_POPUP) //Use Pop-up Help
					g_OptionsConfig.PopupHelpEnabled = !g_OptionsConfig.PopupHelpEnabled;
				else if (g_LastObjectLeftMouseDown == ID_GO_P2_TEXT_COLOR) //Text Color
				{
					TGumpSelectColor *gump = new TGumpSelectColor(g_PlayerSerial, 100, 100, SCGS_OPT_POPUP_TEXT);

					GumpManager->AddGump(gump);
				}
				else if (g_LastObjectLeftMouseDown == ID_GO_P2_TEXT_FONT) //Font
				{
					TGumpSelectFont*gump = new TGumpSelectFont(g_PlayerSerial, 320, 240, SFGS_OPT_POPUP);

					GumpManager->AddGump(gump);
				}
				else if (g_LastObjectLeftMouseDown == ID_GO_P2_MAKE_WINDOW_STICKY) //Make window Sticky
					g_OptionsConfig.PopupHelpSticky = !g_OptionsConfig.PopupHelpSticky;
				else if (g_LastObjectLeftMouseDown == ID_GO_P2_MAKE_TEXT_PERTISTANT) //Make text Persistant
					g_OptionsConfig.PopupHelpPersistant = !g_OptionsConfig.PopupHelpPersistant;

				break;
			}
			case 2: //Language
			{
				if (g_LastObjectLeftMouseDown == ID_GO_P3_USE_TOOLTIP) //Use Tool-tips
					g_OptionsConfig.UseToolTips = !g_OptionsConfig.UseToolTips;
				else if (g_LastObjectLeftMouseDown == ID_GO_P3_TEXT_COLOR) //Color of Tool-tip text
				{
					TGumpSelectColor *gump = new TGumpSelectColor(g_PlayerSerial, 100, 100, SCGS_OPT_TOOLTIP_TEXT);

					GumpManager->AddGump(gump);
				}
				else if (g_LastObjectLeftMouseDown == ID_GO_P3_TEXT_FONT) //Font
				{
					TGumpSelectFont *gump = new TGumpSelectFont(g_PlayerSerial, 320, 240, SFGS_OPT_TOOLTIP);

					GumpManager->AddGump(gump);
				}

				break;
			}
			case 3: //Chat
			{
				if (g_LastObjectLeftMouseDown >= ID_GO_P4_TEXT_COLOR) //Input text color
				{
					TGumpSelectColor *gump = new TGumpSelectColor(g_PlayerSerial, 100, 100, (SELECT_COLOR_GUMP_STATE)(SCGS_OPT_CHAT_INPUT_TEXT + (g_LastObjectLeftMouseDown - ID_GO_P4_TEXT_COLOR)));

					GumpManager->AddGump(gump);
				}
				else if (g_LastObjectLeftMouseDown == ID_GO_P4_TEXT_FONT) //Font
				{
					TGumpSelectFont *gump = new TGumpSelectFont(g_PlayerSerial, 320, 240, SFGS_OPT_CHAT);

					GumpManager->AddGump(gump);
				}

				break;
			}
			case 4: //Macro Options
			{
				if (g_LastObjectLeftMouseDown == ID_GO_P5_BUTTON_ADD) //Add button
				{
					m_MacroPointer = TMacro::CreateBlankMacro();
					m_MacroObjectPointer = (TMacroObject*)m_MacroPointer->m_Items;
					OptionsMacroManager->Add(m_MacroPointer);

					TextEntryMacro->SetText(m_HotkeyText[m_MacroPointer->Key & 0xFF]);
				}
				else if (g_LastObjectLeftMouseDown == ID_GO_P5_BUTTON_DELETE) //Delete button
				{
					TMacro *newpointer = (TMacro*)m_MacroPointer->m_Next;
					if (newpointer == NULL)
						newpointer = (TMacro*)m_MacroPointer->m_Prev;

					OptionsMacroManager->Delete(m_MacroPointer);

					if (newpointer == NULL)
					{
						newpointer = TMacro::CreateBlankMacro();
						OptionsMacroManager->Add(newpointer);
					}

					m_MacroPointer = newpointer;
					m_MacroObjectPointer = (TMacroObject*)m_MacroPointer->m_Items;

					TextEntryMacro->SetText(m_HotkeyText[m_MacroPointer->Key & 0xFF]);
				}
				/*else if (g_LastObjectLeftMouseDown == ID_GO_P5_BUTTON_PREVEOUS) //Preveous button
				{
					if (m_MacroPointer->m_Prev != NULL)
					{
						m_MacroPointer = (TMacro*)m_MacroPointer->m_Prev;
						m_MacroObjectPointer = (TMacroObject*)m_MacroPointer->m_Items;
					}
				}
				else if (g_LastObjectLeftMouseDown == ID_GO_P5_BUTTON_NEXT) //Next button
				{
					if (m_MacroPointer->m_Next != NULL)
					{
						m_MacroPointer = (TMacro*)m_MacroPointer->m_Next;
						m_MacroObjectPointer = (TMacroObject*)m_MacroPointer->m_Items;
					}
				}*/
				else if (g_LastObjectLeftMouseDown == ID_GO_P5_KEY_BOX) //Key box
				{
					int x = g_MouseX - (X + 137);
					int y = g_MouseY - (Y + 57);

					TextEntryMacro->OnClick(this, 1, false, x, y);
				}
				else if (g_LastObjectLeftMouseDown == ID_GO_P5_BUTTON_SHIFT) //Shift checkbox
					m_MacroPointer->Shift = !m_MacroPointer->Shift;
				else if (g_LastObjectLeftMouseDown == ID_GO_P5_BUTTON_ALT) //Alt checkbox
					m_MacroPointer->Alt = !m_MacroPointer->Alt;
				else if (g_LastObjectLeftMouseDown == ID_GO_P5_BUTTON_CTRL) //Ctrl checkbox
					m_MacroPointer->Ctrl = !m_MacroPointer->Ctrl;
				else if (g_LastObjectLeftMouseDown == ID_GO_P5_BUTTON_UP) //Up button
				{
					if (m_MacroObjectPointer->m_Prev != NULL)
						m_MacroObjectPointer = (TMacroObject*)m_MacroObjectPointer->m_Prev;
				}
				else if (g_LastObjectLeftMouseDown == ID_GO_P5_BUTTON_DOWN) //Down button
				{
					if (m_MacroObjectPointer->m_Next != NULL)
						m_MacroObjectPointer = (TMacroObject*)m_MacroObjectPointer->m_Next;
				}
				else if (g_LastSelectedObject >= ID_GO_P5_SELECTION) //Action selection
				{
					DWORD index = g_LastSelectedObject - ID_GO_P5_SELECTION;

					switch (m_MacroSelection)
					{
						case 0x10000000:
						case 0x40000000:
						{
							TMacroObject *obj = m_MacroObjectPointer;

							if (m_MacroSelection == 0x40000000)
							{
								obj = TMacro::CreateMacro((MACRO_CODE)index);
								m_MacroPointer->Add(obj);
							}
							else
							{
								IFOR(i, 0, (int)m_MacroElement)
								{
									if (obj->m_Next == NULL)
									{
										obj = TMacro::CreateMacro((MACRO_CODE)index);
										m_MacroPointer->Add(obj);

										break;
									}

									obj = (TMacroObject*)obj->m_Next;
								}
							}
							
							if (obj->Code != (MACRO_CODE)index)
							{
								TMacroObject *newobj = TMacro::CreateMacro((MACRO_CODE)index);

								if (obj == m_MacroObjectPointer)
									m_MacroObjectPointer = newobj;
								
								m_MacroPointer->ChangeObject(obj, newobj);
							}

							break;
						}
						case 0x20000000:
						{
							TMacroObject *obj = m_MacroObjectPointer;
							
							IFOR(i, 0, (int)m_MacroElement)
							{
								if (obj->m_Next == NULL)
									break;

								obj = (TMacroObject*)obj->m_Next;
							}

							obj->SubCode = (MACRO_SUB_CODE)index;

							break;
						}
						default:
							break;
					}
				}

				break;
			}
			case 5: //Interface
			{
				if (g_LastObjectLeftMouseDown == ID_GO_P6_ENABLE_PATHFINDING) //Enable pathfinding with double-right-click
					g_OptionsConfig.EnablePathfind = !g_OptionsConfig.EnablePathfind;
				else if (g_LastObjectLeftMouseDown == ID_GO_P6_HOLD_TAB_FOR_COMBAT) //Hold down TAB key for combat, instead of tapping it to toggle combat mode
					g_OptionsConfig.HoldTabForCombat = !g_OptionsConfig.HoldTabForCombat;
				else if (g_LastObjectLeftMouseDown == ID_GO_P6_OFFSET_INTERFACE_WINDOWS) //Offset interface windows rather than perfectly stacking them
					g_OptionsConfig.OffsetInterfaceWindows = !g_OptionsConfig.OffsetInterfaceWindows;
				else if (g_LastObjectLeftMouseDown == ID_GO_P6_AUTO_ARRANGE_MINIMIZED_WINDOWS) //Automatically arrange minimized windows
					g_OptionsConfig.AutoArrange = !g_OptionsConfig.AutoArrange;
				else if (g_LastObjectLeftMouseDown == ID_GO_P6_ALWAYS_RUN) //Your character will always run if this is checked
					g_OptionsConfig.AlwaysRun = !g_OptionsConfig.AlwaysRun;
				else if (g_LastObjectLeftMouseDown == ID_GO_P6_DISABLE_MENUBAR) //Disable the Menu Bar
					g_OptionsConfig.DisableMenubar = !g_OptionsConfig.DisableMenubar;
				else if (g_LastObjectLeftMouseDown == ID_GO_P6_GRAY_OUT_OF_RANGE_OBJECTS) //Gray out of range objects
					g_OptionsConfig.GrayOutOfRangeObjects = !g_OptionsConfig.GrayOutOfRangeObjects;
				else if (g_LastObjectLeftMouseDown == ID_GO_P6_DISABLE_NEW_TARGET_SYSTEM) //Disable New Target System
					g_OptionsConfig.DisableNewTargetSystem = !g_OptionsConfig.DisableNewTargetSystem;
				else if (g_LastObjectLeftMouseDown == ID_GO_P6_OBJECT_HANDLES) //Object Handles
					g_OptionsConfig.ObjectHandles = !g_OptionsConfig.ObjectHandles;
				else if (g_LastObjectLeftMouseDown == ID_GO_P6_REDUCE_FPS_UNACTIVE_WINDOW) //Reduce FPS when Window is Unactive
					g_OptionsConfig.SetReduceFPSUnactiveWindow(!g_OptionsConfig.GetReduceFPSUnactiveWindow());
				else if (g_LastObjectLeftMouseDown == ID_GO_P6_DISPLAY_ITEM_PROPERTIES_ICON) //Display Item Properties Icon
					g_OptionsConfig.ItemPropertiesIcon = !g_OptionsConfig.ItemPropertiesIcon;

				break;
			}
			case 6: //Display
			{
				if (g_LastObjectLeftMouseDown == ID_GO_P7_SCALE_SPEECH_DURATION) //Scale speech duration based on length
					g_OptionsConfig.ScaleSpeechDelay = !g_OptionsConfig.ScaleSpeechDelay;
				else if (g_LastObjectLeftMouseDown >= ID_GO_P7_SPEECH_COLOR && g_LastObjectLeftMouseDown <= ID_GO_P7_ALLIANCE_MESSAGE_COLOR) //Speech color
				{
					TGumpSelectColor *gump = new TGumpSelectColor(g_PlayerSerial, 100, 100, (SELECT_COLOR_GUMP_STATE)(SCGS_OPT_DISPLAY_SPEECH + (g_LastObjectLeftMouseDown - ID_GO_P7_SPEECH_COLOR)));

					GumpManager->AddGump(gump);
				}
				else if (g_LastObjectLeftMouseDown == ID_GO_P7_IGNORE_GUILD_MESSAGE) //Ignore Guild Messages
					g_OptionsConfig.IgnoreGuildMessage = !g_OptionsConfig.IgnoreGuildMessage;
				else if (g_LastObjectLeftMouseDown == ID_GO_P7_IGNORE_ALLIANCE_MESSAGE) //Ignore Alliance Messages
					g_OptionsConfig.IgnoreAllianceMessage = !g_OptionsConfig.IgnoreAllianceMessage;
				else if (g_LastObjectLeftMouseDown == ID_GO_P7_DARK_NIGHTS) //Dark Nights
					g_OptionsConfig.DarkNights = !g_OptionsConfig.DarkNights;
				else if (g_LastObjectLeftMouseDown == ID_GO_P7_COLORED_LIGHTING) //Colored Lighting
					g_OptionsConfig.ColoredLighting = !g_OptionsConfig.ColoredLighting;
				else if (g_LastObjectLeftMouseDown == ID_GO_P7_GAME_WINDOW_SIZE_X) //Game Play Window Size X
				{
					int x = g_MouseX - (X + 68);
					int y = g_MouseY - Y;

					TextEntryGameSizeX->OnClick(this, 0, true, x, y);
				}
				else if (g_LastObjectLeftMouseDown == ID_GO_P7_GAME_WINDOW_SIZE_Y) //Game Play Window Size Y
				{
					int x = g_MouseX - (X + 143);
					int y = g_MouseY - Y;

					TextEntryGameSizeY->OnClick(this, 0, true, x, y);
				}
				else if (g_LastObjectLeftMouseDown == ID_GO_P7_CHARACTERS_ANIMATION_DELAY) //Standard characters animation delay
					g_OptionsConfig.StandartCharactersAnimationDelay = !g_OptionsConfig.StandartCharactersAnimationDelay;
				else if (g_LastObjectLeftMouseDown == ID_GO_P7_ITEMS_ANIMATION_DELAY) //Standard items animation delay
					g_OptionsConfig.StandartItemsAnimationDelay = !g_OptionsConfig.StandartItemsAnimationDelay;
				else if (g_LastObjectLeftMouseDown == ID_GO_P7_LOCK_GAME_WINDOW_RESIZING) //Lock game window resizing
					g_OptionsConfig.LockResizingGameWindow = !g_OptionsConfig.LockResizingGameWindow;
				else if (g_LastObjectLeftMouseDown == ID_GO_P7_LOCK_GUMP_MOVING) //Lock gump moving
					g_OptionsConfig.LockGumpsMoving = !g_OptionsConfig.LockGumpsMoving;

				break;
			}
			case 7: //Reputation System
			{
				if (g_LastObjectLeftMouseDown >= ID_GO_P8_INNOCENT_COLOR && g_LastObjectLeftMouseDown <= ID_GO_P8_MURDERER_COLOR) //Innocent highlight color
				{
					/*if (g_LastObjectLeftMouseDown == 100) //Innocent highlight color
					else if (g_LastObjectLeftMouseDown == 101) //Friendly guilds highlight color
					else if (g_LastObjectLeftMouseDown == 102) //Someone that can be attacked color
					else if (g_LastObjectLeftMouseDown == 103) //Criminal highlight color
					else if (g_LastObjectLeftMouseDown == 104) //Enemy guildmembers color
					else if (g_LastObjectLeftMouseDown == 105) //Murderer highlight color*/
					
					TGumpSelectColor *gump = new TGumpSelectColor(g_PlayerSerial, 100, 100, (SELECT_COLOR_GUMP_STATE)(SCGS_OPT_REPSYS_INNOCENT + (g_LastObjectLeftMouseDown - ID_GO_P8_INNOCENT_COLOR)));

					GumpManager->AddGump(gump);
				}
				else if (g_LastObjectLeftMouseDown == ID_GO_P8_QUERY_CRIMINAL_ACTIONS) //Query before performing criminal actions
					g_OptionsConfig.CriminalActionsQuery = !g_OptionsConfig.CriminalActionsQuery;

				break;
			}
			case 8: //Miscellaneous
			{
				if (g_LastObjectLeftMouseDown == ID_GO_P9_SHOW_APPROACHING_NAMES) //Show Names of Approaching Players
					g_OptionsConfig.ShowIncomingNames = !g_OptionsConfig.ShowIncomingNames;
				else if (g_LastObjectLeftMouseDown == ID_GO_P9_USE_CIRCLE_OF_TRANSPARENCY) //Use circle of transparency
					g_OptionsConfig.UseCircleTrans = !g_OptionsConfig.UseCircleTrans;
				else if (g_LastObjectLeftMouseDown == ID_GO_P9_INFORM_STATS) //Inform me of increases in strength, dexterity, and intelligence.
					g_OptionsConfig.StatReport = !g_OptionsConfig.StatReport;
				else if (g_LastObjectLeftMouseDown == ID_GO_P9_CONSOLE_ENTER) //Console need press 'Enter' to activate it.
					g_OptionsConfig.SetConsoleNeedEnter(!g_OptionsConfig.GetConsoleNeedEnter());
				else if (g_LastObjectLeftMouseDown == ID_GO_P9_SPEECH_FONT) //Speech Font
				{
					TGumpSelectFont *gump = new TGumpSelectFont(g_PlayerSerial, 320, 240, SFGS_OPT_MISCELLANEOUS);

					GumpManager->AddGump(gump);
				}

				break;
			}
			case 9: //Filter Options
			{
				break;
			}
			default:
				break;
		}
	}

	if (m_MacroSelection != 0)
		m_MacroSelection = 0;
}
//----------------------------------------------------------------------------
void TGumpOptions::OnCharPress(WPARAM &wparam, LPARAM &lparam)
{
	if (wparam == VK_RETURN || wparam == VK_BACK || wparam == VK_ESCAPE || EntryPointer == NULL)
		return; //Ignore no print keys

	if (EntryPointer == TextEntryGameSizeX || EntryPointer == TextEntryGameSizeY)
	{
		if (wparam >= '0' && wparam <= '9')
		{
			EntryPointer->Insert(wparam);

			int val = atoi(EntryPointer->c_str());
			if (val > 2000)
				EntryPointer->Remove(true);
			else
			{
				GenerateFrame(X, Y);
			}
		}
	}
	else if (EntryPointer != TextEntryMacro)
	{
		TMacroObject *obj = m_MacroPointer->EntryPointerHere();

		if (obj != NULL)
		{
			char canAdd = 0;

			switch (obj->Code)
			{
				case MC_DELAY:
				case MC_SET_UPDATE_RANGE:
				case MC_MODIFY_UPDATE_RANGE:
				{
					if (wparam >= '0' && wparam <= '9')
						canAdd = 1;

					break;
				}
				case MC_SAY:
				case MC_EMOTE:
				case MC_WHISPER:
				case MC_YELL:
				{
					canAdd = 1;

					break;
				}
				default:
					break;
			}

			if (canAdd)
			{
				EntryPointer->Insert(wparam);
				GenerateFrame(X, Y);
			}
		}
	}
}
//----------------------------------------------------------------------------
void TGumpOptions::OnKeyPress(WPARAM &wparam, LPARAM &lparam)
{
	if (EntryPointer == TextEntryMacro)
	{
		m_MacroPointer->Key = wparam & 0xFF;

		TextEntryMacro->SetText(m_HotkeyText[wparam & 0xFF]);

		GenerateFrame(X, Y);
	}
	else
	{
		if (wparam == VK_RETURN)
		{
			if (ConfigManager.GetConsoleNeedEnter())
				EntryPointer = NULL;
			else
				EntryPointer = GameConsole;

			GenerateFrame(X, Y);
		}
		else
			EntryPointer->OnKey(this, wparam);
	}
}
//----------------------------------------------------------------------------
void TGumpOptions::ApplyPageChanges()
{
	switch (m_Page)
	{
		case 0: //Sound and Music
		{
			ConfigManager.Sound = g_OptionsConfig.Sound;
			ConfigManager.Music = g_OptionsConfig.Music;
			ConfigManager.FootstepsSound = g_OptionsConfig.FootstepsSound;
			ConfigManager.CombatMusic = g_OptionsConfig.CombatMusic;
			ConfigManager.SoundVolume = g_OptionsConfig.SoundVolume;
			ConfigManager.MusicVolume = g_OptionsConfig.MusicVolume;

			SoundManager.SetMusicVolume(ConfigManager.MusicVolume);

			if (!ConfigManager.Sound)
				UO->ResetSoundEffects(GetTickCount() + 100000);

			if (!ConfigManager.Music)
				SoundManager.StopMusic();

			break;
		}
		case 1: //Pop-up Help
		{
			ConfigManager.PopupHelpEnabled = g_OptionsConfig.PopupHelpEnabled;
			ConfigManager.PopupHelpTextFont = g_OptionsConfig.PopupHelpTextFont;
			ConfigManager.PopupHelpTextColor = g_OptionsConfig.PopupHelpTextColor;
			ConfigManager.PopupHelpSticky = g_OptionsConfig.PopupHelpSticky;
			ConfigManager.PopupHelpPersistant = g_OptionsConfig.PopupHelpPersistant;

			break;
		}
		case 2: //Language
		{
			ConfigManager.UseToolTips = g_OptionsConfig.UseToolTips;
			ConfigManager.UseToolTips = g_OptionsConfig.UseToolTips;
			ConfigManager.ToolTipsTextColor = g_OptionsConfig.ToolTipsTextColor;
			ConfigManager.ToolTipsTextFont = g_OptionsConfig.ToolTipsTextFont;
			ConfigManager.ToolTipsDelay = g_OptionsConfig.ToolTipsDelay;

			break;
		}
		case 3: //Chat
		{
			ConfigManager.ChatColorInputText = g_OptionsConfig.ChatColorInputText;
			ConfigManager.ChatColorMenuOption = g_OptionsConfig.ChatColorMenuOption;
			ConfigManager.ChatColorPlayerInMemberList = g_OptionsConfig.ChatColorPlayerInMemberList;
			ConfigManager.ChatColorText = g_OptionsConfig.ChatColorText;
			ConfigManager.ChatColorPlayerNameWithout = g_OptionsConfig.ChatColorPlayerNameWithout;
			ConfigManager.ChatColorMuted = g_OptionsConfig.ChatColorMuted;
			ConfigManager.ChatColorChannelModeratorName = g_OptionsConfig.ChatColorChannelModeratorName;
			ConfigManager.ChatColorChannelModeratorText = g_OptionsConfig.ChatColorChannelModeratorText;
			ConfigManager.ChatColorMyName = g_OptionsConfig.ChatColorMyName;
			ConfigManager.ChatColorMyText = g_OptionsConfig.ChatColorMyText;
			ConfigManager.ChatColorSystemMessage = g_OptionsConfig.ChatColorSystemMessage;
			ConfigManager.ChatColorBGOutputText = g_OptionsConfig.ChatColorBGOutputText;
			ConfigManager.ChatColorBGInputText = g_OptionsConfig.ChatColorBGInputText;
			ConfigManager.ChatColorBGUserList = g_OptionsConfig.ChatColorBGUserList;
			ConfigManager.ChatColorBGConfList = g_OptionsConfig.ChatColorBGConfList;
			ConfigManager.ChatColorBGCommandList = g_OptionsConfig.ChatColorBGCommandList;
			ConfigManager.ChatFont = g_OptionsConfig.ChatFont;

			break;
		}
		case 4: //Macro Options
		{
			MacroManager->LoadFromOptions();
			break;
		}
		case 5: //Interface
		{
			ConfigManager.EnablePathfind = g_OptionsConfig.EnablePathfind;
			ConfigManager.HoldTabForCombat = g_OptionsConfig.HoldTabForCombat;

			if (ConfigManager.OffsetInterfaceWindows != g_OptionsConfig.OffsetInterfaceWindows && g_OptionsConfig.OffsetInterfaceWindows)
				ContainerRect.MakeDefault();

			ConfigManager.OffsetInterfaceWindows = g_OptionsConfig.OffsetInterfaceWindows;
			ConfigManager.AutoArrange = g_OptionsConfig.AutoArrange;
			ConfigManager.AlwaysRun = g_OptionsConfig.AlwaysRun;
			ConfigManager.DisableMenubar = g_OptionsConfig.DisableMenubar;
			ConfigManager.GrayOutOfRangeObjects = g_OptionsConfig.GrayOutOfRangeObjects;
			ConfigManager.DisableNewTargetSystem = g_OptionsConfig.DisableNewTargetSystem;
			ConfigManager.ObjectHandles = g_OptionsConfig.ObjectHandles;
			ConfigManager.SetReduceFPSUnactiveWindow(g_OptionsConfig.GetReduceFPSUnactiveWindow());
			ConfigManager.ItemPropertiesIcon = g_OptionsConfig.ItemPropertiesIcon;

			if (!g_OptionsConfig.DisableNewTargetSystem)
				GumpManager->AddGump(new TGumpTargetSystem(NewTargetSystem.GumpX, NewTargetSystem.GumpY));

			if (g_OptionsConfig.DisableMenubar)
				GumpManager->CloseGump(g_PlayerSerial, 0, GT_MENUBAR);
			else
				GumpManager->AddGump(new TGumpMenubar(g_PlayerSerial, 0, 0));

			break;
		}
		case 6: //Display
		{
			int curX = 0;

			if (TextEntryGameSizeX->Length())
				curX = atoi(TextEntryGameSizeX->c_str());

			if (curX < 640)
				curX = 640;
			else if (curX > (g_ClientWidth - 20))
				curX = g_ClientWidth - 20;

			g_GameWindowWidth = curX;
			
			int curY = 0;
			
			if (TextEntryGameSizeY->Length())
				curY = atoi(TextEntryGameSizeY->c_str());

			if (curY < 480)
				curY = 480;
			else if (curY > (g_ClientHeight - 40))
				curY = (g_ClientHeight - 40);

			g_GameWindowHeight = curY;

			TPacketGameWindowSize packet;
			packet.Send();

			ConfigManager.SpeechDelay = g_OptionsConfig.SpeechDelay;
			ConfigManager.ScaleSpeechDelay = g_OptionsConfig.ScaleSpeechDelay;
			ConfigManager.SpeechColor = g_OptionsConfig.SpeechColor;
			ConfigManager.EmoteColor = g_OptionsConfig.EmoteColor;
			ConfigManager.PartyMessageColor = g_OptionsConfig.PartyMessageColor;
			ConfigManager.GuildMessageColor = g_OptionsConfig.GuildMessageColor;
			ConfigManager.AllianceMessageColor = g_OptionsConfig.AllianceMessageColor;
			ConfigManager.IgnoreGuildMessage = g_OptionsConfig.IgnoreGuildMessage;
			ConfigManager.IgnoreAllianceMessage = g_OptionsConfig.IgnoreAllianceMessage;
			ConfigManager.DarkNights = g_OptionsConfig.DarkNights;
			ConfigManager.ColoredLighting = g_OptionsConfig.ColoredLighting;
			ConfigManager.StandartCharactersAnimationDelay = g_OptionsConfig.StandartCharactersAnimationDelay;
			ConfigManager.StandartItemsAnimationDelay = g_OptionsConfig.StandartItemsAnimationDelay;
			ConfigManager.LockResizingGameWindow = g_OptionsConfig.LockResizingGameWindow;
			ConfigManager.LockGumpsMoving = g_OptionsConfig.LockGumpsMoving;
			
			break;
		}
		case 7: //Reputation System
		{
			ConfigManager.InnocentColor = g_OptionsConfig.InnocentColor;
			ConfigManager.FriendlyColor = g_OptionsConfig.FriendlyColor;
			ConfigManager.SomeoneColor = g_OptionsConfig.SomeoneColor;
			ConfigManager.CriminalColor = g_OptionsConfig.CriminalColor;
			ConfigManager.EnemyColor = g_OptionsConfig.EnemyColor;
			ConfigManager.MurdererColor = g_OptionsConfig.MurdererColor;
			ConfigManager.CriminalActionsQuery = g_OptionsConfig.CriminalActionsQuery;

			break;
		}
		case 8: //Miscellaneous
		{
			ConfigManager.ShowIncomingNames = g_OptionsConfig.ShowIncomingNames;
			ConfigManager.UseCircleTrans = g_OptionsConfig.UseCircleTrans;
			ConfigManager.StatReport = g_OptionsConfig.StatReport;
			ConfigManager.SetConsoleNeedEnter(g_OptionsConfig.GetConsoleNeedEnter());
			ConfigManager.CircleTransRadius = g_OptionsConfig.CircleTransRadius;
			ConfigManager.SkillReport = g_OptionsConfig.SkillReport;
			ConfigManager.SpeechFont = g_OptionsConfig.SpeechFont;

			g_CircleOfTransparency.Create(ConfigManager.CircleTransRadius);

			break;
		}
		case 9: //Filter Options
		{
			break;
		}
		default:
			break;
	}
}
//---------------------------------------------------------------------------
const char *TGumpOptions::m_HotkeyText[0x100] =
{
	"",         //0x00
	"Left Mouse",   //0x01
	"Right Mouse",  //0x02
	"Ctrl-Break",   //0x03
	"Middle Mouse", //0x04
	"0x05",         //0x05
	"0x06",         //0x06
	"0x07",         //0x07
	"Backspace",    //0x08
	"Tab",          //0x09
	"0x0a",         //0x0a
	"0x0b",         //0x0b
	"Clear",        //0x0c
	"Enter",        //0x0d
	"0x0e",         //0x0e
	"0x0f",         //0x0f
	"Shift",        //0x10
	"Ctrl",         //0x11
	"Alt",          //0x12
	"Pause",        //0x13
	"Caps Lock",    //0x14
	"0x15",         //0x15
	"0x16",         //0x16
	"0x17",         //0x17
	"0x18",         //0x18
	"0x19",         //0x19
	"0x1a",         //0x1a
	"Esc",          //0x1b
	"0x1c",         //0x1c
	"0x1d",         //0x1d
	"0x1e",         //0x1e
	"0x1f",         //0x1f
	"Space",        //0x20
	"Page Up",      //0x21
	"Page Down",    //0x22
	"End",          //0x23
	"Home",         //0x24
	"Left Arrow",   //0x25
	"Up Arrow",     //0x26
	"Right Arrow",  //0x27
	"Down Arrow",   //0x28
	"Select",       //0x29
	"OEM 2A",       //0x2a
	"Execute",      //0x2b
	"Print Screen", //0x2c
	"Ins",          //0x2d
	"Del",          //0x2e
	"Help",         //0x2f
	"0",            //0x30
	"1",            //0x31
	"2",            //0x32
	"3",            //0x33
	"4",            //0x34
	"5",            //0x35
	"6",            //0x36
	"7",            //0x37
	"8",            //0x38
	"9",            //0x39
	"0x3a",         //0x3a
	"0x3b",         //0x3b
	"0x3c",         //0x3c
	"0x3d",         //0x3d
	"0x3e",         //0x3e
	"0x3f",         //0x3f
	"0x40",         //0x40
	"A",            //0x41
	"B",            //0x42
	"C",            //0x43
	"D",            //0x44
	"E",            //0x45
	"F",            //0x46
	"G",            //0x47
	"H",            //0x48
	"I",            //0x49
	"J",            //0x4a
	"K",            //0x4b
	"L",            //0x4c
	"M",            //0x4d
	"N",            //0x4e
	"O",            //0x4f
	"P",            //0x50
	"Q",            //0x51
	"R",            //0x52
	"S",            //0x53
	"T",            //0x54
	"U",            //0x55
	"V",            //0x56
	"W",            //0x57
	"X",            //0x58
	"Y",            //0x59
	"Z",            //0x5a
	"Left Win",     //0x5b
	"Right Win",    //0x5c
	"Apps",         //0x5d
	"0x5e",         //0x5e
	"0x5f",         //0x5f
	"Num 0",        //0x60
	"Num 1",        //0x61
	"Num 2",        //0x62
	"Num 3",        //0x63
	"Num 4",        //0x64
	"Num 5",        //0x65
	"Num 6",        //0x66
	"Num 7",        //0x67
	"Num 8",        //0x68
	"Num 9",        //0x69
	"Num *",        //0x6a
	"Num +",        //0x6b
	"Separator",    //0x6c
	"Num -",        //0x6d
	"Num .",        //0x6e
	"Num /",        //0x6f
	"F1",           //0x70
	"F2",           //0x71
	"F3",           //0x72
	"F4",           //0x73
	"F5",           //0x74
	"F6",           //0x75
	"F7",           //0x76
	"F8",           //0x77
	"F9",           //0x78
	"F10",          //0x79
	"F11",          //0x7a
	"F12",          //0x7b
	"Wheel Click",	//"F13",	//0x7c
	"Wheel Up",		//"F14",	//0x7d
	"Wheel Down",	//"F15",	//0x7e
	"Mouse+ Forward",//"F16",	//0x7f
	"Mouse+ Back",	//"F17",	//0x80
	"F18",          //0x81
	"F19",          //0x82
	"F20",          //0x83
	"F21",          //0x84
	"F22",          //0x85
	"F23",          //0x86
	"F24",          //0x87
	"0x88",         //0x88
	"0x89",         //0x89
	"0x8a",         //0x8a
	"0x8b",         //0x8b
	"0x8c",         //0x8c
	"0x8d",         //0x8d
	"0x8e",         //0x8e
	"0x8f",         //0x8f
	"Num Lock",      //0x90
	"Scroll Lock",   //0x91
	"0x92",         //0x92
	"0x93",         //0x93
	"0x94",         //0x94
	"0x95",         //0x95
	"0x96",         //0x96
	"0x97",         //0x97
	"0x98",         //0x98
	"0x99",         //0x99
	"0x9a",         //0x9a
	"0x9b",         //0x9b
	"0x9c",         //0x9c
	"0x9d",         //0x9d
	"0x9e",         //0x9e
	"0x9f",         //0x9f
	"0xa0",         //0xa0
	"0xa1",         //0xa1
	"0xa2",         //0xa2
	"0xa3",         //0xa3
	"0xa4",         //0xa4
	"0xa5",         //0xa5
	"0xa6",         //0xa6
	"0xa7",         //0xa7
	"0xa8",         //0xa8
	"0xa9",         //0xa9
	"0xaa",         //0xaa
	"0xab",         //0xab
	"0xac",         //0xac
	"0xad",         //0xad
	"0xae",         //0xae
	"0xaf",         //0xaf
	"0xb0",         //0xb0
	"0xb1",         //0xb1
	"0xb2",         //0xb2
	"0xb3",         //0xb3
	"0xb4",         //0xb4
	"0xb5",         //0xb5
	"0xb6",         //0xb6
	"0xb7",         //0xb7
	"0xb8",         //0xb8
	"0xb9",         //0xb9
	";",            //0xba
	"=",            //0xbb
	",",            //0xbc
	"-",            //0xbd
	".",            //0xbe
	"/",            //0xbf
	"`",            //0xc0
	"0xc1",         //0xc1
	"0xc2",         //0xc2
	"0xc3",         //0xc3
	"0xc4",         //0xc4
	"0xc5",         //0xc5
	"0xc6",         //0xc6
	"0xc7",         //0xc7
	"0xc8",         //0xc8
	"0xc9",         //0xc9
	"0xca",         //0xca
	"0xcb",         //0xcb
	"0xcc",         //0xcc
	"0xcd",         //0xcd
	"0xce",         //0xce
	"0xcf",         //0xcf
	"0xd0",         //0xd0
	"0xd1",         //0xd1
	"0xd2",         //0xd2
	"0xd3",         //0xd3
	"0xd4",         //0xd4
	"0xd5",         //0xd5
	"0xd6",         //0xd6
	"0xd7",         //0xd7
	"0xd8",         //0xd8
	"0xd9",         //0xd9
	"0xda",         //0xda
	"[",            //0xdb
	"\\",            //0xdc
	"]",            //0xdd
	"'",            //0xde
	"OEM DF",       //0xdf
	"OEM E0",       //0xe0
	"OEM E1",       //0xe1
	"OEM E2",       //0x32
	"OEM E3",       //0xe3
	"OEM E4",       //0xe4
	"0xe5",         //0xe5
	"OEM E6",       //0xe6
	"0xe7",         //0xe7
	"0xe8",         //0xe8
	"OEM E9",       //0xe9
	"OEM EA",       //0xea
	"OEM EB",       //0xeb
	"OEM EC",       //0xec
	"OEM ED",       //0xed
	"OEM EE",       //0xee
	"OEM EF",       //0xef
	"OEM F0",       //0xf0
	"OEM F1",       //0xf1
	"OEM F2",       //0xf2
	"OEM F3",       //0xf3
	"OEM F4",       //0xf4
	"OEM F5",       //0xf5
	"Attn",         //0xf6
	"Crsel",        //0xf7
	"Exsel",        //0xf8
	"Erase EOF",    //0xf9
	"Play",         //0xfa
	"Zoom",         //0xfb
	"0xfc",         //0xfc
	"PA1",          //0xfd
	"Clear",        //0xfe
	"0xff"          //0xff
};
//----------------------------------------------------------------------------