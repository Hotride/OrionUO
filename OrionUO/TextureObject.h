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
	std::string path;
	unsigned int offset;
	unsigned int compressedLength;
	unsigned int decompressedLength;
	std::fstream *fileStream;
};
//----------------------------------------------------------------------------------
//Класс для работы с текстурой кадра анимации
class CTextureAnimationFrame : public CGLTexture
{
public:
	//Координаты центра текстуры относительно начала текстуры
	short CenterX = 0;
	short CenterY = 0;

	CTextureAnimationFrame();
	virtual ~CTextureAnimationFrame();
};
//----------------------------------------------------------------------------------
//Класс для работы с направлением анимации
class CTextureAnimationDirection
{
public:
	//Количество кадров
	uchar FrameCount = 0;
	//Адрес в памяти (*.mul / *.uop)
	size_t BaseAddress = 0;
	//Размер данных в памяти (*.mul / *.uop)
	uint BaseSize = 0;
	//Адрес в памяти (Bodyconv.def)
	size_t PatchedAddress = 0;
	//Размер данных в памяти (Bodyconv.def)
	uint PatchedSize = 0;
	//Индекс файла с анимацией
	int FileIndex = 0;
	//Адрес в памяти
	size_t Address = 0;
	//Размер данных в памяти
	uint Size = 0;
	//Время последнего доступа
	uint LastAccessTime = 0;
	//UOP animation indicator
	bool IsUOP = false;
	//Data is within verdata file
	bool IsVerdata = false;

	CTextureAnimationDirection();
	virtual ~CTextureAnimationDirection();

	CTextureAnimationFrame *m_Frames;
};
//----------------------------------------------------------------------------------
//Класс для работы с группой анимаций
class CTextureAnimationGroup
{
public:
	CTextureAnimationGroup();
	virtual ~CTextureAnimationGroup();

	CTextureAnimationDirection m_Direction[5];
	UOPAnimationData m_UOPAnimData;
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
