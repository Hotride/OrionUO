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
#include "bass.h"
#include "bassmidi.h"

#pragma comment(lib, "bass.lib")
#pragma comment(lib, "bassmidi.lib")
//----------------------------------------------------------------------------------
class CIndexObject
{
	SETGET(uint, Address, 0);
	SETGET(int, DataSize, 0);
	SETGET(int, Width, 0);
	SETGET(int, Height, 0);
	SETGET(ushort, ID, 0);
	SETGET(ushort, Color, 0);
	SETGET(uint, LastAccessTime, 0);
	SETGET(CUopBlockHeader*, UopBlock, NULL);

public:
	CIndexObject();
	virtual ~CIndexObject();
	virtual void ReadIndexFile(const uint &address, PBASE_IDX_BLOCK ptr, const ushort id);
	CGLTexture *Texture{ NULL };
};
//----------------------------------------------------------------------------------
class CIndexObjectLand : public CIndexObject
{
	SETGET(bool, AllBlack, false);

public:
	CIndexObjectLand();
	virtual ~CIndexObjectLand();
};
//----------------------------------------------------------------------------------
class CIndexObjectStatic : public CIndexObject
{
	SETGET(ushort, Index, 0);
	SETGET(char, Offset, 0);
	SETGET(char, AnimIndex, 0);
	SETGET(uint, ChangeTime, 0);
	SETGET(ushort, LightColor, 0);
	SETGET(bool, IsFiled, false);

public:
	CIndexObjectStatic();
	virtual ~CIndexObjectStatic();
};
//----------------------------------------------------------------------------------
class CIndexSound : public CIndexObject
{
	SETGET(uint, Delay, 0);

public:
	CIndexSound();
	virtual ~CIndexSound();

	UCHAR_LIST m_WaveFile;
	HSTREAM m_Stream{ NULL };
};
//----------------------------------------------------------------------------------
class CIndexMulti : public CIndexObject
{
	SETGET(uint, Count, 0);

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
	virtual void ReadIndexFile(const uint &address, PBASE_IDX_BLOCK ptr, const ushort id) override;
};
//---------------------------------------------------------------------------
class CIndexAnimation
{
	SETGET(ushort, Graphic, 0);
	SETGET(ushort, Color, 0);
	SETGET(ANIMATION_GROUPS_TYPE, Type, AGT_UNKNOWN);
	SETGET(char, MountedHeightOffset, 0);
	SETGET(bool, IsUOP, false);

public:
	CIndexAnimation();
	virtual ~CIndexAnimation();

	CTextureAnimationGroup m_Groups[ANIMATION_GROUPS_COUNT];
};
//---------------------------------------------------------------------------
class CIndexMusic
{
	SETGET(string, FilePath, "");
	SETGET(bool, Loop, false);

public:
	CIndexMusic();
	virtual ~CIndexMusic();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
