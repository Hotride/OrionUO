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
//Данные о местонахождении сжатого блока данных с уоп фреймами
struct UOPAnimationData
{
	std::string *path;
	unsigned int offset;
	unsigned int compressedLength;
	unsigned int decompressedLength;
	std::fstream *fileStream;
};
//----------------------------------------------------------------------------------
//Класс для работы с текстурой кадра анимации
class CTextureAnimationFrame
{
	//Координаты центра текстуры относительно начала текстуры
	SETGET(short, CenterX);
	SETGET(short, CenterY);


public:
	CTextureAnimationFrame();
	virtual ~CTextureAnimationFrame();

	CGLTexture m_Texture;
	vector<bool> m_PixelData;
};
//----------------------------------------------------------------------------------
//Класс для работы с направлением анимации
class CTextureAnimationDirection
{
	//Количество кадров
	SETGET(uchar, FrameCount);
	//Адрес в памяти (*.mul / *.uop)
	SETGET(uint, BaseAddress);
	//Размер данных в памяти (*.mul / *.uop)
	SETGET(uint, BaseSize);
	//Адрес в памяти (Bodyconv.def)
	SETGET(uint, PatchedAddress);
	//Размер данных в памяти (Bodyconv.def)
	SETGET(uint, PatchedSize);
	//Индекс файла с анимацией
	SETGET(int, FileIndex);
	//Адрес в памяти
	SETGET(uint, Address);
	//Размер данных в памяти
	SETGET(uint, Size);
	//Время последнего доступа
	SETGET(uint, LastAccessTime);
	//UOP animation indicator
	SETGET(bool, IsUOP);

public:
	CTextureAnimationDirection();
	virtual ~CTextureAnimationDirection();

	CTextureAnimationFrame *m_Frames;
};
//----------------------------------------------------------------------------------
//Класс для работы с группой анимаций
class CTextureAnimationGroup
{
	//UOP animation indicator
	SETGET(bool, IsUOP);
public:
	CTextureAnimationGroup();
	virtual ~CTextureAnimationGroup();

	CTextureAnimationDirection m_Direction[5];
	UOPAnimationData m_UOPAnimData;
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
