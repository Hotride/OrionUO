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

GLuint ShaderTexture = 0;
GLuint ShaderColorTable = 0;
GLuint ShaderDrawMode = 0;
//---------------------------------------------------------------------------
int g_LandObjectsCount = 0;
int g_StaticsObjectsCount = 0;
int g_GameObjectsCount = 0;
int g_MultiObjectsCount = 0;
int g_RenderedObjectsCountInGameWindow = 0;
//---------------------------------------------------------------------------
GLuint g_TextureGumpState[2] = {0, 0};
bool g_UseFrameBuffer = false;
WORD g_ObjectHandlesBackgroundPixels[g_ObjectHandlesWidth * g_ObjectHandlesHeight] = { 0 };
//---------------------------------------------------------------------------
bool g_MovingFromMouse = false;
bool g_AutoMoving = false;
bool g_AbyssPacket03First = true;
//---------------------------------------------------------------------------
bool g_AltPressed = false;
bool g_CtrlPressed = false;
bool g_ShiftPressed = false;
//---------------------------------------------------------------------------
HINSTANCE g_Hinstance = NULL;
HWND g_hWnd = NULL;
int g_ClientWidth = 0;
int g_ClientHeight = 0;
int g_FrameDelay[2] = { FRAME_DELAY_UNACTIVE_WINDOW, FRAME_DELAY_ACTIVE_WINDOW };
char g_FoliageIndex = 0;
string g_DirectoryPath = "";
string g_LastLoadFileError = "";
string g_ShardIP = "";
string g_ClientVersionText = "";
string g_AutoLoginNames = "";
wstring g_ConsoleStack[MAX_CONSOLE_STACK_SIZE];
int g_ConsoleStackCount = 0;
int g_ConsoleSelectedIndex = 0;
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
int g_GameWindowWidth = 800;
int g_GameWindowHeight = 600;
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
char g_SelectedCharName[30] = { 0 };

int g_ZBuffer = 0;
bool g_UseCircleTrans = false;

bool g_GumpPressed = false;
bool g_GumpPressedScroller = 0;
DWORD g_GumpSelectElement = 0;
DWORD g_GumpPressedElement = 0;
int g_GumpMovingOffsetX = 0;
int g_GumpMovingOffsetY = 0;
float g_GumpTranslateX = 0.0f;
float g_GumpTranslateY = 0.0f;

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
int g_UpdateRange = 18;
CORPSE_LIST_MAP g_CorpseSerialList;

DWORD g_LastRenderTime = 0;


/*
Обращение к анимации маунта под персом, индекс по арт.мул (203): 004073E7; 00406FCD; 00408B9A
*/



