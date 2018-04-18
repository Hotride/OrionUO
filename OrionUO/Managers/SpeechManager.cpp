// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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
//------------------------------------CSpeechItem-----------------------------------
//----------------------------------------------------------------------------------
CSpeechItem::CSpeechItem(ushort code, const wstring &data)
: Code(code), Data(data)
{
	if (Data.length() && Data[Data.length() - 1] == L'*')
	{
		CheckEnd = true;
		Data.resize(Data.length() - 1);
	}

	if (Data.length() && Data[0] == L'*')
	{
		CheckStart = true;
		Data.erase(Data.begin());
	}

	//LOG(L"[0x%04X]=(cs=%i, ce=%i) %s\n", m_Code, m_CheckStart, m_CheckEnd, m_Data.c_str());
}
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
			CurrentLanguage = &m_LangCodes[i];
			break;
		}
	}

	if (CurrentLanguage == NULL)
	{
		CurrentLanguage = &m_LangCodes[0];
		g_Language = m_LangCodes[0].Abbreviature;
	}

	LOG("Selected language: %s (%s)\n", g_Language.c_str(), lang.c_str());

	WISP_DATASTREAM::CDataReader reader;
	UCHAR_LIST tempData;
	bool isUOP = false;
	
	if (g_FileManager.m_MainMisc.Start != NULL)
	{
		CUopBlockHeader *block = g_FileManager.m_MainMisc.GetBlock(0x0891F809004D8081);

		if (block != NULL)
		{
			tempData = g_FileManager.m_MainMisc.GetData(*block);
			reader.SetData(&tempData[0], tempData.size());
			isUOP = true;
		}
	}

	if (reader.Start == NULL)
		reader.SetData(g_FileManager.m_SpeechMul.Start, g_FileManager.m_SpeechMul.Size);

	if (isUOP)
	{
		LOG("Loading speech from UOP\n");
		reader.Move(2);
		wstring mainData = reader.ReadWString(reader.Size - 2, false).c_str();
		vector<wstring> list;
		wstring temp;

		for (const wchar_t &c : mainData)
		{
			if (c == 0x000D || c == 0x000A)
			{
				if (temp.length())
				{
					list.push_back(temp);
					temp = L"";
				}
			}
			else
				temp.push_back(c);
		}

		if (temp.length())
		{
			list.push_back(temp);
			temp = L"";
		}

		for (const wstring &line : list)
		{
			ushort code = 0xFFFF;
			temp = L"";

			for (const wchar_t c : line)
			{
				if (c == 0x0009)
				{
					if (temp.length())
					{
						code = _wtoi(temp.c_str());
						temp = L"";
					}
				}
				else
					temp.push_back(c);
			}

			if (temp.length() && code != 0xFFFF)
				m_SpeechEntries.push_back(CSpeechItem(code, temp.c_str()));
		}
	}
	else
	{
		LOG("Loading speech from MUL\n");
		while (!reader.IsEOF())
		{
			ushort code = reader.ReadUInt16BE();
			int len = reader.ReadUInt16BE();

			if (!len)
				continue;

			wstring str = DecodeUTF8(reader.ReadString(len)).c_str();

			m_SpeechEntries.push_back(CSpeechItem(code, str));
		}
	}

	LOG(L"m_SpeechEntries.size()=%i\n", m_SpeechEntries.size());

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
	if (!m_Loaded || g_PacketManager.GetClientVersion() < CV_305D) //Но по факту с 2.0.7 версии клиента
		return;

	size_t size = m_SpeechEntries.size();
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
			size_t hits = start.find(data);

			if (hits == wstring::npos)
				continue;
		}

		if (!entry.CheckEnd)
		{
			wstring end = input.substr(input.length() - data.length());
			size_t hits = end.find(data);

			if (hits == wstring::npos)
				continue;
		}

		size_t hits = input.find(data);

		if (hits != wstring::npos)
			codes.push_back(entry.Code);
	}
}
//---------------------------------------------------------------------------
