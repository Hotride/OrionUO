/***********************************************************************************
**
** OrionUO.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include "OrionUO.h"
#include "OrionWindow.h"
#include "Wisp/WispMappedFile.h"
#include "Wisp/WispTextFileParser.h"
#include "Wisp/WispMouse.h"
#include "EnumList.h"
#include "Managers/ConnectionManager.h"
#include "Managers/PacketManager.h"
#include "Managers/FileManager.h"
#include "Managers/ColorManager.h"
#include "Managers/MouseManager.h"
#include "Managers/MulReader.h"
#include "Managers/FontsManager.h"
#include "Managers/SoundManager.h"
#include "Managers/SpeechManager.h"
#include "Managers/ConfigManager.h"
#include "Managers/ClilocManager.h"
#include "Managers/CityManager.h"
#include "Managers/CreateCharacterManager.h"
#include "Managers/ProfessionManager.h"
#include "Managers/SkillGroupManager.h"
#include "Managers/MapManager.h"
#include "Managers/EffectManager.h"
#include "Managers/GumpManager.h"
#include "Managers/AnimationManager.h"
#include "Managers/MacroManager.h"
#include "Screen stages/DebugScreen.h"
#include "Screen stages/MainScreen.h"
#include "Screen stages/ConnectionScreen.h"
#include "Screen stages/ServerScreen.h"
#include "Screen stages/CharacterListScreen.h"
#include "Screen stages/SelectProfessionScreen.h"
#include "Screen stages/CreateCharacterScreen.h"
#include "Screen stages/SelectTownScreen.h"
#include "Screen stages/GameScreen.h"
#include "Screen stages/GameBlockedScreen.h"
#include "GLEngine/GLEngine.h"
#include "GLEngine/GLShader.h"
#include "ShaderData.h"
#include "CharacterList.h"
#include "TextEngine/GameConsole.h"
#include "Skills.h"
#include "GLEngine/GLTextureCircleOfTransparency.h"
#include "SelectedObject.h"
#include "PressedObject.h"
#include "Network/Packets.h"
#include "ServerList.h"
#include "Constants.h"
#include "TextEngine/TextContainer.h"
#include "TextEngine/Journal.h"
#include "TextEngine/TextRenderer.h"
#include "Game objects/GameWorld.h"
#include "Game objects/GameItem.h"
#include "Game objects/GamePlayer.h"
#include "Game objects/ObjectOnCursor.h"
#include "Party.h"
#include "Walker/Walker.h"
#include "Target.h"
#include "Container.h"
#include "QuestArrow.h"
#include "Walker/PathFinder.h"
#include "ClickObject.h"
#include "Gumps/GumpQuestion.h"
#include "Gumps/GumpPopupMenu.h"
#include "Gumps/GumpMinimap.h"
#include "Gumps/GumpPartyManifest.h"
#include "Gumps/GumpStatusbar.h"
#include "Gumps/GumpBuff.h"
#include "Gumps/GumpJournal.h"
#include "Gumps/GumpOptions.h"
#include "Gumps/GumpSkills.h"
//----------------------------------------------------------------------------------
COrion g_Orion;
//----------------------------------------------------------------------------------
COrion::COrion()
: m_ClientVersionText("2.0.3"), m_InverseBuylist(false), m_LandDataCount(0),
m_StaticDataCount(0)
{
}
//----------------------------------------------------------------------------------
COrion::~COrion()
{
}
//----------------------------------------------------------------------------------
uint Reflect(uint source, int c)
{
	uint value = 0;

	IFOR(i, 1, c + 1)
	{
		if (source & 0x1)
			value |= (1 << (c - i));

		source >>= 1;
	}

	return value;
}
//----------------------------------------------------------------------------------
uint COrion::GetFileHashCode(uint address, uint size)
{
	uint crc = 0xFFFFFFFF;

	puchar ptr = (puchar)address;

	while (size > 0)
	{
		crc = (crc >> 8) ^ m_CRC_Table[(crc & 0xFF) ^ *ptr];

		ptr++;
		size--;
	}

	return (crc & 0xFFFFFFFF);
}
//----------------------------------------------------------------------------------
bool COrion::Install()
{
	IFOR(i, 0, 256)
	{
		m_CRC_Table[i] = Reflect(i, 8) << 24;

		IFOR(j, 0, 8)
			m_CRC_Table[i] = (m_CRC_Table[i] << 1) ^ ((m_CRC_Table[i] & (1 << 31)) ? 0x04C11DB7 : 0);

		m_CRC_Table[i] = Reflect(m_CRC_Table[i], 32);
	}

	CreateDirectoryA(g_App.FilePath("snapshots").c_str(), NULL);

	LoadClientConfig();
	LoadAutoLoginNames();

	if (!g_FileManager.Load())
	{
		string tmp = string("Error loading file:\n") + WISP_FILE::g_WispMappedFileError;
		LOG(tmp.c_str());
		g_OrionWindow.ShowMessage(tmp.c_str(), "Error loading file!");

		return false;
	}

	m_AnimData.resize(g_FileManager.m_AnimdataMul.Size);
	memcpy(&m_AnimData[0], &g_FileManager.m_AnimdataMul.Start[0], g_FileManager.m_AnimdataMul.Size);

	g_ColorManager.Init();

	if (g_FileManager.UseUOP)
		g_MulReader = new CUopReader();
	else
		g_MulReader = new CMulReader();

	LoadTiledata(512, 512);
	LoadIndexFiles();
	InitStaticAnimList();

	if (!g_FontManager.LoadFonts())
	{
		LOG("Error loading fonts\n");
		g_OrionWindow.ShowMessage("Error loading fonts", "Error loading fonts!");

		return false;
	}

	if (!LoadSkills())
	{
		LOG("Error loading skills\n");
		g_OrionWindow.ShowMessage("Error loading skills", "Error loading skills!");

		return false;
	}

	if (!g_SpeechManager.LoadSpeech())
	{
		LOG("Error loading speech.mul\n");
		g_OrionWindow.ShowMessage("Error loading speech.mul", "Error loading speech.mul!");

		return false;
	}

	PatchFiles();
	IndexReplaces();

	g_SkillSort.Init();

	if (!g_MouseManager.LoadCursorTextures())
	{
		LOG("Error loading cursors\n");
		g_OrionWindow.ShowMessage("Error loading cursors", "Error loading cursors!");

		return false;
	}

	if (!g_SoundManager.Init())
	{
		//LOG("Error install BASS audio: %s\n", BASS_error());
		g_OrionWindow.ShowMessage("Failed to init BASS audio.", "Sound error!");

		return false;
	}

	g_CityManager.Init();

	g_EntryPointer = NULL;

	g_ProfessionManager.Load();
	g_ProfessionManager.Selected = (CBaseProfession*)g_ProfessionManager.m_Items;

	ExecuteStaticArt(0x0EED); //gp 1
	ExecuteStaticArt(0x0EEE); //gp 2-5
	ExecuteStaticArt(0x0EEF); //gp 6+

	g_CreateCharacterManager.Init();

	if (g_FileManager.UseUOP)
		g_MapManager = new CUopMapManager();
	else
		g_MapManager = new CMapManager();

	IFOR(i, 0, 6)
		g_AnimationManager.Init(i, (uint)g_FileManager.m_AnimIdx[i].Start, (uint)g_FileManager.m_AnimMul[i].Start, (uint)g_FileManager.m_AnimIdx[i].Size);

	g_AnimationManager.InitIndexReplaces((puint)g_FileManager.m_VerdataMul.Start);

	/*
	g_PluginManager = new CPluginManager();
*/

	LoadClientStartupConfig();

	ushort b = 0x0000;
	ushort r = 0xFC00; // 0xFF0000FF;
	ushort g = 0x83E0; // 0xFF00FF00;

	ushort pdwlt[2][140] =
	{
		{
			b, b, b, g, g, g, g, g, g, g,
			b, b, b, g, g, g, g, g, g, g,
			b, b, b, b, g, g, b, b, b, b,
			b, b, b, b, g, g, b, b, b, b,
			b, b, b, b, g, g, b, b, b, b,
			g, g, g, g, g, g, g, g, g, b,
			g, g, g, g, g, g, g, g, g, b,
			g, g, b, b, b, b, b, g, g, b,
			g, g, b, g, g, b, b, g, g, b,
			g, g, b, b, g, b, b, g, g, b,
			g, g, b, b, g, b, b, g, g, b,
			g, g, g, b, b, b, g, g, g, b,
			b, g, g, g, g, g, g, g, b, b,
			b, b, g, g, g, g, g, b, b, b
		},
		{
			b, r, r, r, r, r, r, r, b, b,
			b, r, r, r, r, r, r, r, b, b,
			b, b, r, r, b, r, r, b, b, b,
			b, b, r, r, b, r, r, b, b, b,
			b, b, r, r, b, r, r, b, b, b,
			r, r, r, r, r, r, r, r, r, b,
			r, r, r, r, r, r, r, r, r, b,
			r, r, b, b, b, b, b, r, r, b,
			r, r, b, r, r, b, b, r, r, b,
			r, r, b, b, r, b, b, r, r, b,
			r, r, b, b, r, b, b, r, r, b,
			r, r, r, b, b, b, r, r, r, b,
			b, r, r, r, r, r, r, r, b, b,
			b, b, r, r, r, r, r, b, b, b
		}
	};

	IFOR(i, 0, 2)
	{
		g_TextureGumpState[i].Width = 10;
		g_TextureGumpState[i].Height = 14;
		g_GL.BindTexture16(g_TextureGumpState[i].Texture, 10, 14, &pdwlt[i][0]);
	}

	g_LightBuffer.Init(GetSystemMetrics(SM_CXMAXIMIZED), GetSystemMetrics(SM_CYMAXIMIZED));

	CreateObjectHandlesBackground();

	LoadShaders();

	g_MainScreen.UpdateContent();
	g_MainScreen.LoadGlobalConfig();

#ifdef _DEBUG
	g_CurrentScreen = &g_DebugScreen;
	g_DebugScreen.Init();
	InitScreen(GS_MAIN);
#else
	InitScreen(GS_MAIN);
