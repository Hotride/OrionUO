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
#include "SpeechManager.h"

TSpeechManager *SpeechManager = NULL;
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
Загрузка ASCII шрифтов
@return true при успешной загрузке
*/
bool TSpeechManager::LoadSpeech()
{
	return true;
}
//---------------------------------------------------------------------------