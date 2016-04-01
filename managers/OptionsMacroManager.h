/****************************************************************************
**
** OptionsMacroManager.h
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
#ifndef OptionsMacroManagerH
#define OptionsMacroManagerH
//---------------------------------------------------------------------------
//!Класс менеджера макросов для опций
class TOptionsMacroManager : public TBaseQueue
{
public:
	TOptionsMacroManager();
	virtual ~TOptionsMacroManager();

	/*!
	Загрузить из списка макросов
	@return 
	*/
	void LoadFromMacro();

	/*!
	Проверка на активный TEntryText
	@return true в случае успешной проверки
	*/
	bool EntryPointerHere();
};
//---------------------------------------------------------------------------
//!Ссылка на менеджер макросов для опций
extern TOptionsMacroManager *OptionsMacroManager;
//---------------------------------------------------------------------------
#endif