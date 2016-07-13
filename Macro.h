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
//Класс объекта макроса
class TMacroObject : public TBaseQueueItem
{
private:
	//Код действия
	MACRO_CODE m_Code;

	//Подкод действия
	MACRO_SUB_CODE m_SubCode;

	//Имеет подменю
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
//Класс объекта макроса, содержащий строку
class TMacroObjectString : public TMacroObject
{
private:
	//Строка действия
	string m_String;
public:
	TMacroObjectString(MACRO_CODE code, MACRO_SUB_CODE subCode, string str);
	virtual ~TMacroObjectString();

	SETGET(string, String);

	virtual int GetType() {return 2;}
};
//---------------------------------------------------------------------------
//Класс объекта макроса, содержащий поле для ввода текста (только для опций)
class TMacroObjectEntry : public TMacroObject
{
private:
public:
	TMacroObjectEntry(MACRO_CODE code, MACRO_SUB_CODE subCode, string str);
	virtual ~TMacroObjectEntry();

	//Указатель на поле для ввода текста
	TEntryText *TextEntry;

	virtual int GetType() {return 3;}
};
//---------------------------------------------------------------------------
//Класс макроса
class TMacro : public TBaseQueueItem
{
private:
	//Код клавиши для срабатывания макроса
	WORD m_Key;

	//Флаги дополнительных клавишь
	bool m_Alt;
	bool m_Ctrl;
	bool m_Shift;
public:
	TMacro(WORD key, bool alt, bool ctrl, bool shift);
	virtual ~TMacro();

	static const int MACRO_ACTION_NAME_COUNT = 60;
	static const int MACRO_ACTION_COUNT = 210;

	//Названия макросов
	static const char *m_MacroActionName[MACRO_ACTION_NAME_COUNT];

	//Названия действий
	static const char *m_MacroAction[MACRO_ACTION_COUNT];

	SETGET(WORD, Key);
	SETGET(bool, Alt);
	SETGET(bool, Ctrl);
	SETGET(bool, Shift);
	
	static const char *GetActionName(int index) {return m_MacroActionName[index];}
	static const char *GetAction(int index) {return m_MacroAction[index];}

	//Заменить макрос
	void ChangeObject(TMacroObject *source, TMacroObject *obj);

	//Загрузить макросы из файла
	static TMacro *Load(TMappedHeader &file);

	//Сохранить макросы в файл
	void Save(TFileWriter *writer);

	//Создать пустой макрос
	static TMacro *CreateBlankMacro();

	//Создать макрос по коду
	static TMacroObject *CreateMacro(MACRO_CODE code, bool forOptionsGump = true);

	//Получить смещение индекса названия относительно начала списка и количество в группе
	static void GetBoundByCode(MACRO_CODE code, int &count, int &offset);

	//Получить копию макросов для опций
	TMacro *GetOptionsCopy();

	//Получить копию макросов
	TMacro *GetCopy();

	//Проверка нахождения активного поля для ввода текста
	TMacroObject *EntryPointerHere();
};
//---------------------------------------------------------------------------
extern TMacroObject *MacroPointer;
//---------------------------------------------------------------------------
#endif