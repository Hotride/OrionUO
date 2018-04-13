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
//!Класс с данными интлок-файла
class CIntloc : public CBaseQueueItem
{
public:
	string Language = "";
	int FileIndex = -1;
	bool Loaded = false;

private:
	WSTRING_LIST m_Strings;

public:
	CIntloc(const int &fileIndex, const string &lang);
	virtual ~CIntloc();

	//!Указатель на проекцию файла в памяти (оно же ридер)
	WISP_FILE::CMappedFile m_File;

	wstring Get(const uint &id, const bool &toCamelCase = false);
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
