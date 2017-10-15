//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
bool g_AltPressed = false;
bool g_CtrlPressed = false;
bool g_ShiftPressed = false;
//----------------------------------------------------------------------------------
bool g_MovingFromMouse = false;
bool g_AutoMoving = false;
bool g_TheAbyss = false;
bool g_AbyssPacket03First = true;
bool g_Asmut = false;
//----------------------------------------------------------------------------------
int g_LandObjectsCount = 0;
int g_StaticsObjectsCount = 0;
int g_GameObjectsCount = 0;
int g_MultiObjectsCount = 0;
int g_RenderedObjectsCountInGameWindow = 0;
//----------------------------------------------------------------------------------
GLdouble g_GlobalScale = 1.0;

CGLTexture g_MapTexture[MAX_MAPS_COUNT];
CGLTexture g_AuraTexture;

bool g_LogoutAfterClick = false;

int g_FrameDelay[2] = { FRAME_DELAY_UNACTIVE_WINDOW, FRAME_DELAY_ACTIVE_WINDOW };

uint g_PingByPacket = 0;
uint g_PingByPacketSendTime = 0;

uint g_LastSendTime = 0;
uint g_LastPacketTime = 0;
uint g_TotalSendSize = 0;
uint g_TotalRecvSize = 0;

uint g_Ticks = 0;

GLuint ShaderColorTable = 0;
GLuint g_ShaderDrawMode = 0;

string g_Language = "ENU";

GAME_STATE g_GameState = GS_MAIN;

CGLTexture g_TextureGumpState[2];

WISP_GEOMETRY::CSize g_MapSize[MAX_MAPS_COUNT];
WISP_GEOMETRY::CSize g_MapBlockSize[MAX_MAPS_COUNT];

int g_MultiIndexCount = 0;

CGLFrameBuffer g_LightBuffer;

bool g_GumpPressed = false;
class CRenderObject *g_GumpSelectedElement = NULL;
class CRenderObject *g_GumpPressedElement = NULL;
WISP_GEOMETRY::CPoint2Di g_GumpMovingOffset;
WISP_GEOMETRY::CPoint2Df g_GumpTranslate;
bool g_ShowGumpLocker = false;

bool g_GrayedPixels = false;

bool g_ConfigLoaded = false;

uchar g_LightLevel = 0;
uchar g_PersonalLightLevel = 0;

char g_SelectedCharName[30] = { 0 };

//!Индекс текущей карты
uchar g_CurrentMap = 0;

//!Время сервера
uchar g_ServerTimeHour = 0;
uchar g_ServerTimeMinute = 0;
uchar g_ServerTimeSecond = 0;

bool g_PacketLoginComplete = false;

uint g_ClientFlag = 0;

bool g_SendLogoutNotification = false;
bool g_PopupEnabled = false;
bool g_ChatEnabled = false;
bool g_TooltipsEnabled = false;
bool g_PaperdollBooks = false;

uchar g_GameSeed[4] = { 0 };

ushort g_OutOfRangeColor = 0;
char g_MaxGroundZ = 0;
bool g_NoDrawRoof = false;
char g_FoliageIndex = 0;
bool g_UseCircleTrans = false;

bool g_JournalShowSystem = true;
bool g_JournalShowObjects = true;
bool g_JournalShowClient = true;

uint g_PlayerSerial = 0;
uint g_StatusbarUnderMouse = 0;

int g_LastSpellIndex = 1;
int g_LastSkillIndex = 1;
uint g_LastUseObject = 0;
uint g_LastTargetObject = 0;
uint g_LastAttackObject = 0;

CHARACTER_SPEED_TYPE g_SpeedMode = CST_NORMAL;

uint g_DeathScreenTimer = 0;

float g_AnimCharactersDelayValue = 80.0f; //0x50

WISP_GEOMETRY::CPoint2Di g_RemoveRangeXY;

int g_GrayMenuCount = 0;

PROMPT_TYPE g_ConsolePrompt = PT_NONE;
uchar g_LastASCIIPrompt[11] = { 0 };
uchar g_LastUnicodePrompt[11] = { 0 };

