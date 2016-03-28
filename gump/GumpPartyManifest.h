/****************************************************************************
**
** GumpPartyManifest.h
**
** Copyright (C) September 2015 Hotride
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
#ifndef GumpPartyManifestH
#define GumpPartyManifestH
//---------------------------------------------------------------------------
class TGumpPartyManifest : public TGump
{
private:
	static const int ID_GPM_BUTTON_OKAY = 1;
	static const int ID_GPM_BUTTON_CANCEL = 2;
	static const int ID_GPM_BUTTON_SEND_MESSAGE = 3;
	static const int ID_GPM_BUTTON_LOOT_TYPE = 4;
	static const int ID_GPM_BUTTON_LEAVE = 5;
	static const int ID_GPM_BUTTON_ADD = 6;
	static const int ID_GPM_BUTTON_TELL_MEMBER = 10;
	static const int ID_GPM_BUTTON_KICK_MEMBER = 20;

	bool m_CanLoot;

	static TTextTexture m_Text[9];

public:
	TGumpPartyManifest(DWORD serial, short x, short y, bool canLoot);
	virtual ~TGumpPartyManifest();
	
	static void InitTextTextures();
	static void ReleaseTextTextures();

	void PrepareTextures();

	int Draw(bool &mode);

	void GenerateFrame();

	void OnLeftMouseUp();
};
//---------------------------------------------------------------------------
#endif