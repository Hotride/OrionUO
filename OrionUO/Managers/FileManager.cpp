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

CFileManager g_FileManager;
//----------------------------------------------------------------------------------
CFileManager::CFileManager()
: m_UseVerdata(false), m_UseUOP(false), m_UnicodeFontsCount(0)
{
}
//----------------------------------------------------------------------------------
CFileManager::~CFileManager()
{
}
//----------------------------------------------------------------------------------
bool CFileManager::Load()
{
	if (!g_FileManager.UseUOP)
	{
		 if (!m_ArtIdx.Load(g_App.FilePath("artidx.mul")))
			return false;
		else if (!m_GumpIdx.Load(g_App.FilePath("gumpidx.mul")))
			return false;
		else if (!m_SoundIdx.Load(g_App.FilePath("soundidx.mul")))
			return false;
		else if (!m_ArtMul.Load(g_App.FilePath("art.mul")))
			return false;
		else if (!m_GumpMul.Load(g_App.FilePath("gumpart.mul")))
			return false;
		else if (!m_SoundMul.Load(g_App.FilePath("sound.mul")))
			return false;
	}
	else
	{
		if (!m_artLegacyMUL.Load(g_App.FilePath("artLegacyMUL.uop")))
			return false;
		if (!m_gumpartLegacyMUL.Load(g_App.FilePath("gumpartLegacyMUL.uop")))
			return false;
		if (!m_soundLegacyMUL.Load(g_App.FilePath("soundLegacyMUL.uop")))
			return false;
		if (!m_tileart.Load(g_App.FilePath("tileart.uop")))
			return false;
		if (!m_string_dictionary.Load(g_App.FilePath("string_dictionary.uop")))
			return false;
		//По сути пустой файл?
		//if (!m_MultiCollection.Load(g_App.FilePath("MultiCollection.uop")))
		//	return false;
		if (!m_AnimationSequence.Load(g_App.FilePath("AnimationSequence.uop")))
			return false;
		if (!m_MainMisc.Load(g_App.FilePath("MainMisc.uop")))
			return false;
		/*IFOR(i, 1, 5)
		{
			if (!m_AnimationFrame[i].Load(g_App.FilePath("AnimationFrame%i.uop", i)))
				return false;
		}*/
	}
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
	else if (!m_SpeechMul.Load(g_App.FilePath("speech.mul")))
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

	m_LangcodeIff.Load(g_App.FilePath("Langcode.iff"));

	IFOR(i, 0, 6)
	{
		if (g_FileManager.UseUOP && i > 1 || !g_FileManager.UseUOP && i > 0)
		{
			if (!m_AnimIdx[i].Load(g_App.FilePath("anim%i.idx", i)))
			if (!m_AnimMul[i].Load(g_App.FilePath("anim%i.mul", i)))
		}

		if (g_FileManager.UseUOP)
		{
			if (!m_MapUOP[i].Load(g_App.FilePath("map%iLegacyMUL.uop", i)))
				return false;
			if (i == 0 || i == 1 || i == 2 || i == 5)
			{
				if (!m_MapXUOP[i].Load(g_App.FilePath("map%ixLegacyMUL.uop", i)))
					return false;
			}
		}
		else
		{
			m_MapMul[i].Load(g_App.FilePath("map%i.mul", i));
		}

		m_StaticIdx[i].Load(g_App.FilePath("staidx%i.mul", i));
		m_StaticMul[i].Load(g_App.FilePath("statics%i.mul", i));
		m_FacetMul[i].Load(g_App.FilePath("facet0%i.mul", i));
	}

	IFOR(i, 0, 20)
	{
		string s;
		
		if (i)
			s = g_App.FilePath("unifont%i.mul", i);
		else
			s = g_App.FilePath("unifont.mul");

		if (!m_UnifontMul[i].Load(s))
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

	if (!g_FileManager.UseUOP)
	{
		m_ArtIdx.Unload();
		m_GumpIdx.Unload();
		m_SoundIdx.Unload();
		m_ArtMul.Unload();
		m_GumpMul.Unload();
		m_SoundMul.Unload();
	}
	else
	{
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
		if (g_FileManager.UseUOP)
		{
			m_MapUOP[i].Unload();
			m_MapXUOP[i].Unload();

		}
		else
			m_MapMul[i].Unload();
		m_StaticIdx[i].Unload();
		m_StaticMul[i].Unload();
		m_FacetMul[i].Unload();
	}

	IFOR(i, 0, 20)
		m_UnifontMul[i].Unload();

	m_VerdataMul.Unload();
}
//----------------------------------------------------------------------------------