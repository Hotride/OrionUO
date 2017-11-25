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
class CPluginPacketFileInfo : public CPluginPacket
{
public:
	CPluginPacketFileInfo(const int &index, const uint64 &address, const uint64 &size);
};
//----------------------------------------------------------------------------------
class CPluginPacketFileInfoLocalized : public CPluginPacket
{
public:
	CPluginPacketFileInfoLocalized(const int &index, const uint64 &address, const uint64 &size, const string &language);
};
//----------------------------------------------------------------------------------
class CPluginPacketStaticArtGraphicDataInfo : public CPluginPacket
{
public:
	CPluginPacketStaticArtGraphicDataInfo(const ushort &graphic, const uint64 &address, const uint64 &size, const uint64 &compressedSize);
};
//----------------------------------------------------------------------------------
class CPluginPacketGumpArtGraphicDataInfo : public CPluginPacket
{
public:
	CPluginPacketGumpArtGraphicDataInfo(const ushort &graphic, const uint64 &address, const uint64 &size, const uint64 &compressedSize, const ushort &width, const ushort &height);
};
//----------------------------------------------------------------------------------
class CPluginPacketFilesTransfered : public CPluginPacket
{
public:
	CPluginPacketFilesTransfered();
};
//----------------------------------------------------------------------------------
#endif //PLUGINPACKETS_H
//----------------------------------------------------------------------------------
