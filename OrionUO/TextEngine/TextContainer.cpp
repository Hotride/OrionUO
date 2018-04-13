// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** TextContainer.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CTextContainer g_SystemChat(30);
//----------------------------------------------------------------------------------
CTextContainer::CTextContainer(const int &maxSize)
: MaxSize(maxSize)
{
}
//----------------------------------------------------------------------------------
CTextContainer::~CTextContainer()
{
}
//----------------------------------------------------------------------------------
void CTextContainer::Clear()
{
	WISPFUN_DEBUG("c173_f1");
	CBaseQueue::Clear();

	Size = 0;
}
//----------------------------------------------------------------------------------
void CTextContainer::Add(CTextData *obj)
{
	WISPFUN_DEBUG("c173_f2");
	CBaseQueue::Add(obj);

	if (Size >= MaxSize)
		CBaseQueue::Delete(m_Items);
	else
		Size++;
}
//----------------------------------------------------------------------------------
void CTextContainer::Delete(CTextData *obj)
{
	WISPFUN_DEBUG("c173_f3");
	CBaseQueue::Delete(obj);

	Size--;
}
//----------------------------------------------------------------------------------
void CTextContainer::DrawSystemChat(const int &x, const int &y, const int &height)
{
	WISPFUN_DEBUG("c173_f4");
	int offset = (y + height) - 41;

	CTextData *td = (CTextData*)g_SystemChat.Last();

	while (td != NULL && offset >= y)
	{
		CGLTextTexture &tth = td->m_Texture;

		offset -= tth.Height;

		if (td->Timer >= g_Ticks)
			tth.Draw(x, offset);

		td = (CTextData*)td->m_Prev;
	}
}
//----------------------------------------------------------------------------------
