/***********************************************************************************
**
** ClilocManager.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "ClilocManager.h"
#include "../Wisp/WispApplication.h"
//----------------------------------------------------------------------------------
CClilocManager g_ClilocManager;
//----------------------------------------------------------------------------------
//--------------------------------------CCliloc-------------------------------------
//----------------------------------------------------------------------------------
CCliloc::CCliloc(string lang)
: CBaseQueueItem()
{
	m_Loaded = false;
	m_Language = lang;

	if (m_Language.length())
	{
		string path = g_App.FilePath((string("cliloc.") + lang).c_str());

		if (m_File.Load(path))
			m_Loaded = true;
	}
}
//----------------------------------------------------------------------------------
CCliloc::~CCliloc()
{
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
/*!
Получить ASCII строку по id (и загрузить при необходимости)
@param [__in] id Индекс клилока
@param [__in] result Стандартное сообщение, если клилок не был найден
@return Полученный результат, замена или сообщение с ошибкой
*/
string CCliloc::GetA(uint id, string result)
{
	if (id >= 3000000)
	{
		CLILOC_MAP::iterator i = m_ClilocSupport.find(id);
		if (i != m_ClilocSupport.end() && (*i).second.length())
			return (*i).second;
	}
	else if (id >= 1000000)
	{
		CLILOC_MAP::iterator i = m_ClilocRegular.find(id);
		if (i != m_ClilocRegular.end() && (*i).second.length())
			return (*i).second;
	}
	else
	{
		CLILOC_MAP::iterator i = m_ClilocSystem.find(id);
		if (i != m_ClilocSystem.end() && (*i).second.length())
			return (*i).second;
	}

	uint tmpID = id;
	string loadStr = Load(tmpID);
	if (!tmpID && loadStr.length())
		return loadStr;
	else if (!result.length())
	{
		char str[50] = {0};
		sprintf_s(str, "Unknown Cliloc #%i", id);

		result = str;
	}

	return result;
}
//----------------------------------------------------------------------------------
/*!
Получить Unicode строку по id (и загрузить при необходимости)
@param [__in] id Индекс клилока
@param [__in] result Стандартное сообщение, если клилок не был найден
@return Полученный результат, замена или сообщение с ошибкой
*/
wstring CCliloc::GetW(uint id, string result)
{
	return DecodeUTF8(GetA(id, result));
}
//----------------------------------------------------------------------------------
//-----------------------------------CClilocManager---------------------------------
//----------------------------------------------------------------------------------
CClilocManager::CClilocManager()
: CBaseQueue(), m_LastCliloc(NULL)
{
}
//----------------------------------------------------------------------------------
CClilocManager::~CClilocManager()
{
}
//----------------------------------------------------------------------------------
/*!
Получить ссылку на объект клилока (и загрузить при необходимости)
@param [__in] lang Расширение клилока
@return Ссылка на клилок
*/
CCliloc *CClilocManager::Cliloc(string lang)
{
	string language = ToLowerA(lang);

	if (m_LastCliloc != NULL && m_LastCliloc->GetLanguage() == language)
		return m_LastCliloc;

	CCliloc *obj = (CCliloc*)m_Items;

	while (obj != NULL)
	{
		if (obj->GetLanguage() == language)
		{
			m_LastCliloc = obj;
			return obj;
		}

		obj = (CCliloc*)obj->m_Next;
	}

	obj = new CCliloc(language);

	if (!obj->Loaded)
	{
		const string enu("enu");
		delete obj;

		obj = (CCliloc*)m_Items;

		while (obj != NULL)
		{
			if (obj->GetLanguage() == enu)
			{
				m_LastCliloc = obj;
				return obj;
			}

			obj = (CCliloc*)obj->m_Next;
		}

		obj = new CCliloc(enu);
	}

	Add(obj);

	m_LastCliloc = obj;
	return obj;
}
//----------------------------------------------------------------------------------
wstring CClilocManager::ParseArgumentsToClilocString(uint cliloc, wstring args)
{
	while (args.length() && args[0] == L'\t')
		args.erase(args.begin());

	wstring message = Cliloc(g_Language)->GetW(cliloc).c_str();

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
			DWORD id = _wtoi(arguments[i].c_str() + 1);
			arguments[i] = Cliloc(g_Language)->GetW(id).c_str();
		}

		message.replace(pos1, pos2 - pos1 + 1, arguments[i]);
	}

	return message;
}
//----------------------------------------------------------------------------------
