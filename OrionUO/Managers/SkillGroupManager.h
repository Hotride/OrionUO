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
//!Класс менеджера групп навыков
class CSkillGroupManager
{
	int Count = 0;

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
	CSkillGroupObject *m_Groups{ NULL };

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
	bool Remove(CSkillGroupObject *group);

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
