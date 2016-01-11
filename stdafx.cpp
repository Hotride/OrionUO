/****************************************************************************
**
** stdafx.cpp
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
#include "stdafx.h"

GLhandleARB ShaderProg = 0;
GLhandleARB VertShader = 0;
GLhandleARB FragShader = 0;
GLuint ShaderTexture = 0;
//---------------------------------------------------------------------------
int g_LandObjectsCount = 0;
int g_StaticsObjectsCount = 0;
int g_GameObjectsCount = 0;
int g_MultiObjectsCount = 0;
int g_RenderedObjectsCountInGameWindow = 0;
//---------------------------------------------------------------------------
GLuint g_TextureLockedGump = 0;
GLuint g_TextureUnlockedGump = 0;
GLuint g_TextureGumpState[2] = {0, 0};
bool g_UseFrameBuffer = false;
//---------------------------------------------------------------------------
bool g_MovingFromMouse = false;
bool g_AutoMoving = false;
//---------------------------------------------------------------------------
bool g_AltPressed = false;
bool g_CtrlPressed = false;
bool g_ShiftPressed = false;
//---------------------------------------------------------------------------
HINSTANCE g_Hinstance = NULL;
HWND g_hWnd = NULL;
int g_ClientWidth = 0;
int g_ClientHeight = 0;
string g_DirectoryPath = "";
string g_LastLoadFileError = "";
string g_ShardIP = "";
string g_ClientVersionText = "";
string g_AutoLoginNames = "";
wstring g_LastConsoleEntry = L"";
int g_ShardPort = 2593;
DWORD g_LastSendTime = 0;
DWORD g_LastPacketTime = 0;
DWORD g_TotalSendSize = 0;
DWORD g_TotalRecvSize = 0;
bool g_ConfigLoaded = false;
GAME_STATE g_GameState = GS_MAIN;
DWORD g_CryptKey[3] = {0};
WORD g_SeedKey = 0;
BYTE g_GameSeed[4] = {0};
BYTE g_LightLevel = 0;
BYTE g_PersonalLightLevel = 0;
bool g_InverseBuylist = false;
DWORD g_ClientFlag = 0;
DWORD g_LockedClientFeatures = 0;
short g_MapSizeX[6] = {6144, 7168, 2304, 2560, 1448, 1280};
short g_MapSizeY[6] = {4096, 4096, 1600, 2048, 1448, 4096};
short g_MapBlockX[6] = {768, 896, 288, 320, 181, 160};
short g_MapBlockY[6] = {512, 512, 200, 256, 181, 512};
GLuint g_MapTexture[6] = {0, 0, 0, 0, 0, 0};
BYTE g_CurrentMap = 0;
int g_GameWindowSizeX = 1024; //800;
int g_GameWindowSizeY = 768; //600;
int g_GameWindowPosX = 0;
int g_GameWindowPosY = 0;
int g_CodePage = 0;
BYTE g_ServerTimeHour = 0;
BYTE g_ServerTimeMinute = 0;
BYTE g_ServerTimeSecond = 0;
BYTE g_PingCount = 0;
BYTE g_PingSequence = 0;
bool g_PacketLoginComplete = false;
bool g_GrayedPixels = false;
int g_MultiIndexCount = 0;
BYTE g_WalkRequestCount = 0;
DWORD g_LastStepTime = 0;
float g_SkillsTotal = 0.0f;
DWORD g_PlayerSerial = 0;
int g_GrayMenuCount = 0;
bool g_JournalShowSystem = true;
bool g_JournalShowObjects = true;
bool g_JournalShowClient = true;
DWORD g_StatusbarUnderMouse = 0;
DWORD g_DeathScreenTimer = 0;
bool g_CancelDoubleClick = false;
PROMPT_TYPE g_ConsolePrompt = PT_NONE;
BYTE g_LastASCIIPrompt[11] = {0};
BYTE g_LastUnicodePrompt[11] = {0};
bool g_DrawMode = false;
string g_Language = "RUS";
WORD g_OutOfRangeColor = 0;
bool g_SendLogoutNotification = false;
bool g_NPCPopupEnabled = false;
bool g_ChatEnabled = false;
BYTE g_Season = 0;
float g_AnimCharactersDelayValue = 0x50;
bool g_ShowGumpLocker = false;
POINT g_RemoveRangeXY = {0, 0};
DWORD g_PartyHelperTarget = 0;
DWORD g_PartyHelperTimer = 0;
float g_DrawColor = 1.0f;
bool g_NoDrawRoof = false;
char g_MaxGroundZ = 0;
char g_SelecterCharName[30] = { 0 };
bool g_UseSmoothMonitor = false;
BYTE g_SmoothMonitorMode = 0;
GLfloat g_SmoothMonitorColor = 0.0f;
GLfloat g_SmoothMonitorStep = 0.03f;
BYTE g_SmoothMonitorScale = 15;
float g_ZBuffer = 0.0f;

int g_MouseX = 0;
int g_MouseY = 0;
int g_DroppedLeftMouseX = 0;
int g_DroppedLeftMouseY = 0;
int g_DroppedRightMouseX = 0;
int g_DroppedRightMouseY = 0;
bool g_RightMouseDown = false;
bool g_LeftMouseDown = false;
bool g_SelectGumpObjects = false;

SELECT_OBJECT_TYPE g_LastObjectType = SOT_NO_OBJECT;
int g_LastSelectedObject = 0;
int g_LastObjectLeftMouseDown = 0;
int g_LastObjectRightMouseDown = 0;
DWORD g_LastSelectedGump = 0;
DWORD g_LastGumpLeftMouseDown = 0;
DWORD g_LastGumpMouseDownTime = 0;
DWORD g_LastMouseDownTime = 0;
DWORD g_LastGumpRightMouseDown = 0;
bool g_GeneratedMouseDown = false;
DWORD g_LastLClickTime = 0;
int g_LastSpellIndex = 1;
int g_LastSkillIndex = 1;
bool g_ClickObjectReq = false;
DWORD g_LastUseObject = 0;
DWORD g_LastTargetObject = 0;
DWORD g_LastAttackObject = 0;
IMAGE_BOUNDS g_PlayerRect = {0, 0, 0, 0};
int g_UpdateRange = 18;

DWORD g_LastRenderTime = 0;
DWORD g_FrameDelay = 0x1E;

TARGET_GUMP_INFO TargetGump = {0, 0, 0, 0};
TARGET_GUMP_INFO AttackTargetGump = {0, 0, 0, 0};
//--------------------------------------------------------------------------
bool IMAGE_BOUNDS::InRect(IMAGE_BOUNDS &ib)
{
	bool inRect = false;

	if (X < ib.X)
	{
		if (ib.X < X + Width)
			inRect = true;
	}
	else
	{
		if (X < ib.X + ib.Width)
			inRect = true;
	}

	if (inRect)
	{
		if (Y < ib.Y)
		{
			if (ib.Y < Y + Height)
				inRect = true;
			else
				inRect = false;
		}
		else
		{
			if (Y < ib.Y + ib.Height)
				inRect = true;
			else
				inRect = false;
		}
	}

	return inRect;
}
//--------------------------------------------------------------------------
bool TEXT_IMAGE_BOUNDS::InRect(TEXT_IMAGE_BOUNDS &ib)
{
	bool inRect = false;

	if (X < ib.X)
	{
		if (ib.X < X + Width)
			inRect = true;
	}
	else
	{
		if (X < ib.X + ib.Width)
			inRect = true;
	}

	if (inRect)
	{
		if (Y < ib.Y)
		{
			if (ib.Y < Y + Height)
				inRect = true;
			else
				inRect = false;
		}
		else
		{
			if (Y < ib.Y + ib.Height)
				inRect = true;
			else
				inRect = false;
		}
	}

	return inRect;
}
//--------------------------------------------------------------------------
void TARGET_GUMP_INFO::Draw()
{
	if (Color != 0)
	{
		UO->DrawGump(0x1068, Color, X, Y);

		if (HP > 0)
			UO->DrawGump(0x1069, Color, X, Y, HP, 0);
	}
}
//--------------------------------------------------------------------------
int CalculateSphereOffset(int max, int current, int maxValue, float divizor)
{
	if (max > 0)
	{
		max = (int)((current / divizor) * (float)max);

		max = (maxValue * max) / 100;
		
		if (max < 0)
			max = 0;
	}

	return max;
}
//--------------------------------------------------------------------------
int CalculatePercents(int max, int current, int maxValue)
{
	if (max > 0)
	{
		max = (current * 100) / max;
		if (max > 100)
			max = 100;

		if (max > 1)
			max = (maxValue * max) / 100;
	}

	return max;
}
//--------------------------------------------------------------------------
const bool g_LayerUnsafe[30] =
{
	false,   //0
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	false,
	true,
	true,
	true,
	false,
	false,
	true,
	true,
	true,
	true,
	false, //0x15
	true,
	true,
	true,
	false,
	false,
	false,
	false,
	false
};
//--------------------------------------------------------------------------
const char *g_CityNames[9] =
{
	"Yew",
	"Minoc",
	"Britain",
	"Moonglow",
	"Trinsic",
	"Magincia",
	"Jhelom",
	"Skara Brae",
	"Vesper"
};
//--------------------------------------------------------------------------
char g_SpellCircles[8][15] =
{
	"First Circle",
	"Second Circle",
	"Third Circle",
	"Fourth Circle",
	"Fifth Circle",
	"Sixth Circle",
	"Seventh Circle",
	"Eighth Circle"
};
//---------------------------------------------------------------------------
const char *GetReagentName(WORD ID)
{
	switch (ID)
	{
		case 0x0F7A: return "Black pearl";
		case 0x0F7B: return "Bloodmoss";
		case 0x0F84: return "Garlic";
		case 0x0F85: return "Ginseng";
		case 0x0F86: return "Mandrake root";
		case 0x0F88: return "Nightshade";
		case 0x0F8C: return "Sulfurous ash";
		case 0x0F8D: return "Spiders silk";
		default: break;
	}

	return "";
}
//---------------------------------------------------------------------------
WORD g_SpellReagents[64][4] = 
{
	{0x0F7B,0x0F88,0,0},
	{0x0F84,0x0F85,0x0F86,0},
	{0x0F85,0x0F88,0,0},
	{0x0F84,0x0F85,0x0F8D,0},
	{0x0F7A,0x0F88,0,0},
	{0x0F8C,0x0F8D,0,0},
	{0x0F84,0x0F8C,0x0F8D,0},
	{0x0F84,0x0F88,0,0},
	{0x0F7B,0x0F86,0,0},
	{0x0F86,0x0F88,0,0},
	{0x0F84,0x0F85,0,0},
	{0x0F88,0x0F8D,0,0},
	{0x0F84,0x0F8C,0x0F8D,0},
	{0x0F7B,0x0F8C,0,0},
	{0x0F84,0x0F85,0x0F8C,0},
	{0x0F86,0x0F88,0,0},
	{0x0F84,0x0F86,0,0},
	{0x0F7A,0x0F8C,0,0},
	{0x0F8C,0x0F7B,0x0F84,0},
	{0x0F88,0,0,0},
	{0x0F7B,0x0F86,0,0},
	{0x0F7B,0x0F86,0,0},
	{0x0F7B,0x0F8C,0,0},
	{0x0F7B,0x0F84,0,0},
	{0x0F84,0x0F85,0x0F86,0},
	{0x0F84,0x0F85,0x0F86,0x0F8C},
	{0x0F84,0x0F88,0x0F8C,0},
	{0x0F7A,0x0F8D,0x0F8C,0},
	{0x0F84,0x0F85,0x0F86,0x0F8D},
	{0x0F7A,0x0F86,0x0F8C,0},
	{0x0F7A,0x0F86,0x0F8D,0},
	{0x0F7A,0x0F7B,0x0F86,0},
	{0x0F7A,0x0F86,0x0F88,0},
	{0x0F84,0x0F7A,0x0F8D,0x0F8C},
	{0x0F7B,0x0F84,0x0F88,0},
	{0x0F84,0x0F86,0x0F8D,0},
	{0x0F7A,0x0F86,0x0F88,0x0F8C},
	{0x0F84,0x0F86,0x0F8D,0},
	{0x0F7A,0x0F88,0x0F8D,0},
	{0x0F7B,0x0F86,0x0F8D,0},
	{0x0F84,0x0F86,0x0F8C,0},
	{0x0F7A,0x0F88,0,0},
	{0x0F7A,0x0F86,0x0F8C,0},
	{0x0F7B,0x0F88,0,0},
	{0x0F7A,0x0F7B,0x0F86,0},
	{0x0F84,0x0F86,0x0F88,0x0F8C},
	{0x0F7A,0x0F85,0x0F8D,0},
	{0x0F7B,0x0F8C,0,0},
	{0x0F7A,0x0F86,0x0F7B,0x0F8C},
	{0x0F7A,0x0F86,0x0F8D,0x0F8C},
	{0x0F8D,0x0F8C,0,0},
	{0x0F7A,0x0F86,0x0F8C,0},
	{0x0F7A,0x0F7B,0x0F86,0x0F8D},
	{0x0F7A,0x0F84,0x0F86,0x0F8C},
	{0x0F7B,0x0F8D,0x0F86,0x0F8C},
	{0x0F7B,0x0F86,0x0F8D,0},
	{0x0F7B,0x0F85,0x0F86,0x0F8C},
	{0x0F7A,0x0F7B,0x0F86,0x0F88},
	{0x0F7B,0x0F84,0x0F85,0},
	{0x0F7B,0x0F86,0x0F8D,0},
	{0x0F7B,0x0F86,0x0F8D,0x0F8C},
	{0x0F7B,0x0F86,0x0F8D,0},
	{0x0F7B,0x0F86,0x0F8D,0x0F8C},
	{0x0F7B,0x0F86,0x0F8D,0}
};
//---------------------------------------------------------------------------
char g_SpellCastAbr[64][8] =
{
	"U J",
	"I M Y",
	"R W",
	"I M",
	"I P Y",
	"I L",
	"S F",
	"D M",
	"E U",
	"U W",
	"A N",
	"A M",
	"I J",
	"A J",
	"U S",
	"U M",
	"R S",
	"V F",
	"A P",
	"I N",
	"O P Y",
	"R P",
	"E P",
	"I S Y",
	"W A N",
	"V U S",
	"D S",
	"I F G",
	"I V M",
	"P O G",
	"O R",
	"K O P",
	"I J H Y",
	"A G",
	"K I E",
	"I J S",
	"P C W",
	"A E P",
	"I N G",
	"K X",
	"A O",
	"C P",
	"V O F",
	"K P Y",
	"V D S",
	"I E G",
	"W Q",
	"V O G",
	"I S G",
	"K V F",
	"V R P",
	"O S",
	"V A O",
	"K D F Y",
	"V Y R",
	"I V P",
	"V C P",
	"A C",
	"K V X A",
	"K V X D",
	"K V X T",
	"K V X F",
	"K V X A"
};
//---------------------------------------------------------------------------
string FilePath(string fName) {return g_DirectoryPath + "\\" + fName;}
//---------------------------------------------------------------------------
string ToString(wstring wstr)
{
	int size = wstr.length();
	string str = "";

	if (size > 0)
	{
		char *text = new char[size + 1];
		int len = uucode2str(wstr.c_str(), size, &text[0], size);
		text[size] = 0;
		str = text;
		delete text;
	}

	return str;
}
//---------------------------------------------------------------------------
wstring ToWString(string str)
{
	int size = str.length();
	wstring wstr = L"";

	if (size)
	{
		wchar_t *wbuf = new wchar_t[size + 1];
		MultiByteToWideChar(GetACP(), 0, str.c_str(), size, &wbuf[0], size);
		wbuf[size] = 0;
		wstr = wbuf;
		delete wbuf;
	}

	return wstr;
}
//---------------------------------------------------------------------------
int gumpuucode2str(const wchar_t* wstr,int wlength,LPSTR receiver,int maxsize)
{
	if (!wlength || wlength < -1 || maxsize < 1)
		return 0;

	int msize = ((wlength < 0) ? (maxsize * 2 + 2) : (wlength * 2 + 2));
	PBYTE buf = new BYTE[msize];
	memcpy(buf, wstr, msize - 2);
	int len = 0;
	PBYTE p = buf + 1;

	while (len < maxsize)
	{
		if (wlength < 0)
			if (!*(p - 1) && !*p)
			{
				len++;
				break;
			}
			else;

		if (len >= wlength)
			break;

		BYTE c = *(p - 1);
		*(p - 1) = *p;
		*p = c;
		len++;
		p += 2;
	}

	len = WideCharToMultiByte(GetACP(), 0, (LPCWSTR)buf, len, receiver, maxsize, 0, 0);
	delete buf;

	if (len < maxsize && len && receiver[len - 1])
		receiver[len++] = 0;

	return len;
}
//---------------------------------------------------------------------------
int uucode2str(const wchar_t* wstr, int wlength, LPSTR receiver, int maxsize)
{
	if (!wlength || wlength < -1 || maxsize < 1)
		return 0;

	int msize = ((wlength < 0) ? (maxsize * 2 + 2) : (wlength * 2 + 2));
	int len = lstrlenW(wstr);
	len = WideCharToMultiByte(GetACP(), 0, wstr, len, receiver, maxsize, 0, 0);

	if (len < maxsize && len && receiver[len - 1])
		receiver[len++] = 0;

	return len;
}
//--------------------------------------------------------------------------
int str2uucode(const char* str,int length,wchar_t* wreceiver,int wmaxsize)
{
	if (!length || length < -1 || wmaxsize < 1 || !wreceiver)
		return 0;

	int len = 0, i;
	wreceiver[0] = 0;
	len = MultiByteToWideChar(GetACP(), 0, str, length, wreceiver, wmaxsize);
	PBYTE p = (PBYTE)wreceiver + 1;

	for (i = 0; i < len && (*(p - 1) || *p) && i < wmaxsize; i++, p += 2)
	{
		BYTE c = *(p - 1);
		*(p - 1) = *p;
		*p = c;
	}

	if (i < wmaxsize && i && wreceiver[i - 1])
		wreceiver[i] = 0;

	return i;
}
//---------------------------------------------------------------------------
void error_printf(const char * format, ...)
{
	if (g_Logger != NULL && g_Logger->Ready())
    {
        va_list arg;
        va_start(arg, format);
		g_Logger->Print("***Error: ");
		g_Logger->VPrint(format, arg);
        va_end(arg);
	}
}
//---------------------------------------------------------------------------
void warning_printf(const char * format, ...)
{
	if (g_Logger != NULL && g_Logger->Ready())
    {
        va_list arg;
        va_start(arg, format);
		g_Logger->Print("**Warning: ");
		g_Logger->VPrint(format, arg);
        va_end(arg);
	}
}
//---------------------------------------------------------------------------
void trace_printf(const char * format, ...)
{
	if (g_Logger != NULL && g_Logger->Ready())
    {
        va_list arg;
        va_start(arg, format);
		g_Logger->VPrint(format, arg);
        va_end(arg);
	}
}
//---------------------------------------------------------------------------
void trace_dump(unsigned char * buf, int length)
{
	if (g_Logger != NULL && g_Logger->Ready())
		g_Logger->Dump(buf, length);
}
//---------------------------------------------------------------------------