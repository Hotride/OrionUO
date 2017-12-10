// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** IntlocManager.cpp
**
** Copyright (C) October 2017 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CIntlocManager g_IntlocManager;
//----------------------------------------------------------------------------------
//--------------------------------------CIntloc-------------------------------------
//----------------------------------------------------------------------------------
CIntloc::CIntloc(const int &fileIndex, const string &lang)
: CBaseQueueItem()
{
	WISPFUN_DEBUG("c135_f1");
	m_Loaded = false;
	m_Language = lang;
	m_FileIndex = fileIndex;

	if (m_Language.length())
	{
		if (m_File.Load(g_App.UOFilesPath("intloc%02i.%s", fileIndex, lang.c_str())))
		{
			while (!m_File.IsEOF())
			{
				uint code = m_File.ReadUInt32BE();
				
				if (code == 'TEXT')
				{
					int len = m_File.ReadInt32BE();

					puchar end = m_File.Ptr + len;

					while (m_File.Ptr < end && !m_File.IsEOF())
					{
						m_Strings.push_back(DecodeUTF8(m_File.ReadString().c_str()));
					}
				}
				else if(code == 'FORM')
					m_File.Move(4);
				else if (code == 'INFO')
				{
					int len = m_File.ReadInt32BE();
					m_File.Move(len + 1);
				}
				else if (code == 'DATA' || code == 'LANG')
				{
				}
				else
					break;
			}

			m_Loaded = (m_Strings.size() != 0);
		}
	}
}
//----------------------------------------------------------------------------------
CIntloc::~CIntloc()
{
	WISPFUN_DEBUG("c135_f2");
	m_File.Unload();

	m_Strings.clear();
}
//----------------------------------------------------------------------------------
/*!
Получить ASCII строку по id (и загрузить при необходимости)
@param [__in] id Индекс клилока
@param [__in] result Стандартное сообщение, если клилок не был найден
@return Полученный результат, замена или сообщение с ошибкой
*/
wstring CIntloc::Get(const uint &id, const bool &toCamelCase)
{
	WISPFUN_DEBUG("c135_f4");
	if (id < m_Strings.size())
	{
		if (toCamelCase)
			return ToCamelCaseW(m_Strings[id]);

		return m_Strings[id];
	}

	return L"";
}
//----------------------------------------------------------------------------------
//-----------------------------------CIntlocManager---------------------------------
//----------------------------------------------------------------------------------
CIntlocManager::CIntlocManager()
: CBaseQueue()
{
}
//----------------------------------------------------------------------------------
CIntlocManager::~CIntlocManager()
{
}
//----------------------------------------------------------------------------------
CIntloc *CIntlocManager::Intloc(const int &fileIndex, const string &lang)
{
	WISPFUN_DEBUG("c136_f1");
	QFOR(obj, m_Items, CIntloc*)
	{
		if (obj->Language == lang && obj->FileIndex == fileIndex)
		{
			if (!obj->Loaded)
				return NULL;

			return obj;
		}
	}

	CIntloc *obj = (CIntloc*)Add(new CIntloc(fileIndex, lang));

	if (obj->Loaded)
		return obj;

	QFOR(obj, m_Items, CIntloc*)
	{
		if (obj->Language == "enu" && obj->FileIndex == fileIndex)
		{
			if (obj->Loaded)
				return obj;

			break;
		}
	}

	return NULL;
}
//----------------------------------------------------------------------------------
wstring CIntlocManager::Intloc(const string &lang, uint clilocID, const bool &isNewCliloc)
{
	WISPFUN_DEBUG("c136_f1");
	string language = ToLowerA(lang).c_str();

	if (!language.length())
		language = "enu";

	wstring str = L"";

	if (!isNewCliloc && false)
	{
		int fileIndex = (clilocID / 1000) % 1000;
		CIntloc *obj = Intloc(fileIndex, language);

		if (obj == NULL && language != "enu")
			obj = Intloc(fileIndex, "enu");

		if (obj != NULL)
			str = obj->Get(clilocID % 1000, true);
	}
	
	if (!str.length())
		str = g_ClilocManager.Cliloc(lang)->GetW(clilocID, true);

	return str;
}
//----------------------------------------------------------------------------------
