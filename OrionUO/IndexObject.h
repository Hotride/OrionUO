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
class CIndexObject
{
public:
	size_t Address = 0;
	int DataSize = 0;
	int Width = 0;
	int Height = 0;
	ushort ID = 0;
	ushort Color = 0;
	uint LastAccessTime = 0;
	CUopBlockHeader* UopBlock = NULL;
	CIndexObject();
	virtual ~CIndexObject();
	virtual void ReadIndexFile(const size_t &address, PBASE_IDX_BLOCK ptr, const ushort id);
	CGLTexture *Texture{ NULL };
};
//----------------------------------------------------------------------------------
class CIndexObjectLand : public CIndexObject
{
public:
	bool AllBlack = false;

	CIndexObjectLand();
	virtual ~CIndexObjectLand();
};
//----------------------------------------------------------------------------------
class CIndexObjectStatic : public CIndexObject
{
public:
	ushort Index = 0;
	char Offset = 0;
	char AnimIndex = 0;
	uint ChangeTime = 0;
	ushort LightColor = 0;
	bool IsFiled = false;

	CIndexObjectStatic();
	virtual ~CIndexObjectStatic();
};
//----------------------------------------------------------------------------------
class CIndexSound : public CIndexObject
{
public:
	uint Delay = 0;

	CIndexSound();
	virtual ~CIndexSound();

	UCHAR_LIST m_WaveFile;
	HSTREAM m_Stream{ NULL };
};
//----------------------------------------------------------------------------------
class CIndexMulti : public CIndexObject
{
public:
	uint Count = 0;

	CIndexMulti();
	virtual ~CIndexMulti();
	virtual void ReadIndexFile(const size_t &address, PBASE_IDX_BLOCK ptr, const ushort id) override;
};
//----------------------------------------------------------------------------------
class CIndexGump : public CIndexObject
{
public:
	CIndexGump();
	virtual ~CIndexGump();
	virtual void ReadIndexFile(const size_t &address, PBASE_IDX_BLOCK ptr, const ushort id) override;
};
//----------------------------------------------------------------------------------
class CIndexLight : public CIndexObject
{
public:
	CIndexLight();
	virtual ~CIndexLight();
	virtual void ReadIndexFile(const size_t &address, PBASE_IDX_BLOCK ptr, const ushort id) override;
};
//---------------------------------------------------------------------------
class CIndexAnimation
{
public:
	ushort Graphic = 0;
	ushort Color = 0;
	ANIMATION_GROUPS_TYPE Type = AGT_UNKNOWN;
	uint Flags = 0;
	char MountedHeightOffset = 0;
	bool IsUOP = false;

	CIndexAnimation();
	virtual ~CIndexAnimation();

	CTextureAnimationGroup m_Groups[ANIMATION_GROUPS_COUNT];
};
//---------------------------------------------------------------------------
class CIndexMusic
{
public:
	string FilePath = "";
	bool Loop = false;

	CIndexMusic();
	virtual ~CIndexMusic();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
