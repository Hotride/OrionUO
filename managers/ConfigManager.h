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
//Менеджер конфига
class TConfigManager
{
private:
	bool m_Sound;
	bool m_Music;
	bool m_FootstepsSound;
	bool m_CombatMusic;
	BYTE m_SoundVolume;
	BYTE m_MusicVolume;
	
	bool m_PopupHelpEnabled;
	bool m_PopupHelpSticky;
	bool m_PopupHelpPersistant;
	WORD m_PopupHelpDelay;
	WORD m_PopupHelpTextColor;
	WORD m_PopupHelpTextFont;
	
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
	
	WORD m_SpeechDelay;
	bool m_ScaleSpeechDelay;
	WORD m_SpeechColor;
	WORD m_EmoteColor;
	WORD m_PartyMessageColor;
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
public:
	TConfigManager() {}
	~TConfigManager() {}

	//Инициализация
	void Init();

	//Проставление значений по-умолчанию
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

	SETGET(bool, PopupHelpEnabled);
	SETGET(bool, PopupHelpSticky);
	SETGET(bool, PopupHelpPersistant);
	SETGET(WORD, PopupHelpDelay);
	SETGET(WORD, PopupHelpTextColor);
	SETGET(WORD, PopupHelpTextFont);

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
	
	SETGET(WORD, SpeechDelay);
	SETGET(bool, ScaleSpeechDelay);
	SETGET(WORD, SpeechColor);
	SETGET(WORD, EmoteColor);
	SETGET(WORD, PartyMessageColor);
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

	//Получить цвет исходя из "злобности"
	WORD GetColorByNotoriety(BYTE notoriety);

	//Загрузка конфига
	void Load(string path);

	//Сохранение конфига
	void Save(string path);
};
//--------------------------------------------------------------------------
extern TConfigManager ConfigManager;
extern TConfigManager g_OptionsConfig;
//--------------------------------------------------------------------------
#endif