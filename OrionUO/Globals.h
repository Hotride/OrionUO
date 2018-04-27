/***********************************************************************************
**
** Globals.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GLOBALS_H
#define GLOBALS_H
//----------------------------------------------------------------------------------
//!Глобальные состояния нажатия дополнительных клавишь
extern bool g_AltPressed;
extern bool g_CtrlPressed;
extern bool g_ShiftPressed;
//----------------------------------------------------------------------------------
//!Использование мышки для перемещения персонажа
extern bool g_MovingFromMouse;

//!Использование автоматического перемещения персонажа за курсором мышки, когда курсор находится в игровом окне
extern bool g_AutoMoving;

extern bool g_TheAbyss;
extern bool g_AbyssPacket03First;
extern bool g_Asmut;
//----------------------------------------------------------------------------------
#include "Wisp/WispGlobal.h"
#include "Wisp/WispGeometry.h"
#include "Wisp/WispLogger.h"
#include "OrionApplication.h"
#include "GLEngine/GLFrameBuffer.h"
#include "GLEngine/GLTexture.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "EnumList.h"
#include "DefinitionMacro.h"
#include "Constants.h"
//----------------------------------------------------------------------------------
#define CWISPDEBUGLOGGER 0

#if CWISPDEBUGLOGGER != 0
	#define DEBUGLOG LOG
#else //CWISPDEBUGLOGGER == 0
	#define DEBUGLOG(...)
#endif //CWISPDEBUGLOGGER!=0
//----------------------------------------------------------------------------------
#pragma warning(disable: 4800) //forcing value to bool 'true' or 'false' (performance warning)
//----------------------------------------------------------------------------------
bool CanBeDraggedByOffset(const WISP_GEOMETRY::CPoint2Di &point);
//----------------------------------------------------------------------------------
void TileOffsetOnMonitorToXY(int &ofsX, int &ofsY, int &x, int &y);
//----------------------------------------------------------------------------------
string ToCamelCase(string text);
//----------------------------------------------------------------------------------
//Функции для вычисления дистанции
class CGameObject;
int GetDistance(CGameObject *current, CGameObject *target);
int GetDistance(CGameObject *current, const WISP_GEOMETRY::CPoint2Di &target);
bool CheckMultiDistance(const WISP_GEOMETRY::CPoint2Di &current, CGameObject *target, int maxDistance);
int GetDistance(const WISP_GEOMETRY::CPoint2Di &current, CGameObject *target);
int GetDistance(const WISP_GEOMETRY::CPoint2Di &current, const WISP_GEOMETRY::CPoint2Di &target);
int GetRemoveDistance(const WISP_GEOMETRY::CPoint2Di &current, CGameObject *target);
int GetTopObjDistance(CGameObject *current, CGameObject *target);
//----------------------------------------------------------------------------------
const char *GetReagentName(ushort id);
//----------------------------------------------------------------------------------
//!Счетчики объектов (только для отладки)
extern int g_LandObjectsCount;
extern int g_StaticsObjectsCount;
extern int g_GameObjectsCount;
extern int g_MultiObjectsCount;
extern int g_RenderedObjectsCountInGameWindow;
//----------------------------------------------------------------------------------
extern GLdouble g_GlobalScale;

extern CGLTexture g_MapTexture[MAX_MAPS_COUNT];

extern CGLTexture g_AuraTexture;

extern bool g_LogoutAfterClick;

//!Значения задержек в массиве
extern int g_FrameDelay[2];

//!Время последней отправки пакета серверу
extern uint g_LastSendTime;

//!Время последнего пакета от сервера
extern uint g_LastPacketTime;

//!Общий размер отправленных пакетов
extern uint g_TotalSendSize;

//!Общий размер принятых пакетов
extern uint g_TotalRecvSize;

extern uint g_Ticks;

extern GLuint ShaderColorTable;
extern GLuint g_ShaderDrawMode;

extern string g_Language;

extern GAME_STATE g_GameState;

extern CGLTexture g_TextureGumpState[2];

extern WISP_GEOMETRY::CSize g_MapSize[MAX_MAPS_COUNT];
extern WISP_GEOMETRY::CSize g_MapBlockSize[MAX_MAPS_COUNT];

extern int g_MultiIndexCount;

extern class CGLFrameBuffer g_LightBuffer;

extern bool g_GumpPressed;
extern class CRenderObject *g_GumpSelectedElement;
extern class CRenderObject *g_GumpPressedElement;
extern WISP_GEOMETRY::CPoint2Di g_GumpMovingOffset;
extern WISP_GEOMETRY::CPoint2Df g_GumpTranslate;
extern bool g_ShowGumpLocker;

extern bool g_GrayedPixels;

extern bool g_ConfigLoaded;

extern uchar g_LightLevel;
extern uchar g_PersonalLightLevel;

//!Имя выбранного персонажа
extern char g_SelectedCharName[30];

//!Индекс текущей карты
extern uchar g_CurrentMap;

//!Время сервера
extern uchar g_ServerTimeHour;
extern uchar g_ServerTimeMinute;
extern uchar g_ServerTimeSecond;

extern bool g_PacketLoginComplete;

extern uint g_ClientFlag;

extern bool g_SendLogoutNotification;
extern bool g_PopupEnabled;
extern bool g_ChatEnabled;
extern bool g_TooltipsEnabled;
extern bool g_PaperdollBooks;

//!Сообщение инициализации шифрования серверу
extern uchar g_GameSeed[4];

//!Цвет для объектов, вышедших за пределы g_UpdateRange (если это предусмотрено опциями)
extern ushort g_OutOfRangeColor;

//!Не рисовать крыши
extern bool g_NoDrawRoof;

//!Максимальное значение Z коордираны ландшафта для рендера
extern char g_MaxGroundZ;

//!Индекс прозрачности крон деревьев
extern char g_FoliageIndex;

//!Возможность использования круга прозрачности
extern bool g_UseCircleTrans;

//!Отображать в журнале системные сообщения
extern bool g_JournalShowSystem;

//!Отображать в журнале сообщения от объектов
extern bool g_JournalShowObjects;

//!Отображать в журнале внутренние сообщения клиента
extern bool g_JournalShowClient;

extern uint g_PlayerSerial;
extern uint g_StatusbarUnderMouse;

//!Индекс последнего выбранного заклинания
extern int g_LastSpellIndex;

//!Индекс последнего выбранного навыка
extern int g_LastSkillIndex;

//!Последний использованный объект
extern uint g_LastUseObject;

//!Последний объект, на который навели прицелом
extern uint g_LastTargetObject;

//!Последний атакованный объект
extern uint g_LastAttackObject;

extern CHARACTER_SPEED_TYPE g_SpeedMode;

//!Таймер черного экрана от смерти
extern uint g_DeathScreenTimer;

//!Модификатор расчета анимации персонажа
extern float g_AnimCharactersDelayValue;

typedef vector<pair<uint, uint>> UINTS_PAIR_LIST;

//!Координаты, с которых следует начинать расчет удаления объектов
extern WISP_GEOMETRY::CPoint2Di g_RemoveRangeXY;

//!Количество блокирующих меню
extern int g_GrayMenuCount;

//!Тип промпта
extern PROMPT_TYPE g_ConsolePrompt;

//!Данные последнего ASCII промпта
extern uchar g_LastASCIIPrompt[11];

//!Данные последнего Unicode промпта
extern uchar g_LastUnicodePrompt[11];

//!Серийник для таргета от кнопок пати-гампа
extern uint g_PartyHelperTarget;

//!Время жизни для таргета от кнопок пати-гампа
extern uint g_PartyHelperTimer;

extern float g_DrawColor;

extern SEASON_TYPE g_Season;
extern SEASON_TYPE g_OldSeason;
extern int g_OldSeasonMusic;

extern uint g_LockedClientFeatures;

extern bool g_GeneratedMouseDown;

enum DEVELOPER_MODE{DM_NO_DEBUG = 0, DM_SHOW_FPS_ONLY, DM_DEBUGGING};
extern DEVELOPER_MODE g_DeveloperMode;
extern DEVELOPER_MODE g_OptionsDeveloperMode;

const int g_ObjectHandlesWidth = 142;
const int g_ObjectHandlesWidthOffset = g_ObjectHandlesWidth / 2;
const int g_ObjectHandlesHeight = 20;
extern ushort g_ObjectHandlesBackgroundPixels[g_ObjectHandlesWidth * g_ObjectHandlesHeight];

extern uint g_Ping;

extern bool g_DrawAura;

static const int MAX_ABILITIES_COUNT = 32;
extern ushort g_AbilityList[MAX_ABILITIES_COUNT];
extern uchar g_Ability[2];

extern bool g_DrawStatLockers;

extern uint g_SelectedGameObjectHandle;

extern bool g_ShowWarnings;

extern uint g_ProcessStaticAnimationTimer;
extern uint g_ProcessRemoveRangedTimer;
extern int g_MaxViewRange;
extern uint g_OrionFeaturesFlags;
extern struct PING_INFO_DATA g_GameServerPingInfo;
extern string g_PingString;
extern uint g_PingTimer;
//----------------------------------------------------------------------------------
inline bool IsBackground(const __int64 &flags) { return (flags & 0x00000001); }
inline bool IsWeapon(const __int64 &flags) { return (flags & 0x00000002); }
inline bool IsTransparent(const __int64 &flags) { return (flags & 0x00000004); }
inline bool IsTranslucent(const __int64 &flags) { return (flags & 0x00000008); }
inline bool IsWall(const __int64 &flags) { return (flags & 0x00000010); }
inline bool IsDamaging(const __int64 &flags) { return (flags & 0x00000020); }
inline bool IsImpassable(const __int64 &flags) { return (flags & 0x00000040); }
inline bool IsWet(const __int64 &flags) { return (flags & 0x00000080); }
inline bool IsUnknown(const __int64 &flags) { return (flags & 0x00000100); }
inline bool IsSurface(const __int64 &flags) { return (flags & 0x00000200); }
inline bool IsBridge(const __int64 &flags) { return (flags & 0x00000400); }
inline bool IsStackable(const __int64 &flags) { return (flags & 0x00000800); }
inline bool IsWindow(const __int64 &flags) { return (flags & 0x00001000); }
inline bool IsNoShoot(const __int64 &flags) { return (flags & 0x00002000); }
inline bool IsPrefixA(const __int64 &flags) { return (flags & 0x00004000); }
inline bool IsPrefixAn(const __int64 &flags) { return (flags & 0x00008000); }
inline bool IsInternal(const __int64 &flags) { return (flags & 0x00010000); }
inline bool IsFoliage(const __int64 &flags) { return (flags & 0x00020000); }
inline bool IsPartialHue(const __int64 &flags) { return (flags & 0x00040000); }
inline bool IsUnknown1(const __int64 &flags) { return (flags & 0x00080000); }
inline bool IsMap(const __int64 &flags) { return (flags & 0x00100000); }
inline bool IsContainer(const __int64 &flags) { return (flags & 0x00200000); }
inline bool IsWearable(const __int64 &flags) { return (flags & 0x00400000); }
inline bool IsLightSource(const __int64 &flags) { return (flags & 0x00800000); }
inline bool IsAnimated(const __int64 &flags) { return (flags & 0x01000000); }
inline bool IsNoDiagonal(const __int64 &flags) { return (flags & 0x02000000); }
inline bool IsUnknown2(const __int64 &flags) { return (flags & 0x04000000); }
inline bool IsArmor(const __int64 &flags) { return (flags & 0x08000000); }
inline bool IsRoof(const __int64 &flags) { return (flags & 0x10000000); }
inline bool IsDoor(const __int64 &flags) { return (flags & 0x20000000); }
inline bool IsStairBack(const __int64 &flags) { return (flags & 0x40000000); }
inline bool IsStairRight(const __int64 &flags) { return (flags & 0x80000000); }
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
