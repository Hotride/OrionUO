// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** IndexObject.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CIndexObject::CIndexObject()
{
}
//----------------------------------------------------------------------------------
CIndexObject::~CIndexObject()
{
	WISPFUN_DEBUG("c189_f1");
	if (Texture != NULL)
	{
		delete Texture;
		Texture = NULL;
	}
}
//----------------------------------------------------------------------------------
CIndexObjectLand::CIndexObjectLand()
: CIndexObject()
{
}
//----------------------------------------------------------------------------------
CIndexObjectLand::~CIndexObjectLand()
{
}
//----------------------------------------------------------------------------------
CIndexObjectStatic::CIndexObjectStatic()
: CIndexObject()
{
}
//----------------------------------------------------------------------------------
CIndexObjectStatic::~CIndexObjectStatic()
{
}
//----------------------------------------------------------------------------------
CIndexSound::CIndexSound()
: CIndexObject()
{
}
//----------------------------------------------------------------------------------
CIndexSound::~CIndexSound()
{
}
//----------------------------------------------------------------------------------
CIndexMulti::CIndexMulti()
: CIndexObject()
{
}
//----------------------------------------------------------------------------------
CIndexMulti::~CIndexMulti()
{
}
//----------------------------------------------------------------------------------
CIndexLight::CIndexLight()
: CIndexObject()
{
}
//----------------------------------------------------------------------------------
CIndexLight::~CIndexLight()
{
}
//----------------------------------------------------------------------------------
CIndexGump::CIndexGump()
: CIndexObject()
{
}
//----------------------------------------------------------------------------------
CIndexGump::~CIndexGump()
{
}
//----------------------------------------------------------------------------------
CIndexAnimation::CIndexAnimation()
{
}
//----------------------------------------------------------------------------------
CIndexAnimation::~CIndexAnimation()
{
}
//----------------------------------------------------------------------------------
CIndexMusic::CIndexMusic()
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
//----------------------------------------------------------------------------------
