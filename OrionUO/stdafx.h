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

#include "Dependencies/include/glew.h"
#include "Dependencies/include/wglew.h"
#include "Dependencies/include/bass.h"
#include "Dependencies/include/bassmidi.h"
#include "Dependencies/include/FreeImage.h"
#include "Dependencies/include/zlib.h"

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "Glu32.lib")
//----------------------------------------------------------------------------------
#if defined(_M_IX86)
#pragma comment(lib, "Dependencies/lib/Win32/glew32.lib")
#pragma comment(lib, "Dependencies/lib/Win32/bass.lib")
#pragma comment(lib, "Dependencies/lib/Win32/bassmidi.lib")
#pragma comment(lib, "Dependencies/lib/Win32/FreeImage.lib")
#pragma comment(lib, "Dependencies/lib/Win32/Psapi.lib")
#pragma comment(lib, "Dependencies/lib/Win32/zlib.lib")
//----------------------------------------------------------------------------------
#else
#pragma comment(lib, "Dependencies/lib/x64/glew32.lib")
#pragma comment(lib, "Dependencies/lib/x64/bass.lib")
#pragma comment(lib, "Dependencies/lib/x64/bassmidi.lib")
#pragma comment(lib, "Dependencies/lib/x64/FreeImage.lib")
#pragma comment(lib, "Dependencies/lib/x64/Psapi.lib")
#pragma comment(lib, "Dependencies/lib/x64/zdll.lib")
#endif
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
#include "Utility/PingThread.h"

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
#include "Utility/AutoResetEvent.h"
#include "Managers/FileManager.h"
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

#include "Network/UOHuffman.h"
#include "Network/Packets.h"
#include "Network/PluginPackets.h"
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
#include "GameObjects/CustomHouseObjects.h"
#include "GameObjects/MapObject.h"
#include "GameObjects/RenderStaticObject.h"
#include "GameObjects/LandObject.h"
#include "GameObjects/StaticObject.h"
#include "GameObjects/MultiObject.h"
#include "GameObjects/CustomHouseMultiObject.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/GameItem.h"
#include "GameObjects/GameCharacter.h"
#include "GameObjects/GameEffect.h"
#include "GameObjects/GameEffectDrag.h"
#include "GameObjects/GameEffectMoving.h"
#include "GameObjects/GamePlayer.h"
#include "GameObjects/GameWorld.h"
#include "GameObjects/MapBlock.h"
#include "GameObjects/ObjectOnCursor.h"

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
#include "Gumps/GumpProperty.h"
#include "Gumps/GumpPropertyIcon.h"

#include "Managers/AnimationManager.h"
#include "Managers/CityManager.h"
#include "CityList.h"
#include "Managers/ClilocManager.h"
#include "Managers/IntlocManager.h"
#include "Managers/ObjectPropertiesManager.h"
#include "Managers/ColorManager.h"
#include "Managers/ConfigManager.h"
#include "Managers/ConnectionManager.h"
#include "Managers/CreateCharacterManager.h"
#include "Managers/EffectManager.h"
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

#include "ScreenStages/BaseScreen.h"
#include "ScreenStages/CharacterListScreen.h"
#include "ScreenStages/ConnectionScreen.h"
#include "ScreenStages/CreateCharacterScreen.h"
#include "ScreenStages/GameBlockedScreen.h"
#include "ScreenStages/GameScreen.h"
#include "ScreenStages/MainScreen.h"
#include "ScreenStages/SelectProfessionScreen.h"
#include "ScreenStages/SelectTownScreen.h"
#include "ScreenStages/ServerScreen.h"

//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