uint g_PartyHelperTarget = 0;
uint g_PartyHelperTimer = 0;

float g_DrawColor = 1.0f;

SEASON_TYPE g_Season = ST_SUMMER;
SEASON_TYPE g_OldSeason = ST_SUMMER;
int g_OldSeasonMusic = 0;

uint g_LockedClientFeatures = 0;

bool g_GeneratedMouseDown = false;
bool g_DrawFoliage = true;

DEVELOPER_MODE g_DeveloperMode = DM_SHOW_FPS_ONLY;
DEVELOPER_MODE g_OptionsDeveloperMode = DM_SHOW_FPS_ONLY;

ushort g_ObjectHandlesBackgroundPixels[g_ObjectHandlesWidth * g_ObjectHandlesHeight] = { 0 };

uint g_PingByWalk[0x100][2] = { 0 };
uint g_Ping = 0;

bool g_DrawAura = false;

ushort g_AbilityList[MAX_ABILITIES_COUNT] =
{
	AT_ARMOR_IGNORE,
	AT_BLEED_ATTACK,
	AT_CONCUSSION_BLOW,
	AT_CRUSHING_BLOW,
	AT_DISARM,
	AT_DISMOUNT,
	AT_DOUBLE_STRIKE,
	AT_INFECTING,
	AT_MORTAL_STRIKE,
	AT_MOVING_SHOT,
	AT_PARALYZING_BLOW,
	AT_SHADOW_STRIKE,
	AT_WHIRLWIND_ATTACK,
	AT_RIDING_SWIPE,
	AT_FRENZIED_WHIRLWIND,
	AT_BLOCK,
	AT_DEFENSE_MASTERY,
	AT_NERVE_STRIKE,
	AT_TALON_STRIKE,
	AT_FEINT,
	AT_DUAL_WIELD,
	AT_DOUBLE_SHOT,
	AT_ARMOR_PIERCE,
	AT_BLADEWEAVE,
	AT_FORCE_ARROW,
	AT_LIGHTNING_ARROW,
	AT_PSYCHIC_ATTACK,
	AT_SERPENT_ARROW,
	AT_FORCE_OF_NATURE,
	AT_INFUSED_THROW,
	AT_MYSTIC_ARC,
	AT_DISROBE
};

uchar g_Ability[2] = { AT_DISARM, AT_PARALYZING_BLOW };

bool g_DrawStatLockers = false;

uint g_SelectedGameObjectHandle = 0;

