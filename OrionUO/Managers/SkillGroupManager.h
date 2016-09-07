/***********************************************************************************
**
** SkillGroupManager.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef SKILLGROUPMANAGER_H
#define SKILLGROUPMANAGER_H
//----------------------------------------------------------------------------------
#include "../Globals.h"
#include "../SkillGroup.h"
//----------------------------------------------------------------------------------
//!Класс менеджера групп навыков
class CSkillGroupManager
{
	SETGET(int, Count);

private:
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
	CSkillGroupObject *m_Groups;

	CSkillGroupManager();
	~CSkillGroupManager();


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
	void Add(CSkillGroupObject *group);

	/*!
	Удалить группу
	@param [__in] group Ссылка на группу
	@return 
	*/
	void Remove(CSkillGroupObject *group);

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
	bool Load(string path);

	/*!
	Сохранение групп в файл конфиг
	@param [__in] path Путьк  файлу конфига
	@return 
	*/
	void Save(string path);
};
//----------------------------------------------------------------------------------
//!Менеджер групп навыков
extern CSkillGroupManager g_SkillGroupManager;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
