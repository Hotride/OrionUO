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
#ifndef SkillGroupH
#define SkillGroupH
//---------------------------------------------------------------------------
//Класс группы навыков
class TSkillGroupObject
{
private:
	//Номера навыков
	BYTE m_Items[60];

	//Количество навыков
	int m_Count;

	//Группа раскрыта или нет
	bool m_Maximized;

	//Группа выбрана
	BYTE m_Selected;

	//Имя группы
	string m_Name;

public:
	//Ссылки на следующую и предыдущую группы
	TSkillGroupObject *m_Next;
	TSkillGroupObject *m_Prev;

	TSkillGroupObject();
	~TSkillGroupObject();

	SETGET(int, Count);
	SETGET(bool, Maximized);
	SETGET(BYTE, Selected);

	void SetName(string val);
	string GetName() const {return m_Name;}

	//Текстура текста группы
	TTextTexture m_Texture;

	BYTE GetItem(int index);

	//Добавить навык в группу	
	void Add(BYTE index);

	//Добавить навык и отсортировать
	void AddSorted(BYTE index);

	//Удалить навык
	void Remove(BYTE index);

	//Проверка, содержит ли группа навык
	bool Contains(BYTE index);

	//Сортировать навыки
	void Sort();

	//Передать навык другой группе
	void TransferTo(TSkillGroupObject *group);
};
//---------------------------------------------------------------------------
#endif