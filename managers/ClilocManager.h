/****************************************************************************
**
** ClilocManager.h
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
#ifndef ClilocManagerH
#define ClilocManagerH
//---------------------------------------------------------------------------
typedef std::map<DWORD, string> CLILOC_MAP;
//---------------------------------------------------------------------------
class TCliloc : public ICliloc, public TBaseQueueItem
{
private:
	CLILOC_MAP m_ClilocSystem;
	CLILOC_MAP m_ClilocRegular;
	CLILOC_MAP m_ClilocSupport;

	TMappedHeader m_File;

	string m_Language;
	bool m_Loaded;

	string Load(DWORD &id);
public:
	TCliloc(const char *lang);
	virtual ~TCliloc();

	SETGET(string, Language)
	SETGET(bool, Loaded)

	string GetA(DWORD id, string result = "");
	wstring GetW(DWORD id, string result = "");
};
//---------------------------------------------------------------------------
class TClilocManager : public IClilocManager, public TBaseQueue
{
private:
public:
	TClilocManager();
	virtual ~TClilocManager();

	TCliloc *Cliloc(const char *lang);
};
//---------------------------------------------------------------------------
extern TClilocManager *ClilocManager;
//---------------------------------------------------------------------------
#endif