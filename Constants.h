/****************************************************************************
**
** Constants.h
**
** Copyright (C) February 2016 Hotride
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
#ifndef ConstantsH
#define ConstantsH
//---------------------------------------------------------------------------
//Максимальное количество теней на экране
static const int MAX_SHADOWS_COUNT = 100;

//Масимальное количество источников света на экране
const int MAX_LIGHT_SOURCES = 100;

//Максимальный размер fastwalk стека
const int MAX_FAST_WALK_STACK_SIZE = 255;

//Смещение для анимации сидячего персонажа по оси X
const int SITTING_WIDTH_OFFSET = 10;

//Максимальный размер буфера для данных при записи в файл (класс TFileWriter)
const int MAX_FILE_BUFFER_SIZE = 0x1000;

//Динамический размер пакета
const int SIZE_VARIABLE = 0;

//Ширина пробела для юникод шрифтов
const int UNICODE_SPACE_WIDTH = 8;

//Коэффициаент для расчета наклона текста
const float ITALIC_FONT_KOEFFICIENT = 3.3f;

//Максимально возможная высота HTML текста
const int MAX_HTML_TEXT_HEIGHT = 18;

//Смещение от индекса предмета до гаммпа игровой фигуры (для шахмат)
const int GAME_FIGURE_GUMP_OFFSET = 11369;

//Максимальная ширина текстового сообщения от объекта мира
const int TEXT_MESSAGE_MAX_WIDTH = 190;

//Время жизни черного экрана
const DWORD DEATH_SCREEN_DELAY = 3000;

//Задержка между кадрами для активного окна приложения
const int FRAME_DELAY_ACTIVE_WINDOW = 30;

//Задержка между кадрами для неактивного окна приложения
const int FRAME_DELAY_UNACTIVE_WINDOW = (FRAME_DELAY_ACTIVE_WINDOW + 1) * 7;

//Значения задержек в массиве
static int g_FrameDelay[2] = { FRAME_DELAY_UNACTIVE_WINDOW, FRAME_DELAY_ACTIVE_WINDOW };

//Цвет выбранного ландшафта
const WORD SELECT_LAND_COLOR = 0x0044;

//Цвет выбранной статики
const WORD SELECT_STATIC_COLOR = 0x0077;

//Цвет выбранного мульти-объекта
const WORD SELECT_MULTI_COLOR = 0x0066;

//Смещение гампов для пурсонажа мужского пола
const int MALE_GUMP_OFFSET = 50000;

//Смещение гампов для пурсонажа женского пола
const int FEMALE_GUMP_OFFSET = 60000;

//Дистанция до объектов-контейнеров, выше которой гамп должен закрыться
const int REMOVE_CONTAINER_GUMP_RANGE = 3;

//Максимальная блокирующая высота объекта
const int g_MaxBlockZ = 15;

//Максимальная высота, на которую можно забраться
const int g_MaxClimbZ = 9;

//Максимальная высота, на которую можно упасть
const int g_MaxFallZ = 19;

//Максимальная высота прыжка
const int g_MaxClimbMapZ = 19;

//Высота персонажа
const int DEFAULT_CHARACTER_HEIGHT = 16;

//Задержка для перемещения бегом на маунте
const int STEP_DELAY_MOUNT_RUN = 85;

//Задержка для перемещения хотьбы на маунте
const int STEP_DELAY_MOUNT_WALK = 185;

//Задержка для перемещения бегом
const int STEP_DELAY_RUN = 185;

//Задержка для перемещения хотьбы
const int STEP_DELAY_WALK = 385;

//Таблица задержек перемещений
const int CHARACTER_ANIMATION_DELAY_TABLE[2][2] =
{
	//without mount
	{
		//walk
		STEP_DELAY_WALK,
		//run
		STEP_DELAY_RUN
	},
	//on mount
	{
		//walk
		STEP_DELAY_MOUNT_WALK,
		//run
		STEP_DELAY_MOUNT_RUN
	}
};

//Задержка на двойное нажатие мышки
const int DCLICK_DELAY = 350;

//Задержка между прокруткой скроллера
const int SCROLL_LISTING_DELAY = 150;

//Задержка между изменением количества предметов в байлисте
const int CHANGE_SHOP_COUNT_DELAY = 50;

//Задержка между сменой макросов в опциях
const int CHANGE_MACRO_DELAY = 100;

//Задержка между прокруткой макросов в опциях
const int CHANGE_MACRO_LIST_DELAY = 500;

//Задержка между отправкой последнего пакета и Ping (0x73) пакетом, 55 секунд
const int SEND_TIMEOUT_DELAY = 55000;

//Шаг в пикселях при прокрутке предметов в меню
const int GUMP_MENU_PIXEL_STEP = 5;

//Шаг в пикселях при прокрутке скроллера
const int GUMP_SCROLLING_PIXEL_STEP = 10;

//Время, через которое производить удаление текстур
const int CLEAR_TEXTURES_DELAY = 30000;

//Таймер погоды
const int WEATHER_TIMER = (6 * 60 * 1000);

//Максимальное время ожидания для ловушки на таргет
const int WAIT_FOR_TARGET_DELAY = 2000;

//Список слоев, защищенных от дропа
static const bool LAYER_UNSAFE[30] =
{
	false,   //0
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	false,
	true,
	true,
	true,
	false,
	false,
	true,
	true,
	true,
	true,
	false, //0x15
	true,
	true,
	true,
	false,
	false,
	false,
	false,
	false
};

//Названия городов
static const char *CITY_NAMES[9] =
{
	"Yew",
	"Minoc",
	"Britain",
	"Moonglow",
	"Trinsic",
	"Magincia",
	"Jhelom",
	"Skara Brae",
	"Vesper"
};

//Названия кругов заклинаний
static char SPELL_CIRCLES_NAMES[8][15] =
{
	"First Circle",
	"Second Circle",
	"Third Circle",
	"Fourth Circle",
	"Fifth Circle",
	"Sixth Circle",
	"Seventh Circle",
	"Eighth Circle"
};

//Необходимые реагенты на заклинания
static const WORD SPELL_REAGENTS[64][4] =
{
	{ 0x0F7B, 0x0F88, 0, 0 },
	{ 0x0F84, 0x0F85, 0x0F86, 0 },
	{ 0x0F85, 0x0F88, 0, 0 },
	{ 0x0F84, 0x0F85, 0x0F8D, 0 },
	{ 0x0F7A, 0x0F88, 0, 0 },
	{ 0x0F8C, 0x0F8D, 0, 0 },
	{ 0x0F84, 0x0F8C, 0x0F8D, 0 },
	{ 0x0F84, 0x0F88, 0, 0 },
	{ 0x0F7B, 0x0F86, 0, 0 },
	{ 0x0F86, 0x0F88, 0, 0 },
	{ 0x0F84, 0x0F85, 0, 0 },
	{ 0x0F88, 0x0F8D, 0, 0 },
	{ 0x0F84, 0x0F8C, 0x0F8D, 0 },
	{ 0x0F7B, 0x0F8C, 0, 0 },
	{ 0x0F84, 0x0F85, 0x0F8C, 0 },
	{ 0x0F86, 0x0F88, 0, 0 },
	{ 0x0F84, 0x0F86, 0, 0 },
	{ 0x0F7A, 0x0F8C, 0, 0 },
	{ 0x0F8C, 0x0F7B, 0x0F84, 0 },
	{ 0x0F88, 0, 0, 0 },
	{ 0x0F7B, 0x0F86, 0, 0 },
	{ 0x0F7B, 0x0F86, 0, 0 },
	{ 0x0F7B, 0x0F8C, 0, 0 },
	{ 0x0F7B, 0x0F84, 0, 0 },
	{ 0x0F84, 0x0F85, 0x0F86, 0 },
	{ 0x0F84, 0x0F85, 0x0F86, 0x0F8C },
	{ 0x0F84, 0x0F88, 0x0F8C, 0 },
	{ 0x0F7A, 0x0F8D, 0x0F8C, 0 },
	{ 0x0F84, 0x0F85, 0x0F86, 0x0F8D },
	{ 0x0F7A, 0x0F86, 0x0F8C, 0 },
	{ 0x0F7A, 0x0F86, 0x0F8D, 0 },
	{ 0x0F7A, 0x0F7B, 0x0F86, 0 },
	{ 0x0F7A, 0x0F86, 0x0F88, 0 },
	{ 0x0F84, 0x0F7A, 0x0F8D, 0x0F8C },
	{ 0x0F7B, 0x0F84, 0x0F88, 0 },
	{ 0x0F84, 0x0F86, 0x0F8D, 0 },
	{ 0x0F7A, 0x0F86, 0x0F88, 0x0F8C },
	{ 0x0F84, 0x0F86, 0x0F8D, 0 },
	{ 0x0F7A, 0x0F88, 0x0F8D, 0 },
	{ 0x0F7B, 0x0F86, 0x0F8D, 0 },
	{ 0x0F84, 0x0F86, 0x0F8C, 0 },
	{ 0x0F7A, 0x0F88, 0, 0 },
	{ 0x0F7A, 0x0F86, 0x0F8C, 0 },
	{ 0x0F7B, 0x0F88, 0, 0 },
	{ 0x0F7A, 0x0F7B, 0x0F86, 0 },
	{ 0x0F84, 0x0F86, 0x0F88, 0x0F8C },
	{ 0x0F7A, 0x0F85, 0x0F8D, 0 },
	{ 0x0F7B, 0x0F8C, 0, 0 },
	{ 0x0F7A, 0x0F86, 0x0F7B, 0x0F8C },
	{ 0x0F7A, 0x0F86, 0x0F8D, 0x0F8C },
	{ 0x0F8D, 0x0F8C, 0, 0 },
	{ 0x0F7A, 0x0F86, 0x0F8C, 0 },
	{ 0x0F7A, 0x0F7B, 0x0F86, 0x0F8D },
	{ 0x0F7A, 0x0F84, 0x0F86, 0x0F8C },
	{ 0x0F7B, 0x0F8D, 0x0F86, 0x0F8C },
	{ 0x0F7B, 0x0F86, 0x0F8D, 0 },
	{ 0x0F7B, 0x0F85, 0x0F86, 0x0F8C },
	{ 0x0F7A, 0x0F7B, 0x0F86, 0x0F88 },
	{ 0x0F7B, 0x0F84, 0x0F85, 0 },
	{ 0x0F7B, 0x0F86, 0x0F8D, 0 },
	{ 0x0F7B, 0x0F86, 0x0F8D, 0x0F8C },
	{ 0x0F7B, 0x0F86, 0x0F8D, 0 },
	{ 0x0F7B, 0x0F86, 0x0F8D, 0x0F8C },
	{ 0x0F7B, 0x0F86, 0x0F8D, 0 }
};

//Аббревиатура заклинаний
static char SPELL_CAST_ABBREVIATURE[64][8] =
{
	"U J",
	"I M Y",
	"R W",
	"I M",
	"I P Y",
	"I L",
	"S F",
	"D M",
	"E U",
	"U W",
	"A N",
	"A M",
	"I J",
	"A J",
	"U S",
	"U M",
	"R S",
	"V F",
	"A P",
	"I N",
	"O P Y",
	"R P",
	"E P",
	"I S Y",
	"W A N",
	"V U S",
	"D S",
	"I F G",
	"I V M",
	"P O G",
	"O R",
	"K O P",
	"I J H Y",
	"A G",
	"K I E",
	"I J S",
	"P C W",
	"A E P",
	"I N G",
	"K X",
	"A O",
	"C P",
	"V O F",
	"K P Y",
	"V D S",
	"I E G",
	"W Q",
	"V O G",
	"I S G",
	"K V F",
	"V R P",
	"O S",
	"V A O",
	"K D F Y",
	"V Y R",
	"I V P",
	"V C P",
	"A C",
	"K V X A",
	"K V X D",
	"K V X T",
	"K V X F",
	"K V X A"
};
//---------------------------------------------------------------------------
#endif