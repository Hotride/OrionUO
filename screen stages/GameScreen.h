/****************************************************************************
**
** GameScreen.h
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
#ifndef GameScreenH
#define GameScreenH
//---------------------------------------------------------------------------
//!Структура с данными о параметрах и возможностях игрового окна
struct RENDER_VARIABLES_FOR_GAME_WINDOW
{
	//!Положение окна
	int GameWindowPosX;
	int GameWindowPosY;

	//!Габариты окна
	int GameWindowSizeX;
	int GameWindowSizeY;

	//!Центр окна
	int GameWindowCenterX;
	int GameWindowCenterY;

	//!Дистанции отображения
	int RealMinRangeX;
	int RealMaxRangeX;
	int RealMinRangeY;
	int RealMaxRangeY;

	//!Индексы блоков для отображения
	int MinBlockX;
	int MinBlockY;
	int MaxBlockX;
	int MaxBlockY;

	//!Координаты пикселей для отображения
	int MinPixelsX;
	int MaxPixelsX;
	int MinPixelsY;
	int MaxPixelsY;

	//!Координаты игрока
	int PlayerX;
	int PlayerY;
	int PlayerZ;
};
//---------------------------------------------------------------------------
//!Структура данных о источнике света
struct LIGHT_DATA
{
	//!Индекс света
	BYTE ID;

	//!Цвет света
	WORD Color;

	//!Экранные координаты
	int DrawX;
	int DrawY;
};
//---------------------------------------------------------------------------
//!Структура данных с информацией для рендера
struct RENDER_OBJECT_DATA
{
	//!Ссылка на объект рендера
	TRenderWorldObject *Obj;

	//!Цвет объекта, вышедшего за рамки видимости клиента (если включена опция)
	WORD GrayColor;

	//!Экранные координаты объекта
	int X;
	int Y;
};
//---------------------------------------------------------------------------
//!Структура данных с информацией для гампа с именем объекта (Object Handles)
struct OBJECT_HANDLES_DATA
{
	//!Ссылка на объект рендера
	TGameObject *Obj;

	//!Экранные координаты объекта
	int X;
	int Y;
};
//---------------------------------------------------------------------------
class TGameScreen : public TBaseScreen
{
private:
	//!Структура с данными о параметрах и возможностях игрового окна
	RENDER_VARIABLES_FOR_GAME_WINDOW m_RenderBounds;

	//!Возможность перемещения игрового окна
	bool m_GameWindowMoving;

	//!Возможность изменения размера игрового окна
	bool m_GameWindowResizing;

	//!Список объектов для вывода
	OBJECT_HANDLES_DATA m_ObjectHandlesList[MAX_OBJECT_HANDLES];

	//!Количество объектов для вывода
	int m_ObjectHandlesCount;

	//!Список объектов для отображения
	RENDER_OBJECT_DATA *m_RenderList;

	//!Размер списка объектов рендера
	int m_RenderListSize;

	//!Количество объектов в списке
	int m_RenderListCount;

	//!Флаг, определяющий инициализацию списка рендера
	bool m_RenderListInitalized;

	//!Список источников света
	LIGHT_DATA m_Light[MAX_LIGHT_SOURCES];

	//!Количество источников света
	int m_LightCount;

	//!Использовать ли освещение при перерисовке текущего кадра
	bool m_UseLight;

	//!Максимальное значение отображения по Z координате
	int m_MaxDrawZ;

	//!Указатель на элемент текста
	TRenderTextObject *m_GameWindowText;

	/*!
	Вычисление значений Z координаты для рендера
	@param [__out] noDrawRoof Не рисовать крыши и верхние этажи строений
	@param [__out] maxGroundZ Максимальная Z координата ландшафта
	@return Максимальная Z координата отрисовки
	*/
	int GetMaxDrawZ(__out bool &noDrawRoof, __out char &maxGroundZ);

	/*!
	Обработка сообщений мыши (стягивание статуса. драг-гампа и т.п.)
	@param [__out] charSelected Состояние выделения персонажа
	@return 
	*/
	void CheckMouseEvents(__out bool &charSelected);

	/*!
	Вычисление параметров отображаемого текста
	 @param [__in] mode true - отрисовка,
	 @return 
	*/
	void CalculateGameWindowText(__in bool &mode);

	/*!
	Рисование игрового окна
	@param [__in] mode true - отрисовка, false - выбор
	@return 
	*/
	void DrawGameWindow(__in bool &mode);

	/*!
	Отображение источников света
	@return 
	*/
	void DrawGameWindowLight();

	/*!
	Отображение текста над объектами мира
	@param [__in] mode true - отрисовка, false - выбор
	@return 
	*/
	void DrawGameWindowText(__in bool &mode);

	/*!
	Применение прозрачности крон деревьев в указанных координатах
	@param [__in] graphic ндекс картинки дерева
	@param [__in] x Координата X дерева
	@param [__in] y Координата Y дерева
	@param [__in] z Координата Z дерева
	@return 
	*/
	void ApplyTransparentFoliageToUnion(__in WORD &graphic, __in int &x, __in int &y, __in int &z);

	/*!
	Проверка принадлежности кроны к группе крон (с последующим применением прозрачности всей группе)
	@param [__in] graphic Индекс картинки дерева
	@param [__in] x Координата X дерева
	@param [__in] y Координата Y дерева
	@param [__in] z Координата Z дерева
	@return 
	*/
	void CheckFoliageUnion(__in WORD graphic, __in int x, __in int y, __in int z);

	/*!
	Функция увеличесния размера списка рендера
	@return 
	*/
	void IncreaseRenderList();

	int m_ObjCount;
	RENDER_OBJECT_DATA *m_ObjList;

	//Функция увеличесния размера списка рендера
	void IncreaseObjList();
