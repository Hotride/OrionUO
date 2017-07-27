/***********************************************************************************
**
** PacketManager.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "PacketManager.h"
#include "ConnectionManager.h"
#include "../OrionUO.h"
#include "../OrionWindow.h"
#include "../CharacterList.h"
#include "../ServerList.h"
#include "../CityList.h"
#include "../Screen stages/ConnectionScreen.h"
#include "../Screen stages/MainScreen.h"
#include "../Screen stages/CharacterListScreen.h"
#include "../Screen stages/ServerScreen.h"
#include "../Network/Packets.h"
#include "../Game objects/GameWorld.h"
#include "../Game objects/GamePlayer.h"
#include "../Walker/Walker.h"
#include "../Game objects/ObjectOnCursor.h"
#include "../ClickObject.h"
#include "../Weather.h"
#include "../Managers/MapManager.h"
#include "../Target.h"
#include "../Managers/ConfigManager.h"
#include "../Managers/SoundManager.h"
#include "../Container.h"
#include "../Party.h"
#include "../Macro.h"
#include "../Managers/ClilocManager.h"
#include "../Managers/FontsManager.h"
#include "../Managers/GumpManager.h"
#include "../Managers/FileManager.h"
#include "../Gumps/GumpPaperdoll.h"
#include "../Gumps/GumpContainer.h"
#include "../Gumps/GumpPopupMenu.h"
#include "../Managers/EffectManager.h"
#include "../Managers/PluginManager.h"
#include "../Managers/ColorManager.h"
#include "../Game objects/GameEffectMoving.h"
#include "../Game objects/GameEffectDrag.h"
#include "../QuestArrow.h"
#include "../MultiMap.h"
#include "../TargetGump.h"
#include "../Walker/PathFinder.h"
#include "../Gumps/GumpBuff.h"
#include "../Gumps/GumpSecureTrading.h"
#include "../Gumps/GumpTextEntryDialog.h"
#include "../Gumps/GumpGrayMenu.h"
#include "../Gumps/GumpMenu.h"
#include "../Gumps/GumpDye.h"
#include "../Gumps/GumpGeneric.h"
#include "../Gumps/GumpMap.h"
#include "../Gumps/GumpTip.h"
#include "../Gumps/GumpProfile.h"
#include "../Gumps/GumpBulletinBoard.h"
#include "../Gumps/GumpBulletinBoardItem.h"
#include "../Gumps/GumpBook.h"
#include "../Gumps/GumpShop.h"
#include "../Gumps/GumpSkills.h"
#include "../Gumps/GumpSpellbook.h"
#include "AnimationManager.h"
#include "../zlib.h"  
#include "CustomHousesManager.h"
#include "../Wisp/WispTextFileParser.h"
#include "../Gumps/GumpMinimap.h"

#pragma comment(lib, "zdll.lib")
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
	/*0x28*/ RMSG(ORION_SAVE_PACKET, "Deny move item?", 0x05),
	/*0x29*/ RMSG(ORION_SAVE_PACKET, "Drop Item Acceptem", 0x01),
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
	/*0xFC*/ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE),
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
void CPacketManager::SendMegaClilocRequests(UINT_LIST &list)
{
	WISPFUN_DEBUG("c150_f4");
	if (list.size())
	{
		if (m_ClientVersion >= CV_500A)
			CPacketMegaClilocRequest(list).Send();
		else
		{
			IFOR(i, 0, (int)list.size())
				CPacketMegaClilocRequestOld(list[i]).Send();
		}
		list.clear();
	}
}
//---------------------------------------------------------------------------
void CPacketManager::SendMegaClilocRequests()
{
	WISPFUN_DEBUG("c150_f5");
	if (m_ClientVersion >= CV_308Z)
		SendMegaClilocRequests(m_MegaClilocRequests);
}
//----------------------------------------------------------------------------------
void CPacketManager::AddMegaClilocRequest(const uint &serial, const bool &existsTest)
{
	WISPFUN_DEBUG("c150_f6");
	if (existsTest)
	{
		for (const uint item : m_MegaClilocRequests)
		{
			if (item == serial)
				return;
		}
	}

	m_MegaClilocRequests.push_back(serial);
}
//----------------------------------------------------------------------------------
void CPacketManager::OnReadFailed()
{
	WISPFUN_DEBUG("c150_f7");
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
		LOG("%s--- ^(%d) r(+%d => %d) Server:: %s\n", buffer, ticks - g_LastPacketTime, m_Size, g_TotalRecvSize, info.Name);
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

	LOG("--- ^(%d) r(+%d => %d) Plugin:: %s\n", ticks - g_LastPacketTime, m_Size, g_TotalRecvSize, info.Name);
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
	g_NPCPopupEnabled = (bool)(g_ClientFlag & CLF_CONTEXT_MENU);
	g_TooltipsEnabled = (bool)(g_ClientFlag & CLF_PALADIN_NECROMANCER_TOOLTIPS);
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
	RELEASE_POINTER(g_Walker);
	RELEASE_POINTER(g_ObjectInHand);

	g_World = new CGameWorld(serial);
	g_Walker = new CWalker();
	g_PendingDelayTime = 0;

	g_UseItemActions.Clear();

	g_Ping = 0;
	g_WalkRequestCount = 0;
	g_ClickObject.Clear();
	g_Weather.Reset();
	g_SkillsTotal = 0.0f;
	g_ConsolePrompt = PT_NONE;
	g_MacroPointer = NULL;
	g_Season = ST_SUMMER;
	g_OldSeason = ST_SUMMER;
	g_GlobalScale = 1.0;
	g_PathFinder.BlockMoving = false;

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

	g_Walker->SetSequence(0, dir);
	g_Player->OffsetX = 0;
	g_Player->OffsetY = 0;
	g_Player->OffsetZ = 0;

	//if (m_ClientVersion >= CV_308Z && !g_Player->ClilocMessage.length())
	//	m_MegaClilocRequests.push_back(g_Player->Serial);

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

	Move(2);

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
/*void __cdecl UpdatePlayer(int serial, int graphic, int graphicIncrement, unsigned __int16 color, char flags, __int16 x, __int16 y, unsigned __int16 serverID, char direction, char z)
{
	__int16 fixedColor; // ax@4
	char oldFlags; // bl@4
	int v12; // eax@5
	__int16 v13; // ax@6
	__int16 fixedColor2; // ax@7
	int i; // esi@12

	if (serial != g_PlayerSerial)
		return;
	if (g_PlayerBankContainerGump)
	{
		(**(void(__stdcall ***)(_DWORD))g_PlayerBankContainerGump)(1);
		g_PlayerBankContainerGump = 0;
	}
	g_WalkingFailed = 0;
	(*((void(**)(void))g_Player->GameCharacter.GameObject.VTable + 5))();
	g_Player->GameCharacter.GameObject.X = x;
	g_Player->GameCharacter.GameObject.Y = y;
	g_Player->GameCharacter.GameObject.Z = z;
	SetCharacterGraphic(g_Player, graphic);
	g_Player->GameCharacter.GameObject.Direction = direction;
	(*((void(**)(void))g_Player->GameCharacter.GameObject.VTable + 46))();
	(*((void(**)(void))g_Player->GameCharacter.GameObject.VTable + 4))();
	UpdatePlayerCoordinates((unsigned __int16)x, (unsigned __int16)y, z, serverID);
	fixedColor = FixColor(color);
	g_Player->GameCharacter.GameObject.Color = fixedColor;
	oldFlags = g_Player->GameCharacter.GameObject.Flags;
	g_Player->GameCharacter.GameObject.Flags = flags;
	if (dword_739F58
		&& (v12 = g_Player->GameCharacter.GameObject.Graphic,
		LOWORD(v12) = v12 + 0x4000,
		(unsigned __int8)(*(int(__stdcall **)(int))(*(_DWORD *)dword_739F58 + 4))(v12)))
	{
		v13 = sub_446CA0(dword_739F58, oldFlags, g_Player->GameCharacter.GameObject.Graphic + 0x4000, 0);
		g_Player->GameCharacter.GameObject.Graphic2 = v13 & 0x3FFF;
		if (g_Player->GameCharacter.GameObject.Color)
		{
			g_Player->GameCharacter.GameObject.Color2 = FixColor(color);
			goto LABEL_11;
		}
		fixedColor2 = sub_452670(dword_739F58, oldFlags, color, g_Player->GameCharacter.GameObject.Graphic);
	}
	else
	{
		g_Player->GameCharacter.GameObject.Graphic2 = graphic;
		fixedColor2 = FixColor(color);
	}
	g_Player->GameCharacter.GameObject.Color2 = fixedColor2;
LABEL_11:
	sub_534AC0();
	DenyWalk(-1, -1, -1, -1);
	ClearWeatherEffects();
	if (oldFlags != g_Player->GameCharacter.GameObject.Flags)
	{
		for (i = dword_978178; i; i = *(_DWORD *)(i + 88))
		{
			if ((*(int(__thiscall **)(int))(*(_DWORD *)i + 392))(i))
			{
				if (*(CPlayer **)(i + 76) == g_Player)
					UpdateStatusbarGump(i, 1, 1, 1, 1);
			}
		}
	}
	g_WalkResendPacketSended = 0;
}*/
//----------------------------------------------------------------------------------
PACKET_HANDLER(UpdatePlayer)
{
	/*void __cdecl HandleUpdatePlayer(_BYTE *buf)
	{
		int offset; // [sp+4h] [bp-2Ch]@1
		int z; // [sp+8h] [bp-28h]@1
		int direction; // [sp+Ch] [bp-24h]@1
		int serverID; // [sp+10h] [bp-20h]@1
		int y; // [sp+14h] [bp-1Ch]@1
		int x; // [sp+18h] [bp-18h]@1
		int flags; // [sp+1Ch] [bp-14h]@1
		int color; // [sp+20h] [bp-10h]@1
		int graphicIncrement; // [sp+24h] [bp-Ch]@1
		int graphic; // [sp+28h] [bp-8h]@1
		int serial; // [sp+2Ch] [bp-4h]@1

		offset = 0;
		ReadInt32BE(buf, &offset, &serial);
		ReadInt16BE(buf, &offset, &graphic);
		ReadInt8(buf, &offset, &graphicIncrement);
		ReadInt16BE(buf, &offset, &color);
		ReadInt8(buf, &offset, &flags);
		ReadInt16BE(buf, &offset, &x);
		ReadInt16BE(buf, &offset, &y);
		ReadInt16BE(buf, &offset, &serverID);
		ReadInt8(buf, &offset, &direction);
		ReadInt8(buf, &offset, &z);
		UpdatePlayer(serial, graphic, graphicIncrement, color, flags, x, y, serverID, direction, z);
	}*/
	WISPFUN_DEBUG("c150_f25");
	if (g_World == NULL)
		return;

	uint serial = ReadUInt32BE();


	bool oldDead = g_Player->Dead();
	ushort oldGraphic = g_Player->Graphic;
	g_Player->Graphic = ReadUInt16BE();
	g_Player->OnGraphicChange();

	if (oldGraphic && oldGraphic != g_Player->Graphic)
	{
		if (g_Player->Dead())
		{
			g_Weather.Reset();
			g_Target.Reset();

			if (g_ConfigManager.Music)
				g_Orion.PlayMusic(42, true);

			g_DeathScreenTimer = g_Ticks + DEATH_SCREEN_DELAY;
		}
	}

	if (oldDead != g_Player->Dead())
	{
		if (g_Player->Dead())
			g_Orion.ChangeSeason(ST_DESOLATION, DEATH_MUSIC_INDEX);
		else
			g_Orion.ChangeSeason(g_OldSeason, g_OldSeasonMusic);
	}

	Move(1);
	g_Player->Color = ReadUInt16BE();
	g_Player->Flags = ReadUInt8();
	g_Player->X = ReadUInt16BE();
	g_Player->Y = ReadUInt16BE();
	Move(2);

	g_Player->CloseBank();

	g_Player->m_Steps.clear();

	uchar dir = ReadUInt8();
	g_Walker->SetSequence(0, dir);
	g_WalkRequestCount = 0;
	g_Player->OffsetX = 0;
	g_Player->OffsetY = 0;
	g_Player->OffsetZ = 0;

	g_Player->Direction = dir;
	g_Player->Z = ReadUInt8();

	if (m_ClientVersion >= CV_308Z)
	{
		m_MegaClilocRequests.push_back(g_PlayerSerial);

		if (g_Player->m_Items != NULL && !g_Player->m_Items->Empty())
		{
			CGameItem *backpack = (CGameItem*)g_Player->m_Items;
			m_MegaClilocRequests.push_back(backpack->Serial);
		}

		SendMegaClilocRequests();
	}

	g_GumpManager.RemoveRangedGumps();

	g_World->MoveToTop(g_Player);
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
					sprintf(str, "Your strength has changed by %d.  It is now %d.", deltaStr, newStr);
					g_Orion.CreateTextMessage(TT_SYSTEM, 0, 3, 0x0170, str);
				}

				if (deltaDex)
				{
					sprintf(str, "Your dexterity has changed by %d.  It is now %d.", deltaDex, newDex);
					g_Orion.CreateTextMessage(TT_SYSTEM, 0, 3, 0x0170, str);
				}

				if (deltaInt)
				{
					sprintf(str, "Your intelligence has changed by %d.  It is now %d.", deltaInt, newInt);
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
	/*char __cdecl HandleUpdateItem(_BYTE *buf)
	{
		__int16 y_; // ax@12
		char result; // al@18
		signed int graphic_; // ecx@22
		int offset; // [sp+8h] [bp-30h]@1
		int graphic; // [sp+Ch] [bp-2Ch]@3
		int count; // [sp+10h] [bp-28h]@1
		int serial; // [sp+14h] [bp-24h]@1
		int flags; // [sp+18h] [bp-20h]@1
		int color; // [sp+1Ch] [bp-1Ch]@1
		int direction; // [sp+20h] [bp-18h]@1
		int y; // [sp+24h] [bp-14h]@12
		int x; // [sp+28h] [bp-10h]@10
		int graphicIncrement; // [sp+2Ch] [bp-Ch]@1
		int v14; // [sp+30h] [bp-8h]@22
		int z; // [sp+34h] [bp-4h]@18

		offset = 0;
		LOBYTE(graphicIncrement) = 0;
		count = 0;
		LOBYTE(direction) = 0;
		color = 0;
		LOBYTE(flags) = 0;
		ReadInt32BE(buf, &offset, &serial);
		if (serial & 0x80000000)
		{
			serial &= 0x7FFFFFFFu;
			count = 1;
		}
		ReadInt16BE(buf, &offset, &graphic);
		if (graphic & 0x8000)
		{
			graphic &= 0x7FFFu;
			LOBYTE(graphicIncrement) = 1;
		}
		else if (!(_BYTE)graphicIncrement)
		{
			goto LABEL_7;
		}
		ReadInt8(buf, &offset, &graphicIncrement);
	LABEL_7:
		if ((_WORD)count)
			ReadInt16BE(buf, &offset, &count);
		else
			count = 1;
		ReadInt16BE(buf, &offset, &x);
		if (x & 0x8000)
		{
			x &= 0x7FFFu;
			LOBYTE(direction) = 1;
		}
		ReadInt16BE(buf, &offset, &y);
		y_ = y;
		if (y & 0x8000)
		{
			y_ = y & 0x7FFF;
			color = 1;
			y &= 0x7FFFu;
		}
		if (y_ & 0x4000)
		{
			LOBYTE(flags) = 1;
			y = y_ & 0x3FFF;
		}
		if ((_BYTE)direction)
			ReadInt8(buf, &offset, &direction);
		result = ReadInt8(buf, &offset, &z);
		if ((_WORD)color)
			result = ReadInt16BE(buf, &offset, &color);
		if ((_BYTE)flags)
			result = ReadInt8(buf, &offset, &flags);
		graphic_ = graphic;
		LOBYTE(v14) = 0;
		if ((unsigned __int16)graphic >= 0x4000u)
		{
			graphic_ = graphic + 0xC000;
			graphic += 0xC000;
			LOBYTE(v14) = 1;
		}
		if (serial != g_PlayerSerial)
			result = UpdateGameObject(
			serial,
			graphic_,
			graphicIncrement,
			count,
			(unsigned __int16)x,
			(unsigned __int16)y,
			z,
			direction,
			color,
			flags,
			count,
			v14,
			1);
		return result;
	}*/
	WISPFUN_DEBUG("c150_f27");
	if (g_World == NULL)
		return;

	uint serial = ReadUInt32BE();
	ushort graphic = ReadUInt16BE();

	if (g_TheAbyss && (graphic & 0x7FFF) == 0x0E5C)
		return;

	ushort count = 1;

	if (serial & 0x80000000)
	{
		serial &= 0x7FFFFFFF;
		count = ReadUInt16BE();
	}

	CGameItem *obj = g_World->GetWorldItem(serial);
	if (obj == NULL)
	{
		LOG("no memory??");
		return;
	}

	if (g_ObjectInHand != NULL && g_ObjectInHand->Serial == obj->Serial && g_ObjectInHand->Dropped)
	{
		delete g_ObjectInHand;
		g_ObjectInHand = NULL;
	}

	g_World->RemoveFromContainer(obj);
	obj->Container = 0xFFFFFFFF;
	g_World->m_Items->AddObject(obj);

	if (obj->Dragged)
		g_GumpManager.CloseGump(serial, 0, GT_DRAG);

	obj->Dragged = false;
	obj->MapIndex = g_CurrentMap;

	if (graphic & 0x8000)
	{
		graphic &= 0x7FFF;
		graphic += ReadUInt8();
	}

	if (!obj->Graphic)
		LOG("created ");
	else
		LOG("updated ");

	obj->MultiBody = (graphic & 0x4000);
	ushort oldGraphic = obj->Graphic;
	obj->Graphic = graphic & 0x3FFF;

	if (graphic == 0x2006 && !count)
		count = 1;

	obj->Count = count;
	ushort x = ReadUInt16BE();
	ushort y = ReadUInt16BE();
	uchar dir = 0;

	if (x & 0x8000)
	{
		x &= 0x7FFF;

		//obj->Direction = *Ptr; //Направление предмета?
		dir = ReadUInt8();
	}

	uchar z = ReadUInt8();

	if (y & 0x8000)
	{
		y &= 0x7FFF;

		obj->Color = ReadUInt16BE();
	}
	else
		obj->Color = 0;

	if (y & 0x4000)
	{
		y &= 0x3FFF;
		obj->Flags = ReadUInt8();
	}
	else
		obj->Flags = 0;

	if (obj->MultiBody)
		obj->WantUpdateMulti = ((obj->m_Items == NULL) || (oldGraphic != obj->Graphic) || (obj->X != x) || (obj->Y != y) || (obj->Z != z));

	obj->X = x;
	obj->Y = y;
	obj->Z = z;

	obj->OnGraphicChange(dir);

	g_World->MoveToTop(obj);

	if (m_ClientVersion >= CV_308Z && !obj->ClilocMessage.length())
	{
		m_MegaClilocRequests.push_back(obj->Serial);
		SendMegaClilocRequests();
	}

	LOG("0x%08lX:0x%04X*%d %d:%d:%d\n", serial, graphic, obj->Count, obj->X, obj->Y, obj->Z);
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(UpdateItemSA)
{
	/*int __cdecl HandleUpdateItemSA(_BYTE *buf, int offset)
	{
		unsigned __int16 y; // [sp+8h] [bp-38h]@1
		unsigned __int16 x; // [sp+Ch] [bp-34h]@1
		char unused; // [sp+10h] [bp-30h]@1
		int v6; // [sp+14h] [bp-2Ch]@1
		int v7; // [sp+18h] [bp-28h]@1
		int flags; // [sp+1Ch] [bp-24h]@1
		int color; // [sp+20h] [bp-20h]@1
		int direction; // [sp+24h] [bp-1Ch]@1
		int count; // [sp+28h] [bp-18h]@1
		int graphicIncrement; // [sp+2Ch] [bp-14h]@1
		int updateType; // [sp+30h] [bp-10h]@1
		int z; // [sp+34h] [bp-Ch]@1
		int graphic; // [sp+38h] [bp-8h]@1
		int serial; // [sp+3Ch] [bp-4h]@1

		LOBYTE(graphicIncrement) = 0;
		count = 0;
		v7 = 0;
		LOBYTE(direction) = 0;
		color = 0;
		LOBYTE(flags) = 0;
		v6 = -1;
		ReadInt16BE(buf, (int *)offset, &unused);
		ReadInt8(buf, (int *)offset, &updateType);
		ReadInt32BE(buf, (int *)offset, &serial);
		ReadInt16BE(buf, (int *)offset, &graphic);
		ReadInt8(buf, (int *)offset, &graphicIncrement);
		ReadInt16BE(buf, (int *)offset, &count);
		ReadInt16BE(buf, (int *)offset, &v7);
		ReadInt16BE(buf, (int *)offset, &x);
		ReadInt16BE(buf, (int *)offset, &y);
		ReadInt8(buf, (int *)offset, &z);
		ReadInt8(buf, (int *)offset, &direction);
		ReadInt16BE(buf, (int *)offset, &color);
		ReadInt8(buf, (int *)offset, &flags);
		ReadInt16BE_(buf, (int *)offset, &v6);
		return UpdateGameObject(
			serial,
			graphic,
			graphicIncrement,
			count,
			x,
			y,
			z,
			direction,
			color,
			flags,
			v7,
			updateType,
			v6);
	}*/
	WISPFUN_DEBUG("c150_f28");
	if (g_World == NULL)
		return;

	Move(2);
	char type = ReadUInt8();
	uint serial = ReadUInt32BE();
	ushort graphic = ReadUInt16BE();
	uchar dir = ReadUInt8();
	ushort count = ReadUInt16BE();
	Move(2);
	ushort x = ReadUInt16BE();
	ushort y = ReadUInt16BE();
	char z = ReadUInt8();
	Move(1);
	ushort color = ReadUInt16BE();
	char flags = ReadUInt8();

	if (g_ObjectInHand != NULL && g_ObjectInHand->Serial == serial)
	{
		return;

		//delete g_ObjectInHand;
		//g_ObjectInHand = NULL;
	}

	CGameItem *obj = g_World->GetWorldItem(serial);

	if (obj == NULL)
	{
		LOG("no memory??");
		return;
	}

	g_World->RemoveFromContainer(obj);
	obj->Container = 0xFFFFFFFF;
	g_World->m_Items->AddObject(obj);

	if (obj->Dragged)
		g_GumpManager.CloseGump(serial, 0, GT_DRAG);

	obj->Dragged = false;
	obj->MapIndex = g_CurrentMap;

	if (!obj->Graphic)
		LOG("created ");
	else
		LOG("updated ");

	obj->MultiBody = (type == 2);
	ushort oldGraphic = obj->Graphic;
	obj->Graphic = graphic;
	obj->Count = count;

	if (obj->MultiBody)
		obj->WantUpdateMulti = ((obj->m_Items == NULL) || (oldGraphic != obj->Graphic) || (obj->X != x) || (obj->Y != y) || (obj->Z != z));

	obj->X = x;
	obj->Y = y;
	obj->Z = z;

	obj->Color = color;

	obj->OnGraphicChange(dir);

	obj->Flags = flags;

	LOG("0x%08lX:0x%04X*%d %d:%d:%d\n", serial, obj->Graphic, obj->Count, obj->X, obj->Y, obj->Z);

	if (m_ClientVersion >= CV_308Z && !obj->ClilocMessage.length())
	{
		m_MegaClilocRequests.push_back(obj->Serial);
		SendMegaClilocRequests();
	}

	g_World->MoveToTop(obj);
}
//----------------------------------------------------------------------------------
void CPacketManager::UpdateGameObject(const uint &serial, ushort graphic, const uchar &graphicIncrement, const int &count, const int &x, const int &y, const char &z, const uchar &direction, const ushort &color, const uchar &flags, const int &a11, const char &updateType, const ushort &a13)
{
	ushort multiGraphic = 0xFFFF;

	if (updateType == 2)
	{
		multiGraphic = graphic;
		graphic = 1;
	}

	CGameCharacter *character = NULL;
	CGameItem *item = NULL;
	CGameObject *obj = g_World->FindWorldObject(serial);

	if (obj == NULL)
	{
		if (!(serial & 0x40000000) && updateType != 3)
		{
			character = g_World->GetWorldCharacter(serial);

			if (character == NULL)
			{
				LOG("No memory?\n");
				return;
			}

			character->Direction = direction;
			character->Color = g_ColorManager.FixColor(color, (color & 0x8000));
			character->X = x;
			character->Y = y;
			character->Z = z;
			character->Flags = flags;
		}
		else
		{
			item = g_World->GetWorldItem(serial);

			if (item == NULL)
			{
				LOG("No memory?\n");
				return;
			}

			if (graphic == 0x2006)
				item->Layer = direction;
		}

		if ((_WORD)graphic == 0x2006)
		{
			corpseMem = (CGameCorpse *)j__malloc(0x188u);
			v68 = 0;
			if (corpseMem)
			{
				v22 = (CGameObject *)CGameCorpse__CGameCorpse(corpseMem, serial, 0x2006, count);
				v68 = -1;
			LABEL_48:
				obj = (CGameCharacter *)v22;
				v22->Direction = direction;
				v26 = color & 0x8000;
				v27 = color & 0x4000;
				v28 = color & 0x3FFF;
				if (color & 0x3FFF)
				{
					if ((unsigned int)v28 > 0xBB7)
						v27 |= v26 | 1;
					else
						LOWORD(v27) = v26 | v27 | v28;
				}
				else
				{
					LOWORD(v27) = color & 0x8000;
				}
				obj->GameObject.Color = v27;
				if (dword_739F58
					&& (v29 = obj->GameObject.Graphic,
					LOWORD(v29) = v29 + 0x4000,
					(unsigned __int8)(*(int(__stdcall **)(int))(*(_DWORD *)dword_739F58 + 4))(v29))
					&& !obj->GameObject.Color)
				{
					LOWORD(v30) = sub_452670(dword_739F58, a13, (int)obj, obj->GameObject.Graphic);
				}
				else
				{
					v31 = color & 0x8000;
					v30 = color & 0x4000;
					v32 = color & 0x3FFF;
					if (color & 0x3FFF)
					{
						if ((unsigned int)v32 > 0xBB7)
							v30 |= v31 | 1;
						else
							LOWORD(v30) = v31 | v30 | v32;
					}
					else
					{
						LOWORD(v30) = color & 0x8000;
					}
				}
				obj->GameObject.Color2 = v30;
				v63 = flags;
				v62 = obj;
				obj->GameObject.Count = count;
				sub_5B1850(v62, v63);
				v16 = color;
				v65 = 1;
				obj->GameObject.field_A0 = updateType == 3;
				goto LABEL_66;
			}
		}
		else
		{
			v23 = 48 * (unsigned __int16)graphic;
			if (*(_DWORD *)((char *)g_StaticTiledata + v23) & UO_FLAG_MAP)
			{
				v24 = (CGameMap *)j__malloc(0xF8u);
				v68 = 1;
				if (v24)
				{
					v22 = (CGameObject *)CGameMap__CGameMap(v24, serial, graphic, graphicIncrement, 1);
					v68 = -1;
					goto LABEL_48;
				}
			}
			else if (sub_5CDC90((unsigned __int16)graphic))
			{
				v68 = 2;
				if (j__malloc(0x118u))
				{
					v22 = (CGameObject *)sub_5D3C20(serial, graphic, graphicIncrement, 1);
					v68 = -1;
					goto LABEL_48;
				}
			}
			else
			{
				if (!(*(_DWORD *)((char *)g_StaticTiledata + v23) & UO_FLAG_CONTAINER))
				{
					v22 = CreateCGameObject(
						(char)graphicIncrement,
						a13,
						serial,
						(unsigned __int16)graphic,
						graphicIncrement,
						a13 != 0);
					goto LABEL_48;
				}
				v25 = (CGameContainer *)j__malloc(0x108u);
				v68 = 3;
				if (v25)
				{
					v22 = (CGameObject *)CGameContainer__CGameContainer(v25, serial, graphic, graphicIncrement, 1);
					v68 = -1;
					goto LABEL_48;
				}
			}
		}
		v22 = 0;
		v68 = -1;
		goto LABEL_48;
	}

	/*
		v57 = 1;
		multiGraphic = -1;
		isMapObject = 0;
		if (g_PlayerX < 5120)
		{
			if (g_PlayerX < 18 && x > 5102)
				x -= 5120;
			if (g_PlayerX >= 5102 && x <= 18)
				x += 5120;
			if (g_PlayerY < 18 && y > 4078)
				y -= 4096;
			if (g_PlayerY >= 4078 && y <= 18)
				y += 4096;
		}
		graphic_ = graphic;
		if ((unsigned __int16)graphic >= 0x4000u)
		{
			multiGraphic = (unsigned __int16)graphic - 0x4000;
			graphic_ = 1;
			graphic = 1;
		}
		obj = (CGameCharacter *)g_Player;
		if (!g_Player || serial != g_Player->GameObject.GameObject.Serial)
		{
			obj = (CGameCharacter *)g_WorldObjectFirstPtr;
			if (!g_WorldObjectFirstPtr)
				goto LABEL_23;
			while (obj->GameObject.Serial != serial)
			{
				obj = (CGameCharacter *)obj->GameObject.NextObject;
				if (!obj)
					goto LABEL_23;
			}
		}
		if (!obj)
		{
		LABEL_23:
			if (!(serial & 0x40000000))               // !GameItem (NPC)
			{
				npcMem = (CGameCharacter *)operator new(0x1E0u);
				LOBYTE(v57) = 6;
				if (npcMem)
					obj = CMobile__CMobile(npcMem, serial, graphic, graphicIncrement);
				else
					obj = 0;
				obj->OnMount = (unsigned int)direction >> 7;
				obj->GameObject.DirectionOrLightIndex = direction & 0x7F;
				partialHue = color & 0x8000;
				spectralColor = color & 0x4000;
				realColor = color & 0x3FFF;
				LOBYTE(v57) = 1;
				obj->OffsetX = 0;
				obj->OffsetY = 0;
				if (color & 0x3FFF)
				{
					if (realColor >= 3000)
						spectralColor |= partialHue | 1;
					else
						LOWORD(spectralColor) = partialHue | spectralColor | realColor;
				}
				else
				{
					LOWORD(spectralColor) = color & 0x8000;
				}
				obj->GameObject.Color = spectralColor;
				LOWORD(xy) = x;
				HIWORD(xy) = y;
				v56 = z;
				vTable_1 = obj->GameObject.VTable;
				obj->OffsetZOfSomething = z;
				LOWORD(flags_) = v56;
				((void(__thiscall *)(CGameCharacter *, CGameCharacter *, signed int))vTable_1[UO_ROFUN_INSERT_IN_LISTS])(
					obj,
					xy,
					flags_);
				SetCharacterFlags(obj, flags);
				obj->field_14C = 0;
				goto LABEL_58;
			}
			if (graphic_ == 0x2006)                   // corpse
			{
				corpseMem = (CGameCharacter *)operator new(0x128u);
				obj = corpseMem;
				LOBYTE(v57) = 2;
				if (corpseMem)
				{
					CObj__CObj((CStaticObject *)corpseMem);
					v13 = obj->GameObject.ValidateBits;
					flags_ = 1229;
					obj_1 = (CGameCharacter *)aCProjectsUob_3;
					LOBYTE(v57) = 3;
					obj->GameObject.VTable = (int *)&VTable_CGameContainer;
					obj->FirstContainedItem = 0;
					obj->GumpForItem = 0;
					obj->DrawX_Maybe = 0;
					obj->field_BC = 0;
					if (!ValidateObject((int)obj, v13, (int)obj_1, flags_))
						TraceUOReportError(aCobjCobjEither);
					obj->GameObject.VTable = (int *)&VTable_CCorpse;
					memset(&obj->NextCharacterObject, 0, 0x68u);
					obj->GameObject.DirectionOrLightIndex = 0;
					obj->GameObject.Serial = serial;
					obj->GameObject.Graphic = 8198;
					LOBYTE(v57) = 1;
				}
				else
				{
					obj = 0;
					LOBYTE(v57) = 1;
				}
				goto LABEL_39;
			}
			graphic_1 = graphic_;
			tiledataFlags = g_StaticTiledata[graphic_1].Flags;
			if (tiledataFlags & UO_FLAG_MAP)          // map
			{
				mapMem = operator new(0xB8u);
				LOBYTE(v57) = 4;
				if (mapMem)
				{
					obj_ = (CGameCharacter *)CGameObjectMap__CGameObjectMap(mapMem, serial, graphic, graphicIncrement, 1);
					LOBYTE(v57) = 1;
				LABEL_38:
					obj = obj_;
				LABEL_39:
					obj->GameObject.DirectionOrLightIndex = direction;
					v19 = color & 0x8000;
					v20 = color & 0x4000;
					v21 = color & 0x3FFF;
					if (color & 0x3FFF)
					{
						if (v21 >= 3000)
							v19 |= v20 | 1;
						else
							LOWORD(v19) = v20 | v19 | v21;
					}
					obj->GameObject.Color = v19;
					obj->GameObject.Count = count;
					SetCharacterFlags(obj, flags);
					isMapObject = 1;
					goto LABEL_58;
				}
			}
			else
			{
				if (!(tiledataFlags & UO_FLAG_CONTAINER))// !container
				{
					obj_ = (CGameCharacter *)CreateGameObject(serial, graphic_1, graphicIncrement);
					goto LABEL_38;
				}
				contMem = (CGameContainer *)operator new(0xC0u);
				LOBYTE(v57) = 5;
				if (contMem)
				{
					obj_ = (CGameCharacter *)CGameContainer__CGameContainer(contMem, serial, graphic, graphicIncrement, 1);
					LOBYTE(v57) = 1;
					goto LABEL_38;
				}
			}
			obj_ = 0;
			LOBYTE(v57) = 1;
			goto LABEL_38;
		}
		if (!ValidateObject((int)obj, obj->GameObject.ValidateBits, (int)aCProjectsUob_3, 2705))
			TraceUOReportError(aCobjectmanag_1);
		if (!(*(int(__thiscall **)(CGameCharacter *))((int(__thiscall **)(_DWORD))obj->GameObject.VTable + UO_ROFUN_IS_NPC))(obj)
			&& !obj->GameObject.Multi)
			(*(void(__thiscall **)(CGameCharacter *))((void(__thiscall **)(_DWORD))obj->GameObject.VTable
			+ UO_ROFUN_REMOVE_FROM_LISTS))(obj);
	LABEL_58:
		if (obj)
			validateBits_ = obj->GameObject.ValidateBits;
		else
			validateBits_ = 0xFEEDBEEF;
		if (!ValidateObject((int)obj, validateBits_, (int)aCProjectsUob_3, 2721))
			TraceUOReportError(aCobjectmanag_1);
		if (!(*(int(__thiscall **)(CGameCharacter *))((int(__thiscall **)(_DWORD))obj->GameObject.VTable + UO_ROFUN_IS_NPC))(obj))
		{
			LOWORD(xy) = x;
			HIWORD(xy) = y;
			v56 = z;
			obj->GameObject.GraphicIncrement = graphicIncrement;
			obj->GameObject.DirectionOrLightIndex = direction;
			obj->GameObject.Count = count;
			SetCharacterFlags(obj, flags);
			v28 = color & 0x8000;
			v29 = color & 0x4000;
			v30 = color & 0x3FFF;
			if (color & 0x3FFF)
			{
				if (v30 >= 3000)
					v28 |= v29 | 1;
				else
					v28 |= v29 | v30;
			}
			multiGraphic_ = multiGraphic;
			obj->GameObject.Color = v28;
			if (multiGraphic_ == -1)
			{
				vTable = obj->GameObject.VTable;
				obj->GameObject.Graphic = graphic & 0x3FFF;
				obj_1 = xy;
				LOWORD(flags_) = v56;
				((void(__thiscall *)(CGameCharacter *, CGameCharacter *, signed int))vTable[UO_ROFUN_INSERT_IN_LISTS])(
					obj,
					xy,
					flags_);
			}
			else
			{
				LoadMulti(&obj->GameObject, &xy, multiGraphic_);
			}
			goto LABEL_114;
		}
		obj->LastFidgetAnimationTimer = g_LastRenderTime;
		if (obj->StepsCount != 4)
		{
			if (obj->field_14C)
			{
				if (obj->GameObject.InRenderQueue)
				{
					(*(void(__thiscall **)(CGameCharacter *))((void(__thiscall **)(_DWORD))obj->GameObject.VTable
						+ UO_ROFUN_REMOVE_FROM_RENDER))(obj);
					vTable_2 = obj->GameObject.VTable;
					obj->field_14C = 0;
					((void(__thiscall *)(CGameCharacter *))vTable_2[UO_ROFUN_UPDATE_RENDER_AND_DRAW_POS])(obj);
				}
				else
				{
					obj->field_14C = 0;
				}
			}
			stepsCount = obj->StepsCount;
			if (stepsCount)
			{
				x_ = x;
				v33 = *((_WORD *)&obj->field_14C + 5 * stepsCount + 1);
				v41 = (char *)obj + 10 * stepsCount;
				if (v33 == x && *((_WORD *)v41 + 168) == y && *((_WORD *)v41 + 169) == z && v41[342] == direction)
				{
					LOWORD(v33) = *((_WORD *)&obj->GameObject.VTable + 5 * stepsCount + 170);
					if ((_WORD)v33 == (_WORD)graphic)
					{
						LOBYTE(v33) = flags;
						if (v41[343] == flags)
							goto LABEL_101;
					}
				}
			}
			else
			{
				x_ = x;
				if (obj->GameObject.X == x
					&& obj->GameObject.Y == y
					&& obj->GameObject.Z == z
					&& !((direction ^ obj->GameObject.DirectionOrLightIndex) & 0x7F))
				{
					v33 = (unsigned int)direction >> 7;
					if ((obj->OnMount != 0) == v33 && obj->GameObject.Graphic == (_WORD)graphic)
					{
						LOBYTE(v33) = obj->GameObject.Flags;
						if ((_BYTE)v33 == flags)
							goto LABEL_101;
					}
				}
			}
			obj->StepStack[stepsCount].X = x_;
			obj->StepStack[obj->StepsCount].Y = y;
			obj->StepStack[obj->StepsCount].Z = z;
			obj->StepStack[obj->StepsCount].Direction = direction;
			v42 = obj->StepsCount;
			v33 = 5 * (v42 + 35);
			*((_WORD *)&obj->GameObject.VTable + 5 * (v42 + 35)) = graphic;
			LOBYTE(v33) = flags;
			obj->StepStack[obj->StepsCount++].Flags = flags;
			goto LABEL_101;
		}
		inRenderQueue = 0;
		if (obj->GameObject.InRenderQueue)
		{
			(*(void(__thiscall **)(CGameCharacter *))((void(__thiscall **)(_DWORD))obj->GameObject.VTable
				+ UO_ROFUN_REMOVE_FROM_RENDER))(obj);
			inRenderQueue = 1;
		}
		obj->GameObject.X = x;
		obj->GameObject.Graphic = graphic & 0x3FFF;
		obj->GameObject.Y = y;
		obj->GameObject.Z = z;
		v35 = color & 0x8000;
		v36 = color & 0x4000;
		v37 = color & 0x3FFF;
		obj->PixelOffset = 1;
		obj->GameObject.DirectionOrLightIndex = direction & 0x7F;
		if (color & 0x3FFF)
		{
			if (v37 >= 3000)
				v36 |= v35 | 1;
			else
				LOWORD(v36) = v35 | v36 | v37;
		}
		else
		{
			LOWORD(v36) = color & 0x8000;
		}
		obj->GameObject.Color = v36;
		flags_ = flags;
		obj_1 = obj;
		obj->StepsCount = 0;
		SetCharacterFlags(obj_1, flags_);
		obj->field_14C = 0;
		if (inRenderQueue)
			(*(void(__thiscall **)(CGameCharacter *))((void(__thiscall **)(_DWORD))obj->GameObject.VTable
			+ UO_ROFUN_UPDATE_RENDER_AND_DRAW_POS))(obj);
	LABEL_101:
		if (g_Player->IsFollowing)
		{
			v33 = g_Player->FollowingSerial_Maybe;
			if (obj->GameObject.Serial == v33 && !CheckObjectDistance(&obj->GameObject, 3))
			{
				v43 = obj->StepsCount;
				if (v43 <= 0)
				{
					multiGraphic = *(_DWORD *)&obj->GameObject.X;
					v44 = obj->GameObject.Z;
				}
				else
				{
					multiGraphic = *(int *)((char *)&obj->field_14C + 10 * v43 + 2);
					v44 = *((_WORD *)&obj->LastFidgetAnimationTimer + 5 * v43 + 1);
				}
				v53 = v44;
				if (g_Pathfinding)
					sub_4A64F0();
				v45 = sub_4A6530(
					g_Player->GameObject.GameObject.X,
					g_Player->GameObject.GameObject.Y,
					g_Player->GameObject.GameObject.Z,
					(signed __int16)multiGraphic,
					SHIWORD(multiGraphic),
					v44,
					100,
					1);
				dword_CC9380 = v45;
				if (!v45)
				{
					result = (int)CreateSystemMessageASCII(0, 3, aCanTGetThereFo);
					g_Pathfinding = 0;
					return result;
				}
				g_Pathfinding = 1;
				v47 = v45;
				sub_476700();
				for (i = *(_DWORD *)(v47 + 36); i; i = *(_DWORD *)(i + 36))
					v47 = i;
				dword_CC9380 = v47;
				dword_CC9374 = *(_DWORD *)(v47 + 16);
				dword_CC9370 = *(_DWORD *)(v47 + 20);
				v33 = (int)g_Player;
				dword_CC936C = *(_DWORD *)(v47 + 24);
				v49 = g_Player->GameObject.GameObject.DirectionOrLightIndex;
				dword_CC937C = v47;
				dword_CC9368 = v49;
			}
		}
	LABEL_114:
		if (isMapObject == 1)
			sub_488C20(v33, obj);
		result = CheckObjectDistance(&obj->GameObject, 18);
		if (!result)
		{
			result = (*(int(__thiscall **)(CGameCharacter *))((int(__thiscall **)(_DWORD))obj->GameObject.VTable + UO_ROFUN_28))(obj);
			if (!result)
				result = (*(int(__thiscall **)(CGameCharacter *))((int(__thiscall **)(_DWORD))obj->GameObject.VTable
				+ UO_ROFUN_DESTRUCTOR_))(obj);
		}
		return result;
	}*/
}
//----------------------------------------------------------------------------------
/*void __cdecl UpdateGameObject(int serial, signed int graphic, int graphicIncrement, int count, int x, int y, char z, unsigned __int8 direction, unsigned __int16 color, unsigned __int8 flags, int a11, char updateType, __int16 a13)
{
	int v13; // ebx@1
	int v14; // eax@2
	CGameCharacter *obj; // esi@16
	__int16 v16; // bp@16
	CGameCharacter *characterMem; // eax@25
	__int16 v18; // di@28
	int v19; // eax@29
	int v20; // edx@33
	CGameCorpse *corpseMem; // eax@35
	CGameObject *v22; // eax@36
	int v23; // ebp@38
	CGameMap *v24; // eax@39
	CGameContainer *v25; // eax@45
	int v26; // edx@48
	int v27; // eax@48
	int v28; // ecx@48
	int v29; // edx@54
	int v30; // eax@56
	int v31; // ecx@57
	int v32; // edi@57
	int v33; // edx@63
	unsigned int v34; // eax@67
	int v35; // eax@70
	int v36; // ecx@70
	int v37; // edx@70
	int v38; // edx@81
	int v39; // eax@84
	__int16 v40; // cx@87
	__int16 v41; // ax@87
	int v42; // edi@87
	int v43; // eax@92
	int v44; // ebx@93
	__int16 v45; // bp@95
	int v46; // edx@95
	int v47; // eax@95
	int v48; // ecx@95
	__int16 v49; // ax@102
	__int16 v50; // ax@103
	int v51; // eax@115
	int v52; // eax@121
	char v53; // dl@121
	char v54; // zf@126
	char *v55; // ecx@127
	int v56; // eax@140
	int v57; // eax@141
	__int16 v58; // di@141
	int v59; // eax@145
	int v60; // edi@147
	int i; // eax@147
	CGameCharacter *v62; // [sp-8h] [bp-3Ch]@62
	signed int v63; // [sp-4h] [bp-38h]@28
	int v64; // [sp+14h] [bp-20h]@1
	int v65; // [sp+1Ch] [bp-18h]@1
	int v66; // [sp+20h] [bp-14h]@1
	int v67; // [sp+24h] [bp-10h]@1
	int v68; // [sp+30h] [bp-4h]@25
	char updateTypea; // [sp+64h] [bp+30h]@121

	v13 = a13;
	v66 = 0;
	LOWORD(v67) = 0;
	v65 = 0;
	v64 = -1;
	if (g_PlayerX < g_MapWidth)
	{
		v14 = sub_5E1510();
		if (g_PlayerX < v14 && x > g_MapWidth - v14)
			x -= g_MapWidth;
		if (g_PlayerX >= g_MapWidth - v14 && x <= v14)
			x += g_MapWidth;
		if (g_PlayerY < v14 && y > g_MapHeight - v14)
			y -= g_MapHeight;
		if (g_PlayerY >= g_MapHeight - v14 && y <= v14)
			y += g_MapHeight;
	}
	if (updateType == 2)
	{
		v64 = (unsigned __int16)graphic;
		graphic = 1;
	}
	obj = (CGameCharacter *)g_Player;
	v16 = color;
	if (!g_Player || serial != g_Player->GameCharacter.GameObject.Serial)
	{
		obj = (CGameCharacter *)g_WorldObjectFirstPtr;
		if (!g_WorldObjectFirstPtr)
		{
		LABEL_23:
			if (!(serial & 0x40000000) && updateType != 3)
			{
				characterMem = (CGameCharacter *)j__malloc(0x298u);
				v13 = 0;
				v68 = 4;
				if (characterMem)
					obj = CGameCharacter__CGameCharacter(characterMem, serial, graphic, graphicIncrement);
				else
					obj = 0;
				v63 = color;
				v68 = -1;
				obj->field_1F0 = 0;
				obj->field_1F4 = 0;
				obj->GameObject.Direction = direction & 0x7F;
				obj->field_1A4 = (unsigned int)direction >> 7;
				v18 = FixColor(v63);
				obj->GameObject.Color = v18;
				if (dword_739F58
					&& (v19 = obj->GameObject.Graphic,
					LOWORD(v19) = v19 + 0x4000,
					(unsigned __int8)(*(int(__stdcall **)(int))(*(_DWORD *)dword_739F58 + 4))(v19))
					&& !obj->GameObject.Color)
					obj->GameObject.Color2 = sub_452670(dword_739F58, 0, (int)obj, obj->GameObject.Graphic);
				else
					obj->GameObject.Color2 = v18;
				LOWORD(v66) = x;
				HIWORD(v66) = y;
				LOWORD(v67) = z;
				obj->field_1F8 = z;
				v20 = obj->GameObject.VTable[25];
				LOWORD(v63) = v67;
				((void(__thiscall *)(CGameCharacter *, int, signed int))v20)(obj, v66, v63);
				sub_5B1850(obj, flags);
				goto LABEL_66;
			}
			if ((_WORD)graphic == 0x2006)
			{
				corpseMem = (CGameCorpse *)j__malloc(0x188u);
				v68 = 0;
				if (corpseMem)
				{
					v22 = (CGameObject *)CGameCorpse__CGameCorpse(corpseMem, serial, 0x2006, count);
					v68 = -1;
				LABEL_48:
					obj = (CGameCharacter *)v22;
					v22->Direction = direction;
					v26 = color & 0x8000;
					v27 = color & 0x4000;
					v28 = color & 0x3FFF;
					if (color & 0x3FFF)
					{
						if ((unsigned int)v28 > 0xBB7)
							v27 |= v26 | 1;
						else
							LOWORD(v27) = v26 | v27 | v28;
					}
					else
					{
						LOWORD(v27) = color & 0x8000;
					}
					obj->GameObject.Color = v27;
					if (dword_739F58
						&& (v29 = obj->GameObject.Graphic,
						LOWORD(v29) = v29 + 0x4000,
						(unsigned __int8)(*(int(__stdcall **)(int))(*(_DWORD *)dword_739F58 + 4))(v29))
						&& !obj->GameObject.Color)
					{
						LOWORD(v30) = sub_452670(dword_739F58, a13, (int)obj, obj->GameObject.Graphic);
					}
					else
					{
						v31 = color & 0x8000;
						v30 = color & 0x4000;
						v32 = color & 0x3FFF;
						if (color & 0x3FFF)
						{
							if ((unsigned int)v32 > 0xBB7)
								v30 |= v31 | 1;
							else
								LOWORD(v30) = v31 | v30 | v32;
						}
						else
						{
							LOWORD(v30) = color & 0x8000;
						}
					}
					obj->GameObject.Color2 = v30;
					v63 = flags;
					v62 = obj;
					obj->GameObject.Count = count;
					sub_5B1850(v62, v63);
					v16 = color;
					v65 = 1;
					obj->GameObject.field_A0 = updateType == 3;
					goto LABEL_66;
				}
			}
			else
			{
				v23 = 48 * (unsigned __int16)graphic;
				if (*(_DWORD *)((char *)g_StaticTiledata + v23) & UO_FLAG_MAP)
				{
					v24 = (CGameMap *)j__malloc(0xF8u);
					v68 = 1;
					if (v24)
					{
						v22 = (CGameObject *)CGameMap__CGameMap(v24, serial, graphic, graphicIncrement, 1);
						v68 = -1;
						goto LABEL_48;
					}
				}
				else if (sub_5CDC90((unsigned __int16)graphic))
				{
					v68 = 2;
					if (j__malloc(0x118u))
					{
						v22 = (CGameObject *)sub_5D3C20(serial, graphic, graphicIncrement, 1);
						v68 = -1;
						goto LABEL_48;
					}
				}
				else
				{
					if (!(*(_DWORD *)((char *)g_StaticTiledata + v23) & UO_FLAG_CONTAINER))
					{
						v22 = CreateCGameObject(
							(char)graphicIncrement,
							a13,
							serial,
							(unsigned __int16)graphic,
							graphicIncrement,
							a13 != 0);
						goto LABEL_48;
					}
					v25 = (CGameContainer *)j__malloc(0x108u);
					v68 = 3;
					if (v25)
					{
						v22 = (CGameObject *)CGameContainer__CGameContainer(v25, serial, graphic, graphicIncrement, 1);
						v68 = -1;
						goto LABEL_48;
					}
				}
			}
			v22 = 0;
			v68 = -1;
			goto LABEL_48;
		}
		while (obj->GameObject.Serial != serial)
		{
			obj = (CGameCharacter *)obj->GameObject.NextObject;
			if (!obj)
				goto LABEL_23;
		}
	}
	if (!obj)
		goto LABEL_23;
	v33 = obj->GameObject.ValidateBits;
	v63 = 0x10F4;
	v62 = (CGameCharacter *)byte_6C6AFD;
	ValidateObject((int)obj, v33);
	if (!(*(int(__thiscall **)(CGameCharacter *))((int(__thiscall **)(_DWORD))obj->GameObject.VTable + 9))(obj)
		&& !obj->GameObject.field_C8)
		(*(void(__thiscall **)(CGameCharacter *))((void(__thiscall **)(_DWORD))obj->GameObject.VTable + 20))(obj);
LABEL_66:
	if (obj)
		v34 = obj->GameObject.ValidateBits;
	else
		v34 = 0xFEEDBEEF;
	v63 = 0x1106;
	v62 = (CGameCharacter *)byte_6C6AFD;
	ValidateObject((int)obj, v34);
	if (!(*(int(__thiscall **)(CGameCharacter *))((int(__thiscall **)(_DWORD))obj->GameObject.VTable + 9))(obj))
	{
		obj->GameObject.field_A0 = updateType == 3;
		LOWORD(v66) = x;
		HIWORD(v66) = y;
		LOWORD(v67) = z;
		obj->GameObject.GraphicIncrement = graphicIncrement;
		v63 = flags;
		v62 = obj;
		obj->GameObject.Direction = direction;
		obj->GameObject.Count = count;
		sub_5B1850(v62, v63);
		v35 = v16 & 0x8000;
		v36 = v16 & 0x4000;
		v37 = v16 & 0x3FFF;
		if (v16 & 0x3FFF)
		{
			if ((unsigned int)v37 > 0xBB7)
				v35 |= v36 | 1;
			else
				v35 |= v36 | v37;
		}
		obj->GameObject.Color = v35;
		if (v64 == -1)
		{
			v13 = graphic;
			if (obj->GameObject.Graphic != (_WORD)graphic)
			{
				obj->GameObject.Graphic = graphic;
				obj->GameObject.field_94 = 0;
			}
			if (dword_739F58 && (unsigned __int8)(*(int(__stdcall **)(int))(*(_DWORD *)dword_739F58 + 4))(graphic + 0x4000))
				obj->GameObject.Graphic2 = sub_446CA0(dword_739F58, graphic, (unsigned __int16)graphic + 0x4000, 0) & 0x3FFF;
			else
				obj->GameObject.Graphic2 = graphic;
			v38 = obj->GameObject.VTable[25];
			LOWORD(v63) = v67;
			((void(__thiscall *)(CGameCharacter *, int, signed int))v38)(obj, v66, v63);
		}
		else
		{
			sub_59D910((int)obj, (int)&v66, v64);
			sub_59C820(obj, (unsigned __int16)v16);
		}
		if (dword_739F58
			&& (v39 = obj->GameObject.Graphic,
			LOWORD(v39) = v39 + 0x4000,
			(unsigned __int8)(*(int(__stdcall **)(int))(*(_DWORD *)dword_739F58 + 4))(v39))
			&& !obj->GameObject.Color)
		{
			obj->GameObject.Color2 = sub_452670(dword_739F58, v13, (int)obj, obj->GameObject.Graphic);
		}
		else
		{
			v40 = v16 & 0x8000;
			v41 = v16 & 0x4000;
			v42 = v16 & 0x3FFF;
			if (v16 & 0x3FFF)
			{
				if ((unsigned int)v42 > 0xBB7)
					obj->GameObject.Color2 = v40 | v41 | 1;
				else
					obj->GameObject.Color2 = v40 | v41 | v42;
			}
			else
			{
				obj->GameObject.Color2 = v40;
			}
		}
		goto LABEL_150;
	}
	v43 = sub_45CFB0();
	v54 = obj->field_1EC == 4;
	obj->field_1AC = v43;
	if (v54)
	{
		v44 = 0;
		if (obj->GameObject.InRenderQueue)
		{
			(*(void(__thiscall **)(CGameCharacter *))((void(__thiscall **)(_DWORD))obj->GameObject.VTable + 5))(obj);
			v44 = 1;
		}
		v45 = graphic;
		obj->GameObject.X = x;
		v63 = graphic;
		obj->field_19C = 1;
		obj->GameObject.Y = y;
		obj->GameObject.Z = z;
		obj->GameObject.Graphic = graphic;
		sub_5B01F0((int)&obj->field_108, v63);
		UpdateRace(obj);
		obj->GameObject.Direction = direction & 0x7F;
		v46 = color & 0x8000;
		v47 = color & 0x4000;
		v48 = color & 0x3FFF;
		if (color & 0x3FFF)
		{
			if ((unsigned int)v48 > 0xBB7)
				v47 |= v46 | 1;
			else
				LOWORD(v47) = v46 | v47 | v48;
		}
		else
		{
			LOWORD(v47) = color & 0x8000;
		}
		obj->GameObject.Color = v47;
		obj->field_1EC = 0;
		if (dword_739F58)
		{
			if ((unsigned __int8)(*(int(__stdcall **)(int))(*(_DWORD *)dword_739F58 + 4))(graphic + 0x4000))
			{
				v49 = sub_446CA0(dword_739F58, v44, (unsigned __int16)graphic + 0x4000, 0);
				v54 = obj->GameObject.Color == 0;
				obj->GameObject.Graphic2 = v49 & 0x3FFF;
				if (v54)
				{
					v50 = sub_452670(dword_739F58, v44, (int)obj, graphic);
					goto LABEL_107;
				}
			LABEL_106:
				v50 = FixColor(color);
			LABEL_107:
				v63 = flags;
				v62 = obj;
				obj->GameObject.Color2 = v50;
				sub_5B1850(v62, v63);
				obj->field_1A8 = 0;
				if (v44)
					(*(void(__thiscall **)(CGameCharacter *))((void(__thiscall **)(_DWORD))obj->GameObject.VTable + 4))(obj);
				goto LABEL_137;
			}
			v45 = graphic;
		}
		obj->GameObject.Graphic2 = v45;
		goto LABEL_106;
	}
	if (obj->GameObject.Color != v16)
	{
		v63 = (unsigned __int16)v16;
		obj->GameObject.Color = v16;
		obj->GameObject.Color2 = FixColor(v63);
	}
	if (obj->GameObject.Flags != flags)
		sub_5B1850(obj, flags);
	if (obj->GameObject.Graphic != (_WORD)graphic)
	{
		v63 = graphic;
		obj->GameObject.Graphic = graphic;
		sub_5B01F0((int)&obj->field_108, v63);
		UpdateRace(obj);
		if (dword_739F58
			&& (v51 = obj->GameObject.Graphic,
			LOWORD(v51) = v51 + 0x4000,
			(unsigned __int8)(*(int(__stdcall **)(int))(*(_DWORD *)dword_739F58 + 4))(v51)))
			obj->GameObject.Graphic2 = sub_446CA0(dword_739F58, v13, obj->GameObject.Graphic + 0x4000, 0) & 0x3FFF;
		else
			obj->GameObject.Graphic2 = obj->GameObject.Graphic;
	}
	if (obj->field_1A8 && obj->GameObject.InRenderQueue)
	{
		(*(void(__thiscall **)(CGameCharacter *))((void(__thiscall **)(_DWORD))obj->GameObject.VTable + 5))(obj);
		(*(void(__thiscall **)(CGameCharacter *))((void(__thiscall **)(_DWORD))obj->GameObject.VTable + 4))(obj);
	}
	v52 = obj->field_1EC;
	v53 = 1;
	updateTypea = 1;
	if (v52)
	{
		v55 = (char *)obj + 12 * v52;
		if (*((_WORD *)&obj->field_1A8 + 6 * v52) == x && *((_WORD *)v55 + 213) == y && *((_WORD *)v55 + 214) == z)
			v53 = 0;
		v54 = *((_BYTE *)&obj->GameObject.VTable + 4 * (3 * v52 + 108)) == direction;
	}
	else
	{
		if (obj->GameObject.X == x && obj->GameObject.Y == y && obj->GameObject.Z == z)
			v53 = 0;
		v54 = ((direction ^ obj->GameObject.Direction) & 0x7F) == 0;
	}
	if (v54)
		updateTypea = 0;
	if (v53 || updateTypea)
	{
		obj->field_1A8 = v53 == 0;
		*((_WORD *)&obj->field_1B4 + 6 * v52) = x;
		*((_WORD *)&obj->field_1B4 + 6 * obj->field_1EC + 1) = y;
		*((_WORD *)&obj->field_1B8 + 6 * obj->field_1EC) = z;
		*((_BYTE *)&obj->GameObject.VTable + 12 * (obj->field_1EC++ + 37)) = direction;
	}
LABEL_137:
	if (g_Player->field_2AC
		&& obj->GameObject.Serial == g_Player->field_2B4
		&& !(*(int(__thiscall **)(CGameCharacter *, signed int))((int(__thiscall **)(_DWORD, _DWORD))obj->GameObject.VTable
		+ 22))(
		obj,
		3))
	{
		v56 = obj->field_1EC;
		if (v56 <= 0)
		{
			v58 = obj->GameObject.Z;
			v64 = *(_DWORD *)&obj->GameObject.X;
		}
		else
		{
			v57 = 3 * v56;
			v58 = *((_WORD *)&obj->field_1AC + 2 * v57);
			v64 = *(&obj->field_1A8 + v57);
		}
		if (g_Pathfinding)
			sub_5C3CB0();
		v63 = 1;
		v62 = (CGameCharacter *)100;
		v59 = sub_5C4300(
			g_Player->GameCharacter.GameObject.X,
			g_Player->GameCharacter.GameObject.Y,
			g_Player->GameCharacter.GameObject.Z,
			(signed __int16)v64,
			SHIWORD(v64),
			v58,
			100,
			1);
		dword_AE72B0 = v59;
		if (!v59)
		{
			CreateSystemMessageASCII(0, 3, "Can't get there (follow)");
			g_Pathfinding = 0;
			return;
		}
		g_Pathfinding = 1;
		v60 = v59;
		sub_503C40();
		for (i = *(_DWORD *)(v60 + 36); i; i = *(_DWORD *)(i + 36))
			v60 = i;
		dword_AE72B0 = v60;
		dword_AE72A4 = *(_DWORD *)(v60 + 16);
		dword_AE72A0 = *(_DWORD *)(v60 + 20);
		dword_AE729C = *(_DWORD *)(v60 + 24);
		dword_AE7298 = g_Player->GameCharacter.GameObject.Direction;
		dword_AE72AC = v60;
	}
LABEL_150:
	if (v65 == 1)
		sub_5ADA80(obj);
	if ((*(int(__thiscall **)(CGameCharacter *))((int(__thiscall **)(_DWORD))obj->GameObject.VTable + 23))(obj)
		|| (*(int(__thiscall **)(CGameCharacter *))((int(__thiscall **)(_DWORD))obj->GameObject.VTable + 33))(obj))
		sub_534560(&obj->GameObject);
	else
		(*((void(**)(void))obj->GameObject.VTable + 1))();
}*/
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
		//UpdateGameObject(serial, graphic, 0, 0, x, y, z, direction, color, flags, 0, 0, 1);
	}

	CGameObject *obj = g_World->FindWorldObject(serial);

	obj->ClearNotOpenedItems();
	obj->MapIndex = g_CurrentMap;

	CGameCharacter *character = NULL;
	bool resuestStatus = false;

	if (obj != NULL && obj->NPC)
	{
		character = (CGameCharacter*)obj;
		character->Notoriety = notoriety;

		if (!obj->Graphic)
			resuestStatus = (g_GumpManager.UpdateContent(serial, 0, GT_STATUSBAR) != NULL) || (g_GumpManager.UpdateContent(serial, 0, GT_TARGET_SYSTEM) != NULL) || g_ConfigManager.DrawStatusState || (serial == g_LastTargetObject) || (serial == g_LastAttackObject);
	}

	if (*m_Start != 0x78)
		Move(6);

	uint itemSerial = ReadUInt32BE();

	g_World->MoveToTop(obj);

	while (itemSerial != 0 && character != NULL && !IsEOF())
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

		item->Graphic = graphic;
		item->Color = g_ColorManager.FixColor(color);

		g_World->PutEquipment(item, obj, layer);
		item->OnGraphicChange();

		LOG("\t0x%08X:%04X [%d] %04X\n", item->Serial, item->Graphic, layer, item->Color);

		//if (m_ClientVersion >= CV_308Z && !obj2->ClilocMessage.length())
		//	megaClilocRequestList.push_back(obj2->Serial);

		g_World->MoveToTop(item);

		itemSerial = ReadUInt32BE();
	}

	if (obj->IsPlayer())
		g_Player->UpdateAbilities();

	if (resuestStatus)
		g_Orion.StatusReq(serial);










	/*if (g_ObjectInHand != NULL && g_ObjectInHand->Serial == obj->Serial)
	{
		delete g_ObjectInHand;
		g_ObjectInHand = NULL;
	}

	obj->ClearNotOpenedItems();

	obj->MapIndex = g_CurrentMap;

	ushort oldGraphic = obj->Graphic;
	bool oldDead = g_Player->Dead();
	obj->Graphic = graphic;

	int changeGraphicDir = direction;

	if (character != NULL)
		changeGraphicDir = 1000;
	else
	{
		item->MultiBody = (graphic & 0x4000);

		if (item->MultiBody)
			item->WantUpdateMulti = ((obj->m_Items == NULL) || (oldGraphic != obj->Graphic) || (obj->X != x) || (obj->Y != y) || (obj->Z != z));
	}

	obj->OnGraphicChange(changeGraphicDir);

	bool hidden = obj->Hidden();
	obj->Flags = flags;
	obj->Color = color;
	bool updateCoords = true;//(hidden == obj->Hidden());

	if (character != NULL && !character->m_Steps.empty())
	{
		if (x != obj->X || y != obj->Y)
		{
			obj->X = character->m_Steps.front().X;
			obj->Y = character->m_Steps.front().Y;
			obj->Z = character->m_Steps.front().Z;
			character->m_Steps.clear();
			updateCoords = false;
		}
	}

	if (updateCoords)
	{
		obj->X = x;
		obj->Y = y;
		obj->Z = z;

		if (character != NULL)
			character->Direction = direction;
	}

	if (character != NULL)
	{
		character->Notoriety = notoriety;
		LOG("0x%08X 0x%04X NPC %d,%d,%d C%04X F%02X D%d N%d\n", serial, obj->Graphic, obj->X, obj->Y, obj->Z, obj->Color, obj->Flags, character->Direction, character->Notoriety);
	}
	else
		LOG("0x%08X 0x%04X %d,%d,%d C%04X F%02X\n", serial, obj->Graphic, obj->X, obj->Y, obj->Z, obj->Color, obj->Flags);

	//if (m_ClientVersion >= CV_308Z && !obj->ClilocMessage.length())
	//	m_MegaClilocRequests.push_back(obj->Serial);

	if (serial == g_PlayerSerial)
	{
		if (oldDead != g_Player->Dead())
		{
			if (g_Player->Dead())
				g_Orion.ChangeSeason(ST_DESOLATION, DEATH_MUSIC_INDEX);
			else
				g_Orion.ChangeSeason(g_OldSeason, g_OldSeasonMusic);
		}

		g_Player->CloseBank();
		g_GumpManager.RemoveRangedGumps();
	}

	g_GumpManager.UpdateContent(serial, 0, GT_PAPERDOLL);
	*/

	//SendMegaClilocRequests(megaClilocRequestList);
	/*CPlayer *__cdecl HandleUpdateObject(_BYTE *buf)
	{
		v8 = (int)FindWorldObject(serial);
		v9 = 0;
		if (v8)
		{
			if ((*(int(__thiscall **)(int))(*(_DWORD *)v8 + 36))(v8))
			{
				sub_5A7540(v8);
				(*(void(__thiscall **)(int))(*(_DWORD *)v8 + 20))(v8);
				(*(void(__thiscall **)(int, _DWORD))(*(_DWORD *)v8 + 196))(v8, notoriety);
			}
			else
			{
				v8 = 0;
			}
		}
		if (v8)
		{
			v25 = 1;
			*(_BYTE *)(v8 + 115) = 1;
			v18 = sub_4DAB80(v8, 0, 0, dword_978178, v25);
			v19 = *(void(__thiscall **)(_DWORD))(*(_DWORD *)v8 + 16);
			*(_BYTE *)(v8 + 112) = 0;
			v19(v8);
			v20 = *(_DWORD *)(v8 + 172);
			if (v20 && !*(_DWORD *)(v8 + 392))
			{
				if ((*(int(**)(void))(*(_DWORD *)v20 + 36))())
					*(_DWORD *)(*(_DWORD *)(v8 + 172) + 508) = 0;
				(*(void(__thiscall **)(int))(*(_DWORD *)v8 + 80))(v8);
				v21 = *(void(__thiscall **)(_DWORD, _DWORD, _DWORD))(*(_DWORD *)v8 + 100);
				v24 = *(_DWORD *)(v8 + 36);
				LOWORD(v25) = *(_WORD *)(v8 + 40);
				v21(v8, v24, v25);
			}
			if (v18)
			{
				v22 = *(_DWORD *)(v18 + 80);
				sub_480360(&statusRequestPacket, 0x34);
				sub_4804E0((int)&statusRequestPacket, 0xEDEDEDED);
				sub_4803A0(&statusRequestPacket, 4);
				sub_4804E0((int)&statusRequestPacket, v22);
				SendPacket(&statusRequestPacket);
			}
		}
		result = g_Player;
		if (!g_Player->GameCharacter.field_188)
		{
			g_Player->GameCharacter.field_200 = 0;
			g_Player->GameCharacter.field_1FC = 0;
			result = g_Player;
		}
		if (g_ShowIncomingNames
			&& v8
			&& !objectIsExists
			&& !(*(_BYTE *)(v8 + 204) & 0x80)
			&& *(_WORD *)(v8 + 56)
			&& (CPlayer *)v8 != result)
		{
			CreateClickPacket((int)&clickPacket, *(_DWORD *)(v8 + 168));
			result = (CPlayer *)SendPacket(&clickPacket);
		}
		return result;
	}




char __cdecl HandleUpdateObject(BYTE *buf)
{
	if (serial == g_PlayerSerial)
	{
	obj = FindWorldObject(serial);
	obj_ = obj;
	obj__ = (int)obj;
	if (obj)
	{
		if ((*((int(__thiscall **)(CGameObject *))obj->VTable + UO_ROFUN_IS_NPC))(obj))
		{
			(*((void(__thiscall **)(CGameObject *))obj_->VTable + UO_ROFUN_REMOVE_FROM_RENDER))(obj_);
			(*((void(__thiscall **)(CGameObject *, _DWORD))obj_->VTable + UO_ROFUN_SET_NOTORIETY))(obj_, (char)notoriety);
		}
		else
		{
			obj__ = 0;
			obj_ = 0;
		}
	}
	if (obj_)
	{
		obj_->field_73 = 1;
		v28 = (CGump *)sub_457CA0(obj_, 0, 0, g_GumpObjectFirstPtr, 1);
		v29 = obj_->VTable;
		v30 = v28;
		obj_->NotInLists = 0;
		((void(__thiscall *)(CGameObject *))v29[UO_ROFUN_UPDATE_RENDER_AND_DRAW_POS])(obj_);
		if (v30)
		{
			v31 = v30->GameObject->Serial;
			statusRequestPacketData[0] = 0x34;
			if (BYTE1(g_Packets[104]) & 0x80)
				MovePacketPointer((int)statusRequestPacketData, 3);
			else
				LOWORD(g_PacketPointer) = 1;
			v32 = htonl(0xEDEDEDED);
			v33 = (unsigned __int16)GetPacketPointer((int)statusRequestPacketData);
			*(_DWORD *)&statusRequestPacketData[v33] = v32;
			MovePacketPointer((int)statusRequestPacketData, v33 + 4);
			v34 = (unsigned __int16)GetPacketPointer((int)statusRequestPacketData);
			statusRequestPacketData[v34] = 4;
			MovePacketPointer((int)statusRequestPacketData, v34 + 1);
			PackUInt32BE((int)statusRequestPacketData, v31);
			SendPacket(g_ClientSocket, (int)statusRequestPacketData);
		}
	}
	result = g_ShowIncomingNames;
	if (g_ShowIncomingNames)
	{
		if (obj_)
		{
			result = objectIsExists;
			if (!objectIsExists && !(obj_->Flags & 0x80) && obj_->Graphic && (CPlayer *)obj_ != g_Player)
			{
				v36 = obj_->Serial;
				LOBYTE(clickRequestPacketData) = 9;
				if (BYTE1(g_Packets[18]) & 0x80)
					*(_WORD *)((char *)&clickRequestPacketData + 1) = 3;
				else
					LOWORD(g_PacketPointer) = 1;
				v37 = htonl(v36);
				v38 = (unsigned __int16)GetPacketPointer((int)&clickRequestPacketData);
				*(int *)((char *)&clickRequestPacketData + v38) = v37;
				MovePacketPointer((int)&clickRequestPacketData, v38 + 4);
				result = (unsigned int)SendPacket(g_ClientSocket, (int)&clickRequestPacketData);
			}
		}
	}
	return result;
}*/
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(EquipItem)
{
	WISPFUN_DEBUG("c150_f30");
	if (g_World == NULL)
		return;

	uint serial = ReadUInt32BE();

	if (g_ObjectInHand != NULL && g_ObjectInHand->Serial == serial)
	{
		delete g_ObjectInHand;
		g_ObjectInHand = NULL;
	}

	CGameItem *obj = g_World->GetWorldItem(serial);
	obj->MapIndex = g_CurrentMap;

	if (obj->Graphic && obj->Layer != OL_BACKPACK)
		obj->Clear();

	obj->Graphic = ReadUInt16BE();
	Move(1);
	int layer = ReadUInt8();
	uint cserial = ReadUInt32BE();
	obj->Color = ReadUInt16BE();

	if (obj->Container != 0xFFFFFFFF)
		g_GumpManager.UpdateContent(obj->Container, 0, GT_CONTAINER);

	if (obj->Container != 0xFFFFFFFF)
		g_GumpManager.UpdateContent(obj->Container, 0, GT_PAPERDOLL);

	g_World->PutEquipment(obj, cserial, layer);
	obj->OnGraphicChange();

	if (g_NewTargetSystem.Serial == serial)
		g_NewTargetSystem.Serial = 0;

	//if (m_ClientVersion >= CV_308Z && !obj->ClilocMessage.length())
	//	m_MegaClilocRequests.push_back(obj->Serial);

	if (layer >= OL_BUY_RESTOCK && layer <= OL_SELL)
		obj->Clear();
	else if (layer < OL_MOUNT)
		g_GumpManager.UpdateContent(cserial, 0, GT_PAPERDOLL);

	if (cserial == g_PlayerSerial && (layer == OL_1_HAND || layer == OL_2_HAND))
		g_Player->UpdateAbilities();
}
//----------------------------------------------------------------------------------
void CPacketManager::UpdateContainedItem(const uint &serial, const ushort &graphic, const uchar &graphicIncrement, const ushort &count, const int &x, const int &y, const uint containerSerial, const ushort &color)
{
	CGameObject *container = g_World->FindWorldObject(containerSerial);

	if (container == NULL)
		return;

	CGameObject *obj = g_World->FindWorldObject(serial);

	if (obj != NULL)
	{
		g_World->RemoveObject(obj);
		obj = NULL;
	}

	if (serial & 0x40000000)
		obj = g_World->GetWorldItem(serial);
	else
		obj = g_World->GetWorldCharacter(serial);

	if (obj == NULL)
	{
		LOG("No memory?\n");
		return;
	}

	obj->MapIndex = g_CurrentMap;

	obj->Graphic = graphic + graphicIncrement;
	obj->OnGraphicChange();
	obj->Color = g_ColorManager.FixColor(color, (color & 0x8000));

	obj->Count = count;

	UpdateItemInContainer(obj, container, x, y);

	//sub_534560(v19);

	g_World->MoveToTop(obj);

	LOG("\t|0x%08X<0x%08X:%04X*%d (%d,%d) %04X\n", containerSerial, serial, graphic + graphicIncrement, count, x, y, color);
}
//----------------------------------------------------------------------------------
void CPacketManager::UpdateItemInContainer(CGameObject *obj, CGameObject *container, const int &x, const int &y)
{
	obj->X = x;
	obj->Y = y;
	g_World->PutContainer(obj, container);

	uint containerSerial = container->Serial;

	if (m_ClientVersion >= CV_308Z)
		m_MegaClilocRequests.push_back(obj->Serial);

	if (obj->Graphic == 0x0EB0) //Message board item
	{
		CPacketBulletinBoardRequestMessageSummary(containerSerial, obj->Serial).Send();

		g_GumpManager.UpdateGump(containerSerial, 0, GT_BULLETIN_BOARD);
	}

	CGump *gump = g_GumpManager.UpdateContent(containerSerial, 0, GT_SPELLBOOK);

	if (gump == NULL)
	{
		gump = g_GumpManager.UpdateContent(containerSerial, 0, GT_CONTAINER);

		if (gump != NULL && gump->GumpType == GT_CONTAINER)
			((CGumpContainer*)gump)->UpdateItemCoordinates(obj);

		if (m_ClientVersion >= CV_308Z)
			m_MegaClilocRequests.push_back(containerSerial);
	}

	if (gump != NULL && !container->NPC)
	{
		((CGameItem*)container)->Opened = true;
	}

	CGameObject *top = container->GetTopObject();

	if (top != NULL)
	{
		top = top->FindSecureTradeBox();

		if (top != NULL)
			g_GumpManager.UpdateContent(0, top->Serial, GT_TRADE);
	}
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

	UpdateContainedItem(serial, graphic, graphicIncrement, count, x, y, containerSerial, color);
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
		
		UpdateContainedItem(serial, graphic, graphicIncrement, count, x, y, containerSerial, color);
	}
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(DenyMoveItem)
{
	WISPFUN_DEBUG("c150_f33");
	if (g_World == NULL)
		return;

	if (g_ObjectInHand != NULL)
	{
		CGameItem *obj = g_World->FindWorldItem(g_ObjectInHand->Serial);

		if (obj == NULL)
		{
			obj = g_World->GetWorldItem(g_ObjectInHand->Serial);

			if (obj != NULL)
			{
				g_World->RemoveFromContainer(obj);

				obj->Paste(g_ObjectInHand);

				//if (m_ClientVersion >= CV_308Z)
				//	m_MegaClilocRequests.push_back(obj->Serial);

				g_World->PutContainer(obj, g_ObjectInHand->Container);

				g_World->MoveToTop(obj);
			}
		}

		if (obj != NULL)
			g_GumpManager.UpdateContent(obj->Container, 0, GT_CONTAINER);

		delete g_ObjectInHand;
		g_ObjectInHand = NULL;
	}
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

	if (g_ObjectInHand != NULL && g_ObjectInHand->Serial == serial)
	{
		bool sep = g_ObjectInHand->Separated;

		if (sep)
			g_ObjectInHand->Separated = false;
		else if (g_ObjectInHand->Dropped/*Deleted*/)
		{
			delete g_ObjectInHand;
			g_ObjectInHand = NULL;
		}

		if (g_ObjectInHand != NULL)
		{
			if (obj != NULL && sep)
			{
				if (obj->Count != g_ObjectInHand->DragCount)
					g_ObjectInHand->Deleted = true;
				else
					g_ObjectInHand->Separated = true;
			}
			else
				g_ObjectInHand->Deleted = true;
		}
	}

	if (obj != NULL)
	{
		bool updateAbilities = false;
		uint cont = obj->Container;

		if (cont != 0xFFFFFFFF)
		{
			CGameObject *top = obj->GetTopObject();

			if (top != NULL)
			{
				if (top->IsPlayer() && !obj->NPC)
				{
					CGameItem *item = (CGameItem*)obj;
					updateAbilities = (item->Layer == OL_1_HAND || item->Layer == OL_2_HAND);
				}

				CGameObject *tradeBox = top->FindSecureTradeBox();

				if (tradeBox != NULL)
					g_GumpManager.UpdateContent(0, tradeBox->Serial, GT_TRADE);
			}

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

		if (obj->NPC)
		{
			if (!g_Party.Contains(obj->Serial))
			{
				bool inList = false;

				for (UINTS_PAIR_LIST::iterator i = g_DeletedCharactersStack.begin(); i != g_DeletedCharactersStack.end(); i++)
				{
					if (i->first == serial)
					{
						inList = true;
						i->second = g_Ticks + KEEP_CHARACTERS_IN_REMOVE_LIST_DELAY;

						break;
					}
				}

				if (!inList)
				{
					((CGameCharacter*)obj)->Deleted = true;
					g_DeletedCharactersStack.push_back(pair<uint, uint>(serial, g_Ticks + KEEP_CHARACTERS_IN_REMOVE_LIST_DELAY));
				}
			}
			else
				g_GumpManager.UpdateContent(obj->Serial, 0, GT_STATUSBAR);

			obj->RemoveRender();
		}
		else
		{
			if (obj->IsCorpse() && obj->LastAnimationChangeTime == GetTickCount())
			{
				CGameItem *fake = new CGameItem(1);

				fake->Graphic = 0x2006;
				fake->Color = obj->Color;
				fake->Count = obj->Count;
				fake->X = obj->X;
				fake->Y = obj->Y;
				fake->Z = obj->Z;
				fake->Layer = ((CGameItem*)obj)->Layer;
				fake->RenderQueueIndex = 6;
				fake->UsedLayer = ((CGameItem*)obj)->UsedLayer;
				fake->AnimIndex = 0;
				fake->FieldColor = 1;

				g_World->m_Items->AddObject(fake);
				g_MapManager->AddRender(fake);
			}

			g_World->RemoveObject(obj);

			if (updateAbilities)
				g_Player->UpdateAbilities();
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
	CGameObject *obj = g_World->FindWorldObject(serial);

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

	if (obj->NPC)
		((CGameCharacter*)obj)->Notoriety = notoriety;

	if (obj->IsPlayer())
	{
		obj->Flags = flags;
	}
	else
	{
		//UpdateGameObject(serial, graphic, 0, 0, x, y, z, direction, color, flags, 0, 0, 1);
	}

	/*if (!obj->Graphic)
	{
		bool resuestStatus = (g_GumpManager.UpdateContent(serial, 0, GT_STATUSBAR) != NULL);

		resuestStatus = (g_GumpManager.UpdateContent(serial, 0, GT_TARGET_SYSTEM) != NULL) || g_ConfigManager.DrawStatusState || resuestStatus || (serial == g_LastTargetObject) || (serial == g_LastAttackObject);

		if (resuestStatus)
			g_Orion.StatusReq(serial);
	}

	obj->Deleted = false;
	obj->MapIndex = g_CurrentMap;
	obj->Graphic = ReadUInt16BE();
	obj->OnGraphicChange();

	short x = ReadInt16BE();
	short y = ReadInt16BE();
	char z = ReadInt8();
	uchar dir = ReadUInt8();

	if (!obj->IsTeleportAction(x, y, dir))
	{
		if (serial != g_PlayerSerial)
		{
			if (!obj->m_Steps.empty() && obj->FindLayer(OL_MOUNT) == NULL)
			{
				CWalkData &wd = obj->m_Steps.back();

				obj->X = wd.X;
				obj->Y = wd.Y;
				obj->Z = wd.Z;
				obj->Direction = wd.Direction;

				obj->m_Steps.clear();
			}

			if (obj->m_Steps.empty())
				obj->LastStepTime = g_Ticks;

			obj->m_Steps.push_back(CWalkData(x, y, z, dir));
		}
		else
		{
			obj->X = x;
			obj->Y = y;
			obj->Z = z;
			obj->Direction = dir;
		}
	}
	else if (serial != g_PlayerSerial)
	{
		bool passed = obj->m_Steps.empty();

		if (!passed)
		{
			CWalkData &wd = obj->m_Steps.front();
			passed = (wd.X != x || wd.Y != y || wd.Z != z || wd.Direction != dir);
		}

		if (passed)
		{
			obj->m_Steps.clear();
			obj->X = x;
			obj->Y = y;
			obj->Z = z;
			obj->Direction = dir;
		}
	}

	//if (serial == g_PlayerSerial)
	//	obj->PaperdollText = "";

	if (m_ClientVersion >= CV_308Z)
	{
		m_MegaClilocRequests.push_back(obj->Serial);

		if (serial == g_PlayerSerial)
		{
			if (g_Player->m_Items != NULL)
			{
				CGameItem *backpack = (CGameItem*)g_Player->m_Items;
				m_MegaClilocRequests.push_back(backpack->Serial);
			}
		}

		SendMegaClilocRequests();
	}

	if (obj->IsPlayer())
	{
		g_Player->CloseBank();
		g_GumpManager.RemoveRangedGumps();
	}

	obj->Color = ReadUInt16BE();
	obj->Flags = ReadUInt8();
	obj->Notoriety = ReadUInt8();*/

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
	g_NoMegaCliloc = g_LockedClientFeatures & LFF_T2A || g_LockedClientFeatures & LFF_RE || g_LockedClientFeatures & LFF_TD || g_LockedClientFeatures & LFF_LBR;

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
		CGumpShop *buyGump = (CGumpShop*)g_GumpManager.GetGump(serial, 0, GT_SHOP);

		if (buyGump != NULL && (buyGump->Serial != serial || !buyGump->IsBuyGump))
		{
			g_GumpManager.RemoveGump(buyGump);
			buyGump = NULL;
		}

		if (buyGump == NULL)
			buyGump = new CGumpShop(serial, true, 150, 5);
		else
			addGump = false;

		gump = buyGump;

		buyGump->Visible = true;
	}
	else //Container
	{
		ushort graphic = 0xFFFF;

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

			if (gumpid = 0xFFFF)
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

	int type = ReadUInt8();
	bool HaveCap = (type == 0x02 || type == 0xDF);
	bool IsSingleUpdate = (type == 0xFF || type == 0xDF);
	LOG("Skill update type %i (Cap=%d)\n", type, HaveCap);

	CGumpSkills *gump = (CGumpSkills*)g_GumpManager.UpdateGump(g_PlayerSerial, 0, GT_SKILLS);

	while (m_Ptr < m_End)
	{
		ushort id = ReadUInt16BE();

		if (!id && !type)
			break;
		else if (!type || type == 0x02)
			id--;

		ushort BaseVal = ReadUInt16BE();
		ushort RealVal = ReadUInt16BE();
		uchar lock = ReadUInt8();
		ushort Cap = 0;

		if (HaveCap)
			Cap = ReadUInt16BE();

		if (id < g_SkillsCount)
		{
			if (IsSingleUpdate)
			{
				float change = (float)(BaseVal / 10.0f) - g_Player->GetSkillBaseValue(id);

				if (change)
				{
					char str[128] = { 0 };
					sprintf(str, "Your skill in %s has %s by %.1f%%.  It is now %.1f%%.", g_Skills[id].Name.c_str(), ((change < 0) ? "decreased" : "increased"), change, g_Player->GetSkillBaseValue(id) + change);
					//else if (change > 0) sprintf(str, "Your skill in %s has increased by %.1f%%.  It is now %.1f%%.", UO->m_Skills[id].m_Name.c_str(), change, obj->GetSkillBaseValue(id) + change);
					g_Orion.CreateTextMessage(TT_SYSTEM, 0, 3, 0x58, str);
				}
			}

			g_Player->SetSkillBaseValue(id, (float)(BaseVal / 10.0f));
			g_Player->SetSkillValue(id, (float)(RealVal / 10.0f));
			g_Player->SetSkillCap(id, (float)(Cap / 10.0f));
			g_Player->SetSkillStatus(id, lock);

			if (gump != NULL)
				gump->UpdateSkillValue(id);

			if (HaveCap)
				LOG("Skill %i is %i|%i|%i\n", id, BaseVal, RealVal, Cap);
			else
				LOG("Skill %i is %i|%i\n", id, BaseVal, RealVal);
		}
		else
			LOG("Unknown skill update %d\n", id);
	}

	g_SkillsTotal = 0.0f;

	IFOR(i, 0, g_SkillsCount)
		g_SkillsTotal += g_Player->GetSkillValue(i);

	g_Player->SkillsReceived = true;
	if (gump != NULL)
	{
		gump->UpdateSkillsSum();
		g_Orion.OpenSkills();
	}
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
				if (item != NULL)
					item->Name = ToString(str);

			}
			str = L"";
			ushort crafterNameLen = 0;
			uint next = ReadUInt32BE();
			if (next == 0xFFFFFFFD)
			{
				crafterNameLen = ReadUInt16BE();
				if (crafterNameLen)
				{
					wstring crafterName = ReadWString(crafterNameLen);
					str = L"Crafted by ";
					str += crafterName;
				}
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
			uint houseSerial = ReadUInt32BE();
			uint houseRevision = ReadUInt32BE();
			CPacketCustomHouseDataReq(houseSerial).Send();
			//Попробуем пока без кэша.
			break;
		}
		case 0x20:
		{
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
				text->Color = 0x0035;
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
	g_WalkRequestCount = 0;
	g_PendingDelayTime = 0;
	g_Ping = 0;

	if (g_Player == NULL)
		return;

	Move(1);
	g_Player->X = ReadUInt16BE();
	g_Player->Y = ReadUInt16BE();
	uchar dir = ReadUInt8();
	g_Player->Direction = dir;
	g_Player->Z = ReadUInt8();

	g_Walker->SetSequence(0, dir);
	g_Player->OffsetX = 0;
	g_Player->OffsetY = 0;
	g_Player->OffsetZ = 0;

	g_Player->m_Steps.clear();

	g_Player->CloseBank();
	g_GumpManager.RemoveRangedGumps();

	g_World->MoveToTop(g_Player);
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(ConfirmWalk)
{
	WISPFUN_DEBUG("c150_f49");
	if (g_WalkRequestCount)
		g_WalkRequestCount--;

	if (g_Player == NULL)
		return;

	uchar seq = ReadUInt8();

	g_PingByWalk[seq][1] = g_Ticks;

	if (seq >= 10 && !(seq % 10))
	{
		g_Ping = 0;

		IFOR(i, 0, 10)
		{
			int delay = g_PingByWalk[seq - i][1] - g_PingByWalk[seq - i][0];

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

	g_GumpManager.RemoveRangedGumps();
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

	if (!serial && font == 0xFFFF && textColor == 0xFFFF)
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

	string name(ReadString(0));
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

	if (!serial && font == 0xFFFF && textColor == 0xFFFF)
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

	string name(ReadString(0));
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

	if (g_ConsolePrompt != PT_NONE)
	{
		g_Orion.ConsolePromptCancel();
		g_Orion.CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "Previous prompt cancelled.");
	}

	g_ConsolePrompt = PT_ASCII;
	memcpy(&g_LastASCIIPrompt[0], &m_Start[0], 11);
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(UnicodePrompt)
{
	WISPFUN_DEBUG("c150_f63");
	if (g_World == NULL)
		return;

	if (g_ConsolePrompt != PT_NONE)
	{
		g_Orion.ConsolePromptCancel();
		g_Orion.CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "Previous prompt cancelled.");
	}

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

	CGameCharacter *owner = g_World->FindWorldCharacter(serial);

	if (!corpseSerial)
	{
		if (serial < 0x40000000 && owner != NULL)
		{
			CGameItem *obj = new CGameItem(1);

			obj->Graphic = 0x2006;
			obj->Color = owner->Color;
			obj->Count = owner->Graphic;
			obj->X = owner->X;
			obj->Y = owner->Y;
			obj->Z = owner->Z;
			obj->Layer = owner->Direction;
			obj->RenderQueueIndex = 6;
			obj->UsedLayer = (ReadUInt32BE() ? 1 : 0);
			obj->AnimIndex = 0;
			obj->FieldColor = 1;

			g_World->m_Items->AddObject(obj);
			g_MapManager->AddRender(obj);
		}
	}
	else
	{
		CGameItem *obj = g_World->FindWorldItem(corpseSerial);

		if (obj != NULL)
		{
			obj->AnimIndex = 0;

			if (owner != NULL)
			{
				owner->Deleted = true;
				g_DeletedCharactersStack.push_back(pair<uint, uint>(serial, g_Ticks + KEEP_CHARACTERS_IN_REMOVE_LIST_DELAY));
			}
		}
		else
			g_CorpseSerialList.push_back(pair<uint, uint>(corpseSerial, g_Ticks + 1000));
	}
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

	ushort wat = ReadUInt16BE();
	uint serial = ReadUInt32BE();

	CGameObject *obj = g_World->FindWorldObject(serial);

	if (obj == NULL)
		return;

	ushort wat2 = ReadUInt16BE();
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
		text->Color = 0x0035;
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

	int commandsLength = ReadInt16BE();
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
			if (listSize >= 8)
			{
				int x = ToInt(list[1]);
				int y = ToInt(list[2]);
				int up = ToInt(list[3]);
				int down = ToInt(list[4]);
				int action = ToInt(list[5]);
				int toPage = ToInt(list[6]);
				int index = ToInt(list[7]);

				if (action == 1)
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

				if (action == 1)
					toPage = -1;

				go = new CGUIButtonTileart(index, up, up, down, x, y, tileGraphic, tileColor, tileX, tileY);

				((CGUIButton*)go)->ToPage = toPage;
			}
		}
		else if (cmd == "checkbox")
		{
			if (listSize >= 7)
			{
				int x = ToInt(list[1]);
				int y = ToInt(list[2]);
				int up = ToInt(list[3]);
				int down = ToInt(list[4]);
				int state = ToInt(list[5]);
				int index = ToInt(list[6]);

				go = new CGUICheckbox(index, up, down, up, x, y);

				((CGUICheckbox*)go)->Checked = (state != 0);
			}
		}
		else if (cmd == "radio")
		{
			if (listSize >= 7)
			{
				int x = ToInt(list[1]);
				int y = ToInt(list[2]);
				int up = ToInt(list[3]);
				int down = ToInt(list[4]);
				int state = ToInt(list[5]);
				int index = ToInt(list[6]);

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
				go = new CGUIGenericTextEntry(index + 1, textIndex, color, x, y);
				((CGUIGenericTextEntry*)go)->CheckOnSerial = true;
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
				go = new CGUIGenericTextEntry(index + 1, textIndex, color, x, y, width, length);
				((CGUIGenericTextEntry*)go)->CheckOnSerial = true;
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
			if (listSize >= 5)
			{
				int x = ToInt(list[1]);
				int y = ToInt(list[2]);
				int graphic = ToInt(list[3]);
				int color = ToInt(list[4]);

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
					htmlInfo.Color = ToInt(list[8]);

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
				string text = ReadString(len) + " - ";

				//subject
				len = ReadUInt8();
				text += ReadString(len) + " - ";

				//data time
				len = ReadUInt8();
				text += ReadString(len);

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
				string poster = ReadString(len);

				//subject
				len = ReadUInt8();
				string subject = ReadString(len);

				//data time
				len = ReadUInt8();
				string dataTime = ReadString(len);

				Move(5);

				uchar lines = ReadUInt8();
				string data = "";

				IFOR(i, 0, lines)
				{
					uchar linelen = ReadUInt8();

					if (data.length())
						data += "\n";

					data += ReadString(linelen);
				}

				uchar variant = 1 + (int)(poster == g_Player->Name);
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

	CGumpBook *gump = new CGumpBook(serial, 0, 0, pageCount, flags != 0, false);

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

			uint price = ReadUInt32BE();

			uchar nameLen = ReadUInt8();
			string name = ReadString(nameLen);

			//try int.parse and read cliloc.
			int clilocNum = 0;
			bool nameFromCliloc = false;

			if (Int32TryParse(name, clilocNum))
			{
				name = g_ClilocManager.Cliloc(g_Language)->GetA(clilocNum, true);
				nameFromCliloc = true;
			}

			CGUIShopItem *shopItem = (CGUIShopItem*)htmlGump->Add(new CGUIShopItem(item->Serial, item->Graphic, item->Color, item->Count, price, name, 0, currentY));
			shopItem->NameFromCliloc = nameFromCliloc;

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
	if (m_ClientVersion >= CV_308Z && !g_NoMegaCliloc)
	{
		uint serial = ReadUInt32BE();
		uint revision = ReadUInt32BE();
		//Если хранить ревизию на обьекте, то сравнивая её и то что здесь пришло
		// и получая одинаковый результат, - нет смысла запрашивать у сервера эти данные заного.
		AddMegaClilocRequest(serial, true);
	}
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(CustomHouse)
{
	WISPFUN_DEBUG("c150_f100");
	bool compressed = ReadUInt8() == 0x03;
	bool enableResponse = ReadUInt8() == 0x01;
	uint houseSerial = ReadUInt32BE();
	uint revision = ReadUInt32BE();
	CGameItem *foundationItem = g_World->GetWorldItem(houseSerial);


	ReadUInt16BE();
	ReadUInt16BE();

	uchar planes = ReadUInt8();


	uint header;

	int cLen, planeZ, planeMode;
	uLongf dLen;
	CMulti* multi = foundationItem->GetMulti();
	if (multi == NULL) return;
	short minX = multi->MinX;
	short minY = multi->MinY;
	short maxY = multi->MaxY;

	IFOR(plane, 0, planes)
	{

		header = ReadUInt32BE();
		dLen = ((header & 0xFF0000) >> 16) | ((header & 0xF0) << 4);
		cLen = ((header & 0xFF00) >> 8) | ((header & 0x0F) << 8);
		planeZ = (header & 0x0F000000) >> 24;
		planeMode = (header & 0xF0000000) >> 28;

		if (cLen <= 0) continue;
		UCHAR_LIST decompressedBytes(dLen);
		int z_err = uncompress(&decompressedBytes[0], &dLen, m_Ptr, cLen);
		if (z_err != Z_OK)
		{
			LOG("Bad CustomHouseStruct compressed data received from server, house serial:%i\n", houseSerial);
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
					z += foundationItem->Z;
					if (id == 0) continue;
					foundationItem->AddMulti(id, x, y, z);
				}
				break;
			}
			case 1:
			{
				if (planeZ > 0)
					z = ((planeZ - 1) % 4) * 20 + 7 + foundationItem->Z; // Z=7,27,47,67
				else
					z = foundationItem->Z;

				for (uint i = 0; i < decompressedBytes.size()/4; i++)
				{
					id = tempReader.ReadUInt16BE();
					x = tempReader.ReadUInt8();
					y = tempReader.ReadUInt8();
					if (id == 0) continue;
					foundationItem->AddMulti(id, x, y, z);
				}
				break;
			}
			case 2:
			{
				short xOffs;
				short yOffs;
				short multiHeight;

				if (planeZ > 0)
					z = ((planeZ - 1) % 4) * 20 + 7 + foundationItem->Z; // Z=7,27,47,67
				else
					z = foundationItem->Z;

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

				for (uint i = 0; i < decompressedBytes.size()/2; i++)
				{
					id = tempReader.ReadUInt16BE();
					x = i / multiHeight + xOffs;
					y = i % multiHeight + yOffs;
					if (id == 0) continue;
					foundationItem->AddMulti(id, x, y, z);
				}
				break;
			}
		}
	}
}
//----------------------------------------------------------------------------------
