/***********************************************************************************
**
** EnumList.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef ENUMLIST_H
#define ENUMLIST_H
//----------------------------------------------------------------------------------
//!На какой стадии находится рендер (и вообще клиент)
enum GAME_STATE
{
	GS_MAIN = 0,			//!Основной гамп (клиент только запустили)
	GS_MAIN_CONNECT,		//!Гамп коннекта к серверу авторизации
	GS_SERVER,				//!Гамп списка серверов
	GS_SERVER_CONNECT,		//!Гамп коннекта к гейм серверу
	GS_CHARACTER,			//!Гамп списока чаров
	GS_DELETE,				//!Гамп удаления персонажа
	GS_PROFESSION_SELECT,	//!Гамп выбора профессии
	GS_CREATE,				//!Гамп создания персонажа
	GS_SELECT_TOWN,			//!Гамп выбора стартового города
	GS_GAME_CONNECT,		//!Гамп входа в игру
	GS_GAME,				//!Игровой гамп (отображение мира и прочего)
	GS_GAME_BLOCKED			/*!Модификация игрового гампа, всё отображается, но заблокировано
							для кликов, перемещений и изменения чего-либо, кроме гампа (гампов)
							вызвавшего эту стадию рендера
							*/
};
//----------------------------------------------------------------------------------
//!Тип объекта при рендере
enum RENDER_OBJECT_TYPE
{
	ROT_NO_OBJECT = 0,		//Ничего не выбрано
	ROT_GAME_OBJECT,		//Игровой объект
	ROT_STATIC_OBJECT,		//Статика
	ROT_LAND_OBJECT,		//Ландшафт
	ROT_MULTI_OBJECT,		//Мульти
	ROT_EFFECT				//Эффект
};
//----------------------------------------------------------------------------------
//!Тип объекта поиска
enum SCAN_TYPE_OBJECT
{
	STO_HOSTILE = 0,
	STO_PARTY,
	STO_FOLLOWERS,
	STO_OBJECTS,
	STO_MOBILES
};
//----------------------------------------------------------------------------------
//!Тип поиска объекта
enum SCAN_MODE_OBJECT
{
	SMO_NEXT = 0,
	SMO_PREV,
	SMO_NEAREST
};
//----------------------------------------------------------------------------------
//!Тип эффекта
enum EFFECT_TYPE
{
	EF_MOVING = 0,		//!Движется
	EF_LIGHTING,		//!Удар молнией
	EF_STAY_AT_POS,		//!Стоит на точке
	EF_STAY_AT_SOURCE,	//!Привязан к персонажу
	EF_DRAG				//!Анимация перемещения предмета
};
//----------------------------------------------------------------------------------
//!Тип ускорения
enum CHARACTER_SPEED_TYPE
{
	CST_NORMAL = 0,					//!Нормальная скорость перемещения
	CST_FAST_UNMOUNT,				//!Ускорение движения персонажа, если он перемещается без маунта (выставляется скорость сопоставимая с маунтом)
	CST_CANT_RUN,					//!Персонаж может только ходить, не бежать (как на маунте, так и без него)
	CST_FAST_UNMOUNT_AND_CANT_RUN	//!Совокупность предыдущих 2 вариантов (персонаж не может бежать и ускорение только при перемещении без маунта)
};
//----------------------------------------------------------------------------------
//!Тип шифрования
enum ENCRYPTION_TYPE
{
	ET_NOCRYPT = 0,	//!Без шифрования
	ET_OLD_BFISH,	//!Старое BlowFish
	ET_1_25_36,		//!Специальное для клиента 1.25.36 (BlowFish, изменен алгоритм логин шифрования)
	ET_BFISH,		//!Стандартный BlowFish
	ET_203,			//!Специальное для 2.0.3 клиента (BlowFish + TwoFish без MD5)
	ET_TFISH		//!TwoFish + MD5
};
//----------------------------------------------------------------------------------
//!Версия клиента (для изменения в протоколе и прочих няшках)
enum CLIENT_VERSION
{
	CV_OLD = 0,		//Предшествующие клиенту 2.0.0, Остальные по логике, исходя из названия
	CV_200,			//Отправляется пакет с габаритами экрана
	//CV_204C,		//Использование *.def файлов
	CV_305D,		//Использование клилоков, количество слотов в списке персонажей равно количеству персонажей
	CV_306E,		//Добавлен пакет с типом клиента (0xBF subcmd 0x0F), использование mp3 вместо midi
	CV_308D,		//Добавлен параметр Maximum Stats в статусбар
	CV_308J,		//Добавлен параметр Followers в статусбар
	CV_308Z,		//Добавлены классы paladin, necromancer; custom houses, 5 resistances, изменено окно выбора профессии, убрана галочка Save Password
	CV_400B,		//Удаление тултипов
	CV_405A,		//Добавлены классы ninja, samurai
	CV_4011D,		//Изменение окна создания персонажа. Добавлена расса elves
	CV_500A,		//Кнопки папердолла заменены: Journal -> Quests; Chat -> Guild, использование Mega Cliloc, Убрана загрузка файла Verdata.mul
	CV_5020,		//Добавлен гамп бафов
	CV_5090,		//
	CV_6000,		//Добавлены цвета гильд/алли чата, игноры чатов. Добавлены опции новой таргет системы, вывод свойств предметов, Object Handles,
	CV_6013,		//
	CV_6017,		//
	CV_6040,		//Увеличилось количество слотов персонажей
	CV_6060,		//
	CV_60142,		//
	CV_60144,		//Изменение окна создания персонажа. Добавлена расса gargoyle
	CV_7000,		//
	CV_7090,		//
	CV_70130,		//
	CV_70160,		//
	CV_70180,		//
	CV_70240,		//*.mul -> *.uop
	CV_70331		//
};
//----------------------------------------------------------------------------------
//!На какой стадии находится окно коннекта
enum CONNECTION_SCREEN_TYPE
{
	CST_LOGIN = 0,
	CST_CHARACTER_LIST,
	CST_CONLOST,
	CST_SELECT_PROFESSOIN,
	CST_GAME_LOGIN,
	CST_GAME
};
//----------------------------------------------------------------------------------
//!Рассы
enum RACE_TYPE
{
	RT_HUMAN = 1,
	RT_ELF,
	RT_GARGOYLE
};
//----------------------------------------------------------------------------------
//!Типы профессий
enum PROFESSION_TYPE
{
	PT_NO_PROF = 0,
	PT_CATEGORY,
	PT_PROFESSION
};
//----------------------------------------------------------------------------------
//!Состояния ClientFlag для отправки при создании персонажа
enum CLIENT_FLAG
{
	CF_T2A = 0x00,
	CF_RE = 0x01,
	CF_TD = 0x02,
	CF_LBR = 0x04,
	CF_AOS = 0x08,
	CF_SE = 0x10,
	CF_SA = 0x20,
	CF_UO3D = 0x40,
	CF_RESERVED = 0x80,
	CF_3D = 0x100
};
//----------------------------------------------------------------------------------
//!Расширения для списка персонажей
enum CHARACTER_LIST_FLAG
{
	CLF_UNKNOWN = 0x01,
	CLF_OWERWRITE_CONFIGURATION_BUTTON = 0x02,
	CLF_ONE_CHARACTER_SLOT = 0x04,
	CLF_CONTEXT_MENU = 0x08,
	CLF_LIMIT_CHARACTER_SLOTS = 0x10,
	CLF_PALADIN_NECROMANCER_TOOLTIPS = 0x20,
	CLF_6_CHARACTER_SLOT = 0x40,
	CLF_SAMURAI_NINJA = 0x80,
	CLF_ELVEN_RACE = 0x100,
	CLF_UNKNOWN_1 = 0x200,
	CLF_UO3D = 0x400,
	CLF_UNKNOWN_2 = 0x800,
	CLF_7_CHARACTER_SLOT = 0x1000,
	CLF_UNKNOWN_3 = 0x2000,
	CLF_NEW_MOVEMENT_SYSTEM = 0x4000,
	CLF_UNLOCK_FELUCCA_AREAS = 0x8000
};
//----------------------------------------------------------------------------------
//!Состояния client features
enum LOCKED_FEATURE_FLAG
{
	LFF_T2A = 0x01,
	LFF_RE = 0x02,
	LFF_TD = 0x04,
	LFF_LBR = 0x08,
	LFF_AOS = 0x10,
	LFF_6_SLOT = 0x20,
	LFF_SE = 0x40,
	LFF_ML = 0x80,
	LFF_8_AGE = 0x100,
	LFF_9_AGE = 0x200,
	LFF_10_AGE = 0x400,
	LFF_HOUSING = 0x800,
	LFF_7_SLOT = 0x1000,
	LFF_KR = 0x2000,
	LFF_TRIAL_ACC = 0x4000,
	LFF_11_AGE = 0x8000,
	LFF_SA = 0x10000,
	LFF_HSA = 0x20000,
	LFF_GOTHIC_HOUSING = 0x40000,
	LFF_RUSTIC_HOUSING = 0x80000
};
//----------------------------------------------------------------------------------
//!Типы спеллбук
enum SPELLBOOK_TYPE
{
	ST_MAGE = 0,
	ST_NECRO,
	ST_PALADIN,
	ST_BUSHIDO = 4,
	ST_NINJITSU,
	ST_SPELL_WEAVING,
	ST_MYSTICISM
};
//----------------------------------------------------------------------------------
//!Смещение для книг заклинаний
enum SPELLBOOK_OFFSET
{
	SO_MAGE = 1,
	SO_NECRO = 101,
	SO_PALADIN = 201,
	SO_BUSHIDO = 301,
	SO_NINJITSU = 401,
	SO_SPELL_WEAVING = 501,
	SO_MYSTICISM = 601
};
//----------------------------------------------------------------------------------
//!Состояния плавного переключения экранов
enum SCREEN_EFFECT_MODE
{
	SEM_NONE = 0,
	SEM_SUNRISE,
	SEM_SUNSET
};
//----------------------------------------------------------------------------------
//!Тип плавного переключения экрана
enum SCREEN_EFFECT_TYPE
{
	SET_TO_BLACK,
	SET_TO_WHITE,
	SET_TO_WHITE_FAST,
	SET_TO_WHITE_THEN_BLACK,
	SET_TO_BLACK_VERY_FAST
};
//----------------------------------------------------------------------------------
//!Состояния плавного переключения экранов
enum WEATHER_TYPE
{
	WT_RAIN = 0,
	WT_FIERCE_STORM,
	WT_SNOW,
	WT_STORM
};
//----------------------------------------------------------------------------------
//!Типы гампов
enum GUMP_TYPE
{
	GT_NONE = 0,				//!Нет гампа
	GT_PAPERDOLL,				//!Папердолл 100%
	GT_STATUSBAR,				//!Статус бар (все виды) 100%
	GT_DYE,						//!Гамп выбора цвета 100% (напрямую зависит от GT_SELECT_COLOR)
	GT_OPTIONS,					//!Опции 90%
	GT_GENERIC,					//!Гамп от сервера 100%
	GT_MENU,					//!Меню от сервера 100%
	GT_GRAY_MENU,				//!"Серое" меню от сервера (блокирующий) 100%
	GT_TRADE,					//!Трэйд гамп 100%
	GT_SHOP,					//!Гампы покупки/продажи 100% / 100%
	GT_MINIMAP,					//!Мини-карта 100%
	GT_MAP,						//!Карта (городов/кладов) 100%
	GT_SKILLS,					//!Скиллы 100%
	GT_JOURNAL,					//!Журнал 100%
	GT_CONTAINER,				//!Контейнеры 100%
	GT_SPELLBOOK,				//!Спеллбука 100%
	GT_SPELL,					//!Иконка спелла 100%
	GT_DRAG,					//!Разделение стакуемых предметов (больше 1 штуки в стеке) 100%
	GT_TEXT_ENTRY_DIALOG,		//!Гамп ввода текста от сервера (блокирующий) 100%
	GT_QUESTION,				//!Гамп вопроса (блокирующий) 100%
	GT_SELECT_COLOR,			//!Гамп выбора цвета (для опций) 100%
	GT_SELECT_FONT,				//!Гамп выбора шрифта (для опций) 100%
	GT_NOTIFY,					//!Гамп сообщения (блокирующий) 100%
	GT_BOOK,					//!Гамп книг 10%
	GT_TIP,						//!Tip window 90%
	GT_PARTY_MANIFEST,			//!Пати манифест гамп 100%
	GT_MAIL,					//!Гамп почты 0%
	GT_PROFILE,					//!Гамп профиля 100%
	GT_MENUBAR,					//!Гамп менюбара 90%
	GT_BUFF,					//!Гамп баффов 100%
	GT_CHAT,					//!Гамп чата 0%
	GT_BULLETIN_BOARD,			//!Гамп доски объявлений 100%
	GT_BULLETIN_BOARD_ITEM,		//!Гамп предмета доски объявлений 100%
	GT_WORLD_MAP,				//!Гамп карты мира 100%
	GT_PLUGIN,					//!Гампы плагинов 0%
	GT_TARGET_SYSTEM,			//!Гамп таргет системы (статусбар) 100%
	GT_POPUP_MENU,				//!Гамп всплывающего меню 100%
	GT_CONSOLE_TYPE,			//!Гамп типа консоли 100%
	GT_SKILL,					//!Гамп навыка 100%
	GT_COMBAT_BOOK,				//!Гамп книги абилок 100%
	GT_ABILITY,					//!Гамп абилки 100%
	GT_RACIAL_ABILITIES_BOOK,	//!Гамп книги рассовых абилок 100%
	GT_RACIAL_ABILITY,			//!Гамп рассовой абилки 100%
	GT_CUSTOM_HOUSE,
	GT_PROPERTY,
	GT_PROPERTY_ICON
};
//----------------------------------------------------------------------------------
//!Типы объектов гампов
enum GUMP_OBJECT_TYPE
{
	GOT_NONE = 0,			//!Нет типа
	GOT_PAGE,				//!Идентификатор страницы
	GOT_GROUP,				//!Идентификатор группы (для радио-кнопок)
	GOT_MASTERGUMP,			//!Шта?
	GOT_RESIZEPIC,			//!Фон (лепится из 9 кусков)
	GOT_SCOPE,				//!Рамка (лепится из 4 кусков)
	GOT_CHECKTRANS,			//!Полупрозрачная зона на гампе
	GOT_SCISSOR,			//!Ножницы
	GOT_COLOREDPOLYGONE,	//!Цветной прямоугольник
	GOT_LINE,				//!Линия
	GOT_SLIDER,				//!Ползунок
	GOT_MINMAXBUTTONS,		//!Кнопки регулировки мин/макс значения
	GOT_SHADER,				//!Шейдерная программа
	GOT_HITBOX,				//!Область выделения (для текста)
	GOT_DATABOX,			//!Предметы для отрисовке (актуально использовать в гампах папердолла, контейнера и т.п, содержащих предметы мира)
	GOT_BUTTON,				//!Кнопка
	GOT_RESIZEBUTTON,		//!Кнопка изменения размера
	GOT_BUTTONTILEART,		//!Кнопка с картинкой
	GOT_BLENDING,			//!Блендинг цветов
	GOT_GLOBAL_COLOR,		//!Глобальный цвет
	GOT_BUFF,				//!Иконка баффа\дебаффа
	GOT_SHOPITEM,			//!Предмет магазина
	GOT_SHOPRESULT,			//!Предмет магазина (результат)
	GOT_SKILLITEM,			//!Объект скиллов
	GOT_SKILLGROUP,			//!Объект группы скиллов
	GOT_BB_OBJECT,			//!Объект доски объявлений (Bulletin Board)
	GOT_SCROLLBACKGROUND,	//!Фон в виде скролла
	GOT_MENUOBJECT,			//!Объект меню
	GOT_COMBOBOX,			//!Комбо-бокс
	GOT_CHECKBOX,			//!Чекбокс
	GOT_RADIO,				//!Радио-кнопка
	GOT_EXTERNALTEXTURE,	//!Внешняя текстура
	GOT_TILEPIC,			//!Картинка из статики
	GOT_TILEPICHIGHTLIGHTED,//!Картинка из статики, цветная с подсветкой с серийником
	GOT_GUMPPIC,			//!Картинка из гампа
	GOT_GUMPPICTILED,		//!Картинка из гампа (повторяющаяся)
	GOT_GUMPPICHIGHTLIGHTED,//!Картинка из гампа, цветная с подсветкой с серийником
	GOT_TEXT,				//!Текст
	GOT_HTMLTEXT,			//!Текст с поддержкой HTML разметки
	GOT_TEXTENTRY,			//!Ввод текста
	GOT_HTMLGUMP,			//!ХТМЛ гамп
	GOT_XFMHTMLGUMP,		//!ХТМЛ гамп из клилока (цветной там же)
	GOT_XFMHTMLTOKEN,		//
	GOT_TOOLTIP,				//
	GOT_VIRTURE_GUMP			//
};
//----------------------------------------------------------------------------------
enum SLIDER_TEXT_POSITION
{
	STP_TOP = 0,
	STP_BOTTOM,
	STP_LEFT,
	STP_RIGHT,
	STP_TOP_CENTER,
	STP_BOTTOM_CENTER,
	STP_LEFT_CENTER,
	STP_RIGHT_CENTER
};
//----------------------------------------------------------------------------------
enum SELECT_COLOR_GUMP_STATE
{
	//SCGS_OPT_POPUP_TEXT = 0,
	SCGS_OPT_TOOLTIP_TEXT = 0,
	SCGS_OPT_CHAT_INPUT_TEXT,
	SCGS_OPT_CHAT_MENU_OPTION,
	SCGS_OPT_CHAT_PLAYER_IN_MEMBER_LIST,
	SCGS_OPT_CHAT_TEXT,
	SCGS_OPT_CHAT_PLAYER_WITHOUT_PRIV,
	SCGS_OPT_CHAT_MUTED_TEXT,
	SCGS_OPT_CHAT_CHAN_MODER_NAME,
	SCGS_OPT_CHAT_CHAN_MODER_TEXT,
	SCGS_OPT_CHAT_MY_NAME,
	SCGS_OPT_CHAT_MY_TEXT,
	SCGS_OPT_CHAT_SYSTEM_MESSAGE,
	SCGS_OPT_CHAT_BG_OUTPUT,
	SCGS_OPT_CHAT_BG_INPUT,
	SCGS_OPT_CHAT_BG_USER_LIST,
	SCGS_OPT_CHAT_BG_CONF_LIST,
	SCGS_OPT_CHAT_BG_COMMAND_LIST,
	SCGS_OPT_DISPLAY_SPEECH,
	SCGS_OPT_DISPLAY_EMOTE,
	SCGS_OPT_DISPLAY_PARTY_MESSAGE,
	SCGS_OPT_DISPLAY_GUILD_MESSAGE,
	SCGS_OPT_DISPLAY_ALLIANCE_MESSAGE,
	SCGS_OPT_REPSYS_INNOCENT,
	SCGS_OPT_REPSYS_FRIENDLY,
	SCGS_OPT_REPSYS_SOMEONE,
	SCGS_OPT_REPSYS_CRIMINAL,
	SCGS_OPT_REPSYS_ENEMY,
	SCGS_OPT_REPSYS_MURDERER
};
//----------------------------------------------------------------------------------
enum SELECT_FONT_GUMP_STATE
{
	SFGS_OPT_POPUP = 1,
	SFGS_OPT_TOOLTIP,
	SFGS_OPT_CHAT,
	SFGS_OPT_MISCELLANEOUS
};
//----------------------------------------------------------------------------------
enum MAP_MESSAGE
{
	MM_ADD = 1,
	MM_INSERT,
	MM_MOVE,
	MM_REMOVE,
	MM_CLEAR,
	MM_EDIT,
	MM_EDIT_RESPONSE
};
//----------------------------------------------------------------------------------
enum UPDATE_GAME_OBJECT_TYPE
{
	UGOT_ITEM = 0,
	UGOT_NEW_ITEM = 1,
	UGOT_MULTI = 2,
};
//----------------------------------------------------------------------------------
enum ORION_COMMAND_TYPE
{
	OCT_RESERVED = 1,
	OCT_ORION_FEATURES = 50,
	OCT_ORION_IGNORE_TILES_IN_FILTER = 51,
	OCT_ORION_VERSION = 52,
	OCT_CLOSE_GENERIC_GUMP_WITHOUT_RESPONSE = 100,
	OCT_SELECT_MENU = 101,
	OCT_CAST_SPELL_REQUEST = 102,
	OCT_USE_SKILL_REQUEST = 103,
	OCT_DRAW_STATUSBAR = 104,
	OCT_CLOSE_STATUSBAR = 105,
	OCT_SECURE_TRADE_CHECK = 106,
	OCT_SECURE_TRADE_CLOSE = 107,
	OCT_UNICODE_SPEECH_REQUEST = 108,
	OCT_RENAME_MOUNT_REQUEST = 109,
	OCT_RECONNECT = 110,
	OCT_PLAY_MACRO = 111,
	OCT_MOVE_PAPERDOLL = 112,
	OCT_USE_ABILITY = 113
};
//----------------------------------------------------------------------------------
enum ORION_FILE_INDEX
{
	OFI_MAP_0_MUL = 1,
	OFI_MAP_1_MUL,
	OFI_MAP_2_MUL,
	OFI_MAP_3_MUL,
	OFI_MAP_4_MUL,
	OFI_MAP_5_MUL,
	OFI_MAP_0_UOP,
	OFI_MAP_1_UOP,
	OFI_MAP_2_UOP,
	OFI_MAP_3_UOP,
	OFI_MAP_4_UOP,
	OFI_MAP_5_UOP,
	OFI_MAPX_0_UOP,
	OFI_MAPX_1_UOP,
	OFI_MAPX_2_UOP,
	OFI_MAPX_3_UOP,
	OFI_MAPX_4_UOP,
	OFI_MAPX_5_UOP,
	OFI_STAIDX_0_MUL,
	OFI_STAIDX_1_MUL,
	OFI_STAIDX_2_MUL,
	OFI_STAIDX_3_MUL,
	OFI_STAIDX_4_MUL,
	OFI_STAIDX_5_MUL,
	OFI_STATICS_0_MUL,
	OFI_STATICS_1_MUL,
	OFI_STATICS_2_MUL,
	OFI_STATICS_3_MUL,
	OFI_STATICS_4_MUL,
	OFI_STATICS_5_MUL,
	OFI_MAP_DIF_0_MUL,
	OFI_MAP_DIF_1_MUL,
	OFI_MAP_DIF_2_MUL,
	OFI_MAP_DIF_3_MUL,
	OFI_MAP_DIF_4_MUL,
	OFI_MAP_DIF_5_MUL,
	OFI_MAP_DIFL_0_MUL,
	OFI_MAP_DIFL_1_MUL,
	OFI_MAP_DIFL_2_MUL,
	OFI_MAP_DIFL_3_MUL,
	OFI_MAP_DIFL_4_MUL,
	OFI_MAP_DIFL_5_MUL,
	OFI_STA_DIF_0_MUL,
	OFI_STA_DIF_1_MUL,
	OFI_STA_DIF_2_MUL,
	OFI_STA_DIF_3_MUL,
	OFI_STA_DIF_4_MUL,
	OFI_STA_DIF_5_MUL,
	OFI_STA_DIFI_0_MUL,
	OFI_STA_DIFI_1_MUL,
	OFI_STA_DIFI_2_MUL,
	OFI_STA_DIFI_3_MUL,
	OFI_STA_DIFI_4_MUL,
	OFI_STA_DIFI_5_MUL,
	OFI_STA_DIFL_0_MUL,
	OFI_STA_DIFL_1_MUL,
	OFI_STA_DIFL_2_MUL,
	OFI_STA_DIFL_3_MUL,
	OFI_STA_DIFL_4_MUL,
	OFI_STA_DIFL_5_MUL,
	OFI_TILEDATA_MUL,
	OFI_MULTI_IDX,
	OFI_MULTI_MUL,
	OFI_MULTI_UOP,
	OFI_HUES_MUL,
	OFI_VERDATA_MUL,
	OFI_CLILOC_MUL,
	OFI_RADARCOL_MUL,
	OFI_FILES_COUNT
};
//----------------------------------------------------------------------------------
enum ORION_GRAPHIC_DATA_TYPE
{
	OGDT_STATIC_ART = 1,
	OGDT_GUMP_ART
};
//----------------------------------------------------------------------------------
enum ORION_FEATURE_FLAGS
{
	OFF_DRAW_CHARACTERS_STATUS_IN_WORLD		= 0x00000001,
	OFF_CHANGE_TREES_TO_STUMPS				= 0x00000002,
	OFF_MARKING_CAVES						= 0x00000004,
	OFF_NO_VEGETATION						= 0x00000008,
	OFF_NO_FIELDS_ANIMATION					= 0x00000010,
	OFF_COLORED_CHARACTERS_STATE			= 0x00000020,
	OFF_TILED_FIELDS						= 0x00000040,
	OFF_DRAW_AURA							= 0x00000080,
	OFF_NO_DRAW_ROOFS						= 0x00000100,

