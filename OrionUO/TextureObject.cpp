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
: CGLTexture(), CenterX(0), CenterY(0)
{
}
//----------------------------------------------------------------------------------
CTextureAnimationFrame::~CTextureAnimationFrame()
{
}
//----------------------------------------------------------------------------------
CTextureAnimationDirection::CTextureAnimationDirection()
: FrameCount(0), BaseAddress(0), BaseSize(0), PatchedAddress(0),
PatchedSize(0), Address(0), Size(0), LastAccessTime(0), m_Frames(NULL), IsUOP(false), IsVerdata(false)
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
