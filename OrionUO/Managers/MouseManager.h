/***********************************************************************************
**
** ColorManager.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef MOUSEMANAGER_H
#define MOUSEMANAGER_H
//----------------------------------------------------------------------------------
//!Картинки курсора мышки
extern ushort g_CursorData[2][16];
//----------------------------------------------------------------------------------
//!Класс менеджера мыши
class CMouseManager : public WISP_MOUSE::CMouse
{
private:
    //!Смещение картинки относительно курсора
    int m_CursorOffset[2][16];

public:
    CMouseManager()
        : WISP_MOUSE::CMouse()
    {
        memset(&m_CursorOffset[0][0], 0, sizeof(m_CursorOffset));
    }
    ~CMouseManager() {}

    /*!
	Загрузка текстур курсора
	@return true при успешной загрузке
	*/
    bool LoadCursorTextures();

    /*!
	Конвертирование значения направления
	@param [__in] val Значение направления
	@return Смещение
	*/
    int Sgn(int val);

    /*!
	Получить направление курсора относительно центра игрового окна
	@param [__in] x1 Координата X центра
	@param [__in] y1 Координата Y центра
	@param [__in] to_x Координата X мыши
	@param [__in] to_y Координата Y мыши
	@param [__in] current_facing Текущее направление
	@return Направление курсора
	*/
    int GetFacing(int x1, int y1, int to_x, int to_y, int current_facing);

    /*!
	Получить индекс картинки игрового курсора
	@return Индекс картинки
	*/
    ushort GetGameCursor();

    /*!
	Обработка движения персонажа
	@return 
	*/
    void ProcessWalking();

    /*!
	Отрисовать курсор
	@param [__in] id Индекс картинки курсора
	@return 
	*/
    void Draw(ushort id);
};
//--------------------------------------------------------------------------
//!Менеджер мыши
extern CMouseManager g_MouseManager;
//--------------------------------------------------------------------------
#endif