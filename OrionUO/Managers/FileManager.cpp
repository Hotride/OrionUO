﻿/***********************************************************************************
**
** FileManager.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
CFileManager g_FileManager;
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
	//Try to use map uop files first, if we can, we will use them.
	if (g_PacketManager.ClientVersion < CV_7000)
	{
		if (!m_ArtIdx.Load(g_Orion.ClientFilePath("artidx.mul"))) return false;
		if (!m_ArtMul.Load(g_Orion.ClientFilePath("art.mul"))) return false;
		if (!m_GumpIdx.Load(g_Orion.ClientFilePath("gumpidx.mul"))) return false;
		if (!m_GumpMul.Load(g_Orion.ClientFilePath("gumpart.mul"))) return false;
		if (!m_SoundIdx.Load(g_Orion.ClientFilePath("soundidx.mul"))) return false;
		if (!m_SoundMul.Load(g_Orion.ClientFilePath("sound.mul"))) return false;
	}
	else
	{
		if (!m_artLegacyMUL.Load(g_Orion.ClientFilePath("artLegacyMUL.uop")))
		{
			if (!m_ArtIdx.Load(g_Orion.ClientFilePath("artidx.mul"))) return false;
			if (!m_ArtMul.Load(g_Orion.ClientFilePath("art.mul"))) return false;
		}

		if (!m_gumpartLegacyMUL.Load(g_Orion.ClientFilePath("gumpartLegacyMUL.uop")))
		{
			if (!m_GumpIdx.Load(g_Orion.ClientFilePath("gumpidx.mul"))) return false;
			if (!m_GumpMul.Load(g_Orion.ClientFilePath("gumpart.mul"))) return false;
			UseUOPGumps = false;
		}
		else
			UseUOPGumps = true;
		if (!m_soundLegacyMUL.Load(g_Orion.ClientFilePath("soundLegacyMUL.uop")))
		{
			if (!m_SoundIdx.Load(g_Orion.ClientFilePath("soundidx.mul"))) return false;
			if (!m_SoundMul.Load(g_Orion.ClientFilePath("sound.mul"))) return false;
		}
	}


	/* Эти файлы не используются самой последней версией клиента 7.0.52.2
	if (!m_tileart.Load(g_Orion.ClientFilePath("tileart.uop")))
	return false;
	if (!m_string_dictionary.Load(g_Orion.ClientFilePath("string_dictionary.uop")))
	return false;
	if (!m_MultiCollection.Load(g_Orion.ClientFilePath("MultiCollection.uop")))
	return false;
	if (!m_AnimationSequence.Load(g_Orion.ClientFilePathh("AnimationSequence.uop")))
	return false;
	if (!m_MainMisc.Load(g_Orion.ClientFilePath("MainMisc.uop")))
	return false;
	IFOR(i, 1, 5)
	{
	if (!m_AnimationFrame[i].Load(g_Orion.ClientFilePath("AnimationFrame%i.uop", i)))
	return false;
	}*/

	if (!m_AnimIdx[0].Load(g_Orion.ClientFilePath("anim.idx")))
		return false;
	if (!m_LightIdx.Load(g_Orion.ClientFilePath("lightidx.mul")))
		return false;
	else if (!m_MultiIdx.Load(g_Orion.ClientFilePath("multi.idx")))
		return false;
	else if (!m_SkillsIdx.Load(g_Orion.ClientFilePath("skills.idx")))
		return false;
	else if (!m_MultiMap.Load(g_Orion.ClientFilePath("multimap.rle")))
		return false;
	else if (!m_TextureIdx.Load(g_Orion.ClientFilePath("texidx.mul")))
		return false;
	else if (!m_AnimMul[0].Load(g_Orion.ClientFilePath("anim.mul")))
		return false;
	else if (!m_AnimdataMul.Load(g_Orion.ClientFilePath("animdata.mul")))
		return false;
	else if (!m_HuesMul.Load(g_Orion.ClientFilePath("hues.mul")))
		return false;
	else if (!m_FontsMul.Load(g_Orion.ClientFilePath("fonts.mul")))
		return false;
	else if (!m_LightMul.Load(g_Orion.ClientFilePath("light.mul")))
		return false;
	else if (!m_MultiMul.Load(g_Orion.ClientFilePath("multi.mul")))
		return false;
	else if (!m_PaletteMul.Load(g_Orion.ClientFilePath("palette.mul")))
		return false;
	else if (!m_RadarcolMul.Load(g_Orion.ClientFilePath("radarcol.mul")))
		return false;
	else if (!m_SkillsMul.Load(g_Orion.ClientFilePath("skills.mul")))
		return false;
	else if (!m_TextureMul.Load(g_Orion.ClientFilePath("texmaps.mul")))
		return false;
	else if (!m_TiledataMul.Load(g_Orion.ClientFilePath("tiledata.mul")))
		return false;
	m_SpeechMul.Load(g_Orion.ClientFilePath("speech.mul"));
	m_LangcodeIff.Load(g_Orion.ClientFilePath("Langcode.iff"));

	IFOR(i, 0, 6)
	{
		if (i > 0)
		{
			m_AnimIdx[i].Load(g_Orion.ClientFilePath("anim%i.idx", i));
			m_AnimMul[i].Load(g_Orion.ClientFilePath("anim%i.mul", i));
		}
		if (g_PacketManager.ClientVersion < CV_7000)
		{
			m_MapMul[i].Load(g_Orion.ClientFilePath("map%i.mul", i));
		}
		else
		{
			if (m_MapUOP[i].Load(g_Orion.ClientFilePath("map%iLegacyMUL.uop", i)))
				UseUOPMap = true;
			else
				m_MapMul[i].Load(g_Orion.ClientFilePath("map%i.mul", i));
		}


		m_StaticIdx[i].Load(g_Orion.ClientFilePath("staidx%i.mul", i));
		m_StaticMul[i].Load(g_Orion.ClientFilePath("statics%i.mul", i));
		m_FacetMul[i].Load(g_Orion.ClientFilePath("facet0%i.mul", i));

		m_MapDifl[i].Load(g_Orion.ClientFilePath("mapdifl%i.mul", i));
		m_MapDif[i].Load(g_Orion.ClientFilePath("mapdif%i.mul", i));

		m_StaDifl[i].Load(g_Orion.ClientFilePath("stadifl%i.mul", i));
		m_StaDifi[i].Load(g_Orion.ClientFilePath("stadifi%i.mul", i));
		m_StaDif[i].Load(g_Orion.ClientFilePath("stadif%i.mul", i));
	}

	IFOR(i, 0, 20)
	{
		string s;
		
		if (i)
			s = g_Orion.ClientFilePath("unifont%i.mul", i);
		else
			s = g_Orion.ClientFilePath("unifont.mul");

		if (!m_UnifontMul[i].Load(s) && i > 1)
			break;

		m_UnicodeFontsCount++;
	}

	if (m_UseVerdata && !m_VerdataMul.Load(g_Orion.ClientFilePath("verdata.mul")))
		m_UseVerdata = false;

	return true;
}
//----------------------------------------------------------------------------------
void CFileManager::Unload()
{
	WISPFUN_DEBUG("c142_f2");
	m_ArtIdx.Unload();
	m_GumpIdx.Unload();
	m_SoundIdx.Unload();
	m_ArtMul.Unload();
	m_GumpMul.Unload();
	m_SoundMul.Unload();
	m_artLegacyMUL.Unload();
	m_gumpartLegacyMUL.Unload();
	m_soundLegacyMUL.Unload();
	m_tileart.Unload();
	m_string_dictionary.Unload();
	m_MultiCollection.Unload();
	m_AnimationSequence.Unload();
	m_MainMisc.Unload();

	IFOR(i, 0, 4)
	{
		m_AnimationFrame[i].Unload();
	}
	m_LightIdx.Unload();
	m_MultiIdx.Unload();
	m_SkillsIdx.Unload();

	m_MultiMap.Unload();
	m_TextureIdx.Unload();
	m_SpeechMul.Unload();
	m_AnimdataMul.Unload();

	m_HuesMul.Unload();
	m_FontsMul.Unload();

	m_LightMul.Unload();
	m_MultiMul.Unload();
	m_PaletteMul.Unload();
	m_RadarcolMul.Unload();
	m_SkillsMul.Unload();

	m_TextureMul.Unload();
	m_TiledataMul.Unload();

	m_LangcodeIff.Unload();

	IFOR(i, 0, 6)
	{
		m_AnimIdx[i].Unload();
		m_AnimMul[i].Unload();
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

	IFOR(i, 0, 20)
		m_UnifontMul[i].Unload();

	m_VerdataMul.Unload();
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
	std::unordered_map<unsigned long long, UOPAnimationData> hashes;
	IFOR(i, 1, 5)
	{
		char magic[4];
		char version[4];
		char signature[4];
		char nextBlock[8];

		std::fstream *animFile = new std::fstream();
		string *path = new string(g_Orion.ClientFilePath("AnimationFrame%i.uop", i));
		if (!FileExists(*path))
		{
			continue;
		}
		animFile->open(*path, std::ios::binary | std::ios::in);

		if (!animFile) continue;


		animFile->read(magic, 4);
		animFile->read(version, 4);
		animFile->read(signature, 4);
		animFile->read(nextBlock, 8);

		animFile->seekg(*reinterpret_cast<unsigned long long*>(nextBlock), 0);

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
			int count = *reinterpret_cast<unsigned int*>(fileCount);
			IFOR(i, 0, count)
			{
				animFile->read(offset, 8);
				animFile->read(headerlength, 4);
				animFile->read(compressedlength, 4);
				animFile->read(decompressedlength, 4);
				animFile->read(hash, 8);
				animFile->read(skip1, 4);
				animFile->read(skip2, 2);

				auto hashVal = *reinterpret_cast<unsigned long long*>(hash);
				auto offsetVal = *reinterpret_cast<unsigned long long*>(offset);
				if (offsetVal == 0)
				{
					continue;
				}

				UOPAnimationData dataStruct;
				dataStruct.offset = static_cast<uint>(offsetVal + *reinterpret_cast<unsigned int*>(headerlength));
				dataStruct.compressedLength = *reinterpret_cast<unsigned int*>(compressedlength);
				dataStruct.decompressedLength = *reinterpret_cast<unsigned int*>(decompressedlength);

				dataStruct.fileStream = animFile;
				dataStruct.path = path;
				hashes[hashVal] = dataStruct;
			}

			animFile->seekg(*reinterpret_cast<unsigned long long*>(nextBlock), 0);
		} while (*reinterpret_cast<unsigned long long*>(nextBlock) != 0);
	}

	int maxGroup = 0;

	IFOR(animId, 0, MAX_ANIMATIONS_DATA_INDEX_COUNT)
	{
		CIndexAnimation *indexAnim = &g_AnimationManager.m_DataIndex[animId];

		IFOR(grpId, 0, ANIMATION_GROUPS_COUNT)
		{
			CTextureAnimationGroup *group = &(*indexAnim).m_Groups[grpId];
			char hashString[100];
			sprintf_s(hashString, "build/animationlegacyframe/%06i/%02i.bin", animId, grpId);
			auto hash = g_Orion.CreateHash(hashString);
			if (hashes.find(hash) != hashes.end())
			{
				if (grpId > maxGroup)
					maxGroup = grpId;

				UOPAnimationData dataStruct = hashes.at(hash);
				indexAnim->IsUOP = true;
				group->IsUOP = true;
				group->m_UOPAnimData = dataStruct;
				IFOR(dirId, 0, 5)
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
bool CFileManager::FileExists(const std::string& filename)
{
	struct stat buf;
	if (stat(filename.c_str(), &buf) != -1)
	{
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------------
char *CFileManager::ReadUOPDataFromFileStream(UOPAnimationData &animData)
{
	animData.fileStream->seekg(animData.offset, 0);
	//reading into buffer on the heap
	char *buf = new char[animData.compressedLength];
	animData.fileStream->read(buf, animData.compressedLength);
	return buf;
}
//----------------------------------------------------------------------------------
bool CFileManager::DecompressUOPFileData(UOPAnimationData &animData, UCHAR_LIST &decLayoutData, char *buf)
{
	uLongf cLen = animData.compressedLength;
	uLongf dLen = animData.decompressedLength;

	int z_err = uncompress(&decLayoutData[0], &dLen, reinterpret_cast<unsigned char const*>(buf), cLen);
	delete buf;

	if (z_err != Z_OK)
	{
		LOG("UOP anim decompression failed %d\n", z_err);
		LOG("Anim file: %s\n", *animData.path);
		LOG("Anim offset: %d\n", animData.offset);
		return false;
	}
	return true;
}