/***********************************************************************************
**
** EffectManager.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef EFFECTMANAGER_H
#define EFFECTMANAGER_H
//----------------------------------------------------------------------------------
//!Класс менеджера эффектов
class CEffectManager : public CBaseQueue
{
public:
    CEffectManager();
    virtual ~CEffectManager() {}

    /*!
	Добавить эффект
	@param [__in] effect Ссылка на эффект
	@return 
	*/
    void AddEffect(CGameEffect *effect);

    /*!
	Удалить эффект
	@param [__in] effect Ссылка на эффект
	@return 
	*/
    void RemoveEffect(CGameEffect *effect);

    /*!
	Создать эффект взрыва
	@param [__in] effect Ссылка на эффект
	@return 
	*/
    void CreateExplodeEffect(CGameEffect *effect, const EFFECT_TYPE &type);

    /*!
	Обновление эффектов
	@return 
	*/
    void UpdateEffects();

    /*!
	Удаление эффектов, вышедших за пределы экрана
	@return 
	*/
    void RemoveRangedEffects();
};
//----------------------------------------------------------------------------------
//!Ссылка на менеджер эффектов
extern CEffectManager g_EffectManager;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
