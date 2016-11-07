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
#include "bassmidi.h"
#include "Gumps/GumpBulletinBoardItem.h"
#include "Managers/PacketManager.h"

#pragma comment(lib, "bass.lib")
#pragma comment(lib, "bassmidi.lib")
//----------------------------------------------------------------------------------
class CIndexObject
{
	SETGET(uint, Address);
	SETGET(int, DataSize);
	SETGET(int, Width);
	SETGET(int, Height);
	SETGET(ushort, ID);
	SETGET(ushort, Color);
	SETGET(uint, LastAccessTime);

public:
	CIndexObject();
	virtual ~CIndexObject();
	virtual void ReadIndexFile(const uint &address, PBASE_IDX_BLOCK ptr, const ushort id);
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
class CIndexSound : public CIndexObject
{
	SETGET(uint, Delay);

public:
	CIndexSound();
	virtual ~CIndexSound();

	UCHAR_LIST m_WaveFile;
	HSTREAM m_Stream;
};
//----------------------------------------------------------------------------------
class CIndexMulti : public CIndexObject
{
	SETGET(uint, Count);

public:
	CIndexMulti();
	virtual ~CIndexMulti();
	virtual void ReadIndexFile(const uint &address, PBASE_IDX_BLOCK ptr, const ushort id) override;
};
//----------------------------------------------------------------------------------
class CIndexGump : public CIndexObject
{
public:
	CIndexGump();
	virtual ~CIndexGump();
	virtual void ReadIndexFile(const uint &address, PBASE_IDX_BLOCK ptr, const ushort id) override;
};
//----------------------------------------------------------------------------------
class CIndexLight : public CIndexObject
{
public:
	CIndexLight();
	virtual ~CIndexLight();
	void ReadIndexFile(const uint &address, PBASE_IDX_BLOCK ptr, const ushort id) override;
};
//---------------------------------------------------------------------------
class CIndexAnimation
{
	SETGET(uint, Address);
	SETGET(uint, Offset);
	SETGET(ushort, Graphic);
	SETGET(ushort, Color);
	SETGET(ANIMATION_GROUPS_TYPE, Type);
public:
	CIndexAnimation();
	virtual ~CIndexAnimation();

	//Указатель на группу анимаций
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
