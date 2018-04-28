// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** CommonInterfaces.cpp
**
** Copyright (C) September 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
IOrionString g_OrionString;
//----------------------------------------------------------------------------------
IOrionString::IOrionString()
: m_Unicode(false), m_DataA(NULL), m_DataW(NULL)
{
}
//----------------------------------------------------------------------------------
IOrionString::~IOrionString()
{
	RELEASE_POINTER(m_DataA);
	RELEASE_POINTER(m_DataW);
}
//----------------------------------------------------------------------------------
IOrionString &IOrionString::operator()(const std::string &str)
{
	RELEASE_POINTER(m_DataA);

	m_Unicode = false;
	if (str.length())
	{
		m_DataA = new char[str.length() + 1];
		memcpy(&m_DataA[0], &str[0], str.length());
		m_DataA[str.length()] = 0;
	}

	return *this;
}
//----------------------------------------------------------------------------------
IOrionString &IOrionString::operator()(const std::wstring &str)
{
	RELEASE_POINTER(m_DataW);

	m_Unicode = true;
	if (str.length())
	{
		m_DataW = new wchar_t[str.length() + 1];
		memcpy(&m_DataW[0], &str[0], str.length() * 2);
		m_DataA[str.length()] = 0;
	}

	return *this;
}
//----------------------------------------------------------------------------------
//IGLEngine
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_PushScissor(int x, int y, int width, int height)
{
	g_GL.PushScissor(x, y, width, height);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_PopScissor()
{
	g_GL.PopScissor();
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawLine(unsigned int color, int x, int y, int width, int height)
{
	glColor4ub(GetRValue(color), GetGValue(color), GetBValue(color), (color >> 24) & 0xFF);
	g_GL.DrawLine(x, y, width, height);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawPolygone(unsigned int color, int x, int y, int width, int height)
{
	glColor4ub(GetRValue(color), GetGValue(color), GetBValue(color), (color >> 24) & 0xFF);
	g_GL.DrawPolygone(x, y, width, height);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawCircle(unsigned int color, float x, float y, float radius, int gradientMode)
{
	glColor4ub(GetRValue(color), GetGValue(color), GetBValue(color), (color >> 24) & 0xFF);
	g_GL.DrawCircle(x, y, radius, gradientMode);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawTextA(int x, int y, unsigned char font, const char *text, unsigned short color, int width, TEXT_ALIGN_TYPE align, unsigned short flags)
{
	g_FontManager.DrawA(font, text, color, x, y, width, align, flags);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawTextW(int x, int y, unsigned char font, const wchar_t *text, unsigned short color, int width, TEXT_ALIGN_TYPE align, unsigned short flags)
{
	g_FontManager.DrawW(font, text, color, x, y, 30, width, align, flags);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawArt(int x, int y, unsigned short graphic, unsigned short color)
{
	g_Orion.DrawStaticArt(graphic, color, x, y);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawArtAnimated(int x, int y, unsigned short graphic, unsigned short color)
{
	g_Orion.DrawStaticArtAnimated(graphic, color, x, y);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawResizepicGump(int x, int y, unsigned short graphic, int width, int height)
{
	g_Orion.DrawResizepicGump(graphic, x, y, width, height);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawGump(int x, int y, unsigned short graphic, unsigned short color)
{
	g_Orion.DrawGump(graphic, color, x, y);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawGumppic(int x, int y, unsigned short graphic, unsigned short color, int width, int height)
{
	g_Orion.DrawGump(graphic, color, x, y, width, height);
}
//----------------------------------------------------------------------------------
//IUltimaOnline
//----------------------------------------------------------------------------------
uint64_t __cdecl FUNCBODY_GetLandFlags(unsigned short graphic)
{
	return g_Orion.GetLandFlags(graphic);
}
//----------------------------------------------------------------------------------
uint64_t __cdecl FUNCBODY_GetStaticFlags(unsigned short graphic)
{
	return g_Orion.GetStaticFlags(graphic);
}
//----------------------------------------------------------------------------------
int __cdecl FUNCBODY_GetValueInt(VALUE_KEY_INT key, int value)
{
	return g_Orion.ValueInt(key, value);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_SetValueInt(VALUE_KEY_INT key, int value)
{
	g_Orion.ValueInt(key, value);
}
//----------------------------------------------------------------------------------
IOrionString *__cdecl FUNCBODY_GetValueString(VALUE_KEY_STRING key, const char* value)
{
	return &g_OrionString(g_Orion.ValueString(key, value));
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_SetValueString(VALUE_KEY_STRING key, const char *value)
{
	g_Orion.ValueString(key, value);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_SetTargetData(unsigned char *buf, int size)
{
	WISP_DATASTREAM::CDataReader reader(buf, size);
	reader.Move(1);

	if (*buf == 0x6C)
		g_Target.SetData(reader);
	else
		g_Target.SetMultiData(reader);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_SendTargetObject(unsigned int serial)
{
	g_Target.Plugin_SendTargetObject(serial);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_SendTargetTile(unsigned short graphic, short x, short y, char z)
{
	g_Target.Plugin_SendTargetTile(graphic, x, y, z);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_SendTargetCancel()
{
	g_Target.Plugin_SendCancelTarget();
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_SendCastSpell(int index)
{
	if (index >= 0)
	{
		g_LastSpellIndex = index;

		CPacketCastSpell packet(index);
		SendMessage(g_OrionWindow.Handle, UOMSG_SEND, (WPARAM)packet.Data().data(), packet.Data().size());
	}
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_SendUseSkill(int index)
{
	if (index >= 0)
	{
		g_LastSkillIndex = index;

		CPacketUseSkill packet(index);
		SendMessage(g_OrionWindow.Handle, UOMSG_SEND, (WPARAM)packet.Data().data(), packet.Data().size());
	}
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_SendAsciiSpeech(const char *text, unsigned short color)
{
	if (!color)
		color = g_ConfigManager.SpeechColor;

	CPacketASCIISpeechRequest packet(text, ST_NORMAL, 3, color);
	SendMessage(g_OrionWindow.Handle, UOMSG_SEND, (WPARAM)packet.Data().data(), packet.Data().size());
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_SendUnicodeSpeech(const wchar_t *text, unsigned short color)
{
	if (!color)
		color = g_ConfigManager.SpeechColor;

	CPacketUnicodeSpeechRequest packet(text, ST_NORMAL, 3, color, (puchar)g_Language.c_str());
	SendMessage(g_OrionWindow.Handle, UOMSG_SEND, (WPARAM)packet.Data().data(), packet.Data().size());
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_SendRenameMount(uint serial, const char *text)
{
	CPacketRenameRequest packet(serial, text);
	SendMessage(g_OrionWindow.Handle, UOMSG_SEND, (WPARAM)packet.Data().data(), packet.Data().size());
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_SendMenuResponse(unsigned int serial, unsigned int id, int code)
{
	UOI_MENU_RESPONSE data = { serial, id, code };
	SendMessage(g_OrionWindow.Handle, UOMSG_MENU_RESPONSE, (WPARAM)&data, 0);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_DisplayStatusbarGump(unsigned int serial, int x, int y)
{
	g_Orion.DisplayStatusbarGump(serial, x, y);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_CloseStatusbarGump(unsigned int serial)
{
	g_GumpManager.CloseGump(serial, 0, GT_STATUSBAR);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_Logout()
{
	g_Orion.LogOut();
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_SecureTradingCheckState(unsigned int id1, bool state)
{
	CGumpSecureTrading *gump = (CGumpSecureTrading*)g_GumpManager.UpdateGump(id1, 0, GT_TRADE);

	if (gump != NULL)
	{
		gump->StateMy = state;

		CPacketTradeResponse packet(gump, 2);
		SendMessage(g_OrionWindow.Handle, UOMSG_SEND, (WPARAM)packet.Data().data(), packet.Data().size());
	}
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_SecureTradingClose(unsigned int id1)
{
	CGumpSecureTrading *gump = (CGumpSecureTrading*)g_GumpManager.GetGump(id1, 0, GT_TRADE);

	if (gump != NULL)
	{
		gump->RemoveMark = true;

		CPacketTradeResponse packet(gump, 1);
		SendMessage(g_OrionWindow.Handle, UOMSG_SEND, (WPARAM)packet.Data().data(), packet.Data().size());
	}
}
//----------------------------------------------------------------------------------
//IClilocManager
//----------------------------------------------------------------------------------
IOrionString *__cdecl FUNCBODY_GetClilocA(unsigned int clilocID, const char *defaultText)
{
	return &g_OrionString(g_ClilocManager.Cliloc(g_Language)->GetA(clilocID, false, defaultText));
}
//----------------------------------------------------------------------------------
IOrionString *__cdecl FUNCBODY_GetClilocW(unsigned int clilocID, const char *defaultText)
{
	return &g_OrionString(g_ClilocManager.Cliloc(g_Language)->GetW(clilocID, false, defaultText));
}
//----------------------------------------------------------------------------------
IOrionString *__cdecl FUNCBODY_GetClilocArguments(unsigned int clilocID, const wchar_t *args)
{
	return &g_OrionString(g_ClilocManager.ParseArgumentsToClilocString(clilocID, false, args));
}
//----------------------------------------------------------------------------------
//IColorManager
//----------------------------------------------------------------------------------
int __cdecl FUNCBODY_GetHuesCount()
{
	return g_ColorManager.GetHuesCount();
}
//----------------------------------------------------------------------------------
unsigned short __cdecl FUNCBODY_GetColor32To16(unsigned int &color)
{
	return g_ColorManager.Color32To16(color);
}
//----------------------------------------------------------------------------------
unsigned int __cdecl FUNCBODY_GetColor16To32(unsigned short &color)
{
	return g_ColorManager.Color16To32(color);
}
//----------------------------------------------------------------------------------
unsigned short __cdecl FUNCBODY_GetColorToGray(unsigned short &color)
{
	return g_ColorManager.ConvertToGray(color);
}
//----------------------------------------------------------------------------------
unsigned int __cdecl FUNCBODY_GetPolygoneColor(unsigned short c, unsigned short color)
{
	return g_ColorManager.GetPolygoneColor(c, color);
}
//----------------------------------------------------------------------------------
unsigned int __cdecl FUNCBODY_GetColor(unsigned short &c, unsigned short color)
{
	return g_ColorManager.GetColor(c, color);
}
//----------------------------------------------------------------------------------
unsigned int __cdecl FUNCBODY_GetPartialHueColor(unsigned short &c, unsigned short color)
{
	return g_ColorManager.GetPartialHueColor(c, color);
}
//----------------------------------------------------------------------------------
//IPathFinder
//----------------------------------------------------------------------------------
bool __cdecl FUNCBODY_GetCanWalk(unsigned char &direction, int &x, int &y, char &z)
{
	return g_PathFinder.CanWalk(direction, x, y, z);
}
//----------------------------------------------------------------------------------
bool __cdecl FUNCBODY_GetWalk(bool run, unsigned char direction)
{
	return SendMessage(g_OrionWindow.Handle, UOMSG_WALK, run, direction);
}
//----------------------------------------------------------------------------------
bool __cdecl FUNCBODY_GetWalkTo(int x, int y, int z, int distance)
{
	if (g_Player == NULL)
		return false;

	WISP_GEOMETRY::CPoint2Di startPoint(g_Player->GetX(), g_Player->GetY());

	if (!g_Player->m_Steps.empty())
	{
		CWalkData &wd = g_Player->m_Steps.back();

		startPoint.X = wd.X;
		startPoint.Y = wd.Y;
	}

	if (GetDistance(startPoint, WISP_GEOMETRY::CPoint2Di(x, y)) <= distance)
		return true;

	bool result = SendMessage(g_OrionWindow.Handle, UOMSG_PATHFINDING, ((x << 16) & 0xFFFF0000) | (y & 0xFFFF), ((x << 16) & 0xFFFF0000) | (distance & 0xFFFF));

	if (result)
	{
		while (g_PathFinder.AutoWalking)
			Sleep(100);

		if (g_Player == NULL)
			return false;

		WISP_GEOMETRY::CPoint2Di p(g_Player->GetX(), g_Player->GetY());

		if (!g_Player->m_Steps.empty())
		{
			CWalkData &wd = g_Player->m_Steps.back();

			p.X = wd.X;
			p.Y = wd.Y;
		}

		result = (GetDistance(p, WISP_GEOMETRY::CPoint2Di(x, y)) <= distance);
	}

	return result;
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_GetStopAutowalk()
{
	if (g_PathFinder.PathFindidngCanBeCancelled)
		g_PathFinder.StopAutoWalk();
}
//----------------------------------------------------------------------------------
bool __cdecl FUNCBODY_GetAutowalking()
{
	return g_PathFinder.AutoWalking;
}
//----------------------------------------------------------------------------------
//IFileManager
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_GetFileInfo(unsigned int index, ORION_RAW_FILE_INFO &info)
{
	WISP_FILE::CMappedFile *file = NULL;
	unsigned int extra = 0;

	switch (index)
	{
		case OFI_TILEDATA_MUL:
		{
			file = &g_FileManager.m_TiledataMul;
			break;
		}
		case OFI_MULTI_IDX:
		{
			file = &g_FileManager.m_MultiIdx;
			break;
		}
		case OFI_MULTI_MUL:
		{
			file = &g_FileManager.m_MultiMul;
			break;
		}
		case OFI_MULTI_UOP:
		{
			file = &g_FileManager.m_MultiCollection;
			break;
		}
		case OFI_HUES_MUL:
		{
			file = &g_FileManager.m_HuesMul;
			break;
		}
		case OFI_VERDATA_MUL:
		{
			file = &g_FileManager.m_VerdataMul;
			break;
		}
		case OFI_RADARCOL_MUL:
		{
			file = &g_FileManager.m_RadarcolMul;
			break;
		}
		case OFI_MAP_0_MUL:
		case OFI_MAP_1_MUL:
		case OFI_MAP_2_MUL:
		case OFI_MAP_3_MUL:
		case OFI_MAP_4_MUL:
		case OFI_MAP_5_MUL:
		{
			file = &g_FileManager.m_MapMul[index - OFI_MAP_0_MUL];
			break;
		}
		case OFI_MAP_0_UOP:
		case OFI_MAP_1_UOP:
		case OFI_MAP_2_UOP:
		case OFI_MAP_3_UOP:
		case OFI_MAP_4_UOP:
		case OFI_MAP_5_UOP:
		{
			file = &g_FileManager.m_MapUOP[index - OFI_MAP_0_UOP];
			break;
		}
		case OFI_MAPX_0_UOP:
		case OFI_MAPX_1_UOP:
		case OFI_MAPX_2_UOP:
		case OFI_MAPX_3_UOP:
		case OFI_MAPX_4_UOP:
		case OFI_MAPX_5_UOP:
		{
			file = &g_FileManager.m_MapXUOP[index - OFI_MAPX_0_UOP];
			break;
		}
		case OFI_STAIDX_0_MUL:
		case OFI_STAIDX_1_MUL:
		case OFI_STAIDX_2_MUL:
		case OFI_STAIDX_3_MUL:
		case OFI_STAIDX_4_MUL:
		case OFI_STAIDX_5_MUL:
		{
			file = &g_FileManager.m_StaticIdx[index - OFI_STAIDX_0_MUL];
			break;
		}
		case OFI_STATICS_0_MUL:
		case OFI_STATICS_1_MUL:
		case OFI_STATICS_2_MUL:
		case OFI_STATICS_3_MUL:
		case OFI_STATICS_4_MUL:
		case OFI_STATICS_5_MUL:
		{
			file = &g_FileManager.m_StaticMul[index - OFI_STATICS_0_MUL];
			break;
		}
		case OFI_MAP_DIF_0_MUL:
		case OFI_MAP_DIF_1_MUL:
		case OFI_MAP_DIF_2_MUL:
		case OFI_MAP_DIF_3_MUL:
		case OFI_MAP_DIF_4_MUL:
		case OFI_MAP_DIF_5_MUL:
		{
			file = &g_FileManager.m_MapDif[index - OFI_MAP_DIF_0_MUL];
			break;
		}
		case OFI_MAP_DIFL_0_MUL:
		case OFI_MAP_DIFL_1_MUL:
		case OFI_MAP_DIFL_2_MUL:
		case OFI_MAP_DIFL_3_MUL:
		case OFI_MAP_DIFL_4_MUL:
		case OFI_MAP_DIFL_5_MUL:
		{
			file = &g_FileManager.m_MapDifl[index - OFI_MAP_DIFL_0_MUL];
			break;
		}
		case OFI_STA_DIF_0_MUL:
		case OFI_STA_DIF_1_MUL:
		case OFI_STA_DIF_2_MUL:
		case OFI_STA_DIF_3_MUL:
		case OFI_STA_DIF_4_MUL:
		case OFI_STA_DIF_5_MUL:
		{
			file = &g_FileManager.m_StaDif[index - OFI_STA_DIF_0_MUL];
			break;
		}
		case OFI_STA_DIFI_0_MUL:
		case OFI_STA_DIFI_1_MUL:
		case OFI_STA_DIFI_2_MUL:
		case OFI_STA_DIFI_3_MUL:
		case OFI_STA_DIFI_4_MUL:
		case OFI_STA_DIFI_5_MUL:
		{
			file = &g_FileManager.m_StaDifi[index - OFI_STA_DIFI_0_MUL];
			break;
		}
		case OFI_STA_DIFL_0_MUL:
		case OFI_STA_DIFL_1_MUL:
		case OFI_STA_DIFL_2_MUL:
		case OFI_STA_DIFL_3_MUL:
		case OFI_STA_DIFL_4_MUL:
		case OFI_STA_DIFL_5_MUL:
		{
			file = &g_FileManager.m_StaDifl[index - OFI_STA_DIFL_0_MUL];
			break;
		}
		case OFI_CLILOC_MUL:
		{
			QFOR(item, g_ClilocManager.m_Items, CCliloc*)
			{
				if (item->Loaded && item->m_File.Start != NULL && item->Language == "enu")
				{
					file = &item->m_File;
					extra = 'ENU';
					break;
				}
			}

			break;
		}
		default:
			break;
	}

	if (file != NULL)
	{
		info.Address = (uint64_t)file->Start;
		info.Size = (uint64_t)file->Size;
		info.Extra = extra;
	}
	else
	{
		info.Address = 0;
		info.Size = 0;
		info.Extra = 0;
	}
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_GetLandArtInfo(unsigned short index, ORION_RAW_ART_INFO &info)
{
	if (index < MAX_LAND_DATA_INDEX_COUNT)
	{
		CIndexObjectLand &landData = g_Orion.m_LandDataIndex[index];

		if (landData.Address != NULL)
		{
			info.Address = (uint64_t)landData.Address;
			info.Size = (uint64_t)landData.DataSize;

			if (landData.UopBlock != NULL)
				info.CompressedSize = (uint64_t)landData.UopBlock->CompressedSize;
			else
				info.CompressedSize = 0;

			return;
		}
	}

	info.Address = 0;
	info.Size = 0;
	info.CompressedSize = 0;
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_GetStaticArtInfo(unsigned short index, ORION_RAW_ART_INFO &info)
{
	if (index < MAX_STATIC_DATA_INDEX_COUNT)
	{
		CIndexObjectStatic &staticData = g_Orion.m_StaticDataIndex[index];

		if (staticData.Address != NULL)
		{
			info.Address = (uint64_t)staticData.Address;
			info.Size = (uint64_t)staticData.DataSize;

			if (staticData.UopBlock != NULL)
				info.CompressedSize = (uint64_t)staticData.UopBlock->CompressedSize;
			else
				info.CompressedSize = 0;

			return;
		}
	}

	info.Address = 0;
	info.Size = 0;
	info.CompressedSize = 0;
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_GetGumpArtInfo(unsigned short index, ORION_RAW_GUMP_INFO &info)
{
	if (index < MAX_GUMP_DATA_INDEX_COUNT)
	{
		CIndexGump &gumpData = g_Orion.m_GumpDataIndex[index];

		if (gumpData.Address != NULL)
		{
			info.Address = (uint64_t)gumpData.Address;
			info.Size = (uint64_t)gumpData.DataSize;
			info.Width = gumpData.Width;
			info.Height = gumpData.Height;

			if (gumpData.UopBlock != NULL)
				info.CompressedSize = (uint64_t)gumpData.UopBlock->CompressedSize;
			else
				info.CompressedSize = 0;

			return;
		}
	}

	info.Address = 0;
	info.Size = 0;
	info.CompressedSize = 0;
	info.Width = 0;
	info.Height = 0;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
IGLEngine g_Interface_GL =
{
	0,
	sizeof(IGLEngine),
	FUNCBODY_PushScissor,
	FUNCBODY_PopScissor,
	FUNCBODY_DrawLine,
	FUNCBODY_DrawPolygone,
	FUNCBODY_DrawCircle,
	FUNCBODY_DrawTextA,
	FUNCBODY_DrawTextW,
	FUNCBODY_DrawArt,
	FUNCBODY_DrawArtAnimated,
	FUNCBODY_DrawResizepicGump,
	FUNCBODY_DrawGump,
	FUNCBODY_DrawGumppic
};
//----------------------------------------------------------------------------------
IUltimaOnline g_Interface_UO =
{
	2,
	sizeof(IUltimaOnline),
	FUNCBODY_GetLandFlags,
	FUNCBODY_GetStaticFlags,
	FUNCBODY_GetValueInt,
	FUNCBODY_SetValueInt,
	FUNCBODY_GetValueString,
	FUNCBODY_SetValueString,
	FUNCBODY_SetTargetData,
	FUNCBODY_SendTargetObject,
	FUNCBODY_SendTargetTile,
	FUNCBODY_SendTargetCancel,
	FUNCBODY_SendCastSpell,
	FUNCBODY_SendUseSkill,
	FUNCBODY_SendAsciiSpeech,
	FUNCBODY_SendUnicodeSpeech,
	FUNCBODY_SendRenameMount,
	FUNCBODY_SendMenuResponse,
	FUNCBODY_DisplayStatusbarGump,
	FUNCBODY_CloseStatusbarGump,
	FUNCBODY_Logout,
	FUNCBODY_SecureTradingCheckState,
	FUNCBODY_SecureTradingClose
};
//----------------------------------------------------------------------------------
IClilocManager g_Interface_ClilocManager =
{
	0,
	sizeof(IClilocManager),
	FUNCBODY_GetClilocA,
	FUNCBODY_GetClilocW,
	FUNCBODY_GetClilocArguments
};
//----------------------------------------------------------------------------------
IColorManager g_Interface_ColorManager =
{
	0,
	sizeof(IColorManager),
	FUNCBODY_GetHuesCount,
	FUNCBODY_GetColor32To16,
	FUNCBODY_GetColor16To32,
	FUNCBODY_GetColorToGray,
	FUNCBODY_GetPolygoneColor,
	FUNCBODY_GetColor,
	FUNCBODY_GetPartialHueColor
};
//----------------------------------------------------------------------------------
IPathFinder g_Interface_PathFinder =
{
	0,
	sizeof(IPathFinder),
	FUNCBODY_GetCanWalk,
	FUNCBODY_GetWalk,
	FUNCBODY_GetWalkTo,
	FUNCBODY_GetStopAutowalk,
	FUNCBODY_GetAutowalking
};
//----------------------------------------------------------------------------------
IFileManager g_Interface_FileManager =
{
	0,
	sizeof(IFileManager),
	FUNCBODY_GetFileInfo,
	FUNCBODY_GetLandArtInfo,
	FUNCBODY_GetStaticArtInfo,
	FUNCBODY_GetGumpArtInfo
};
//----------------------------------------------------------------------------------