	OFF_ALL_FLAGS							= 0xFFFFFFFF
};
//----------------------------------------------------------------------------------
enum VALUE_KEY_INT
{
	VKI_SOUND = 0,
	VKI_SOUND_VALUE,
	VKI_MUSIC,
	VKI_MUSIC_VALUE,
	VKI_USE_TOOLTIPS,
	VKI_ALWAYS_RUN,
	VKI_NEW_TARGET_SYSTEM,
	VKI_OBJECT_HANDLES,
	VKI_SCALE_SPEECH_DELAY,
	VKI_SPEECH_DELAY,
	VKI_IGNORE_GUILD_MESSAGES,
	VKI_IGNORE_ALLIANCE_MESSAGES,
	VKI_DARK_NIGHTS,
	VKI_COLORED_LIGHTING,
	VKI_CRIMINAL_ACTIONS_QUERY,
	VKI_CIRCLETRANS,
	VKI_CIRCLETRANS_VALUE,
	VKI_LOCK_RESIZING_GAME_WINDOW,
	VKI_CLIENT_FPS_VALUE,
	VKI_USE_SCALING_GAME_WINDOW,
	VKI_DRAW_STATUS_STATE,
	VKI_DRAW_STUMPS,
	VKI_MARKING_CAVES,
	VKI_NO_VEGETATION,
	VKI_NO_ANIMATE_FIELDS,
	VKI_STANDARD_CHARACTERS_DELAY,
	VKI_STANDARD_ITEMS_DELAY,
	VKI_LOCK_GUMPS_MOVING,
	VKI_CONSOLE_NEED_ENTER,
	VKI_HIDDEN_CHARACTERS_MODE,
	VKI_HIDDEN_CHARACTERS_ALPHA,
	VKI_HIDDEN_CHARACTERS_MODE_ONLY_FOR_SELF,
	VKI_TRANSPARENT_SPELL_ICONS,
	VKI_SPELL_ICONS_ALPHA,
	VKI_SKILLS_COUNT,
	VKI_SKILL_CAN_BE_USED,
	VKI_STATIC_ART_ADDRESS,
	VKI_USED_LAYER,
	VKI_SPELLBOOK_COUNT,
	VKI_BLOCK_MOVING,
	VKI_SET_PLAYER_GRAPHIC,
	VKI_FAST_ROTATION,
	VKI_IGNORE_STAMINA_CHECK,
	VKI_LAST_TARGET,
	VKI_LAST_ATTACK,
	VKI_NEW_TARGET_SYSTEM_SERIAL,
	VKI_GET_MAP_SIZE,
	VKI_GET_MAP_BLOCK_SIZE,
	VKI_MAP_MUL_ADDRESS,
	VKI_STATIC_IDX_ADDRESS,
	VKI_STATIC_MUL_ADDRESS,
	VKI_MAP_DIFL_ADDRESS,
	VKI_MAP_DIF_ADDRESS,
	VKI_STATIC_DIFL_ADDRESS,
	VKI_STATIC_DIFI_ADDRESS,
	VKI_STATIC_DIF_ADDRESS,
	VKI_VERDATA_ADDRESS,
	VKI_MAP_MUL_SIZE,
	VKI_STATIC_IDX_SIZE,
	VKI_STATIC_MUL_SIZE,
	VKI_MAP_DIFL_SIZE,
	VKI_MAP_DIF_SIZE,
	VKI_STATIC_DIFL_SIZE,
	VKI_STATIC_DIFI_SIZE,
	VKI_STATIC_DIF_SIZE,
	VKI_VERDATA_SIZE,
	VKI_MAP_UOP_ADDRESS,
	VKI_MAP_UOP_SIZE,
	VKI_MAP_X_UOP_ADDRESS,
	VKI_MAP_X_UOP_SIZE,
	VKI_CLILOC_ENU_ADDRESS,
	VKI_CLILOC_ENU_SIZE,
	VKI_GUMP_ART_ADDRESS,
	VKI_VIEW_RANGE
};
//----------------------------------------------------------------------------------
enum VALUE_KEY_STRING
{
	VKS_SKILL_NAME = 0,
	VKS_SERVER_NAME,
	VKS_CHARACTER_NAME,
	VKS_SPELLBOOK_1_SPELL_NAME,
	VKS_SPELLBOOK_2_SPELL_NAME,
	VKS_SPELLBOOK_3_SPELL_NAME,
	VKS_SPELLBOOK_4_SPELL_NAME,
	VKS_SPELLBOOK_5_SPELL_NAME,
	VKS_SPELLBOOK_6_SPELL_NAME,
	VKS_SPELLBOOK_7_SPELL_NAME
};
//----------------------------------------------------------------------------------
//!Типы текстовых сообщений
enum OBJECT_LAYERS
{
	OL_NONE = 0,	//0
	OL_1_HAND,		//1
	OL_2_HAND,		//2
	OL_SHOES,		//3
	OL_PANTS,		//4
	OL_SHIRT,		//5
	OL_HELMET,		//6
	OL_GLOVES,		//7
	OL_RING,		//8
	OL_TALISMAN,	//9
	OL_NECKLACE,	//10
	OL_HAIR,		//11
	OL_WAIST,		//12
	OL_TORSO,		//13
	OL_BRACELET,	//14
	OL_15,			//15
	OL_BEARD,		//16
	OL_TUNIC,		//17
	OL_EARRINGS,	//18
	OL_ARMS,		//19
	OL_CLOAK,		//20
	OL_BACKPACK,	//21
	OL_ROBE,		//22
	OL_SKIRT,		//23
	OL_LEGS,		//24
	OL_MOUNT,		//25
	OL_BUY_RESTOCK,	//26
	OL_BUY,			//27
	OL_SELL,		//28
	OL_BANK			//29
};
//----------------------------------------------------------------------------------
//!Типы текстовых сообщений
enum SPEECH_TYPE
{
	ST_NORMAL = 0,
	ST_BROADCAST,				//System
	ST_EMOTE = 0x02,			//Emote
	ST_SYSTEM = 0x06,			//System / Lower Corner
	ST_SYSTEM_WHITE_NAME = 0x07,//Message / Corner With Name
	ST_WHISPER = 0x08,			//Whisper
	ST_YELL = 0x09,				//Yell
	ST_SPELL = 0x0A,			//Spell
	ST_GUILD_CHAT = 0x0D,		//Guild Chat
	ST_ALLIANCE_CHAT = 0x0E,	//Alliance Chat
	ST_COMMAND_PROMT = 0x0F,	//Command Prompts
	ST_ENCODED_COMMAND = 0xC0,	//Encoded Commands
};
//----------------------------------------------------------------------------------
enum TEXT_TYPE
{
	TT_CLIENT = 0,
	TT_SYSTEM,
	TT_OBJECT
};
//----------------------------------------------------------------------------------
enum TEXT_ALIGN_TYPE
{
	TS_LEFT = 0,
	TS_CENTER,
	TS_RIGHT
};
//----------------------------------------------------------------------------------
enum HTML_TAG_TYPE
{
	HTT_NONE = 0,
	HTT_B,
	HTT_I,
	HTT_A,
	HTT_U,
	HTT_P,
	HTT_BIG,
	HTT_SMALL,
	HTT_BODY,
	HTT_BASEFONT,
	HTT_H1,
	HTT_H2,
	HTT_H3,
	HTT_H4,
	HTT_H5,
	HTT_H6,
	HTT_BR,
	HTT_BQ,
	HTT_LEFT,
	HTT_CENTER,
	HTT_RIGHT,
	HTT_DIV
};
//----------------------------------------------------------------------------------
//!Типы промптов
enum PROMPT_TYPE
{
	PT_NONE = 0,
	PT_ASCII,
	PT_UNICODE
};
//----------------------------------------------------------------------------------
enum MOUSE_WHEEL_STATE
{
	MWS_UP = 0,
	MWS_DOWN,
	MWS_DOUBLE_CLICK,
	MWS_SCROLL_UP,
	MWS_SCROLL_DOWN
};
//----------------------------------------------------------------------------------
enum DRAW_CHARACTERS_STATUS_STATE
{
	DCSS_NO_DRAW = 0,
	DCSS_ABOVE,
	DCSS_UNDER
};
//----------------------------------------------------------------------------------
enum DRAW_CHARACTERS_STATUS_CONDITION_STATE
{
	DCSCS_ALWAYS = 0,
	DCSCS_NOT_MAX,
	DCSCS_LOWER
};
//----------------------------------------------------------------------------------
enum HIDDEN_CHARACTERS_RENDER_MODE
{
	HCRM_ORIGINAL = 0,
	HCRM_ALPHA_BLENDING,
	HCRM_SPECTRAL_COLOR,
	HCRM_SPECIAL_SPECTRAL_COLOR
};
//----------------------------------------------------------------------------------
enum DRAW_AURA_STATE
{
	DAS_NEVER = 0,
	DAS_IN_WARMODE,
	DAS_ALWAYS
};
//----------------------------------------------------------------------------------
enum SCREENSHOT_FORMAT
{
	SF_BMP = 0,
	SF_PNG,
	SF_TIFF,
	SF_JPEG
};
//----------------------------------------------------------------------------------
enum OBJECT_PROPERTIES_MODE
{
	OPM_AT_ICON = 0,
	OPM_ALWAYS_UP,
	OPM_FOLLOW_MOUSE,
	OPM_SINGLE_CLICK
};
//----------------------------------------------------------------------------------
enum CHARACTER_BACKPACK_STYLE
{
	CBS_DEFAULT = 0,
	CBS_SUEDE,
	CBS_POLAR_BEAR,
	CBS_GHOUL_SKIN
};
//----------------------------------------------------------------------------------
enum SHADER_DRAW_MODE
{
	SDM_NO_COLOR = 0,
	SDM_COLORED = 1,
	SDM_PARTIAL_HUE = 2,
	SDM_TEXT_COLORED_NO_BLACK = 3,
	SDM_TEXT_COLORED = 4,
	SDM_LAND = 6,
	SDM_LAND_COLORED = 7,
	SDM_SPECTRAL = 10,
	SDM_SPECIAL_SPECTRAL = 11,
	SDM_SHADOW = 12
};
//----------------------------------------------------------------------------------
enum STATIC_TILES_FILTER_FLAGS
{
	STFF_CAVE			= 0x01,
	STFF_STUMP			= 0x02,
	STFF_STUMP_HATCHED	= 0x04,
	STFF_VEGETATION		= 0x08,
	STFF_WATER			= 0x10
};
//----------------------------------------------------------------------------------
enum ORION_INTERNAL_PACKET_MESSAGE_TYPE
{
	OIPMT_FILES_TRANSFERED = 1,
	OIPMT_FILE_INFO = 2,
	OIPMT_FILE_INFO_LOCALIZED = 3,
	OIPMT_GRAPHIC_DATA_INFO = 4,
	OIPMT_SKILL_LIST = 100,
	OIPMT_SPELL_LIST = 101,
	OIPMT_MACRO_LIST = 102,
	OIPMT_OPEN_MAP = 103
};
//----------------------------------------------------------------------------------
//!Направления чара
enum DIRECTION_TYPE
{
	DT_N = 0,
	DT_NE,
	DT_E,
	DT_SE,
	DT_S,
	DT_SW,
	DT_W,
	DT_NW
};
//----------------------------------------------------------------------------------
enum ANIMATION_GROUPS_TYPE
{
	AGT_MONSTER = 0,
	AGT_SEA_MONSTER,
	AGT_ANIMAL,
	AGT_HUMAN,
	AGT_EQUIPMENT,
	AGT_UNKNOWN
};
//----------------------------------------------------------------------------------
enum ANIMATION_GROUPS
{
	AG_NONE = 0,
	AG_LOW,
	AG_HIGHT,
	AG_PEOPLE
};
//----------------------------------------------------------------------------------
enum LOW_ANIMATION_GROUP
{
	LAG_WALK = 0,
	LAG_RUN,
	LAG_STAND,
	LAG_EAT,
	LAG_UNKNOWN,
	LAG_ATTACK_1,
	LAG_ATTACK_2,
	LAG_ATTACK_3,
	LAG_DIE_1,
	LAG_FIDGET_1,
	LAG_FIDGET_2,
	LAG_LIE_DOWN,
	LAG_DIE_2,

