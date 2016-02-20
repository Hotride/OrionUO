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

DWORD g_LastRenderTime = 0;
//---------------------------------------------------------------------------