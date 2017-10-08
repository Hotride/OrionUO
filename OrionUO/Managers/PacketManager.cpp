/***********************************************************************************
**
** PacketManager.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CPacketManager g_PacketManager;
//----------------------------------------------------------------------------------
//Карта пакетов УО для анализа
#define UMSG(save, size) { save, "?", size, DIR_BOTH, 0 }
// A message type sent to the server
#define SMSG(save, name, size) { save, name, size, DIR_SEND, 0 }
// A message type received from the server
#define RMSG(save, name, size) { save, name, size, DIR_RECV, 0 }
// A message type transmitted in both directions
#define BMSG(save, name, size) { save, name, size, DIR_BOTH, 0 }
// Message types that have handler methods
#define RMSGH(save, name, size, rmethod) \
{ save, name, size, DIR_RECV, &CPacketManager::Handle ##rmethod }
#define BMSGH(save, name, size, rmethod) \
{ save, name, size, DIR_BOTH, &CPacketManager::Handle ##rmethod }
//----------------------------------------------------------------------------------
CPacketInfo CPacketManager::m_Packets[0x100] =
{
	/*0x00*/ SMSG(ORION_SAVE_PACKET, "Create Character", 0x68),
	/*0x01*/ SMSG(ORION_SAVE_PACKET, "Disconnect", 0x05),
	/*0x02*/ SMSG(ORION_IGNORE_PACKET, "Walk Request", 0x07),
	/*0x03*/ BMSGH(ORION_SAVE_PACKET, "Client Talk", PACKET_VARIABLE_SIZE, ClientTalk),
	/*0x04*/ BMSG(ORION_SAVE_PACKET, "Request God mode (God client)", 0x02),
	/*0x05*/ SMSG(ORION_IGNORE_PACKET, "Attack", 0x05),
	/*0x06*/ SMSG(ORION_IGNORE_PACKET, "Double Click", 0x05),
	/*0x07*/ SMSG(ORION_SAVE_PACKET, "Pick Up Item", 0x07),
	/*0x08*/ SMSG(ORION_SAVE_PACKET, "Drop Item", 0x0e),
	/*0x09*/ SMSG(ORION_IGNORE_PACKET, "Single Click", 0x05),
	/*0x0A*/ BMSG(ORION_SAVE_PACKET, "Edit (God client)", 0x0b),
	/*0x0B*/ RMSGH(ORION_IGNORE_PACKET, "Damage Visualization", 0x07, Damage),
	/*0x0C*/ BMSG(ORION_SAVE_PACKET, "Edit tiledata (God client)", PACKET_VARIABLE_SIZE),
	/*0x0D*/ BMSG(ORION_SAVE_PACKET, "Edit NPC data (God client)", 0x03),
	/*0x0E*/ BMSG(ORION_SAVE_PACKET, "Edit template data (God client)", 0x01),
	/*0x0F*/ UMSG(ORION_SAVE_PACKET, 0x3d),
	/*0x10*/ BMSG(ORION_SAVE_PACKET, "Edit hue data (God client)", 0xd7),
	/*0x11*/ RMSGH(ORION_IGNORE_PACKET, "Character Status", PACKET_VARIABLE_SIZE, CharacterStatus),
	/*0x12*/ SMSG(ORION_IGNORE_PACKET, "Perform Action", PACKET_VARIABLE_SIZE),
	/*0x13*/ SMSG(ORION_IGNORE_PACKET, "Client Equip Item", 0x0a),
	/*0x14*/ BMSG(ORION_SAVE_PACKET, "Change tile Z (God client)", 0x06),
	/*0x15*/ BMSG(ORION_SAVE_PACKET, "Follow", 0x09),
	/*0x16*/ UMSG(ORION_SAVE_PACKET, 0x01),
	/*0x17*/ RMSGH(ORION_IGNORE_PACKET, "Health status bar update (KR)", PACKET_VARIABLE_SIZE, NewHealthbarUpdate),
	/*0x18*/ BMSG(ORION_SAVE_PACKET, "Add script (God client)", PACKET_VARIABLE_SIZE),
	/*0x19*/ BMSG(ORION_SAVE_PACKET, "Edit NPC speech (God client)", PACKET_VARIABLE_SIZE),
	/*0x1A*/ RMSGH(ORION_SAVE_PACKET, "Update Item", PACKET_VARIABLE_SIZE, UpdateItem),
	/*0x1B*/ RMSGH(ORION_SAVE_PACKET, "Enter World", 0x25, EnterWorld),
	/*0x1C*/ RMSGH(ORION_IGNORE_PACKET, "Server Talk", PACKET_VARIABLE_SIZE, Talk),
	/*0x1D*/ RMSGH(ORION_SAVE_PACKET, "Delete Object", 0x05, DeleteObject),
	/*0x1E*/ BMSG(ORION_SAVE_PACKET, "Animate?", 0x04),
	/*0x1F*/ BMSG(ORION_SAVE_PACKET, "Explode?", 0x08),
	/*0x20*/ RMSGH(ORION_SAVE_PACKET, "Update Player", 0x13, UpdatePlayer),
	/*0x21*/ RMSGH(ORION_IGNORE_PACKET, "Deny Walk", 0x08, DenyWalk),
	/*0x22*/ BMSGH(ORION_IGNORE_PACKET, "Confirm Walk", 0x03, ConfirmWalk),
	/*0x23*/ RMSGH(ORION_SAVE_PACKET, "Drag Animation", 0x1a, DragAnimation),
	/*0x24*/ RMSGH(ORION_SAVE_PACKET, "Open Container", 0x07, OpenContainer),
	/*0x25*/ RMSGH(ORION_SAVE_PACKET, "Update Contained Item", 0x14, UpdateContainedItem),
	/*0x26*/ BMSG(ORION_SAVE_PACKET, "Kick client (God client)", 0x05),
	/*0x27*/ RMSGH(ORION_SAVE_PACKET, "Deny Move Item", 0x02, DenyMoveItem),
	/*0x28*/ RMSGH(ORION_SAVE_PACKET, "End dragging item", 0x05, EndDraggingItem),
	/*0x29*/ RMSGH(ORION_SAVE_PACKET, "Drop Item Accepted", 0x01, DropItemAccepted),
	/*0x2A*/ RMSG(ORION_SAVE_PACKET, "Blood mode", 0x05),
	/*0x2B*/ BMSG(ORION_SAVE_PACKET, "Toggle God mode (God client)", 0x02),
	/*0x2C*/ BMSGH(ORION_IGNORE_PACKET, "Death Screen", 0x02, DeathScreen),
	/*0x2D*/ RMSGH(ORION_SAVE_PACKET, "Mobile Attributes", 0x11, MobileAttributes),
	/*0x2E*/ RMSGH(ORION_SAVE_PACKET, "Server Equip Item", 0x0f, EquipItem),
	/*0x2F*/ RMSG(ORION_SAVE_PACKET, "Combat Notification", 0x0a),
	/*0x30*/ RMSG(ORION_SAVE_PACKET, "Attack ok", 0x05),
	/*0x31*/ RMSG(ORION_SAVE_PACKET, "Attack end", 0x01),
	/*0x32*/ BMSG(ORION_SAVE_PACKET, "Toggle hack mover (God client)", 0x02),
	/*0x33*/ RMSGH(ORION_IGNORE_PACKET, "Pause Control", 0x02, PauseControl),
	/*0x34*/ SMSG(ORION_IGNORE_PACKET, "Status Request", 0x0a),
	/*0x35*/ BMSG(ORION_SAVE_PACKET, "Resource type (God client)", 0x28d),
	/*0x36*/ BMSG(ORION_SAVE_PACKET, "Resource tile data (God client)", PACKET_VARIABLE_SIZE),
	/*0x37*/ BMSG(ORION_SAVE_PACKET, "Move object (God client)", 0x08),
	/*0x38*/ BMSG(ORION_SAVE_PACKET, "Pathfinding in Client", 0x07),
	/*0x39*/ BMSG(ORION_SAVE_PACKET, "Remove group (God client)", 0x09),
	/*0x3A*/ BMSGH(ORION_IGNORE_PACKET, "Update Skills", PACKET_VARIABLE_SIZE, UpdateSkills),
	/*0x3B*/ BMSGH(ORION_IGNORE_PACKET, "Vendor Buy Reply", PACKET_VARIABLE_SIZE, BuyReply),
	/*0x3C*/ RMSGH(ORION_SAVE_PACKET, "Update Contained Items", PACKET_VARIABLE_SIZE, UpdateContainedItems),
	/*0x3D*/ BMSG(ORION_SAVE_PACKET, "Ship (God client)", 0x02),
	/*0x3E*/ BMSG(ORION_SAVE_PACKET, "Versions (God client)", 0x25),
	/*0x3F*/ BMSG(ORION_SAVE_PACKET, "Update Statics (God Client)", PACKET_VARIABLE_SIZE),
	/*0x40*/ BMSG(ORION_SAVE_PACKET, "Update terrains (God client)", 0xc9),
	/*0x41*/ BMSG(ORION_SAVE_PACKET, "Update terrains (God client)", PACKET_VARIABLE_SIZE),
	/*0x42*/ BMSG(ORION_SAVE_PACKET, "Update art (God client)", PACKET_VARIABLE_SIZE),
	/*0x43*/ BMSG(ORION_SAVE_PACKET, "Update animation (God client)", 0x229),
	/*0x44*/ BMSG(ORION_SAVE_PACKET, "Update hues (God client)", 0x2c9),
	/*0x45*/ BMSG(ORION_SAVE_PACKET, "Version OK (God client)", 0x05),
	/*0x46*/ BMSG(ORION_SAVE_PACKET, "New art (God client)", PACKET_VARIABLE_SIZE),
	/*0x47*/ BMSG(ORION_SAVE_PACKET, "New terrain (God client)", 0x0b),
	/*0x48*/ BMSG(ORION_SAVE_PACKET, "New animation (God client)", 0x49),
	/*0x49*/ BMSG(ORION_SAVE_PACKET, "New hues (God client)", 0x5d),
	/*0x4A*/ BMSG(ORION_SAVE_PACKET, "Destroy art (God client)", 0x05),
	/*0x4B*/ BMSG(ORION_SAVE_PACKET, "Check version (God client)", 0x09),
	/*0x4C*/ BMSG(ORION_SAVE_PACKET, "Script names (God client)", PACKET_VARIABLE_SIZE),
	/*0x4D*/ BMSG(ORION_SAVE_PACKET, "Edit script (God client)", PACKET_VARIABLE_SIZE),
	/*0x4E*/ RMSGH(ORION_IGNORE_PACKET, "Personal Light Level", 0x06, PersonalLightLevel),
	/*0x4F*/ RMSGH(ORION_IGNORE_PACKET, "Global Light Level", 0x02, LightLevel),
	/*0x50*/ BMSG(ORION_IGNORE_PACKET, "Board header", PACKET_VARIABLE_SIZE),
	/*0x51*/ BMSG(ORION_IGNORE_PACKET, "Board message", PACKET_VARIABLE_SIZE),
	/*0x52*/ BMSG(ORION_IGNORE_PACKET, "Post board message", PACKET_VARIABLE_SIZE),
	/*0x53*/ RMSGH(ORION_SAVE_PACKET, "Error Code", 0x02, ErrorCode),
	/*0x54*/ RMSGH(ORION_SAVE_PACKET, "Sound Effect", 0x0c, PlaySoundEffect),
	/*0x55*/ RMSGH(ORION_IGNORE_PACKET, "Login Complete", 0x01, LoginComplete),
	/*0x56*/ BMSGH(ORION_IGNORE_PACKET, "Map Data", 0x0b, MapData),
	/*0x57*/ BMSG(ORION_SAVE_PACKET, "Update regions (God client)", 0x6e),
	/*0x58*/ BMSG(ORION_SAVE_PACKET, "New region (God client)", 0x6a),
	/*0x59*/ BMSG(ORION_SAVE_PACKET, "New content FX (God client)", PACKET_VARIABLE_SIZE),
	/*0x5A*/ BMSG(ORION_SAVE_PACKET, "Update content FX (God client)", PACKET_VARIABLE_SIZE),
	/*0x5B*/ RMSGH(ORION_IGNORE_PACKET, "Set Time", 0x04, SetTime),
	/*0x5C*/ BMSG(ORION_SAVE_PACKET, "Restart Version", 0x02),
	/*0x5D*/ SMSG(ORION_IGNORE_PACKET, "Select Character", 0x49),
	/*0x5E*/ BMSG(ORION_SAVE_PACKET, "Server list (God client)", PACKET_VARIABLE_SIZE),
	/*0x5F*/ BMSG(ORION_SAVE_PACKET, "Add server (God client)", 0x31),
	/*0x60*/ BMSG(ORION_SAVE_PACKET, "Remove server (God client)", 0x05),
	/*0x61*/ BMSG(ORION_SAVE_PACKET, "Destroy static (God client)", 0x09),
	/*0x62*/ BMSG(ORION_SAVE_PACKET, "Move static (God client)", 0x0f),
	/*0x63*/ BMSG(ORION_SAVE_PACKET, "Area load (God client)", 0x0d),
	/*0x64*/ BMSG(ORION_SAVE_PACKET, "Area load request (God client)", 0x01),
	/*0x65*/ RMSGH(ORION_IGNORE_PACKET, "Set Weather", 0x04, SetWeather),
	/*0x66*/ BMSGH(ORION_IGNORE_PACKET, "Book Page Data", PACKET_VARIABLE_SIZE, BookData),
	/*0x67*/ BMSG(ORION_SAVE_PACKET, "Simped? (God client)", 0x15),
	/*0x68*/ BMSG(ORION_SAVE_PACKET, "Script attach (God client)", PACKET_VARIABLE_SIZE),
	/*0x69*/ BMSG(ORION_SAVE_PACKET, "Friends (God client)", PACKET_VARIABLE_SIZE),
	/*0x6A*/ BMSG(ORION_SAVE_PACKET, "Notify friend (God client)", 0x03),
	/*0x6B*/ BMSG(ORION_SAVE_PACKET, "Key use (God client)", 0x09),
	/*0x6C*/ BMSGH(ORION_IGNORE_PACKET, "Target Data", 0x13, Target),
	/*0x6D*/ RMSGH(ORION_SAVE_PACKET, "Play Music", 0x03, PlayMusic),
	/*0x6E*/ RMSGH(ORION_IGNORE_PACKET, "Character Animation", 0x0e, CharacterAnimation),
	/*0x6F*/ BMSGH(ORION_IGNORE_PACKET, "Secure Trading", PACKET_VARIABLE_SIZE, SecureTrading),
	/*0x70*/ RMSGH(ORION_IGNORE_PACKET, "Graphic Effect", 0x1c, GraphicEffect),
	/*0x71*/ BMSGH(ORION_IGNORE_PACKET, "Bulletin Board Data", PACKET_VARIABLE_SIZE, BulletinBoardData),
	/*0x72*/ BMSGH(ORION_IGNORE_PACKET, "War Mode", 0x05, Warmode),
	/*0x73*/ BMSGH(ORION_IGNORE_PACKET, "Ping", 0x02, Ping),
	/*0x74*/ RMSGH(ORION_IGNORE_PACKET, "Vendor Buy List", PACKET_VARIABLE_SIZE, BuyList),
	/*0x75*/ SMSG(ORION_SAVE_PACKET, "Rename Character", 0x23),
	/*0x76*/ RMSG(ORION_SAVE_PACKET, "New Subserver", 0x10),
	/*0x77*/ RMSGH(ORION_SAVE_PACKET, "Update Character", 0x11, UpdateCharacter),
	/*0x78*/ RMSGH(ORION_SAVE_PACKET, "Update Object", PACKET_VARIABLE_SIZE, UpdateObject),
	/*0x79*/ BMSG(ORION_SAVE_PACKET, "Resource query (God client)", 0x09),
	/*0x7A*/ BMSG(ORION_SAVE_PACKET, "Resource data (God client)", PACKET_VARIABLE_SIZE),
	/*0x7B*/ RMSG(ORION_SAVE_PACKET, "Sequence?", 0x02),
	/*0x7C*/ RMSGH(ORION_IGNORE_PACKET, "Open Menu Gump", PACKET_VARIABLE_SIZE, OpenMenuGump),
	/*0x7D*/ SMSG(ORION_IGNORE_PACKET, "Menu Choice", 0x0d),
	/*0x7E*/ BMSG(ORION_SAVE_PACKET, "God view query (God client)", 0x02),
	/*0x7F*/ BMSG(ORION_SAVE_PACKET, "God view data (God client)", PACKET_VARIABLE_SIZE),
	/*0x80*/ SMSG(ORION_IGNORE_PACKET, "First Login", 0x3e),
	/*0x81*/ RMSG(ORION_SAVE_PACKET, "Change character", PACKET_VARIABLE_SIZE),
	/*0x82*/ RMSGH(ORION_IGNORE_PACKET, "Login Error", 0x02, LoginError),
	/*0x83*/ SMSG(ORION_IGNORE_PACKET, "Delete Character", 0x27),
	/*0x84*/ BMSG(ORION_SAVE_PACKET, "Change password", 0x45),
	/*0x85*/ RMSGH(ORION_IGNORE_PACKET, "Character List Notification", 0x02, CharacterListNotification),
	/*0x86*/ RMSGH(ORION_IGNORE_PACKET, "Resend Character List", PACKET_VARIABLE_SIZE, ResendCharacterList),
	/*0x87*/ BMSG(ORION_SAVE_PACKET, "Send resources (God client)", PACKET_VARIABLE_SIZE),
	/*0x88*/ RMSGH(ORION_IGNORE_PACKET, "Open Paperdoll", 0x42, OpenPaperdoll),
	/*0x89*/ RMSGH(ORION_SAVE_PACKET, "Corpse Equipment", PACKET_VARIABLE_SIZE, CorpseEquipment),
	/*0x8A*/ BMSG(ORION_SAVE_PACKET, "Trigger edit (God client)", PACKET_VARIABLE_SIZE),
	/*0x8B*/ RMSG(ORION_SAVE_PACKET, "Display sign gump", PACKET_VARIABLE_SIZE),
	/*0x8C*/ RMSGH(ORION_IGNORE_PACKET, "Relay Server", 0x0b, RelayServer),
	/*0x8D*/ SMSG(ORION_SAVE_PACKET, "UO3D create character", PACKET_VARIABLE_SIZE),
	/*0x8E*/ BMSG(ORION_SAVE_PACKET, "Move character (God client)", PACKET_VARIABLE_SIZE),
	/*0x8F*/ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE),
	/*0x90*/ RMSGH(ORION_IGNORE_PACKET, "Display Map", 0x13, DisplayMap),
	/*0x91*/ SMSG(ORION_IGNORE_PACKET, "Second Login", 0x41),
	/*0x92*/ BMSG(ORION_SAVE_PACKET, "Update multi data (God client)", PACKET_VARIABLE_SIZE),
	/*0x93*/ RMSGH(ORION_IGNORE_PACKET, "Open Book", 0x63, OpenBook),
	/*0x94*/ BMSG(ORION_SAVE_PACKET, "Update skills data (God client)", PACKET_VARIABLE_SIZE),
	/*0x95*/ BMSGH(ORION_IGNORE_PACKET, "Dye Data", 0x09, DyeData),
	/*0x96*/ BMSG(ORION_SAVE_PACKET, "Game central monitor (God client)", PACKET_VARIABLE_SIZE),
	/*0x97*/ RMSG(ORION_SAVE_PACKET, "Move Player", 0x02),
	/*0x98*/ BMSG(ORION_SAVE_PACKET, "All Names (3D Client Only)", PACKET_VARIABLE_SIZE),
	/*0x99*/ BMSGH(ORION_SAVE_PACKET, "Multi Placement", 0x1a, MultiPlacement),
	/*0x9A*/ BMSGH(ORION_SAVE_PACKET, "ASCII Prompt", PACKET_VARIABLE_SIZE, ASCIIPrompt),
	/*0x9B*/ SMSG(ORION_IGNORE_PACKET, "Help Request", 0x102),
	/*0x9C*/ BMSG(ORION_SAVE_PACKET, "Assistant request (God client)", 0x135),
	/*0x9D*/ BMSG(ORION_SAVE_PACKET, "GM single (God client)", 0x33),
	/*0x9E*/ RMSGH(ORION_IGNORE_PACKET, "Vendor Sell List", PACKET_VARIABLE_SIZE, SellList),
	/*0x9F*/ SMSG(ORION_IGNORE_PACKET, "Vendor Sell Reply", PACKET_VARIABLE_SIZE),
	/*0xA0*/ SMSG(ORION_IGNORE_PACKET, "Select Server", 0x03),
	/*0xA1*/ RMSGH(ORION_IGNORE_PACKET, "Update Hitpoints", 0x09, UpdateHitpoints),
	/*0xA2*/ RMSGH(ORION_IGNORE_PACKET, "Update Mana", 0x09, UpdateMana),
	/*0xA3*/ RMSGH(ORION_IGNORE_PACKET, "Update Stamina", 0x09, UpdateStamina),
	/*0xA4*/ SMSG(ORION_SAVE_PACKET, "System Information", 0x95),
	/*0xA5*/ RMSGH(ORION_SAVE_PACKET, "Open URL", PACKET_VARIABLE_SIZE, OpenUrl),
	/*0xA6*/ RMSGH(ORION_IGNORE_PACKET, "Tip Window", PACKET_VARIABLE_SIZE, TipWindow),
	/*0xA7*/ SMSG(ORION_SAVE_PACKET, "Request Tip", 0x04),
	/*0xA8*/ RMSGH(ORION_IGNORE_PACKET, "Server List", PACKET_VARIABLE_SIZE, ServerList),
	/*0xA9*/ RMSGH(ORION_IGNORE_PACKET, "Character List", PACKET_VARIABLE_SIZE, CharacterList),
	/*0xAA*/ RMSGH(ORION_IGNORE_PACKET, "Attack Reply", 0x05, AttackCharacter),
	/*0xAB*/ RMSGH(ORION_SAVE_PACKET, "Text Entry Dialog", PACKET_VARIABLE_SIZE, TextEntryDialog),
	/*0xAC*/ SMSG(ORION_SAVE_PACKET, "Text Entry Dialog Reply", PACKET_VARIABLE_SIZE),
	/*0xAD*/ SMSG(ORION_IGNORE_PACKET, "Unicode Client Talk", PACKET_VARIABLE_SIZE),
	/*0xAE*/ RMSGH(ORION_IGNORE_PACKET, "Unicode Server Talk", PACKET_VARIABLE_SIZE, UnicodeTalk),
	/*0xAF*/ RMSGH(ORION_SAVE_PACKET, "Display Death", 0x0d, DisplayDeath),
	/*0xB0*/ RMSGH(ORION_IGNORE_PACKET, "Open Dialog Gump", PACKET_VARIABLE_SIZE, OpenGump),
	/*0xB1*/ SMSG(ORION_IGNORE_PACKET, "Dialog Choice", PACKET_VARIABLE_SIZE),
	/*0xB2*/ BMSG(ORION_SAVE_PACKET, "Chat Data", PACKET_VARIABLE_SIZE),
	/*0xB3*/ RMSG(ORION_SAVE_PACKET, "Chat Text ?", PACKET_VARIABLE_SIZE),
	/*0xB4*/ BMSG(ORION_SAVE_PACKET, "Target object list (God client)", PACKET_VARIABLE_SIZE),
	/*0xB5*/ BMSGH(ORION_SAVE_PACKET, "Open Chat Window", 0x40, OpenChat),
	/*0xB6*/ SMSG(ORION_SAVE_PACKET, "Popup Help Request", 0x09),
	/*0xB7*/ RMSG(ORION_SAVE_PACKET, "Popup Help Data", PACKET_VARIABLE_SIZE),
	/*0xB8*/ BMSGH(ORION_IGNORE_PACKET, "Character Profile", PACKET_VARIABLE_SIZE, CharacterProfile),
	/*0xB9*/ RMSGH(ORION_SAVE_PACKET, "Enable locked client features", 0x03, EnableLockedFeatures),
	/*0xBA*/ RMSGH(ORION_IGNORE_PACKET, "Display Quest Arrow", 0x06, DisplayQuestArrow),
	/*0xBB*/ SMSG(ORION_SAVE_PACKET, "Account ID ?", 0x09),
	/*0xBC*/ RMSGH(ORION_IGNORE_PACKET, "Season", 0x03, Season),
	/*0xBD*/ BMSGH(ORION_SAVE_PACKET, "Client Version", PACKET_VARIABLE_SIZE, ClientVersion),
	/*0xBE*/ BMSGH(ORION_SAVE_PACKET, "Assist Version", PACKET_VARIABLE_SIZE, AssistVersion),
	/*0xBF*/ BMSGH(ORION_SAVE_PACKET, "Extended Command", PACKET_VARIABLE_SIZE, ExtendedCommand),
	/*0xC0*/ RMSGH(ORION_IGNORE_PACKET, "Graphical Effect", 0x24, GraphicEffect),
	/*0xC1*/ RMSGH(ORION_IGNORE_PACKET, "Display cliloc String", PACKET_VARIABLE_SIZE, DisplayClilocString),
	/*0xC2*/ BMSGH(ORION_SAVE_PACKET, "Unicode prompt", PACKET_VARIABLE_SIZE, UnicodePrompt),
	/*0xC3*/ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE),
	/*0xC4*/ UMSG(ORION_SAVE_PACKET, 0x06),
	/*0xC5*/ BMSG(ORION_SAVE_PACKET, "Invalid map (God client)", 0xcb),
	/*0xC6*/ RMSG(ORION_SAVE_PACKET, "Invalid map enable", 0x01),
	/*0xC7*/ RMSGH(ORION_IGNORE_PACKET, "Graphical Effect", 0x31, GraphicEffect),
	/*0xC8*/ BMSGH(ORION_SAVE_PACKET, "Client View Range", 0x02, ClientViewRange),
	/*0xC9*/ BMSG(ORION_SAVE_PACKET, "Trip time", 0x06),
	/*0xCA*/ BMSG(ORION_SAVE_PACKET, "UTrip time", 0x06),
	/*0xCB*/ UMSG(ORION_SAVE_PACKET, 0x07),
	/*0xCC*/ RMSGH(ORION_IGNORE_PACKET, "Localized Text Plus String", PACKET_VARIABLE_SIZE, DisplayClilocString),
	/*0xCD*/ UMSG(ORION_SAVE_PACKET, 0x01),
	/*0xCE*/ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE),
	/*0xCF*/ UMSG(ORION_SAVE_PACKET, 0x4e),
	/*0xD0*/ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE),
	/*0xD1*/ RMSGH(ORION_IGNORE_PACKET, "Logout", 0x02, Logout),
	/*0xD2*/ RMSGH(ORION_SAVE_PACKET, "Update Character (New)", 0x19, UpdateCharacter),
	/*0xD3*/ RMSGH(ORION_SAVE_PACKET, "Update Object (New)", PACKET_VARIABLE_SIZE, UpdateObject),
	/*0xD4*/ BMSGH(ORION_IGNORE_PACKET, "Open Book (New)", PACKET_VARIABLE_SIZE, OpenBookNew),
	/*0xD5*/ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE),
	/*0xD6*/ BMSGH(ORION_IGNORE_PACKET, "Mega cliloc", PACKET_VARIABLE_SIZE, MegaCliloc),
	/*0xD7*/ BMSG(ORION_SAVE_PACKET, "+AoS command", PACKET_VARIABLE_SIZE),
	/*0xD8*/ RMSGH(ORION_IGNORE_PACKET, "Custom house", PACKET_VARIABLE_SIZE, CustomHouse),
	/*0xD9*/ SMSG(ORION_SAVE_PACKET, "+Metrics", 0x10c),
	/*0xDA*/ BMSG(ORION_SAVE_PACKET, "Mahjong game command", PACKET_VARIABLE_SIZE),
	/*0xDB*/ RMSG(ORION_SAVE_PACKET, "Character transfer log", PACKET_VARIABLE_SIZE),
	/*0xDC*/ RMSGH(ORION_IGNORE_PACKET, "OPL Info Packet", 9, OPLInfo),
	/*0xDD*/ RMSGH(ORION_IGNORE_PACKET, "Compressed Gump", PACKET_VARIABLE_SIZE, OpenCompressedGump),
	/*0xDE*/ RMSG(ORION_SAVE_PACKET, "Update characters combatants", PACKET_VARIABLE_SIZE),
	/*0xDF*/ RMSGH(ORION_SAVE_PACKET, "Buff/Debuff", PACKET_VARIABLE_SIZE, BuffDebuff),
	/*0xE0*/ SMSG(ORION_SAVE_PACKET, "Bug Report KR", PACKET_VARIABLE_SIZE),
	/*0xE1*/ SMSG(ORION_SAVE_PACKET, "Client Type KR/SA", 0x09),
	/*0xE2*/ RMSGH(ORION_IGNORE_PACKET, "New Character Animation", 0xa, NewCharacterAnimation),
	/*0xE3*/ RMSG(ORION_SAVE_PACKET, "KR Encryption Responce", 0x4d),
	/*0xE4*/ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE),
	/*0xE5*/ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE),
	/*0xE6*/ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE),
	/*0xE7*/ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE),
	/*0xE8*/ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE),
	/*0xE9*/ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE),
	/*0xEA*/ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE),
	/*0xEB*/ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE),
	/*0xEC*/ SMSG(ORION_SAVE_PACKET, "Equip Macro", PACKET_VARIABLE_SIZE),
	/*0xED*/ SMSG(ORION_SAVE_PACKET, "Unequip item macro", PACKET_VARIABLE_SIZE),
	/*0xEE*/ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE),
	/*0xEF*/ SMSG(ORION_SAVE_PACKET, "KR/2D Client Login/Seed", 0x15),
	/*0xF0*/ BMSGH(ORION_SAVE_PACKET, "Krrios client special", PACKET_VARIABLE_SIZE, KrriosClientSpecial),
	/*0xF1*/ SMSG(ORION_SAVE_PACKET, "Client-Server Time Synchronization Request", PACKET_VARIABLE_SIZE),
	/*0xF2*/ RMSG(ORION_SAVE_PACKET, "Client-Server Time Synchronization Response", PACKET_VARIABLE_SIZE),
	/*0xF3*/ RMSGH(ORION_SAVE_PACKET, "Update Item (SA)", 0x18, UpdateItemSA),
	/*0xF4*/ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE),
	/*0xF5*/ RMSGH(ORION_IGNORE_PACKET, "Display New Map", 0x15, DisplayMap),
	/*0xF6*/ RMSG(ORION_SAVE_PACKET, "Boat moving", PACKET_VARIABLE_SIZE),
	/*0xF7*/ RMSG(ORION_SAVE_PACKET, "Packets (0xF3) list", PACKET_VARIABLE_SIZE),
	/*0xF8*/ SMSG(ORION_SAVE_PACKET, "Character Creation (7.0.16.0)", 0x6a),
	/*0xF9*/ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE),
	/*0xFA*/ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE),
	/*0xFB*/ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE),
	/*0xFC*/ BMSGH(ORION_SAVE_PACKET, "Orion messages", PACKET_VARIABLE_SIZE, OrionMessages),
	/*0xFD*/ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE),
	/*0xFE*/ RMSG(ORION_SAVE_PACKET, "Razor Handshake", 0x8),
	/*0xFF*/ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE)
};
//----------------------------------------------------------------------------------
CPacketManager::CPacketManager()
: WISP_NETWORK::CPacketReader()
{
}
//----------------------------------------------------------------------------------
CPacketManager::~CPacketManager()
{
}
//---------------------------------------------------------------------------
bool CPacketManager::AutoLoginNameExists(const string &name)
{
	WISPFUN_DEBUG("c150_f1");
	if (!m_AutoLoginNames.length())
		return false;

	string search = string("|") + name + "|";
	
	return (m_AutoLoginNames.find(search) != string::npos);
}
//----------------------------------------------------------------------------------
#define CV_PRINT 0

