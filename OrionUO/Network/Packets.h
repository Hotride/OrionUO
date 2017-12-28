/***********************************************************************************
**
** Packets.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef PACKETS_H
#define PACKETS_H
//----------------------------------------------------------------------------------
class CPacket : public WISP_DATASTREAM::CDataWritter
{
public:
	CPacket(const size_t &size, const bool &autoResize = false);

	void Send();
};
//----------------------------------------------------------------------------------
class CPacketFirstLogin : public CPacket
{
public:
	CPacketFirstLogin();
};
//----------------------------------------------------------------------------------
class CPacketSelectServer : public CPacket
{
public:
	CPacketSelectServer(uchar index);
};
//----------------------------------------------------------------------------------
class CPacketSecondLogin : public CPacket
{
public:
	CPacketSecondLogin();
};
//----------------------------------------------------------------------------------
class CPacketCreateCharacter : public CPacket
{
public:
	CPacketCreateCharacter(const string &name);
};
//----------------------------------------------------------------------------------
class CPacketDeleteCharacter : public CPacket
{
public:
	CPacketDeleteCharacter(const uint &charIndex);
};
//----------------------------------------------------------------------------------
class CPacketSelectCharacter : public CPacket
{
public:
	CPacketSelectCharacter(const uint &index, const string &name);
};
//----------------------------------------------------------------------------------
class CPacketPickupRequest : public CPacket
{
public:
	CPacketPickupRequest(uint serial, ushort count);
};
//----------------------------------------------------------------------------------
class CPacketDropRequestOld : public CPacket
{
public:
	CPacketDropRequestOld(uint serial, ushort x, ushort y, char z, uint container);
};
//----------------------------------------------------------------------------------
class CPacketDropRequestNew : public CPacket
{
public:
	CPacketDropRequestNew(uint serial, ushort x, ushort y, char z, uchar slot, uint container);
};
//----------------------------------------------------------------------------------
class CPacketEquipRequest : public CPacket
{
public:
	CPacketEquipRequest(uint serial, uchar layer, uint container);
};
//----------------------------------------------------------------------------------
class CPacketChangeWarmode : public CPacket
{
public:
	CPacketChangeWarmode(uchar state);
};
//----------------------------------------------------------------------------------
class CPacketHelpRequest : public CPacket
{
public:
	CPacketHelpRequest();
};
//----------------------------------------------------------------------------------
class CPacketStatusRequest : public CPacket
{
public:
	CPacketStatusRequest(uint serial);
};
//----------------------------------------------------------------------------------
class CPacketSkillsRequest : public CPacket
{
public:
	CPacketSkillsRequest(uint serial);
};
//----------------------------------------------------------------------------------
class CPacketSkillsStatusChangeRequest : public CPacket
{
public:
	CPacketSkillsStatusChangeRequest(uchar skill, uchar state);
};
//----------------------------------------------------------------------------------
class CPacketClickRequest : public CPacket
{
public:
	CPacketClickRequest(uint serial);
};
//----------------------------------------------------------------------------------
class CPacketDoubleClickRequest : public CPacket
{
public:
	CPacketDoubleClickRequest(uint serial);
};
//----------------------------------------------------------------------------------
class CPacketAttackRequest : public CPacket
{
public:
	CPacketAttackRequest(uint serial);
};
//----------------------------------------------------------------------------------
class CPacketClientVersion : public CPacket
{
public:
	CPacketClientVersion(string version);
};
//----------------------------------------------------------------------------------
class CPacketASCIISpeechRequest : public CPacket
{
public:
	CPacketASCIISpeechRequest(const char *text, SPEECH_TYPE type, ushort font, ushort color);
};
//----------------------------------------------------------------------------------
class CPacketUnicodeSpeechRequest : public CPacket
{
public:
	CPacketUnicodeSpeechRequest(const wchar_t *text, SPEECH_TYPE type, ushort font, ushort color, puchar language);
};
//----------------------------------------------------------------------------------
class CPacketCastSpell : public CPacket
{
public:
	CPacketCastSpell(int index);
};
//----------------------------------------------------------------------------------
class CPacketCastSpellFromBook : public CPacket
{
public:
	CPacketCastSpellFromBook(int index, uint serial);
};
//----------------------------------------------------------------------------------
class CPacketUseSkill : public CPacket
{
public:
	CPacketUseSkill(int index);
};
//----------------------------------------------------------------------------------
class CPacketOpenDoor : public CPacket
{
public:
	CPacketOpenDoor();
};
//----------------------------------------------------------------------------------
class CPacketOpenSpellbook : public CPacket
{
public:
	CPacketOpenSpellbook(SPELLBOOK_TYPE type);
};
//----------------------------------------------------------------------------------
class CPacketEmoteAction : public CPacket
{
public:
	CPacketEmoteAction(const char *action);
};
//----------------------------------------------------------------------------------
class CPacketGumpResponse : public CPacket
{
public:
	CPacketGumpResponse(class CGumpGeneric *gump, int code);
};
//----------------------------------------------------------------------------------
class CPacketVirtureGumpResponse : public CPacket
{
public:
	CPacketVirtureGumpResponse(class CGump *gump, int code);
};
//----------------------------------------------------------------------------------
class CPacketMenuResponse : public CPacket
{
public:
	CPacketMenuResponse(class CGump *gump, int code);
};
//----------------------------------------------------------------------------------
class CPacketGrayMenuResponse : public CPacket
{
public:
	CPacketGrayMenuResponse(class CGump *gump, int code);
};
//----------------------------------------------------------------------------------
class CPacketTradeResponse : public CPacket
{
public:
	CPacketTradeResponse(class CGumpSecureTrading *gump, int code);
};
//----------------------------------------------------------------------------------
class CPacketLogoutNotification : public CPacket
{
public:
	CPacketLogoutNotification();
};
//----------------------------------------------------------------------------------
class CPacketTextEntryDialogResponse : public CPacket
{
public:
	CPacketTextEntryDialogResponse(class CGumpTextEntryDialog *gump, class CEntryText *entry, bool code);
};
//----------------------------------------------------------------------------------
class CPacketRenameRequest : public CPacket
{
public:
	CPacketRenameRequest(uint serial, string newName);
};
//----------------------------------------------------------------------------------
class CPacketTipRequest : public CPacket
{
public:
	CPacketTipRequest(ushort id, uchar flag);
};
//----------------------------------------------------------------------------------
class CPacketASCIIPromptResponse : public CPacket
{
public:
	CPacketASCIIPromptResponse(const char *text, size_t len, bool cancel);
};
//----------------------------------------------------------------------------------
class CPacketUnicodePromptResponse : public CPacket
{
public:
	CPacketUnicodePromptResponse(const wchar_t *text, size_t len, const string &lang, bool cancel);
};
//----------------------------------------------------------------------------------
class CPacketDyeDataResponse : public CPacket
{
public:
	CPacketDyeDataResponse(uint serial, ushort graphic, ushort color);
};
//----------------------------------------------------------------------------------
class CPacketProfileRequest : public CPacket
{
public:
	CPacketProfileRequest(uint serial);
};
//----------------------------------------------------------------------------------
class CPacketProfileUpdate : public CPacket
{
public:
	CPacketProfileUpdate(uint serial, const wchar_t *text, size_t len);
};
//----------------------------------------------------------------------------------
class CPacketCloseStatusbarGump : public CPacket
{
public:
	CPacketCloseStatusbarGump(uint serial);
};
//----------------------------------------------------------------------------------
class CPacketPartyInviteRequest : public CPacket
{
public:
	CPacketPartyInviteRequest();
};
//----------------------------------------------------------------------------------
class CPacketPartyRemoveRequest : public CPacket
{
public:
	CPacketPartyRemoveRequest(uint serial);
};
//----------------------------------------------------------------------------------
class CPacketPartyChangeLootTypeRequest : public CPacket
{
public:
	CPacketPartyChangeLootTypeRequest(uchar type);
};
//----------------------------------------------------------------------------------
class CPacketPartyAccept : public CPacket
{
public:
	CPacketPartyAccept(uint serial);
};
//----------------------------------------------------------------------------------
class CPacketPartyDecline : public CPacket
{
public:
	CPacketPartyDecline(uint serial);
};
//----------------------------------------------------------------------------------
class CPacketPartyMessage : public CPacket
{
public:
	CPacketPartyMessage(const wchar_t *text, size_t len, uint serial = 0x00000000);
};
//----------------------------------------------------------------------------------
class CPacketGameWindowSize : public CPacket
{
public:
	CPacketGameWindowSize();
};
//----------------------------------------------------------------------------------
class CPacketClientViewRange : public CPacket
{
public:
	CPacketClientViewRange(uchar range);
};
//----------------------------------------------------------------------------------
class CPacketBulletinBoardRequestMessage : public CPacket
{
public:
	CPacketBulletinBoardRequestMessage(uint serial, uint msgSerial);
};
//----------------------------------------------------------------------------------
class CPacketBulletinBoardRequestMessageSummary : public CPacket
{
public:
	CPacketBulletinBoardRequestMessageSummary(uint serial, uint msgSerial);
};
//----------------------------------------------------------------------------------
class CPacketBulletinBoardPostMessage : public CPacket
{
public:
	CPacketBulletinBoardPostMessage(uint serial, uint replySerial, const char *subject, const char *message);
};
//----------------------------------------------------------------------------------
class CPacketBulletinBoardRemoveMessage : public CPacket
{
public:
	CPacketBulletinBoardRemoveMessage(uint serial, uint msgSerial);
};
//----------------------------------------------------------------------------------
class CPacketAssistVersion : public CPacket
{
public:
	CPacketAssistVersion(uint version, string clientVersion);
};
//----------------------------------------------------------------------------------
class CPacketRazorAnswer : public CPacket
{
public:
	CPacketRazorAnswer();
};
//----------------------------------------------------------------------------------
class CPacketLanguage : public CPacket
{
public:
	CPacketLanguage(const string &lang);
};
//----------------------------------------------------------------------------------
class CPacketClientType : public CPacket
{
public:
	CPacketClientType();
};
//----------------------------------------------------------------------------------
class CPacketRequestPopupMenu : public CPacket
{
public:
	CPacketRequestPopupMenu(uint serial);
};
//----------------------------------------------------------------------------------
class CPacketPopupMenuSelection : public CPacket
{
public:
	CPacketPopupMenuSelection(uint serial, ushort menuID);
};
//----------------------------------------------------------------------------------
class CPacketOpenChat : public CPacket
{
public:
	CPacketOpenChat(const wstring &name);
};
//----------------------------------------------------------------------------------
class CPacketMapMessage : public CPacket
{
public:
	CPacketMapMessage(uint serial, MAP_MESSAGE action, uchar pin = 0, short x = -24, short y = -31);
};
//----------------------------------------------------------------------------------
class CPacketGuildMenuRequest : public CPacket
{
public:
	CPacketGuildMenuRequest();
};
//----------------------------------------------------------------------------------
class CPacketQuestMenuRequest : public CPacket
{
public:
	CPacketQuestMenuRequest();
};
//----------------------------------------------------------------------------------
class CPacketEquipLastWeapon : public CPacket
{
public:
	CPacketEquipLastWeapon();
};
//----------------------------------------------------------------------------------
class CPacketVirtureRequest : public CPacket
{
public:
	CPacketVirtureRequest(int buttonID);
};
//----------------------------------------------------------------------------------
class CPacketInvokeVirtureRequest : public CPacket
{
public:
	CPacketInvokeVirtureRequest(uchar id);
};
//----------------------------------------------------------------------------------
class CPacketMegaClilocRequestOld : public CPacket
{
public:
	CPacketMegaClilocRequestOld(const uint &serial);
};
//----------------------------------------------------------------------------------
class CPacketMegaClilocRequest : public CPacket
{
public:
	CPacketMegaClilocRequest(UINT_LIST &list);
};
//----------------------------------------------------------------------------------
class CPacketChangeStatLockStateRequest : public CPacket
{
public:
	CPacketChangeStatLockStateRequest(uchar stat, uchar state);
};
//----------------------------------------------------------------------------------
class CPacketBookPageData : public CPacket
{
public:
	CPacketBookPageData(class CGumpBook *gump, int page);
};
//----------------------------------------------------------------------------------
class CPacketBookPageDataRequest : public CPacket
{
public:
	CPacketBookPageDataRequest(const uint &serial, const int &page);
};
//----------------------------------------------------------------------------------
class CPacketBuyRequest : public CPacket
{
public:
	CPacketBuyRequest(class CGumpShop *gump);
};
//----------------------------------------------------------------------------------
class CPacketSellRequest : public CPacket
{
public:
	CPacketSellRequest(class CGumpShop *gump);
};
//----------------------------------------------------------------------------------
class CPacketUseCombatAbility : public CPacket
{
public:
	CPacketUseCombatAbility(const uchar &index);
};
//----------------------------------------------------------------------------------
class CPacketTargetSelectedObject : public CPacket
{
public:
	CPacketTargetSelectedObject(const uint &useObjectSerial, const uint &targetObjectSerial);
};
//----------------------------------------------------------------------------------
class CPacketToggleGargoyleFlying : public CPacket
{
public:
	CPacketToggleGargoyleFlying();
};
//----------------------------------------------------------------------------------
class CPacketCustomHouseDataReq : public CPacket
{
public:
	CPacketCustomHouseDataReq(const uint &serial);
};
//----------------------------------------------------------------------------------
class CPacketStunReq : public CPacket
{
public:
	CPacketStunReq();
};
//----------------------------------------------------------------------------------
class CPacketDisarmReq : public CPacket
{
public:
	CPacketDisarmReq();
};
//----------------------------------------------------------------------------------
class CPacketResend : public CPacket
{
public:
	CPacketResend();
};
//----------------------------------------------------------------------------------
class CPacketWalkRequest : public CPacket
{
public:
	CPacketWalkRequest(const uchar &direction, const uchar &sequence, const uint &fastWalkKey);
};
//----------------------------------------------------------------------------------
class CPacketCustomHouseBackup : public CPacket
{
public:
	CPacketCustomHouseBackup();
};
//----------------------------------------------------------------------------------
class CPacketCustomHouseRestore : public CPacket
{
public:
	CPacketCustomHouseRestore();
};
//----------------------------------------------------------------------------------
class CPacketCustomHouseCommit : public CPacket
{
public:
	CPacketCustomHouseCommit();
};
//----------------------------------------------------------------------------------
class CPacketCustomHouseBuildingExit : public CPacket
{
public:
	CPacketCustomHouseBuildingExit();
};
//----------------------------------------------------------------------------------
class CPacketCustomHouseGoToFloor : public CPacket
{
public:
	CPacketCustomHouseGoToFloor(const uchar &floor);
};
//----------------------------------------------------------------------------------
class CPacketCustomHouseSync : public CPacket
{
public:
	CPacketCustomHouseSync();
};
//----------------------------------------------------------------------------------
class CPacketCustomHouseClear : public CPacket
{
public:
	CPacketCustomHouseClear();
};
//----------------------------------------------------------------------------------
class CPacketCustomHouseRevert : public CPacket
{
public:
	CPacketCustomHouseRevert();
};
//----------------------------------------------------------------------------------
class CPacketCustomHouseResponse : public CPacket
{
public:
	CPacketCustomHouseResponse();
};
//----------------------------------------------------------------------------------
class CPacketCustomHouseAddItem : public CPacket
{
public:
	CPacketCustomHouseAddItem(const ushort &graphic, const int &x, const int &y);
};
//----------------------------------------------------------------------------------
class CPacketCustomHouseDeleteItem : public CPacket
{
public:
	CPacketCustomHouseDeleteItem(const ushort &graphic, const int &x, const int &y, const int &z);
};
//----------------------------------------------------------------------------------
class CPacketCustomHouseAddRoof : public CPacket
{
public:
	CPacketCustomHouseAddRoof(const ushort &graphic, const int &x, const int &y, const int &z);
};
//----------------------------------------------------------------------------------
class CPacketCustomHouseDeleteRoof : public CPacket
{
public:
	CPacketCustomHouseDeleteRoof(const ushort &graphic, const int &x, const int &y, const int &z);
};
//----------------------------------------------------------------------------------
class CPacketCustomHouseAddStair : public CPacket
{
public:
	CPacketCustomHouseAddStair(const ushort &graphic, const int &x, const int &y);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
