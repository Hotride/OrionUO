/***********************************************************************************
**
** FileManager.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "FileManager.h"
#include "../Wisp/WispApplication.h"
#include <thread>
#include <fstream>
#include "AnimationManager.h"

CFileManager g_FileManager;
//----------------------------------------------------------------------------------
CFileManager::CFileManager()
: m_UseVerdata(false), m_UseUOP(false), m_UnicodeFontsCount(0), m_AutoResetEvent(false)
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
	if (UseUOP)
	{
		if (!m_artLegacyMUL.Load(g_App.FilePath("artLegacyMUL.uop")))
		{
			if (!m_ArtIdx.Load(g_App.FilePath("artidx.mul")))
				return false;
			if (!m_ArtMul.Load(g_App.FilePath("art.mul")))
				return false;
		}
		if (!m_gumpartLegacyMUL.Load(g_App.FilePath("gumpartLegacyMUL.uop")))
		{
			if (!m_GumpIdx.Load(g_App.FilePath("gumpidx.mul")))
				return false;
			if (!m_GumpMul.Load(g_App.FilePath("gumpart.mul")))
				return false;
		}
		if (!m_soundLegacyMUL.Load(g_App.FilePath("soundLegacyMUL.uop")))
		{
			if (!m_SoundIdx.Load(g_App.FilePath("soundidx.mul")))
				return false;
			if (!m_SoundMul.Load(g_App.FilePath("sound.mul")))
				return false;
		}
	}
	else
	{
		if (!m_ArtIdx.Load(g_App.FilePath("artidx.mul")))
			return false;
		if (!m_ArtMul.Load(g_App.FilePath("art.mul")))
			return false;
		if (!m_GumpIdx.Load(g_App.FilePath("gumpidx.mul")))
			return false;
		if (!m_GumpMul.Load(g_App.FilePath("gumpart.mul")))
			return false;
		if (!m_SoundIdx.Load(g_App.FilePath("soundidx.mul")))
			return false;
		if (!m_SoundMul.Load(g_App.FilePath("sound.mul")))
			return false;
	}

	/* Эти файлы не используются самой последней версией клиента 7.0.52.2
	if (!m_tileart.Load(g_App.FilePath("tileart.uop")))
	return false;
	if (!m_string_dictionary.Load(g_App.FilePath("string_dictionary.uop")))
	return false;
	if (!m_MultiCollection.Load(g_App.FilePath("MultiCollection.uop")))
	return false;
	if (!m_AnimationSequence.Load(g_App.FilePath("AnimationSequence.uop")))
	return false;
	if (!m_MainMisc.Load(g_App.FilePath("MainMisc.uop")))
	return false;
	IFOR(i, 1, 5)
	{
	if (!m_AnimationFrame[i].Load(g_App.FilePath("AnimationFrame%i.uop", i)))
	return false;
	}*/

	if (!m_AnimIdx[0].Load(g_App.FilePath("anim.idx")))
		return false;
	if (!m_LightIdx.Load(g_App.FilePath("lightidx.mul")))
		return false;
	else if (!m_MultiIdx.Load(g_App.FilePath("multi.idx")))
		return false;
	else if (!m_SkillsIdx.Load(g_App.FilePath("skills.idx")))
		return false;
	else if (!m_MultiMap.Load(g_App.FilePath("multimap.rle")))
		return false;
	else if (!m_TextureIdx.Load(g_App.FilePath("texidx.mul")))
		return false;
	else if (!m_AnimMul[0].Load(g_App.FilePath("anim.mul")))
		return false;
	else if (!m_AnimdataMul.Load(g_App.FilePath("animdata.mul")))
		return false;
	else if (!m_HuesMul.Load(g_App.FilePath("hues.mul")))
		return false;
	else if (!m_FontsMul.Load(g_App.FilePath("fonts.mul")))
		return false;
	else if (!m_LightMul.Load(g_App.FilePath("light.mul")))
		return false;
	else if (!m_MultiMul.Load(g_App.FilePath("multi.mul")))
		return false;
	else if (!m_PaletteMul.Load(g_App.FilePath("palette.mul")))
		return false;
	else if (!m_RadarcolMul.Load(g_App.FilePath("radarcol.mul")))
		return false;
	else if (!m_SkillsMul.Load(g_App.FilePath("skills.mul")))
		return false;
	else if (!m_TextureMul.Load(g_App.FilePath("texmaps.mul")))
		return false;
	else if (!m_TiledataMul.Load(g_App.FilePath("tiledata.mul")))
		return false;
	m_SpeechMul.Load(g_App.FilePath("speech.mul"));
	//m_LangcodeIff.Load(g_App.FilePath("Langcode.iff"));

	IFOR(i, 0, 6)
	{
		if (i > 0)
		{
			m_AnimIdx[i].Load(g_App.FilePath("anim%i.idx", i));
			m_AnimMul[i].Load(g_App.FilePath("anim%i.mul", i));
		}

		if (UseUOP)
			m_MapUOP[i].Load(g_App.FilePath("map%iLegacyMUL.uop", i));
		else
			m_MapMul[i].Load(g_App.FilePath("map%i.mul", i));
		/*else
		{
		/if (i == 0 || i == 1 || i == 2 || i == 5)
		{
		if (!m_MapXUOP[i].Load(g_App.FilePath("map%ixLegacyMUL.uop", i)))
		return false;
		}
		}*/

		m_StaticIdx[i].Load(g_App.FilePath("staidx%i.mul", i));
		m_StaticMul[i].Load(g_App.FilePath("statics%i.mul", i));
		m_FacetMul[i].Load(g_App.FilePath("facet0%i.mul", i));

		m_MapDifl[i].Load(g_App.FilePath("mapdifl%i.mul", i));
		m_MapDif[i].Load(g_App.FilePath("mapdif%i.mul", i));

		m_StaDifl[i].Load(g_App.FilePath("stadifl%i.mul", i));
		m_StaDifi[i].Load(g_App.FilePath("stadifi%i.mul", i));
		m_StaDif[i].Load(g_App.FilePath("stadif%i.mul", i));
	}

	IFOR(i, 0, 20)
	{
		string s;

		if (i)
			s = g_App.FilePath("unifont%i.mul", i);
		else
			s = g_App.FilePath("unifont.mul");

		if (!m_UnifontMul[i].Load(s) && i > 1)
			break;

		m_UnicodeFontsCount++;
	}

	if (m_UseVerdata && !m_VerdataMul.Load(g_App.FilePath("verdata.mul")))
		m_UseVerdata = false;

	return true;
}
//----------------------------------------------------------------------------------
void CFileManager::Unload()
{
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

	IFOR(i, 1, 5)
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
	IFOR(i, 1, 5)
	{
		char magic[4];
		char version[4];
		char signature[4];
		char nextBlock[8];

		std::fstream *animFile = new std::fstream();
		string *path = new string(g_App.FilePath("AnimationFrame%i.uop", i));
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

				auto offsetVal = *reinterpret_cast<unsigned long long*>(offset);
				if (offsetVal == 0)
				{
					continue;
				}

				UOPAnimationData dataStruct;
				dataStruct.offset = offsetVal + *reinterpret_cast<unsigned int*>(headerlength);
				dataStruct.length = *reinterpret_cast<unsigned int*>(compressedlength);

				dataStruct.fileStream = animFile;
				dataStruct.path = path;
				g_AnimationManager.AddUopAnimData(*reinterpret_cast<unsigned long long*>(hash), dataStruct);
			}

			animFile->seekg(*reinterpret_cast<unsigned long long*>(nextBlock), 0);
		} while (*reinterpret_cast<unsigned long long*>(nextBlock) != 0);

		animFile->close();
	}
	m_AutoResetEvent.Set();
}