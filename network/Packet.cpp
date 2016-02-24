/****************************************************************************
**
** Packet.cpp
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
#include "stdafx.h"
//---------------------------------------------------------------------------
TMessageFragment::TMessageFragment(PBYTE buf, int size, int msg_size)
: m_size(size), m_msg_size(msg_size)
{
	if (m_msg_size == 0) m_data = new BYTE[3];
	else m_data = new BYTE[msg_size];

	memcpy(m_data, buf, m_size);
}
//---------------------------------------------------------------------------
int TMessageFragment::Append(PBYTE buf, int size)
{
	if (m_msg_size == 0)
	{
		if(m_size + size < 3)
		{
			memcpy(m_data + m_size, buf, size);
			m_size += size;

			return size;
		}

		if (m_size == 2)
			m_msg_size = (m_data[1] << 8) | buf[0];
		else
			m_msg_size = (buf[0] << 8) | buf[1];

		PBYTE newbuf = new BYTE[m_msg_size];
		memcpy(newbuf, m_data, m_size);

		delete m_data;

		m_data = newbuf;
	}

	int append_size = m_msg_size - m_size;

	if (append_size > size)
		append_size = size;

	// Concatenate the new data onto the fragment
	memcpy(m_data + m_size, buf, append_size);
	m_size += append_size;

	return append_size;
}
//---------------------------------------------------------------------------
TPacket::TPacket(int size, bool removeBuffer)
: m_Size(size), m_RemoveBuffer(removeBuffer)
{
	if (removeBuffer && size > 0)
	{
		Buffer = new BYTE[size];
		memset(&Buffer[0], 0, size);
		Ptr = Buffer;
	}
	else
	{
		Buffer = NULL;
		Ptr = NULL;
	}
}
//---------------------------------------------------------------------------
TPacket::~TPacket()
{
	Free();
}
//---------------------------------------------------------------------------
void TPacket::Create(int size)
{
	if (Buffer != NULL)
	{
		delete Buffer;
		Buffer = NULL;
		Ptr = NULL;
		m_Size = 0;
	}

	if (size > 0)
	{
		m_Size = size;
		Buffer = new BYTE[size];
		memset(&Buffer[0], 0, size);
		Ptr = Buffer;
	}
}
//---------------------------------------------------------------------------
void TPacket::Free()
{
	if (m_RemoveBuffer && Buffer != NULL)
		delete Buffer;
	
	Ptr = NULL;
	Buffer = NULL;
	m_Size = 0;
}
//---------------------------------------------------------------------------
void TPacket::Send(int size)
{
	if (!size)
		size = m_Size;

	if (Buffer != NULL)
		UO->Send(Buffer, m_Size);
}
//---------------------------------------------------------------------------
void TPacket::WriteByte(BYTE val, int offset)
{
	*(Ptr + offset) = val;
	Ptr++;
}
//---------------------------------------------------------------------------
void TPacket:: WriteWord(WORD val, int offset)
{
	pack16(Ptr + offset, val);
	Ptr += 2;
}
//---------------------------------------------------------------------------
void TPacket:: WriteDWord(DWORD val, int offset)
{
	pack32(Ptr + offset, val);
	Ptr += 4;
}
//---------------------------------------------------------------------------
void TPacket:: WriteString(const char *str, int size, int offset)
{
	int len = strlen(str);

	if (len > size)
		len = size;

	strncpy((char*)(Ptr + offset), str, len);
	Ptr += size;
}
//---------------------------------------------------------------------------
void TPacket:: WritePByte(PBYTE buf, int size, int offset)
{
	memcpy((Ptr + offset), buf, size);
	Ptr += size;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TPacketFirstLogin::TPacketFirstLogin()
: TPacket(62, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));

	WriteByte(0x80);
	WriteString(MainScreen->m_Account->c_str(), 30);
	WriteString(MainScreen->m_Password->c_str(), 30);
	WriteByte(0xFF);
}
//---------------------------------------------------------------------------
TPacketSelectServer::TPacketSelectServer(BYTE index)
: TPacket(3, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	
	WriteByte(0xA0);
	WriteByte(0x00);
	WriteByte(index);
}
//---------------------------------------------------------------------------
TPacketSecondLogin::TPacketSecondLogin()
: TPacket(65, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0x91);
	WritePByte(g_GameSeed, 4);
	WriteString(MainScreen->m_Account->c_str(), 30);
	WriteString(MainScreen->m_Password->c_str(), 30);
}
//---------------------------------------------------------------------------
TPacketCreateCharacter::TPacketCreateCharacter(string name)
: TPacket(104, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	int skillsCount = 3;
	BYTE packetID = 0x00;

	if (PacketManager.GetClientVersion() >= CV_70160)
	{
		skillsCount++;
		AddSize(2);
		packetID = 0xF8;
	}

	WriteByte(packetID); //packetID
	WriteDWord(0xEDEDEDED);
	WriteDWord(0xFFFFFFFF);
	WriteByte(0x00); //pattern 3
	WriteString(name.c_str(), 30);
	WriteWord(0x0000); //?
	WriteDWord(0x00000000); //clientflag
	WriteDWord(0x00000000); //?
	WriteDWord(0x00000000); //logincount

	TProfession *profession = (TProfession*)ProfessionManager->Selected;
	BYTE val = (BYTE)profession->DescriptionIndex;
	WriteByte(val); //profession
	Move(15); //?
	val = (BYTE)CreateCharacterManager.Sex;
	WriteByte(val);
	val = profession->Str;
	WriteByte(val);
	val = profession->Dex;
	WriteByte(val);
	val = profession->Int;
	WriteByte(val);

	IFOR(i, 0, skillsCount)
	{
		val = profession->GetSkillIndex(i);
		if (val == 0xFF)
		{
			//error, skill is not selected
			WriteWord(0x0000);
		}
		else
		{
			WriteByte(val);
			WriteByte(profession->GetSkillValue(i));
		}
	}

	WriteWord(CreateCharacterManager.SkinTone);
	WriteWord(CreateCharacterManager.HairStyle);
	WriteWord(CreateCharacterManager.HairColor);
	WriteWord(CreateCharacterManager.BeardStyle);
	WriteWord(CreateCharacterManager.BeardColor);

	TCityItem *city = SelectTownScreen->m_City;

	WORD location = 0;
	if (city != NULL)
		location = city->LocationIndex;

	WriteWord(location); //location
	WriteWord(0x0000); //?

	WORD slot = 0xFFFF;
	IFOR(i, 0, CharacterList.Count)
	{
		if (!CharacterList.GetName(i).length())
		{
			slot = (WORD)i;
			break;
		}
	}
	WriteWord(slot); //slot

	WritePByte(ConnectionManager.GetClientIP(), 4);
	WriteWord(CreateCharacterManager.ShirtColor);
	WriteWord(CreateCharacterManager.PantsColor);
}
//---------------------------------------------------------------------------
TPacketDeleteCharacter::TPacketDeleteCharacter(DWORD charIndex)
: TPacket(39, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0x83);
	Move(30); //character password
	WriteDWord(charIndex);
	WritePByte(ConnectionManager.GetClientIP(), 4);
}
//---------------------------------------------------------------------------
TPacketSelectCharacter::TPacketSelectCharacter(DWORD index, string name)
: TPacket(73, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	int copyLen = name.length();
	if (copyLen > 30)
		copyLen = 30;
	memcpy(&g_SelectedCharName[0], name.c_str(), copyLen);

	WriteByte(0x5D);
	WriteDWord(0xEDEDEDED);
	WriteString(name.c_str(), 30);
	Move(30); //character password
	WriteDWord(index);
	WritePByte(ConnectionManager.GetClientIP(), 4);
}
//---------------------------------------------------------------------------
TPacketPickupRequest::TPacketPickupRequest(DWORD serial, WORD count)
: TPacket(7, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0x07);
	WriteDWord(serial);
	WriteWord(count);
}
//---------------------------------------------------------------------------
TPacketDropRequestOld::TPacketDropRequestOld(DWORD serial, WORD x, WORD y, char z, DWORD container)
: TPacket(14, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0x08);
	WriteDWord(serial);
	WriteWord(x);
	WriteWord(y);
	WriteByte(z);
	WriteDWord(container);
}
//---------------------------------------------------------------------------
TPacketDropRequestNew::TPacketDropRequestNew(DWORD serial, WORD x, WORD y, char z, BYTE slot, DWORD container)
: TPacket(15, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0x08);
	WriteDWord(serial);
	WriteWord(x);
	WriteWord(y);
	WriteByte(z);
	WriteByte(slot);
	WriteDWord(container);
}
//---------------------------------------------------------------------------
TPacketEquipRequest::TPacketEquipRequest(DWORD serial, BYTE layer, DWORD container)
: TPacket(10, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0x13);
	WriteDWord(serial);
	WriteByte(layer);
	WriteDWord(container);
}
//---------------------------------------------------------------------------
TPacketChangeWarmode::TPacketChangeWarmode(BYTE state)
: TPacket(5, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0x72);
	WriteByte(state);
	WriteWord(0x0032);
}
//---------------------------------------------------------------------------
TPacketHelpRequest::TPacketHelpRequest()
: TPacket(258, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0x9B);
}
//---------------------------------------------------------------------------
TPacketStatusRequest::TPacketStatusRequest(DWORD serial)
: TPacket(10, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0x34);
	WriteDWord(0xEDEDEDED);
	WriteByte(4);
	WriteDWord(serial);
}
//---------------------------------------------------------------------------
TPacketSkillsRequest::TPacketSkillsRequest(DWORD serial)
: TPacket(10, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0x34);
	WriteDWord(0xEDEDEDED);
	WriteByte(5);
	WriteDWord(serial);
}
//---------------------------------------------------------------------------
TPacketSkillsStatusChangeRequest::TPacketSkillsStatusChangeRequest(BYTE skill, BYTE state)
: TPacket(6, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0x3A);
	WriteWord(0x0006);
	WriteWord((WORD)skill);
	WriteByte(state);
}
//---------------------------------------------------------------------------
TPacketClickRequest::TPacketClickRequest(DWORD serial)
: TPacket(5, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0x09);
	WriteDWord(serial);
}
//---------------------------------------------------------------------------
TPacketDoubleClickRequest::TPacketDoubleClickRequest(DWORD serial)
: TPacket(5, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0x06);
	WriteDWord(serial);
}
//---------------------------------------------------------------------------
TPacketAttackRequest::TPacketAttackRequest(DWORD serial)
: TPacket(5, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0x05);
	WriteDWord(serial);
}
//---------------------------------------------------------------------------
TPacketClientVersion::TPacketClientVersion(string version)
: TPacket(0, true)
{
	int size = 4 + version.length();
	Create(size);
	
	WriteByte(0xBD);
	WriteWord((WORD)size);
	WriteString(version.c_str(), version.length());
}
//---------------------------------------------------------------------------
TPacketASCIISpeechRequest::TPacketASCIISpeechRequest(const char *text, SPEECH_TYPE type, WORD font, WORD color)
: TPacket(0, true)
{
	int len = strlen(text);
	int size = 8 + len + 1;
	Create(size);
	
	WriteByte(0x03);
	WriteWord((WORD)size);
	WriteByte((BYTE)type);
	WriteWord(color);
	WriteWord(font);
	WriteString(text, len);
}
//---------------------------------------------------------------------------
TPacketUnicodeSpeechRequest::TPacketUnicodeSpeechRequest(const wchar_t *text, SPEECH_TYPE type, WORD font, WORD color, PBYTE language)
: TPacket(0, true)
{
	int len = lstrlenW(text);
	int size = 12 + (len * 2) + 2;
	Create(size);
	
	WriteByte(0xAD);
	WriteWord((WORD)size);
	WriteByte((BYTE)type);
	WriteWord(color);
	WriteWord(font);
	WritePByte(language, 4);

	PBYTE str = (PBYTE)text;

	IFOR(i, 0, len)
	{
		*Ptr = *(str + 1);
		Ptr++;
		*Ptr = *str;
		Ptr++;
		str += 2;
	}
}
//---------------------------------------------------------------------------
TPacketCastSpell::TPacketCastSpell(int index)
: TPacket(8, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	char spell[5] = {0};
	sprintf(spell, "%i", index);

	int len = strlen(spell);
	int size = 5 + len;

	WriteByte(0x12);
	WriteWord((WORD)size);
	WriteByte(0x56);
	WriteString(spell, len);
	Size = size;
}
//---------------------------------------------------------------------------
TPacketCastSpellFromBook::TPacketCastSpellFromBook(int index, DWORD serial)
: TPacket(0, true)
{
	char spell[25] = {0};
	sprintf(spell, "%i %d", index, (int)serial);

	int len = strlen(spell);
	int size = 5 + len;
	Create(size);

	WriteByte(0x12);
	WriteWord((WORD)size);
	WriteByte(0x27);
	WriteString(spell, len);
	Size = size;
}
//---------------------------------------------------------------------------
TPacketUseSkill::TPacketUseSkill(int index)
: TPacket(10, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	char skill[10] = {0};
	sprintf(skill, "%d 0", index);

	int len = strlen(skill);
	int size = 5 + len;

	WriteByte(0x12);
	WriteWord((WORD)size);
	WriteByte(0x24);
	WriteString(skill, len);
	Size = size;
}
//---------------------------------------------------------------------------
TPacketOpenDoor::TPacketOpenDoor()
: TPacket(5, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0x12);
	WriteWord(0x0005);
	WriteByte(0x58);
}
//---------------------------------------------------------------------------
TPacketOpenSpellbook::TPacketOpenSpellbook(SPELLBOOK_TYPE type)
: TPacket(6, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0x12);
	WriteWord(0x0006);
	WriteByte(0x43);
	WriteByte((BYTE)type + 30);
}
//---------------------------------------------------------------------------
TPacketEmoteAction::TPacketEmoteAction(const char *action)
: TPacket(0, true)
{
	int len = strlen(action);
	int size = 5 + len;
	Create(size);
	
	WriteByte(0x12);
	WriteWord((WORD)size);
	WriteByte(0xC7);
	WriteString(action, len);
}
//---------------------------------------------------------------------------
TPacketGumpResponse::TPacketGumpResponse(TGump *gump, int code)
: TPacket(0, true)
{
	int switchesCount = 0;
	int textLinesCount = 0;
	int textLinesLength = 0;

	TGumpObject *item = (TGumpObject*)gump->m_Items;
	while (item != NULL)
	{
		switch (item->GetType())
		{
			case GOT_CHECKBOX:
			{
				if (((TGumpCheckbox*)item)->Action)
					switchesCount++;

				break;
			}
			case GOT_RADIO:
			{
				if (((TGumpRadio*)item)->Action)
					switchesCount++;

				break;
			}
			case GOT_TEXTENTRYLIMITED:
			case GOT_TEXTENTRY:
			{
				TGumpTextEntry *gte = (TGumpTextEntry*)item;

				textLinesCount++;
				textLinesLength += (gte->TextEntry->Length() * 2);

				break;
			}
			default:
				break;
		}

		item = (TGumpObject*)item->m_Next;
	}
	
	int size = 19 + (switchesCount *4) + 4 + ((textLinesCount * 4) + textLinesLength);
	Create(size);
	
	WriteByte(0xB1);
	WriteWord((WORD)size);
	WriteDWord(gump->Serial);
	WriteDWord(gump->ID);
	WriteDWord(code);
	WriteDWord(switchesCount);

	PBYTE textptr = Ptr + (switchesCount * 4);
	pack32(textptr, textLinesCount);
	textptr += 4;

	item = (TGumpObject*)gump->m_Items;

	while (item != NULL)
	{
		switch (item->Type)
		{
			case GOT_CHECKBOX:
			{
				if (((TGumpCheckbox*)item)->Action)
					WriteDWord(((TGumpCheckbox*)item)->Index);

				break;
			}
			case GOT_RADIO:
			{
				if (((TGumpRadio*)item)->Action)
					WriteDWord(((TGumpRadio*)item)->Index);

				break;
			}
			case GOT_TEXTENTRYLIMITED:
			case GOT_TEXTENTRY:
			{
				TGumpTextEntry *gte = (TGumpTextEntry*)item;

				pack16(textptr, gte->TextIndex);
				textptr += 2;
				int tlen = gte->TextEntry->Length();
				pack16(textptr, tlen);
				textptr += 2;
				const char *gteText = gte->TextEntry->c_str();
				
				IFOR(i, 0, tlen)
				{
					*textptr = 0;
					textptr++;
					*textptr = gteText[i];
					textptr++;
				}
				break;
			}
			default:
				break;
		}

		item = (TGumpObject*)item->m_Next;
	}
}
//---------------------------------------------------------------------------
TPacketMenuResponse::TPacketMenuResponse(TGump *gump, int code)
: TPacket(13, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0x7D);
	WriteDWord(gump->Serial);
	WriteWord((WORD)gump->ID);

	if (code)
	{
		WriteWord((WORD)code);

		TGumpObject *go = (TGumpObject*)gump->m_Items;
		int idx = 1;

		while (go != NULL)
		{
			if (idx == code)
			{
				WriteWord(go->Graphic);
				WriteWord(go->Color);

				break;
			}

			idx++;
			go = (TGumpObject*)go->m_Next;
		}
	}
}
//---------------------------------------------------------------------------
TPacketTradeResponse::TPacketTradeResponse(TGump *gump, int code)
: TPacket(17, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0x6F);
	WriteWord(17);

	if (code == 1) //Закрываем окно
	{
		WriteByte(0x01);
		WriteDWord(gump->ID);
	}
	else if (code == 2) //Ткнули на чекбокс
	{
		WriteByte(0x02);
		WriteDWord(gump->ID);
		WriteDWord((DWORD)((TGumpSecureTrading*)gump)->StateMy);
	}
}
//---------------------------------------------------------------------------
TPacketTextEntryDialogResponse::TPacketTextEntryDialogResponse(TGumpTextEntryDialog *gump, bool code)
: TPacket(0, true)
{
	int len = gump->TextEntry->Length();
	int size = 12 + len + 1;
	Create(size);
	
	WriteByte(0xAC);
	WriteWord((WORD)size);
	WriteDWord(gump->Serial);
	WriteByte(gump->Variant);
	WriteByte(gump->ButtonID);
	WriteByte((BYTE)(code ? 0x01 : 0x00));

	WriteWord((WORD)len);

	WriteString(gump->TextEntry->c_str(), len);

	Buffer[size - 1] = 0;
}
//---------------------------------------------------------------------------
TPacketRenameRequest::TPacketRenameRequest(DWORD serial, string newName)
: TPacket(35, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0x75);
	WriteDWord(serial);
	WriteString(newName.c_str(), newName.length());
}
//---------------------------------------------------------------------------
TPacketTipRequest::TPacketTipRequest(WORD id, BYTE flag)
: TPacket(4, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0xA7);
	WriteWord(id);
	WriteByte(flag);
}
//---------------------------------------------------------------------------
TPacketASCIIPromptResponse::TPacketASCIIPromptResponse(const char *text, int len, bool cancel)
: TPacket(0, true)
{
	int size = 15 + len + 1;
	Create(size);

	WritePByte(g_LastASCIIPrompt, 11);
	pack16(Buffer + 1, size);
	WriteDWord((int)((bool)!cancel));

	WriteString(text, len);
	Buffer[size - 1] = 0;
}
//---------------------------------------------------------------------------
TPacketUnicodePromptResponse::TPacketUnicodePromptResponse(const wchar_t *text, int len, const char *lang, bool cancel)
: TPacket(0, true)
{
	int size = 19 + (len * 2) + 2;
	Create(size);

	WritePByte(g_LastUnicodePrompt, 11);
	pack16(Buffer + 1, size);
	WriteDWord((int)((bool)!cancel));
	WriteString(lang, 4);

	WritePByte((PBYTE)text, len * 2);
}
//---------------------------------------------------------------------------
TPacketDyeDataResponse::TPacketDyeDataResponse(DWORD serial, WORD graphic, WORD color)
: TPacket(9, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0x95);
	WriteDWord(serial);
	WriteWord(graphic);
	WriteWord(color);
}
//---------------------------------------------------------------------------
TPacketProfileRequest::TPacketProfileRequest(DWORD serial)
: TPacket(8, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0xB8);
	WriteWord(0x0008);
	WriteByte(0);
	WriteDWord(serial);
}
//---------------------------------------------------------------------------
TPacketProfileUpdate::TPacketProfileUpdate(DWORD serial, const wchar_t *text, int len)
: TPacket(0, true)
{
	int size = 12 + (len * 2);
	Create(size);
	
	WriteByte(0xB8);
	WriteWord(size);
	WriteByte(1);
	WriteDWord(serial);
	WriteWord(0x0001);
	WriteWord((WORD)len);
	
	PBYTE str = (PBYTE)text;

	IFOR(i, 0, len)
	{
		*Ptr = *(str + 1);
		Ptr++;
		*Ptr = *str;
		Ptr++;
		str += 2;
	}
}
//---------------------------------------------------------------------------
TPacketCloseStatusbarGump::TPacketCloseStatusbarGump(DWORD serial)
: TPacket(9, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0xBF);
	WriteWord(0x0009);
	WriteWord(0x000C);
	WriteDWord(serial);
}
//---------------------------------------------------------------------------
TPacketPartyInviteRequest::TPacketPartyInviteRequest()
: TPacket(10, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0xBF);
	WriteWord(0x000a);
	WriteWord(0x0006);
	WriteByte(0x01);
	WriteDWord(0x00000000);
}
//---------------------------------------------------------------------------
TPacketPartyRemoveRequest::TPacketPartyRemoveRequest(DWORD serial)
: TPacket(10, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0xBF);
	WriteWord(0x000a);
	WriteWord(0x0006);
	WriteByte(0x02);
	WriteDWord(serial);
}
//---------------------------------------------------------------------------
TPacketPartyChangeLootTypeRequest::TPacketPartyChangeLootTypeRequest(BYTE type)
: TPacket(7, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0xBF);
	WriteWord(0x0007);
	WriteWord(0x0006);
	WriteByte(0x06);
	WriteByte(type);
}
//---------------------------------------------------------------------------
TPacketPartyAccept::TPacketPartyAccept(DWORD serial)
: TPacket(10, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0xBF);
	WriteWord(0x000a);
	WriteWord(0x0006);
	WriteByte(0x08);
	WriteDWord(serial);
}
//---------------------------------------------------------------------------
TPacketPartyDecline::TPacketPartyDecline(DWORD serial)
: TPacket(10, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0xBF);
	WriteWord(0x000a);
	WriteWord(0x0006);
	WriteByte(0x09);
	WriteDWord(serial);
}
//---------------------------------------------------------------------------
TPacketPartyMessage::TPacketPartyMessage(const wchar_t *text, int len, DWORD serial)
: TPacket(0, true)
{
	int size = 10 + (len * 2) + 2;
	Create(size);
	
	WriteByte(0xBF);
	WriteWord(size);
	WriteWord(0x0006);

	if (serial) //Private message to member
	{
		WriteByte(0x03);
		WriteDWord(serial);
	}
	else //Message to full party
		WriteByte(0x04);

	PBYTE str = (PBYTE)text;

	IFOR(i, 0, len)
	{
		*Ptr = *(str + 1);
		Ptr++;
		*Ptr = *str;
		Ptr++;
		str += 2;
	}
}
//---------------------------------------------------------------------------
TPacketGameWindowSize::TPacketGameWindowSize()
: TPacket(13, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0xBF);
	WriteWord(0x000D);
	WriteWord(0x0005);
	WriteWord(0);
	WriteWord(g_GameWindowWidth);
	WriteWord(g_GameWindowHeight);
	WriteWord(0);
}
//---------------------------------------------------------------------------
TPacketOpenGuildGump::TPacketOpenGuildGump()
: TPacket(10, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0xD7);
	WriteWord(0x000A);
	WriteWord(0x0002);
	WriteDWord(g_PlayerSerial);
	WriteWord(0x0028);
	WriteWord(0x07);
}
//---------------------------------------------------------------------------
TPacketOpenQuestGump::TPacketOpenQuestGump()
: TPacket(10, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0xD7);
	WriteWord(0x000A);
	WriteWord(0x0002);
	WriteDWord(g_PlayerSerial);
	WriteWord(0x0032);
	WriteWord(0x07);
}
//---------------------------------------------------------------------------
TPacketClientViewRange::TPacketClientViewRange(BYTE range)
: TPacket(2, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0xC8);

	if (range < 5)
		range = 5;
	else if (range > 18)
		range = 18;

	WriteByte(range);
}
//---------------------------------------------------------------------------
TPacketBulletinBoardRequestMessage::TPacketBulletinBoardRequestMessage(DWORD serial, DWORD msgSerial)
: TPacket(12, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0x71);
	WriteWord(0x000C);
	WriteByte(0x03);
	WriteDWord(serial);
	WriteDWord(msgSerial);
}
//---------------------------------------------------------------------------
TPacketBulletinBoardRequestMessageSummary::TPacketBulletinBoardRequestMessageSummary(DWORD serial, DWORD msgSerial)
: TPacket(12, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0x71);
	WriteWord(0x000C);
	WriteByte(0x04);
	WriteDWord(serial);
	WriteDWord(msgSerial);
}
//---------------------------------------------------------------------------
TPacketBulletinBoardPostMessage::TPacketBulletinBoardPostMessage(DWORD serial, DWORD replySerial, const char *subject, const char *message)
: TPacket(0, true)
{
	int subjectLen = strlen(subject);
	int size = 14 + subjectLen + 1;

	int lines = 1;

	int msgLen = strlen(message);
	int len = 0;

	IFOR(i, 0, msgLen)
	{
		if (message[i] == '\n')
		{
			len++;

			size += len + 1;
			len = 0;
			lines++;
		}
		else
			len++;
	}

	size += len + 2;

	Create(size);
	
	WriteByte(0x71);
	WriteWord(size);
	WriteByte(0x05);
	WriteDWord(serial);
	WriteDWord(replySerial);

	WriteByte(subjectLen + 1);
	WriteString(subject, subjectLen);
	WriteByte(0);

	WriteByte(lines);
	len = 0;
	char *msgPtr = (char*)message;

	IFOR(i, 0, msgLen)
	{
		if (msgPtr[len] == '\n')
		{
			len++;

			WriteByte(len);
			
			if (len > 1)
				WriteString(msgPtr, len - 1);

			WriteByte(0);

			msgPtr += len;
			len = 0;
		}
		else
			len++;
	}

	WriteByte(len + 1);
	WriteString(msgPtr, len);
	WriteByte(0);
}
//---------------------------------------------------------------------------
TPacketBulletinBoardRemoveMessage::TPacketBulletinBoardRemoveMessage(DWORD serial, DWORD msgSerial)
: TPacket(12, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0x71);
	WriteWord(0x000C);
	WriteByte(0x06);
	WriteDWord(serial);
	WriteDWord(msgSerial);
}
//---------------------------------------------------------------------------
TPacketAssistVersion::TPacketAssistVersion(DWORD version, string clientVersion)
: TPacket(0, true)
{
	int size = 7 + clientVersion.length();
	Create(size);
	
	WriteByte(0xBE);
	WriteWord((WORD)size);
	WriteDWord(version);
	WriteString(clientVersion.c_str(), clientVersion.length());
}
//---------------------------------------------------------------------------
TPacketRazorAnswer::TPacketRazorAnswer()
: TPacket(4, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0xF0);
	WriteWord(0x0004);
	WriteByte(0xFF);
}
//---------------------------------------------------------------------------
TPacketLanguage::TPacketLanguage(const char *lang)
: TPacket(0, true)
{
	int len = strlen(lang);
	int size = 5 + len + 1;
	Create(size);
	
	WriteByte(0xBF);
	WriteWord(0x0009);
	WriteWord(0x000B);
	WriteString(lang, len);
}
//---------------------------------------------------------------------------
TPacketClientType::TPacketClientType(DWORD type)
: TPacket(10, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0xBF);
	WriteWord(0x000A);
	WriteWord(0x000F);
	WriteByte(0x0A);
	WriteDWord(type);
}
//---------------------------------------------------------------------------
TPacketRequestPopupMenu::TPacketRequestPopupMenu(DWORD serial)
: TPacket(9, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0xBF);
	WriteWord(0x0009);
	WriteWord(0x0013);
	WriteDWord(serial);
}
//---------------------------------------------------------------------------
TPacketPopupMenuSelection::TPacketPopupMenuSelection(DWORD serial, WORD menuID)
: TPacket(11, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0xBF);
	WriteWord(0x000B);
	WriteWord(0x0015);
	WriteDWord(serial);
	WriteWord(menuID);
}
//---------------------------------------------------------------------------
TPacketOpenChat::TPacketOpenChat(const wchar_t *name)
: TPacket(64, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0xB5);

	int len = lstrlenW(name);

	if (len > 0)
	{
		if (len > 30)
			len = 30;

		PBYTE str = (PBYTE)name;

		IFOR(i, 0, len)
		{
			*Ptr = *(str + 1);
			Ptr++;
			*Ptr = *str;
			Ptr++;
			str += 2;
		}
	}
}
//---------------------------------------------------------------------------
TPacketLogoutNotification::TPacketLogoutNotification()
: TPacket(5, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0x01);
	WriteDWord(0xFFFFFFFF);
}
//---------------------------------------------------------------------------
TPacketMapMessage::TPacketMapMessage(DWORD serial, MAP_MESSAGE action, BYTE pin, short x, short y)
: TPacket(11, false)
{
	Buffer = m_Buf;
	Ptr = m_Buf;
	memset(&m_Buf[0], 0, sizeof(m_Buf));
	
	WriteByte(0x56);
	WriteDWord(serial);
	WriteByte((BYTE)action);
	WriteByte(pin);
	WriteWord(x);
	WriteWord(y);
}
//---------------------------------------------------------------------------