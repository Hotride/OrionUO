/****************************************************************************
**
** MacroManager.h
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
#ifndef MacroManagerH
#define MacroManagerH
//---------------------------------------------------------------------------
class TMacroManager : public TBaseQueue
{
private:
	DWORD m_NextTimer;
	DWORD m_WaitForTargetTimer;
	bool m_WaitingBandageTarget;
	
	static BYTE m_SkillIndexTable[24];

	void ProcessSubMenu();

public:
	TMacroManager();
	virtual ~TMacroManager();

	SETGET(bool, WaitingBandageTarget);
	SETGET(DWORD, WaitForTargetTimer);

	TMacro *FindMacro(WORD key, bool alt, bool ctrl, bool shift);

	void Load(string path);
	void Save(string path);

	void LoadFromOptions();

	void Execute();
	MACRO_RETURN_CODE Process();
};
//---------------------------------------------------------------------------
extern TMacroManager *MacroManager;
//---------------------------------------------------------------------------
#endif