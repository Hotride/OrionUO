/****************************************************************************
**
** DefinitionMacro.h
**
** Copyright (C) December 2015 Hotride
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
#ifndef DefinitionMacroH
#define DefinitionMacroH
//---------------------------------------------------------------------------
#define UO_DEBUG_INFO 1
//---------------------------------------------------------------------------
//Включить/выключить логгер.
//Логгер будет работать в любом случае, но если эта опция выключена
//то он не будет писать в лог дампы пакетов и всякую отладочкую информацию
//Если значение равно 2 - будет писать отладочную инфу, но без дампов пакетов
//Без пакетов от нее толку мало...

#define UOLOGGER 1

#if UOLOGGER != 0
	#define EPRINT error_printf
	#define WPRINT warning_printf
	#define TPRINT trace_printf
	#define T_TPRINT trace_printf
	#if UOLOGGER == 2
		#define TDUMP ;/##/
	#else //UOLOGGER != 2
		#define TDUMP trace_dump
	#endif //UOLOGGER == 2
#else //UOLOGGER == 0
	#define EPRINT ;/##/
	#define WPRINT ;/##/
	#define TPRINT ;/##/
	#define TDUMP ;/##/
#endif //UOLOGGER!=0
//---------------------------------------------------------------------------
//Эффекты шрифта
#define UOFONT_SOLID			0x01 //Жирный
#define UOFONT_ITALIC			0x02 //Наклонный
#define UOFONT_INDENTION		0x04 //Отступ текста (кроме 1 строки) для журнала
#define UOFONT_BLACK_BORDER		0x08 //Черная обводка
#define UOFONT_UNDERLINE		0x10 //Подчеркнутый
#define UOFONT_FIXED			0x20 //Фиксированной длины
#define UOFONT_CROPPED			0x40 //Фиксированной длины с многоточием
//---------------------------------------------------------------------------
#define PLUGIN_FLAGS_PARSE_RECV		0x01
#define PLUGIN_FLAGS_PARSE_SEND		0x02
#define PLUGIN_FLAGS_RECV		0x04
#define PLUGIN_FLAGS_SEND		0x08
#define PLUGIN_FLAGS_WINDOW_PROC	0x10
#define PLUGIN_FLAGS_CLIENT_ACCESS	0x20
#define PLUGIN_FLAGS_GAME_WORLD_DRAW	0x40
#define PLUGIN_FLAGS_SCENE_DRAW		0x80
//---------------------------------------------------------------------------
//Connection Errors
#define CE_NO_ERROR					0
#define CE_INIT						-1
#define CE_CREATE					-2
#define CE_NOT_CONNECTED			-3
#define CE_CONNECTED				-4
#define CE_NO_SOCKET				-5
#define CE_HOST						-6
#define CE_CONNECT_ERROR			-7
#define CE_RECV_ERROR				-8
#define CE_RECV_SIZE_ERROR			-9
//---------------------------------------------------------------------------
#define DCLICK_DELAY				350
#define SCROLL_LISTING_DELAY		150
#define CHANGE_SHOP_COUNT_DELAY		50
#define CHANGE_MACRO_DELAY			100
#define CHANGE_MACRO_LIST_DELAY		500
//---------------------------------------------------------------------------
//Задержка между отправкой последнего пакета и Ping (0x73) пакетом, 55 секунд
#define SEND_TIMEOUT_DELAY			55000

#define GUMP_MENU_PIXEL_STEP		5

#define GUMP_SCROLLING_PIXEL_STEP	10

#define CLEAR_TEXTURES_DELAY		30000

#define RANDOM_FIDGET_ANIMATION_DELAY		(30000 + (rand() % 5000))

#define WEATHER_TIMER				(6 * 60 * 1000)

#define WAIT_FOR_TARGET_DELAY		2000
//---------------------------------------------------------------------------
#define MODKEY_ALT					0x0100
#define MODKEY_CTRL					0x0200
#define MODKEY_SHIFT				0x0400
//---------------------------------------------------------------------------
//incremented ordinary for
#define IFOR(var, value, target) for (int var = value; var < target; var ++)
//decremented ordinary for
#define DFOR(var, value, target) for (int var = value; var >= target; var --)
//TBaseQueue for [extended]
#define QFOR(var, src, type) for (type var = (type)src->Begin(); var != NULL; var = (type)src->Next())
//TBaseQueue for
#define BQFOR(var, src) QFOR(var, src, TBaseQueueItem*)
//---------------------------------------------------------------------------
//Set/Get ordinary class property
#define SETGET(type, name) \
	void Set##name(type val) { m_##name = val; } \
	type Get##name() const { return m_##name; } \
	__declspec(property(get = Get##name, put = Set##name)) type name;
//---------------------------------------------------------------------------
//Set/Get ordinary class property
#define SETGETEX(type, name) \
	void Set##name(type val) { m_##name = val; } \
	type Get##name() const { return m_##name; } \
	void Inc##name() { m_##name++; } \
	void Dec##name() { m_##name--; } \
	void Add##name(type val) { m_##name += val; } \
	__declspec(property(get = Get##name, put = Set##name)) type name;
//---------------------------------------------------------------------------
//Get ordinary class property customized
#define GET_PARTY(type, name) \
	type Get##name(); \
	__declspec(property(get = Get##name)) type name;
//---------------------------------------------------------------------------
#define CMP_RANGE(name, id1, id2) (name >= id1 && name <= id2)
//---------------------------------------------------------------------------
// GCC_NORETURN means the function never returns
// GCC_PRINTF means the function has printf-style arguments
#ifdef __GNUC__
#   define GCC_NORETURN __attribute__((noreturn))
#   define GCC_PRINTF(n,m) __attribute__((format (printf, n, m)))
#else
#   define GCC_NORETURN
#   define GCC_PRINTF(n,m)
#endif
//---------------------------------------------------------------------------
#endif