	LAG_ANIMATION_COUNT
};
//----------------------------------------------------------------------------------
enum HIGHT_ANIMATION_GROUP
{
	HAG_WALK = 0,
	HAG_STAND,
	HAG_DIE_1,
	HAG_DIE_2,
	HAG_ATTACK_1,
	HAG_ATTACK_2,
	HAG_ATTACK_3,
	HAG_MISC_1,
	HAG_MISC_2,
	HAG_MISC_3,
	HAG_STUMBLE,
	HAG_SLAP_GROUND,
	HAG_CAST,
	HAG_GET_HIT_1,
	HAG_MISC_4,
	HAG_GET_HIT_2,
	HAG_GET_HIT_3,
	HAG_FIDGET_1,
	HAG_FIDGET_2,
	HAG_FLY,
	HAG_LAND,
	HAG_DIE_IN_FLIGHT,

	HAG_ANIMATION_COUNT
};
//----------------------------------------------------------------------------------
enum PEOPLE_ANIMATION_GROUP
{
	PAG_WALK_UNARMED = 0,
	PAG_WALK_ARMED,
	PAG_RUN_UNARMED,
	PAG_RUN_ARMED,
	PAG_STAND,
	PAG_FIDGET_1,
	PAG_FIDGET_2,
	PAG_STAND_ONEHANDED_ATTACK,
	PAG_STAND_TWOHANDED_ATTACK,
	PAG_ATTACK_ONEHANDED,
	PAG_ATTACK_UNARMED_1,
	PAG_ATTACK_UNARMED_2,
	PAG_ATTACK_TWOHANDED_DOWN,
	PAG_ATTACK_TWOHANDED_WIDE,
	PAG_ATTACK_TWOHANDED_JAB,
	PAG_WALK_WARMODE,
	PAG_CAST_DIRECTED,
	PAG_CAST_AREA,
	PAG_ATTACK_BOW,
	PAG_ATTACK_CROSSBOW,
	PAG_GET_HIT,
	PAG_DIE_1,
	PAG_DIE_2,
	PAG_ONMOUNT_RIDE_SLOW,
	PAG_ONMOUNT_RIDE_FAST,
	PAG_ONMOUNT_STAND,
	PAG_ONMOUNT_ATTACK,
	PAG_ONMOUNT_ATTACK_BOW,
	PAG_ONMOUNT_ATTACK_CROSSBOW,
	PAG_ONMOUNT_SLAP_HORSE,
	PAG_TURN,
	PAG_ATTACK_UNARMED_AND_WALK,
	PAG_EMOTE_BOW,
	PAG_EMOTE_SALUTE,
	PAG_FIDGET_3,

