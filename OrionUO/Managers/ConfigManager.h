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
	bool FootstepsSound = true;
	bool CombatMusic = true;
	SETGETE(uchar, SoundVolume, 255);
	SETGETE(uchar, MusicVolume, 255);
	bool BackgroundSound = false;

	SETGETE(uchar, ClientFPS, 32);
	SETGETE(bool, UseScaling, false);
	bool RemoveTextWithBlending = false;
	SETGETE(uchar, DrawStatusState, 0);
	uchar DrawStatusConditionState = 0;
	uchar DrawStatusConditionValue = 0;
	SETGETE(bool, DrawStumps, false);
	SETGETE(bool, MarkingCaves, false);
	SETGETE(bool, NoVegetation, false);
	SETGETE(bool, NoAnimateFields, false);
	SETGETE(bool, ReduceFPSUnactiveWindow, false);
	bool StandartCharactersAnimationDelay = false;
	bool StandartItemsAnimationDelay = false;
	bool LockGumpsMoving = false;
	SETGETE(bool, ConsoleNeedEnter, false);
	uchar HiddenCharactersRenderMode = 0;
	uchar HiddenAlpha = 0;
	bool UseHiddenModeOnlyForSelf = false;
	uchar TransparentSpellIcons = 0;
	SETGETE(uchar, SpellIconAlpha, 0);
	SETGETE(bool, OldStyleStatusbar, false);
	SETGETE(bool, OriginalPartyStatusbar, false);
	SETGETE(bool, ApplyStateColorOnCharacters, false);
	SETGETE(bool, ChangeFieldsGraphic, false);
	SETGETE(bool, PaperdollSlots, false);
	bool RemoveStatusbarsWithoutObjects = false;
	bool ShowDefaultConsoleEntryMode = false;
	SETGETE(uchar, DrawAuraState, 0);
	bool DrawAuraWithCtrlPressed = false;
	uchar ScreenshotFormat = 0;
	SETGETE(bool, ScaleImagesInPaperdollSlots, false);
	bool RemoveOrCreateObjectsWithBlending = false;
	bool DrawHelmetsOnShroud = false;
	SETGETE(bool, UseGlobalMapLayer, false);
	SETGETE(bool, NoDrawRoofs, false);
	bool HighlightTargetByType = true;
	bool AutoDisplayWorldMap = false;
	SETGETE(bool, UseGLListsForInterface, false);
	bool CheckPing = true;
	SETGETE(uchar, PingTimer, 10);
	bool CancelNewTargetSystemOnShiftEsc = false;
	bool DrawStatusForHumanoids = true;

	bool UseToolTips = false;
	ushort ToolTipsTextColor = 0;
	ushort ToolTipsTextFont = 0;
	ushort ToolTipsDelay = 0;

	ushort ChatColorInputText = 0;
	ushort ChatColorMenuOption = 0;
	ushort ChatColorPlayerInMemberList = 0;
	ushort ChatColorText = 0;
	ushort ChatColorPlayerNameWithout = 0;
	ushort ChatColorMuted = 0;
	ushort ChatColorChannelModeratorName = 0;
	ushort ChatColorChannelModeratorText = 0;
	ushort ChatColorMyName = 0;
	ushort ChatColorMyText = 0;
	ushort ChatColorSystemMessage = 0;
	ushort ChatFont = 0;
	ushort ChatColorBGOutputText = 0;
	ushort ChatColorBGInputText = 0;
	ushort ChatColorBGUserList = 0;
	ushort ChatColorBGConfList = 0;
	ushort ChatColorBGCommandList = 0;

	bool EnablePathfind = false;
	bool HoldTabForCombat = false;
	bool OffsetInterfaceWindows = false;
	bool AutoArrange = false;
	bool AlwaysRun = false;
	bool DisableMenubar = false;
	bool GrayOutOfRangeObjects = false;
	bool DisableNewTargetSystem = false;
	SETGETE(uchar, ItemPropertiesMode, OPM_FOLLOW_MOUSE);
	SETGETE(bool, ItemPropertiesIcon, false);
	bool ObjectHandles = false;
	bool HoldShiftForContextMenus = false;
	bool HoldShiftForEnablePathfind = false;
	SETGETE(uchar, CharacterBackpackStyle, CBS_DEFAULT);

	int GameWindowWidth = 0;
	int GameWindowHeight = 0;
	ushort SpeechDelay = 0;
	bool ScaleSpeechDelay = false;
	ushort SpeechColor = 0;
	ushort EmoteColor = 0;
	ushort PartyMessageColor = 0;
	ushort GuildMessageColor = 0;
	ushort AllianceMessageColor = 0;
	bool IgnoreGuildMessage = false;
	bool IgnoreAllianceMessage = false;
	bool DarkNights = false;
	bool ColoredLighting = false;
	bool LockResizingGameWindow = false;

	ushort InnocentColor = 0;
	ushort FriendlyColor = 0;
	ushort SomeoneColor = 0;
	ushort CriminalColor = 0;
	ushort EnemyColor = 0;
	ushort MurdererColor = 0;
	bool CriminalActionsQuery = false;

	bool ShowIncomingNames = false;
	bool UseCircleTrans = false;
	bool StatReport = false;
	uchar CircleTransRadius = 0;
	uchar SkillReport = 0;
	ushort SpeechFont = 0;

	bool FilterPWOn = false;
	bool ObscenityFilter = false;
	string FilterPassword = "";

	bool ToggleBufficonWindow = false;
	int GameWindowX = 0;
	int GameWindowY = 0;
	int UpdateRange = g_MaxViewRange;

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
		CMKC_BACKGROUND_SOUND,
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
		CMKC_CANCEL_NEW_TARGET_SYSTEM_ON_SHIFT_ESC,
		CMKC_DRAW_STATUS_FOR_HUMANOIDS,
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