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
	SETGET(short, CenterX, 0);
	SETGET(short, CenterY, 0);

public:
	CTextureAnimationFrame();
	virtual ~CTextureAnimationFrame();

	CGLTexture m_Texture;
};
//----------------------------------------------------------------------------------
//Класс для работы с направлением анимации
class CTextureAnimationDirection
{
	//Количество кадров
	SETGET(uchar, FrameCount, 0);
	//Адрес в памяти (*.mul / *.uop)
	SETGET(uint, BaseAddress, 0);
	//Размер данных в памяти (*.mul / *.uop)
	SETGET(uint, BaseSize, 0);
	//Адрес в памяти (Bodyconv.def)
	SETGET(uint, PatchedAddress, 0);
	//Размер данных в памяти (Bodyconv.def)
	SETGET(uint, PatchedSize, 0);
	//Индекс файла с анимацией
	SETGET(int, FileIndex, 0);
	//Адрес в памяти
	SETGET(uint, Address, 0);
	//Размер данных в памяти
	SETGET(uint, Size, 0);
	//Время последнего доступа
	SETGET(uint, LastAccessTime, 0);
	//UOP animation indicator
	SETGET(bool, IsUOP, false);

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
	SETGET(bool, IsUOP, false);

public:
	CTextureAnimationGroup();
	virtual ~CTextureAnimationGroup();

	CTextureAnimationDirection m_Direction[5];
	UOPAnimationData m_UOPAnimData;
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
