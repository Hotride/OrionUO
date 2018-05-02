/***********************************************************************************
**
** GameItem.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GAMEITEM_H
#define GAMEITEM_H
//----------------------------------------------------------------------------------
//!Класс игрового предмета (или трупа)
class CGameItem : public CGameObject
{
public:
    //!Слой, в котором расположен объект
    uchar Layer = 0;
    //!Индекс картинки анимации
    ushort AnimID = 0;
    //!Используемый предметом слой (из tiledata.mul/verdata)
    uchar UsedLayer = 0;
    //!Открыт ли контейнер/спеллбук
    bool Opened = false;
    //!Выведен ли гамп разделения количества?
    bool Dragged = false;
    //!Это прокси-объект мульти
    bool MultiBody = false;
    //!Необходимо обновить мульти-объект
    bool WantUpdateMulti = true;
    //!Цвет тайла для замены филдов (если 0 - не филд)
    ushort FieldColor = 0;
    //!Бонус дистанции для мульти-объекта
    short MultiDistanceBonus = 0;
    //!Цена для магазина
    uint Price = 0;
    //!Имя из клилока (для гампа магазина)
    bool NameFromCliloc = false;
    //!Тайл мульти для отображения
    ushort MultiTileGraphic = 0;
    //!Индекс источника света
    uchar LightID = 0;

protected:
    /*!
	Добавить объекта-мульти
	@param [__in] obj Ссылка на мульти-объект
	@return 
	*/
    void AddMultiObject(CMultiObject *obj);

public:
    CGameItem(int serial = 0);
    virtual ~CGameItem();

    void CalculateFieldColor();

    CMulti *GetMulti();

    void ClearMultiItems();

    virtual void Draw(int x, int y);

    virtual void Select(int x, int y);

    void ClearCustomHouseMultis(int state);

    CMultiObject *
    AddMulti(ushort graphic, ushort color, char x, char y, char z, bool isCustomHouseMulti);

    /*!
	Событие изменения картинки объекта
	@param [__in_opt] direction Направление предмета (для трупов)
	@return
	*/
    void OnGraphicChange(int direction = 0);

    /*!
	Получить индекс картинки (для анимации)
	@return Индекс картинки
	*/
    ushort GetMountAnimation();

    /*!
	Внутренний ли это объект, исключая трупы
	@return 
	*/
    virtual bool IsInternal() { return ((m_TiledataPtr->Flags & 0x00010000) && !IsCorpse()); }

    /*!
	Человекоподобный труп ли это?
	@return 
	*/
    bool IsHuman()
    {
        return (
            IsCorpse() && (IN_RANGE(Count, 0x0192, 0x0193) || IN_RANGE(Count, 0x025F, 0x0260) ||
                           IN_RANGE(Count, 0x02B6, 0x02B7)));
    }

    /*!
	Загрузка мульти в текущий объект
	@return 
	*/
    void LoadMulti(bool dropAlpha);

    /*!
	Получение объекта мульти в заданных координатах
	@param [__in] x Координата X
	@param [__in] y Координата Y
	@return Ссылка на мульти или NULL
	*/
    CMulti *GetMultiAtXY(short x, short y);

    /*!
	Найти объект внутри (рекурсивно) по типу с учетом (и без) цвета
	@param [__in] graphic Индекс картинки
	@param [__in_opt] color Цвет предмета
	@return Ссылка на найденный объект или NULL
	*/
    CGameItem *FindItem(ushort graphic, ushort color = 0xFFFF);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
