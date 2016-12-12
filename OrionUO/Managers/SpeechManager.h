/***********************************************************************************
**
** SpeechManager.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef SPEECHMANAGER_H
#define SPEECHMANAGER_H
//----------------------------------------------------------------------------------
#include "../Globals.h"
//----------------------------------------------------------------------------------
class CLangCode
{
	SETGET(string, Code);

	//! Строковая, уникальная абревиатура локали.
	SETGET(string, LangString);

	//! Числовая абревиатура группы? локали.
	SETGET(uint, Unknown);

	//! Название языка.
	SETGET(string, LangName);

	//! Название страны.
	SETGET(string, LangCountry);

public:
	CLangCode();
	virtual ~CLangCode();
};
//----------------------------------------------------------------------------------
class CSpeechItem
{
	//! Код посылаемый клиенту.
	SETGET(ushort, Code);

	//! Строка с ключевым словом для поиска в вводе.
	SETGET(wstring, Data);

	SETGET(bool, CheckStart);
	SETGET(bool, CheckEnd);

public:
	CSpeechItem();
	virtual ~CSpeechItem();
};
//----------------------------------------------------------------------------------
//!Класс менеджера cпича
class CSpeechManager
{
private:
	vector<CSpeechItem> m_SpeechEntries;
	vector<CLangCode> m_LangCodes;
	bool m_Loaded;
	
public:
	CSpeechManager();
	~CSpeechManager();

	/*!
	Загрузка cпич данных
	@return true при успешной загрузке
	*/
	bool LoadSpeech();

	/*!
	Загрузка данных о локалях
	@return true при успешной загрузке
	*/
	bool LoadLangCodes();

	//Достаём вектор с кодами словосочетаний найденных в вводе
	void GetKeywords(const wchar_t *text, UINT_LIST &codes);
};
//----------------------------------------------------------------------------------
//!Ссылка на менеджер cпича
extern CSpeechManager g_SpeechManager;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
