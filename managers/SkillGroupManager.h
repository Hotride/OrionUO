/****************************************************************************
**
** SkillGroupManager.h
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
#ifndef SkillGroupManagerH
#define SkillGroupManagerH
//---------------------------------------------------------------------------
//Менеджер групп навыков
class TSkillGroupManager
{
private:
	int m_Count; //Количество групп

	//Выставление значение по-умолчанию для стандартных групп
	void MakeDefaultMiscellaneous();
	void MakeDefaultCombat();
	void MakeDefaultTradeSkills();
	void MakeDefaultMagic();
	void MakeDefaultWilderness();
	void MakeDefaultThieving();
	void MakeDefaultBard();

public:
	TSkillGroupObject *m_Groups; //Указатель на список групп

	TSkillGroupManager();
	~TSkillGroupManager();

	SETGET(int, Count);

	//Выставить группы по-умолчанию
	void MakeDefault();
	//Очистить список групп
	void Clear();
	//Добавить группу
	void Add(TSkillGroupObject *group);
	//Удалить группу
	void Remove(TSkillGroupObject *group);

	//Получить список видимых строк
	int GetVisibleLinesCount();

	//Загрузка групп из файла конфига
	void Load(string path);
	//Сохранение групп в файл конфиг
	void Save(string path);
};
//---------------------------------------------------------------------------
extern TSkillGroupManager SkillGroupManager;
//---------------------------------------------------------------------------
#endif