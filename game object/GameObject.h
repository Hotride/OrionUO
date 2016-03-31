/****************************************************************************
**
** GameObject.h
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
#ifndef GameObjectH
#define GameObjectH
//--------------------------------------------------------------------------
//!Класс игрового объекта
class TGameObject : public TRenderStaticObject
{
protected:
	//!Серийник контейнера, содержащего объект (0xFFFFFFFF - объект лежит в мире)
	DWORD m_Container;

	//!Карта объекта
	BYTE m_MapIndex;

	//!Количество
	DWORD m_Count;

	//!Флаги от сервера
	BYTE m_Flags;

	//!Имя
	string m_Name;

	//!НПС или предмет
	bool m_NPC;

	//!На предмет кликнули
	bool m_Clicked;

	//!Текущий индекс анимации
	char m_AnimIndex;

	//!Время последнего изменения анимации
	DWORD m_LastAnimationChangeTime;

	//!Ссылка на список эффектов персонажа
	TGameEffect *m_Effects;

	//!Указатель на текстуру дл яопции Object Handles
	TTextureObject m_TextureObjectHalndes;

	/*!
	Создать текстуру для опции Object Handles
	@param [__in] text Текст названия объекта
	@return 
	*/
	void GenerateObjectHandlesTexture(__in wstring text);

public:
	TGameObject(DWORD serial = 0);
	virtual ~TGameObject();

	SETGET(DWORD, Container);
	SETGET(BYTE, MapIndex);
	SETGET(DWORD, Count);
	SETGET(BYTE, Flags);
	virtual SETGET(string, Name);
	SETGET(bool, NPC);
	SETGET(bool, Clicked);
	SETGETEX(char, AnimIndex);
	SETGET(DWORD, LastAnimationChangeTime);

	/*!
	Добавить текст в контейнер
	@param [__in] td Ссылка на объект текста
	@return 
	*/
	void AddText(__in TTextData *td);

	/*!
	Отрисовать текстуру опции Object Handles (автоматически создает текстуру, если она не была создана)
	@param [__in] x Экранная координата X
	@param [__in] y Экранная координата Y
	@return 
	*/
	void DrawObjectHandlesTexture(__in int &x, __in int &y);

	/*!
	Получить индекс анимации
	@return Индекс анимации
	*/
	virtual WORD GetMountAnimation();
	
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
	void DrawEffects(__in int &drawX, __in int &drawY, __in DWORD &ticks);

	/*!
	Добавить эффект
	@param [__in] effect Ссылка на эффект
	@return 
	*/
	void AddEffect(__in TGameEffect *effect);

	/*!
	Удалить эффект
	@param [__in] effect Ссылка на эффект
	@return 
	*/
	void RemoveEffect(__in TGameEffect *effect);

	/*!
	Добавить объект в список объектов текущего контейнера
	@param [__in] obj Ссылка на объект
	@return 
	*/
	void AddObject(__in TGameObject *obj); 

	/*!
	Добавить объект в контейнер (this - контейнер)
	@param [__in] obj Ссылка на объект
	@return 
	*/
	void AddItem(__in TGameObject *obj);

	/*!
	Изъять объект из контейнера
	@param [__in] obj Ссылка на объект
	@return 
	*/
	void Reject(__in TGameObject *obj);

	/*!
	Очистить контейнер
	@return 
	*/
	void Clear();

	/*!
	Отравлен ли объект
	@return
	*/
	bool Poisoned() {return (m_Flags & 0x04);}

	/*!
	Объект имеет желтую полоску жизней
	@return
	*/
	bool YellowHits() {return (m_Flags & 0x08);}

	/*!
	Можно ли брать объект в руку
	@return
	*/
	bool Locked() {return !(m_Flags & 0x20);}

	/*!
	Объект в режиме боя
	@return
	*/
	bool InWarMode() {return (m_Flags & 0x40);}

	/*!
	Объект спрятан
	@return
	*/
	bool Hidden() {return (m_Flags & 0x80);}

	/*!
	Проверка на человекоподобного персонажа
	@return Человекоподобное или нет
	*/
	virtual bool IsHuman() {return false;}

	/*!
	Проверка на игрока (персонаж - игрок человека за компьютером)
	@return Игрок или нет
	*/
	virtual bool IsPlayer() {return false;}

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
	WORD GetDrawGraphic(__out bool &doubleDraw);

	/*!
	Это игровой объект
	@return 
	*/
	bool IsGameObject() {return true;}

	/*!
	Это труп
	@return
	*/
	bool IsCorpse() {return (m_Graphic == 0x2006);}

	/*!
	Найти объект в мире, в котором содержится контейнер
	@return Ссылка на объект в мире
	*/
	TGameObject *GetTopObject();
};
//---------------------------------------------------------------------------
#endif