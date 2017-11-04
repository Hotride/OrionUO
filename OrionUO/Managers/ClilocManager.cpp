// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** ClilocManager.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CClilocManager g_ClilocManager;
//----------------------------------------------------------------------------------
//--------------------------------------CCliloc-------------------------------------
//----------------------------------------------------------------------------------
CCliloc::CCliloc(const string &lang)
: CBaseQueueItem()
{
	WISPFUN_DEBUG("c135_f1");
	m_Loaded = false;
	m_Language = lang;

	if (m_Language.length())
	{
		string path = g_App.UOFilesPath((string("cliloc.") + lang).c_str());

		if (m_File.Load(path))
			m_Loaded = true;
	}
}
//----------------------------------------------------------------------------------
CCliloc::~CCliloc()
{
	WISPFUN_DEBUG("c135_f2");
	m_File.Unload();

	m_ClilocSystem.clear();
	m_ClilocRegular.clear();
	m_ClilocSupport.clear();
}
//----------------------------------------------------------------------------------
/*!
Загрузить клилок
@param [__in] id Индекс клилока
@return Результат загрузки или сообщение с ошибкой
*/
string CCliloc::Load(uint &id)
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
wstring CCliloc::Get(const uint &id, const bool &toCamelCase, string result)
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
wstring CCliloc::GetW(const uint &id, const bool &toCamelCase, string result)
{
	WISPFUN_DEBUG("c135_f5");
	return Get(id, toCamelCase, result);
}
//----------------------------------------------------------------------------------
//-----------------------------------CClilocManager---------------------------------
//----------------------------------------------------------------------------------
CClilocManager::CClilocManager()
: CBaseQueue()
{
}
//----------------------------------------------------------------------------------
CClilocManager::~CClilocManager()
{
	m_ENUCliloc = NULL;
	m_LastCliloc = NULL;
}
//----------------------------------------------------------------------------------
/*!
Получить ссылку на объект клилока (и загрузить при необходимости)
@param [__in] lang Расширение клилока
@return Ссылка на клилок
*/
CCliloc *CClilocManager::Cliloc(const string &lang)
{
	WISPFUN_DEBUG("c136_f1");
	string language = ToLowerA(lang).c_str();

	if (!language.length())
		language = "enu";

	if (language == "enu")
	{
		if (m_ENUCliloc == NULL)
			m_ENUCliloc = (CCliloc*)Add(new CCliloc(language));

		return m_ENUCliloc;
	}

	if (m_LastCliloc != NULL && m_LastCliloc->Language == language)
	{
		if (!m_LastCliloc->Loaded)
			return m_ENUCliloc;

		return m_LastCliloc;
	}

	QFOR(obj, m_Items, CCliloc*)
	{
		if (obj->Language == language)
		{
			if (!obj->Loaded)
				return m_ENUCliloc;

			m_LastCliloc = obj;
			return obj;
		}
	}

	CCliloc *obj = (CCliloc*)Add(new CCliloc(language));

	if (!obj->Loaded)
		return Cliloc("enu");

	m_LastCliloc = obj;

	return obj;
}
//----------------------------------------------------------------------------------
wstring CClilocManager::ParseArgumentsToClilocString(const uint &cliloc, const bool &toCamelCase, wstring args)
{
	WISPFUN_DEBUG("c136_f2");
	while (args.length() && args[0] == L'\t')
		args.erase(args.begin());

	wstring message = Cliloc(g_Language)->GetW(cliloc, toCamelCase).c_str();

	vector<wstring> arguments;

	while (true)
	{
		size_t pos = args.find(L"\t");

		if (pos != string::npos)
		{
			arguments.push_back(args.substr(0, pos));
			args = args.substr(pos + 1);
		}
		else
		{
			arguments.push_back(args);
			break;
		}
	}

	IFOR(i, 0, (int)arguments.size())
	{
		size_t pos1 = message.find(L"~");

		if (pos1 == string::npos)
			break;

		size_t pos2 = message.find(L"~", pos1 + 1);

		if (pos2 == string::npos)
			break;

		if (arguments[i].length() > 1 && *arguments[i].c_str() == L'#')
		{
			uint id = _wtoi(arguments[i].c_str() + 1);
			arguments[i] = Cliloc(g_Language)->GetW(id, toCamelCase).c_str();
		}

		message.replace(pos1, pos2 - pos1 + 1, arguments[i]);
	}

	if (toCamelCase)
		return ToCamelCaseW(message);

	return message;
}
//----------------------------------------------------------------------------------
