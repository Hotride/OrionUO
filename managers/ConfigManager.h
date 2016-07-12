/****************************************************************************
**
** ConfigManager.h
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
//--------------------------------------------------------------------------
#ifndef ConfigManagerH
#define ConfigManagerH
//--------------------------------------------------------------------------
//!Класс менеджера конфига
class TConfigManager
{
private:
	bool m_Sound;
	bool m_Music;
	bool m_FootstepsSound;
	bool m_CombatMusic;
	BYTE m_SoundVolume;
	BYTE m_MusicVolume;
	
	//page 2 vars
	BYTE m_ClientFPS;
	
	bool m_UseToolTips;
	WORD m_ToolTipsTextColor;
	WORD m_ToolTipsTextFont;
	WORD m_ToolTipsDelay;
	
	WORD m_ChatColorInputText;
	WORD m_ChatColorMenuOption;
	WORD m_ChatColorPlayerInMemberList;
	WORD m_ChatColorText;
	WORD m_ChatColorPlayerNameWithout;
	WORD m_ChatColorMuted;
	WORD m_ChatColorChannelModeratorName;
	WORD m_ChatColorChannelModeratorText;
	WORD m_ChatColorMyName;
	WORD m_ChatColorMyText;
	WORD m_ChatColorSystemMessage;
	WORD m_ChatFont;
	WORD m_ChatColorBGOutputText;
	WORD m_ChatColorBGInputText;
	WORD m_ChatColorBGUserList;
	WORD m_ChatColorBGConfList;
	WORD m_ChatColorBGCommandList;

	bool m_EnablePathfind;
	bool m_HoldTabForCombat;
	bool m_OffsetInterfaceWindows;
	bool m_AutoArrange;
	bool m_AlwaysRun;
	bool m_DisableMenubar;
	bool m_GrayOutOfRangeObjects;
	bool m_DisableNewTargetSystem;
	BYTE m_ItemPropertiesMode;
	bool m_ItemPropertiesIcon;
	bool m_ObjectHandles;
	bool m_ReduceFPSUnactiveWindow;
	bool m_HoldShiftForContextMenus;
	bool m_HoldShiftForEnablePathfind;
	
	WORD m_SpeechDelay;
	bool m_ScaleSpeechDelay;
	WORD m_SpeechColor;
	WORD m_EmoteColor;
	WORD m_PartyMessageColor;
	WORD m_GuildMessageColor;
	WORD m_AllianceMessageColor;
	bool m_IgnoreGuildMessage;
	bool m_IgnoreAllianceMessage;
	bool m_DarkNights;
	bool m_ColoredLighting;
	bool m_StandartCharactersAnimationDelay;
	bool m_StandartItemsAnimationDelay;
	bool m_LockResizingGameWindow;
	bool m_LockGumpsMoving;
	
	WORD m_InnocentColor;
	WORD m_FriendlyColor;
	WORD m_SomeoneColor;
	WORD m_CriminalColor;
	WORD m_EnemyColor;
	WORD m_MurdererColor;
	bool m_CriminalActionsQuery;
	
	bool m_ShowIncomingNames;
	bool m_UseCircleTrans;
	bool m_StatReport;
	bool m_ConsoleNeedEnter;
	BYTE m_CircleTransRadius;
	BYTE m_SkillReport;
	WORD m_SpeechFont;

	bool m_FilterPWOn;
	bool m_ObscenityFilter;
	string m_FilterPassword;

	bool m_ToggleBufficonWindow;

public:
	TConfigManager() {}
	~TConfigManager() {}

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
	
	SETGET(bool, Sound);
	SETGET(bool, Music);
	SETGET(bool, FootstepsSound);
	SETGET(bool, CombatMusic);
	SETGET(BYTE, SoundVolume);
	SETGET(BYTE, MusicVolume);

	//page 2 setget
	void SetClientFPS(BYTE val);
	BYTE GetClientFPS() const { return m_ClientFPS; }

	SETGET(bool, UseToolTips);
	SETGET(WORD, ToolTipsTextColor);
	SETGET(WORD, ToolTipsTextFont);
	SETGET(WORD, ToolTipsDelay);

	SETGET(WORD, ChatColorInputText);
	SETGET(WORD, ChatColorMenuOption);
	SETGET(WORD, ChatColorPlayerInMemberList);
	SETGET(WORD, ChatColorText);
	SETGET(WORD, ChatColorPlayerNameWithout);
	SETGET(WORD, ChatColorMuted);
	SETGET(WORD, ChatColorChannelModeratorName);
	SETGET(WORD, ChatColorChannelModeratorText);
	SETGET(WORD, ChatColorMyName);
	SETGET(WORD, ChatColorMyText);
	SETGET(WORD, ChatColorSystemMessage);
	SETGET(WORD, ChatFont);
	SETGET(WORD, ChatColorBGOutputText);
	SETGET(WORD, ChatColorBGInputText);
	SETGET(WORD, ChatColorBGUserList);
	SETGET(WORD, ChatColorBGConfList);
	SETGET(WORD, ChatColorBGCommandList);

	SETGET(bool, EnablePathfind);
	SETGET(bool, HoldTabForCombat);
	SETGET(bool, OffsetInterfaceWindows);
	SETGET(bool, AutoArrange);
	SETGET(bool, AlwaysRun);
	SETGET(bool, DisableMenubar);
	SETGET(bool, GrayOutOfRangeObjects);
	SETGET(bool, DisableNewTargetSystem);
	SETGET(BYTE, ItemPropertiesMode);
	SETGET(bool, ItemPropertiesIcon);
	SETGET(bool, ObjectHandles);
	void SetReduceFPSUnactiveWindow(bool val);
	bool GetReduceFPSUnactiveWindow() const { return m_ReduceFPSUnactiveWindow; }
	SETGET(bool, HoldShiftForContextMenus);
	SETGET(bool, HoldShiftForEnablePathfind);
	
	SETGET(WORD, SpeechDelay);
	SETGET(bool, ScaleSpeechDelay);
	SETGET(WORD, SpeechColor);
	SETGET(WORD, EmoteColor);
	SETGET(WORD, PartyMessageColor);
	SETGET(WORD, GuildMessageColor);
	SETGET(WORD, AllianceMessageColor);
	SETGET(bool, IgnoreGuildMessage);
	SETGET(bool, IgnoreAllianceMessage);
	SETGET(bool, DarkNights);
	SETGET(bool, ColoredLighting);
	SETGET(bool, StandartCharactersAnimationDelay);
	SETGET(bool, StandartItemsAnimationDelay);
	SETGET(bool, LockResizingGameWindow);
	SETGET(bool, LockGumpsMoving);

	SETGET(WORD, InnocentColor);
	SETGET(WORD, FriendlyColor);
	SETGET(WORD, SomeoneColor);
	SETGET(WORD, CriminalColor);
	SETGET(WORD, EnemyColor);
	SETGET(WORD, MurdererColor);
	SETGET(bool, CriminalActionsQuery);

	SETGET(bool, ShowIncomingNames);
	SETGET(bool, UseCircleTrans);
	SETGET(bool, StatReport);
	SETGET(BYTE, CircleTransRadius);
	SETGET(BYTE, SkillReport);
	SETGET(WORD, SpeechFont);
	void SetConsoleNeedEnter(bool val);
	bool GetConsoleNeedEnter() const {return m_ConsoleNeedEnter;}

	SETGET(bool, FilterPWOn);
	SETGET(bool, ObscenityFilter);
	SETGET(string, FilterPassword);

	SETGET(bool, ToggleBufficonWindow);

	/*!
	Получить цвет исходя из "злобности"
	@param [__in] notoriety Злобность
	@return Индекс цвета
	*/
	WORD GetColorByNotoriety(__in BYTE notoriety);

	/*!
	Загрузка конфига
	@param [__in] path Путь к файлу с конфигом
	@return 
	*/
	void Load(__in string path);

	/*!
	Сохранение конфига
	@param [__in] path Путь к файлу с конфигом
	@return 
	*/
	void Save(__in string path);
 };
//--------------------------------------------------------------------------
//!Менеджер конфига
extern TConfigManager ConfigManager;

//!Менеджер кофнига для опций
extern TConfigManager g_OptionsConfig;
//--------------------------------------------------------------------------
#endif