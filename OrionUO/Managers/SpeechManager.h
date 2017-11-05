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
class CLangCode
{
	SETGET(int, Code, 0);

	//! Строковая, уникальная абревиатура локали.
	SETGET(string, Abbreviature, "");

	//! Числовая абревиатура группы? локали.
	SETGET(uint, Unknown, 0);

	//! Название языка.
	SETGET(string, Language, "");

	//! Название страны.
	SETGET(string, Country, "");

public:
	CLangCode() {}
	CLangCode(const string &abbreviature, const int &code, const string &language, const string &country)
		: m_Abbreviature(abbreviature), m_Code(code), m_Language(language), m_Country(country) {}
	virtual ~CLangCode() {}
};
//----------------------------------------------------------------------------------
class CSpeechItem
{
	//! Код посылаемый клиенту.
	SETGET(ushort, Code, 0);

	//! Строка с ключевым словом для поиска в вводе.
	SETGET(wstring, Data, L"");

	SETGET(bool, CheckStart, false);
	SETGET(bool, CheckEnd, false);

public:
	CSpeechItem() {}
	CSpeechItem(const ushort &code, const wstring &data);
	virtual ~CSpeechItem() {}
};
//----------------------------------------------------------------------------------
//!Класс менеджера cпича
class CSpeechManager
{
	SETGETP(CLangCode*, CurrentLanguage, NULL);

private:
	vector<CSpeechItem> m_SpeechEntries;
	vector<CLangCode> m_LangCodes;
	bool m_Loaded{ false };
	
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
