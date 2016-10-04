/***********************************************************************************
**
** IndexObject.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef INDEXOBJECT_H
#define INDEXOBJECT_H
//----------------------------------------------------------------------------------
#include "Wisp/WispGlobal.h"
#include "Wisp/WispGeometry.h"
#include "GLEngine/GLTexture.h"
#include "bass.h"

#pragma comment(lib, "bass.lib")
//----------------------------------------------------------------------------------
class CIndexObject
{
	SETGET(uint, Address);
	SETGET(int, DataSize);
	SETGET(int, Width);
	SETGET(int, Height);
	SETGET(ushort, Graphic);
	SETGET(ushort, Color);
	SETGET(uint, LastAccessTime);

public:
	CIndexObject();
	virtual ~CIndexObject();

	CGLTexture *Texture;
};
//----------------------------------------------------------------------------------
class CIndexObjectStatic : public CIndexObject
{
	SETGET(ushort, Index);
	SETGET(char, Offset);
	SETGET(char, AnimIndex);
	SETGET(uint, ChangeTime);
	SETGET(ushort, LightColor);
	SETGET(bool, IsFiled);

public:
	CIndexObjectStatic();
	virtual ~CIndexObjectStatic();
};
//----------------------------------------------------------------------------------
class CIndexSound
{
	SETGET(uint, Address);
	SETGET(int, DataSize);
	SETGET(uint, Delay);
	SETGET(uint, LastAccessTime);

public:
	CIndexSound();
	virtual ~CIndexSound();

	UCHAR_LIST m_WaveFile;
	HSTREAM m_Stream;
};
//----------------------------------------------------------------------------------
class CIndexMulti
{
	SETGET(uint, Address);
	SETGET(int, DataSize);
	SETGET(uint, Count);

public:
	CIndexMulti();
	virtual ~CIndexMulti();
};
//---------------------------------------------------------------------------
class CIndexAnimation
{
	SETGET(uint, Address);
	SETGET(uint, Offset);
	SETGET(ushort, Graphic);
	SETGET(ushort, Color);

public:
	CIndexAnimation();
	virtual ~CIndexAnimation();

	//”казатель на группу анимаций
	class CTextureAnimation *Group;
};
//---------------------------------------------------------------------------
class CIndexMusic
{
	SETGET(string, FilePath);
	SETGET(bool, Loop);

public:
	CIndexMusic();
	virtual ~CIndexMusic();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
