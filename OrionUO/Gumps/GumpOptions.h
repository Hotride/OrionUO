/***********************************************************************************
**
** GumpOptions.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPOPTIONS_H
#define GUMPOPTIONS_H
//----------------------------------------------------------------------------------
#include "Gump.h"
//----------------------------------------------------------------------------------
class CGumpOptions : public CGump
{
private:

	static const char *m_HotkeyText[0x100];

	uint m_LastChangeMacroTime;
	uint m_MacroElement;
	uint m_MacroSelection;
	int m_MacroListOffset;
	int m_MacroListNameOffset;
	int m_MacroListCount;
	int m_MacroListOffsetYStart;
	int m_MacroListOffsetYEnd;

	static const int m_MacroListMaxCount = 20;

	void DrawPage1(); //Sound and Music
	void DrawPage2(); //Pop-up Help
	void DrawPage3(); //Language
	void DrawPage4(); //Chat
	void DrawPage5(); //Macro Options
	void DrawPage6(); //Interface
	void DrawPage7(); //Display
	void DrawPage8(); //Reputation System
	void DrawPage9(); //Miscellaneous
	void DrawPage10(); //Filter Options

	void ApplyPageChanges();
	
	static const int ID_GO_PAGE_1 = 1;
	static const int ID_GO_PAGE_2 = 2;
	static const int ID_GO_PAGE_3 = 3;
	static const int ID_GO_PAGE_4 = 4;
	static const int ID_GO_PAGE_5 = 5;
	static const int ID_GO_PAGE_6 = 6;
	static const int ID_GO_PAGE_7 = 7;
	static const int ID_GO_PAGE_8 = 8;
	static const int ID_GO_PAGE_9 = 9;
	static const int ID_GO_PAGE_10 = 10;
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
	static const int ID_GO_P7_GAME_WINDOW_WIDTH = 711;
	static const int ID_GO_P7_GAME_WINDOW_HEIGHT = 712;
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

	class CMacro *m_MacroPointer;
	class CMacroObject *m_MacroObjectPointer;

	CGUISlider *m_SliderSound;
	CGUISlider *m_SliderMusic;
	CGUISlider *m_SliderClientFPS;
	CGUISlider *m_SliderTooltipDelay;
	CGUISlider *m_SliderSpeeckDuration;
	CGUISlider *m_SliderCircleTransparency;
	CGUISlider *m_SliderInformSkills;

	CGUIColoredPolygone *m_ColorTooltipText;

	CGUIColoredPolygone *m_ColorInputText;
	CGUIColoredPolygone *m_ColorMenuOption;
	CGUIColoredPolygone *m_ColorPlayerColorInMemberList;
	CGUIColoredPolygone *m_ColorChatText;
	CGUIColoredPolygone *m_ColorPlayerNameWithoutSpeakingPrivileges;
	CGUIColoredPolygone *m_ColorMutedText;
	CGUIColoredPolygone *m_ColorChannelModeratorName;
	CGUIColoredPolygone *m_ColorChannelModeratorText;
	CGUIColoredPolygone *m_ColorMyName;
	CGUIColoredPolygone *m_ColorMyText;
	CGUIColoredPolygone *m_ColorSystemMessage;
	CGUIColoredPolygone *m_ColorTextOutputBackground;
	CGUIColoredPolygone *m_ColorTextInputBackground;
	CGUIColoredPolygone *m_ColorUserListBackground;
	CGUIColoredPolygone *m_ColorConferenceListBackground;
	CGUIColoredPolygone *m_ColorCommandListBackground;

	CGUIColoredPolygone *m_ColorSpeech;
	CGUIColoredPolygone *m_ColorEmote;
	CGUIColoredPolygone *m_ColorPartyMessage;
	CGUIColoredPolygone *m_ColorGuildMessage;
	CGUIColoredPolygone *m_ColorAllianceMessage;

	CGUIColoredPolygone *m_ColorInnocent;
	CGUIColoredPolygone *m_ColorFriendly;
	CGUIColoredPolygone *m_ColorSomeone;
	CGUIColoredPolygone *m_ColorCriminal;
	CGUIColoredPolygone *m_ColorEnemy;
	CGUIColoredPolygone *m_ColorMurderer;

	CGUIDataBox *m_MacroDataBox;
	CGUICheckbox *m_MacroCheckboxShift;
	CGUICheckbox *m_MacroCheckboxAlt;
	CGUICheckbox *m_MacroCheckboxCtrl;

	void RedrawMacroData();

public:
	CGumpOptions(uint serial, short x, short y);
	virtual ~CGumpOptions();

	void UpdateColor(const SELECT_COLOR_GUMP_STATE &state, const ushort &color);

	CGUITextEntry *m_GameWindowWidth;
	CGUITextEntry *m_GameWindowHeight;
	CGUITextEntry *m_MacroKey;
	
	void Init();

	GUMP_BUTTON_EVENT_H;
	GUMP_CHECKBOX_EVENT_H;
	GUMP_SLIDER_CLICK_EVENT_H;
	GUMP_SLIDER_MOVE_EVENT_H;
	
	void OnLeftMouseDown();
	void OnCharPress(const WPARAM &wParam, const LPARAM &lParam);
	void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
