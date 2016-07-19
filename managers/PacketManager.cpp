/****************************************************************************
**
** PacketManager.cpp
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
//----------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------
TPacketManager PacketManager;
//----------------------------------------------------------------------------
//Карта пакетов УО для анализа
#define UMSG(size) { "?", size, DIR_BOTH, 0 }
// A message type sent to the server
#define SMSG(name, size) { name, size, DIR_SEND, 0 }
// A message type received from the server
#define RMSG(name, size) { name, size, DIR_RECV, 0 }
// A message type transmitted in both directions
#define BMSG(name, size) { name, size, DIR_BOTH, 0 }
// Message types that have handler methods
#define RMSGH(name, size, rmethod) \
	{ name, size, DIR_RECV, &TPacketManager::Handle ##rmethod }
#define BMSGH(name, size, rmethod) \
	{ name, size, DIR_BOTH, &TPacketManager::Handle ##rmethod }
//---------------------------------------------------------------------------
TMessageType TPacketManager::m_MessageTypes[0x100] =
{
	/*0x00*/ SMSG("Create Character", 0x68),
	/*0x01*/ SMSG("Disconnect", 0x05),
	/*0x02*/ SMSG("Walk Request", 0x07),
	/*0x03*/ BMSGH("Client Talk", SIZE_VARIABLE, ClientTalk),
	/*0x04*/ SMSG("Request God mode (God client)", 0x02),
	/*0x05*/ SMSG("Attack", 0x05),
	/*0x06*/ SMSG("Double Click", 0x05),
	/*0x07*/ SMSG("Pick Up Item", 0x07),
	/*0x08*/ SMSG("Drop Item", 0x0e),
	/*0x09*/ SMSG("Single Click", 0x05),
	/*0x0A*/ SMSG("Edit (God client)", 0x0b),
	/*0x0B*/ RMSGH("Damage Visualization", 0x07, Damage),
	/*0x0C*/ BMSG("Edit tiledata (God client)", SIZE_VARIABLE),
	/*0x0D*/ UMSG(0x03),
	/*0x0E*/ UMSG(0x01),
	/*0x0F*/ UMSG(0x3d),
	/*0x10*/ UMSG(0xd7),
	/*0x11*/ RMSGH("Character Status", SIZE_VARIABLE, CharacterStatus),
	/*0x12*/ SMSG("Perform Action", SIZE_VARIABLE),
	/*0x13*/ SMSG("Client Equip Item", 0x0a),
	/*0x14*/ SMSG("Send elevation (God client)", 0x06),
	/*0x15*/ BMSG("Follow", 0x09),
	/*0x16*/ UMSG(0x01),
	/*0x17*/ RMSG("Health status bar update (KR)", SIZE_VARIABLE),
	/*0x18*/ UMSG(SIZE_VARIABLE),
	/*0x19*/ UMSG(SIZE_VARIABLE),
	/*0x1A*/ RMSGH("Update Item", SIZE_VARIABLE, UpdateItem),
	/*0x1B*/ RMSGH("Enter World", 0x25, EnterWorld),
	/*0x1C*/ RMSGH("Server Talk", SIZE_VARIABLE, Talk),
	/*0x1D*/ RMSGH("Delete Object", 0x05, DeleteObject),
	/*0x1E*/ UMSG(0x04),
	/*0x1F*/ UMSG(0x08),
	/*0x20*/ RMSGH("Update Player", 0x13, UpdatePlayer),
	/*0x21*/ RMSGH("Deny Walk", 0x08, DenyWalk),
	/*0x22*/ BMSGH("Confirm Walk", 0x03, ConfirmWalk),
	/*0x23*/ RMSGH("Drag Animation", 0x1a, DragAnimation),
	/*0x24*/ RMSGH("Open Container", 0x07, OpenContainer),
	/*0x25*/ RMSGH("Update Contained Item", 0x14, UpdateContainedItem),
	/*0x26*/ UMSG(0x05),
	/*0x27*/ RMSGH("Deny Move Item", 0x02, DenyMoveItem),
	/*0x28*/ UMSG(0x05),
	/*0x29*/ RMSG("Drop Item Approved", 0x01),
	/*0x2A*/ UMSG(0x05),
	/*0x2B*/ UMSG(0x02),
	/*0x2C*/ BMSGH("Death Screen", 0x02, DeathScreen),
	/*0x2D*/ RMSG("Mob Attributes", 0x11),
	/*0x2E*/ RMSGH("Server Equip Item", 0x0f, EquipItem),
	/*0x2F*/ RMSG("Combat Notification", 0x0a),
	/*0x30*/ RMSG("Attack ok", 0x05),
	/*0x31*/ RMSG("Attack end", 0x01),
	/*0x32*/ UMSG(0x02),
	/*0x33*/ RMSGH("Pause Control", 0x02, PauseControl),
	/*0x34*/ SMSG("Status Request", 0x0a),
	/*0x35*/ UMSG(0x28d),
	/*0x36*/ UMSG(SIZE_VARIABLE),
	/*0x37*/ UMSG(0x08),
	/*0x38*/ BMSG("Pathfinding in Client", 0x07),
	/*0x39*/ RMSG("Remove (Group)", 0x09),
	/*0x3A*/ BMSGH("Update Skills", SIZE_VARIABLE, UpdateSkills),
	/*0x3B*/ BMSGH("Vendor Buy Reply", SIZE_VARIABLE, BuyReply),
	/*0x3C*/ RMSGH("Update Contained Items", SIZE_VARIABLE, UpdateContainedItems),
	/*0x3D*/ UMSG(0x02),
	/*0x3E*/ UMSG(0x25),
	/*0x3F*/ RMSG("Update Statics (God Client)", SIZE_VARIABLE),
	/*0x40*/ UMSG(0xc9),
	/*0x41*/ UMSG(SIZE_VARIABLE),
	/*0x42*/ UMSG(SIZE_VARIABLE),
	/*0x43*/ UMSG(0x229),
	/*0x44*/ UMSG(0x2c9),
	/*0x45*/ BMSG("Version OK", 0x05),
	/*0x46*/ UMSG(SIZE_VARIABLE),
	/*0x47*/ UMSG(0x0b),
	/*0x48*/ UMSG(0x49),
	/*0x49*/ UMSG(0x5d),
	/*0x4A*/ UMSG(0x05),
	/*0x4B*/ UMSG(0x09),
	/*0x4C*/ UMSG(SIZE_VARIABLE),
	/*0x4D*/ UMSG(SIZE_VARIABLE),
	/*0x4E*/ RMSGH("Personal Light Level", 0x06, PersonalLightLevel),
	/*0x4F*/ RMSGH("Global Light Level", 0x02, LightLevel),
	/*0x50*/ UMSG(SIZE_VARIABLE),
	/*0x51*/ UMSG(SIZE_VARIABLE),
	/*0x52*/ UMSG(SIZE_VARIABLE),
	/*0x53*/ RMSGH("Error Code", 0x02, ErrorCode),
	/*0x54*/ RMSGH("Sound Effect", 0x0c, PlaySoundEffect),
	/*0x55*/ RMSGH("Login Complete", 0x01, LoginComplete),
	/*0x56*/ BMSGH("Map Data", 0x0b, MapData),
	/*0x57*/ BMSG("Update Regions", 0x6e),
	/*0x58*/ UMSG(0x6a),
	/*0x59*/ UMSG(SIZE_VARIABLE),
	/*0x5A*/ UMSG(SIZE_VARIABLE),
	/*0x5B*/ RMSGH("Set Time", 0x04, SetTime),
	/*0x5C*/ BMSG("Restart Version", 0x02),
	/*0x5D*/ SMSG("Select Character", 0x49),
	/*0x5E*/ UMSG(SIZE_VARIABLE),
	/*0x5F*/ UMSG(0x31),
	/*0x60*/ UMSG(0x05),
	/*0x61*/ UMSG(0x09),
	/*0x62*/ UMSG(0x0f),
	/*0x63*/ UMSG(0x0d),
	/*0x64*/ UMSG(0x01),
	/*0x65*/ RMSGH("Set Weather", 0x04, SetWeather),
	/*0x66*/ BMSGH("Book Page Data", SIZE_VARIABLE, BookData),
	/*0x67*/ UMSG(0x15),
	/*0x68*/ UMSG(SIZE_VARIABLE),
	/*0x69*/ UMSG(SIZE_VARIABLE),
	/*0x6A*/ UMSG(0x03),
	/*0x6B*/ UMSG(0x09),
	/*0x6C*/ BMSGH("Target Data", 0x13, Target),
	/*0x6D*/ RMSGH("Play Music", 0x03, PlayMusic),
	/*0x6E*/ RMSGH("Character Animation", 0x0e, CharacterAnimation),
	/*0x6F*/ BMSGH("Secure Trading", SIZE_VARIABLE, SecureTrading),
	/*0x70*/ RMSGH("Graphic Effect", 0x1c, GraphicEffect),
	/*0x71*/ BMSGH("Bulletin Board Data", SIZE_VARIABLE, BulletinBoardData),
	/*0x72*/ BMSGH("War Mode", 0x05, Warmode),
	/*0x73*/ BMSGH("Ping", 0x02, Ping),
	/*0x74*/ RMSGH("Vendor Buy List", SIZE_VARIABLE, BuyList),
	/*0x75*/ SMSG("Rename Character", 0x23),
	/*0x76*/ RMSG("New Subserver", 0x10),
	/*0x77*/ RMSGH("Update Character", 0x11, UpdateCharacter),
	/*0x78*/ RMSGH("Update Object", SIZE_VARIABLE, UpdateObject),
	/*0x79*/ UMSG(0x09),
	/*0x7A*/ UMSG(SIZE_VARIABLE),
	/*0x7B*/ UMSG(0x02),
	/*0x7C*/ RMSGH("Open Menu Gump", SIZE_VARIABLE, OpenMenuGump),
	/*0x7D*/ SMSG("Menu Choice", 0x0d),
	/*0x7E*/ UMSG(0x02),
	/*0x7F*/ UMSG(SIZE_VARIABLE),
	/*0x80*/ SMSG("First Login", 0x3e),
	/*0x81*/ UMSG(SIZE_VARIABLE),
	/*0x82*/ RMSGH("Login Error", 0x02, LoginError),
	/*0x83*/ SMSG("Delete Character", 0x27),
	/*0x84*/ UMSG(0x45),
	/*0x85*/ RMSGH("Character List Notification", 0x02, CharacterListNotification),
	/*0x86*/ RMSGH("Resend Character List", SIZE_VARIABLE, ResendCharacterList),
	/*0x87*/ UMSG(SIZE_VARIABLE),
	/*0x88*/ RMSGH("Open Paperdoll", 0x42, OpenPaperdoll),
	/*0x89*/ RMSGH("Corpse Equipment", SIZE_VARIABLE, CorpseEquipment),
	/*0x8A*/ UMSG(SIZE_VARIABLE),
	/*0x8B*/ UMSG(SIZE_VARIABLE),
	/*0x8C*/ RMSGH("Relay Server", 0x0b, RelayServer),
	/*0x8D*/ UMSG(SIZE_VARIABLE),
	/*0x8E*/ UMSG(SIZE_VARIABLE),
	/*0x8F*/ UMSG(SIZE_VARIABLE),
	/*0x90*/ RMSGH("Display Map", 0x13, DisplayMap),
	/*0x91*/ SMSG("Second Login", 0x41),
	/*0x92*/ UMSG(SIZE_VARIABLE),
	/*0x93*/ RMSGH("Open Book", 0x63, OpenBook),
	/*0x94*/ UMSG(SIZE_VARIABLE),
	/*0x95*/ BMSGH("Dye Data", 0x09, DyeData),
	/*0x96*/ UMSG(SIZE_VARIABLE),
	/*0x97*/ RMSG("Move Player", 0x02),
	/*0x98*/ BMSG("All Names (3D Client Only)", SIZE_VARIABLE),
	/*0x99*/ BMSGH("Multi Placement", 0x1a, MultiPlacement),
	/*0x9A*/ BMSGH("ASCII Prompt", SIZE_VARIABLE, ASCIIPrompt),
	/*0x9B*/ SMSG("Help Request", 0x102),
	/*0x9C*/ UMSG(0x135),
	/*0x9D*/ UMSG(0x33),
	/*0x9E*/ RMSGH("Vendor Sell List", SIZE_VARIABLE, SellList),
	/*0x9F*/ SMSG("Vendor Sell Reply", SIZE_VARIABLE),
	/*0xA0*/ SMSG("Select Server", 0x03),
	/*0xA1*/ RMSGH("Update Hitpoints", 0x09, UpdateHitpoints),
	/*0xA2*/ RMSGH("Update Mana", 0x09, UpdateMana),
	/*0xA3*/ RMSGH("Update Stamina", 0x09, UpdateStamina),
	/*0xA4*/ SMSG("System Information", 0x95),
	/*0xA5*/ RMSGH("Open URL", SIZE_VARIABLE, OpenUrl),
	/*0xA6*/ RMSGH("Tip Window", SIZE_VARIABLE, TipWindow),
	/*0xA7*/ SMSG("Request Tip", 0x04),
	/*0xA8*/ RMSGH("Server List", SIZE_VARIABLE, ServerList),
	/*0xA9*/ RMSGH("Character List", SIZE_VARIABLE, CharacterList),
	/*0xAA*/ RMSGH("Attack Reply", 0x05, AttackCharacter),
	/*0xAB*/ RMSGH("Text Entry Dialog", SIZE_VARIABLE, TextEntryDialog),
	/*0xAC*/ SMSG("Text Entry Dialog Reply", SIZE_VARIABLE),
	/*0xAD*/ SMSG("Unicode Client Talk", SIZE_VARIABLE),
	/*0xAE*/ RMSGH("Unicode Server Talk", SIZE_VARIABLE, UnicodeTalk),
	/*0xAF*/ RMSGH("Display Death", 0x0d, DisplayDeath),
	/*0xB0*/ RMSGH("Open Dialog Gump", SIZE_VARIABLE, OpenGump),
	/*0xB1*/ SMSG("Dialog Choice", SIZE_VARIABLE),
	/*0xB2*/ BMSG("Chat Data", SIZE_VARIABLE),
	/*0xB3*/ RMSG("Chat Text ?", SIZE_VARIABLE),
	/*0xB4*/ UMSG(SIZE_VARIABLE),
	/*0xB5*/ BMSGH("Open Chat Window", 0x40, OpenChat),
	/*0xB6*/ SMSG("Popup Help Request", 0x09),
	/*0xB7*/ RMSG("Popup Help Data", SIZE_VARIABLE),
	/*0xB8*/ BMSGH("Character Profile", SIZE_VARIABLE, CharacterProfile),
	/*0xB9*/ RMSGH("Enable locked client features", 0x03, EnableLockedFeatures),
	/*0xBA*/ RMSGH("Display Quest Arrow", 0x06, DisplayQuestArrow),
	/*0xBB*/ SMSG("Account ID ?", 0x09),
	/*0xBC*/ RMSGH("Season", 0x03, Season),
	/*0xBD*/ BMSGH("Client Version", SIZE_VARIABLE, ClientVersion),
	/*0xBE*/ BMSGH("Assist Version", SIZE_VARIABLE, AssistVersion),
	/*0xBF*/ BMSGH("Extended Command", SIZE_VARIABLE, ExtendedCommand),
	/*0xC0*/ RMSGH("Graphical Effect", 0x24, GraphicEffect),
	/*0xC1*/ RMSGH("Display cliloc String", SIZE_VARIABLE, DisplayClilocString),
	/*0xC2*/ BMSGH("Unicode prompt", SIZE_VARIABLE, UnicodePrompt),
	/*0xC3*/ UMSG(SIZE_VARIABLE),
	/*0xC4*/ UMSG(0x06),
	/*0xC5*/ UMSG(0xcb),
	/*0xC6*/ UMSG(0x01),
	/*0xC7*/ UMSG(0x31),
	/*0xC8*/ BMSGH("Client View Range", 0x02, ClientViewRange),
	/*0xC9*/ UMSG(0x06),
	/*0xCA*/ UMSG(0x06),
	/*0xCB*/ UMSG(0x07),
	/*0xCC*/ RMSGH("Localized Text Plus String", SIZE_VARIABLE, DisplayClilocString),
	/*0xCD*/ UMSG(0x01),
	/*0xCE*/ UMSG(SIZE_VARIABLE),
	/*0xCF*/ UMSG(0x4e),
	/*0xD0*/ UMSG(SIZE_VARIABLE),
	/*0xD1*/ UMSG(0x02),
	/*0xD2*/ RMSGH("Update Player (New)", 0x19, UpdatePlayer),
	/*0xD3*/ RMSGH("Update Object (New)", SIZE_VARIABLE, UpdateObject),
	/*0xD4*/ BMSGH("Open Book (New)", SIZE_VARIABLE, OpenBookNew),
	/*0xD5*/ UMSG(SIZE_VARIABLE),
	/*0xD6*/ BMSGH("Mega cliloc", SIZE_VARIABLE, MegaCliloc),
	/*0xD7*/ BMSG("+AoS command", SIZE_VARIABLE),
	/*0xD8*/ RMSG("+Custom house", SIZE_VARIABLE),
	/*0xD9*/ SMSG("+Metrics", 0x10c),
	/*0xDA*/ UMSG(SIZE_VARIABLE),
	/*0xDB*/ UMSG(SIZE_VARIABLE),
	/*0xDC*/ RMSG("OPL Info Packet", 9),
	/*0xDD*/ RMSGH("Compressed Gump", SIZE_VARIABLE, OpenCompressedGump),
	/*0xDE*/ UMSG(SIZE_VARIABLE),
	/*0xDF*/ RMSGH("Buff/Debuff", SIZE_VARIABLE, BuffDebuff),
	/*0xE0*/ SMSG("Bug Report KR", SIZE_VARIABLE),
	/*0xE1*/ SMSG("Client Type KR/SA", 0x09),
	/*0xE2*/ RMSG("New Character Animation", 0xa),
	/*0xE3*/ RMSG("KR Encryption Responce", 0x4d),
	/*0xE4*/ UMSG(SIZE_VARIABLE),
	/*0xE5*/ UMSG(SIZE_VARIABLE),
	/*0xE6*/ UMSG(SIZE_VARIABLE),
	/*0xE7*/ UMSG(SIZE_VARIABLE),
	/*0xE8*/ UMSG(SIZE_VARIABLE),
	/*0xE9*/ UMSG(SIZE_VARIABLE),
	/*0xEA*/ UMSG(SIZE_VARIABLE),
	/*0xEB*/ UMSG(SIZE_VARIABLE),
	/*0xEC*/ SMSG("Equip Macro", SIZE_VARIABLE),
	/*0xED*/ SMSG("Unequip item macro", SIZE_VARIABLE),
	/*0xEE*/ UMSG(SIZE_VARIABLE),
	/*0xEF*/ SMSG("KR/2D Client Login/Seed", 0x15),
	/*0xF0*/ BMSGH("Krrios client special", SIZE_VARIABLE, KrriosClientSpecial),
	/*0xF1*/ BMSG("Freeshard List", SIZE_VARIABLE),
	/*0xF2*/ UMSG(SIZE_VARIABLE),
	/*0xF3*/ RMSGH("Update Item (SA)", 0x18, UpdateItemSA),
	/*0xF4*/ UMSG(SIZE_VARIABLE),
	/*0xF5*/ RMSGH("Display New Map", 0x15, DisplayMap),
	/*0xF6*/ UMSG(SIZE_VARIABLE),
	/*0xF7*/ UMSG(SIZE_VARIABLE),
	/*0xF8*/ SMSG("Character Creation (7.0.16.0)", 0x6a),
	/*0xF9*/ UMSG(SIZE_VARIABLE),
	/*0xFA*/ UMSG(SIZE_VARIABLE),
	/*0xFB*/ UMSG(SIZE_VARIABLE),
	/*0xFC*/ UMSG(SIZE_VARIABLE),
	/*0xFE*/ UMSG(SIZE_VARIABLE),
	/*0xFD*/ RMSG("Razor Handshake", 0x8),
	/*0xFF*/ UMSG(SIZE_VARIABLE)
};
//---------------------------------------------------------------------------
TPacketManager::TPacketManager()
: m_ClientVersion(CV_OLD)
{
}
//---------------------------------------------------------------------------
#define CV_PRINT 0

