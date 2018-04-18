﻿// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** PluginPackets.cpp
**
** Copyright (C) November 2017 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CPluginPacket::CPluginPacket()
: WISP_DATASTREAM::CDataWritter()
{
	WriteUInt8(0xFC);
	WriteUInt16BE(0); //size reserved
}
//----------------------------------------------------------------------------------
CPluginPacket::CPluginPacket(int size, bool autoResize)
: WISP_DATASTREAM::CDataWritter(size, autoResize)
{
}
//----------------------------------------------------------------------------------
void CPluginPacket::SendToPlugin()
{
	if (m_Data.size() >= 5)
	{
		pack16(&m_Data[1], (ushort)m_Data.size());
		g_PluginManager.PacketRecv(&m_Data[0], (int)m_Data.size());
	}
}
//----------------------------------------------------------------------------------
CPluginPacketSkillsList::CPluginPacketSkillsList()
: CPluginPacket()
{
	int count = g_SkillsManager.Count;

	WriteUInt16BE(OIPMT_SKILL_LIST);
	WriteUInt16BE(count);

	IFOR(i, 0, count)
	{
		CSkill *skill = g_SkillsManager.Get((uint)i);

		if (skill == NULL)
			continue;

		WriteUInt8(skill->Button);
		WriteString(skill->Name);
	}
}
//----------------------------------------------------------------------------------
CPluginPacketSpellsList::CPluginPacketSpellsList()
: CPluginPacket()
{
	WriteUInt16BE(OIPMT_SPELL_LIST);
	WriteUInt16BE(7);

	WriteUInt16BE(CGumpSpellbook::SPELLBOOK_1_SPELLS_COUNT);

	IFOR(i, 0, CGumpSpellbook::SPELLBOOK_1_SPELLS_COUNT)
		WriteString(CGumpSpellbook::m_SpellName1[i][0]);

	WriteUInt16BE(CGumpSpellbook::SPELLBOOK_2_SPELLS_COUNT);

	IFOR(i, 0, CGumpSpellbook::SPELLBOOK_2_SPELLS_COUNT)
		WriteString(CGumpSpellbook::m_SpellName2[i][0]);

	WriteUInt16BE(CGumpSpellbook::SPELLBOOK_3_SPELLS_COUNT);

	IFOR(i, 0, CGumpSpellbook::SPELLBOOK_3_SPELLS_COUNT)
		WriteString(CGumpSpellbook::m_SpellName3[i][0]);

	WriteUInt16BE(CGumpSpellbook::SPELLBOOK_4_SPELLS_COUNT);

	IFOR(i, 0, CGumpSpellbook::SPELLBOOK_4_SPELLS_COUNT)
		WriteString(CGumpSpellbook::m_SpellName4[i]);

	WriteUInt16BE(CGumpSpellbook::SPELLBOOK_5_SPELLS_COUNT);

	IFOR(i, 0, CGumpSpellbook::SPELLBOOK_5_SPELLS_COUNT)
		WriteString(CGumpSpellbook::m_SpellName5[i]);

	WriteUInt16BE(CGumpSpellbook::SPELLBOOK_6_SPELLS_COUNT);

	IFOR(i, 0, CGumpSpellbook::SPELLBOOK_6_SPELLS_COUNT)
		WriteString(CGumpSpellbook::m_SpellName6[i][0]);

	WriteUInt16BE(CGumpSpellbook::SPELLBOOK_7_SPELLS_COUNT);

	IFOR(i, 0, CGumpSpellbook::SPELLBOOK_7_SPELLS_COUNT)
		WriteString(CGumpSpellbook::m_SpellName7[i][0]);
}
//----------------------------------------------------------------------------------
CPluginPacketMacrosList::CPluginPacketMacrosList()
: CPluginPacket()
{
	WriteUInt16BE(OIPMT_MACRO_LIST);
	WriteUInt16BE(CMacro::MACRO_ACTION_NAME_COUNT);

	IFOR(i, 0, CMacro::MACRO_ACTION_NAME_COUNT)
	{
		WriteString(CMacro::m_MacroActionName[i]);

		int count = 0;
		int offset = 0;
		CMacro::GetBoundByCode((MACRO_CODE)i, count, offset);

		WriteUInt16BE(count);

		IFOR(j, 0, count)
		{
			WriteString(CMacro::m_MacroAction[j + offset]);
		}
	}
}
//----------------------------------------------------------------------------------
CPluginPacketFileInfo::CPluginPacketFileInfo(int index, uint64 address, uint64 size)
: CPluginPacket()
{
	WriteUInt16BE(OIPMT_FILE_INFO);
	WriteUInt16BE(index);
	WriteUInt64BE(address);
	WriteUInt64BE(size);
}
//----------------------------------------------------------------------------------
CPluginPacketFileInfoLocalized::CPluginPacketFileInfoLocalized(int index, uint64 address, uint64 size, const string &language)
: CPluginPacket()
{
	WriteUInt16BE(OIPMT_FILE_INFO_LOCALIZED);
	WriteUInt16BE(index);
	WriteUInt64BE(address);
	WriteUInt64BE(size);
	WriteString(language);
}
//----------------------------------------------------------------------------------
CPluginPacketStaticArtGraphicDataInfo::CPluginPacketStaticArtGraphicDataInfo(ushort graphic, uint64 address, uint64 size, uint64 compressedSize)
: CPluginPacket()
{
	WriteUInt16BE(OIPMT_GRAPHIC_DATA_INFO);
	WriteUInt8(OGDT_STATIC_ART);
	WriteUInt16BE(graphic);
	WriteUInt64BE(address);
	WriteUInt64BE(size);
	WriteUInt64BE(compressedSize);
}
//----------------------------------------------------------------------------------
CPluginPacketGumpArtGraphicDataInfo::CPluginPacketGumpArtGraphicDataInfo(ushort graphic, uint64 address, uint64 size, uint64 compressedSize, ushort width, ushort height)
: CPluginPacket()
{
	WriteUInt16BE(OIPMT_GRAPHIC_DATA_INFO);
	WriteUInt8(OGDT_GUMP_ART);
	WriteUInt16BE(graphic);
	WriteUInt64BE(address);
	WriteUInt64BE(size);
	WriteUInt64BE(compressedSize);
	WriteUInt16BE(width);
	WriteUInt16BE(height);
}
//----------------------------------------------------------------------------------
CPluginPacketFilesTransfered::CPluginPacketFilesTransfered()
: CPluginPacket()
{
	WriteUInt16BE(OIPMT_FILES_TRANSFERED);
}
//----------------------------------------------------------------------------------
CPluginPacketOpenMap::CPluginPacketOpenMap()
: CPluginPacket()
{
	WriteUInt16BE(OIPMT_OPEN_MAP);
}
//----------------------------------------------------------------------------------
