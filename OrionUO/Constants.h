/***********************************************************************************
**
** Constants.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef CONSTANTS_H
#define CONSTANTS_H
//----------------------------------------------------------------------------------
const int MAX_CONSOLE_STACK_SIZE = 100;

const int DRAG_ITEMS_DISTANCE = 3;

const int MAX_ANIMATIONS_OBJECT_REMOVED_BY_GARBAGE_COLLECTOR = 5;

const int MAX_ART_OBJECT_REMOVED_BY_GARBAGE_COLLECTOR = 20;

const int MAX_GUMP_OBJECT_REMOVED_BY_GARBAGE_COLLECTOR = 20;

const int MAX_SOUND_OBJECT_REMOVED_BY_GARBAGE_COLLECTOR = 20;

const int MAX_MAP_OBJECT_REMOVED_BY_GARBAGE_COLLECTOR = 50;

const int TRANSLUCENT_ALPHA = 0xB2; //0.7

const int FOLIAGE_ALPHA = 0x4C; //0.3

const int ALPHA_STEP = 25;

const int ANIMATION_GROUPS_COUNT = 100;

const int CONTEXT_MENU_FONT = 1;

const int KEEP_CHARACTERS_IN_REMOVE_LIST_DELAY = 100;

const int MAX_STEPS_COUNT = 4;

//!Задержка между кадрами для активного окна приложения
const int FRAME_DELAY_ACTIVE_WINDOW = 30;

//!Задержка между кадрами для неактивного окна приложения
const int FRAME_DELAY_UNACTIVE_WINDOW = (FRAME_DELAY_ACTIVE_WINDOW + 1) * 7;

const int MIN_VIEW_RANGE = 5;

const int MAX_VIEW_RANGE = 18;

const int MAX_MAPS_COUNT = 6;

const int DAMAGE_TEXT_NORMAL_DELAY = 1000;

const int DAMAGE_TEXT_TRANSPARENT_DELAY = 400;

const int DAMAGE_TEXT_STEP = 2;

const int DAMAGE_TEXT_ALPHA_STEP = (DAMAGE_TEXT_TRANSPARENT_DELAY / FRAME_DELAY_ACTIVE_WINDOW);

const int DRAG_PIXEL_RANGE = 1;

const int DRAG_PIXEL_RANGE_WITH_TARGET = 3;

static const int DEATH_MUSIC_INDEX = 42;

static const int ORIGINAL_ITEMS_ANIMATION_DELAY = 80;
static const int ORION_ITEMS_ANIMATION_DELAY = 50;

static const int ORIGINAL_CHARACTERS_ANIMATION_DELAY = 100;
static const int ORION_CHARACTERS_ANIMATION_DELAY = 70;

static const ushort FIELD_REPLACE_GRAPHIC = 0x1826;

//!Масимальное количество источников света на экране
const int MAX_LIGHT_SOURCES = 100;

//!Максимальное количество гампов для вывода Object Handles
const int MAX_OBJECT_HANDLES = 200;

//!Максимальный размер fastwalk стека
const int MAX_FAST_WALK_STACK_SIZE = 5;

//!Максимальный размер буфера для данных при записи в файл (класс TFileWriter)
const int MAX_FILE_BUFFER_SIZE = 0x1000;

//!Динамический размер пакета
const int SIZE_VARIABLE = 0;

//!Размер таблицы индексов земли из арт.мул
const int MAX_LAND_DATA_INDEX_COUNT = 0x4000;

//!Размер таблицы индексов статики из арт.мул, >=CV_7090: count=0x10000, >=CV_7000: count=0x8000, <CV_7000: count=0x4000
const int MAX_STATIC_DATA_INDEX_COUNT = 0x10000;

//!Размер таблицы индексов текстур ландшафта
const int MAX_LAND_TEXTURES_DATA_INDEX_COUNT = 0x4000;

//!Размер таблицы индексов гампов
const int MAX_GUMP_DATA_INDEX_COUNT = 0x10000;

//!Размер таблицы индексов звуков
const int MAX_SOUND_DATA_INDEX_COUNT = 0x0800;

const int MAX_MULTI_DATA_INDEX_COUNT = 0x2200;

//!Размер массива с мп3 файлами
const int MAX_MUSIC_DATA_INDEX_COUNT = 150;

//!Размер таблицы индексов анимаций
const int MAX_ANIMATIONS_DATA_INDEX_COUNT = 2048;

//!Размер таблицы индексов анимаций
const int MAX_LIGHTS_DATA_INDEX_COUNT = 100;

//!Спектральные цвета
const WORD SPECTRAL_COLOR_FLAG = 0x4000;

//!Специальный цвет спектральной группы (более мягкий переход по краям, нет сильно выраженных границ)
const WORD SPECTRAL_COLOR_SPECIAL = 0x4666;

//!Ширина пробела для юникод шрифтов
const int UNICODE_SPACE_WIDTH = 8;

//!Коэффициаент для расчета наклона текста
const float ITALIC_FONT_KOEFFICIENT = 3.3f;

//!Максимально возможная высота HTML текста
const int MAX_HTML_TEXT_HEIGHT = 18;

//!Смещение от индекса предмета до гаммпа игровой фигуры (для шахмат)
const int GAME_FIGURE_GUMP_OFFSET = 11369;

//!Максимальная ширина текстового сообщения от объекта мира
const int TEXT_MESSAGE_MAX_WIDTH = 200;

//!Максимальная ширина текстового сообщения систем чата
const int TEXT_SYSTEM_MESSAGE_MAX_WIDTH = 320;

//!Время жизни черного экрана
const DWORD DEATH_SCREEN_DELAY = 3000;

//!Цвет выбранного ландшафта
const WORD SELECT_LAND_COLOR = 0x0044;

//!Цвет выбранной статики
const WORD SELECT_STATIC_COLOR = 0x0077;

//!Цвет выбранного мульти-объекта
const WORD SELECT_MULTI_COLOR = 0x0066;

//!Смещение гампов для пурсонажа мужского пола
const int MALE_GUMP_OFFSET = 50000;

//!Смещение гампов для пурсонажа женского пола
const int FEMALE_GUMP_OFFSET = 60000;

//!Дистанция до объектов-контейнеров, выше которой гамп должен закрыться
const int REMOVE_CONTAINER_GUMP_RANGE = 3;

//!Высота персонажа
const int DEFAULT_CHARACTER_HEIGHT = 16;

//!Высота блокирования пережвижения предметами/персонажами
const int DEFAULT_BLOCK_HEIGHT = 16;

//!Задержка для перемещения бегом на маунте
const int STEP_DELAY_MOUNT_RUN = 100;

//!Задержка для перемещения хотьбы на маунте
const int STEP_DELAY_MOUNT_WALK = 200;

//!Задержка для перемещения бегом
const int STEP_DELAY_RUN = 200;

//!Задержка для перемещения хотьбы
const int STEP_DELAY_WALK = 400;

//!Задержка для смены направления
const int TURN_DELAY = 100;

//!Задержка для быстрой смены направления
const int TURN_DELAY_FAST = 45;

//!Задержка таймера хотьбы
const int WALKING_DELAY = 200;

//!Таблица задержек перемещений
const int CHARACTER_ANIMATION_DELAY_TABLE[2][2] =
{
	//!Пешком
	{
		//!Идет
		STEP_DELAY_WALK,
		//!Бежит
		STEP_DELAY_RUN
	},
	//Верхом
	{
		//!Идет
		STEP_DELAY_MOUNT_WALK,
		//!Бежит
		STEP_DELAY_MOUNT_RUN
	}
};

//!Задержка на двойное нажатие мышки
const int DCLICK_DELAY = 350;

//!Задержка между прокруткой скроллера
const int SCROLL_LISTING_DELAY = 150;

//!Задержка между изменением количества предметов в байлисте
const int CHANGE_SHOP_COUNT_DELAY = 50;

//!Задержка между сменой макросов в опциях
const int CHANGE_MACRO_DELAY = 100;

//!Задержка между прокруткой макросов в опциях
const int CHANGE_MACRO_LIST_DELAY = 500;

//!Задержка между отправкой последнего пакета и Ping (0x73) пакетом, 20 секунд
const int SEND_TIMEOUT_DELAY = 20000;

//!Шаг в пикселях при прокрутке предметов в меню
const int GUMP_MENU_PIXEL_STEP = 5;

//!Шаг в пикселях при прокрутке скроллера
const int GUMP_SCROLLING_PIXEL_STEP = 10;

//!Время, через которое производить удаление текстур
const int CLEAR_TEXTURES_DELAY = 3000;

//!Время, через которое производить удаление текстур
const int CLEAR_ANIMATION_TEXTURES_DELAY = 10000;

//!Таймер погоды
const int WEATHER_TIMER = (6 * 60 * 1000);

//!Максимальное время ожидания для ловушки на таргет
const int WAIT_FOR_TARGET_DELAY = 5000;

//!Список слоев, защищенных от дропа
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

//!Структура с данными о объединениях крон деревьев
struct TREE_UNIONS
{
	//!Индекс начала объединения
	ushort GraphicStart;

	//!Индекс завершения объединения
	ushort GraphicEnd;
};

//!Количество деревьев, имеющих состаные кроны
const int TREE_COUNT = 9;

//!Данные о составных кронах (индексы картинок из клиента 7.0.45.0)
static const TREE_UNIONS TREE_INFO[TREE_COUNT] =
{
	{ 0x0D45, 0x0D4C },
	{ 0x0D4D, 0x0D53 },
	{ 0x0D5C, 0x0D62 },
	{ 0x0D63, 0x0D69 },
	{ 0x0D73, 0x0D79 },
	{ 0x0D7A, 0x0D7F },
	{ 0x0D87, 0x0D8B },
	{ 0x0D8C, 0x0D90 },
	{ 0x12BE, 0x12C7 }
};

//!Смещение для анимации сидячего персонажа по оси X
const int SITTING_OFFSET_X = 8;

//!Структура данных о предметах, на которых можно сидеть и возможные направления
struct SITTING_INFO_DATA
{
	//!Индекс картинки предмета
	ushort Graphic;

	//!Направления анимации для направлени персонажа:
	//!Для 7, 0
	char Direction1; //0

	//!Для 1, 2
	char Direction2; //2
	
	//!Для 3, 4
	char Direction3; //4
	
	//!Для 5, 6
	char Direction4; //6
	
	//!Смещение по оси Y
	char OffsetY;
	
	//!Смещение по оси Y при зеркальном отображении персонажа
	char MirrorOffsetY;

	//!Рисуем спинку или нет?
	bool DrawBack;
};

//!Количество объектов, на которых можно сидеть
const int SITTING_ITEMS_COUNT = 98;

#define SITTING_N 0, 0, 0, 0
#define SITTING_E 2, 2, 2, 2
#define SITTING_S 4, 4, 4, 4
#define SITTING_W 6, 6, 6, 6
#define SITTING_N_S 0, -1, 4, -1
#define SITTING_E_W -1, 2, -1, 6
#define SITTING_ALL 0, 2, 4, 6
#define SITTING_UNKNOWN 0, 2, 4, 6

//!Данные о предметах, на которых можно сидеть (индексы картинок из клиента 7.0.45.0)
static const SITTING_INFO_DATA SITTING_INFO[SITTING_ITEMS_COUNT] =
{
	{ 0x0459, SITTING_N_S, 2, 2, false },
	{ 0x045A, SITTING_E_W, 2, 2, false },
	{ 0x045B, SITTING_N_S, 2, 2, false },
	{ 0x045C, SITTING_E_W, 2, 2, false },
	{ 0x0A2A, SITTING_ALL, -4, -4, false },
	{ 0x0A2B, SITTING_ALL, -8, -8, false },
	{ 0x0B2C, SITTING_E_W, 2, 2, false },
	{ 0x0B2D, SITTING_N_S, 2, 2, false },
	{ 0x0B2E, SITTING_S, 0, 0, false },
	{ 0x0B2F, SITTING_E, 6, 6, false },
	{ 0x0B30, SITTING_W, -8, 8, true },
	{ 0x0B31, SITTING_N, 0, 4, true },
	{ 0x0B32, SITTING_S, 0, 0, false },
	{ 0x0B33, SITTING_E, 0, 0, false },
	{ 0x0B4E, SITTING_E, 0, 0, false },
	{ 0x0B4F, SITTING_S, 0, 0, false },
	{ 0x0B50, SITTING_N, 0, 0, true },
	{ 0x0B51, SITTING_W, 0, 0, true },
	{ 0x0B52, SITTING_E, 0, 0, false },
	{ 0x0B53, SITTING_S, 0, 0, false },
	{ 0x0B54, SITTING_N, 0, 0, true },
	{ 0x0B55, SITTING_W, 0, 0, true },
	{ 0x0B56, SITTING_E, 4, 4, false },
	{ 0x0B57, SITTING_S, 4, 4, false },
	{ 0x0B58, SITTING_W, 0, 8, true },
	{ 0x0B59, SITTING_N, 0, 8, true },
	{ 0x0B5A, SITTING_E, 8, 8, false },
	{ 0x0B5B, SITTING_S, 8, 8, false },
	{ 0x0B5C, SITTING_N, 0, 8, true },
	{ 0x0B5D, SITTING_W, 0, 8, true },
	{ 0x0B5E, SITTING_ALL, -8, -8, false },
	{ 0x0B5F, SITTING_E_W, 3, 14, false },
	{ 0x0B60, SITTING_E_W, 3, 14, false },
	{ 0x0B61, SITTING_E_W, 3, 14, false },
	{ 0x0B62, SITTING_E_W, 3, 10, false },
	{ 0x0B63, SITTING_E_W, 3, 10, false },
	{ 0x0B64, SITTING_E_W, 3, 10, false },
	{ 0x0B65, SITTING_N_S, 3, 10, false },
	{ 0x0B66, SITTING_N_S, 3, 10, false },
	{ 0x0B67, SITTING_N_S, 3, 10, false },
	{ 0x0B68, SITTING_N_S, 3, 10, false },
	{ 0x0B69, SITTING_N_S, 3, 10, false },
	{ 0x0B6A, SITTING_N_S, 3, 10, false },
	{ 0x0B91, SITTING_S, 6, 6, false },
	{ 0x0B92, SITTING_S, 6, 6, false },
	{ 0x0B93, SITTING_E, 6, 6, false },
	{ 0x0B94, SITTING_E, 6, 6, false },
	{ 0x0CF3, SITTING_E_W, 2, 8, false },
	{ 0x0CF4, SITTING_E_W, 2, 8, false },
	{ 0x0CF6, SITTING_N_S, 2, 8, false },
	{ 0x0CF7, SITTING_N_S, 2, 8, false },
	{ 0x11FC, SITTING_ALL, 2, 7, false },
	{ 0x1218, SITTING_S, 4, 4, false },
	{ 0x1219, SITTING_E, 4, 4, false },
	{ 0x121A, SITTING_N, 0, 8, true },
	{ 0x121B, SITTING_W, 0, 8, true },
	{ 0x1527, SITTING_E, 0, 0, false },
	{ 0x1771, SITTING_ALL, 0, 0, false },
	{ 0x1776, SITTING_ALL, 0, 0, false },
	{ 0x1779, SITTING_ALL, 0, 0, false },
	{ 0x1DC7, SITTING_E_W, 3, 10, false },
	{ 0x1DC8, SITTING_E_W, 3, 10, false },
	{ 0x1DC9, SITTING_E_W, 3, 10, false },
	{ 0x1DCA, SITTING_N_S, 3, 10, false },
	{ 0x1DCB, SITTING_N_S, 3, 10, false },
	{ 0x1DCC, SITTING_N_S, 3, 10, false },
	{ 0x1DCD, SITTING_E_W, 3, 10, false },
	{ 0x1DCE, SITTING_E_W, 3, 10, false },
	{ 0x1DCF, SITTING_E_W, 3, 10, false },
	{ 0x1DD0, SITTING_N_S, 3, 10, false },
	{ 0x1DD1, SITTING_N_S, 3, 10, false },
	{ 0x1DD2, SITTING_E_W, 3, 10, false },
	//!Нет данных
	{ 0x2A58, SITTING_S, 0, 0, false },
	{ 0x2A59, SITTING_E, 0, 0, false },
	{ 0x2A5A, SITTING_UNKNOWN, 0, 0, false },
	{ 0x2A5B, SITTING_UNKNOWN, 10, 10, false },
	{ 0x2A7F, SITTING_UNKNOWN, 0, 0, false },
	{ 0x2A80, SITTING_UNKNOWN, 0, 0, false },
	{ 0x2DDF, SITTING_UNKNOWN, 2, 2, false },
	{ 0x2DE0, SITTING_UNKNOWN, 2, 2, false },
	{ 0x2DE3, SITTING_E, 4, 4, false },
	{ 0x2DE4, SITTING_S, 4, 4, false },
	{ 0x2DE5, SITTING_W, 4, 4, false },
	{ 0x2DE6, SITTING_N, 4, 4, false },
	{ 0x2DEB, SITTING_N, 4, 4, false },
	{ 0x2DEC, SITTING_S, 4, 4, false },
	{ 0x2DED, SITTING_E, 4, 4, false },
	{ 0x2DEE, SITTING_W, 4, 4, false },
	{ 0x2DF5, SITTING_UNKNOWN, 4, 4, false },
	{ 0x2DF6, SITTING_UNKNOWN, 4, 4, false },
	{ 0x3088, SITTING_UNKNOWN, 4, 4, false },
	{ 0x3089, SITTING_UNKNOWN, 4, 4, false },
	{ 0x308A, SITTING_UNKNOWN, 4, 4, false },
	{ 0x308B, SITTING_UNKNOWN, 4, 4, false },
	{ 0x35ED, SITTING_UNKNOWN, 0, 0, false },
	{ 0x35EE, SITTING_UNKNOWN, 0, 0, false },
	//
	{ 0x3DFF, SITTING_N_S, 2, 2, false },
	{ 0x3E00, SITTING_E_W, 2, 2, false }
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
