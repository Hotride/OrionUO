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
//!Класс менеджера групп навыков
class TSkillGroupManager
{
private:
	//!Количество групп
	int m_Count;

	//!Выставление значение по-умолчанию для стандартных групп
	void MakeDefaultMiscellaneous();
	void MakeDefaultCombat();
	void MakeDefaultTradeSkills();
	void MakeDefaultMagic();
	void MakeDefaultWilderness();
	void MakeDefaultThieving();
	void MakeDefaultBard();

public:
	//!Указатель на список групп
	TSkillGroupObject *m_Groups;

	TSkillGroupManager();
	~TSkillGroupManager();

	SETGET(int, Count);

	/*!
	Выставить группы по-умолчанию
	@return
	*/
	void MakeDefault();

	/*!
	Очистить список групп
	@return 
	*/
	void Clear();

	/*!
	Добавить группу
	@param [__in] group Ссылка на группу
	@return 
	*/
	void Add(__in TSkillGroupObject *group);

	/*!
	Удалить группу
	@param [__in] group Ссылка на группу
	@return 
	*/
	void Remove(__in TSkillGroupObject *group);

	/*!
	Получить список видимых строк
	@return Количество видимых строк
	*/
	int GetVisibleLinesCount();

	/*!
	Загрузка групп из файла конфига
	@param [__in] path Путь к файлу конфига
	@return 
	*/
	void Load(__in string path);

	/*!
	Сохранение групп в файл конфиг
	@param [__in] path Путьк  файлу конфига
	@return 
	*/
	void Save(__in string path);
};
//---------------------------------------------------------------------------
//!Менеджер групп навыков
extern TSkillGroupManager SkillGroupManager;
//---------------------------------------------------------------------------
#endif