/****************************************************************************
**
** SpeechManager.h
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
//--------------------------------------------------------------------------
#ifndef SpeechManagerH
#define SpeechManagerH
//--------------------------------------------------------------------------
class TSpeechItem
{
private:
	WORD m_Code;
	wstring m_Data;
	bool m_CheckStart;
	bool m_CheckEnd;

public:
	TSpeechItem();
	virtual ~TSpeechItem();

	SETGET(WORD, Code);
	SETGET(wstring, Data);
	SETGET(bool, CheckStart);
	SETGET(bool, CheckEnd);
};
//--------------------------------------------------------------------------
//!Класс менеджера cпича
class TSpeechManager
{
private:
public:
	TSpeechManager();
	~TSpeechManager();

	/*!
	Загрузка cпич данных
	@return true при успешной загрузке
	*/
	bool LoadSpeech();
};
//--------------------------------------------------------------------------
//!Ссылка на менеджер cпича
extern TSpeechManager *SpeechManager;
//--------------------------------------------------------------------------
#endif