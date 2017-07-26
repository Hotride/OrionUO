/***********************************************************************************
**
** TextContainer.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef TEXTCONTAINER_H
#define TEXTCONTAINER_H
//----------------------------------------------------------------------------------
#include "TextData.h"
#include "TextData.h"
//----------------------------------------------------------------------------------
//Контейнер для текста
class CTextContainer : public CBaseQueue
{
	SETGET(int, MaxSize, 0);
	SETGET(int, Size, 0);

public:
	CTextContainer(const int &maxSize);
	~CTextContainer();

	//Добавить текст
	void Add(CTextData *obj);

	//Удалить текст
	void Delete(CTextData *obj);

	//Очистить контейнер
	virtual void Clear();

	//Отрисовать системный чат
	void DrawSystemChat(const int &x, const int &y, const int &height);
};
//----------------------------------------------------------------------------------
extern CTextContainer g_SystemChat;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
