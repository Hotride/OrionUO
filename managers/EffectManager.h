/****************************************************************************
**
** EffectManager.h
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
//--------------------------------------------------------------------------
#ifndef EffectManagerH
#define EffectManagerH
//--------------------------------------------------------------------------
//Менеджер эффектов
class TEffectManager : public TBaseQueue
{
private:
public:
	TEffectManager() : TBaseQueue() {}
	virtual ~TEffectManager() {}

	//Добавить эффект
	void AddEffect(TGameEffect *effect);

	//Удалить эффект
	void RemoveEffect(TGameEffect *effect);

	//Создать эффект взрыва
	void CreateExplodeEffect(TGameEffect *effect);

	//Обновление эффектов
	void UpdateEffects();

	//Удаление эффектов, вышедших за пределы экрана
	void RemoveRangedEffects();
};
//--------------------------------------------------------------------------
extern TEffectManager *EffectManager;
//--------------------------------------------------------------------------
#endif