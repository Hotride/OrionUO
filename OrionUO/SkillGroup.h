/***********************************************************************************
**
** SkillGroup.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef SKILLGROUP_H
#define SKILLGROUP_H
//----------------------------------------------------------------------------------
#include "Globals.h"
#include "GLEngine/GLTextTexture.h"
//----------------------------------------------------------------------------------
//Класс группы навыков
class CSkillGroupObject
{
	SETGET(int, Count);
	SETGET(bool, Maximized);
	SETGET(uchar, Selected);
	SETGETE(string, Name, OnChangeName);

private:
	//Номера навыков
	uchar m_Items[60];

public:
	//Ссылки на следующую и предыдущую группы
	CSkillGroupObject *m_Next;
	CSkillGroupObject *m_Prev;

	CSkillGroupObject();
	~CSkillGroupObject();

	//Текстура текста группы
	CGLTextTexture m_Texture;

	uchar GetItem(int index);

	//Добавить навык в группу	
	void Add(uchar index);

	//Добавить навык и отсортировать
	void AddSorted(uchar index);

	//Удалить навык
	void Remove(uchar index);

	//Проверка, содержит ли группа навык
	bool Contains(uchar index);

	//Сортировать навыки
	void Sort();

	//Передать навык другой группе
	void TransferTo(CSkillGroupObject *group);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
