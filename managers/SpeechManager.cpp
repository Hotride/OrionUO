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
Загрузка
@return true при успешной загрузке
*/
bool TSpeechManager::LoadSpeech()
{
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

		//TPRINT("[0x%04X]=(len=%i, cs=%i, ce=%i) %s\n", item.Code, len, item.CheckStart, item.CheckEnd, ToString(str).c_str());
	}

	return true;
}
//---------------------------------------------------------------------------