#if CV_PRINT!=0
	#define CVPRINT(s) LOG(s)
#else //CV_PRINT==0
	#define CVPRINT(s)
#endif //CV_PRINT!=0
//----------------------------------------------------------------------------------
void CPacketManager::OnClientVersionChange(const CLIENT_VERSION &newClientVersion)
{
	WISPFUN_DEBUG("c150_f2");
	if (newClientVersion >= CV_500A)
	{
		CVPRINT("Set new length for packet 0x0B (>= 5.0.0a)\n");
		m_Packets[0x0B].Size = 0x07;
		CVPRINT("Set new length for packet 0x16 (>= 5.0.0a)\n");
		m_Packets[0x16].Size = PACKET_VARIABLE_SIZE;
		CVPRINT("Set new length for packet 0x31 (>= 5.0.0a)\n");
		m_Packets[0x31].Size = PACKET_VARIABLE_SIZE;
	}
	else
	{
		CVPRINT("Set standart length for packet 0x0B (< 5.0.0a)\n");
		m_Packets[0x0B].Size = 0x10A;
		CVPRINT("Set standart length for packet 0x16 (< 5.0.0a)\n");
		m_Packets[0x16].Size = 0x01;
		CVPRINT("Set standart length for packet 0x31 (< 5.0.0a)\n");
		m_Packets[0x31].Size = 0x01;
	}

	if (newClientVersion >= CV_5090)
	{
		CVPRINT("Set new length for packet 0xE1 (>= 5.0.9.0)\n");
		m_Packets[0xE1].Size = PACKET_VARIABLE_SIZE;
	}
	else
	{
		CVPRINT("Set standart length for packet 0xE1 (<= 5.0.9.0)\n");
		m_Packets[0xE1].Size = 0x09;
	}

	if (newClientVersion >= CV_6013)
	{
		CVPRINT("Set new length for packet 0xE3 (>= 6.0.1.3)\n");
		m_Packets[0xE3].Size = PACKET_VARIABLE_SIZE;
		CVPRINT("Set new length for packet 0xE6 (>= 6.0.1.3)\n");
		m_Packets[0xE6].Size = 0x05;
		CVPRINT("Set new length for packet 0xE7 (>= 6.0.1.3)\n");
		m_Packets[0xE7].Size = 0x0C;
		CVPRINT("Set new length for packet 0xE8 (>= 6.0.1.3)\n");
		m_Packets[0xE8].Size = 0x0D;
		CVPRINT("Set new length for packet 0xE9 (>= 6.0.1.3)\n");
		m_Packets[0xE9].Size = 0x4B;
		CVPRINT("Set new length for packet 0xEA (>= 6.0.1.3)\n");
		m_Packets[0xEA].Size = 0x03;
	}
	else
	{
		CVPRINT("Set standart length for packet 0xE3 (<= 6.0.1.3)\n");
		m_Packets[0xE3].Size = 0x4D;
		CVPRINT("Set standart length for packet 0xE6 (<= 6.0.1.3)\n");
		m_Packets[0xE6].Size = PACKET_VARIABLE_SIZE;
		CVPRINT("Set standart length for packet 0xE7 (<= 6.0.1.3)\n");
		m_Packets[0xE7].Size = PACKET_VARIABLE_SIZE;
		CVPRINT("Set standart length for packet 0xE8 (<= 6.0.1.3)\n");
		m_Packets[0xE8].Size = PACKET_VARIABLE_SIZE;
		CVPRINT("Set standart length for packet 0xE9 (<= 6.0.1.3)\n");
		m_Packets[0xE9].Size = PACKET_VARIABLE_SIZE;
		CVPRINT("Set standart length for packet 0xEA (<= 6.0.1.3)\n");
		m_Packets[0xEA].Size = PACKET_VARIABLE_SIZE;
	}

	if (newClientVersion >= CV_6017)
	{
		CVPRINT("Set new length for packet 0x08 (>= 6.0.1.7)\n");
		m_Packets[0x08].Size = 0x0F;
		CVPRINT("Set new length for packet 0x25 (>= 6.0.1.7)\n");
		m_Packets[0x25].Size = 0x15;
	}
	else
	{
		CVPRINT("Set standart length for packet 0x08 (<= 6.0.1.7)\n");
		m_Packets[0x08].Size = 0x0E;
		CVPRINT("Set standart length for packet 0x25 (<= 6.0.1.7)\n");
		m_Packets[0x25].Size = 0x14;
	}

	if (newClientVersion == CV_6060)
	{
		CVPRINT("Set new length for packet 0xEE (>= 6.0.6.0)\n");
		m_Packets[0xEE].Size = 0x2000;
		CVPRINT("Set new length for packet 0xEF (>= 6.0.6.0)\n");
		m_Packets[0xEF].Size = 0x2000;
		CVPRINT("Set new length for packet 0xF1 (>= 6.0.6.0)\n");
		m_Packets[0xF1].Size = 0x09;
	}
	else
	{
		CVPRINT("Set standart length for packet 0xEE (<= 6.0.6.0)\n");
		m_Packets[0xEE].Size = PACKET_VARIABLE_SIZE;
		CVPRINT("Set standart length for packet 0xEF (<= 6.0.6.0)\n");
		m_Packets[0xEF].Size = 0x15;
		CVPRINT("Set standart length for packet 0xF1 (<= 6.0.6.0)\n");
		m_Packets[0xF1].Size = PACKET_VARIABLE_SIZE;
	}

	if (newClientVersion >= CV_60142)
	{
		CVPRINT("Set new length for packet 0xB9 (>= 6.0.14.2)\n");
		m_Packets[0xB9].Size = 0x05;
	}
	else
	{
		CVPRINT("Set standart length for packet 0xB9 (<= 6.0.14.2)\n");
		m_Packets[0xB9].Size = 0x03;
	}

	if (newClientVersion >= CV_7000)
	{
		CVPRINT("Set new length for packet 0xEE (>= 7.0.0.0)\n");
		m_Packets[0xEE].Size = 0x2000;
		CVPRINT("Set new length for packet 0xEF (>= 7.0.0.0)\n");
		m_Packets[0xEF].Size = 0x2000;
		/*CVPRINT("Set new length for packet 0xF0 (>= 7.0.0.0)\n");
		m_Packets[0xF0].size = 0x2000;
		CVPRINT("Set new length for packet 0xF1 (>= 7.0.0.0)\n");
		m_Packets[0xF1].size = 0x2000;
		CVPRINT("Set new length for packet 0xF2 (>= 7.0.0.0)\n");
		m_Packets[0xF2].size = 0x2000;*/
	}
	else
	{
		CVPRINT("Set standart length for packet 0xEE (<= 7.0.0.0)\n");
		m_Packets[0xEE].Size = PACKET_VARIABLE_SIZE;
		CVPRINT("Set standart length for packet 0xEF (<= 7.0.0.0)\n");
		m_Packets[0xEF].Size = 0x15;
		/*CVPRINT("Set standart length for packet 0xF0 (<= 7.0.0.0)\n");
		m_Packets[0xF0].size = PACKET_VARIABLE_SIZE;
		CVPRINT("Set standart length for packet 0xF1 (<= 7.0.0.0)\n");
		m_Packets[0xF1].size = PACKET_VARIABLE_SIZE;
		CVPRINT("Set standart length for packet 0xF2 (<= 7.0.0.0)\n");
		m_Packets[0xF2].size = PACKET_VARIABLE_SIZE;*/
	}

	if (newClientVersion >= CV_7090)
	{
		CVPRINT("Set new length for packet 0x24 (>= 7.0.9.0)\n");
		m_Packets[0x24].Size = 0x09;
		CVPRINT("Set new length for packet 0x99 (>= 7.0.9.0)\n");
		m_Packets[0x99].Size = 0x1E;
		CVPRINT("Set new length for packet 0xBA (>= 7.0.9.0)\n");
		m_Packets[0xBA].Size = 0x0A;
		CVPRINT("Set new length for packet 0xF3 (>= 7.0.9.0)\n");
		m_Packets[0xF3].Size = 0x1A;

		//В клиенте 7.0.8.2 уже изменено
		CVPRINT("Set new length for packet 0xF1 (>= 7.0.9.0)\n");
		m_Packets[0xF1].Size = 0x09;
		CVPRINT("Set new length for packet 0xF2 (>= 7.0.9.0)\n");
		m_Packets[0xF2].Size = 0x19;
	}
	else
	{
		CVPRINT("Set standart length for packet 0x24 (<= 7.0.9.0)\n");
		m_Packets[0x24].Size = 0x07;
		CVPRINT("Set standart length for packet 0x99 (<= 7.0.9.0)\n");
		m_Packets[0x99].Size = 0x1A;
		CVPRINT("Set standart length for packet 0xBA (<= 7.0.9.0)\n");
		m_Packets[0xBA].Size = 0x06;
		CVPRINT("Set standart length for packet 0xF3 (<= 7.0.9.0)\n");
		m_Packets[0xF3].Size = 0x18;

		//В клиенте 7.0.8.2 уже изменено
		CVPRINT("Set standart length for packet 0xF1 (<= 7.0.9.0)\n");
		m_Packets[0xF1].Size = PACKET_VARIABLE_SIZE;
		CVPRINT("Set standart length for packet 0xF2 (<= 7.0.9.0)\n");
		m_Packets[0xF2].Size = PACKET_VARIABLE_SIZE;
	}

	if (newClientVersion >= CV_70180)
	{
		CVPRINT("Set new length for packet 0x00 (>= 7.0.18.0)\n");
		m_Packets[0x00].Size = 0x6A;
	}
	else
	{
		CVPRINT("Set standart length for packet 0x24 (<= 7.0.18.0)\n");
		m_Packets[0x00].Size = 0x68;
	}
}
//----------------------------------------------------------------------------------
int CPacketManager::GetPacketSize(const UCHAR_LIST &packet, int &offsetToSize)
{
	WISPFUN_DEBUG("c150_f3");
	if (packet.size())
		return m_Packets[packet[0]].Size;

	return 0;
}
//---------------------------------------------------------------------------
void CPacketManager::SendMegaClilocRequests()
{
	WISPFUN_DEBUG("c150_f5");
	if (g_TooltipsEnabled && m_MegaClilocRequests.size())
	{
		if (m_ClientVersion >= CV_500A)
		{
			while (m_MegaClilocRequests.size())
				CPacketMegaClilocRequest(m_MegaClilocRequests).Send();
		}
		else
		{
			IFOR(i, 0, (int)m_MegaClilocRequests.size())
				CPacketMegaClilocRequestOld(m_MegaClilocRequests[i]).Send();

			m_MegaClilocRequests.clear();
		}
	}
}
//----------------------------------------------------------------------------------
void CPacketManager::AddMegaClilocRequest(const uint &serial)
{
	WISPFUN_DEBUG("c150_f6");
	for (const uint item : m_MegaClilocRequests)
	{
		if (item == serial)
			return;
	}

	m_MegaClilocRequests.push_back(serial);
}
//----------------------------------------------------------------------------------
void CPacketManager::OnReadFailed()
{
	WISPFUN_DEBUG("c150_f7");
	LOG("OnReadFailed...Disconnecting...\n");
	g_Orion.DisconnectGump();
	g_Orion.Disconnect();
}
//----------------------------------------------------------------------------------
void CPacketManager::OnPacket()
{
	WISPFUN_DEBUG("c150_f8");
	uint ticks = g_Ticks;
	g_TotalRecvSize += m_Size;

	CPacketInfo &info = m_Packets[*m_Start];

	if (info.save)
	{
		time_t rawtime;
		struct tm timeinfo;
		char buffer[80];

		time(&rawtime);
		localtime_s(&timeinfo, &rawtime);
		strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", &timeinfo);
		LOG("--- ^(%d) r(+%d => %d) %s Server:: %s\n", ticks - g_LastPacketTime, m_Size, g_TotalRecvSize, buffer, info.Name);
		LOG_DUMP(m_Start, m_Size);
	}

	g_LastPacketTime = ticks;

	if (info.Direction != DIR_RECV && info.Direction != DIR_BOTH)
		LOG("message direction invalid: 0x%02X\n", *m_Start);
	else if (g_PluginManager.PacketRecv(m_Start, m_Size))
	{
		if (info.Handler != 0)
		{
			m_Ptr = m_Start + 1;

			if (!info.Size)
				m_Ptr += 2;

			(this->*(info.Handler))();
		}
	}
}
//----------------------------------------------------------------------------------
void CPacketManager::SavePluginReceivePacket(puchar buf, const int &size)
{
	WISPFUN_DEBUG("c150_f9");
	UCHAR_LIST packet(size);

	memcpy(&packet[0], &buf[0], size);

	m_PluginData.push_front(packet);
}
//----------------------------------------------------------------------------------
void CPacketManager::ProcessPluginPackets()
{
	WISPFUN_DEBUG("c150_f10");
	while (!m_PluginData.empty())
	{
		UCHAR_LIST &packet = m_PluginData.back();

		PluginReceiveHandler(&packet[0], packet.size());
		packet.clear();

		m_PluginData.pop_back();
	}
}
//----------------------------------------------------------------------------------
void CPacketManager::PluginReceiveHandler(puchar buf, const int &size)
{
	WISPFUN_DEBUG("c150_f11");
	SetData(buf, size);

	uint ticks = g_Ticks;
	g_TotalRecvSize += m_Size;

	CPacketInfo &info = m_Packets[*m_Start];

	LOG("--- ^(%d) r(+%d => %d) Plugin->Client:: %s\n", ticks - g_LastPacketTime, m_Size, g_TotalRecvSize, info.Name);
	LOG_DUMP(m_Start, m_Size);

	g_LastPacketTime = ticks;

	if (info.Direction != DIR_RECV && info.Direction != DIR_BOTH)
		LOG("message direction invalid: 0x%02X\n", *buf);
	else if (info.Handler != 0)
	{
		m_Ptr = m_Start + 1;

		if (!info.Size)
			m_Ptr += 2;

		(this->*(info.Handler))();
	}
}
//----------------------------------------------------------------------------------
#define PACKET_HANDLER(name) void CPacketManager::Handle ##name ()
//----------------------------------------------------------------------------------
PACKET_HANDLER(LoginError)
{
	WISPFUN_DEBUG("c150_f12");
	if (g_GameState == GS_MAIN_CONNECT || g_GameState == GS_SERVER_CONNECT || g_GameState == GS_GAME_CONNECT)
	{
		g_ConnectionScreen.ConnectionFailed = true;
		g_ConnectionScreen.ErrorCode = ReadUInt8();
		g_ConnectionManager.Disconnect();
	}
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(ServerList)
{
	WISPFUN_DEBUG("c150_f13");
	g_ServerList.Clear();
	Move(1);

	int numServers = ReadUInt16BE();

	if (numServers == 0)
		LOG("Warning!!! Empty server list\n");

	// Calculate expected message size
	int exSize = 6 + numServers * 40;
	if (m_Size != exSize)
		LOG("Warning!!! Server list message size should be %d\n", exSize);

	IFOR(i, 0, numServers)
	{
		ushort id = ReadUInt16BE();
		string name = ReadString(32);
		uchar fullPercent = ReadUInt8();
		uchar timezone = ReadUInt8();
		uchar ip = ReadUInt32LE(); //little-endian!!!

		g_ServerList.AddServer(new CServer(id, name, fullPercent, timezone, ip, i == 0));
	}

	if (numServers && g_MainScreen.m_AutoLogin->Checked)
		g_Orion.ServerSelection(0);
	else
		g_Orion.InitScreen(GS_SERVER);

	g_ServerScreen.UpdateContent();
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(RelayServer)
{
	WISPFUN_DEBUG("c150_f14");
	memset(&g_SelectedCharName[0], 0, sizeof(g_SelectedCharName));
	in_addr addr;
	puint paddr = (puint)m_Ptr;
	Move(4);
	addr.S_un.S_addr = *paddr;
	char relayIP[30] = { 0 };
	memcpy(&relayIP[0], inet_ntoa(addr), 29);
	int relayPort = ReadUInt16BE();
	g_Orion.RelayServer(relayIP, relayPort, m_Ptr);
	g_PacketLoginComplete = false;
	g_CurrentMap = 0;
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(CharacterList)
{
	WISPFUN_DEBUG("c150_f15");
	HandleResendCharacterList();

	uchar locCount = ReadUInt8();

	g_CityList.Clear();

	if (m_ClientVersion >= CV_70130)
	{
		IFOR(i, 0, locCount)
		{
			CCityItemNew *city = new CCityItemNew();

			city->LocationIndex = ReadUInt8();

			city->SetName(ReadString(32));
			city->SetArea(ReadString(32));

			city->X = ReadUInt32BE();
			city->Y = ReadUInt32BE();
			city->Z = ReadUInt32BE();
			city->MapIndex = ReadUInt32BE();
			city->Cliloc = ReadUInt32BE();

			Move(4);

			g_CityList.AddCity(city);
		}
	}
	else
	{
		IFOR(i, 0, locCount)
		{
			CCityItem *city = new CCityItem();

			city->LocationIndex = ReadUInt8();

			city->Name = ReadString(31);
			city->Area = ReadString(31);

			city->InitCity();

			g_CityList.AddCity(city);
		}
	}

	g_ClientFlag = ReadUInt32BE();

	g_CharacterList.OnePerson = (bool)(g_ClientFlag & CLF_ONE_CHARACTER_SLOT);
	//g_SendLogoutNotification = (bool)(g_ClientFlag & LFF_RE);
	g_PopupEnabled = (bool)(g_ClientFlag & CLF_CONTEXT_MENU);
	g_TooltipsEnabled = (bool)((g_ClientFlag & CLF_PALADIN_NECROMANCER_TOOLTIPS) && (g_PacketManager.ClientVersion >= CV_308Z));
	g_PaperdollBooks = (bool)(g_ClientFlag & CLF_PALADIN_NECROMANCER_TOOLTIPS);

	g_CharacterListScreen.UpdateContent();
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(ResendCharacterList)
{
	WISPFUN_DEBUG("c150_f16");
	g_Orion.InitScreen(GS_CHARACTER);

	int numSlots = ReadInt8();
	if (*m_Start == 0x86)
		LOG("/======Resend chars===\n");
	else
		LOG("/======Chars===\n");

	g_CharacterList.Clear();
	g_CharacterList.Count = numSlots;

	int AutoPos = -1;
	bool autoLogin = g_MainScreen.m_AutoLogin->Checked;

	if (numSlots == 0)
		LOG("Warning!!! No slots in character list\n");
	else
	{
		IFOR(i, 0, numSlots)
		{
			g_CharacterList.SetName(i, (char*)Ptr);

			if (autoLogin && AutoPos == -1 && AutoLoginNameExists((char*)Ptr))
				AutoPos = i;

			LOG("%d: %s\n", i, (char*)Ptr);

			Move(60);
		}
	}

	if (autoLogin && numSlots)
	{
		if (AutoPos == -1)
			AutoPos = 0;

		g_CharacterList.Selected = AutoPos;

		if (g_CharacterList.GetName(AutoPos).length())
			g_Orion.CharacterSelection(AutoPos);
	}

	if (*m_Start == 0x86)
		g_CharacterListScreen.UpdateContent();
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(LoginComplete)
{
	WISPFUN_DEBUG("c150_f17");
	g_PacketLoginComplete = true;

	g_Orion.LoginComplete();
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(SetTime)
{
	WISPFUN_DEBUG("c150_f18");
	g_ServerTimeHour = ReadUInt8();
	g_ServerTimeMinute = ReadUInt8();
	g_ServerTimeSecond = ReadUInt8();
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(EnterWorld)
{
	WISPFUN_DEBUG("c150_f19");
	uint serial = ReadUInt32BE();

	if (g_World != NULL)
	{
		LOG("Error!!! Duplicate enter world message\n");

		g_Orion.SaveLocalConfig();
		g_Orion.ClearWorld();
	}

	RELEASE_POINTER(g_World);

	g_CorpseManager.Clear();
	g_Walker.Reset();
	g_ObjectInHand.Clear();
	g_World = new CGameWorld(serial);

	g_UseItemActions.Clear();

	g_Ping = 0;
	g_ClickObject.Clear();
	g_Weather.Reset();
	g_ConsolePrompt = PT_NONE;
	g_MacroPointer = NULL;
	g_Season = ST_SUMMER;
	g_OldSeason = ST_SUMMER;
	g_GlobalScale = 1.0;
	g_PathFinder.BlockMoving = false;
	g_SkillsManager.SkillsTotal = 0.0f;
	g_SkillsManager.SkillsRequested = false;

	Move(4);

	if (strlen(g_SelectedCharName))
		g_Player->Name = g_SelectedCharName;

	g_Player->Graphic = ReadUInt16BE();
	g_Player->OnGraphicChange();

	g_Player->X = ReadUInt16BE();
	g_Player->Y = ReadUInt16BE();
	Move(1);
	g_Player->Z = ReadUInt8();
	uchar dir = ReadUInt8();
	g_Player->Direction = dir;
	g_Player->Flags = m_Start[28];

	g_Player->OffsetX = 0;
	g_Player->OffsetY = 0;
	g_Player->OffsetZ = 0;

	if (g_TooltipsEnabled && !g_Player->ClilocMessage.length())
		AddMegaClilocRequest(g_Player->Serial);

	LOG("Player 0x%08lX entered the world.\n", serial);

	g_MapManager->Init();
	g_MapManager->AddRender(g_Player);

	g_Orion.LoadStartupConfig();

	g_LastSpellIndex = 1;
	g_LastSkillIndex = 1;

	g_Orion.Click(g_PlayerSerial);
	g_Orion.StatusReq(g_PlayerSerial);

	if (m_ClientVersion >= CV_200)
		CPacketGameWindowSize().Send();

	if (g_Player->Dead())
		g_Orion.ChangeSeason(ST_DESOLATION, DEATH_MUSIC_INDEX);
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(UpdateHitpoints)
{
	WISPFUN_DEBUG("c150_f20");
	if (g_World == NULL)
		return;

	uint serial = ReadUInt32BE();

	CGameCharacter *obj = g_World->FindWorldCharacter(serial);

	if (obj == NULL)
		return;

	obj->MaxHits = ReadInt16BE();
	obj->Hits = ReadInt16BE();

	g_GumpManager.UpdateContent(serial, 0, GT_STATUSBAR);
	g_GumpManager.UpdateContent(serial, 0, GT_TARGET_SYSTEM);
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(UpdateMana)
{
	WISPFUN_DEBUG("c150_f21");
	if (g_World == NULL)
		return;

	uint serial = ReadUInt32BE();

	CGameCharacter *obj = g_World->FindWorldCharacter(serial);

	if (obj == NULL)
		return;

	obj->MaxMana = ReadInt16BE();
	obj->Mana = ReadInt16BE();

	g_GumpManager.UpdateContent(serial, 0, GT_STATUSBAR);
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(UpdateStamina)
{
	WISPFUN_DEBUG("c150_f22");
	if (g_World == NULL)
		return;

	uint serial = ReadUInt32BE();

	CGameCharacter *obj = g_World->FindWorldCharacter(serial);

	if (obj == NULL)
		return;

	obj->MaxStam = ReadInt16BE();
	obj->Stam = ReadInt16BE();

	g_GumpManager.UpdateContent(serial, 0, GT_STATUSBAR);
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(MobileAttributes)
{
	WISPFUN_DEBUG("c150_f23");
	if (g_World == NULL)
		return;

	uint serial = ReadUInt32BE();

	CGameCharacter *obj = g_World->FindWorldCharacter(serial);

	if (obj == NULL)
		return;

	obj->MaxHits = ReadInt16BE();
	obj->Hits = ReadInt16BE();

	obj->MaxMana = ReadInt16BE();
	obj->Mana = ReadInt16BE();

	obj->MaxStam = ReadInt16BE();
	obj->Stam = ReadInt16BE();

	g_GumpManager.UpdateContent(serial, 0, GT_STATUSBAR);
	g_GumpManager.UpdateContent(serial, 0, GT_TARGET_SYSTEM);
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(NewHealthbarUpdate)
{
	WISPFUN_DEBUG("c150_f24");
	if (g_World == NULL)
		return;

	uint serial = ReadUInt32BE();

	CGameCharacter *obj = g_World->FindWorldCharacter(serial);

	if (obj == NULL)
		return;

	ushort count = ReadUInt16BE();

	IFOR(i, 0, count)
	{
		ushort type = ReadUInt16BE();
		uchar enable = ReadUInt8(); //enable/disable

		uchar flags = obj->Flags;

		if (type == 1) //Poison, enable as poisonlevel + 1
		{
			uchar poisonFlag = 0x04;

			if (enable)
			{
				if (m_ClientVersion >= CV_7000)
					obj->SA_Poisoned = true;
				else
					flags |= poisonFlag;
			}
			else
			{
				if (m_ClientVersion >= CV_7000)
					obj->SA_Poisoned = false;
				else
					flags &= ~poisonFlag;
			}
		}
		else if (type == 2) //Yellow hits
		{
			if (enable)
				flags |= 0x08;
			else
				flags &= ~0x08;
		}
		else if (type == 3) //Red?
		{
		}

		obj->Flags = flags;
	}
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(UpdatePlayer)
{
	WISPFUN_DEBUG("c150_f25");
	if (g_World == NULL)
		return;

	uint serial = ReadUInt32BE();
	ushort graphic = ReadUInt16BE();
	uchar graphicIncrement = ReadUInt8();
	ushort color = ReadUInt16BE();
	uchar flags = ReadUInt8();
	ushort x = ReadUInt16BE();
	ushort y = ReadUInt16BE();
	ushort serverID = ReadUInt16BE();
	uchar direction = ReadUInt8();
	char z = ReadUInt8();

	LOG("0x%08X 0x%04X %i 0x%04X 0x%02X %i %i %i %i %i\n", serial, graphic, graphicIncrement, color, flags, x, y, serverID, direction, z);
	g_World->UpdatePlayer(serial, graphic, graphicIncrement, color, flags, x, y, serverID, direction, z);
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(CharacterStatus)
{
	WISPFUN_DEBUG("c150_f26");
	if (g_World == NULL)
		return;

	uint serial = ReadUInt32BE();

	CGameCharacter *obj = g_World->FindWorldCharacter(serial);
	if (obj == NULL)
		return;

	string name = ReadString(30);
	obj->Name = name;

	obj->Hits = ReadInt16BE();
	obj->MaxHits = ReadInt16BE();

	obj->CanChangeName = (ReadUInt8() != 0);

	uchar flag = ReadUInt8();

	if (flag > 0)
	{
		obj->Female = (ReadUInt8() != 0); //buf[43];

		if (serial == g_PlayerSerial)
		{
			short newStr = ReadInt16BE();
			short newDex = ReadInt16BE();
			short newInt = ReadInt16BE();

			if (g_ConfigManager.StatReport && g_Player->Str)
			{
				short currentStr = g_Player->Str;
				short currentDex = g_Player->Dex;
				short currentInt = g_Player->Int;

				short deltaStr = newStr - currentStr;
				short deltaDex = newDex - currentDex;
				short deltaInt = newInt - currentInt;

				char str[64] = { 0 };
				if (deltaStr)
				{
					sprintf_s(str, "Your strength has changed by %d.  It is now %d.", deltaStr, newStr);
					g_Orion.CreateTextMessage(TT_SYSTEM, 0, 3, 0x0170, str);
				}

				if (deltaDex)
				{
					sprintf_s(str, "Your dexterity has changed by %d.  It is now %d.", deltaDex, newDex);
					g_Orion.CreateTextMessage(TT_SYSTEM, 0, 3, 0x0170, str);
				}

				if (deltaInt)
				{
					sprintf_s(str, "Your intelligence has changed by %d.  It is now %d.", deltaInt, newInt);
					g_Orion.CreateTextMessage(TT_SYSTEM, 0, 3, 0x0170, str);
				}
			}

			g_Player->Str = newStr;
			g_Player->Dex = newDex;
			g_Player->Int = newInt;

			g_Player->Stam = ReadInt16BE();
			g_Player->MaxStam = ReadInt16BE();
			g_Player->Mana = ReadInt16BE();
			g_Player->MaxMana = ReadInt16BE();
			g_Player->Gold = ReadUInt32BE();
			g_Player->Armor = ReadInt16BE();
			g_Player->Weight = ReadInt16BE(); //+64

			if (flag >= 5)
			{
				g_Player->MaxWeight = ReadInt16BE(); //unpack16(buf + 66);
				uint race = ReadUInt8();

				if (!race)
					race = 1;

				g_Player->Race = (RACE_TYPE)race;
			}
			else
				g_Player->MaxWeight = (g_Player->Str * 4) + 25;

			if (flag >= 3)
			{
				g_Player->StatsCap = ReadUInt16BE();
				g_Player->Followers = ReadUInt8();
				g_Player->MaxFollowers = ReadUInt8();
			}

			if (flag >= 4)
			{
				g_Player->FireResistance = ReadInt16BE();
				g_Player->ColdResistance = ReadInt16BE();
				g_Player->PoisonResistance = ReadInt16BE();
				g_Player->EnergyResistance = ReadInt16BE();
				g_Player->Luck = ReadInt16BE();
				g_Player->MinDamage = ReadInt16BE();
				g_Player->MaxDamage = ReadInt16BE();
				g_Player->TithingPoints = ReadUInt32BE();
			}

			if (flag >= 6)
			{
				g_Player->MaxPhysicalResistance = ReadInt16BE();
				g_Player->MaxFireResistance = ReadInt16BE();
				g_Player->MaxColdResistance = ReadInt16BE();
				g_Player->MaxPoisonResistance = ReadInt16BE();
				g_Player->MaxEnergyResistance = ReadInt16BE();
				g_Player->DefenceChance = ReadInt16BE();
				g_Player->MaxDefenceChance = ReadInt16BE();
				g_Player->AttackChance = ReadInt16BE();
				g_Player->WeaponSpeed = ReadInt16BE();
				g_Player->WeaponDamage = ReadInt16BE();
				g_Player->LowerRegCost = ReadInt16BE();
				g_Player->SpellDamage = ReadInt16BE();
				g_Player->CastRecovery = ReadInt16BE();
				g_Player->CastSpeed = ReadInt16BE();
				g_Player->LowerManaCost = ReadInt16BE();
			}

			if (!g_ConnectionScreen.Completed && g_PacketLoginComplete)
				g_Orion.LoginComplete();
		}
	}

	g_GumpManager.UpdateContent(serial, 0, GT_STATUSBAR);
	g_GumpManager.UpdateContent(serial, 0, GT_TARGET_SYSTEM);
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(UpdateItem)
{
	WISPFUN_DEBUG("c150_f27");
	if (g_World == NULL)
		return;

	UPDATE_GAME_OBJECT_TYPE updateType = UGOT_ITEM;
	uint serial = ReadUInt32BE();
	ushort count = 0;
	uchar graphicIncrement = 0;
	uchar direction = 0;
	ushort color = 0;
	uchar flags = 0;

	if (serial & 0x80000000)
	{
		serial &= 0x7FFFFFFF;
		count = 1;
	}

	ushort graphic = ReadUInt16BE();

	if (g_TheAbyss && (graphic & 0x7FFF) == 0x0E5C)
		return;

	if (graphic & 0x8000)
	{
		graphic &= 0x7FFF;
		graphicIncrement = ReadUInt8();
	}

	if (count)
		count = ReadUInt16BE();
	else
		count++;

	ushort x = ReadUInt16BE();

	if (x & 0x8000)
	{
		x &= 0x7FFF;
		direction = 1;
	}

	ushort y = ReadUInt16BE();

	if (y & 0x8000)
	{
		y &= 0x7FFF;
		color = 1;
	}

	if (y & 0x4000)
	{
		y &= 0x3FFF;
		flags = 1;
	}

	if (direction)
		direction = ReadUInt8();

	char z = ReadUInt8();

	if (color)
		color = ReadUInt16BE();

	if (flags)
		flags = ReadUInt8();

	if (graphic >= 0x4000)
	{
		//graphic += 0xC000;
		//updateType = UGOT_NEW_ITEM;
		updateType = UGOT_MULTI;
	}

	if (serial != g_PlayerSerial)
		g_World->UpdateGameObject(serial, graphic, graphicIncrement, count, x, y, z, direction, color, flags, count, updateType, 1);
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(UpdateItemSA)
{
	WISPFUN_DEBUG("c150_f28");
	if (g_World == NULL)
		return;

	Move(2);
	UPDATE_GAME_OBJECT_TYPE updateType = (UPDATE_GAME_OBJECT_TYPE)ReadUInt8();
	uint serial = ReadUInt32BE();
	ushort graphic = ReadUInt16BE();
	uchar graphicIncrement = ReadUInt8();
	ushort count = ReadUInt16BE();
	ushort unknown = ReadUInt16BE();
	ushort x = ReadUInt16BE();
	ushort y = ReadUInt16BE();
	uchar z = ReadUInt8();
	uchar direction = ReadUInt8();
	ushort color = ReadUInt16BE();
	uchar flags = ReadUInt8();
	ushort unknown2 = ReadUInt16BE();

	g_World->UpdateGameObject(serial, graphic, graphicIncrement, count, x, y, z, direction, color, flags, unknown, updateType, unknown2);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(UpdateObject)
{
	WISPFUN_DEBUG("c150_f29");
	if (g_World == NULL)
		return;

	uint serial = ReadUInt32BE();
	ushort graphic = ReadUInt16BE();
	ushort x = ReadUInt16BE();
	ushort y = ReadUInt16BE();
	uchar z = ReadUInt8();
	uchar direction = ReadUInt8();
	ushort color = ReadUInt16BE();
	uchar flags = ReadUInt8();
	uchar notoriety = ReadUInt8();

	bool isAlreadyExists = (g_World->FindWorldObject(serial) != NULL);

	if (serial == g_PlayerSerial)
	{
		if (g_Player != NULL)
		{
			bool updateStatusbar = (g_Player->Flags != flags);

			g_Player->Graphic = graphic;
			g_Player->OnGraphicChange(1000);
			g_Player->Color = g_ColorManager.FixColor(color);
			g_Player->Flags = flags;

			if (updateStatusbar)
				g_GumpManager.UpdateContent(serial, 0, GT_STATUSBAR);
		}
	}
	else
	{
		g_World->UpdateGameObject(serial, graphic, 0, 0, x, y, z, direction, color, flags, 0, UGOT_ITEM, 1);
	}

	CGameObject *obj = g_World->FindWorldObject(serial);

	if (obj == NULL)
		return;

	obj->ClearNotOpenedItems();

	if (obj->NPC)
	{
		((CGameCharacter*)obj)->Notoriety = notoriety;
		g_GumpManager.UpdateContent(serial, 0, GT_PAPERDOLL);
	}

	if (*m_Start != 0x78)
		Move(6);

	uint itemSerial = ReadUInt32BE();

	while (itemSerial != 0 && !IsEOF())
	{
		ushort itemGraphic = ReadUInt16BE();
		uchar layer = ReadUInt8();
		ushort itemColor = 0;

		if (m_ClientVersion >= CV_70331)
			itemColor = ReadUInt16BE();
		else if (itemGraphic & 0x8000)
		{
			itemGraphic &= 0x7FFF;
			itemColor = ReadUInt16BE();
		}

		CGameItem *item = g_World->GetWorldItem(itemSerial);

		item->MapIndex = g_CurrentMap;

		item->Graphic = itemGraphic;
		item->Color = g_ColorManager.FixColor(itemColor);

		g_World->PutEquipment(item, obj, layer);
		item->OnGraphicChange();

		LOG("\t0x%08X:%04X [%d] %04X\n", item->Serial, item->Graphic, layer, item->Color);

		if (g_TooltipsEnabled && !item->ClilocMessage.length())
			AddMegaClilocRequest(item->Serial);

		g_World->MoveToTop(item);

		itemSerial = ReadUInt32BE();
	}

	if (g_TooltipsEnabled && !obj->ClilocMessage.length())
		AddMegaClilocRequest(obj->Serial);

	if (obj->IsPlayer())
	{
		/*if (oldDead != g_Player->Dead())
		{
			if (g_Player->Dead())
				g_Orion.ChangeSeason(ST_DESOLATION, DEATH_MUSIC_INDEX);
			else
				g_Orion.ChangeSeason(g_OldSeason, g_OldSeasonMusic);
		}

		g_GumpManager.UpdateContent(serial, 0, GT_PAPERDOLL);
		*/

		g_Player->UpdateAbilities();
	}
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(EquipItem)
{
	WISPFUN_DEBUG("c150_f30");
	if (g_World == NULL)
		return;

	uint serial = ReadUInt32BE();

	CGameItem *obj = g_World->GetWorldItem(serial);
	obj->MapIndex = g_CurrentMap;

	if (obj->Graphic && obj->Layer != OL_BACKPACK)
		obj->Clear();

	obj->Graphic = ReadUInt16BE();
	Move(1);
	int layer = ReadUInt8();
	uint cserial = ReadUInt32BE();
	obj->Color = g_ColorManager.FixColor(ReadUInt16BE());

	if (obj->Container != 0xFFFFFFFF)
	{
		g_GumpManager.UpdateContent(obj->Container, 0, GT_CONTAINER);
		g_GumpManager.UpdateContent(obj->Container, 0, GT_PAPERDOLL);
	}

	g_World->PutEquipment(obj, cserial, layer);
	obj->OnGraphicChange();

	if (g_NewTargetSystem.Serial == serial)
		g_NewTargetSystem.Serial = 0;

	if (g_TooltipsEnabled && !obj->ClilocMessage.length())
		AddMegaClilocRequest(obj->Serial);

	if (layer >= OL_BUY_RESTOCK && layer <= OL_SELL)
		obj->Clear();
	else if (layer < OL_MOUNT)
		g_GumpManager.UpdateContent(cserial, 0, GT_PAPERDOLL);

	if (cserial == g_PlayerSerial && (layer == OL_1_HAND || layer == OL_2_HAND))
		g_Player->UpdateAbilities();
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(UpdateContainedItem)
{
	WISPFUN_DEBUG("c150_f31");
	if (g_World == NULL)
		return;

	uint serial = ReadUInt32BE();
	ushort graphic = ReadUInt16BE();
	uchar graphicIncrement = ReadUInt8();
	ushort count = ReadUInt16BE();
	ushort x = ReadUInt16BE();
	ushort y = ReadUInt16BE();

	if (m_ClientVersion >= CV_6017)
		Move(1);

	uint containerSerial = ReadUInt32BE();
	ushort color = ReadUInt16BE();

	g_World->UpdateContainedItem(serial, graphic, graphicIncrement, count, x, y, containerSerial, color);
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(UpdateContainedItems)
{
	WISPFUN_DEBUG("c150_f32");
	if (g_World == NULL)
		return;

	ushort itemsCount = ReadUInt16BE();

	IFOR(i, 0, itemsCount)
	{
		uint serial = ReadUInt32BE();
		ushort graphic = ReadUInt16BE();
		uchar graphicIncrement = ReadUInt8();
		ushort count = ReadUInt16BE();
		ushort x = ReadUInt16BE();
		ushort y = ReadUInt16BE();

		if (m_ClientVersion >= CV_6017)
			Move(1);

		uint containerSerial = ReadUInt32BE();
		ushort color = ReadUInt16BE();

		if (!i)
		{
			CGameObject *container = g_World->FindWorldObject(containerSerial);

			if (container != NULL)
			{
				LOG("Making %08X empty...\n", containerSerial);

				if (container->IsCorpse())
					container->ClearUnequipped();
				else
					container->Clear();

				/*if ((*(int(__thiscall **)(CGameContainer *))((int(__thiscall **)(_DWORD))containerObj1->GameObject.VTable
					+ 12))(containerObj1))
				{
					if (containerObj1->GameObject.Parent)
					{
						if ((*((int(**)(void))containerObj1->GameObject.Parent->VTable + 9))())
						{
							v3 = containerObj1->GameObject.Parent;
							if (v3->field_B8)
							{
								if ((*(int(**)(void))(*(_DWORD *)v3->field_B8 + 380))())
								{
									v4 = containerObj1->GameObject.Parent->field_B8;
									if (v4)
										(**(void(__stdcall ***)(_DWORD))v4)(1);
								}
							}
						}
					}
					sub_5A6FE0(containerObj1);
				}*/
			}
		}
		
		g_World->UpdateContainedItem(serial, graphic, graphicIncrement, count, x, y, containerSerial, color);
	}
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(DenyMoveItem)
{
	WISPFUN_DEBUG("c150_f33");
	if (g_World == NULL)
		return;

	if (g_ObjectInHand.Enabled)
	{
		if (g_World->ObjectToRemove == g_ObjectInHand.Serial)
			g_World->ObjectToRemove = 0;

		if (!g_ObjectInHand.UpdatedInWorld)
		{
			if (!g_ObjectInHand.Layer && g_ObjectInHand.Container && g_ObjectInHand.Container != 0xFFFFFFFF)
			{
				g_World->UpdateContainedItem(g_ObjectInHand.Serial, g_ObjectInHand.Graphic, 0, g_ObjectInHand.TotalCount, g_ObjectInHand.X, g_ObjectInHand.Y, g_ObjectInHand.Container, g_ObjectInHand.Color);

				g_GumpManager.UpdateContent(g_ObjectInHand.Container, 0, GT_CONTAINER);
			}
			else
			{
				CGameItem *obj = g_World->GetWorldItem(g_ObjectInHand.Serial);

				if (obj != NULL)
				{
					obj->Graphic = g_ObjectInHand.Graphic;
					obj->Color = g_ObjectInHand.Color;
					obj->Count = g_ObjectInHand.TotalCount;
					obj->Flags = g_ObjectInHand.Flags;
					obj->X = g_ObjectInHand.X;
					obj->Y = g_ObjectInHand.Y;
					obj->Z = g_ObjectInHand.Z;
					obj->OnGraphicChange();

					CGameObject *container = g_World->FindWorldObject(g_ObjectInHand.Container);

					if (container != NULL)
					{
						if (container->NPC)
						{
							g_World->PutEquipment(obj, container, g_ObjectInHand.Layer);

							g_GumpManager.UpdateContent(obj->Container, 0, GT_PAPERDOLL);
						}
						else
						{
							g_World->RemoveObject(obj);
							obj = NULL;
						}
					}
					else
						g_World->RemoveFromContainer(obj);

					if (g_TooltipsEnabled)
						AddMegaClilocRequest(g_ObjectInHand.Serial);

					if (obj != NULL)
						g_World->MoveToTop(obj);
				}
			}
		}

		g_GumpManager.CloseGump(g_ObjectInHand.Serial, 0, GT_DRAG);

		g_ObjectInHand.Clear();
	}

	uchar code = ReadUInt8();

	if (code < 5)
	{
		const string errorMessages[] =
		{
			"You can not pick that up.",
			"That is too far away.",
			"That is out of sight.",
			"That item does not belong to you.  You'll have to steal it.",
			"You are already holding an item."
		};

		g_Orion.CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, errorMessages[code]);
	}
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(EndDraggingItem)
{
	WISPFUN_DEBUG("c150_f33_1");
	if (g_World == NULL)
		return;

	//Unused
	//Move(2);
	//Move(2);

	g_ObjectInHand.Enabled = false;
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(DropItemAccepted)
{
	WISPFUN_DEBUG("c150_f33_1");
	if (g_World == NULL)
		return;

	g_ObjectInHand.Enabled = false;
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(DeleteObject)
{
	WISPFUN_DEBUG("c150_f34");
	if (g_World == NULL)
		return;

	uint serial = ReadUInt32BE();

	if (serial == g_PlayerSerial)
		return;

	CGameObject *obj = g_World->FindWorldObject(serial);

	if (obj != NULL)
	{
		bool updateAbilities = false;
		uint cont = obj->Container & 0x7FFFFFFF;

		if (obj->Container != 0xFFFFFFFF)
		{
			CGameObject *top = obj->GetTopObject();

			if (top != NULL)
			{
				if (top->IsPlayer())
				{
					CGameItem *item = (CGameItem*)obj;
					updateAbilities = (item->Layer == OL_1_HAND || item->Layer == OL_2_HAND);
				}

				CGameObject *tradeBox = top->FindSecureTradeBox();

				if (tradeBox != NULL)
					g_GumpManager.UpdateContent(0, tradeBox->Serial, GT_TRADE);
			}

			if (cont == g_PlayerSerial && ((CGameItem*)obj)->Layer == OL_NONE)
				g_ObjectInHand.Enabled = false;

			if (!obj->NPC && ((CGameItem*)obj)->Layer != OL_NONE)
				g_GumpManager.UpdateContent(cont, 0, GT_PAPERDOLL);

			CGump *gump = g_GumpManager.UpdateContent(cont, 0, GT_CONTAINER);

			if (obj->Graphic == 0x0EB0)
			{
				g_GumpManager.CloseGump(serial, cont, GT_BULLETIN_BOARD_ITEM);

				CGumpBulletinBoard *bbGump = (CGumpBulletinBoard*)g_GumpManager.UpdateGump(cont, 0, GT_BULLETIN_BOARD);

				if (bbGump != NULL && bbGump->m_HTMLGump != NULL)
				{
					QFOR(go, bbGump->m_HTMLGump->m_Items, CBaseGUI*)
					{
						if (go->Serial == serial)
						{
							bbGump->m_HTMLGump->Delete(go);

							int posY = 0;

							QFOR(go1, bbGump->m_HTMLGump->m_Items, CBaseGUI*)
							{
								if (go1->Type == GOT_BB_OBJECT)
								{
									go1->Y = posY;
									posY += 18;
								}
							}

							bbGump->m_HTMLGump->CalculateDataSize();

							break;
						}
					}
				}
			}
		}

		if (!g_CorpseManager.InList(0, serial))
		{
			if (obj->NPC)
			{
				if (g_Party.Contains(obj->Serial))
				{
					g_GumpManager.UpdateContent(obj->Serial, 0, GT_STATUSBAR);

					obj->RemoveRender();
				}
				else
					g_World->RemoveObject(obj);
			}
			else
			{
				g_World->RemoveObject(obj);

				if (updateAbilities)
					g_Player->UpdateAbilities();
			}
		}
	}
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(UpdateCharacter)
{
	WISPFUN_DEBUG("c150_f35");
	if (g_World == NULL)
		return;

	uint serial = ReadUInt32BE();
	CGameCharacter *obj = g_World->FindWorldCharacter(serial);

	if (obj == NULL)
		return;

	ushort graphic = ReadUInt16BE();
	ushort x = ReadUInt16BE();
	ushort y = ReadUInt16BE();
	char z = ReadUInt8();
	uchar direction = ReadUInt8();
	ushort color = ReadUInt16BE();
	uchar flags = ReadUInt8();
	uchar notoriety = ReadUInt8();

	obj->Notoriety = notoriety;

	if (obj->IsPlayer())
	{
		obj->Flags = flags;
	}
	else
	{
		if (!obj->m_Steps.empty() && obj->Direction == obj->m_Steps.back().Direction)
		{
			CWalkData &wd = obj->m_Steps.back();

			obj->X = wd.X;
			obj->Y = wd.Y;
			obj->Z = wd.Z;
			obj->Direction = wd.Direction;

			obj->m_Steps.clear();
		}

		g_World->UpdateGameObject(serial, graphic, 0, 0, x, y, z, direction, color, flags, 0, UGOT_ITEM, 1);
	}

	g_World->MoveToTop(obj);
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(Warmode)
{
	WISPFUN_DEBUG("c150_f36");
	if (g_World == NULL)
		return;

	g_Player->Warmode = ReadUInt8();

	g_GumpManager.UpdateContent(g_PlayerSerial, 0, GT_STATUSBAR);

	CGumpPaperdoll *gump = (CGumpPaperdoll*)g_GumpManager.GetGump(g_PlayerSerial, 0, GT_PAPERDOLL);

	if (gump != NULL && gump->m_ButtonWarmode != NULL)
	{
		ushort graphic = 0x07E5;

		if (g_Player->Warmode)
			graphic += 3;

		gump->m_ButtonWarmode->Graphic = graphic;
		gump->m_ButtonWarmode->GraphicSelected = graphic + 2;
		gump->m_ButtonWarmode->GraphicPressed = graphic + 1;

		gump->WantRedraw = true;
	}

	g_World->MoveToTop(g_Player);
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(PauseControl)
{
	WISPFUN_DEBUG("c150_f37");
	/*g_ClientPaused = ReadUInt8();

	if (!g_ClientPaused)
	UO->ResumeClient();*/
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(OpenPaperdoll)
{
	WISPFUN_DEBUG("c150_f38");
	if (g_World == NULL)
		return;

	uint serial = ReadUInt32BE();

	CGameCharacter *obj = g_World->FindWorldCharacter(serial);

	string text = ReadString(60);

	uchar flags = ReadUInt8();

	if (obj != NULL)
		obj->PaperdollText = text;

	CGumpPaperdoll *gump = (CGumpPaperdoll*)g_GumpManager.UpdateGump(serial, 0, GT_PAPERDOLL);

	if (gump == NULL)
	{
		gump = new CGumpPaperdoll(serial, 0, 0, false);
		g_GumpManager.AddGump(gump);
	}

	gump->CanLift = (flags & 0x02);
	gump->UpdateDescription(text);
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(ClientVersion)
{
	WISPFUN_DEBUG("c150_f39");
	CPacketClientVersion(g_Orion.ClientVersionText).Send();
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(Ping)
{
	WISPFUN_DEBUG("c150_f40");
	g_Ping = 0;
	g_PingByPacket = g_Ticks - g_PingByPacketSendTime;
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(SetWeather)
{
	WISPFUN_DEBUG("c150_f41");
	g_Weather.Reset();

	uchar type = ReadUInt8();
	g_Weather.Type = type;
	g_Weather.Count = ReadUInt8();

	bool showMessage = (g_Weather.Count > 0);

	if (g_Weather.Count > 70)
		g_Weather.Count = 70;

	g_Weather.Temperature = ReadUInt8();
	g_Weather.Timer = g_Ticks + WEATHER_TIMER;
	g_Weather.Generate();

	switch (type)
	{
		case 0:
		{
			if (showMessage)
				g_Orion.CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "It begins to rain.");

			break;
		}
		case 1:
		{
			if (showMessage)
				g_Orion.CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "A fierce storm approaches.");

			break;
		}
		case 2:
		{
			if (showMessage)
				g_Orion.CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "It begins to snow.");

			break;
		}
		case 3:
		{
			if (showMessage)
				g_Orion.CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "A storm is brewing.");

			break;
		}
		case 0xFE:
		case 0xFF:
		{
			g_Weather.Timer = 0;
			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(PersonalLightLevel)
{
	WISPFUN_DEBUG("c150_f42");
	uint serial = ReadUInt32BE();

	if (serial == g_PlayerSerial)
	{
		uchar level = ReadUInt8();

		if (level > 0x1F)
			level = 0x1F;

		g_PersonalLightLevel = level;
	}
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(LightLevel)
{
	WISPFUN_DEBUG("c150_f43");
	uchar level = ReadUInt8();

	if (level > 0x1F)
		level = 0x1F;

	g_LightLevel = level;
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(EnableLockedFeatures)
{
	WISPFUN_DEBUG("c150_f44");
	if (m_ClientVersion >= CV_60142)
		g_LockedClientFeatures = ReadUInt32BE();
	else
		g_LockedClientFeatures = ReadUInt16BE();

	g_ChatEnabled = (bool)(g_LockedClientFeatures & LFF_T2A);

	g_AnimationManager.UpdateAnimationAddressTable();
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(OpenContainer)
{
	WISPFUN_DEBUG("c150_f45");
	if (g_World == NULL)
		return;

	uint serial = ReadUInt32BE();
	ushort gumpid = ReadUInt16BE();

	CGump *gump = NULL;
	bool addGump = true;

	if (gumpid == 0xFFFF) //Spellbook
	{
		int gameWindowCenterX = (g_ConfigManager.GameWindowX - 4) + g_ConfigManager.GameWindowWidth / 2;
		int gameWindowCenterY = (g_ConfigManager.GameWindowY - 4) + g_ConfigManager.GameWindowHeight / 2;

		int x = gameWindowCenterX - 200;
		int y = gameWindowCenterY - 100;

		if (x < 0)
			x = 0;

		if (y < 0)
			y = 0;

		gump = new CGumpSpellbook(serial, x, y);
		g_Orion.PlaySoundEffect(0x0055);
	}
	else if (gumpid == 0x0030) //Buylist
	{
		g_GumpManager.CloseGump(serial, 0, GT_SHOP);

		CGameCharacter *vendor = g_World->FindWorldCharacter(serial);

		if (vendor != NULL)
		{
			CGumpShop *buyGump = new CGumpShop(serial, true, 150, 5);
			gump = buyGump;
			buyGump->Visible = true;

			IFOR(layer, OL_BUY_RESTOCK, OL_BUY + 1)
			{
				CGameItem *item = vendor->FindLayer(layer);

				if (item == NULL)
				{
					LOG("Buy layer %i not found!\n", layer);
					continue;
				}

				item = (CGameItem*)item->m_Items;

				if (item == NULL)
				{
					LOG("Buy items not found!\n");
					continue;
				}

				bool reverse = (item->X > 1);

				if (reverse)
				{
					while (item != NULL && item->m_Next != NULL)
						item = (CGameItem*)item->m_Next;
				}

				CGUIHTMLGump *htmlGump = buyGump->m_ItemList[0];

				int currentY = 0;

				QFOR(shopItem, htmlGump->m_Items, CBaseGUI*)
				{
					if (shopItem->Type == GOT_SHOPITEM)
						currentY += shopItem->GetSize().Height;
				}

				while (item != NULL)
				{
					CGUIShopItem *shopItem = (CGUIShopItem*)htmlGump->Add(new CGUIShopItem(item->Serial, item->Graphic, item->Color, item->Count, item->Price, item->Name, 0, currentY));
					shopItem->NameFromCliloc = item->NameFromCliloc;

					if (!currentY)
					{
						shopItem->Selected = true;
						shopItem->CreateNameText();
						shopItem->UpdateOffsets();
					}

					currentY += shopItem->GetSize().Height;

					if (reverse)
						item = (CGameItem*)item->m_Prev;
					else
						item = (CGameItem*)item->m_Next;
				}

				htmlGump->CalculateDataSize();
			}
		}
		else
			LOG("Buy vendor not found!\n");
	}
	else //Container
	{
		ushort graphic = 0xFFFF;

		IFOR(i, 0, (int)g_ContainerOffset.size())
		{
			if (gumpid == g_ContainerOffset[i].Gump)
			{
				graphic = i;
				break;
			}
		}

		if (graphic == 0xFFFF)
			return;

		g_ContainerRect.Calculate(gumpid);

		gump = new CGumpContainer(serial, gumpid, g_ContainerRect.X, g_ContainerRect.Y);
		gump->Graphic = graphic;
		g_Orion.ExecuteGump(gumpid);
	}

	if (gump == NULL)
		return;

	gump->ID = gumpid;

	if (gumpid != 0x0030)
	{
		if (g_ContainerStack.size())
		{
			for (deque<CContainerStackItem>::iterator cont = g_ContainerStack.begin(); cont != g_ContainerStack.end(); cont++)
			{
				if (cont->Serial == serial)
				{
					gump->X = cont->X;
					gump->Y = cont->Y;
					gump->Minimized = cont->Minimized;
					gump->MinimizedX = cont->MinimizedX;
					gump->MinimizedY = cont->MinimizedY;
					gump->LockMoving = cont->LockMoving;

					if (gumpid != 0xFFFF)
					{
						if (cont->Minimized)
							gump->Page = 1;
						else
							gump->Page = 2;
					}

					g_ContainerStack.erase(cont);

					break;
				}
			}

			if (g_CheckContainerStackTimer < g_Ticks)
				g_ContainerStack.clear();
		}

		CGameItem *obj = g_World->FindWorldItem(serial);

		if (obj != NULL)
		{
			/*if (gumpid != 0xFFFF)*/ obj->Opened = true;
			if (!obj->IsCorpse())
				g_World->ClearContainer(obj);

			if (gumpid == 0xFFFF)
				((CGumpSpellbook*)gump)->UpdateGraphic(obj->Graphic);
		}
	}

	if (addGump)
		g_GumpManager.AddGump(gump);
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(UpdateSkills)
{
	WISPFUN_DEBUG("c150_f46");
	if (g_World == NULL)
		return;

	uchar type = ReadUInt8();
	bool haveCap = ((type && type <= 0x03) || type == 0xDF);
	bool isSingleUpdate = (type == 0xFF || type == 0xDF);
	LOG("Skill update type %i (cap=%d)\n", type, haveCap);

	if (type == 0xFE)
	{
		int count = ReadInt16BE();
		g_SkillsManager.Clear();

		IFOR(i, 0, count)
		{
			bool haveButton = (ReadUInt8() != 0);
			int nameLength = ReadUInt8();
			g_SkillsManager.Add(CSkill(haveButton, ReadString(nameLength)));
		}

		g_SkillsManager.Sort();

		return;
	}

	CGumpSkills *gump = (CGumpSkills*)g_GumpManager.UpdateGump(g_PlayerSerial, 0, GT_SKILLS);

	if (!isSingleUpdate && (type == 1 || type == 3 || g_SkillsManager.SkillsRequested))
	{
		g_SkillsManager.SkillsRequested = false;

		if (gump == NULL)
		{
			gump = new CGumpSkills(g_PlayerSerial, 0, 0, false);
			g_GumpManager.AddGump(gump);
		}

		gump->Visible = true;
	}

	while (!IsEOF())
	{
		ushort id = ReadUInt16BE();

		if (IsEOF())
			break;

		if (!id && !type)
			break;
		else if (!type || type == 0x02)
			id--;

		ushort baseVal = ReadUInt16BE();
		ushort realVal = ReadUInt16BE();
		uchar lock = ReadUInt8();
		ushort cap = 1000;

		if (haveCap)
			cap = ReadUInt16BE();

		CSkill *skill = g_SkillsManager.Get(id);

		if (id < g_SkillsManager.Count && skill != NULL)
		{
			if (isSingleUpdate)
			{
				float change = (float)(baseVal / 10.0f) - skill->BaseValue;

				if (change)
				{
					char str[128] = { 0 };
					sprintf_s(str, "Your skill in %s has %s by %.1f%%.  It is now %.1f%%.", skill->Name.c_str(), ((change < 0) ? "decreased" : "increased"), change, skill->BaseValue + change);
					//else if (change > 0) sprintf(str, "Your skill in %s has increased by %.1f%%.  It is now %.1f%%.", UO->m_Skills[id].m_Name.c_str(), change, obj->GetSkillBaseValue(id) + change);
					g_Orion.CreateTextMessage(TT_SYSTEM, 0, 3, 0x58, str);
				}
			}

			skill->BaseValue = (float)(baseVal / 10.0f);
			skill->Value = (float)(realVal / 10.0f);
			skill->Cap = (float)(cap / 10.0f);
			skill->Status = lock;

			if (gump != NULL)
				gump->UpdateSkillValue(id);

			/*if (haveCap)
				LOG("Skill %i is %i|%i|%i\n", id, baseVal, realVal, cap);
			else
				LOG("Skill %i is %i|%i\n", id, baseVal, realVal);*/
		}
		//else
		//	LOG("Unknown skill update %d\n", id);

		if (isSingleUpdate)
			break;
	}

	g_SkillsManager.UpdateSkillsSum();

	if (gump != NULL)
		gump->UpdateSkillsSum();

	LOG("Skill(s) updated!\n");
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(ExtendedCommand)
{
	WISPFUN_DEBUG("c150_f47");
	ushort cmd = ReadUInt16BE();

	switch (cmd)
	{
		case 0:
			break;
		case 1: //Initialize Fast Walk Prevention
		{
			IFOR(i, 0, 6)
				g_Player->m_FastWalkStack.SetValue(i, ReadUInt32BE());

			break;
		}
		case 2: //Add key to Fast Walk Stack
		{
			g_Player->m_FastWalkStack.AddValue(ReadUInt32BE());

			break;
		}
		case 4: //Close generic gump
		{
			uint id = ReadUInt32BE();
			uint button = ReadUInt32BE();

			for (CGump *gump = (CGump*)g_GumpManager.m_Items; gump != NULL;)
			{
				CGump *next = (CGump*)gump->m_Next;

				if (gump->GumpType == GT_GENERIC && gump->ID == id)
					((CGumpGeneric*)gump)->SendGumpResponse(button);

				gump = next;
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
			g_Party.ParsePacketData(*this);

			break;
		}
		case 8: //Set cursor / map
		{
			g_Orion.ChangeMap(ReadUInt8());

			break;
		}
		case 0xC: //Close statusbar gump
		{
			uint serial = ReadUInt32BE();
			g_GumpManager.CloseGump(serial, 0, GT_STATUSBAR);

			break;
		}
		case 0x10: //DisplayEquipmentInfo
		{
			uint serial = ReadUInt32BE();
			CGameItem *item = g_World->FindWorldItem(serial);
			wstring str = L"";
			int clilocNum = ReadInt32BE();
			if (clilocNum)
			{
				str = g_ClilocManager.Cliloc(g_Language)->GetW(clilocNum, true);
				g_Orion.CreateUnicodeTextMessage(TT_OBJECT, serial, 0x03, 0x3B2, str);
				if (item != NULL && !item->NPC)
					item->Name = ToString(str);

			}
			str = L"";
			ushort crafterNameLen = 0;
			uint next = ReadUInt32BE();
			if (next == 0xFFFFFFFD)
			{
				crafterNameLen = ReadUInt16BE();
				if (crafterNameLen)
					str = L"Crafted by " + DecodeUTF8(ReadString(crafterNameLen));
			}
			
			if (crafterNameLen != 0)
				next = ReadUInt32BE();
			if (next == 0xFFFFFFFC)
				str += L"[Unidentified";

			// -4 потому что последние 4 байта в пакете 0xFFFFFFFF
			puchar end = m_Start + m_Size - 4;
			uchar count = 0;
			while (m_Ptr < end)
			{
				if (count != 0 || next == 0xFFFFFFFD || next == 0xFFFFFFFC)
					next = ReadInt32BE();
				short charges = ReadInt16BE();
				wstring attrsString = g_ClilocManager.Cliloc(g_Language)->GetW(next, true);
				if (charges == -1)
				{
					if (count > 0)
					{
						str += L"/";
						str += attrsString;
					}
					else
					{
						str += L" [";
						str += attrsString;
					}
				}
				else
				{
					str += L"\n[";
					str += attrsString;
					str += L" : ";
					str += std::to_wstring(charges);
					str += L"]";
					count += 20;
				}
				count++;
			}
			if ((count < 20 && count > 0) || (next == 0xFFFFFFFC && count == 0))
				str += L"]";

			if (str.length())
				g_Orion.CreateUnicodeTextMessage(TT_OBJECT, serial, 0x03, 0x3B2, str);
			//g_Orion.CreateTextMessage(TT_OBJECT, serial, 0x03, 0x3B2, str);
			CPacketMegaClilocRequestOld(serial).Send();
			break;
		}
		case 0x14: //Display Popup/context menu (2D and KR)
		{
			Move(1);
			uchar mode = ReadUInt8();
			uint serial = ReadUInt32BE();
			uchar count = ReadUInt8();

			CGumpPopupMenu *menu = new CGumpPopupMenu(serial, g_MouseManager.Position.X, g_MouseManager.Position.Y);
			menu->Add(new CGUIPage(0));
			int width = 0;
			int height = 20;

			menu->Add(new CGUIAlphaBlending(true, 0.5f));
			CGUIResizepic *resizepic = (CGUIResizepic*)menu->Add(new CGUIResizepic(0, 0x0A3C, 0, 0, 0, 0));
			menu->Add(new CGUIAlphaBlending(false, 0.5f));

			int offsetY = 10;
			bool arrowAdded = false;

			IFOR(i, 0, count)
			{
				uint index = 0;
				uint cliloc = 0;

				if (mode == 2 /*|| m_ClientVersion >= CV_6000*/)
				{
					cliloc = ReadUInt32BE();
					index = ReadUInt16BE() + 1;
				}
				else
				{
					index = ReadUInt16BE() + 1;
					cliloc = ReadUInt16BE() + 3000000;
				}

				wstring str = g_ClilocManager.Cliloc(g_Language)->GetW(cliloc, true);

				ushort flags = ReadUInt16BE();
				ushort color = 0xFFFE;

				if (flags == 0x01)
					color = 0x0386;

				CGUITextEntry *item = new CGUITextEntry(index, color, color, color, 10, offsetY, 0, true, CONTEXT_MENU_FONT);

				if (flags == 0x04)
					Move(2);
				else if (flags == 0x20)
				{
					ushort newColor = ReadUInt16BE() & 0x3FFF;
					item->SetGlobalColor(true, g_ColorManager.Color16To32(newColor), 0xFFFFFFFE, 0xFFFFFFFE);
				}
				else if (flags == 0x02)
				{
					if (!arrowAdded)
					{
						arrowAdded = true;
						menu->Add(new CGUIPage(1));
						menu->Add(new CGUIButton(CGumpPopupMenu::ID_GPM_MAXIMIZE, 0x15E6, 0x15E2, 0x15E2, 20, offsetY));
						menu->Add(new CGUIPage(2));

						height += 20;
					}
				}

				menu->Add(item);

				CEntryText &entry = item->m_Entry;
				entry.SetText(str);
				entry.PrepareToDrawW(CONTEXT_MENU_FONT, color);

				CGLTextTexture &texture = entry.m_Texture;

				menu->Add(new CGUIHitBox(index, 10, offsetY, texture.Width, texture.Height, true));

				offsetY += texture.Height;

				if (!arrowAdded)
				{
					height += texture.Height;

					if (width < texture.Width)
						width = texture.Width;
				}
			}

			width += 20;

			if (height <= 10 || width <= 20)
				delete menu;
			else
			{
				resizepic->Width = width;
				resizepic->Height = height;

				QFOR(item, menu->m_Items, CBaseGUI*)
				{
					if (item->Type == GOT_PAGE && ((CGUIPage*)item)->Index)
						break;
					else if (item->Type == GOT_HITBOX)
						((CGUIHitBox*)item)->Width = width - 20;
				}

				g_GumpManager.AddGump(menu);
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
			uint ID = ReadUInt32BE();
			uint Serial = ReadUInt32BE();

			switch (ID)
			{
				case 1: //Paperdoll
				{
					g_GumpManager.CloseGump(Serial, 0, GT_PAPERDOLL);
					break;
				}
				case 2: //Statusbar
				{
					g_GumpManager.CloseGump(Serial, 0, GT_STATUSBAR);
					break;
				}
				case 8: //Character Profile
				{
					//UO->CloseGump(Serial, 0, GT_PROFILE);
					break;
				}
				case 0xC: //Container
				{
					g_GumpManager.CloseGump(Serial, 0, GT_CONTAINER);
					break;
				}
				default:
					break;
			}

			break;
		}
		case 0x18: //Enable map (diff) patches
		{
			g_MapManager->ApplyPatches(*this);

			break;
		}
		case 0x19: //Extended stats
		{
			Move(1);

			if (ReadUInt32BE() == g_PlayerSerial)
			{
				Move(1);
				uchar state = ReadUInt8();

				g_DrawStatLockers = true;

				g_Player->LockStr = (state >> 4) & 3;
				g_Player->LockDex = (state >> 2) & 3;
				g_Player->LockInt = state & 3;

				CGump *statusbar = g_GumpManager.GetGump(g_PlayerSerial, 0, GT_STATUSBAR);

				if (statusbar != NULL && !statusbar->Minimized)
					statusbar->WantUpdateContent = true;
			}

			break;
		}
		case 0x1B: //New spellbook content
		{
			Move(2);
			uint serial = ReadUInt32BE();

			CGameItem *spellbook = g_World->FindWorldItem(serial);

			if (spellbook == NULL)
			{
				LOG("Where is a spellbook?!?\n");
				return;
			}

			g_World->ClearContainer(spellbook);

			ushort graphic = ReadUInt16BE();
			SPELLBOOK_TYPE bookType = (SPELLBOOK_TYPE)ReadUInt16BE();

			IFOR(j, 0, 2)
			{
				uint spells = 0;

				IFOR(i, 0, 4)
					spells |= (ReadUInt8() << (i * 8));

				IFOR(i, 0, 32)
				{
					if (spells & (1 << i))
					{
						CGameItem *spellItem = new CGameItem();
						spellItem->Graphic = 0x1F2E;
						spellItem->Count = (j * 32) + i + 1;

						spellbook->AddItem(spellItem);
					}
				}
			}

			break;
		}
		case 0x1D:
		{
			//house revision state, server sends this when player comes in range of a custom house
			uint serial = ReadUInt32BE();
			uint revision = ReadUInt32BE();

			CCustomHouse *house = g_CustomHousesManager.Get(serial);
			LOG("Seek house: 0x%08X 0x%08X\n", serial, revision);

			if (house != NULL)
				LOG("House found: 0x%08X 0x%08X\n", house->Serial, house->Revision);

			if (house == NULL || house->Revision != revision)
				CPacketCustomHouseDataReq(serial).Send();
			else if (house != NULL)
				house->Paste(g_World->FindWorldItem(serial));

			break;
		}
		case 0x20:
		{
			uint serial = ReadUInt32BE();
			uchar type = ReadUInt8();
			ushort graphic = ReadUInt16BE();
			ushort x = ReadUInt16BE();
			ushort y = ReadUInt16BE();
			uchar z = ReadUInt8();

			switch (type)
			{
				case CHUT_UPDATE:
				{
					break;
				}
				case CHUT_REMOVE:
				{
					break;
				}
				case CHUT_UPDATE_MULTI_POS:
				{
					break;
				}
				case CHUT_CONSTRUCT_BEGIN:
				{
					if (g_GumpManager.GetGump(0, 0, GT_CUSTOM_HOUSE))
						break;

					CGumpCustomHouse *gump = new CGumpCustomHouse(serial, 50, 50);

					g_GumpManager.AddGump(gump);

					break;
				}
				case CHUT_CONSTRUCT_END:
				{
					g_GumpManager.CloseGump(serial, 0, GT_CUSTOM_HOUSE);
					break;
				}
				default:
					break;
			}

			break;
		}
		case 0x21:
		{
			IFOR(i, 0, 2)
			{
				g_Ability[i] &= 0x7F;
				g_GumpManager.UpdateContent(i, 0, GT_ABILITY);
			}

			break;
		}
		case 0x22:
		{
			if (g_World == NULL)
				return;

			Move(1);
			uint serial = ReadUInt32BE();
			CGameCharacter *character = g_World->FindWorldCharacter(serial);

			if (character != NULL)
			{
				int damage = ReadUInt8();

				CTextData *text = new CTextData();
				text->Unicode = false;
				text->Font = 3;
				text->Serial = serial;
				text->Color = (serial == g_PlayerSerial ? 0x0034 : 0x0021);
				text->Type = TT_OBJECT;
				text->SetText(std::to_string(damage));
				text->GenerateTexture(0);
				text->X = text->m_Texture.Width / 2;
				int height = text->m_Texture.Height;

				CTextData *head = (CTextData*)character->m_DamageTextControl.Last();

				if (head != NULL)
				{
					height += head->Y;

					if (height > 0)
					{
						if (height > 100)
							height = 0;

						text->Y = height;
					}
				}

				character->m_DamageTextControl.Add(text);
				text->Timer = g_Ticks + DAMAGE_TEXT_NORMAL_DELAY;
			}

			break;
		}
		case 0x26:
		{
			uchar val = ReadUInt8();

			if (val > CST_FAST_UNMOUNT_AND_CANT_RUN)
				val = 0;

			g_SpeedMode = (CHARACTER_SPEED_TYPE)val;

			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(DenyWalk)
{
	WISPFUN_DEBUG("c150_f48");
	if (g_Player == NULL)
		return;

	g_Ping = 0;

	uchar sequence = ReadUInt8();
	ushort x = ReadUInt16BE();
	ushort y = ReadUInt16BE();
	uchar direction = ReadUInt8();
	char z = ReadUInt8();

	g_Walker.DenyWalk(sequence, x, y, z);

	g_Player->Direction = direction;

	g_World->MoveToTop(g_Player);
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(ConfirmWalk)
{
	WISPFUN_DEBUG("c150_f49");
	if (g_Player == NULL)
		return;

	uchar sequence = ReadUInt8();

	g_PingByWalk[sequence][1] = g_Ticks;

	if (sequence >= 10 && !(sequence % 10))
	{
		g_Ping = 0;

		IFOR(i, 0, 10)
		{
			int delay = g_PingByWalk[sequence - i][1] - g_PingByWalk[sequence - i][0];

			if (delay > 0)
			{
				if (delay >= 600)
					delay = 0;
				else
					delay--;
			}

			g_Ping += delay;
		}

		g_Ping /= 10;
	}

	//player->SetDirection(newdir);

	uchar newnoto = ReadUInt8() & (~0x40);

	if (!newnoto || newnoto >= 7)
		newnoto = 0x01;

	g_Player->Notoriety = newnoto;

	g_Walker.ConfirmWalk(sequence);

	g_World->MoveToTop(g_Player);
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(OpenUrl)
{
	WISPFUN_DEBUG("c150_f50");
	g_Orion.GoToWebLink(ReadString(0));
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(Target)
{
	WISPFUN_DEBUG("c150_f51");
	g_Target.SetData(*this);

	if (g_PartyHelperTimer > g_Ticks && g_PartyHelperTarget)
	{
		g_Target.SendTargetObject(g_PartyHelperTarget);
		g_PartyHelperTimer = 0;
		g_PartyHelperTarget = 0;
	}
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(Talk)
{
	WISPFUN_DEBUG("c150_f52");
	if (g_World == NULL)
	{
		if (g_GameState == GS_GAME_CONNECT)
		{
			uint serial = ReadUInt32BE();
			ushort graphic = ReadUInt16BE();
			SPEECH_TYPE type = (SPEECH_TYPE)ReadUInt8();
			ushort textColor = ReadUInt16BE();
			ushort font = ReadUInt16BE();

			string name(ReadString(0));
			string str = "";

			if (m_Size > 44)
			{
				m_Ptr = m_Start + 44;
				g_ConnectionScreen.ConnectionFailed = true;
				g_ConnectionScreen.Message = ReadString(0);
			}
		}

		return;
	}

	uint serial = ReadUInt32BE();
	ushort graphic = ReadUInt16BE();
	SPEECH_TYPE type = (SPEECH_TYPE)ReadUInt8();
	ushort textColor = ReadUInt16BE();
	ushort font = ReadUInt16BE();
	string name(ReadString(0));

	if (!serial && !graphic && !type && font == 0xFFFF && textColor == 0xFFFF && ToLowerA(name) == "system")
	{
		uchar sbuffer[0x28] =
		{
			0x03, 0x00, 0x28, 0x20, 0x00, 0x34, 0x00, 0x03, 0xdb, 0x13, 0x14, 0x3f, 0x45, 0x2c, 0x58, 0x0f,
			0x5d, 0x44, 0x2e, 0x50, 0x11, 0xdf, 0x75, 0x5c, 0xe0, 0x3e, 0x71, 0x4f, 0x31, 0x34, 0x05, 0x4e,
			0x18, 0x1e, 0x72, 0x0f, 0x59, 0xad, 0xf5, 0x00
		};

		g_Orion.Send(sbuffer, 0x28);

		return;
	}

	if (font >= g_FontManager.FontCount)
		return;

	string str = "";

	if (m_Size > 44)
	{
		m_Ptr = m_Start + 44;
		str = ReadString(0);
	}

	CGameObject *obj = g_World->FindWorldObject(serial);

	if (type == ST_BROADCAST || /*type == ST_SYSTEM ||*/ serial == 0xFFFFFFFF || !serial || (ToLowerA(name) == "system" && obj == NULL))
		g_Orion.CreateTextMessage(TT_SYSTEM, serial, (uchar)font, textColor, str);
	else
	{
		if (type == ST_EMOTE)
		{
			textColor = g_ConfigManager.EmoteColor;
			str = "*" + str + "*";
		}

		if (obj != NULL)
		{
			obj->YouSeeJournalPrefix = (type == ST_SYSTEM);

			if (!obj->Name.length())
			{
				obj->Name = name;

				if (obj->NPC)
					g_GumpManager.UpdateContent(serial, 0, GT_STATUSBAR);
			}
		}

		g_Orion.CreateTextMessage(TT_OBJECT, serial, (uchar)font, textColor, str);

		if (obj != NULL)
			obj->YouSeeJournalPrefix = false;
	}
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(UnicodeTalk)
{
	WISPFUN_DEBUG("c150_f53");
	if (g_World == NULL)
	{
		if (g_GameState == GS_GAME_CONNECT)
		{
			uint serial = ReadUInt32BE();
			ushort graphic = ReadUInt16BE();
			SPEECH_TYPE type = (SPEECH_TYPE)ReadUInt8();
			ushort textColor = ReadUInt16BE();
			ushort font = ReadUInt16BE();
			uint language = ReadUInt32BE();

			string name(ReadString(0));

			if (m_Size > 48)
			{
				m_Ptr = m_Start + 48;
				g_ConnectionScreen.ConnectionFailed = true;
				g_ConnectionScreen.Message = ToString(ReadWString((m_Size - 48) / 2));
			}
		}

		return;
	}

	uint serial = ReadUInt32BE();
	ushort graphic = ReadUInt16BE();
	SPEECH_TYPE type = (SPEECH_TYPE)ReadUInt8();
	ushort textColor = ReadUInt16BE();
	ushort font = ReadUInt16BE();
	uint language = ReadUInt32BE();
	string name(ReadString(0));

	if (!serial && !graphic && !type && font == 0xFFFF && textColor == 0xFFFF && ToLowerA(name) == "system")
	{
		uchar sbuffer[0x28] =
		{
			0x03, 0x00, 0x28, 0x20, 0x00, 0x34, 0x00, 0x03, 0xdb, 0x13, 0x14, 0x3f, 0x45, 0x2c, 0x58, 0x0f,
			0x5d, 0x44, 0x2e, 0x50, 0x11, 0xdf, 0x75, 0x5c, 0xe0, 0x3e, 0x71, 0x4f, 0x31, 0x34, 0x05, 0x4e,
			0x18, 0x1e, 0x72, 0x0f, 0x59, 0xad, 0xf5, 0x00
		};

		g_Orion.Send(sbuffer, 0x28);

		return;
	}

	wstring str = L"";

	if (m_Size > 48)
	{
		m_Ptr = m_Start + 48;
		str = ReadWString((m_Size - 48) / 2);
	}

	CGameObject *obj = g_World->FindWorldObject(serial);

	if (type == ST_BROADCAST /*|| type == ST_SYSTEM*/ || serial == 0xFFFFFFFF || !serial || (ToLowerA(name) == "system" && obj == NULL))
		g_Orion.CreateUnicodeTextMessage(TT_SYSTEM, serial, (uchar)g_ConfigManager.SpeechFont, textColor, str);
	else
	{
		if (type == ST_EMOTE)
		{
			textColor = g_ConfigManager.EmoteColor;
			str = L"*" + str + L"*";
		}

		if (obj != NULL)
		{
			obj->YouSeeJournalPrefix = (type == ST_SYSTEM);

			if (!obj->Name.length())
			{
				obj->Name = name;

				if (obj->NPC)
					g_GumpManager.UpdateContent(serial, 0, GT_STATUSBAR);
			}
		}

		g_Orion.CreateUnicodeTextMessage(TT_OBJECT, serial, (uchar)g_ConfigManager.SpeechFont, textColor, str);

		if (obj != NULL)
			obj->YouSeeJournalPrefix = false;
	}
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(ClientTalk)
{
	WISPFUN_DEBUG("c150_f54");
	if (!g_AbyssPacket03First)
	{
		bool parse = true;

		if (m_Start[4] == 0x78)
		{
			m_Size = ReadInt16BE(2);
			m_Start += 4;
		}
		else if (m_Start[4] == 0x3C)
		{
			m_Size = ReadInt16BE(2);
			m_Start += 4;
		}
		else if (m_Start[4] == 0x25)
		{
			m_Size = 0x14;
			m_Start += 4;
		}
		else if (m_Start[4] == 0x2E)
		{
			m_Size = 0x0F;
			m_Start += 4;
		}
		else
			parse = false;

		if (parse)
			OnPacket();
	}

	g_AbyssPacket03First = false;
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(MultiPlacement)
{
	WISPFUN_DEBUG("c150_f55");
	if (g_World == NULL)
		return;

	//uint serial = unpack32(buf + 2);
	//ushort graphic = unpack16(buf + 18);

	g_Target.SetMultiData(*this);
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(GraphicEffect)
{
	WISPFUN_DEBUG("c150_f56");
	if (g_World == NULL)
		return;

	uchar type = ReadUInt8();

	if (type > 3)
	{
		if (type == 4 && *m_Start == 0x70)
		{
			Move(8);
			ushort val = ReadInt16BE();

			if (val > SET_TO_BLACK_VERY_FAST)
				val = SET_TO_BLACK_VERY_FAST;

			g_ScreenEffectManager.Use(SEM_SUNSET, (SCREEN_EFFECT_TYPE)val, true);
		}

		return;
	}

	uint sourceSerial = ReadUInt32BE();
	uint destSerial = ReadUInt32BE();
	ushort graphic = ReadUInt16BE();
	short sourceX = ReadInt16BE();
	short sourceY = ReadInt16BE();
	char sourceZ = ReadInt8();
	short destX = ReadInt16BE();
	short destY = ReadInt16BE();
	char destZ = ReadInt8();
	uchar speed = ReadUInt8();
	short duration = (short)ReadUInt8() * 50;
	//what is in 24-25 bytes?
	Move(2);
	uchar fixedDirection = ReadUInt8();
	uchar explode = ReadUInt8();

	uint color = 0;
	uint renderMode = 0;

	if (*m_Start != 0x70)
	{
		//0xC0
		color = ReadUInt32BE();
		renderMode = ReadUInt32BE() % 7;

		if (*m_Start == 0xC7)
		{
			/*
			0000: c7 03 00 13 82 2f 00 00 00 00 37 6a 05 d6 06 47 : ...../....7j...G
			0010: 15 05 d6 06 47 15 09 20 00 00 01 00 00 00 00 00 : ....G.. ........
			0020: 00 00 00 00 13 8d 00 01 00 00 00 13 82 2f 03 00 : ............./..
			0030: 00 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- : .
			*/
		}
	}

	CGameEffect *effect = NULL;
	if (!type) //Moving
	{
		effect = new CGameEffectMoving();

		if (!speed)
			speed++;

		((CGameEffectMoving*)effect)->MoveDelay = 20 / speed;
	}
	else
		effect = new CGameEffect();

	effect->EffectType = (EFFECT_TYPE)type;
	effect->Serial = sourceSerial;
	effect->DestSerial = destSerial;
	effect->Graphic = graphic;

	CGameObject *sourceObject = NULL;

	if (!sourceX && !sourceY)
		sourceObject = g_World->FindWorldObject(sourceSerial);

	if (sourceObject != NULL)
	{
		effect->X = sourceObject->X;
		effect->Y = sourceObject->Y;
		effect->Z = sourceObject->Z;
	}
	else
	{
		effect->X = sourceX;
		effect->Y = sourceY;
		effect->Z = sourceZ;
	}

	effect->DestX = destX;
	effect->DestY = destY;
	effect->DestZ = destZ;

	uint addressAnimData = (uint)g_FileManager.m_AnimdataMul.Start;

	if (addressAnimData)
	{
		PANIM_DATA pad = (PANIM_DATA)(addressAnimData + ((graphic * 68) + 4 * ((graphic / 8) + 1)));

		effect->Speed = pad->FrameInterval * 45;
		//effect->Speed = (pad->FrameInterval - effect->Speed) * 45;
	}
	else
		effect->Speed = speed + 6;

	effect->Duration = g_Ticks + duration;
	effect->FixedDirection = (fixedDirection != 0);
	effect->Explode = (explode != 0);

	effect->Color = (ushort)color;
	effect->RenderMode = renderMode;

	g_EffectManager.AddEffect(effect);
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(DeathScreen)
{
	WISPFUN_DEBUG("c150_f57");
	g_Weather.Reset();
	g_Target.Reset();
	g_DeathScreenTimer = g_Ticks + DEATH_SCREEN_DELAY;
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(PlaySoundEffect)
{
	WISPFUN_DEBUG("c150_f58");
	Move(1);
	ushort index = ReadUInt16BE();
	ushort volume = ReadUInt16BE();
	ushort xCoord = ReadUInt16BE();
	ushort yCoord = ReadUInt16BE();

	int distance = GetDistance(g_Player, WISP_GEOMETRY::CPoint2Di(xCoord, yCoord));
	//LOG("Play sound 0x%04X\n", index);

	g_Orion.PlaySoundEffect(index, g_SoundManager.GetVolumeValue(distance));

}
//----------------------------------------------------------------------------------
PACKET_HANDLER(PlayMusic)
{
	WISPFUN_DEBUG("c150_f59");
	ushort index = ReadUInt16BE();

	//LOG("Play midi music 0x%04X\n", index);
	if (!g_ConfigManager.Music || GetForegroundWindow() != g_OrionWindow.Handle || g_ConfigManager.MusicVolume < 1)
		return;

	g_Orion.PlayMusic(index);
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(DragAnimation)
{
	WISPFUN_DEBUG("c150_f60");
	if (g_World == NULL)
		return;

	ushort graphic = ReadUInt16BE();

	if (graphic == 0x0EED)
		graphic = 0x0EEF;
	else if (graphic == 0x0EEA)
		graphic = 0x0EEC;
	else if (graphic == 0x0EF0)
		graphic = 0x0EF2;

	Move(1);
	ushort color = ReadUInt16BE();
	ushort count = ReadUInt16BE();

	uint sourceSerial = ReadUInt32BE();
	short sourceX = ReadInt16BE();
	short sourceY = ReadInt16BE();
	char sourceZ = ReadInt8();
	uint destSerial = ReadUInt32BE();
	short destX = ReadInt16BE();
	short destY = ReadInt16BE();
	char destZ = ReadInt8();

	uchar speed = 5;

	CGameEffect *effect = NULL;

	if (sourceSerial < 0x40000000) //Игрок/НПС кладет предмет в контейнер
	{
		return;

		effect = new CGameEffectMoving();
		effect->FixedDirection = true;

		((CGameEffectMoving*)effect)->MoveDelay = 20 / speed;

		/*
		23 
		0E 85 
		00 
		00 00 
		00 00 
		01 87 53 C5 
		00 00 
		04 6F 
		A4 
		00 00 00 00 
		17 8B 
		04 6F 
		00



		--- ^(1900) r(+26 => 109303) Server:: Drag Animation
		0000: 23 0E 85 00 00 00 00 00 01 87 53 C5 00 00 04 6F : #.........S....o
		0010: A4 00 00 00 00 17 8B 04 6F 00 -- -- -- -- -- -- : ........o.
		*/
	}
	else //Предмет взяли из контейнера
	{
		effect = new CGameEffectDrag();
	}

	effect->Graphic = graphic;
	effect->Color = color;
	effect->EffectType = EF_DRAG;
	effect->Serial = sourceSerial;
	effect->DestSerial = destSerial;
	effect->X = sourceX;
	effect->Y = sourceY;
	effect->Z = sourceZ;
	effect->DestX = destX;
	effect->DestY = destY;
	effect->DestZ = destZ;
	effect->Duration = g_Ticks + 5000;

	uint addressAnimData = (uint)g_FileManager.m_AnimdataMul.Start;

	if (addressAnimData)
	{
		PANIM_DATA pad = (PANIM_DATA)(addressAnimData + ((graphic * 68) + 4 * ((graphic / 8) + 1)));

		effect->Speed = pad->FrameInterval * 45;
	}
	else
		effect->Speed = speed + 6;

	g_EffectManager.AddEffect(effect);
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(CorpseEquipment)
{
	WISPFUN_DEBUG("c150_f61");
	if (g_World == NULL)
		return;

	uint cserial = ReadUInt32BE();

	puchar end = m_Start + m_Size;

	int layer = ReadUInt8();

	while (layer && m_Ptr < end)
	{
		uint serial = ReadUInt32BE();

		CGameItem *obj = g_World->FindWorldItem(serial);

		if (obj != NULL && obj->Container == cserial)
			g_World->PutEquipment(obj, cserial, layer);

		layer = ReadUInt8();
	}
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(ASCIIPrompt)
{
	WISPFUN_DEBUG("c150_f62");
	if (g_World == NULL)
		return;

	g_ConsolePrompt = PT_ASCII;
	memcpy(&g_LastASCIIPrompt[0], &m_Start[0], 11);
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(UnicodePrompt)
{
	WISPFUN_DEBUG("c150_f63");
	if (g_World == NULL)
		return;

	g_ConsolePrompt = PT_UNICODE;
	memcpy(&g_LastUnicodePrompt[0], &m_Start[0], 11);
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(CharacterAnimation)
{
	WISPFUN_DEBUG("c150_f64");
	if (g_World == NULL)
		return;

	uint serial = ReadUInt32BE();
	CGameCharacter *obj = g_World->FindWorldCharacter(serial);

	if (obj != NULL)
	{
		ushort action = ReadUInt16BE();
		ushort frameCount = ReadUInt16BE();
		frameCount = 0;
		ushort repeatMode = ReadUInt16BE();
		bool frameDirection = (ReadUInt8() == 0); //true - forward, false - backward
		bool repeat = (ReadUInt8() != 0);
		uchar delay = ReadUInt8();
		obj->SetAnimation(obj->GetTrueAnimationGroup(action), delay, (uchar)frameCount, (uchar)repeatMode, repeat, frameDirection);
		obj->AnimationFromServer = true;
	}
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(NewCharacterAnimation)
{
	WISPFUN_DEBUG("c150_f65");
	if (g_World == NULL)
		return;

	uint serial = ReadUInt32BE();
	CGameCharacter *obj = g_World->FindWorldCharacter(serial);

	if (obj != NULL)
	{
		ushort action = ReadUInt16BE();
		ushort frameCount = ReadUInt16BE();
		frameCount = 0;
		uchar delay = ReadUInt8();

		obj->SetAnimation(obj->GetTrueAnimationGroup(action), delay, (uchar)frameCount);
		obj->AnimationFromServer = true;
	}
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(DisplayQuestArrow)
{
	WISPFUN_DEBUG("c150_f66");
	g_QuestArrow.Timer = g_Ticks + 1000;
	g_QuestArrow.Enabled = (ReadUInt8() != 0);
	g_QuestArrow.X = ReadUInt16BE();
	g_QuestArrow.Y = ReadUInt16BE();
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(ClientViewRange)
{
	WISPFUN_DEBUG("c150_f67");
	g_ConfigManager.UpdateRange = ReadUInt8();
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(KrriosClientSpecial)
{
	WISPFUN_DEBUG("c150_f68");
	CPacketRazorAnswer().Send();
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(AssistVersion)
{
	WISPFUN_DEBUG("c150_f69");
	uint version = ReadUInt32BE();

	CPacketAssistVersion(version, g_Orion.ClientVersionText).Send();
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(CharacterListNotification)
{
	WISPFUN_DEBUG("c150_f70");
	g_Orion.InitScreen(GS_DELETE);
	g_ConnectionScreen.Type = CST_CHARACTER_LIST;
	g_ConnectionScreen.ConnectionFailed = true;
	g_ConnectionScreen.ErrorCode = ReadUInt8();
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(ErrorCode)
{
	WISPFUN_DEBUG("c150_f71");
	uchar code = ReadUInt8();

	g_Orion.InitScreen(GS_DELETE);
	g_ConnectionScreen.Type = CST_GAME_LOGIN;
	g_ConnectionScreen.ErrorCode = code;

	if (code > 7)
		g_ConnectionScreen.ErrorCode = 3;
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(AttackCharacter)
{
	WISPFUN_DEBUG("c150_f72");
	g_LastAttackObject = ReadUInt32BE();

	if (g_LastAttackObject != 0 && g_World != NULL)
	{
		CGameCharacter *obj = g_World->FindWorldCharacter(g_LastAttackObject);

		if (obj != NULL && !obj->MaxHits)
			CPacketStatusRequest(g_LastAttackObject).Send();
	}
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(Season)
{
	WISPFUN_DEBUG("c150_f73");
	if (g_World == NULL)
		return;

	uchar season = ReadUInt8();

	if (season > ST_DESOLATION)
		season = 0;

	if ((g_Player->Dead() /*|| g_Season == ST_DESOLATION*/) && (SEASON_TYPE)season != ST_DESOLATION)
		return;

	g_OldSeason = (SEASON_TYPE)season;
	g_OldSeasonMusic = ReadUInt8();

	if (g_Season == ST_DESOLATION)
		g_OldSeasonMusic = DEATH_MUSIC_INDEX;

	g_Orion.ChangeSeason((SEASON_TYPE)season, g_OldSeasonMusic);
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(DisplayDeath)
{
	WISPFUN_DEBUG("c150_f74");
	if (g_World == NULL)
		return;

	uint serial = ReadUInt32BE();
	uint corpseSerial = ReadUInt32BE();
	uint running = ReadUInt32BE();

	CGameCharacter *owner = g_World->FindWorldCharacter(serial);

	if (owner == NULL)
		return;

	serial |= 0x80000000;

	g_World->ReplaceObject(owner, serial);

	if (corpseSerial)
		g_CorpseManager.Add(CCorpse(corpseSerial, serial, owner->Direction, running != 0));

	uchar group = g_AnimationManager.GetDieGroupIndex(owner->Graphic, running != 0);

	owner->SetAnimation(group, 0, 5, 1, false, false);

	/*ushort graphic = owner->Graphic;

	if (graphic >= 150)
	{
		if (graphic >= 200)
		{
			if (graphic >= 400)
				owner->SetAnimation(22 - (running != 0), 0, 5, 1, false, false);
			else
				owner->SetAnimation((running != 0 ? 8 : 12), 0, 5, 1, false, false);
		}
		else
			owner->SetAnimation(8, 0, 5, 1, false, false);
	}
	else
		owner->SetAnimation(3 - (running != 0), 0, 5, 1, false, false);*/
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(OpenChat)
{
	WISPFUN_DEBUG("c150_f75");
	uchar newbuf[4] = { 0xf0, 0x00, 0x04, 0xff };
	g_Orion.Send(newbuf, 4);
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(DisplayClilocString)
{
	WISPFUN_DEBUG("c150_f76");
	if (g_World == NULL)
		return;

	uint serial = ReadUInt32BE();
	ushort graphic = ReadUInt16BE();
	uchar type = ReadUInt8();
	ushort color = ReadUInt16BE();
	ushort font = g_FontManager.UnicodeFontExists((uchar)ReadUInt16BE());
	uint cliloc = ReadUInt32BE();

	uchar flags = 0;

	if (*m_Start == 0xCC)
		flags = ReadUInt8();

	string name = ReadString(30);

	string affix = "";
	if (*m_Start == 0xCC)
		affix = ReadString(0);

	wstring args((wchar_t*)Ptr);
	//wstring args = ReadUnicodeStringLE(0);
	wstring message = g_ClilocManager.ParseArgumentsToClilocString(cliloc, false, args);
	//wstring message = ClilocManager->Cliloc(g_Language)->GetW(cliloc);

	CGameObject *obj = g_World->FindWorldObject(serial);

	if (/*type == ST_BROADCAST || type == ST_SYSTEM ||*/ serial == 0xFFFFFFFF || !serial || (ToLowerA(name) == "system" && obj == NULL))
		g_Orion.CreateUnicodeTextMessage(TT_SYSTEM, serial, (uchar)font, color, message);
	else
	{
		/*if (type == ST_EMOTE)
		{
			color = ConfigManager.EmoteColor;
			str = L"*" + str + L"*";
		}*/

		//if (serial >= 0x40000000) //Только для предметов
		{
			if (obj != NULL)
			{
				if (!obj->Name.length())
				{
					obj->Name = name;

					if (obj->NPC)
						g_GumpManager.UpdateContent(serial, 0, GT_STATUSBAR);
				}
				else //names in journal should always be seen as -> You see: NAME, unless speaking, emoting or else
					obj->YouSeeJournalPrefix = true;
			}
		}

		g_Orion.CreateUnicodeTextMessage(TT_OBJECT, serial, (uchar)font, color, message);
	}
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(MegaCliloc)
{
	WISPFUN_DEBUG("c150_f77");
	if (g_World == NULL)
		return;

	ushort unknown = ReadUInt16BE();

	if (unknown > 1)
		return;

	uint serial = ReadUInt32BE();

	CGameObject *obj = g_World->FindWorldObject(serial);

	if (obj == NULL)
		return;

	Move(2);
	uint testedSerial = ReadUInt32BE();
	wstring message(L"");
	bool coloredStartFont = false;

	CGameItem *container = g_World->FindWorldItem(obj->Container);
	bool inBuyList = false;

	if (container != NULL)
		inBuyList = (container->Layer == OL_BUY || container->Layer == OL_BUY_RESTOCK || container->Layer == OL_SELL);

	if (!inBuyList)
	{
		if (!obj->NPC)
		{
			message = L"<basefont color=\"yellow\">";
			coloredStartFont = true;
		}
		else
		{
			CGameCharacter *gc = obj->GameCharacterPtr();
			coloredStartFont = true;

			switch (gc->Notoriety)
			{
				case NT_INNOCENT:
				{
					message = L"<basefont color=\"cyan\">";
					break;
				}
				case NT_SOMEONE_GRAY:
				case NT_CRIMINAL:
				{
					message = L"<basefont color=\"gray\">";
					break;
				}
				case NT_MURDERER:
				{
					message = L"<basefont color=\"red\">";
					break;
				}
				case NT_INVULNERABLE:
				{
					message = L"<basefont color=\"yellow\">";
					break;
				}
				default:
				{
					coloredStartFont = false;
					break;
				}
			}
		}
	}

	puchar end = m_Start + m_Size;
	bool first = true;

	while (m_Ptr < end)
	{
		uint cliloc = ReadUInt32BE();

		if (!cliloc)
			break;

		short len = ReadInt16BE();

		wstring argument = L"";

		if (len > 0)
		{
			argument = wstring((wchar_t*)Ptr, len / 2);
			Ptr += len;
			//wstring argument = ReadUnicodeStringLE(len / 2);
		}

		wstring str = g_ClilocManager.ParseArgumentsToClilocString(cliloc, true, argument);
		//LOG("Cliloc: argstr=%s\n", ToString(str).c_str());

		if (message.length() && !first)
			message += L"\n";

		message += str;

		if (first)
		{
			if (coloredStartFont)
				message += L"<basefont color=\"#FFFFFFFF\">";

			if (!obj->NPC)
				obj->Name = ToString(str);

			first = false;
		}

		//LOG("Cliloc: 0x%08X len=%i arg=%s\n", cliloc, len, ToString(argument).c_str());
	}

	//LOG_DUMP((PBYTE)message.c_str(), message.length() * 2);
	obj->ClilocMessage = message;

	if (g_ToolTip.m_Object == obj)
		g_ToolTip.Reset();

	//LOG("message=%s\n", ToString(message).c_str());

	if (inBuyList && container->Serial)
	{
		CGumpShop *gump = (CGumpShop*)g_GumpManager.GetGump(container->Serial, 0, GT_SHOP);

		if (gump != NULL)
		{
			CGUIHTMLGump *htmlGump = gump->m_ItemList[0];

			QFOR(shopItem, htmlGump->m_Items, CBaseGUI*)
			{
				if (shopItem->Type == GOT_SHOPITEM && shopItem->Serial == serial && ((CGUIShopItem*)shopItem)->NameFromCliloc)
				{
					size_t pos = message.find_first_of(L'\n');

					if (pos != wstring::npos)
						message.resize(pos);

					CGUIShopItem *si = (CGUIShopItem*)shopItem;

					int oldHeight = si->GetSize().Height;

					si->Name = Trim(ToString(message));
					si->CreateNameText();
					si->UpdateOffsets();

					int delta = si->GetSize().Height - oldHeight;

					if (delta && shopItem->m_Next != NULL)
					{
						QFOR(shopItem2, shopItem->m_Next, CBaseGUI*)
						{
							if (shopItem2->Type == GOT_SHOPITEM)
								((CGUIShopItem*)shopItem2)->Y = ((CGUIShopItem*)shopItem2)->Y + delta;
						}
					}

					break;
				}
			}

			htmlGump->CalculateDataSize();
		}
	}
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(Damage)
{
	WISPFUN_DEBUG("c150_f78");
	if (g_World == NULL)
		return;

	uint serial = ReadUInt32BE();
	CGameCharacter *character = g_World->FindWorldCharacter(serial);

	if (character != NULL)
	{
		int damage = ReadUInt16BE();

		CTextData *text = new CTextData();
		text->Unicode = false;
		text->Font = 3;
		text->Serial = serial;
		text->Color = (serial == g_PlayerSerial ? 0x0034 : 0x0021);
		text->Type = TT_OBJECT;
		text->SetText(std::to_string(damage));
		text->GenerateTexture(0);
		text->X = text->m_Texture.Width / 2;
		int height = text->m_Texture.Height;

		CTextData *head = (CTextData*)character->m_DamageTextControl.Last();

		if (head != NULL)
		{
			height += head->Y;

			if (height > 0)
			{
				if (height > 100)
					height = 0;

				text->Y = height;
			}
		}

		character->m_DamageTextControl.Add(text);
		text->Timer = g_Ticks + DAMAGE_TEXT_NORMAL_DELAY;
	}
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(BuffDebuff)
{
	WISPFUN_DEBUG("c150_f79");
	if (g_World == NULL)
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

	static const ushort table[tableCount] =
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

	const ushort buffIconStart = 0x03E9; //0x03EA ???

	uint serial = ReadUInt32BE();

	ushort iconID = ReadUInt16BE() - buffIconStart;

	if (iconID < tableCount) //buff
	{
		CGumpBuff *gump = (CGumpBuff*)g_GumpManager.UpdateGump(serial, 0, GT_BUFF);

		if (gump != NULL)
		{
			ushort mode = ReadUInt16BE();

			if (mode)
			{
				Move(12);

				ushort timer = ReadUInt16BE();

				Move(3);

				uint titleCliloc = ReadUInt32BE();
				uint descriptionCliloc = ReadUInt32BE();
				uint wtfCliloc = ReadUInt32BE();

				Move(4);

				wstring title = g_ClilocManager.Cliloc(g_Language)->GetW(titleCliloc, true).c_str();
				wstring description = L"";
				wstring wtf = L"";

				if (descriptionCliloc)
				{
					//wstring arguments((wchar_t*)Ptr);
					wstring arguments = ReadWString(0, false);

					//LOG("Buff arguments: %s\n", ToString(arguments).c_str());
					//LOG("Buff arguments: %s\n", ToString(ClilocManager->ParseArgumentsToClilocString(descriptionCliloc, arguments)).c_str());
					

					description = L'\n' + g_ClilocManager.ParseArgumentsToClilocString(descriptionCliloc, true, arguments);

					if (description.length() < 2)
						description = L"";
				}

				if (wtfCliloc)
					wtf = L'\n' + g_ClilocManager.Cliloc(g_Language)->GetW(wtfCliloc, true).c_str();

				wstring text = L"<left>" + title + description + wtf + L"</left>";

				gump->AddBuff(table[iconID], timer, text);
			}
			else
				gump->DeleteBuff(table[iconID]);
		}
	}
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(SecureTrading)
{
	WISPFUN_DEBUG("c150_f80");
	if (g_World == NULL)
		return;

	uchar type = ReadUInt8();
	uint serial = ReadUInt32BE();

	if (type == 0) //Новое трэйд окно
	{
		uint id1 = ReadUInt32BE();
		uint id2 = ReadUInt32BE();
		uchar hasName = ReadUInt8();

		CGumpSecureTrading *gump = new CGumpSecureTrading(id1, 0, 0, id1, id2);

		CGameObject *obj = g_World->FindWorldObject(id1);

		if (obj != NULL)
		{
			obj = obj->GetTopObject()->FindSecureTradeBox();

			if (obj != NULL)
				obj->Clear();
		}

		obj = g_World->FindWorldObject(id2);

		if (obj != NULL)
		{
			obj = obj->GetTopObject()->FindSecureTradeBox();

			if (obj != NULL)
				obj->Clear();
		}

		if (hasName && *m_Ptr)
			gump->Text = ReadString(0);

		g_GumpManager.AddGump(gump);
	}
	else if (type == 1) //Отмена
		g_GumpManager.CloseGump(serial, 0, GT_TRADE);
	else if (type == 2) //Обновление
	{
		CGumpSecureTrading *gump = (CGumpSecureTrading*)g_GumpManager.UpdateGump(serial, 0, GT_TRADE);

		if (gump != NULL)
		{
			uint id1 = ReadUInt32BE();
			uint id2 = ReadUInt32BE();

			gump->StateMy = (id1 != 0);
			gump->StateOpponent = (id2 != 0);
		}
	}
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(TextEntryDialog)
{
	WISPFUN_DEBUG("c150_f81");
	if (g_World == NULL)
		return;

	uint serial = ReadUInt32BE();
	uchar parentID = ReadUInt8();
	uchar buttonID = ReadUInt8();

	short textLen = ReadInt16BE();
	string text = ReadString(textLen);

	bool haveCancel = !ReadUInt8();
	uchar variant = ReadUInt8();
	int maxLength = ReadUInt32BE();

	short descLen = ReadInt16BE();
	string desc = ReadString(descLen);

	CGumpTextEntryDialog *gump = new CGumpTextEntryDialog(serial, 143, 172, variant, maxLength, text, desc);
	gump->NoClose = haveCancel;
	gump->ParentID = parentID;
	gump->ButtonID = buttonID;

	g_GumpManager.AddGump(gump);
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(OpenMenuGump)
{
	WISPFUN_DEBUG("c150_f82");
	if (g_World == NULL)
		return;

	uint serial = ReadUInt32BE();
	uint id = ReadUInt16BE();

	uchar nameLen = ReadUInt8();
	string name = ReadString(nameLen);

	uchar count = ReadUInt8();

	if (unpack16(m_Ptr)) //menu
	{
		CGumpMenu *gump = new CGumpMenu(serial, id, 0, 0);

		gump->Add(new CGUIGumppic(0x0910, 0, 0));

		gump->Add(new CGUIColoredPolygone(0, 0, 40, 42, 217, 49, 0xFF000001));

		CGUIText *text = (CGUIText*)gump->Add(new CGUIText(0x0386, 39, 18));
		text->CreateTextureA(1, name, 200, TS_LEFT, UOFONT_FIXED);

		CGUIHTMLGump *htmlGump = (CGUIHTMLGump*)gump->Add(new CGUIHTMLGump(CGumpMenu::ID_GM_HTMLGUMP, 0, 40, 42, 217, 49, false, true));
		htmlGump->Initalize(true);

		htmlGump->Add(new CGUIShader(g_ColorizerShader, true));

		int posX = 0;

		IFOR(i, 0, count)
		{
			ushort graphic = ReadUInt16BE();
			ushort color = ReadUInt16BE();

			nameLen = ReadUInt8();
			name = ReadString(nameLen);

			WISP_GEOMETRY::CSize size = g_Orion.GetArtDimension(graphic, true);

			if (size.Width && size.Height)
			{
				int posY = size.Height;

				if (posY >= 47)
					posY = 0;
				else
					posY = ((47 - posY) / 2);

				CGUIMenuObject *menuObject = (CGUIMenuObject*)htmlGump->Add(new CGUIMenuObject(i + 1, graphic, color, posX, posY, name));

				posX += size.Width;
			}
		}

		htmlGump->Add(new CGUIShader(g_ColorizerShader, false));

		htmlGump->CalculateDataSize();

		gump->m_TextObject = (CGUIText*)gump->Add(new CGUIText(0x0386, 42, 105));
		//gump->m_TextObject->CreateTextureA(1, name, 200, TS_LEFT, UOFONT_FIXED); //На данный момнт создавать нечего

		g_GumpManager.AddGump(gump);
	}
	else //gray menu
	{
		int x = (g_OrionWindow.Size.Width / 2) - 200;
		int y = (g_OrionWindow.Size.Height / 2) - ((121 + (count * 21)) / 2);

		CGumpGrayMenu *gump = new CGumpGrayMenu(serial, id, x, y);

		CGUIResizepic *background = (CGUIResizepic*)gump->Add(new CGUIResizepic(0, 0x13EC, 0, 0, 400, 11111));

		CGUIText *text = (CGUIText*)gump->Add(new CGUIText(0x0386, 20, 16));
		text->CreateTextureA(1, name);

		int offsetY = 51;
		int gumpHeight = 121;

		IFOR(i, 0, count)
		{
			Move(4);

			nameLen = ReadUInt8();
			name = ReadString(nameLen);

			gump->Add(new CGUIRadio(i + 1, 0x138A, 0x138B, 0x138A, 20, offsetY)); //Button

			offsetY += 2;

			text = (CGUIText*)gump->Add(new CGUIText(0x0386, 50, offsetY));
			text->CreateTextureA(1, name, 340);

			int addHeight = text->m_Texture.Height;

			if (addHeight < 21)
				addHeight = 21;

			offsetY += addHeight;
			gumpHeight += addHeight;
		}

		offsetY += 5;

		gump->Add(new CGUIButton(CGumpGrayMenu::ID_GGM_CANCEL, 0x1450, 0x1450, 0x1451, 70, offsetY)); //CANCEL
		gump->Add(new CGUIButton(CGumpGrayMenu::ID_GGM_CONTINUE, 0x13B2, 0x13B2, 0x13B3, 200, offsetY)); //CONTINUE

		background->Height = gumpHeight;

		g_GumpManager.AddGump(gump);
	}
}
//----------------------------------------------------------------------------------
void CPacketManager::AddHTMLGumps(CGump *gump, vector<HTMLGumpDataInfo> &list)
{
	WISPFUN_DEBUG("c150_f83");
	IFOR(i, 0, (int)list.size())
	{
		HTMLGumpDataInfo &data = list[i];

		CGUIHTMLGump *htmlGump = (CGUIHTMLGump*)gump->Add(new CGUIHTMLGump(data.TextID + 1, 0x0BB8, data.X, data.Y, data.Width, data.Height, data.HaveBackground, data.HaveScrollbar));
		htmlGump->DrawOnly = (data.HaveScrollbar == 0);

		int width = htmlGump->Width;

		if (data.HaveScrollbar)
			width -= 16;

		uint htmlColor = 0xFFFFFFFF;
		ushort color = 0;

		if (data.Color)
		{
			if (data.Color == 0x00FFFFFF)
				htmlColor = 0xFFFFFFFE;
			else
				htmlColor = (g_ColorManager.Color16To32((ushort)data.Color) << 8) | 0xFF;
		}
		else if (!data.HaveBackground)
		{
			color = 0xFFFF;

			if (!data.HaveScrollbar)
				htmlColor = 0x010101FF;
		}
		else
		{
			width -= 9;
			htmlColor = 0x010101FF;
		}

		CGUIHTMLText *htmlText = (CGUIHTMLText*)htmlGump->Add(new CGUIHTMLText(data.TextID, (uchar)(m_ClientVersion >= CV_308Z), color, 0, 0, width, TS_LEFT, /*UOFONT_BLACK_BORDER*/0, htmlColor));

		if (data.IsXMF)
		{
			htmlText->Text = g_ClilocManager.Cliloc(g_Language)->GetW(data.TextID);
			htmlText->CreateTexture(!data.HaveBackground);
			htmlGump->CalculateDataSize();
		}
	}

	list.clear();
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(OpenGump)
{
	WISPFUN_DEBUG("c150_f84");
	if (g_World == NULL)
		return;

	vector<HTMLGumpDataInfo> htmlGumlList;

	//TPRINT("Gump dump::\n");
	//TDUMP(buf, size);

	uint serial = ReadUInt32BE();
	uint id = ReadUInt32BE();
	int x = ReadInt32BE();
	int y = ReadInt32BE();

	if (m_LastGumpID == id)
	{
		x = m_LastGumpX;
		y = m_LastGumpY;
	}

	CGumpGeneric *gump = new CGumpGeneric(serial, x, y, id);

	ushort commandsLength = ReadUInt16BE();
	string commands = ReadString(commandsLength);

	WISP_FILE::CTextFileParser parser("", "", "", "{}");
	WISP_FILE::CTextFileParser cmdParser("", " ", "", "");

	STRING_LIST commandList = parser.GetTokens(commands.c_str());

	for (const string &str : commandList)
	{
		STRING_LIST list = cmdParser.GetTokens(str.c_str());

		int listSize = (int)list.size();

		if (!listSize)
			continue;

		string cmd = ToLowerA(list[0]);

		CBaseGUI *go = NULL;

		if (cmd == "nodispose")
		{
		}
		else if (cmd == "nomove")
		{
			gump->NoMove = true;
		}
		else if (cmd == "noclose")
		{
			gump->NoClose = true;
		}
		else if (cmd == "page")
		{
			if (listSize >= 2)
			{
				AddHTMLGumps(gump, htmlGumlList);

				int page = ToInt(list[1]);
				go = new CGUIPage(page);
			}
		}
		else if (cmd == "group")
		{
			if (listSize >= 2)
			{
				int group = ToInt(list[1]);
				go = new CGUIGroup(group);
			}
		}
		else if (cmd == "endgroup")
		{
			go = new CGUIGroup(0);
		}
		else if (cmd == "resizepic")
		{
			if (listSize >= 6)
			{
				int x = ToInt(list[1]);
				int y = ToInt(list[2]);
				int graphic = ToInt(list[3]);
				int width = ToInt(list[4]);
				int height = ToInt(list[5]);

				go = new CGUIResizepic(0, graphic, x, y, width, height);
				go->DrawOnly = true;
			}
		}
		else if (cmd == "checkertrans")
		{
			if (listSize >= 5)
			{
				int x = ToInt(list[1]);
				int y = ToInt(list[2]);
				int width = ToInt(list[3]);
				int height = ToInt(list[4]);

				go = new CGUIChecktrans(x, y, width, height);
			}
		}
		else if (cmd == "button")
		{
			if (listSize >= 5)
			{
				int x = ToInt(list[1]);
				int y = ToInt(list[2]);
				int up = ToInt(list[3]);
				int down = ToInt(list[4]);
				int action = 0;
				int toPage = 0;
				int index = 0;

				if (listSize >= 6)
					action = ToInt(list[5]);
				if (listSize >= 7)
					toPage = ToInt(list[6]);
				if (listSize >= 8)
					index = ToInt(list[7]);

				if (action != 0)
					toPage = -1;

				go = new CGUIButton(index, up, up, down, x, y);
				go->CheckPolygone = true;

				((CGUIButton*)go)->ToPage = toPage;
			}
		}
		else if (cmd == "buttontileart")
		{
			if (listSize >= 12)
			{
				int x = ToInt(list[1]);
				int y = ToInt(list[2]);
				int up = ToInt(list[3]);
				int down = ToInt(list[4]);
				int action = ToInt(list[5]);
				int toPage = ToInt(list[6]);
				int index = ToInt(list[7]);
				int tileGraphic = ToInt(list[8]);
				int tileColor = ToInt(list[9]);
				int tileX = ToInt(list[10]);
				int tileY = ToInt(list[11]);

				if (action != 0)
					toPage = -1;

				go = new CGUIButtonTileart(index, up, up, down, x, y, tileGraphic, tileColor, tileX, tileY);

				((CGUIButton*)go)->ToPage = toPage;
			}
		}
		else if (cmd == "checkbox")
		{
			if (listSize >= 5)
			{
				int x = ToInt(list[1]);
				int y = ToInt(list[2]);
				int up = ToInt(list[3]);
				int down = ToInt(list[4]);
				int state = 0;
				int index = 0;

				if (listSize >= 6)
					state = ToInt(list[5]);
				if (listSize >= 7)
					index = ToInt(list[6]);

				go = new CGUICheckbox(index, up, down, up, x, y);

				((CGUICheckbox*)go)->Checked = (state != 0);
			}
		}
		else if (cmd == "radio")
		{
			if (listSize >= 5)
			{
				int x = ToInt(list[1]);
				int y = ToInt(list[2]);
				int up = ToInt(list[3]);
				int down = ToInt(list[4]);
				int state = 0;
				int index = 0;

				if (listSize >= 6)
					state = ToInt(list[5]);
				if (listSize >= 7)
					index = ToInt(list[6]);

				go = new CGUIRadio(index, up, down, up, x, y);

				((CGUIRadio*)go)->Checked = (state != 0);
			}
		}
		else if (cmd == "text")
		{
			if (listSize >= 5)
			{
				int x = ToInt(list[1]);
				int y = ToInt(list[2]);
				int color = ToInt(list[3]);
				int index = ToInt(list[4]);

				if (color)
					color++;

				go = new CGUIGenericText(index, color, x, y);
				go->DrawOnly = true;
			}
		}
		else if (cmd == "croppedtext")
		{
			if (listSize >= 7)
			{
				int x = ToInt(list[1]);
				int y = ToInt(list[2]);
				int width = ToInt(list[3]);
				int height = ToInt(list[4]);
				int color = ToInt(list[5]);
				int index = ToInt(list[6]);

				if (color)
					color++;

				go = new CGUIGenericText(index, color, x, y, width);
				go->DrawOnly = true;
			}
		}
		else if (cmd == "textentry")
		{
			if (listSize >= 8)
			{
				int x = ToInt(list[1]);
				int y = ToInt(list[2]);
				int width = ToInt(list[3]);
				int height = ToInt(list[4]);
				int color = ToInt(list[5]);
				int index = ToInt(list[6]);
				int textIndex = ToInt(list[7]);

				if (color)
					color++;

				gump->Add(new CGUIHitBox(index + 1, x, y, width, height));
				gump->Add(new CGUIScissor(true, x, y, 0, 0, width, height));
				go = new CGUIGenericTextEntry(index + 1, textIndex, color, x, y);
				((CGUIGenericTextEntry*)go)->CheckOnSerial = true;
				((CGUITextEntry*)go)->m_Entry.Width = width;
			}
		}
		else if (cmd == "textentrylimited")
		{
			if (listSize >= 9)
			{
				int x = ToInt(list[1]);
				int y = ToInt(list[2]);
				int width = ToInt(list[3]);
				int height = ToInt(list[4]);
				int color = ToInt(list[5]);
				int index = ToInt(list[6]);
				int textIndex = ToInt(list[7]);
				int length = ToInt(list[8]);

				if (color)
					color++;

				gump->Add(new CGUIHitBox(index + 1, x, y, width, height));
				gump->Add(new CGUIScissor(true, x, y, 0, 0, width, height));
				go = new CGUIGenericTextEntry(index + 1, textIndex, color, x, y, width, length);
				((CGUIGenericTextEntry*)go)->CheckOnSerial = true;
				((CGUITextEntry*)go)->m_Entry.Width = width;
			}
		}
		else if (cmd == "tilepic")
		{
			if (listSize >= 4)
			{
				int x = ToInt(list[1]);
				int y = ToInt(list[2]);
				int graphic = ToInt(list[3]);

				go = new CGUITilepic(graphic, 0, x, y);
				go->DrawOnly = true;
			}
		}
		else if (cmd == "tilepichue")
		{
			if (listSize >= 4)
			{
				int x = ToInt(list[1]);
				int y = ToInt(list[2]);
				int graphic = ToInt(list[3]);
				int color = 0;

				if (listSize >= 5)
					color = ToInt(list[4]);

				if (color)
					gump->Add(new CGUIShader(g_ColorizerShader, true));

				go = new CGUITilepic(graphic, color, x, y);
				go->DrawOnly = true;
			}
		}
		else if (cmd == "gumppic")
		{
			if (listSize >= 4)
			{
				int x = ToInt(list[1]);
				int y = ToInt(list[2]);
				int graphic = ToInt(list[3]);
				int color = 0;

				if (listSize >= 5)
				{
					WISP_FILE::CTextFileParser gumppicParser("", "=", "", "");
					STRING_LIST hueList = gumppicParser.GetTokens(list[4].c_str());

					if (hueList.size() > 1)
						color = ToInt(hueList[1]);
					else
						color = ToInt(hueList[0]);

					if (listSize >= 6)
					{
						STRING_LIST classList = gumppicParser.GetTokens(list[5].c_str());

						if (hueList.size() > 1)
						{
							if (ToLowerA(classList[0]) == "class" && ToLowerA(Trim(classList[1])) == "virtuegumpitem")
							{
								go = new CGUIVirtureGump(graphic, x, y);
							}
						}
					}
				}

				if (go == NULL)
				{
					if (color)
						gump->Add(new CGUIShader(g_ColorizerShader, true));

					go = new CGUIGumppic(graphic, x, y);
					go->Color = color;
					go->DrawOnly = true;
				}
			}
		}
		else if (cmd == "gumppictiled")
		{
			if (listSize >= 6)
			{
				int x = ToInt(list[1]);
				int y = ToInt(list[2]);
				int width = ToInt(list[3]);
				int height = ToInt(list[4]);
				int graphic = ToInt(list[5]);

				go = new CGUIGumppicTiled(graphic, x, y, width, height);
				go->DrawOnly = true;
			}
		}
		else if (cmd == "htmlgump" || cmd == "xmfhtmlgump" || cmd == "xmfhtmlgumpcolor")
		{
			if (listSize >= 8)
			{
				HTMLGumpDataInfo htmlInfo = { 0 };
				htmlInfo.IsXMF = (cmd != "htmlgump");

				htmlInfo.X = ToInt(list[1]);
				htmlInfo.Y = ToInt(list[2]);
				htmlInfo.Width = ToInt(list[3]);
				htmlInfo.Height = ToInt(list[4]);
				htmlInfo.TextID = ToInt(list[5]);
				htmlInfo.HaveBackground = ToInt(list[6]);
				htmlInfo.HaveScrollbar = ToInt(list[7]);
				htmlInfo.Color = 0;

				if (cmd == "xmfhtmlgumpcolor" && listSize >= 9)
				{
					htmlInfo.Color = ToInt(list[8]);

					if (htmlInfo.Color == 0x7FFF)
						htmlInfo.Color = 0x00FFFFFF;
				}

				htmlGumlList.push_back(htmlInfo);
			}
		}
		/*else if (cmd == "xmfhtmltok")
		{
			if (listSize >= 5)
			{
			}
		}
		else if (cmd == "tooltip")
		{
			if (listSize >= 2)
			{
				int cliloc = ToInt(list[1]);
			}
		}
		else if (cmd == "mastergump")
		{
			if (listSize >= 2)
			{
				int index = ToInt(list[1]);
			}
		}*/

		if (go != NULL)
		{
			gump->Add(go);

			if ((go->Type == GOT_TILEPIC || go->Type == GOT_GUMPPIC) && go->Color)
				gump->Add(new CGUIShader(g_ColorizerShader, false));
			else if (go->Type == GOT_TEXTENTRY)
				gump->Add(new CGUIScissor(false));
		}
	}

	AddHTMLGumps(gump, htmlGumlList);

	int textLinesCount = ReadInt16BE();

	IFOR(i, 0, textLinesCount)
	{
		int linelen = ReadInt16BE();

		if (linelen)
			gump->AddText(i, ReadWString(linelen, true));
		else
			gump->AddText(i, L"");
	}

	g_GumpManager.AddGump(gump);
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(OpenCompressedGump)
{
	WISPFUN_DEBUG("c150_f85");
	if (g_World == NULL)
		return;

	uint senderID = ReadUInt32BE();
	uint gumpID = ReadUInt32BE();
	uint x = ReadUInt32BE();
	uint y = ReadUInt32BE();
	uLongf cLen = ReadUInt32BE() - 4; //Compressed Length (4 == sizeof(DecompressedLen)
	uLongf dLen = ReadUInt32BE(); //Decompressed Length

	if (cLen < 1)
	{
		LOG("CLen=%d\nServer Sends bad Compressed Gumpdata!\n", cLen);

		return;
	}
	else if ((int)(28 + cLen) > m_Size)
	{
		LOG("Server Sends bad Compressed Gumpdata!\n");

		return;
	}

	// Layout data.....
	UCHAR_LIST decLayoutData(dLen);
	LOG("Gump layout:\n\tSenderID=0x%08X\n\tGumpID=0x%08X\n\tCLen=%d\n\tDLen=%d\nDecompressing layout gump data...\n", senderID, gumpID, cLen, dLen);

	int z_err = uncompress(&decLayoutData[0], &dLen, m_Ptr, cLen);

	if (z_err != Z_OK)
	{
		LOG("Decompress layout gump error %d\n", z_err);

		return;
	}

	LOG("Layout gump data decompressed!\n");
	// Text data.....

	Move(cLen);

	uint linesCount = ReadUInt32BE(); //Text lines count
	uint cTLen = 0;
	uLongf dTLen = 0;
	UCHAR_LIST gumpDecText;

	if (linesCount > 0)
	{
		cTLen = ReadUInt32BE(); //Compressed lines length
		dTLen = ReadUInt32BE(); //Decompressed lines length

		gumpDecText.resize(dTLen);

		LOG("Decompressing text gump data...\n");

		z_err = uncompress(&gumpDecText[0], &dTLen, m_Ptr, cTLen);

		if (z_err != Z_OK)
		{
			LOG("Decompress text gump error %d\n", z_err);

			return;
		}

		LOG("Text gump data decompressed!\nGump text lines:\n\tLinesCount=%d\n\tCTLen=%d\n\tDTLen=%d\n", linesCount, cTLen, dTLen);
	}

	int newsize = 21 + dLen + 2 + dTLen;

	UCHAR_LIST newbufData(newsize);
	puchar newbuf = &newbufData[0];
	newbuf[0] = 0xb0;
	pack16(newbuf + 1, newsize);
	pack32(newbuf + 3, senderID);
	pack32(newbuf + 7, gumpID);
	pack32(newbuf + 11, x);
	pack32(newbuf + 15, y);
	pack16(newbuf + 19, (ushort)dLen);
	memcpy(newbuf + 21, &decLayoutData[0], dLen);
	pack16(newbuf + 21 + dLen, (ushort)linesCount);

	if (linesCount > 0)
		memcpy(newbuf + 23 + dLen, &gumpDecText[0], dTLen);
	else
		newbuf[newsize - 1] = 0x00;

	LOG("Gump decompressed! newsize=%d\n", newsize);

	m_Size = newsize;
	m_Start = newbuf;
	m_End = m_Start + m_Size;

	OnPacket();
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(DyeData)
{
	WISPFUN_DEBUG("c150_f86");
	uint serial = ReadUInt32BE();
	Move(2);
	ushort graphic = ReadUInt16BE();

	WISP_GEOMETRY::CSize gumpSize = g_Orion.GetGumpDimension(0x0906);

	int x = (WORD)((g_OrionWindow.Size.Width / 2) - (gumpSize.Width / 2));
	int y = (WORD)((g_OrionWindow.Size.Height / 2) - (gumpSize.Height / 2));

	CGumpDye *gump = new CGumpDye(serial, x, y, graphic);

	g_GumpManager.AddGump(gump);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(DisplayMap)
{
	WISPFUN_DEBUG("c150_f87");
	if (g_World == NULL)
		return;

	uint serial = ReadUInt32BE();
	ushort gumpid = ReadUInt16BE();

	//TPRINT("gumpid = 0x%04X\n", gumpid);

	ushort startX = ReadUInt16BE();
	ushort startY = ReadUInt16BE();
	ushort endX = ReadUInt16BE();
	ushort endY = ReadUInt16BE();
	ushort width = ReadUInt16BE();
	ushort height = ReadUInt16BE();

	CGumpMap *gump = new CGumpMap(serial, gumpid, startX, startY, endX, endY, width, height);

	if (*m_Start == 0xF5 || m_ClientVersion >= CV_308Z) //308z или выше?
	{
		ushort facet = 0;

		if (*m_Start == 0xF5)
			facet = ReadUInt16BE();

		g_MultiMap.LoadFacet(gump, gump->m_Texture, facet);
	}
	else
		g_MultiMap.LoadMap(gump, gump->m_Texture);

	//TPRINT("GumpX=%d GumpY=%d\n", startX, startY);
	//TPRINT("GumpTX=%d GumpTY=%d\n", endX, endY);
	//TPRINT("GumpW=%d GumpH=%d\n", width, height);

	g_GumpManager.AddGump(gump);

	CGameItem *obj = g_World->FindWorldItem(serial);

	if (obj != NULL)
		obj->Opened = true;
}
//---------------------------------------------------------------------------
PACKET_HANDLER(MapData)
{
	WISPFUN_DEBUG("c150_f88");
	if (g_World == NULL)
		return;

	uint serial = ReadUInt32BE();
	CGumpMap *gump = (CGumpMap*)g_GumpManager.UpdateGump(serial, 0, GT_MAP);

	if (gump != NULL && gump->m_DataBox != NULL)
	{
		switch ((MAP_MESSAGE)ReadUInt8()) //Action
		{
			case MM_ADD: //Add Pin
			{
				Move(1);

				short x = ReadUInt16BE();
				short y = ReadUInt16BE();

				gump->m_DataBox->Add(new CGUIGumppic(0x139B, x, y));
				gump->WantRedraw = true;

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
				gump->m_DataBox->Clear();
				gump->WantRedraw = true;

				break;
			}
			case MM_EDIT_RESPONSE: //Reply From Server to Action 6 (Plotting request)
			{
				gump->PlotState = ReadUInt8();

				break;
			}
			default:
				break;
		}
	}
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(TipWindow)
{
	WISPFUN_DEBUG("c150_f89");
	uchar flag = ReadUInt8();

	if (flag != 1) //1 - ignore
	{
		uint serial = ReadUInt32BE();
		short len = ReadInt16BE();

		string str = ReadString(len);

		int x = 20;
		int y = 20;

		if (!flag)
		{
			x = 200;
			y = 100;
		}

		CGumpTip *gump = new CGumpTip(serial, x, y, str, flag != 0);

		g_GumpManager.AddGump(gump);
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(CharacterProfile)
{
	WISPFUN_DEBUG("c150_f90");
	if (g_World == NULL)
		return;

	uint serial = ReadUInt32BE();

	wstring topText = ToWString(ReadString(0));

	wstring bottomText = ReadWString(0);
	wstring dataText = ReadWString(0);

	CGumpProfile *gump = new CGumpProfile(serial, 170, 90, topText, bottomText, dataText);

	g_GumpManager.AddGump(gump);
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(BulletinBoardData)
{
	WISPFUN_DEBUG("c150_f91");
	if (g_World == NULL)
		return;

	switch (ReadUInt8())
	{
		case 0: //Open board
		{
			uint serial = ReadUInt32BE();

			CGameItem *item = g_World->FindWorldItem(serial);

			if (item != NULL)
			{
				CGumpBulletinBoard *bbGump = (CGumpBulletinBoard*)g_GumpManager.UpdateGump(serial, 0, GT_BULLETIN_BOARD);

				if (bbGump != NULL)
				{
					CBaseGUI *bbItem = (CBaseGUI*)bbGump->m_HTMLGump->m_Items;

					while (bbItem != NULL)
					{
						CBaseGUI *bbNext = (CBaseGUI*)bbItem->m_Next;

						if (bbItem->Type == GOT_BB_OBJECT)
							bbGump->m_HTMLGump->Delete(bbItem);

						bbItem = bbNext;
					}

					bbGump->m_HTMLGump->CalculateDataSize();
				}

				item->Opened = true;
			}

			string str((char*)Ptr);

			int x = (g_OrionWindow.Size.Width / 2) - 245;
			int y = (g_OrionWindow.Size.Height / 2) - 205;

			CGumpBulletinBoard *gump = new CGumpBulletinBoard(serial, x, y, str);

			g_GumpManager.AddGump(gump);

			break;
		}
		case 1: //Summary message
		{
			uint boardSerial = ReadUInt32BE();

			CGumpBulletinBoard *gump = (CGumpBulletinBoard*)g_GumpManager.GetGump(boardSerial, 0, GT_BULLETIN_BOARD);

			if (gump != NULL)
			{
				uint serial = ReadUInt32BE();
				uint parentID = ReadUInt32BE();

				//poster
				int len = ReadUInt8();
				wstring text = (len > 0 ? DecodeUTF8(ReadString(len)) : L"") + L" - ";

				//subject
				len = ReadUInt8();
				text += (len > 0 ? DecodeUTF8(ReadString(len)) : L"") + L" - ";

				//data time
				len = ReadUInt8();
				text += (len > 0 ? DecodeUTF8(ReadString(len)) : L"");

				int posY = (gump->m_HTMLGump->GetItemsCount() - 5) * 18;

				if (posY < 0)
					posY = 0;

				gump->m_HTMLGump->Add(new CGUIBulletinBoardObject(serial, 0, posY, text));
				gump->m_HTMLGump->CalculateDataSize();
			}

			break;
		}
		case 2: //Message
		{
			uint boardSerial = ReadUInt32BE();

			CGumpBulletinBoard *gump = (CGumpBulletinBoard*)g_GumpManager.GetGump(boardSerial, 0, GT_BULLETIN_BOARD);

			if (gump != NULL)
			{
				uint serial = ReadUInt32BE();

				//poster
				int len = ReadUInt8();
				wstring poster = (len > 0 ? DecodeUTF8(ReadString(len)) : L"");

				//subject
				len = ReadUInt8();
				wstring subject = (len > 0 ? DecodeUTF8(ReadString(len)) : L"");

				//data time
				len = ReadUInt8();
				wstring dataTime = (len > 0 ? DecodeUTF8(ReadString(len)) : L"");

				//unused, in old clients: user's graphic, color
				Move(4);

				uchar unknown = ReadUInt8();

				if (unknown > 0)
				{
					//unused data
					Move(unknown * 4);
				}

				uchar lines = ReadUInt8();
				wstring data = L"";

				IFOR(i, 0, lines)
				{
					uchar linelen = ReadUInt8();

					if (data.length())
						data += L"\n";

					if (linelen > 0)
						data += DecodeUTF8(ReadString(linelen));
				}

				uchar variant = 1 + (int)(poster == ToWString(g_Player->Name));
				g_GumpManager.AddGump(new CGumpBulletinBoardItem(serial, 0, 0, variant, boardSerial, poster, subject, dataTime, data));
			}

			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(OpenBook)
{
	WISPFUN_DEBUG("c150_f92");
	if (g_World == NULL)
		return;

	uint serial = ReadUInt32BE();
	uchar flags = ReadUInt8();
	Move(1);
	WORD pageCount = ReadUInt16BE();

	CGumpBook *gump = new CGumpBook(serial, 0, 0, pageCount, flags != 0, (g_PacketManager.ClientVersion >= CV_308Z));

	gump->m_EntryTitle->m_Entry.SetText(ReadString(60));
	gump->m_EntryAuthor->m_Entry.SetText(ReadString(30));

	g_GumpManager.AddGump(gump);
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(OpenBookNew)
{
	WISPFUN_DEBUG("c150_f93");
	if (g_World == NULL)
		return;

	uint serial = ReadUInt32BE();
	uchar flag1 = ReadUInt8();
	uchar flag2 = ReadUInt8();
	ushort pageCount = ReadUInt16BE();

	CGumpBook *gump = new CGumpBook(serial, 0, 0, pageCount, (flag1 + flag2) != 0, true);


	int titleLen = ReadUInt16BE();

	if (titleLen > 0)
		gump->m_EntryTitle->m_Entry.SetText(ReadString(titleLen));

	int authorLen = ReadUInt16BE();

	if (authorLen > 0)
		gump->m_EntryAuthor->m_Entry.SetText(ReadString(authorLen));

	g_GumpManager.AddGump(gump);
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(BookData)
{
	WISPFUN_DEBUG("c150_f94");
	if (g_World == NULL)
		return;

	uint serial = ReadUInt32BE();

	CGumpBook *gump = (CGumpBook*)g_GumpManager.GetGump(serial, 0, GT_BOOK);

	if (gump != NULL)
	{
		ushort pageCount = ReadUInt16BE();

		IFOR(i, 0, pageCount)
		{
			ushort page = ReadUInt16BE();

			if (page > gump->PageCount)
				continue;

			ushort lineCount = ReadUInt16BE();

			wstring str = L"";

			IFOR(j, 0, lineCount)
			{
				if (j)
					str += L'\n';

				wstring temp = DecodeUTF8(ReadString(0));

				while (temp.length() && (temp.back() == L'\n' || temp.back() == L'\r'))
					temp.resize(temp.length() - 1);

				str += temp;
			}

			gump->SetPageData(page, str);
		}
	}
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(BuyList)
{
	WISPFUN_DEBUG("c150_f95");
	if (g_World == NULL)
		return;

	uint serial = ReadUInt32BE();

	CGameItem *container = g_World->FindWorldItem(serial);

	if (container == NULL)
	{
		LOG("Error!!! Buy container is not found!!!\n");
		return;
	}

	uint vendorSerial = container->Container;

	CGameCharacter *vendor = g_World->FindWorldCharacter(vendorSerial);

	if (vendor == NULL)
	{
		LOG("Error!!! Buy vendor is not found!!!\n");
		return;
	}

	CGumpShop *gump = (CGumpShop*)g_GumpManager.GetGump(vendorSerial, 0, GT_SHOP);

	if (gump != NULL && (gump->Serial != vendorSerial || !gump->IsBuyGump))
	{
		g_GumpManager.RemoveGump(gump);
		gump = NULL;
	}

	if (gump == NULL)
	{
		gump = new CGumpShop(vendorSerial, true, 150, 5);
		g_GumpManager.AddGump(gump);
	}

	gump->WantRedraw = true;

	if (container->Layer == OL_BUY_RESTOCK || container->Layer == OL_BUY)
	{
		uchar count = ReadUInt8();

		CGameItem *item = (CGameItem*)container->m_Items;
		//oldp spams this packet twice in a row on NPC verndors. NULL check is needed t
		if (item == NULL)
			return;

		bool reverse = (item->X > 1);

		if (reverse)
		{
			while (item != NULL && item->m_Next != NULL)
				item = (CGameItem*)item->m_Next;
		}

		CGUIHTMLGump *htmlGump = gump->m_ItemList[0];

		int currentY = 0;

		QFOR(shopItem, htmlGump->m_Items, CBaseGUI*)
		{
			if (shopItem->Type == GOT_SHOPITEM)
				currentY += shopItem->GetSize().Height;
		}

		IFOR(i, 0, count)
		{
			if (item == NULL)
			{
				LOG("Error!!! Buy item is not found!!!\n");
				break;
			}

			item->Price = ReadUInt32BE();

			uchar nameLen = ReadUInt8();
			string name = ReadString(nameLen);

			//try int.parse and read cliloc.
			int clilocNum = 0;

			if (Int32TryParse(name, clilocNum))
			{
				item->Name = g_ClilocManager.Cliloc(g_Language)->GetA(clilocNum, true);
				item->NameFromCliloc = true;
			}
			else
			{
				item->Name = name;
				item->NameFromCliloc = false;
			}

			if (reverse)
				item = (CGameItem*)item->m_Prev;
			else
				item = (CGameItem*)item->m_Next;
		}

		htmlGump->CalculateDataSize();
	}
	else
		LOG("Unknown layer for buy container!!!\n");
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(SellList)
{
	WISPFUN_DEBUG("c150_f96");
	if (g_World == NULL)
		return;

	uint serial = ReadUInt32BE();

	CGameCharacter *vendor = g_World->FindWorldCharacter(serial);

	if (vendor == NULL)
	{
		LOG("Error!!! Sell vendor is not found!!!\n");
		return;
	}

	ushort count = ReadUInt16BE();

	if (!count)
	{
		LOG("Error!!! Sell list is empty.\n");
		return;
	}

	g_GumpManager.CloseGump(0, 0, GT_SHOP);

	CGumpShop *gump = new CGumpShop(serial, false, 100, 0);
	CGUIHTMLGump *htmlGump = gump->m_ItemList[0];

	int currentY = 0;

	IFOR(i, 0, count)
	{
		uint itemSerial = ReadUInt32BE();
		ushort graphic = ReadUInt16BE();
		ushort color = ReadUInt16BE();
		ushort count = ReadUInt16BE();
		ushort price = ReadUInt16BE();
		int nameLen = ReadInt16BE();
		string name = ReadString(nameLen);

		int clilocNum = 0;
		bool nameFromCliloc = false;

		if (Int32TryParse(name, clilocNum))
		{
			name = g_ClilocManager.Cliloc(g_Language)->GetA(clilocNum, true);
			nameFromCliloc = true;
		}

		CGUIShopItem *shopItem = (CGUIShopItem*)htmlGump->Add(new CGUIShopItem(itemSerial, graphic, color, count, price, name, 0, currentY));
		shopItem->NameFromCliloc = nameFromCliloc;

		if (!i)
		{
			shopItem->Selected = true;
			shopItem->CreateNameText();
			shopItem->UpdateOffsets();
		}

		currentY += shopItem->GetSize().Height;
	}

	htmlGump->CalculateDataSize();

	g_GumpManager.AddGump(gump);
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(BuyReply)
{
	WISPFUN_DEBUG("c150_f97");
	uint serial = ReadUInt32BE();
	uchar flag = ReadUInt8();

	if (!flag) //Close shop gump
		g_GumpManager.CloseGump(serial, 0, GT_SHOP);
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(Logout)
{
	WISPFUN_DEBUG("c150_f98");
	g_Orion.LogOut();
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(OPLInfo)
{
	WISPFUN_DEBUG("c150_f99");
	if (g_TooltipsEnabled)
	{
		uint serial = ReadUInt32BE();
		uint revision = ReadUInt32BE();
		//Если хранить ревизию на обьекте, то сравнивая её и то что здесь пришло
		// и получая одинаковый результат, - нет смысла запрашивать у сервера эти данные заного.
		AddMegaClilocRequest(serial);
	}
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(CustomHouse)
{
	WISPFUN_DEBUG("c150_f100");
	bool compressed = ReadUInt8() == 0x03;
	bool enableResponse = ReadUInt8() == 0x01;
	uint serial = ReadUInt32BE();
	uint revision = ReadUInt32BE();
	CGameItem *foundationItem = g_World->GetWorldItem(serial);

	if (foundationItem == NULL)
		return;

	CMulti* multi = foundationItem->GetMulti();

	if (multi == NULL)
		return;

	ReadUInt16BE();
	ReadUInt16BE();

	CCustomHouse *house = g_CustomHousesManager.Get(serial);

	if (house == NULL)
	{
		house = new CCustomHouse(serial, revision);
		g_CustomHousesManager.Add(house);
	}
	else
		house->Revision = revision;

	LOG("House update in cache: 0x%08X 0x%08X\n", serial, revision);

	house->m_Items.clear();

	short minX = multi->MinX;
	short minY = multi->MinY;
	short maxY = multi->MaxY;

	uchar planes = ReadUInt8();

	IFOR(plane, 0, planes)
	{
		uint header = ReadUInt32BE();
		uLongf dLen = ((header & 0xFF0000) >> 16) | ((header & 0xF0) << 4);
		int cLen = ((header & 0xFF00) >> 8) | ((header & 0x0F) << 8);
		int planeZ = (header & 0x0F000000) >> 24;
		int planeMode = (header & 0xF0000000) >> 28;

		if (cLen <= 0)
			continue;

		UCHAR_LIST decompressedBytes(dLen);
		int z_err = uncompress(&decompressedBytes[0], &dLen, m_Ptr, cLen);

		if (z_err != Z_OK)
		{
			LOG("Bad CustomHouseStruct compressed data received from server, house serial:%i\n", serial);
			//LOG("House plane idx:%i\n", idx);
			continue;
		}

		WISP_DATASTREAM::CDataReader tempReader(static_cast<puchar>(&decompressedBytes[0]), dLen);
		Move(cLen);

		ushort id = 0;
		char x = 0;
		char y = 0;
		char z = 0;

		switch (planeMode)
		{
			case 0:
			{
				for (uint i = 0; i < decompressedBytes.size() / 5; i++)
				{
					id = tempReader.ReadUInt16BE();
					x = tempReader.ReadUInt8();
					y = tempReader.ReadUInt8();
					z = tempReader.ReadUInt8();

					if (id != 0)
						house->m_Items.push_back(CBuildObject(id, x, y, z));
				}

				break;
			}
			case 1:
			{
				if (planeZ > 0)
					z = ((planeZ - 1) % 4) * 20 + 7; // Z=7,27,47,67
				else
					z = 0;

				for (uint i = 0; i < decompressedBytes.size()/4; i++)
				{
					id = tempReader.ReadUInt16BE();
					x = tempReader.ReadUInt8();
					y = tempReader.ReadUInt8();

					if (id != 0)
						house->m_Items.push_back(CBuildObject(id, x, y, z));
				}

				break;
			}
			case 2:
			{
				short xOffs = 0;
				short yOffs = 0;
				short multiHeight = 0;

				if (planeZ > 0)
					z = ((planeZ - 1) % 4) * 20 + 7; // Z=7,27,47,67
				else
					z = 0;

				if (planeZ <= 0)
				{
					xOffs = minX;
					yOffs = minY;
					multiHeight = (maxY - minY) + 2;
				}
				else if (planeZ <= 4)
				{
					xOffs = minX + 1;
					yOffs = minY + 1;
					multiHeight = (maxY - minY);
				}
				else
				{
					xOffs = minX;
					yOffs = minY;
					multiHeight = (maxY - minY) + 1;
				}

				for (uint i = 0; i < decompressedBytes.size() / 2; i++)
				{
					id = tempReader.ReadUInt16BE();
					x = i / multiHeight + xOffs;
					y = i % multiHeight + yOffs;

					if (id != 0)
						house->m_Items.push_back(CBuildObject(id, x, y, z));
				}

				break;
			}
			default:
				break;
		}
	}

	house->Paste(foundationItem);

	if (enableResponse)
		CPacketCustomHouseResponse().Send();
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(OrionMessages)
{
	ushort command = ReadUInt16BE();
	ushort type = command >> 12;
	command &= 0x5FFF;

	switch (command)
	{
		case OCT_CLOSE_GENERIC_GUMP_WITHOUT_RESPONSE:
		{
			uint serial = ReadUInt32BE();
			uint id = ReadUInt32BE();
			uchar all = ReadUInt8();

			QFOR(gump, g_GumpManager.m_Items, CGump*)
			{
				if (gump->GumpType == GT_GENERIC && gump->Serial == serial && gump->ID == id)
				{
					gump->RemoveMark = true;

					if (!all)
						break;
				}
			}

			break;
		}
		case OCT_SELECT_MENU:
		{
			uint serial = ReadUInt32BE();
			uint id = ReadUInt32BE();
			uint code = ReadUInt32BE();

			if (!serial && !id)
			{
				for (CGump *gump = (CGump*)g_GumpManager.m_Items; gump != NULL;)
				{
					CGump *next = (CGump*)gump->m_Next;

					if (gump->GumpType == GT_MENU || gump->GumpType == GT_GRAY_MENU)
					{
						CPacketMenuResponse(gump, code).Send();
						g_GumpManager.RemoveGump(gump);
					}

					gump = next;
				}

				break;
			}

			CGump *gump = g_GumpManager.GetGump(serial, id, GT_MENU);

			if (gump == NULL)
			{
				gump = g_GumpManager.GetGump(serial, id, GT_GRAY_MENU);

				if (gump != NULL)
				{
					CPacketGrayMenuResponse(gump, code).Send();
					g_GumpManager.RemoveGump(gump);
				}
			}
			else
			{
				CPacketMenuResponse(gump, code).Send();
				g_GumpManager.RemoveGump(gump);
			}

			break;
		}
		case OCT_CAST_SPELL_REQUEST:
		{
			int id = ReadUInt32BE();

			if (id >= 0)
			{
				g_LastSpellIndex = id;

				CPacketCastSpell(id).Send();
			}

			break;
		}
		case OCT_USE_SKILL_REQUEST:
		{
			int id = ReadUInt32BE();
			
			if (id >= 0)
			{
				g_LastSkillIndex = id;

				CPacketUseSkill(id).Send();
			}

			break;
		}
		case OCT_DRAW_STATUSBAR:
		{
			uint serial = ReadUInt32BE();
			int x = ReadInt32BE();
			int y = ReadInt32BE();
			bool minimized = (ReadUInt8() != 0);

			if (serial != g_PlayerSerial)
				minimized = true;

			CGump *gump = g_GumpManager.GetGump(serial, 0, GT_STATUSBAR);

			if (gump != NULL)
			{
				gump->Minimized = minimized;

				if (gump->Minimized)
				{
					gump->MinimizedX = x;
					gump->MinimizedY = y;
				}
				else
				{
					gump->X = x;
					gump->Y = y;
				}
			}
			else
			{
				CPacketStatusRequest(serial).Send();
				g_GumpManager.AddGump(new CGumpStatusbar(serial, x, y, minimized));
			}

			break;
		}
		case OCT_CLOSE_STATUSBAR:
		{
			uint serial = ReadUInt32BE();
			g_GumpManager.CloseGump(serial, 0, GT_STATUSBAR);
			break;
		}
		case OCT_SECURE_TRADE_CHECK:
		{
			uint id1 = ReadUInt32BE();

			CGumpSecureTrading *gump = (CGumpSecureTrading*)g_GumpManager.UpdateGump(id1, 0, GT_TRADE);

			if (gump != NULL)
			{
				gump->StateMy = (ReadUInt8() != 0);
				CPacketTradeResponse(gump, 2).Send();
			}

			break;
		}
		case OCT_SECURE_TRADE_CLOSE:
		{
			uint id1 = ReadUInt32BE();

			CGumpSecureTrading *gump = (CGumpSecureTrading*)g_GumpManager.GetGump(id1, 0, GT_TRADE);

			if (gump != NULL)
			{
				gump->RemoveMark = true;
				CPacketTradeResponse(gump, 1).Send();
			}

			break;
		}
		case OCT_UNICODE_SPEECH_REQUEST:
		{
			ushort color = ReadUInt16BE();
			wstring text = ReadWString(0);

			if (!color)
				color = g_ConfigManager.SpeechColor;

			CPacketUnicodeSpeechRequest(text.c_str(), ST_NORMAL, 3, color, (puchar)g_Language.c_str()).Send();

			break;
		}
		case OCT_RENAME_MOUNT_REQUEST:
		{
			uint serial = ReadUInt32BE();
			string text = ReadString(0);

			CPacketRenameRequest(serial, text).Send();

			if (g_TooltipsEnabled)
				g_PacketManager.AddMegaClilocRequest(serial);

			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------------
