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
//Менеджер гампов
class TGumpManager : public IGumpManager, public TBaseQueue
{
private:
	//Вычислить количество статусбаров без пати
	int GetNonpartyStatusbarsCount();
public:
	TGumpManager() : TBaseQueue() {}
	virtual ~TGumpManager() {}

	//Добавить гамп
	void AddGump(TGump *obj);

	//Обновить гамп
	TGump *UpdateGump(DWORD serial, DWORD ID, GUMP_TYPE Type);

	//Получить гамп
	TGump *GetGump(DWORD serial, DWORD ID, GUMP_TYPE Type);

	//Получить гамп-владелец текущей активной TEntryText
	TGump *GetTextEntryOwner();

	//Проверить, существует ли гамп
	TGump *GumpExists(DWORD gumpID);

	//Закрыть все гампы с указанными параметрами
	void CloseGump(DWORD serial, DWORD ID, GUMP_TYPE Type);

	//Удалить гамп
	void RemoveGump(TGump *obj);

	//Перерисовать все гампы
	void RedrawAll();

	//Подготовка текстур
	void PrepareTextures();

	//Рисование гампов
	void Draw(bool &mode, bool blocked);

	//События
	void OnLeftMouseDown(bool blocked);
	bool OnLeftMouseUp(bool blocked);
	void OnRightMouseDown(bool blocked);
	void OnRightMouseUp(bool blocked);
	bool OnLeftMouseDoubleClick(bool blocked);
	bool OnRightMouseDoubleClick(bool blocked);
	void OnMouseWheel(MOUSE_WHEEL_STATE state, bool blocked);
	bool OnCharPress(WPARAM wparam, LPARAM lparam, bool blocked);
	bool OnKeyPress(WPARAM wparam, LPARAM lparam, bool blocked);

	//Событие удаления менеджера (вызывается перед удалением)
	void OnDelete();

	//Удалить гампы, которые не могут быть досягаемы из-за изменения дистанции до объекта
	void RemoveRangedGumps();

	//Загрузка гампов из конфига
	void Load(string path);

	//Сохранить гампы в конфиг
	void Save(string path);
};
//--------------------------------------------------------------------------
extern TGumpManager *GumpManager;
//--------------------------------------------------------------------------
#endif