if(CMAKE_SYSTEM_NAME MATCHES "Windows")
  add_definitions(-DNOMINMAX)
  add_definitions(-DUNICODE)
  add_definitions(-D_UNICODE)
  add_definitions(-DWIN32_LEAN_AND_MEAN)
  add_definitions(-D_WIN32_WINNT=0x0602)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
endif()

if(CMAKE_C_COMPILER_ID MATCHES "MSVC")
  # enable the latest C++ standard feature set,
  # and also disable MSVC specific extensions
  # to be even more standards compliant.
  check_and_add_flag(CPPLATEST /std:c++latest)
  check_and_add_flag(STANDARD_COMPLIANCE /permissive-)
else()
  # Enable C++17, but fall back to C++14 if it isn't available.
  # CMAKE_CXX_STANDARD cannot be used here because we require C++14 or newer, not any standard.
  check_and_add_flag(CXX17 -std=c++17)
  if(NOT FLAG_CXX_CXX17)
    set(CMAKE_CXX_STANDARD 14)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
  endif()
  check_and_add_flag(THREADS -pthread)
  set(THREAD "-pthread")
endif()

# These aren't actually needed for C11/C++11
# but some dependencies require them (LLVM, libav).
add_definitions(-D__STDC_LIMIT_MACROS)
add_definitions(-D__STDC_CONSTANT_MACROS)

add_custom_target(PCH COMMAND "$CXX -cc1 \"${pch_hdr}\" -emit-pch -o \"${pch}\"")

set(wisp_src
  Wisp/WispApplication.cpp        
  Wisp/WispConnection.cpp  
  Wisp/WispGeometry.cpp  
  Wisp/WispLogger.cpp      
  Wisp/WispMouse.cpp          
  Wisp/WispPacketReader.cpp    
  Wisp/WispThread.cpp         
  Wisp/WispWindow.cpp
  Wisp/WispBinaryFileWritter.cpp  
  Wisp/WispDataStream.cpp  
  Wisp/WispGlobal.cpp    
  Wisp/WispMappedFile.cpp  
  Wisp/WispPacketMessage.cpp  
  Wisp/WispTextFileParser.cpp  
  Wisp/WispThreadedTimer.cpp
)
add_library(wisp ${wisp_src})

set(gameobjects_src
  GameObjects/CustomHouseMultiObject.cpp  
  GameObjects/GameEffect.cpp        
  GameObjects/GameItem.cpp    
  GameObjects/GameWorld.cpp   
  GameObjects/MapObject.cpp       
  GameObjects/RenderStaticObject.cpp
  GameObjects/CustomHouseObjects.cpp      
  GameObjects/GameEffectDrag.cpp    
  GameObjects/GameObject.cpp  
  GameObjects/LandObject.cpp  
  GameObjects/MultiObject.cpp     
  GameObjects/StaticObject.cpp
  GameObjects/GameCharacter.cpp           
  GameObjects/GameEffectMoving.cpp  
  GameObjects/GamePlayer.cpp  
  GameObjects/MapBlock.cpp    
  GameObjects/ObjectOnCursor.cpp
)
add_library(gameobjects ${gameobjects_src})

set(glengine_src
  GLEngine/GLEngine.cpp
  GLEngine/GLFrameBuffer.cpp
  GLEngine/GLHTMLTextTexture.cpp
  GLEngine/GLShader.cpp
  GLEngine/GLTextTexture.cpp
  GLEngine/GLTextureCircleOfTransparency.cpp
  GLEngine/GLTexture.cpp
  GLEngine/GLVector.cpp
)
add_library(glengine ${glengine_src}) # WGLEW_ARB_render_texture

set(gui_src
  GUI/BaseGUI.cpp
  GUI/GUIAlphaBlending.cpp
  GUI/GUIBlending.cpp
  GUI/GUIBuff.cpp
  GUI/GUIBulletinBoardObject.cpp
  GUI/GUIButton.cpp
  GUI/GUIButtonTileart.cpp
  GUI/GUICheckbox.cpp
  GUI/GUIChecktrans.cpp
  GUI/GUIColoredPolygone.cpp
  GUI/GUIComboBox.cpp
  GUI/GUIComboboxText.cpp
  GUI/GUIDataBox.cpp
  GUI/GUIDrawObject.cpp
  GUI/GUIExternalTexture.cpp
  GUI/GUIGenericText.cpp
  GUI/GUIGenericTextEntry.cpp
  GUI/GUIGlobalColor.cpp
  GUI/GUIGroup.cpp
  GUI/GUIGumppic.cpp
  GUI/GUIGumppicHightlighted.cpp
  GUI/GUIGumppicTiled.cpp
  GUI/GUIHitBox.cpp
  GUI/GUIHTMLButton.cpp
  GUI/GUIHTMLGump.cpp
  GUI/GUIHTMLHitBox.cpp
  GUI/GUIHTMLResizepic.cpp
  GUI/GUIHTMLSlider.cpp
  GUI/GUIHTMLText.cpp
  GUI/GUILine.cpp
  GUI/GUIMenuObject.cpp
  GUI/GUIMinMaxButtons.cpp
  GUI/GUIPage.cpp
  GUI/GUIPolygonal.cpp
  GUI/GUIRadio.cpp
  GUI/GUIResizeButton.cpp
  GUI/GUIResizepic.cpp
  GUI/GUIScissor.cpp
  GUI/GUIScrollBackground.cpp
  GUI/GUIShader.cpp
  GUI/GUIShopItem.cpp
  GUI/GUIShopResult.cpp
  GUI/GUISkillGroup.cpp
  GUI/GUISkillItem.cpp
  GUI/GUISlider.cpp
  GUI/GUIText.cpp
  GUI/GUITextEntry.cpp
  GUI/GUITilepic.cpp
  GUI/GUITilepicHightlighted.cpp
  GUI/GUITilepicScaled.cpp
  GUI/GUIVirtireGump.cpp
  GUI/GUIWorldMapTexture.cpp
)
add_library(gui ${gui_src})

