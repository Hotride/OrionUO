// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** FileManager.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
#include "FileSystem.h"
CFileManager g_FileManager;
//----------------------------------------------------------------------------------
CUopMappedFile::CUopMappedFile()
    : WISP_FILE::CMappedFile()
{
}
//----------------------------------------------------------------------------------
CUopMappedFile::~CUopMappedFile()
{
}
//----------------------------------------------------------------------------------
void CUopMappedFile::Add(uint64 hash, const CUopBlockHeader &item)
{
    m_Map[hash] = item;
}
//----------------------------------------------------------------------------------
CUopBlockHeader *CUopMappedFile::GetBlock(uint64 hash)
{
    std::unordered_map<uint64, CUopBlockHeader>::iterator found = m_Map.find(hash);

    if (found != m_Map.end())
        return &found->second;

    return NULL;
}
//----------------------------------------------------------------------------------
UCHAR_LIST CUopMappedFile::GetData(const CUopBlockHeader &block)
{
    ResetPtr();
    Move((int)block.Offset);

    uLongf compressedSize = block.CompressedSize;
    uLongf decompressedSize = block.DecompressedSize;
    UCHAR_LIST result(decompressedSize, 0);

    if (compressedSize && compressedSize != decompressedSize)
    {
        int z_err = uncompress(&result[0], &decompressedSize, Ptr, compressedSize);

        if (z_err != Z_OK)
        {
            LOG("Uncompress error: %i\n", z_err);
            result.clear();
        }
    }
    else
        memcpy(&result[0], &Ptr[0], decompressedSize);

    return result;
}
//----------------------------------------------------------------------------------
CFileManager::CFileManager()
    : WISP_DATASTREAM::CDataReader()
{
}
//----------------------------------------------------------------------------------
CFileManager::~CFileManager()
{
}
//----------------------------------------------------------------------------------
bool CFileManager::Load()
{
    WISPFUN_DEBUG("");
    LOG("Client Verison: %d\n", g_PacketManager.GetClientVersion());

    if (g_PacketManager.GetClientVersion() >= CV_7000 && LoadUOPFile(m_MainMisc, "MainMisc.uop"))
        return LoadWithUOP();

    if (!m_ArtIdx.Load(g_App.UOFilesPath("artidx.mul")))
        return false;
    else if (!m_ArtMul.Load(g_App.UOFilesPath("art.mul")))
        return false;
    else if (!m_GumpIdx.Load(g_App.UOFilesPath("gumpidx.mul")))
        return false;
    else if (!m_GumpMul.Load(g_App.UOFilesPath("gumpart.mul")))
        return false;
    else if (!m_SoundIdx.Load(g_App.UOFilesPath("soundidx.mul")))
        return false;
    else if (!m_SoundMul.Load(g_App.UOFilesPath("sound.mul")))
        return false;
    else if (!m_AnimIdx[0].Load(g_App.UOFilesPath("anim.idx")))
        return false;
    else if (!m_LightIdx.Load(g_App.UOFilesPath("lightidx.mul")))
        return false;
    else if (!m_MultiIdx.Load(g_App.UOFilesPath("multi.idx")))
        return false;
    else if (!m_SkillsIdx.Load(g_App.UOFilesPath("Skills.idx"))) // FIXME: need be case insensitive
        return false;
    else if (!m_MultiMap.Load(g_App.UOFilesPath("Multimap.rle"))) // FIXME: need be case insensitive
        return false;
    else if (!m_TextureIdx.Load(g_App.UOFilesPath("texidx.mul")))
        return false;
    else if (!TryOpenFileStream(m_AnimMul[0], g_App.UOFilesPath("anim.mul")))
        return false;
    else if (!m_AnimdataMul.Load(g_App.UOFilesPath("animdata.mul")))
        return false;
    else if (!m_HuesMul.Load(g_App.UOFilesPath("hues.mul")))
        return false;
    else if (!m_LightMul.Load(g_App.UOFilesPath("light.mul")))
        return false;
    else if (!m_MultiMul.Load(g_App.UOFilesPath("multi.mul")))
        return false;
    else if (!m_RadarcolMul.Load(g_App.UOFilesPath("radarcol.mul")))
        return false;
    else if (!m_SkillsMul.Load(g_App.UOFilesPath("skills.mul")))
        return false;
    else if (!m_TextureMul.Load(g_App.UOFilesPath("texmaps.mul")))
        return false;
    else if (!m_TiledataMul.Load(g_App.UOFilesPath("tiledata.mul")))
        return false;

    m_SpeechMul.Load(g_App.UOFilesPath("speech.mul"));
    m_LangcodeIff.Load(g_App.UOFilesPath("Langcode.iff"));

    IFOR (i, 0, 6)
    {
        if (i > 1)
        {
            m_AnimIdx[i].Load(g_App.UOFilesPath("anim%i.idx", i));
            TryOpenFileStream(m_AnimMul[i], g_App.UOFilesPath("anim%i.mul", i));
        }

        m_MapMul[i].Load(g_App.UOFilesPath("map%i.mul", i));

        m_StaticIdx[i].Load(g_App.UOFilesPath("staidx%i.mul", i));
        m_StaticMul[i].Load(g_App.UOFilesPath("statics%i.mul", i));
        m_FacetMul[i].Load(g_App.UOFilesPath("facet0%i.mul", i));

        m_MapDifl[i].Load(g_App.UOFilesPath("mapdifl%i.mul", i));
        m_MapDif[i].Load(g_App.UOFilesPath("mapdif%i.mul", i));

        m_StaDifl[i].Load(g_App.UOFilesPath("stadifl%i.mul", i));
        m_StaDifi[i].Load(g_App.UOFilesPath("stadifi%i.mul", i));
        m_StaDif[i].Load(g_App.UOFilesPath("stadif%i.mul", i));
    }

    IFOR (i, 0, 20)
    {
        auto s = i != 0 ? g_App.UOFilesPath("unifont%i.mul", i) : g_App.UOFilesPath("unifont.mul");
        if (m_UnifontMul[i].Load(s))
            UnicodeFontsCount++;
    }

    if (UseVerdata && !m_VerdataMul.Load(g_App.UOFilesPath("verdata.mul")))
        UseVerdata = false;

    return true;
}
//----------------------------------------------------------------------------------
bool CFileManager::LoadWithUOP()
{
    if (true)
    {
        //m_MainMisc
    }

    //Try to use map uop files first, if we can, we will use them.

    if (!LoadUOPFile(m_ArtLegacyMUL, "artLegacyMUL.uop"))
    {
        if (!m_ArtIdx.Load(g_App.UOFilesPath("artidx.mul")))
            return false;
        else if (!m_ArtMul.Load(g_App.UOFilesPath("art.mul")))
            return false;
    }

    if (!LoadUOPFile(m_GumpartLegacyMUL, "gumpartLegacyMUL.uop"))
    {
        if (!m_GumpIdx.Load(g_App.UOFilesPath("gumpidx.mul")))
            return false;
        else if (!m_GumpMul.Load(g_App.UOFilesPath("gumpart.mul")))
            return false;

        UseUOPGumps = false;
    }
    else
        UseUOPGumps = true;

    if (!LoadUOPFile(m_SoundLegacyMUL, "soundLegacyMUL.uop"))
    {
        if (!m_SoundIdx.Load(g_App.UOFilesPath("soundidx.mul")))
            return false;
        else if (!m_SoundMul.Load(g_App.UOFilesPath("sound.mul")))
            return false;
    }

    if (!LoadUOPFile(m_MultiCollection, "MultiCollection.uop"))
    {
        if (!m_MultiIdx.Load(g_App.UOFilesPath("multi.idx")))
            return false;
        else if (!m_MultiMul.Load(g_App.UOFilesPath("multi.mul")))
            return false;
    }

    LoadUOPFile(m_AnimationSequence, "AnimationSequence.uop");
    LoadUOPFile(m_Tileart, "tileart.uop");

    /* Эти файлы не используются самой последней версией клиента 7.0.52.2
	if (!m_tileart.Load(g_App.UOFilesPath("tileart.uop")))
	return false;
	if (!m_AnimationSequence.Load(g_App.UOFilesPath("AnimationSequence.uop")))
	return false;
	*/

    if (!m_AnimIdx[0].Load(g_App.UOFilesPath("anim.idx")))
        return false;
    if (!m_LightIdx.Load(g_App.UOFilesPath("lightidx.mul")))
        return false;
    else if (!m_SkillsIdx.Load(g_App.UOFilesPath("Skills.idx"))) // FIXME: need be case insensitive
        return false;
    else if (!m_MultiMap.Load(g_App.UOFilesPath("Multimap.rle"))) // FIXME: need be case insensitive
        return false;
    else if (!m_TextureIdx.Load(g_App.UOFilesPath("texidx.mul")))
        return false;
    else if (!TryOpenFileStream(m_AnimMul[0], g_App.UOFilesPath("anim.mul")))
        return false;
    else if (!m_AnimdataMul.Load(g_App.UOFilesPath("animdata.mul")))
        return false;
    else if (!m_HuesMul.Load(g_App.UOFilesPath("hues.mul")))
        return false;
    else if (!m_LightMul.Load(g_App.UOFilesPath("light.mul")))
        return false;
    else if (!m_RadarcolMul.Load(g_App.UOFilesPath("radarcol.mul")))
        return false;
    else if (!m_SkillsMul.Load(g_App.UOFilesPath("skills.mul")))
        return false;
    else if (!m_TextureMul.Load(g_App.UOFilesPath("texmaps.mul")))
        return false;
    else if (!m_TiledataMul.Load(g_App.UOFilesPath("tiledata.mul")))
        return false;

    m_SpeechMul.Load(g_App.UOFilesPath("speech.mul"));
    m_LangcodeIff.Load(g_App.UOFilesPath("Langcode.iff"));

    IFOR (i, 0, 6)
    {
        if (i > 1)
        {
            m_AnimIdx[i].Load(g_App.UOFilesPath("anim%i.idx", i));
            TryOpenFileStream(m_AnimMul[i], g_App.UOFilesPath("anim%i.mul", i));
        }

        string mapName = string("map") + std::to_string(i);

        if (!LoadUOPFile(m_MapUOP[i], (mapName + "LegacyMUL.uop").c_str()))
            m_MapMul[i].Load(g_App.UOFilesPath((mapName + ".mul")));

        m_StaticIdx[i].Load(g_App.UOFilesPath("staidx%i.mul", i));
        m_StaticMul[i].Load(g_App.UOFilesPath("statics%i.mul", i));
        m_FacetMul[i].Load(g_App.UOFilesPath("facet0%i.mul", i));

        m_MapDifl[i].Load(g_App.UOFilesPath("mapdifl%i.mul", i));
        m_MapDif[i].Load(g_App.UOFilesPath("mapdif%i.mul", i));

        m_StaDifl[i].Load(g_App.UOFilesPath("stadifl%i.mul", i));
        m_StaDifi[i].Load(g_App.UOFilesPath("stadifi%i.mul", i));
        m_StaDif[i].Load(g_App.UOFilesPath("stadif%i.mul", i));
    }

    IFOR (i, 0, 20)
    {
        auto s = i != 0 ? g_App.UOFilesPath("unifont%i.mul", i) : g_App.UOFilesPath("unifont.mul");
        if (m_UnifontMul[i].Load(s))
            UnicodeFontsCount++;
    }

    if (UseVerdata && !m_VerdataMul.Load(g_App.UOFilesPath("verdata.mul")))
        UseVerdata = false;

    return true;
}
//----------------------------------------------------------------------------------
void CFileManager::Unload()
{
    //WISPFUN_DEBUG("c142_f2");
    m_ArtIdx.Unload();
    m_GumpIdx.Unload();
    m_SoundIdx.Unload();
    m_ArtMul.Unload();
    m_GumpMul.Unload();
    m_SoundMul.Unload();
    m_ArtLegacyMUL.Unload();
    m_GumpartLegacyMUL.Unload();
    m_SoundLegacyMUL.Unload();
    m_Tileart.Unload();
    m_MultiCollection.Unload();
    m_AnimationSequence.Unload();
    m_MainMisc.Unload();

    m_LightIdx.Unload();
    m_MultiIdx.Unload();
    m_SkillsIdx.Unload();

    m_MultiMap.Unload();
    m_TextureIdx.Unload();
    m_SpeechMul.Unload();
    m_AnimdataMul.Unload();

    m_HuesMul.Unload();

    m_LightMul.Unload();
    m_MultiMul.Unload();
    m_RadarcolMul.Unload();
    m_SkillsMul.Unload();

    m_TextureMul.Unload();
    m_TiledataMul.Unload();

    m_LangcodeIff.Unload();

    IFOR (i, 0, 6)
    {
        m_AnimIdx[i].Unload();
        m_AnimMul[i].close();
        m_MapUOP[i].Unload();
        m_MapXUOP[i].Unload();
        m_MapMul[i].Unload();
        m_StaticIdx[i].Unload();
        m_StaticMul[i].Unload();
        m_FacetMul[i].Unload();

        m_MapDifl[i].Unload();
        m_MapDif[i].Unload();

        m_StaDifl[i].Unload();
        m_StaDifi[i].Unload();
        m_StaDif[i].Unload();
    }

    IFOR (i, 0, 20)
        m_UnifontMul[i].Unload();

    m_VerdataMul.Unload();
}
//----------------------------------------------------------------------------------
void CFileManager::SendFilesInfo()
{
    if (m_TiledataMul.Start != NULL)
        CPluginPacketFileInfo(
            OFI_TILEDATA_MUL, (uint64)m_TiledataMul.Start, (uint64)m_TiledataMul.Size)
            .SendToPlugin();

    if (m_MultiIdx.Start != NULL)
        CPluginPacketFileInfo(OFI_MULTI_IDX, (uint64)m_MultiIdx.Start, (uint64)m_MultiIdx.Size)
            .SendToPlugin();

    if (m_MultiMul.Start != NULL)
        CPluginPacketFileInfo(OFI_MULTI_MUL, (uint64)m_MultiMul.Start, (uint64)m_MultiMul.Size)
            .SendToPlugin();

    if (m_MultiCollection.Start != NULL)
        CPluginPacketFileInfo(
            OFI_MULTI_UOP, (uint64)m_MultiCollection.Start, (uint64)m_MultiCollection.Size)
            .SendToPlugin();

    if (m_HuesMul.Start != NULL)
        CPluginPacketFileInfo(OFI_HUES_MUL, (uint64)m_HuesMul.Start, (uint64)m_HuesMul.Size)
            .SendToPlugin();

    IFOR (i, 0, 6)
    {
        if (m_MapMul[i].Start != NULL)
            CPluginPacketFileInfo(
                OFI_MAP_0_MUL + i, (uint64)m_MapMul[i].Start, (uint64)m_MapMul[i].Size)
                .SendToPlugin();

        if (m_MapUOP[i].Start != NULL)
            CPluginPacketFileInfo(
                OFI_MAP_0_UOP + i, (uint64)m_MapUOP[i].Start, (uint64)m_MapUOP[i].Size)
                .SendToPlugin();

        if (m_MapXUOP[i].Start != NULL)
            CPluginPacketFileInfo(
                OFI_MAPX_0_UOP + i, (uint64)m_MapXUOP[i].Start, (uint64)m_MapXUOP[i].Size)
                .SendToPlugin();

        if (m_StaticIdx[i].Start != NULL)
            CPluginPacketFileInfo(
                OFI_STAIDX_0_MUL + i, (uint64)m_StaticIdx[i].Start, (uint64)m_StaticIdx[i].Size)
                .SendToPlugin();

        if (m_StaticMul[i].Start != NULL)
            CPluginPacketFileInfo(
                OFI_STATICS_0_MUL + i, (uint64)m_StaticMul[i].Start, (uint64)m_StaticMul[i].Size)
                .SendToPlugin();

        if (m_MapDif[i].Start != NULL)
            CPluginPacketFileInfo(
                OFI_MAP_DIF_0_MUL + i, (uint64)m_MapDif[i].Start, (uint64)m_MapDif[i].Size)
                .SendToPlugin();

        if (m_MapDifl[i].Start != NULL)
            CPluginPacketFileInfo(
                OFI_MAP_DIFL_0_MUL + i, (uint64)m_MapDifl[i].Start, (uint64)m_MapDifl[i].Size)
                .SendToPlugin();

        if (m_StaDif[i].Start != NULL)
            CPluginPacketFileInfo(
                OFI_STA_DIF_0_MUL + i, (uint64)m_StaDif[i].Start, (uint64)m_StaDif[i].Size)
                .SendToPlugin();

        if (m_StaDifi[i].Start != NULL)
            CPluginPacketFileInfo(
                OFI_STA_DIFI_0_MUL + i, (uint64)m_StaDifi[i].Start, (uint64)m_StaDifi[i].Size)
                .SendToPlugin();

        if (m_StaDifl[i].Start != NULL)
            CPluginPacketFileInfo(
                OFI_STA_DIFL_0_MUL + i, (uint64)m_StaDifl[i].Start, (uint64)m_StaDifl[i].Size)
                .SendToPlugin();
    }

    if (m_VerdataMul.Start != NULL)
        CPluginPacketFileInfo(
            OFI_VERDATA_MUL, (uint64)m_VerdataMul.Start, (uint64)m_VerdataMul.Size)
            .SendToPlugin();

    if (m_RadarcolMul.Start != NULL)
        CPluginPacketFileInfo(
            OFI_RADARCOL_MUL, (uint64)m_RadarcolMul.Start, (uint64)m_RadarcolMul.Size)
            .SendToPlugin();

    QFOR(item, g_ClilocManager.m_Items, CCliloc *)
    {
        if (item->Loaded && item->m_File.Start != NULL)
            CPluginPacketFileInfoLocalized(
                OFI_CLILOC_MUL,
                (uint64)item->m_File.Start,
                (uint64)item->m_File.Size,
                item->Language)
                .SendToPlugin();
    }
}
//----------------------------------------------------------------------------------
void CFileManager::TryReadUOPAnimations()
{
    std::thread readThread(&CFileManager::ReadTask, this);
    readThread.detach();
}
//----------------------------------------------------------------------------------
void CFileManager::ReadTask()
{
    std::unordered_map<uint64_t, UOPAnimationData> hashes;
    IFOR (i, 1, 5)
    {
        char magic[4];
        char version[4];
        char signature[4];
        char nextBlock[8];

        std::fstream *animFile = new std::fstream();
        if (!animFile)
            continue;

        auto path{ g_App.UOFilesPath("AnimationFrame%i.uop", i) };
        if (!FileExists(path))
            continue;

        animFile->open(path, std::ios::binary | std::ios::in);
        animFile->read(magic, 4);
        animFile->read(version, 4);
        animFile->read(signature, 4);
        animFile->read(nextBlock, 8);
        animFile->seekg(*reinterpret_cast<uint64_t *>(nextBlock), std::ios::beg);

        do
        {
            char fileCount[4];
            char offset[8];
            char headerlength[4];
            char compressedlength[4];
            char hash[8];
            char decompressedlength[4];
            char skip1[4];
            char skip2[2];

            animFile->read(fileCount, 4);
            animFile->read(nextBlock, 8);
            int count = *reinterpret_cast<unsigned int *>(fileCount);
            IFOR (i, 0, count)
            {
                animFile->read(offset, 8);
                animFile->read(headerlength, 4);
                animFile->read(compressedlength, 4);
                animFile->read(decompressedlength, 4);
                animFile->read(hash, 8);
                animFile->read(skip1, 4);
                animFile->read(skip2, 2);

                auto hashVal = *reinterpret_cast<uint64_t *>(hash);
                auto offsetVal = *reinterpret_cast<uint64_t *>(offset);
                if (offsetVal == 0)
                {
                    continue;
                }

                UOPAnimationData dataStruct;
                dataStruct.offset =
                    static_cast<uint>(offsetVal + *reinterpret_cast<unsigned int *>(headerlength));
                dataStruct.compressedLength = *reinterpret_cast<unsigned int *>(compressedlength);
                dataStruct.decompressedLength =
                    *reinterpret_cast<unsigned int *>(decompressedlength);

                dataStruct.fileStream = animFile;
                dataStruct.path = path;
                hashes[hashVal] = dataStruct;
            }

            animFile->seekg(*reinterpret_cast<uint64_t *>(nextBlock), std::ios::beg);
        } while (*reinterpret_cast<uint64_t *>(nextBlock) != 0);
    }

    int maxGroup = 0;

    IFOR (animId, 0, MAX_ANIMATIONS_DATA_INDEX_COUNT)
    {
        CIndexAnimation *indexAnim = &g_AnimationManager.m_DataIndex[animId];

        IFOR (grpId, 0, ANIMATION_GROUPS_COUNT)
        {
            CTextureAnimationGroup *group = &(*indexAnim).m_Groups[grpId];
            char hashString[100];
            sprintf_s(hashString, "build/animationlegacyframe/%06i/%02i.bin", animId, grpId);
            auto hash = g_Orion.CreateHash(hashString);
            if (hashes.find(hash) != hashes.end())
            {
                if (grpId > maxGroup)
                    maxGroup = (int)grpId;

                UOPAnimationData dataStruct = hashes.at(hash);
                indexAnim->IsUOP = true;
                group->m_UOPAnimData = dataStruct;
                IFOR (dirId, 0, 5)
                {
                    CTextureAnimationDirection *dir = &group->m_Direction[dirId];
                    dir->IsUOP = true;
                    dir->BaseAddress = 0;
                    dir->Address = 0;
                }
            }
        }
    }

    if (g_AnimationManager.AnimGroupCount < maxGroup)
        g_AnimationManager.AnimGroupCount = maxGroup;

    m_AutoResetEvent.Set();
}
//----------------------------------------------------------------------------------
bool CFileManager::FileExists(const os_path &filename)
{
    WISPFUN_DEBUG("");
    auto r = fs_path_exists(filename);
    LOG("FileExists: %s = %d\n", CStringFromPath(filename), r);
    return r;
}
//----------------------------------------------------------------------------------
char *CFileManager::ReadUOPDataFromFileStream(UOPAnimationData &animData)
{
    animData.fileStream->clear();
    animData.fileStream->seekg(animData.offset, std::ios::beg);
    //reading into buffer on the heap
    char *buf = new char[animData.compressedLength];
    animData.fileStream->read(buf, animData.compressedLength);
    return buf;
}
//----------------------------------------------------------------------------------
bool CFileManager::DecompressUOPFileData(
    UOPAnimationData &animData, UCHAR_LIST &decLayoutData, char *buf)
{
    uLongf cLen = animData.compressedLength;
    uLongf dLen = animData.decompressedLength;

    int z_err =
        uncompress(&decLayoutData[0], &dLen, reinterpret_cast<unsigned char const *>(buf), cLen);
    delete[] buf;

    if (z_err != Z_OK)
    {
        LOG("UOP anim decompression failed %d\n", z_err);
        LOG("Anim file: %s\n", CStringFromPath(animData.path));
        LOG("Anim offset: %d\n", animData.offset);
        return false;
    }
    return true;
}
//----------------------------------------------------------------------------------
bool CFileManager::LoadUOPFile(CUopMappedFile &file, const char *fileName)
{
    LOG("Loading UOP fileName: %s\n", fileName);
    if (!file.Load(g_App.UOFilesPath(fileName)))
        return false;

    uint formatID = file.ReadUInt32LE();

    if (formatID != 0x0050594D)
    {
        LOG("WARNING!!! UOP file '%s' formatID is %i!\n", fileName, formatID);
        return false;
    }

    uint formatVersion = file.ReadUInt32LE();

    if (formatVersion > 5)
        LOG("WARNING!!! UOP file '%s' version is %i!\n", fileName, formatVersion);

    file.Move(4); //Signature?
    uint64 next = file.ReadUInt64LE();

    file.Move(4); //Block capacity?
    uint filesCount = file.ReadUInt32LE();

    file.ResetPtr();
    file.Move((int)next);

    do
    {
        int count = file.ReadInt32LE();
        next = file.ReadInt64LE();

        IFOR (i, 0, count)
        {
            uint64 offset = file.ReadInt64LE();

            uint headerSize = file.ReadInt32LE();
            uint compressedSize = file.ReadInt32LE();
            uint decompressedSize = file.ReadInt32LE();

            uint64 hash = file.ReadInt64LE();
            uint unknown = file.ReadInt32LE();
            ushort flag = file.ReadInt16LE();

            if (!offset || !decompressedSize)
                continue;

            if (!flag)
                compressedSize = 0;

            CUopBlockHeader item;
            item.Offset = offset + headerSize;
            item.CompressedSize = compressedSize;
            item.DecompressedSize = decompressedSize;

            file.Add(hash, item);
        }

        file.ResetPtr();
        file.Move((int)next);
    } while (next != 0);

    file.ResetPtr();

    //if (string("MainMisc.uop") != fileName)
    //if (string("AnimationSequence.uop") != fileName)
    //if (string("tileart.uop") != fileName)
    return true;

    for (std::unordered_map<uint64, CUopBlockHeader>::iterator i = file.m_Map.begin();
         i != file.m_Map.end();
         ++i)
    {
        LOG("item dump start: %016llX, %i\n", i->first, i->second.CompressedSize);

        UCHAR_LIST data = file.GetData(i->second);

        if (data.empty())
            continue;

        WISP_DATASTREAM::CDataReader reader(&data[0], data.size());

        //LOG("%s\n", reader.ReadString(decompressedSize));

        LOG_DUMP(reader.Start, (int)reader.Size);

        LOG("item dump end:\n");
    }

    file.ResetPtr();

    return true;
}
//----------------------------------------------------------------------------------
bool CFileManager::TryOpenFileStream(std::fstream &fileStream, const os_path &filePath)
{
    LOG("Trying to open file stream for %s\n", CStringFromPath(filePath));
    if (!FileExists(filePath))
    {
        LOG("%s doesnt exist\n", CStringFromPath(filePath));
        return false;
    }
    fileStream.open(filePath, std::ios::binary | std::ios::in);
    LOG("Opened file stream for %s\n", CStringFromPath(filePath));
    return true;
}
//----------------------------------------------------------------------------------
bool CFileManager::IsMulFileOpen(int idx) const
{
    //we only have 5 anim mul files atm
    if (idx > 5)
        return false;
    return m_AnimMul[idx].is_open();
}
//----------------------------------------------------------------------------------
void CFileManager::ReadAnimMulDataFromFileStream(
    vector<char> &animData, CTextureAnimationDirection &direction)
{
    std::fstream *fileStream = &m_AnimMul[direction.FileIndex];
    fileStream->clear();
    fileStream->seekg(direction.Address, std::ios::beg);
    fileStream->read(static_cast<char *>(animData.data()), direction.Size);
}
//----------------------------------------------------------------------------------
