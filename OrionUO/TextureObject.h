/***********************************************************************************
**
** TextureObject.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef TEXTUREOBJECT_H
#define TEXTUREOBJECT_H
//----------------------------------------------------------------------------------
#include "BaseQueue.h"
#include "Globals.h"
//----------------------------------------------------------------------------------
//Класс для работы с текстурой кадра анимации
class CTextureAnimationFrame : public CBaseQueueItem
{
	//Индекс кадра
	SETGET(uchar, Frame);
	//Габариты текстуры
	SETGET(short, Width);
	SETGET(short, Height);
	//Координаты центра текстуры относительно начала текстуры
	SETGET(short, CenterX);
	SETGET(short, CenterY);

public:
	CTextureAnimationFrame(const int &frame);
	virtual ~CTextureAnimationFrame();

	//Индекс текстуры
	GLuint Texture;
};
//----------------------------------------------------------------------------------
//Класс для работы с направлением анимации
class CTextureAnimationDirection : public CBaseQueueItem
{
	//Направление
	SETGET(uchar, Direction);
	//Количество кадров
	SETGET(uchar, FrameCount);
	//Адрес в памяти
	SETGET(uint, Address);
	//Размер данных в памяти
	SETGET(uint, Size);
	//Время последнего доступа
	SETGET(uint, LastAccessTime);

public:
	CTextureAnimationDirection(int direction);
	virtual ~CTextureAnimationDirection();

	//Получить ссылку на фрэйм
	CTextureAnimationFrame *GetFrame(const uchar &frame);

	//Найти ссылку на фрэйм
	CTextureAnimationFrame *FindFrame(const uchar &frame);
};
//----------------------------------------------------------------------------------
//Класс для работы с группой анимаций
class CTextureAnimationGroup : public CBaseQueueItem
{
	//Индекс группы
	SETGET(uchar, Index);

public:
	CTextureAnimationGroup(int index);
	virtual ~CTextureAnimationGroup();

	//Получить ссылку на направление
	CTextureAnimationDirection *GetDirection(const uchar &direction);
};
//----------------------------------------------------------------------------------
//Класс для работы с группами текстур анимаций
class CTextureAnimation : public CBaseQueue
{
public:
	CTextureAnimation();
	virtual ~CTextureAnimation();

	//Получить ссылку на группу
	CTextureAnimationGroup *GetGroup(const uchar &index);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
