/****************************************************************************
**
** GumpManager.h
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
#ifndef GumpManagerH
#define GumpManagerH
//--------------------------------------------------------------------------
//!Класс менеджера гампов
class TGumpManager : public TBaseQueue
{
private:
	/*!
	Вычислить количество статусбаров без пати
	@return Количество non-party статусбыров
	*/
	int GetNonpartyStatusbarsCount();

public:
	TGumpManager() : TBaseQueue() {}
	virtual ~TGumpManager() {}

	/*!
	Добавить гамп
	@param [__in] obj Ссылка на гамп
	@return 
	*/
	void AddGump(__in TGump *obj);

	/*!
	Обновить гамп
	@param [__in] serial Серийник гампа
	@param [__in] ID ID гампа
	@param [__in] Type Тип гампа
	@return Ссылку на обновленный гамп или NULL
	*/
	TGump *UpdateGump(__in DWORD serial, __in DWORD ID, __in GUMP_TYPE Type);

	/*!
	Найти гамп
	@param [__in] serial Серийник гампа
	@param [__in] ID ID гампа
	@param [__in] Type Тип гампа
	@return Ссылку на гамп или NULL
	*/
	TGump *GetGump(__in DWORD serial, __in DWORD ID, __in GUMP_TYPE Type);

	/*!
	Получить гамп-владелец текущей активной TEntryText
	@return Ссылку на гамп или NULL
	*/
	TGump *GetTextEntryOwner();

	/*!
	Проверить, существует ли гамп
	@param [__in] gumpID ID гампа (в памяти)
	@return 
	*/
	TGump *GumpExists(__in DWORD gumpID);

	/*!
	Закрыть все гампы с указанными параметрами
	@param [__in] serial Серийник гампа
	@param [__in] ID ID гампа
	@param [__in] Type Тип гампа
	@return 
	*/
	void CloseGump(__in DWORD serial, __in DWORD ID, __in GUMP_TYPE Type);

	/*!
	Удалить гамп
	@param [__in] obj Ссылка на гамп
	@return 
	*/
	void RemoveGump(__in TGump *obj);

	/*!
	Перерисовать все гампы
	@return 
	*/
	void RedrawAll();

	/*!
	Подготовка текстур
	@return 
	*/
	void PrepareTextures();

	/*!
	Рисование/выбор гампов
	@param [__in] mode true - рисование, false - выбор
	@param [__in] blocked Состояние экрана, для которого рисовать гампы
	@return 
	*/
	void Draw(__in bool &mode, __in bool blocked);

	void OnPopupHelp();



	/*!
	Нажатие левой кнопки мыши
	@param [__in] blocked Состояние экрана
	@return 
	*/
	void OnLeftMouseDown(__in bool blocked);

	/*!
	Отпускание левой кнопки мыши
	@param [__in] blocked Состояние экрана
	@return 
	*/
	bool OnLeftMouseUp(__in bool blocked);

	/*!
	Нажатие правой кнопки мыши
	@param [__in] blocked Состояние экрана
	@return 
	*/
	void OnRightMouseDown(__in bool blocked);

	/*!
	Отпускание правой кнопки мыши
	@param [__in] blocked Состояние экрана
	@return 
	*/
	void OnRightMouseUp(__in bool blocked);

	/*!
	Двойной клик левой кнопкой мыши
	@param [__in] blocked Состояние экрана
	@return true при успешной обработке
	*/
	bool OnLeftMouseDoubleClick(__in bool blocked);

	/*!
	Двойной клик правой кнопкой мыши
	@param [__in] blocked Состояние экрана
	@return true при успешной обработке
	*/
	bool OnRightMouseDoubleClick(__in bool blocked);

	/*!
	Обработка средней кнопки (колесика) мыши
	@param [__in] state Состояние колесика
	@param [__in] blocked Состояние экрана
	@return 
	*/
	void OnMouseWheel(__in MOUSE_WHEEL_STATE state, __in bool blocked);

	/*!
	Обработка нажатия клавиши
	@param [__in] wparam не подписанный параметр
	@param [__in] lparam не подписанный параметр
	@param [__in] blocked Состояние экрана
	@return true при успешной обработке
	*/
	bool OnCharPress(__in WPARAM wparam, __in LPARAM lparam, __in bool blocked);

	/*!
	Обработка нажатия клавиши
	@param [__in] wparam не подписанный параметр
	@param [__in] lparam не подписанный параметр
	@param [__in] blocked Состояние экрана
	@return true при успешной обработке
	*/
	bool OnKeyPress(__in WPARAM wparam, __in LPARAM lparam, __in bool blocked);

	/*!
	Событие удаления менеджера (вызывается перед удалением)
	@return 
	*/
	void OnDelete();

	/*!
	Удалить гампы, которые не могут быть досягаемы из-за изменения дистанции до объекта
	@return 
	*/
	void RemoveRangedGumps();

	/*!
	Загрузка гампов из конфига
	@param [__in] path Путь к файлу конфига
	@return 
	*/
	void Load(__in string path);

	/*!
	Сохранить гампы в конфиг
	@param [__in] path Путь к файлу кофнига
	@return 
	*/
	void Save(__in string path);
 };
//--------------------------------------------------------------------------
//!Ссылка на менеджер гампов
extern TGumpManager *GumpManager;
//--------------------------------------------------------------------------
#endif