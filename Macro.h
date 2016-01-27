/****************************************************************************
**
** Macro.h
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
#ifndef MacroH
#define MacroH
//---------------------------------------------------------------------------
class TMacroObject : public TBaseQueueItem
{
private:
	MACRO_CODE m_Code;
	MACRO_SUB_CODE m_SubCode;
	char m_HasSubMenu;
public:
	TMacroObject(MACRO_CODE code, MACRO_SUB_CODE subCode);
	virtual ~TMacroObject();

	SETGET(MACRO_CODE, Code);
	SETGET(MACRO_SUB_CODE, SubCode);
	SETGET(char, HasSubMenu);

	virtual int GetType() {return 0;}
};
//---------------------------------------------------------------------------
class TMacroObjectString : public TMacroObject
{
private:
	string m_String;
public:
	TMacroObjectString(MACRO_CODE code, MACRO_SUB_CODE subCode, string str);
	virtual ~TMacroObjectString();

	SETGET(string, String);

	virtual int GetType() {return 2;}
};
//---------------------------------------------------------------------------
class TMacroObjectEntry : public TMacroObject
{
private:
public:
	TMacroObjectEntry(MACRO_CODE code, MACRO_SUB_CODE subCode, string str);
	virtual ~TMacroObjectEntry();

	TEntryText *TextEntry;

	virtual int GetType() {return 3;}
};
//---------------------------------------------------------------------------
class TMacro : public TBaseQueueItem
{
private:
	WORD m_Key;
	bool m_Alt;
	bool m_Ctrl;
	bool m_Shift;

	static const char *m_MacroActionName[60];
	static const char *m_MacroAction[210];
public:
	TMacro(WORD key, bool alt, bool ctrl, bool shift);
	virtual ~TMacro();

	SETGET(WORD, Key);
	SETGET(bool, Alt);
	SETGET(bool, Ctrl);
	SETGET(bool, Shift);
	
	static const char *GetActionName(int index) {return m_MacroActionName[index];}
	static const char *GetAction(int index) {return m_MacroAction[index];}

	void ChangeObject(TMacroObject *source, TMacroObject *obj);

	static TMacro *Load(TMappedHeader &file);
	void Save(TFileWriter *writer);
	
	static TMacro *CreateBlankMacro();
	static TMacroObject *CreateMacro(MACRO_CODE code);
	static void GetBoundByCode(MACRO_CODE code, int &count, int &offset);

	TMacro *GetOptionsCopy();
	TMacro *GetCopy();

	TMacroObject *EntryPointerHere();
};
//---------------------------------------------------------------------------
extern TMacroObject *MacroPointer;
//---------------------------------------------------------------------------
#endif