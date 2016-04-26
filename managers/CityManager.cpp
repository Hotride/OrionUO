/****************************************************************************
**
** CityManager.cpp
**
** Copyright (C) September 2015 Hotride
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

TCityManager *CityManager = NULL;
//---------------------------------------------------------------------------
//-------------------------------------TCity---------------------------------
//---------------------------------------------------------------------------
TCity::TCity(string name)
: TBaseQueueItem(), m_Name(name)
{
}
//---------------------------------------------------------------------------
TCity::~TCity()
{
	m_Texture.Clear();
}
//---------------------------------------------------------------------------
//---------------------------------TCityManager------------------------------
//---------------------------------------------------------------------------
TCityManager::TCityManager()
: TBaseQueue()
{
	TMappedHeader file;
	memset(&file, 0, sizeof(file));

	if (FileManager.LoadFileToMemory(file, FilePath("citytext.enu").c_str()))
	{
		PBYTE end = file.Ptr + file.Size;

		while (file.Ptr < end)
		{
			if (!memcmp(&file.Ptr[0], "END\0", 4))
			{
				file.Move(4);

				PBYTE startBlock = file.Ptr + 4;
				PBYTE ptrBlock = startBlock;
				char *name = NULL;

				while (ptrBlock < end)
				{
					if (*ptrBlock == '<')
					{
						int len = ptrBlock - startBlock;
						name = new char[len + 1];
						memcpy(&name[0], &startBlock[0], len);
						name[len] = 0;

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

				if (name != NULL)
				{
					TCity *city = new TCity(name);

					FontManager->SetUseHTML(true);

					wstring wtext = ToWString(text);
					TTextTexture &th = city->m_Texture;
					FontManager->GenerateW(0, th, wtext.c_str(), 0, 30, 150);

					FontManager->SetUseHTML(false);

					Add(city);

					delete name;
				}
			}
			else
				file.Move(1);
		}

		FileManager.UnloadFileFromMemory(file);
	}
	else
	{
		TCliloc *cliloc = ClilocManager->Cliloc(g_Language);

		if (cliloc != NULL)
		{
			IFOR(i, 0, 9)
			{
				TCity *city = new TCity(CITY_NAMES[i]);

				FontManager->SetUseHTML(true);

				wstring wtext = cliloc->GetW(1075072 + i);

				TTextTexture &th = city->m_Texture;
				FontManager->GenerateW(0, th, wtext.c_str(), 0, 30, 150);

				FontManager->SetUseHTML(false);

				Add(city);
			}
		}
	}
}
//---------------------------------------------------------------------------
TCityManager::~TCityManager()
{
}
//---------------------------------------------------------------------------
/*!
Получить указатель на город
@param [__in] name Имя города
@return Ссылка на город или NULL
*/
TCity *TCityManager::GetCity(string name)
{
	TCity *obj = (TCity*)m_Items;

	while (obj != NULL)
	{
		if (obj->GetName() == name)
			break;

		obj = (TCity*)obj->m_Next;
	}

	return obj;
}
//---------------------------------------------------------------------------