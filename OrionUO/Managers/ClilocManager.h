/***********************************************************************************
**
** ClilocManager.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef CLILOCMANAGER_H
#define CLILOCMANAGER_H
//----------------------------------------------------------------------------------
typedef map<uint, string> CLILOC_MAP;
//----------------------------------------------------------------------------------
//!Класс с данными клилок-файла
class CCliloc : public CBaseQueueItem
{
public:
	string Language = "";
	bool Loaded = false;

private:
	//!Системные клилоки (id < 1000000)
	CLILOC_MAP m_ClilocSystem;

	//!Обычные клилоки (id >= 1000000 && id < 3000000)
	CLILOC_MAP m_ClilocRegular;

	//!Клилоки для помощи (id >= 3000000)
	CLILOC_MAP m_ClilocSupport;

	/*!
	Загрузить клилок
	@param [__in] id Индекс клилока
	@return Результат загрузки или сообщение с ошибкой
	*/
	string Load(uint &id);

	inline wstring CamelCaseTest(bool toCamelCase, const string &result);

public:
	CCliloc(const string &lang);
	virtual ~CCliloc();

	//!Указатель на проекцию файла в памяти (оно же ридер)
	WISP_FILE::CMappedFile m_File;

	/*!
	Получить ASCII строку по id (и загрузить при необходимости)
	@param [__in] id Индекс клилока
	@param [__in] result Стандартное сообщение, если клилок не был найден
	@return Полученный результат, замена или сообщение с ошибкой
	*/
	wstring Get(int id, bool toCamelCase = false, string result = "");

	string GetA(int id, bool toCamelCase = false, string result = "");

	/*!
	Получить Unicode строку по id (и загрузить при необходимости)
	@param [__in] id Индекс клилока
	@param [__in] result Стандартное сообщение, если клилок не был найден
	@return Полученный результат, замена или сообщение с ошибкой
	*/
	wstring GetW(int id, bool toCamelCase = false, string result = "");
};
//----------------------------------------------------------------------------------
//!Класс менеджера клилоков
class CClilocManager : public CBaseQueue
{
private:
	//!Ссылка на последний использованный клилок (для более быстрого доступа)
	CCliloc *m_LastCliloc{ NULL };

	//!Ссылка на дефолтный клилок (для более быстрого доступа)
	CCliloc *m_ENUCliloc{ NULL };

public:
	CClilocManager();
	virtual ~CClilocManager();

	/*!
	Получить ссылку на объект клилока (и загрузить при необходимости)
	@param [__in] lang Расширение клилока
	@return Ссылка на клилок
	*/
	CCliloc *Cliloc(const string &lang);

	wstring ParseArgumentsToClilocString(int cliloc, bool toCamelCase, wstring args);
};
//----------------------------------------------------------------------------------
//!Ссылка на менеджер клилоков
extern CClilocManager g_ClilocManager;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
