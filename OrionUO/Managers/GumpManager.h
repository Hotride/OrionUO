/***********************************************************************************
**
** GumpManager.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPMANAGER_H
#define GUMPMANAGER_H
//----------------------------------------------------------------------------------
//!Класс менеджера гампов
class CGumpManager : public CBaseQueue
{
private:
	/*!
	Вычислить количество статусбаров без пати
	@return Количество non-party статусбыров
	*/
	int GetNonpartyStatusbarsCount();

	void SaveDefaultGumpProperties(WISP_FILE::CBinaryFileWritter &writer, CGump *gump, int size);

public:
	CGumpManager() : CBaseQueue() {}
	virtual ~CGumpManager() {}

	/*!
	Добавить гамп
	@param [__in] obj Ссылка на гамп
	@return 
	*/
	void AddGump(CGump *obj);

	/*!
	Обновить содержимое гампа
	@param [__in] serial Серийник гампа
	@param [__in] ID ID гампа
	@param [__in] Type Тип гампа
	@return Ссылку на обновленный гамп или NULL
	*/
	CGump *UpdateContent(int serial, int id, const GUMP_TYPE &type);

	/*!
	Обновить гамп
	@param [__in] serial Серийник гампа
	@param [__in] ID ID гампа
	@param [__in] Type Тип гампа
	@return Ссылку на обновленный гамп или NULL
	*/
	CGump *UpdateGump(int serial, int id, const GUMP_TYPE &type);

	/*!
	Найти гамп
	@param [__in] serial Серийник гампа
	@param [__in] ID ID гампа
	@param [__in] Type Тип гампа
	@return Ссылку на гамп или NULL
	*/
	CGump *GetGump(int serial, int id, const GUMP_TYPE &type);

	/*!
	Получить гамп-владелец текущей активной TEntryText
	@return Ссылку на гамп или NULL
	*/
	CGump *GetTextEntryOwner();

	/*!
	Проверить, существует ли гамп
	@param [__in] gumpID ID гампа (в памяти)
	@return 
	*/
	CGump *GumpExists(uintptr_t gumpID);

	/*!
	Закрыть все гампы с указанными параметрами
	@param [__in] serial Серийник гампа
	@param [__in] ID ID гампа
	@param [__in] Type Тип гампа
	@return 
	*/
	void CloseGump(uint serial, uint ID, GUMP_TYPE Type);

	/*!
	Удалить гамп
	@param [__in] obj Ссылка на гамп
	@return 
	*/
	void RemoveGump(CGump *obj);

	/*!
	Перерисовать все гампы
	@return 
	*/
	void RedrawAll();

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

	void PrepareContent();

	void RemoveMarked();

	/*!
	Подготовка текстур
	@return 
	*/
	void PrepareTextures();

	void Draw(bool blocked);

	void Select(bool blocked);

	void InitToolTip();



	void OnLeftMouseButtonDown(bool blocked);
	bool OnLeftMouseButtonUp(bool blocked);
	bool OnLeftMouseButtonDoubleClick(bool blocked);

	void OnRightMouseButtonDown(bool blocked);
	void OnRightMouseButtonUp(bool blocked);
	bool OnRightMouseButtonDoubleClick(bool blocked) { return false; }

	void OnMidMouseButtonScroll(bool up, bool blocked);

	void OnDragging(bool blocked);

	bool OnCharPress(const WPARAM &wParam, const LPARAM &lParam, bool blocked);
	bool OnKeyDown(const WPARAM &wParam, const LPARAM &lParam, bool blocked);

	void Load(const string &path);
	void Save(const string &path);
};
//----------------------------------------------------------------------------------
//!Ссылка на менеджер гампов
extern CGumpManager g_GumpManager;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
