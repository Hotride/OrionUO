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

	//Координаты центра текстуры относительно начала текстуры
	SETGET(short, CenterX);
	SETGET(short, CenterY);

public:
	CTextureAnimationFrame(const int &frame);
	virtual ~CTextureAnimationFrame();

	CGLTexture m_Texture;
};
//----------------------------------------------------------------------------------
//Класс для работы с направлением анимации
class CTextureAnimationDirection : public CBaseQueue
{
	//Количество кадров
	SETGET(uchar, FrameCount);
	//Адрес в памяти
	SETGET(uint, Address);
	//Размер данных в памяти
	SETGET(uint, Size);
	//Время последнего доступа
	SETGET(uint, LastAccessTime);

public:
	CTextureAnimationDirection();
	virtual ~CTextureAnimationDirection();

	//Получить ссылку на фрэйм
	CTextureAnimationFrame *GetFrame(const uchar &frame);

	//Найти ссылку на фрэйм
	CTextureAnimationFrame *FindFrame(const uchar &frame);
};
//----------------------------------------------------------------------------------
//Класс для работы с группой анимаций
class CTextureAnimationGroup
{
public:
	CTextureAnimationGroup();
	virtual ~CTextureAnimationGroup();

	CTextureAnimationDirection m_Direction[5];
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
