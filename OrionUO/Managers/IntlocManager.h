/***********************************************************************************
**
** IntlocManager.h
**
** Copyright (C) October 2017 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef INTLOCMANAGER_H
#define INTLOCMANAGER_H
//----------------------------------------------------------------------------------
typedef map<uint, uint> INTLOC_MAP;
//----------------------------------------------------------------------------------
//!Класс с данными интлок-файла
class CIntloc : public CBaseQueueItem
{
	SETGET(string, Language, "");
	SETGET(int, FileIndex, -1);
	SETGET(bool, Loaded, false);

private:
	INTLOC_MAP m_Map;

	//string Load(uint &id);

	//inline wstring CamelCaseTest(const bool &toCamelCase, const string &result);

public:
	CIntloc(const int &fileIndex, const string &lang);
	virtual ~CIntloc();

	//!Указатель на проекцию файла в памяти (оно же ридер)
	WISP_FILE::CMappedFile m_File;

	/*wstring Get(const uint &id, const bool &toCamelCase = false, string result = "");

	string GetA(const uint &id, const bool &toCamelCase = false, string result = "");

	wstring GetW(const uint &id, const bool &toCamelCase = false, string result = "");*/
};
//----------------------------------------------------------------------------------
//!Класс менеджера клилоков
class CIntlocManager : public CBaseQueue
{
private:
	CIntloc *Intloc(const int &fileIndex, const string &lang);

public:
	CIntlocManager();
	virtual ~CIntlocManager();

	wstring Intloc(const string &lang, uint clilocID, const bool &isNewCliloc);
};
//----------------------------------------------------------------------------------
//!Ссылка на менеджер интлоков
extern CIntlocManager g_IntlocManager;
//----------------------------------------------------------------------------------
#endif //INTLOCMANAGER_H
//----------------------------------------------------------------------------------