set(gumps_src
  Gumps/GumpAbility.cpp
  Gumps/GumpBaseScroll.cpp
  Gumps/GumpBook.cpp
  Gumps/GumpBuff.cpp
  Gumps/GumpBulletinBoard.cpp
  Gumps/GumpBulletinBoardItem.cpp
  Gumps/GumpCombatBook.cpp
  Gumps/GumpConsoleType.cpp
  Gumps/GumpContainer.cpp
  Gumps/Gump.cpp
  Gumps/GumpCustomHouse.cpp
  Gumps/GumpDrag.cpp
  Gumps/GumpDye.cpp
  Gumps/GumpGeneric.cpp
  Gumps/GumpGrayMenu.cpp
  Gumps/GumpJournal.cpp
  Gumps/GumpMap.cpp
  Gumps/GumpMenubar.cpp
  Gumps/GumpMenu.cpp
  Gumps/GumpMinimap.cpp
  Gumps/GumpNotify.cpp
  Gumps/GumpOptions.cpp
  Gumps/GumpPaperdoll.cpp
  Gumps/GumpPartyManifest.cpp
  Gumps/GumpPopupMenu.cpp
  Gumps/GumpProfile.cpp
  Gumps/GumpProperty.cpp
  Gumps/GumpPropertyIcon.cpp
  Gumps/GumpQuestion.cpp
  Gumps/GumpRacialAbilitiesBook.cpp
  Gumps/GumpRacialAbility.cpp
  Gumps/GumpScreenCharacterList.cpp
  Gumps/GumpScreenConnection.cpp
  Gumps/GumpScreenCreateCharacter.cpp
  Gumps/GumpScreenGame.cpp
  Gumps/GumpScreenMain.cpp
  Gumps/GumpScreenSelectProfession.cpp
  Gumps/GumpScreenSelectTown.cpp
  Gumps/GumpScreenServer.cpp
  Gumps/GumpSecureTrading.cpp
  Gumps/GumpSelectColor.cpp
  Gumps/GumpSelectFont.cpp
  Gumps/GumpShop.cpp
  Gumps/GumpSkill.cpp
  Gumps/GumpSkills.cpp
  Gumps/GumpSpellbook.cpp
  Gumps/GumpSpell.cpp
  Gumps/GumpStatusbar.cpp
  Gumps/GumpTargetSystem.cpp
  Gumps/GumpTextEntryDialog.cpp
  Gumps/GumpTip.cpp
  Gumps/GumpWorldMap.cpp
)
add_library(gumps ${gumps_src})

set(managers_src
  Managers/AnimationManager.cpp
  Managers/CityManager.cpp
  Managers/ClilocManager.cpp
  Managers/ColorManager.cpp
  Managers/ConfigManager.cpp
  Managers/ConnectionManager.cpp
  Managers/CorpseManager.cpp
  Managers/CreateCharacterManager.cpp
  Managers/CustomHousesManager.cpp
  Managers/EffectManager.cpp
  Managers/FileManager.cpp
  Managers/FontsManager.cpp
  Managers/GumpManager.cpp
  Managers/IntlocManager.cpp
  Managers/MacroManager.cpp
  Managers/MapManager.cpp
  Managers/MouseManager.cpp
  Managers/ObjectPropertiesManager.cpp
  Managers/OptionsMacroManager.cpp
  Managers/PacketManager.cpp
  Managers/PluginManager.cpp
  Managers/ProfessionManager.cpp
  Managers/ScreenEffectManager.cpp
  Managers/SkillGroupManager.cpp
  Managers/SkillsManager.cpp
  Managers/SoundManager.cpp
  Managers/SpeechManager.cpp
  Managers/UOFileReader.cpp
)
add_library(managers ${managers_src})

