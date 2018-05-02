/***********************************************************************************
**
** GameEffect.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GAMEEFFECT_H
#define GAMEEFFECT_H
//----------------------------------------------------------------------------------
//!Game effect class
class CGameEffect : public CRenderWorldObject
{
public:
    //!Effect type
    EFFECT_TYPE EffectType = EF_MOVING;

    //!Серийник назначения
    uint DestSerial = 0;
    //!Координата назначения по оси X
    ushort DestX = 0;
    //!Координата назначения по оси Y
    ushort DestY = 0;
    //!Координата назначения по оси Z
    char DestZ = 0;
    //!Скорость
    uchar Speed = 0;
    //!Время жизни
    uint Duration = 0;
    //!Изменять направление картинки при полете эффекта?
    bool FixedDirection = false;
    //!Взрывать по достижении цели?
    bool Explode = false;
    //!Режим отображения эффекта
    uint RenderMode = 0;

    //!Индекс картинки
    int AnimIndex = 0;
    //!Смещение от базового индекса
    int Increment = 0;
    //!Последнее время изменения картинки
    uint LastChangeFrameTime = 0;

    CGameEffect();
    virtual ~CGameEffect();

    /*!
	Отрисовать эффект
	@param [__in] mode Режим рисования. true - рисование, false - выбор объектов
	@param [__in] drawX Экранная координата X объекта
	@param [__in] drawY Экранная координата Y объекта
	@param [__in] ticks Таймер рендера
	@return При выборе объектов возвращает выбранный элемент
	*/
    virtual void Draw(int x, int y);

    /*!
	Получить текущий индекс картинки
	@return Индекс картинки
	*/
    ushort GetCurrentGraphic();

    /*!
	Вычислить текущий индекс картинки
	@return Индекс картинки
	*/
    ushort CalculateCurrentGraphic();

    /*!
	Применение режима отображения
	@return 
	*/
    void ApplyRenderMode();

    /*!
	Отмена режима отображения
	@return 
	*/
    void RemoveRenderMode();

    /*!
	Проверка, эффект ли это
	@return Эффект или нет
	*/
    bool IsEffectObject() { return true; }

    /*!
	Обновить эффект
	@return 
	*/
    virtual void Update(class CGameObject *parent);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
