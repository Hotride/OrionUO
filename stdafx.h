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
#ifndef stdafxH
#define stdafxH
//---------------------------------------------------------------------------
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
#include <algorithm>
#include <time.h>
#include "glew.h"
#include "wglew.h"
#include <gl\gl.h>
#include <gl\glu.h>
#include "bass.h"
#include <winsock.h>
#include <deque>
#include <Shellapi.h>
#include <Shlwapi.h>
#include "zlib.h"
#include <Mmsystem.h>
#include <process.h>

#include <vector>
using std::vector;

#include <string>
using std::string;
using std::wstring;

#include <unordered_map>
using std::unordered_map;

using std::pair;

#define _USE_MATH_DEFINES // for C++
#include <math.h>

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "Glu32.lib")
#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "zdll.lib")
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "bass.lib")

#pragma warning(disable: 4390)
#pragma warning(disable: 4800)
//---------------------------------------------------------------------------
#define UO_FW_SHARD 0
#define UO_ABYSS_SHARD 0

#define UO_USE_SHADER_FILES 0

#include "Constants.h"
#include "EnumList.h"
#include "PluginInterface.h"
#include "DefinitionMacro.h"
//---------------------------------------------------------------------------
//!Указатель на текстуру шейдера
extern GLuint ShaderTexture;

//!Указатель на состояние рисования шейдера
extern GLuint ShaderDrawMode;

//!Указатель на палитру цветов для шейдера
extern GLuint ShaderColorTable;
//---------------------------------------------------------------------------
//!Счетчики объектов (только для отладки)
extern int g_LandObjectsCount;
extern int g_StaticsObjectsCount;
extern int g_GameObjectsCount;
extern int g_MultiObjectsCount;
extern int g_RenderedObjectsCountInGameWindow;
//---------------------------------------------------------------------------
//!Текстуры замков для гампов (locked/unlocked)
extern GLuint g_TextureGumpState[2];

//!Возможность использовать фрэймбуфер
extern bool g_UseFrameBuffer;

const int g_ObjectHandlesWidth = 142;
const int g_ObjectHandlesHeight = 20;
extern WORD g_ObjectHandlesBackgroundPixels[g_ObjectHandlesWidth * g_ObjectHandlesHeight];
//---------------------------------------------------------------------------
//!Глобальные состояния нажатия дополнительных клавишь
extern bool g_AltPressed;
extern bool g_CtrlPressed;
extern bool g_ShiftPressed;
//---------------------------------------------------------------------------
//!Использование мышки для перемещения персонажа
extern bool g_MovingFromMouse;

//!Использование автоматического перемещения персонажа за курсором мышки, когда курсор находится в игровом окне
extern bool g_AutoMoving;

extern bool g_AbyssPacket03First;
//---------------------------------------------------------------------------
//!Инстанс приложения
extern HINSTANCE g_Hinstance;

//!Хэндл окна
extern HWND g_hWnd;

//!Габариты клиентской области окна
extern int g_ClientWidth;
extern int g_ClientHeight;

//!Значения задержек в массиве
extern int g_FrameDelay[2];

//!Индекс прозрачности крон деревьев
extern char g_FoliageIndex;

//!Путь к папке запуска
extern string g_DirectoryPath;

//!Имя последнего файла, загрузка которого завершилась ошибкой
extern string g_LastLoadFileError;

//!Текст версии клиента
extern string g_ClientVersionText;

//!Имена для автовхода
extern string g_AutoLoginNames;

//!Стек последнего введенного текста
extern wstring g_ConsoleStack[MAX_CONSOLE_STACK_SIZE];

//!Количество добавленного в стек текста
extern int g_ConsoleStackCount;

//!Указатель на текущий элемент стека
extern int g_ConsoleSelectedIndex;

//!IP сервера
extern string g_ShardIP;

//!Порт к которому подключаться к серверу
extern int g_ShardPort;

//!Время последней отправки пакета серверу
extern DWORD g_LastSendTime;

//!Время последнего пакета от сервера
extern DWORD g_LastPacketTime;

//!Общий размер отправленных пакетов
extern DWORD g_TotalSendSize;

//!Общий размер принятых пакетов
extern DWORD g_TotalRecvSize;

//!Флаг загрузки конфига
extern bool g_ConfigLoaded;

//!Состояние игры (отображаемый экран)
extern GAME_STATE g_GameState;

