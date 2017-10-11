/***********************************************************************************
**
** Journal.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CJournal g_Journal(50);
//----------------------------------------------------------------------------------
CJournal::CJournal(const int &maxSize)
: CBaseQueue(), m_MaxSize(maxSize)
{
}
//----------------------------------------------------------------------------------
CJournal::~CJournal()
{
}
//----------------------------------------------------------------------------------
void CJournal::Clear()
{
	WISPFUN_DEBUG("c171_f1");
	CBaseQueue::Clear();

	m_Size = 0;
}
//----------------------------------------------------------------------------------
void CJournal::Add(CTextData *obj)
{
	WISPFUN_DEBUG("c171_f2");
	CBaseQueue::Add(obj);

	CGumpJournal *gump = (CGumpJournal*)g_GumpManager.UpdateGump(0, 0, GT_JOURNAL);

	if (gump != NULL)
		gump->AddText(obj);

	if (m_Size >= m_MaxSize)
	{
		CTextData *item = (CTextData*)m_Items;

		if (gump != NULL)
			gump->DeleteText(item);

		Delete(item);
	}
	else
		m_Size++;
}
//----------------------------------------------------------------------------------
