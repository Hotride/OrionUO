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
class CGumpOptions : public CGump
{
private:
	static const char *m_HotkeyText[0x100];

	uint m_LastChangeMacroTime{ 0 };

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
	static const int ID_GO_P2_REDUCE_FPS_UNACTIVE_WINDOW = 201;
	static const int ID_GO_P2_CHARACTERS_ANIMATION_DELAY = 202;
	static const int ID_GO_P2_ITEMS_ANIMATION_DELAY = 203;
	static const int ID_GO_P2_ENABLE_SCALING = 204;
	static const int ID_GO_P2_REMOVE_TEXT_WITH_BLENDING = 205;
	static const int ID_GO_P2_NO_DRAW_CHARACTERS_STATUS = 206;
	static const int ID_GO_P2_DRAW_CHARACTERS_STATUS_TOP = 207;
	static const int ID_GO_P2_DRAW_CHARACTERS_STATUS_BOTTOM = 208;
	static const int ID_GO_P2_DRAW_STUMPS = 209;
	static const int ID_GO_P2_MARKING_CAVES = 210;
	static const int ID_GO_P2_NO_VEGETATION = 211;
	static const int ID_GO_P2_NO_ANIMATE_FIELDS = 212;
	static const int ID_GO_P2_LOCK_GUMP_MOVING = 213;
	static const int ID_GO_P2_CONSOLE_ENTER = 214;
	static const int ID_GO_P2_HIDDEN_CHARACTES_MODE_1 = 215;
	static const int ID_GO_P2_HIDDEN_CHARACTES_MODE_2 = 216;
	static const int ID_GO_P2_HIDDEN_CHARACTES_MODE_3 = 217;
	static const int ID_GO_P2_HIDDEN_CHARACTES_MODE_4 = 218;
	static const int ID_GO_P2_HIDDEN_ALPHA = 219;
	static const int ID_GO_P2_USE_HIDDEN_MODE_ONLY_FOR_SELF = 220;
	static const int ID_GO_P2_TRANSPARENT_SPELL_ICONS = 221;
	static const int ID_GO_P2_SPELL_ICONS_ALPHA = 222;
	static const int ID_GO_P2_OLD_STYLE_STATUSBAR = 223;
	static const int ID_GO_P2_ORIGINAL_PARTY_STATUSBAR = 224;
	static const int ID_GO_P2_APPLY_STATE_COLOR_ON_CHARACTERS = 225;
	static const int ID_GO_P2_CHANGE_FIELDS_GRAPHIC = 226;
	static const int ID_GO_P2_PAPERDOLL_SLOTS = 227;
	static const int ID_GO_P2_DRAW_CHARACTER_BARS_ALWAYS = 228;
	static const int ID_GO_P2_DRAW_CHARACTER_BARS_NOT_MAX = 229;
	static const int ID_GO_P2_DRAW_CHARACTER_BARS_LOWER = 230;
	static const int ID_GO_P2_DRAW_CHARACTER_BARS_LOWER_VALUE = 231;
	static const int ID_GO_P2_REMOVE_STATUSBARS_WITHOUT_OBJECTS = 232;
	static const int ID_GO_P2_SHOW_CONSOLE_ENTRY_MODE = 233;
	static const int ID_GO_P2_DRAW_AURA_NEVER = 234;
	static const int ID_GO_P2_DRAW_AURA_IN_WARMODE = 235;
	static const int ID_GO_P2_DRAW_AURA_ALWAYS = 236;
	static const int ID_GO_P2_DRAW_AURA_WITH_CTRL_PRESSED = 237;
	static const int ID_GO_P2_SCREENSHOT_FORMAT_BMP = 238;
	static const int ID_GO_P2_SCREENSHOT_FORMAT_PNG = 239;
	static const int ID_GO_P2_SCREENSHOT_FORMAT_TIFF = 240;
	static const int ID_GO_P2_SCREENSHOT_FORMAT_JPEG = 241;
	static const int ID_GO_P2_SCALE_IMAGES_IN_PAPERDOLL_SLOTS = 242;
	static const int ID_GO_P2_REMOVE_OR_CREATE_OBJECTS_WITH_BLENDING = 243;
	static const int ID_GO_P2_DRAW_HELMETS_ON_SHROUD = 244;
	static const int ID_GO_P2_USE_GLOBAL_MAP_LAYER = 245;
	static const int ID_GO_P2_NO_DRAW_ROOFS = 246;
	static const int ID_GO_P2_HIGHLIGHT_TARGET_BY_TYPE = 247;
	static const int ID_GO_P2_AUTO_DISPLAY_WORLD_MAP = 248;
	static const int ID_GO_P2_DEV_MODE_1 = 290;
	static const int ID_GO_P2_DEV_MODE_2 = 291;
	static const int ID_GO_P2_DEV_MODE_3 = 292;

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
	static const int ID_GO_P5_MACRO_SELECTION = 2000;
	static const int ID_GO_P5_ACTION_SELECTION = 10000;

