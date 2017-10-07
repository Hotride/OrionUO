/***********************************************************************************
**
** stdafx.h
**
** Copyright (C) August 2017 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef stdafxH
#define stdafxH
//----------------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS
//----------------------------------------------------------------------------------
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <Shlwapi.h>
#include <windows.h>
#include <windowsx.h>
#include <Mmsystem.h>
#include <time.h>
#include <process.h>
#include <stdarg.h>
#include <locale>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <functional>

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "Glu32.lib")
//----------------------------------------------------------------------------------
#include "GLEngine/glew.h"
#include "zlib.h"

#pragma comment(lib, "zdll.lib")
//----------------------------------------------------------------------------------
#include "targetver.h"

#include "Wisp/WispDefinitions.h"
#include "Wisp/WispGlobal.h"
#include "Wisp/WispApplication.h"
#include "Wisp/WispDataStream.h"
#include "Wisp/WispBinaryFileWritter.h"
#include "Wisp/WispConnection.h"
#include "Wisp/WispGeometry.h"
#include "Wisp/WispLogger.h"
#include "Wisp/WispMappedFile.h"
#include "Wisp/WispMouse.h"
#include "Wisp/WispPacketMessage.h"
#include "Wisp/WispPacketReader.h"
#include "Wisp/WispTextFileParser.h"
#include "Wisp/WispThread.h"
#include "Wisp/WispThreadedTimer.h"
#include "Wisp/WispWindow.h"

#include "DefinitionMacro.h"
#include "EnumList.h"
#include "BaseQueue.h"
#include "Constants.h"
#include "DateTimeStamp.h"

#include "GLEngine/GLTexture.h"
#include "GLEngine/GLTextTexture.h"
#include "GLEngine/GLHTMLTextTexture.h"
#include "GLEngine/GLTextureCircleOfTransparency.h"
#include "GLEngine/GLShader.h"
#include "GLEngine/GLVector.h"
#include "GLEngine/GLFrameBuffer.h"
#include "GLEngine/GLEngine.h"

#include "Globals.h"

class CGump;

#include "CharacterList.h"
#include "ClickObject.h"
#include "CommonInterfaces.h"
#include "Container.h"
#include "ContainerStack.h"
#include "UseItemsList.h"
#include "ExceptionFilter.h"
#include "ImageBounds.h"
#include "MulStruct.h"
#include "TextureObject.h"
#include "IndexObject.h"
#include "Macro.h"
#include "Multi.h"
#include "MultiMap.h"
#include "OrionApplication.h"
#include "OrionStackWalker.h"
#include "OrionUO.h"
#include "OrionWindow.h"
#include "PartyObject.h"
#include "Party.h"
#include "PluginInterface.h"
#include "SelectedObject.h"
#include "PressedObject.h"
#include "Profession.h"
#include "QuestArrow.h"
#include "RenderObject.h"
#include "RenderWorldObject.h"
#include "Resource.h"
#include "ScreenshotBuilder.h"
#include "ServerList.h"
#include "ShaderData.h"
#include "SkillGroup.h"
#include "StackWalker.h"
#include "StumpsData.h"
#include "TargetGump.h"
#include "VMQuery.h"
#include "WeatherEffect.h"
#include "Weather.h"

#include "Utility/AutoResetEvent.h"

#include "Network/UOHuffman.h"
#include "Network/Packets.h"
#include "Network/Connection.h"

#include "ToolTip.h"

#include "TextEngine/EntryText.h"
#include "TextEngine/GameConsole.h"
#include "TextEngine/Journal.h"
#include "TextEngine/RenderTextObject.h"
#include "TextEngine/TextData.h"
#include "TextEngine/TextContainer.h"
#include "TextEngine/TextRenderer.h"

#include "Walker/FastWalk.h"
#include "Walker/PathNode.h"
#include "Walker/PathFinder.h"
#include "Walker/WalkData.h"
#include "Walker/Walker.h"

#include "GUI/GUI.h"

#include "Gumps/Gump.h"
#include "Game objects/CustomHouseObjects.h"
#include "Game objects/MapObject.h"
#include "Game objects/RenderStaticObject.h"
#include "Game objects/LandObject.h"
#include "Game objects/StaticObject.h"
#include "Game objects/MultiObject.h"
#include "Game objects/CustomHouseMultiObject.h"
#include "Game objects/GameObject.h"
#include "Game objects/GameItem.h"
#include "Game objects/GameCharacter.h"
#include "Game objects/GameEffect.h"
#include "Game objects/GameEffectDrag.h"
#include "Game objects/GameEffectMoving.h"
#include "Game objects/GamePlayer.h"
#include "Game objects/GameWorld.h"
#include "Game objects/MapBlock.h"
#include "Game objects/ObjectOnCursor.h"

#include "Target.h"
#include "Managers/CustomHousesManager.h"

#include "Gumps/GumpAbility.h"
#include "Gumps/GumpBaseScroll.h"
#include "Gumps/GumpBook.h"
#include "Gumps/GumpBuff.h"
#include "Gumps/GumpBulletinBoard.h"
#include "Gumps/GumpBulletinBoardItem.h"
#include "Gumps/GumpCombatBook.h"
#include "Gumps/GumpConsoleType.h"
#include "Gumps/GumpContainer.h"
#include "Gumps/GumpCustomHouse.h"
#include "Gumps/GumpDrag.h"
#include "Gumps/GumpSelectColor.h"
#include "Gumps/GumpDye.h"
#include "Gumps/GumpGeneric.h"
#include "Gumps/GumpGrayMenu.h"
#include "Gumps/GumpJournal.h"
#include "Gumps/GumpMap.h"
#include "Gumps/GumpMenu.h"
#include "Gumps/GumpMenubar.h"
#include "Gumps/GumpMinimap.h"
#include "Gumps/GumpNotify.h"
#include "Gumps/GumpOptions.h"
#include "Gumps/GumpPaperdoll.h"
#include "Gumps/GumpPartyManifest.h"
#include "Gumps/GumpPopupMenu.h"
#include "Gumps/GumpProfile.h"
#include "Gumps/GumpQuestion.h"
#include "Gumps/GumpRacialAbilitiesBook.h"
#include "Gumps/GumpRacialAbility.h"
#include "Gumps/GumpScreenCharacterList.h"
#include "Gumps/GumpScreenConnection.h"
#include "Gumps/GumpScreenCreateCharacter.h"
#include "Gumps/GumpScreenGame.h"
#include "Gumps/GumpScreenMain.h"
#include "Gumps/GumpScreenSelectProfession.h"
#include "Gumps/GumpScreenSelectTown.h"
#include "Gumps/GumpScreenServer.h"
#include "Gumps/GumpSecureTrading.h"
#include "Gumps/GumpSelectFont.h"
#include "Gumps/GumpShop.h"
#include "Gumps/GumpSkill.h"
#include "Gumps/GumpSkills.h"
#include "Gumps/GumpSpell.h"
#include "Gumps/GumpSpellbook.h"
#include "Gumps/GumpStatusbar.h"
#include "Gumps/GumpTargetSystem.h"
#include "Gumps/GumpTextEntryDialog.h"
#include "Gumps/GumpTip.h"
#include "Gumps/GumpWorldMap.h"

#include "Managers/AnimationManager.h"
#include "Managers/CityManager.h"
#include "CityList.h"
#include "Managers/ClilocManager.h"
#include "Managers/ColorManager.h"
#include "Managers/ConfigManager.h"
#include "Managers/ConnectionManager.h"
#include "Managers/CreateCharacterManager.h"
#include "Managers/EffectManager.h"
#include "Managers/FileManager.h"
#include "Managers/FontsManager.h"
#include "Managers/GumpManager.h"
#include "Managers/MacroManager.h"
#include "Managers/MapManager.h"
#include "Managers/MouseManager.h"
#include "Managers/OptionsMacroManager.h"
#include "Managers/PacketManager.h"
#include "Managers/PluginManager.h"
#include "Managers/ProfessionManager.h"
#include "Managers/ScreenEffectManager.h"
#include "Managers/SkillGroupManager.h"
#include "Managers/SoundManager.h"
#include "Managers/SpeechManager.h"
#include "Managers/UOFileReader.h"
#include "Managers/CorpseManager.h"
#include "Managers/SkillsManager.h"

#include "Screen stages/BaseScreen.h"
#include "Screen stages/CharacterListScreen.h"
#include "Screen stages/ConnectionScreen.h"
#include "Screen stages/CreateCharacterScreen.h"
#include "Screen stages/GameBlockedScreen.h"
#include "Screen stages/GameScreen.h"
#include "Screen stages/MainScreen.h"
#include "Screen stages/SelectProfessionScreen.h"
#include "Screen stages/SelectTownScreen.h"
#include "Screen stages/ServerScreen.h"

//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
