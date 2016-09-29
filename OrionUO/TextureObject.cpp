/***********************************************************************************
**
** TextureObject.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "TextureObject.h"
//----------------------------------------------------------------------------------
CTextureAnimationFrame::CTextureAnimationFrame(const int &frame)
: CBaseQueueItem(), m_Frame(frame), m_Width(0), m_Height(0), m_CenterX(0),
m_CenterY(0), Texture(0)
{
}
//----------------------------------------------------------------------------------
CTextureAnimationFrame::~CTextureAnimationFrame()
{
	if (Texture)
	{
		glDeleteTextures(1, &Texture);
		Texture = 0;
	}
}
//----------------------------------------------------------------------------------
CTextureAnimationDirection::CTextureAnimationDirection(int direction)
: CBaseQueueItem(), m_Direction(direction), m_FrameCount(0), m_Address(0),
m_Size(0), m_LastAccessTime(0)
{
}
//----------------------------------------------------------------------------------
CTextureAnimationDirection::~CTextureAnimationDirection()
{
}
//----------------------------------------------------------------------------------
CTextureAnimationFrame *CTextureAnimationDirection::GetFrame(const uchar &frame)
{
	CTextureAnimationFrame *item = FindFrame(frame);

	if (item == NULL && frame < m_FrameCount)
	{
		item = new CTextureAnimationFrame(frame);
		Add(item);
	}

	return item;
}
//----------------------------------------------------------------------------------
CTextureAnimationFrame *CTextureAnimationDirection::FindFrame(const uchar &frame)
{
	CTextureAnimationFrame *item = (CTextureAnimationFrame*)m_Items;

	while (item != NULL)
	{
		if (item->Frame == frame)
			break;

		item = (CTextureAnimationFrame*)item->m_Next;
	}

	return item;
}
//----------------------------------------------------------------------------------
CTextureAnimationGroup::CTextureAnimationGroup(int index)
: CBaseQueueItem(), m_Index(index)
{
}
//----------------------------------------------------------------------------------
CTextureAnimationGroup::~CTextureAnimationGroup()
{
}
//----------------------------------------------------------------------------------
CTextureAnimationDirection *CTextureAnimationGroup::GetDirection(const uchar &direction)
{
	CTextureAnimationDirection *item = (CTextureAnimationDirection*)m_Items;

	while (item != NULL)
	{
		if (item->Direction == direction)
			break;

		item = (CTextureAnimationDirection*)item->m_Next;
	}

	if (item == NULL)
	{
		item = new CTextureAnimationDirection(direction);
		Add(item);
	}

	return item;
}
//----------------------------------------------------------------------------------
CTextureAnimation::CTextureAnimation()
: CBaseQueue()
{
}
//----------------------------------------------------------------------------------
CTextureAnimation::~CTextureAnimation()
{
}
//----------------------------------------------------------------------------------
CTextureAnimationGroup *CTextureAnimation::GetGroup(const uchar &index)
{
	CTextureAnimationGroup *item = (CTextureAnimationGroup*)m_Items;

	while (item != NULL)
	{
		if (item->Index == index)
			break;

		item = (CTextureAnimationGroup*)item->m_Next;
	}

	if (item == NULL)
	{
		item = new CTextureAnimationGroup(index);
		Add(item);
	}

	return item;
}
//----------------------------------------------------------------------------------
