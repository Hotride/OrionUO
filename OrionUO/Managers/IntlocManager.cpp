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
		if (m_File.Load(g_App.FilePath("intloc%02i.%s", fileIndex, lang.c_str())))
		{
			m_Loaded = true;
		}
	}
}
//----------------------------------------------------------------------------------
CIntloc::~CIntloc()
{
	WISPFUN_DEBUG("c135_f2");
	m_File.Unload();

	m_Map.clear();
}
//----------------------------------------------------------------------------------
/*!
Загрузить клилок
@param [__in] id Индекс клилока
@return Результат загрузки или сообщение с ошибкой
*/
/*string CCliloc::Load(uint &id)
{
	WISPFUN_DEBUG("c135_f3");
	string result = "";

	if (m_Loaded)
	{
		m_File.ResetPtr();
		m_File.Move(6);

		while (!m_File.IsEOF())
		{
			DWORD currentID = m_File.ReadUInt32LE();

			m_File.Move(1);

			short len = m_File.ReadUInt16LE();

			if (currentID == id)
			{
				result = m_File.ReadString(len);

				if (id >= 3000000)
					m_ClilocSupport[currentID] = result;
				else if (id >= 1000000)
					m_ClilocRegular[currentID] = result;
				else
					m_ClilocSystem[currentID] = result;

				break;
			}
			else
				m_File.Move(len);
		}
	}

	id = 0;

	return result;
}
//----------------------------------------------------------------------------------
wstring CCliloc::CamelCaseTest(const bool &toCamelCase, const string &result)
{
	if (toCamelCase)
		return ToCamelCaseW(DecodeUTF8(result));

	return DecodeUTF8(result);
}
//----------------------------------------------------------------------------------
/*!
Получить ASCII строку по id (и загрузить при необходимости)
@param [__in] id Индекс клилока
@param [__in] result Стандартное сообщение, если клилок не был найден
@return Полученный результат, замена или сообщение с ошибкой
*/
/*wstring CCliloc::Get(const uint &id, const bool &toCamelCase, string result)
{
	WISPFUN_DEBUG("c135_f4");
	if (id >= 3000000)
	{
		CLILOC_MAP::iterator i = m_ClilocSupport.find(id);
		if (i != m_ClilocSupport.end() && (*i).second.length())
			return CamelCaseTest(toCamelCase, (*i).second);
	}
	else if (id >= 1000000)
	{
		CLILOC_MAP::iterator i = m_ClilocRegular.find(id);
		if (i != m_ClilocRegular.end() && (*i).second.length())
			return CamelCaseTest(toCamelCase, (*i).second);
	}
	else
	{
		CLILOC_MAP::iterator i = m_ClilocSystem.find(id);
		if (i != m_ClilocSystem.end() && (*i).second.length())
			return CamelCaseTest(toCamelCase, (*i).second);
	}

	uint tmpID = id;
	string loadStr = Load(tmpID);

	if (!tmpID && loadStr.length())
		return CamelCaseTest(toCamelCase, loadStr);
	else
	{
		if (m_Language != "ENU" && this->Language != "enu")
			return g_ClilocManager.Cliloc("enu")->Get(id, toCamelCase, result);
		else if (!result.length())
		{

			char str[50] = { 0 };
			sprintf_s(str, "Unknown Cliloc #%i", id);

			result = str;
		}
	}

	return CamelCaseTest(toCamelCase, result);
}
//----------------------------------------------------------------------------------
string CCliloc::GetA(const uint &id, const bool &toCamelCase, string result)
{
	WISPFUN_DEBUG("c135_f4_1");
	return ToString(Get(id, toCamelCase, result));
}
//----------------------------------------------------------------------------------
/*!
Получить Unicode строку по id (и загрузить при необходимости)
@param [__in] id Индекс клилока
@param [__in] result Стандартное сообщение, если клилок не был найден
@return Полученный результат, замена или сообщение с ошибкой
*/
/*wstring CCliloc::GetW(const uint &id, const bool &toCamelCase, string result)
{
	WISPFUN_DEBUG("c135_f5");
	return Get(id, toCamelCase, result);
}*/
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

	return NULL;
}
//----------------------------------------------------------------------------------
wstring CIntlocManager::Intloc(const string &lang, uint clilocID, const bool &isNewCliloc)
{
	WISPFUN_DEBUG("c136_f1");
	string language = ToLowerA(lang).c_str();

	if (!language.length())
		language = "enu";

	int fileIndex = (clilocID / 1000) % 1000;
	CIntloc *obj = Intloc(fileIndex, language);
	wstring str = L"";

	//if (obj != NULL)
	//	str = obj->GetW(clilocID % 1000, true);
	
	if (!str.length())
	{
		if (!isNewCliloc)
			clilocID += 3000000;

		str = g_ClilocManager.Cliloc(lang)->GetW(clilocID, true);
	}

	return str;
}
//----------------------------------------------------------------------------------