#endif

	return true;
}
//----------------------------------------------------------------------------------
void COrion::Uninstall()
{
	LOG("COrion::Uninstall()\n");
	SaveLocalConfig();
	g_MainScreen.SaveGlobalConfig();
	g_GumpManager.OnDelete();

	Disconnect();

	UnloadIndexFiles();

	g_EntryPointer = NULL;
	g_CurrentScreen = NULL;
	RELEASE_POINTER(g_World);
	//RELEASE_POINTER(PluginManager);
	RELEASE_POINTER(g_DeathShader);
	RELEASE_POINTER(g_ColorizerShader);
	RELEASE_POINTER(g_FontColorizerShader);
	RELEASE_POINTER(g_LightColorizerShader);
	g_CurrentShader = NULL;
	RELEASE_POINTER(g_MulReader);
	RELEASE_POINTER(g_MapManager);

	IFOR(i, 0, 6)
		g_MapTexture[i].Clear();

	IFOR(i, 0, 2)
		g_TextureGumpState[i].Clear();

	g_SoundManager.Free();
	g_ServerList.Clear();
	g_FileManager.Unload();
	g_LightBuffer.Free();

	g_GL.Uninstall();
}
//----------------------------------------------------------------------------------
void COrion::InitScreen(const GAME_STATE &state)
{
	g_GameState = state;
	g_SelectedObject.Clear();
	g_LastSelectedObject.Clear();
	g_PressedObject.ClearAll();

	switch (state)
	{
		case GS_MAIN:
		{
			g_CurrentScreen = &g_MainScreen;
			break;
		}
		case GS_MAIN_CONNECT:
		{
			g_CurrentScreen = &g_ConnectionScreen;
			break;
		}
		case GS_SERVER:
		{
			g_CurrentScreen = &g_ServerScreen;
			break;
		}
		case GS_SERVER_CONNECT:
		{
			g_CurrentScreen = &g_ConnectionScreen;
			break;
		}
		case GS_CHARACTER:
		{
			g_CurrentScreen = &g_CharacterListScreen;
			break;
		}
		case GS_DELETE:
		{
			g_CurrentScreen = &g_ConnectionScreen;
			break;
		}
		case GS_PROFESSION_SELECT:
		{
			g_CurrentScreen = &g_SelectProfessionScreen;
			break;
		}
		case GS_CREATE:
		{
			g_CurrentScreen = &g_CreateCharacterScreen;
			break;
		}
		case GS_SELECT_TOWN:
		{
			g_CurrentScreen = &g_SelectTownScreen;
			break;
		}
		case GS_GAME_CONNECT:
		{
			g_CurrentScreen = &g_ConnectionScreen;
			break;
		}
		case GS_GAME:
		{
			g_CurrentScreen = &g_GameScreen;
			break;
		}
		case GS_GAME_BLOCKED:
		{
			g_CurrentScreen = &g_GameBlockedScreen;
			break;
		}
		default:
			break;
	}

	if (g_CurrentScreen != NULL)
		g_CurrentScreen->Init();
}
//----------------------------------------------------------------------------------
void COrion::LoadClientConfig()
{
	WISP_FILE::CMappedFile file;

	if (file.Load(g_App.FilePath("Client.cuo")))
	{
		uchar version = file.ReadInt8();

		g_ConnectionManager.EncryptionType = (ENCRYPTION_TYPE)file.ReadInt8();

		g_ConnectionManager.ClientVersion = (CLIENT_VERSION)file.ReadInt8();
		g_PacketManager.ClientVersion = g_ConnectionManager.ClientVersion;

		int len = file.ReadInt8();
		m_ClientVersionText = file.ReadString(len);

		g_ConnectionManager.CryptKey1 = file.ReadUInt32LE();
		g_ConnectionManager.CryptKey2 = file.ReadUInt32LE();
		g_ConnectionManager.CryptKey3 = file.ReadUInt32LE();

		g_ConnectionManager.CryptSeed = file.ReadUInt16LE();

		m_InverseBuylist = (file.ReadUInt8() != 0);

		IFOR(i, 0, 6)
		{
			g_MapSize[i].Width = file.ReadUInt16LE();
			g_MapSize[i].Height = file.ReadUInt16LE();

			g_MapBlockSize[i].Width = g_MapSize[i].Width / 8;
			g_MapBlockSize[i].Height = g_MapSize[i].Height / 8;
		}

		if (version >= 2)
		{
			g_CharacterList.ClientFlag = file.ReadInt8();

			if (version >= 3)
			{
				g_FileManager.UseVerdata = (file.ReadInt8() != 0);

				if (version >= 4)
				{
					g_FileManager.UseUOP = (file.ReadInt8() != 0);

					if (g_FileManager.UseUOP)
						g_FileManager.UseVerdata = false;
				}
			}
		}

		if (version < 3 && m_ClientVersionText.length())
			g_FileManager.UseVerdata = (*m_ClientVersionText.c_str() <= '5');

		file.Unload();
	}
}
//----------------------------------------------------------------------------------
void COrion::LoadAutoLoginNames()
{
	WISP_FILE::CTextFileParser file(g_App.FilePath("AutoLoginNames.cfg"), "", "#;", "");

	string names = "|";

	while (!file.IsEOF())
	{
		STRING_LIST strings = file.ReadTokens();

		if (strings.size())
			names += strings[0] + "|";
	}

	g_PacketManager.AutoLoginNames = names;
}
//---------------------------------------------------------------------------
void COrion::ProcessDelayedClicks()
{
	if (g_ClickObject.Enabled && g_ClickObject.Timer < g_Ticks)
	{
		uint serial = 0;

		if (g_ClickObject.Object() != NULL)
			serial = g_ClickObject.Object()->Serial;

		if (g_ClickObject.Gump() == NULL)
		{
			if (serial)
			{
				NameReq(serial);

				if (serial < 0x40000000)
				{
					if (!g_ConfigManager.HoldShiftForContextMenus || g_ShiftPressed)
						CPacketRequestPopupMenu(serial).Send();
				}
			}
		}
		else
		{
			g_ClickObject.Gump()->DelayedClick(g_ClickObject.Object());

			/*if (g_ClickObject.GumpType == GT_SPELLBOOK)
			{
				TGumpSpellbook *gump = (TGumpSpellbook*)GumpManager->UpdateGump(g_ClickObject.Serial, g_ClickObject.GumpID, g_ClickObject.GumpType);

				if (gump != NULL)
				{
					gump->Page = g_ClickObject.GumpButtonID;
					Orion->PlaySoundEffect(0x0055);
				}
			}*/
		}

		g_ClickObject.Clear();
	}
}
//----------------------------------------------------------------------------------
void COrion::Process(const bool &rendering)
{
	if (g_CurrentScreen == NULL)
		return;

	static uint removeUnusedTexturesTime = 0;

	g_ConnectionManager.Recv();
	g_MouseManager.Update();

	if (g_GameState >= GS_CHARACTER && g_LastSendTime + SEND_TIMEOUT_DELAY < g_Ticks)
	{
		g_PingCount++;

		BYTE ping[2] = { 0x73, g_PingSequence };

		Send(ping, 2);
	}

	g_AltPressed = GetAsyncKeyState(VK_MENU) & 0x80000000;
	g_CtrlPressed = GetAsyncKeyState(VK_CONTROL) & 0x80000000;
	g_ShiftPressed = GetAsyncKeyState(VK_SHIFT) & 0x80000000;

	if (g_GameState >= GS_GAME) // || g_GameState == GS_GAME_BLOCKED)
	{
		g_ShowGumpLocker = g_ConfigManager.LockGumpsMoving && g_AltPressed && g_CtrlPressed;

		g_SelectedObject.Clear();

		CWalkData *wd = g_Player->m_WalkStack.m_Items;

		if (wd != NULL)
		{
			g_RemoveRangeXY.X = wd->X;
			g_RemoveRangeXY.Y = wd->Y;
		}
		else
		{
			g_RemoveRangeXY.X = g_Player->X;
			g_RemoveRangeXY.Y = g_Player->Y;
		}

		ProcessStaticAnimList();
		RemoveRangedObjects();

		g_EffectManager.UpdateEffects();

		CGumpBuff *gumpBuff = (CGumpBuff*)g_GumpManager.GetGump(g_PlayerSerial, 0, GT_BUFF);

		if (gumpBuff != NULL)
			gumpBuff->UpdateBuffIcons();

		g_World->ProcessAnimation();

		g_PathFinder.ProcessAutowalk();

		bool canRenderSelect = false;

		if (g_GameState == GS_GAME)
		{
			g_MouseManager.ProcessWalking();

			g_MacroManager.Execute();

			ProcessDelayedClicks();

			canRenderSelect = true;

			//Game window scope
			if (g_PressedObject.LeftGump() == NULL && g_PressedObject.LeftObject() != NULL && g_PressedObject.LeftObject()->IsGUI())
				canRenderSelect = false;
		}

		if (rendering)
		{
			g_GameScreen.CalculateGameWindowBounds();

			g_GameScreen.CalculateRenderList();
			g_GameScreen.RenderListInitalized = true;


			if (!IsIconic(g_OrionWindow.Handle))
			{
				if (canRenderSelect)
				{
					g_GameScreen.Render(false);

					CGump::ProcessListing();
				}

				g_GameScreen.PrepareContent();

				g_GameScreen.Render(true);
			}

			g_GameScreen.RenderListInitalized = false;

			g_MapManager->Init(true);

			for (CORPSE_LIST_MAP::iterator i = g_CorpseSerialList.begin(); i != g_CorpseSerialList.end(); i++)
			{
				if (i->second < g_Ticks)
					i = g_CorpseSerialList.erase(i);
			}
		}
	}
	else if (rendering)
	{
		if (!IsIconic(g_OrionWindow.Handle))
		{
			g_CurrentScreen->Render(false);

			g_CurrentScreen->PrepareContent();

			CGump::ProcessListing();

			g_CurrentScreen->Render(true);
		}
	}

	if (removeUnusedTexturesTime < g_Ticks)
	{
		ClearUnusedTextures();
		removeUnusedTexturesTime = g_Ticks + CLEAR_TEXTURES_DELAY;
	}
}
//----------------------------------------------------------------------------------
void COrion::LoadStartupConfig()
{
	char buf[MAX_PATH] = { 0 };
	sprintf_s(buf, "Desktop\\%s\\%s\\0x%08X\\options_debug.cuo", g_MainScreen.m_Account->c_str(), g_ServerList.GetSelectedServer()->Name.c_str(), g_PlayerSerial);

	g_ConfigManager.Load(g_App.FilePath(buf));

	g_SoundManager.SetMusicVolume(g_ConfigManager.MusicVolume);

	if (!g_ConfigManager.Sound)
		AdjustSoundEffects(g_Ticks + 100000);

	if (!g_ConfigManager.Music)
		g_SoundManager.StopMusic();
}
//----------------------------------------------------------------------------------
void COrion::LoadPluginConfig()
{
	/*PluginClientInterface.GL = &g_Interface_GL;
	PluginClientInterface.UO = &g_Interface_UO;
	PluginClientInterface.ClilocManager = &g_Interface_ClilocManager;
	PluginClientInterface.ColorManager = &g_Interface_ColorManager;
	PluginClientInterface.PathFinder = &g_Interface_PathFinder;

	TMappedHeader file;
	memset(&file, 0, sizeof(file));

	if (FileManager.LoadFileToMemory(file, FilePath("Client.cuo").c_str()))
	{
		BYTE ver = file.ReadByte();

		file.Move(2);
		int len = file.ReadByte();
		file.Move(len + 39);

		if (ver >= 2)
		{
			file.Move(1);

			if (ver >= 3)
				file.Move(1);

			char count = file.ReadChar();

			IFOR(i, 0, count)
			{
				short len = file.ReadShort();
				string name = file.ReadString(len);

				DWORD password = file.ReadWord() << 16;
				DWORD flags = file.ReadDWord();
				password |= file.ReadWord();

				len = file.ReadShort();
				string subName = file.ReadString(len);

				HMODULE dll = LoadLibraryA(FilePath(name).c_str());

				if (dll != NULL)
				{
					typedef void __cdecl dllFunc(PPLUGIN_INTERFACE);

					dllFunc *initFunc = (dllFunc*)GetProcAddress(dll, subName.c_str());
					TPlugin *plugin = NULL;

					if (initFunc != NULL)
					{
						plugin = new TPlugin(flags);

						initFunc(plugin->m_PPS);

						if (password && (DWORD)plugin->m_PPS->Owner != password)
						{
							delete plugin;
							plugin = NULL;
						}
					}

					if (plugin == NULL)
						FreeLibrary(dll);
					else
					{
						plugin->m_PPS->Owner = plugin;

						if (plugin->CanClientAccess())
							plugin->m_PPS->Client = &PluginClientInterface;

						if (plugin->CanParseRecv())
							plugin->m_PPS->Recv = PluginRecvFunction;

						if (plugin->CanParseSend())
							plugin->m_PPS->Send = PluginSendFunction;

						initFunc(plugin->m_PPS);

						PluginManager->Add(plugin);
					}
				}
			}
		}

		FileManager.UnloadFileFromMemory(file);
	}*/
}
//----------------------------------------------------------------------------------
void COrion::LoadLocalConfig()
{
	if (g_ConfigLoaded)
		return;

	/*char lco[150] = {0};
	sprintf(lco, "Login confirm to %s", ServerList.GetServerName().c_str());

	CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0x35, lco);
	CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "Welcome to Ultima Online!");*/

	g_CheckContainerStackTimer = g_Ticks + 1000;

	char buf[MAX_PATH] = { 0 };
	sprintf_s(buf, "Desktop\\%s\\%s\\0x%08X", g_MainScreen.m_Account->c_str(), g_ServerList.GetSelectedServer()->Name.c_str(), g_PlayerSerial);

	string path = g_App.FilePath(buf);

	if (!g_ConfigManager.Load(path + "\\options_debug.cuo"))
	{
		if (!g_ConfigManager.Load(g_App.FilePath("options_debug.cuo")))
		{
			g_ConfigManager.Init();

			if (g_GameState >= GS_GAME)
			{
				SendMessage(g_OrionWindow.Handle, WM_SYSCOMMAND, SC_RESTORE, 0);
				SendMessage(g_OrionWindow.Handle, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
			}
		}
	}

	if (!g_SkillGroupManager.Load(path + "\\skills_debug.cuo"))
		g_SkillGroupManager.Load(g_App.FilePath("skills_debug.cuo"));

	if (!g_MacroManager.Load(path + "\\macros_debug.cuo", path + "\\macros.txt"))
	{
		if (!g_MacroManager.Load(g_App.FilePath("\\macros_debug.cuo"), g_App.FilePath("\\macros.txt")))
		{
			//Создать стандартные макросы
		}
	}

	g_GumpManager.Load(path + "\\gumps_debug.cuo");

	if (g_ConfigManager.OffsetInterfaceWindows)
		g_ContainerRect.MakeDefault();

	if (g_ConfigManager.GetConsoleNeedEnter())
		g_EntryPointer = NULL;
	else
		g_EntryPointer = &g_GameConsole;

	g_SoundManager.SetMusicVolume(g_ConfigManager.MusicVolume);

	if (!g_ConfigManager.Sound)
		AdjustSoundEffects(g_Ticks + 100000);

	if (!g_ConfigManager.Music)
		g_SoundManager.StopMusic();

	g_ConfigLoaded = true;
}
//----------------------------------------------------------------------------------
void COrion::SaveLocalConfig()
{
	if (!g_ConfigLoaded)
		return;

	string path = g_App.FilePath("Desktop");
	CreateDirectoryA(path.c_str(), NULL);

	path += string("\\") + g_MainScreen.m_Account->c_str();
	CreateDirectoryA(path.c_str(), NULL);

	path += string("\\") + g_ServerList.GetSelectedServer()->Name;
	CreateDirectoryA(path.c_str(), NULL);

	char serbuf[20] = { 0 };
	sprintf_s(serbuf, "\\0x%08X", g_PlayerSerial);

	path += serbuf;
	CreateDirectoryA(path.c_str(), NULL);

	g_ConfigManager.Save(path + "\\options_debug.cuo");
	g_SkillGroupManager.Save(path + "\\skills_debug.cuo");
	g_MacroManager.Save(path + "\\macros_debug.cuo");
	g_GumpManager.Save(path + "\\gumps_debug.cuo");

	g_ConfigManager.Save(g_App.FilePath("options_debug.cuo"));
	g_MacroManager.Save(g_App.FilePath("macros_debug.cuo"));

	if (g_Player != NULL)
	{
		path += string("_") + g_Player->Name + ".cuo";

		if (!PathFileExistsA(path.c_str()))
		{
			FILE *file = NULL;
			fopen_s(&file, path.c_str(), "wb");
			fclose(file);
		}
	}
}
//----------------------------------------------------------------------------------
void COrion::ClearUnusedTextures()
{
	if (g_GameState < GS_GAME)
		return;

	static int clearMap = 0;

	if (clearMap == 1)
	{
		clearMap = 2;

		g_MapManager->ClearUnusedBlocks();

		return;
	}
	else if (clearMap == 2)
	{
		g_AnimationManager.ClearUnusedTextures(g_Ticks);
		
		clearMap = 0;

		return;
	}

	clearMap = 1;

	m_StaticDataIndex[0x0EED].LastAccessTime = g_Ticks;
	m_StaticDataIndex[0x0EEE].LastAccessTime = g_Ticks;
	m_StaticDataIndex[0x0EEF].LastAccessTime = g_Ticks;

	IFOR(i, 0, 2)
	{
		IFOR(j, 0, 16)
			m_StaticDataIndex[g_CursorData[i][j]].LastAccessTime = g_Ticks;
	}

	IFOR(i, 0, 8)
		m_StaticDataIndex[g_QuestArrow.m_Gump + i].LastAccessTime = g_Ticks;

	g_GumpManager.PrepareTextures();

	g_Ticks -= CLEAR_TEXTURES_DELAY;
	
	PVOID lists[6] =
	{
		&m_UsedLandList,
		&m_UsedStaticList,
		&m_UsedGumpList,
		&m_UsedTextureList,
		&m_UsedLightList,
		&m_UsedSoundList
	};

	IFOR(i, 0, 6)
	{
		if (i < 5)
		{
			deque<CIndexObject*> *list = (deque<CIndexObject*>*)lists[i];

			for (deque<CIndexObject*>::iterator it = list->begin(); it != list->end();)
			{
				CIndexObject *obj = *it;

				if (obj->Texture != NULL && obj->LastAccessTime < g_Ticks)
				{
					delete obj->Texture;
					obj->Texture = NULL;

					it = list->erase(it);
				}
				else
					it++;
			}

			list->clear();
		}
		else
		{
			deque<CIndexSound*> *list = (deque<CIndexSound*>*)lists[i];

			for (deque<CIndexSound*>::iterator it = list->begin(); it != list->end();)
			{
				CIndexSound *obj = *it;

				if (obj->m_Stream != 0 && obj->LastAccessTime < g_Ticks)
				{
					BASS_StreamFree(obj->m_Stream);
					obj->m_Stream = 0;

					it = list->erase(it);
				}
				else
					it++;
			}

			list->clear();
		}
	}

	AdjustSoundEffects(g_Ticks);

	g_Ticks += CLEAR_TEXTURES_DELAY;
}
//----------------------------------------------------------------------------------
void COrion::Connect()
{
	InitScreen(GS_MAIN_CONNECT);

	Process(true);

	g_ConnectionManager.Disconnect();

	g_ConnectionManager.Init(); //Configure

	string login = "";
	int port;

	LoadLogin(login, port);

	if (g_ConnectionManager.Connect(login, port, g_GameSeed))
	{
		g_ConnectionScreen.Connected = true;

		CPacketFirstLogin().Send();
	}
	else
	{
		g_ConnectionScreen.ConnectionFailed = true;
		g_ConnectionScreen.ErrorCode = 8;
	}
}
//----------------------------------------------------------------------------------
void COrion::Disconnect()
{
	/*if (g_PluginManager != NULL)
		g_PluginManager->Disconnect();*/

	g_SystemChat.Clear();
	g_Journal.Clear();

	g_ConnectionManager.Disconnect();

	g_Party.Leader = 0;
	g_Party.Inviter = 0;
	g_Party.Clear();
	
	g_GameConsole.ClearStack();

	g_ResizedGump = NULL;
}
//----------------------------------------------------------------------------------
int COrion::Send(puchar buf, const int &size)
{
	uint ticks = g_Ticks;
	g_TotalSendSize += size;

	CPacketInfo &type = g_PacketManager.GetInfo(*buf);

	LOG("--- ^(%d) s(+%d => %d) Client:: %s\n", ticks - g_LastPacketTime, size, g_TotalSendSize, type.Name);

	if (*buf == 0x80 || *buf == 0x91)
	{
		LOG_DUMP(buf, size / 2);
		LOG("**** PASSWORD CENSORED ****\n");
	}
	else
		LOG_DUMP(buf, size);

	g_LastPacketTime = ticks;
	g_LastSendTime = ticks;

	int result = 0;

	if (type.Direction != DIR_SEND && type.Direction != DIR_BOTH)
		LOG("Warning!!! Message direction invalid: 0x%02X\n", *buf);
	else
	{
		//if (g_PluginManager->PacketSend(buf, size))
			result = g_ConnectionManager.Send(buf, size);
	}

	return result;
}
//----------------------------------------------------------------------------------
void COrion::ServerSelection(int pos)
{
	InitScreen(GS_SERVER_CONNECT);
	Process(true);

	CServer *server = g_ServerList.Select(pos);

	//if (PluginManager != NULL)
	//	PluginManager->WindowProc(g_hWnd, UOMSG_SET_SERVER_NAME, (WPARAM)ServerList.GetServerName().c_str(), 0);

	CPacketSelectServer((uchar)server->Index).Send();
}
//----------------------------------------------------------------------------------
void COrion::RelayServer(const char *ip, int port, puchar gameSeed)
{
	memcpy(&g_GameSeed[0], &gameSeed[0], 4);
	g_ConnectionManager.Init(gameSeed);

	if (g_ConnectionManager.Connect(ip, port, gameSeed))
	{
		g_ConnectionScreen.Connected = true;

		//if (g_PluginManager != NULL)
		//	g_PluginManager->WindowProc(g_hWnd, UOMSG_SET_ACCOUNT, (WPARAM)MainScreen->m_Account->c_str(), 0);

		CPacketSecondLogin().Send();
	}
	else
	{
		g_ConnectionScreen.ConnectionFailed = true;
		g_ConnectionScreen.ErrorCode = 8;
	}
}
//----------------------------------------------------------------------------------
void COrion::CharacterSelection(int pos)
{
	InitScreen(GS_GAME_CONNECT);
	g_ConnectionScreen.Type = CST_GAME;

	//if (PluginManager != NULL)
	//	PluginManager->WindowProc(g_hWnd, UOMSG_SET_PLAYER_NAME, (WPARAM)CharacterList.GetName(pos).c_str(), 0);

	CPacketSelectCharacter(pos, g_CharacterList.GetName(pos)).Send();
}
//----------------------------------------------------------------------------------
void COrion::LoginComplete()
{
	if (!g_ConnectionScreen.Completed)
	{
		g_ConnectionScreen.Completed = true;

		char buf[256] = { 0 };
		sprintf_s(buf, "Ultima Online - %s (%s)", g_Player->Name.c_str(), g_ServerList.GetSelectedServer()->Name.c_str());

		g_OrionWindow.SetTitle(buf);

		//UO->SkillsReq(serial);
		CPacketClientVersion(m_ClientVersionText).Send();

		SkillsReq(g_PlayerSerial);
		StatusReq(g_PlayerSerial);
		PaperdollReq(g_PlayerSerial);

		/*TPacketOpenChat openChatPacket(L"");
		openChatPacket.Send();

		TPacketRazorAnswer razorPacket;
		razorPacket.Send();*/

		CPacketLanguage(g_Language.c_str()).Send();

		if (g_PacketManager.ClientVersion >= CV_306E)
			CPacketClientType(g_ClientFlag).Send();

		if (g_PacketManager.ClientVersion >= CV_305D)
			CPacketClientViewRange(g_ConfigManager.UpdateRange).Send();

		InitScreen(GS_GAME);

		LoadLocalConfig();
	}
}
//----------------------------------------------------------------------------------
void COrion::LoadLogin(string &login, int &port)
{
	WISP_FILE::CTextFileParser file(g_App.FilePath("login.cfg"), "=,", "#;", "");

	while (!file.IsEOF())
	{
		STRING_LIST strings = file.ReadTokens();

		if (strings.size() >= 3)
		{
			string lo = ToLowerA(strings[0]);

			if (lo == "loginserver")
			{
				login = strings[1];
				port = atoi(strings[2].c_str());
			}
		}
	}
}
//----------------------------------------------------------------------------------
void COrion::GoToWebLink(const string &url)
{
	if (url.length())
		ShellExecuteA(0, "Open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
}
//----------------------------------------------------------------------------------
void COrion::LoadTiledata(const int &landSize, const int &staticsSize)
{
	if (g_FileManager.m_TiledataMul.Size)
	{
		m_LandDataCount = landSize;
		m_LandData.resize(landSize);
		memcpy(&m_LandData[0], (PVOID)g_FileManager.m_TiledataMul.Start, landSize * sizeof(LAND_GROUP));

		m_StaticDataCount = staticsSize;
		m_StaticData.resize(staticsSize);
		memcpy(&m_StaticData[0], (PVOID)((uint)g_FileManager.m_TiledataMul.Start + 428032), staticsSize * sizeof(STATIC_GROUP));
	}
}
//----------------------------------------------------------------------------------
void COrion::LoadIndexFiles()
{
	PART_IDX_BLOCK LandArtPtr = (PART_IDX_BLOCK)g_FileManager.m_ArtIdx.Start;
	PART_IDX_BLOCK StaticArtPtr = (PART_IDX_BLOCK)((uint)g_FileManager.m_ArtIdx.Start + (0x4000 * sizeof(ART_IDX_BLOCK)));
	PGUMP_IDX_BLOCK GumpArtPtr = (PGUMP_IDX_BLOCK)g_FileManager.m_GumpIdx.Start;
	PTEXTURE_IDX_BLOCK TexturePtr = (PTEXTURE_IDX_BLOCK)g_FileManager.m_TextureIdx.Start;
	PMULTI_IDX_BLOCK MultiPtr = (PMULTI_IDX_BLOCK)g_FileManager.m_MultiIdx.Start;
	PSOUND_IDX_BLOCK SoundPtr = (PSOUND_IDX_BLOCK)g_FileManager.m_SoundIdx.Start;
	PLIGHT_IDX_BLOCK LightPtr = (PLIGHT_IDX_BLOCK)g_FileManager.m_LightIdx.Start;

	g_MultiIndexCount = g_FileManager.m_MultiIdx.Size / sizeof(MULTI_IDX_BLOCK);

	if (g_MultiIndexCount > 0x2000)
		g_MultiIndexCount = 0x2000;

	IFOR(i, 0, MAX_GUMP_DATA_INDEX_COUNT)
	{
		if (i < MAX_LAND_DATA_INDEX_COUNT)
		{
			CIndexObject &land = m_LandDataIndex[i];

			land.Address = LandArtPtr->Position;
			land.DataSize = LandArtPtr->Size;

			if (land.Address == 0xFFFFFFFF || !land.DataSize || land.DataSize == 0xFFFFFFFF)
			{
				land.Address = 0;
				land.DataSize = 0;
			}
			else
				land.Address = land.Address + (uint)g_FileManager.m_ArtMul.Start;

			land.Width = 0;
			land.Height = 0;
			land.Texture = NULL;
			land.LastAccessTime = 0;

			LandArtPtr++;

			CIndexObject &statics = m_StaticDataIndex[i];

			statics.Address = StaticArtPtr->Position;
			statics.DataSize = StaticArtPtr->Size;

			if (statics.Address == 0xFFFFFFFF || !statics.DataSize || statics.DataSize == 0xFFFFFFFF)
			{
				statics.Address = 0;
				statics.DataSize = 0;
			}
			else
				statics.Address = statics.Address + (uint)g_FileManager.m_ArtMul.Start;

			statics.Width = 0;
			statics.Height = 0;
			statics.Texture = NULL;
			statics.LastAccessTime = 0;

			StaticArtPtr++;

			if (i < MAX_LAND_TEXTURES_DATA_INDEX_COUNT)
			{
				CIndexObject &textures = m_TextureDataIndex[i];

				textures.Address = TexturePtr->Position;
				textures.DataSize = TexturePtr->Size;

				if (textures.Address == 0xFFFFFFFF || !textures.DataSize || textures.DataSize == 0xFFFFFFFF)
				{
					textures.Address = 0;
					textures.DataSize = 0;
				}
				else
					textures.Address = textures.Address + (uint)g_FileManager.m_TextureMul.Start;

				textures.Width = 0;
				textures.Height = 0;
				textures.Texture = NULL;
				textures.LastAccessTime = 0;

				TexturePtr++;

				if (i < MAX_SOUND_DATA_INDEX_COUNT)
				{
					CIndexSound &sound = m_SoundDataIndex[i];

					sound.Address = SoundPtr->Position;
					sound.DataSize = SoundPtr->Size;
					sound.LastAccessTime = 0;
					sound.Delay = 0;
					//sound.Sound = NULL;
					sound.m_Stream = 0;

					if (sound.Address == 0xFFFFFFFF || !sound.DataSize || sound.DataSize == 0xFFFFFFFF)
					{
						sound.Address = 0;
						sound.DataSize = 0;
					}
					else
						sound.Address = sound.Address + (uint)g_FileManager.m_SoundMul.Start;

					SoundPtr++;

					if (i < MAX_LIGHTS_DATA_INDEX_COUNT)
					{
						CIndexObject &light = m_LightDataIndex[i];

						light.Address = LightPtr->Position;
						light.DataSize = LightPtr->Size;
						light.Width = LightPtr->Width;
						light.Height = LightPtr->Height;
						light.LastAccessTime = 0;
						light.Texture = NULL;

						if (light.Address == 0xFFFFFFFF || !light.DataSize || light.DataSize == 0xFFFFFFFF)
						{
							light.Address = 0;
							light.DataSize = 0;
						}
						else
							light.Address = light.Address + (uint)g_FileManager.m_LightMul.Start;

						LightPtr++;
					}
				}
			}
		}

		CIndexObject &gump = m_GumpDataIndex[i];

		gump.Address = GumpArtPtr->Position;
		gump.DataSize = GumpArtPtr->Size;

		if (gump.Address == 0xFFFFFFFF || !gump.DataSize || gump.DataSize == 0xFFFFFFFF)
		{
			gump.Address = 0;
			gump.DataSize = 0;
		}
		else
			gump.Address = gump.Address + (uint)g_FileManager.m_GumpMul.Start;

		gump.Width = GumpArtPtr->Width;
		gump.Height = GumpArtPtr->Height;
		gump.Texture = NULL;
		gump.LastAccessTime = 0;

		GumpArtPtr++;

		if (i < g_MultiIndexCount)
		{
			CIndexMulti &multi = m_MultiDataIndex[i];

			multi.Address = MultiPtr->Position;
			multi.DataSize = MultiPtr->Size;

			if (multi.Address == 0xFFFFFFFF || !multi.DataSize || multi.DataSize == 0xFFFFFFFF)
			{
				multi.Address = 0;
				multi.DataSize = 0;
			}
			else
				multi.Address += (uint)g_FileManager.m_MultiMul.Start;

			multi.Count = (ushort)(multi.DataSize / sizeof(MULTI_IDX_BLOCK));

			MultiPtr++;
		}
	}
}
//----------------------------------------------------------------------------------
void COrion::UnloadIndexFiles()
{
	PVOID lists[6] =
	{
		&m_UsedLandList,
		&m_UsedStaticList,
		&m_UsedGumpList,
		&m_UsedTextureList,
		&m_UsedLightList,
		&m_UsedSoundList
	};

	IFOR(i, 0, 6)
	{
		if (i < 5)
		{
			deque<CIndexObject*> *list = (deque<CIndexObject*>*)lists[i];

			for (deque<CIndexObject*>::iterator it = list->begin(); it != list->end(); it++)
			{
				CIndexObject *obj = *it;

				if (obj->Texture != NULL)
				{
					delete obj->Texture;
					obj->Texture = NULL;
				}
			}

			list->clear();
		}
		else
		{
			deque<CIndexSound*> *list = (deque<CIndexSound*>*)lists[i];

			for (deque<CIndexSound*>::iterator it = list->begin(); it != list->end(); it++)
			{
				CIndexSound *obj = *it;

				if (obj->m_Stream != 0)
				{
					BASS_StreamFree(obj->m_Stream);
					obj->m_Stream = 0;
				}
			}

			list->clear();
		}
	}
}
//----------------------------------------------------------------------------------
void COrion::InitStaticAnimList()
{
	if (m_AnimData.size())
	{
		IFOR(i, 0, MAX_STATIC_DATA_INDEX_COUNT)
		{
			m_StaticDataIndex[i].Index = i;

			DWORD flags = m_StaticData[i / 32].Tiles[i % 32].Flags;

			m_StaticDataIndex[i].LightColor = CalculateLightColor((WORD)i);

			if (IsAnimated(flags))
				m_StaticAnimList.push_back(&m_StaticDataIndex[i]);
		}
	}
}
//----------------------------------------------------------------------------------
ushort COrion::CalculateLightColor(const ushort &id)
{
	ushort color = m_StaticData[id / 32].Tiles[id % 32].Hue;

	if (!color)
	{
		switch (id)
		{
			case 0x0B1A:
			case 0x0B1B:
			case 0x0B1C:
			case 0x0B1D:
			case 0x0B1E:
			case 0x0B1F:
			case 0x0B20:
			case 0x0B21:
			case 0x0B22:
			case 0x0B23:
			case 0x0B24:
			case 0x0B25:
			case 0x0B26:
			case 0x0B27:
			case 0x0B28:
			{
				color = 0x029A;
				break;
			}
			case 0x0E2D:
			case 0x0E2E:
			case 0x0E2F:
			case 0x0E30:
			{
				color = 0x003E;
				break;
			}
			case 0x088C:
			{
				color = 0x001F;
				break;
			}
			//fire pit
			case 0x0FAC:
			{
				color = 0x001E;
				break;
			}
			//forge
			case 0x0FB1:
			{
				color = 0x003C;
				break;
			}
			case 0x1647:
			{
				color = 0x003D;
				break;
			}
			//blue moongate
			case 0x0F6C:
			//moongate
			case 0x1FD4:
			{
				color = 0x0002;
				break;
			}
			//brazier
			case 0x0E31:
			case 0x0E32:
			case 0x0E33:
			case 0x19BB:
			case 0x1F2B:
			{
				color = 0x0028;
				break;
			}
			//lava
			case 0x3547:
			case 0x3548:
			case 0x3549:
			case 0x354A:
			case 0x354B:
			case 0x354C:
			{
				color = 0x001F;
				break;
			}
			default:
				break;
		}

		if (!color)
		{
			//wall sconce
			if (IN_RANGE(id, 0x09FB, 0x0A14))
				color = 0x001E;
			//lava
			else if (IN_RANGE(id, 0x12EE, 0x134D) || IN_RANGE(id, 0x306A, 0x329B) || IN_RANGE(id, 0x343B, 0x346C))
				color = 0x001F;
			//?
			else if (IN_RANGE(id, 0x3E27, 0x3E3A))
				color = 0x001F;
			//?
			else if (IN_RANGE(id, 0x3E02, 0x3E0B))
				color = 0x0001;
			//fire field
			else if (IN_RANGE(id, 0x398C, 0x399F))
				color = 0x001F;
			//paralyze field
			else if (IN_RANGE(id, 0x3967, 0x397A))
				color = 0x0006;
			//energy field
			else if (IN_RANGE(id, 0x3946, 0x3964))
				color = 0x0006;
			//poison field
			else if (IN_RANGE(id, 0x3914, 0x3929))
				color = 0x0001;
			//glowing runes
			else if (IN_RANGE(id, 0x0E5C, 0x0E6A))
				color = 0x0006;
			//?
			else if (IN_RANGE(id, 0x3E02, 0x3E0B))
				color = 0x0002;
			else if (IN_RANGE(id, 0x3E27, 0x3E3A) || IN_RANGE(id, 0x398C, 0x399F))
				color = 0x029A;
			else if (IN_RANGE(id, 0x1ECD, 0x1ECF) || IN_RANGE(id, 0x1ED0, 0x1ED2))
				color = 0x0001;
			else if (IN_RANGE(id, 0x19AB, 0x19B6) || IN_RANGE(id, 0x197A, 0x19A9))
				color = 0x003C;
			else if (IN_RANGE(id, 0x1853, 0x185A) || IN_RANGE(id, 0x1849, 0x1850))
				color = 0x003D;
			else if (IN_RANGE(id, 0x0DE1, 0x0DEA))
				color = 0x001F;
			else if (IN_RANGE(id, 0x0A15, 0x0A29))
				color = 0x029A;
			else if (IN_RANGE(id, 0x09FB, 0x0A14))
				color = 0x001E;
		}
	}

	return color;
}
//----------------------------------------------------------------------------------
void COrion::ProcessStaticAnimList()
{
	if (m_AnimData.size())
	{
		int delay = (g_ConfigManager.StandartItemsAnimationDelay ? 0x50 : 50);

		for (deque<CIndexObjectStatic*>::iterator i = m_StaticAnimList.begin(); i != m_StaticAnimList.end(); i++)
		{
			CIndexObjectStatic *obj = *i;

			if (obj->ChangeTime < g_Ticks)
			{
				uint addr = (obj->Index * 68) + 4 * ((obj->Index / 8) + 1);
				PANIM_DATA pad = (PANIM_DATA)(&m_AnimData[0] + addr);

				int offset = obj->AnimIndex;

				obj->ChangeTime = g_Ticks + (pad->FrameInterval * delay);

				if (offset < pad->FrameCount)
					obj->Offset = pad->FrameData[offset++];

				if (offset >= pad->FrameCount)
					offset = 0;

				obj->AnimIndex = offset;
			}
		}
	}
}
//----------------------------------------------------------------------------------
void COrion::PatchFiles()
{
	/*
	-map0 = 0;
	-staidx0 = 1;
	-statics0 = 2;
	+artidx = 3;
	+art = 4;
	-animidx = 5;
	-anim = 6;
	-soundidx = 7;
	-sound = 8;
	-texidx = 9;
	-texmaps = 10;
	+gumpidx = 11;
	+gumpart = 12;
	+multiidx = 13;
	+multi = 14;
	-skillsidx = 15;
	-skills = 16;
	+tiledata = 30;
	-animdata = 31;
	+hues = 32;
	*/

	if (!g_FileManager.UseVerdata || !g_FileManager.m_VerdataMul.Size)
	{
		g_ColorManager.CreateHuesPalette();
		return;
	}

	int dataCount = *(pint)g_FileManager.m_VerdataMul.Start;

	uint vAddr = (uint)g_FileManager.m_VerdataMul.Start;

	IFOR(i, 0, dataCount)
	{
		PVERDATA_HEADER vh = (PVERDATA_HEADER)(vAddr + 4 + (i * sizeof(VERDATA_HEADER)));

		if (vh->FileID == 4) //Art
		{
			if (vh->BlockID >= MAX_LAND_DATA_INDEX_COUNT) //Run
			{
				WORD ID = (WORD)vh->BlockID - MAX_LAND_DATA_INDEX_COUNT;
				m_StaticDataIndex[ID].Address = vAddr + vh->Position;
				m_StaticDataIndex[ID].DataSize = vh->Size;
			}
			else //Raw
			{
				m_LandDataIndex[vh->BlockID].Address = vAddr + vh->Position;
				m_LandDataIndex[vh->BlockID].DataSize = vh->Size;
			}
		}
		else if (vh->FileID == 12) //Gumpart
		{
			m_GumpDataIndex[vh->BlockID].Address = vAddr + vh->Position;
			m_GumpDataIndex[vh->BlockID].DataSize = vh->Size;
			m_GumpDataIndex[vh->BlockID].Width = vh->GumpData >> 16;
			m_GumpDataIndex[vh->BlockID].Height = vh->GumpData & 0xFFFF;
		}
		else if (vh->FileID == 14 && (int)vh->BlockID < g_MultiIndexCount) //Multi
		{
			m_MultiDataIndex[vh->BlockID].Address = vAddr + vh->Position;
			m_MultiDataIndex[vh->BlockID].DataSize = vh->Size;
			m_MultiDataIndex[vh->BlockID].Count = (WORD)(vh->Size / sizeof(MULTI_IDX_BLOCK));
		}
		else if (vh->FileID == 16 && (int)vh->BlockID < g_SkillsCount) //Skills
		{
			int index = (int)vh->BlockID;
			g_Skills[index].Button = *(puchar)(vAddr + vh->Position);

			char namebuf[128] = { 0 };
			memcpy(&namebuf[0], (char*)(vAddr + vh->Position + 1), vh->Size - 1);

			if (strlen(namebuf) > 0)
			{
				g_Skills[index].Name = namebuf;
				g_Skills[index].m_Texture.Clear();
				g_FontManager.GenerateA(9, g_Skills[index].m_Texture, namebuf, 0x0288);
			}
			else
			{
				g_Skills[index].m_Texture.Clear();
				g_Skills[i].Name = "";
			}
		}
		else if (vh->FileID == 30) //Tiledata
		{
			if (vh->Size == 836)
			{
				if (vh->BlockID >= 512)
					continue;

				memcpy(&m_LandData[vh->BlockID], (PVOID)(vAddr + vh->Position), sizeof(LAND_GROUP));
			}
			else if (vh->Size == 1188)
			{
				int bID = (int)vh->BlockID - 0x0200;

				if (bID < 0 || bID >= 512)
					continue;

				memcpy(&m_StaticData[bID], (PVOID)(vAddr + vh->Position), sizeof(STATIC_GROUP));
			}
		}
		else if (vh->FileID == 32) //Hues
		{
			if ((int)vh->BlockID < g_ColorManager.GetHuesCount())
			{
				PVERDATA_HUES_GROUP group = (PVERDATA_HUES_GROUP)(vAddr + vh->Position);
				g_ColorManager.SetHuesBlock(vh->BlockID, group);
			}
		}
		else if (vh->FileID != 5 && vh->FileID != 6) //no Anim / Animidx
		{
			LOG("Unused verdata block (fileID) = %i (BlockID+ %i\n", vh->FileID, vh->BlockID);
		}
	}

	g_ColorManager.CreateHuesPalette();
}
//----------------------------------------------------------------------------------
void COrion::IndexReplaces()
{
	WISP_FILE::CTextFileParser newDataParser("", " \t,{}", "#;//", "");
	WISP_FILE::CTextFileParser artParser(g_App.FilePath("Art.def"), " \t", "#;//", "{}");
	WISP_FILE::CTextFileParser textureParser(g_App.FilePath("TexTerr.def"), " \t", "#;//", "{}");
	WISP_FILE::CTextFileParser gumpParser(g_App.FilePath("Gump.def"), " \t", "#;//", "{}");
	WISP_FILE::CTextFileParser multiParser(g_App.FilePath("Multi.def"), " \t", "#;//", "{}");
	WISP_FILE::CTextFileParser soundParser(g_App.FilePath("Sound.def"), " \t", "#;//", "{}");
	WISP_FILE::CTextFileParser mp3Parser(g_App.FilePath("Music\\Digital\\Config.txt"), " ,", "#;", "");

	while (!artParser.IsEOF())
	{
		STRING_LIST strings = artParser.ReadTokens();

		if (strings.size() >= 3)
		{
			int index = atoi(strings[0].c_str());

			if (index < 0 || index >= MAX_LAND_DATA_INDEX_COUNT + MAX_STATIC_DATA_INDEX_COUNT)
				continue;

			STRING_LIST newArt = newDataParser.GetTokens(strings[1].c_str());

			int size = (int)newArt.size();

			IFOR(i, 0, size)
			{
				int checkIndex = atoi(newArt[i].c_str());

				if (checkIndex < 0 || checkIndex >= MAX_LAND_DATA_INDEX_COUNT + MAX_STATIC_DATA_INDEX_COUNT)
					continue;

				if (index < MAX_LAND_DATA_INDEX_COUNT && checkIndex < MAX_LAND_DATA_INDEX_COUNT && m_LandDataIndex[checkIndex].Address != NULL)
				{
					memcpy(&m_LandDataIndex[index], &m_LandDataIndex[checkIndex], sizeof(CIndexObject));
					m_LandDataIndex[index].Texture = NULL;
					m_LandDataIndex[index].Color = atoi(strings[2].c_str());

					break;
				}
				else if (index >= MAX_LAND_DATA_INDEX_COUNT && checkIndex >= MAX_LAND_DATA_INDEX_COUNT && m_StaticDataIndex[checkIndex - 0x4000].Address != NULL)
				{
					checkIndex -= MAX_LAND_DATA_INDEX_COUNT;
					index -= MAX_LAND_DATA_INDEX_COUNT;

					memcpy(&m_StaticDataIndex[index], &m_StaticDataIndex[checkIndex], sizeof(CIndexObjectStatic));
					m_StaticDataIndex[index].Texture = NULL;
					m_StaticDataIndex[index].Color = atoi(strings[2].c_str());

					break;
				}
			}
		}
	}

	while (!textureParser.IsEOF())
	{
		STRING_LIST strings = textureParser.ReadTokens();

		if (strings.size() >= 3)
		{
			int index = atoi(strings[0].c_str());

			if (index < 0)
				continue;

			STRING_LIST newTexture = newDataParser.GetTokens(strings[1].c_str());

			int size = (int)newTexture.size();

			IFOR(i, 0, size)
			{
				int checkIndex = atoi(newTexture[i].c_str());

				if (checkIndex < 0)
					continue;

				if (index < MAX_LAND_TEXTURES_DATA_INDEX_COUNT && checkIndex < MAX_LAND_TEXTURES_DATA_INDEX_COUNT && m_TextureDataIndex[checkIndex].Address != NULL)
				{
					memcpy(&m_TextureDataIndex[index], &m_TextureDataIndex[checkIndex], sizeof(CIndexObject));
					m_TextureDataIndex[index].Texture = NULL;
					m_TextureDataIndex[index].Color = atoi(strings[2].c_str());

					break;
				}
			}
		}
	}

	while (!gumpParser.IsEOF())
	{
		STRING_LIST strings = gumpParser.ReadTokens();

		if (strings.size() >= 3)
		{
			int index = atoi(strings[0].c_str());

			if (index < 0 || index >= MAX_GUMP_DATA_INDEX_COUNT)
				continue;

			STRING_LIST newGump = newDataParser.GetTokens(strings[1].c_str());

			int size = (int)newGump.size();

			IFOR(i, 0, size)
			{
				int checkIndex = atoi(newGump[i].c_str());

				if (checkIndex < 0 || checkIndex >= MAX_GUMP_DATA_INDEX_COUNT || m_GumpDataIndex[checkIndex].Address == NULL)
					continue;

				memcpy(&m_GumpDataIndex[index], &m_GumpDataIndex[checkIndex], sizeof(CIndexObject));
				m_GumpDataIndex[index].Texture = NULL;
				m_GumpDataIndex[index].Color = atoi(strings[2].c_str());

				break;
			}
		}
	}

	while (!multiParser.IsEOF())
	{
		STRING_LIST strings = multiParser.ReadTokens();

		if (strings.size() >= 3)
		{
			int index = atoi(strings[0].c_str());

			if (index < 0 || index >= g_MultiIndexCount)
				continue;

			STRING_LIST newMulti = newDataParser.GetTokens(strings[1].c_str());

			int size = (int)newMulti.size();

			IFOR(i, 0, size)
			{
				int checkIndex = atoi(newMulti[i].c_str());

				if (checkIndex < 0 || checkIndex >= g_MultiIndexCount && m_MultiDataIndex[checkIndex].Address == NULL)
					continue;

				memcpy(&m_MultiDataIndex[index], &m_MultiDataIndex[checkIndex], sizeof(CIndexMulti));

				break;
			}
		}
	}

	while (!soundParser.IsEOF())
	{
		std::vector<std::string> strings = soundParser.ReadTokens();

		if (strings.size() >= 2)
		{
			int index = atoi(strings[0].c_str());

			if (index < 0 || index >= MAX_SOUND_DATA_INDEX_COUNT)
				continue;

			STRING_LIST newSound = newDataParser.GetTokens(strings[1].c_str());

			int size = (int)newSound.size();

			IFOR(i, 0, size)
			{
				int checkIndex = atoi(newSound[i].c_str());

				if (checkIndex < -1 || checkIndex >= MAX_SOUND_DATA_INDEX_COUNT)
					continue;

				CIndexSound &in = m_SoundDataIndex[index];

				if (checkIndex == -1)
				{
					in.Address = 0;
					in.DataSize = 0;
					in.Delay = 0;
					in.LastAccessTime = 0;
				}
				else
				{
					CIndexSound &out = m_SoundDataIndex[checkIndex];

					if (out.Address == NULL)
						continue;

					in.Address = out.Address;
					in.DataSize = out.DataSize;
					in.Delay = out.Delay;
					in.LastAccessTime = out.LastAccessTime;
				}

				in.m_WaveFile.clear();
				in.m_Stream = NULL;

				break;
			}
		}
	}

	while (!mp3Parser.IsEOF())
	{
		std::vector<std::string> strings = mp3Parser.ReadTokens();
		int size = strings.size();
		if (size > 0)
		{
			CIndexMusic &mp3 = m_MP3Data[std::stoi(strings[0])];

			if (size > 1)
				mp3.FilePath = g_App.FilePath(("Music\\Digital\\" + strings[1] + ".mp3").c_str());
			if (size > 2)
				mp3.Loop = true;
		}
	}
}
//----------------------------------------------------------------------------------
bool COrion::LoadSkills()
{
	if (!g_FileManager.m_SkillsIdx.Size || !g_FileManager.m_SkillsMul.Size || g_SkillsCount)
		return false;

	PSKILLS_IDX_BLOCK sidx = NULL, start = NULL, end = NULL;

	int cnt = g_FileManager.m_SkillsIdx.Size / sizeof(SKILLS_IDX_BLOCK);

	IFOR(i, 0, cnt)
	{
		sidx = (PSKILLS_IDX_BLOCK)((uint)g_FileManager.m_SkillsIdx.Start + (i * sizeof(SKILLS_IDX_BLOCK)));

		if (sidx->Size && sidx->Position != 0xFFFFFFFF && sidx->Size != 0xFFFFFFFF)
		{
			if (start == NULL)
				start = sidx;
			end = sidx;

			//LOG("Skill load[%d] = %s\n", i, (char*)((DWORD)FileManager.SkillsMul.Address + sidx->Position + 1));
		}
	}

	g_SkillsCount = (int)(end - start) + 1;

	if (g_SkillsCount < 2 || g_SkillsCount > 60)
	{
		g_SkillsCount = 0;

		return false;
	}

	g_Skills.resize(g_SkillsCount);

	for (int i = 0; ; i++, start++)
	{
		g_Skills[i].Button = *(puchar)((uint)g_FileManager.m_SkillsMul.Start + start->Position);

		char namebuf[128] = { 0 };
		memcpy(namebuf, (char*)((uint)g_FileManager.m_SkillsMul.Start + start->Position + 1), start->Size - 1);

		if (strlen(namebuf) > 0)
			g_Skills[i].Name = namebuf;
		else
			g_Skills[i].Name = "NoNameSkill";

		if (start->Position == end->Position)
			break;
	}

	return true;
}
//----------------------------------------------------------------------------------
void COrion::CreateObjectHandlesBackground()
{
	CGLTexture *th[9] = { NULL };
	ushort gumpID[9] = { 0 };

	IFOR(i, 0, 9)
	{
		CGLTexture *pth = ExecuteGump(0x24EA + i);

		if (pth == NULL)
		{
			LOG("Error!!! Failed to create Object Handles background data!\n");
			return;
		}

		if (i == 4)
		{
			th[8] = pth;
			gumpID[8] = 0x24EA + i;
		}
		else if (i > 4)
		{
			th[i - 1] = pth;
			gumpID[i - 1] = 0x24EA + i;
		}
		else
		{
			th[i] = pth;
			gumpID[i] = 0x24EA + i;
		}
	}

	IFOR(i, 0, 8)
	{
		if (i == 3 || i == 4)
			continue;

		CIndexObject &io = m_GumpDataIndex[gumpID[i]];

		int drawWidth = io.Width;
		int drawHeight = io.Height;
		int drawX = 0;
		int drawY = 0;

		switch (i)
		{
			case 1:
			{
				drawX = th[0]->Width;

				drawWidth = g_ObjectHandlesWidth - th[0]->Width - th[2]->Width;

				break;
			}
			case 2:
			{
				drawX = g_ObjectHandlesWidth - drawWidth;

				break;
			}
			case 3:
			{
				drawY = th[0]->Height;

				drawHeight = g_ObjectHandlesHeight - th[0]->Height - th[5]->Height;

				break;
			}
			case 4:
			{
				drawX = g_ObjectHandlesWidth - drawWidth;
				drawY = th[2]->Height;

				drawHeight = g_ObjectHandlesHeight - th[2]->Height - th[7]->Height;

				break;
			}
			case 5:
			{
				drawY = g_ObjectHandlesHeight - drawHeight;

				break;
			}
			case 6:
			{
				drawX = th[5]->Width;
				drawY = g_ObjectHandlesHeight - drawHeight;

				drawWidth = g_ObjectHandlesWidth - th[5]->Width - th[7]->Width;

				break;
			}
			case 7:
			{
				drawX = g_ObjectHandlesWidth - drawWidth;
				drawY = g_ObjectHandlesHeight - drawHeight;

				break;
			}
			case 8:
			{
				drawX = th[0]->Width;
				drawY = th[0]->Height;

				drawWidth = g_ObjectHandlesWidth - th[0]->Width - th[2]->Width;
				drawHeight = g_ObjectHandlesHeight - th[2]->Height - th[7]->Height;

				break;
			}
			default:
				break;
		}

		if (drawX < 0)
			drawX = 0;

		if (drawY < 0)
			drawY = 0;

		drawWidth += drawX;
		if (drawWidth > g_ObjectHandlesWidth)
			drawWidth = g_ObjectHandlesWidth - drawX;

		drawHeight += drawY;
		if (drawHeight > g_ObjectHandlesHeight)
			drawHeight = g_ObjectHandlesHeight - drawY;

		USHORT_LIST pixels = g_MulReader->GetGumpPixels(io);

		if (pixels.size())
		{
			int gumpWidth = io.Width;
			int gumpHeight = io.Height;
			int srcX = 0;

			IFOR(x, drawX, drawWidth)
			{
				int srcY = 0;

				IFOR(y, drawY, drawHeight)
				{
					ushort &pixel = g_ObjectHandlesBackgroundPixels[(y * g_ObjectHandlesWidth) + x];

					if (pixel == 0)
						pixel = pixels[((srcY % gumpHeight) * gumpWidth) + (srcX % gumpWidth)];

					srcY++;
				}

				srcX++;
			}
		}
	}
}
//----------------------------------------------------------------------------------
void COrion::LoadShaders()
{
	g_CurrentShader = NULL;

#if UO_USE_SHADER_FILES == 1
	TMappedHeader frag;
	memset(&frag, 0, sizeof(TMappedHeader));
	TMappedHeader vert;
	memset(&vert, 0, sizeof(TMappedHeader));

	FileManager.LoadFileToMemory(vert, FilePath("shaders\\Shader.vert").c_str());
	FileManager.LoadFileToMemory(frag, FilePath("shaders\\DeathShader.frag").c_str());

	DeathShader = new TDeathShader((char*)vert.Address, (char*)frag.Address);

	FileManager.UnloadFileFromMemory(frag);



	FileManager.LoadFileToMemory(frag, FilePath("shaders\\ColorizerShader.frag").c_str());

	ColorizerShader = new TColorizerShader((char*)vert.Address, (char*)frag.Address);

	FileManager.UnloadFileFromMemory(frag);



	FileManager.LoadFileToMemory(frag, FilePath("shaders\\FontColorizerShader.frag").c_str());

	FontColorizerShader = new TColorizerShader((char*)vert.Address, (char*)frag.Address);

	FileManager.UnloadFileFromMemory(frag);



	FileManager.LoadFileToMemory(frag, FilePath("shaders\\LightColorizerShader.frag").c_str());

	LightColorizerShader = new TColorizerShader((char*)vert.Address, (char*)frag.Address);

	FileManager.UnloadFileFromMemory(frag);
	FileManager.UnloadFileFromMemory(vert);
#else
	g_DeathShader = new CDeathShader(g_Vert_ShaderData, g_Frag_DeathShaderData);
	g_ColorizerShader = new CColorizerShader(g_Vert_ShaderData, g_Frag_ColorizerShaderData);
	g_FontColorizerShader = new CColorizerShader(g_Vert_ShaderData, g_Frag_FontShaderData);
	g_LightColorizerShader = new CColorizerShader(g_Vert_ShaderData, g_Frag_LightShaderData);
#endif
}
//----------------------------------------------------------------------------------
void COrion::LoadClientStartupConfig()
{
	g_ConfigManager.Load(g_App.FilePath("options_debug.cuo"));

	g_SoundManager.SetMusicVolume(g_ConfigManager.MusicVolume);

	if (g_ConfigManager.Music)
		PlayMusic(8);
}
//----------------------------------------------------------------------------------
void COrion::PlayMusic(const int &index, const bool &warmode)
{
	//Тимур, здесь прикручивай взависимости от конфига играть мп3 или миди.
	//Сейчас только мп3 будет играть.

	if (index >= MAX_MUSIC_DATA_INDEX_COUNT)
		return;

	CIndexMusic &mp3Info = m_MP3Data[index];
	g_SoundManager.PlayMP3(mp3Info.FilePath, mp3Info.Loop, warmode);
	//SoundManager.PlayMidi(index);
}
//----------------------------------------------------------------------------------
void COrion::PlaySoundEffect(const ushort &id, float volume)
{
	if (id >= 0x0800 || !g_ConfigManager.Sound)
		return;

	CIndexSound &is = m_SoundDataIndex[id];

	if (is.Address == 0)
		return;

	if (is.m_Stream == 0)
	{
		is.m_Stream = g_SoundManager.LoadSoundEffect(is);

		if (is.m_Stream == 0)
			return;

		m_UsedSoundList.push_back(&m_SoundDataIndex[id]);
	}
	else
	{
		if (is.LastAccessTime + is.Delay > g_Ticks)
			return;

		g_SoundManager.FreeStream(is.m_Stream);

		is.m_Stream = g_SoundManager.LoadSoundEffect(is);
	}

	if (volume <= 0)
		volume = g_SoundManager.GetVolumeValue();

	if (volume > 0)
	{
		g_SoundManager.PlaySoundEffect(is.m_Stream, volume);
		is.LastAccessTime = g_Ticks;
	}
}
//----------------------------------------------------------------------------------
void COrion::AdjustSoundEffects(const uint &ticks, const float &volume)
{
	for (deque<CIndexSound*>::iterator i = m_UsedSoundList.begin(); i != m_UsedSoundList.end();)
	{
		CIndexSound *obj = *i;

		if (obj->m_Stream != 0 && obj->LastAccessTime + obj->Delay < ticks)
		{
			if (volume > 0)
				BASS_ChannelSetAttribute(obj->m_Stream, BASS_ATTRIB_VOL, volume);
			else
			{
				BASS_StreamFree(obj->m_Stream);

				obj->m_Stream = 0;
			}

			i = m_UsedSoundList.erase(i);
		}
		else
			i++;
	}
}
//----------------------------------------------------------------------------------
CGLTexture *COrion::ExecuteGump(const ushort &id)
{
	CIndexObject &io = m_GumpDataIndex[id];

	if (io.Texture == 0)
	{
		if (!io.Address)
			return NULL;

		io.Texture = g_MulReader->ReadGump(io);

		if (io.Texture != 0)
			m_UsedGumpList.push_back(&m_GumpDataIndex[id]);
	}

	io.LastAccessTime = g_Ticks;

	return io.Texture;
}
//----------------------------------------------------------------------------------
CGLTexture *COrion::ExecuteLandArt(const ushort &id)
{
	CIndexObject &io = m_LandDataIndex[id];

	if (io.Texture == 0)
	{
		if (!io.Address || id == 0x02) //nodraw tiles banned
			return NULL;

		io.Texture = g_MulReader->ReadArt(id, io);

		if (io.Texture != 0)
			m_UsedLandList.push_back(&m_LandDataIndex[id]);
	}

	io.LastAccessTime = g_Ticks;

	return io.Texture;
}
//----------------------------------------------------------------------------------
CGLTexture *COrion::ExecuteStaticArtAnimated(const ushort &id)
{
	return ExecuteStaticArt(id + m_StaticDataIndex[id].Offset);
}
//----------------------------------------------------------------------------------
CGLTexture *COrion::ExecuteStaticArt(const ushort &id)
{
	CIndexObject &io = m_StaticDataIndex[id];

	if (io.Texture == 0)
	{
		if (!io.Address || id == 0x01) //nodraw tiles banned
			return NULL;

		io.Texture = g_MulReader->ReadArt(id + 0x4000, io);

		if (io.Texture != 0)
		{
			io.Width = ((io.Texture->Width / 2) + 1);
			io.Height = io.Texture->Height - 20;

			m_UsedStaticList.push_back(&m_StaticDataIndex[id]);
		}
	}

	io.LastAccessTime = g_Ticks;

	return io.Texture;
}
//----------------------------------------------------------------------------------
CGLTexture *COrion::ExecuteTexture(ushort id)
{
	id = m_LandData[id / 32].Tiles[id % 32].TexID;

	if (!id)
		return NULL;

	CIndexObject &io = m_TextureDataIndex[id];

	if (io.Texture == 0)
	{
		if (!io.Address)
			return NULL;

		io.Texture = g_MulReader->ReadTexture(io);

		if (io.Texture != 0)
			m_UsedTextureList.push_back(&m_TextureDataIndex[id]);
	}

	io.LastAccessTime = g_Ticks;

	return io.Texture;
}
//----------------------------------------------------------------------------------
CGLTexture *COrion::ExecuteLight(uchar &id)
{
	if (id >= MAX_LIGHTS_DATA_INDEX_COUNT)
		id = 0;

	CIndexObject &io = m_LightDataIndex[id];

	if (io.Texture == 0)
	{
		if (!io.Address)
			return NULL;

		io.Texture = g_MulReader->ReadLight(io);

		if (io.Texture != 0)
			m_UsedLightList.push_back(&m_LightDataIndex[id]);
	}

	io.LastAccessTime = g_Ticks;

	return io.Texture;
}
//----------------------------------------------------------------------------------
bool COrion::ExecuteGumpPart(const ushort &id, const int &count)
{
	bool result = true;

	IFOR(i, 0, count)
	{
		if (ExecuteGump(id + i) == NULL)
			result = false;
	}

	return result;
}
//----------------------------------------------------------------------------------
void COrion::DrawGump(const ushort &id, const ushort &color, const int &x, const int &y, const bool &partialHue)
{
	CGLTexture *th = ExecuteGump(id);

	if (th != NULL)
	{
		int drawMode = (!g_GrayedPixels && color);

		if (drawMode)
		{
			if (partialHue)
				drawMode = 2;

			g_ColorManager.SendColorsToShader(color);
		}

		glUniform1iARB(g_ShaderDrawMode, drawMode);

		th->Draw(x, y);
	}
}
//----------------------------------------------------------------------------------
void COrion::DrawGump(const ushort &id, const ushort &color, const int &x, const int &y, const int &width, const int &height, const bool &partialHue)
{
	CGLTexture *th = ExecuteGump(id);

	if (th != NULL)
	{
		int drawMode = (!g_GrayedPixels && color);

		if (drawMode)
		{
			if (partialHue)
				drawMode = 2;

			g_ColorManager.SendColorsToShader(color);
		}

		glUniform1iARB(g_ShaderDrawMode, drawMode);

		th->Draw(x, y, width, height);
	}
}
//----------------------------------------------------------------------------------
void COrion::DrawResizepicGump(const ushort &id, const int &x, const int &y, const int &width, const int &height)
{
	CGLTexture *th[9] = { NULL };

	IFOR(i, 0, 9)
	{
		CGLTexture *pth = ExecuteGump(id + i);

		if (pth == NULL)
			return;

		if (i == 4)
			th[8] = pth;
		else if (i > 4)
			th[i - 1] = pth;
		else
			th[i] = pth;
	}

	g_GL.DrawResizepic(th, x, y, width, height);
}
//----------------------------------------------------------------------------------
void COrion::DrawLandTexture(CLandObject *land, ushort color, const int &x, const int &y)
{
	ushort id = land->Graphic;

	if (id == 2)
		DrawLandArt(id, color, x, y, land->Rect.left / 4);
	else
	{
		CGLTexture *th = ExecuteTexture(id);

		if (th == NULL)
			DrawLandArt(id, color, x, y, land->Rect.left / 4);
		else
		{
			if (g_OutOfRangeColor)
				color = g_OutOfRangeColor;

			int drawMode = 6;

			if (!g_GrayedPixels && color)
			{
				drawMode = 7;
				g_ColorManager.SendColorsToShader(color);
			}

			glUniform1iARB(g_ShaderDrawMode, drawMode);

			g_GL.DrawLandTexture(th->Texture, x, y, land->Rect, land->m_Normals);
		}
	}
}
//----------------------------------------------------------------------------------
void COrion::DrawLandArt(const ushort &id, ushort color, const int &x, const int &y, const int &z)
{
	CGLTexture *th = ExecuteLandArt(id);

	if (th != NULL)
	{
		if (g_OutOfRangeColor)
			color = g_OutOfRangeColor;

		int drawMode = (!g_GrayedPixels && color);

		if (drawMode)
			g_ColorManager.SendColorsToShader(color);

		glUniform1iARB(g_ShaderDrawMode, drawMode);

		th->Draw(x - 23, y - (23 + (z * 4)));
	}
}
//----------------------------------------------------------------------------------
void COrion::DrawStaticArt(const ushort &id, ushort color, const int &x, const int &y, const int &z, const bool &selection)
{
	CGLTexture *th = ExecuteStaticArt(id);

	if (th != NULL && id > 1)
	{
		if (g_OutOfRangeColor)
			color = g_OutOfRangeColor;

		int drawMode = (!g_GrayedPixels && color);

		if (drawMode)
		{
			bool partialHue = (!selection && IsPartialHue(GetStaticFlags(id)));

			if (partialHue)
				drawMode = 2;

			g_ColorManager.SendColorsToShader(color);
		}

		glUniform1iARB(g_ShaderDrawMode, drawMode);

		th->Draw(x - m_StaticDataIndex[id].Width, y - (m_StaticDataIndex[id].Height + (z * 4)));
	}
}
//----------------------------------------------------------------------------------
void COrion::DrawStaticArtAnimated(const ushort &id, const ushort &color, const int &x, const int &y, const int &z, const bool &selection)
{
	DrawStaticArt(id + m_StaticDataIndex[id].Offset, color, x, y, z, selection);
}
//----------------------------------------------------------------------------------
void COrion::DrawStaticArtRotated(const ushort &id, ushort color, const int &x, const int &y, const int &z, const float &angle)
{
	CGLTexture *th = ExecuteStaticArt(id);

	if (th != NULL && id > 1)
	{
		if (g_OutOfRangeColor)
			color = g_OutOfRangeColor;

		int drawMode = (!g_GrayedPixels && color);

		if (drawMode)
			g_ColorManager.SendColorsToShader(color);

		glUniform1iARB(g_ShaderDrawMode, drawMode);

		th->DrawRotated(x, y - (z * 4), angle);
	}
}
//----------------------------------------------------------------------------------
void COrion::DrawStaticArtAnimatedRotated(const ushort &id, const ushort &color, const int &x, const int &y, const int &z, const float &angle)
{
	DrawStaticArtRotated(id + m_StaticDataIndex[id].Offset, color, x, y, z, angle);
}
//----------------------------------------------------------------------------------
void COrion::DrawStaticArtTransparent(const ushort &id, ushort color, int x, int y, const int &z, const bool &selection)
{
	CGLTexture *th = ExecuteStaticArt(id);

	if (th != NULL && id > 1)
	{
		if (g_OutOfRangeColor)
			color = g_OutOfRangeColor;

		int drawMode = (!g_GrayedPixels && color);

		if (drawMode)
		{
			bool partialHue = (!selection && IsPartialHue(GetStaticFlags(id)));

			if (partialHue)
				drawMode = 2;

			g_ColorManager.SendColorsToShader(color);
		}

		glUniform1iARB(g_ShaderDrawMode, drawMode);

		th->DrawTransparent(x - m_StaticDataIndex[id].Width, y - (m_StaticDataIndex[id].Height + (z * 4)));
	}
}
//----------------------------------------------------------------------------------
void COrion::DrawStaticArtAnimatedTransparent(const ushort &id, const ushort &color, const int &x, const int &y, const int &z, const bool &selection)
{
	DrawStaticArtTransparent(id + m_StaticDataIndex[id].Offset, color, x, y, z, selection);
}
//----------------------------------------------------------------------------------
void COrion::DrawStaticArtInContainer(const ushort &id, ushort color, int x, int y, const bool &selection, const bool &onMouse)
{
	CGLTexture *th = ExecuteStaticArt(id);

	if (th != NULL)
	{
		if (onMouse)
		{
			x -= th->Width / 2;
			y -= th->Height / 2;
		}

		int drawMode = (!g_GrayedPixels && color);

		if (drawMode)
		{
			bool partialHue = (!selection && IsPartialHue(GetStaticFlags(id)));

			if (color >= 0x4000)
				color = 0x1;
			else if (partialHue)
				drawMode = 2;

			g_ColorManager.SendColorsToShader(color);
		}

		glUniform1iARB(g_ShaderDrawMode, drawMode);

		th->Draw(x, y);
	}
}
//----------------------------------------------------------------------------------
void COrion::DrawLight(LIGHT_DATA &light)
{
	CGLTexture *th = ExecuteLight(light.ID);

	if (th != NULL)
	{
		int drawMode = 0;

		if (light.Color)
		{
			drawMode = 1;

			g_ColorManager.SendColorsToShader(light.Color);
		}

		glUniform1iARB(g_ShaderDrawMode, drawMode);

		th->Draw(light.DrawX - g_RenderBounds.GameWindowPosX - (th->Width / 2), light.DrawY - g_RenderBounds.GameWindowPosY - (th->Height / 2));
	}
}
//----------------------------------------------------------------------------------
bool COrion::PolygonePixelsInXY(int x, int y, const int &width, const int &height)
{
	x = g_MouseManager.Position.X - x;
	y = g_MouseManager.Position.Y - y;

	return !(x < 0 || y < 0 || x >= width || y >= height);
}
//----------------------------------------------------------------------------------
bool COrion::GumpPixelsInXY(const ushort &id, int x, int y, const bool &noSubMouse)
{
	CIndexObject &io = m_GumpDataIndex[id];

	bool result = false;

	CGLTexture *th = io.Texture;

	if (th != NULL)
	{
		if (!noSubMouse)
		{
			x = g_MouseManager.Position.X - x;
			y = g_MouseManager.Position.Y - y;
		}

#if UO_ENABLE_TEXTURE_DATA_SAVING == 1
		if (x >= 0 && y >= 0 && x < th->Width && y < th->Height)
			result = th->PixelsData[(y * th->Width) + x] != 0;
#else
		result = g_MulReader->GumpPixelsInXY(io, x, y);
#endif
	}

	return result;
}
//----------------------------------------------------------------------------------
bool COrion::GumpPixelsInXY(const ushort &id, int x, int y, int width, int height, const bool &noSubMouse)
{
	if (!noSubMouse)
	{
		x = g_MouseManager.Position.X - x;
		y = g_MouseManager.Position.Y - y;
	}

	if (x < 0 || y < 0 || (width > 0 && x >= width) || (height > 0 && y >= height))
		return false;

	CIndexObject &io = m_GumpDataIndex[id];

	CGLTexture *th = io.Texture;

	if (th == NULL)
		return false;

	if (width == 0)
		width = th->Width;

	if (height == 0)
		height = th->Height;

	while (x > th->Width && width > th->Width)
	{
		x -= th->Width;
		width -= th->Width;
	}

	while (y > th->Height && height > th->Height)
	{
		y -= th->Height;
		height -= th->Height;
	}

	if (x > width || y > height)
		return false;

	bool result = false;

#if UO_ENABLE_TEXTURE_DATA_SAVING == 1
	if (x >= 0 && y >= 0 && x < th->Width && y < th->Height)
		result = th->PixelsData[(y * th->Width) + x] != 0;
#else
	result = g_MulReader->GumpPixelsInXY(io, x, y);
#endif

	return result;
}
//----------------------------------------------------------------------------------
bool COrion::ResizepicPixelsInXY(const ushort &id, int x, int y, const int &width, const int &height)
{
	x = g_MouseManager.Position.X - x;
	y = g_MouseManager.Position.Y - y;

	if (x < 0 || y < 0 || x >= width || y >= height)
		return false;

	CGLTexture *th[9] = { NULL };

	IFOR(i, 0, 9)
	{
		CGLTexture *pth = m_GumpDataIndex[id + i].Texture;

		if (pth == NULL)
			return false;

		if (i == 4)
			th[8] = pth;
		else if (i > 4)
			th[i - 1] = pth;
		else
			th[i] = pth;
	}

	IFOR(i, 0, 9)
	{
		switch (i)
		{
		case 0:
		{
			if (GumpPixelsInXY(id, x, y, true))
				return true;
			break;
		}
		case 1:
		{
			int DW = width - th[0]->Width - th[2]->Width;
			if (DW < 1)
				break;

			if (GumpPixelsInXY(id + 1, x - th[0]->Width, y, DW, 0, true))
				return true;

			break;
		}
		case 2:
		{
			if (GumpPixelsInXY(id + 2, x - width + th[i]->Width, y, true))
				return true;

			break;
		}
		case 3:
		{
			int DH = height - th[0]->Height - th[5]->Height;
			if (DH < 1)
				break;

			if (GumpPixelsInXY(id + 3, x, y - th[0]->Height, 0, DH, true))
				return true;

			break;
		}
		case 4:
		{
			int DH = height - th[2]->Height - th[7]->Height;
			if (DH < 1)
				break;

			if (GumpPixelsInXY(id + 5, x - width + th[i]->Width, y - th[2]->Height, 0, DH, true))
				return true;

			break;
		}
		case 5:
		{
			if (GumpPixelsInXY(id + 6, x, y - height + th[i]->Height, true))
				return true;

			break;
		}
		case 6:
		{
			int DW = width - th[5]->Width - th[7]->Width;
			if (DW < 1)
				break;

			if (GumpPixelsInXY(id + 7, x - th[5]->Width, y - height + th[i]->Height, DW, 0, true))
				return true;

			break;
		}
		case 7:
		{
			if (GumpPixelsInXY(id + 8, x - width + th[i]->Width, y - height + th[i]->Height, true))
				return true;

			break;
		}
		case 8:
		{
			int DW = width - th[0]->Width - th[2]->Width;

			if (DW < 1)
				break;

			int DH = height - th[2]->Height - th[7]->Height;

			if (DH < 1)
				break;

			if (GumpPixelsInXY(id + 4, x - th[0]->Width, y - th[0]->Height, DW, DH, true))
				return true;

			break;
		}
		default:
			break;
		}
	}

	return false;
}
//----------------------------------------------------------------------------------
bool COrion::StaticPixelsInXY(const ushort &id, int x, int y, const int &z)
{
	CIndexObject &io = m_StaticDataIndex[id];

	bool result = false;

	CGLTexture *th = io.Texture;

	if (th != NULL)
	{
		x = (g_MouseManager.Position.X - x) + io.Width;
		y = (g_MouseManager.Position.Y - y) + io.Height + (z * 4);

#if UO_ENABLE_TEXTURE_DATA_SAVING == 1
		if (x >= 0 && y >= 0 && x < th->Width && y < th->Height)
			result = th->PixelsData[(y * th->Width) + x] != 0;
#else
		result = g_MulReader->ArtPixelsInXY(false, io, x, y);
#endif
	}

	return result;
}
//----------------------------------------------------------------------------------
bool COrion::StaticPixelsInXYAnimated(const ushort &id, const int &x, const int &y, const int &z)
{
	return StaticPixelsInXY(id + m_StaticDataIndex[id].Offset, x, y, z);
}
//----------------------------------------------------------------------------------
bool COrion::CircleTransPixelsInXY()
{
	int x = (g_MouseManager.Position.X - g_CircleOfTransparency.X);
	int y = (g_MouseManager.Position.Y - g_CircleOfTransparency.Y);

	bool result = false;

	if (x >= 0 && y >= 0 && x < g_CircleOfTransparency.Width && y < g_CircleOfTransparency.Height)
	{
		int pos = (y * g_CircleOfTransparency.Width) + x;
		result = (g_CircleOfTransparency.PixelsData[pos] != 0);
	}

	return result;
}
//----------------------------------------------------------------------------------
bool COrion::StaticPixelsInXYInContainer(const ushort &id, int x, int y)
{
	CIndexObject &io = m_StaticDataIndex[id];

	bool result = false;

	CGLTexture *th = io.Texture;

	if (th != NULL)
	{
		x = g_MouseManager.Position.X - x;
		y = g_MouseManager.Position.Y - y;

#if UO_ENABLE_TEXTURE_DATA_SAVING == 1
		if (x >= 0 && y >= 0 && x < th->Width && y < th->Height)
			result = th->PixelsData[(y * th->Width) + x] != 0;
#else
		result = g_MulReader->ArtPixelsInXY(false, io, x, y);
#endif
	}

	return result;
}
//----------------------------------------------------------------------------------
bool COrion::LandPixelsInXY(const ushort &id, int x, int  y, const int &z)
{
	CIndexObject &io = m_LandDataIndex[id];

	bool result = false;

	CGLTexture *th = io.Texture;

	if (th != NULL)
	{
		x = (g_MouseManager.Position.X - x) + 23;
		y = (g_MouseManager.Position.Y - y) + 23 + (z * 4);

#if UO_ENABLE_TEXTURE_DATA_SAVING == 1
		if (x >= 0 && y >= 0 && x < th->Width && y < th->Height)
			result = th->PixelsData[(y * th->Width) + x] != 0;
#else
		result = g_MulReader->ArtPixelsInXY(true, io, x, y);
#endif
	}

	return result;
}
//----------------------------------------------------------------------------------
bool COrion::LandTexturePixelsInXY(int x, int  y, RECT &r)
{
	y -= 23;
	int testX = g_MouseManager.Position.X - x;
	int testY = g_MouseManager.Position.Y;

	int y0 = -r.left;
	int y1 = 22 - r.top;
	int y2 = 44 - r.right;
	int y3 = 22 - r.bottom;

	bool result =
		(
			(testY >= testX * (y1 - y0) / -22 + y + y0) &&
			(testY >= testX * (y3 - y0) / 22 + y + y0) &&
			(testY <= testX * (y3 - y2) / 22 + y + y2) &&
			(testY <= testX * (y1 - y2) / -22 + y + y2)
		);

	return result;
}
//----------------------------------------------------------------------------------
void COrion::CreateTextMessageF(uchar font, ushort color, const char *format, ...)
{
	va_list arg;
	va_start(arg, format);

	char buf[128] = { 0 };
	vsprintf_s(buf, format, arg);

	CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, font, color, buf);

	va_end(arg);
}
//----------------------------------------------------------------------------------
void COrion::CreateUnicodeTextMessageF(uchar font, ushort color, const char *format, ...)
{
	va_list arg;
	va_start(arg, format);

	char buf[128] = { 0 };
	vsprintf_s(buf, format, arg);

	CreateUnicodeTextMessage(TT_SYSTEM, 0xFFFFFFFF, font, color, ToWString(buf));

	va_end(arg);
}
//----------------------------------------------------------------------------------
void COrion::CreateTextMessage(TEXT_TYPE type, uint serial, uchar font, ushort color, string text)
{
	CTextData *td = new CTextData();
	td->Unicode = false;
	td->Font = font;
	td->Serial = serial;
	td->Color = color;
	td->Timer = g_Ticks;
	td->Type = type;
	td->SetText(text);
	
	switch (type)
	{
		case TT_SYSTEM:
		{
			td->GenerateTexture(300);
			AddSystemMessage(td);

			break;
		}
		case TT_OBJECT:
		{
			CGameObject *obj = g_World->FindWorldObject(serial);

			if (obj != NULL)
			{
				int width = g_FontManager.GetWidthA(font, text.c_str(), text.length());

				g_FontManager.SavePixels = true;

				td->Color = 0;

				if (width > TEXT_MESSAGE_MAX_WIDTH)
				{
					width = g_FontManager.GetWidthExA((BYTE)font, text.c_str(), text.length(), TEXT_MESSAGE_MAX_WIDTH, TS_LEFT, 0);
					td->GenerateTexture(width, 0, TS_LEFT);
					//td->GenerateTexture(TEXT_MESSAGE_MAX_WIDTH, 0, TS_LEFT);
				}
				else
					td->GenerateTexture(0, 0, TS_CENTER);

				td->Color = color;

				g_FontManager.SavePixels = false;

				obj->AddText(td);

				uint container = obj->Container;

				if (container == 0xFFFFFFFF)
					g_WorldTextRenderer.AddText(td);
				else if (!obj->NPC)
				{
					td->X = g_ClickObject.X;
					td->Y = g_ClickObject.Y;

					CGump *gump = g_GumpManager.GetGump(container, 0, GT_CONTAINER);
					
					if (gump == NULL)
					{
						CGameObject *topobj = obj->GetTopObject();

						if (((CGameItem*)obj)->Layer != OL_NONE)
							gump = g_GumpManager.GetGump(topobj->Serial, 0, GT_PAPERDOLL);

						if (gump == NULL)
						{
							gump = g_GumpManager.GetGump(topobj->Serial, 0, GT_TRADE);

							if (gump == NULL)
							{
								topobj = (CGameObject*)topobj->m_Items;

								while (topobj != NULL && topobj->Graphic != 0x1E5E)
									topobj = (CGameObject*)topobj->m_Next;

								if (topobj != NULL)
									gump = g_GumpManager.GetGump(0, topobj->Serial, GT_TRADE);
							}
						}
					}

					if (gump != NULL)
					{
						CTextRenderer *tr = gump->GetTextRenderer();

						if (tr != NULL)
							tr->AddText(td);
					}
				}
			}
			else
			{
				td->GenerateTexture(300);
				AddSystemMessage(td);
			}

			break;
		}
		case TT_CLIENT:
		{
			int width = g_FontManager.GetWidthA((BYTE)font, text.c_str(), text.length());
			
			g_FontManager.SavePixels = true;

			if (width > TEXT_MESSAGE_MAX_WIDTH)
			{
				width = g_FontManager.GetWidthExA((BYTE)font, text.c_str(), text.length(), TEXT_MESSAGE_MAX_WIDTH, TS_LEFT, 0);
				td->GenerateTexture(width, 0, TS_LEFT);
				//td->GenerateTexture(TEXT_MESSAGE_MAX_WIDTH, 0, TS_CENTER);
			}
			else
				td->GenerateTexture(0, 0, TS_CENTER);

			g_FontManager.SavePixels = false;

			((CRenderWorldObject*)serial)->AddText(td);
			g_WorldTextRenderer.AddText(td);

			break;
		}
	}
}
//----------------------------------------------------------------------------------
void COrion::CreateUnicodeTextMessage(TEXT_TYPE type, uint serial, uchar font, ushort color, wstring text)
{
	CTextData *td = new CTextData();
	td->Unicode = true;
	td->Font = font;
	td->Serial = serial;
	td->Color = color;
	td->Timer = g_Ticks;
	td->Type = type;
	td->SetUnicodeText(text);
	
	switch (type)
	{
		case TT_SYSTEM:
		{
			td->GenerateTexture(300, UOFONT_BLACK_BORDER);
			AddSystemMessage(td);

			break;
		}
		case TT_OBJECT:
		{
			CGameObject *obj = g_World->FindWorldObject(serial);

			if (obj != NULL)
			{
				int width = g_FontManager.GetWidthW((BYTE)font, text.c_str(), text.length());

				g_FontManager.SavePixels = true;

				if (width > TEXT_MESSAGE_MAX_WIDTH)
				{
					width = g_FontManager.GetWidthExW((BYTE)font, text.c_str(), text.length(), TEXT_MESSAGE_MAX_WIDTH, TS_LEFT, UOFONT_BLACK_BORDER);
					td->GenerateTexture(width, UOFONT_BLACK_BORDER, TS_LEFT);
					//td->GenerateTexture(TEXT_MESSAGE_MAX_WIDTH, UOFONT_BLACK_BORDER, TS_LEFT);
				}
				else
					td->GenerateTexture(0, UOFONT_BLACK_BORDER, TS_CENTER);
				
				g_FontManager.SavePixels = false;

				obj->AddText(td);

				uint container = obj->Container;

				if (container == 0xFFFFFFFF)
					g_WorldTextRenderer.AddText(td);
				else if (!obj->NPC)
				{
					td->X = g_ClickObject.X;
					td->Y = g_ClickObject.Y;

					CGump *gump = g_GumpManager.GetGump(container, 0, GT_CONTAINER);

					if (gump == NULL)
					{
						CGameObject *topobj = obj->GetTopObject();

						if (((CGameItem*)obj)->Layer != OL_NONE)
							gump = g_GumpManager.GetGump(topobj->Serial, 0, GT_PAPERDOLL);

						if (gump == NULL)
						{
							gump = g_GumpManager.GetGump(topobj->Serial, 0, GT_TRADE);

							if (gump == NULL)
							{
								topobj = (CGameObject*)topobj->m_Items;

								while (topobj != NULL && topobj->Graphic != 0x1E5E)
									topobj = (CGameObject*)topobj->m_Next;

								if (topobj != NULL)
									gump = g_GumpManager.GetGump(0, topobj->Serial, GT_TRADE);
							}
						}
					}

					if (gump != NULL)
					{
						CTextRenderer *tr = gump->GetTextRenderer();

						if (tr != NULL)
							tr->AddText(td);
					}
				}
			}
			else
			{
				td->GenerateTexture(300, UOFONT_BLACK_BORDER);
				AddSystemMessage(td);
			}

			break;
		}
		default:
		{
			delete td;
			break;
		}
	}
}
//----------------------------------------------------------------------------------
void COrion::AddSystemMessage(CTextData *msg)
{
	g_SystemChat.Add(msg);
	AddJournalMessage(msg, "");
}
//----------------------------------------------------------------------------------
void COrion::AddJournalMessage(CTextData *msg, string name)
{
	CTextData *jmsg = new CTextData(msg);

	if (!jmsg->Unicode)
	{
		jmsg->SetText(name + jmsg->Text);
		jmsg->Font = 9;
	}
	else
	{
		if (msg->Type == TT_SYSTEM)
			jmsg->Color = 0;

		jmsg->SetUnicodeText(ToWString(name) + jmsg->UnicodeText);
		jmsg->Font = 0;
	}

	/*if (msg->Type == TT_OBJECT)
		jmsg->GenerateTexture(214, UOFONT_INDENTION | UOFONT_BLACK_BORDER);
	else
		jmsg->GenerateTexture(214, UOFONT_INDENTION);*/

	g_Journal.Add(jmsg);
}
//----------------------------------------------------------------------------------
void COrion::ChangeMap(uchar newmap)
{
	if (newmap < 0 || newmap > 5)
		newmap = 0;

	if (g_CurrentMap != newmap)
	{
		g_CurrentMap = newmap;

		if (g_World != NULL && g_Player != NULL)
		{
			g_Player->MapIndex = g_CurrentMap;
			g_Player->RemoveRender();

			CGameObject *obj = (CGameObject*)g_World->m_Items;

			while (obj != NULL)
			{
				CGameObject *next = (CGameObject*)obj->m_Next;

				if (obj->MapIndex != g_CurrentMap)
					g_World->RemoveObject(obj);

				obj = next;
			}

			g_MapManager->Clear();
			g_MapManager->Init();

			g_MapManager->AddRender(g_Player);
		}
	}
}
//----------------------------------------------------------------------------------
void COrion::PickupItem(CGameItem *obj, int count, bool isGameFigure)
{
	if (g_ObjectInHand == NULL)
	{
		g_ObjectInHand = new CObjectOnCursor(obj);

		if (!count)
			count = obj->Count;

		g_ObjectInHand->Separated = count != obj->Count;
		g_ObjectInHand->IsGameFigure = isGameFigure;
		g_ObjectInHand->DragCount = count;

		if (obj->Container != 0xFFFFFFFF)
			g_GumpManager.UpdateGump(obj->Container, 0, GT_CONTAINER);

		CPacketPickupRequest(obj->Serial, count).Send();
	}
}
//----------------------------------------------------------------------------------
void COrion::DropItem(uint container, ushort x, ushort y, char z)
{
	if (g_ObjectInHand != NULL)
	{
		if (g_ObjectInHand->Dropped)
		{
			delete g_ObjectInHand;
			g_ObjectInHand = NULL;
		}
		else
		{
			g_ObjectInHand->Dropped = true;

			if (g_ConnectionManager.ClientVersion >= CV_6017)
				CPacketDropRequestNew(g_ObjectInHand->Serial, x, y, z, 0, container).Send();
			else
				CPacketDropRequestOld(g_ObjectInHand->Serial, x, y, z, container).Send();
		}
	}
}
//----------------------------------------------------------------------------------
void COrion::EquipItem(uint container)
{
	if (g_ObjectInHand != NULL)
	{
		if (g_ObjectInHand->Dropped)
		{
			delete g_ObjectInHand;
			g_ObjectInHand = NULL;
		}
		else if (g_ObjectInHand->IsWearable())
		{
			g_ObjectInHand->Dropped = true;

			ushort graphic = g_ObjectInHand->Graphic;

			if (graphic < 0x4000)
			{
				STATIC_TILES &st = m_StaticData[graphic / 32].Tiles[graphic % 32];

				if (!container)
					container = g_PlayerSerial;

				CPacketEquipRequest(g_ObjectInHand->Serial, st.Quality, container).Send();
			}
		}
	}
}
//----------------------------------------------------------------------------------
void COrion::ChangeWarmode(uchar status)
{
	uchar newstatus = (uchar)(!g_Player->Warmode);

	if (status != 0xFF)
	{
		if (g_Player->Warmode == (bool)status)
			return;

		newstatus = status;
	}

	//38, 39 и 40 это индексы боевой музыки.
	if (newstatus == 1 && g_ConfigManager.Music)
		PlayMusic(rand() % (40 - 38 + 1) + 38, true);
	else if (newstatus == 0)
		g_SoundManager.StopWarMusic();

	CPacketChangeWarmode(newstatus).Send();
}
//----------------------------------------------------------------------------------
void COrion::HelpRequest()
{
	CPacketHelpRequest().Send();
}
//----------------------------------------------------------------------------------
void COrion::StatusReq(uint serial)
{
	CPacketStatusRequest(serial).Send();
}
//----------------------------------------------------------------------------------
void COrion::SkillsReq(uint serial)
{
	CPacketSkillsRequest(serial).Send();
}
//----------------------------------------------------------------------------------
void COrion::SkillStatusChange(uchar skill, uchar state)
{
	CPacketSkillsStatusChangeRequest(skill, state).Send();
}
//----------------------------------------------------------------------------------
void COrion::Click(uint serial)
{
	CPacketClickRequest(serial).Send();

	CGameObject *obj = g_World->FindWorldObject(serial);
	if (obj != NULL)
		obj->Clicked = true;
}
//----------------------------------------------------------------------------------
void COrion::DoubleClick(uint serial)
{
	if (serial >= 0x40000000)
		g_LastUseObject = serial;

	CPacketDoubleClickRequest(serial).Send();
}
//----------------------------------------------------------------------------------
void COrion::PaperdollReq(uint serial)
{
	//g_LastUseObject = serial;

	CPacketDoubleClickRequest(serial | 0x80000000).Send();
}
//----------------------------------------------------------------------------------
void COrion::Attack(uint serial)
{
	if (g_ConfigManager.CriminalActionsQuery && g_World != NULL)
	{
		CGameCharacter *target = g_World->FindWorldCharacter(serial);

		if (target != NULL && (NOTORIETY_TYPE)g_Player->Notoriety == NT_INNOCENT && (NOTORIETY_TYPE)target->Notoriety == NT_INNOCENT)
		{
			int x = g_ConfigManager.GameWindowX + (g_ConfigManager.GameWindowWidth / 2) - 40;
			int y = g_ConfigManager.GameWindowY + (g_ConfigManager.GameWindowHeight / 2) - 20;

			CGumpQuestion *newgump = new CGumpQuestion(g_PlayerSerial, x, y, CGumpQuestion::ID_GQ_STATE_ATTACK_REQUEST);
			newgump->SetID(serial);

			g_GumpManager.AddGump(newgump);

			InitScreen(GS_GAME_BLOCKED);
			g_GameBlockedScreen.Code = 3;

			return;
		}
	}

	AttackReq(serial);
}
//----------------------------------------------------------------------------------
void COrion::AttackReq(uint serial)
{
	g_LastAttackObject = serial;

	CPacketStatusRequest(serial).Send();

	CPacketAttackRequest(serial).Send();
}
//----------------------------------------------------------------------------------
void COrion::SendASCIIText(const char *str, SPEECH_TYPE type)
{
	CPacketASCIISpeechRequest(str, type, 3, g_ConfigManager.SpeechColor).Send();
}
//----------------------------------------------------------------------------------
void COrion::CastSpell(int index)
{
	if (index >= 0)
	{
		g_LastSpellIndex = index;

		CPacketCastSpell(index).Send();
	}
}
//----------------------------------------------------------------------------------
void COrion::CastSpellFromBook(int index, uint serial)
{
	if (index >= 0)
	{
		g_LastSpellIndex = index;

		CPacketCastSpellFromBook(index, serial).Send();
	}
}
//----------------------------------------------------------------------------------
void COrion::UseSkill(int index)
{
	if (index >= 0)
	{
		g_LastSkillIndex = index;

		CPacketUseSkill(index).Send();
	}
}
//----------------------------------------------------------------------------------
void COrion::OpenDoor()
{
	CPacketOpenDoor().Send();
}
//----------------------------------------------------------------------------------
void COrion::EmoteAction(const char *action)
{
	CPacketEmoteAction(action).Send();
}
//----------------------------------------------------------------------------------
void COrion::AllNames()
{
	CGameObject *obj = g_World->m_Items;

	while (obj != NULL)
	{
		if ((obj->NPC && !obj->IsPlayer()) || obj->IsCorpse())
			CPacketClickRequest(obj->Serial).Send();

		obj = (CGameObject*)obj->m_Next;
	}
}
//----------------------------------------------------------------------------------
void COrion::RemoveRangedObjects()
{
	int objectsRange = g_ConfigManager.UpdateRange + 1;

	if (g_World != NULL)
	{
		CGameObject *go = g_World->m_Items;

		while (go != NULL)
		{
			CGameObject *next = (CGameObject*)go->m_Next;

			if (go->Container == 0xFFFFFFFF && !go->IsPlayer())
			{
				if (go->Graphic >= 0x4000)
				{
					if (GetMultiDistance(g_RemoveRangeXY, go) > objectsRange)
						//if (GetDistance(g_RemoveRangeXY, go) > 31)
						g_World->RemoveObject(go);
				}
				else if (GetDistance(g_RemoveRangeXY, go) > objectsRange)
				{
					if (g_Party.Contains(go->Serial))
						go->RemoveRender();
					else
						g_World->RemoveObject(go);
				}
			}

			go = next;
		}
	}

	g_GumpManager.RemoveRangedGumps();

	g_EffectManager.RemoveRangedEffects();
}
//----------------------------------------------------------------------------------
void COrion::LogOut()
{
	LOG("COrion::LogOut->Start\n");
	SaveLocalConfig();

	if (g_SendLogoutNotification)
		CPacketLogoutNotification().Send();

	Disconnect();
	LOG("\tDisconnected?\n");

	RELEASE_POINTER(g_Walker);

	LOG("\tWalker deleted?\n");

	RELEASE_POINTER(g_ObjectInHand);
	LOG("\tObjectInHand removed?\n");

	RELEASE_POINTER(g_World)
	LOG("\tWorld removed?\n");

	g_PopupMenu = NULL;

	g_GumpManager.Clear();
	LOG("\tGump Manager cleared?\n");

	g_EffectManager.Clear();
	LOG("\tEffect List cleared?\n");

	g_GameConsole.Clear();
	LOG("\tGame console cleared?\n");

	g_EntryPointer = NULL;
	g_GrayMenuCount = 0;

	g_Target.Reset();
	LOG("\tTarget reseted?\n");

	g_SystemChat.Clear();
	LOG("\tSystem chat cleared?\n");

	g_Journal.Clear();
	LOG("\tJournal cleared?\n");

	if (g_MapManager != NULL)
		g_MapManager->Clear();

	g_CurrentMap = 0;

	LOG("COrion::LogOut->End\n");
	InitScreen(GS_MAIN);
}
//----------------------------------------------------------------------------------
void COrion::ConsolePromptSend()
{
	int len = g_GameConsole.Length();
	bool cancel = (len < 1);

	if (g_ConsolePrompt == PT_ASCII)
		CPacketASCIIPromptResponse(g_GameConsole.c_str(), len, cancel).Send();
	else if (g_ConsolePrompt == PT_UNICODE)
		CPacketUnicodePromptResponse(g_GameConsole.Data(), len, "ENU", cancel).Send();

	g_ConsolePrompt = PT_NONE;
}
//----------------------------------------------------------------------------------
void COrion::ConsolePromptCancel()
{
	if (g_ConsolePrompt == PT_ASCII)
		CPacketASCIIPromptResponse("", 0, true).Send();
	else if (g_ConsolePrompt == PT_UNICODE)
		CPacketUnicodePromptResponse(L"", 0, "ENU", true).Send();

	g_ConsolePrompt = PT_NONE;
}
//----------------------------------------------------------------------------------
uint COrion::GetLandFlags(const ushort &id)
{
	ushort divID = id / 32;

	if (divID < m_LandDataCount)
		return m_LandData[divID].Tiles[id % 32].Flags;

	return 0;
}
//----------------------------------------------------------------------------------
uint COrion::GetStaticFlags(const ushort &id)
{
	ushort divID = id / 32;

	if (divID < m_StaticDataCount)
		return m_StaticData[divID].Tiles[id % 32].Flags;

	return 0;
}
//----------------------------------------------------------------------------------
WISP_GEOMETRY::CSize COrion::GetArtDimension(const ushort &id)
{
	WISP_GEOMETRY::CSize size;

	if (id >= 0x4000) //run
	{
		CGLTexture *th = ExecuteStaticArt(id - 0x4000); // m_StaticDataIndex[id - 0x4000].Texture;

		if (th != NULL)
		{
			size.Width = th->Width;
			size.Height = th->Height;
		}
	}
	else //raw
	{
		CGLTexture *th = ExecuteLandArt(id); // m_LandDataIndex[id].Texture;

		if (th != NULL)
		{
			size.Width = th->Width;
			size.Height = th->Height;
		}
	}

	return size;
}
//----------------------------------------------------------------------------------
WISP_GEOMETRY::CRect COrion::GetStaticArtRealPixelDimension(const ushort &id)
{
	return g_MulReader->ReadStaticArtPixelDimension(m_StaticDataIndex[id]);
}
//----------------------------------------------------------------------------------
WISP_GEOMETRY::CSize COrion::GetGumpDimension(const ushort &id)
{
	WISP_GEOMETRY::CSize size;
	CGLTexture *th = ExecuteGump(id); // m_GumpDataIndex[id].Texture;

	if (th != NULL)
	{
		size.Width = th->Width;
		size .Height = th->Height;
	}

	return size;
}
//----------------------------------------------------------------------------------
void COrion::OpenPaperdoll()
{
	PaperdollReq(g_PlayerSerial);
}
//----------------------------------------------------------------------------------
void COrion::OpenStatus(uint serial)
{
	int x = g_MouseManager.Position.X - 76;
	int y = g_MouseManager.Position.Y - 30;

	StatusReq(serial);

	g_GumpManager.AddGump(new CGumpStatusbar(serial, x, y, true));
}
//----------------------------------------------------------------------------------
void COrion::OpenMinimap()
{
	g_GumpManager.AddGump(new CGumpMinimap(g_PlayerSerial, 0, 0, true));
}
//----------------------------------------------------------------------------------
void COrion::OpenWorldMap()
{
	int x = g_ConfigManager.GameWindowX + (g_ConfigManager.GameWindowWidth / 2) - 200;
	int y = g_ConfigManager.GameWindowY + (g_ConfigManager.GameWindowHeight / 2) - 150;

	/*CGumpWorldMap *gump = new CGumpWorldMap(g_PlayerSerial, x, y);
	gump->Called = true;

	g_GumpManager.AddGump(gump);*/
}
//----------------------------------------------------------------------------------
void COrion::OpenJournal()
{
	g_GumpManager.AddGump(new CGumpJournal(g_PlayerSerial, 0, 0, false));
}
//----------------------------------------------------------------------------------
void COrion::OpenSkills()
{
	g_GumpManager.AddGump(new CGumpSkills(g_PlayerSerial, 0, 0, false));
}
//----------------------------------------------------------------------------------
void COrion::OpenBackpack()
{
	if (g_Player != NULL)
	{
		CGameItem *pack = g_Player->FindLayer(OL_BACKPACK);

		if (pack != NULL)
			DoubleClick(pack->Serial);
	}
}
//----------------------------------------------------------------------------------
void COrion::OpenLogOut()
{
	int x = g_ConfigManager.GameWindowX + (g_ConfigManager.GameWindowWidth / 2) - 40;
	int y = g_ConfigManager.GameWindowY + (g_ConfigManager.GameWindowHeight / 2) - 20;

	g_GumpManager.AddGump(new CGumpQuestion(g_PlayerSerial, x, y, CGumpQuestion::ID_GQ_STATE_QUIT));

	InitScreen(GS_GAME_BLOCKED);
	g_GameBlockedScreen.SetCode(3);
}
//----------------------------------------------------------------------------------
void COrion::OpenChat()
{
}
//----------------------------------------------------------------------------------
void COrion::OpenConfiguration()
{
	int x = (GetSystemMetrics(SM_CXSCREEN) / 2) - 383;
	int y = (GetSystemMetrics(SM_CYSCREEN) / 2) - 260;

	g_OptionsConfig = g_ConfigManager;

	g_GumpManager.AddGump(new CGumpOptions(g_PlayerSerial, x, y));
}
//----------------------------------------------------------------------------------
void COrion::OpenMail()
{
}
//----------------------------------------------------------------------------------
void COrion::OpenPartyManifest()
{
	int x = (g_OrionWindow.Size.Width / 2) - 272;
	int y = (g_OrionWindow.Size.Height / 2) - 240;

	g_GumpManager.AddGump(new CGumpPartyManifest(0, x, y, g_Party.CanLoot));
}
//----------------------------------------------------------------------------------
void COrion::OpenProfile(uint serial)
{
	if (!serial)
		serial = g_PlayerSerial;

	CPacketProfileRequest(serial).Send();
}
//----------------------------------------------------------------------------------
void COrion::RequestGuildGump()
{
	CPacketGuildMenuRequest().Send();
}
//----------------------------------------------------------------------------------
void COrion::RequestQuestGump()
{
	CPacketQuestMenuRequest().Send();
}
//----------------------------------------------------------------------------------