public:
	TGameScreen();
	virtual ~TGameScreen();

	//Идентификаторы событий для плавного перехода
	static const BYTE ID_SMOOTH_GS_LOGOUT = 1;

	SETGET(bool, UseLight);
	SETGET(bool, RenderListInitalized);

	/*!
	Обработка события после плавного затемнения экрана
	@param [__in_opt] action Идентификатор действия
	@return 
	*/
	void ProcessSmoothAction(__in_opt BYTE action = 0xFF);

	/*!
	Вычисление параметров игрового окна
	@return 
	*/
	void CalculateGameWindowBounds();

	/*!
	Расчет списка объектов рендера, вычисление прозрачности крон деревьев (в т.ч. составных)
	@return 
	*/
	void CalculateRenderList();

	/*!
	Инициализация
	@return 
	*/
	void Init();

	/*!
	Инициализация всплывающих подсказок
	@return 
	*/
	void InitPopupHelp();

	/*!
	Функция добавления источника света
	@param [__in] rwo Верхний объект, источающий свет
	@param [__in] lightObject Объект, источающий свет
	@param [__in] x Экранная координата X
	@param [__in] y Экранная координата Y
	@return 
	*/
	void AddLight(__in TRenderWorldObject *rwo, __in TRenderWorldObject *lightObject, __in int x, __in int y);

	/*!
	Отрисовка/выбор объектов
	@param [__in] mode true - отрисовка, false - выбор
	@return При выборе объектов - идентификатор выбранного объекта
	*/
	int Render(__in bool mode);



	/*!
	Нажатие левой кнопки мыши
	@return 
	*/
	void OnLeftMouseDown();

	/*!
	Отпускание левой кнопки мыши
	@return 
	*/
	void OnLeftMouseUp();

	/*!
	Нажатие правой кнопки мыши
	@return 
	*/
	void OnRightMouseDown();

	/*!
	Отпускание правой кнопки мыши
	@return 
	*/
	void OnRightMouseUp();

	/*!
	Двойной клик левой кнопкой мыши
	@return true при успешной обработке
	*/
	bool OnLeftMouseDoubleClick();

	/*!
	Двойной клик правой кнопкой мыши
	@return true при успешной обработке
	*/
	bool OnRightMouseDoubleClick();

	/*!
	Обработка средней кнопки (колесика) мыши
	@param [__in] state Состояние колесика
	@return 
	*/
	void OnMouseWheel(__in MOUSE_WHEEL_STATE state);

	/*!
	Обработка нажатия клавиши
	@param [__in] wparam не подписанный параметр
	@param [__in] lparam не подписанный параметр
	@return 
	*/
	void OnCharPress(__in WPARAM wparam, __in LPARAM lparam);

	/*!
	Обработка нажатия клавиши
	@param [__in] wparam не подписанный параметр
	@param [__in] lparam не подписанный параметр
	@return 
	*/
	void OnKeyPress(__in WPARAM wparam, __in LPARAM lparam);
 };
//---------------------------------------------------------------------------
 //!Ссылка на экран
 extern TGameScreen *GameScreen;

 //!Ссылка на положение и характеристики игрового окна
extern RENDER_VARIABLES_FOR_GAME_WINDOW *g_RenderBounds;
//---------------------------------------------------------------------------
#endif