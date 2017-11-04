// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** TextureObject.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CTextureAnimationFrame::CTextureAnimationFrame()
: CGLTexture(), m_CenterX(0), m_CenterY(0)
{
}
//----------------------------------------------------------------------------------
CTextureAnimationFrame::~CTextureAnimationFrame()
{
}
//----------------------------------------------------------------------------------
CTextureAnimationDirection::CTextureAnimationDirection()
: m_FrameCount(0), m_BaseAddress(0), m_BaseSize(0), m_PatchedAddress(0),
m_PatchedSize(0), m_Address(0), m_Size(0), m_LastAccessTime(0), m_Frames(NULL), m_IsUOP(false)
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
