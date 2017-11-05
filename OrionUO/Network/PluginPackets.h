/***********************************************************************************
**
** PluginPackets.h
**
** Copyright (C) November 2017 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef PLUGINPACKETS_H
#define PLUGINPACKETS_H
//----------------------------------------------------------------------------------
class CPluginPacket : public WISP_DATASTREAM::CDataWritter
{
public:
	CPluginPacket();
	CPluginPacket(const int &size, const bool &autoResize = false);

	void SendToPlugin();
};
//----------------------------------------------------------------------------------
class CPluginPacketSkillsList : public CPluginPacket
{
public:
	CPluginPacketSkillsList();
};
//----------------------------------------------------------------------------------
class CPluginPacketSpellsList : public CPluginPacket
{
public:
	CPluginPacketSpellsList();
};
//----------------------------------------------------------------------------------
class CPluginPacketMacrosList : public CPluginPacket
{
public:
	CPluginPacketMacrosList();
};
//----------------------------------------------------------------------------------
#endif //PLUGINPACKETS_H
//----------------------------------------------------------------------------------
