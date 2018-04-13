/***********************************************************************************
**
** Macro.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef MACRO_H
#define MACRO_H
//----------------------------------------------------------------------------------
//Класс объекта макроса
class CMacroObject : public CBaseQueueItem
{
public:
	//Код действия
	MACRO_CODE Code = MC_NONE;

	//Подкод действия
	MACRO_SUB_CODE SubCode = MSC_NONE;

	//Имеет подменю
	char HasSubMenu = 0;

	CMacroObject(const MACRO_CODE &code, const MACRO_SUB_CODE &subCode);
	virtual ~CMacroObject();

	virtual bool HaveString() { return false; }
};
//----------------------------------------------------------------------------------
//Класс объекта макроса, содержащий строку
class CMacroObjectString : public CMacroObject
{
public:
	//Строка действия
	string String = "";

	CMacroObjectString(const MACRO_CODE &code, const MACRO_SUB_CODE &subCode, const string &str);
	virtual ~CMacroObjectString();

	virtual bool HaveString() { return true; }
};
//----------------------------------------------------------------------------------
//Класс макроса
class CMacro : public CBaseQueueItem
{
public:
	//Код клавиши для срабатывания макроса
	ushort Key = 0;

	//Флаги дополнительных клавиш
	bool Alt = false;
	bool Ctrl = false;
	bool Shift = false;

	CMacro(const ushort &key, const bool &alt, const bool &ctrl, const bool &shift);
	virtual ~CMacro();

	static const int MACRO_ACTION_NAME_COUNT = 60;
	static const int MACRO_ACTION_COUNT = 210;

	//Названия макросов
	static const char *m_MacroActionName[MACRO_ACTION_NAME_COUNT];

	//Названия действий
	static const char *m_MacroAction[MACRO_ACTION_COUNT];

	static const char *GetActionName(int index) { return m_MacroActionName[index]; }
	static const char *GetAction(int index) { return m_MacroAction[index]; }

	//Заменить макрос
	void ChangeObject(CMacroObject *source, CMacroObject *obj);

	//Загрузить макросы из файла
	static CMacro *Load(WISP_FILE::CMappedFile &file);

	//Сохранить макросы в файл
	void Save(WISP_FILE::CBinaryFileWritter &writter);

	//Создать пустой макрос
	static CMacro *CreateBlankMacro();

	//Создать макрос по коду
	static CMacroObject *CreateMacro(const MACRO_CODE &code);

	//Получить смещение индекса названия относительно начала списка и количество в группе
	static void GetBoundByCode(const MACRO_CODE &code, int &count, int &offset);

	//Получить копию макросов
	CMacro *GetCopy();
};
//----------------------------------------------------------------------------------
extern CMacroObject *g_MacroPointer;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
