/****************************************************************************
**
** ConfigManager.cpp
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
#include "stdafx.h"

TConfigManager ConfigManager;
TConfigManager g_OptionsConfig;
//---------------------------------------------------------------------------
/*!
Инициализация
@return 
*/
void TConfigManager::Init()
{
	DefaultPage1();

	DefaultPage2();
	
	DefaultPage3();

	DefaultPage4();

	DefaultPage6();

	DefaultPage7();
	
	DefaultPage8();
	
	DefaultPage9();

	g_CircleOfTransparency.Create(m_CircleTransRadius);

	m_FilterPWOn = false;
	m_ObscenityFilter = false;
	m_FilterPassword = "";
	
	g_GameWindowPosX = 0;
	g_GameWindowPosY = 0;
}
//---------------------------------------------------------------------------
void TConfigManager::DefaultPage1()
{
	m_Sound = true;
	m_Music = true;
	m_FootstepsSound = true;
	m_CombatMusic = true;
	m_SoundVolume = 255;
	m_MusicVolume = 255;
}
//---------------------------------------------------------------------------
void TConfigManager::DefaultPage2()
{
	m_PopupHelpEnabled = true;
	m_PopupHelpSticky = false;
	m_PopupHelpPersistant = true;
	m_PopupHelpDelay = 1000;
	m_PopupHelpTextColor = 0xFFFF;
	m_PopupHelpTextFont = 1;
}
//---------------------------------------------------------------------------
void TConfigManager::DefaultPage3()
{
	m_UseToolTips = true;
	m_ToolTipsTextColor = 0xFFFF;
	m_ToolTipsTextFont = 1;
	m_ToolTipsDelay = 1000;
}
//---------------------------------------------------------------------------
void TConfigManager::DefaultPage4()
{
	m_ChatColorInputText = 0xFFFF;
	m_ChatColorMenuOption = 0xFFFF;
	m_ChatColorPlayerInMemberList = 0xFFFF;
	m_ChatColorText = 0xFFFF;
	m_ChatColorPlayerNameWithout = 0xFFFF;
	m_ChatColorMuted = 0xFFFF;
	m_ChatColorChannelModeratorName = 0xFFFF;
	m_ChatColorChannelModeratorText = 0xFFFF;
	m_ChatColorMyName = 0xFFFF;
	m_ChatColorMyText = 0xFFFF;
	m_ChatColorSystemMessage = 0xFFFF;
	m_ChatColorBGOutputText = 0xFFFF;
	m_ChatColorBGInputText = 0xFFFF;
	m_ChatColorBGUserList = 0xFFFF;
	m_ChatColorBGConfList = 0xFFFF;
	m_ChatColorBGCommandList = 0xFFFF;
	
	m_ChatFont = 2;
}
//---------------------------------------------------------------------------
void TConfigManager::DefaultPage6()
{
	m_EnablePathfind = true;
	m_HoldTabForCombat = true;
	m_OffsetInterfaceWindows = true;
	m_AutoArrange = true;
	m_AlwaysRun = false;
	m_DisableMenubar = true;
	m_GrayOutOfRangeObjects = true;
	m_DisableNewTargetSystem = true;
	m_ItemPropertiesMode = 0;
	m_ItemPropertiesIcon = false;
	m_ObjectHandles = false;
	m_ReduceFPSUnactiveWindow = true;
	m_HoldShiftForContextMenus = true;
}
//---------------------------------------------------------------------------
void TConfigManager::DefaultPage7()
{
	g_GameWindowWidth = 800;
	g_GameWindowHeight = 600;
	m_SpeechDelay = 100;
	m_ScaleSpeechDelay = true;
	m_SpeechColor = 0x02B2;
	m_EmoteColor = 0x0021;
	m_PartyMessageColor = 0x0044;
	m_GuildMessageColor = 0x0044;
	m_AllianceMessageColor = 0x0044;
	m_IgnoreGuildMessage = false;
	m_IgnoreAllianceMessage = false;
	m_DarkNights = false;
	m_ColoredLighting = true;
	m_StandartCharactersAnimationDelay = true;
	m_StandartItemsAnimationDelay = true;
	m_LockResizingGameWindow = true;
	m_LockGumpsMoving = false;
}
//---------------------------------------------------------------------------
void TConfigManager::DefaultPage8()
{
	m_InnocentColor = 0x005A;
	m_FriendlyColor = 0x0044;
	m_SomeoneColor = 0x03B2;
	m_CriminalColor = 0x03B2;
	m_EnemyColor = 0x002B;
	m_MurdererColor = 0x0023;
	m_CriminalActionsQuery = true;
}
//---------------------------------------------------------------------------
void TConfigManager::DefaultPage9()
{
	m_ShowIncomingNames = true;
	m_UseCircleTrans = false;
	m_StatReport = true;
	m_ConsoleNeedEnter = false;
	m_CircleTransRadius = 64;
	m_SkillReport = 1;
	m_SpeechFont = 0;
}
//---------------------------------------------------------------------------
void TConfigManager::SetConsoleNeedEnter(bool val)
{
	m_ConsoleNeedEnter = val;
	if (m_ConsoleNeedEnter && EntryPointer == GameConsole)
		EntryPointer = NULL;
}
//---------------------------------------------------------------------------
void TConfigManager::SetReduceFPSUnactiveWindow(bool val)
{
	m_ReduceFPSUnactiveWindow = val;

	if (m_ReduceFPSUnactiveWindow)
		g_FrameDelay[0] = FRAME_DELAY_UNACTIVE_WINDOW;
	else
		g_FrameDelay[0] = FRAME_DELAY_ACTIVE_WINDOW;
}
//---------------------------------------------------------------------------
/*!
Получить цвет исходя из "злобности"
@param [__in] notoriety Злобность
@return Индекс цвета
*/
WORD TConfigManager::GetColorByNotoriety( __in BYTE notoriety)
{
	WORD color = 0;

	switch ((NOTORIETY_TYPE)notoriety)
	{
		case NT_INNOCENT: //Blue
		{
			color = m_InnocentColor;
			break;
		}
		case NT_FRIENDLY: //Green
		{
			color = m_FriendlyColor;
			break;
		}
		case NT_SOMEONE_GRAY: //Someone gray
		{
			color = m_SomeoneColor;
			break;
		}
		case NT_CRIMINAL: //Criminal
		{
			color = m_CriminalColor;
			break;
		}
		case NT_ENEMY: //Orange
		{
			color = m_EnemyColor;
			break;
		}
		case NT_MURDERER: //Red*
		{
			color = m_MurdererColor;
			break;
		}
		default: break;
	}

	return color;
}
//---------------------------------------------------------------------------
/*!
Загрузка конфига
@param [__in] path Путь к файлу с конфигом
@return 
*/
void TConfigManager::Load( __in string path)
{
	TMappedHeader file;
	memset(&file, 0, sizeof(TMappedHeader));

	FileManager.LoadFileToMemory(file, path.c_str());

	if (file.Size)
	{
		BYTE version = file.ReadByte();
		
		//Page 1
		PBYTE next = file.Ptr;
		char blockSize = file.ReadChar();
		next += blockSize;

		if (file.ReadChar() == 1)
		{
			m_Sound = file.ReadByte();
			m_Music = file.ReadByte();
			m_FootstepsSound = file.ReadByte();
			m_CombatMusic = file.ReadByte();
			m_SoundVolume = file.ReadByte();
			m_MusicVolume = file.ReadByte();
		}
		
		file.Ptr = next;
		
		//Page 2
		next = file.Ptr;
		blockSize = file.ReadChar();
		next += blockSize;

		if (file.ReadChar() == 2)
		{
			m_PopupHelpEnabled = file.ReadByte();
			m_PopupHelpSticky = file.ReadByte();
			m_PopupHelpPersistant = file.ReadByte();
			m_PopupHelpDelay = file.ReadWord();
			m_PopupHelpTextColor = file.ReadWord();
			m_PopupHelpTextFont = file.ReadWord();
		}
		
		file.Ptr = next;
		
		//Page 3
		next = file.Ptr;
		blockSize = file.ReadChar();
		next += blockSize;

		if (file.ReadByte() == 3)
		{
			m_UseToolTips = file.ReadByte();
			m_ToolTipsTextColor = file.ReadWord();
			m_ToolTipsTextFont = file.ReadWord();
			m_ToolTipsDelay = file.ReadWord();
		}
		
		file.Ptr = next;
		
		//Page 4
		next = file.Ptr;
		blockSize = file.ReadChar();
		next += blockSize;

		if (file.ReadByte() == 4)
		{
			m_ChatColorInputText = file.ReadWord();
			m_ChatColorMenuOption = file.ReadWord();
			m_ChatColorPlayerInMemberList = file.ReadWord();
			m_ChatColorText = file.ReadWord();
			m_ChatColorPlayerNameWithout = file.ReadWord();
			m_ChatColorMuted = file.ReadWord();
			m_ChatColorChannelModeratorName = file.ReadWord();
			m_ChatColorChannelModeratorText = file.ReadWord();
			m_ChatColorMyName = file.ReadWord();
			m_ChatColorMyText = file.ReadWord();
			m_ChatColorSystemMessage = file.ReadWord();
			m_ChatFont = file.ReadWord();
			m_ChatColorBGOutputText = file.ReadWord();
			m_ChatColorBGInputText = file.ReadWord();
			m_ChatColorBGUserList = file.ReadWord();
			m_ChatColorBGConfList = file.ReadWord();
			m_ChatColorBGCommandList = file.ReadWord();
		}
		
		file.Ptr = next;
		
		//Page 6
		next = file.Ptr;
		blockSize = file.ReadChar();
		next += blockSize;

		if (file.ReadByte() == 6)
		{
			m_EnablePathfind = file.ReadByte();
			m_HoldTabForCombat = file.ReadByte();
			m_OffsetInterfaceWindows = file.ReadByte();
			m_AutoArrange = file.ReadByte();
			m_AlwaysRun = file.ReadByte();
			m_DisableMenubar = file.ReadByte();
			m_GrayOutOfRangeObjects = file.ReadByte();

			if (blockSize > 9)
			{
				m_DisableNewTargetSystem = file.ReadByte();
				m_ItemPropertiesMode = file.ReadByte();
				m_ItemPropertiesIcon = file.ReadByte();
				m_ObjectHandles = file.ReadByte();
				SetReduceFPSUnactiveWindow(file.ReadByte());

				if (blockSize > 14)
					m_HoldShiftForContextMenus = file.ReadByte();
				else
					m_HoldShiftForContextMenus = true;
			}
			else
			{
				m_DisableNewTargetSystem = true;
				m_ItemPropertiesMode = 0;
				m_ItemPropertiesIcon = false;
				m_ObjectHandles = false;
				SetReduceFPSUnactiveWindow(true);
				m_HoldShiftForContextMenus = true;
			}
		}
		
		file.Ptr = next;
		
		//Page 7
		next = file.Ptr;
		blockSize = file.ReadChar();
		next += blockSize;

		if (file.ReadByte() == 7)
		{
			g_GameWindowWidth = file.ReadWord();
			g_GameWindowHeight = file.ReadWord();
			m_SpeechDelay = file.ReadWord();
			m_ScaleSpeechDelay = file.ReadWord();
			m_SpeechColor = file.ReadWord();
			m_EmoteColor = file.ReadWord();
			m_PartyMessageColor = file.ReadWord();
			m_GuildMessageColor = file.ReadWord();
			m_AllianceMessageColor = file.ReadWord();
			m_IgnoreGuildMessage = file.ReadByte();
			m_IgnoreAllianceMessage = file.ReadByte();
			m_DarkNights = file.ReadByte();
			m_ColoredLighting = file.ReadByte();
			m_StandartCharactersAnimationDelay = file.ReadByte();
			m_StandartItemsAnimationDelay = file.ReadByte();
			m_LockResizingGameWindow = file.ReadByte();
			m_LockGumpsMoving = file.ReadByte();
		}
		
		file.Ptr = next;
		
		//Page 8
		next = file.Ptr;
		blockSize = file.ReadChar();
		next += blockSize;

		if (file.ReadByte() == 8)
		{
			m_InnocentColor = file.ReadWord();
			m_FriendlyColor = file.ReadWord();
			m_SomeoneColor = file.ReadWord();
			m_CriminalColor = file.ReadWord();
			m_EnemyColor = file.ReadWord();
			m_MurdererColor = file.ReadWord();
			m_CriminalActionsQuery = file.ReadByte();
		}
		
		file.Ptr = next;
		
		//Page 9
		next = file.Ptr;
		blockSize = file.ReadChar();
		next += blockSize;

		if (file.ReadByte() == 9)
		{
			m_ShowIncomingNames = file.ReadByte();
			m_UseCircleTrans = file.ReadByte();
			m_StatReport = file.ReadByte();
			m_ConsoleNeedEnter = file.ReadByte();
			m_CircleTransRadius = file.ReadByte();
			m_SkillReport = file.ReadByte();
			m_SpeechFont = file.ReadWord();
		}
		
		g_CircleOfTransparency.Create(m_CircleTransRadius);

		file.Ptr = next;
		
		//Page 10
		next = file.Ptr;
		blockSize = file.ReadChar();
		next += blockSize;

		if (file.ReadByte() == 10)
		{
		}
		
		file.Ptr = next;
		
		//No page
		blockSize = file.ReadChar();

		if (!file.ReadByte())
		{
			g_GameWindowPosX = file.ReadWord();
			g_GameWindowPosY = file.ReadWord();
		}

		FileManager.UnloadFileFromMemory(file);
	}
	else
		Init();
}
//---------------------------------------------------------------------------
/*!
Сохранение конфига
@param [__in] path Путь к файлу с конфигом
@return 
*/
void TConfigManager::Save( __in string path)
{
	TFileWriter *writer = new TFileWriter(path, true);
	
	writer->WriteByte(0); //version

	//Page 1
	writer->WriteByte(8); //size of block
	writer->WriteByte(1); //page index
	writer->WriteByte(m_Sound);
	writer->WriteByte(m_Music);
	writer->WriteByte(m_FootstepsSound);
	writer->WriteByte(m_CombatMusic);
	writer->WriteByte(m_SoundVolume);
	writer->WriteByte(m_MusicVolume);
	writer->WriteBuffer();
	
	//Page 2
	writer->WriteByte(11); //size of block
	writer->WriteByte(2); //page index
	writer->WriteByte(m_PopupHelpEnabled);
	writer->WriteByte(m_PopupHelpSticky);
	writer->WriteByte(m_PopupHelpPersistant);
	writer->WriteWord(m_PopupHelpDelay);
	writer->WriteWord(m_PopupHelpTextColor);
	writer->WriteWord(m_PopupHelpTextFont);
	writer->WriteBuffer();
	
	//Page 3
	writer->WriteByte(9); //size of block
	writer->WriteByte(3); //page index
	writer->WriteByte(m_UseToolTips);
	writer->WriteWord(m_ToolTipsTextColor);
	writer->WriteWord(m_ToolTipsTextFont);
	writer->WriteWord(m_ToolTipsDelay);
	writer->WriteBuffer();
	
	//Page 4
	writer->WriteByte(36); //size of block
	writer->WriteByte(4); //page index
	writer->WriteWord(m_ChatColorInputText);
	writer->WriteWord(m_ChatColorMenuOption);
	writer->WriteWord(m_ChatColorPlayerInMemberList);
	writer->WriteWord(m_ChatColorText);
	writer->WriteWord(m_ChatColorPlayerNameWithout);
	writer->WriteWord(m_ChatColorMuted);
	writer->WriteWord(m_ChatColorChannelModeratorName);
	writer->WriteWord(m_ChatColorChannelModeratorText);
	writer->WriteWord(m_ChatColorMyName);
	writer->WriteWord(m_ChatColorMyText);
	writer->WriteWord(m_ChatColorSystemMessage);
	writer->WriteWord(m_ChatFont);
	writer->WriteWord(m_ChatColorBGOutputText);
	writer->WriteWord(m_ChatColorBGInputText);
	writer->WriteWord(m_ChatColorBGUserList);
	writer->WriteWord(m_ChatColorBGConfList);
	writer->WriteWord(m_ChatColorBGCommandList);
	writer->WriteBuffer();
	
	//Page 6
	writer->WriteByte(15); //size of block
	writer->WriteByte(6); //page index
	writer->WriteByte(m_EnablePathfind);
	writer->WriteByte(m_HoldTabForCombat);
	writer->WriteByte(m_OffsetInterfaceWindows);
	writer->WriteByte(m_AutoArrange);
	writer->WriteByte(m_AlwaysRun);
	writer->WriteByte(m_DisableMenubar);
	writer->WriteByte(m_GrayOutOfRangeObjects);
	writer->WriteByte(m_DisableNewTargetSystem);
	writer->WriteByte(m_ItemPropertiesMode);
	writer->WriteByte(m_ItemPropertiesIcon);
	writer->WriteByte(m_ObjectHandles);
	writer->WriteByte(m_ReduceFPSUnactiveWindow);
	writer->WriteByte(m_HoldShiftForContextMenus);
	writer->WriteBuffer();
	
	//Page 7
	writer->WriteByte(28); //size of block
	writer->WriteByte(7); //page index
	writer->WriteWord(g_GameWindowWidth);
	writer->WriteWord(g_GameWindowHeight);
	writer->WriteWord(m_SpeechDelay);
	writer->WriteWord(m_ScaleSpeechDelay);
	writer->WriteWord(m_SpeechColor);
	writer->WriteWord(m_EmoteColor);
	writer->WriteWord(m_PartyMessageColor);
	writer->WriteWord(m_GuildMessageColor);
	writer->WriteWord(m_AllianceMessageColor);
	writer->WriteByte(m_IgnoreGuildMessage);
	writer->WriteByte(m_IgnoreAllianceMessage);
	writer->WriteByte(m_DarkNights);
	writer->WriteByte(m_ColoredLighting);
	writer->WriteByte(m_StandartCharactersAnimationDelay);
	writer->WriteByte(m_StandartItemsAnimationDelay);
	writer->WriteByte(m_LockResizingGameWindow);
	writer->WriteByte(m_LockGumpsMoving);
	writer->WriteBuffer();
	
	//Page 8
	writer->WriteByte(15); //size of block
	writer->WriteByte(8); //page index
	writer->WriteWord(m_InnocentColor);
	writer->WriteWord(m_FriendlyColor);
	writer->WriteWord(m_SomeoneColor);
	writer->WriteWord(m_CriminalColor);
	writer->WriteWord(m_EnemyColor);
	writer->WriteWord(m_MurdererColor);
	writer->WriteByte(m_CriminalActionsQuery);
	writer->WriteBuffer();
	
	//Page 9
	writer->WriteByte(10); //size of block
	writer->WriteByte(9); //page index
	writer->WriteByte(m_ShowIncomingNames);
	writer->WriteByte(m_UseCircleTrans);
	writer->WriteByte(m_StatReport);
	writer->WriteByte(m_ConsoleNeedEnter);
	writer->WriteByte(m_CircleTransRadius);
	writer->WriteByte(m_SkillReport);
	writer->WriteWord(m_SpeechFont);
	writer->WriteBuffer();
	
	//Page 10
	writer->WriteByte(2); //size of block
	writer->WriteByte(10); //page index
	writer->WriteBuffer();
	
	//No page
	writer->WriteByte(6); //size of block
	writer->WriteByte(0); //page index
	writer->WriteWord(g_GameWindowPosX);
	writer->WriteWord(g_GameWindowPosY);
	writer->WriteBuffer();
	
	/*b = (BYTE)m_FilterPWOn;
	fwrite(&b, 1, 1, file);
	b = (BYTE)m_ObscenityFilter;
	fwrite(&b, 1, 1, file);
	w = (WORD)m_FilterPassword.length();
	fwrite(&w, 2, 1, file);
	if (w)
		fwrite(&((char*)m_FilterPassword.c_str())[0], w, 1, file);*/

	delete writer;
}
//---------------------------------------------------------------------------