set(network_src
  Network/Connection.cpp
  Network/Packets.cpp
  Network/PluginPackets.cpp
  Network/UOHuffman.cpp
)
add_library(network ${network_src})

set(screenstages_src
  ScreenStages/BaseScreen.cpp
  ScreenStages/CharacterListScreen.cpp
  ScreenStages/ConnectionScreen.cpp
  ScreenStages/CreateCharacterScreen.cpp
  ScreenStages/GameBlockedScreen.cpp
  ScreenStages/GameScreen.cpp
  ScreenStages/MainScreen.cpp
  ScreenStages/SelectProfessionScreen.cpp
  ScreenStages/SelectTownScreen.cpp
  ScreenStages/ServerScreen.cpp
)
add_library(screenstages ${screenstages_src})

set(textengine_src
  TextEngine/EntryText.cpp
  TextEngine/GameConsole.cpp
  TextEngine/Journal.cpp
  TextEngine/RenderTextObject.cpp
  TextEngine/TextContainer.cpp
  TextEngine/TextData.cpp
  TextEngine/TextRenderer.cpp
)
add_library(textengine ${textengine_src})

set(utility_src
  Utility/AutoResetEvent.cpp
  Utility/PingThread.cpp
)
add_library(utility ${utility_src})

set(walker_src
  Walker/FastWalk.cpp
  Walker/PathFinder.cpp
  Walker/PathNode.cpp
  Walker/WalkData.cpp
  Walker/Walker.cpp
)
add_library(walker ${walker_src})

set(orion_src
  BaseQueue.cpp
  CharacterList.cpp
  CityList.cpp
  ClickObject.cpp
  CommonInterfaces.cpp
  Container.cpp
  ContainerStack.cpp
  DateTimeStamp.cpp
  Globals.cpp
  ImageBounds.cpp
  IndexObject.cpp
  Macro.cpp
  Multi.cpp
  MultiMap.cpp
  OrionApplication.cpp
  OrionUO.cpp                 # FIXME: localtime_s c11
  OrionWindow.cpp
  Party.cpp
  PartyObject.cpp
  PressedObject.cpp
  Profession.cpp
  QuestArrow.cpp
  RenderObject.cpp
  RenderWorldObject.cpp
  ScreenshotBuilder.cpp
  SelectedObject.cpp
  ServerList.cpp
  SkillGroup.cpp
  stdafx.cpp
  Target.cpp
  TargetGump.cpp
  TextureObject.cpp
  ToolTip.cpp
  UseItemsList.cpp
  Weather.cpp
  WeatherEffeect.cpp

  Stubs.cpp                     # FIXME: temporary stubbed stuff
)

if(ORION_WINDOWS)
  set(orion_src ${orion_src}
    VMQuery.cpp                 # FIXME: windows only, rap, coredump
    StackWalker.cpp             # FIXME: windows only, wrap, use backtrace
    ExceptionFilter.cpp         # FIXME: exception handler, wrap, use signals
  )
endif()

add_executable(OrionUO ${orion_src})
#add_executable(OrionUO ${orion_src} ${gameobjects_src} ${glengine_src} ${gui_src} ${gumps_src} ${managers_src} ${network_src} ${screenstages_src} ${textengine_src} ${utility_src} ${walker_src})


if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  #target_link_libraries(wisp PUBLIC dl rt)
  add_definitions(-DNULL=nullptr)
endif()

include_directories("${PROJECT_SOURCE_DIR}/OrionUO/GameObjects")
include_directories("${PROJECT_SOURCE_DIR}/OrionUO/GLEngine")
include_directories("${PROJECT_SOURCE_DIR}/OrionUO/GUI")
include_directories("${PROJECT_SOURCE_DIR}/OrionUO/Gumps")
include_directories("${PROJECT_SOURCE_DIR}/OrionUO/Managers")
include_directories("${PROJECT_SOURCE_DIR}/OrionUO/Network")
include_directories("${PROJECT_SOURCE_DIR}/OrionUO/ScreenStages")
include_directories("${PROJECT_SOURCE_DIR}/OrionUO/TextEngine")
include_directories("${PROJECT_SOURCE_DIR}/OrionUO/Utility")
include_directories("${PROJECT_SOURCE_DIR}/OrionUO/Walker")
include_directories("${PROJECT_SOURCE_DIR}/OrionUO/Wisp")
include_directories("${PROJECT_SOURCE_DIR}/OrionUO/")

target_link_libraries(screenstages PRIVATE gumps)
target_link_libraries(gumps PRIVATE managers)
target_link_libraries(OrionUO PUBLIC managers network gumps screenstages gameobjects glengine gui textengine utility walker wisp)
#${SDL2_IMAGE_LIBRARIES}
target_link_libraries(OrionUO PUBLIC ${SDL2_LIBRARIES} ${ZLIB_LIBRARIES} ${OPENGL_LIBRARIES} ${FREEIMAGE_LIBRARIES} ${GLEW_LIBRARIES} ${THREAD})
