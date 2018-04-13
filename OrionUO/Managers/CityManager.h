/***********************************************************************************
**
** CityManager.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef CITYMANAGER_H
#define CITYMANAGER_H
//----------------------------------------------------------------------------------
//!Класс с данными о городе
class CCity
{
public:
	string Name = "";
	wstring Description = L"";

	CCity(const string &name, const wstring &description);
	virtual ~CCity();
};
//---------------------------------------------------------------------------
//!Класс менеджера списка городов
class CCityManager
{
public:
	CCityManager();
	virtual ~CCityManager();

	void Init();

	deque<CCity> m_CityList;

	/*!
	Получить указатель на город
	@param [__in] name Имя города
	@return Ссылка на город или NULL
	*/
	CCity GetCity(const string &name);

	void Clear();
};
//---------------------------------------------------------------------------
//!Ссылка на менеджер городов
extern CCityManager g_CityManager;
//---------------------------------------------------------------------------
#endif