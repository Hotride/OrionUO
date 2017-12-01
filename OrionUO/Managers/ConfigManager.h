/***********************************************************************************
**
** ConfigManager.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H
//----------------------------------------------------------------------------------
//!Класс менеджера конфига
class CConfigManager
{
	SETGETE(bool, Sound, true);
	SETGETE(bool, Music, false);
	SETGET(bool, FootstepsSound, true);
	SETGET(bool, CombatMusic, true);
	SETGETE(uchar, SoundVolume, 255);
	SETGETE(uchar, MusicVolume, 255);

	SETGETE(uchar, ClientFPS, 32);
	SETGETE(bool, UseScaling, false);
	SETGET(bool, RemoveTextWithBlending, false);
	SETGETE(uchar, DrawStatusState, 0);
	SETGET(uchar, DrawStatusConditionState, 0);
	SETGET(uchar, DrawStatusConditionValue, 0);
	SETGETE(bool, DrawStumps, false);
	SETGETE(bool, MarkingCaves, false);
	SETGETE(bool, NoVegetation, false);
	SETGETE(bool, NoAnimateFields, false);
	SETGETE(bool, ReduceFPSUnactiveWindow, false);
	SETGET(bool, StandartCharactersAnimationDelay, false);
	SETGET(bool, StandartItemsAnimationDelay, false);
	SETGET(bool, LockGumpsMoving, false);
	SETGETE(bool, ConsoleNeedEnter, false);
	SETGET(uchar, HiddenCharactersRenderMode, 0);
	SETGET(uchar, HiddenAlpha, 0);
	SETGET(bool, UseHiddenModeOnlyForSelf, false);
	SETGET(uchar, TransparentSpellIcons, 0);
	SETGETE(uchar, SpellIconAlpha, 0);
	SETGETE(bool, OldStyleStatusbar, false);
	SETGETE(bool, OriginalPartyStatusbar, false);
	SETGETE(bool, ApplyStateColorOnCharacters, false);
	SETGETE(bool, ChangeFieldsGraphic, false);
	SETGETE(bool, PaperdollSlots, false);
	SETGET(bool, RemoveStatusbarsWithoutObjects, false);
	SETGET(bool, ShowDefaultConsoleEntryMode, false);
	SETGETE(uchar, DrawAuraState, 0);
	SETGET(bool, DrawAuraWithCtrlPressed, false);
	SETGET(uchar, ScreenshotFormat, 0);
	SETGETE(bool, ScaleImagesInPaperdollSlots, false);
	SETGET(bool, RemoveOrCreateObjectsWithBlending, false);
	SETGET(bool, DrawHelmetsOnShroud, false);
	SETGETE(bool, UseGlobalMapLayer, false);
	SETGETE(bool, NoDrawRoofs, false);
	SETGET(bool, HighlightTargetByType, true);
	SETGET(bool, AutoDisplayWorldMap, false);
	SETGETE(bool, UseGLListsForInterface, false);
	SETGET(bool, CheckPing, true);
	SETGETE(uchar, PingTimer, 10);

	SETGET(bool, UseToolTips, false);
	SETGET(ushort, ToolTipsTextColor, 0);
	SETGET(ushort, ToolTipsTextFont, 0);
	SETGET(ushort, ToolTipsDelay, 0);

	SETGET(ushort, ChatColorInputText, 0);
	SETGET(ushort, ChatColorMenuOption, 0);
	SETGET(ushort, ChatColorPlayerInMemberList, 0);
	SETGET(ushort, ChatColorText, 0);
	SETGET(ushort, ChatColorPlayerNameWithout, 0);
	SETGET(ushort, ChatColorMuted, 0);
	SETGET(ushort, ChatColorChannelModeratorName, 0);
	SETGET(ushort, ChatColorChannelModeratorText, 0);
	SETGET(ushort, ChatColorMyName, 0);
	SETGET(ushort, ChatColorMyText, 0);
	SETGET(ushort, ChatColorSystemMessage, 0);
	SETGET(ushort, ChatFont, 0);
	SETGET(ushort, ChatColorBGOutputText, 0);
	SETGET(ushort, ChatColorBGInputText, 0);
	SETGET(ushort, ChatColorBGUserList, 0);
	SETGET(ushort, ChatColorBGConfList, 0);
	SETGET(ushort, ChatColorBGCommandList, 0);

	SETGET(bool, EnablePathfind, false);
	SETGET(bool, HoldTabForCombat, false);
	SETGET(bool, OffsetInterfaceWindows, false);
	SETGET(bool, AutoArrange, false);
	SETGET(bool, AlwaysRun, false);
	SETGET(bool, DisableMenubar, false);
	SETGET(bool, GrayOutOfRangeObjects, false);
	SETGET(bool, DisableNewTargetSystem, false);
	SETGETE(uchar, ItemPropertiesMode, OPM_FOLLOW_MOUSE);
	SETGETE(bool, ItemPropertiesIcon, false);
	SETGET(bool, ObjectHandles, false);
	SETGET(bool, HoldShiftForContextMenus, false);
	SETGET(bool, HoldShiftForEnablePathfind, false);
	SETGETE(uchar, CharacterBackpackStyle, CBS_DEFAULT);

	SETGET(int, GameWindowWidth, 0);
	SETGET(int, GameWindowHeight, 0);
	SETGET(ushort, SpeechDelay, 0);
	SETGET(bool, ScaleSpeechDelay, false);
	SETGET(ushort, SpeechColor, 0);
	SETGET(ushort, EmoteColor, 0);
	SETGET(ushort, PartyMessageColor, 0);
	SETGET(ushort, GuildMessageColor, 0);
	SETGET(ushort, AllianceMessageColor, 0);
	SETGET(bool, IgnoreGuildMessage, false);
	SETGET(bool, IgnoreAllianceMessage, false);
	SETGET(bool, DarkNights, false);
	SETGET(bool, ColoredLighting, false);
	SETGET(bool, LockResizingGameWindow, false);

	SETGET(ushort, InnocentColor, 0);
	SETGET(ushort, FriendlyColor, 0);
	SETGET(ushort, SomeoneColor, 0);
	SETGET(ushort, CriminalColor, 0);
	SETGET(ushort, EnemyColor, 0);
	SETGET(ushort, MurdererColor, 0);
	SETGET(bool, CriminalActionsQuery, false);

	SETGET(bool, ShowIncomingNames, false);
	SETGET(bool, UseCircleTrans, false);
	SETGET(bool, StatReport, false);
	SETGET(uchar, CircleTransRadius, 0);
	SETGET(uchar, SkillReport, 0);
	SETGET(ushort, SpeechFont, 0);

	SETGET(bool, FilterPWOn, false);
	SETGET(bool, ObscenityFilter, false);
	SETGET(string, FilterPassword, "");

	SETGET(bool, ToggleBufficonWindow, false);
	SETGET(int, GameWindowX, 0);
	SETGET(int, GameWindowY, 0);
	SETGET(int, UpdateRange, g_MaxViewRange);

public:
	CConfigManager();
	~CConfigManager() {}

	/*!
	Инициализация
	@return 
	*/
	void Init();

	//!Проставление значений по-умолчанию
	void DefaultPage1();
	void DefaultPage2();
	void DefaultPage3();
	void DefaultPage4();
	//5 нету
	void DefaultPage6();
	void DefaultPage7();
	void DefaultPage8();
	void DefaultPage9();
	//10 нету

	void UpdateFeatures();

	/*!
	Получить цвет исходя из "злобности"
	@param [__in] notoriety Злобность
	@return Индекс цвета
	*/
	ushort GetColorByNotoriety(uchar notoriety);

	/*!
	Загрузка конфига
	@param [__in] path Путь к файлу с конфигом
	@return 
	*/
	bool LoadBin(string path);

	int GetConfigKeyCode(const string &key);

	enum CONFIG_MANAGER_KEY_CODE
	{
		CMKC_SOUND = 0,
		CMKC_SOUND_VOLUME,
		CMKC_MUSIC,
		CMKC_MUSIC_VOLUME,
		CMKC_FOOTSTEPS_SOUND,
		CMKC_COMBAT_MUSIC,
		CMKC_CLIENT_FPS,
		CMKC_USE_SCALING,
		CMKC_REMOVE_TEXT_WITH_BLENDING,
		CMKC_DRAW_STATUS_STATE,
		CMKC_DRAW_STUMPS,
		CMKC_MARKING_CAVES,
		CMKC_NO_ANIMATE_FIELDS,
		CMKC_NO_VEGETATION,
		CMKC_HIDDEN_CHARACTERS_RENDER_MODE,
		CMKC_HIDDEN_ALPHA,
		CMKC_USE_HIDDEN_MODE_ONLY_FOR_SELF,
		CMKC_TRANSPARENT_SPELL_ICONS,
		CMKC_SPELL_ICON_ALPHA,
		CMKC_OLD_STYLE_STATUSBAR,
		CMKC_ORIGINAL_PARTY_STATUSBAR,
		CMKC_APPLY_STATE_COLOR_ON_CHARACTERS,
		CMKC_CHANGE_FIELDS_GRAPHIC,
		CMKC_PAPERDOLL_SLOTS,
		CMKC_DRAW_STATUS_CONDITION_STATE,
		CMKC_DRAW_STATUS_CONDITION_VALUE,
		CMKC_REMOVE_STATUSBARS_WITHOUT_OBJECTS,
		CMKC_SHOW_DEFAULT_CONSOLE_ENTRY_MODE,
		CMKC_DRAW_AURA_STATE,
		CMKC_DRAW_AURA_WITH_CTRL_PRESSED,
		CMKC_SCREENSHOT_FORMAT,
		CMKC_SCALE_IMAGES_IN_PAPERDOLL_SLOTS,
		CMKC_REMOVE_OR_CREATE_OBJECTS_WITH_BLENDING,
		CMKC_DRAW_HELMETS_ON_SHROUD,
		CMKC_USE_GLOBAL_MAP_LAYER,
		CMKC_NO_DRAW_ROOFS,
		CMKC_HIGHLIGHT_TARGET_BY_TYPE,
		CMKC_AUTO_DISPLAY_WORLD_MAP,
		CMKC_USE_GL_LISTS_FOR_INTERFACE,
		CMKC_USE_TOOLTIPS,
		CMKC_TOOLTIPS_TEXT_COLOR,
		CMKC_TOOLTIPS_TEXT_FONT,
		CMKC_TOOLTIPS_DELAY,
		CMKC_CHAT_COLOR_INPUT_TEXT,
		CMKC_CHAT_COLOR_MENU_OPTION,
		CMKC_CHAT_COLOR_PLAYER_IN_MEMBER_LIST,
		CMKC_CHAT_COLOR_TEXT,
		CMKC_CHAT_COLOR_PLAYER_NAME_WITHOUT,
		CMKC_CHAT_COLOR_MUTED,
		CMKC_CHAT_COLOR_CHANNEL_MODERATOR_NAME,
		CMKC_CHAT_COLOR_CHANNEL_MODERATOR_TEXT,
		CMKC_CHAT_COLOR_MY_NAME,
		CMKC_CHAT_COLOR_MY_TEXT,
		CMKC_CHAT_COLOR_SYSTEM_MESSAGE,
		CMKC_CHAT_FONT,
		CMKC_CHAT_COLOR_BG_OUTPUT_TEXT,
		CMKC_CHAT_COLOR_BG_INPUT_TEXT,
		CMKC_CHAT_COLOR_BG_USER_LIST,
		CMKC_CHAT_COLOR_BG_CONF_LIST,
		CMKC_CHAT_COLOR_BG_COMMAND_LIST,
		CMKC_ENABLE_PATHFIND,
		CMKC_HOLD_TAB_FOR_COMBAT,
		CMKC_OFFSET_INTERFACE_WINDOWS,
		CMKC_AUTO_ARRANGE,
		CMKC_ALWAYS_RUN,
		CMKC_DISABLE_MENUBAR,
		CMKC_GRAY_OUT_OF_RANGE_OBJECTS,
		CMKC_DISABLE_NEW_TARGET_SYSTEM,
		CMKC_ITEMP_ROPERTIES_MODE,
		CMKC_ITEMP_ROPERTIES_ICON,
		CMKC_OBJECT_HANDLES,
		CMKC_REDUCE_FPS_UNACTIVE_WINDOW,
		CMKC_HOLD_SHIFT_FOR_CONTEXT_MENUS,
		CMKC_HOLD_SHIFT_FOR_ENABLE_PATHFIND,
		CMKC_CONTAINER_DEFAULT_X,
		CMKC_CONTAINER_DEFAULT_Y,
		CMKC_GAME_WINDOW_WIDTH,
		CMKC_GAME_WINDOW_HEIGHT,
		CMKC_SPEECH_DELAY,
		CMKC_SCALE_SPEECH_DELAY,
		CMKC_SPEECH_COLOR,
		CMKC_EMOTE_COLOR,
		CMKC_PARTY_MESSAGE_COLOR,
		CMKC_GUILD_MESSAGE_COLOR,
		CMKC_ALLIANCE_MESSAGE_COLOR,
		CMKC_IGNORE_GUILD_MESSAGE,
		CMKC_IGNORE_ALLIANCE_MESSAGE,
		CMKC_DARK_NIGHTS,
		CMKC_COLORED_LIGHTING,
		CMKC_STANDART_CHARACTERS_ANIMATION_DELAY,
		CMKC_STANDART_ITEMS_ANIMATION_DELAY,
		CMKC_LOCK_RESIZING_GAME_WINDOW,
		CMKC_LOCK_GUMPS_MOVING,
		CMKC_INNOCENT_COLOR,
		CMKC_FRIENDLY_COLOR,
		CMKC_SOMEONE_COLOR,
		CMKC_CRIMINAL_COLOR,
		CMKC_ENEMY_COLOR,
		CMKC_MURDERER_COLOR,
		CMKC_CRIMINAL_ACTIONS_QUERY,
		CMKC_SHOW_INCOMING_NAMES,
		CMKC_USE_CIRCLE_TRANS,
		CMKC_STAT_REPORT,
		CMKC_CONSOLE_NEED_ENTER,
		CMKC_CIRCLE_TRANS_RADIUS,
		CMKC_SKILL_REPORT,
		CMKC_SPEECH_FONT,
		CMKC_GAME_WINDOW_X,
		CMKC_GAME_WINDOW_Y,
		CMKC_ZOOMED,
		CMKC_REAL_X,
		CMKC_REAL_Y,
		CMKC_REAL_WIDTH,
		CMKC_REAL_HEIGHT,
		CMKC_TOGGLE_BUFFICON_WINDOW,
		CMKC_DEVELOPER_MODE,
		CMKC_LAST_SERVER,
		CMKC_LAST_CHARACTER,
		CMKC_CHARACTER_BACKPACK_STYLE,
		CMKC_CHECK_PING,
		CMKC_PING_TIMER,
		CMKC_COUNT
	};

	bool Load(const string &path);

	void Save(const string &path);
 };
 //----------------------------------------------------------------------------------
//!Менеджер конфига
extern CConfigManager g_ConfigManager;

//!Менеджер кофнига для опций
extern CConfigManager g_OptionsConfig;
//----------------------------------------------------------------------------------
#endif