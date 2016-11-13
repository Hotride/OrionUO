/***********************************************************************************
**
** IndexObject.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "IndexObject.h"
//----------------------------------------------------------------------------------
CIndexObject::CIndexObject()
: m_Address(0), m_DataSize(0), m_Width(0), m_Height(0), m_Graphic(0), m_Color(0),
m_LastAccessTime(0), Texture(NULL)
{
}
//----------------------------------------------------------------------------------
CIndexObject::~CIndexObject()
{
	if (Texture != NULL)
	{
		delete Texture;
		Texture = NULL;
	}
}
//----------------------------------------------------------------------------------
CIndexObjectLand::CIndexObjectLand()
: CIndexObject(), m_AllBlack(false)
{
}
//----------------------------------------------------------------------------------
CIndexObjectLand::~CIndexObjectLand()
{
}
//----------------------------------------------------------------------------------
CIndexObjectStatic::CIndexObjectStatic()
: CIndexObject(), m_Index(0), m_Offset(0), m_AnimIndex(0), m_ChangeTime(0),
m_LightColor(0), m_IsFiled(false)
{
}
//----------------------------------------------------------------------------------
CIndexObjectStatic::~CIndexObjectStatic()
{
}
//----------------------------------------------------------------------------------
CIndexSound::CIndexSound()
: m_Address(0), m_DataSize(0), m_Delay(0), m_LastAccessTime(0), m_Stream(0)
{
}
//----------------------------------------------------------------------------------
CIndexSound::~CIndexSound()
{
}
//----------------------------------------------------------------------------------
CIndexMulti::CIndexMulti()
: m_Address(0), m_DataSize(0), m_Count(0)
{
}
//----------------------------------------------------------------------------------
CIndexMulti::~CIndexMulti()
{
}
//----------------------------------------------------------------------------------
CIndexAnimation::CIndexAnimation()
: m_Address(0), m_Offset(0), m_Graphic(0), m_Color(0), Group(NULL), m_Type(AGT_MONSTER),
m_MountedHeightOffset(0)
{
}
//----------------------------------------------------------------------------------
CIndexAnimation::~CIndexAnimation()
{
}
//----------------------------------------------------------------------------------
CIndexMusic::CIndexMusic()
: m_FilePath(""), m_Loop(false)
{
}
//----------------------------------------------------------------------------------
CIndexMusic::~CIndexMusic()
{
}
//----------------------------------------------------------------------------------
