/****************************************************************************
**
** IndexObject.cpp
**
** Copyright (C) January 2016 Hotride
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
TIndexObject::TIndexObject()
: m_Address(0), m_Size(0), m_LastAccessTime(0), m_Width(0), m_Height(0),
m_Color(0), Texture(NULL)
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
: m_Address(0), m_Size(0), m_LastAccessTime(0), hStream(0)
{
}
//---------------------------------------------------------------------------
TIndexSound::~TIndexSound()
{
	//if (Sound != NULL)
	//{
	//	Mix_FreeChunk(Sound);
	//	Sound = NULL;
	//}

	if (this->hStream != 0)
	{
		BASS_StreamFree(this->hStream);
		hStream = 0;
	}
}
//---------------------------------------------------------------------------
TIndexAnimation::TIndexAnimation()
: m_Address(0), m_Offset(0), m_Graphic(0), m_Color(0), Group(NULL)
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
TIndexMulti::TIndexMulti()
: m_Address(0), m_Size(0), m_Count(0)
{
}
//---------------------------------------------------------------------------
TIndexMulti::~TIndexMulti()
{
}
//---------------------------------------------------------------------------
TMP3Struct::TMP3Struct()
: m_FileName(""), m_Loop(false)
{
}
//---------------------------------------------------------------------------
TMP3Struct::~TMP3Struct()
{
}
//---------------------------------------------------------------------------