	PAG_ANIMATION_COUNT
};
//----------------------------------------------------------------------------------
//!Злобность персонажа
enum NOTORIETY_TYPE
{
	NT_NONE = 0,
	NT_INNOCENT,
	NT_FRIENDLY,
	NT_SOMEONE_GRAY,
	NT_CRIMINAL,
	NT_ENEMY,
	NT_MURDERER,
	NT_INVULNERABLE
};
//----------------------------------------------------------------------------------
enum CLICK_OBJECT_TYPE
{
	COT_NONE = 0,
	COT_GAME_OBJECT,
	COT_STATIC,
	COT_GUMP
};
//----------------------------------------------------------------------------------
enum CUSTOM_HOUSE_UPDATE_TYPE
{
	CHUT_UPDATE = 1,
	CHUT_REMOVE,
	CHUT_UPDATE_MULTI_POS,
	CHUT_CONSTRUCT_BEGIN,
	CHUT_CONSTRUCT_END
};
//----------------------------------------------------------------------------------
enum MACRO_CODE
{
	MC_NONE = 0,
	MC_SAY,
	MC_EMOTE,
	MC_WHISPER,
	MC_YELL,
	MC_WALK,
	MC_WAR_PEACE,
	MC_PASTE,
	MC_OPEN,
	MC_CLOSE,
	MC_MINIMIZE,
	MC_MAXIMIZE,
	MC_OPEN_DOOR,
	MC_USE_SKILL,
	MC_LAST_SKILL,
	MC_CAST_SPELL,
	MC_LAST_SPELL,
	MC_LAST_OBJECT,
	MC_BOW,
	MC_SALUTE,
	MC_QUIT_GAME,
	MC_ALL_NAMES,
	MC_LAST_TARGET,
	MC_TARGET_SELF,
	MC_ARM_DISARM,
	MC_WAIT_FOR_TARGET,
	MC_TARGET_NEXT,
	MC_ATTACK_LAST,
	MC_DELAY,
	MC_CIRCLE_TRANS,
	MC_CLOSE_GUMPS,
	MC_ALWAYS_RUN,
	MC_SAVE_DESKTOP,
	MC_KILL_GUMP_OPEN,
	MC_PRIMARY_ABILITY,
	MC_SECONDARY_ABILITY,
	MC_EQUIP_LAST_WEAPON,
	MC_SET_UPDATE_RANGE,
	MC_MODIFY_UPDATE_RANGE,
	MC_INCREASE_UPDATE_RANGE,
	MC_DECREASE_UPDATE_RANGE,
	MC_MAX_UPDATE_RANGE,
	MC_MIN_UPDATE_RANGE,
	MC_DEFAULT_UPDATE_RANGE,
	MC_UPDATE_RANGE_INFO,
	MC_ENABLE_RANGE_COLOR,
	MC_DISABLE_RANGE_COLOR,
	MC_TOGGLE_RANGE_COLOR,
	MC_INVOKE_VIRTURE,
	MC_SELECT_NEXT,
	MC_SELECT_PREVEOUS,
	MC_SELECT_NEAREST,
	MC_ATTACK_SELECTED_TARGET,
	MC_USE_SELECTED_TARGET,
	MC_CURRENT_TARGET,
	MC_TARGET_SYSTEM_ON_OFF,
	MC_TOGGLE_BUICON_WINDOW,
	MC_BANDAGE_SELF,
	MC_BANDAGE_TARGET,
	MC_TOGGLE_GARGOYLE_FLYING
};
//----------------------------------------------------------------------------------
enum MACRO_SUB_CODE
{
	MSC_NONE = 0,
	MSC_G1_NW, //Walk group
	MSC_G1_N,
	MSC_G1_NE,
	MSC_G1_E,
	MSC_G1_SE,
	MSC_G1_S,
	MSC_G1_SW,
	MSC_G1_W,
	MSC_G2_CONFIGURATION, //Open/Close/Minimize/Maximize group
	MSC_G2_PAPERDOLL,
	MSC_G2_STATUS,
	MSC_G2_JOURNAL,
	MSC_G2_SKILLS,
	MSC_G2_MAGE_SPELLBOOK,
	MSC_G2_CHAT,
	MSC_G2_BACKPACK,
	MSC_G2_OWERVIEW,
	MSC_G2_WORLD_MAP,
	MSC_G2_MAIL,
	MSC_G2_PARTY_MANIFEST,
	MSC_G2_PARTY_CHAT,
	MSC_G2_NECRO_SPELLBOOK,
	MSC_G2_PALADIN_SPELLBOOK,
	MSC_G2_COMBAT_BOOK,
	MSC_G2_BUSHIDO_SPELLBOOK,
	MSC_G2_NINJITSU_SPELLBOOK,
	MSC_G2_GUILD,
	MSC_G2_SPELL_WEAVING_SPELLBOOK,
	MSC_G2_QUEST_LOG,
	MSC_G2_MYSTICISM_SPELLBOOK,
	MSC_G2_RACIAL_ABILITIES_BOOK,
	MSC_G2_BARD_SPELLBOOK,
	MSC_G3_ANATOMY, //Skills group
	MSC_G3_ANIMAL_LORE,
	MSC_G3_ANIMAL_TAMING,
	MSC_G3_ARMS_LORE,
	MSC_G3_BEGGING,
	MSC_G3_CARTOGRAPHY,
	MSC_G3_DETECTING_HIDDEN,
	MSC_G3_ENTICEMENT,
	MSC_G3_EVALUATING_INTELLIGENCE,
	MSC_G3_FORENSIC_EVALUATION,
	MSC_G3_HIDING,
	MSC_G3_IMBUING,
	MSC_G3_INSCRIPTION,
	MSC_G3_ITEM_IDENTIFICATION,
	MSC_G3_MEDITATION,
	MSC_G3_PEACEMAKING,
	MSC_G3_POISONING,
	MSC_G3_PROVOCATION,
	MSC_G3_REMOVE_TRAP,
	MSC_G3_SPIRIT_SPEAK,
	MSC_G3_STEALING,
	MSC_G3_STEALTH,
	MSC_G3_TASTE_IDENTIFICATION,
	MSC_G3_TRACKING,
	MSC_G4_LEFT_HAND, ///Arm/Disarm group
	MSC_G4_RIGHT_HAND,
	MSC_G5_HONOR, //Invoke Virture group
	MSC_G5_SACRIFICE,
	MSC_G5_VALOR,
	MSC_G6_CLUMSY, //Cast Spell group
	MSC_G6_CREATE_FOOD,
	MSC_G6_FEEBLEMIND,
	MSC_G6_HEAL,
	MSC_G6_MAGIC_ARROW,
	MSC_G6_NIGHT_SIGHT,
	MSC_G6_REACTIVE_ARMOR,
	MSC_G6_WEAKEN,
	MSC_G6_AGILITY,
	MSC_G6_CUNNING,
	MSC_G6_CURE,
	MSC_G6_HARM,
	MSC_G6_MAGIC_TRAP,
	MSC_G6_MAGIC_UNTRAP,
	MSC_G6_PROTECTION,
	MSC_G6_STRENGTH,
	MSC_G6_BLESS,
	MSC_G6_FIREBALL,
	MSC_G6_MAGIC_LOCK,
	MSC_G6_POISON,
	MSC_G6_TELEKINESIS,
	MSC_G6_TELEPORT,
	MSC_G6_UNLOCK,
	MSC_G6_WALL_OF_STONE,
	MSC_G6_ARCH_CURE,
	MSC_G6_ARCH_PROTECTION,
	MSC_G6_CURSE,
	MSC_G6_FIRE_FIELD,
	MSC_G6_GREATER_HEAL,
	MSC_G6_LIGHTNING,
	MSC_G6_MANA_DRAIN,
	MSC_G6_RECALL,
	MSC_G6_BLADE_SPIRITS,
	MSC_G6_DISPELL_FIELD,
	MSC_G6_INCOGNITO,
	MSC_G6_MAGIC_REFLECTION,
	MSC_G6_MIND_BLAST,
	MSC_G6_PARALYZE,
	MSC_G6_POISON_FIELD,
	MSC_G6_SUMMON_CREATURE,
	MSC_G6_DISPEL,
	MSC_G6_ENERGY_BOLT,
	MSC_G6_EXPLOSION,
	MSC_G6_INVISIBILITY,
	MSC_G6_MARK,
	MSC_G6_MASS_CURSE,
	MSC_G6_PARALYZE_FIELD,
	MSC_G6_REVEAL,
	MSC_G6_CHAIN_LIGHTNING,
	MSC_G6_ENERGY_FIELD,
	MSC_G6_FLAME_STRIKE,
	MSC_G6_GATE_TRAVEL,
	MSC_G6_MANA_VAMPIRE,
	MSC_G6_MASS_DISPEL,
	MSC_G6_METEOR_SWARM,
	MSC_G6_POLYMORPH,
	MSC_G6_EARTHQUAKE,
	MSC_G6_ENERGY_VORTEX,
	MSC_G6_RESURRECTION,
	MSC_G6_AIR_ELEMENTAL,
	MSC_G6_SUMMON_DAEMON,
	MSC_G6_EARTH_ELEMENTAL,
	MSC_G6_FIRE_ELEMENTAL,
	MSC_G6_WATER_ELEMENTAL,
	MSC_G6_ANIMATE_DEAD,
	MSC_G6_BLOOD_OATH,
	MSC_G6_CORPSE_SKIN,
	MSC_G6_CURSE_WEAPON,
	MSC_G6_EVIL_OMEN,
	MSC_G6_HORRIFIC_BEAST,
	MSC_G6_LICH_FORM,
	MSC_G6_MIND_ROT,
	MSC_G6_PAIN_SPIKE,
	MSC_G6_POISON_STRIKE,
	MSC_G6_STRANGLE,
	MSC_G6_SUMMON_FAMILAR,
	MSC_G6_VAMPIRIC_EMBRACE,
	MSC_G6_VENGEFUL_SPIRIT,
	MSC_G6_WITHER,
	MSC_G6_WRAITH_FORM,
	MSC_G6_EXORCISM,
	MSC_G6_CLEANCE_BY_FIRE,
	MSC_G6_CLOSE_WOUNDS,
	MSC_G6_CONSECRATE_WEAPON,
	MSC_G6_DISPEL_EVIL,
	MSC_G6_DIVINE_FURY,
	MSC_G6_ENEMY_OF_ONE,
	MSC_G6_HOLY_LIGHT,
	MSC_G6_NOBLE_SACRIFICE,
	MSC_G6_REMOVE_CURSE,
	MSC_G6_SACRED_JOURNEY,
	MSC_G6_HONORABLE_EXECUTION,
	MSC_G6_CONFIDENCE,
	MSC_G6_EVASION,
	MSC_G6_COUNTER_ATTACK,
	MSC_G6_LIGHTING_STRIKE,
	MSC_G6_MOMENTUM_STRIKE,
	MSC_G6_FOCUS_ATTACK,
	MSC_G6_DEATH_STRIKE,
	MSC_G6_ANIMAL_FORM,
	MSC_G6_KI_ATTACK,
	MSC_G6_SURPRICE_ATTACK,
	MSC_G6_BACKSTAB,
	MSC_G6_SHADOWJUMP,
	MSC_G6_MIRROR_IMAGE,
	MSC_G6_ARCANE_CIRCLE,
	MSC_G6_GIFT_OF_RENEWAL,
	MSC_G6_IMMOLATING_WEAPON,
	MSC_G6_ATTUNEMENT,
	MSC_G6_THUNDERSTORM,
	MSC_G6_NATURES_FURY,
	MSC_G6_SUMMON_FEY,
	MSC_G6_SUMMON_FIEND,
	MSC_G6_REAPER_FORM,
	MSC_G6_WILDFIRE,
	MSC_G6_ESSENCE_OF_WIND,
	MSC_G6_DRYAD_ALLURE,
	MSC_G6_ETHEREAL_VOYAGE,
	MSC_G6_WORD_OF_DEATH,
	MSC_G6_GIFT_OF_LIFE,
	MSC_G6_ARCANE_EMPOWERMEN,
	MSC_G6_NETHER_BOLT,
	MSC_G6_HEALING_STONE,
	MSC_G6_PURGE_MAGIC,
	MSC_G6_ENCHANT,
	MSC_G6_SLEEP,
	MSC_G6_EAGLE_STRIKE,
	MSC_G6_ANIMATED_WEAPON,
	MSC_G6_STONE_FORM,
	MSC_G6_SPELL_TRIGGER,
	MSC_G6_MASS_SLEEP,
	MSC_G6_CLEANSING_WINDS,
	MSC_G6_BOMBARD,
	MSC_G6_SPELL_PLAGUE,
	MSC_G6_HAIL_STORM,
	MSC_G6_NETHER_CYCLONE,
	MSC_G6_RISING_COLOSSUS,
	MSC_G6_INSPIRE,
	MSC_G6_INVIGORATE,
	MSC_G6_RESILIENCE,
	MSC_G6_PERSEVERANCE,
	MSC_G6_TRIBULATION,
	MSC_G6_DESPAIR,
	MSC_G7_HOSTILE, //Select Next/Preveous/Nearest group
	MSC_G7_PARTY,
	MSC_G7_FOLLOWER,
	MSC_G7_OBJECT,
	MSC_G7_MOBILE,
	MSC_TOTAL_COUNT
};
//----------------------------------------------------------------------------------
enum MACRO_RETURN_CODE
{
	MRC_PARSE_NEXT = 0,
	MRC_BREAK_PARSER,
	MRC_STOP
};
//----------------------------------------------------------------------------------
enum GAME_CONSOLE_TEXT_TYPE
{
	GCTT_NORMAL = 0,
	GCTT_YELL,
	GCTT_WHISPER,
	GCTT_EMOTE,
	GCTT_C,
	GCTT_BROADCAST,
	GCTT_PARTY,
	GCTT_GUILD,
	GCTT_ALLIANCE,
	GCTT_PARTY_ACCEPT,
	GCTT_PARTY_DECLINE,
	GCTT_PARTY_ADD,
	GCTT_PARTY_LEAVE
};
//----------------------------------------------------------------------------------
enum SEASON_TYPE
{
	ST_SPRING = 0,
	ST_SUMMER,
	ST_FALL,
	ST_WINTER,
	ST_DESOLATION
};
//----------------------------------------------------------------------------------
enum ABILITY_TYPE
{
	AT_NONE = 0,
	AT_ARMOR_IGNORE,
	AT_BLEED_ATTACK,
	AT_CONCUSSION_BLOW,
	AT_CRUSHING_BLOW,
	AT_DISARM,
	AT_DISMOUNT,
	AT_DOUBLE_STRIKE,
	AT_INFECTING,
	AT_MORTAL_STRIKE,
	AT_MOVING_SHOT,
	AT_PARALYZING_BLOW,
	AT_SHADOW_STRIKE,
	AT_WHIRLWIND_ATTACK,
	AT_RIDING_SWIPE,
	AT_FRENZIED_WHIRLWIND,
	AT_BLOCK,
	AT_DEFENSE_MASTERY,
	AT_NERVE_STRIKE,
	AT_TALON_STRIKE,
	AT_FEINT,
	AT_DUAL_WIELD,
	AT_DOUBLE_SHOT,
	AT_ARMOR_PIERCE,
	AT_BLADEWEAVE,
	AT_FORCE_ARROW,
	AT_LIGHTNING_ARROW,
	AT_PSYCHIC_ATTACK,
	AT_SERPENT_ARROW,
	AT_FORCE_OF_NATURE,
	AT_INFUSED_THROW,
	AT_MYSTIC_ARC,
	AT_DISROBE
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
