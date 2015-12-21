/****************************************************************************
**
** MacroManager.cpp
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

TOptionsMacroManager *OptionsMacroManager = NULL;
//---------------------------------------------------------------------------
TOptionsMacroManager::TOptionsMacroManager()
: TBaseQueue()
{
}
//---------------------------------------------------------------------------
TOptionsMacroManager::~TOptionsMacroManager()
{
}
//---------------------------------------------------------------------------
void TOptionsMacroManager::LoadFromMacro()
{
	Clear();

	TMacro *obj = (TMacro*)MacroManager->m_Items;

	while (obj != NULL)
	{
		Add(obj->GetOptionsCopy());

		obj = (TMacro*)obj->m_Next;
	}

	if (m_Items == NULL)
		Add(TMacro::CreateBlankMacro());
}
//---------------------------------------------------------------------------
bool TOptionsMacroManager::EntryPointerHere()
{
	TMacro *obj = (TMacro*)m_Items;

	while (obj != NULL)
	{
		if (obj->EntryPointerHere() != NULL)
			break;

		obj = (TMacro*)obj->m_Next;
	}

	return (obj != NULL && obj->EntryPointerHere() != NULL);
}
//---------------------------------------------------------------------------