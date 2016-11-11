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
#include "RenderStaticObject.h"
#include "../GLEngine/GLEngine.h"
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
	//!Серийник контейнера, содержащего объект (0xFFFFFFFF - объект лежит в мире)
	SETGET(uint, Container);
	//!Карта объекта
	SETGET(uchar, MapIndex);
	//!Количество
	SETGET(uint, Count);
	//!Флаги от сервера
	SETGETE(uchar, Flags, OnChangedFlags);
	//!Имя
	SETGETE(string, Name, OnChangeName);
	//!НПС или предмет
	SETGET(bool, NPC);
	//!На предмет кликнули
	SETGET(bool, Clicked);
	//!Текущий индекс анимации
	SETGETEX(char, AnimIndex);
	//!Префикс You see: в журнале
	SETGET(bool, YouSeeJournalPrefix);
	//!Время последнего изменения анимации
	SETGET(uint, LastAnimationChangeTime);
	SETGET(wstring, ClilocMessage);

protected:
	//!Ссылка на список эффектов персонажа
	class CGameEffect *m_Effects;

	//!Указатель на текстуру для опции Object Handles
	CGLTexture m_TextureObjectHalndes;

	/*!
	Создать текстуру для опции Object Handles
	@param [__in] text Текст названия объекта
	@return
	*/
	void GenerateObjectHandlesTexture(wstring text);

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
	Отрисовать текстуру опции Object Handles (автоматически создает текстуру, если она не была создана)
	@param [__in] mode Режим отображения
	@param [__in] x Экранная координата X
	@param [__in] y Экранная координата Y
	@return
	*/
	void DrawObjectHandlesTexture(const int &x, const int &y);

	void SelectObjectHandlesTexture(const int &x, const int &y);

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
	virtual bool TranparentTest(int &playerZ) { return false; }

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
	bool Locked() { return (!(m_Flags & 0x20) && !IsContainer()); }

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
	int IsGold();

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
	bool IsCorpse() { return (m_Graphic == 0x2006); }

	/*!
	Найти объект в мире, в котором содержится контейнер
	@return Ссылка на объект в мире
	*/
	CGameObject *GetTopObject();

	CGameItem *FindLayer(const int &layer);

	CGameItem *FindSecureTradeBox();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
