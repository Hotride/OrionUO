/***********************************************************************************
**
** CharacterList.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef CHARACTERLIST_H
#define CHARACTERLIST_H
//----------------------------------------------------------------------------------
//!Класс списка персонажей для экрана выбора персонажей
class CCharacterList
{
public:
	int Count = 5;
	int Selected = 0;
	bool OnePerson = false;
	bool Have6Slot = false;
	bool Have7Slot = false;
	ushort ClientFlag = 0;

	string LastCharacterName = "";

private:
	//!Память под 7 персонажей
	string m_Name[7];

public:
	CCharacterList();
	virtual ~CCharacterList() {}

	/*!
	Очистка списка
	@return 
	*/
	void Clear();


	/*!
	Установить имя персонажа в указанном слоте
	@param [__in] pos Позиция в списке
	@param [__in] name Новое имя
	@return 
	*/
	void SetName(intptr_t pos, const string &name);

	/*!
	Получить имя персонажа в указанном слоте
	@param [__in] pos Позиция в списке
	@return Имя персонажа
	*/
	string GetName(intptr_t pos) const;

	/*!
	Получить имя выбранного персонажа
	@return Имя персонажа
	*/
	string GetSelectedName() const;
};
//----------------------------------------------------------------------------------
//!Указатель на список персонажей
extern CCharacterList g_CharacterList;
//----------------------------------------------------------------------------------
#endif