	static const int ID_GO_P6_ENABLE_PATHFINDING = 600;
	static const int ID_GO_P6_HOLD_TAB_FOR_COMBAT = 601;
	static const int ID_GO_P6_OFFSET_INTERFACE_WINDOWS = 602;
	static const int ID_GO_P6_AUTO_ARRANGE_MINIMIZED_WINDOWS = 603;
	static const int ID_GO_P6_ALWAYS_RUN = 604;
	static const int ID_GO_P6_DISABLE_MENUBAR = 605;
	static const int ID_GO_P6_GRAY_OUT_OF_RANGE_OBJECTS = 606;
	static const int ID_GO_P6_DISABLE_NEW_TARGET_SYSTEM = 607;
	static const int ID_GO_P6_OBJECT_HANDLES = 608;
	static const int ID_GO_P6_DISPLAY_ITEM_PROPERTIES_ICON = 609;
	static const int ID_GO_P6_HOLD_SHIFT_FOR_CONTEXT_MENUS = 610;
	static const int ID_GO_P6_HOLD_SHIFT_FOR_ENABLE_PATHFINDING = 611;
	static const int ID_GO_P6_CONTAINER_OFFSET_X = 612;
	static const int ID_GO_P6_CONTAINER_OFFSET_Y = 613;

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
	static const int ID_GO_P7_LOCK_GAME_WINDOW_RESIZING = 713;

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
	static const int ID_GO_P9_SPEECH_FONT = 903;
	static const int ID_GO_P9_TRANSPARENCY_RADIUS = 904;
	static const int ID_GO_P9_INFORM_SKILLS = 905;

	class CMacro *m_MacroPointer{ NULL };
	class CMacroObject *m_MacroObjectPointer{ NULL };

	CGUISlider *m_SliderSound{ NULL };
	CGUISlider *m_SliderMusic{ NULL };
	CGUISlider *m_SliderClientFPS{ NULL };
	CGUISlider *m_SliderHiddenAlpha{ NULL };
	CGUISlider *m_SliderSpellIconsAlpha{ NULL };
	CGUISlider *m_SliderDrawStatusConditionValue{ NULL };
	CGUISlider *m_SliderTooltipDelay{ NULL };
	CGUISlider *m_SliderSpeechDuration{ NULL };
	CGUISlider *m_SliderCircleTransparency{ NULL };
	CGUISlider *m_SliderInformSkills{ NULL };

	CGUIColoredPolygone *m_ColorTooltipText{ NULL };

	CGUIColoredPolygone *m_ColorInputText{ NULL };
	CGUIColoredPolygone *m_ColorMenuOption{ NULL };
	CGUIColoredPolygone *m_ColorPlayerColorInMemberList{ NULL };
	CGUIColoredPolygone *m_ColorChatText{ NULL };
	CGUIColoredPolygone *m_ColorPlayerNameWithoutSpeakingPrivileges{ NULL };
	CGUIColoredPolygone *m_ColorMutedText{ NULL };
	CGUIColoredPolygone *m_ColorChannelModeratorName{ NULL };
	CGUIColoredPolygone *m_ColorChannelModeratorText{ NULL };
	CGUIColoredPolygone *m_ColorMyName{ NULL };
	CGUIColoredPolygone *m_ColorMyText{ NULL };
	CGUIColoredPolygone *m_ColorSystemMessage{ NULL };
	CGUIColoredPolygone *m_ColorTextOutputBackground{ NULL };
	CGUIColoredPolygone *m_ColorTextInputBackground{ NULL };
	CGUIColoredPolygone *m_ColorUserListBackground{ NULL };
	CGUIColoredPolygone *m_ColorConferenceListBackground{ NULL };
	CGUIColoredPolygone *m_ColorCommandListBackground{ NULL };

	CGUIColoredPolygone *m_ColorSpeech{ NULL };
	CGUIColoredPolygone *m_ColorEmote{ NULL };
	CGUIColoredPolygone *m_ColorPartyMessage{ NULL };
	CGUIColoredPolygone *m_ColorGuildMessage{ NULL };
	CGUIColoredPolygone *m_ColorAllianceMessage{ NULL };

	CGUIColoredPolygone *m_ColorInnocent{ NULL };
	CGUIColoredPolygone *m_ColorFriendly{ NULL };
	CGUIColoredPolygone *m_ColorSomeone{ NULL };
	CGUIColoredPolygone *m_ColorCriminal{ NULL };
	CGUIColoredPolygone *m_ColorEnemy{ NULL };
	CGUIColoredPolygone *m_ColorMurderer{ NULL };

	CGUIDataBox *m_MacroDataBox{ NULL };
	CGUICheckbox *m_MacroCheckboxShift{ NULL };
	CGUICheckbox *m_MacroCheckboxAlt{ NULL };
	CGUICheckbox *m_MacroCheckboxCtrl{ NULL };

	CGUITextEntry *m_ContainerOffsetX{ NULL };
	CGUITextEntry *m_ContainerOffsetY{ NULL };

	bool m_WantRedrawMacroData{ true };
	void RedrawMacroData();

protected:
	virtual void CalculateGumpState();

public:
	CGumpOptions(short x, short y);
	virtual ~CGumpOptions();

	void UpdateColor(const SELECT_COLOR_GUMP_STATE &state, const ushort &color);

	CGUITextEntry *m_GameWindowWidth{ NULL };
	CGUITextEntry *m_GameWindowHeight{ NULL };
	CGUITextEntry *m_MacroKey{ NULL };

	virtual void InitToolTip();

	virtual void PrepareContent();

	virtual void UpdateContent();
	
	void Init();

	GUMP_BUTTON_EVENT_H;
	GUMP_CHECKBOX_EVENT_H;
	GUMP_RADIO_EVENT_H;
	GUMP_COMBOBOX_SELECTION_EVENT_H;
	GUMP_SLIDER_CLICK_EVENT_H;
	GUMP_SLIDER_MOVE_EVENT_H;
	
	void OnCharPress(const WPARAM &wParam, const LPARAM &lParam);
	void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