/*
96 объектов, на которых можно сидеть (структура из клиента 7.0.45.0, от 2.0.3 отличается только добавлением нескольких предметов)
//{ graphic, direction, offsetY, mirrorOffsetY, z-buffer bonus}
{ 0x0B2F, 2, 6, 6, 0 },
{ 0x0B4E, 2, 0, 0, 0 },
{ 0x0B4F, 4, 0, 0, 0 },
{ 0x0B52, 2, 0, 0, 0 },
{ 0x0B53, 4, 0, 0, 0 },
{ 0x0B56, 2, 4, 4, 0 },
{ 0x0B57, 4, 4, 4, 0 },
{ 0x0B5A, 2, 8, 8, 0 },
{ 0x0B5B, 4, 8, 8, 0 },
{ 0x1218, 4, 4, 4, 0 },
{ 0x1219, 2, 4, 4, 0 },
{ 0x2DDF, 5, 2, 2, 0 },
{ 0x2DE0, 10, 2, 2, 0 },
{ 0x2DE3, 2, 4, 4, 0 },
{ 0x2DE4, 4, 4, 4, 0 },
{ 0x2DE5, 8, 4, 4, 0 },
{ 0x2DE6, 1, 4, 4, 0 },
{ 0x2DEB, 1, 4, 4, 0 },
{ 0x2DEC, 4, 4, 4, 0 },
{ 0x2DED, 2, 4, 4, 0 },
{ 0x2DEE, 8, 4, 4, 0 },
{ 0x2DF6, 4, 4, 4, 0 },
{ 0x2DF5, 2, 4, 4, 0 },
{ 0x3088, 5, 4, 4, 0 },
{ 0x308A, 10, 4, 4, 0 },
{ 0x3089, 5, 4, 4, 0 },
{ 0x308B, 10, 4, 4, 0 },
{ 0x0B30, 8, -8, 8, 1 },
{ 0x0B31, 1, 0, 4, 1 },
{ 0x0B50, 1, 0, 0, 1 },
{ 0x0B51, 8, 0, 0, 1 },
{ 0x0B54, 1, 0, 0, 1 },
{ 0x0B55, 8, 0, 0, 1 },
{ 0x0B58, 8, 0, 8, 1 },
{ 0x0B59, 1, 0, 8, 1 },
{ 0x0B5C, 1, 0, 8, 1 },
{ 0x0B5D, 8, 0, 8, 1 },
{ 0x121A, 1, 0, 8, 3 },
{ 0x121B, 8, 0, 8, 3 },
{ 0x0B32, 4, 0, 0, 0 },
{ 0x0B33, 2, 0, 0, 0 },
{ 0x35ED, 4, 0, 0, 0 },
{ 0x35EE, 2, 0, 0, 0 },
{ 0x1527, 2, 0, 0, 0 },
{ 0x2A58, 4, 0, 0, 0 },
{ 0x2A59, 2, 0, 0, 0 },
{ 0x2A5A, 4, 0, 0, 0 },
{ 0x2A5B, 4, 10, 10, 0 },
{ 0x2A7F, 2, 0, 0, 0 },
{ 0x2A80, 2, 0, 0, 0 },
{ 0x0459, 5, 2, 2, 0 },
{ 0x045A, 10, 2, 2, 0 },
{ 0x045B, 5, 2, 2, 0 },
{ 0x045C, 10, 2, 2, 0 },
{ 0x0B2C, 10, 2, 2, 0 },
{ 0x0B2D, 5, 2, 2, 0 },
{ 0x3DFF, 5, 2, 2, 0 },
{ 0x3E00, 10, 2, 2, 0 },
{ 0x0B5F, 10, 3, 14, 0 },
{ 0x0B60, 10, 3, 14, 0 },
{ 0x0B61, 10, 3, 14, 0 },
{ 0x0B62, 10, 3, 10, 0 },
{ 0x0B63, 10, 3, 10, 0 },
{ 0x0B64, 10, 3, 10, 0 },
{ 0x0B65, 5, 3, 10, 0 },
{ 0x0B66, 5, 3, 10, 0 },
{ 0x0B67, 5, 3, 10, 0 },
{ 0x0B68, 5, 3, 10, 0 },
{ 0x0B69, 5, 3, 10, 0 },
{ 0x0B6A, 5, 3, 10, 0 },
{ 0x0B91, 4, 6, 6, 0 },
{ 0x0B92, 4, 6, 6, 0 },
{ 0x0B93, 2, 6, 6, 0 },
{ 0x0B94, 2, 6, 6, 0 },
{ 0x1DC7, 10, 3, 10, 0 },
{ 0x1DC8, 10, 3, 10, 0 },
{ 0x1DC9, 10, 3, 10, 0 },
{ 0x1DCA, 5, 3, 10, 0 },
{ 0x1DCB, 5, 3, 10, 0 },
{ 0x1DCC, 5, 3, 10, 0 },
{ 0x1DCD, 10, 3, 10, 0 },
{ 0x1DCE, 10, 3, 10, 0 },
{ 0x1DCF, 10, 3, 10, 0 },
{ 0x1DD0, 5, 3, 10, 0 },
{ 0x1DD1, 5, 3, 10, 0 },
{ 0x1DD2, 10, 3, 10, 0 },
{ 0x11FC, 15, 2, 7, 0 },
{ 0x0A2A, 15, -4, -4, 0 },
{ 0x0A2B, 15, -8, -8, 0 },
{ 0x0CF3, 26, 2, 8, 0 },
{ 0x0CF4, 26, 2, 8, 0 },
{ 0x0CF6, 21, 2, 8, 0 },
{ 0x0CF7, 21, 2, 8, 0 },
{ 0x1771, 31, 0, 0, 0 },
{ 0x1776, 31, 0, 0, 0 },
{ 0x1779, 31, 0, 0, 0 }
*/
//---------------------------------------------------------------------------