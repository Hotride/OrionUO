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
class TGameObject : public TRenderWorldObject
{
//private:
protected:
	DWORD m_Container; //Серийник контейнера, содержащего объект (0xFFFFFFFF - объект лежит в мире)
	BYTE m_MapIndex; //Карта объекта
	WORD m_Count; //Количество
	BYTE m_Flags; //Флаги от сервера
	string m_Name; //Имя
	bool m_NPC; //НПС или предмет
	bool m_Clicked; //На предмет кликнули
	char m_AnimIndex; //Текущий индекс анимации
	DWORD m_LastAnimationChangeTime; //Время последнего изменения анимации

	TGameEffect *m_Effects; //Ссылка на список эффектов персонажа
public:
	TGameObject(DWORD serial = 0);
	virtual ~TGameObject();
	
	//Добавить текст в контейнер
	void AddText(TTextData *td);

	SETGET(DWORD, Container);
	SETGET(BYTE, MapIndex);
	SETGET(WORD, Count);
	SETGET(BYTE, Flags);
	virtual SETGET(string, Name);
	SETGET(bool, NPC);
	SETGET(bool, Clicked);
	SETGETEX(char, AnimIndex);
	SETGET(DWORD, LastAnimationChangeTime);

	virtual WORD GetMountAnimation() {return Graphic;}

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

	bool IsGameObject() {return true;}
	bool IsCorpse() {return (Graphic == 0x2006);}

	//Найти объект в мире, в котором содержится контейнер
	TGameObject *GetTopObject();
};
//---------------------------------------------------------------------------
//Функции для вычисления дистанции
int GetDistance(TGameObject *current, TGameObject *target);
int GetDistance(TGameObject *current, POINT target);
int GetDistance(POINT current, TGameObject *target);
int GetDistance(POINT current, POINT target);
int GetTopObjDistance(TGameObject *current, TGameObject *target);
//---------------------------------------------------------------------------
#endif