//!Логин ключи
extern DWORD g_CryptKey[3];

//!Модификатор логин ключа
extern WORD g_SeedKey;

//!Сообщение инициализации шифрования серверу
extern BYTE g_GameSeed[4];

//!Уровень освещенности
extern BYTE g_LightLevel;

//!Уровень освещенности персонажа
extern BYTE g_PersonalLightLevel;

//!Перевернутыый байлист
extern bool g_InverseBuylist;

//!Флаги клиента
extern DWORD g_ClientFlag;

//!Флаги расширений клиента
extern DWORD g_LockedClientFeatures;

//!Максимальный габариты карты в точках по осям
extern short g_MapSizeX[6];
extern short g_MapSizeY[6];

//!Максимальный габариты карты в блоках по осям
extern short g_MapBlockX[6];
extern short g_MapBlockY[6];

//!Текстуры карт мира
extern GLuint g_MapTexture[6];

//!Индекс текущей карты
extern BYTE g_CurrentMap;

//!Габариты игрового окна
extern int g_GameWindowWidth;
extern int g_GameWindowHeight;

//!Позиция игрового окна
extern int g_GameWindowPosX;
extern int g_GameWindowPosY;

//!Кодовая страница пользователя
extern int g_CodePage;

//!Время сервера
extern BYTE g_ServerTimeHour;
extern BYTE g_ServerTimeMinute;
extern BYTE g_ServerTimeSecond;

//!Счетчик стека пинга
extern BYTE g_PingCount;
extern BYTE g_PingSequence;

//!Пакет о завершении входа в игру получен
extern bool g_PacketLoginComplete;

//!Рисовать мир в черно-белом представлении
extern bool g_GrayedPixels;

//!Количество индексов мульти-объектов
extern int g_MultiIndexCount;

//!Количество запросов перемещения
extern BYTE g_WalkRequestCount;

//!Время последнего шага
extern DWORD g_LastStepTime;

//!Сумма значений навыков
extern float g_SkillsTotal;

//!Серийник персонажа
extern DWORD g_PlayerSerial;

//!Количество блокирующих меню
extern int g_GrayMenuCount;

//!Отображать в журнале системные сообщения
extern bool g_JournalShowSystem;

//!Отображать в журнале сообщения от объектов
extern bool g_JournalShowObjects;

//!Отображать в журнале внутренние сообщения клиента
extern bool g_JournalShowClient;

//!Серийник статусбара под мышкой
extern DWORD g_StatusbarUnderMouse;

//!Таймер черного экрана от смерти
extern DWORD g_DeathScreenTimer;

//!Отмена двойного нажатия мышки
extern bool g_CancelDoubleClick;

//!Тип промпта
extern PROMPT_TYPE g_ConsolePrompt;

//!Данные последнего ASCII промпта
extern BYTE g_LastASCIIPrompt[11];

//!Данные последнего Unicode промпта
extern BYTE g_LastUnicodePrompt[11];

//!Режим рисования. true - рисование, false - выбор объектов
extern bool g_DrawMode;

//!Текущий язык
extern string g_Language;

//!Цвет для объектов, вышедших за пределы g_UpdateRange (если это предусмотрено опциями)
extern WORD g_OutOfRangeColor;

//!Отправлять ли уведомление о выходе из игры
extern bool g_SendLogoutNotification;

//!Включены ли всплывающие меню НПС
extern bool g_NPCPopupEnabled;

//!Включен ли чат
extern bool g_ChatEnabled;

//!Текущий сезон года
extern BYTE g_Season;

//!Модификатор расчета анимации персонажа
extern float g_AnimCharactersDelayValue;

//!Отображать ли локеры гампов
extern bool g_ShowGumpLocker;

//!Серийник для таргета от кнопок пати-гампа
extern DWORD g_PartyHelperTarget;

//!Время жизни для таргета от кнопок пати-гампа
extern DWORD g_PartyHelperTimer;

//!Цвет OpenGL рисования
extern float g_DrawColor;

//!Не рисовать крыши
extern bool g_NoDrawRoof;

//!Максимальное значение Z коордираны ландшафта для рендера
extern char g_MaxGroundZ;

//!Имя выбранного персонажа
extern char g_SelectedCharName[30];

//!Режим скорости перемещения персонажа
extern CHARACTER_SPEED_TYPE g_SpeedMode;

//!Значение Z-buffer'а
extern int g_ZBuffer;

