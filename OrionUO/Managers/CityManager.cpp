// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** CityManager.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CCityManager g_CityManager;
//----------------------------------------------------------------------------------
//---------------------------------------CCity--------------------------------------
//----------------------------------------------------------------------------------
CCity::CCity(const string &name, const wstring &description)
: m_Name(name), m_Description(description)
{
}
//----------------------------------------------------------------------------------
CCity::~CCity()
{
}
//----------------------------------------------------------------------------------
//------------------------------------CCityManager----------------------------------
//----------------------------------------------------------------------------------
CCityManager::CCityManager()
{
}
//----------------------------------------------------------------------------------
void CCityManager::Init()
{
	WISPFUN_DEBUG("c134_f1");
	WISP_FILE::CMappedFile file;

	if (file.Load(g_App.UOFilesPath("citytext.enu")))
	{
		puchar end = file.Ptr + file.Size;

		while (file.Ptr < end)
		{
			if (!memcmp(&file.Ptr[0], "END\0", 4))
			{
				file.Move(4);

				puchar startBlock = file.Ptr + 4;
				puchar ptrBlock = startBlock;
				string name = "";

				while (ptrBlock < end)
				{
					if (*ptrBlock == '<')
					{
						int len = ptrBlock - startBlock;
						name.resize(len);
						memcpy(&name[0], &startBlock[0], len);

						break;
					}

					ptrBlock++;
				}
				
				string text = "";

				while (file.Ptr < end)
				{
					string str = file.ReadString(0);

					if (text.length())
						text += "\n\n";

					text += str;

					if (*file.Ptr == 0x2E || !memcmp(&file.Ptr[0], "END\0", 4))
						break;
				}

				m_CityList.push_back(CCity(name, ToWString(text)));
			}
			else
				file.Move(1);
		}

		file.Unload();
	}
	else
	{
		//!Названия городов
		static const string cityNames[9] =
		{
			"Yew",
			"Minoc",
			"Britain",
			"Moonglow",
			"Trinsic",
			"Magincia",
			"Jhelom",
			"Skara Brae",
			"Vesper"
		};

		CCliloc *cliloc = g_ClilocManager.Cliloc(g_Language);

		if (cliloc != NULL)
		{
			IFOR(i, 0, 9)
				m_CityList.push_back(CCity(cityNames[i], cliloc->GetW(1075072 + i)));
		}
	}
}
//---------------------------------------------------------------------------
CCityManager::~CCityManager()
{
	Clear();
}
//---------------------------------------------------------------------------
/*!
Получить указатель на город
@param [__in] name Имя города
@return Ссылка на город или NULL
*/
CCity CCityManager::GetCity(const string &name)
{
	WISPFUN_DEBUG("c134_f2");
	for (deque<CCity>::iterator i = m_CityList.begin(); i != m_CityList.end(); i++)
	{
		if (i->Name == name)
			return *i;
	}

	return CCity("", L"");
}
//---------------------------------------------------------------------------
void CCityManager::Clear()
{
	WISPFUN_DEBUG("c134_f3");
	m_CityList.clear();
}
//---------------------------------------------------------------------------