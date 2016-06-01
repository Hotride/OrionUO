/****************************************************************************
**
** SpeechManager.cpp
**
** Copyright (C) May 2016 Aimed
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
*****************************************************************************
*/
//---------------------------------------------------------------------------
#include "stdafx.h"

TSpeechManager *SpeechManager = NULL;
//---------------------------------------------------------------------------
//--------------------------------TLangCode--------------------------------
//---------------------------------------------------------------------------
TLangCode::TLangCode()
: m_Code(0), m_LangString(), m_Unknown(0), m_LangName(), m_LangCountry()
{
}
//---------------------------------------------------------------------------
TLangCode::~TLangCode()
{
}
//---------------------------------------------------------------------------
//--------------------------------TSpeechItem--------------------------------
//---------------------------------------------------------------------------
TSpeechItem::TSpeechItem()
: m_Code(0), m_Data(L""), m_CheckStart(false), m_CheckEnd(false)
{
}
//---------------------------------------------------------------------------
TSpeechItem::~TSpeechItem()
{
}
//---------------------------------------------------------------------------
//------------------------------TSpeechManager-------------------------------
//---------------------------------------------------------------------------
TSpeechManager::TSpeechManager()
{
}
//---------------------------------------------------------------------------
TSpeechManager::~TSpeechManager()
{
}
//---------------------------------------------------------------------------
/*!
«агрузка данных из Speech.mul
@return true при успешной загрузке
*/
bool TSpeechManager::LoadSpeech()
{
	if (!LoadLangCodes())
		return false;

	vector<TSpeechItem> items;
	TMappedHeader &file = FileManager.SpeechMul;

	while (!file.IsEof())
	{
		TSpeechItem item;
		item.Code = file.ReadWordBE();
		WORD len = file.ReadWordBE();
		wstring str = file.ReadUtf8String(len);

		const WCHAR *data = str.c_str();

		
		DFOR(i, str.length(), 1)
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
		items.push_back(item);

		//TPRINT("[0x%04X]=(len=%i, cs=%i, ce=%i) %s\n", item.Code, len, item.CheckStart, item.CheckEnd, ToString(str).c_str());
	}

	return true;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
/*!
«агрузка данных из Langcode.iff
@return true при успешной загрузке
*/
bool TSpeechManager::LoadLangCodes()
{
	vector<TLangCode> langCodes;
	TMappedHeader &file = FileManager.LangcodeIff;

	//скипаем заголовок файла
	file.ReadString(36);

	while (!file.IsEof())
	{
		
		TLangCode item;

		file.ReadString(4); // read "CODE" string

		DWORD entryLen = file.ReadDWordBE();
		item.LangString = file.ReadString(0);
		item.Unknown = file.ReadDWord(); // LangCode 99.99% ( абревиатура локали )
		item.LangName = file.ReadString(0);
		item.LangCountry = file.ReadString(0);

		//длинна LangName и LangCountry + null terminator всегда €вл€ютс€ четным количеством в файле.
		if ((item.LangName.length() + item.LangCountry.length() + 2) % 2) {
			int x = file.ReadByte();
			
			if (x != 0) {
				throw std::invalid_argument("speechManager @ 138, x != 0 with modulus -.-' cry cry cry");
			}
		}
				
		langCodes.push_back(item);
		//TPRINT("[0x%04X]=(len=%i, cs=%i, ce=%i) %s\n", item.Code, len, item.CheckStart, item.CheckEnd, ToString(str).c_str());
	}

	return true;
}
//---------------------------------------------------------------------------
