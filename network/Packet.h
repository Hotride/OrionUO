/****************************************************************************
**
** Packet.h
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
#ifndef PacketH
#define PacketH
//---------------------------------------------------------------------------
#include <windows.h>
//---------------------------------------------------------------------------
//Фрагментированное сообщение
class TMessageFragment
{
private:
	//Фрагмент данных
	PBYTE m_data;

	//Полученный размер данных
	int m_size;

	//Размер сообщения
	int m_msg_size;

public:
	TMessageFragment(PBYTE buf, int size, int msg_size);
	~TMessageFragment() {delete m_data;}

	int Append(PBYTE buf, int size);

	bool Complete() const {return m_size == m_msg_size && m_msg_size != 0;}

	PBYTE Get() {return m_data;}
	int Size() {return m_size;}
};
//---------------------------------------------------------------------------
//Базовый класс пакета
class TPacket
{
private:
	//Размер пакета
	int m_Size;

	//Удалять ли пакет при удалении пакета? (статичный буфер или динамический)
	bool m_RemoveBuffer;

public:
	TPacket(int size, bool removeBuffer = true);
	virtual ~TPacket();

	//Создать пакет указанного размера
	virtual void Create(int size);

	//Освободить память
	virtual void Free();

	//Отправить пакет
	virtual void Send(int size = 0);
	
	SETGETEX(int, Size);
	SETGET(bool, RemoveBuffer);

	//Указатель на буфер
	PBYTE Buffer;

	//Указатель на позицию в буфере
	PBYTE Ptr;

	//Переместиться по буферу
	void Move(int offset) {Ptr += offset;}

	//Записать байт
	void WriteByte(BYTE val, int offset = 0);

	//Записать слово
	void WriteWord(WORD val, int offset = 0);

	//Записать двойное слово
	void WriteDWord(DWORD val, int offset = 0);

	//Записать строку
	void WriteString(const char *str, int size, int offset = 0);

	//Записать массив байт
	void WritePByte(PBYTE buf, int size, int offset = 0);
};
//---------------------------------------------------------------------------
class TPacketFirstLogin : public TPacket
{
private:
	BYTE m_Buf[62];
public:
	TPacketFirstLogin();
};
//---------------------------------------------------------------------------
class TPacketSelectServer : public TPacket
{
private:
	BYTE m_Buf[3];
public:
	TPacketSelectServer(BYTE index);
};
//---------------------------------------------------------------------------
class TPacketSecondLogin : public TPacket
{
private:
	BYTE m_Buf[65];
public:
	TPacketSecondLogin();
};
//---------------------------------------------------------------------------
class TPacketCreateCharacter : public TPacket
{
private:
	BYTE m_Buf[106];
public:
	TPacketCreateCharacter(string name);
};
//---------------------------------------------------------------------------
class TPacketDeleteCharacter : public TPacket
{
private:
	BYTE m_Buf[39];
public:
	TPacketDeleteCharacter(DWORD charIndex);
};
//---------------------------------------------------------------------------
class TPacketSelectCharacter : public TPacket
{
private:
	BYTE m_Buf[73];
public:
	TPacketSelectCharacter(DWORD index, string name);
};
//---------------------------------------------------------------------------
class TPacketPickupRequest : public TPacket
{
private:
	BYTE m_Buf[7];
public:
	TPacketPickupRequest(DWORD serial, WORD count);
};
//---------------------------------------------------------------------------
class TPacketDropRequestOld : public TPacket
{
private:
	BYTE m_Buf[14];
public:
	TPacketDropRequestOld(DWORD serial, WORD x, WORD y, char z, DWORD container);
};
//---------------------------------------------------------------------------
class TPacketDropRequestNew : public TPacket
{
private:
	BYTE m_Buf[15];
public:
	TPacketDropRequestNew(DWORD serial, WORD x, WORD y, char z, BYTE slot, DWORD container);
};
//---------------------------------------------------------------------------
class TPacketEquipRequest : public TPacket
{
private:
	BYTE m_Buf[10];
public:
	TPacketEquipRequest(DWORD serial, BYTE layer, DWORD container);
};
//---------------------------------------------------------------------------
class TPacketChangeWarmode : public TPacket
{
private:
	BYTE m_Buf[5];
public:
	TPacketChangeWarmode(BYTE state);
};
//---------------------------------------------------------------------------
class TPacketHelpRequest : public TPacket
{
private:
	BYTE m_Buf[258];
public:
	TPacketHelpRequest();
};
//---------------------------------------------------------------------------
class TPacketStatusRequest : public TPacket
{
private:
	BYTE m_Buf[10];
public:
	TPacketStatusRequest(DWORD serial);
};
//---------------------------------------------------------------------------
class TPacketSkillsRequest : public TPacket
{
private:
	BYTE m_Buf[10];
public:
	TPacketSkillsRequest(DWORD serial);
};
//---------------------------------------------------------------------------
class TPacketSkillsStatusChangeRequest : public TPacket
{
private:
	BYTE m_Buf[6];
public:
	TPacketSkillsStatusChangeRequest(BYTE skill, BYTE state);
};
//---------------------------------------------------------------------------
class TPacketClickRequest : public TPacket
{
private:
	BYTE m_Buf[5];
public:
	TPacketClickRequest(DWORD serial);
};
//---------------------------------------------------------------------------
class TPacketDoubleClickRequest : public TPacket
{
private:
	BYTE m_Buf[5];
public:
	TPacketDoubleClickRequest(DWORD serial);
};
//---------------------------------------------------------------------------
class TPacketAttackRequest : public TPacket
{
private:
	BYTE m_Buf[5];
public:
	TPacketAttackRequest(DWORD serial);
};
//---------------------------------------------------------------------------
class TPacketClientVersion : public TPacket
{
private:
public:
	TPacketClientVersion(string version);
};
//---------------------------------------------------------------------------
class TPacketASCIISpeechRequest : public TPacket
{
private:
public:
	TPacketASCIISpeechRequest(const char *text, SPEECH_TYPE type, WORD font, WORD color);
};
//---------------------------------------------------------------------------
class TPacketUnicodeSpeechRequest : public TPacket
{
private:
public:
	TPacketUnicodeSpeechRequest(const wchar_t *text, SPEECH_TYPE type, WORD font, WORD color, PBYTE language);
};
//---------------------------------------------------------------------------
class TPacketCastSpell : public TPacket
{
private:
	BYTE m_Buf[8];
public:
	TPacketCastSpell(int index);
};
//---------------------------------------------------------------------------
class TPacketCastSpellFromBook : public TPacket
{
private:
public:
	TPacketCastSpellFromBook(int index, DWORD serial);
};
//---------------------------------------------------------------------------
class TPacketUseSkill : public TPacket
{
private:
	BYTE m_Buf[10];
public:
	TPacketUseSkill(int index);
};
//---------------------------------------------------------------------------
class TPacketOpenDoor : public TPacket
{
private:
	BYTE m_Buf[5];
public:
	TPacketOpenDoor();
};
//---------------------------------------------------------------------------
class TPacketOpenSpellbook : public TPacket
{
private:
	BYTE m_Buf[6];
public:
	TPacketOpenSpellbook(SPELLBOOK_TYPE type);
};
//---------------------------------------------------------------------------
class TPacketEmoteAction : public TPacket
{
private:
public:
	TPacketEmoteAction(const char *action);
};
//---------------------------------------------------------------------------
class TGump;
//---------------------------------------------------------------------------
class TPacketGumpResponse : public TPacket
{
private:
public:
	TPacketGumpResponse(TGump *gump, int code);
};
//---------------------------------------------------------------------------
class TPacketMenuResponse : public TPacket
{
private:
	BYTE m_Buf[13];
public:
	TPacketMenuResponse(TGump *gump, int code);
};
//---------------------------------------------------------------------------
class TPacketTradeResponse : public TPacket
{
private:
	BYTE m_Buf[17];
public:
	TPacketTradeResponse(TGump *gump, int code);
};
//---------------------------------------------------------------------------
class TGumpTextEntryDialog;
//---------------------------------------------------------------------------
class TPacketTextEntryDialogResponse : public TPacket
{
private:
public:
	TPacketTextEntryDialogResponse(TGumpTextEntryDialog *gump, bool code);
};
//---------------------------------------------------------------------------
class TPacketRenameRequest : public TPacket
{
private:
	BYTE m_Buf[35];
public:
	TPacketRenameRequest(DWORD serial, string newName);
};
//---------------------------------------------------------------------------
class TPacketTipRequest : public TPacket
{
private:
	BYTE m_Buf[4];
public:
	TPacketTipRequest(WORD id, BYTE flag);
};
//---------------------------------------------------------------------------
class TPacketASCIIPromptResponse : public TPacket
{
private:
public:
	TPacketASCIIPromptResponse(const char *text, int len, bool cancel);
};
//---------------------------------------------------------------------------
class TPacketUnicodePromptResponse : public TPacket
{
private:
public:
	TPacketUnicodePromptResponse(const wchar_t *text, int len, const char *lang, bool cancel);
};
//---------------------------------------------------------------------------
class TPacketDyeDataResponse : public TPacket
{
private:
	BYTE m_Buf[9];
public:
	TPacketDyeDataResponse(DWORD serial, WORD graphic, WORD color);
};
//---------------------------------------------------------------------------
class TPacketProfileRequest : public TPacket
{
private:
	BYTE m_Buf[8];
public:
	TPacketProfileRequest(DWORD serial);
};
//---------------------------------------------------------------------------
class TPacketProfileUpdate : public TPacket
{
private:
public:
	TPacketProfileUpdate(DWORD serial, const wchar_t *text, int len);
};
//---------------------------------------------------------------------------
class TPacketCloseStatusbarGump : public TPacket
{
private:
	BYTE m_Buf[9];
public:
	TPacketCloseStatusbarGump(DWORD serial);
};
//---------------------------------------------------------------------------
class TPacketPartyInviteRequest : public TPacket
{
private:
	BYTE m_Buf[10];
public:
	TPacketPartyInviteRequest();
};
//---------------------------------------------------------------------------
class TPacketPartyRemoveRequest : public TPacket
{
private:
	BYTE m_Buf[10];
public:
	TPacketPartyRemoveRequest(DWORD serial);
};
//---------------------------------------------------------------------------
class TPacketPartyChangeLootTypeRequest : public TPacket
{
private:
	BYTE m_Buf[7];
public:
	TPacketPartyChangeLootTypeRequest(BYTE type);
};
//---------------------------------------------------------------------------
class TPacketPartyAccept : public TPacket
{
private:
	BYTE m_Buf[10];
public:
	TPacketPartyAccept(DWORD serial);
};
//---------------------------------------------------------------------------
class TPacketPartyDecline : public TPacket
{
private:
	BYTE m_Buf[10];
public:
	TPacketPartyDecline(DWORD serial);
};
//---------------------------------------------------------------------------
class TPacketPartyMessage : public TPacket
{
private:
public:
	TPacketPartyMessage(const wchar_t *text, int len, DWORD serial = 0x00000000);
};
//---------------------------------------------------------------------------
class TPacketGameWindowSize : public TPacket
{
private:
	BYTE m_Buf[13];
public:
	TPacketGameWindowSize();
};
//---------------------------------------------------------------------------
class TPacketOpenGuildGump : public TPacket
{
private:
	BYTE m_Buf[10];
public:
	TPacketOpenGuildGump();
};
//---------------------------------------------------------------------------
class TPacketOpenQuestGump : public TPacket
{
private:
	BYTE m_Buf[10];
public:
	TPacketOpenQuestGump();
};
//---------------------------------------------------------------------------
class TPacketClientViewRange : public TPacket
{
private:
	BYTE m_Buf[2];
public:
	TPacketClientViewRange(BYTE range);
};
//---------------------------------------------------------------------------
class TPacketBulletinBoardRequestMessage : public TPacket
{
private:
	BYTE m_Buf[12];
public:
	TPacketBulletinBoardRequestMessage(DWORD serial, DWORD msgSerial);
};
//---------------------------------------------------------------------------
class TPacketBulletinBoardRequestMessageSummary : public TPacket
{
private:
	BYTE m_Buf[12];
public:
	TPacketBulletinBoardRequestMessageSummary(DWORD serial, DWORD msgSerial);
};
//---------------------------------------------------------------------------
class TPacketBulletinBoardPostMessage : public TPacket
{
private:
public:
	TPacketBulletinBoardPostMessage(DWORD serial, DWORD replySerial, const char *subject, const char *message);
};
//---------------------------------------------------------------------------
class TPacketBulletinBoardRemoveMessage : public TPacket
{
private:
	BYTE m_Buf[12];
public:
	TPacketBulletinBoardRemoveMessage(DWORD serial, DWORD msgSerial);
};
//---------------------------------------------------------------------------
class TPacketAssistVersion : public TPacket
{
private:
public:
	TPacketAssistVersion(DWORD version, string clientVersion);
};
//---------------------------------------------------------------------------
class TPacketRazorAnswer : public TPacket
{
private:
	BYTE m_Buf[4];
public:
	TPacketRazorAnswer();
};
//---------------------------------------------------------------------------
class TPacketLanguage : public TPacket
{
private:
public:
	TPacketLanguage(const char *lang);
};
//---------------------------------------------------------------------------
class TPacketClientType : public TPacket
{
private:
	BYTE m_Buf[10];
public:
	TPacketClientType(DWORD type);
};
//---------------------------------------------------------------------------
class TPacketRequestPopupMenu : public TPacket
{
private:
	BYTE m_Buf[9];
public:
	TPacketRequestPopupMenu(DWORD serial);
};
//---------------------------------------------------------------------------
class TPacketPopupMenuSelection : public TPacket
{
private:
	BYTE m_Buf[11];
public:
	TPacketPopupMenuSelection(DWORD serial, WORD menuID);
};
//---------------------------------------------------------------------------
class TPacketOpenChat : public TPacket
{
private:
	BYTE m_Buf[64];
public:
	TPacketOpenChat(const wchar_t *name);
};
//---------------------------------------------------------------------------
class TPacketLogoutNotification : public TPacket
{
private:
	BYTE m_Buf[5];
public:
	TPacketLogoutNotification();
};
//---------------------------------------------------------------------------
class TPacketMapMessage : public TPacket
{
private:
	BYTE m_Buf[11];
public:
	TPacketMapMessage(DWORD serial, MAP_MESSAGE action, BYTE pin = 0, short x = -24, short y = -31);
};
//---------------------------------------------------------------------------
class TPacketGuildMenuRequest : public TPacket
{
private:
	BYTE m_Buf[10];
public:
	TPacketGuildMenuRequest();
};
//---------------------------------------------------------------------------
class TPacketQuestMenuRequest : public TPacket
{
private:
	BYTE m_Buf[10];
public:
	TPacketQuestMenuRequest();
};
//---------------------------------------------------------------------------
class TPacketVirtureRequest : public TPacket
{
private:
	BYTE m_Buf[23];
public:
	TPacketVirtureRequest(int buttonID);
};
//---------------------------------------------------------------------------
class TPacketInvokeVirtureRequest : public TPacket
{
private:
	BYTE m_Buf[6];
public:
	TPacketInvokeVirtureRequest(BYTE id);
};
//---------------------------------------------------------------------------
class TPacketMegaClilocRequest : public TPacket
{
private:
public:
	TPacketMegaClilocRequest(vector<DWORD> list);
};
//---------------------------------------------------------------------------
class TPacketPopupMenuRequest : public TPacket
{
private:
	BYTE m_Buf[9];
public:
	TPacketPopupMenuRequest(DWORD serial);
};
//---------------------------------------------------------------------------
#endif