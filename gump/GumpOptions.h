/****************************************************************************
**
** GumpOptions.h
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
//---------------------------------------------------------------------------
#ifndef OptionsGumpH
#define OptionsGumpH
//--------------------------------------------------------------------------
const float g_SphereListWidth = 88.0f;
//---------------------------------------------------------------------------
class TGumpOptions : public TGump
{
private:
	int m_Page;

	static const char *m_HotkeyText[0x100];

	DWORD m_LastChangeMacroTime;
	DWORD m_MacroElement;
	DWORD m_MacroSelection;
	int m_MacroListOffset;
	int m_MacroListNameOffset;
	int m_MacroListCount;
	int m_MacroListOffsetYStart;
	int m_MacroListOffsetYEnd;

	static const int m_MacroListMaxCount = 20;

	static TTextTexture m_TexturePage1[8];	//Sound and Music
	static TTextTexture m_TexturePage2[3];	//Orion's configuration
	static TTextTexture m_TexturePage3[6];	//Language
	static TTextTexture m_TexturePage4[19];	//Chat
	static TTextTexture m_TexturePage5[7];	//Macro Options
	static TTextTexture m_TexturePage6[15];	//Interface
	static TTextTexture m_TexturePage7[18];	//Display
	static TTextTexture m_TexturePage8[9];	//Reputation System
	static TTextTexture m_TexturePage9[9];	//Miscellaneous
	static TTextTexture m_TexturePage10[2];	//Filter Options
	
	int DrawPage1(bool &mode, DWORD &index); //Sound and Music
	int DrawPage2(bool &mode, DWORD &index); //Pop-up Help
	int DrawPage3(bool &mode, DWORD &index); //Language
	int DrawPage4(bool &mode, DWORD &index); //Chat
	int DrawPage5(bool &mode, DWORD &index); //Macro Options
	int DrawPage6(bool &mode, DWORD &index); //Interface
	int DrawPage7(bool &mode, DWORD &index); //Display
	int DrawPage8(bool &mode, DWORD &index); //Reputation System
	int DrawPage9(bool &mode, DWORD &index); //Miscellaneous
	int DrawPage10(bool &mode, DWORD &index); //Filter Options

	void ApplyPageChanges();
	
	static const int ID_GO_PAGE_0 = 1;
	static const int ID_GO_PAGE_1 = 2;
	static const int ID_GO_PAGE_2 = 3;
	static const int ID_GO_PAGE_3 = 4;
	static const int ID_GO_PAGE_4 = 5;
	static const int ID_GO_PAGE_5 = 6;
	static const int ID_GO_PAGE_6 = 7;
	static const int ID_GO_PAGE_7 = 8;
	static const int ID_GO_PAGE_8 = 9;
	static const int ID_GO_PAGE_9 = 10;
	static const int ID_GO_CANCEL = 11;
	static const int ID_GO_APPLY = 12;
	static const int ID_GO_DEFAULT = 13;
	static const int ID_GO_OKAY = 14;
	
	static const int ID_GO_P1_SOUND_ON_OFF = 100;
	static const int ID_GO_P1_MUSIC_ON_OFF = 101;
	static const int ID_GO_P1_PLAY_FOOTSTEP_SOUNDS = 102;
	static const int ID_GO_P1_PLAY_COMBAT_MUSIC = 103;
	static const int ID_GO_P1_SOUND_VOLUME = 104;
	static const int ID_GO_P1_MUSIC_VOLUME = 105;
	
	static const int ID_GO_P2_CLIENT_FPS = 200;

	static const int ID_GO_P3_USE_TOOLTIP = 300;
	static const int ID_GO_P3_TEXT_COLOR = 301;
	static const int ID_GO_P3_TEXT_FONT = 302;
	static const int ID_GO_P3_DELAY_BEFORE_TOOLTIP = 303;

	static const int ID_GO_P4_TEXT_FONT = 400;
	static const int ID_GO_P4_TEXT_COLOR = 401;

	static const int ID_GO_P5_BUTTON_ADD = 500;
	static const int ID_GO_P5_BUTTON_DELETE = 501;
	static const int ID_GO_P5_BUTTON_PREVEOUS = 502;
	static const int ID_GO_P5_BUTTON_NEXT = 503;
	static const int ID_GO_P5_KEY_BOX = 504;
	static const int ID_GO_P5_BUTTON_SHIFT = 505;
	static const int ID_GO_P5_BUTTON_ALT = 506;
	static const int ID_GO_P5_BUTTON_CTRL = 507;
	static const int ID_GO_P5_BUTTON_UP = 508;
	static const int ID_GO_P5_BUTTON_DOWN = 509;
	static const int ID_GO_P5_LEFT_BOX = 510;
	static const int ID_GO_P5_RIGHT_BOX = 520;
	static const int ID_GO_P5_EMPTY_BOX = 530;
	///////////////////////////////////////////
	static const int ID_GO_P5_SELECTION = 5000;

	static const int ID_GO_P6_ENABLE_PATHFINDING = 600;
	static const int ID_GO_P6_HOLD_TAB_FOR_COMBAT = 601;
	static const int ID_GO_P6_OFFSET_INTERFACE_WINDOWS = 602;
	static const int ID_GO_P6_AUTO_ARRANGE_MINIMIZED_WINDOWS = 603;
	static const int ID_GO_P6_ALWAYS_RUN = 604;
	static const int ID_GO_P6_DISABLE_MENUBAR = 605;
	static const int ID_GO_P6_GRAY_OUT_OF_RANGE_OBJECTS = 606;
	static const int ID_GO_P6_DISABLE_NEW_TARGET_SYSTEM = 607;
	static const int ID_GO_P6_OBJECT_HANDLES = 608;
	static const int ID_GO_P6_REDUCE_FPS_UNACTIVE_WINDOW = 609;
	static const int ID_GO_P6_DISPLAY_ITEM_PROPERTIES_ICON = 610;
	static const int ID_GO_P6_HOLD_SHIFT_FOR_CONTEXT_MENUS = 611;
	static const int ID_GO_P6_HOLD_SHIFT_FOR_ENABLE_PATHFINDING = 612;

	static const int ID_GO_P7_SCALE_SPEECH_DURATION = 700;
	static const int ID_GO_P7_SPEECH_COLOR = 701;
	static const int ID_GO_P7_EMOTE_COLOR = 702;
	static const int ID_GO_P7_PARTY_MESSAGE_COLOR = 703;
	static const int ID_GO_P7_GUILD_MESSAGE_COLOR = 704;
	static const int ID_GO_P7_ALLIANCE_MESSAGE_COLOR = 705;
	static const int ID_GO_P7_IGNORE_GUILD_MESSAGE = 706;
	static const int ID_GO_P7_IGNORE_ALLIANCE_MESSAGE = 707;
	static const int ID_GO_P7_DARK_NIGHTS = 708;
	static const int ID_GO_P7_COLORED_LIGHTING = 709;
	static const int ID_GO_P7_AJUST_LONG_SPEECH = 710;
	static const int ID_GO_P7_GAME_WINDOW_SIZE_X = 711;
	static const int ID_GO_P7_GAME_WINDOW_SIZE_Y = 712;
	static const int ID_GO_P7_CHARACTERS_ANIMATION_DELAY = 713;
	static const int ID_GO_P7_ITEMS_ANIMATION_DELAY = 714;
	static const int ID_GO_P7_LOCK_GAME_WINDOW_RESIZING = 715;
	static const int ID_GO_P7_LOCK_GUMP_MOVING = 716;

	static const int ID_GO_P8_INNOCENT_COLOR = 800;
	static const int ID_GO_P8_FRIENDLY_COLOR = 801;
	static const int ID_GO_P8_SOMEONE_COLOR = 802;
	static const int ID_GO_P8_CRIMINAL_COLOR = 803;
	static const int ID_GO_P8_ENEMY_COLOR = 804;
	static const int ID_GO_P8_MURDERER_COLOR = 805;
	static const int ID_GO_P8_QUERY_CRIMINAL_ACTIONS = 806;

	static const int ID_GO_P9_SHOW_APPROACHING_NAMES = 900;
	static const int ID_GO_P9_USE_CIRCLE_OF_TRANSPARENCY = 901;
	static const int ID_GO_P9_INFORM_STATS = 902;
	static const int ID_GO_P9_CONSOLE_ENTER = 903;
	static const int ID_GO_P9_SPEECH_FONT = 904;
	static const int ID_GO_P9_TRANSPARENCY_RADIUS = 905;
	static const int ID_GO_P9_INFORM_SKILLS = 906;

	TMacro *m_MacroPointer;
	TMacroObject *m_MacroObjectPointer;
public:
	TGumpOptions(DWORD serial, short x, short y);
	virtual ~TGumpOptions();
	
	TEntryText *TextEntryGameSizeX;
	TEntryText *TextEntryGameSizeY;
	TEntryText *TextEntryMacro;

	static void InitTextTextures();
	static void ReleaseTextTextures();

	SETGET(int, Page);

	void Init();
	
	void PrepareTextures();

	int Draw(bool &mode);

	void GenerateFrame();
	
	void OnLeftMouseDown();
	void OnLeftMouseUp();
	void OnCharPress(WPARAM &wparam, LPARAM &lparam);
	void OnKeyPress(WPARAM &wparam, LPARAM &lparam);

	bool EntryPointerHere();
};
//---------------------------------------------------------------------------
#endif