#if CV_PRINT!=0
#define CVPRINT(s) TPRINT(s)
#else //CV_PRINT==0
#define CVPRINT(s)
#endif //CV_PRINT!=0
//---------------------------------------------------------------------------
/*!
Установить версию протокола клиента и подменить размерности соответствующих пакетов
@param [__in] val Новая версия протокола клиента
@return
*/
void TPacketManager::SetClientVersion(__in CLIENT_VERSION cver)
{
	m_ClientVersion = cver;
	
	if (m_ClientVersion >= CV_500A)
	{
		CVPRINT("Set new length for packet 0x0B (>= 5.0.0a)\n");
		m_MessageTypes[0x0B].Size = 0x07;
		CVPRINT("Set new length for packet 0x16 (>= 5.0.0a)\n");
		m_MessageTypes[0x16].Size = SIZE_VARIABLE;
		CVPRINT("Set new length for packet 0x31 (>= 5.0.0a)\n");
		m_MessageTypes[0x31].Size = SIZE_VARIABLE;
	}
	else
	{
		CVPRINT("Set standart length for packet 0x0B (< 5.0.0a)\n");
		m_MessageTypes[0x0B].Size = 0x10A;
		CVPRINT("Set standart length for packet 0x16 (< 5.0.0a)\n");
		m_MessageTypes[0x16].Size = 0x01;
		CVPRINT("Set standart length for packet 0x31 (< 5.0.0a)\n");
		m_MessageTypes[0x31].Size = 0x01;
	}

	if (m_ClientVersion >= CV_5090)
	{
		CVPRINT("Set new length for packet 0xE1 (>= 5.0.9.0)\n");
		m_MessageTypes[0xE1].Size = SIZE_VARIABLE;
	}
	else
	{
		CVPRINT("Set standart length for packet 0xE1 (<= 5.0.9.0)\n");
		m_MessageTypes[0xE1].Size = 0x09;
	}

	if (m_ClientVersion >= CV_6013)
	{
		CVPRINT("Set new length for packet 0xE3 (>= 6.0.1.3)\n");
		m_MessageTypes[0xE3].Size = SIZE_VARIABLE;
		CVPRINT("Set new length for packet 0xE6 (>= 6.0.1.3)\n");
		m_MessageTypes[0xE6].Size = 0x05;
		CVPRINT("Set new length for packet 0xE7 (>= 6.0.1.3)\n");
		m_MessageTypes[0xE7].Size = 0x0C;
		CVPRINT("Set new length for packet 0xE8 (>= 6.0.1.3)\n");
		m_MessageTypes[0xE8].Size = 0x0D;
		CVPRINT("Set new length for packet 0xE9 (>= 6.0.1.3)\n");
		m_MessageTypes[0xE9].Size = 0x4B;
		CVPRINT("Set new length for packet 0xEA (>= 6.0.1.3)\n");
		m_MessageTypes[0xEA].Size = 0x03;
	}
	else
	{
		CVPRINT("Set standart length for packet 0xE3 (<= 6.0.1.3)\n");
		m_MessageTypes[0xE3].Size = 0x4D;
		CVPRINT("Set standart length for packet 0xE6 (<= 6.0.1.3)\n");
		m_MessageTypes[0xE6].Size = SIZE_VARIABLE;
		CVPRINT("Set standart length for packet 0xE7 (<= 6.0.1.3)\n");
		m_MessageTypes[0xE7].Size = SIZE_VARIABLE;
		CVPRINT("Set standart length for packet 0xE8 (<= 6.0.1.3)\n");
		m_MessageTypes[0xE8].Size = SIZE_VARIABLE;
		CVPRINT("Set standart length for packet 0xE9 (<= 6.0.1.3)\n");
		m_MessageTypes[0xE9].Size = SIZE_VARIABLE;
		CVPRINT("Set standart length for packet 0xEA (<= 6.0.1.3)\n");
		m_MessageTypes[0xEA].Size = SIZE_VARIABLE;
	}

	if (m_ClientVersion >= CV_6017)
	{
		CVPRINT("Set new length for packet 0x08 (>= 6.0.1.7)\n");
		m_MessageTypes[0x08].Size = 0x0F;
		CVPRINT("Set new length for packet 0x25 (>= 6.0.1.7)\n");
		m_MessageTypes[0x25].Size = 0x15;
	}
	else
	{
		CVPRINT("Set standart length for packet 0x08 (<= 6.0.1.7)\n");
		m_MessageTypes[0x08].Size = 0x0E;
		CVPRINT("Set standart length for packet 0x25 (<= 6.0.1.7)\n");
		m_MessageTypes[0x25].Size = 0x14;
	}

	if (m_ClientVersion == CV_6060)
	{
		CVPRINT("Set new length for packet 0xEE (>= 6.0.6.0)\n");
		m_MessageTypes[0xEE].Size = 0x2000;
		CVPRINT("Set new length for packet 0xEF (>= 6.0.6.0)\n");
		m_MessageTypes[0xEF].Size = 0x2000;
		CVPRINT("Set new length for packet 0xF1 (>= 6.0.6.0)\n");
		m_MessageTypes[0xF1].Size = 0x09;
	}
	else
	{
		CVPRINT("Set standart length for packet 0xEE (<= 6.0.6.0)\n");
		m_MessageTypes[0xEE].Size = SIZE_VARIABLE;
		CVPRINT("Set standart length for packet 0xEF (<= 6.0.6.0)\n");
		m_MessageTypes[0xEF].Size = 0x15;
		CVPRINT("Set standart length for packet 0xF1 (<= 6.0.6.0)\n");
		m_MessageTypes[0xF1].Size = SIZE_VARIABLE;
	}

	if (m_ClientVersion >= CV_60142)
	{
		CVPRINT("Set new length for packet 0xB9 (>= 6.0.14.2)\n");
		m_MessageTypes[0xB9].Size = 0x05;
	}
	else
	{
		CVPRINT("Set standart length for packet 0xB9 (<= 6.0.14.2)\n");
		m_MessageTypes[0xB9].Size = 0x03;
	}

	if (m_ClientVersion >= CV_7000)
	{
		CVPRINT("Set new length for packet 0xEE (>= 7.0.0.0)\n");
		m_MessageTypes[0xEE].Size = 0x2000;
		CVPRINT("Set new length for packet 0xEF (>= 7.0.0.0)\n");
		m_MessageTypes[0xEF].Size = 0x2000;
		/*CVPRINT("Set new length for packet 0xF0 (>= 7.0.0.0)\n");
		m_MessageTypes[0xF0].size = 0x2000;
		CVPRINT("Set new length for packet 0xF1 (>= 7.0.0.0)\n");
		m_MessageTypes[0xF1].size = 0x2000;
		CVPRINT("Set new length for packet 0xF2 (>= 7.0.0.0)\n");
		m_MessageTypes[0xF2].size = 0x2000;*/
	}
	else
	{
		CVPRINT("Set standart length for packet 0xEE (<= 7.0.0.0)\n");
		m_MessageTypes[0xEE].Size = SIZE_VARIABLE;
		CVPRINT("Set standart length for packet 0xEF (<= 7.0.0.0)\n");
		m_MessageTypes[0xEF].Size = 0x15;
		/*CVPRINT("Set standart length for packet 0xF0 (<= 7.0.0.0)\n");
		m_MessageTypes[0xF0].size = SIZE_VARIABLE;
		CVPRINT("Set standart length for packet 0xF1 (<= 7.0.0.0)\n");
		m_MessageTypes[0xF1].size = SIZE_VARIABLE;
		CVPRINT("Set standart length for packet 0xF2 (<= 7.0.0.0)\n");
		m_MessageTypes[0xF2].size = SIZE_VARIABLE;*/
	}

	if (m_ClientVersion >= CV_7090)
	{
		CVPRINT("Set new length for packet 0x24 (>= 7.0.9.0)\n");
		m_MessageTypes[0x24].Size = 0x09;
		CVPRINT("Set new length for packet 0x99 (>= 7.0.9.0)\n");
		m_MessageTypes[0x99].Size = 0x1E;
		CVPRINT("Set new length for packet 0xBA (>= 7.0.9.0)\n");
		m_MessageTypes[0xBA].Size = 0x0A;
		CVPRINT("Set new length for packet 0xF3 (>= 7.0.9.0)\n");
		m_MessageTypes[0xF3].Size = 0x1A;

		//В клиенте 7.0.8.2 уже изменено
		CVPRINT("Set new length for packet 0xF1 (>= 7.0.9.0)\n");
		m_MessageTypes[0xF1].Size = 0x09;
		CVPRINT("Set new length for packet 0xF2 (>= 7.0.9.0)\n");
		m_MessageTypes[0xF2].Size = 0x19;
	}
	else
	{
		CVPRINT("Set standart length for packet 0x24 (<= 7.0.9.0)\n");
		m_MessageTypes[0x24].Size = 0x07;
		CVPRINT("Set standart length for packet 0x99 (<= 7.0.9.0)\n");
		m_MessageTypes[0x99].Size = 0x1A;
		CVPRINT("Set standart length for packet 0xBA (<= 7.0.9.0)\n");
		m_MessageTypes[0xBA].Size = 0x06;
		CVPRINT("Set standart length for packet 0xF3 (<= 7.0.9.0)\n");
		m_MessageTypes[0xF3].Size = 0x18;

		//В клиенте 7.0.8.2 уже изменено
		CVPRINT("Set standart length for packet 0xF1 (<= 7.0.9.0)\n");
		m_MessageTypes[0xF1].Size = SIZE_VARIABLE;
		CVPRINT("Set standart length for packet 0xF2 (<= 7.0.9.0)\n");
		m_MessageTypes[0xF2].Size = SIZE_VARIABLE;
	}

	if (m_ClientVersion >= CV_70180)
	{
		CVPRINT("Set new length for packet 0x00 (>= 7.0.18.0)\n");
		m_MessageTypes[0x00].Size = 0x6A;
	}
	else
	{
		CVPRINT("Set standart length for packet 0x24 (<= 7.0.18.0)\n");
		m_MessageTypes[0x00].Size = 0x68;
	}
}
//---------------------------------------------------------------------------
/*!
Прочитать байт (1 байт)
@return Данные
*/
BYTE TPacketManager::ReadByte()
{
	BYTE result = *Ptr;

	Ptr++;

	return result;
}
//---------------------------------------------------------------------------
/*!
Прочитать слово (2 байта)
@return Данные
*/
WORD TPacketManager::ReadWord()
{
	WORD result = unpack16(Ptr);

	Ptr += 2;

	return result;
}
//---------------------------------------------------------------------------
/*!
Прочитать двойное слово (4 байта)
@return Данные
*/
DWORD TPacketManager::ReadDWord()
{
	DWORD result = unpack32(Ptr);

	Ptr += 4;

	return result;
}
//---------------------------------------------------------------------------
/*!
Прочитать символ (1 байт)
@return Данные
*/
char TPacketManager::ReadChar()
{
	char result = (char)*Ptr;

	Ptr++;

	return result;
}
//---------------------------------------------------------------------------
/*!
Прочитать короткое значение (2 байта)
@return Данные
*/
short TPacketManager::ReadShort()
{
	short result = (short)unpack16(Ptr);

	Ptr += 2;

	return result;
}
//---------------------------------------------------------------------------
/*!
Прочитать целое значение (4 байта)
@return Данные
*/
int TPacketManager::ReadInt()
{
	int result = (int)unpack32(Ptr);

	Ptr += 4;

	return result;
}
//---------------------------------------------------------------------------
/*!
Прочитать ASCII строку
@param [__in] size Размер строки, если указан 0 - читает до нуля
@return Данные
*/
string TPacketManager::ReadString( __in int size)
{
	if (!size)
	{
		PBYTE buf = Ptr;

		while (*buf)
			buf++;

		size = (buf - Ptr) + 1;
	}

	char *str = new char[size + 1];
	memcpy(&str[0], &Ptr[0], size);
	str[size] = 0;

	Ptr += size;

	string result(str);
	delete str;

	return result;
}
//---------------------------------------------------------------------------
/*!
Прочитать Unicode строку (big-endian)
@param [__in] size Размер строки, если указан 0 - читает до нуля
@return Данные
*/
wstring TPacketManager::ReadUnicodeString( __in int size)
{
	int count = size;

	if (!count)
		count = 1024;

	wchar_t *str = new wchar_t[count + 1];
	wchar_t *pStr = str;

	IFOR(i, 0, count)
	{
		wchar_t ch = (Ptr[0] << 8) | Ptr[1];

		*pStr = ch;

		Ptr += 2;
			
		if (!size && *pStr == 0)
			break;

		pStr++;
	}

	str[count] = 0;

	wstring result(str);
	delete str;

	return result;
}
//---------------------------------------------------------------------------
/*!
Прочитать Unicode строку (little-endian)
@param [__in] size Размер строки, если указан 0 - читает до нуля
@return Данные
*/
wstring TPacketManager::ReadUnicodeStringLE(__in int size)
{
	int count = size;

	if (!count)
		count = 1024;

	wchar_t *str = new wchar_t[count + 1];
	wchar_t *pStr = str;

	IFOR(i, 0, count)
	{
		wchar_t ch = *(PWORD)Ptr;
		
		if (ch < 0x0400)
		{
			if (ch != 0xFF)
				ch = (char)(ch % 0xFF);
			else
				ch = (char)ch;
		}

		*pStr = ch;

		Ptr += 2;

		if (!size && *pStr == 0)
			break;

		pStr++;
	}

	str[count] = 0;

	wstring result(str);
	delete str;

	return result;
}
//---------------------------------------------------------------------------
/*!
Обработчик пришедшего сообщения от плагина
@param [__in] buf Буфер
@param [__in] size Размер сообщения
@return
*/
void TPacketManager::PluginReceiveHandler(__in PBYTE buf, __in const int &size)
{
	DWORD ticks = g_Ticks;
	g_TotalRecvSize += size;

	TMessageType &type = m_MessageTypes[*buf];

	TPRINT("--- ^(%d) r(+%d => %d) Plugin:: %s\n", ticks - g_LastPacketTime, size, g_TotalRecvSize, type.Name);
	TDUMP(buf, size);

	g_LastPacketTime = ticks;

	if (type.Direction != DIR_RECV && type.Direction != DIR_BOTH)
		WPRINT("message direction invalid: 0x%02X\n", *buf);
	else if (type.Handler != 0)
	{
		Ptr = buf + 1;

		if (!type.Size)
			Ptr += 2;

		(this->*(type.Handler))(buf, size);
	}
}
//---------------------------------------------------------------------------
/*!
Обработчик пришедшего сообщения
@param [__in] buf Буфер
@param [__in] size Размер сообщения
@return 
*/
void TPacketManager::ReceiveHandler(__in PBYTE buf, __in const int &size)
{
	DWORD ticks = g_Ticks;
	g_TotalRecvSize += size;
	
	TMessageType &type = m_MessageTypes[*buf];

	TPRINT("--- ^(%d) r(+%d => %d) Server:: %s\n", ticks - g_LastPacketTime, size, g_TotalRecvSize, type.Name);
	TDUMP(buf, size);
	
	g_LastPacketTime = ticks;

	if (type.Direction != DIR_RECV && type.Direction != DIR_BOTH)
		WPRINT("message direction invalid: 0x%02X\n", *buf);
	else if (type.Handler != 0)
	{
		if (PluginManager->PacketRecv(buf, size))
		{
			Ptr = buf + 1;

			if (!type.Size)
				Ptr += 2;

			(this->*(type.Handler))(buf, size);
		}
	}
}
//---------------------------------------------------------------------------
void TPacketManager::SendMegaClilocRequests()
{
	if (m_MegaClilocRequests.size())
	{
		TPacketMegaClilocRequest packet(m_MegaClilocRequests);
		packet.Send();
		packet.Free();

		m_MegaClilocRequests.clear();
	}
}
//---------------------------------------------------------------------------
#define PACKET_HANDLER(name) void TPacketManager::Handle ##name (PBYTE buf, const int &size)
//---------------------------------------------------------------------------
PACKET_HANDLER(ClientTalk)
{
	if (!g_AbyssPacket03First)
	{
		if (buf[4] == 0x78)
		{
			//Ptr = buf + 7;
			//HandleUpdateObject(buf + 4, unpack16(buf + 5));
			ReceiveHandler(buf + 4, unpack16(buf + 5));
		}
		else if (buf[4] == 0x3C)
		{
			//Ptr = buf + 7;
			//HandleUpdateContainedItems(buf + 4, unpack16(buf + 5));
			ReceiveHandler(buf + 4, unpack16(buf + 5));
		}
		else if (buf[4] == 0x25)
		{
			//Ptr = buf + 5;
			//HandleUpdateContainedItem(buf + 4, 0x14);
			ReceiveHandler(buf + 4, 0x14);
		}
		else if (buf[4] == 0x2E)
		{
			//Ptr = buf + 5;
			//HandleEquipItem(buf + 4, 0x0F);
			ReceiveHandler(buf + 4, 0x0F);
		}
	}

	g_AbyssPacket03First = false;
}
//---------------------------------------------------------------------------
PACKET_HANDLER(LoginError)
{
	if (g_GameState == GS_MAIN_CONNECT || g_GameState == GS_SERVER_CONNECT)
	{
		ConnectionScreen->ConnectionFailed = true;
		ConnectionScreen->ErrorCode = ReadByte();
		ConnectionManager.Disconnect();
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(ServerList)
{
	// We need to remember the server list so that we know the name of
	// the server when the relay message comes back from the server.

	ServerList.Clear();
	Move(1);

	int numServers = ReadShort();
	ServerList.Init(numServers);

	if (numServers == 0)
		WPRINT("empty server list\n");

	// Calculate expected message size
	int exSize = 6 + numServers * 40;
	if (size != exSize)
		WPRINT("server list message size should be %d\n", exSize);

	IFOR(i, 0, numServers)
	{
		WORD id = ReadWord();
		ServerList.SetServer(i, id, (char*)Ptr);
		Move(40);
	}

	if (numServers && MainScreen->AutoLogin)
		Orion->ServerSelection(0);
	else
		Orion->InitScreen(GS_SERVER);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(RelayServer)
{
	memset(&g_SelectedCharName[0], 0, sizeof(g_SelectedCharName));
	in_addr addr;
	PDWORD paddr = (PDWORD)Ptr;
	Move(4);
	addr.S_un.S_addr = *paddr;
	char relayIP[30] = {0};
	strncpy(relayIP, inet_ntoa(addr), 29);
	int relayPort = ReadShort();
	Orion->RelayServer(relayIP, relayPort, Ptr);
	g_PacketLoginComplete = false;
	g_CurrentMap = 0;
}
//---------------------------------------------------------------------------
PACKET_HANDLER(CharacterList)
{
	Orion->InitScreen(GS_CHARACTER);

	int numSlots = ReadChar();
	TPRINT("/======Chars===\n");

	CharacterList.Clear();
	CharacterList.Count = numSlots;

	int AutoPos = -1;
	bool autoLogin = MainScreen->AutoLogin;

	if (numSlots == 0)
		WPRINT("no slots in character list\n");
	else
	{
		IFOR(i, 0, numSlots)
		{
			CharacterList.SetName(i, (char*)Ptr);

			if (autoLogin && AutoPos == -1 && Orion->AutoLoginNameExists((char*)Ptr))
				AutoPos = i;

			TPRINT("%d: %s\n", i, (char*)Ptr);

			Move(60);
		}
	}

	BYTE locCount = ReadByte();

	CityList->Clear();

	if (m_ClientVersion >= CV_70130)
	{
		IFOR(i, 0, locCount)
		{
			TCityItemNew *city = new TCityItemNew();

			city->LocationIndex = ReadByte();

			city->SetName(ReadString(32));
			city->SetArea(ReadString(32));

			city->X = ReadDWord();
			city->Y = ReadDWord();
			city->Z = ReadDWord();
			city->MapIndex = ReadDWord();

			DWORD clilocID = ReadDWord();

			city->CreateTextTexture(clilocID);

			Move(4);

			CityList->Add(city);
		}
	}
	else
	{
		IFOR(i, 0, locCount)
		{
			TCityItem *city = new TCityItem();

			city->LocationIndex = ReadByte();

			city->SetName(ReadString(31));
			city->SetArea(ReadString(31));

			city->InitCity();

			CityList->Add(city);
		}
	}

	g_ClientFlag = ReadDWord();

	CharacterList.OnePerson = (bool)(g_ClientFlag & LFF_TD);
	g_SendLogoutNotification = (bool)(g_ClientFlag & LFF_RE);
	g_NPCPopupEnabled = (bool)(g_ClientFlag & LFF_LBR);
	g_ChatEnabled = (bool)(g_ClientFlag & LFF_T2A);

	if (autoLogin && numSlots)
	{
		if (AutoPos == -1)
			AutoPos = 0;

		CharacterList.Selected = AutoPos ;

		if (CharacterList.GetName(AutoPos).length())
			Orion->CharacterSelection(AutoPos);
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(ResendCharacterList)
{
	Orion->InitScreen(GS_CHARACTER);

	int numSlots = ReadByte();
	TPRINT("/======Resend chars===\n");

	CharacterList.Clear();
	CharacterList.Count = numSlots;

	int AutoPos = -1;
	bool autoLogin = MainScreen->AutoLogin;

	if (numSlots == 0)
		WPRINT("no slots in character list\n");
	else
	{
		IFOR(i, 0, numSlots)
		{
			CharacterList.SetName(i, (char*)Ptr);

			if (autoLogin && AutoPos == -1 && Orion->AutoLoginNameExists((char*)Ptr))
				AutoPos = i;

			TPRINT("%d: %s\n", i, (char*)Ptr);

			Move(60);
		}
	}

	if (autoLogin && numSlots)
	{
		if (AutoPos == -1)
			AutoPos = 0;

		CharacterList.Selected = AutoPos ;

		if (CharacterList.GetName(AutoPos).length())
			Orion->CharacterSelection(AutoPos);
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(LoginComplete)
{
	g_PacketLoginComplete = true;

	Orion->LoginComplete();
}
//---------------------------------------------------------------------------
PACKET_HANDLER(SetTime)
{
	g_ServerTimeHour = ReadByte();
	g_ServerTimeMinute = ReadByte();
	g_ServerTimeSecond = ReadByte();
}
//---------------------------------------------------------------------------
PACKET_HANDLER(EnterWorld)
{
	DWORD serial = ReadDWord();

	if (World != NULL)
		EPRINT("duplicate enter world message\n");
	else
	{
		if (World != NULL)
			delete World;
		World = new TGameWorld(serial);
		
		if (Walker != NULL)
			delete Walker;
		Walker = new TWalker();
		g_PendingDelayTime = 0;

		if (ObjectInHand != NULL)
			delete ObjectInHand;
		ObjectInHand = NULL;

		g_WalkRequestCount = 0;
		g_PingCount = 0;
		g_PingSequence = 0;
		g_ClickObject.Init(COT_NONE);
		Weather.Reset();
		g_SkillsTotal = 0.0f;
		g_ConsolePrompt = PT_NONE;
		MacroPointer = NULL;
		g_Season = 0;
	}

	Move(4);

	if (strlen(g_SelectedCharName))
		g_Player->SetName(g_SelectedCharName);

	g_Player->Graphic = ReadWord();
	g_Player->OnGraphicChange();

	g_Player->X = ReadWord();
	g_Player->Y = ReadWord();
	Move(1);
	g_Player->Z = ReadByte();
	BYTE dir = ReadByte();
	g_Player->Direction = dir;
	g_Player->Flags = buf[28];

	MapManager->Init();
	
	MapManager->AddRender(g_Player);

	Walker->SetSequence(0, dir);
	g_Player->OffsetX = 0;
	g_Player->OffsetY = 0;
	g_Player->OffsetZ = 0;

	if (m_ClientVersion >= CV_500A && !g_Player->GetClilocMessage().length())
		m_MegaClilocRequests.push_back(g_Player->Serial);

	TPRINT("Player 0x%08lX entered the world.\n", serial);

	Orion->LoadStartupConfig();

	g_LastSpellIndex = 0;
	g_LastSkillIndex = 1;

	Orion->Click(g_PlayerSerial);
	
	Orion->StatusReq(g_PlayerSerial);
	
	if (m_ClientVersion >= CV_200)
	{
		TPacketGameWindowSize packet;
		packet.Send();
	}
	
	/*BYTE wbuf[4] = {0x65, 0x01, 0x46, 0};
	Ptr = wbuf + 1;
	HandleSetWeather(wbuf, 4);*/
}
//---------------------------------------------------------------------------
PACKET_HANDLER(UpdateHitpoints)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();

	TGameCharacter *obj = World->FindWorldCharacter(serial);
	if (obj == NULL)
		return;
	
	obj->MaxHits = ReadShort();
	obj->Hits = ReadShort();
	
	GumpManager->UpdateGump(serial, 0, GT_STATUSBAR);
	GumpManager->UpdateGump(serial, 0, GT_TARGET_SYSTEM);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(UpdateMana)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();

	TGameCharacter *obj = World->FindWorldCharacter(serial);
	if (obj == NULL)
		return;

	obj->MaxMana = ReadShort();
	obj->Mana = ReadShort();
	
	GumpManager->UpdateGump(serial, 0, GT_STATUSBAR);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(UpdateStamina)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();

	TGameCharacter *obj = World->FindWorldCharacter(serial);
	if (obj == NULL)
		return;

	obj->MaxStam = ReadShort();
	obj->Stam = ReadShort();
	
	GumpManager->UpdateGump(serial, 0, GT_STATUSBAR);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(UpdatePlayer)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();

	if (serial != g_PlayerSerial)
	{
		WPRINT("Current player changed from 0x%08lX to 0x%08lX\n", g_PlayerSerial, serial);
		World->SetPlayer(serial);
	}

	WORD oldGraphic = g_Player->Graphic;
	g_Player->Graphic = ReadWord();
	g_Player->OnGraphicChange();

	if (oldGraphic && oldGraphic != g_Player->Graphic)
	{
		if (g_Player->Dead())
		{
			Weather.Reset();
			Target.Reset();
			if (ConfigManager.Music)
			{
				SoundManager.StopMusic();
				SoundManager.StopWarMusic();
				Orion->PlayMusic(42, true);
			}
			g_DeathScreenTimer = g_Ticks + DEATH_SCREEN_DELAY;
		}
	}
	
	Move(1);
	g_Player->Color = ReadWord();
	g_Player->Flags = ReadByte();
	g_Player->X = ReadWord();
	g_Player->Y = ReadWord();
	Move(2);

	g_Player->m_WalkStack.Clear();

	BYTE dir = ReadByte();
	Walker->SetSequence(0, dir);
	g_WalkRequestCount = 0;
	g_Player->OffsetX = 0;
	g_Player->OffsetY = 0;
	g_Player->OffsetZ = 0;

	if (g_Player->Direction != dir)
	{
		TGameItem *bank = g_Player->FindLayer(OL_BANK);

		if (bank != NULL)
		{
			TGump *bankContainer = GumpManager->GetGump(bank->Serial, 0, GT_CONTAINER);

			if (bankContainer != NULL)
				GumpManager->RemoveGump(bankContainer);
		}
	}

	g_Player->Direction = dir;
	g_Player->Z = ReadByte();
	
	World->MoveToTop(g_Player);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(CharacterStatus)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();
	TGameCharacter *obj = World->FindWorldCharacter(serial);
	if (obj == NULL)
		return;

	string name = ReadString(30);
	obj->SetName(name);

	obj->Hits = ReadShort();
	obj->MaxHits = ReadShort();

	obj->CanChangeName = (ReadByte() != 0);

	BYTE flag = ReadByte();

	if (flag > 0)
	{
		obj->Sex = ReadByte(); //buf[43];

		if (serial == g_PlayerSerial)
		{
			short newStr = ReadShort();
			short newDex = ReadShort();
			short newInt = ReadShort();

			if (ConfigManager.StatReport && g_Player->Str)
			{
				short currentStr = g_Player->Str;
				short currentDex = g_Player->Dex;
				short currentInt = g_Player->Int;
			
				short deltaStr = newStr - currentStr;
				short deltaDex = newDex - currentDex;
				short deltaInt = newInt - currentInt;
			
				char str[64] = {0};
				if (deltaStr)
				{
					sprintf(str, "Your strength has changed by %d.  It is now %d.", deltaStr, newStr);
					Orion->CreateTextMessage(TT_SYSTEM, 0, 3, 0x0170, str);
				}
			
				if (deltaDex)
				{
					sprintf(str, "Your dexterity has changed by %d.  It is now %d.", deltaDex, newDex);
					Orion->CreateTextMessage(TT_SYSTEM, 0, 3, 0x0170, str);
				}
			
				if (deltaInt)
				{
					sprintf(str, "Your intelligence has changed by %d.  It is now %d.", deltaInt, newInt);
					Orion->CreateTextMessage(TT_SYSTEM, 0, 3, 0x0170, str);
				}
			}

			g_Player->Str = newStr;
			g_Player->Dex = newDex;
			g_Player->Int = newInt;

			g_Player->Stam = ReadShort();
			g_Player->MaxStam = ReadShort();
			g_Player->Mana = ReadShort();
			g_Player->MaxMana = ReadShort();
			g_Player->Gold = ReadDWord();
			g_Player->Armor = ReadShort();
			g_Player->Weight = ReadShort(); //+64

			if (flag >= 5)
			{
				g_Player->MaxWeight = ReadShort(); //unpack16(buf + 66);
				g_Player->Race = (CHARACTER_RACE_TYPE)ReadByte();
			}
			else
				g_Player->MaxWeight = (g_Player->Str * 4) + 25;

			if (flag >= 3)
			{
				g_Player->StatsCap = ReadWord();
				g_Player->Followers = ReadByte();
				g_Player->MaxFollowers = ReadByte();
			}

			if (flag >= 4)
			{
				g_Player->FireResistance = ReadShort();
				g_Player->ColdResistance = ReadShort();
				g_Player->PoisonResistance = ReadShort();
				g_Player->EnergyResistance = ReadShort();
				g_Player->Luck = ReadShort();
				g_Player->MinDamage = ReadShort();
				g_Player->MaxDamage = ReadShort();
				g_Player->TithingPoints = ReadDWord();
			}

			if (!ConnectionScreen->Completed && g_PacketLoginComplete)
				Orion->LoginComplete();
		}
	}
	
	TGump *gump = GumpManager->UpdateGump(serial, 0, GT_STATUSBAR);
	if (gump != NULL)
	{
		TGumpStatusbar *sbg = (TGumpStatusbar*)gump;

		if (sbg->TextEntry != NULL && sbg->TextEntry != EntryPointer)
			sbg->TextEntry->SetText(name);
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(UpdateItem)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();
	WORD graphic = ReadWord();

#if UO_ABYSS_SHARD == 1
	if ((graphic & 0x7FFF) == 0x0E5C)
		return;
#endif

	WORD count = 0;

	if (serial & 0x80000000)
	{
		serial &= 0x7FFFFFFF;
		count = ReadWord();
	}

	TGameItem *obj = World->GetWorldItem(serial);
	if (obj == NULL)
	{
		TPRINT("no memory??");
		return;
	}
	
	if (ObjectInHand != NULL && ObjectInHand->Serial == obj->Serial && ObjectInHand->Dropped)
	{
		delete ObjectInHand;
		ObjectInHand = NULL;
	}

	if (obj->Dragged)
		GumpManager->CloseGump(serial, 0, GT_DRAG);

	obj->Dragged = false;
	obj->MapIndex = g_CurrentMap;

	if (graphic & 0x8000)
	{
		graphic &= 0x7FFF;
		graphic += ReadByte();
	}

	if (!obj->Graphic)
		TPRINT("created ");
	else
		TPRINT("updated ");

	obj->Graphic = graphic;
	obj->Count = count;
	WORD X = ReadWord();
	WORD Y = ReadWord();
	BYTE dir = 0;

	if (X & 0x8000)
	{
		X &= 0x7FFF;

		//obj->Direction = *Ptr; //Направление предмета?
		dir = ReadByte();
	}
	
	obj->X = X;
	obj->Z = ReadByte();

	if (Y & 0x8000)
	{
		Y &= 0x7FFF;
		
		obj->Color = ReadWord();
	}
	else
		obj->Color = 0;

	if (Y & 0x4000)
	{
		Y &= 0x3FFF;
		obj->Flags = ReadByte();
	}
	
	obj->Y = Y;

	obj->OnGraphicChange(dir);

	World->MoveToTop(obj);

	if (m_ClientVersion >= CV_500A && !obj->GetClilocMessage().length())
		m_MegaClilocRequests.push_back(obj->Serial);

	TPRINT("0x%08lX:0x%04X*%d %d:%d:%d\n", serial, graphic, obj->Count, obj->X, obj->Y, obj->Z);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(UpdateItemSA)
{
	if (World == NULL)
		return;

	Move(2);
	char type = ReadByte(); //buf[3];
	DWORD serial = ReadDWord();
	WORD graphic = ReadWord();
	BYTE dir = ReadByte();
	WORD count = ReadWord();
	Move(2);
	WORD x = ReadWord();
	WORD y = ReadWord();
	char z = ReadByte();
	Move(1);
	WORD color = ReadWord();
	char flags = ReadByte();

	TGameItem *obj = World->GetWorldItem(serial);
	if (obj == NULL)
	{
		TPRINT("no memory??");
		return;
	}
	
	if (ObjectInHand != NULL && ObjectInHand->Serial == obj->Serial)
	{
		delete ObjectInHand;
		ObjectInHand = NULL;
	}

	if (obj->Dragged)
		GumpManager->CloseGump(serial, 0, GT_DRAG);

	obj->Dragged = false;
	obj->MapIndex = g_CurrentMap;

	if (!obj->Graphic)
		TPRINT("created ");
	else
		TPRINT("updated ");

	obj->Graphic = graphic;
	obj->Count = count;

	obj->X = x;
	obj->Y = y;
	obj->Z = z;

	obj->Color = color;

	obj->OnGraphicChange(dir);

	obj->Flags = flags;

	TPRINT("0x%08lX:0x%04X*%d %d:%d:%d\n", serial, obj->Graphic, obj->Count, obj->X, obj->Y, obj->Z);

	if (m_ClientVersion >= CV_500A && !obj->GetClilocMessage().length())
		m_MegaClilocRequests.push_back(obj->Serial);

	World->MoveToTop(obj);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(UpdateObject)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();

	TGameItem *item = NULL;
	TGameCharacter *character = NULL;
	TGameObject *obj = NULL;

	if (serial & 0x80000000)
	{
		item = World->GetWorldItem(serial & 0x7FFFFFFF);
		obj = item;
	}
	else
	{
		character = World->GetWorldCharacter(serial & 0x7FFFFFFF);
		obj = character;
	}

	if (obj == NULL)
	{
		TPRINT("no memory??");
		return;
	}

	if (ObjectInHand != NULL && ObjectInHand->Serial == obj->Serial)
	{
		delete ObjectInHand;
		ObjectInHand = NULL;
	}

	obj->MapIndex = g_CurrentMap;

	WORD graphic = ReadWord();

	if (serial & 0x80000000)
	{
		obj->Count = ReadWord();
		obj->NPC = false;
	}
	else
		obj->NPC = true;

	if (graphic & 0x8000)
	{
		graphic &= 0x7FFF;

		graphic += ReadWord();
		//obj->GraphicIncrement = unpack16(ptr);
		//Move(2);
	}

	obj->Graphic = graphic;

	WORD X = ReadWord();

	if (X & 0x8000)
	{
		X &= 0x7FFF;

		Move(1); //direction2 ?????
	}

	WORD newX = X;
	WORD newY = ReadWord();
	char newZ = ReadByte();

	BYTE dir = ReadByte();

	if (character != NULL)
	{
		obj->OnGraphicChange(1000);

		TGump *statusGump = GumpManager->UpdateGump(character->Serial, 0, GT_STATUSBAR);

		if (statusGump != NULL)
			Orion->StatusReq(character->Serial);
	}
	else
		obj->OnGraphicChange(dir);

	obj->Color = ReadWord();

	bool hidden = obj->Hidden();
	obj->Flags = ReadByte();
	bool updateCoords = (hidden == obj->Hidden());

	BYTE noto = ReadByte();

	if (updateCoords)
	{
		if (character != NULL && !character->m_WalkStack.Empty())
		{
			if (newX != obj->X || newX != obj->X)
			{
				obj->X = character->m_WalkStack.m_Items->X;
				obj->Y = character->m_WalkStack.m_Items->Y;
				obj->Z = character->m_WalkStack.m_Items->Z;
				character->m_WalkStack.Clear();
				updateCoords = false;
			}
		}

		if (updateCoords)
		{
			obj->X = newX;
			obj->Y = newY;
			obj->Z = newZ;

			if (character != NULL)
				character->Direction = dir;
		}
	}

	if (character != NULL)
	{
		character->Notoriety = noto;
		TPRINT("0x%08X 0x%04X NPC %d,%d,%d C%04X F%02X D%d N%d\n", serial, obj->Graphic, obj->X, obj->Y, obj->Z, obj->Color, obj->Flags, character->Direction, character->Notoriety);
	}
	else
		TPRINT("0x%08X 0x%04X %d,%d,%d C%04X F%02X\n", serial, obj->Graphic, obj->X, obj->Y, obj->Z, obj->Color, obj->Flags);

	if (m_ClientVersion >= CV_500A && !obj->GetClilocMessage().length())
		m_MegaClilocRequests.push_back(obj->Serial);

	serial = ReadDWord();

	World->MoveToTop(obj);

	PBYTE end = buf + size;

	if (*buf != 0x78)
		end -= 6;

	vector<DWORD> megaClilocRequestList;

	while (serial != 0)
	{
		if (Ptr >= end)
			break;

		TGameItem *obj2 = World->GetWorldItem(serial);

		obj2->MapIndex = g_CurrentMap;

		graphic = ReadWord();

		BYTE layer = ReadByte();

		if (graphic & 0x8000)
		{
			graphic &= 0x7FFF;

			obj2->Color = ReadWord();
		}
		else if (m_ClientVersion >= CV_7090)
			obj2->Color = ReadWord();

		obj2->Graphic = graphic;

		World->PutEquipment(obj2, obj, layer);
		obj2->OnGraphicChange();

		TPRINT("\t0x%08X:%04X [%d] %04X\n", obj2->Serial, obj2->Graphic, layer, obj2->Color);

		if (m_ClientVersion >= CV_500A && !obj2->GetClilocMessage().length())
			megaClilocRequestList.push_back(obj2->Serial);

		World->MoveToTop(obj2);

		serial = ReadDWord();
	}

	if (megaClilocRequestList.size())
	{
		TPacketMegaClilocRequest packet(megaClilocRequestList);
		packet.Send();
		packet.Free();

		megaClilocRequestList.clear();
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(EquipItem)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();
	
	if (ObjectInHand != NULL && ObjectInHand->Serial == serial)
	{
		delete ObjectInHand;
		ObjectInHand = NULL;
	}

	TGameItem *obj = World->GetWorldItem(serial);
	obj->MapIndex = g_CurrentMap;
	obj->Graphic = ReadWord();
	Move(1);
	int layer = ReadByte();
	DWORD cserial = ReadDWord();
	obj->Color = ReadWord();

	World->PutEquipment(obj, cserial, layer);
	obj->OnGraphicChange();

	if (m_ClientVersion >= CV_500A && !obj->GetClilocMessage().length())
		m_MegaClilocRequests.push_back(obj->Serial);

	if (layer < OL_MOUNT)
		GumpManager->UpdateGump(cserial, 0, GT_PAPERDOLL);

	if (layer >= OL_BUY_RESTOCK && layer <= OL_SELL)
		obj->Clear();
}
//---------------------------------------------------------------------------
PACKET_HANDLER(UpdateContainedItem)
{
	if (World == NULL)
		return;

	TGameItem *obj = World->GetWorldItem(ReadDWord());
	if (obj == NULL)
		return;

	if (ObjectInHand != NULL && ObjectInHand->Serial == obj->Serial)
	{
		if (ObjectInHand->Separated)
			ObjectInHand->Separated = false;
		else
		{
			delete ObjectInHand;
			ObjectInHand = NULL;
		}
	}

	if (obj->Dragged)
		GumpManager->CloseGump(obj->Serial, 0, GT_DRAG);

	obj->Dragged = false;
	obj->MapIndex = g_CurrentMap;

	obj->Graphic = ReadWord();
	obj->OnGraphicChange();
	Move(1);
	obj->Count = ReadWord();
	obj->X = ReadWord();
	obj->Y = ReadWord();
	obj->Z = 0;

	if (m_ClientVersion >= CV_6017)
		Move(1);

	DWORD cserial = ReadDWord();
	bool canPut = true;

	if (obj->Layer != OL_NONE)
	{
		TGameObject *container = World->FindWorldObject(cserial);
		if (container != NULL && container->IsCorpse())
		{
			canPut = false;
			World->PutEquipment(obj, container, obj->Layer);
		}
	}

	if (canPut)
	{
		obj->Layer = OL_NONE;
		World->PutContainer(obj, cserial);
	}

	obj->Color = ReadWord();

	if (m_ClientVersion >= CV_500A && !obj->GetClilocMessage().length())
		m_MegaClilocRequests.push_back(obj->Serial);

	if (obj->Graphic == 0x0EB0) //Message board item
	{
		TPacketBulletinBoardRequestMessageSummary bbPacket(cserial, obj->Serial);
		bbPacket.Send();

		GumpManager->UpdateGump(cserial, 0, GT_BULLETIN_BOARD);
	}

	World->MoveToTop(obj);

	TGameItem *container = World->FindWorldItem(cserial);
	if (container != NULL)
	{
		TGump *gump = GumpManager->UpdateGump(cserial, 0, GT_SPELLBOOK);
		if (gump == NULL)
			gump = GumpManager->UpdateGump(cserial, 0, GT_CONTAINER);

		if (gump != NULL)
			container->Opened = true;

		TGameObject *top = container->GetTopObject();

		if (top != NULL)
			GumpManager->UpdateGump(top->Serial, 0, GT_TRADE);
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(UpdateContainedItems)
{
	if (World == NULL)
		return;

	WORD count = ReadWord();
	DWORD cupd = 0;
	TGameItem *contobj = NULL;
	bool isContGameBoard = false;
	bool bbUpdated = false;
	vector<CORPSE_EQUIPMENT_DATA> vced;
	bool containerIsCorpse = false;
	vector<DWORD> megaClilocRequestList;

	IFOR(i, 0, count)
	{
		DWORD serial = ReadDWord();
		WORD graphic = ReadWord();
		Move(1);
		WORD count = ReadWord();
		WORD x = ReadWord();
		WORD y = ReadWord();

		if (m_ClientVersion >= CV_6017)
			Move(1);

		DWORD cserial = ReadDWord();

		if (cupd && cserial != cupd)
		{
			PBYTE oldPtr = Ptr;
			Ptr -= 5;

			cserial = ReadDWord();

			if (cserial != cupd)
				continue;
		}

		WORD color = ReadWord();

		contobj = World->GetWorldItem(cserial);

		contobj->MapIndex = g_CurrentMap;
		WORD contgraphic = contobj->Graphic;

		if (!cupd)
		{
			cupd = cserial;
			TGameObject *objA = World->FindWorldObject(cupd);
			TPRINT("Making %08X empty...\n", cupd);

			if (objA != NULL)
			{
				if (objA->IsCorpse())
				{
					containerIsCorpse = true;

					QFOR(citem, objA->m_Items, TGameItem*)
					{
						int lay = citem->Layer;

						if (lay > 0 && lay < OL_MOUNT)
						{
							CORPSE_EQUIPMENT_DATA ced = {citem->Serial, lay};
							vced.push_back(ced);
						}
					}
				}

				objA->Clear();

				if (contobj->Opened)
				{
					TGump *gameGump = GumpManager->GetGump(contobj->Serial, 0, GT_CONTAINER);
					if (gameGump != NULL)
						isContGameBoard = ((TGumpContainer*)gameGump)->IsGameBoard;
				}
			}
		}

		TGameItem *obj = World->GetWorldItem(serial);

		//if (obj->GetDragged()) UO->CloseGump(obj->GetSerial(), 0, GT_DRAG);
		//obj->SetDragged(false);

		obj->MapIndex = g_CurrentMap;
		obj->Layer = 0;

		if (m_ClientVersion >= CV_500A && !obj->GetClilocMessage().length())
			megaClilocRequestList.push_back(obj->Serial);

		World->PutContainer(obj, cserial);

		obj->Graphic = graphic;
		obj->OnGraphicChange();
		obj->Count = count;
		obj->X = x;
		obj->Y = y;
		obj->Color = color;

		if (obj->Graphic == 0x0EB0) //Message board item
		{
			TPacketBulletinBoardRequestMessageSummary bbPacket(cserial, serial);
			bbPacket.Send();

			if (!bbUpdated)
			{
				bbUpdated = true;

				GumpManager->UpdateGump(cserial, 0, GT_BULLETIN_BOARD);
			}
		}

		TPRINT("\t|0x%08X<0x%08X:%04X*%d (%d,%d) %04X\n", obj->Container, obj->Serial, obj->Graphic, obj->Count, obj->X, obj->Y, obj->Color);
	}

	if (megaClilocRequestList.size())
	{
		TPacketMegaClilocRequest packet(megaClilocRequestList);
		packet.Send();
		packet.Free();

		megaClilocRequestList.clear();
	}

	if (containerIsCorpse)
	{
		IFOR(i, 0, (int)vced.size())
		{
			TGameItem *gi = World->FindWorldItem(vced[i].Serial);

			if (gi != NULL)
				World->PutEquipment(gi, cupd, vced[i].Layer);
		}

		vced.clear();
	}

	if (contobj != NULL)
	{
		TGump *gump = GumpManager->UpdateGump(contobj->Serial, 0, GT_SPELLBOOK);
		if (gump == NULL)
			gump = GumpManager->UpdateGump(contobj->Serial, 0, GT_CONTAINER);

		if (gump != NULL)
			contobj->Opened = true;

		TGameObject *top = contobj->GetTopObject();

		if (top != NULL)
			GumpManager->UpdateGump(top->Serial, 0, GT_TRADE);
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(DenyMoveItem)
{
	if (World == NULL)
		return;

	if (ObjectInHand != NULL)
	{
		TGameItem *obj = World->FindWorldItem(ObjectInHand->Serial);

		if (obj == NULL)
		{
			obj = World->GetWorldItem(ObjectInHand->Serial);

			if (obj != NULL)
			{
				obj->Paste(ObjectInHand);

				if (m_ClientVersion >= CV_500A)
					m_MegaClilocRequests.push_back(obj->Serial);
				World->PutContainer(obj, ObjectInHand->Container);

				World->MoveToTop(obj);
			}
		}

		delete ObjectInHand;
		ObjectInHand = NULL;
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(DeleteObject)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();
	TGameObject *obj = World->FindWorldObject(serial);

	if (ObjectInHand != NULL && ObjectInHand->Serial == serial)
	{
		bool sep = ObjectInHand->Separated;

		if (sep)
			ObjectInHand->Separated = false;
		else if (ObjectInHand->Dropped/*Deleted*/)
		{
			delete ObjectInHand;
			ObjectInHand = NULL;
		}
		
		if (ObjectInHand != NULL)
		{
			if (obj != NULL && sep)
			{
				if (obj->Count != ObjectInHand->Count)
					ObjectInHand->Deleted = true;
				else
					ObjectInHand->Separated = true;
			}
			else
				ObjectInHand->Deleted = true;
		}
	}

	if (obj != NULL && obj->Serial != g_PlayerSerial)
	{
		DWORD cont = obj->Container;

		if (cont != 0xFFFFFFFF)
		{
			TGameObject *top = obj->GetTopObject();

			if (top != NULL)
				GumpManager->UpdateGump(top->Serial, 0, GT_TRADE);

			if (!obj->NPC && ((TGameItem*)obj)->Layer != OL_NONE)
				GumpManager->UpdateGump(cont, 0, GT_PAPERDOLL);

			TGump *gump = GumpManager->UpdateGump(cont, 0, GT_CONTAINER);

			if (obj->Graphic == 0x0EB0)
			{
				GumpManager->CloseGump(serial, cont, GT_BULLETIN_BOARD_ITEM);
				
				gump = GumpManager->UpdateGump(cont, 0, GT_BULLETIN_BOARD);

				if (gump != NULL)
				{
					QFOR(go, gump->m_Items, TGumpObject*)
					{
						if (go->Serial == serial)
						{
							gump->Delete(go);
							break;
						}
					}
				}
			}
		}

		if (obj->NPC && Party.Contains(obj->Serial))
			obj->RemoveRender();
		else
			World->RemoveObject(obj);
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(UpdateCharacter)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();
	TGameCharacter *obj = World->GetWorldCharacter(serial);

	if (obj == NULL)
		return;

	obj->MapIndex = g_CurrentMap;
	obj->Graphic = ReadWord();
	obj->OnGraphicChange();

	if (m_ClientVersion >= CV_500A && !obj->GetClilocMessage().length())
		m_MegaClilocRequests.push_back(obj->Serial);

	WORD x = ReadWord();
	WORD y = ReadWord();
	char z = ReadChar();
	BYTE dir = ReadByte();

	if (!obj->IsTeleportAction(x, y, dir))
	{
		if (serial != g_PlayerSerial)
		{
			TWalkData *wd = new TWalkData();
			wd->X = x;
			wd->Y = y;
			wd->Z = z;
			wd->Direction = dir;

			if (obj->m_WalkStack.Empty())
				obj->LastStepTime = g_Ticks;

			obj->m_WalkStack.Push(wd);
		}
		else
		{
			obj->X = x;
			obj->Y = y;
			obj->Z = z;
			obj->Direction = dir;
		}
	}

	obj->Color = ReadWord();
	obj->Flags = ReadByte();
	
	GumpManager->UpdateGump(serial, 0, GT_STATUSBAR);
	
	World->MoveToTop(obj);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(Warmode)
{
	if (World == NULL)
		return;

	g_Player->Warmode = ReadByte();
	
	GumpManager->UpdateGump(g_PlayerSerial, 0, GT_STATUSBAR);
	GumpManager->UpdateGump(g_PlayerSerial, 0, GT_PAPERDOLL);

	World->MoveToTop(g_Player);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(PauseControl)
{
	/*g_ClientPaused = ReadByte();

	if (!g_ClientPaused)
		UO->ResumeClient();*/
}
//---------------------------------------------------------------------------
PACKET_HANDLER(OpenPaperdoll)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();

	TGameCharacter *obj = World->FindWorldCharacter(serial);
	if (obj != NULL)
		obj->GeneratePaperdollText(ReadString(0));

	TGump *gump = GumpManager->UpdateGump(serial, 0, GT_PAPERDOLL);
	if (gump == NULL)
	{
		gump = new TGumpPaperdoll(serial, 0, 0, false);
		GumpManager->AddGump(gump);
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(ClientVersion)
{
	TPacketClientVersion packet(g_ClientVersionText);
	packet.Send();
	packet.Free();
}
//---------------------------------------------------------------------------
PACKET_HANDLER(Ping)
{
	g_PingSequence = ReadByte();

	if (g_PingCount)
		g_PingCount--;
	else
		Orion->Send(buf, 2);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(SetWeather)
{
	Weather.Reset();
	BYTE type = ReadByte();
	Weather.Type = type;
	Weather.SetCount(ReadByte());

	if (Weather.GetCount() > 70)
		Weather.SetCount(70);

	Weather.SetTemperature(ReadByte());
	Weather.SetTimer(g_Ticks + WEATHER_TIMER);
	Weather.Generate();

	switch (type)
	{
		case 0:
		{
			Orion->CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "It begins to rain.");
			break;
		}
		case 1:
		{
			Orion->CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "A fierce storm approaches.");
			break;
		}
		case 2:
		{
			Orion->CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "It begins to snow.");
			break;
		}
		case 3:
		{
			Orion->CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "A storm is brewing.");
			break;
		}
		case 0xFE:
		case 0xFF:
		{
			Weather.SetTimer(0);
			break;
		}
		default:
			break;
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(PersonalLightLevel)
{
	DWORD serial = ReadDWord();

	if (serial == g_PlayerSerial)
	{
		BYTE level = ReadByte();
		if (level > 0x1F)
			level = 0x1F;

		g_PersonalLightLevel = level;
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(LightLevel)
{
	BYTE level = ReadByte();
	if (level > 0x1F)
		level = 0x1F;

	g_LightLevel = level;
}
//---------------------------------------------------------------------------
PACKET_HANDLER(EnableLockedFeatures)
{
	if (m_ClientVersion >= CV_60142)
		g_LockedClientFeatures = ReadDWord();
	else
		g_LockedClientFeatures = ReadWord();
}
//---------------------------------------------------------------------------
PACKET_HANDLER(OpenContainer)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();
	WORD gumpid = ReadWord();

	TGump *gump = NULL;

	if (gumpid == 0xFFFF) //Spellbook
	{
		int GameWindowCenterX = (g_GameWindowPosX - 4) + g_GameWindowWidth / 2;
		int GameWindowCenterY = (g_GameWindowPosY - 4) + g_GameWindowHeight / 2;

		int x = GameWindowCenterX - 200;
		int y = GameWindowCenterY - 100;

		if (x < 0)
			x = 0;

		if (y < 0)
			y = 0;

		gump = new TGumpSpellbook(serial, x, y);
		Orion->PlaySoundEffect(0x0055);
	}
	else if (gumpid == 0x0030) //Buylist
	{
		GumpManager->CloseGump(0, 0, GT_SHOP);

		gump = new TGumpBuy(serial, 150, 5);
	}
	else //Container
	{
		WORD graphic = 0xFFFF;

		IFOR(i, 0, CONTAINERS_COUNT)
		{
			if (gumpid == g_ContainerOffset[i].Gump)
			{
				graphic = i;
				break;
			}
		}

		if (graphic == 0xFFFF)
			return;

		ContainerRect.Calculate(graphic);

		gump = new TGumpContainer(serial, ContainerRect.X, ContainerRect.Y);
		gump->Graphic = graphic;
		((TGumpContainer*)gump)->IsGameBoard = (gumpid == 0x091A || gumpid == 0x092E);
		Orion->ExecuteGump(gumpid, 0);
	}

	if (gump == NULL)
		return;

	gump->ID = gumpid;
	
	if (gumpid != 0x0030)
	{
		if (ContainerStack != NULL)
		{
			TContainerStack *cont = ContainerStack;

			while (cont != NULL)
			{
				if (cont->Serial == serial)
				{
					gump->X = cont->X;
					gump->Y = cont->Y;
					gump->Minimized = cont->Minimized;
					gump->MinimizedX = cont->MinimizedX;
					gump->MinimizedY = cont->MinimizedY;
					gump->LockMoving = cont->LockMoving;

					if (cont == ContainerStack)
					{
						ContainerStack = ContainerStack->m_Next;

						if (ContainerStack != NULL)
							ContainerStack->m_Prev = NULL;
					}
					else
					{
						cont->m_Prev->m_Next = cont->m_Next;

						if (cont->m_Next != NULL)
							cont->m_Next->m_Prev = cont->m_Prev;
					}

					cont->m_Next = NULL;
					cont->m_Prev = NULL;

					delete cont;

					break;
				}

				cont = cont->m_Next;
			}

			if (g_CheckContainerStackTimer < g_Ticks && ContainerStack != NULL)
			{
				delete ContainerStack;
				ContainerStack = NULL;
			}
		}

		TGameItem *obj = World->FindWorldItem(serial);
		if (obj != NULL)
		{
			/*if (gumpid != 0xFFFF)*/ obj->Opened = true;
			if (!obj->IsCorpse())
				World->ClearContainer(obj);
		}
	}

	GumpManager->AddGump(gump);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(UpdateSkills)
{
	if (World == NULL)
		return;

	int type = ReadByte();
	bool HaveCap = (type == 0x02 || type == 0xDF);
	bool IsSingleUpdate = (type == 0xFF || type == 0xDF);
	TPRINT("Skill update type %i (Cap=%d)\n", type, HaveCap);

	PBYTE end = buf + size;

	while (Ptr < end)
	{
		WORD id = ReadWord();

		if (!id && !type)
			break;
		else if (!type || type == 0x02)
			id--;

		WORD BaseVal = ReadWord();
		WORD RealVal = ReadWord();
		BYTE lock = ReadByte();
		WORD Cap = 0;

		if (HaveCap)
			Cap = ReadWord();

		if (id < g_SkillsCount)
		{
			if (IsSingleUpdate)
			{
				float change = (float)(BaseVal / 10.0f) - g_Player->GetSkillBaseValue(id);

				if (change)
				{
					char str[128] = {0};
					sprintf(str, "Your skill in %s has %s by %.1f%%.  It is now %.1f%%.", g_Skills[id].m_Name.c_str(), ((change < 0) ? "decreased" : "increased"), change, g_Player->GetSkillBaseValue(id) + change);
					//else if (change > 0) sprintf(str, "Your skill in %s has increased by %.1f%%.  It is now %.1f%%.", UO->m_Skills[id].m_Name.c_str(), change, obj->GetSkillBaseValue(id) + change);
					Orion->CreateTextMessage(TT_SYSTEM, 0, 3, 0x58, str);
				}
			}

			g_Player->SetSkillBaseValue(id, (float)(BaseVal / 10.0f));
			g_Player->SetSkillValue(id, (float)(RealVal / 10.0f));
			g_Player->SetSkillCap(id, (float)(Cap / 10.0f));
			g_Player->SetSkillStatus(id, lock);

			if (HaveCap)
				TPRINT("Skill %i is %i|%i|%i\n", id, BaseVal, RealVal, Cap);
			else
				TPRINT("Skill %i is %i|%i\n", id, BaseVal, RealVal);
		}
		else
			TPRINT("Unknown skill update %d\n", id);
	}

	g_SkillsTotal = 0.0f;
	IFOR(i, 0, g_SkillsCount)
		g_SkillsTotal += g_Player->GetSkillValue(i);

	GumpManager->UpdateGump(g_PlayerSerial, 0, GT_SKILLS);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(ExtendedCommand)
{
	WORD cmd = ReadWord();

	switch (cmd)
	{
		case 0:
			break;
		case 1: //Initialize Fast Walk Prevention
		{
			IFOR(i, 0, 6)
				Walker->m_FastWalkStack.Push(ReadDWord());

			break;
		}
		case 2: //Add key to Fast Walk Stack
		{
			Walker->m_FastWalkStack.Push(ReadDWord());

			break;
		}
		case 4: //Close generic gump
		{
			DWORD id = ReadDWord();
			DWORD button = ReadDWord();

			QFOR(gump, GumpManager->m_Items, TGump*)
			{
				if (gump->GumpType == GT_GENERIC && gump->ID == id)
				{
					((TGumpGeneric*)gump)->SendGumpResponse(button);

					break;
				}
			}

			break;
		}
		case 5: //Screen size
		{
			//g_GameWindowSizeX = unpack16(buf + 5);
			//g_GameWindowSizeY = unpack16(buf + 9);
			break;
		}
		case 6: //Party commands
		{
			Party.ParsePacketData(buf + 5, size);

			break;
		}
		case 8: //Set cursor / map
		{
			Orion->ChangeMap(ReadByte());

			break;
		}
		case 0xC: //Close statusbar gump
		{
			DWORD serial = ReadDWord();
			GumpManager->CloseGump(serial, 0, GT_STATUSBAR);

			break;
		}
		case 0x14: //Display Popup/context menu (2D and KR)
		{
			Move(1);
			BYTE mode = ReadByte();
			DWORD serial = ReadDWord();
			BYTE count = ReadByte();

			TGumpPopupMenu *menu = new TGumpPopupMenu(serial, g_MouseX, g_MouseY);
			int width = 0;
			int height = 20;

			IFOR(i, 0, count)
			{
				DWORD cliloc = ReadDWord();
				WORD index = ReadWord();
				WORD flags = ReadWord();
				WORD color = 0xFFFF;

				if (flags == 0x20)
					color = ReadWord();

				TGumpText *item = new TGumpText(index, color, 0, 0);

				wstring str = ClilocManager->Cliloc(g_Language)->GetW(cliloc);
				FontManager->GenerateW(0, item->m_Text, str.c_str(), color);

				height += item->m_Text.Height;

				if (width < item->m_Text.Width)
					width = item->m_Text.Width;

				menu->Add(item);
			}

			width += 20;

			if (height <= 20 || width <= 20)
				delete menu;
			else
			{
				menu->Width = width;
				menu->Height = height;
				GumpManager->AddGump(menu);
			}

			break;
		}
		case 0x16: //Close User Interface Windows
		{
			//ID:
			//0x01: Paperdoll
			//0x02: Status
			//0x08: Character Profile
			//0x0C: Container
			DWORD ID = ReadDWord();
			DWORD Serial = ReadDWord();

			switch (ID)
			{
				case 1: //Paperdoll
				{
					GumpManager->CloseGump(Serial, 0, GT_PAPERDOLL);
					break;
				}
				case 2: //Statusbar
				{
					GumpManager->CloseGump(Serial, 0, GT_STATUSBAR);
					break;
				}
				case 8: //Character Profile
				{
					//UO->CloseGump(Serial, 0, GT_PROFILE);
					break;
				}
				case 0xC: //Container
				{
					GumpManager->CloseGump(Serial, 0, GT_CONTAINER);
					break;
				}
				default:
					break;
			}

			break;
		}
		case 0x19: //Extended stats
		{
			Move(1);
			
			if (ReadDWord() == g_PlayerSerial)
			{
				Move(1);
				BYTE state = ReadByte();

				g_Player->LockStr = (state >> 4) & 3;
				g_Player->LockDex = (state >> 2) & 3;
				g_Player->LockInt = state & 3;

				TGump *statusbar = GumpManager->GetGump(g_PlayerSerial, 0, GT_STATUSBAR);

				if (statusbar != NULL && !statusbar->Minimized)
					statusbar->FrameCreated = false;
			}

			break;
		}
		case 0x1B: //New spellbook content
		{
			Move(2);
			DWORD serial = ReadDWord();

			TGameItem *spellbook = World->FindWorldItem(serial);

			if (spellbook == NULL)
			{
				TPRINT("Where is a spellbook?!?\n");
				return;
			}

			World->ClearContainer(spellbook);

			WORD graphic = ReadWord();
			SPELLBOOK_TYPE bookType = (SPELLBOOK_TYPE)ReadWord();

			DWORD spells[2] = { 0 };
			
			IFOR(j, 0, 2)
			{
				IFOR(i, 0, 4)
					spells[j] |= (ReadByte() << (i * 8));
			}

			switch (bookType)
			{
				case ST_MAGE:
				{
					IFOR(j, 0, 2)
					{
						IFOR(i, 0, 32)
						{
							if (spells[j] & (1 << i))
							{
								TGameItem *spellItem = new TGameItem();
								spellItem->Graphic = 0x1F2E;
								spellItem->Count = (j * 32) + i + 1;

								spellbook->AddItem(spellItem);
							}
						}
					}

					break;
				}
				default:
					break;
			}
		}
		case 0x26:
		{
			BYTE val = ReadByte();

			if (val > CST_FAST_UNMOUNT_AND_CANT_RUN)
				val = 0;

			g_SpeedMode = (CHARACTER_SPEED_TYPE)val;

			break;
		}
		default:
			break;
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(DenyWalk)
{
	g_WalkRequestCount = 0;

	if (g_Player == NULL)
		return;

	Move(1);
	g_Player->X = ReadWord();
	g_Player->Y = ReadWord();
	BYTE dir = ReadByte();
	g_Player->Direction = dir;
	g_Player->Z = ReadByte();

	Walker->SetSequence(0, dir);
	g_Player->OffsetX = 0;
	g_Player->OffsetY = 0;
	g_Player->OffsetZ = 0;

	g_Player->m_WalkStack.Clear();
}
//---------------------------------------------------------------------------
PACKET_HANDLER(ConfirmWalk)
{
	if (g_WalkRequestCount)
		g_WalkRequestCount--;

	if (g_Player == NULL)
		return;

	BYTE Seq = ReadByte();
	//player->SetDirection(newdir);

	BYTE newnoto = ReadByte();

	if (newnoto >= 0x40)
		newnoto ^= 0x40;

	if (!newnoto || newnoto >= 7)
		newnoto = 0x01;

	g_Player->Notoriety = newnoto;

	World->MoveToTop(g_Player);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(OpenUrl)
{
	Orion->GoToWebLink(ReadString(0));
}
//---------------------------------------------------------------------------
PACKET_HANDLER(Target)
{
	Target.SetData(buf, size);

	if (g_PartyHelperTimer > g_Ticks && g_PartyHelperTarget)
	{
		Target.SendTargetObject(g_PartyHelperTarget);
		g_PartyHelperTimer = 0;
		g_PartyHelperTarget = 0;
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(Talk)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();
	WORD graphic = ReadWord();
	SPEECH_TYPE type = (SPEECH_TYPE)ReadByte();
	WORD textColor = ReadWord();
	WORD font = ReadWord();

	if (!serial && font == 0xFFFF && textColor == 0xFFFF)
	{
		BYTE sbuffer[0x28] =
		{
			0x03, 0x00, 0x28, 0x20, 0x00, 0x34, 0x00, 0x03, 0xdb, 0x13, 0x14, 0x3f, 0x45, 0x2c, 0x58, 0x0f,
			0x5d, 0x44, 0x2e, 0x50, 0x11, 0xdf, 0x75, 0x5c, 0xe0, 0x3e, 0x71, 0x4f, 0x31, 0x34, 0x05, 0x4e,
			0x18, 0x1e, 0x72, 0x0f, 0x59, 0xad, 0xf5, 0x00
		};

		Orion->Send(sbuffer, 0x28);

		return;
	}
	
	string name(ReadString(0));
	string str = "";

	if (size > 44)
	{
		Ptr = buf + 44;
		str = ReadString(0);
	}

	if (type == ST_BROADCAST || /*type == ST_SYSTEM ||*/ serial == 0xFFFFFFFF || !serial || name == string("System"))
		Orion->CreateTextMessage(TT_SYSTEM, serial, font, textColor, str);
	else
	{
		if (type == ST_EMOTE)
		{
			textColor = ConfigManager.EmoteColor;
			str = "*" + str + "*";
		}

		TGameObject *obj = World->FindWorldObject(serial);

		if (obj != NULL)
		{
			obj->YouSeeJournalPrefix = (type == ST_SYSTEM);

			if (!obj->GetName().length())
			{
				obj->SetName(name);

				if (obj->NPC)
					GumpManager->UpdateGump(serial, 0, GT_STATUSBAR);
			}
		}

		Orion->CreateTextMessage(TT_OBJECT, serial, font, textColor, str);

		if (obj != NULL)
			obj->YouSeeJournalPrefix = false;
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(UnicodeTalk)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();
	WORD graphic = ReadWord();
	SPEECH_TYPE type = (SPEECH_TYPE)ReadByte();
	WORD textColor = ReadWord();
	WORD font = ReadWord();
	DWORD language = ReadDWord();

	if (!serial && font == 0xFFFF && textColor == 0xFFFF)
	{
		BYTE sbuffer[0x28] =
		{
			0x03, 0x00, 0x28, 0x20, 0x00, 0x34, 0x00, 0x03, 0xdb, 0x13, 0x14, 0x3f, 0x45, 0x2c, 0x58, 0x0f,
			0x5d, 0x44, 0x2e, 0x50, 0x11, 0xdf, 0x75, 0x5c, 0xe0, 0x3e, 0x71, 0x4f, 0x31, 0x34, 0x05, 0x4e,
			0x18, 0x1e, 0x72, 0x0f, 0x59, 0xad, 0xf5, 0x00
		};

		Orion->Send(sbuffer, 0x28);

		return;
	}
	
	wstring name((wchar_t*)Ptr);
	//wstring name = ReadUnicodeStringLE(0);
	wstring str = L"";

	if (size > 48)
	{
		Ptr = buf + 48;
		str = ReadUnicodeString((size - 48) / 2);
	}
	
	if (type == ST_BROADCAST /*|| type == ST_SYSTEM*/ || serial == 0xFFFFFFFF || !serial || name == wstring(L"System"))
		Orion->CreateUnicodeTextMessage(TT_SYSTEM, serial, ConfigManager.SpeechFont, textColor, str);
	else
	{
		if (type == ST_EMOTE)
		{
			textColor = ConfigManager.EmoteColor;
			str = L"*" + str + L"*";
		}

		TGameObject *obj = World->FindWorldObject(serial);

		if (obj != NULL)
		{
			obj->YouSeeJournalPrefix = (type == ST_SYSTEM);

			if (!obj->GetName().length())
			{
				obj->SetName(ToString(name));

				if (obj->NPC)
					GumpManager->UpdateGump(serial, 0, GT_STATUSBAR);
			}
		}

		Orion->CreateUnicodeTextMessage(TT_OBJECT, serial, ConfigManager.SpeechFont, textColor, str);

		if (obj != NULL)
			obj->YouSeeJournalPrefix = false;
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(OpenMenuGump)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();
	DWORD id = ReadWord();

	BYTE nameLen = ReadByte();
	string name = ReadString(nameLen);

	BYTE count = ReadByte();

	TGump *gump = NULL;

	if (unpack16(Ptr)) //menu
	{
		gump = new TGumpMenu(serial, 0, 0, name);
		((TGumpMenu*)gump)->Count = count;
	}
	else //gray menu
	{
		int x = (g_ClientWidth / 2) - 200;
		int y = (g_ClientHeight / 2) - ((121 + (count * 21)) / 2);

		gump = new TGumpGrayMenu(serial, x, y, name);
		((TGumpGrayMenu*)gump)->Count = count;
	}

	gump->ID = id;

	IFOR(i, 0, count)
	{
		WORD graphic = ReadWord();
		Orion->ExecuteStaticArt(graphic);

		WORD color = ReadWord();
		if (color)
			color++;

		nameLen = ReadByte();
		name = ReadString(nameLen);

		TGumpObject *go = new TGumpMenuObject(graphic, color, name);
		gump->Add(go);
	}

	GumpManager->AddGump(gump);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(SecureTrading)
{
	if (World == NULL)
		return;

	BYTE type = ReadByte();
	DWORD serial = ReadDWord();

	if (type == 0) //Новое трэйд окно
	{
		DWORD id1 = ReadDWord();
		DWORD id2 = ReadDWord();
		BYTE hasName = ReadByte();

		TGumpSecureTrading *gump = new TGumpSecureTrading(serial, 0, 0, id1, id2);

		if (hasName && *Ptr)
			gump->SetText(ReadString(0));

		GumpManager->AddGump(gump);
	}
	else if (type == 1) //Отмена
		GumpManager->CloseGump(serial, 0, GT_TRADE);
	else if (type == 2) //Обновление
	{
		TGumpSecureTrading *gump = (TGumpSecureTrading*)GumpManager->UpdateGump(serial, 0, GT_TRADE);
		if (gump != NULL)
		{
			DWORD id1 = ReadDWord();
			DWORD id2 = ReadDWord();

			gump->StateMy = id1 != 0;
			gump->StateOpponent = id2 != 0;
		}
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(TextEntryDialog)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();
	BYTE parentID = ReadByte();
	BYTE buttonID = ReadByte();

	short textLen = ReadShort();
	string text = ReadString(textLen);

	bool haveCancel = !ReadByte();
	BYTE variant = ReadByte();
	int maxLength = ReadDWord();
	
	short descLen = ReadShort();
	string desc = ReadString(descLen);

	TGumpTextEntryDialog *gump = new TGumpTextEntryDialog(serial, 143, 172, variant, maxLength, text, desc);
	gump->NoClose = haveCancel;
	gump->ParentID = parentID;
	gump->ButtonID = buttonID;

	GumpManager->AddGump(gump);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(OpenGump)
{
	if (World == NULL)
		return;

	//TPRINT("Gump dump::\n");
	//TDUMP(buf, size);

	DWORD serial = ReadDWord();
	DWORD id = ReadDWord();
	int x = ReadInt();
	int y = ReadInt();

	TGumpGeneric *gump = new TGumpGeneric(serial, x, y, id);

	PBYTE p = buf + 21;
	PBYTE e = p;

	int commandsLen = unpack16(buf + 19);
	PBYTE end = p + commandsLen;

	while (p < end)
	{
		while (p < end && *p && *p != '{')
			p++;

		e = p + 1;

		while (e < end && *e && *e == ' ')
			e++;

		char lowc[20] = {0};

		int eLen = strlen((char*)e);
		eLen = (eLen > 19 ? 20 : eLen);
		memcpy(&lowc[0], &e[0], eLen);
		lowc[19] = 0;
		_strlwr(lowc);

		TGumpObject *go = NULL;

		//TPRINT("\tlwr.token=%s\n", lowc);
		if (!memcmp(lowc, "nodispose", 9))
			e += 10;
		else if (!memcmp(lowc, "nomove", 6))
		{
			gump->NoMove = true;
			e += 7;
		}
		else if (!memcmp(lowc, "noclose", 7))
		{
			gump->NoClose = true;
			e += 7;
		}
		else if (!memcmp(lowc, "page", 4))
		{
			e += 5;
			int page = 0;
			sscanf((char*)e, "%d", &page);

			go = new TGumpPage(page);
		}
		else if (!memcmp(lowc, "group", 5))
		{
			e += 6;
			int group = 0;
			sscanf((char*)e, "%d", &group);

			go = new TGumpGroup(group);
		}
		else if (!memcmp(lowc, "endgroup", 8))
		{
			e += 9;

			go = new TGumpEndGroup();
		}
		else if (!memcmp(lowc, "resizepic", 9))
		{
			e += 10;
			int x = 0, y = 0, w = 0, h = 0, graphic = 0;
			sscanf((char*)e, "%d %d %d %d %d", &x, &y, &graphic, &w, &h);

			Orion->ExecuteResizepic(graphic);

			go = new TGumpResizepic(graphic, x, y, w, h);
		}
		else if (!memcmp(lowc, "checkertrans", 12))
		{
			e += 13;
			int x = 0, y = 0, w = 0, h = 0;
			sscanf((char*)e, "%d %d %d %d", &x, &y, &w, &h);

			go = new TGumpChecktrans(x, y, w, h);
		}
		else if (!memcmp(lowc, "buttontileart", 13))
		{
			e += 14;
			int x = 0, y = 0, action = 0, topage = 0, number = 0, up = 0, down = 0, tid = 0, tcolor = 0, tx = 0, ty = 0;
			sscanf((char*)e, "%d %d %d %d %d %d %d %d %d %d %d", &x, &y, &up, &down, &action, &topage, &number, &tid, &tcolor, &tx, &ty);

			Orion->ExecuteButton(up);
			Orion->ExecuteStaticArt(tid);

			go = new TGumpButtonTileArt(up, up + 1, down, number, topage, action != 0, x, y, tid, tcolor,tx, ty);
		}
		else if (!memcmp(lowc, "button", 6))
		{
			e += 7;
			int x = 0, y = 0, action = 0, topage = 0, number = 0, up = 0, down = 0;
			sscanf((char*)e, "%d %d %d %d %d %d %d", &x, &y, &up, &down, &action, &topage, &number);

			Orion->ExecuteButton(up);

			go = new TGumpButton(up, up + 1, down, number, topage, action != 0, x, y);
		}
		else if (!memcmp(lowc, "checkbox", 8))
		{
			e += 9;
			int x = 0, y = 0, state = 0, number = 0, up = 0, down = 0;
			sscanf((char*)e, "%d %d %d %d %d %d", &x, &y, &up, &down, &state, &number);

			Orion->ExecuteGump(up, 0);
			Orion->ExecuteGump(down, 0);

			go = new TGumpCheckbox(up, down, up + 1, number, state != 0, x, y);
		}
		else if (!memcmp(lowc, "radio", 5))
		{
			e += 6;
			int x = 0, y = 0, state = 0, number = 0, up = 0, down = 0;
			sscanf((char*)e, "%d %d %d %d %d %d", &x, &y, &up, &down, &state, &number);

			Orion->ExecuteGump(up, 0);
			Orion->ExecuteGump(down, 0);

			go = new TGumpRadio(up, down, up + 1, number, state != 0, x, y);
		}
		else if (!memcmp(lowc, "croppedtext", 11))
		{
			e +=12;
			int x = 0, y = 0, w = 0, h = 0, number = 0, color = 0;
			sscanf((char*)e, "%d %d %d %d %d %d", &x, &y, &w, &h, &color, &number);

			go = new TGumpCroppedText(number, color, x, y, w, h);
		}
		else if (!memcmp(lowc, "textentrylimited", 16))
		{
			e += 17;
			int x = 0, y = 0, w = 0, h = 0, index = 0, number = 0, length = 0, color = 0;
			sscanf((char*)e, "%d %d %d %d %d %d %d %d", &x, &y, &w, &h, &color, &index, &number, &length);

			go = new TGumpTextEntryLimited(number, color, x, y, w, h, index, length);
		}
		else if (!memcmp(lowc, "textentry", 9))
		{
			e += 10;
			int x = 0, y = 0, w = 0, h = 0, index = 0, number = 0, color = 0;
			sscanf((char*)e, "%d %d %d %d %d %d %d", &x, &y, &w, &h, &color, &index, &number);

			go = new TGumpTextEntry(number, color, x, y, w, h, index);
		}
		else if (!memcmp(lowc, "text", 4))
		{
			e += 5;
			int x = 0, y = 0, n = 0, color = 0;
			sscanf((char*)e, "%d %d %d %d", &x, &y, &color, &n);

			go = new TGumpText(n, color, x, y);
		}
		else if (!memcmp(lowc, "tilepichue", 10))
		{
			e += 11;
			int x = 0, y = 0, graphic = 0, color = 0;
			sscanf((char*)e, "%d %d %d %d", &x, &y, &graphic, &color);

			Orion->ExecuteStaticArt(graphic);

			go = new TGumpTilepic(graphic, color, x, y);
		}
		else if (!memcmp(lowc, "tilepic", 7))
		{
			e += 8;
			int x = 0, y = 0, graphic = 0;
			sscanf((char*)e, "%d %d %d", &x, &y, &graphic);

			Orion->ExecuteStaticArt(graphic);

			go = new TGumpTilepic(graphic, 0, x, y);
		}
		else if (!memcmp(lowc, "gumppictiled", 12))
		{
			e += 13;
			int x = 0, y = 0, w = 0, h = 0, graphic = 0;
			sscanf((char*)e, "%d %d %d %d %d", &x, &y, &w, &h, &graphic);

			Orion->ExecuteGump(graphic, 0);

			go = new TGumpGumppicTiled(graphic, x, y, w, h);
		}
		else if (!memcmp(lowc, "gumppic", 7))
		{
			e += 8;
			int x = 0, y = 0, graphic = 0, color = 0;
			sscanf((char*)e, "%d %d %d", &x, &y, &graphic);

			Orion->ExecuteGump(graphic, 0);

			char bufptr[20] = {0};
			sprintf(bufptr, "%d %d %d", x, y, graphic);

			int curlen = strlen(bufptr);
			while (e + curlen < end && e[curlen] == ' ')
				curlen++;

			if (e[curlen] != '}')
				sscanf((char*)(e + strlen(bufptr) + 5), "%d", &color);
			
			go = new TGumpGumppic(graphic, color, x, y);
		}
		else if (!memcmp(lowc, "xmfhtmlgump", 11))
		{
			e += 12;
			int x = 0, y = 0, w = 0, h = 0, background = 0, scrollbar = 0, clilocID = 0;
			sscanf((char*)e, "%d %d %d %d %d %d %d", &x, &y, &w, &h, &clilocID, &background, &scrollbar);

			go = new TGumpXFMHTMLGump(clilocID, 0, x, y, w, h, background, scrollbar);
		}
		else if (!memcmp(lowc, "xfmhtmlgumpcolor", 16))
		{
			e += 17;
			int x = 0, y = 0, w = 0, h = 0, background = 0, scrollbar = 0, clilocID = 0, color = 0;
			sscanf((char*)e, "%d %d %d %d %d %d %d %d", &x, &y, &w, &h, &clilocID, &background, &scrollbar, &color);

			go = new TGumpXFMHTMLGump(clilocID, color, x, y, w, h, background, scrollbar);
		}
		else if (!memcmp(lowc, "htmlgump", 8))
		{
			e += 9;
			int x = 0, y = 0, w = 0, h = 0, background = 0, scrollbar = 0, textID = 0;
			sscanf((char*)e, "%d %d %d %d %d %d %d", &x, &y, &w, &h, &textID, &background, &scrollbar);

			go = new TGumpHTMLGump(textID, x, y, w, h, background, scrollbar);
		}
		/*else if (!memcmp(lowc, "xfmhtmltok", 10))
		{
			e += 11;
			int x = 0, y = 0, w = 0, h = 0, background = 0, scrollbar = 0, clilocID = 0;
			sscanf((char*)e, "%d %d %d %d %d %d", &x, &y, &w, &h, &background, &scrollbar, &clilocID);

			go = new TGumpXFMHTMLToken(clilocID, x, y, w, h, background, scrollbar);
		}*/
		else if (!memcmp(lowc, "tooltip", 7))
		{
			e += 8;
			int clilocID = 0;
			sscanf((char*)e, "%d", &clilocID);

			go = new TGumpTooltip(clilocID);
		}
		else if (!memcmp(lowc, "mastergump", 10))
		{
			e += 11;
			int index = 0;
			sscanf((char*)e, "%d", &index);

			go = new TGumpMasterGump(index);
		}

		if (go != NULL)
			gump->Add(go);

		while (e < end && *e && *e != '}')
			e++;

		p = e + 1;
	}
	
	Ptr = buf + 21 + commandsLen;

	short textLinesCount = ReadShort();
	
	IFOR(i, 0, textLinesCount)
	{
		int linelen = ReadShort();

		if (linelen)
			gump->AddText(i, ReadUnicodeString(linelen));
	}
	
	GumpManager->AddGump(gump);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(OpenCompressedGump)
{
	if (World == NULL)
		return;

	DWORD senderID = ReadDWord();
	DWORD gumpID = ReadDWord();
	DWORD x = ReadDWord();
	DWORD y = ReadDWord();
	DWORD cLen = ReadDWord() - 4; //Compressed Length (4 == sizeof(DecompressedLen)
	DWORD dLen = ReadDWord(); //Decompressed Length

	if (cLen < 1)
	{
		TPRINT("CLen=%d\nServer Sends bad Compressed Gumpdata!\n", cLen);

		return;
	}
	else if ((int)(28 + cLen) > size)
	{
		TPRINT("Server Sends bad Compressed Gumpdata!\n");

		return;
	}

	// Layout data.....
	PBYTE decLayoutData = new BYTE[dLen];
	TPRINT("Gump layout:\n\tSenderID=0x%08X\n\tGumpID=0x%08X\n\tCLen=%d\n\tDLen=%d\nDecompressing layout gump data...\n", senderID, gumpID, cLen, dLen);

	int z_err = uncompress(decLayoutData, &dLen, Ptr, cLen);

	if (z_err != Z_OK)
	{
		delete decLayoutData;
		TPRINT("Decompress layout gump error %d\n", z_err);

		return;
	}

	TPRINT("Layout gump data decompressed!\n");
	// Text data.....

	Move(cLen);

	DWORD linesCount = ReadDWord(); //Text lines count
	DWORD cTLen = 0;
	DWORD dTLen = 0;
	PBYTE gumpDecText = NULL;

	if (linesCount > 0)
	{
		cTLen = ReadDWord(); //Compressed lines length
		dTLen = ReadDWord(); //Decompressed lines length

		gumpDecText = new BYTE[dTLen];

		TPRINT("Decompressing text gump data...\n");

		z_err = uncompress(gumpDecText, &dTLen, Ptr, cTLen);

		if (z_err != Z_OK)
		{
			delete decLayoutData;
			delete gumpDecText;
			TPRINT("Decompress text gump error %d\n", z_err);

			return;
		}

		TPRINT("Text gump data decompressed!\nGump text lines:\n\tLinesCount=%d\n\tCTLen=%d\n\tDTLen=%d\n", linesCount, cTLen, dTLen);
	}

	int newsize = 21 + dLen + 2 + dTLen;

	PBYTE newbuf = new BYTE[newsize];
	newbuf[0] = 0xb0;
	pack16(newbuf + 1, newsize);
	pack32(newbuf + 3, senderID);
	pack32(newbuf + 7, gumpID);
	pack32(newbuf + 11, x);
	pack32(newbuf + 15, y);
	pack16(newbuf + 19, (WORD)dLen);
	memcpy(newbuf + 21, decLayoutData, dLen);
	pack16(newbuf + 21 + dLen, (WORD)linesCount);

	if (linesCount > 0)
		memcpy(newbuf + 23 + dLen, gumpDecText, dTLen);
	else
		newbuf[newsize - 1] = 0x00;

	TPRINT("Gump decompressed! newsize=%d\n", newsize);

	ReceiveHandler(newbuf, newsize);

	delete decLayoutData;

	if (linesCount > 0)
		delete gumpDecText;

	delete newbuf;
}
//---------------------------------------------------------------------------
PACKET_HANDLER(TipWindow)
{
	BYTE flag = ReadByte();

	if (flag != 1) //1 - ignore
	{
		DWORD serial = ReadDWord();
		short len = ReadShort();

		string str = ReadString(len);

		int x = 20;
		int y = 20;

		if (!flag)
		{
			x = 200;
			y = 100;
		}

		TGumpTip *gump = new TGumpTip(serial, x, y, flag != 0);
		FontManager->GenerateA(6, gump->m_Text, str.c_str(), 0, 200);

		GumpManager->AddGump(gump);
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(MultiPlacement)
{
	if (World == NULL)
		return;

	//DWORD serial = unpack32(buf + 2);
	//WORD graphic = unpack16(buf + 18);

	Target.SetMultiData(buf, size);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(GraphicEffect)
{
	if (World == NULL)
		return;

	BYTE type = ReadByte();

	if (type > 3) //error
		return;

	DWORD sourceSerial = ReadDWord();
	DWORD destSerial = ReadDWord();
	WORD graphic = ReadWord();
	short sourceX = ReadShort();
	short sourceY = ReadShort();
	char sourceZ = ReadChar();
	short destX = ReadShort();
	short destY = ReadShort();
	char destZ = ReadChar();
	BYTE speed = ReadByte();
	short duration = (short)ReadByte() * 50;
	//what is in 24-25 bytes?
	Move(2);
	BYTE fixedDirection = ReadByte();
	BYTE explode = ReadByte();
	
	DWORD color = 0;
	DWORD renderMode = 0;

	if (*buf == 0xC0)
	{
		color = ReadDWord();
		renderMode = ReadDWord() % 7;
	}

	TGameEffect *effect = NULL;
	if (!type) //Moving
		effect = new TGameEffectMoving();
	else
		effect = new TGameEffect();

	effect->EffectType = (EFFECT_TYPE)type;
	effect->Serial = sourceSerial;
	effect->DestSerial = destSerial;
	effect->Graphic = graphic;
	effect->X = sourceX;
	effect->Y = sourceY;
	effect->Z = sourceZ;
	effect->DestX = destX;
	effect->DestY = destY;
	effect->DestZ = destZ;

	/*DWORD addressAnimData = (DWORD)FileManager.AnimdataMul.Address;

	if (addressAnimData)
	{
		PANIM_DATA pad = (PANIM_DATA)(addressAnimData + ((graphic * 68) + 4 * ((graphic / 8) + 1)));

		effect->Speed = (pad->FrameInterval - effect->Speed) * 50;
	}
	else*/
		effect->Speed = speed + 6;

	effect->Duration = g_Ticks + duration;
	effect->FixedDirection = (fixedDirection != 0);
	effect->Explode = (explode != 0);
	
	effect->Color = (WORD)color;
	effect->RenderMode = renderMode;

	EffectManager->AddEffect(effect);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(BuyList)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();

	TGameItem *container = World->FindWorldItem(serial);

	if (container == NULL)
	{
		EPRINT("Buy container is not found!!!\n");
		return;
	}

	TGameCharacter *vendor = World->FindWorldCharacter(container->Container);

	if (vendor == NULL)
	{
		EPRINT("Buy vendor is not found!!!\n");
		return;
	}

	if (container->Layer == OL_BUY_RESTOCK || container->Layer == OL_BUY)
	{
		BYTE count = ReadByte();

		int i = (g_InverseBuylist ? count - 1 : 0);
		int end = (g_InverseBuylist ? - 1 : count);
		int add = (g_InverseBuylist ? - 1 : 1);

		TGameItem *item = (TGameItem*)container->m_Items;

		if (g_InverseBuylist)
		{
			while (item != NULL && item->m_Next != NULL)
				item = (TGameItem*)item->m_Next;
		}

		for (; i != end; i += add)
		{
			if (item == NULL)
			{
				EPRINT("Buy item is not found!!!\n");
				break;
			}

			if (item->ShopItem == NULL)
				item->ShopItem = new TShopItem();

			TShopItem *shop = item->ShopItem;

			shop->Price = ReadDWord();

			BYTE nameLen = ReadByte();

			shop->SetName(ReadString(nameLen));

			if (g_InverseBuylist)
				item = (TGameItem*)item->m_Prev;
			else
				item = (TGameItem*)item->m_Next;
		}
	}
	else
		TPRINT("Unknown layer for buy container!!!\n");
}
//---------------------------------------------------------------------------
PACKET_HANDLER(SellList)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();

	TGameCharacter *vendor = World->FindWorldCharacter(serial);

	if (vendor == NULL)
	{
		EPRINT("Sell vendor is not found!!!\n");
		return;
	}

	WORD count = ReadWord();

	if (!count)
	{
		EPRINT("Sell list is empty.\n");
		return;
	}

	GumpManager->CloseGump(0,0, GT_SHOP);

	TGumpSell *gump = new TGumpSell(serial, 100, 0, count);

	IFOR(i, 0, count)
	{
		TGumpSellObject *go = new TGumpSellObject();
		go->Serial = ReadDWord();
		go->Graphic = ReadWord();
		go->Color = ReadWord();
		go->Count = ReadWord();
		go->Price = ReadWord();
		int nameLen = ReadShort();
		go->SetName(ReadString(0));
		
		if (!i) //First item must be selected
			go->Selected = true;

		gump->Add(go);
	}

	GumpManager->AddGump(gump);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(BuyReply)
{
	DWORD serial = ReadDWord();
	BYTE flag = ReadByte();

	if (!flag) //Close shop gump
		GumpManager->CloseGump(serial, 0, GT_SHOP);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(DeathScreen)
{
	Weather.Reset();
	Target.Reset();
	g_DeathScreenTimer = g_Ticks + DEATH_SCREEN_DELAY;
}
//---------------------------------------------------------------------------
PACKET_HANDLER(PlaySoundEffect)
{
	Move(1);
	WORD index = ReadWord();
	WORD volume = ReadWord();
	WORD xCoord = ReadWord();
	WORD yCoord = ReadWord();

	struct tagPOINT coords;
	coords.x = xCoord;
	coords.y = yCoord;
	int distance = GetDistance(g_Player, coords);
	//TPRINT("Play sound 0x%04X\n", index);

	Orion->PlaySoundEffect(index, SoundManager.GetVolumeValue(distance));
	
}
//---------------------------------------------------------------------------
PACKET_HANDLER(PlayMusic)
{
	WORD index = ReadWord();

	//TPRINT("Play midi music 0x%04X\n", index);
	if (!ConfigManager.Music || GetForegroundWindow() != g_hWnd || ConfigManager.MusicVolume < 1)
		return;
	Orion->PlayMusic(index);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(DragAnimation)
{
	if (World == NULL)
		return;

	WORD graphic = ReadWord();

	if (graphic == 0x0EED)
		graphic = 0x0EEF;
	else if (graphic == 0x0EEA)
		graphic = 0x0EEC;
	else if (graphic == 0x0EF0)
		graphic = 0x0EF2;

	Move(3);
	WORD count = ReadWord();

	DWORD sourceSerial = ReadDWord();
	short sourceX = ReadShort();
	short sourceY = ReadShort();
	char sourceZ = ReadChar();
	DWORD destSerial = ReadDWord();
	short destX = ReadShort();
	short destY = ReadShort();
	char destZ = ReadChar();
	
	TGameEffect *effect = NULL;

	if (sourceSerial < 0x40000000) //Игрок/НПС кладет предмет в контейнер
	{
		effect = new TGameEffectMoving();
		effect->FixedDirection = true;
	}
	else //Предмет взяли из контейнера
	{
		effect = new TGameEffectDrag();
	}

	effect->EffectType = EF_DRAG;
	effect->Serial = sourceSerial;
	effect->DestSerial = destSerial;
	effect->X = sourceX;
	effect->Y = sourceY;
	effect->Z = sourceZ;
	effect->DestX = destX;
	effect->DestY = destY;
	effect->DestZ = destZ;
	effect->Speed = 5;
	effect->Duration = g_Ticks + 5000;

	effect->Graphic = graphic;

	EffectManager->AddEffect(effect);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(CorpseEquipment)
{
	if (World == NULL)
		return;

	DWORD cserial = ReadDWord();

	PBYTE end = buf + size;

	int layer = ReadByte();

	while (layer && Ptr < end)
	{
		DWORD serial = ReadDWord();

		TGameItem *obj = World->FindWorldItem(serial);

		if (obj != NULL)
			World->PutEquipment(obj, cserial, layer);

		layer = ReadByte();
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(ASCIIPrompt)
{
	if (World == NULL)
		return;

	if (g_ConsolePrompt != PT_NONE)
	{
		Orion->ConsolePromptCancel();
		Orion->CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "Previous prompt cancelled.");
	}

	g_ConsolePrompt = PT_ASCII;
	memcpy(&g_LastASCIIPrompt[0], &buf[0], 11);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(UnicodePrompt)
{
	if (World == NULL)
		return;

	if (g_ConsolePrompt != PT_NONE)
	{
		Orion->ConsolePromptCancel();
		Orion->CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "Previous prompt cancelled.");
	}

	g_ConsolePrompt = PT_UNICODE;
	memcpy(&g_LastUnicodePrompt[0], &buf[0], 11);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(CharacterAnimation)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();
	TGameCharacter *obj = World->FindWorldCharacter(serial);

	if (obj != NULL)
	{
		WORD action = ReadWord();
		Move(1);
		BYTE frameCount = ReadByte();
		frameCount = 0;
		WORD repeatMode = ReadWord();
		bool frameDirection = (ReadByte() == 0); //true - forward, false - backward
		bool repeat = (ReadByte() != 0);
		BYTE delay = ReadByte();

		obj->SetAnimation((BYTE)action, delay, frameCount, (BYTE)repeatMode, repeat, frameDirection);
		obj->AnimationFromServer = true;

		TPRINT("Anim is set\n");
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(DisplayQuestArrow)
{
	QuestArrow.Timer = g_Ticks + 1000;
	QuestArrow.Enabled = (ReadByte() != 0);
	QuestArrow.X = ReadWord();
	QuestArrow.Y = ReadWord();
}
//---------------------------------------------------------------------------
PACKET_HANDLER(DisplayMap)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();
	WORD gumpid = ReadWord();

	//TPRINT("gumpid = 0x%04X\n", gumpid);

	WORD startX = ReadWord();
	WORD startY = ReadWord();
	WORD endX = ReadWord();
	WORD endY = ReadWord();
	WORD width = ReadWord();
	WORD height = ReadWord();
	WORD facet = 0;

	if (*buf == 0xF5)
		facet = ReadWord();

	TGumpMap *gump = new TGumpMap(serial, 0, 0);

	gump->StartX = startX;
	gump->StartY = startY;
	gump->EndX = endX;
	gump->EndY = endY;
	gump->Width = width;
	gump->Height = height;

	MultiMap->LoadMap(gump);

	//TPRINT("GumpX=%d GumpY=%d\n", startX, startY);
	//TPRINT("GumpTX=%d GumpTY=%d\n", endX, endY);
	//TPRINT("GumpW=%d GumpH=%d\n", width, height);

	GumpManager->AddGump(gump);

	TGameItem *obj = World->FindWorldItem(serial);
	if (obj != NULL)
		obj->Opened = true;
}
//---------------------------------------------------------------------------
PACKET_HANDLER(MapData)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();
	TGumpMap *gump = (TGumpMap*)GumpManager->UpdateGump(serial, 0, GT_MAP);

	if (gump != NULL)
	{
		switch ((MAP_MESSAGE)ReadByte()) //Action
		{
			case MM_ADD: //Add Pin
			{
				Move(1);

				short x = ReadShort();
				short y = ReadShort();

				TGumpObject *go = new TGumpObject(GOT_NONE, 0, 0, x, y);

				gump->Add(go);

				break;
			}
			case MM_INSERT: //Insert New Pin
			{
				break;
			}
			case MM_MOVE: //Change Pin
			{
				break;
			}
			case MM_REMOVE: //Remove Pin
			{
				break;
			}
			case MM_CLEAR: //Clear Pins
			{
				gump->Clear();

				break;
			}
			case MM_EDIT_RESPONSE: //Reply From Server to Action 6 (Plotting request)
			{
				gump->PlotState = ReadByte();

				break;
			}
			default:
				break;
		}
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(DyeData)
{
	DWORD serial = ReadDWord();
	Move(2);
	WORD graphic = ReadWord();

	TGumpDye *gump = new TGumpDye(serial, 0, 0, graphic);

	GumpManager->AddGump(gump);

	POINT p = { 0, 0 };
	Orion->GetGumpDimension(0x0906, p);

	gump->X = (WORD)((g_ClientWidth / 2) - (p.x / 2));
	gump->Y = (WORD)((g_ClientHeight / 2) - (p.y / 2));
}
//---------------------------------------------------------------------------
PACKET_HANDLER(CharacterProfile)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();

	wstring topText = ToWString(ReadString(0));

	wstring bottomText = ReadUnicodeString(0);
	wstring dataText = ReadUnicodeString(0);

	TGumpProfile *gump = new TGumpProfile(serial, 170, 90, topText, bottomText, dataText);

	GumpManager->AddGump(gump);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(ClientViewRange)
{
	g_UpdateRange = ReadByte();
}
//---------------------------------------------------------------------------
PACKET_HANDLER(KrriosClientSpecial)
{
	TPacketRazorAnswer packet;
	packet.Send();
}
//---------------------------------------------------------------------------
PACKET_HANDLER(AssistVersion)
{
	DWORD version = ReadDWord();

	TPacketAssistVersion packet(version, g_ClientVersionText);
	packet.Send();
	packet.Free();
}
//---------------------------------------------------------------------------
PACKET_HANDLER(CharacterListNotification)
{
	Orion->InitScreen(GS_DELETE);
	ConnectionScreen->Type = CST_CHARACTER_LIST;
	ConnectionScreen->ConnectionFailed = true;
	ConnectionScreen->ErrorCode = ReadByte();
}
//---------------------------------------------------------------------------
PACKET_HANDLER(ErrorCode)
{
	BYTE code = ReadByte();

	Orion->InitScreen(GS_DELETE);
	ConnectionScreen->Type = CST_GAME_LOGIN;
	ConnectionScreen->ErrorCode = code;

	if (code > 7)
		ConnectionScreen->ErrorCode = 3;
}
//---------------------------------------------------------------------------
PACKET_HANDLER(AttackCharacter)
{
	g_LastAttackObject = ReadDWord();

	if (g_LastAttackObject != 0 && World != NULL)
	{
		TGameCharacter *obj = World->FindWorldCharacter(g_LastAttackObject);

		if (obj != NULL && !obj->MaxHits)
			TPacketStatusRequest(g_LastAttackObject).Send();
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(Season)
{
	g_Season = ReadByte();

	if (ReadByte()) //Play sound
	{
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(BulletinBoardData)
{
	if (World == NULL)
		return;

	switch (ReadByte())
	{
		case 0: //Open board
		{
			DWORD serial = ReadDWord();

			TGameItem *item = World->FindWorldItem(serial);
			if (item != NULL)
			{
				TGump *bbGump = GumpManager->UpdateGump(serial, 0, GT_BULLETIN_BOARD);

				if (bbGump != NULL)
					bbGump->Clear();

				item->Opened = true;
			}

			string str((char*)Ptr);

			int x = (g_ClientWidth / 2) - 245;
			int y = (g_ClientHeight / 2) - 205;

			TGumpBulletinBoard *gump = new TGumpBulletinBoard(serial, x, y, str);

			GumpManager->AddGump(gump);

			break;
		}
		case 1: //Summary message
		{
			DWORD boardSerial = ReadDWord();

			TGumpBulletinBoard *gump = (TGumpBulletinBoard*)GumpManager->GetGump(boardSerial, 0, GT_BULLETIN_BOARD);

			if (gump != NULL)
			{
				DWORD serial = ReadDWord();
				DWORD parentID = ReadDWord();

				//poster
				int len = ReadByte();
				string text = ReadString(len) + " - ";

				//subject
				len = ReadByte();
				text += ReadString(len) + " - ";

				//data time
				len = ReadByte();
				text += ReadString(len);

				TGumpBulletinBoardObject *item = new TGumpBulletinBoardObject(serial, text);

				gump->Add(item);
			}

			break;
		}
		case 2: //Message
		{
			DWORD boardSerial = ReadDWord();

			TGumpBulletinBoard *gump = (TGumpBulletinBoard*)GumpManager->GetGump(boardSerial, 0, GT_BULLETIN_BOARD);

			if (gump != NULL)
			{
				DWORD serial = ReadDWord();

				//poster
				int len = ReadByte();
				string poster = ReadString(len);

				//subject
				len = ReadByte();
				string subject = ReadString(len);

				//data time
				len = ReadByte();
				string dataTime = ReadString(len);

				Move(5);

				BYTE lines = ReadByte();
				string data = "";

				IFOR(i, 0, lines)
				{
					BYTE linelen = ReadByte();

					if (data.length())
						data += "\n";

					data += ReadString(linelen);
				}

				BYTE variant = 1 + (int)(poster == g_Player->GetName());
				TGumpBulletinBoardItem *gump = new TGumpBulletinBoardItem(serial, 0, 0, variant, boardSerial, poster, subject, dataTime, data);

				GumpManager->AddGump(gump);
			}

			break;
		}
		default:
			break;
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(DisplayDeath)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();
	DWORD corpseSerial = ReadDWord();

	TGameItem *obj = World->FindWorldItem(corpseSerial);
	if (obj != NULL)
		obj->AnimIndex = 0;
	else
	{
		pair<DWORD, DWORD> p(corpseSerial, g_Ticks + 1000);
		g_CorpseSerialList.push_back(p);
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(OpenChat)
{
	BYTE newbuf[4] = { 0xf0, 0x00, 0x04, 0xff };
	Orion->Send(newbuf, 4);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(DisplayClilocString)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();
	WORD graphic = ReadWord();
	BYTE type = ReadByte();
	WORD color = ReadWord();
	WORD font = FontManager->UnicodeFontExists((BYTE)ReadWord());
	DWORD cliloc = ReadDWord();

	BYTE flags = 0;
	if (*buf == 0xCC)
		flags = ReadByte();

	string name = ReadString(30);

	string affix = "";
	if (*buf == 0xCC)
		affix = ReadString(0);

	wstring args((wchar_t*)Ptr);
	//wstring args = ReadUnicodeStringLE(0);
	wstring message = ClilocManager->ParseArgumentsToClilocString(cliloc, args);
	//wstring message = ClilocManager->Cliloc(g_Language)->GetW(cliloc);

	if (/*type == ST_BROADCAST || type == ST_SYSTEM ||*/ serial == 0xFFFFFFFF || !serial || name == string("System"))
		Orion->CreateUnicodeTextMessage(TT_SYSTEM, serial, font, color, message);
	else
	{
		/*if (type == ST_EMOTE)
		{
			color = ConfigManager.EmoteColor;
			str = L"*" + str + L"*";
		}*/

		//if (serial >= 0x40000000) //Только для предметов
		{
			TGameObject *obj = World->FindWorldObject(serial);

			if (obj != NULL && !obj->GetName().length())
			{
				obj->SetName(name);

				if (obj->NPC)
					GumpManager->UpdateGump(serial, 0, GT_STATUSBAR);
			}
		}

		Orion->CreateUnicodeTextMessage(TT_OBJECT, serial, font, color, message);
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(MegaCliloc)
{
	if (World == NULL)
		return;

	WORD wat = ReadWord();
	DWORD serial = ReadDWord();

	TGameObject *obj = World->FindWorldObject(serial);

	if (obj == NULL)
		return;

	WORD wat2 = ReadWord();
	DWORD testedSerial = ReadDWord();
	wstring message(L"");

	if (!obj->NPC)
		message = L"<basefont color=\"yellow\">";

	PBYTE end = buf + size;
	bool first = true;

	while (Ptr < end)
	{
		DWORD cliloc = ReadDWord();

		if (!cliloc)
			break;

		short len = ReadShort();

		wstring argument = L"";

		if (len > 0)
		{
			argument = wstring((wchar_t*)Ptr, len / 2);
			Ptr += len;
			//wstring argument = ReadUnicodeStringLE(len / 2);
		}

		wstring str = ClilocManager->ParseArgumentsToClilocString(cliloc, argument);
		//TPRINT("Cliloc: argstr=%s\n", ToString(str).c_str());

		if (message.length() && !first)
			message += L"\n";

		message += str;

		if (first)
		{
			if (!obj->NPC)
				message += L"<basefont color=\"#FFFFFFFF\">";

			first = false;
		}

		//TPRINT("Cliloc: 0x%08X len=%i arg=%s\n", cliloc, len, ToString(argument).c_str());
	}

	//TDUMP((PBYTE)message.c_str(), message.length() * 2);
	obj->ClilocMessage = message;
	//TPRINT("message=%s\n", ToString(message).c_str());
}
//---------------------------------------------------------------------------
PACKET_HANDLER(Damage)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();
	TGameCharacter *character = World->FindWorldCharacter(serial);

	if (character != NULL)
	{
		int damage = ReadWord();

		TTextData *text = new TTextData();
		text->Unicode = false;
		text->Font = 3;
		text->Serial = serial;
		text->Color = 0x0035;
		text->Type = TT_OBJECT;
		text->SetText(std::to_string(damage));
		text->GenerateTexture(0);
		text->DrawX = text->m_Texture.Width / 2;
		int height = text->m_Texture.Height;

		TTextData *head = character->m_DamageTextControl->m_Head;
		if (head != NULL)
		{
			height += head->DrawY;

			if (height > 0)
			{
				if (height > 100)
					height = 0;

				text->DrawY = height;
			}
		}

		character->m_DamageTextControl->Add(text);
		text->Timer = g_Ticks + DAMAGE_TEXT_NORMAL_DELAY;
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(OpenBook)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();
	BYTE flags = ReadByte();
	Move(1);
	WORD pageCount = ReadWord();

	TGumpBook *gump = new TGumpBook(serial, 0, 0, pageCount, flags != 0, false);

	gump->TextEntryTitle->SetText(ReadString(60));
	gump->TextEntryAuthor->SetText(ReadString(30));

	GumpManager->AddGump(gump);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(OpenBookNew)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();
	BYTE flag1 = ReadByte();
	BYTE flag2 = ReadByte();
	WORD pageCount = ReadWord();

	TGumpBook *gump = new TGumpBook(serial, 0, 0, pageCount, (flag1 + flag2) != 0, true);

	int authorLen = ReadWord();

	PBYTE ptr = buf + 13;

	if (authorLen > 0)
	{
		wchar_t *author = new wchar_t[authorLen];
		*author = 0;
		PBYTE aptr = (PBYTE)author;
		//
		ptr += (authorLen * 2);
		//
		gump->TextEntryAuthor->SetText(author);

		delete author;
	}
	else
		ptr += 2;

	int titleLen = unpack16(ptr);
	ptr += 2;

	if (titleLen > 0)
	{
		wchar_t *title = new wchar_t[titleLen];
		*title = 0;
		PBYTE tptr = (PBYTE)title;
		//
		gump->TextEntryTitle->SetText(title);

		delete title;
	}

	GumpManager->AddGump(gump);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(BookData)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();

	TGumpBook *gump = (TGumpBook*)GumpManager->GetGump(serial, 0, GT_BOOK);

	if (gump != NULL)
	{
		WORD pageCount = ReadWord();

		IFOR(i, 0, pageCount)
		{
			WORD page = ReadWord();

			if (page >= gump->PageCount)
				continue;

			TEntryText *entry = &gump->TextEntry[page];
			entry->Clear();

			WORD lineCount = ReadWord();

			if (!gump->Unicode)
			{
				string str = "";

				IFOR(j, 0, lineCount)
				{
					if (j)
						str += '\n';

					str += ReadString(0);
				}

				entry->SetText(str);

				//TPRINT("BookPageData[%i] = %s\n", page, str.c_str());
			}
			else
			{
				wstring str = L"";

				IFOR(j, 0, lineCount)
				{
					if (j)
						str += L'\n';

					str += ReadUnicodeString(0);
				}

				entry->SetText(str);
			}
		}
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(BuffDebuff)
{
	if (World == NULL)
		return;

/*
df 
00 2e 
00 00 2b b5 
04 04 
00 01 
00 00 00 00 04 04 00 01 00 00 00 00 
01 ca 
00 00 00 
00 10 6a 6b 
00 10 6a 6c 
00 00 00 00 
00 00 
00 00 
00 00



df 
00 46 
00 00 2b b5 
04 05 
00 01 
00 00 00 00 04 05 00 01 00 00 00 00 
00 85 
00 00 00 
00 10 6a 66 
00 10 56 82 
00 00 00 00 
00 01 
00 00 
09 00 2b 00 20 00 39 00 20 00 41 00 72 00 6d 00 6f 00 72 00 00 00 
00 01 
00 00




Buffs And Attributes Packet.
from server
byte    ID (DF)
word    Packet Size
dword    Player Serial
word    Buff Type
			(
				BonusStr = 0x01, BonusDex = 0x02, BonusInt = 0x03, BonusHits = 0x07, BonusStamina = 0x08, BonusMana = 0x09, 
				RegenHits = 0x0A, RegenStam = 0x0B, RegenMana = 0x0C, NightSight = 0x0D, Luck = 0x0E, ReflectPhysical = 0x10, 
				EnhancePotions = 0x11, AttackChance = 0x12, DefendChance = 0x13, SpellDamage = 0x14, CastRecovery = 0x15, 
				CastSpeed = 0x16, ManaCost = 0x17, ReagentCost = 0x18, WeaponSpeed = 0x19, WeaponDamage = 0x1A, 
				PhysicalResistance = 0x1B, FireResistance = 0x1C, ColdResistance = 0x1D, PoisonResistance = 0x1E, 
				EnergyResistance = 0x1F, MaxPhysicalResistance = 0x20, MaxFireResistance = 0x21, MaxColdResistance = 0x22, 
				MaxPoisonResistance = 0x23, MaxEnergyResistance = 0x24, AmmoCost = 0x26, KarmaLoss = 0x28, 0x3EA+ = buff icons
			)

word    Buffs Count

loop    Buffs >>>
	word    Source Type
				(
					0 = Character, 50 = two-handed weapon, 53 = one-handed weapon or spellbook, 54 = shield or ranged weapon,
					55 = shoes, 56 = pants or legs, 58 = helm or hat, 59 = gloves, 60 = ring, 61 = talisman, 62 = necklace or gorget,
					64 = waist, 65 = inner torso, 66 = bracelet, 69 = middle torso, 70 = earring, 71 = arms, 72 = cloak or quiver,
					74 = outer torso, 1000 = spells
				)

	word    0x00
	word    Buff Icon ID (0 for attributes)
	word    Buff Queue Index (Delta Value for attributes)
	dword    0x00
	word    Buff Duration in seconds (0 for attributes)
	byte[3]    0x00
	dword    Buff Title Cliloc
	dword    Buff Secondary Cliloc (0 for attributes)
	dword    Buff Third Cliloc (0 for attributes)
	word    Primary Cliloc Arguments Length (0 for attributes)
	uchar[*]    Primary Cliloc Arguments
	word    Secondary Cliloc Arguments Length (0 for attributes)
	uchar[*]    Secondary Cliloc Arguments
	word    Third Cliloc Arguments Length (0 for attributes)
	uchar[*]    Third Cliloc Arguments
endloop    Buffs <<<<
*/



	const int tableCount = 126;

	static const WORD table[tableCount] =
	{
		0x754C, 0x754A, 0x0000, 0x0000, 0x755E, 0x7549, 0x7551, 0x7556, 0x753A, 0x754D,
		0x754E, 0x7565, 0x753B, 0x7543, 0x7544, 0x7546, 0x755C, 0x755F, 0x7566, 0x7554,
		0x7540, 0x7568, 0x754F, 0x7550, 0x7553, 0x753E, 0x755D, 0x7563, 0x7562, 0x753F,
		0x7559, 0x7557, 0x754B, 0x753D, 0x7561, 0x7558, 0x755B, 0x7560, 0x7541, 0x7545,
		0x7552, 0x7569, 0x7548, 0x755A, 0x753C, 0x7547, 0x7567, 0x7542, 0x758A, 0x758B,
		0x758C, 0x758D, 0x0000, 0x758E, 0x094B, 0x094C, 0x094D, 0x094E, 0x094F, 0x0950,
		0x753E, 0x5011, 0x7590, 0x7591, 0x7592, 0x7593, 0x7594, 0x7595, 0x7596, 0x7598,
		0x7599, 0x759B, 0x759C, 0x759E, 0x759F, 0x75A0, 0x75A1, 0x75A3, 0x75A4, 0x75A5,
		0x75A6, 0x75A7, 0x75C0, 0x75C1, 0x75C2, 0x75C3, 0x75C4, 0x75F2, 0x75F3, 0x75F4,
		0x75F5, 0x75F6, 0x75F7, 0x75F8, 0x75F9, 0x75FA, 0x75FB, 0x75FC, 0x75FD, 0x75FE,
		0x75FF, 0x7600, 0x7601, 0x7602, 0x7603, 0x7604, 0x7605, 0x7606, 0x7607, 0x7608,
		0x7609, 0x760A, 0x760B, 0x760C, 0x760D, 0x760E, 0x760F, 0x7610, 0x7611, 0x7612,
		0x7613, 0x7614, 0x7615, 0x75C5, 0x75F6, 0x761B
	};

	const WORD buffIconStart = 0x03E9; //0x03EA ???

	DWORD serial = ReadDWord();

	WORD iconID = ReadWord() - buffIconStart;

	if (iconID < tableCount) //buff
	{
		TGumpBuff *gump = (TGumpBuff*)GumpManager->UpdateGump(serial, 0, GT_BUFF);

		if (gump != NULL)
		{
			WORD mode = ReadWord();

			if (mode)
			{
				Move(12);

				WORD timer = ReadWord();

				Move(3);

				DWORD titleCliloc = ReadDWord();
				DWORD descriptionCliloc = ReadDWord();
				DWORD wtfCliloc = ReadDWord();

				Move(4);

				wstring title = ClilocManager->Cliloc(g_Language)->GetW(titleCliloc).c_str();
				wstring description = L"";
				wstring wtf = L"";

				if (descriptionCliloc)
				{
					//wstring arguments((wchar_t*)Ptr);
					wstring arguments = ReadUnicodeStringLE(0);

					//TPRINT("Buff arguments: %s\n", ToString(arguments).c_str());
					//TPRINT("Buff arguments: %s\n", ToString(ClilocManager->ParseArgumentsToClilocString(descriptionCliloc, arguments)).c_str());

					description = L'\n' + ClilocManager->ParseArgumentsToClilocString(descriptionCliloc, arguments);

					if (description.length() < 2)
						description = L"";
				}

				if (wtfCliloc)
					wtf = L'\n' + ClilocManager->Cliloc(g_Language)->GetW(wtfCliloc).c_str();

				wstring text = L"<left>" + title + description + wtf + L"</left>";

				gump->AddBuff(table[iconID], timer, text);
			}
			else
				gump->DeleteBuff(table[iconID]);
		}
	}
}
//---------------------------------------------------------------------------