//!Возможность использования круга прозрачности
extern bool g_UseCircleTrans;

extern bool g_GumpPressed;
extern bool g_GumpPressedScroller;
extern DWORD g_GumpSelectElement;
extern DWORD g_GumpPressedElement;
extern int g_GumpMovingOffsetX;
extern int g_GumpMovingOffsetY;
extern float g_GumpTranslateX;
extern float g_GumpTranslateY;
//---------------------------------------------------------------------------
//!Текущие координаты мыши
extern int g_MouseX;
extern int g_MouseY;

//!Координаты нажатой левой кнопки мыши
extern int g_DroppedLeftMouseX;
extern int g_DroppedLeftMouseY;

//!Координаты нажатой правой кнопки мыши
extern int g_DroppedRightMouseX;
extern int g_DroppedRightMouseY;

//!Состояния нажатия кнопок мыши
extern bool g_RightMouseDown;
extern bool g_LeftMouseDown;
//---------------------------------------------------------------------------
//!Выбор объектов гампов
extern bool g_SelectGumpObjects;

//!Тип выбранного объекта
extern SELECT_OBJECT_TYPE g_LastObjectType;

//!Последний выбранный объект
extern int g_LastSelectedObject;

//!Последний объект, на котором была нажата левая кнопка мыши
extern int g_LastObjectLeftMouseDown;

//!Последний объект, на котором была нажата правая кнопка мыши
extern int g_LastObjectRightMouseDown;

//!Последний выбранный гамп
extern DWORD g_LastSelectedGump;

//!Последний гамп, на котором была нажата левая кнопка мыши
extern DWORD g_LastGumpLeftMouseDown;

//!Время последнего нажатия мышкой на гамп
extern DWORD g_LastGumpMouseDownTime;

//!Время последнего нажатия мышки
extern DWORD g_LastMouseDownTime;

//!Последний гамп, на котором была нажата правая кнопка мыши
extern DWORD g_LastGumpRightMouseDown;

//!Сгенерированное событие нажатия кнопки мыши
extern bool g_GeneratedMouseDown;

//!Последнее время клика мышкой
extern DWORD g_LastLClickTime;

//!Индекс последнего выбранного заклинания
extern int g_LastSpellIndex;

//!Индекс последнего выбранного навыка
extern int g_LastSkillIndex;

//!Запрос нажатия на объект
extern bool g_ClickObjectReq;

//!Последний использованный объект
extern DWORD g_LastUseObject;

//!Последний объект, на который навели прицелом
extern DWORD g_LastTargetObject;

//!Последний атакованный объект
extern DWORD g_LastAttackObject;

//!Время последней отрисовки
extern DWORD g_LastRenderTime;

//!Дистанция для обновления предметов
extern int g_UpdateRange;

//!Координаты, с которых следует начинать расчет удаления объектов
extern POINT g_RemoveRangeXY;

typedef vector<pair<DWORD, DWORD>> CORPSE_LIST_MAP;
extern CORPSE_LIST_MAP g_CorpseSerialList;
//---------------------------------------------------------------------------





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




#define DPOLY(x, y, w, h) g_GL.DrawPolygone(x, y, w, h)
//---------------------------------------------------------------------------
typedef std::vector<BYTE> WaveFile;




//---------------------------------------------------------------------------
#include "CommonFunctions.h"
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
#include "TextFileParser.h"
#include "ImageBounds.h"
#include "TargetGump.h"
#include "SmoothMonitor.h"
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
#include "MapObject.h"
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
#include "ToolTip.h"
#include "game object\\ClickObject.h"
#include "Target.h"
#include "Party.h"
#include "ServerList.h"
#include "CharacterList.h"
#include "Profession.h"
#include "Target.h"
#include "SkillGroup.h"
#include "Weather.h"
#include "MapBlock.h"
#include "Container.h"
#include "QuestArrow.h"
#include "MultiMap.h"
#include "PathFinder.h"
#include "Macro.h"
#include "CityList.h"
#include "ScreenshotBuilder.h"
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
#include "gump\\GumpTargetSystem.h"
#include "gump\\GumpPopupMenu.h"
#include "gump\\GumpConsoleType.h"
#include "gump\\GumpBuff.h"
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
#include "managers\\ProfessionManager.h"
#include "managers\\SpeechManager.h"
#include "ShaderData.h"
#include "ClientUO.h"
//---------------------------------------------------------------------------
#endif
