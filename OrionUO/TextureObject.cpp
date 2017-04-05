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
CTextureAnimationFrame::CTextureAnimationFrame()
: m_CenterX(0), m_CenterY(0), m_Texture()
{
}
//----------------------------------------------------------------------------------
CTextureAnimationFrame::~CTextureAnimationFrame()
{
	m_Texture.Clear();
}
//----------------------------------------------------------------------------------
CTextureAnimationDirection::CTextureAnimationDirection()
: m_FrameCount(0), m_Address(0), m_Size(0), m_LastAccessTime(0), m_Frames(NULL)
{
}
//----------------------------------------------------------------------------------
CTextureAnimationDirection::~CTextureAnimationDirection()
{
	if (m_Frames != NULL)
	{
		delete[] m_Frames;
		m_Frames = NULL;
	}
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
