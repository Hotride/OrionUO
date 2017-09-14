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
	SETGETE(bool, Sound, true, OnChangedSound);
	SETGETE(bool, Music, false, OnChangedMusic);
	SETGET(bool, FootstepsSound, true);
	SETGET(bool, CombatMusic, true);
	SETGETE(uchar, SoundVolume, 255, OnChangedSoundVolume);
	SETGETE(uchar, MusicVolume, 255, OnChangedMusicVolume);

	SETGETE(uchar, ClientFPS, 32, OnChangeClientFPS);
	SETGETE(bool, UseScaling, false, OnChangeUseScaling);
	SETGET(bool, RemoveTextWithBlending, false);
	SETGETE(uchar, DrawStatusState, 0, OnChangeDrawStatusState);
	SETGET(uchar, DrawStatusConditionState, 0);
	SETGET(uchar, DrawStatusConditionValue, 0);
	SETGETE(bool, DrawStumps, false, OnChangeDrawStumps);
	SETGETE(bool, MarkingCaves, false, OnChangeMarkingCaves);
	SETGET(bool, NoVegetation, false);
	SETGET(bool, NoAnimateFields, false);
	SETGETE(bool, ReduceFPSUnactiveWindow, false, OnChangeReduceFPSUnactiveWindow);
	SETGET(bool, StandartCharactersAnimationDelay, false);
	SETGET(bool, StandartItemsAnimationDelay, false);
	SETGET(bool, LockGumpsMoving, false);
	SETGETE(bool, ConsoleNeedEnter, false, OnChangeConsoleNeedEnter);
	SETGET(uchar, HiddenCharactersRenderMode, 0);
	SETGET(uchar, HiddenAlpha, 0);
	SETGET(bool, UseHiddenModeOnlyForSelf, false);
	SETGET(uchar, TransparentSpellIcons, 0);
	SETGETE(uchar, SpellIconAlpha, 0, OnChangedSpellIconAlpha);
	SETGETE(bool, OldStyleStatusbar, false, OnChangeOldStyleStatusbar);
	SETGETE(bool, OriginalPartyStatusbar, false, OnChangeOriginalPartyStatusbar);
	SETGET(bool, ApplyStateColorOnCharacters, false);
	SETGETE(bool, ChangeFieldsGraphic, false, OnChangeChangeFieldsGraphic);
	SETGETE(bool, PaperdollSlots, false, OnChangePaperdollSlots);
	SETGET(bool, RemoveStatusbarsWithoutObjects, false);
	SETGET(bool, ShowDefaultConsoleEntryMode, false);
	SETGET(uchar, DrawAuraState, 0);
	SETGET(bool, DrawAuraWithCtrlPressed, false);
	SETGET(uchar, ScreenshotFormat, 0);
	SETGETE(bool, ScaleImagesInPaperdollSlots, false, OnChangeScaleImagesInPaperdollSlots);
	SETGET(bool, RemoveOrCreateObjectsWithBlending, false);
	SETGET(bool, DrawHelmetsOnShroud, false);
	SETGETE(bool, UseGlobalMapLayer, false, OnChangeUseGlobalMapLayer);
	SETGETE(bool, NoDrawRoofs, false, OnChangeNoDrawRoofs);
	SETGET(bool, HighlightTargetByType, true);

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
	SETGET(uchar, ItemPropertiesMode, 0);
	SETGET(bool, ItemPropertiesIcon, false);
	SETGET(bool, ObjectHandles, false);
	SETGET(bool, HoldShiftForContextMenus, false);
	SETGET(bool, HoldShiftForEnablePathfind, false);

	SETGET(int, GameWindowWidth, false);
	SETGET(int, GameWindowHeight, false);
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
	SETGET(int, UpdateRange, 0);

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
	bool Load(string path);

	/*!
	Сохранение конфига
	@param [__in] path Путь к файлу с конфигом
	@return 
	*/
	void Save(string path);
 };
 //----------------------------------------------------------------------------------
//!Менеджер конфига
extern CConfigManager g_ConfigManager;

//!Менеджер кофнига для опций
extern CConfigManager g_OptionsConfig;
//----------------------------------------------------------------------------------
#endif