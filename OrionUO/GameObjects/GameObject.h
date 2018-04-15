/***********************************************************************************
**
** GameObject.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
//----------------------------------------------------------------------------------
struct DRAW_FRAME_INFORMATION
{
	int OffsetX;
	int OffsetY;
	int Width;
	int Height;
};
//----------------------------------------------------------------------------------
//!Класс игрового объекта
class CGameObject : public CRenderStaticObject
{
public:
	//!Серийник контейнера, содержащего объект (0xFFFFFFFF - объект лежит в мире)
	uint Container = 0xFFFFFFFF;
	//!Карта объекта
	uchar MapIndex = 0;
	//!Количество
	uint Count = 0;
	//!Флаги от сервера
protected:
    uchar m_Flags = 0;
public:
    uchar GetFlags() { return m_Flags; };
    void SetFlags(const uchar& val);
	//!Имя
protected:
    string m_Name = "";
public:
    string GetName() { return m_Name; };
    void SetName(const string& val);
	//!НПС или предмет
	bool NPC = false;
	//!На предмет кликнули
	bool Clicked = false;
	//!Текущий индекс анимации
	char AnimIndex = 0;
	//!Префикс You see: в журнале
	string JournalPrefix = "";
	//!Время последнего изменения анимации
	uint LastAnimationChangeTime = 0;
	bool SA_Poisoned = false;
	bool ClosedObjectHandle = false;

protected:
	//!Ссылка на список эффектов персонажа
	class CGameEffect *m_Effects{ NULL };

	//!Указатель на текстуру для опции Object Handles
	CGLTexture m_TextureObjectHalndes{ CGLTexture() };

public:
	CGameObject(const uint &serial = 0);
	virtual ~CGameObject();

	//!Информация о ФБ объекта (для Animation Manager)
	DRAW_FRAME_INFORMATION m_FrameInfo;

	vector<class CGameItem*> m_DrawLayeredObjects;

	/*!
	Добавить текст в контейнер
	@param [__in] td Ссылка на объект текста
	@return
	*/
	virtual void AddText(CTextData *td);

	/*!
	Создать текстуру для опции Object Handles
	@param [__in] text Текст названия объекта
	@return
	*/
	void GenerateObjectHandlesTexture(wstring text);

	/*!
	Отрисовать текстуру опции Object Handles (автоматически создает текстуру, если она не была создана)
	@param [__in] mode Режим отображения
	@param [__in] x Экранная координата X
	@param [__in] y Экранная координата Y
	@return
	*/
	void DrawObjectHandlesTexture();

	void SelectObjectHandlesTexture();

	/*!
	Получить индекс анимации
	@return Индекс анимации
	*/
	virtual ushort GetMountAnimation();

	/*!
	Событие, вызываемое при изменении индекса картинки
	@param [__in_opt] direction Направление объекта
	@return
	*/
	virtual void OnGraphicChange(int direction = 0) {}

	/*!
	Проверка прозрачности (для круга прозрачности)
	@param [__in] playerZ Координата Z игрока
	@return Может ли быть прозрачным
	*/
	virtual bool TranparentTest(const int &playerZ) { return false; }

	/*!
	Отрисовать эффект
	@param [__in] drawX Экранная координата X
	@param [__in] drawY Экранная координата Y
	@param [__in] ticks Таймер рендера
	@return
	*/
	void DrawEffects(int x, int y);

	void UpdateEffects();

	/*!
	Добавить эффект
	@param [__in] effect Ссылка на эффект
	@return
	*/
	void AddEffect(class CGameEffect *effect);

	/*!
	Удалить эффект
	@param [__in] effect Ссылка на эффект
	@return
	*/
	void RemoveEffect(class CGameEffect *effect);

	/*!
	Добавить объект в список объектов текущего контейнера
	@param [__in] obj Ссылка на объект
	@return
	*/
	void AddObject(CGameObject *obj);

	/*!
	Добавить объект в контейнер (this - контейнер)
	@param [__in] obj Ссылка на объект
	@return
	*/
	void AddItem(CGameObject *obj);

	/*!
	Изъять объект из контейнера
	@param [__in] obj Ссылка на объект
	@return
	*/
	void Reject(CGameObject *obj);

	/*!
	Очистить контейнер
	@return
	*/
	void Clear();

	void ClearUnequipped();

	void ClearNotOpenedItems();

	/*!
	Заморожен ли объект
	@return
	*/
	bool Frozen() { return (m_Flags & 0x01); }

	/*!
	Отравлен ли объект
	@return
	*/
	bool Poisoned();

	/*!
	Летит ли объект
	@return
	*/
	bool Flying();

	/*!
	Объект имеет желтую полоску жизней
	@return
	*/
	bool YellowHits() { return (m_Flags & 0x08); }

	/*!
	Объект игнорирует персонажей при перемещении
	@return
	*/
	bool IgnoreCharacters() { return (m_Flags & 0x10); }

	/*!
	Можно ли брать объект в руку
	@return
	*/
	bool Locked() { return (!(m_Flags & 0x20) && m_TiledataPtr->Weight > 90); }

	/*!
	Объект в режиме боя
	@return
	*/
	bool InWarMode() { return (m_Flags & 0x40); }

	/*!
	Объект спрятан
	@return
	*/
	bool Hidden() { return (m_Flags & 0x80); }

	/*!
	Проверка на человекоподобного персонажа
	@return Человекоподобное или нет
	*/
	virtual bool IsHuman() { return false; }

	/*!
	Проверка на игрока (персонаж - игрок человека за компьютером)
	@return Игрок или нет
	*/
	virtual bool IsPlayer() { return false; }

	/*!
	Золото ли это
	@return Индекс в таблице золота
	*/
	static int IsGold(const ushort &graphic);

	/*!
	Получить индекс картинки для рисования
	@param [__out] doubleDraw Двойная отрисовка объекта
	@return Индекс картинки
	*/
	ushort GetDrawGraphic(bool &doubleDraw);

	/*!
	Это игровой объект
	@return
	*/
	bool IsGameObject() { return true; }

	/*!
	Это труп
	@return
	*/
	bool IsCorpse() { return (Graphic == 0x2006); }

	/*!
	Найти объект в мире, в котором содержится контейнер
	@return Ссылка на объект в мире
	*/
	CGameObject *GetTopObject();

	CGameItem *FindLayer(const int &layer);

	virtual CGameItem *FindSecureTradeBox() { return NULL; }
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
