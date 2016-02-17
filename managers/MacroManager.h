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
//Макрос менеджер
class TMacroManager : public TBaseQueue
{
private:
	//Время следующего выполнения
	DWORD m_NextTimer;

	//Время ожидания таргета
	DWORD m_WaitForTargetTimer;

	//Ожидание таргета от бинтов
	bool m_WaitingBandageTarget;

	//Таблица скиллов дял использования
	static BYTE m_SkillIndexTable[24];

	//Выполнить команды подменю
	void ProcessSubMenu();

public:
	TMacroManager();
	virtual ~TMacroManager();

	SETGET(bool, WaitingBandageTarget);
	SETGET(DWORD, WaitForTargetTimer);

	//Поиск макроса
	TMacro *FindMacro(WORD key, bool alt, bool ctrl, bool shift);

	//Загрузить макросы из конфига
	void Load(string path);

	//Сохранить макросы в конфиг
	void Save(string path);

	//Загрузить макросы из опций
	void LoadFromOptions();

	//Начать выполнение макроса
	void Execute();

	//Выполнить действие макроса (или набор действий)
	MACRO_RETURN_CODE Process();
};
//---------------------------------------------------------------------------
extern TMacroManager *MacroManager;
//---------------------------------------------------------------------------
#endif