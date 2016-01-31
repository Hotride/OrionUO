/****************************************************************************
**
** stdafx.h
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
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN
// Файлы заголовков Windows:
#include <windows.h>
// Файлы заголовков C RunTime
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <map>
#include <time.h>
#include "glew\include\GL\glew.h"
#include "glew\include\GL\wglew.h"
#include <gl\gl.h>
#include <gl\glu.h>
#include <winsock.h>
#include <deque>
#include <Shellapi.h>
#include <Shlwapi.h>
#include "zlib.h"
#include <Mmsystem.h>
#include "SDL2-2.0.3\include\SDL.h"
#include "SDL2-2.0.3\include\SDL_mixer.h"

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "Glu32.lib")
#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "zdll.lib")
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "SDL2-2.0.3\\lib\\x86\\SDL2.lib")
#pragma comment(lib, "SDL2-2.0.3\\lib\\x86\\SDL2_mixer.lib")

#pragma warning(disable: 4390)
#pragma warning(disable: 4800)

#include <string>
using std::string;
using std::wstring;
//---------------------------------------------------------------------------
#define UO_FW_SHARD 0

extern GLuint ShaderTexture;
extern GLuint ShaderDrawMode;
extern GLuint ShaderColorTable;
//---------------------------------------------------------------------------
extern int g_LandObjectsCount;
extern int g_StaticsObjectsCount;
extern int g_GameObjectsCount;
extern int g_MultiObjectsCount;
extern int g_RenderedObjectsCountInGameWindow;
//---------------------------------------------------------------------------
extern GLuint g_TextureLockedGump;
extern GLuint g_TextureUnlockedGump;
extern GLuint g_TextureGumpState[2];
extern bool g_UseFrameBuffer;
//---------------------------------------------------------------------------
extern bool g_AltPressed;
extern bool g_CtrlPressed;
extern bool g_ShiftPressed;
//---------------------------------------------------------------------------
extern bool g_MovingFromMouse;
extern bool g_AutoMoving;
//---------------------------------------------------------------------------
#include "EnumList.h"
#include "DefinitionMacro.h"
//---------------------------------------------------------------------------
const int GAME_FIGURE_GUMP_OFFSET = 11369;
const int TEXT_MESSAGE_MAX_WIDTH = 190;
//---------------------------------------------------------------------------
extern HINSTANCE g_Hinstance;
extern HWND g_hWnd;
extern int g_ClientWidth;
extern int g_ClientHeight;
extern string g_DirectoryPath;
extern string g_LastLoadFileError;
extern string g_ShardIP;
extern string g_ClientVersionText;
extern string g_AutoLoginNames;
extern wstring g_LastConsoleEntry;
extern int g_ShardPort;
extern DWORD g_LastSendTime;
extern DWORD g_LastPacketTime;
extern DWORD g_TotalSendSize;
extern DWORD g_TotalRecvSize;
extern bool g_ConfigLoaded;
extern GAME_STATE g_GameState;
extern DWORD g_CryptKey[3];
extern WORD g_SeedKey;
extern BYTE g_GameSeed[4];
extern BYTE g_LightLevel;
extern BYTE g_PersonalLightLevel;
extern bool g_InverseBuylist;
extern DWORD g_ClientFlag;
extern DWORD g_LockedClientFeatures;
extern short g_MapSizeX[6];
extern short g_MapSizeY[6];
extern short g_MapBlockX[6];
extern short g_MapBlockY[6];
extern GLuint g_MapTexture[6];
extern BYTE g_CurrentMap;
extern int g_GameWindowSizeX;
extern int g_GameWindowSizeY;
extern int g_GameWindowPosX;
extern int g_GameWindowPosY;
extern int g_CodePage;
extern BYTE g_ServerTimeHour;
extern BYTE g_ServerTimeMinute;
extern BYTE g_ServerTimeSecond;
extern BYTE g_PingCount;
extern BYTE g_PingSequence;
extern bool g_PacketLoginComplete;
extern bool g_GrayedPixels;
extern int g_MultiIndexCount;
extern BYTE g_WalkRequestCount;
extern DWORD g_LastStepTime;
extern float g_SkillsTotal;
extern DWORD g_PlayerSerial;
extern int g_GrayMenuCount;
extern bool g_JournalShowSystem;
extern bool g_JournalShowObjects;
extern bool g_JournalShowClient;
extern DWORD g_StatusbarUnderMouse;
extern DWORD g_DeathScreenTimer;
const DWORD g_DeathScreenDelay = 3000;
extern bool g_CancelDoubleClick;
extern PROMPT_TYPE g_ConsolePrompt;
extern BYTE g_LastASCIIPrompt[11];
extern BYTE g_LastUnicodePrompt[11];
extern bool g_DrawMode;
extern string g_Language;
extern WORD g_OutOfRangeColor;
extern bool g_SendLogoutNotification;
extern bool g_NPCPopupEnabled;
extern bool g_ChatEnabled;
extern BYTE g_Season;
extern float g_AnimCharactersDelayValue;
extern bool g_ShowGumpLocker;
extern DWORD g_PartyHelperTarget;
extern DWORD g_PartyHelperTimer;
extern float g_DrawColor;
extern bool g_NoDrawRoof;
extern char g_MaxGroundZ;
extern char g_SelecterCharName[30];
extern int g_ZBuffer;
extern bool g_UseCircleTrans;

const BYTE SMOOTH_MONITOR_NONE = 0;
const BYTE SMOOTH_MONITOR_SUNRISE = 1;
const BYTE SMOOTH_MONITOR_SUNSET = 2;

extern bool g_UseSmoothMonitor;
extern BYTE g_SmoothMonitorMode;
extern GLfloat g_SmoothMonitorColor;
extern GLfloat g_SmoothMonitorStep;
extern BYTE g_SmoothMonitorScale;
//---------------------------------------------------------------------------
extern int g_MouseX;
extern int g_MouseY;
extern int g_DroppedLeftMouseX;
extern int g_DroppedLeftMouseY;
extern int g_DroppedRightMouseX;
extern int g_DroppedRightMouseY;
extern bool g_RightMouseDown;
extern bool g_LeftMouseDown;
extern bool g_SelectGumpObjects;
//---------------------------------------------------------------------------
extern SELECT_OBJECT_TYPE g_LastObjectType;
extern int g_LastSelectedObject;
extern int g_LastObjectLeftMouseDown;
extern int g_LastObjectRightMouseDown;
extern DWORD g_LastSelectedGump;
extern DWORD g_LastGumpLeftMouseDown;
extern DWORD g_LastGumpMouseDownTime;
extern DWORD g_LastMouseDownTime;
extern DWORD g_LastGumpRightMouseDown;
extern bool g_GeneratedMouseDown;
extern DWORD g_LastLClickTime;
extern int g_LastSpellIndex;
extern int g_LastSkillIndex;
extern bool g_ClickObjectReq;
extern DWORD g_LastUseObject;
extern DWORD g_LastTargetObject;
extern DWORD g_LastAttackObject;
//---------------------------------------------------------------------------
extern DWORD g_LastRenderTime;
const int g_FrameDelay_ActiveWindow = 30;
const int g_FrameDelay_UnactiveWindow = 200;
static int g_FrameDelay[2] = {g_FrameDelay_UnactiveWindow, g_FrameDelay_ActiveWindow};
//---------------------------------------------------------------------------
const WORD g_SelectLandColor = 0x0044;
const WORD g_SelectStaticColor = 0x0077;
const WORD g_SelectMultiColor = 0x0066;
//---------------------------------------------------------------------------
const int g_MaleGumpOffset = 50000;
const int g_FemaleGumpOffset = 60000;
//---------------------------------------------------------------------------
extern int g_UpdateRange;
extern POINT g_RemoveRangeXY;
//---------------------------------------------------------------------------
const int RemoveObjectRange = 22; //25;
const int RemoveMultiRange = 35;
const int RemoveContainerGumpRange = 3;
//---------------------------------------------------------------------------
const int g_StepDelayMountRun = 85;
const int g_StepDelayMountWalk = 185;
const int g_StepDelayRun = 185;
const int g_StepDelayWalk = 385;
//---------------------------------------------------------------------------
const int g_CharacterAnimationDelayTable[2][2] =
{
	//without mount
	{
		//walk
		g_StepDelayWalk,
		//run
		g_StepDelayRun
	},
	//on mount
	{
		//walk
		g_StepDelayMountWalk,
		//run
		g_StepDelayMountRun
	}
};
//---------------------------------------------------------------------------
extern const bool g_LayerUnsafe[30];
//---------------------------------------------------------------------------
const int g_MaxBlockZ = 15;
const int g_MaxClimbZ = 9;
const int g_MaxFallZ = 19;
const int g_MaxClimbMapZ = 19;
const int g_CharacterHeight = 16;
//---------------------------------------------------------------------------
extern char g_SpellCircles[8][15];
const char *GetReagentName(WORD ID);
extern WORD g_SpellReagents[64][4];
extern char g_SpellCastAbr[64][8];
extern const char *g_CityNames[9];
//---------------------------------------------------------------------------
struct POINT3DF
{
	float x, y;
	char z;
};
//---------------------------------------------------------------------------
struct IMAGE_BOUNDS
{
	int X;
	int Y;
	int Width;
	int Height;

	bool InRect(IMAGE_BOUNDS &ib);
};
//---------------------------------------------------------------------------
class TRenderTextObject;
//---------------------------------------------------------------------------
struct TEXT_IMAGE_BOUNDS : public IMAGE_BOUNDS
{
	int X;
	int Y;
	int Width;
	int Height;
	TRenderTextObject *m_Text;

	bool InRect(TEXT_IMAGE_BOUNDS &ib);
};

extern IMAGE_BOUNDS g_PlayerRect;
//---------------------------------------------------------------------------
struct TARGET_GUMP_INFO
{
	int X;
	int Y;
	int HP;
	WORD Color;

	void Draw();
};

extern TARGET_GUMP_INFO TargetGump;
extern TARGET_GUMP_INFO AttackTargetGump;
//---------------------------------------------------------------------------
// перевод Радианов в Градусы
inline float deg2radf(float degr)
{
	return degr * (float)(M_PI / 180.0f);
}
//---------------------------------------------------------------------------
inline float rad2degf(float radians)
{
	return (float)(radians * 180.0f / M_PI);
}
//---------------------------------------------------------------------------
inline int RandomInt(int n)
{
	return (rand() % n);
}
//---------------------------------------------------------------------------
inline int RandomIntMinMax(int n, int m)
{
	return (rand() % (m - n) + n);
}
//---------------------------------------------------------------------------
int CalculateSphereOffset(int max, int current, int maxValue, float divizor = 100.0f);
int CalculatePercents(int max, int current, int maxValue);
//---------------------------------------------------------------------------
inline DWORD unpack32(PBYTE buf)
{
	return (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
}
//---------------------------------------------------------------------------
inline WORD unpack16(PBYTE buf)
{
	return (buf[0] << 8) | buf[1];
}
//---------------------------------------------------------------------------
void UnuseShader();
//---------------------------------------------------------------------------
inline void pack32(PBYTE buf, DWORD x)
{
	buf[0] = BYTE(x >> 24);
	buf[1] = BYTE((x >> 16) & 0xff);
	buf[2] = BYTE((x >> 8) & 0xff);
	buf[3] = BYTE(x & 0xff);
}
//---------------------------------------------------------------------------
inline void pack16(PBYTE buf, WORD x)
{
	buf[0] = x >> 8;
	buf[1] = x & 0xff;
}
//---------------------------------------------------------------------------
void error_printf(const char * format, ...) GCC_PRINTF(1,2);
void warning_printf(const char * format, ...) GCC_PRINTF(1,2);
void trace_printf(const char * format, ...) GCC_PRINTF(1,2);
void trace_dump(PBYTE buf, int length);
//---------------------------------------------------------------------------
string FilePath(string fName);
string ToString(wstring wstr);
wstring ToWString(string str);
//---------------------------------------------------------------------------
int gumpuucode2str(const wchar_t *wstr, int wlength, char *receiver, int maxsize);
int uucode2str(const wchar_t *wstr, int wlength, char *receiver, int maxsize);
int str2uucode(const char *str, int length, wchar_t* wreceiver, int wmaxsize);
//---------------------------------------------------------------------------
inline bool IsBackground(DWORD Flags) {return (Flags & 0x00000001);}
inline bool IsWeapon(DWORD Flags) {return (Flags & 0x00000002);}
inline bool IsTransparent(DWORD Flags) {return (Flags & 0x00000004);}
inline bool IsTranslucent(DWORD Flags) {return (Flags & 0x00000008);}
inline bool IsWall(DWORD Flags) {return (Flags & 0x00000010);}
inline bool IsDamaging(DWORD Flags) {return (Flags & 0x00000020);}
inline bool IsImpassable(DWORD Flags) {return (Flags & 0x00000040);}
inline bool IsWet(DWORD Flags) {return (Flags & 0x00000080);}
inline bool IsUnknown(DWORD Flags) {return (Flags & 0x00000100);}
inline bool IsSurface(DWORD Flags) {return (Flags & 0x00000200);}
inline bool IsBridge(DWORD Flags) {return (Flags & 0x00000400);}
inline bool IsStackable(DWORD Flags) {return (Flags & 0x00000800);}
inline bool IsWindow(DWORD Flags) {return (Flags & 0x00001000);}
inline bool IsNoShoot(DWORD Flags) {return (Flags & 0x00002000);}
inline bool IsPrefixA(DWORD Flags) {return (Flags & 0x00004000);}
inline bool IsPrefixAn(DWORD Flags) {return (Flags & 0x00008000);}
inline bool IsInternal(DWORD Flags) {return (Flags & 0x00010000);}
inline bool IsFoliage(DWORD Flags) {return (Flags & 0x00020000);}
inline bool IsPartialHue(DWORD Flags) {return (Flags & 0x00040000);}
inline bool IsUnknown1(DWORD Flags) {return (Flags & 0x00080000);}
inline bool IsMap(DWORD Flags) {return (Flags & 0x00100000);}
inline bool IsContainer(DWORD Flags) {return (Flags & 0x00200000);}
inline bool IsWearable(DWORD Flags) {return (Flags & 0x00400000);}
inline bool IsLightSource(DWORD Flags) {return (Flags & 0x00800000);}
inline bool IsAnimated(DWORD Flags) {return (Flags & 0x01000000);}
inline bool IsNoDiagonal(DWORD Flags) {return (Flags & 0x02000000);}
inline bool IsUnknown2(DWORD Flags) {return (Flags & 0x04000000);}
inline bool IsArmor(DWORD Flags) {return (Flags & 0x08000000);}
inline bool IsRoof(DWORD Flags) {return (Flags & 0x10000000);}
inline bool IsDoor(DWORD Flags) {return (Flags & 0x20000000);}
inline bool IsStairBack(DWORD Flags) {return (Flags & 0x40000000);}
inline bool IsStairRight(DWORD Flags) {return (Flags & 0x80000000);}
//---------------------------------------------------------------------------











#include <vector>
using std::vector;
//---------------------------------------------------------------------------
typedef struct MULTILINES_FONT_DATA
{
	wchar_t item;
	WORD flags;
	BYTE font;
	WORD linkID;
	DWORD color;

	MULTILINES_FONT_DATA *Next;
} *PMULTILINES_FONT_DATA;
//---------------------------------------------------------------------------
typedef struct MULTILINES_FONT_INFO
{
	int Width;
	int IndentionOffset;
	int MaxHeight;
	int CharStart;
	int CharCount;
	TEXT_ALIGN_TYPE Align;
	vector<MULTILINES_FONT_DATA> Data;

	MULTILINES_FONT_INFO *m_Next;

	void Reset()
	{
		Width = 0;
		IndentionOffset = 0;
		MaxHeight = 0;
		CharStart = 0;
		CharCount = 0;
		Align = TS_LEFT;
		m_Next = NULL;
	}
} *PMULTILINES_FONT_INFO;

#define DPOLY(x, y, w, h) g_GL.DrawPolygone(0x7f7f7f7f, x, y, w, h)






//---------------------------------------------------------------------------
#include "CommonInterfaces.h"
#include "MulStruct.h"
#include "ExceptionFilter.h"
#include "network\\platform.h"
#include "network\\aes.h"
#include "network\\table.h"
#include "network\\UOHuffman.h"
#include "network\\Packet.h"
#include "network\\LoginCrypt.h"
#include "network\\GameCrypt.h"
#include "network\\Connection.h"
#include "managers\\ConnectionManager.h"
#include "managers\\PacketManager.h"
#include "managers\\FileManager.h"
#include "BaseQueue.h"
#include "TextureObject.h"
#include "IndexObject.h"
#include "LinkedList.h"
#include "EntryText.h"
#include "BookEntryText.h"
#include "GameConsole.h"
#include "Logger.h"
#include "TextData.h"
#include "GLEngine.h"
#include "Shader.h"
#include "FrameBuffer.h"
#include "RenderObject.h"
#include "Multi.h"
#include "game object\\GameEffect.h"
#include "game object\\GameObject.h"
#include "game object\\ShopItem.h"
#include "game object\\GameItem.h"
#include "game object\\ObjectOnCursor.h"
#include "Walker.h"
#include "game object\\GameCharacter.h"
#include "Skills.h"
#include "game object\\GamePlayer.h"
#include "game object\\GameWorld.h"
#include "Tooltip.h"
#include "game object\\ClickObject.h"
#include "Target.h"
#include "Party.h"
#include "ServerList.h"
#include "CharacterList.h"
#include "Profession.h"
#include "Target.h"
#include "SkillGroup.h"
#include "Weather.h"
#include "MapObject.h"
#include "MapBlock.h"
#include "Container.h"
#include "QuestArrow.h"
#include "MultiMap.h"
#include "PathFinder.h"
#include "Macro.h"
#include "CityList.h"
#include "VideoRecorder.h"
#include "gump\\GumpObject.h"
#include "gump\\Gump.h"
#include "gump\\GumpPaperdoll.h"
#include "gump\\GumpMinimap.h"
#include "gump\\GumpDrag.h"
#include "gump\\GumpContainer.h"
#include "gump\\GumpGrayMenu.h"
#include "gump\\GumpMenu.h"
#include "gump\\GumpMenubar.h"
#include "gump\\GumpNotify.h"
#include "gump\\GumpQuestion.h"
#include "gump\\GumpSelectFont.h"
#include "gump\\GumpStatusbar.h"
#include "gump\\GumpJournal.h"
#include "gump\\GumpOptions.h"
#include "gump\\GumpSecureTrading.h"
#include "gump\\GumpSelectColor.h"
#include "gump\\GumpSpell.h"
#include "gump\\GumpSpellbook.h"
#include "gump\\GumpTextEntryDialog.h"
#include "gump\\GumpTip.h"
#include "gump\\GumpGeneric.h"
#include "gump\\GumpSkills.h"
#include "gump\\GumpBuy.h"
#include "gump\\GumpSell.h"
#include "gump\\GumpMap.h"
#include "gump\\GumpDye.h"
#include "gump\\GumpProfile.h"
#include "gump\\GumpPartyManifest.h"
#include "gump\\GumpBook.h"
#include "gump\\GumpBulletinBoard.h"
#include "gump\\GumpBulletinBoardItem.h"
#include "gump\\GumpWorldMap.h"
#include "screen stages\\BaseScreen.h"
#include "screen stages\\DebugScreen.h"
#include "screen stages\\MainScreen.h"
#include "screen stages\\ConnectionScreen.h"
#include "screen stages\\ServerScreen.h"
#include "screen stages\\CharacterListScreen.h"
#include "screen stages\\CreateCharacterScreen.h"
#include "screen stages\\SelectProfessionScreen.h"
#include "screen stages\\SelectTownScreen.h"
#include "screen stages\\GameScreen.h"
#include "screen stages\\GameBlockedScreen.h"
#include "managers\\CreateCharacterManager.h"
#include "managers\\SkillGroupManager.h"
#include "managers\\GumpManager.h"
#include "managers\\EffectManager.h"
#include "managers\\ColorManager.h"
#include "managers\\ClilocManager.h"
#include "managers\\ConfigManager.h"
#include "managers\\FontsManager.h"
#include "managers\\AnimationManager.h"
#include "managers\\MapManager.h"
#include "managers\\MouseManager.h"
#include "managers\\MulReader.h"
#include "managers\\SoundManager.h"
#include "managers\\MacroManager.h"
#include "managers\\OptionsMacroManager.h"
#include "managers\\CityManager.h"
#include "managers\\PluginManager.h"
#include "ClientUO.h"
//---------------------------------------------------------------------------