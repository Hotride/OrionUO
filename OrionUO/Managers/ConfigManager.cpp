/***********************************************************************************
**
** ConfigManager.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CConfigManager g_ConfigManager;
CConfigManager g_OptionsConfig;
//----------------------------------------------------------------------------------
CConfigManager::CConfigManager()
{
	WISPFUN_DEBUG("c138_f1");
	Init();
}
//----------------------------------------------------------------------------------
/*!
Инициализация
@return 
*/
void CConfigManager::Init()
{
	WISPFUN_DEBUG("c138_f2");
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
	
	m_ToggleBufficonWindow = true;

	GameWindowX = 0;
	GameWindowY = 0;

	m_UpdateRange = MAX_VIEW_RANGE;
}
//---------------------------------------------------------------------------
void CConfigManager::DefaultPage1()
{
	WISPFUN_DEBUG("c138_f3");
	m_Sound = true;
	m_Music = false;
	m_FootstepsSound = true;
	m_CombatMusic = true;
	m_SoundVolume = 255;
	m_MusicVolume = 255;
}
//---------------------------------------------------------------------------
void CConfigManager::DefaultPage2()
{
	WISPFUN_DEBUG("c138_f4");
	m_ClientFPS = 32;
	m_ReduceFPSUnactiveWindow = true;
	m_StandartCharactersAnimationDelay = false;
	m_StandartItemsAnimationDelay = true;
	m_UseScaling = false;
	m_RemoveTextWithBlending = true;
	m_DrawStatusState = DCSS_NO_DRAW;
	m_DrawStatusConditionState = DCSCS_LOWER;
	m_DrawStatusConditionValue = 70;
	m_DrawStumps = false;
	m_MarkingCaves = false;
	m_NoVegetation = false;
	m_NoAnimateFields = false;
	m_LockGumpsMoving = true;
	m_ConsoleNeedEnter = false;
	m_HiddenCharactersRenderMode = 0;
	m_HiddenAlpha = 0x7F;
	m_UseHiddenModeOnlyForSelf = true;
	m_TransparentSpellIcons = true;
	m_SpellIconAlpha = 0x7F;
	m_OldStyleStatusbar = false;
	m_ApplyStateColorOnCharacters = false;
	m_OriginalPartyStatusbar = false;
	m_ChangeFieldsGraphic = false;
	m_PaperdollSlots = true;
	m_RemoveStatusbarsWithoutObjects = false;
	m_ShowDefaultConsoleEntryMode = true;
	m_DrawAuraState = DAS_ALWAYS;
	m_DrawAuraWithCtrlPressed = true;
	m_ScreenshotFormat = SF_PNG;
	m_ScaleImagesInPaperdollSlots = true;
	m_RemoveOrCreateObjectsWithBlending = true;
	m_DrawHelmetsOnShroud = false;
	m_UseGlobalMapLayer = false;
	m_NoDrawRoofs = false;
	m_HighlightTargetByType = true;
	m_AutoDisplayWorldMap = false;
}
//---------------------------------------------------------------------------
void CConfigManager::DefaultPage3()
{
	WISPFUN_DEBUG("c138_f5");
	m_UseToolTips = true;
	m_ToolTipsTextColor = 0xFFFF;
	m_ToolTipsTextFont = 0;
	m_ToolTipsDelay = 200;
}
//---------------------------------------------------------------------------
void CConfigManager::DefaultPage4()
{
	WISPFUN_DEBUG("c138_f6");
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
	
	m_ChatFont = 0;
}
//---------------------------------------------------------------------------
void CConfigManager::DefaultPage6()
{
	WISPFUN_DEBUG("c138_f7");
	m_EnablePathfind = true;
	m_HoldTabForCombat = true;
	m_OffsetInterfaceWindows = true;
	m_AutoArrange = true;
	m_AlwaysRun = false;
	m_DisableMenubar = false;
	m_GrayOutOfRangeObjects = false;
	m_DisableNewTargetSystem = true;
	m_ItemPropertiesMode = 0;
	m_ItemPropertiesIcon = false;
	m_ObjectHandles = false;
	m_HoldShiftForContextMenus = false;
	m_HoldShiftForEnablePathfind = false;
}
//---------------------------------------------------------------------------
void CConfigManager::DefaultPage7()
{
	WISPFUN_DEBUG("c138_f8");
	m_GameWindowWidth = 800;
	m_GameWindowHeight = 600;
	m_SpeechDelay = 500;
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
	m_LockResizingGameWindow = false;
}
//---------------------------------------------------------------------------
void CConfigManager::DefaultPage8()
{
	WISPFUN_DEBUG("c138_f9");
	m_InnocentColor = 0x005A;
	m_FriendlyColor = 0x0044;
	m_SomeoneColor = 0x03B2;
	m_CriminalColor = 0x03B2;
	m_EnemyColor = 0x002B;
	m_MurdererColor = 0x0023;
	m_CriminalActionsQuery = true;
}
//---------------------------------------------------------------------------
void CConfigManager::DefaultPage9()
{
	WISPFUN_DEBUG("c138_f10");
	m_ShowIncomingNames = true;
	m_UseCircleTrans = false;
	m_StatReport = true;
	m_CircleTransRadius = 64;
	m_SkillReport = 1;
	m_SpeechFont = 0;
}
//---------------------------------------------------------------------------
void CConfigManager::OnChangedSound(const bool &val)
{
	WISPFUN_DEBUG("c138_f11");
	if (this == &g_ConfigManager && !val)
		g_Orion.AdjustSoundEffects(g_Ticks + 100000);
}
//---------------------------------------------------------------------------
void CConfigManager::OnChangedMusic(const bool &val)
{
	WISPFUN_DEBUG("c138_f12");
	if (this == &g_ConfigManager && !val)
	{
		g_SoundManager.StopMusic();
	}
}
//---------------------------------------------------------------------------
void CConfigManager::OnChangedSoundVolume(const uchar &val)
{
	WISPFUN_DEBUG("c138_f13");
	if (this == &g_ConfigManager && m_SoundVolume != val)
		g_Orion.AdjustSoundEffects(g_Ticks + 100000, val);
}
//---------------------------------------------------------------------------
void CConfigManager::OnChangedMusicVolume(const uchar &val)
{
	WISPFUN_DEBUG("c138_f14");
	if (this == &g_ConfigManager && m_MusicVolume != val)
	{
		m_MusicVolume = val;
		g_SoundManager.SetMusicVolume(g_SoundManager.GetVolumeValue(-1, true));
	}
}
//---------------------------------------------------------------------------
void CConfigManager::OnChangeClientFPS(const uchar &val)
{
	WISPFUN_DEBUG("c138_f15");
	if (this == &g_ConfigManager)
	{
		uchar newVal = val;
		if (newVal < 16)
			newVal = 16;
		else if (newVal > 64)
			newVal = 64;

		g_FrameDelay[1] = 1000 / newVal;

		if (!m_ReduceFPSUnactiveWindow)
			g_FrameDelay[0] = g_FrameDelay[1];

		g_OrionWindow.SetRenderTimerDelay(g_FrameDelay[g_OrionWindow.IsActive()]);
	}
}
//---------------------------------------------------------------------------
void CConfigManager::OnChangeUseScaling(const bool &val)
{
	WISPFUN_DEBUG("c138_f16");
	if (!val && this == &g_ConfigManager)
		g_GlobalScale = 1.0;
}
//---------------------------------------------------------------------------
void CConfigManager::OnChangeDrawStatusState(const uchar &val)
{
	WISPFUN_DEBUG("c138_f17");
	if (val && !m_DrawStatusState && this == &g_ConfigManager)
	{
		QFOR(item, g_World->m_Items, CGameObject*)
		{
			if (item->NPC)
				CPacketStatusRequest(item->Serial).Send();
		}
	}
}
//---------------------------------------------------------------------------
void CConfigManager::OnChangeDrawStumps(const bool &val)
{
	WISPFUN_DEBUG("c138_f18");
	if (m_DrawStumps != val && this == &g_ConfigManager)
		g_Orion.ClearTreesTextures();
}
//---------------------------------------------------------------------------
void CConfigManager::OnChangeMarkingCaves(const bool &val)
{
	WISPFUN_DEBUG("c138_f19");
	if (m_MarkingCaves != val && this == &g_ConfigManager)
		g_Orion.ClearCaveTextures();
}
//---------------------------------------------------------------------------
void CConfigManager::OnChangeReduceFPSUnactiveWindow(const bool &val)
{
	WISPFUN_DEBUG("c138_f20");
	if (this == &g_ConfigManager)
	{
		if (val)
			g_FrameDelay[0] = FRAME_DELAY_UNACTIVE_WINDOW;
		else
			g_FrameDelay[0] = g_FrameDelay[1];

		g_OrionWindow.SetRenderTimerDelay(g_FrameDelay[g_OrionWindow.IsActive()]);
	}
}
//---------------------------------------------------------------------------
void CConfigManager::OnChangeConsoleNeedEnter(const bool &val)
{
	WISPFUN_DEBUG("c138_f21");
	if (this == &g_ConfigManager && val && g_EntryPointer == &g_GameConsole)
		g_EntryPointer = NULL;
}
//---------------------------------------------------------------------------
void CConfigManager::OnChangedSpellIconAlpha(const uchar &val)
{
	WISPFUN_DEBUG("c138_f22");
	if (this == &g_ConfigManager && val != m_SpellIconAlpha)
	{
		float alpha = val / 255.0f;
		bool redraw = g_ConfigManager.TransparentSpellIcons;

		QFOR(gump, g_GumpManager.m_Items, CGump*)
		{
			if (gump->GumpType == GT_SPELL)
			{
				((CGumpSpell*)gump)->m_Blender->Alpha = alpha;

				if (redraw)
					gump->WantRedraw = true;
			}
		}
	}
}
//---------------------------------------------------------------------------
void CConfigManager::OnChangeOldStyleStatusbar(const bool &val)
{
	WISPFUN_DEBUG("c138_f23");
	if (this == &g_ConfigManager)
	{
		m_OldStyleStatusbar = val;

		CGump *gump = g_GumpManager.UpdateGump(g_PlayerSerial, 0, GT_STATUSBAR);

		if (gump != NULL && !gump->Minimized)
			gump->UpdateContent();
	}
}
//---------------------------------------------------------------------------
void CConfigManager::OnChangeOriginalPartyStatusbar(const bool &val)
{
	WISPFUN_DEBUG("c138_f24");
	if (this == &g_ConfigManager)
	{
		m_OriginalPartyStatusbar = val;

		if (g_Party.Leader != 0)
		{
			QFOR(gump, g_GumpManager.m_Items, CGump*)
			{
				if (gump->GumpType == GT_STATUSBAR && (g_Party.Leader == gump->Serial || g_Party.Contains(gump->Serial)))
				{
					gump->WantRedraw = true;
					gump->UpdateContent();
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void CConfigManager::OnChangeChangeFieldsGraphic(const bool &val)
{
	WISPFUN_DEBUG("c138_f25");
	if (this == &g_ConfigManager && g_World != NULL)
	{
		m_ChangeFieldsGraphic = val;

		QFOR(item, g_World->m_Items, CGameObject*)
		{
			if (!item->NPC)
				((CGameItem*)item)->CalculateFieldColor();
		}
	}
}
//---------------------------------------------------------------------------
void CConfigManager::OnChangePaperdollSlots(const bool &val)
{
	WISPFUN_DEBUG("c138_f26");
	if (this == &g_ConfigManager && g_World != NULL)
	{
		m_PaperdollSlots = val;

		QFOR(gump, g_GumpManager.m_Items, CGump*)
		{
			if (gump->GumpType == GT_PAPERDOLL)
			{
				gump->UpdateContent();
				gump->WantRedraw = true;
			}
		}
	}
}
//---------------------------------------------------------------------------
void CConfigManager::OnChangeScaleImagesInPaperdollSlots(const bool &val)
{
	WISPFUN_DEBUG("c138_f27");
	if (this == &g_ConfigManager && g_World != NULL)
	{
		m_ScaleImagesInPaperdollSlots = val;

		QFOR(gump, g_GumpManager.m_Items, CGump*)
		{
			if (gump->GumpType == GT_PAPERDOLL)
			{
				gump->UpdateContent();
				gump->WantRedraw = true;
			}
		}
	}
}
//---------------------------------------------------------------------------
void CConfigManager::OnChangeUseGlobalMapLayer(const bool &val)
{
	WISPFUN_DEBUG("c138_f28");
	if (this == &g_ConfigManager && val)
	{
		QFOR(gump, g_GumpManager.m_Items, CGump*)
		{
			if (gump->GumpType == GT_WORLD_MAP)
				g_GumpManager.MoveToFront(gump);
		}
	}
}
//---------------------------------------------------------------------------
void CConfigManager::OnChangeNoDrawRoofs(const bool &val)
{
	WISPFUN_DEBUG("c138_f28");
	if (this == &g_ConfigManager && g_Player != NULL)
	{
		g_Player->OldX = 0;
		g_Player->OldY = 0;
	}
}
//---------------------------------------------------------------------------
/*!
Получить цвет исходя из "злобности"
@param [__in] notoriety Злобность
@return Индекс цвета
*/
ushort CConfigManager::GetColorByNotoriety(uchar notoriety)
{
	WISPFUN_DEBUG("c138_f29");
	ushort color = 0;

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
		case NT_INVULNERABLE:
		{
			color = 0x0034;
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
bool CConfigManager::LoadBin(string path)
{
	WISPFUN_DEBUG("c138_f30");
	bool result = false;
	g_DeveloperMode = DM_SHOW_FPS_ONLY;

	WISP_FILE::CMappedFile file;

	if (file.Load(path) && file.Size)
	{
		uchar version = file.ReadUInt8();
		
		//Page 1
		puchar next = file.Ptr;
		char blockSize = file.ReadInt8();
		next += blockSize;

		if (file.ReadInt8() == 1)
		{
			Sound = file.ReadUInt8();
			Music = file.ReadUInt8();
			m_FootstepsSound = file.ReadUInt8();
			m_CombatMusic = file.ReadUInt8();
			m_SoundVolume = file.ReadUInt8();
			m_MusicVolume = file.ReadUInt8();
		}
		
		file.Ptr = next;
		
		//Page 2
		next = file.Ptr;
		blockSize = file.ReadInt8();
		next += blockSize;

		m_UseScaling = false;
		m_RemoveTextWithBlending = true;
		m_DrawStatusState = DCSS_NO_DRAW;
		m_DrawStatusConditionState = DCSCS_LOWER;
		m_DrawStatusConditionValue = 70;
		bool drawStumps = false;
		bool markingCaves = false;
		m_NoAnimateFields = false;
		m_NoVegetation = false;
		m_TransparentSpellIcons = true;
		m_SpellIconAlpha = 0x7F;
		m_OldStyleStatusbar = false;
		m_OriginalPartyStatusbar = false;
		m_ApplyStateColorOnCharacters = false;
		bool changeFieldsGraphic = false;
		bool paperdollSlots = true;
		m_RemoveStatusbarsWithoutObjects = false;
		m_ShowDefaultConsoleEntryMode = true;
		m_DrawAuraState = DAS_ALWAYS;
		m_DrawAuraWithCtrlPressed = true;
		m_ScreenshotFormat = SF_PNG;
		bool scaleImagesInPaperdollSlots = true;
		m_DrawHelmetsOnShroud = false;
		bool useGlobalMapLayer = false;
		bool noDrawRoofs = false;
		m_HighlightTargetByType = true;
		m_AutoDisplayWorldMap = false;

		if (file.ReadInt8() == 2)
		{
			if (blockSize > 23)
			{
				ClientFPS = file.ReadUInt8();
				m_UseScaling = file.ReadUInt8();
				m_RemoveTextWithBlending = file.ReadUInt8();
				m_DrawStatusState = file.ReadUInt8();
				drawStumps = file.ReadUInt8();
				markingCaves = file.ReadUInt8();
				m_NoAnimateFields = file.ReadUInt8();
				m_NoVegetation = file.ReadUInt8();
				m_HiddenCharactersRenderMode = file.ReadUInt8();
				m_HiddenAlpha = file.ReadUInt8();
				m_UseHiddenModeOnlyForSelf = file.ReadUInt8();
				m_TransparentSpellIcons = file.ReadUInt8();
				m_SpellIconAlpha = file.ReadUInt8();
				m_OldStyleStatusbar = file.ReadUInt8();
				m_OriginalPartyStatusbar = file.ReadUInt8();
				m_ApplyStateColorOnCharacters = file.ReadUInt8();
				changeFieldsGraphic = file.ReadUInt8();
				paperdollSlots = file.ReadUInt8();
				m_DrawStatusConditionState = file.ReadUInt8();
				m_DrawStatusConditionValue = file.ReadUInt8();
				m_RemoveStatusbarsWithoutObjects = file.ReadUInt8();

				m_ShowDefaultConsoleEntryMode = file.ReadUInt8();

				if (blockSize > 24)
				{
					uchar auraState = file.ReadUInt8();

					m_DrawAuraState = auraState & 0x7F;
					m_DrawAuraWithCtrlPressed = (auraState & 0x80);

					if (blockSize > 25)
					{
						m_ScreenshotFormat = file.ReadUInt8();

						if (blockSize > 26)
						{
							scaleImagesInPaperdollSlots = file.ReadUInt8();

							if (blockSize > 27)
							{
								m_RemoveOrCreateObjectsWithBlending = file.ReadUInt8();

								if (blockSize > 28)
								{
									m_DrawHelmetsOnShroud = file.ReadUInt8();

									if (blockSize > 29)
									{
										useGlobalMapLayer = file.ReadUInt8();

										if (blockSize > 30)
										{
											noDrawRoofs = file.ReadUInt8();

											if (blockSize > 31)
												m_HighlightTargetByType = file.ReadUInt8();
										}
									}
								}
							}
						}
					}
				}
			}
			else if (blockSize > 2)
			{
				ClientFPS = file.ReadUInt8();

				if (blockSize > 3)
				{
					m_UseScaling = file.ReadUInt8();

					if (blockSize > 4)
					{
						m_RemoveTextWithBlending = file.ReadUInt8();
						m_DrawStatusState = file.ReadUInt8();

						if (blockSize > 6)
						{
							drawStumps = file.ReadUInt8();
							markingCaves = file.ReadUInt8();
							m_NoAnimateFields = file.ReadUInt8();

							if (blockSize > 9)
							{
								m_NoVegetation = file.ReadUInt8();
								m_HiddenCharactersRenderMode = file.ReadUInt8();
								m_HiddenAlpha = file.ReadUInt8();
								m_UseHiddenModeOnlyForSelf = file.ReadUInt8();

								if (blockSize > 13)
								{
									m_TransparentSpellIcons = file.ReadUInt8();
									m_SpellIconAlpha = file.ReadUInt8();

									if (blockSize > 15)
									{
										m_OldStyleStatusbar = file.ReadUInt8();
										m_OriginalPartyStatusbar = file.ReadUInt8();
										m_ApplyStateColorOnCharacters = file.ReadUInt8();

										if (blockSize > 18)
										{
											changeFieldsGraphic = file.ReadUInt8();

											if (blockSize > 19)
											{
												paperdollSlots = file.ReadUInt8();

												if (blockSize > 21)
												{
													m_DrawStatusConditionState = file.ReadUInt8();
													m_DrawStatusConditionValue = file.ReadUInt8();

													if (blockSize > 22)
														m_RemoveStatusbarsWithoutObjects = file.ReadUInt8();
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
			else
				ClientFPS = 32;
		}
		else
			ClientFPS = 32;

		DrawStumps = drawStumps;
		MarkingCaves = markingCaves;
		ChangeFieldsGraphic = changeFieldsGraphic;
		PaperdollSlots = paperdollSlots;
		ScaleImagesInPaperdollSlots = scaleImagesInPaperdollSlots;
		UseGlobalMapLayer = useGlobalMapLayer;
		NoDrawRoofs = noDrawRoofs;
		
		file.Ptr = next;
		
		//Page 3
		next = file.Ptr;
		blockSize = file.ReadInt8();
		next += blockSize;

		if (file.ReadInt8() == 3)
		{
			m_UseToolTips = file.ReadUInt8();
			m_ToolTipsTextColor = file.ReadUInt16LE();
			m_ToolTipsTextFont = file.ReadUInt16LE();
			m_ToolTipsDelay = file.ReadUInt16LE();
		}
		
		file.Ptr = next;
		
		//Page 4
		next = file.Ptr;
		blockSize = file.ReadInt8();
		next += blockSize;

		if (file.ReadInt8() == 4)
		{
			m_ChatColorInputText = file.ReadUInt16LE();
			m_ChatColorMenuOption = file.ReadUInt16LE();
			m_ChatColorPlayerInMemberList = file.ReadUInt16LE();
			m_ChatColorText = file.ReadUInt16LE();
			m_ChatColorPlayerNameWithout = file.ReadUInt16LE();
			m_ChatColorMuted = file.ReadUInt16LE();
			m_ChatColorChannelModeratorName = file.ReadUInt16LE();
			m_ChatColorChannelModeratorText = file.ReadUInt16LE();
			m_ChatColorMyName = file.ReadUInt16LE();
			m_ChatColorMyText = file.ReadUInt16LE();
			m_ChatColorSystemMessage = file.ReadUInt16LE();
			m_ChatFont = file.ReadUInt16LE();
			m_ChatColorBGOutputText = file.ReadUInt16LE();
			m_ChatColorBGInputText = file.ReadUInt16LE();
			m_ChatColorBGUserList = file.ReadUInt16LE();
			m_ChatColorBGConfList = file.ReadUInt16LE();
			m_ChatColorBGCommandList = file.ReadUInt16LE();
		}
		
		file.Ptr = next;
		
		//Page 6
		next = file.Ptr;
		blockSize = file.ReadInt8();
		next += blockSize;

		if (file.ReadInt8() == 6)
		{
			m_EnablePathfind = file.ReadUInt8();
			m_HoldTabForCombat = file.ReadUInt8();
			m_OffsetInterfaceWindows = file.ReadUInt8();
			m_AutoArrange = file.ReadUInt8();
			m_AlwaysRun = file.ReadUInt8();
			m_DisableMenubar = file.ReadUInt8();
			m_GrayOutOfRangeObjects = file.ReadUInt8();
			m_HoldShiftForContextMenus = true;
			m_HoldShiftForEnablePathfind = false;

			if (blockSize > 9)
			{
				m_DisableNewTargetSystem = file.ReadUInt8();
				m_ItemPropertiesMode = file.ReadUInt8();
				m_ItemPropertiesIcon = file.ReadUInt8();
				m_ObjectHandles = file.ReadUInt8();
				ReduceFPSUnactiveWindow = file.ReadUInt8();

				if (blockSize > 14)
				{
					m_HoldShiftForContextMenus = file.ReadUInt8();

					if (blockSize > 15)
					{
						m_HoldShiftForEnablePathfind = file.ReadUInt8();

						if (blockSize > 16)
						{
							g_ContainerRect.DefaultX = file.ReadUInt16LE();
							g_ContainerRect.DefaultY = file.ReadUInt16LE();
						}
					}
				}
			}
			else
			{
				m_DisableNewTargetSystem = true;
				m_ItemPropertiesMode = 0;
				m_ItemPropertiesIcon = false;
				m_ObjectHandles = false;
				ReduceFPSUnactiveWindow = true;
			}
		}
		
		file.Ptr = next;
		
		//Page 7
		next = file.Ptr;
		blockSize = file.ReadInt8();
		next += blockSize;

		if (file.ReadInt8() == 7)
		{
			m_GameWindowWidth = file.ReadUInt16LE();
			m_GameWindowHeight = file.ReadUInt16LE();
			m_SpeechDelay = file.ReadUInt16LE();
			m_ScaleSpeechDelay = file.ReadUInt16LE();
			m_SpeechColor = file.ReadUInt16LE();
			m_EmoteColor = file.ReadUInt16LE();
			m_PartyMessageColor = file.ReadUInt16LE();
			m_GuildMessageColor = file.ReadUInt16LE();
			m_AllianceMessageColor = file.ReadUInt16LE();
			m_IgnoreGuildMessage = file.ReadUInt8();
			m_IgnoreAllianceMessage = file.ReadUInt8();
			m_DarkNights = file.ReadUInt8();
			m_ColoredLighting = file.ReadUInt8();
			m_StandartCharactersAnimationDelay = file.ReadUInt8();
			m_StandartItemsAnimationDelay = file.ReadUInt8();
			m_LockResizingGameWindow = file.ReadUInt8();
			m_LockGumpsMoving = file.ReadUInt8();
		}
		
		file.Ptr = next;
		
		//Page 8
		next = file.Ptr;
		blockSize = file.ReadInt8();
		next += blockSize;

		if (file.ReadInt8() == 8)
		{
			m_InnocentColor = file.ReadUInt16LE();
			m_FriendlyColor = file.ReadUInt16LE();
			m_SomeoneColor = file.ReadUInt16LE();
			m_CriminalColor = file.ReadUInt16LE();
			m_EnemyColor = file.ReadUInt16LE();
			m_MurdererColor = file.ReadUInt16LE();
			m_CriminalActionsQuery = file.ReadUInt8();
		}
		
		file.Ptr = next;
		
		//Page 9
		next = file.Ptr;
		blockSize = file.ReadInt8();
		next += blockSize;

		if (file.ReadInt8() == 9)
		{
			m_ShowIncomingNames = file.ReadUInt8();
			m_UseCircleTrans = file.ReadUInt8();
			m_StatReport = file.ReadUInt8();
			ConsoleNeedEnter = file.ReadUInt8();
			m_CircleTransRadius = file.ReadUInt8();
			m_SkillReport = file.ReadUInt8();
			m_SpeechFont = file.ReadUInt16LE();
		}
		
		g_CircleOfTransparency.Create(m_CircleTransRadius);

		file.Ptr = next;
		
		//Page 10
		next = file.Ptr;
		blockSize = file.ReadInt8();
		next += blockSize;

		if (file.ReadInt8() == 10)
		{
		}
		
		file.Ptr = next;
		
		//No page
		blockSize = file.ReadInt8();

		if (!file.ReadInt8())
		{
			m_GameWindowX = file.ReadInt16LE();

			if (m_GameWindowX < 0)
				m_GameWindowX = 0;

			m_GameWindowY = file.ReadInt16LE();

			if (m_GameWindowY < 0)
				m_GameWindowY = 0;

			if (blockSize > 6)
			{
				bool zoomed = file.ReadUInt8();

				int windowX = file.ReadInt16LE();

				if (windowX < 0)
					windowX = 0;

				int windowY = file.ReadInt16LE();

				if (windowY < 0)
					windowY = 0;

				int windowWidth = file.ReadInt16LE();
				int windowHeight = file.ReadInt16LE();

				if (g_GameState >= GS_GAME)
				{
					if (windowWidth < 640)
						windowWidth = 640;

					if (windowWidth >= GetSystemMetrics(SM_CXSCREEN) - 20)
						windowWidth = GetSystemMetrics(SM_CXSCREEN) - 20;

					if (windowHeight < 480)
						windowHeight = 480;

					if (windowHeight >= GetSystemMetrics(SM_CYSCREEN) - 60)
						windowHeight = GetSystemMetrics(SM_CYSCREEN) - 60;
				}
				else
				{
					windowWidth = 640;
					windowHeight = 480;
				}

				SendMessage(g_OrionWindow.Handle, WM_SYSCOMMAND, SC_RESTORE, 0);

				if (zoomed)
					SendMessage(g_OrionWindow.Handle, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
				else
					SetWindowPos(g_OrionWindow.Handle, NULL, windowX, windowY, windowWidth, windowHeight, 0);

				g_GL.UpdateRect();

				if (blockSize > 15)
				{
					m_ToggleBufficonWindow = file.ReadUInt8();

					if (blockSize == 17)
						g_DeveloperMode = (DEVELOPER_MODE)file.ReadUInt8();
				}
			}
			else
			{
				SendMessage(g_OrionWindow.Handle, WM_SYSCOMMAND, SC_RESTORE, 0);
				SendMessage(g_OrionWindow.Handle, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
			}
		}

		result = true;
	}
	/*else
	{
		Init();

		if (g_GameState >= GS_GAME)
		{
			SendMessage(g_hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
			SendMessage(g_hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		}
	}*/

	if (g_GameState >= GS_GAME)
	{
		if (m_GameWindowWidth < 640)
			m_GameWindowWidth = 640;

		if (m_GameWindowWidth >= GetSystemMetrics(SM_CXSCREEN) - 20)
			m_GameWindowWidth = GetSystemMetrics(SM_CXSCREEN) - 20;

		if (m_GameWindowHeight < 480)
			m_GameWindowHeight = 480;

		if (m_GameWindowHeight >= GetSystemMetrics(SM_CYSCREEN) - 60)
			m_GameWindowHeight = GetSystemMetrics(SM_CYSCREEN) - 60;
	}
	else
	{
		m_GameWindowWidth = 640;
		m_GameWindowHeight = 480;
	}

	file.Unload();

	return result;
}
//---------------------------------------------------------------------------
int CConfigManager::GetConfigKeyCode(const string &key)
{
	WISPFUN_DEBUG("");

	static const string keys[CMKC_COUNT] =
	{
		"Sound",
		"SoundVolume",
		"Music",
		"MusicVolume",
		"FootstepsSound",
		"CombatMusic",
		"ClientFPS",
		"UseScaling",
		"RemoveTextWithBlending",
		"DrawStatusState",
		"DrawStumps",
		"MarkingCaves",
		"NoAnimateFields",
		"NoVegetation",
		"HiddenCharactersRenderMode",
		"HiddenAlpha",
		"UseHiddenModeOnlyForSelf",
		"TransparentSpellIcons",
		"SpellIconAlpha",
		"OldStyleStatusbar",
		"OriginalPartyStatusbar",
		"ApplyStateColorOnCharacters",
		"ChangeFieldsGraphic",
		"PaperdollSlots",
		"DrawStatusConditionState",
		"DrawStatusConditionValue",
		"RemoveStatusbarsWithoutObjects",
		"ShowDefaultConsoleEntryMode",
		"DrawAuraState",
		"DrawAuraWithCtrlPressed",
		"ScreenshotFormat",
		"ScaleImagesInPaperdollSlots",
		"RemoveOrCreateObjectsWithBlending",
		"DrawHelmetsOnShroud",
		"UseGlobalMapLayer",
		"NoDrawRoofs",
		"HighlightTargetByType",
		"AutoDisplayWorldMap",
		"UseToolTips",
		"ToolTipsTextColor",
		"ToolTipsTextFont",
		"ToolTipsDelay",
		"ChatColorInputText",
		"ChatColorMenuOption",
		"ChatColorPlayerInMemberList",
		"ChatColorText",
		"ChatColorPlayerNameWithout",
		"ChatColorMuted",
		"ChatColorChannelModeratorName",
		"ChatColorChannelModeratorText",
		"ChatColorMyName",
		"ChatColorMyText",
		"ChatColorSystemMessage",
		"ChatFont",
		"ChatColorBGOutputText",
		"ChatColorBGInputText",
		"ChatColorBGUserList",
		"ChatColorBGConfList",
		"ChatColorBGCommandList",
		"EnablePathfind",
		"HoldTabForCombat",
		"OffsetInterfaceWindows",
		"AutoArrange",
		"AlwaysRun",
		"DisableMenubar",
		"GrayOutOfRangeObjects",
		"DisableNewTargetSystem",
		"ItemPropertiesMode",
		"ItemPropertiesIcon",
		"ObjectHandles",
		"ReduceFPSUnactiveWindow",
		"HoldShiftForContextMenus",
		"HoldShiftForEnablePathfind",
		"ContainerDefaultX",
		"ContainerDefaultY",
		"GameWindowWidth",
		"GameWindowHeight",
		"SpeechDelay",
		"ScaleSpeechDelay",
		"SpeechColor",
		"EmoteColor",
		"PartyMessageColor",
		"GuildMessageColor",
		"AllianceMessageColor",
		"IgnoreGuildMessage",
		"IgnoreAllianceMessage",
		"DarkNights",
		"ColoredLighting",
		"StandartCharactersAnimationDelay",
		"StandartItemsAnimationDelay",
		"LockResizingGameWindow",
		"LockGumpsMoving",
		"InnocentColor",
		"FriendlyColor",
		"SomeoneColor",
		"CriminalColor",
		"EnemyColor",
		"MurdererColor",
		"CriminalActionsQuery",
		"ShowIncomingNames",
		"UseCircleTrans",
		"StatReport",
		"ConsoleNeedEnter",
		"CircleTransRadius",
		"SkillReport",
		"SpeechFont",
		"GameWindowX",
		"GameWindowY",
		"Zoomed",
		"RealX",
		"RealY",
		"RealWidth",
		"RealHeight",
		"ToggleBufficonWindow",
		"DeveloperMode"
	};

	string str = ToLowerA(key);

	IFOR(i, 0, CMKC_COUNT)
	{
		if (str == ToLowerA(keys[i]))
			return i;
	}

	return -1;
}
//---------------------------------------------------------------------------
bool CConfigManager::Load(const string &path)
{
	WISP_FILE::CTextFileParser file(path, "=", "#;", "");

	bool zoomed = false;
	int windowX = -1;
	int windowY = -1;
	int windowWidth = -1;
	int windowHeight = -1;

	while (!file.IsEOF())
	{
		std::vector<std::string> strings = file.ReadTokens();

		if (strings.size() >= 2)
		{
			int code = GetConfigKeyCode(strings[0]);

			if (code == -1)
				continue;

			switch (code)
			{
				//Page 1
				case CMKC_SOUND:
					Sound = ToBool(strings[1]);
					break;
				case CMKC_SOUND_VOLUME:
					m_SoundVolume = atoi(strings[1].c_str());
					break;
				case CMKC_MUSIC:
					Music = ToBool(strings[1]);
					break;
				case CMKC_MUSIC_VOLUME:
					m_MusicVolume = atoi(strings[1].c_str());
					break;
				case CMKC_FOOTSTEPS_SOUND:
					m_FootstepsSound = ToBool(strings[1]);
					break;
				case CMKC_COMBAT_MUSIC:
					m_CombatMusic = ToBool(strings[1]);
					break;

				//Page 2
				case CMKC_CLIENT_FPS:
					ClientFPS = atoi(strings[1].c_str());
					break;
				case CMKC_USE_SCALING:
					m_UseScaling = ToBool(strings[1]);
					break;
				case CMKC_REMOVE_TEXT_WITH_BLENDING:
					m_RemoveTextWithBlending = ToBool(strings[1]);
					break;
				case CMKC_DRAW_STATUS_STATE:
					m_DrawStatusState = atoi(strings[1].c_str());
					break;
				case CMKC_DRAW_STUMPS:
					DrawStumps = ToBool(strings[1]);
					break;
				case CMKC_MARKING_CAVES:
					MarkingCaves = ToBool(strings[1]);
					break;
				case CMKC_NO_ANIMATE_FIELDS:
					m_NoAnimateFields = ToBool(strings[1]);
					break;
				case CMKC_NO_VEGETATION:
					m_NoVegetation = ToBool(strings[1]);
					break;
				case CMKC_HIDDEN_CHARACTERS_RENDER_MODE:
					m_HiddenCharactersRenderMode = atoi(strings[1].c_str());
					break;
				case CMKC_HIDDEN_ALPHA:
					m_HiddenAlpha = atoi(strings[1].c_str());
					break;
				case CMKC_USE_HIDDEN_MODE_ONLY_FOR_SELF:
					m_UseHiddenModeOnlyForSelf = ToBool(strings[1]);
					break;
				case CMKC_TRANSPARENT_SPELL_ICONS:
					m_TransparentSpellIcons = atoi(strings[1].c_str());
					break;
				case CMKC_SPELL_ICON_ALPHA:
					m_SpellIconAlpha = atoi(strings[1].c_str());
					break;
				case CMKC_OLD_STYLE_STATUSBAR:
					m_OldStyleStatusbar = ToBool(strings[1]);
					break;
				case CMKC_ORIGINAL_PARTY_STATUSBAR:
					m_OriginalPartyStatusbar = ToBool(strings[1]);
					break;
				case CMKC_APPLY_STATE_COLOR_ON_CHARACTERS:
					m_ApplyStateColorOnCharacters = ToBool(strings[1]);
					break;
				case CMKC_CHANGE_FIELDS_GRAPHIC:
					ChangeFieldsGraphic = ToBool(strings[1]);
					break;
				case CMKC_PAPERDOLL_SLOTS:
					PaperdollSlots = ToBool(strings[1]);
					break;
				case CMKC_DRAW_STATUS_CONDITION_STATE:
					m_DrawStatusConditionState = atoi(strings[1].c_str());
					break;
				case CMKC_DRAW_STATUS_CONDITION_VALUE:
					m_DrawStatusConditionValue = atoi(strings[1].c_str());
					break;
				case CMKC_REMOVE_STATUSBARS_WITHOUT_OBJECTS:
					m_RemoveStatusbarsWithoutObjects = ToBool(strings[1]);
					break;
				case CMKC_SHOW_DEFAULT_CONSOLE_ENTRY_MODE:
					m_ShowDefaultConsoleEntryMode = ToBool(strings[1]);
					break;
				case CMKC_DRAW_AURA_STATE:
					m_DrawAuraState = atoi(strings[1].c_str());
					break;
				case CMKC_DRAW_AURA_WITH_CTRL_PRESSED:
					m_DrawAuraWithCtrlPressed = ToBool(strings[1]);
					break;
				case CMKC_SCREENSHOT_FORMAT:
					m_ScreenshotFormat = atoi(strings[1].c_str());
					break;
				case CMKC_SCALE_IMAGES_IN_PAPERDOLL_SLOTS:
					ScaleImagesInPaperdollSlots = ToBool(strings[1]);
					break;
				case CMKC_REMOVE_OR_CREATE_OBJECTS_WITH_BLENDING:
					m_RemoveOrCreateObjectsWithBlending = ToBool(strings[1]);
					break;
				case CMKC_DRAW_HELMETS_ON_SHROUD:
					m_DrawHelmetsOnShroud = ToBool(strings[1]);
					break;
				case CMKC_USE_GLOBAL_MAP_LAYER:
					UseGlobalMapLayer = ToBool(strings[1]);
					break;
				case CMKC_NO_DRAW_ROOFS:
					NoDrawRoofs = ToBool(strings[1]);
					break;
				case CMKC_HIGHLIGHT_TARGET_BY_TYPE:
					m_HighlightTargetByType = ToBool(strings[1]);
					break;
				case CMKC_AUTO_DISPLAY_WORLD_MAP:
					m_AutoDisplayWorldMap = ToBool(strings[1]);
					break;

				//Page 3
				case CMKC_USE_TOOLTIPS:
					m_UseToolTips = ToBool(strings[1]);
					break;
				case CMKC_TOOLTIPS_TEXT_COLOR:
					m_ToolTipsTextColor = atoi(strings[1].c_str());
					break;
				case CMKC_TOOLTIPS_TEXT_FONT:
					m_ToolTipsTextFont = atoi(strings[1].c_str());
					break;
				case CMKC_TOOLTIPS_DELAY:
					m_ToolTipsDelay = atoi(strings[1].c_str());
					break;

				//Page 4
				case CMKC_CHAT_COLOR_INPUT_TEXT:
					m_ChatColorInputText = atoi(strings[1].c_str());
					break;
				case CMKC_CHAT_COLOR_MENU_OPTION:
					m_ChatColorMenuOption = atoi(strings[1].c_str());
					break;
				case CMKC_CHAT_COLOR_PLAYER_IN_MEMBER_LIST:
					m_ChatColorPlayerInMemberList = atoi(strings[1].c_str());
					break;
				case CMKC_CHAT_COLOR_TEXT:
					m_ChatColorText = atoi(strings[1].c_str());
					break;
				case CMKC_CHAT_COLOR_PLAYER_NAME_WITHOUT:
					m_ChatColorPlayerNameWithout = atoi(strings[1].c_str());
					break;
				case CMKC_CHAT_COLOR_MUTED:
					m_ChatColorMuted = atoi(strings[1].c_str());
					break;
				case CMKC_CHAT_COLOR_CHANNEL_MODERATOR_NAME:
					m_ChatColorChannelModeratorName = atoi(strings[1].c_str());
					break;
				case CMKC_CHAT_COLOR_CHANNEL_MODERATOR_TEXT:
					m_ChatColorChannelModeratorText = atoi(strings[1].c_str());
					break;
				case CMKC_CHAT_COLOR_MY_NAME:
					m_ChatColorMyName = atoi(strings[1].c_str());
					break;
				case CMKC_CHAT_COLOR_MY_TEXT:
					m_ChatColorMyText = atoi(strings[1].c_str());
					break;
				case CMKC_CHAT_COLOR_SYSTEM_MESSAGE:
					m_ChatColorSystemMessage = atoi(strings[1].c_str());
					break;
				case CMKC_CHAT_FONT:
					m_ChatFont = atoi(strings[1].c_str());
					break;
				case CMKC_CHAT_COLOR_BG_OUTPUT_TEXT:
					m_ChatColorBGOutputText = atoi(strings[1].c_str());
					break;
				case CMKC_CHAT_COLOR_BG_INPUT_TEXT:
					m_ChatColorBGInputText = atoi(strings[1].c_str());
					break;
				case CMKC_CHAT_COLOR_BG_USER_LIST:
					m_ChatColorBGUserList = atoi(strings[1].c_str());
					break;
				case CMKC_CHAT_COLOR_BG_CONF_LIST:
					m_ChatColorBGConfList = atoi(strings[1].c_str());
					break;
				case CMKC_CHAT_COLOR_BG_COMMAND_LIST:
					m_ChatColorBGCommandList = atoi(strings[1].c_str());
					break;

				//Page 6
				case CMKC_ENABLE_PATHFIND:
					m_EnablePathfind = ToBool(strings[1]);
					break;
				case CMKC_HOLD_TAB_FOR_COMBAT:
					m_HoldTabForCombat = ToBool(strings[1]);
					break;
				case CMKC_OFFSET_INTERFACE_WINDOWS:
					m_OffsetInterfaceWindows = ToBool(strings[1]);
					break;
				case CMKC_AUTO_ARRANGE:
					m_AutoArrange = ToBool(strings[1]);
					break;
				case CMKC_ALWAYS_RUN:
					m_AlwaysRun = ToBool(strings[1]);
					break;
				case CMKC_DISABLE_MENUBAR:
					m_DisableMenubar = ToBool(strings[1]);
					break;
				case CMKC_GRAY_OUT_OF_RANGE_OBJECTS:
					m_GrayOutOfRangeObjects = ToBool(strings[1]);
					break;
				case CMKC_DISABLE_NEW_TARGET_SYSTEM:
					m_DisableNewTargetSystem = ToBool(strings[1]);
					break;
				case CMKC_ITEMP_ROPERTIES_MODE:
					m_ItemPropertiesMode = atoi(strings[1].c_str());
					break;
				case CMKC_ITEMP_ROPERTIES_ICON:
					m_ItemPropertiesIcon = ToBool(strings[1]);
					break;
				case CMKC_OBJECT_HANDLES:
					m_ObjectHandles = ToBool(strings[1]);
					break;
				case CMKC_REDUCE_FPS_UNACTIVE_WINDOW:
					ReduceFPSUnactiveWindow = ToBool(strings[1]);
					break;
				case CMKC_HOLD_SHIFT_FOR_CONTEXT_MENUS:
					m_HoldShiftForContextMenus = ToBool(strings[1]);
					break;
				case CMKC_HOLD_SHIFT_FOR_ENABLE_PATHFIND:
					m_HoldShiftForEnablePathfind = ToBool(strings[1]);
					break;
				case CMKC_CONTAINER_DEFAULT_X:
					g_ContainerRect.DefaultX = atoi(strings[1].c_str());
					break;
				case CMKC_CONTAINER_DEFAULT_Y:
					g_ContainerRect.DefaultY = atoi(strings[1].c_str());
					break;

				//Page 7
				case CMKC_GAME_WINDOW_WIDTH:
					m_GameWindowWidth = atoi(strings[1].c_str());
					break;
				case CMKC_GAME_WINDOW_HEIGHT:
					m_GameWindowHeight = atoi(strings[1].c_str());
					break;
				case CMKC_SPEECH_DELAY:
					m_SpeechDelay = atoi(strings[1].c_str());
					break;
				case CMKC_SCALE_SPEECH_DELAY:
					m_ScaleSpeechDelay = ToBool(strings[1]);
					break;
				case CMKC_SPEECH_COLOR:
					m_SpeechColor = atoi(strings[1].c_str());
					break;
				case CMKC_EMOTE_COLOR:
					m_EmoteColor = atoi(strings[1].c_str());
					break;
				case CMKC_PARTY_MESSAGE_COLOR:
					m_PartyMessageColor = atoi(strings[1].c_str());
					break;
				case CMKC_GUILD_MESSAGE_COLOR:
					m_GuildMessageColor = atoi(strings[1].c_str());
					break;
				case CMKC_ALLIANCE_MESSAGE_COLOR:
					m_AllianceMessageColor = atoi(strings[1].c_str());
					break;
				case CMKC_IGNORE_GUILD_MESSAGE:
					m_IgnoreGuildMessage = ToBool(strings[1]);
					break;
				case CMKC_IGNORE_ALLIANCE_MESSAGE:
					m_IgnoreAllianceMessage = ToBool(strings[1]);
					break;
				case CMKC_DARK_NIGHTS:
					m_DarkNights = ToBool(strings[1]);
					break;
				case CMKC_COLORED_LIGHTING:
					m_ColoredLighting = ToBool(strings[1]);
					break;
				case CMKC_STANDART_CHARACTERS_ANIMATION_DELAY:
					m_StandartCharactersAnimationDelay = ToBool(strings[1]);
					break;
				case CMKC_STANDART_ITEMS_ANIMATION_DELAY:
					m_StandartItemsAnimationDelay = ToBool(strings[1]);
					break;
				case CMKC_LOCK_RESIZING_GAME_WINDOW:
					m_LockResizingGameWindow = ToBool(strings[1]);
					break;
				case CMKC_LOCK_GUMPS_MOVING:
					m_LockGumpsMoving = ToBool(strings[1]);
					break;

				//Page 8
				case CMKC_INNOCENT_COLOR:
					m_InnocentColor = atoi(strings[1].c_str());
					break;
				case CMKC_FRIENDLY_COLOR:
					m_FriendlyColor = atoi(strings[1].c_str());
					break;
				case CMKC_SOMEONE_COLOR:
					m_SomeoneColor = atoi(strings[1].c_str());
					break;
				case CMKC_CRIMINAL_COLOR:
					m_CriminalColor = atoi(strings[1].c_str());
					break;
				case CMKC_ENEMY_COLOR:
					m_EnemyColor = atoi(strings[1].c_str());
					break;
				case CMKC_MURDERER_COLOR:
					m_MurdererColor = atoi(strings[1].c_str());
					break;
				case CMKC_CRIMINAL_ACTIONS_QUERY:
					m_CriminalActionsQuery = ToBool(strings[1]);
					break;

				//Page 9
				case CMKC_SHOW_INCOMING_NAMES:
					m_ShowIncomingNames = ToBool(strings[1]);
					break;
				case CMKC_USE_CIRCLE_TRANS:
					m_UseCircleTrans = ToBool(strings[1]);
					break;
				case CMKC_STAT_REPORT:
					m_StatReport = ToBool(strings[1]);
					break;
				case CMKC_CONSOLE_NEED_ENTER:
					ConsoleNeedEnter = ToBool(strings[1]);
					break;
				case CMKC_CIRCLE_TRANS_RADIUS:
					m_CircleTransRadius = atoi(strings[1].c_str());
					g_CircleOfTransparency.Create(m_CircleTransRadius);
					break;
				case CMKC_SKILL_REPORT:
					m_SkillReport = atoi(strings[1].c_str());
					break;
				case CMKC_SPEECH_FONT:
					m_SpeechFont = atoi(strings[1].c_str());
					break;

				//No page
				case CMKC_GAME_WINDOW_X:
					m_GameWindowX = atoi(strings[1].c_str());
					break;
				case CMKC_GAME_WINDOW_Y:
					m_GameWindowY = atoi(strings[1].c_str());
					break;
				case CMKC_ZOOMED:
					zoomed = ToBool(strings[1]);
					break;
				case CMKC_REAL_X:
					windowX = atoi(strings[1].c_str());
					break;
				case CMKC_REAL_Y:
					windowY = atoi(strings[1].c_str());
					break;
				case CMKC_REAL_WIDTH:
					windowWidth = atoi(strings[1].c_str());
					break;
				case CMKC_REAL_HEIGHT:
					windowHeight = atoi(strings[1].c_str());
					break;
				case CMKC_TOGGLE_BUFFICON_WINDOW:
					m_ToggleBufficonWindow = ToBool(strings[1]);
					break;
				case CMKC_DEVELOPER_MODE:
					g_DeveloperMode = (DEVELOPER_MODE)atoi(strings[1].c_str());
					break;
				default:
					break;
			}
		}
	}

	if (m_GameWindowX < 0)
		m_GameWindowX = 0;

	if (m_GameWindowY < 0)
		m_GameWindowY = 0;

	if (windowX != -1 && windowY != -1 && windowWidth != -1 && windowHeight != -1)
	{
		if (windowX < 0)
			windowX = 0;

		if (windowY < 0)
			windowY = 0;

		if (g_GameState >= GS_GAME)
		{
			if (windowWidth < 640)
				windowWidth = 640;

			if (windowWidth >= GetSystemMetrics(SM_CXSCREEN) - 20)
				windowWidth = GetSystemMetrics(SM_CXSCREEN) - 20;

			if (windowHeight < 480)
				windowHeight = 480;

			if (windowHeight >= GetSystemMetrics(SM_CYSCREEN) - 60)
				windowHeight = GetSystemMetrics(SM_CYSCREEN) - 60;
		}
		else
		{
			windowWidth = 640;
			windowHeight = 480;
		}

		SendMessage(g_OrionWindow.Handle, WM_SYSCOMMAND, SC_RESTORE, 0);

		if (zoomed)
			SendMessage(g_OrionWindow.Handle, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		else
			SetWindowPos(g_OrionWindow.Handle, NULL, windowX, windowY, windowWidth, windowHeight, 0);

		g_GL.UpdateRect();
	}
	else
	{
		SendMessage(g_OrionWindow.Handle, WM_SYSCOMMAND, SC_RESTORE, 0);
		SendMessage(g_OrionWindow.Handle, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	}

	return PathFileExistsA(path.c_str());
}
//---------------------------------------------------------------------------
void CConfigManager::Save(const string &path)
{
	WISPFUN_DEBUG("");
	WISP_FILE::CTextFileWritter writter(path);

	if (writter.Opened())
	{
		//Page 1
		writter.WriteBool("Sound", m_Sound);
		writter.WriteInt("SoundVolume", m_SoundVolume);
		writter.WriteBool("Music", m_Music);
		writter.WriteInt("MusicVolume", m_MusicVolume);
		writter.WriteBool("FootstepsSound", m_FootstepsSound);
		writter.WriteBool("CombatMusic", m_CombatMusic);

		//Page 2
		writter.WriteInt("ClientFPS", m_ClientFPS);
		writter.WriteBool("UseScaling", m_UseScaling);
		writter.WriteBool("RemoveTextWithBlending", m_RemoveTextWithBlending);
		writter.WriteInt("DrawStatusState", m_DrawStatusState);
		writter.WriteBool("DrawStumps", m_DrawStumps);
		writter.WriteBool("MarkingCaves", m_MarkingCaves);
		writter.WriteBool("NoAnimateFields", m_NoAnimateFields);
		writter.WriteBool("NoVegetation", m_NoVegetation);
		writter.WriteInt("HiddenCharactersRenderMode", m_HiddenCharactersRenderMode);
		writter.WriteInt("HiddenAlpha", m_HiddenAlpha);
		writter.WriteBool("UseHiddenModeOnlyForSelf", m_UseHiddenModeOnlyForSelf);
		writter.WriteInt("TransparentSpellIcons", m_TransparentSpellIcons);
		writter.WriteInt("SpellIconAlpha", m_SpellIconAlpha);
		writter.WriteBool("OldStyleStatusbar", m_OldStyleStatusbar);
		writter.WriteBool("OriginalPartyStatusbar", m_OriginalPartyStatusbar);
		writter.WriteBool("ApplyStateColorOnCharacters", m_ApplyStateColorOnCharacters);
		writter.WriteBool("ChangeFieldsGraphic", m_ChangeFieldsGraphic);
		writter.WriteBool("PaperdollSlots", m_PaperdollSlots);
		writter.WriteInt("DrawStatusConditionState", m_DrawStatusConditionState);
		writter.WriteInt("DrawStatusConditionValue", m_DrawStatusConditionValue);
		writter.WriteBool("RemoveStatusbarsWithoutObjects", m_RemoveStatusbarsWithoutObjects);
		writter.WriteBool("ShowDefaultConsoleEntryMode", m_ShowDefaultConsoleEntryMode);
		writter.WriteInt("DrawAuraState", m_DrawAuraState);
		writter.WriteBool("DrawAuraWithCtrlPressed", m_DrawAuraWithCtrlPressed);
		writter.WriteInt("ScreenshotFormat", m_ScreenshotFormat);
		writter.WriteBool("ScaleImagesInPaperdollSlots", m_ScaleImagesInPaperdollSlots);
		writter.WriteBool("RemoveOrCreateObjectsWithBlending", m_RemoveOrCreateObjectsWithBlending);
		writter.WriteBool("DrawHelmetsOnShroud", m_DrawHelmetsOnShroud);
		writter.WriteBool("UseGlobalMapLayer", m_UseGlobalMapLayer);
		writter.WriteBool("NoDrawRoofs", m_NoDrawRoofs);
		writter.WriteBool("HighlightTargetByType", m_HighlightTargetByType);
		writter.WriteBool("AutoDisplayWorldMap", m_AutoDisplayWorldMap);

		//Page 3
		writter.WriteBool("UseToolTips", m_UseToolTips);
		writter.WriteInt("ToolTipsTextColor", m_ToolTipsTextColor);
		writter.WriteInt("ToolTipsTextFont", m_ToolTipsTextFont);
		writter.WriteInt("ToolTipsDelay", m_ToolTipsDelay);

		//Page 4
		writter.WriteInt("ChatColorInputText", m_ChatColorInputText);
		writter.WriteInt("ChatColorMenuOption", m_ChatColorMenuOption);
		writter.WriteInt("ChatColorPlayerInMemberList", m_ChatColorPlayerInMemberList);
		writter.WriteInt("ChatColorText", m_ChatColorText);
		writter.WriteInt("ChatColorPlayerNameWithout", m_ChatColorPlayerNameWithout);
		writter.WriteInt("ChatColorMuted", m_ChatColorMuted);
		writter.WriteInt("ChatColorChannelModeratorName", m_ChatColorChannelModeratorName);
		writter.WriteInt("ChatColorChannelModeratorText", m_ChatColorChannelModeratorText);
		writter.WriteInt("ChatColorMyName", m_ChatColorMyName);
		writter.WriteInt("ChatColorMyText", m_ChatColorMyText);
		writter.WriteInt("ChatColorSystemMessage", m_ChatColorSystemMessage);
		writter.WriteInt("ChatFont", m_ChatFont);
		writter.WriteInt("ChatColorBGOutputText", m_ChatColorBGOutputText);
		writter.WriteInt("ChatColorBGInputText", m_ChatColorBGInputText);
		writter.WriteInt("ChatColorBGUserList", m_ChatColorBGUserList);
		writter.WriteInt("ChatColorBGConfList", m_ChatColorBGConfList);
		writter.WriteInt("ChatColorBGCommandList", m_ChatColorBGCommandList);

		//Page 6
		writter.WriteBool("EnablePathfind", m_EnablePathfind);
		writter.WriteBool("HoldTabForCombat", m_HoldTabForCombat);
		writter.WriteBool("OffsetInterfaceWindows", m_OffsetInterfaceWindows);
		writter.WriteBool("AutoArrange", m_AutoArrange);
		writter.WriteBool("AlwaysRun", m_AlwaysRun);
		writter.WriteBool("DisableMenubar", m_DisableMenubar);
		writter.WriteBool("GrayOutOfRangeObjects", m_GrayOutOfRangeObjects);
		writter.WriteBool("DisableNewTargetSystem", m_DisableNewTargetSystem);
		writter.WriteBool("ItemPropertiesMode", m_ItemPropertiesMode);
		writter.WriteBool("ItemPropertiesIcon", m_ItemPropertiesIcon);
		writter.WriteBool("ObjectHandles", m_ObjectHandles);
		writter.WriteBool("ReduceFPSUnactiveWindow", m_ReduceFPSUnactiveWindow);
		writter.WriteBool("HoldShiftForContextMenus", m_HoldShiftForContextMenus);
		writter.WriteBool("HoldShiftForEnablePathfind", m_HoldShiftForEnablePathfind);
		writter.WriteInt("ContainerDefaultX", g_ContainerRect.DefaultX);
		writter.WriteInt("ContainerDefaultY", g_ContainerRect.DefaultY);

		//Page 7
		writter.WriteInt("GameWindowWidth", m_GameWindowWidth);
		writter.WriteInt("GameWindowHeight", m_GameWindowHeight);
		writter.WriteInt("SpeechDelay", m_SpeechDelay);
		writter.WriteInt("ScaleSpeechDelay", m_ScaleSpeechDelay);
		writter.WriteInt("SpeechColor", m_SpeechColor);
		writter.WriteInt("EmoteColor", m_EmoteColor);
		writter.WriteInt("PartyMessageColor", m_PartyMessageColor);
		writter.WriteInt("GuildMessageColor", m_GuildMessageColor);
		writter.WriteInt("AllianceMessageColor", m_AllianceMessageColor);
		writter.WriteBool("IgnoreGuildMessage", m_IgnoreGuildMessage);
		writter.WriteBool("IgnoreAllianceMessage", m_IgnoreAllianceMessage);
		writter.WriteBool("DarkNights", m_DarkNights);
		writter.WriteBool("ColoredLighting", m_ColoredLighting);
		writter.WriteBool("StandartCharactersAnimationDelay", m_StandartCharactersAnimationDelay);
		writter.WriteBool("StandartItemsAnimationDelay", m_StandartItemsAnimationDelay);
		writter.WriteBool("LockResizingGameWindow", m_LockResizingGameWindow);
		writter.WriteBool("LockGumpsMoving", m_LockGumpsMoving);

		//Page 8
		writter.WriteInt("InnocentColor", m_InnocentColor);
		writter.WriteInt("FriendlyColor", m_FriendlyColor);
		writter.WriteInt("SomeoneColor", m_SomeoneColor);
		writter.WriteInt("CriminalColor", m_CriminalColor);
		writter.WriteInt("EnemyColor", m_EnemyColor);
		writter.WriteInt("MurdererColor", m_MurdererColor);
		writter.WriteBool("CriminalActionsQuery", m_CriminalActionsQuery);

		//Page 9
		writter.WriteBool("ShowIncomingNames", m_ShowIncomingNames);
		writter.WriteBool("UseCircleTrans", m_UseCircleTrans);
		writter.WriteBool("StatReport", m_StatReport);
		writter.WriteBool("ConsoleNeedEnter", m_ConsoleNeedEnter);
		writter.WriteBool("CircleTransRadius", m_CircleTransRadius);
		writter.WriteBool("SkillReport", m_SkillReport);
		writter.WriteInt("SpeechFont", m_SpeechFont);

		//No page
		writter.WriteInt("GameWindowX", m_GameWindowX);
		writter.WriteInt("GameWindowY", m_GameWindowY);

		writter.WriteBool("Zoomed", g_OrionWindow.Zoomed());

		RECT rect = { 0 };
		GetWindowRect(g_OrionWindow.Handle, &rect);

		writter.WriteInt("RealX", rect.left);
		writter.WriteInt("RealY", rect.top);
		writter.WriteInt("RealWidth", (rect.right - rect.left));
		writter.WriteInt("RealHeight", (rect.bottom - rect.top));

		writter.WriteBool("ToggleBufficonWindow", m_ToggleBufficonWindow);
		writter.WriteInt("DeveloperMode", g_DeveloperMode);

		writter.Close();
	}
}
//---------------------------------------------------------------------------