/***********************************************************************************
**
** CityList.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef CITYLIST_H
#define CITYLIST_H
//----------------------------------------------------------------------------------
//!Класс города (для старых клиентов)
class CCityItem
{
	SETGET(uchar, LocationIndex, 0);
	SETGET(string, Name, "");
	SETGET(string, Area, "");
	SETGET(bool, Selected, false);

public:
	CCityItem();
	virtual ~CCityItem();

	//!Ссылка на описание города
	class CCity m_City;

	/*!
	Определение версии класса
	@return Новая или старая
	*/
	virtual bool IsNewCity() { return false; }

	/*!
	Инициализация
	@return 
	*/
	void InitCity();
};
//--------------------------------------------------------------------------
//!Класс города (для новых клиентов)
class CCityItemNew : public CCityItem
{
	SETGET(uint, X, 0);
	SETGET(uint, Y, 0);
	SETGET(uint, Z, 0);
	SETGET(uint, MapIndex, 0);
	SETGET(uint, Cliloc, 0);

public:
	CCityItemNew();
	virtual ~CCityItemNew();

	/*!
	Определение версии класса
	@return Новая или старая
	*/
	bool IsNewCity() { return true; }
};
//--------------------------------------------------------------------------
//!Класс списка городов
class CCityList
{
private:
	vector<CCityItem*> m_CityList;

public:
	CCityList();
	virtual ~CCityList();

	size_t CityCount() { return m_CityList.size(); }

	void AddCity(CCityItem *city) { m_CityList.push_back(city); }

	CCityItem *GetCity(const uint &index);

	void Clear();
};
//---------------------------------------------------------------------------
//!Ссылка на список городов
extern CCityList g_CityList;
//---------------------------------------------------------------------------
#endif