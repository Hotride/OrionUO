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
    CPluginPacket(int size, bool autoResize = false);

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
    CPluginPacketFileInfo(int index, uint64 address, uint64 size);
};
//----------------------------------------------------------------------------------
class CPluginPacketFileInfoLocalized : public CPluginPacket
{
public:
    CPluginPacketFileInfoLocalized(int index, uint64 address, uint64 size, const string &language);
};
//----------------------------------------------------------------------------------
class CPluginPacketStaticArtGraphicDataInfo : public CPluginPacket
{
public:
    CPluginPacketStaticArtGraphicDataInfo(
        ushort graphic, uint64 address, uint64 size, uint64 compressedSize);
};
//----------------------------------------------------------------------------------
class CPluginPacketGumpArtGraphicDataInfo : public CPluginPacket
{
public:
    CPluginPacketGumpArtGraphicDataInfo(
        ushort graphic,
        uint64 address,
        uint64 size,
        uint64 compressedSize,
        ushort width,
        ushort height);
};
//----------------------------------------------------------------------------------
class CPluginPacketFilesTransfered : public CPluginPacket
{
public:
    CPluginPacketFilesTransfered();
};
//----------------------------------------------------------------------------------
class CPluginPacketOpenMap : public CPluginPacket
{
public:
    CPluginPacketOpenMap();
};
//----------------------------------------------------------------------------------
#endif //PLUGINPACKETS_H
//----------------------------------------------------------------------------------
