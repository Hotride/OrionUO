/****************************************************************************
**
** MacroManager.h
**
** Copyright (C) September 2015 Hotride
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
*****************************************************************************
*/
//---------------------------------------------------------------------------
#ifndef MacroManagerH
#define MacroManagerH
//---------------------------------------------------------------------------
//!Класс менеджера макросов
class TMacroManager : public TBaseQueue
{
private:
	//!Время следующего выполнения
	DWORD m_NextTimer;

	//!Время ожидания таргета
	DWORD m_WaitForTargetTimer;

	//!Ожидание таргета от бинтов
	bool m_WaitingBandageTarget;

	//!Таблица скиллов дял использования
	static BYTE m_SkillIndexTable[24];

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
	WORD ConvertStringToKeyCode(__in const vector<string> &strings);

public:
	TMacroManager();
	virtual ~TMacroManager();

	SETGET(bool, WaitingBandageTarget);
	SETGET(DWORD, WaitForTargetTimer);

	/*!
	Поиск макроса
	@param [__in] key Индекс кнопки
	@param [__in] alt Зажатый альт
	@param [__in] ctrl Зажатый контрол
	@param [__in] shift Зажатый шифт
	@return Ссылку на макрос или NULL
	*/
	TMacro *FindMacro(__in WORD key, __in bool alt, __in bool ctrl, __in bool shift);

	/*!
	Сконвертировать файл макросов оригинального клиента
	@param [__in] path Путь к файлу с макросами
	@return true при успешном конвертировании
	*/
	bool Convert(__in string path);

	/*!
	Загрузить макросы из конфига
	@param [__in] path Путь к файлу конфига
	@param [__in] originalPath Путь к файлу с макросами оригинального клиента
	@return 
	*/
	bool Load(__in string path, __in string originalPath);

	/*!
	Сохранить макросы в конфиг
	@param [__in] path Путь к файлу конфига
	@return 
	*/
	void Save(__in string path);

	/*!
	Загрузить макросы из опций
	@return 
	*/
	void LoadFromOptions();

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
};
//---------------------------------------------------------------------------
//!Ссылка на менеджер макросов
extern TMacroManager *MacroManager;
//---------------------------------------------------------------------------
#endif