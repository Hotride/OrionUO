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
class TGameObject : public TRenderStaticObject
{
protected:
	//Серийник контейнера, содержащего объект (0xFFFFFFFF - объект лежит в мире)
	DWORD m_Container;

	//Карта объекта
	BYTE m_MapIndex;

	//Количество
	DWORD m_Count;

	//Флаги от сервера
	BYTE m_Flags;

	//Имя
	string m_Name;

	//НПС или предмет
	bool m_NPC;

	//На предмет кликнули
	bool m_Clicked;

	//Текущий индекс анимации
	char m_AnimIndex;

	//Время последнего изменения анимации
	DWORD m_LastAnimationChangeTime;

	//Ссылка на список эффектов персонажа
	TGameEffect *m_Effects;

	//Указатель на текстуру дл яопции Object Handles
	TTextureObject m_TextureObjectHalndes;

	//Создать текстуру для опции Object Handles
	void GenerateObjectHandlesTexture(wstring text);

public:
	TGameObject(DWORD serial = 0);
	virtual ~TGameObject();

	//Добавить текст в контейнер
	void AddText(TTextData *td);

	SETGET(DWORD, Container);
	SETGET(BYTE, MapIndex);
	SETGET(DWORD, Count);
	SETGET(BYTE, Flags);
	virtual SETGET(string, Name);
	SETGET(bool, NPC);
	SETGET(bool, Clicked);
	SETGETEX(char, AnimIndex);
	SETGET(DWORD, LastAnimationChangeTime);

	//Отрисовать текстуру опции Object Handles (автоматически создает текстуру, если она не была создана)
	void DrawObjectHandlesTexture(int &x, int &y);

	//Получить индекс анимации
	virtual WORD GetMountAnimation();

	//Событие, вызываемое при изменении индекса картинки
	virtual void OnGraphicChange(int direction = 0) {}

	//Проверка прозрачности (для круга прозрачности)
	virtual bool TranparentTest(int &playerZ) { return false; }

	//Управление эффектами
	void DrawEffects(int &drawX, int &drawY, DWORD &ticks);
	void AddEffect(TGameEffect *effect); //Добавить эффект
	void RemoveEffect(TGameEffect *effect); //Удалить эффект

	//Управление объектами внутри текущего объекта
	void AddObject(TGameObject *obj); //Добавить объект в список объектов текущего контейнера
	void AddItem(TGameObject *obj); //Добавить объект в контейнер (this - контейнер)
	void Reject(TGameObject *obj); //Изъять объект из контейнера
	//void MoveToTop(TGameObject *obj); //Наверх отрисовки (в конец списка)

	void Clear(); //Очистить контейнер

	//Получение значений состояния в зависимости от флагов
	bool Poisoned() {return (m_Flags & 0x04);}
	bool YellowHits() {return (m_Flags & 0x08);}
	bool Locked() {return !(m_Flags & 0x20);}
	bool InWarMode() {return (m_Flags & 0x40);}
	bool Hidden() {return (m_Flags & 0x80);}

	//Виртуальные указатели
	virtual bool IsHuman() {return false;}
	virtual bool IsPlayer() {return false;}
	
	int IsGold();
	WORD GetDrawGraphic(bool &doubleDraw);

	bool IsGameObject() {return true;}
	bool IsCorpse() {return (m_Graphic == 0x2006);}

	//Найти объект в мире, в котором содержится контейнер
	TGameObject *GetTopObject();
};
//---------------------------------------------------------------------------
//Функции для вычисления дистанции
int GetDistance(TGameObject *current, TGameObject *target);
int GetDistance(TGameObject *current, POINT target);
int GetMultiDistance(POINT current, TGameObject *target);
int GetDistance(POINT current, TGameObject *target);
int GetDistance(POINT current, POINT target);
int GetTopObjDistance(TGameObject *current, TGameObject *target);
//---------------------------------------------------------------------------
#endif