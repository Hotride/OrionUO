/***********************************************************************************
**
** MacroManager.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef MACROMANAGER_H
#define MACROMANAGER_H
//----------------------------------------------------------------------------------
//!Класс менеджера макросов
class CMacroManager : public CBaseQueue
{
	//!Ожидание таргета от бинтов
	SETGET(bool, WaitingBandageTarget, false);
	//!Время ожидания таргета
	SETGET(uint, WaitForTargetTimer, 0);
	SETGET(bool, SendNotificationToPlugin, false);

private:
	//!Время следующего выполнения
	uint m_NextTimer{ 0 };

	//!Таблица скиллов дял использования
	static uchar m_SkillIndexTable[24];

	/*!
	Выполнить команды подменю
	@return 
	*/
	void ProcessSubMenu();

	/*!
	Конвертирование строки в виртуальный код клавиши
	@param [__in] strings Исходные строки, при склейке получим входную строку
	@return Ключ
	*/
	ushort ConvertStringToKeyCode(const STRING_LIST &strings);

public:
	CMacroManager();
	virtual ~CMacroManager();

	/*!
	Поиск макроса
	@param [__in] key Индекс кнопки
	@param [__in] alt Зажатый альт
	@param [__in] ctrl Зажатый контрол
	@param [__in] shift Зажатый шифт
	@return Ссылку на макрос или NULL
	*/
	class CMacro *FindMacro(const ushort &key, const bool &alt, const bool &ctrl, const bool &shift);

	/*!
	Сконвертировать файл макросов оригинального клиента
	@param [__in] path Путь к файлу с макросами
	@return true при успешном конвертировании
	*/
	bool Convert(const string &path);

	/*!
	Загрузить макросы из конфига
	@param [__in] path Путь к файлу конфига
	@param [__in] originalPath Путь к файлу с макросами оригинального клиента
	@return 
	*/
	bool Load(const string &path, const string &originalPath);

	/*!
	Сохранить макросы в конфиг
	@param [__in] path Путь к файлу конфига
	@return 
	*/
	void Save(const string &path);

	/*!
	Загрузить макросы из опций
	@return 
	*/
	void LoadFromOptions();

	void ChangePointer(CMacroObject *macro);

	/*!
	Начать выполнение макроса
	@return 
	*/
	void Execute();

	/*!
	Выполнить действие макроса (или набор действий)
	@return Код выполнения
	*/
	MACRO_RETURN_CODE Process();

	MACRO_RETURN_CODE Process(CMacroObject *macro);
};
//----------------------------------------------------------------------------------
//!Ссылка на менеджер макросов
extern CMacroManager g_MacroManager;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
