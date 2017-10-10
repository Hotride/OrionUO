/***********************************************************************************
**
** SpeechManager.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CSpeechManager g_SpeechManager;
//----------------------------------------------------------------------------------
//-----------------------------------CSpeechManager---------------------------------
//----------------------------------------------------------------------------------
CSpeechManager::CSpeechManager()
{
}
//----------------------------------------------------------------------------------
CSpeechManager::~CSpeechManager()
{
	WISPFUN_DEBUG("c157_f1");
	m_SpeechEntries.clear();
	m_LangCodes.clear();
}
//----------------------------------------------------------------------------------
/*!
Загрузка данных из Speech.mul
@return true при успешной загрузке
*/
bool CSpeechManager::LoadSpeech()
{
	WISPFUN_DEBUG("c157_f2");
	LoadLangCodes();

	string lang(255, 0);

	if (!GetProfileStringA("intl", "sLanguage", "default", &lang[0], 4))
		g_Language = m_LangCodes[0].Abbreviature;
	else
		g_Language = lang.c_str();

	IFOR(i, 0, (int)m_LangCodes.size())
	{
		if (m_LangCodes[i].Abbreviature == g_Language)
		{
			m_CurrentLanguage = &m_LangCodes[i];
			break;
		}
	}

	if (m_CurrentLanguage == NULL)
	{
		m_CurrentLanguage = &m_LangCodes[0];
		g_Language = m_LangCodes[0].Abbreviature;
	}

	LOG("Selected language: %s (%s)\n", g_Language.c_str(), lang.c_str());

	WISP_DATASTREAM::CDataReader reader(g_FileManager.m_SpeechMul.Start, g_FileManager.m_SpeechMul.Size);

	while (!reader.IsEOF())
	{
		CSpeechItem item;
		item.Code = reader.ReadUInt16BE();
		int len = reader.ReadUInt16BE();

		if (!len)
			continue;

		wstring str = DecodeUTF8(reader.ReadString(len)).c_str();
		const WCHAR *data = str.c_str();

		
		DFOR(i, str.length() - 1, 1)
		{
			if (data[i])
			{
				if (data[i] == L'*')
				{
					item.CheckEnd = true;
					str.resize(i);
				}

				break;
			}
		}

		if (str.length() && *str.c_str() == L'*')
		{
			item.CheckStart = true;
			str.erase(str.begin());
		}

		item.Data = str;
		m_SpeechEntries.push_back(item);

		//LOG(L"[0x%04X]=(len=%i, cs=%i, ce=%i) %s\n", item.Code, len, item.CheckStart, item.CheckEnd, str.c_str());
	}

	m_Loaded = true;
	return true;
}
//----------------------------------------------------------------------------------
/*!
Загрузка данных из Langcode.iff
@return true при успешной загрузке
*/
bool CSpeechManager::LoadLangCodes()
{
	WISPFUN_DEBUG("c157_f3");

	m_LangCodes.push_back(CLangCode("enu", 101, "English", "United States"));

	WISP_FILE::CMappedFile &file = g_FileManager.m_LangcodeIff;

	//скипаем заголовок файла
	file.ReadString(36);

	while (!file.IsEOF())
	{
		CLangCode langCodeData;

		file.Move(4);

		uint entryLen = file.ReadUInt32BE();
		langCodeData.Abbreviature = file.ReadString(0); 
		langCodeData.Code = file.ReadUInt32LE();
		langCodeData.Language = file.ReadString(0);
		langCodeData.Country = file.ReadString(0);

		//длинна LangName и LangCountry + null terminator всегда являются четным количеством в файле.
		if ((langCodeData.Language.length() + langCodeData.Country.length() + 2) % 2)
		{
			int nullTerminator = file.ReadUInt8();
			
			if (nullTerminator != 0) {
				throw "speechManager @ 138, invalid null terminator in langcodes.iff";
			}
		}

		m_LangCodes.push_back(langCodeData);
		//LOG("[0x%04X]: %s\n", langCodeData.Code, langCodeData.Abbreviature.c_str());
	}

	//if (m_LangCodes.size() != 135)
	//	return false;

	return true;
}
//----------------------------------------------------------------------------------
void CSpeechManager::GetKeywords(const wchar_t *text, UINT_LIST &codes)
{
	WISPFUN_DEBUG("c157_f4");
	if (!m_Loaded || g_PacketManager.ClientVersion < CV_305D) //Но по факту с 2.0.7 версии клиента
		return;

	int size = m_SpeechEntries.size();
	wstring input = ToLowerW(text);
	//to lower, case insensitive approach.

	IFOR(i, 0, size) 
	{
		CSpeechItem entry = m_SpeechEntries[i];
		wstring data = entry.Data;

		if (data.length() > input.length() || data.length() == 0)
			continue;

		if (!entry.CheckStart)
		{
			wstring start = input.substr(0, data.length());
			int hits = start.find(data);

			if (hits < 0)
				continue;
		}

		if (!entry.CheckEnd)
		{
			wstring end = input.substr(input.length() - data.length());
			int hits = end.find(data);

			if (hits < 0)
				continue;
		}

		int hits = input.find(data);

		if (hits > -1)
			codes.push_back(entry.Code);
	}
}
//---------------------------------------------------------------------------
