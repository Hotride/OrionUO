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
#define UO_UNUSE_WALK_STACK 1

#define UO_DEPTH_TEST 0
#define UO_RENDER_LIST_SORT 1
#define UO_CHECKERBOARD_SEQUENCE_RENDER_LIST 1
//---------------------------------------------------------------------------
#define UO_DEBUG_INFO 1
//---------------------------------------------------------------------------
#define ADD_LINKED(list, item) \
TLinkedList *nextList = list; \
list = new TLinkedList(&item); \
	list->Next = nextList
//---------------------------------------------------------------------------
#define UOMSG_PROCESS			WM_USER + 900
//---------------------------------------------------------------------------
//!Эффекты шрифта
#define UOFONT_SOLID			0x01 //!Жирный
#define UOFONT_ITALIC			0x02 //!Наклонный
#define UOFONT_INDENTION		0x04 //!Отступ текста (кроме 1 строки) для журнала
#define UOFONT_BLACK_BORDER		0x08 //!Черная обводка
#define UOFONT_UNDERLINE		0x10 //!Подчеркнутый
#define UOFONT_FIXED			0x20 //!Фиксированной длины
#define UOFONT_CROPPED			0x40 //!Фиксированной длины с многоточием
//---------------------------------------------------------------------------
#define PLUGIN_FLAGS_PARSE_RECV			0x01
#define PLUGIN_FLAGS_PARSE_SEND			0x02
#define PLUGIN_FLAGS_RECV				0x04
#define PLUGIN_FLAGS_SEND				0x08
#define PLUGIN_FLAGS_WINDOW_PROC		0x10
#define PLUGIN_FLAGS_CLIENT_ACCESS		0x20
#define PLUGIN_FLAGS_GAME_WORLD_DRAW	0x40
#define PLUGIN_FLAGS_SCENE_DRAW			0x80
//---------------------------------------------------------------------------
//!Коды ошибок подключения
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
#define RANDOM_FIDGET_ANIMATION_DELAY		(30000 + (rand() % 5000))
//---------------------------------------------------------------------------
#define MODKEY_ALT					0x0100
#define MODKEY_CTRL					0x0200
#define MODKEY_SHIFT				0x0400
//---------------------------------------------------------------------------
//!incremented ordinary for
#define IFOR(var, start, stop) for (int var = start; var < stop; var ++)
//!decremented ordinary for
#define DFOR(var, start, stop) for (int var = start; var >= stop; var --)
//!TBaseQueue for [extended]
#define QFOR(var, start, type) for (type var = (type)start; var != NULL; var = (type)var->m_Next)
//!TBaseQueue for
#define BQFOR(var, start) QFOR(var, start, TBaseQueueItem*)
//---------------------------------------------------------------------------
//!Set/Get ordinary class property
#define SETGET(type, name) \
	inline void Set##name(type val) { m_##name = val; } \
	inline type Get##name() const { return m_##name; } \
	__declspec(property(get = Get##name, put = Set##name)) type name
//---------------------------------------------------------------------------
//!Set/Get ordinary class property
#define SETGETEX(type, name) \
	SETGET(type, name); \
	inline void Inc##name() { m_##name++; } \
	inline void Dec##name() { m_##name--; } \
	inline void Add##name(type val) { m_##name += val; }
//---------------------------------------------------------------------------
//!Get ordinary class property customized
#define GET_PARTY(type, name) \
	type Get##name(); \
	__declspec(property(get = Get##name)) type name
//---------------------------------------------------------------------------
#define CMP_RANGE(name, id1, id2) (name >= id1 && name <= id2)
//---------------------------------------------------------------------------
#endif