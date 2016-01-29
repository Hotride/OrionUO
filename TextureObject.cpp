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

TCircleOfTransparencyTexture g_CircleOfTransparency;
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
TCircleOfTransparencyTexture::TCircleOfTransparencyTexture()
: TTextureObject(), m_Radius(0)
{
}
//---------------------------------------------------------------------------
TCircleOfTransparencyTexture::~TCircleOfTransparencyTexture()
{
}
//---------------------------------------------------------------------------
bool TCircleOfTransparencyTexture::Create(int radius)
{
	if (radius <= 0)
		return false;
	else if (radius == m_Radius)
		return true;

	if (radius > 200)
		radius = 200;

	int fixRadius = radius + 10;
	int mulRadius = fixRadius * 2;

	PDWORD pixels = new DWORD[mulRadius * mulRadius];

	if (pixels == NULL)
		return false;

	m_Width = mulRadius;
	m_Height = mulRadius;

	if (Texture != NULL)
	{
		glDeleteTextures(1, &Texture);
		Texture = 0;
	}

	for (int x = -fixRadius; x < fixRadius; x++)
	{
		for (int y = -fixRadius; y < fixRadius; y++)
		{
			int r = (int)sqrt((x * x) + (y * y));

			int pos = (x + fixRadius) * mulRadius + (y + fixRadius);

			if (r <= radius)
			{
				//pixels[pos] = 0x000000FF | (((radius - r) & 0xFF) << 24);

				//pixels[pos] = (((radius - r) & 0xFF) << 8) | ((radius - r) & 0xFF);

				pixels[pos] = (radius - r) & 0xFF;
			}
			else
				pixels[pos] = 0;
		}
	}

	g_GL.BindTexture32(Texture, m_Width, m_Height, &pixels[0]);

	delete pixels;

	return true;
}
//---------------------------------------------------------------------------
void TCircleOfTransparencyTexture::Draw(int x, int y)
{
	if (Texture != 0)
	{
		x -= m_Width / 2;
		y -= m_Height / 2;

		//glEnable(GL_BLEND);
		//glBlendFunc(GL_ONE, GL_DST_ALPHA);

		g_GL.Draw(Texture, x, y, m_Width, m_Height);

		//glDisable(GL_BLEND);
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
TTextTexture::TTextTexture()
: m_Width(0), m_Height(0), m_Lines(0), m_Texture(0), Data(NULL)
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

	Data = NULL;
}
//--------------------------------------------------------------------------
void TTextTexture::Draw(int x, int y)
{
	if (m_Texture != 0)
		g_GL.Draw(m_Texture, x, y, m_Width, m_Height);
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