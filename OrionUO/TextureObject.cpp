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
: CBaseQueueItem(), m_Frame(frame), m_CenterX(0), m_CenterY(0), m_Texture()
{
}
//----------------------------------------------------------------------------------
CTextureAnimationFrame::~CTextureAnimationFrame()
{
	m_Texture.Clear();
}
//----------------------------------------------------------------------------------
CTextureAnimationDirection::CTextureAnimationDirection()
: CBaseQueue(), m_FrameCount(0), m_Address(0), m_Size(0), m_LastAccessTime(0)
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
CTextureAnimationGroup::CTextureAnimationGroup()
{
}
//----------------------------------------------------------------------------------
CTextureAnimationGroup::~CTextureAnimationGroup()
{
}
//----------------------------------------------------------------------------------
