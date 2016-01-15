/****************************************************************************
**
** TextureObject.h
**
** Copyright (C) September 2015 Hotride
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
*****************************************************************************
*/
//---------------------------------------------------------------------------
#ifndef TextureObjectH
#define TextureObjectH
//---------------------------------------------------------------------------
class TTextureObject
{
private:
	int m_Width;
	int m_Height;
public:
	TTextureObject();
	~TTextureObject();

	SETGET(int, Width)
	SETGET(int, Height)

	GLuint Texture;
};
//---------------------------------------------------------------------------
class TIndexObject
{
private:
	DWORD m_Address;
	DWORD m_Size;
	DWORD m_LastAccessTime;
	short m_Width;
	short m_Height;
public:
	TIndexObject();
	virtual ~TIndexObject();

	SETGET(DWORD, Address)
	SETGET(DWORD, Size)
	SETGET(DWORD, LastAccessTime)
	SETGET(short, Width)
	SETGET(short, Height)

	TTextureObject *Texture;
};
//---------------------------------------------------------------------------
class TIndexObjectStatic : public TIndexObject
{
private:
	WORD m_Index;
	char m_Increment;
	char m_AnimIndex;
	DWORD m_NextChange;
	WORD m_LightColor;
public:
	TIndexObjectStatic();

	SETGET(WORD, Index)
	SETGET(char, Increment)
	SETGET(char, AnimIndex)
	SETGET(DWORD, NextChange)
	SETGET(WORD, LightColor)
};
//---------------------------------------------------------------------------
class TIndexSound
{
private:
	DWORD m_Address;
	DWORD m_Size;
	DWORD m_Timer;
	DWORD m_LastAccessTime;
public:
	TIndexSound();
	virtual ~TIndexSound();

	SETGET(DWORD, Address)
	SETGET(DWORD, Size)
	SETGET(DWORD, Timer)
	SETGET(DWORD, LastAccessTime)

	Mix_Chunk *Sound;
};
//---------------------------------------------------------------------------
class TTextureAnimationFrame : public TBaseQueueItem
{
private:
	BYTE m_Frame;
	short m_Width;
	short m_Height;
	short m_CenterX;
	short m_CenterY;
public:
	TTextureAnimationFrame(int frame);
	virtual ~TTextureAnimationFrame();

	SETGET(BYTE, Frame)
	SETGET(short, Width)
	SETGET(short, Height)
	SETGET(short, CenterX)
	SETGET(short, CenterY)

	GLuint Texture;
};
//---------------------------------------------------------------------------
class TTextureAnimationDirection : public TBaseQueueItem
{
private:
	BYTE m_Direction;
	BYTE m_FrameCount;
	DWORD m_Address;
	DWORD m_LastAccessTime;
public:
	TTextureAnimationDirection(int direction);
	virtual ~TTextureAnimationDirection();

	SETGET(BYTE, Direction)
	SETGET(BYTE, FrameCount)
	SETGET(DWORD, Address)
	SETGET(DWORD, LastAccessTime)

	TTextureAnimationFrame *GetFrame(BYTE frame);
};
//---------------------------------------------------------------------------
class TTextureAnimationGroup : public TBaseQueueItem //объединение направлений
{
private:
	BYTE m_Index;
public:
	TTextureAnimationGroup(int index);
	virtual ~TTextureAnimationGroup();

	SETGET(BYTE, Index)

	TTextureAnimationDirection *GetDirection(BYTE direction);
};
//---------------------------------------------------------------------------
class TTextureAnimation : public TBaseQueue //раздел анимации
{
private:
public:
	TTextureAnimation();
	virtual ~TTextureAnimation();

	TTextureAnimationGroup *GetGroup(BYTE index);
};
//---------------------------------------------------------------------------
class TIndexAnimation
{
private:
	DWORD m_Address;
	DWORD m_Offset;
	WORD m_Graphic;
public:
	TIndexAnimation();
	virtual ~TIndexAnimation();

	TTextureAnimation *Group;

	SETGET(DWORD, Address)
	SETGET(DWORD, Offset)
	SETGET(WORD, Graphic)
};
//---------------------------------------------------------------------------
class TLinkedList
{
private:
public:
	TLinkedList(PVOID data);
	~TLinkedList();
	
	PVOID Data;
	TLinkedList *Next;
};
//---------------------------------------------------------------------------
class TIndexMulti
{
private:
	DWORD m_Address;
	DWORD m_Size;
	WORD m_Count;
public:
	TIndexMulti();
	virtual ~TIndexMulti();

	SETGET(DWORD, Address)
	SETGET(DWORD, Size)
	SETGET(WORD, Count)
};
//---------------------------------------------------------------------------
struct WEB_LINK_RECT
{
	WORD LinkID;
	int StartX;
	int StartY;
	int EndX;
	int EndY;
};
//---------------------------------------------------------------------------
class TTextTexture
{
private:
	int m_Width;
	int m_Height;
	int m_Lines;
	GLuint m_Texture;
public:
	TTextTexture();
	virtual ~TTextTexture();

	PBYTE Data;

	SETGET(int, Width)
	SETGET(int, Height)
	SETGET(int, Lines)
	SETGET(GLuint, Texture)

	bool Empty() {return (m_Texture == 0);}

	void Init();
	void Clear();
	
	void Draw(int x, int y);

	bool UnderMouse(int x, int y);

	virtual void ClearWebLink() {}
	virtual void AddWebLink(WEB_LINK_RECT &wl) {}
	virtual WORD WebLinkUnderMouse(int x, int y) {return 0;}
};
//---------------------------------------------------------------------------
class THTMLTextTexture : public TTextTexture
{
private:
public:
	THTMLTextTexture();
	virtual ~THTMLTextTexture();

	std::deque<WEB_LINK_RECT> WebLinkRect;

	void ClearWebLink() {WebLinkRect.clear();}
	void AddWebLink(WEB_LINK_RECT &wl) {WebLinkRect.push_back(wl);}
	WORD WebLinkUnderMouse(int x, int y);
};
//---------------------------------------------------------------------------
#endif