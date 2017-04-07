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
: m_CenterX(0), m_CenterY(0), m_Texture(), m_PixelData(NULL)
{
}
//----------------------------------------------------------------------------------
CTextureAnimationFrame::~CTextureAnimationFrame()
{
	m_PixelData.clear();
}
//----------------------------------------------------------------------------------
CTextureAnimationDirection::CTextureAnimationDirection()
: m_FrameCount(0), m_Address(0), m_Size(0), m_LastAccessTime(0), m_IsUOP(false), m_Frames(NULL)
{
}
//----------------------------------------------------------------------------------
CTextureAnimationDirection::~CTextureAnimationDirection()
{
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
