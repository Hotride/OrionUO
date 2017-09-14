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
bool CConfigManager::Load(string path)
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
/*!
Сохранение конфига
@param [__in] path Путь к файлу с конфигом
@return 
*/
void CConfigManager::Save(string path)
{
	WISPFUN_DEBUG("c138_f31");
	WISP_FILE::CBinaryFileWritter writter;
	writter.Open(path);
	
	writter.WriteInt8(0); //version

	//Page 1
	writter.WriteInt8(8); //size of block
	writter.WriteInt8(1); //page index
	writter.WriteUInt8(m_Sound);
	writter.WriteUInt8(m_Music);
	writter.WriteUInt8(m_FootstepsSound);
	writter.WriteUInt8(m_CombatMusic);
	writter.WriteUInt8(m_SoundVolume);
	writter.WriteUInt8(m_MusicVolume);
	writter.WriteBuffer();

	//Page 2
	writter.WriteInt8(32); //size of block
	writter.WriteInt8(2); //page index
	writter.WriteUInt8(m_ClientFPS);
	writter.WriteUInt8(m_UseScaling);
	writter.WriteUInt8(m_RemoveTextWithBlending);
	writter.WriteUInt8(m_DrawStatusState);
	writter.WriteUInt8(m_DrawStumps);
	writter.WriteUInt8(m_MarkingCaves);
	writter.WriteUInt8(m_NoAnimateFields);
	writter.WriteUInt8(m_NoVegetation);
	writter.WriteUInt8(m_HiddenCharactersRenderMode);
	writter.WriteUInt8(m_HiddenAlpha);
	writter.WriteUInt8(m_UseHiddenModeOnlyForSelf);
	writter.WriteUInt8(m_TransparentSpellIcons);
	writter.WriteUInt8(m_SpellIconAlpha);
	writter.WriteUInt8(m_OldStyleStatusbar);
	writter.WriteUInt8(m_OriginalPartyStatusbar);
	writter.WriteUInt8(m_ApplyStateColorOnCharacters);
	writter.WriteUInt8(m_ChangeFieldsGraphic);
	writter.WriteUInt8(m_PaperdollSlots);
	writter.WriteUInt8(m_DrawStatusConditionState);
	writter.WriteUInt8(m_DrawStatusConditionValue);
	writter.WriteUInt8(m_RemoveStatusbarsWithoutObjects);
	writter.WriteUInt8(m_ShowDefaultConsoleEntryMode);

	uchar auraState = m_DrawAuraState;

	if (m_DrawAuraWithCtrlPressed)
		auraState |= 0x80;

	writter.WriteUInt8(auraState);
	writter.WriteUInt8(m_ScreenshotFormat);
	writter.WriteUInt8(m_ScaleImagesInPaperdollSlots);
	writter.WriteUInt8(m_RemoveOrCreateObjectsWithBlending);
	writter.WriteUInt8(m_DrawHelmetsOnShroud);
	writter.WriteUInt8(m_UseGlobalMapLayer);
	writter.WriteUInt8(m_NoDrawRoofs);
	writter.WriteUInt8(m_HighlightTargetByType);
	
	writter.WriteBuffer();

	//Page 3
	writter.WriteInt8(9); //size of block
	writter.WriteInt8(3); //page index
	writter.WriteUInt8(m_UseToolTips);
	writter.WriteUInt16LE(m_ToolTipsTextColor);
	writter.WriteUInt16LE(m_ToolTipsTextFont);
	writter.WriteUInt16LE(m_ToolTipsDelay);
	writter.WriteBuffer();

	//Page 4
	writter.WriteInt8(36); //size of block
	writter.WriteInt8(4); //page index
	writter.WriteUInt16LE(m_ChatColorInputText);
	writter.WriteUInt16LE(m_ChatColorMenuOption);
	writter.WriteUInt16LE(m_ChatColorPlayerInMemberList);
	writter.WriteUInt16LE(m_ChatColorText);
	writter.WriteUInt16LE(m_ChatColorPlayerNameWithout);
	writter.WriteUInt16LE(m_ChatColorMuted);
	writter.WriteUInt16LE(m_ChatColorChannelModeratorName);
	writter.WriteUInt16LE(m_ChatColorChannelModeratorText);
	writter.WriteUInt16LE(m_ChatColorMyName);
	writter.WriteUInt16LE(m_ChatColorMyText);
	writter.WriteUInt16LE(m_ChatColorSystemMessage);
	writter.WriteUInt16LE(m_ChatFont);
	writter.WriteUInt16LE(m_ChatColorBGOutputText);
	writter.WriteUInt16LE(m_ChatColorBGInputText);
	writter.WriteUInt16LE(m_ChatColorBGUserList);
	writter.WriteUInt16LE(m_ChatColorBGConfList);
	writter.WriteUInt16LE(m_ChatColorBGCommandList);
	writter.WriteBuffer();

	//Page 6
	writter.WriteInt8(20); //size of block
	writter.WriteInt8(6); //page index
	writter.WriteUInt8(m_EnablePathfind);
	writter.WriteUInt8(m_HoldTabForCombat);
	writter.WriteUInt8(m_OffsetInterfaceWindows);
	writter.WriteUInt8(m_AutoArrange);
	writter.WriteUInt8(m_AlwaysRun);
	writter.WriteUInt8(m_DisableMenubar);
	writter.WriteUInt8(m_GrayOutOfRangeObjects);
	writter.WriteUInt8(m_DisableNewTargetSystem);
	writter.WriteUInt8(m_ItemPropertiesMode);
	writter.WriteUInt8(m_ItemPropertiesIcon);
	writter.WriteUInt8(m_ObjectHandles);
	writter.WriteUInt8(m_ReduceFPSUnactiveWindow);
	writter.WriteUInt8(m_HoldShiftForContextMenus);
	writter.WriteUInt8(m_HoldShiftForEnablePathfind);
	writter.WriteUInt16LE(g_ContainerRect.DefaultX);
	writter.WriteUInt16LE(g_ContainerRect.DefaultY);
	writter.WriteBuffer();

	//Page 7
	writter.WriteInt8(28); //size of block
	writter.WriteInt8(7); //page index
	writter.WriteUInt16LE(m_GameWindowWidth);
	writter.WriteUInt16LE(m_GameWindowHeight);
	writter.WriteUInt16LE(m_SpeechDelay);
	writter.WriteUInt16LE(m_ScaleSpeechDelay);
	writter.WriteUInt16LE(m_SpeechColor);
	writter.WriteUInt16LE(m_EmoteColor);
	writter.WriteUInt16LE(m_PartyMessageColor);
	writter.WriteUInt16LE(m_GuildMessageColor);
	writter.WriteUInt16LE(m_AllianceMessageColor);
	writter.WriteUInt8(m_IgnoreGuildMessage);
	writter.WriteUInt8(m_IgnoreAllianceMessage);
	writter.WriteUInt8(m_DarkNights);
	writter.WriteUInt8(m_ColoredLighting);
	writter.WriteUInt8(m_StandartCharactersAnimationDelay);
	writter.WriteUInt8(m_StandartItemsAnimationDelay);
	writter.WriteUInt8(m_LockResizingGameWindow);
	writter.WriteUInt8(m_LockGumpsMoving);
	writter.WriteBuffer();

	//Page 8
	writter.WriteInt8(15); //size of block
	writter.WriteInt8(8); //page index
	writter.WriteUInt16LE(m_InnocentColor);
	writter.WriteUInt16LE(m_FriendlyColor);
	writter.WriteUInt16LE(m_SomeoneColor);
	writter.WriteUInt16LE(m_CriminalColor);
	writter.WriteUInt16LE(m_EnemyColor);
	writter.WriteUInt16LE(m_MurdererColor);
	writter.WriteUInt8(m_CriminalActionsQuery);
	writter.WriteBuffer();

	//Page 9
	writter.WriteInt8(10); //size of block
	writter.WriteInt8(9); //page index
	writter.WriteUInt8(m_ShowIncomingNames);
	writter.WriteUInt8(m_UseCircleTrans);
	writter.WriteUInt8(m_StatReport);
	writter.WriteUInt8(m_ConsoleNeedEnter);
	writter.WriteUInt8(m_CircleTransRadius);
	writter.WriteUInt8(m_SkillReport);
	writter.WriteUInt16LE(m_SpeechFont);
	writter.WriteBuffer();

	//Page 10
	writter.WriteInt8(2); //size of block
	writter.WriteInt8(10); //page index
	writter.WriteBuffer();

	//No page
	writter.WriteInt8(6 + 9 + 1 + 1); //size of block
	writter.WriteInt8(0); //page index
	writter.WriteUInt16LE(m_GameWindowX);
	writter.WriteUInt16LE(m_GameWindowY);

	writter.WriteUInt8(g_OrionWindow.Zoomed());

	RECT rect = { 0 };
	GetWindowRect(g_OrionWindow.Handle, &rect);

	writter.WriteInt16LE((short)rect.left);
	writter.WriteInt16LE((short)rect.top);
	writter.WriteInt16LE((short)(rect.right - rect.left));
	writter.WriteInt16LE((short)(rect.bottom - rect.top));

	writter.WriteUInt8(m_ToggleBufficonWindow);
	writter.WriteUInt8(g_DeveloperMode);

	writter.WriteBuffer();

	writter.Close();
	
	/*b = (BYTE)m_FilterPWOn;
	fwrite(&b, 1, 1, file);
	b = (BYTE)m_ObscenityFilter;
	fwrite(&b, 1, 1, file);
	w = (WORD)m_FilterPassword.length();
	fwrite(&w, 2, 1, file);
	if (w)
		fwrite(&((char*)m_FilterPassword.c_str())[0], w, 1, file);*/
}
//---------------------------------------------------------------------------