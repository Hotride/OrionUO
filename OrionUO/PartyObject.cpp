// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** PartyObject.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CPartyObject::CPartyObject()
{
}
//----------------------------------------------------------------------------------
string CPartyObject::GetName(const int &index)
{
	WISPFUN_DEBUG("c197_f1");
	if (Serial)
	{
		if (Character == NULL)
			Character = g_World->FindWorldCharacter(Serial);
		if (Character != NULL)
			return Character->GetName();
	}

	char buf[10] = {0};
	sprintf_s(buf, "[%i]", index);

	return string(buf);
}
//----------------------------------------------------------------------------------
