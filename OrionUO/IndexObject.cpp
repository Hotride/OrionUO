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
: m_Address(0), m_DataSize(0), m_Width(0), m_Height(0), m_ID(0), m_Color(0),
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
: m_Delay(0), m_Stream(0)
{
}
//----------------------------------------------------------------------------------
CIndexSound::~CIndexSound()
{
}
//----------------------------------------------------------------------------------
CIndexMulti::CIndexMulti()
: m_Count(0)
{
}
//----------------------------------------------------------------------------------
CIndexMulti::~CIndexMulti()
{
}
//----------------------------------------------------------------------------------
CIndexLight::CIndexLight()
{
}
//----------------------------------------------------------------------------------
CIndexLight::~CIndexLight()
{
}
//----------------------------------------------------------------------------------
CIndexGump::CIndexGump()
{
}
//----------------------------------------------------------------------------------
CIndexGump::~CIndexGump()
{
}
//----------------------------------------------------------------------------------
CIndexAnimation::CIndexAnimation()
: m_Address(0), m_Offset(0), m_Graphic(0), m_Color(0), Group(NULL)
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
void CIndexObject::ReadIndexFile(const uint &address, PBASE_IDX_BLOCK ptr, const ushort id)
{
	m_Address = ptr->Position;
	m_DataSize = ptr->Size;

	if (m_Address == 0xFFFFFFFF || !m_DataSize || m_DataSize == 0xFFFFFFFF)
	{
		m_Address = 0;
		m_DataSize = 0;
	}
	else
		m_Address = m_Address + address;
	m_ID = id;
};
//----------------------------------------------------------------------------------
void CIndexMulti::ReadIndexFile(const uint &address, PBASE_IDX_BLOCK ptr, const ushort id)
{
	CIndexObject::ReadIndexFile(address, ptr, id);
	if (g_PacketManager.ClientVersion >= CV_7090)
		m_Count = (ushort)(DataSize / sizeof(MULTI_BLOCK_NEW));
	else
		m_Count = (ushort)(DataSize / sizeof(MULTI_BLOCK));
};
//----------------------------------------------------------------------------------
void CIndexLight::ReadIndexFile(const uint &address, PBASE_IDX_BLOCK ptr, const ushort id)
{
	CIndexObject::ReadIndexFile(address, ptr, id);
	PLIGHT_IDX_BLOCK realPtr = (PLIGHT_IDX_BLOCK)ptr;
	m_Width = realPtr->Width;
	m_Height = realPtr->Height;
};
//----------------------------------------------------------------------------------
void CIndexGump::ReadIndexFile(const uint &address, PBASE_IDX_BLOCK ptr, const ushort id)
{
	CIndexObject::ReadIndexFile(address, ptr, id);
	PGUMP_IDX_BLOCK realPtr = (PGUMP_IDX_BLOCK)ptr;
	m_Width = realPtr->Width;
	m_Height = realPtr->Height;
};