bool g_ShowWarnings = true;
//----------------------------------------------------------------------------------
bool CanBeDraggedByOffset(const WISP_GEOMETRY::CPoint2Di &point)
{
	if (g_Target.IsTargeting())
		return (abs(point.X) >= DRAG_PIXEL_RANGE_WITH_TARGET || abs(point.Y) >= DRAG_PIXEL_RANGE_WITH_TARGET);

	return (abs(point.X) >= DRAG_PIXEL_RANGE || abs(point.Y) >= DRAG_PIXEL_RANGE);
}
//----------------------------------------------------------------------------------
void TileOffsetOnMonitorToXY(int &ofsX, int &ofsY, int &x, int &y)
{
	if (!ofsX)
		x = y = ofsY / 2;
	else if (!ofsY)
	{
		x = ofsX / 2;
		y = -x;
	}
	else //if (ofsX && ofsY)
	{
		int absX = abs(ofsX);
		int absY = abs(ofsY);
		x = ofsX;

		if (ofsY > ofsX)
		{
			if (ofsX < 0 && ofsY < 0)
				y = absX - absY;
			else if (ofsX > 0 && ofsY > 0)
				y = absY - absX;
		}
		else if (ofsX > ofsY)
		{
			if (ofsX < 0 && ofsY < 0)
				y = -(absY - absX);
			else if (ofsX > 0 && ofsY > 0)
				y = -(absX - absY);
		}

		if (!y && ofsY != ofsX)
		{
			if (ofsY < 0)
				y = -(absX + absY);
			else
				y = absX + absY;
		}

		y /= 2;
		x += y;
	}
}
//----------------------------------------------------------------------------------
string ToCamelCase(string text)
{
	bool lastSpace = true;

	for (char &c : text)
	{
		if (lastSpace && (c >= 'a' && c <= 'z'))
			c = 'A' + (c - 'a');

		lastSpace = (c == ' ');
	}

	return text;
}
//----------------------------------------------------------------------------------
int GetDistance(CGameObject *current, CGameObject *target)
{
	if (current != NULL && target != NULL)
	{
		int distx = abs(target->X - current->X);
		int disty = abs(target->Y - current->Y);

		if (disty > distx)
			distx = disty;

		return distx;
	}

	return 100500;
}
//----------------------------------------------------------------------------------
int GetDistance(CGameObject *current, WISP_GEOMETRY::CPoint2Di target)
{
	if (current != NULL)
	{
		int distx = abs(target.X - current->X);
		int disty = abs(target.Y - current->Y);

		if (disty > distx)
			distx = disty;

		return distx;
	}

	return 100500;
}
//----------------------------------------------------------------------------------
int GetDistance(WISP_GEOMETRY::CPoint2Di current, CGameObject *target)
{
	if (target != NULL)
	{
		int distx = abs(target->X - current.X);
		int disty = abs(target->Y - current.Y);

		if (disty > distx)
			distx = disty;

		return distx;
	}

	return 100500;
}
//----------------------------------------------------------------------------------
int GetRemoveDistance(WISP_GEOMETRY::CPoint2Di current, CGameObject *target)
{
	if (target != NULL)
	{
		WISP_GEOMETRY::CPoint2Di targetPoint(target->X, target->Y);

		if (target->NPC && !((CGameCharacter*)target)->m_Steps.empty())
		{
			CWalkData &wd = ((CGameCharacter*)target)->m_Steps.back();

			targetPoint = WISP_GEOMETRY::CPoint2Di(wd.X, wd.Y);
		}

		int distx = abs(targetPoint.X - current.X);
		int disty = abs(targetPoint.Y - current.Y);

		if (disty > distx)
			distx = disty;

		return distx;
	}

	return 100500;
}
//----------------------------------------------------------------------------------
bool CheckMultiDistance(WISP_GEOMETRY::CPoint2Di current, CGameObject *target, int maxDistance)
{
	bool result = false;

	if (target != NULL)
	{
		maxDistance += ((CGameItem*)target)->MultiDistanceBonus;

		result = ((abs(target->X - current.X) <= maxDistance) && (abs(target->Y - current.Y) <= maxDistance));
	}

	return result;
}
//----------------------------------------------------------------------------------
int GetDistance(WISP_GEOMETRY::CPoint2Di current, WISP_GEOMETRY::CPoint2Di target)
{
	int distx = abs(target.X - current.X);
	int disty = abs(target.Y - current.Y);

	if (disty > distx)
		distx = disty;

	return distx;
}
//----------------------------------------------------------------------------------
int GetTopObjDistance(CGameObject *current, CGameObject *target)
{
	if (current != NULL && target != NULL)
	{
		while (target != NULL && target->Container != 0xFFFFFFFF)
			target = g_World->FindWorldObject(target->Container);

		if (target != NULL)
		{
			int distx = abs(target->X - current->X);
			int disty = abs(target->Y - current->Y);

			if (disty > distx)
				distx = disty;

			return distx;
		}
	}

	return 100500;
}
//---------------------------------------------------------------------------
const char *GetReagentName(const ushort &id)
{
	switch (id)
	{
		case 0x0F7A:
			return "Black pearl";
		case 0x0F7B:
			return "Bloodmoss";
		case 0x0F84:
			return "Garlic";
		case 0x0F85:
			return "Ginseng";
		case 0x0F86:
			return "Mandrake root";
		case 0x0F88:
			return "Nightshade";
		case 0x0F8C:
			return "Sulfurous ash";
		case 0x0F8D:
			return "Spiders silk";
		default:
			break;
	}

	return "";
}
//----------------------------------------------------------------------------------
