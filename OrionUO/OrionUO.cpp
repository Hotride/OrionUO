// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** OrionUO.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------

#include "stdafx.h"
#include <SDL_loadso.h>
#include <SDL_keyboard.h>
#include "Wisp/WispGlobal.h"
#include "FileSystem.h"
#include "Crypt/CryptEntry.h"

#if defined(ORION_LINUX)
#define CDECL
#endif

//----------------------------------------------------------------------------------
typedef void CDECL PLUGIN_INIT_TYPE_OLD(STRING_LIST &, STRING_LIST &, UINT_LIST &);
typedef void CDECL PLUGIN_INIT_TYPE_NEW(PLUGIN_INFO *);
//----------------------------------------------------------------------------------
PLUGIN_CLIENT_INTERFACE g_PluginClientInterface = { 0 };
PLUGIN_INIT_TYPE_OLD *g_PluginInitOld = NULL;
PLUGIN_INIT_TYPE_NEW *g_PluginInitNew = NULL;

#if USE_ORIONDLL
typedef size_t CDECL PLUGIN_GET_COUNT_FUNC();
PLUGIN_GET_COUNT_FUNC *GetPluginsCount = NULL;
#else
extern ENCRYPTION_TYPE g_EncryptionType;
#endif

COrion g_Orion;
//----------------------------------------------------------------------------------
COrion::COrion()
{
}
//----------------------------------------------------------------------------------
COrion::~COrion()
{
}
//----------------------------------------------------------------------------------
uint Reflect(uint source, int c)
{
    //WISPFUN_DEBUG("c_refl");
    uint value = 0;

    IFOR (i, 1, c + 1)
    {
        if (source & 0x1)
            value |= (1 << (c - i));

        source >>= 1;
    }

    return value;
}
//----------------------------------------------------------------------------------
uint COrion::GetFileHashCode(puchar ptr, size_t size)
{
    WISPFUN_DEBUG("c194_f1");
    uint crc = 0xFFFFFFFF;

    while (size > 0)
    {
        crc = (crc >> 8) ^ m_CRC_Table[(crc & 0xFF) ^ *ptr];

        ptr++;
        size--;
    }

    return (crc & 0xFFFFFFFF);
}
//----------------------------------------------------------------------------------
string COrion::DecodeArgumentString(const char *text, int length)
{
    WISPFUN_DEBUG("c194_f2");
    string result = "";

    for (int i = 0; i < length; i += 2)
    {
        char buf[5] = { '0', 'x', text[i], text[i + 1], 0 };

        char *end = NULL;
        result += (char)strtoul(buf, &end, 16);
    }

    return result;
}
//----------------------------------------------------------------------------------
void COrion::ParseCommandLine() // FIXME: move this out
{
    WISPFUN_DEBUG("c194_f3");
    bool fastLogin = false;
    int argc = 0;
    LPWSTR *args = CommandLineToArgvW(GetCommandLineW(), &argc);

    auto defaultPluginPath{ g_App.ExeFilePath("OA/OrionAssistant.dll") };
    string defaultPluginFunction = "Install";
    uint defaultPluginFlags = 0xFFFFFFFF;

    IFOR (i, 0, argc)
    {
        if (!args[i] || *args[i] != L'-')
            continue;

        string str = ToString(args[i] + 1);

        WISP_FILE::CTextFileParser parser({}, " ,:", "", "''");

        STRING_LIST strings = parser.GetTokens(str.c_str());

        if (!strings.size())
            continue;

        str = ToLowerA(strings[0]);
        bool haveParam = (strings.size() > 1);
        bool have2Param = (strings.size() > 2);

        if (have2Param)
        {
            if (str == "login")
            {
                DefaultLogin = strings[1];
                DefaultPort = atoi(strings[2].c_str());
            }
            else if (str == "proxyhost")
            {
                g_ConnectionManager.SetUseProxy(true);
                g_ConnectionManager.SetProxyAddress(strings[1]);
                g_ConnectionManager.SetProxyPort(atoi(strings[2].c_str()));
            }
            else if (str == "proxyaccount")
            {
                g_ConnectionManager.SetProxySocks5(true);
                g_ConnectionManager.SetProxyAccount(
                    DecodeArgumentString(strings[1].c_str(), (int)strings[1].length()));
                g_ConnectionManager.SetProxyPassword(
                    DecodeArgumentString(strings[2].c_str(), (int)strings[2].length()));
            }
            else if (str == "account")
                g_MainScreen.SetAccounting(
                    DecodeArgumentString(strings[1].c_str(), (int)strings[1].length()),
                    DecodeArgumentString(strings[2].c_str(), (int)strings[2].length()));
            else if (str == "plugin")
            {
                strings = WISP_FILE::CTextFileParser({}, ",:", "", "")
                              .GetTokens(ToString(args[i] + 1).c_str(), false);

                if (strings.size() > 4)
                {
                    defaultPluginFlags = 0;

                    if (ToLowerA(strings[4]).find("0x") == 0)
                    {
                        char *end = NULL;
                        defaultPluginFlags = strtoul(strings[4].c_str(), &end, 16);
                    }
                    else
                        defaultPluginFlags = atoi(strings[4].c_str());

                    defaultPluginPath = ToPath(strings[1]) + ToPath(":") + ToPath(strings[2]);
                    defaultPluginFunction = strings[3];
                }
            }
        }
        else if (str == "autologin")
        {
            bool enabled = true;

            if (haveParam)
                enabled = atoi(strings[1].c_str());

            g_MainScreen.m_AutoLogin->Checked = enabled;
        }
        else if (str == "savepassword")
        {
            bool enabled = true;

            if (haveParam)
                enabled = atoi(strings[1].c_str());

            g_MainScreen.m_SavePassword->Checked = enabled;
        }
        else if (str == "fastlogin")
            fastLogin = true;
        else if (str == "autologinname")
        {
            if (g_PacketManager.AutoLoginNames.length())
                g_PacketManager.AutoLoginNames =
                    string("|") +
                    DecodeArgumentString(strings[1].c_str(), (int)strings[1].length()) +
                    g_PacketManager.AutoLoginNames;
            else
                g_PacketManager.AutoLoginNames =
                    string("|") +
                    DecodeArgumentString(strings[1].c_str(), (int)strings[1].length());
        }
        else if (str == "nowarnings")
            g_ShowWarnings = false;
#if !USE_ORIONDLL
        else if (str == "nocrypt")
            g_EncryptionType = ET_NOCRYPT;
#endif
    }

    LocalFree(args);

    LoadPlugin(defaultPluginPath, defaultPluginFunction, defaultPluginFlags);

    if (fastLogin)
        g_OrionWindow.CreateTimer(COrionWindow::FASTLOGIN_TIMER_ID, 50);
}
//----------------------------------------------------------------------------------
#if defined(ORION_WINDOWS) // FIXME: Used only by ExceptionFiler
UINT_LIST COrion::FindPattern(puchar ptr, int size, const UCHAR_LIST &pattern)
{
    WISPFUN_DEBUG("c194_f4");
    UINT_LIST result;

    int patternSize = (int)pattern.size();

    int count = size - patternSize - 1;

    IFOR (i, 0, count)
    {
        if (!memcmp(&ptr[0], &pattern[0], patternSize))
            result.push_back(0x00400000 + (int)i);

        ptr++;
    }

    return result;
}
#endif // ORION_WINDOWS
//----------------------------------------------------------------------------------
bool COrion::Install()
{
    WISPFUN_DEBUG("c194_f5");

    LOG("COrion::Install()\n");
#if USE_WISP
    SetUnhandledExceptionFilter(OrionUnhandledExceptionFilter);
#endif
    auto orionVersionStr = g_App.GetFileVersion(&OrionVersionNumeric);
    auto buildStamp = GetBuildDateTimeStamp();
    LOG("Orion version is: %s (build %s)\n", orionVersionStr.c_str(), buildStamp.c_str());
    CRASHLOG("Orion version is: %s (build %s)\n", orionVersionStr.c_str(), buildStamp.c_str());

    auto clientCuoPath{ g_App.UOFilesPath("Client.cuo") };
    if (!fs_path_exists(clientCuoPath))
    {
        clientCuoPath = g_App.ExeFilePath("Client.cuo");
        if (!fs_path_exists(clientCuoPath))
        {
            LOG("Client.cuo is missing!\n");
            CRASHLOG("Client.cuo is missing!\n");
            g_OrionWindow.ShowMessage(
                "Configuration file 'Client.cuo' not found in " + ToString(clientCuoPath) +
                    "! Client can't be started!",
                "Error!");
            return false;
        }
    }

    IFOR (i, 0, 256)
    {
        m_CRC_Table[i] = Reflect((int)i, 8) << 24;

        IFOR (j, 0, 8)
            m_CRC_Table[i] =
                (m_CRC_Table[i] << 1) ^ ((m_CRC_Table[i] & (1 << 31)) ? 0x04C11DB7 : 0);

        m_CRC_Table[i] = Reflect(m_CRC_Table[i], 32);
    }

    //GetCurrentLocale();
    fs_path_create(g_App.ExeFilePath("snapshots"));

    LOG("Loading client config.\n");
    if (!LoadClientConfig())
        return false;

    LOG("Client config loaded!\n");
    if (g_PacketManager.GetClientVersion() >= CV_305D)
    {
        CGumpSpellbook::m_SpellReagents1[4] = "Sulfurous ash";                 //Magic Arrow
        CGumpSpellbook::m_SpellReagents1[17] = "Black pearl";                  //Fireball
        CGumpSpellbook::m_SpellReagents1[29] = "Mandrake root\nSulfurous ash"; //Lightning
        CGumpSpellbook::m_SpellReagents1[42] = "Bloodmoss\nMandrake root";     //Explosion
    }

    LoadAutoLoginNames();

    LOG("Load files\n");

    if (g_PacketManager.GetClientVersion() >= CV_7000)
    {
        g_FileManager.TryReadUOPAnimations();
    }

    if (!g_FileManager.Load())
    {
        auto errMsg =
            string(
                "Error loading a memmapped file. Please check the log for more info.\nUsing UO search path: ") +
            StringFromPath(g_App.m_UOPath);
        g_OrionWindow.ShowMessage(errMsg, "FileManager::Load");
        return false;
    }

    g_SpeechManager.LoadSpeech();

    CGumpSpellbook::InitStaticData();

    m_AnimData.resize(g_FileManager.m_AnimdataMul.Size);
    memcpy(&m_AnimData[0], &g_FileManager.m_AnimdataMul.Start[0], g_FileManager.m_AnimdataMul.Size);

    g_ColorManager.Init();

    LOG("Load tiledata\n");

    int staticsCount = 512;

    if (g_PacketManager.GetClientVersion() >= CV_7090)
        staticsCount = (int)(g_FileManager.m_TiledataMul.Size - (512 * sizeof(LAND_GROUP_NEW))) /
                       sizeof(STATIC_GROUP_NEW);
    else
        staticsCount = (int)(g_FileManager.m_TiledataMul.Size - (512 * sizeof(LAND_GROUP_OLD))) /
                       sizeof(STATIC_GROUP_OLD);

    if (staticsCount > 2048)
        staticsCount = 2048;

    LOG("staticsCount=%i\n", staticsCount);
    LoadTiledata(512, staticsCount);
    LOG("Load indexes\n");
    LoadIndexFiles();
    InitStaticAnimList();

    LOG("Load fonts.\n");
    if (!g_FontManager.LoadFonts())
    {
        LOG("Error loading fonts\n");
        g_OrionWindow.ShowMessage("Error loading fonts", "Error loading fonts!");

        return false;
    }

    LOG("Load skills.\n");
    if (!g_SkillsManager.Load())
    {
        LOG("Error loading skills\n");
        g_OrionWindow.ShowMessage("Error loading skills", "Error loading skills!");

        return false;
    }

    LOG("Create map blocksTable\n");
    g_MapManager.CreateBlocksTable();

    LOG("Patch files\n");
    PatchFiles();
    LOG("Replaces...\n");
    IndexReplaces();

    CheckStaticTileFilterFiles();

    WISP_GEOMETRY::CSize statusbarDims = GetGumpDimension(0x0804);

    CGumpStatusbar::m_StatusbarDefaultWidth = statusbarDims.Width;
    CGumpStatusbar::m_StatusbarDefaultHeight = statusbarDims.Height;

    LOG("Sort skills...\n");
    g_SkillsManager.Sort();

    LOG("Load cursors.\n");
    if (!g_MouseManager.LoadCursorTextures())
    {
        LOG("Error loading cursors\n");
        g_OrionWindow.ShowMessage("Error loading cursors", "Error loading cursors!");

        return false;
    }

    if (!g_SoundManager.Init())
    {
        //LOG("Error install BASS audio: %s\n", BASS_error());

        if (g_ShowWarnings)
            g_OrionWindow.ShowMessage("Failed to init BASS audio.", "Sound error!");

        //return false;
    }

    LoadContainerOffsets();

    g_CityManager.Init();

    g_EntryPointer = NULL;

    LOG("Load prof.\n");
    g_ProfessionManager.Load();
    g_ProfessionManager.Selected = (CBaseProfession *)g_ProfessionManager.m_Items;

    ExecuteStaticArt(0x0EED); //gp 1
    ExecuteStaticArt(0x0EEE); //gp 2-5
    ExecuteStaticArt(0x0EEF); //gp 6+

    g_CreateCharacterManager.Init();

    IFOR (i, 0, 6)
        g_AnimationManager.Init(
            (int)i,
            (size_t)g_FileManager.m_AnimIdx[i].Start,
            (size_t)g_FileManager.m_AnimIdx[i].Size);

    g_AnimationManager.InitIndexReplaces((puint)g_FileManager.m_VerdataMul.Start);

    LOG("Load client startup.\n");
    LoadClientStartupConfig();

    ushort b = 0x0000;
    ushort r = 0xFC00; // 0xFF0000FF;
    ushort g = 0x83E0; // 0xFF00FF00;

    ushort pdwlt[2][140] = {
        { b, b, b, g, g, g, g, g, g, g, b, b, b, g, g, g, g, g, g, g, b, b, b, b, g, g, b, b,
          b, b, b, b, b, b, g, g, b, b, b, b, b, b, b, b, g, g, b, b, b, b, g, g, g, g, g, g,
          g, g, g, b, g, g, g, g, g, g, g, g, g, b, g, g, b, b, b, b, b, g, g, b, g, g, b, g,
          g, b, b, g, g, b, g, g, b, b, g, b, b, g, g, b, g, g, b, b, g, b, b, g, g, b, g, g,
          g, b, b, b, g, g, g, b, b, g, g, g, g, g, g, g, b, b, b, b, g, g, g, g, g, b, b, b },
        { b, r, r, r, r, r, r, r, b, b, b, r, r, r, r, r, r, r, b, b, b, b, r, r, b, r, r, b,
          b, b, b, b, r, r, b, r, r, b, b, b, b, b, r, r, b, r, r, b, b, b, r, r, r, r, r, r,
          r, r, r, b, r, r, r, r, r, r, r, r, r, b, r, r, b, b, b, b, b, r, r, b, r, r, b, r,
          r, b, b, r, r, b, r, r, b, b, r, b, b, r, r, b, r, r, b, b, r, b, b, r, r, b, r, r,
          r, b, b, b, r, r, r, b, b, r, r, r, r, r, r, r, b, b, b, b, r, r, r, r, r, b, b, b }
    };

    IFOR (i, 0, 2)
        g_GL_BindTexture16(g_TextureGumpState[i], 10, 14, &pdwlt[i][0]);

    memset(&m_WinterTile[0], 0, sizeof(m_WinterTile));

    m_WinterTile[196] = 0x011A;
    m_WinterTile[197] = 0x011B;
    m_WinterTile[198] = 0x011C;
    m_WinterTile[199] = 0x011D;
    m_WinterTile[206] = 0x05C0;
    m_WinterTile[248] = 0x011A;
    m_WinterTile[249] = 0x011B;
    m_WinterTile[250] = 0x011C;
    m_WinterTile[251] = 0x011D;
    m_WinterTile[804] = 0x0391;
    m_WinterTile[805] = 0x0392;
    m_WinterTile[806] = 0x0393;
    m_WinterTile[807] = 0x0394;
    m_WinterTile[808] = 0x0395;
    m_WinterTile[809] = 0x0396;
    m_WinterTile[1521] = 0x011A;
    m_WinterTile[1522] = 0x011B;
    m_WinterTile[1523] = 0x011C;
    m_WinterTile[1524] = 0x011D;
    m_WinterTile[1529] = 0x011A;
    m_WinterTile[1530] = 0x011B;
    m_WinterTile[1531] = 0x011C;
    m_WinterTile[1532] = 0x011D;
    m_WinterTile[1533] = 0x011B;
    m_WinterTile[1534] = 0x011C;
    m_WinterTile[1535] = 0x011D;
    m_WinterTile[1536] = 0x011B;
    m_WinterTile[1537] = 0x011C;
    m_WinterTile[1538] = 0x011D;
    m_WinterTile[1539] = 0x011C;
    m_WinterTile[1540] = 0x011D;

    LOG("Init light buffer.\n");
    g_LightBuffer.Init(640, 480);

    LOG("Create object handles.\n");
    CreateObjectHandlesBackground();

    LOG("Create aura.\n");
    CreateAuraTexture();

    LOG("Load shaders.\n");
    LoadShaders();

    LOG("Update main screen content\n");
    g_MainScreen.UpdateContent();
    g_MainScreen.LoadGlobalConfig();

    LOG("Init screen...\n");

    InitScreen(GS_MAIN);

    if (g_PacketManager.GetClientVersion() >= CV_7000)
    {
        LOG("Waiting for FileManager to try & load AnimationFrame files\n");
        g_FileManager.m_AutoResetEvent.WaitOne();
        LOG("FileManager.TryReadUOPAnimations() done!\n");
    }

    LOG("Installation completed!\n");

    return true;
}
//----------------------------------------------------------------------------------
void COrion::Uninstall()
{
    WISPFUN_DEBUG("c194_f6");
    LOG("COrion::Uninstall()\n");
    SaveLocalConfig(g_PacketManager.ConfigSerial);
    g_MainScreen.SaveGlobalConfig();
    g_GumpManager.OnDelete();

    Disconnect();

    UnloadIndexFiles();

    g_EntryPointer = NULL;
    g_CurrentScreen = NULL;

    g_AuraTexture.Clear();

    IFOR (i, 0, 6)
        g_MapTexture[i].Clear();

    IFOR (i, 0, 2)
        g_TextureGumpState[i].Clear();

    g_SoundManager.Free();
    g_FileManager.Unload();
    g_LightBuffer.Free();

    g_GL.Uninstall();
}
//----------------------------------------------------------------------------------
void COrion::InitScreen(GAME_STATE state)
{
    WISPFUN_DEBUG("c194_f7");
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
void COrion::GetCurrentLocale()
{
    WISPFUN_DEBUG("c194_f8");
    switch (LOBYTE(GetSystemDefaultLangID()))
    {
        case LANG_RUSSIAN:
        {
            g_Language = "RUS";
            break;
        }
        case LANG_FRENCH:
        {
            g_Language = "FRA";
            break;
        }
        case LANG_GERMAN:
        {
            g_Language = "DEU";
            break;
        }
        case LANG_SPANISH:
        {
            g_Language = "ESP";
            break;
        }
        case LANG_JAPANESE:
        {
            g_Language = "JPN";
            break;
        }
        case LANG_KOREAN:
        {
            g_Language = "KOR";
            break;
        }
        default:
        {
            g_Language = "ENU";
            break;
        }
    }

    LOG("Locale set to: %s\n", g_Language.c_str());

    //https://msdn.microsoft.com/en-us/library/cc233982.aspx

    /*wchar_t localeName[LOCALE_NAME_MAX_LENGTH] = { 0 };

	if (GetSystemDefaultLocaleName(&localeName[0], LOCALE_NAME_MAX_LENGTH))
	{
		IFOR(i, 0, 10)
		{
			if (!localeName[i])
				break;
			else if (localeName[i] == L'-')
			{
				localeName[i] = 0;
				break;
			}
		}

		wstring locale = ToLowerW(localeName);

		if (locale == L"ru")
			g_Language = "RUS";
		else if (locale == L"fr")
			g_Language = "FRA";
		else if (locale == L"de")
			g_Language = "DEU";

		LOG("Locale set to: %s\n", g_Language.c_str());
	}
	else
		LOG("Locale set to default value: ENU\n");*/
}
//----------------------------------------------------------------------------------
ushort COrion::TextToGraphic(const char *text)
{
    WISPFUN_DEBUG("c194_f9");
    if (strlen(text) > 2 && text[0] == '0' && (text[1] == 'x' || text[1] == 'X'))
    {
        long l = strtol(text + 2, NULL, 16);

        if (l < 0 || l > 0xFFFF)
            return 0xFFFF;

        return (ushort)l;
    }
    else if (text[0] >= '0' && text[0] <= '9')
        return atoi(text);

    return 0;
}
//----------------------------------------------------------------------------------
void COrion::CheckStaticTileFilterFiles()
{
    WISPFUN_DEBUG("c194_f10");
    memset(&m_StaticTilesFilterFlags[0], 0, sizeof(m_StaticTilesFilterFlags));

    auto path{ g_App.ExeFilePath("OrionData") };
    fs_path_create(path);

    auto filePath{ path + PATH_SEP + ToPath("cave.txt") };
    if (!fs_path_exists(filePath))
    {
        WISP_LOGGER::CLogger file;

        file.Init(filePath);
        file.Print("#Format: graphic\n");

        IFOR (i, 0x053B, 0x0553 + 1)
        {
            if (i != 0x0550)
                file.Print("0x%04X\n", i);
        }
    }

    filePath = path + PATH_SEP + ToPath("vegetation.txt");
    WISP_LOGGER::CLogger vegetationFile;

    if (!fs_path_exists(filePath))
    {
        vegetationFile.Init(filePath);
        vegetationFile.Print("#Format: graphic\n");

        static const int vegetationTilesCount = 178;

        static const ushort vegetationTiles[vegetationTilesCount] = {
            0x0D45, 0x0D46, 0x0D47, 0x0D48, 0x0D49, 0x0D4A, 0x0D4B, 0x0D4C, 0x0D4D, 0x0D4E, 0x0D4F,
            0x0D50, 0x0D51, 0x0D52, 0x0D53, 0x0D54, 0x0D5C, 0x0D5D, 0x0D5E, 0x0D5F, 0x0D60, 0x0D61,
            0x0D62, 0x0D63, 0x0D64, 0x0D65, 0x0D66, 0x0D67, 0x0D68, 0x0D69, 0x0D6D, 0x0D73, 0x0D74,
            0x0D75, 0x0D76, 0x0D77, 0x0D78, 0x0D79, 0x0D7A, 0x0D7B, 0x0D7C, 0x0D7D, 0x0D7E, 0x0D7F,
            0x0D80, 0x0D83, 0x0D87, 0x0D88, 0x0D89, 0x0D8A, 0x0D8B, 0x0D8C, 0x0D8D, 0x0D8E, 0x0D8F,
            0x0D90, 0x0D91, 0x0D93, 0x12B6, 0x12B7, 0x12BC, 0x12BD, 0x12BE, 0x12BF, 0x12C0, 0x12C1,
            0x12C2, 0x12C3, 0x12C4, 0x12C5, 0x12C6, 0x12C7, 0x0CB9, 0x0CBC, 0x0CBD, 0x0CBE, 0x0CBF,
            0x0CC0, 0x0CC1, 0x0CC3, 0x0CC5, 0x0CC6, 0x0CC7, 0x0CF3, 0x0CF4, 0x0CF5, 0x0CF6, 0x0CF7,
            0x0D04, 0x0D06, 0x0D07, 0x0D08, 0x0D09, 0x0D0A, 0x0D0B, 0x0D0C, 0x0D0D, 0x0D0E, 0x0D0F,
            0x0D10, 0x0D11, 0x0D12, 0x0D13, 0x0D14, 0x0D15, 0x0D16, 0x0D17, 0x0D18, 0x0D19, 0x0D28,
            0x0D29, 0x0D2A, 0x0D2B, 0x0D2D, 0x0D34, 0x0D36, 0x0DAE, 0x0DAF, 0x0DBA, 0x0DBB, 0x0DBC,
            0x0DBD, 0x0DBE, 0x0DC1, 0x0DC2, 0x0DC3, 0x0C83, 0x0C84, 0x0C85, 0x0C86, 0x0C87, 0x0C88,
            0x0C89, 0x0C8A, 0x0C8B, 0x0C8C, 0x0C8D, 0x0C8E, 0x0C93, 0x0C94, 0x0C98, 0x0C9F, 0x0CA0,
            0x0CA1, 0x0CA2, 0x0CA3, 0x0CA4, 0x0CA7, 0x0CAC, 0x0CAD, 0x0CAE, 0x0CAF, 0x0CB0, 0x0CB1,
            0x0CB2, 0x0CB3, 0x0CB4, 0x0CB5, 0x0CB6, 0x0C45, 0x0C46, 0x0C49, 0x0C47, 0x0C48, 0x0C4A,
            0x0C4B, 0x0C4C, 0x0C4D, 0x0C4E, 0x0C37, 0x0C38, 0x0CBA, 0x0D2F, 0x0D32, 0x0D33, 0x0D3F,
            0x0D40, 0x0CE9
        };

        IFOR (i, 0, vegetationTilesCount)
        {
            int64_t flags = g_Orion.GetStaticFlags(vegetationTiles[i]);
            if (flags & 0x00000040)
            {
                continue;
            }

            vegetationFile.Print("0x%04X\n", vegetationTiles[i]);
        }
    }

    filePath = path + PATH_SEP + ToPath("stumps.txt");

    if (!fs_path_exists(filePath))
    {
        WISP_LOGGER::CLogger file;

        file.Init(filePath);
        file.Print("#Format: graphic hatched\n");

        static const int treeTilesCount = 53;

        static const ushort treeTiles[treeTilesCount] = {
            0x0CCA, 0x0CCB, 0x0CCC, 0x0CCD, 0x0CD0, 0x0CD3, 0x0CD6, 0x0CD8, 0x0CDA, 0x0CDD, 0x0CE0,
            0x0CE3, 0x0CE6, 0x0D41, 0x0D42, 0x0D43, 0x0D44, 0x0D57, 0x0D58, 0x0D59, 0x0D5A, 0x0D5B,
            0x0D6E, 0x0D6F, 0x0D70, 0x0D71, 0x0D72, 0x0D84, 0x0D85, 0x0D86, 0x0D94, 0x0D98, 0x0D9C,
            0x0DA0, 0x0DA4, 0x0DA8, 0x0C9E, 0x0CA8, 0x0CAA, 0x0CAB, 0x0CC9, 0x0CF8, 0x0CFB, 0x0CFE,
            0x0D01, 0x12B6, 0x12B7, 0x12B8, 0x12B9, 0x12BA, 0x12BB, 0x12BC, 0x12BD
        };

        IFOR (i, 0, treeTilesCount)
        {
            ushort graphic = treeTiles[i];
            uchar hatched = 1;

            switch (graphic)
            {
                case 0x0C9E:
                case 0x0CA8:
                case 0x0CAA:
                case 0x0CAB:
                case 0x0CC9:
                case 0x0CF8:
                case 0x0CFB:
                case 0x0CFE:
                case 0x0D01:
                case 0x12B6:
                case 0x12B7:
                case 0x12B8:
                case 0x12B9:
                case 0x12BA:
                case 0x12BB:
                    hatched = 0;
                default:
                    break;
            }

            int64_t flags = g_Orion.GetStaticFlags(graphic);

            if (!(flags & 0x00000040))
            {
                vegetationFile.Print("0x%04X\n", graphic);
            }
            else
                file.Print("0x%04X\t%i\n", graphic, hatched);
        }
    }

    filePath = path + PATH_SEP + ToPath("cave.txt");

    WISP_FILE::CTextFileParser caveParser(filePath, " \t", "#;//", "");

    while (!caveParser.IsEOF())
    {
        STRING_LIST strings = caveParser.ReadTokens();

        if (strings.size() >= 1)
        {
            ushort graphic = TextToGraphic(strings[0].c_str());

            m_StaticTilesFilterFlags[graphic] |= STFF_CAVE;

            m_CaveTiles.push_back(graphic);
        }
    }

    filePath = path + PATH_SEP + ToPath("stumps.txt");

    WISP_FILE::CTextFileParser stumpParser(filePath, " \t", "#;//", "");

    while (!stumpParser.IsEOF())
    {
        STRING_LIST strings = stumpParser.ReadTokens();

        if (strings.size() >= 2)
        {
            uchar flag = STFF_STUMP;

            if (atoi(strings[1].c_str()))
                flag |= STFF_STUMP_HATCHED;

            ushort graphic = TextToGraphic(strings[0].c_str());

            m_StaticTilesFilterFlags[graphic] |= flag;

            m_StumpTiles.push_back(graphic);
        }
    }

    filePath = path + PATH_SEP + ToPath("vegetation.txt");

    WISP_FILE::CTextFileParser vegetationParser(filePath, " \t", "#;//", "");

    while (!vegetationParser.IsEOF())
    {
        STRING_LIST strings = vegetationParser.ReadTokens();

        if (strings.size() >= 1)
            m_StaticTilesFilterFlags[TextToGraphic(strings[0].c_str())] |= STFF_VEGETATION;
    }
}
//----------------------------------------------------------------------------------
void COrion::LoadContainerOffsets()
{
    auto path{ g_App.ExeFilePath("OrionData") };
    fs_path_create(path);

    auto filePath{ path + PATH_SEP + ToPath("containers.txt") };
    LOG("Containers: %s\n", CStringFromPath(filePath));
    if (!fs_path_exists(filePath))
    {
        //												Gump   OpenSnd  CloseSnd					minX minY maxX maxY
        g_ContainerOffset.push_back(CContainerOffset(
            0x0009, 0x0000, 0x0000, CContainerOffsetRect(20, 85, 124, 196))); //corpse
        g_ContainerOffset.push_back(
            CContainerOffset(0x003C, 0x0048, 0x0058, CContainerOffsetRect(44, 65, 186, 159)));
        g_ContainerOffset.push_back(
            CContainerOffset(0x003D, 0x0048, 0x0058, CContainerOffsetRect(29, 34, 137, 128)));
        g_ContainerOffset.push_back(
            CContainerOffset(0x003E, 0x002F, 0x002E, CContainerOffsetRect(33, 36, 142, 148)));
        g_ContainerOffset.push_back(
            CContainerOffset(0x003F, 0x004F, 0x0058, CContainerOffsetRect(19, 47, 182, 123)));
        g_ContainerOffset.push_back(
            CContainerOffset(0x0040, 0x002D, 0x002C, CContainerOffsetRect(16, 51, 150, 140)));
        g_ContainerOffset.push_back(
            CContainerOffset(0x0041, 0x004F, 0x0058, CContainerOffsetRect(35, 38, 145, 116)));
        g_ContainerOffset.push_back(
            CContainerOffset(0x0042, 0x002D, 0x002C, CContainerOffsetRect(18, 105, 162, 178)));
        g_ContainerOffset.push_back(
            CContainerOffset(0x0043, 0x002D, 0x002C, CContainerOffsetRect(16, 51, 181, 124)));
        g_ContainerOffset.push_back(
            CContainerOffset(0x0044, 0x002D, 0x002C, CContainerOffsetRect(20, 10, 170, 100)));
        g_ContainerOffset.push_back(
            CContainerOffset(0x0048, 0x002F, 0x002E, CContainerOffsetRect(16, 10, 154, 94)));
        g_ContainerOffset.push_back(
            CContainerOffset(0x0049, 0x002D, 0x002C, CContainerOffsetRect(18, 105, 162, 178)));
        g_ContainerOffset.push_back(
            CContainerOffset(0x004A, 0x002D, 0x002C, CContainerOffsetRect(18, 105, 162, 178)));
        g_ContainerOffset.push_back(
            CContainerOffset(0x004B, 0x002D, 0x002C, CContainerOffsetRect(16, 51, 184, 124)));
        g_ContainerOffset.push_back(
            CContainerOffset(0x004C, 0x002D, 0x002C, CContainerOffsetRect(46, 74, 196, 184)));
        g_ContainerOffset.push_back(
            CContainerOffset(0x004D, 0x002F, 0x002E, CContainerOffsetRect(76, 12, 140, 68)));
        g_ContainerOffset.push_back(CContainerOffset(
            0x004E, 0x002D, 0x002C, CContainerOffsetRect(24, 96, 140, 152))); //bugged
        g_ContainerOffset.push_back(CContainerOffset(
            0x004F, 0x002D, 0x002C, CContainerOffsetRect(24, 96, 140, 152))); //bugged
        g_ContainerOffset.push_back(
            CContainerOffset(0x0051, 0x002F, 0x002E, CContainerOffsetRect(16, 10, 154, 94)));
        g_ContainerOffset.push_back(CContainerOffset(
            0x091A, 0x0000, 0x0000, CContainerOffsetRect(1, 13, 260, 199))); //game board
        g_ContainerOffset.push_back(CContainerOffset(
            0x092E, 0x0000, 0x0000, CContainerOffsetRect(1, 13, 260, 199))); //backgammon game
        g_ContainerOffset.push_back(
            CContainerOffset(0x0104, 0x002F, 0x002E, CContainerOffsetRect(0, 20, 168, 115)));
        g_ContainerOffset.push_back(
            CContainerOffset(0x0105, 0x002F, 0x002E, CContainerOffsetRect(0, 20, 168, 115)));
        g_ContainerOffset.push_back(
            CContainerOffset(0x0106, 0x002F, 0x002E, CContainerOffsetRect(0, 20, 168, 115)));
        g_ContainerOffset.push_back(
            CContainerOffset(0x0107, 0x002F, 0x002E, CContainerOffsetRect(0, 20, 168, 115)));
        g_ContainerOffset.push_back(
            CContainerOffset(0x0108, 0x004F, 0x0058, CContainerOffsetRect(0, 35, 150, 105)));
        g_ContainerOffset.push_back(
            CContainerOffset(0x0109, 0x002F, 0x002E, CContainerOffsetRect(0, 20, 175, 105)));
        g_ContainerOffset.push_back(
            CContainerOffset(0x010A, 0x002F, 0x002E, CContainerOffsetRect(0, 20, 175, 105)));
        g_ContainerOffset.push_back(
            CContainerOffset(0x010B, 0x002F, 0x002E, CContainerOffsetRect(0, 20, 175, 105)));
        g_ContainerOffset.push_back(
            CContainerOffset(0x010C, 0x002F, 0x002E, CContainerOffsetRect(0, 20, 168, 115)));
        g_ContainerOffset.push_back(
            CContainerOffset(0x010D, 0x002F, 0x002E, CContainerOffsetRect(0, 20, 168, 115)));
        g_ContainerOffset.push_back(
            CContainerOffset(0x010E, 0x002F, 0x002E, CContainerOffsetRect(0, 20, 168, 115)));
        //present boxes
        g_ContainerOffset.push_back(
            CContainerOffset(0x0102, 0x004F, 0x0058, CContainerOffsetRect(15, 10, 245, 120)));
        g_ContainerOffset.push_back(
            CContainerOffset(0x011B, 0x004F, 0x0058, CContainerOffsetRect(15, 10, 220, 120)));
        g_ContainerOffset.push_back(
            CContainerOffset(0x011C, 0x004F, 0x0058, CContainerOffsetRect(10, 10, 220, 145)));
        g_ContainerOffset.push_back(
            CContainerOffset(0x011D, 0x004F, 0x0058, CContainerOffsetRect(10, 10, 220, 130)));
        g_ContainerOffset.push_back(
            CContainerOffset(0x011E, 0x004F, 0x0058, CContainerOffsetRect(15, 10, 290, 130)));
        g_ContainerOffset.push_back(
            CContainerOffset(0x011F, 0x004F, 0x0058, CContainerOffsetRect(15, 10, 220, 120)));
        //secret chest
        g_ContainerOffset.push_back(
            CContainerOffset(0x058E, 0x002D, 0x002C, CContainerOffsetRect(16, 51, 184, 124)));
    }
    else
    {
        WISP_FILE::CTextFileParser parser(filePath, " \t", "#;//", "");

        while (!parser.IsEOF())
        {
            STRING_LIST strings = parser.ReadTokens();

            if (strings.size() >= 7)
            {
                ushort gump = TextToGraphic(strings[0].c_str());
                ushort openSound = TextToGraphic(strings[1].c_str());
                ushort closeSound = TextToGraphic(strings[2].c_str());
                ushort minX = atoi(strings[3].c_str());
                ushort minY = atoi(strings[4].c_str());
                ushort maxX = atoi(strings[5].c_str());
                ushort maxY = atoi(strings[6].c_str());

                g_ContainerOffset.push_back(CContainerOffset(
                    gump, openSound, closeSound, CContainerOffsetRect(minX, minY, maxX, maxY)));
            }
        }
    }

    if (!fs_path_exists(filePath))
    {
        WISP_LOGGER::CLogger file;

        file.Init(filePath);
        file.Print("#Format: gump open_sound close_sound minX minY maxX maxY\n");

        for (const CContainerOffset &item : g_ContainerOffset)
        {
            file.Print(
                "0x%04X 0x%04X 0x%04X %i %i %i %i\n",
                item.Gump,
                item.OpenSound,
                item.CloseSound,
                item.Rect.MinX,
                item.Rect.MinY,
                item.Rect.MaxX,
                item.Rect.MaxY);
        }
    }

    LOG("g_ContainerOffset.size()=%i\n", g_ContainerOffset.size());
}
//----------------------------------------------------------------------------------
#if !USE_ORIONDLL
bool COrion::LoadClientConfig()
{
    WISPFUN_DEBUG("c194_f11");

    WISP_FILE::CMappedFile config;

    if (config.Load(g_App.UOFilesPath("Client.cuo")) ||
        config.Load(g_App.ExeFilePath("Client.cuo")))
    {
        unsigned char data[1024] = {};
        size_t dataSize = sizeof(data);
        CryptInstallNew(config.Start, config.Size, data, dataSize);
        config.Unload();

        if (!dataSize)
        {
            g_OrionWindow.ShowMessage("Corrupted config data!", "Error!");
            return false;
        }

        WISP_DATASTREAM::CDataReader file(data, dataSize);

        uchar version = file.ReadInt8();
        uchar dllVersion = file.ReadInt8();
        uchar subVersion = file.ReadInt8();
        g_PacketManager.SetClientVersion((CLIENT_VERSION)file.ReadInt8());

        if (g_PacketManager.GetClientVersion() >= CV_70331)
            g_MaxViewRange = MAX_VIEW_RANGE_NEW;
        else
            g_MaxViewRange = MAX_VIEW_RANGE_OLD;

        int len = file.ReadInt8();
        ClientVersionText = file.ReadString(len);

#if defined(_M_IX86)
        g_NetworkInit = (NETWORK_INIT_TYPE *)file.ReadUInt32LE();
        g_NetworkAction = (NETWORK_ACTION_TYPE *)file.ReadUInt32LE();
        g_NetworkPostAction = (NETWORK_POST_ACTION_TYPE *)file.ReadUInt32LE();
        g_PluginInitNew = (PLUGIN_INIT_TYPE_NEW *)file.ReadUInt32LE();
#else
        g_NetworkInit = (NETWORK_INIT_TYPE *)file.ReadUInt64LE();
        g_NetworkAction = (NETWORK_ACTION_TYPE *)file.ReadUInt64LE();
        g_NetworkPostAction = (NETWORK_POST_ACTION_TYPE *)file.ReadUInt64LE();
        g_PluginInitNew = (PLUGIN_INIT_TYPE_NEW *)file.ReadUInt64LE();
#endif

        int mapsCount = MAX_MAPS_COUNT;

        if (version >= 4)
            mapsCount = file.ReadUInt8();
        else
            file.Move(1);

        IFOR (i, 0, mapsCount)
        {
            g_MapSize[i].Width = file.ReadUInt16LE();
            g_MapSize[i].Height = file.ReadUInt16LE();

            g_MapBlockSize[i].Width = g_MapSize[i].Width / 8;
            g_MapBlockSize[i].Height = g_MapSize[i].Height / 8;
        }

        g_CharacterList.ClientFlag = file.ReadInt8();
        g_FileManager.UseVerdata = (file.ReadInt8() != 0);

        LOG("\tCUO Version: %d\n", version);
        LOG("\tClient Version: %d\n", g_PacketManager.GetClientVersion());
        LOG("\tClient Text: %s\n", ClientVersionText.c_str());
        LOG("\tMaps Count: %d\n", mapsCount);
        LOG("\tUse Verdata: %d\n", g_FileManager.UseVerdata);

        return true;
    }

    return false;
}
#else
bool COrion::LoadClientConfig()
{
    WISPFUN_DEBUG("c194_f11");
    auto path = g_App.ExeFilePath("Orion.dll");
    auto orionDll = SDL_LoadObject(CStringFromPath(path));

    if (orionDll == 0)
    {
        g_OrionWindow.ShowMessage("Orion.dll not found in " + ToString(path), "Error!");
        return false;
    }

    typedef void CDECL installFuncOld(uchar *, int, UCHAR_LIST *);
    typedef void CDECL installFuncNew(uchar *, size_t, uchar *, size_t &);

    installFuncOld *installOld = (installFuncOld *)SDL_LoadFunction(orionDll, "Install");
    installFuncNew *installNew = (installFuncNew *)SDL_LoadFunction(orionDll, "InstallNew");

    if (installNew == NULL)
    {
        if (installOld == NULL)
        {
            g_OrionWindow.ShowMessage(
                "Install of InstallNew function in Orion.dll not found!", "Error!");
            return false;
        }
    }
    else
        installOld = NULL;

    WISP_FILE::CMappedFile config;

    if (config.Load(g_App.UOFilesPath("Client.cuo")) ||
        config.Load(g_App.ExeFilePath("Client.cuo")))
    {
        UCHAR_LIST realData(config.Size * 2, 0);
        size_t realSize = 0;

        if (installOld != NULL)
        {
            installOld(config.Start, (int)config.Size, &realData);
            realSize = realData.size();
        }
        else
            installNew(config.Start, config.Size, &realData[0], realSize);

        config.Unload();

        if (!realSize)
        {
            g_OrionWindow.ShowMessage("Corrupted config data!", "Error!");
            return false;
        }

        GetPluginsCount = (PLUGIN_GET_COUNT_FUNC *)SDL_LoadFunction(orionDll, "GetPluginsCount");

        WISP_DATASTREAM::CDataReader file(&realData[0], realSize);

        uchar version = file.ReadInt8();
        uchar dllVersion = file.ReadInt8();
        uchar subVersion = 0;

        if (dllVersion != 0xFE)
        {
            g_OrionWindow.ShowMessage(
                "Old version of Orion.dll detected!!!\nClient may be crashed in process.",
                "Warning!");
            file.Move(-1);
        }
        else
            subVersion = file.ReadInt8();

        g_PacketManager.SetClientVersion((CLIENT_VERSION)file.ReadInt8());

        if (g_PacketManager.GetClientVersion() >= CV_70331)
            g_MaxViewRange = MAX_VIEW_RANGE_NEW;
        else
            g_MaxViewRange = MAX_VIEW_RANGE_OLD;

        int len = file.ReadInt8();
        ClientVersionText = file.ReadString(len);

#if defined(_M_IX86)
        g_NetworkInit = (NETWORK_INIT_TYPE *)file.ReadUInt32LE();
        g_NetworkAction = (NETWORK_ACTION_TYPE *)file.ReadUInt32LE();
        if (dllVersion == 0xFE)
            g_NetworkPostAction = (NETWORK_POST_ACTION_TYPE *)file.ReadUInt32LE();

        if (installOld != NULL)
            g_PluginInitOld = (PLUGIN_INIT_TYPE_OLD *)file.ReadUInt32LE();
        else
            g_PluginInitNew = (PLUGIN_INIT_TYPE_NEW *)file.ReadUInt32LE();
#else
        g_NetworkInit = (NETWORK_INIT_TYPE *)file.ReadUInt64LE();
        g_NetworkAction = (NETWORK_ACTION_TYPE *)file.ReadUInt64LE();
        if (dllVersion == 0xFE)
            g_NetworkPostAction = (NETWORK_POST_ACTION_TYPE *)file.ReadUInt64LE();

        if (installOld != NULL)
            g_PluginInitOld = (PLUGIN_INIT_TYPE_OLD *)file.ReadUInt64LE();
        else
            g_PluginInitNew = (PLUGIN_INIT_TYPE_NEW *)file.ReadUInt64LE();
#endif

        int mapsCount = MAX_MAPS_COUNT;

        if (version >= 4)
            mapsCount = file.ReadUInt8();
        else
            file.Move(1);

        IFOR (i, 0, mapsCount)
        {
            g_MapSize[i].Width = file.ReadUInt16LE();
            g_MapSize[i].Height = file.ReadUInt16LE();

            g_MapBlockSize[i].Width = g_MapSize[i].Width / 8;
            g_MapBlockSize[i].Height = g_MapSize[i].Height / 8;
        }

        g_CharacterList.ClientFlag = file.ReadInt8();
        g_FileManager.UseVerdata = (file.ReadInt8() != 0);
    }
    return true;
}
#endif
//----------------------------------------------------------------------------------
void COrion::LoadAutoLoginNames()
{
    WISPFUN_DEBUG("c194_f12");
    WISP_FILE::CTextFileParser file(g_App.UOFilesPath("AutoLoginNames.cfg"), "", "#;", "");

    string names = g_PacketManager.AutoLoginNames + "|";

    while (!file.IsEOF())
    {
        STRING_LIST strings = file.ReadTokens(false);

        if (strings.size())
            names += strings[0] + "|";
    }

    g_PacketManager.AutoLoginNames = names;
}
//---------------------------------------------------------------------------
void COrion::ProcessDelayedClicks()
{
    WISPFUN_DEBUG("c194_f13");
    if (g_ClickObject.Enabled && g_ClickObject.Timer < g_Ticks)
    {
        uint serial = 0;

        if (g_ClickObject.Object != NULL)
            serial = g_ClickObject.Object->Serial;

        if (g_ClickObject.Gump == NULL)
        {
            if (serial)
            {
                CGameObject *go = (CGameObject *)g_ClickObject.Object;

                if (!g_TooltipsEnabled || (!go->NPC && go->Locked()))
                    Click(serial);

                g_ObjectPropertiesManager.OnItemClicked(serial);

                if (g_PopupEnabled && (!g_ConfigManager.HoldShiftForContextMenus || g_ShiftPressed))
                    CPacketRequestPopupMenu(serial).Send();
            }
        }
        else
            g_ClickObject.Gump->DelayedClick(g_ClickObject.Object);

        g_ClickObject.Clear();
    }
}
//----------------------------------------------------------------------------------
void COrion::Process(bool rendering)
{
    WISPFUN_DEBUG("c194_f14");
    if (g_CurrentScreen == NULL)
        return;

    static uint removeUnusedTexturesTime = 0;
    static uint removeUnusedAnimationTexturesTime = 0;

    g_MouseManager.Update();

    if (g_GameState >= GS_CHARACTER && (g_LastSendTime + SEND_TIMEOUT_DELAY) < g_Ticks)
    {
        uchar ping[2] = { 0x73, 0 };
        Send(ping, 2);
    }

    bool oldCtrl = g_CtrlPressed;
    bool oldShift = g_ShiftPressed;

#if USE_WISP
    g_AltPressed = GetAsyncKeyState(VK_MENU) & 0x80000000;
    g_CtrlPressed = GetAsyncKeyState(VK_CONTROL) & 0x80000000;
    g_ShiftPressed = GetAsyncKeyState(VK_SHIFT) & 0x80000000;
#else
    auto mod = SDL_GetModState();
    g_AltPressed = mod & KMOD_ALT;
    g_CtrlPressed = mod & KMOD_CTRL;
    g_ShiftPressed = mod & KMOD_SHIFT;
#endif

    if (g_GameState >= GS_GAME) // || g_GameState == GS_GAME_BLOCKED)
    {
        if (g_LogoutAfterClick)
        {
            g_LogoutAfterClick = false;
            LogOut();
            return;
        }

        if (g_ConfigManager.CheckPing && g_PingTimer < g_Ticks)
        {
            CPingThread *pingThread = new CPingThread(0xFFFFFFFF, m_GameServerIP, 10);
            pingThread->Run();
            g_PingTimer = g_Ticks + (g_ConfigManager.GetPingTimer() * 1000);
        }

        g_UseItemActions.Process();

        g_ShowGumpLocker = g_ConfigManager.LockGumpsMoving && g_AltPressed && g_CtrlPressed;

        ProcessStaticAnimList();

        g_EffectManager.UpdateEffects();

        CGumpBuff *gumpBuff = (CGumpBuff *)g_GumpManager.GetGump(0, 0, GT_BUFF);

        if (gumpBuff != NULL)
            gumpBuff->UpdateBuffIcons();

        if (g_World != NULL)
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
            if (g_PressedObject.LeftGump == NULL && g_PressedObject.LeftObject != NULL &&
                g_PressedObject.LeftObject->IsGUI())
                canRenderSelect = false;
        }

        if (g_World != NULL)
        {
            if (g_World->ObjectToRemove != 0)
            {
                CGameObject *removeObj = g_World->FindWorldObject(g_World->ObjectToRemove);
                g_World->ObjectToRemove = 0;

                if (removeObj != NULL)
                {
                    CGameCharacter *character = g_World->FindWorldCharacter(removeObj->Container);

                    g_World->RemoveObject(removeObj);

                    if (character != NULL)
                    {
                        character->m_FrameInfo =
                            g_AnimationManager.CollectFrameInformation(character);
                        g_GumpManager.UpdateContent(g_ObjectInHand.Container, 0, GT_PAPERDOLL);
                    }
                }
            }

            if (g_ProcessRemoveRangedTimer < g_Ticks)
            {
                g_Orion.RemoveRangedObjects();

                g_ProcessRemoveRangedTimer = g_Ticks + 50;
            }

            if (g_ConfigManager.ObjectHandles && g_CtrlPressed && g_ShiftPressed &&
                (oldCtrl != g_CtrlPressed || oldShift != g_ShiftPressed))
                g_World->ResetObjectHandlesState();

            if (rendering)
            {
                g_GameScreen.CalculateGameWindowBounds();

                g_GameScreen.CalculateRenderList();
                g_GameScreen.RenderListInitalized = true;

                g_SelectedObject.Clear();
                g_SelectedGameObjectHandle = 0;

                if (!g_OrionWindow.IsMinimizedWindow())
                {
                    if (canRenderSelect)
                        g_GameScreen.Render(false);

                    CGump::ProcessListing();

                    g_GameScreen.PrepareContent();

                    g_GameScreen.Render(true);

                    UOI_SELECTED_TILE uoiSelectedObject;

                    if (g_SelectedObject.Object != NULL && g_SelectedObject.Object->IsWorldObject())
                    {
                        CRenderWorldObject *rwo = (CRenderWorldObject *)g_SelectedObject.Object;

                        if (rwo->IsLandObject())
                            uoiSelectedObject.Serial = 0xFFFFFFFF;
                        else if (!rwo->IsGameObject())
                            uoiSelectedObject.Serial = 0;
                        else
                            uoiSelectedObject.Serial = rwo->Serial;

                        uoiSelectedObject.Graphic = rwo->Graphic;
                        uoiSelectedObject.Color = rwo->Color;
                        uoiSelectedObject.X = rwo->GetX();
                        uoiSelectedObject.Y = rwo->GetY();
                        uoiSelectedObject.Z = rwo->GetZ();

                        rwo = rwo->GetLand();

                        if (rwo != NULL)
                        {
                            uoiSelectedObject.LandGraphic = rwo->Graphic;
                            uoiSelectedObject.LandX = rwo->GetX();
                            uoiSelectedObject.LandY = rwo->GetY();
                            uoiSelectedObject.LandZ = rwo->GetZ();
                        }
                        else
                        {
                            uoiSelectedObject.LandGraphic = 0;
                            uoiSelectedObject.LandX = 0;
                            uoiSelectedObject.LandY = 0;
                            uoiSelectedObject.LandZ = 0;
                        }
                    }
                    else
                        memset(&uoiSelectedObject, 0, sizeof(uoiSelectedObject));

                    g_PluginManager.WindowProc(
                        g_OrionWindow.Handle, UOMSG_SELECTED_TILE, (WPARAM)&uoiSelectedObject, 0);
                }

                g_Target.UnloadMulti();

                g_GameScreen.RenderListInitalized = false;

                g_MapManager.Init(true);
            }
        }
    }
    else if (rendering)
    {
        g_SelectedObject.Clear();

        if (!g_OrionWindow.IsMinimizedWindow())
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

    if (removeUnusedAnimationTexturesTime < g_Ticks)
    {
        g_AnimationManager.ClearUnusedTextures(g_Ticks);
        removeUnusedAnimationTexturesTime = g_Ticks + CLEAR_ANIMATION_TEXTURES_DELAY;
    }
}
//----------------------------------------------------------------------------------
void COrion::LoadStartupConfig(int serial)
{
    WISPFUN_DEBUG("c194_f15");
    char buf[MAX_PATH] = { 0 };
    CServer *server = g_ServerList.GetSelectedServer();
    if (server != NULL)
        sprintf_s(
            buf,
            "Desktop/%s/%s/0x%08X",
            g_MainScreen.m_Account->c_str(),
            FixServerName(server->Name).c_str(),
            serial);
    else
        sprintf_s(buf, "Desktop/%s/0x%08X", g_MainScreen.m_Account->c_str(), serial);

    auto orionFilesPath{ g_App.ExeFilePath(buf) };
    if (!g_ConfigManager.Load(orionFilesPath + ToPath("/orion_options.cfg")))
    {
        auto uoFilesPath{ g_App.UOFilesPath(buf) };
        if (!g_ConfigManager.Load(uoFilesPath + ToPath("/orion_options.cfg")))
            if (!g_ConfigManager.LoadBin(orionFilesPath + ToPath("/options_debug.cuo")))
                g_ConfigManager.LoadBin(uoFilesPath + ToPath("/options_debug.cuo"));
    }

    g_SoundManager.SetMusicVolume(g_ConfigManager.GetMusicVolume());

    if (!g_ConfigManager.GetSound())
        AdjustSoundEffects(g_Ticks + 100000);

    if (!g_ConfigManager.GetMusic())
        g_SoundManager.StopMusic();
}
//----------------------------------------------------------------------------------
void COrion::LoadPlugin(const os_path &libpath, const string &function, int flags)
{
    LOG("Trying to load %s into memory...\n", CStringFromPath(libpath));
    WISPFUN_DEBUG("c194_f16");
    auto dll = SDL_LoadObject(CStringFromPath(libpath));
    if (dll != nullptr)
    {
        typedef void CDECL dllFunc(PPLUGIN_INTERFACE);

        dllFunc *initFunc = (dllFunc *)SDL_LoadFunction(dll, function.c_str());
        CPlugin *plugin = nullptr;

        if (initFunc != nullptr)
        {
            plugin = new CPlugin(flags);

            initFunc(plugin->m_PPS);
        }

        if (plugin == nullptr)
        {
            SDL_UnloadObject(dll);
        }
        else
        {
            CRASHLOG("Plugin['%s'] loaded at: 0x%08X\n", CStringFromPath(libpath), dll);

            plugin->m_PPS->Owner = plugin;

            if (plugin->CanClientAccess())
                plugin->m_PPS->Client = &g_PluginClientInterface;

            if (plugin->CanParseRecv())
                plugin->m_PPS->Recv = PluginRecvFunction;

            if (plugin->CanParseSend())
                plugin->m_PPS->Send = PluginSendFunction;

            initFunc(plugin->m_PPS);

            g_PluginManager.Add(plugin);
        }
    }
    else
    {
        LOG("Failed to LoadLibrary %s\n", CStringFromPath(libpath));
#if USE_WISP
        LOG("Error code: %i\n", GetLastError());
#else
        LOG("Error code: %s\n", SDL_GetError());
#endif
    }
}
//----------------------------------------------------------------------------------
void COrion::LoadPluginConfig()
{
    WISPFUN_DEBUG("c194_f17");
    g_PluginClientInterface.Version = 2;
    g_PluginClientInterface.Size = sizeof(g_PluginClientInterface);
    g_PluginClientInterface.GL = &g_Interface_GL;
    g_PluginClientInterface.UO = &g_Interface_UO;
    g_PluginClientInterface.ClilocManager = &g_Interface_ClilocManager;
    g_PluginClientInterface.ColorManager = &g_Interface_ColorManager;
    g_PluginClientInterface.PathFinder = &g_Interface_PathFinder;
    g_PluginClientInterface.FileManager = &g_Interface_FileManager;

    ParseCommandLine();

	// FIXME Delete this after sorting out the Module/UOA hotmess
	CPluginPacketSkillsList().SendToPlugin();
    CPluginPacketSpellsList().SendToPlugin();
    CPluginPacketMacrosList().SendToPlugin();

    STRING_LIST libName;
    STRING_LIST functions;
    UINT_LIST flags;

    if (g_PluginInitOld != NULL)
    {
        g_PluginInitOld(libName, functions, flags);
    }
    else
    {
        size_t pluginsInfoCount = GetPluginsCount();
        if (!pluginsInfoCount)
            return;

        PLUGIN_INFO *pluginsInfo = new PLUGIN_INFO[pluginsInfoCount];
        g_PluginInitNew(pluginsInfo);

        IFOR (i, 0, pluginsInfoCount)
        {
            libName.push_back(pluginsInfo[i].FileName);
            functions.push_back(pluginsInfo[i].FunctionName);
            flags.push_back((uint)pluginsInfo[i].Flags);
        }

        delete[] pluginsInfo;
    }

    IFOR (i, 0, (int)libName.size())
        LoadPlugin(g_App.ExeFilePath(libName[i].c_str()), functions[i], flags[i]);

    if (g_PluginManager.m_Items != NULL)
    {
        CPluginPacketSkillsList().SendToPlugin();
        CPluginPacketSpellsList().SendToPlugin();
        CPluginPacketMacrosList().SendToPlugin();

        /*g_FileManager.SendFilesInfo();

		IFOR(i, 0, 0x10000)
		{
			CIndexObjectStatic &staticObj = m_StaticDataIndex[i];

			if (staticObj.Address)
			{
				uint64 compressedSize = 0;

				if (staticObj.UopBlock)
					compressedSize = staticObj.UopBlock->CompressedSize;

				CPluginPacketStaticArtGraphicDataInfo(i, staticObj.Address, staticObj.DataSize, compressedSize).SendToPlugin();
			}
		}

		CPluginPacketFilesTransfered().SendToPlugin();*/
    }

#if USE_WISP
    BringWindowToTop(g_OrionWindow.Handle);
#else
    SDL_RaiseWindow(g_OrionWindow.m_window);
#endif
}
//----------------------------------------------------------------------------------
string COrion::FixServerName(string name)
{
    WISPFUN_DEBUG("c194_f18");
    size_t i = 0;

    while ((i = name.find(":")) != string::npos)
        name.erase(i, 1);

    return name;
}
//----------------------------------------------------------------------------------
void COrion::LoadLocalConfig(int serial)
{
    WISPFUN_DEBUG("c194_f19");
    if (g_ConfigLoaded)
        return;

    g_CheckContainerStackTimer = g_Ticks + 30000;

    char buf[MAX_PATH] = { 0 };
    CServer *server = g_ServerList.GetSelectedServer();
    if (server != NULL)
        sprintf_s(
            buf,
            "Desktop/%s/%s/0x%08X",
            g_MainScreen.m_Account->c_str(),
            FixServerName(server->Name).c_str(),
            serial);
    else
        sprintf_s(buf, "Desktop/%s/0x%08X", g_MainScreen.m_Account->c_str(), serial);

    auto path = g_App.ExeFilePath(buf);

    if (!g_ConfigManager.Load(path + ToPath("/orion_options.cfg")))
    {
        if (!g_ConfigManager.Load(g_App.UOFilesPath("orion_options.cfg")))
        {
            if (!g_ConfigManager.LoadBin(path + ToPath("/options_debug.cuo")))
            {
                if (!g_ConfigManager.LoadBin(g_App.UOFilesPath("options_debug.cuo")))
                {
                    g_ConfigManager.Init();

                    if (g_GameState >= GS_GAME)
                    {
                        SendMessage(g_OrionWindow.Handle, WM_SYSCOMMAND, SC_RESTORE, 0);
                        SendMessage(g_OrionWindow.Handle, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
                    }
                }
            }
        }
    }

    if (!g_SkillGroupManager.Load(path + ToPath("/skills_debug.cuo")))
        g_SkillGroupManager.Load(g_App.UOFilesPath("skills_debug.cuo"));

    if (!g_MacroManager.Load(path + ToPath("/macros_debug.cuo"), path + ToPath("/macros.txt")))
    {
        if (!g_MacroManager.Load(
                g_App.UOFilesPath("/macros_debug.cuo"), g_App.UOFilesPath("/macros.txt")))
        {
        }
    }

    g_GumpManager.Load(path + ToPath("/gumps_debug.cuo"));
    g_CustomHousesManager.Load(path + ToPath("/customhouses_debug.cuo"));

    if (g_ConfigManager.OffsetInterfaceWindows)
        g_ContainerRect.MakeDefault();

    if (g_ConfigManager.GetConsoleNeedEnter())
        g_EntryPointer = NULL;
    else
        g_EntryPointer = &g_GameConsole;

    g_SoundManager.SetMusicVolume(g_ConfigManager.GetMusicVolume());

    if (!g_ConfigManager.GetSound())
        AdjustSoundEffects(g_Ticks + 100000);

    if (!g_ConfigManager.GetMusic())
        g_SoundManager.StopMusic();

    g_ConfigLoaded = true;
}
//----------------------------------------------------------------------------------
void COrion::SaveLocalConfig(int serial)
{
    WISPFUN_DEBUG("c194_f20");
    if (!g_ConfigLoaded)
        return;
    char buf[MAX_PATH] = { 0 };
    auto path = g_App.ExeFilePath("Desktop");

    if (!fs_path_exists(path))
    {
        LOG("%s Does not exist, creating.\n", CStringFromPath(path));
        fs_path_create(path);
    }

    path += PATH_SEP + ToPath(g_MainScreen.m_Account->c_str());

    if (!fs_path_exists(path))
    {
        LOG("%s Does not exist, creating.\n", CStringFromPath(path));
        fs_path_create(path);
    }
    CServer *server = g_ServerList.GetSelectedServer();
    if (server != NULL)
        path += PATH_SEP + ToPath(FixServerName(server->Name.c_str()));
    if (!fs_path_exists(path))
    {
        LOG("%s Does not exist, creating.\n", CStringFromPath(path));
        fs_path_create(path);
    }
    char serbuf[20] = { 0 };
    sprintf_s(serbuf, "/0x%08X", g_PlayerSerial);
    path += ToPath(serbuf);
    if (!fs_path_exists(path))
    {
        LOG("%s Does not exist, creating.\n", CStringFromPath(path));
        fs_path_create(path);
    }
    else
        LOG("SaveLocalConfig using path: %s\n", CStringFromPath(path));

    LOG("managers:saving\n");
    g_ConfigManager.Save(path + ToPath("/orion_options.cfg"));
    g_SkillGroupManager.Save(path + ToPath("/skills_debug.cuo"));
    g_MacroManager.Save(path + ToPath("/macros_debug.cuo"));
    g_GumpManager.Save(path + ToPath("/gumps_debug.cuo"));
    g_CustomHousesManager.Save(path + ToPath("/customhouses_debug.cuo"));

    LOG("managers:saving in to root\n");
    g_ConfigManager.Save(g_App.UOFilesPath("orion_options.cfg"));
    g_MacroManager.Save(g_App.UOFilesPath("macros_debug.cuo"));

    if (g_Player != NULL)
    {
        LOG("player exists\n");
        LOG("name len: %i\n", g_Player->GetName().length());
        path += ToPath("_") + ToPath(g_Player->GetName()) + ToPath(".cuo");

        if (!fs_path_exists(path))
        {
            LOG("file saving\n");
            FILE *file = fs_open(path, FS_WRITE); // "wb"

            LOG("file closing\n");
            if (file != nullptr)
                fs_close(file);
        }
    }
    LOG("SaveLocalConfig end\n");
}
//----------------------------------------------------------------------------------
void COrion::ClearUnusedTextures()
{
    WISPFUN_DEBUG("c194_f21");
    if (g_GameState < GS_GAME)
        return;

    g_MapManager.ClearUnusedBlocks();

    g_GumpManager.PrepareTextures();

    g_Ticks -= CLEAR_TEXTURES_DELAY;

    PVOID lists[5] = {
        &m_UsedLandList, &m_UsedStaticList, &m_UsedGumpList, &m_UsedTextureList, &m_UsedLightList
    };

    int counts[5] = { MAX_ART_OBJECT_REMOVED_BY_GARBAGE_COLLECTOR,
                      MAX_ART_OBJECT_REMOVED_BY_GARBAGE_COLLECTOR,
                      MAX_GUMP_OBJECT_REMOVED_BY_GARBAGE_COLLECTOR,
                      MAX_ART_OBJECT_REMOVED_BY_GARBAGE_COLLECTOR,
                      100 };

    IFOR (i, 0, 5)
    {
        int count = 0;
        deque<CIndexObject *> *list = (deque<CIndexObject *> *)lists[i];
        int &maxCount = counts[i];

        for (deque<CIndexObject *>::iterator it = list->begin(); it != list->end();)
        {
            CIndexObject *obj = *it;

            if (obj->LastAccessTime < g_Ticks)
            {
                if (obj->Texture != NULL)
                {
                    delete obj->Texture;
                    obj->Texture = NULL;
                }

                it = list->erase(it);

                if (++count >= maxCount)
                    break;
            }
            else
                ++it;
        }
    }

    int count = 0;

    for (deque<CIndexSound *>::iterator it = m_UsedSoundList.begin(); it != m_UsedSoundList.end();)
    {
        CIndexSound *obj = *it;

        if (obj->LastAccessTime < g_Ticks)
        {
            if (obj->m_Stream != 0)
            {
                BASS_StreamFree(obj->m_Stream);
                obj->m_Stream = 0;
            }

            it = m_UsedSoundList.erase(it);

            if (++count >= MAX_SOUND_OBJECT_REMOVED_BY_GARBAGE_COLLECTOR)
                break;
        }
        else
            ++it;
    }

    AdjustSoundEffects(g_Ticks);

    g_Ticks += CLEAR_TEXTURES_DELAY;
}
//----------------------------------------------------------------------------------
void COrion::Connect()
{
    WISPFUN_DEBUG("c194_f22");
    InitScreen(GS_MAIN_CONNECT);
    Process(true);

    g_ConnectionManager.Disconnect();
    g_ConnectionManager.Init(); //Configure

    string login = "";
    int port;

    LoadLogin(login, port);

    if (g_ConnectionManager.Connect(login, port, g_GameSeed))
    {
        g_ConnectionScreen.SetConnected(true);
        CPacketFirstLogin().Send();
    }
    else
    {
        g_ConnectionScreen.SetConnectionFailed(true);
        g_ConnectionScreen.SetErrorCode(8);
    }
}
//----------------------------------------------------------------------------------
void COrion::Disconnect()
{
    WISPFUN_DEBUG("c194_f23");
    g_AbyssPacket03First = true;
    g_PluginManager.Disconnect();

    g_ConnectionManager.Disconnect();

    ClearWorld();
}
//----------------------------------------------------------------------------------
int COrion::Send(puchar buf, int size)
{
    WISPFUN_DEBUG("c194_f24");
    uint ticks = g_Ticks;
    g_TotalSendSize += size;

    CPacketInfo &type = g_PacketManager.GetInfo(*buf);

    if (type.save)
    {
#if !defined(ORION_LINUX) // FIXME: localtime_s (use C++ if possible)
        time_t rawtime;
        struct tm timeinfo;
        char buffer[80];

        time(&rawtime);
        localtime_s(&timeinfo, &rawtime);
        strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", &timeinfo);
        LOG("--- ^(%d) s(+%d => %d) %s Client:: %s\n",
            ticks - g_LastPacketTime,
            size,
            g_TotalSendSize,
            buffer,
            type.Name);
#endif

        if (*buf == 0x80 || *buf == 0x91)
        {
            LOG_DUMP(buf, 1);
            LOG("**** ACCOUNT AND PASSWORD CENSORED ****\n");
        }
        else
            LOG_DUMP(buf, size);
    }

    int result = 0;

    if (type.Direction != DIR_SEND && type.Direction != DIR_BOTH)
        LOG("Warning!!! Message direction invalid: 0x%02X\n", *buf);
    else
    {
        if (g_PluginManager.PacketSend(buf, size))
            result = g_ConnectionManager.Send(buf, size);
    }

    if (result)
    {
        g_LastPacketTime = ticks;
        g_LastSendTime = ticks;
    }

    return result;
}
//----------------------------------------------------------------------------------
void COrion::ServerSelection(int pos)
{
    WISPFUN_DEBUG("c194_f25");
    InitScreen(GS_SERVER_CONNECT);
    Process(true);

    CServer *server = g_ServerList.Select(pos);

    if (server != NULL)
    {
        string name = g_ServerList.GetSelectedServer()->Name;
        g_ServerList.LastServerName = name.c_str();

        g_PluginManager.WindowProc(
            g_OrionWindow.Handle, UOMSG_SET_SERVER_NAME, (WPARAM)FixServerName(name).c_str(), 0);

        CPacketSelectServer((uchar)server->Index).Send();
    }
}
//----------------------------------------------------------------------------------
void COrion::RelayServer(const char *ip, int port, puchar gameSeed)
{
    WISPFUN_DEBUG("c194_f26");
    memcpy(&g_GameSeed[0], &gameSeed[0], 4);
    g_ConnectionManager.Init(gameSeed);
    m_GameServerIP = ip;
    memset(&g_GameServerPingInfo, 0, sizeof(g_GameServerPingInfo));

    if (g_ConnectionManager.Connect(ip, port, gameSeed))
    {
        g_ConnectionScreen.SetConnected(true);

        CPacketSecondLogin().Send();
    }
    else
    {
        g_ConnectionScreen.SetConnectionFailed(true);
        g_ConnectionScreen.SetErrorCode(8);
    }
}
//----------------------------------------------------------------------------------
void COrion::CharacterSelection(int pos)
{
    WISPFUN_DEBUG("c194_f27");
    InitScreen(GS_GAME_CONNECT);
    g_ConnectionScreen.SetType(CST_GAME);

    g_CharacterList.LastCharacterName = g_CharacterList.GetName(pos);

    g_PluginManager.WindowProc(
        g_OrionWindow.Handle,
        UOMSG_SET_PLAYER_NAME,
        (WPARAM)g_CharacterList.LastCharacterName.c_str(),
        0);

    CPacketSelectCharacter(pos, g_CharacterList.LastCharacterName).Send();
}
//----------------------------------------------------------------------------------
void COrion::LoginComplete(bool reload)
{
    WISPFUN_DEBUG("c194_f28");
    bool load = reload;

    if (!load && !g_ConnectionScreen.GetCompleted())
    {
        load = true;
        g_ConnectionScreen.SetCompleted(true);

        InitScreen(GS_GAME);
    }

    if (load && g_Player != NULL)
    {
        string title = "Ultima Online - " + g_Player->GetName();

        CServer *server = g_ServerList.GetSelectedServer();

        if (server != NULL)
            title += " (" + server->Name + ")";

        g_OrionWindow.SetTitle(title);

        CPacketSkillsRequest(g_PlayerSerial).Send();
        g_UseItemActions.Add(g_PlayerSerial);

        //CPacketOpenChat(L"").Send();
        //CPacketRazorAnswer().Send();

        if (g_PacketManager.GetClientVersion() >= CV_306E)
            CPacketClientType().Send();

        if (g_PacketManager.GetClientVersion() >= CV_305D)
            CPacketClientViewRange(g_ConfigManager.UpdateRange).Send();

        LoadLocalConfig(g_PacketManager.ConfigSerial);
    }
}
//----------------------------------------------------------------------------------
void COrion::ChangeSeason(const SEASON_TYPE &season, int music)
{
    WISPFUN_DEBUG("c194_f29");

    g_Season = season;

    QFOR(item, g_MapManager.m_Items, CMapBlock *)
    {
        IFOR (x, 0, 8)
        {
            IFOR (y, 0, 8)
            {
                QFOR(obj, item->GetRender((int)x, (int)y), CRenderWorldObject *)
                {
                    obj->UpdateGraphicBySeason();
                }
            }
        }
    }

    CGumpMinimap *gump = (CGumpMinimap *)g_GumpManager.UpdateGump(0, 0, GT_MINIMAP);

    if (gump != NULL)
        gump->LastX = 0;

    if (music)
        g_Orion.PlayMusic(music, true);
}
//----------------------------------------------------------------------------------
ushort COrion::GetLandSeasonGraphic(ushort graphic)
{
    WISPFUN_DEBUG("c194_f30");
    if (g_Season == ST_WINTER)
    {
        ushort buf = m_WinterTile[graphic];

        if (buf != 0)
            graphic = buf;
    }

    return graphic;
}
//----------------------------------------------------------------------------------
ushort COrion::GetSeasonGraphic(ushort graphic)
{
    WISPFUN_DEBUG("c194_f31");
    switch (g_Season)
    {
        case ST_SPRING:
            return GetSpringGraphic(graphic);
        //case ST_SUMMER:
        //	return GetSummerGraphic(graphic);
        case ST_FALL:
            return GetFallGraphic(graphic);
        //case ST_WINTER:
        //	return GetWinterGraphic(graphic);
        case ST_DESOLATION:
            return GetDesolationGraphic(graphic);
        default:
            break;
    }

    return graphic;
}
//----------------------------------------------------------------------------------
ushort COrion::GetSpringGraphic(ushort graphic)
{
    WISPFUN_DEBUG("c194_f32");
    switch (graphic)
    {
        case 0x0CA7:
            graphic = 0x0C84;
            break;
        case 0x0CAC:
            graphic = 0x0C46;
            break;
        case 0x0CAD:
            graphic = 0x0C48;
            break;
        case 0x0CAE:
        case 0x0CB5:
            graphic = 0x0C4A;
            break;
        case 0x0CAF:
            graphic = 0x0C4E;
            break;
        case 0x0CB0:
            graphic = 0x0C4D;
            break;
        case 0x0CB6:
        case 0x0D0D:
        case 0x0D14:
            graphic = 0x0D2B;
            break;
        case 0x0D0C:
            graphic = 0x0D29;
            break;
        case 0x0D0E:
            graphic = 0x0CBE;
            break;
        case 0x0D0F:
            graphic = 0x0CBF;
            break;
        case 0x0D10:
            graphic = 0x0CC0;
            break;
        case 0x0D11:
            graphic = 0x0C87;
            break;
        case 0x0D12:
            graphic = 0x0C38;
            break;
        case 0x0D13:
            graphic = 0x0D2F;
            break;
        default:
            break;
    }

    return graphic;
}
//----------------------------------------------------------------------------------
ushort COrion::GetSummerGraphic(ushort graphic)
{
    WISPFUN_DEBUG("c194_f33");
    return graphic;
}
//----------------------------------------------------------------------------------
ushort COrion::GetFallGraphic(ushort graphic)
{
    WISPFUN_DEBUG("c194_f34");
    switch (graphic)
    {
        case 0x0CD1:
            graphic = 0x0CD2;
            break;
        case 0x0CD4:
            graphic = 0x0CD5;
            break;
        case 0x0CDB:
            graphic = 0x0CDC;
            break;
        case 0x0CDE:
            graphic = 0x0CDF;
            break;
        case 0x0CE1:
            graphic = 0x0CE2;
            break;
        case 0x0CE4:
            graphic = 0x0CE5;
            break;
        case 0x0CE7:
            graphic = 0x0CE8;
            break;
        case 0x0D95:
            graphic = 0x0D97;
            break;
        case 0x0D99:
            graphic = 0x0D9B;
            break;
        case 0x0CCE:
            graphic = 0x0CCF;
            break;
        case 0x0CE9:
        case 0x0C9E:
            graphic = 0x0D3F;
            break;
        case 0x0CEA:
            graphic = 0x0D40;
            break;
        case 0x0C84:
        case 0x0CB0:
            graphic = 0x1B22;
            break;
        case 0x0C8B:
        case 0x0C8C:
        case 0x0C8D:
        case 0x0C8E:
            graphic = 0x0CC6;
            break;
        case 0x0CA7:
            graphic = 0x0C48;
            break;
        case 0x0CAC:
            graphic = 0x1B1F;
            break;
        case 0x0CAD:
            graphic = 0x1B20;
            break;
        case 0x0CAE:
            graphic = 0x1B21;
            break;
        case 0x0CAF:
            graphic = 0x0D0D;
            break;
        case 0x0CB5:
            graphic = 0x0D10;
            break;
        case 0x0CB6:
            graphic = 0x0D2B;
            break;
        case 0x0CC7:
            graphic = 0x0C4E;
            break;
        default:
            break;
    }

    return graphic;
}
//----------------------------------------------------------------------------------
ushort COrion::GetWinterGraphic(ushort graphic)
{
    WISPFUN_DEBUG("c194_f35");
    return graphic;
}
//----------------------------------------------------------------------------------
ushort COrion::GetDesolationGraphic(ushort graphic)
{
    WISPFUN_DEBUG("c194_f36");
    switch (graphic)
    {
        case 0x1B7E:
            graphic = 0x1E34;
            break;
        case 0x0D2B:
            graphic = 0x1B15;
            break;
        case 0x0D11:
        case 0x0D14:
        case 0x0D17:
            graphic = 0x122B;
            break;
        case 0x0D16:
        case 0x0CB9:
        case 0x0CBA:
        case 0x0CBB:
        case 0x0CBC:
        case 0x0CBD:
        case 0x0CBE:
            graphic = 0x1B8D;
            break;
        case 0x0CC7:
            graphic = 0x1B0D;
            break;
        case 0x0CE9:
            graphic = 0x0ED7;
            break;
        case 0x0CEA:
            graphic = 0x0D3F;
            break;
        case 0x0D0F:
            graphic = 0x1B1C;
            break;
        case 0x0CB8:
            graphic = 0x1CEA;
            break;
        case 0x0C84:
        case 0x0C8B:
            graphic = 0x1B84;
            break;
        case 0x0C9E:
            graphic = 0x1182;
            break;
        case 0x0CAD:
            graphic = 0x1AE1;
            break;
        case 0x0C4C:
            graphic = 0x1B16;
            break;
        case 0x0C8E:
        case 0x0C99:
        case 0x0CAC:
            graphic = 0x1B8D;
            break;
        case 0x0C46:
        case 0x0C49:
        case 0x0CB6:
            graphic = 0x1B9D;
            break;
        case 0x0C45:
        case 0x0C48:
        case 0x0C4E:
        case 0x0C85:
        case 0x0CA7:
        case 0x0CAE:
        case 0x0CAF:
        case 0x0CB5:
        case 0x0D15:
        case 0x0D29:
            graphic = 0x1B9C;
            break;
        case 0x0C37:
        case 0x0C38:
        case 0x0C47:
        case 0x0C4A:
        case 0x0C4B:
        case 0x0C4D:
        case 0x0C8C:
        case 0x0C8D:
        case 0x0C93:
        case 0x0C94:
        case 0x0C98:
        case 0x0C9F:
        case 0x0CA0:
        case 0x0CA1:
        case 0x0CA2:
        case 0x0CA3:
        case 0x0CA4:
        case 0x0CB0:
        case 0x0CB1:
        case 0x0CB2:
        case 0x0CB3:
        case 0x0CB4:
        case 0x0CB7:
        case 0x0CC5:
        case 0x0D0C:
        case 0x0D0D:
        case 0x0D0E:
        case 0x0D10:
        case 0x0D12:
        case 0x0D13:
        case 0x0D18:
        case 0x0D19:
        case 0x0D2D:
        case 0x0D2F:
            graphic = 0x1BAE;
            break;
        default:
            break;
    }

    return graphic;
}
//----------------------------------------------------------------------------------
int COrion::ValueInt(const VALUE_KEY_INT &key, int value)
{
    WISPFUN_DEBUG("c194_f37");
    switch (key)
    {
        case VKI_SOUND:
        {
            if (value == -1)
                value = g_ConfigManager.GetSound();
            else
                g_ConfigManager.SetSound(value != 0);

            break;
        }
        case VKI_SOUND_VALUE:
        {
            if (value == -1)
                value = g_ConfigManager.GetSoundVolume();
            else
            {
                if (value < 0)
                    value = 0;
                else if (value > 255)
                    value = 255;

                g_ConfigManager.SetSoundVolume(value);
            }

            break;
        }
        case VKI_MUSIC:
        {
            if (value == -1)
                value = g_ConfigManager.GetMusic();
            else
                g_ConfigManager.SetMusic(value != 0);

            break;
        }
        case VKI_MUSIC_VALUE:
        {
            if (value == -1)
                value = g_ConfigManager.GetMusicVolume();
            else
            {
                if (value < 0)
                    value = 0;
                else if (value > 255)
                    value = 255;

                g_ConfigManager.SetMusicVolume(value);
            }

            break;
        }
        case VKI_USE_TOOLTIPS:
        {
            if (value == -1)
                value = g_ConfigManager.UseToolTips;
            else
                g_ConfigManager.UseToolTips = (value != 0);

            break;
        }
        case VKI_ALWAYS_RUN:
        {
            if (value == -1)
                value = g_ConfigManager.AlwaysRun;
            else
                g_ConfigManager.AlwaysRun = (value != 0);

            break;
        }
        case VKI_NEW_TARGET_SYSTEM:
        {
            if (value == -1)
                value = g_ConfigManager.DisableNewTargetSystem;
            else
                g_ConfigManager.DisableNewTargetSystem =
                    (value == 0); //������ == 0!!! �.�. � ������� ��� Target System enable/disable

            break;
        }
        case VKI_OBJECT_HANDLES:
        {
            if (value == -1)
                value = g_ConfigManager.ObjectHandles;
            else
                g_ConfigManager.ObjectHandles = (value != 0);

            break;
        }
        case VKI_SCALE_SPEECH_DELAY:
        {
            if (value == -1)
                value = g_ConfigManager.ScaleSpeechDelay;
            else
                g_ConfigManager.ScaleSpeechDelay = (value != 0);

            break;
        }
        case VKI_SPEECH_DELAY:
        {
            if (value == -1)
                value = g_ConfigManager.SpeechDelay;
            else
            {
                if (value < 0)
                    value = 0;
                else if (value > 999)
                    value = 999;

                g_ConfigManager.SpeechDelay = value;
            }

            break;
        }
        case VKI_IGNORE_GUILD_MESSAGES:
        {
            if (value == -1)
                value = g_ConfigManager.IgnoreGuildMessage;
            else
                g_ConfigManager.IgnoreGuildMessage = (value != 0);

            break;
        }
        case VKI_IGNORE_ALLIANCE_MESSAGES:
        {
            if (value == -1)
                value = g_ConfigManager.IgnoreAllianceMessage;
            else
                g_ConfigManager.IgnoreAllianceMessage = (value != 0);

            break;
        }
        case VKI_DARK_NIGHTS:
        {
            if (value == -1)
                value = g_ConfigManager.DarkNights;
            else
                g_ConfigManager.DarkNights = (value != 0);

            break;
        }
        case VKI_COLORED_LIGHTING:
        {
            if (value == -1)
                value = g_ConfigManager.ColoredLighting;
            else
                g_ConfigManager.ColoredLighting = (value != 0);

            break;
        }
        case VKI_CRIMINAL_ACTIONS_QUERY:
        {
            if (value == -1)
                value = g_ConfigManager.CriminalActionsQuery;
            else
                g_ConfigManager.CriminalActionsQuery = (value != 0);

            break;
        }
        case VKI_CIRCLETRANS:
        {
            if (value == -1)
                value = g_ConfigManager.UseCircleTrans;
            else
                g_ConfigManager.UseCircleTrans = (value != 0);

            break;
        }
        case VKI_CIRCLETRANS_VALUE:
        {
            if (value == -1)
                value = g_ConfigManager.CircleTransRadius;
            else
            {
                if (value < 0)
                    value = 0;
                else if (value > 255)
                    value = 255;

                g_ConfigManager.CircleTransRadius = value;
            }

            break;
        }
        case VKI_LOCK_RESIZING_GAME_WINDOW:
        {
            if (value == -1)
                value = g_ConfigManager.LockResizingGameWindow;
            else
                g_ConfigManager.LockResizingGameWindow = (value != 0);

            break;
        }
        case VKI_CLIENT_FPS_VALUE:
        {
            if (value == -1)
                value = g_ConfigManager.GetClientFPS();
            else
            {
                if (value < MIN_FPS_LIMIT)
                    value = MIN_FPS_LIMIT;
                else if (value > MAX_FPS_LIMIT)
                    value = MAX_FPS_LIMIT;

                g_ConfigManager.SetClientFPS(value);
            }

            break;
        }
        case VKI_USE_SCALING_GAME_WINDOW:
        {
            if (value == -1)
                value = g_ConfigManager.GetUseScaling();
            else
                g_ConfigManager.SetUseScaling((value != 0));

            break;
        }
        case VKI_DRAW_STATUS_STATE:
        {
            if (value == -1)
                value = g_ConfigManager.GetDrawStatusState();
            else
            {
                if (value < 0)
                    value = 0;
                else if (value > DCSS_UNDER)
                    value = DCSS_UNDER;

                g_ConfigManager.SetDrawStatusState(value);
            }

            break;
        }
        case VKI_DRAW_STUMPS:
        {
            if (value == -1)
                value = g_ConfigManager.GetDrawStumps();
            else
                g_ConfigManager.SetDrawStumps((value != 0));

            break;
        }
        case VKI_MARKING_CAVES:
        {
            if (value == -1)
                value = g_ConfigManager.GetMarkingCaves();
            else
                g_ConfigManager.SetMarkingCaves((value != 0));

            break;
        }
        case VKI_NO_VEGETATION:
        {
            if (value == -1)
                value = g_ConfigManager.GetNoVegetation();
            else
                g_ConfigManager.SetNoVegetation((value != 0));

            break;
        }
        case VKI_NO_ANIMATE_FIELDS:
        {
            if (value == -1)
                value = g_ConfigManager.GetNoAnimateFields();
            else
                g_ConfigManager.SetNoAnimateFields((value != 0));

            break;
        }
        case VKI_STANDARD_CHARACTERS_DELAY:
        {
            if (value == -1)
                value = g_ConfigManager.StandartCharactersAnimationDelay;
            else
                g_ConfigManager.StandartCharactersAnimationDelay = (value != 0);

            break;
        }
        case VKI_STANDARD_ITEMS_DELAY:
        {
            if (value == -1)
                value = g_ConfigManager.StandartItemsAnimationDelay;
            else
                g_ConfigManager.StandartItemsAnimationDelay = (value != 0);

            break;
        }
        case VKI_LOCK_GUMPS_MOVING:
        {
            if (value == -1)
                value = g_ConfigManager.LockGumpsMoving;
            else
                g_ConfigManager.LockGumpsMoving = (value != 0);

            break;
        }
        case VKI_CONSOLE_NEED_ENTER:
        {
            if (value == -1)
                value = g_ConfigManager.GetConsoleNeedEnter();
            else
                g_ConfigManager.SetConsoleNeedEnter(value != 0);

            break;
        }
        case VKI_HIDDEN_CHARACTERS_MODE:
        {
            if (value == -1)
                value = g_ConfigManager.HiddenCharactersRenderMode;
            else
            {
                if (value < 0)
                    value = 0;
                else if (value > HCRM_SPECIAL_SPECTRAL_COLOR)
                    value = HCRM_SPECIAL_SPECTRAL_COLOR;

                g_ConfigManager.HiddenCharactersRenderMode = value;
            }

            break;
        }
        case VKI_HIDDEN_CHARACTERS_ALPHA:
        {
            if (value == -1)
                value = g_ConfigManager.HiddenAlpha;
            else
            {
                if (value < 20)
                    value = 20;
                else if (value > 255)
                    value = 255;

                g_ConfigManager.HiddenAlpha = value;
            }

            break;
        }
        case VKI_HIDDEN_CHARACTERS_MODE_ONLY_FOR_SELF:
        {
            if (value == -1)
                value = g_ConfigManager.UseHiddenModeOnlyForSelf;
            else
                g_ConfigManager.UseHiddenModeOnlyForSelf = (value != 0);

            break;
        }
        case VKI_TRANSPARENT_SPELL_ICONS:
        {
            if (value == -1)
                value = g_ConfigManager.TransparentSpellIcons;
            else
                g_ConfigManager.TransparentSpellIcons = (value != 0);

            break;
        }
        case VKI_SPELL_ICONS_ALPHA:
        {
            if (value == -1)
                value = g_ConfigManager.GetSpellIconAlpha();
            else
            {
                if (value < 30)
                    value = 30;
                else if (value > 255)
                    value = 255;

                g_ConfigManager.SetSpellIconAlpha(value);
            }

            break;
        }
        case VKI_SKILLS_COUNT:
        {
            value = g_SkillsManager.Count;
            break;
        }
        case VKI_SKILL_CAN_BE_USED:
        {
            CSkill *skill = g_SkillsManager.Get(value);

            if (skill != NULL)
                value = skill->Button;

            break;
        }
        case VKI_STATIC_ART_ADDRESS:
        {
            if (value >= 0 && value < (int)m_StaticData.size())
                value = (int)m_StaticDataIndex[value].Address;

            break;
        }
        case VKI_USED_LAYER:
        {
            if (value >= 0 && value < (int)m_StaticData.size())
                value = m_StaticData[value].Layer;

            break;
        }
        case VKI_SPELLBOOK_COUNT:
        {
            if (value >= 0 && value < 7)
            {
                switch (value)
                {
                    case 1:
                    {
                        value = CGumpSpellbook::SPELLBOOK_1_SPELLS_COUNT;
                        break;
                    }
                    case 2:
                    {
                        value = CGumpSpellbook::SPELLBOOK_2_SPELLS_COUNT;
                        break;
                    }
                    case 3:
                    {
                        value = CGumpSpellbook::SPELLBOOK_3_SPELLS_COUNT;
                        break;
                    }
                    case 4:
                    {
                        value = CGumpSpellbook::SPELLBOOK_4_SPELLS_COUNT;
                        break;
                    }
                    case 5:
                    {
                        value = CGumpSpellbook::SPELLBOOK_5_SPELLS_COUNT;
                        break;
                    }
                    case 6:
                    {
                        value = CGumpSpellbook::SPELLBOOK_6_SPELLS_COUNT;
                        break;
                    }
                    case 7:
                    {
                        value = CGumpSpellbook::SPELLBOOK_7_SPELLS_COUNT;
                        break;
                    }
                    default:
                        break;
                }
            }

            break;
        }
        case VKI_BLOCK_MOVING:
        {
            g_PathFinder.BlockMoving = (value != 0);

            break;
        }
        case VKI_SET_PLAYER_GRAPHIC:
        {
            if (g_Player != NULL && g_Player->Graphic != value)
            {
                g_Player->Graphic = value;
                g_Player->OnGraphicChange(1000);
            }

            break;
        }
        case VKI_FAST_ROTATION:
        {
            g_PathFinder.FastRotation = (value != 0);

            break;
        }
        case VKI_IGNORE_STAMINA_CHECK:
        {
            g_PathFinder.IgnoreStaminaCheck = (value != 0);

            break;
        }
        case VKI_LAST_TARGET:
        {
            if (value == -1)
                value = g_LastTargetObject;
            else
            {
                g_LastTargetObject = value;
                g_Target.SetLastTargetObject(g_LastTargetObject);
            }

            break;
        }
        case VKI_LAST_ATTACK:
        {
            if (value == -1)
                value = g_LastAttackObject;
            else
                g_LastAttackObject = value;

            break;
        }
        case VKI_NEW_TARGET_SYSTEM_SERIAL:
        {
            if (value == -1)
                value = g_NewTargetSystem.Serial;
            else
                g_NewTargetSystem.Serial = value;

            break;
        }
        case VKI_GET_MAP_SIZE:
        {
            if (value >= 0 && value < 6)
                value = (g_MapSize[value].Width << 16) | g_MapSize[value].Height;

            break;
        }
        case VKI_GET_MAP_BLOCK_SIZE:
        {
            if (value >= 0 && value < 6)
                value = (g_MapBlockSize[value].Width << 16) | g_MapBlockSize[value].Height;

            break;
        }
        case VKI_MAP_MUL_ADDRESS:
        {
            if (value >= 0 && value < 6)
                value = (int)g_FileManager.m_MapMul[value].Start;

            break;
        }
        case VKI_MAP_MUL_SIZE:
        {
            if (value >= 0 && value < 6)
                value = (int)g_FileManager.m_MapMul[value].Size;

            break;
        }
        case VKI_STATIC_IDX_ADDRESS:
        {
            if (value >= 0 && value < 6)
                value = (int)g_FileManager.m_StaticIdx[value].Start;

            break;
        }
        case VKI_STATIC_IDX_SIZE:
        {
            if (value >= 0 && value < 6)
                value = (int)g_FileManager.m_StaticIdx[value].Size;

            break;
        }
        case VKI_STATIC_MUL_ADDRESS:
        {
            if (value >= 0 && value < 6)
                value = (int)g_FileManager.m_StaticMul[value].Start;

            break;
        }
        case VKI_STATIC_MUL_SIZE:
        {
            if (value >= 0 && value < 6)
                value = (int)g_FileManager.m_StaticMul[value].Size;

            break;
        }
        case VKI_MAP_DIFL_ADDRESS:
        {
            if (value >= 0 && value < 6)
                value = (int)g_FileManager.m_MapDifl[value].Start;

            break;
        }
        case VKI_MAP_DIFL_SIZE:
        {
            if (value >= 0 && value < 6)
                value = (int)g_FileManager.m_MapDifl[value].Size;

            break;
        }
        case VKI_MAP_DIF_ADDRESS:
        {
            if (value >= 0 && value < 6)
                value = (int)g_FileManager.m_MapDif[value].Start;

            break;
        }
        case VKI_MAP_DIF_SIZE:
        {
            if (value >= 0 && value < 6)
                value = (int)g_FileManager.m_MapDif[value].Size;

            break;
        }
        case VKI_STATIC_DIFL_ADDRESS:
        {
            if (value >= 0 && value < 6)
                value = (int)g_FileManager.m_StaDifl[value].Start;

            break;
        }
        case VKI_STATIC_DIFL_SIZE:
        {
            if (value >= 0 && value < 6)
                value = (int)g_FileManager.m_StaDifl[value].Size;

            break;
        }
        case VKI_STATIC_DIFI_ADDRESS:
        {
            if (value >= 0 && value < 6)
                value = (int)g_FileManager.m_StaDifi[value].Start;

            break;
        }
        case VKI_STATIC_DIFI_SIZE:
        {
            if (value >= 0 && value < 6)
                value = (int)g_FileManager.m_StaDifi[value].Size;

            break;
        }
        case VKI_STATIC_DIF_ADDRESS:
        {
            if (value >= 0 && value < 6)
                value = (int)g_FileManager.m_StaDif[value].Start;

            break;
        }
        case VKI_STATIC_DIF_SIZE:
        {
            if (value >= 0 && value < 6)
                value = (int)g_FileManager.m_StaDif[value].Size;

            break;
        }
        case VKI_VERDATA_ADDRESS:
        {
            value = (int)g_FileManager.m_VerdataMul.Start;

            break;
        }
        case VKI_VERDATA_SIZE:
        {
            value = (int)g_FileManager.m_VerdataMul.Size;

            break;
        }
        case VKI_MAP_UOP_ADDRESS:
        {
            if (value >= 0 && value < 6)
                value = (int)g_FileManager.m_MapUOP[value].Start;

            break;
        }
        case VKI_MAP_UOP_SIZE:
        {
            if (value >= 0 && value < 6)
                value = (int)g_FileManager.m_MapUOP[value].Size;

            break;
        }
        case VKI_MAP_X_UOP_ADDRESS:
        {
            if (value >= 0 && value < 6)
                value = (int)g_FileManager.m_MapXUOP[value].Start;

            break;
        }
        case VKI_MAP_X_UOP_SIZE:
        {
            if (value >= 0 && value < 6)
                value = (int)g_FileManager.m_MapXUOP[value].Size;

            break;
        }
        case VKI_CLILOC_ENU_ADDRESS:
        {
            value = (int)g_ClilocManager.Cliloc("enu")->m_File.Start;

            break;
        }
        case VKI_CLILOC_ENU_SIZE:
        {
            value = (int)g_ClilocManager.Cliloc("enu")->m_File.Size;

            break;
        }
        case VKI_GUMP_ART_ADDRESS:
        {
            if (value >= 0 && value < MAX_GUMP_DATA_INDEX_COUNT)
                value = (int)m_GumpDataIndex[value].Address;

            break;
        }
        case VKI_VIEW_RANGE:
        {
            value = g_ConfigManager.UpdateRange;

            break;
        }
        case VKI_SET_PVPCALLER:
        {
            CGameCharacter *obj = g_World->FindWorldCharacter(value);
            if (obj == NULL)
                break;

            if (!obj->pvpCaller)
                obj->pvpCaller = true;
            else
                obj->pvpCaller = false;

            break;
        }
        default:
            break;
    }

    return value;
}
//----------------------------------------------------------------------------------
string COrion::ValueString(const VALUE_KEY_STRING &key, string value)
{
    WISPFUN_DEBUG("c194_f38");
    switch (key)
    {
        case VKS_SKILL_NAME:
        {
            int index = atoi(value.c_str());

            CSkill *skill = g_SkillsManager.Get(index);

            if (skill != NULL)
                value = skill->Name;

            break;
        }
        case VKS_SERVER_NAME:
        {
            CServer *server = g_ServerList.GetSelectedServer();

            if (server != NULL)
                value = server->Name;

            break;
        }
        case VKS_CHARACTER_NAME:
        {
            if (g_Player != NULL)
                value = g_Player->GetName();

            break;
        }
        case VKS_SPELLBOOK_1_SPELL_NAME:
        {
            int index = atoi(value.c_str());

            if (index >= 0 && index < CGumpSpellbook::SPELLBOOK_1_SPELLS_COUNT)
                value = CGumpSpellbook::m_SpellName1[index][0];

            break;
        }
        case VKS_SPELLBOOK_2_SPELL_NAME:
        {
            int index = atoi(value.c_str());

            if (index >= 0 && index < CGumpSpellbook::SPELLBOOK_2_SPELLS_COUNT)
                value = CGumpSpellbook::m_SpellName2[index][0];

            break;
        }
        case VKS_SPELLBOOK_3_SPELL_NAME:
        {
            int index = atoi(value.c_str());

            if (index >= 0 && index < CGumpSpellbook::SPELLBOOK_3_SPELLS_COUNT)
                value = CGumpSpellbook::m_SpellName3[index][0];

            break;
        }
        case VKS_SPELLBOOK_4_SPELL_NAME:
        {
            int index = atoi(value.c_str());

            if (index >= 0 && index < CGumpSpellbook::SPELLBOOK_4_SPELLS_COUNT)
                value = CGumpSpellbook::m_SpellName4[index];

            break;
        }
        case VKS_SPELLBOOK_5_SPELL_NAME:
        {
            int index = atoi(value.c_str());

            if (index >= 0 && index < CGumpSpellbook::SPELLBOOK_5_SPELLS_COUNT)
                value = CGumpSpellbook::m_SpellName5[index];

            break;
        }
        case VKS_SPELLBOOK_6_SPELL_NAME:
        {
            int index = atoi(value.c_str());

            if (index >= 0 && index < CGumpSpellbook::SPELLBOOK_6_SPELLS_COUNT)
                value = CGumpSpellbook::m_SpellName6[index][0];

            break;
        }
        case VKS_SPELLBOOK_7_SPELL_NAME:
        {
            int index = atoi(value.c_str());

            if (index >= 0 && index < CGumpSpellbook::SPELLBOOK_7_SPELLS_COUNT)
                value = CGumpSpellbook::m_SpellName7[index][0];

            break;
        }
        default:
            break;
    }

    return value;
}
//----------------------------------------------------------------------------------
void COrion::ClearRemovedStaticsTextures()
{
    WISPFUN_DEBUG("c194_f39");
    for (deque<CIndexObject *>::iterator it = m_UsedStaticList.begin();
         it != m_UsedStaticList.end();)
    {
        CIndexObject *obj = *it;

        if (!obj->LastAccessTime)
        {
            if (obj->Texture != NULL)
            {
                delete obj->Texture;
                obj->Texture = NULL;
            }

            it = m_UsedStaticList.erase(it);
        }
        else
            ++it;
    }
}
//----------------------------------------------------------------------------------
void COrion::ClearTreesTextures()
{
    WISPFUN_DEBUG("c194_f40");
    for (ushort graphic : m_StumpTiles)
        m_StaticDataIndex[graphic].LastAccessTime = 0;

    ClearRemovedStaticsTextures();
}
//----------------------------------------------------------------------------------
bool COrion::InTileFilter(ushort graphic)
{
    if (!m_IgnoreInFilterTiles.empty())
    {
        for (const std::pair<ushort, ushort> &i : m_IgnoreInFilterTiles)
        {
            if (i.first == graphic || (i.second && IN_RANGE(graphic, i.first, i.second)))
                return true;
        }
    }

    return false;
}
//----------------------------------------------------------------------------------
bool COrion::IsTreeTile(ushort graphic, int &index)
{
    WISPFUN_DEBUG("c194_f41");
    if (!g_ConfigManager.GetDrawStumps() || InTileFilter(graphic))
        return false;

    uchar flags = m_StaticTilesFilterFlags[graphic];

    if (flags & STFF_STUMP)
    {
        if (flags & STFF_STUMP_HATCHED)
            index = g_StumpHatchedID;
        else
            index = g_StumpID;

        return true;
    }

    return false;
}
//----------------------------------------------------------------------------------
void COrion::ClearCaveTextures()
{
    WISPFUN_DEBUG("c194_f42");
    for (ushort graphic : m_CaveTiles)
        m_StaticDataIndex[graphic].LastAccessTime = 0;

    ClearRemovedStaticsTextures();
}
//----------------------------------------------------------------------------------
bool COrion::IsCaveTile(ushort graphic)
{
    WISPFUN_DEBUG("c194_f43");
    return (g_ConfigManager.GetMarkingCaves() && (m_StaticTilesFilterFlags[graphic] & STFF_CAVE));
}
//----------------------------------------------------------------------------------
bool COrion::IsVegetation(ushort graphic)
{
    WISPFUN_DEBUG("c194_f44");
    return (m_StaticTilesFilterFlags[graphic] & STFF_VEGETATION);
}
//----------------------------------------------------------------------------------
void COrion::LoadLogin(string &login, int &port)
{
    WISPFUN_DEBUG("c194_f45");
    if (DefaultPort)
    {
        login = DefaultLogin;
        port = DefaultPort;

        return;
    }

    WISP_FILE::CTextFileParser file(g_App.UOFilesPath("login.cfg"), "=,", "#;", "");

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
    WISPFUN_DEBUG("c194_f46");
    if (url.length())
    {
        std::size_t found = url.find("http://");
        if (found == std::string::npos)
            found = url.find("https://");
        const string header = "http://";
        if (found != std::string::npos)
            ShellExecuteA(0, "Open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
        else
            ShellExecuteA(0, "Open", (header + url).c_str(), NULL, NULL, SW_SHOWNORMAL);
    }
}
//----------------------------------------------------------------------------------
void COrion::LoadTiledata(int landSize, int staticsSize)
{
    WISPFUN_DEBUG("c194_f47");
    WISP_FILE::CMappedFile &file = g_FileManager.m_TiledataMul;

    if (file.Size)
    {
        bool isOldVersion = (g_PacketManager.GetClientVersion() < CV_7090);
        file.ResetPtr();

        m_LandData.resize(landSize * 32);

        m_StaticData.resize(staticsSize * 32);

        IFOR (i, 0, landSize)
        {
            file.ReadUInt32LE();

            IFOR (j, 0, 32)
            {
                LAND_TILES &tile = m_LandData[(i * 32) + j];

                if (isOldVersion)
                    tile.Flags = file.ReadUInt32LE();
                else
                    tile.Flags = file.ReadInt64LE();

                tile.TexID = file.ReadUInt16LE();
                tile.Name = file.ReadString(20);
            }
        }

        IFOR (i, 0, staticsSize)
        {
            file.ReadUInt32LE();

            IFOR (j, 0, 32)
            {
                STATIC_TILES &tile = m_StaticData[(i * 32) + j];

                if (isOldVersion)
                    tile.Flags = file.ReadUInt32LE();
                else
                    tile.Flags = file.ReadInt64LE();

                tile.Weight = file.ReadInt8();
                tile.Layer = file.ReadInt8();
                tile.Count = file.ReadInt32LE();
                tile.AnimID = file.ReadInt16LE();
                tile.Hue = file.ReadInt16LE();
                tile.LightIndex = file.ReadInt16LE();
                tile.Height = file.ReadInt8();
                tile.Name = file.ReadString(20);
            }
        }

        file.Unload();
    }
}
//----------------------------------------------------------------------------------
void COrion::ReadMulIndexFile(
    size_t indexMaxCount,
    std::function<CIndexObject *(int index)> getIdxObj,
    size_t address,
    PBASE_IDX_BLOCK ptr,
    std::function<PBASE_IDX_BLOCK()> getNewPtrValue)
{
    IFOR (i, 0, indexMaxCount)
    {
        CIndexObject *obj = getIdxObj((int)i);
        obj->ReadIndexFile(address, ptr, (ushort)i);
        ptr = getNewPtrValue();
    }
}
//----------------------------------------------------------------------------------
void COrion::ReadUOPIndexFile(
    size_t indexMaxCount,
    std::function<CIndexObject *(int)> getIdxObj,
    const char *uopFileName,
    int padding,
    const char *extesion,
    CUopMappedFile &uopFile,
    int startIndex)
{
    string p = uopFileName;
    std::transform(p.begin(), p.end(), p.begin(), ::tolower);

    bool isGump = (string("gumpartlegacymul") == p);
    char basePath[200] = { 0 };
    sprintf_s(basePath, "build/%s/%%0%ii%s", p.c_str(), padding, extesion);

    IFOR (i, startIndex, indexMaxCount)
    {
        char hashString[200] = { 0 };
        sprintf_s(hashString, basePath, (int)i);

        CUopBlockHeader *block = uopFile.GetBlock(CreateHash(hashString));

        if (block != NULL)
        {
            CIndexObject *obj = getIdxObj((int)i);
            obj->Address = (uintptr_t)uopFile.Start + (uint)block->Offset;
            obj->DataSize = block->DecompressedSize;
            obj->UopBlock = block;
            obj->ID = -1;

            if (isGump)
            {
                obj->Address += 8;
                obj->DataSize -= 8;

                uopFile.ResetPtr();
                uopFile.Move((int)block->Offset);

                obj->Width = uopFile.ReadUInt32LE();
                obj->Height = uopFile.ReadUInt32LE();
            }
        }
    }
}
//----------------------------------------------------------------------------------
uint64_t COrion::CreateHash(const char *s)
{
    const auto l = (int)strlen(s);
    uint32_t eax, ecx, edx, ebx, esi, edi;

    eax = ecx = edx = ebx = esi = edi = 0;
    ebx = edi = esi = l + 0xDEADBEEF;

    uint32_t i = 0;
    for (i = 0; i + 12 < l; i += 12)
    {
        edi = (uint32_t)((s[i + 7] << 24) | (s[i + 6] << 16) | (s[i + 5] << 8) | s[i + 4]) + edi;
        esi = (uint32_t)((s[i + 11] << 24) | (s[i + 10] << 16) | (s[i + 9] << 8) | s[i + 8]) + esi;
        edx = (uint32_t)((s[i + 3] << 24) | (s[i + 2] << 16) | (s[i + 1] << 8) | s[i]) - esi;

        edx = (edx + ebx) ^ (esi >> 28) ^ (esi << 4);
        esi += edi;
        edi = (edi - edx) ^ (edx >> 26) ^ (edx << 6);
        edx += esi;
        esi = (esi - edi) ^ (edi >> 24) ^ (edi << 8);
        edi += edx;
        ebx = (edx - esi) ^ (esi >> 16) ^ (esi << 16);
        esi += edi;
        edi = (edi - ebx) ^ (ebx >> 13) ^ (ebx << 19);
        ebx += esi;
        esi = (esi - edi) ^ (edi >> 28) ^ (edi << 4);
        edi += ebx;
    }

    if (l - i > 0)
    {
        switch (l - i)
        {
            case 12:
                esi += static_cast<uint32_t>(s[i + 11]) << 24;
                goto case_11;
                break;
            case 11:
            case_11:
                esi += static_cast<uint32_t>(s[i + 10]) << 16;
                goto case_10;
                break;
            case 10:
            case_10:
                esi += static_cast<uint32_t>(s[i + 9]) << 8;
                goto case_9;
                break;
            case 9:
            case_9:
                esi += s[i + 8];
                goto case_8;
                break;
            case 8:
            case_8:
                edi += static_cast<uint32_t>(s[i + 7]) << 24;
                goto case_7;
                break;
            case 7:
            case_7:
                edi += static_cast<uint32_t>(s[i + 6]) << 16;
                goto case_6;
                break;
            case 6:
            case_6:
                edi += static_cast<uint32_t>(s[i + 5]) << 8;
                goto case_5;
                break;
            case 5:
            case_5:
                edi += s[i + 4];
                goto case_4;
                break;
            case 4:
            case_4:
                ebx += static_cast<uint32_t>(s[i + 3]) << 24;
                goto case_3;
                break;
            case 3:
            case_3:
                ebx += static_cast<uint32_t>(s[i + 2]) << 16;
                goto case_2;
                break;
            case 2:
            case_2:
                ebx += static_cast<uint32_t>(s[i + 1]) << 8;
                goto case_1;
            case 1:
            case_1:
                ebx += s[i];
                break;
        }

        esi = (esi ^ edi) - ((edi >> 18) ^ (edi << 14));
        ecx = (esi ^ ebx) - ((esi >> 21) ^ (esi << 11));
        edi = (edi ^ ecx) - ((ecx >> 7) ^ (ecx << 25));
        esi = (esi ^ edi) - ((edi >> 16) ^ (edi << 16));
        edx = (esi ^ ecx) - ((esi >> 28) ^ (esi << 4));
        edi = (edi ^ edx) - ((edx >> 18) ^ (edx << 14));
        eax = (esi ^ edi) - ((edi >> 8) ^ (edi << 24));

        return (static_cast<uint64_t>(edi) << 32) | eax;
    }

    return (static_cast<uint64_t>(esi) << 32) | eax;
}
//----------------------------------------------------------------------------------
void COrion::LoadIndexFiles()
{
    PART_IDX_BLOCK LandArtPtr = (PART_IDX_BLOCK)g_FileManager.m_ArtIdx.Start;
    PART_IDX_BLOCK StaticArtPtr = (PART_IDX_BLOCK)(
        (size_t)g_FileManager.m_ArtIdx.Start + (m_LandData.size() * sizeof(ART_IDX_BLOCK)));
    PGUMP_IDX_BLOCK GumpArtPtr = (PGUMP_IDX_BLOCK)g_FileManager.m_GumpIdx.Start;
    PTEXTURE_IDX_BLOCK TexturePtr = (PTEXTURE_IDX_BLOCK)g_FileManager.m_TextureIdx.Start;
    PMULTI_IDX_BLOCK MultiPtr = (PMULTI_IDX_BLOCK)g_FileManager.m_MultiIdx.Start;
    PSOUND_IDX_BLOCK SoundPtr = (PSOUND_IDX_BLOCK)g_FileManager.m_SoundIdx.Start;
    PLIGHT_IDX_BLOCK LightPtr = (PLIGHT_IDX_BLOCK)g_FileManager.m_LightIdx.Start;

    if (g_FileManager.m_MultiCollection.Start != NULL)
        g_MultiIndexCount = MAX_MULTI_DATA_INDEX_COUNT;
    else
    {
        g_MultiIndexCount = (int)(g_FileManager.m_MultiIdx.Size / sizeof(MULTI_IDX_BLOCK));

        if (g_MultiIndexCount > MAX_MULTI_DATA_INDEX_COUNT)
            g_MultiIndexCount = MAX_MULTI_DATA_INDEX_COUNT;
    }

    int maxGumpsCount = (int)(g_FileManager.m_GumpIdx.Start == nullptr ? MAX_GUMP_DATA_INDEX_COUNT : (g_FileManager.m_GumpIdx.End - g_FileManager.m_GumpIdx.Start) / sizeof(GUMP_IDX_BLOCK));

    if (g_FileManager.m_ArtMul.Start != nullptr)
    {
        ReadMulIndexFile(
            MAX_LAND_DATA_INDEX_COUNT,
            [&](int i) { return &m_LandDataIndex[i]; },
            (size_t)g_FileManager.m_ArtMul.Start,
            LandArtPtr,
            [&LandArtPtr]() { return ++LandArtPtr; });
        ReadMulIndexFile(
            m_StaticData.size(),
            [&](int i) { return &m_StaticDataIndex[i]; },
            (size_t)g_FileManager.m_ArtMul.Start,
            StaticArtPtr,
            [&StaticArtPtr]() { return ++StaticArtPtr; });
    }
    else
    {
        ReadUOPIndexFile(
            MAX_LAND_DATA_INDEX_COUNT,
            [&](int i) { return &m_LandDataIndex[i]; },
            "artLegacyMUL",
            8,
            ".tga",
            g_FileManager.m_ArtLegacyMUL);
        ReadUOPIndexFile(
            m_StaticData.size() + MAX_LAND_DATA_INDEX_COUNT,
            [&](int i) { return &m_StaticDataIndex[i - MAX_LAND_DATA_INDEX_COUNT]; },
            "artLegacyMUL",
            8,
            ".tga",
            g_FileManager.m_ArtLegacyMUL,
            MAX_LAND_DATA_INDEX_COUNT);
    }

    if (g_FileManager.m_SoundMul.Start != nullptr)
        ReadMulIndexFile(
            MAX_SOUND_DATA_INDEX_COUNT,
            [&](int i) { return &m_SoundDataIndex[i]; },
            (size_t)g_FileManager.m_SoundMul.Start,
            SoundPtr,
            [&SoundPtr]() { return ++SoundPtr; });
    else
        ReadUOPIndexFile(
            MAX_SOUND_DATA_INDEX_COUNT,
            [&](int i) { return &m_SoundDataIndex[i]; },
            "soundLegacyMUL",
            8,
            ".dat",
            g_FileManager.m_SoundLegacyMUL);

    if (g_FileManager.m_GumpMul.Start != nullptr)
        ReadMulIndexFile(
            maxGumpsCount,
            [&](int i) { return &m_GumpDataIndex[i]; },
            (size_t)g_FileManager.m_GumpMul.Start,
            GumpArtPtr,
            [&GumpArtPtr]() { return ++GumpArtPtr; });
    else
        ReadUOPIndexFile(
            maxGumpsCount,
            [&](int i) { return &m_GumpDataIndex[i]; },
            "gumpartLegacyMUL",
            8,
            ".tga",
            g_FileManager.m_GumpartLegacyMUL);

    ReadMulIndexFile(
        g_FileManager.m_TextureIdx.Size / sizeof(TEXTURE_IDX_BLOCK),
        [&](int i) { return &m_TextureDataIndex[i]; },
        (size_t)g_FileManager.m_TextureMul.Start,
        TexturePtr,
        [&TexturePtr]() { return ++TexturePtr; });
    ReadMulIndexFile(
        MAX_LIGHTS_DATA_INDEX_COUNT,
        [&](int i) { return &m_LightDataIndex[i]; },
        (size_t)g_FileManager.m_LightMul.Start,
        LightPtr,
        [&LightPtr]() { return ++LightPtr; });

    if (g_FileManager.m_MultiMul.Start != nullptr)
        ReadMulIndexFile(
            g_MultiIndexCount,
            [&](int i) { return &m_MultiDataIndex[i]; },
            (size_t)g_FileManager.m_MultiMul.Start,
            MultiPtr,
            [&MultiPtr]() { return ++MultiPtr; });
    else
    {
        CUopMappedFile &file = g_FileManager.m_MultiCollection;

        for (std::unordered_map<uint64, CUopBlockHeader>::iterator i = file.m_Map.begin();
             i != file.m_Map.end();
             ++i)
        {
            CUopBlockHeader &block = i->second;
            UCHAR_LIST data = file.GetData(block);

            if (data.empty())
                continue;

            WISP_DATASTREAM::CDataReader reader(&data[0], data.size());

            uint id = reader.ReadUInt32LE();

            if (id < MAX_MULTI_DATA_INDEX_COUNT)
            {
                CIndexMulti &index = m_MultiDataIndex[id];

                index.Address = (size_t)file.Start + (size_t)block.Offset;
                index.DataSize = block.DecompressedSize;
                index.UopBlock = &i->second;
                index.ID = -1;
                index.Count = reader.ReadUInt32LE();
            }
        }

        //ReadUOPIndexFile(g_MultiIndexCount, [&](int i){ return &m_MultiDataIndex[i]; }, "MultiCollection", 6, ".bin", g_FileManager.m_MultiCollection);
    }
}
//----------------------------------------------------------------------------------
void COrion::UnloadIndexFiles()
{
    WISPFUN_DEBUG("c194_f50");
    deque<CIndexObject *> *lists[5] = {
        &m_UsedLandList, &m_UsedStaticList, &m_UsedGumpList, &m_UsedTextureList, &m_UsedLightList
    };

    IFOR (i, 0, 5)
    {
        deque<CIndexObject *> &list = *lists[i];

        for (deque<CIndexObject *>::iterator it = list.begin(); it != list.end(); ++it)
        {
            CIndexObject *obj = *it;

            if (obj->Texture != NULL)
            {
                delete obj->Texture;
                obj->Texture = NULL;
            }
        }

        list.clear();
    }

    for (deque<CIndexSound *>::iterator it = m_UsedSoundList.begin(); it != m_UsedSoundList.end();
         ++it)
    {
        CIndexSound *obj = *it;

        if (obj->m_Stream != 0)
        {
            BASS_StreamFree(obj->m_Stream);
            obj->m_Stream = 0;
        }
    }

    m_UsedSoundList.clear();
}
//----------------------------------------------------------------------------------
void COrion::InitStaticAnimList()
{
    WISPFUN_DEBUG("c194_f51");
    if (m_AnimData.size())
    {
        uintptr_t lastElement = (uintptr_t)(&m_AnimData[0] + m_AnimData.size() - sizeof(ANIM_DATA));

        IFOR (i, 0, (int)m_StaticData.size())
        {
            m_StaticDataIndex[i].Index = (ushort)i;

            m_StaticDataIndex[i].LightColor = CalculateLightColor((ushort)i);

            bool isField = false;

            //fire field
            if (IN_RANGE(i, 0x398C, 0x399F))
                isField = true;
            //paralyze field
            else if (IN_RANGE(i, 0x3967, 0x397A))
                isField = true;
            //energy field
            else if (IN_RANGE(i, 0x3946, 0x3964))
                isField = true;
            //poison field
            else if (IN_RANGE(i, 0x3914, 0x3929))
                isField = true;

            m_StaticDataIndex[i].IsFiled = isField;

            if (IsAnimated(m_StaticData[i].Flags))
            {
                uintptr_t addr = (uintptr_t)((i * 68) + 4 * ((i / 8) + 1));
                uintptr_t offset = (uintptr_t)(&m_AnimData[0] + addr);

                if (offset <= lastElement)
                    m_StaticAnimList.push_back(&m_StaticDataIndex[i]);
            }
        }
    }
}
//----------------------------------------------------------------------------------
ushort COrion::CalculateLightColor(ushort id)
{
    //WISPFUN_DEBUG("c194_f52");

    ushort color = 0;

    if (id < 0x3E27)
    {
        //color = ???;
    }
    else
    {
        color = 666;

        //if (id > 0x3E3A)
        //	color = ???;
    }

    switch (id)
    {
        case 0x088C:
            color = 31;
            break;
        case 0x0FAC:
            color = 30;
            break;
        case 0x0FB1:
            color = 60;
            break;
        case 0x1647:
            color = 61;
            break;
        case 0x19BB:
        case 0x1F2B:
            color = 40;
            break;
        default:
            break;
    };

    if (id < 0x09FB || id > 0x0A14)
    {
        if (id < 0x0A15 || id > 0x0A29)
        {
            if (id < 0x0B1A || id > 0x0B1F)
            {
                if (id < 0x0B20 || id > 0x0B25)
                {
                    if (id < 0x0B26 || id > 0x0B28)
                    {
                        if (id < 0x0DE1 || id > 0x0DEA)
                        {
                            if (id < 0x1849 || id > 0x1850)
                            {
                                if (id < 0x1853 || id > 0x185A)
                                {
                                    if (id < 0x197A || id > 0x19A9)
                                    {
                                        if (id < 0x19AB || id > 0x19B6)
                                        {
                                            if (id >= 0x1ECD && id <= 0x1ECF ||
                                                id >= 0x1ED0 && id <= 0x1ED2)
                                                color = 1;
                                        }
                                        else
                                            color = 60;
                                    }
                                    else
                                        color = 60;
                                }
                                else
                                    color = 61;
                            }
                            else
                                color = 61;
                        }
                        else
                            color = 31;
                    }
                    else
                        color = 666;
                }
                else
                    color = 666;
            }
            else
                color = 666;
        }
        else
            color = 666;
    }
    else
        color = 30;

    if (id == 0x1FD4 || id == 0x0F6C)
        color = 2;

    if (id < 0x0E2D || id > 0x0E30)
    {
        if (id < 0x0E31 || id > 0x0E33)
        {
            if (id < 0x0E5C || id > 0x0E6A)
            {
                if (id < 0x12EE || id > 0x134D)
                {
                    if (id < 0x306A || id > 0x329B)
                    {
                        if (id < 0x343B || id > 0x346C)
                        {
                            if (id < 0x3547 || id > 0x354C)
                            {
                                if (id < 0x3914 || id > 0x3929)
                                {
                                    if (id < 0x3946 || id > 0x3964)
                                    {
                                        if (id < 0x3967 || id > 0x397A)
                                        {
                                            if (id < 0x398C || id > 0x399F)
                                            {
                                                if (id < 0x3E02 || id > 0x3E0B)
                                                {
                                                    if (id < 0x3E27 || id > 0x3E3A)
                                                    {
                                                        switch (id)
                                                        {
                                                            case 0x40FE:
                                                                color = 40;
                                                                break;
                                                            case 0x40FF:
                                                                color = 10;
                                                                break;
                                                            case 0x4100:
                                                                color = 20;
                                                                break;
                                                            case 0x4101:
                                                                color = 32;
                                                                break;
                                                            default:
                                                                if (id >= 0x983B && id <= 0x983D ||
                                                                    id >= 0x983F && id <= 0x9841)
                                                                    color = 30;
                                                                break;
                                                        }
                                                    }
                                                    else
                                                        color = 31;
                                                }
                                                else
                                                    color = 1;
                                            }
                                            else
                                                color = 31;
                                        }
                                        else
                                            color = 6;
                                    }
                                    else
                                        color = 6;
                                }
                                else
                                    color = 1;
                            }
                            else
                                color = 31;
                        }
                        else
                            color = 31;
                    }
                    else
                        color = 31;
                }
                else
                    color = 31;
            }
            else
                color = 6;
        }
        else
            color = 40;
    }
    else
        color = 62;

    /*ushort color = m_StaticData[id].Hue;

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
	}*/

    return color;
}
//----------------------------------------------------------------------------------
void COrion::ProcessStaticAnimList()
{
    WISPFUN_DEBUG("c194_f53");
    if (m_AnimData.size() && g_ProcessStaticAnimationTimer < g_Ticks)
    {
        int delay =
            (g_ConfigManager.StandartItemsAnimationDelay ? ORIGINAL_ITEMS_ANIMATION_DELAY :
                                                           ORION_ITEMS_ANIMATION_DELAY);
        bool noAnimateFields = g_ConfigManager.GetNoAnimateFields();
        uint nextTime = g_Ticks + 500;

        for (deque<CIndexObjectStatic *>::iterator i = m_StaticAnimList.begin();
             i != m_StaticAnimList.end();
             ++i)
        {
            CIndexObjectStatic &obj = *(*i);

            if (noAnimateFields && obj.IsFiled)
            {
                obj.AnimIndex = 0;
                continue;
            }

            if (obj.ChangeTime < g_Ticks)
            {
                uint addr = (obj.Index * 68) + 4 * ((obj.Index / 8) + 1);
                ANIM_DATA &pad = *(PANIM_DATA)(&m_AnimData[0] + addr);

                int offset = obj.AnimIndex;

                if (pad.FrameInterval > 0)
                    obj.ChangeTime = g_Ticks + (pad.FrameInterval * delay);
                else
                    obj.ChangeTime = g_Ticks + delay;

                if (offset < pad.FrameCount)
                    obj.Offset = pad.FrameData[offset++];

                if (offset >= pad.FrameCount)
                    offset = 0;

                obj.AnimIndex = offset;
            }

            if (obj.ChangeTime < nextTime)
                nextTime = obj.ChangeTime;
        }

        g_ProcessStaticAnimationTimer = nextTime;
    }
}
//----------------------------------------------------------------------------------
void COrion::PatchFiles()
{
    WISPFUN_DEBUG("c194_f54");
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

    WISP_FILE::CMappedFile &file = g_FileManager.m_VerdataMul;

    if (!g_FileManager.UseVerdata || !file.Size)
    {
        g_ColorManager.CreateHuesPalette();
        return;
    }

    int dataCount = *(pint)file.Start;

    size_t vAddr = (size_t)file.Start;

    IFOR (i, 0, dataCount)
    {
        PVERDATA_HEADER vh = (PVERDATA_HEADER)(vAddr + 4 + (i * sizeof(VERDATA_HEADER)));

        if (vh->FileID == 0) //Map0
        {
            g_MapManager.SetPatchedMapBlock(vh->BlockID, vAddr + vh->Position);
        }
        else if (vh->FileID == 4) //Art
        {
            if (vh->BlockID >= MAX_LAND_DATA_INDEX_COUNT) //Run
            {
                ushort ID = (ushort)vh->BlockID - MAX_LAND_DATA_INDEX_COUNT;
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
        else if (vh->FileID == 16 && (int)vh->BlockID < g_SkillsManager.Count) //Skills
        {
            CSkill *skill = g_SkillsManager.Get(vh->BlockID);

            if (skill != NULL)
            {
                WISP_DATASTREAM::CDataReader reader((puchar)vAddr + vh->Position, vh->Size);
                skill->Button = (reader.ReadUInt8() != 0);
                skill->Name = reader.ReadString(vh->Size - 1);
            }
        }
        else if (vh->FileID == 30) //Tiledata
        {
            file.ResetPtr();
            file.Move(vh->Position);

            if (vh->Size == 836)
            {
                int offset = vh->BlockID * 32;

                if (offset + 32 > (int)m_LandData.size())
                    continue;

                file.ReadUInt32LE();

                IFOR (j, 0, 32)
                {
                    LAND_TILES &tile = m_LandData[offset + j];

                    if (g_PacketManager.GetClientVersion() < CV_7090)
                        tile.Flags = file.ReadUInt32LE();
                    else
                        tile.Flags = file.ReadInt64LE();

                    tile.TexID = file.ReadUInt16LE();
                    tile.Name = file.ReadString(20);
                }
            }
            else if (vh->Size == 1188)
            {
                int offset = (vh->BlockID - 0x0200) * 32;

                if (offset + 32 > (int)m_StaticData.size())
                    continue;

                file.ReadUInt32LE();

                IFOR (j, 0, 32)
                {
                    STATIC_TILES &tile = m_StaticData[offset + j];

                    if (g_PacketManager.GetClientVersion() < CV_7090)
                        tile.Flags = file.ReadUInt32LE();
                    else
                        tile.Flags = file.ReadInt64LE();

                    tile.Weight = file.ReadInt8();
                    tile.Layer = file.ReadInt8();
                    tile.Count = file.ReadInt32LE();
                    tile.AnimID = file.ReadInt16LE();
                    tile.Hue = file.ReadInt16LE();
                    tile.LightIndex = file.ReadInt16LE();
                    tile.Height = file.ReadInt8();
                    tile.Name = file.ReadString(20);
                }
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
    WISPFUN_DEBUG("c194_f55");
    if (g_PacketManager.GetClientVersion() < CV_305D) //CV_204C
        return;

    WISP_FILE::CTextFileParser newDataParser({}, " \t,{}", "#;//", "");
    WISP_FILE::CTextFileParser artParser(
        g_App.UOFilesPath("art.def"), " \t", "#;//", "{}"); // FIXME: case insensitive
    WISP_FILE::CTextFileParser textureParser(g_App.UOFilesPath("TexTerr.def"), " \t", "#;//", "{}");
    WISP_FILE::CTextFileParser gumpParser(
        g_App.UOFilesPath("gump.def"), " \t", "#;//", "{}"); // FIXME: case insensitive
    WISP_FILE::CTextFileParser multiParser(g_App.UOFilesPath("Multi.def"), " \t", "#;//", "{}");
    WISP_FILE::CTextFileParser soundParser(g_App.UOFilesPath("Sound.def"), " \t", "#;//", "{}");
    WISP_FILE::CTextFileParser mp3Parser(
        g_App.UOFilesPath("Music/Digital/Config.txt"), " ,", "#;", "");

    DEBUGLOG("Replace arts\n");
    while (!artParser.IsEOF())
    {
        STRING_LIST strings = artParser.ReadTokens();

        if (strings.size() >= 3)
        {
            int index = atoi(strings[0].c_str());

            if (index < 0 || index >= MAX_LAND_DATA_INDEX_COUNT + (int)m_StaticData.size())
                continue;

            STRING_LIST newArt = newDataParser.GetTokens(strings[1].c_str());

            int size = (int)newArt.size();

            IFOR (i, 0, size)
            {
                int checkIndex = atoi(newArt[i].c_str());

                if (checkIndex < 0 ||
                    checkIndex >= MAX_LAND_DATA_INDEX_COUNT + (int)m_StaticData.size())
                    continue;

                if (index < MAX_LAND_DATA_INDEX_COUNT && checkIndex < MAX_LAND_DATA_INDEX_COUNT &&
                    m_LandDataIndex[checkIndex].Address != NULL &&
                    m_LandDataIndex[index].Address == NULL)
                {
                    m_LandDataIndex[index] = m_LandDataIndex[checkIndex];
                    m_LandDataIndex[index].Texture = NULL;
                    m_LandDataIndex[index].Color = atoi(strings[2].c_str());

                    break;
                }
                else if (
                    index >= MAX_LAND_DATA_INDEX_COUNT && checkIndex >= MAX_LAND_DATA_INDEX_COUNT)
                {
                    checkIndex -= MAX_LAND_DATA_INDEX_COUNT;
                    checkIndex &= 0x3FFF;
                    index -= MAX_LAND_DATA_INDEX_COUNT;

                    if (m_StaticDataIndex[index].Address == NULL &&
                        m_StaticDataIndex[checkIndex].Address != NULL)
                    {
                        m_StaticDataIndex[index] = m_StaticDataIndex[checkIndex];
                        m_StaticDataIndex[index].Texture = NULL;
                        m_StaticDataIndex[index].Color = atoi(strings[2].c_str());

                        break;
                    }
                }
            }
        }
    }

    DEBUGLOG("Replace textures\n");
    while (!textureParser.IsEOF())
    {
        STRING_LIST strings = textureParser.ReadTokens();

        if (strings.size() >= 3)
        {
            int index = atoi(strings[0].c_str());

            if (index < 0 || index >= MAX_LAND_TEXTURES_DATA_INDEX_COUNT ||
                m_TextureDataIndex[index].Address != NULL)
                continue;

            STRING_LIST newTexture = newDataParser.GetTokens(strings[1].c_str());

            int size = (int)newTexture.size();

            IFOR (i, 0, size)
            {
                int checkIndex = atoi(newTexture[i].c_str());

                if (checkIndex < 0)
                    continue;

                if (index < TexturesDataCount && checkIndex < TexturesDataCount &&
                    m_TextureDataIndex[checkIndex].Address != NULL)
                {
                    m_TextureDataIndex[index] = m_TextureDataIndex[checkIndex];
                    m_TextureDataIndex[index].Texture = NULL;
                    m_TextureDataIndex[index].Color = atoi(strings[2].c_str());

                    break;
                }
            }
        }
    }

    DEBUGLOG("Replace gumps\n");
    while (!gumpParser.IsEOF())
    {
        STRING_LIST strings = gumpParser.ReadTokens();

        if (strings.size() >= 3)
        {
            int index = atoi(strings[0].c_str());

            if (index < 0 || index >= MAX_GUMP_DATA_INDEX_COUNT ||
                m_GumpDataIndex[index].Address != NULL)
                continue;

            STRING_LIST newGump = newDataParser.GetTokens(strings[1].c_str());

            int size = (int)newGump.size();

            IFOR (i, 0, size)
            {
                int checkIndex = atoi(newGump[i].c_str());

                if (checkIndex < 0 || checkIndex >= MAX_GUMP_DATA_INDEX_COUNT ||
                    m_GumpDataIndex[checkIndex].Address == NULL)
                    continue;

                m_GumpDataIndex[index] = m_GumpDataIndex[checkIndex];
                m_GumpDataIndex[index].Texture = NULL;
                m_GumpDataIndex[index].Color = atoi(strings[2].c_str());

                break;
            }
        }
    }

    DEBUGLOG("Replace multi\n");
    while (!multiParser.IsEOF())
    {
        STRING_LIST strings = multiParser.ReadTokens();

        if (strings.size() >= 3)
        {
            int index = atoi(strings[0].c_str());

            if (index < 0 || index >= g_MultiIndexCount || m_MultiDataIndex[index].Address != NULL)
                continue;

            STRING_LIST newMulti = newDataParser.GetTokens(strings[1].c_str());

            int size = (int)newMulti.size();

            IFOR (i, 0, size)
            {
                int checkIndex = atoi(newMulti[i].c_str());

                if (checkIndex < 0 || checkIndex >= g_MultiIndexCount ||
                    m_MultiDataIndex[checkIndex].Address == NULL)
                    continue;

                m_MultiDataIndex[index] = m_MultiDataIndex[checkIndex];

                break;
            }
        }
    }

    DEBUGLOG("Replace sounds\n");
    while (!soundParser.IsEOF())
    {
        STRING_LIST strings = soundParser.ReadTokens();

        if (strings.size() >= 2)
        {
            int index = atoi(strings[0].c_str());

            if (index < 0 || index >= MAX_SOUND_DATA_INDEX_COUNT ||
                m_SoundDataIndex[index].Address != NULL)
                continue;

            STRING_LIST newSound = newDataParser.GetTokens(strings[1].c_str());

            int size = (int)newSound.size();

            IFOR (i, 0, size)
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

    DEBUGLOG("Loading mp3 config\n");
    while (!mp3Parser.IsEOF())
    {
        STRING_LIST strings = mp3Parser.ReadTokens();

        size_t size = strings.size();

        if (size > 0)
        {
            uint index = std::atoi(strings[0].c_str());
            CIndexMusic &mp3 = m_MP3Data[index];
            string name = "Music/Digital/" + strings[1];
            string extension = ".mp3";
            if (name.find(extension) == string::npos)
                name += extension;
            if (size > 1)
                mp3.FilePath = g_App.UOFilesPath(name);

            if (size > 2)
                mp3.Loop = true;
        }
    }
}
//----------------------------------------------------------------------------------
void COrion::CreateAuraTexture()
{
    WISPFUN_DEBUG("c194_f57");
    UINT_LIST pixels;
    short width = 0;
    short height = 0;

    CGLTextureCircleOfTransparency::CreatePixels(30, width, height, pixels);

    IFOR (i, 0, (int)pixels.size())
    {
        uint &pixel = pixels[i];

        if (pixel)
        {
            ushort value = pixel << 3;

            if (value > 0xFF)
                value = 0xFF;

            pixel = (value << 24) | (value << 16) | (value << 8) | value;
        }
    }

    g_GL_BindTexture32(g_AuraTexture, width, height, &pixels[0]);
}
//----------------------------------------------------------------------------------
void COrion::CreateObjectHandlesBackground()
{
    WISPFUN_DEBUG("c194_f58");
    CGLTexture *th[9] = { NULL };
    ushort gumpID[9] = { 0 };

    IFOR (i, 0, 9)
    {
        CGLTexture *pth = ExecuteGump(0x24EA + (ushort)i);

        if (pth == NULL)
        {
            LOG("Error!!! Failed to create Object Handles background data!\n");
            return;
        }

        if (i == 4)
        {
            th[8] = pth;
            gumpID[8] = 0x24EA + (ushort)i;
        }
        else if (i > 4)
        {
            th[i - 1] = pth;
            gumpID[i - 1] = 0x24EA + (ushort)i;
        }
        else
        {
            th[i] = pth;
            gumpID[i] = 0x24EA + (ushort)i;
        }
    }

    IFOR (i, 0, 8)
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

        USHORT_LIST pixels = g_UOFileReader.GetGumpPixels(io);

        if (pixels.size())
        {
            int gumpWidth = io.Width;
            int gumpHeight = io.Height;
            int srcX = 0;

            IFOR (x, drawX, drawWidth)
            {
                int srcY = 0;

                IFOR (y, drawY, drawHeight)
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
    WISPFUN_DEBUG("c194_f59");

#if UO_USE_SHADER_FILES == 1
    WISP_FILE::CMappedFile frag;
    WISP_FILE::CMappedFile vert;

    if (vert.Load(g_App.FilePath("shaders/Shader.vert")))
    {
        frag.Load(g_App.FilePath("shaders/DeathShader.frag"));

        g_DeathShader.Init((char *)vert.Start, (char *)frag.Start);

        frag.Unload();

        frag.Load(g_App.FilePath("shaders/ColorizerShader.frag"));

        g_ColorizerShader.Init((char *)vert.Start, (char *)frag.Start);

        frag.Unload();

        frag.Load(g_App.FilePath("shaders/FontColorizerShader.frag"));

        g_FontColorizerShader.Init((char *)vert.Start, (char *)frag.Start);

        frag.Unload();

        frag.Load(g_App.FilePath("shaders/LightColorizerShader.frag"));

        g_LightColorizerShader.Init((char *)vert.Start, (char *)frag.Start);

        frag.Unload();
        vert.Unload();
    }
#else
    g_DeathShader.Init(g_Vert_ShaderData, g_Frag_DeathShaderData);
    g_ColorizerShader.Init(g_Vert_ShaderData, g_Frag_ColorizerShaderData);
    g_FontColorizerShader.Init(g_Vert_ShaderData, g_Frag_FontShaderData);
    g_LightColorizerShader.Init(g_Vert_ShaderData, g_Frag_LightShaderData);
#endif
}
//----------------------------------------------------------------------------------
void COrion::LoadClientStartupConfig()
{
    WISPFUN_DEBUG("c194_f60");
    if (!g_ConfigManager.Load(g_App.ExeFilePath("orion_options.cfg")))
    {
        if (!g_ConfigManager.Load(g_App.UOFilesPath("orion_options.cfg")))
            if (!g_ConfigManager.LoadBin(g_App.ExeFilePath("options_debug.cuo")))
                g_ConfigManager.LoadBin(g_App.UOFilesPath("options_debug.cuo"));
    }

    g_SoundManager.SetMusicVolume(g_ConfigManager.GetMusicVolume());

    if (g_ConfigManager.GetMusic())
    {
        if (g_PacketManager.GetClientVersion() >= CV_7000)
            PlayMusic(78);
        else if (
            g_PacketManager.GetClientVersion() >
            CV_308Z) //from 4.x the music played is 0, the first one
            PlayMusic(0);
        else
            PlayMusic(8);
    }
}
//----------------------------------------------------------------------------------
void COrion::PlayMusic(int index, bool warmode)
{
    WISPFUN_DEBUG("c194_f61");
    if (!g_ConfigManager.GetMusic() || index >= MAX_MUSIC_DATA_INDEX_COUNT)
        return;
    if (!warmode && g_SoundManager.CurrentMusicIndex == index &&
        g_SoundManager.IsPlayingNormalMusic())
        return;

    if (g_PacketManager.GetClientVersion() >= CV_306E)
    {
        CIndexMusic &mp3Info = m_MP3Data[index];
        g_SoundManager.PlayMP3(mp3Info.FilePath, index, mp3Info.Loop, warmode);
    }
    else
        g_SoundManager.PlayMidi(index, warmode);
}
//----------------------------------------------------------------------------------
void COrion::PauseSound()
{
    WISPFUN_DEBUG("c194_f62");
    g_SoundManager.PauseSound();
}
//----------------------------------------------------------------------------------
void COrion::ResumeSound()
{
    WISPFUN_DEBUG("c194_f63");
    g_SoundManager.ResumeSound();
}
//----------------------------------------------------------------------------------
void COrion::PlaySoundEffect(ushort id, float volume)
{
    WISPFUN_DEBUG("c194_f64");
    if (id >= 0x0800 || !g_ConfigManager.GetSound())
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
void COrion::AdjustSoundEffects(int ticks, float volume)
{
    WISPFUN_DEBUG("c194_f65");
    for (deque<CIndexSound *>::iterator i = m_UsedSoundList.begin(); i != m_UsedSoundList.end();)
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
            ++i;
    }
}
//----------------------------------------------------------------------------------
CGLTexture *COrion::ExecuteGump(ushort id)
{
    //WISPFUN_DEBUG("c194_f66");
    if (id >= MAX_GUMP_DATA_INDEX_COUNT)
        return nullptr;

    CIndexObject &io = m_GumpDataIndex[id];
    if (io.Texture == 0)
    {
        if (!io.Address)
            return nullptr;

        io.Texture = g_UOFileReader.ReadGump(io);

        if (io.Texture != 0)
            m_UsedGumpList.push_back(&m_GumpDataIndex[id]);
    }

    io.LastAccessTime = g_Ticks;

    return io.Texture;
}
//----------------------------------------------------------------------------------
CGLTexture *COrion::ExecuteLandArt(ushort id)
{
    WISPFUN_DEBUG("c194_f67");
    if (id >= MAX_LAND_DATA_INDEX_COUNT)
        return NULL;
    CIndexObject &io = m_LandDataIndex[id];

    if (io.Texture == 0)
    {
        if (!io.Address) //nodraw tiles banned
            return NULL;

        io.Texture = g_UOFileReader.ReadArt(id, io, false);

        if (io.Texture != 0)
            m_UsedLandList.push_back(&m_LandDataIndex[id]);
    }

    io.LastAccessTime = g_Ticks;

    return io.Texture;
}
//----------------------------------------------------------------------------------
CGLTexture *COrion::ExecuteStaticArtAnimated(ushort id)
{
    WISPFUN_DEBUG("c194_f68");
    return ExecuteStaticArt(id + m_StaticDataIndex[id].Offset);
}
//----------------------------------------------------------------------------------
CGLTexture *COrion::ExecuteStaticArt(ushort id)
{
    WISPFUN_DEBUG("c194_f69");
    if (id >= MAX_STATIC_DATA_INDEX_COUNT)
        return NULL;
    CIndexObject &io = m_StaticDataIndex[id];

    if (io.Texture == 0)
    {
        if (!io.Address) //nodraw tiles banned
            return NULL;

        io.Texture = g_UOFileReader.ReadArt(id, io, true);

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
    WISPFUN_DEBUG("c194_f70");
    id = m_LandData[id].TexID;

    if (!id || id >= MAX_LAND_TEXTURES_DATA_INDEX_COUNT)
        return NULL;

    CIndexObject &io = m_TextureDataIndex[id];

    if (io.Texture == 0)
    {
        if (!io.Address)
            return NULL;

        io.Texture = g_UOFileReader.ReadTexture(io);

        if (io.Texture != 0)
            m_UsedTextureList.push_back(&m_TextureDataIndex[id]);
    }

    io.LastAccessTime = g_Ticks;

    return io.Texture;
}
//----------------------------------------------------------------------------------
CGLTexture *COrion::ExecuteLight(uchar &id)
{
    WISPFUN_DEBUG("c194_f71");
    if (id >= MAX_LIGHTS_DATA_INDEX_COUNT)
        id = 0;

    CIndexObject &io = m_LightDataIndex[id];

    if (io.Texture == 0)
    {
        if (!io.Address)
            return NULL;

        io.Texture = g_UOFileReader.ReadLight(io);

        if (io.Texture != 0)
            m_UsedLightList.push_back(&m_LightDataIndex[id]);
    }

    io.LastAccessTime = g_Ticks;

    return io.Texture;
}
//----------------------------------------------------------------------------------
bool COrion::ExecuteGumpPart(ushort id, int count)
{
    WISPFUN_DEBUG("c194_f72");
    bool result = true;

    IFOR (i, 0, count)
    {
        if (ExecuteGump(id + (ushort)i) == NULL)
            result = false;
    }

    return result;
}
//----------------------------------------------------------------------------------
void COrion::DrawGump(ushort id, ushort color, int x, int y, bool partialHue)
{
    WISPFUN_DEBUG("c194_f73");
    CGLTexture *th = ExecuteGump(id);

    if (th != NULL)
    {
        if (!g_GrayedPixels && color)
        {
            if (partialHue)
                glUniform1iARB(g_ShaderDrawMode, SDM_PARTIAL_HUE);
            else
                glUniform1iARB(g_ShaderDrawMode, SDM_COLORED);

            g_ColorManager.SendColorsToShader(color);
        }
        else
            glUniform1iARB(g_ShaderDrawMode, SDM_NO_COLOR);

        th->Draw(x, y);
    }
}
//----------------------------------------------------------------------------------
void COrion::DrawGump(ushort id, ushort color, int x, int y, int width, int height, bool partialHue)
{
    WISPFUN_DEBUG("c194_f74");
    CGLTexture *th = ExecuteGump(id);

    if (th != NULL)
    {
        if (!g_GrayedPixels && color)
        {
            if (partialHue)
                glUniform1iARB(g_ShaderDrawMode, SDM_PARTIAL_HUE);
            else
                glUniform1iARB(g_ShaderDrawMode, SDM_COLORED);

            g_ColorManager.SendColorsToShader(color);
        }
        else
            glUniform1iARB(g_ShaderDrawMode, SDM_NO_COLOR);

        th->Draw(x, y, width, height);
    }
}
//----------------------------------------------------------------------------------
void COrion::DrawResizepicGump(ushort id, int x, int y, int width, int height)
{
    WISPFUN_DEBUG("c194_f75");
    CGLTexture *th[9] = { NULL };

    IFOR (i, 0, 9)
    {
        CGLTexture *pth = ExecuteGump(id + (ushort)i);

        if (pth == NULL)
            return;

        if (i == 4)
            th[8] = pth;
        else if (i > 4)
            th[i - 1] = pth;
        else
            th[i] = pth;
    }

    g_GL_DrawResizepic(th, x, y, width, height);
}
//----------------------------------------------------------------------------------
void COrion::DrawLandTexture(CLandObject *land, ushort color, int x, int y)
{
    WISPFUN_DEBUG("c194_f76");
    ushort id = land->Graphic;

    CGLTexture *th = ExecuteTexture(id);

    if (th == NULL)
        DrawLandArt(id, color, x, y);
    else
    {
        if (g_OutOfRangeColor)
            color = g_OutOfRangeColor;

        if (!g_GrayedPixels && color)
        {
            glUniform1iARB(g_ShaderDrawMode, SDM_LAND_COLORED);
            g_ColorManager.SendColorsToShader(color);
        }
        else
            glUniform1iARB(g_ShaderDrawMode, SDM_LAND);

        g_GL_DrawLandTexture(*th, x, y + (land->GetZ() * 4), land);
    }
}
//----------------------------------------------------------------------------------
void COrion::DrawLandArt(ushort id, ushort color, int x, int y)
{
    WISPFUN_DEBUG("c194_f77");
    CGLTexture *th = ExecuteLandArt(id);

    if (th != NULL)
    {
        if (g_OutOfRangeColor)
            color = g_OutOfRangeColor;

        if (!g_GrayedPixels && color)
        {
            glUniform1iARB(g_ShaderDrawMode, SDM_COLORED);
            g_ColorManager.SendColorsToShader(color);
        }
        else
            glUniform1iARB(g_ShaderDrawMode, SDM_NO_COLOR);

        th->Draw(x - 22, y - 22);
    }
}
//----------------------------------------------------------------------------------
void COrion::DrawStaticArt(ushort id, ushort color, int x, int y, bool selection)
{
    WISPFUN_DEBUG("c194_f78");
    CGLTexture *th = ExecuteStaticArt(id);

    if (th != NULL && id > 1)
    {
        if (g_OutOfRangeColor)
            color = g_OutOfRangeColor;

        if (!g_GrayedPixels && color)
        {
            if (!selection && IsPartialHue(GetStaticFlags(id)))
                glUniform1iARB(g_ShaderDrawMode, SDM_PARTIAL_HUE);
            else
                glUniform1iARB(g_ShaderDrawMode, SDM_COLORED);

            g_ColorManager.SendColorsToShader(color);
        }
        else
            glUniform1iARB(g_ShaderDrawMode, SDM_NO_COLOR);

        th->Draw(x - m_StaticDataIndex[id].Width, y - m_StaticDataIndex[id].Height);
    }
}
//----------------------------------------------------------------------------------
void COrion::DrawStaticArtAnimated(ushort id, ushort color, int x, int y, bool selection)
{
    WISPFUN_DEBUG("c194_f79");
    DrawStaticArt(id + m_StaticDataIndex[id].Offset, color, x, y, selection);
}
//----------------------------------------------------------------------------------
void COrion::DrawStaticArtRotated(ushort id, ushort color, int x, int y, float angle)
{
    WISPFUN_DEBUG("c194_f80");
    CGLTexture *th = ExecuteStaticArt(id);

    if (th != NULL && id > 1)
    {
        if (g_OutOfRangeColor)
            color = g_OutOfRangeColor;

        if (!g_GrayedPixels && color)
        {
            glUniform1iARB(g_ShaderDrawMode, SDM_COLORED);
            g_ColorManager.SendColorsToShader(color);
        }
        else
            glUniform1iARB(g_ShaderDrawMode, SDM_NO_COLOR);

        th->DrawRotated(x, y, angle);
    }
}
//----------------------------------------------------------------------------------
void COrion::DrawStaticArtAnimatedRotated(ushort id, ushort color, int x, int y, float angle)
{
    WISPFUN_DEBUG("c194_f81");
    DrawStaticArtRotated(id + m_StaticDataIndex[id].Offset, color, x, y, angle);
}
//----------------------------------------------------------------------------------
void COrion::DrawStaticArtTransparent(ushort id, ushort color, int x, int y, bool selection)
{
    WISPFUN_DEBUG("c194_f82");
    CGLTexture *th = ExecuteStaticArt(id);

    if (th != NULL && id > 1)
    {
        if (g_OutOfRangeColor)
            color = g_OutOfRangeColor;

        if (!g_GrayedPixels && color)
        {
            if (!selection && IsPartialHue(GetStaticFlags(id)))
                glUniform1iARB(g_ShaderDrawMode, SDM_PARTIAL_HUE);
            else
                glUniform1iARB(g_ShaderDrawMode, SDM_COLORED);

            g_ColorManager.SendColorsToShader(color);
        }
        else
            glUniform1iARB(g_ShaderDrawMode, SDM_NO_COLOR);

        th->DrawTransparent(x - m_StaticDataIndex[id].Width, y - m_StaticDataIndex[id].Height);
    }
}
//----------------------------------------------------------------------------------
void COrion::DrawStaticArtAnimatedTransparent(ushort id, ushort color, int x, int y, bool selection)
{
    WISPFUN_DEBUG("c194_f83");
    DrawStaticArtTransparent(id + m_StaticDataIndex[id].Offset, color, x, y, selection);
}
//----------------------------------------------------------------------------------
void COrion::DrawStaticArtInContainer(
    ushort id, ushort color, int x, int y, bool selection, bool onMouse)
{
    WISPFUN_DEBUG("c194_f84");
    CGLTexture *th = ExecuteStaticArt(id);

    if (th != NULL)
    {
        if (onMouse)
        {
            x -= th->Width / 2;
            y -= th->Height / 2;
        }

        if (!g_GrayedPixels && color)
        {
            if (color >= 0x4000)
            {
                color = 0x1;
                glUniform1iARB(g_ShaderDrawMode, SDM_COLORED);
            }
            else if (!selection && IsPartialHue(GetStaticFlags(id)))
                glUniform1iARB(g_ShaderDrawMode, SDM_PARTIAL_HUE);
            else
                glUniform1iARB(g_ShaderDrawMode, SDM_COLORED);

            g_ColorManager.SendColorsToShader(color);
        }
        else
            glUniform1iARB(g_ShaderDrawMode, SDM_NO_COLOR);

        th->Draw(x, y);
    }
}
//----------------------------------------------------------------------------------
void COrion::DrawLight(LIGHT_DATA &light)
{
    WISPFUN_DEBUG("c194_f85");
    CGLTexture *th = ExecuteLight(light.ID);

    if (th != NULL)
    {
        if (light.Color)
        {
            glUniform1iARB(g_ShaderDrawMode, SDM_COLORED);
            g_ColorManager.SendColorsToShader(light.Color);
        }
        else
            glUniform1iARB(g_ShaderDrawMode, SDM_NO_COLOR);

        th->Draw(
            light.DrawX - g_RenderBounds.GameWindowPosX - (th->Width / 2),
            light.DrawY - g_RenderBounds.GameWindowPosY - (th->Height / 2));
    }
}
//----------------------------------------------------------------------------------
bool COrion::PolygonePixelsInXY(int x, int y, int width, int height)
{
    WISPFUN_DEBUG("c194_f86");
    x = g_MouseManager.Position.X - x;
    y = g_MouseManager.Position.Y - y;

    return !(x < 0 || y < 0 || x >= width || y >= height);
}
//----------------------------------------------------------------------------------
bool COrion::GumpPixelsInXY(ushort id, int x, int y)
{
    WISPFUN_DEBUG("c194_f87");
    CGLTexture *texture = m_GumpDataIndex[id].Texture;

    if (texture != NULL)
        return texture->Select(x, y);

    return false;
}
//----------------------------------------------------------------------------------
bool COrion::GumpPixelsInXY(ushort id, int x, int y, int width, int height)
{
    WISPFUN_DEBUG("c194_f88");
    CGLTexture *texture = m_GumpDataIndex[id].Texture;

    if (texture == NULL)
        return false;

    x = g_MouseManager.Position.X - x;
    y = g_MouseManager.Position.Y - y;

    if (x < 0 || y < 0 || (width > 0 && x >= width) || (height > 0 && y >= height))
        return false;

    int textureWidth = texture->Width;
    int textureHeight = texture->Height;

    if (width == 0)
        width = textureWidth;

    if (height == 0)
        height = textureHeight;

    while (x > textureWidth && width > textureWidth)
    {
        x -= textureWidth;
        width -= textureWidth;
    }

    if (x < 0 || x > width)
        return false;

    while (y > textureHeight && height > textureHeight)
    {
        y -= textureHeight;
        height -= textureHeight;
    }

    if (y < 0 || y > height)
        return false;

    int pos = (y * textureWidth) + x;

    if (pos < (int)texture->m_HitMap.size())
        return (texture->m_HitMap[pos] != 0);

    return false;
}
//----------------------------------------------------------------------------------
bool COrion::ResizepicPixelsInXY(ushort id, int x, int y, int width, int height)
{
    WISPFUN_DEBUG("c194_f89");
    int tempX = g_MouseManager.Position.X - x;
    int tempY = g_MouseManager.Position.Y - y;

    if (tempX < 0 || tempY < 0 || tempX >= width || tempY >= height)
        return false;

    CGLTexture *th[9] = { NULL };

    IFOR (i, 0, 9)
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

    int offsetTop = max(th[0]->Height, th[2]->Height) - th[1]->Height;
    int offsetBottom = max(th[5]->Height, th[7]->Height) - th[6]->Height;
    int offsetLeft = max(th[0]->Width, th[5]->Width) - th[3]->Width;
    int offsetRight = max(th[2]->Width, th[7]->Width) - th[4]->Width;

    IFOR (i, 0, 9)
    {
        switch (i)
        {
            case 0:
            {
                if (GumpPixelsInXY(id, x, y))
                    return true;
                break;
            }
            case 1:
            {
                int DW = width - th[0]->Width - th[2]->Width;
                if (DW < 1)
                    break;

                if (GumpPixelsInXY(id + 1, x + th[0]->Width, y, DW, 0))
                    return true;

                break;
            }
            case 2:
            {
                if (GumpPixelsInXY(id + 2, x + width - th[i]->Width, y + offsetTop))
                    return true;

                break;
            }
            case 3:
            {
                int DH = height - th[0]->Height - th[5]->Height;
                if (DH < 1)
                    break;

                if (GumpPixelsInXY(id + 3, x + offsetLeft, y + th[0]->Height, 0, DH))
                    return true;

                break;
            }
            case 4:
            {
                int DH = height - th[2]->Height - th[7]->Height;
                if (DH < 1)
                    break;

                if (GumpPixelsInXY(
                        id + 5, x + width - th[i]->Width - offsetRight, y + th[2]->Height, 0, DH))
                    return true;

                break;
            }
            case 5:
            {
                if (GumpPixelsInXY(id + 6, x, y + height - th[i]->Height))
                    return true;

                break;
            }
            case 6:
            {
                int DW = width - th[5]->Width - th[7]->Width;
                if (DW < 1)
                    break;

                if (GumpPixelsInXY(
                        id + 7, x + th[5]->Width, y + height - th[i]->Height - offsetBottom, DW, 0))
                    return true;

                break;
            }
            case 7:
            {
                if (GumpPixelsInXY(id + 8, x + width - th[i]->Width, y + height - th[i]->Height))
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

                if (GumpPixelsInXY(id + 4, x + th[0]->Width, y + th[0]->Height, DW, DH))
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
bool COrion::StaticPixelsInXY(ushort id, int x, int y)
{
    WISPFUN_DEBUG("c194_f90");
    CIndexObject &io = m_StaticDataIndex[id];
    CGLTexture *texture = io.Texture;

    if (texture != NULL)
        return texture->Select(x - io.Width, y - io.Height);

    return false;
}
//----------------------------------------------------------------------------------
bool COrion::StaticPixelsInXYAnimated(ushort id, int x, int y)
{
    WISPFUN_DEBUG("c194_f91");
    return StaticPixelsInXY(id + m_StaticDataIndex[id].Offset, x, y);
}
//----------------------------------------------------------------------------------
bool COrion::StaticPixelsInXYInContainer(ushort id, int x, int y)
{
    WISPFUN_DEBUG("c194_f93");
    CGLTexture *texture = m_StaticDataIndex[id].Texture;

    if (texture != NULL)
        return texture->Select(x, y);

    return false;
}
//----------------------------------------------------------------------------------
bool COrion::LandPixelsInXY(ushort id, int x, int y)
{
    WISPFUN_DEBUG("c194_f94");
    CGLTexture *texture = m_LandDataIndex[id].Texture;

    if (texture != NULL)
        return texture->Select(x - 22, y - 22);

    return false;
}
//----------------------------------------------------------------------------------
bool COrion::LandTexturePixelsInXY(int x, int y, RECT &r)
{
    WISPFUN_DEBUG("c194_f95");
    y -= 22;
    int testX = g_MouseManager.Position.X - x;
    int testY = g_MouseManager.Position.Y;

    int y0 = -r.left;
    int y1 = 22 - r.top;
    int y2 = 44 - r.right;
    int y3 = 22 - r.bottom;

    bool result =
        ((testY >= testX * (y1 - y0) / -22 + y + y0) &&
         (testY >= testX * (y3 - y0) / 22 + y + y0) && (testY <= testX * (y3 - y2) / 22 + y + y2) &&
         (testY <= testX * (y1 - y2) / -22 + y + y2));

    return result;
}
//----------------------------------------------------------------------------------
void COrion::CreateTextMessageF(uchar font, ushort color, const char *format, ...)
{
    WISPFUN_DEBUG("c194_f96");
    va_list arg;
    va_start(arg, format);

    char buf[512] = { 0 };
    vsprintf_s(buf, format, arg);

    CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, font, color, buf);

    va_end(arg);
}
//----------------------------------------------------------------------------------
void COrion::CreateUnicodeTextMessageF(uchar font, ushort color, const char *format, ...)
{
    WISPFUN_DEBUG("c194_f97");
    va_list arg;
    va_start(arg, format);

    char buf[512] = { 0 };
    vsprintf_s(buf, format, arg);

    CreateUnicodeTextMessage(TT_SYSTEM, 0xFFFFFFFF, font, color, ToWString(buf));

    va_end(arg);
}
//----------------------------------------------------------------------------------
void COrion::CreateTextMessage(
    const TEXT_TYPE &type,
    int serial,
    uchar font,
    ushort color,
    const string &text,
    CRenderWorldObject *clientObj)
{
    WISPFUN_DEBUG("c194_f98");
    CTextData *td = new CTextData();
    td->Unicode = false;
    td->Font = font;
    td->Serial = serial;
    td->Color = color;
    td->Timer = g_Ticks;
    td->Type = type;
    td->Text = text;

    switch (type)
    {
        case TT_SYSTEM:
        {
            td->GenerateTexture(TEXT_SYSTEM_MESSAGE_MAX_WIDTH);
            AddSystemMessage(td);

            break;
        }
        case TT_OBJECT:
        {
            CGameObject *obj = g_World->FindWorldObject(serial);

            if (obj != NULL)
            {
                int width = g_FontManager.GetWidthA(font, text);

                td->Color = 0;

                if (width > TEXT_MESSAGE_MAX_WIDTH)
                {
                    width =
                        g_FontManager.GetWidthExA(font, text, TEXT_MESSAGE_MAX_WIDTH, TS_LEFT, 0);
                    td->GenerateTexture(width, 0, TS_LEFT);
                }
                else
                    td->GenerateTexture(0, 0, TS_CENTER);

                td->Color = color;

                uint container = obj->Container;

                if (container == 0xFFFFFFFF)
                    g_WorldTextRenderer.AddText(td);
                else if (!obj->NPC)
                {
                    td->SetX(g_ClickObject.X);
                    td->SetY(g_ClickObject.Y);

                    CGump *gump = g_GumpManager.GetGump(container, 0, GT_CONTAINER);

                    if (gump == NULL)
                    {
                        CGameObject *topobj = obj->GetTopObject();

                        if (((CGameItem *)obj)->Layer != OL_NONE)
                            gump = g_GumpManager.GetGump(topobj->Serial, 0, GT_PAPERDOLL);

                        if (gump == NULL)
                        {
                            gump = g_GumpManager.GetGump(topobj->Serial, 0, GT_TRADE);

                            if (gump == NULL)
                            {
                                topobj = (CGameObject *)topobj->m_Items;

                                while (topobj != NULL && topobj->Graphic != 0x1E5E)
                                    topobj = (CGameObject *)topobj->m_Next;

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

                obj->AddText(td);
            }
            else
            {
                td->GenerateTexture(TEXT_SYSTEM_MESSAGE_MAX_WIDTH);
                AddSystemMessage(td);
            }

            break;
        }
        case TT_CLIENT:
        {
            int width = g_FontManager.GetWidthA(font, text);

            if (width > TEXT_MESSAGE_MAX_WIDTH)
            {
                width = g_FontManager.GetWidthExA(font, text, TEXT_MESSAGE_MAX_WIDTH, TS_LEFT, 0);
                td->GenerateTexture(width, 0, TS_LEFT);
            }
            else
                td->GenerateTexture(0, 0, TS_CENTER);

            clientObj->AddText(td);
            g_WorldTextRenderer.AddText(td);

            break;
        }
    }
}
//----------------------------------------------------------------------------------
void COrion::CreateUnicodeTextMessage(
    const TEXT_TYPE &type,
    int serial,
    uchar font,
    ushort color,
    const wstring &text,
    CRenderWorldObject *clientObj)
{
    WISPFUN_DEBUG("c194_f99");
    CTextData *td = new CTextData();
    td->Unicode = true;
    td->Font = font;
    td->Serial = serial;
    td->Color = color;
    td->Timer = g_Ticks;
    td->Type = type;
    td->UnicodeText = text;

    switch (type)
    {
        case TT_SYSTEM:
        {
            td->GenerateTexture(TEXT_SYSTEM_MESSAGE_MAX_WIDTH, UOFONT_BLACK_BORDER);
            AddSystemMessage(td);

            break;
        }
        case TT_OBJECT:
        {
            CGameObject *obj = g_World->FindWorldObject(serial);

            if (obj != NULL)
            {
                int width = g_FontManager.GetWidthW(font, text);

                if (width > TEXT_MESSAGE_MAX_WIDTH)
                {
                    width = g_FontManager.GetWidthExW(
                        font, text, TEXT_MESSAGE_MAX_WIDTH, TS_LEFT, UOFONT_BLACK_BORDER);
                    td->GenerateTexture(width, UOFONT_BLACK_BORDER, TS_LEFT);
                }
                else
                    td->GenerateTexture(0, UOFONT_BLACK_BORDER, TS_CENTER);

                uint container = obj->Container;

                if (container == 0xFFFFFFFF)
                    g_WorldTextRenderer.AddText(td);
                else if (!obj->NPC)
                {
                    td->SetX(g_ClickObject.X);
                    td->SetY(g_ClickObject.Y);

                    CGump *gump = g_GumpManager.GetGump(container, 0, GT_CONTAINER);

                    if (gump == NULL)
                    {
                        CGameObject *topobj = obj->GetTopObject();

                        if (((CGameItem *)obj)->Layer != OL_NONE)
                            gump = g_GumpManager.GetGump(topobj->Serial, 0, GT_PAPERDOLL);

                        if (gump == NULL)
                        {
                            gump = g_GumpManager.GetGump(topobj->Serial, 0, GT_TRADE);

                            if (gump == NULL)
                            {
                                topobj = (CGameObject *)topobj->m_Items;

                                while (topobj != NULL && topobj->Graphic != 0x1E5E)
                                    topobj = (CGameObject *)topobj->m_Next;

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

                obj->AddText(td);
            }
            else
            {
                td->GenerateTexture(TEXT_SYSTEM_MESSAGE_MAX_WIDTH, UOFONT_BLACK_BORDER);
                AddSystemMessage(td);
            }

            break;
        }
        case TT_CLIENT:
        {
            int width = g_FontManager.GetWidthW(font, text);

            if (width > TEXT_MESSAGE_MAX_WIDTH)
            {
                width = g_FontManager.GetWidthExW(font, text, TEXT_MESSAGE_MAX_WIDTH, TS_LEFT, 0);
                td->GenerateTexture(width, 0, TS_LEFT);
            }
            else
                td->GenerateTexture(0, UOFONT_BLACK_BORDER, TS_LEFT);

            clientObj->AddText(td);
            g_WorldTextRenderer.AddText(td);

            break;
        }
    }
}
//----------------------------------------------------------------------------------
void COrion::AddSystemMessage(CTextData *msg)
{
    WISPFUN_DEBUG("c194_f100");
    g_SystemChat.Add(msg);
    AddJournalMessage(msg, "");
}
//----------------------------------------------------------------------------------
void COrion::AddJournalMessage(CTextData *msg, const string &name)
{
    WISPFUN_DEBUG("c194_f101");
    CTextData *jmsg = new CTextData(msg);

    if (!jmsg->Unicode)
    {
        jmsg->Text = name + jmsg->Text;
        jmsg->Font = 9;
    }
    else
    {
        //if (msg->Type == TT_SYSTEM)
        //	jmsg->Color = 0;

        jmsg->UnicodeText = ToWString(name) + jmsg->UnicodeText;
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
    WISPFUN_DEBUG("c194_f102");
    if (newmap < 0 || newmap > 5)
        newmap = 0;

    if (g_CurrentMap != newmap)
    {
        g_CurrentMap = newmap;

        if (g_World != NULL && g_Player != NULL)
        {
            g_Player->MapIndex = g_CurrentMap;
            g_Player->RemoveRender();

            CGameObject *obj = (CGameObject *)g_World->m_Items;

            while (obj != NULL)
            {
                CGameObject *next = (CGameObject *)obj->m_Next;

                if (obj->MapIndex != g_CurrentMap)
                {
                    if (g_Party.Contains(obj->Serial))
                    {
                        obj->RemoveRender();
                        g_GumpManager.UpdateContent(obj->Serial, 0, GT_STATUSBAR);
                    }
                    else
                        g_World->RemoveObject(obj);
                }

                obj = next;
            }

            g_MapManager.Clear();
            g_MapManager.Init();

            g_MapManager.AddRender(g_Player);
        }
    }
}
//----------------------------------------------------------------------------------
void COrion::PickupItem(CGameItem *obj, int count, bool isGameFigure)
{
    WISPFUN_DEBUG("c194_f103");
    if (!g_ObjectInHand.Enabled)
    {
        g_ObjectInHand.Clear();
        g_ObjectInHand.Enabled = true;

        if (!count)
            count = obj->Count;

        g_ObjectInHand.Serial = obj->Serial;
        g_ObjectInHand.Graphic = obj->Graphic;
        g_ObjectInHand.Color = obj->Color;
        g_ObjectInHand.Container = obj->Container;
        g_ObjectInHand.Layer = obj->Layer;
        g_ObjectInHand.Flags = obj->GetFlags();
        g_ObjectInHand.TiledataPtr = obj->GetStaticData();
        g_ObjectInHand.Count = count;
        g_ObjectInHand.IsGameFigure = isGameFigure;
        g_ObjectInHand.X = obj->GetX();
        g_ObjectInHand.Y = obj->GetY();
        g_ObjectInHand.Z = obj->GetZ();
        g_ObjectInHand.TotalCount = obj->Count;

        CPacketPickupRequest(g_ObjectInHand.Serial, count).Send();

        g_World->ObjectToRemove = g_ObjectInHand.Serial;
    }
}
//----------------------------------------------------------------------------------
void COrion::DropItem(int container, ushort x, ushort y, char z)
{
    WISPFUN_DEBUG("c194_f104");
    if (g_ObjectInHand.Enabled && g_ObjectInHand.Serial != container)
    {
        if (g_PacketManager.GetClientVersion() >= CV_6017)
            CPacketDropRequestNew(g_ObjectInHand.Serial, x, y, z, 0, container).Send();
        else
            CPacketDropRequestOld(g_ObjectInHand.Serial, x, y, z, container).Send();

        g_ObjectInHand.Enabled = false;
        g_ObjectInHand.Dropped = true;
    }
}
//----------------------------------------------------------------------------------
void COrion::EquipItem(uint container)
{
    WISPFUN_DEBUG("c194_f105");
    if (g_ObjectInHand.Enabled)
    {
        if (IsWearable(g_ObjectInHand.TiledataPtr->Flags))
        {
            ushort graphic = g_ObjectInHand.Graphic;

            if (!container)
                container = g_PlayerSerial;

            CPacketEquipRequest(g_ObjectInHand.Serial, m_StaticData[graphic].Layer, container)
                .Send();

            g_ObjectInHand.Enabled = false;
            g_ObjectInHand.Dropped = true;
        }
    }
}
//----------------------------------------------------------------------------------
void COrion::ChangeWarmode(uchar status)
{
    WISPFUN_DEBUG("c194_f106");
    uchar newstatus = (uchar)(!g_Player->Warmode);

    if (status != 0xFF)
    {
        if (g_Player->Warmode == (bool)status)
            return;

        newstatus = status;
    }

    if (newstatus == 1 && g_ConfigManager.GetMusic())
        PlayMusic(rand() % 3 + 38, true);
    else if (newstatus == 0)
        g_SoundManager.StopWarMusic();

    CPacketChangeWarmode(newstatus).Send();
}
//----------------------------------------------------------------------------------
void COrion::Click(uint serial)
{
    WISPFUN_DEBUG("c194_f111");
    CPacketClickRequest(serial).Send();

    CGameObject *obj = g_World->FindWorldObject(serial);
    if (obj != NULL)
        obj->Clicked = true;
}
//----------------------------------------------------------------------------------
void COrion::DoubleClick(uint serial)
{
    WISPFUN_DEBUG("c194_f112");
    if (serial >= 0x40000000)
        g_LastUseObject = serial;

    CPacketDoubleClickRequest(serial).Send();
}
//----------------------------------------------------------------------------------
void COrion::PaperdollReq(uint serial)
{
    WISPFUN_DEBUG("c194_f113");
    //g_LastUseObject = serial;

    CPacketDoubleClickRequest(serial | 0x80000000).Send();
}
//----------------------------------------------------------------------------------
void COrion::Attack(uint serial)
{
    WISPFUN_DEBUG("c194_f114");
    if (g_ConfigManager.CriminalActionsQuery && g_World != NULL)
    {
        CGameCharacter *target = g_World->FindWorldCharacter(serial);

        if (target != NULL && (NOTORIETY_TYPE)g_Player->Notoriety == NT_INNOCENT &&
            (NOTORIETY_TYPE)target->Notoriety == NT_INNOCENT)
        {
            int x = g_ConfigManager.GameWindowX + (g_ConfigManager.GameWindowWidth / 2) - 40;
            int y = g_ConfigManager.GameWindowY + (g_ConfigManager.GameWindowHeight / 2) - 20;

            CGumpQuestion *newgump =
                new CGumpQuestion(0, x, y, CGumpQuestion::ID_GQ_STATE_ATTACK_REQUEST);
            newgump->ID = serial;

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
    WISPFUN_DEBUG("c194_f115");
    g_LastAttackObject = serial;

    //CPacketStatusRequest(serial).Send();

    CPacketAttackRequest(serial).Send();
}
//----------------------------------------------------------------------------------
void COrion::SendASCIIText(const char *str, SPEECH_TYPE type)
{
    WISPFUN_DEBUG("c194_f116");
    CPacketASCIISpeechRequest(str, type, 3, g_ConfigManager.SpeechColor).Send();
}
//----------------------------------------------------------------------------------
void COrion::CastSpell(int index)
{
    WISPFUN_DEBUG("c194_f117");
    if (index >= 0)
    {
        g_LastSpellIndex = index;

        CPacketCastSpell(index).Send();
    }
}
//----------------------------------------------------------------------------------
void COrion::CastSpellFromBook(int index, uint serial)
{
    WISPFUN_DEBUG("c194_f118");
    if (index >= 0)
    {
        g_LastSpellIndex = index;

        CPacketCastSpellFromBook(index, serial).Send();
    }
}
//----------------------------------------------------------------------------------
void COrion::UseSkill(int index)
{
    WISPFUN_DEBUG("c194_f119");
    if (index >= 0)
    {
        g_LastSkillIndex = index;

        CPacketUseSkill(index).Send();
    }
}
//----------------------------------------------------------------------------------
void COrion::OpenDoor()
{
    WISPFUN_DEBUG("c194_f120");
    CPacketOpenDoor().Send();
}
//----------------------------------------------------------------------------------
void COrion::EmoteAction(const char *action)
{
    WISPFUN_DEBUG("c194_f121");
    CPacketEmoteAction(action).Send();
}
//----------------------------------------------------------------------------------
void COrion::AllNames()
{
    WISPFUN_DEBUG("c194_f122");
    CGameObject *obj = g_World->m_Items;

    while (obj != NULL)
    {
        if ((obj->NPC && !obj->IsPlayer()) || obj->IsCorpse())
            CPacketClickRequest(obj->Serial).Send();

        obj = (CGameObject *)obj->m_Next;
    }
}
//----------------------------------------------------------------------------------
void COrion::RemoveRangedObjects()
{
    WISPFUN_DEBUG("c194_f123");
    if (g_World != NULL)
    {
        int objectsRange = g_ConfigManager.UpdateRange;

        CGameObject *go = g_World->m_Items;

        while (go != NULL)
        {
            CGameObject *next = (CGameObject *)go->m_Next;

            if (go->Container == 0xFFFFFFFF && !go->IsPlayer())
            {
                if (go->NPC)
                {
                    if (GetRemoveDistance(g_RemoveRangeXY, go) > objectsRange)
                    {
                        if (g_Party.Contains(go->Serial))
                        {
                            go->RemoveRender();
                            g_GumpManager.UpdateContent(go->Serial, 0, GT_STATUSBAR);
                        }
                        else
                            g_World->RemoveObject(go);
                    }
                }
                else if (((CGameItem *)go)->MultiBody)
                {
                    if (!CheckMultiDistance(g_RemoveRangeXY, go, objectsRange))
                        ((CGameItem *)go)->ClearMultiItems();
                    //g_World->RemoveObject(go);
                }
                else if (GetRemoveDistance(g_RemoveRangeXY, go) > objectsRange)
                    g_World->RemoveObject(go);
            }

            go = next;
        }
    }

    g_EffectManager.RemoveRangedEffects();
}
//----------------------------------------------------------------------------------
void COrion::ClearWorld()
{
    g_CorpseManager.Clear();
    g_Walker.Reset();
    g_ObjectInHand.Clear();
    g_UseItemActions.Clear();

    g_Ping = 0;
    g_ClickObject.Clear();
    g_Weather.Reset();
    g_ConsolePrompt = PT_NONE;
    g_MacroPointer = NULL;
    g_Season = ST_SUMMER;
    g_OldSeason = ST_SUMMER;
    g_GlobalScale = 1.0;
    g_PathFinder.BlockMoving = false;
    g_SkillsManager.SkillsTotal = 0.0f;
    g_SkillsManager.SkillsRequested = false;

    RELEASE_POINTER(g_World)
    LOG("\tWorld removed?\n");

    g_PopupMenu = NULL;

    g_GumpManager.Clear();
    LOG("\tGump Manager cleared?\n");

    g_EffectManager.Clear();
    LOG("\tEffect List cleared?\n");

    g_GameConsole.Clear();

    g_EntryPointer = NULL;
    g_GrayMenuCount = 0;

    g_Target.Reset();

    g_SystemChat.Clear();
    LOG("\tSystem chat cleared?\n");

    g_Journal.Clear();
    LOG("\tJournal cleared?\n");

    g_MapManager.Clear();
    LOG("\tMap cleared?\n");

    g_CurrentMap = 0;

    g_Party.Leader = 0;
    g_Party.Inviter = 0;
    g_Party.Clear();

    g_Ability[0] = AT_DISARM;
    g_Ability[1] = AT_PARALYZING_BLOW;

    g_ResizedGump = NULL;

    g_DrawStatLockers = false;
}
//----------------------------------------------------------------------------------
void COrion::LogOut()
{
    WISPFUN_DEBUG("c194_f124");
    LOG("COrion::LogOut->Start\n");
    SaveLocalConfig(g_PacketManager.ConfigSerial);

    if (g_SendLogoutNotification)
        CPacketLogoutNotification().Send();

    Disconnect();
    LOG("\tDisconnected?\n");

    ClearWorld();

    LOG("COrion::LogOut->End\n");
    InitScreen(GS_MAIN);
}
//----------------------------------------------------------------------------------
void COrion::ConsolePromptSend()
{
    WISPFUN_DEBUG("c194_f125");
    size_t len = g_GameConsole.Length();
    bool cancel = (len < 1);

    if (g_ConsolePrompt == PT_ASCII)
        CPacketASCIIPromptResponse(g_GameConsole.c_str(), len, cancel).Send();
    else if (g_ConsolePrompt == PT_UNICODE)
        CPacketUnicodePromptResponse(g_GameConsole.Data(), len, g_Language, cancel).Send();

    g_ConsolePrompt = PT_NONE;
}
//----------------------------------------------------------------------------------
void COrion::ConsolePromptCancel()
{
    WISPFUN_DEBUG("c194_f126");
    if (g_ConsolePrompt == PT_ASCII)
        CPacketASCIIPromptResponse("", 0, true).Send();
    else if (g_ConsolePrompt == PT_UNICODE)
        CPacketUnicodePromptResponse(L"", 0, g_Language, true).Send();

    g_ConsolePrompt = PT_NONE;
}
//----------------------------------------------------------------------------------
uint64 COrion::GetLandFlags(ushort id)
{
    WISPFUN_DEBUG("c194_f127");

    if (id < m_LandData.size())
        return m_LandData[id].Flags;

    return 0;
}
//----------------------------------------------------------------------------------
uint64 COrion::GetStaticFlags(ushort id)
{
    WISPFUN_DEBUG("c194_f128");

    if (id < (int)m_StaticData.size())
        return m_StaticData[id].Flags;

    return 0;
}
//----------------------------------------------------------------------------------
WISP_GEOMETRY::CSize COrion::GetStaticArtDimension(ushort id)
{
    WISPFUN_DEBUG("c194_f129");

    CGLTexture *th = ExecuteStaticArt(id);

    if (th != NULL)
        return WISP_GEOMETRY::CSize(th->Width, th->Height);

    return WISP_GEOMETRY::CSize();
}
//----------------------------------------------------------------------------------
WISP_GEOMETRY::CSize COrion::GetGumpDimension(ushort id)
{
    WISPFUN_DEBUG("c194_f131");
    CGLTexture *th = ExecuteGump(id);

    if (th != NULL)
        return WISP_GEOMETRY::CSize(th->Width, th->Height);

    return WISP_GEOMETRY::CSize();
}
//----------------------------------------------------------------------------------
void COrion::OpenStatus(uint serial)
{
    WISPFUN_DEBUG("c194_f133");
    int x = g_MouseManager.Position.X - 76;
    int y = g_MouseManager.Position.Y - 30;

    CPacketStatusRequest(serial).Send();

    g_GumpManager.AddGump(new CGumpStatusbar(serial, x, y, true));
}
//----------------------------------------------------------------------------------
void COrion::DisplayStatusbarGump(int serial, int x, int y)
{
    WISPFUN_DEBUG("c194_f134");
    CPacketStatusRequest packet(serial);
    SendMessage(
        g_OrionWindow.Handle, UOMSG_SEND, (WPARAM)packet.Data().data(), packet.Data().size());

    CGump *gump = g_GumpManager.GetGump(serial, 0, GT_STATUSBAR);

    if (gump != NULL)
    {
        if (gump->Minimized)
        {
            gump->MinimizedX = x;
            gump->MinimizedY = y;
        }
        else
        {
            gump->SetX(x);
            gump->SetY(y);
        }
    }
    else
        g_GumpManager.AddGump(new CGumpStatusbar(serial, x, y, true));
}
//----------------------------------------------------------------------------------
void COrion::OpenMinimap()
{
    WISPFUN_DEBUG("c194_f136");
    g_GumpManager.AddGump(new CGumpMinimap(0, 0, true));
}
//----------------------------------------------------------------------------------
void COrion::OpenWorldMap()
{
    WISPFUN_DEBUG("c194_f137");
    CPluginPacketOpenMap().SendToPlugin();

    /*int x = g_ConfigManager.GameWindowX + (g_ConfigManager.GameWindowWidth / 2) - 200;
	int y = g_ConfigManager.GameWindowY + (g_ConfigManager.GameWindowHeight / 2) - 150;

	CGumpWorldMap *gump = new CGumpWorldMap(x, y);
	gump->Called = true;

	g_GumpManager.AddGump(gump);*/
}
//----------------------------------------------------------------------------------
void COrion::OpenJournal()
{
    WISPFUN_DEBUG("c194_f138");
    g_GumpManager.AddGump(new CGumpJournal(0, 0, false, 250));
}
//----------------------------------------------------------------------------------
void COrion::OpenSkills()
{
    WISPFUN_DEBUG("c194_f139");

    g_SkillsManager.SkillsRequested = true;
    CPacketSkillsRequest(g_PlayerSerial).Send();
}
//----------------------------------------------------------------------------------
void COrion::OpenBackpack()
{
    WISPFUN_DEBUG("c194_f140");
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
    WISPFUN_DEBUG("c194_f141");
    int x = g_ConfigManager.GameWindowX + (g_ConfigManager.GameWindowWidth / 2) - 40;
    int y = g_ConfigManager.GameWindowY + (g_ConfigManager.GameWindowHeight / 2) - 20;

    g_GumpManager.AddGump(new CGumpQuestion(0, x, y, CGumpQuestion::ID_GQ_STATE_QUIT));

    InitScreen(GS_GAME_BLOCKED);
    g_GameBlockedScreen.Code = 3;
}
//----------------------------------------------------------------------------------
void COrion::OpenChat()
{
    WISPFUN_DEBUG("c194_f142");
    CPacketOpenChat(L"").Send();
}
//----------------------------------------------------------------------------------
void COrion::OpenConfiguration()
{
    WISPFUN_DEBUG("c194_f143");
    int x = (g_OrionWindow.GetSize().Width / 2) - 320;
    int y = (g_OrionWindow.GetSize().Height / 2) - 240;

    g_OptionsConfig = g_ConfigManager;

    g_GumpManager.AddGump(new CGumpOptions(x, y));
}
//----------------------------------------------------------------------------------
void COrion::OpenMail()
{
    WISPFUN_DEBUG("c194_f144");
}
//----------------------------------------------------------------------------------
void COrion::OpenPartyManifest()
{
    WISPFUN_DEBUG("c194_f145");
    int x = (g_OrionWindow.GetSize().Width / 2) - 272;
    int y = (g_OrionWindow.GetSize().Height / 2) - 240;

    g_GumpManager.AddGump(new CGumpPartyManifest(0, x, y, g_Party.CanLoot));
}
//----------------------------------------------------------------------------------
void COrion::OpenProfile(uint serial)
{
    WISPFUN_DEBUG("c194_f146");
    if (!serial)
        serial = g_PlayerSerial;

    CPacketProfileRequest(serial).Send();
}
//----------------------------------------------------------------------------------
void COrion::DisconnectGump()
{
    WISPFUN_DEBUG("c194_f149");
    CServer *server = g_ServerList.GetSelectedServer();
    string str = "Disconnected from " + (server != NULL ? server->Name : "server name...");
    g_Orion.CreateTextMessage(TT_SYSTEM, 0, 3, 0x21, str);

    int x = g_ConfigManager.GameWindowX + (g_ConfigManager.GameWindowWidth / 2) - 100;
    int y = g_ConfigManager.GameWindowY + (g_ConfigManager.GameWindowHeight / 2) - 62;

    CGumpNotify *gump =
        new CGumpNotify(x, y, CGumpNotify::ID_GN_STATE_LOGOUT, 200, 125, "Connection lost");

    g_GumpManager.AddGump(gump);

    g_Orion.InitScreen(GS_GAME_BLOCKED);
    g_GameBlockedScreen.Code = 0;
}
//----------------------------------------------------------------------------------
void COrion::OpenCombatBookGump()
{
    WISPFUN_DEBUG("c194_f150");
    int gameWindowCenterX = (g_ConfigManager.GameWindowX - 4) + g_ConfigManager.GameWindowWidth / 2;
    int gameWindowCenterY =
        (g_ConfigManager.GameWindowY - 4) + g_ConfigManager.GameWindowHeight / 2;

    int x = gameWindowCenterX - 200;
    int y = gameWindowCenterY - 100;

    if (x < 0)
        x = 0;

    if (y < 0)
        y = 0;

    g_GumpManager.AddGump(new CGumpCombatBook(x, y));
}
//----------------------------------------------------------------------------------
void COrion::OpenRacialAbilitiesBookGump()
{
    WISPFUN_DEBUG("c194_f151");
    int gameWindowCenterX = (g_ConfigManager.GameWindowX - 4) + g_ConfigManager.GameWindowWidth / 2;
    int gameWindowCenterY =
        (g_ConfigManager.GameWindowY - 4) + g_ConfigManager.GameWindowHeight / 2;

    int x = gameWindowCenterX - 200;
    int y = gameWindowCenterY - 100;

    if (x < 0)
        x = 0;

    if (y < 0)
        y = 0;

    g_GumpManager.AddGump(new CGumpRacialAbilitiesBook(x, y));
}
//----------------------------------------------------------------------------------
void COrion::StartReconnect()
{
    if (!g_ConnectionManager.Connected() || g_World == NULL)
    {
        LogOut();
        g_MainScreen.m_AutoLogin->Checked = true;
        InitScreen(GS_MAIN);
        g_OrionWindow.CreateTimer(COrionWindow::FASTLOGIN_TIMER_ID, 50);
    }
}
//----------------------------------------------------------------------------------
