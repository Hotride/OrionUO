/****************************************************************************
**
** TextureObject.cpp
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
#include "stdafx.h"
//---------------------------------------------------------------------------
TTextureObject::TTextureObject()
: m_Width(0), m_Height(0), Texture(0)
{
}
//---------------------------------------------------------------------------
TTextureObject::~TTextureObject()
{
	if (Texture)
	{
		glDeleteTextures(1, &Texture);
		Texture = 0;
	}
}
//---------------------------------------------------------------------------
TIndexObject::TIndexObject()
: m_Address(0), m_Size(0), m_LastAccessTime(0), m_Width(0), m_Height(0), Texture(0)
{
}
//---------------------------------------------------------------------------
TIndexObject::~TIndexObject()
{
}
//---------------------------------------------------------------------------
TIndexObjectStatic::TIndexObjectStatic()
: TIndexObject(), m_Index(0), m_Increment(0), m_AnimIndex(0), m_NextChange(0),
m_LightColor(0)
{
}
//---------------------------------------------------------------------------
TIndexSound::TIndexSound()
: m_Address(0), m_Size(0), m_LastAccessTime(0), Sound(NULL)
{
}
//---------------------------------------------------------------------------
TIndexSound::~TIndexSound()
{
	if (Sound != NULL)
	{
		Mix_FreeChunk(Sound);
		Sound = NULL;
	}
}
//---------------------------------------------------------------------------
TTextureAnimationFrame::TTextureAnimationFrame(int frame)
: TBaseQueueItem(), m_Frame(frame), m_Width(0), m_Height(0), m_CenterX(0),
m_CenterY(0), Texture(0)
{
}
//---------------------------------------------------------------------------
TTextureAnimationFrame::~TTextureAnimationFrame()
{
	if (Texture)
	{
		glDeleteTextures(1, &Texture);
		Texture = 0;
	}
}
//---------------------------------------------------------------------------
TTextureAnimationDirection::TTextureAnimationDirection(int direction)
: TBaseQueueItem(), m_Direction(direction), m_FrameCount(0), m_Address(0),
m_LastAccessTime(0)
{
}
//---------------------------------------------------------------------------
TTextureAnimationDirection::~TTextureAnimationDirection()
{
}
//---------------------------------------------------------------------------
TTextureAnimationFrame *TTextureAnimationDirection::GetFrame(BYTE frame)
{
	TTextureAnimationFrame *item = (TTextureAnimationFrame*)m_Items;

	while (item != NULL)
	{
		if (item->Frame == frame)
			break;

		item = (TTextureAnimationFrame*)item->m_Next;
	}

	if (item == NULL)
	{
		item = new TTextureAnimationFrame(frame);
		Add(item);
	}

	return item;
}
//---------------------------------------------------------------------------
TTextureAnimationGroup::TTextureAnimationGroup(int index)
: TBaseQueueItem(), m_Index(index)
{
}
//---------------------------------------------------------------------------
TTextureAnimationGroup::~TTextureAnimationGroup()
{
}
//---------------------------------------------------------------------------
TTextureAnimationDirection *TTextureAnimationGroup::GetDirection(BYTE direction)
{
	TTextureAnimationDirection *item = (TTextureAnimationDirection*)m_Items;

	while (item != NULL)
	{
		if (item->Direction == direction)
			break;

		item = (TTextureAnimationDirection*)item->m_Next;
	}

	if (item == NULL)
	{
		item = new TTextureAnimationDirection(direction);
		Add(item);
	}

	return item;
}
//---------------------------------------------------------------------------
TTextureAnimation::TTextureAnimation()
: TBaseQueue()
{
}
//---------------------------------------------------------------------------
TTextureAnimation::~TTextureAnimation()
{
}
//---------------------------------------------------------------------------
TTextureAnimationGroup *TTextureAnimation::GetGroup(BYTE index)
{
	TTextureAnimationGroup *item = (TTextureAnimationGroup*)m_Items;

	while (item != NULL)
	{
		if (item->Index == index)
			break;

		item = (TTextureAnimationGroup*)item->m_Next;
	}

	if (item == NULL)
	{
		item = new TTextureAnimationGroup(index);
		Add(item);
	}

	return item;
}
//---------------------------------------------------------------------------
TIndexAnimation::TIndexAnimation()
: m_Address(0), m_Offset(0), m_Graphic(0), Group(NULL)
{
}
//---------------------------------------------------------------------------
TIndexAnimation::~TIndexAnimation()
{
	if (Group != NULL)
	{
		delete Group;
		Group = NULL;
	}
}
//---------------------------------------------------------------------------
TLinkedList::TLinkedList(PVOID data)
: Data(data), Next(NULL)
{
}
//---------------------------------------------------------------------------
TLinkedList::~TLinkedList()
{
	if (Next != NULL)
	{
		delete Next;
		Next = NULL;
	}

	Data = NULL;
}
//---------------------------------------------------------------------------
TIndexMulti::TIndexMulti()
: m_Address(0), m_Size(0), m_Count(0)
{
}
//---------------------------------------------------------------------------
TIndexMulti::~TIndexMulti()
{
}
//---------------------------------------------------------------------------
TTextTexture::TTextTexture()
: m_Width(0), m_Height(0), m_Lines(0), m_Texture(0), m_YellowTexture(0),
Data(NULL)
{
}
//---------------------------------------------------------------------------
TTextTexture::~TTextTexture()
{
	if (m_Texture)
	{
		glDeleteTextures(1, &m_Texture);
		m_Texture = 0;
	}

	if (m_YellowTexture)
	{
		glDeleteTextures(1, &m_YellowTexture);
		m_YellowTexture = 0;
	}

	if (Data != NULL)
	{
		delete Data;
		Data = NULL;
	}
}
//--------------------------------------------------------------------------
void TTextTexture::Clear()
{
	m_Width = 0;
	m_Height = 0;
	m_Lines = 0;

	if (m_Texture)
	{
		glDeleteTextures(1, &m_Texture);
		m_Texture = 0;
	}

	if (m_YellowTexture)
	{
		glDeleteTextures(1, &m_YellowTexture);
		m_YellowTexture = 0;
	}

	if (Data != NULL)
	{
		delete Data;
		Data = NULL;
	}
}
//--------------------------------------------------------------------------
void TTextTexture::Init()
{
	m_Width = 0;
	m_Height = 0;
	m_Lines = 0;

	m_Texture = 0;
	m_YellowTexture = 0;

	Data = NULL;
}
//--------------------------------------------------------------------------
void TTextTexture::Draw(int x, int y)
{
	if (m_Texture != 0)
		g_GL.Draw(m_Texture, (GLfloat)x, (GLfloat)y, (GLfloat)m_Width, (GLfloat)m_Height);
}
//--------------------------------------------------------------------------
void TTextTexture::DrawYellow(int x, int y)
{
	if (m_YellowTexture != 0)
		g_GL.Draw(m_YellowTexture, (GLfloat)x, (GLfloat)y, (GLfloat)m_Width, (GLfloat)m_Height);
	else if (m_Texture != 0)
		g_GL.Draw(m_Texture, (GLfloat)x, (GLfloat)y, (GLfloat)m_Width, (GLfloat)m_Height);
}
//--------------------------------------------------------------------------
bool TTextTexture::UnderMouse(int x, int y)
{
	if (Data != NULL)
	{
		x = g_MouseX - x;
		y = g_MouseY - y;

		if (x > 0 && y > 0 && x < m_Width && y < m_Height)
			return (Data[(y * m_Width) + x] != 0);
	}

	return false;
}
//---------------------------------------------------------------------------
THTMLTextTexture::THTMLTextTexture()
: TTextTexture()
{
}
//---------------------------------------------------------------------------
THTMLTextTexture::~THTMLTextTexture()
{
	WebLinkRect.clear();
}
//--------------------------------------------------------------------------
WORD THTMLTextTexture::WebLinkUnderMouse(int x, int y)
{
	WORD result = 0;
	x = g_MouseX - x;
	y = g_MouseY - y;

	for (std::deque<WEB_LINK_RECT>::iterator it = WebLinkRect.begin(); it != WebLinkRect.end(); it++)
	{
		if (y >= (*it).StartY && y < (*it).StartY + (*it).EndY)
		{
			if (x >= (*it).StartX && x < (*it).StartX + (*it).EndX)
			{
				result = (*it).LinkID;
				break;
			}
		}
	}

	return result;
}
//---------------------------------------------------------------------------