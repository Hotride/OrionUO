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
#include "../Globals.h"
//----------------------------------------------------------------------------------
//!Класс менеджера конфига
class CConfigManager
{
	SETGETE(bool, Sound, OnChangedSound);
	SETGETE(bool, Music, OnChangedMusic);
	SETGET(bool, FootstepsSound);
	SETGET(bool, CombatMusic);
	SETGETE(uchar, SoundVolume, OnChangedSoundVolume);
	SETGETE(uchar, MusicVolume, OnChangedMusicVolume);

	SETGETE(uchar, ClientFPS, OnChangeClientFPS);
	SETGETE(bool, UseScaling, OnChangeUseScaling);
	SETGET(bool, RemoveTextWithBlending);
	SETGETE(uchar, DrawStatusState, OnChangeDrawStatusState);
	SETGET(uchar, DrawStatusConditionState);
	SETGET(uchar, DrawStatusConditionValue);
	SETGETE(bool, DrawStumps, OnChangeDrawStumps);
	SETGETE(bool, MarkingCaves, OnChangeMarkingCaves);
	SETGET(bool, NoVegetation);
	SETGET(bool, NoAnimateFields);
	SETGETE(bool, ReduceFPSUnactiveWindow, OnChangeReduceFPSUnactiveWindow);
	SETGET(bool, StandartCharactersAnimationDelay);
	SETGET(bool, StandartItemsAnimationDelay);
	SETGET(bool, LockGumpsMoving);
	SETGETE(bool, ConsoleNeedEnter, OnChangeConsoleNeedEnter);
	SETGET(uchar, HiddenCharactersRenderMode);
	SETGET(uchar, HiddenAlpha);
	SETGET(bool, UseHiddenModeOnlyForSelf);
	SETGET(uchar, TransparentSpellIcons);
	SETGETE(uchar, SpellIconAlpha, OnChangedSpellIconAlpha);
	SETGETE(bool, OldStyleStatusbar, OnChangeOldStyleStatusbar);
	SETGETE(bool, OriginalPartyStatusbar, OnChangeOriginalPartyStatusbar);
	SETGET(bool, ApplyStateColorOnCharacters);
	SETGETE(bool, ChangeFieldsGraphic, OnChangeChangeFieldsGraphic);
	SETGETE(bool, PaperdollSlots, OnChangePaperdollSlots);
	SETGET(bool, RemoveStatusbarsWithoutObjects);
	SETGET(bool, ShowDefaultConsoleEntryMode);
	SETGET(uchar, DrawAuraState);
	SETGET(bool, DrawAuraWithCtrlPressed);
	SETGET(uchar, ScreenshotFormat);
	SETGETE(bool, ScaleImagesInPaperdollSlots, OnChangeScaleImagesInPaperdollSlots);
	SETGET(bool, RemoveOrCreateObjectsWithBlending);
	SETGET(bool, DrawHelmetsOnShroud);
	SETGETE(bool, UseGlobalMapLayer, OnChangeUseGlobalMapLayer);

	SETGET(bool, UseToolTips);
	SETGET(ushort, ToolTipsTextColor);
	SETGET(ushort, ToolTipsTextFont);
	SETGET(ushort, ToolTipsDelay);

	SETGET(ushort, ChatColorInputText);
	SETGET(ushort, ChatColorMenuOption);
	SETGET(ushort, ChatColorPlayerInMemberList);
	SETGET(ushort, ChatColorText);
	SETGET(ushort, ChatColorPlayerNameWithout);
	SETGET(ushort, ChatColorMuted);
	SETGET(ushort, ChatColorChannelModeratorName);
	SETGET(ushort, ChatColorChannelModeratorText);
	SETGET(ushort, ChatColorMyName);
	SETGET(ushort, ChatColorMyText);
	SETGET(ushort, ChatColorSystemMessage);
	SETGET(ushort, ChatFont);
	SETGET(ushort, ChatColorBGOutputText);
	SETGET(ushort, ChatColorBGInputText);
	SETGET(ushort, ChatColorBGUserList);
	SETGET(ushort, ChatColorBGConfList);
	SETGET(ushort, ChatColorBGCommandList);

	SETGET(bool, EnablePathfind);
	SETGET(bool, HoldTabForCombat);
	SETGET(bool, OffsetInterfaceWindows);
	SETGET(bool, AutoArrange);
	SETGET(bool, AlwaysRun);
	SETGET(bool, DisableMenubar);
	SETGET(bool, GrayOutOfRangeObjects);
	SETGET(bool, DisableNewTargetSystem);
	SETGET(uchar, ItemPropertiesMode);
	SETGET(bool, ItemPropertiesIcon);
	SETGET(bool, ObjectHandles);
	SETGET(bool, HoldShiftForContextMenus);
	SETGET(bool, HoldShiftForEnablePathfind);

	SETGET(int, GameWindowWidth);
	SETGET(int, GameWindowHeight);
	SETGET(ushort, SpeechDelay);
	SETGET(bool, ScaleSpeechDelay);
	SETGET(ushort, SpeechColor);
	SETGET(ushort, EmoteColor);
	SETGET(ushort, PartyMessageColor);
	SETGET(ushort, GuildMessageColor);
	SETGET(ushort, AllianceMessageColor);
	SETGET(bool, IgnoreGuildMessage);
	SETGET(bool, IgnoreAllianceMessage);
	SETGET(bool, DarkNights);
	SETGET(bool, ColoredLighting);
	SETGET(bool, LockResizingGameWindow);

	SETGET(ushort, InnocentColor);
	SETGET(ushort, FriendlyColor);
	SETGET(ushort, SomeoneColor);
	SETGET(ushort, CriminalColor);
	SETGET(ushort, EnemyColor);
	SETGET(ushort, MurdererColor);
	SETGET(bool, CriminalActionsQuery);

	SETGET(bool, ShowIncomingNames);
	SETGET(bool, UseCircleTrans);
	SETGET(bool, StatReport);
	SETGET(uchar, CircleTransRadius);
	SETGET(uchar, SkillReport);
	SETGET(ushort, SpeechFont);

	SETGET(bool, FilterPWOn);
	SETGET(bool, ObscenityFilter);
	SETGET(string, FilterPassword);

	SETGET(bool, ToggleBufficonWindow);
	SETGET(int, GameWindowX);
	SETGET(int, GameWindowY);
	SETGET(int, UpdateRange);

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