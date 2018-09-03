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

if(ORION_DLL_STATIC)
  add_definitions(-DUSE_ORIONDLL=0)
else()
  add_definitions(-DUSE_ORIONDLL=1)
endif()

if(ORION_WISP)
  add_definitions(-DUSE_WISP=1)
else()
  add_definitions(-DUSE_WISP=0)
endif()

if(CMAKE_C_COMPILER_ID MATCHES "MSVC")
  # enable the latest C++ standard feature set,
  # and also disable MSVC specific extensions
  # to be even more standards compliant.
  check_and_add_flag(CPPLATEST /std:c++latest)
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

# WGLEW_ARB_render_texture
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
  GUI/GUIVirtireGump.cpp             # FIXME: filename GUIVirtureGump
  GUI/GUIWorldMapTexture.cpp
)

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

set(network_src
  Network/Connection.cpp
  Network/Packets.cpp
  Network/PluginPackets.cpp
  Network/UOHuffman.cpp
)

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

set(textengine_src
  TextEngine/EntryText.cpp
  TextEngine/GameConsole.cpp
  TextEngine/Journal.cpp
  TextEngine/RenderTextObject.cpp
  TextEngine/TextContainer.cpp
  TextEngine/TextData.cpp
  TextEngine/TextRenderer.cpp
)

set(utility_src
  Utility/AutoResetEvent.cpp
  Utility/PingThread.cpp
)

set(walker_src
  Walker/FastWalk.cpp
  Walker/PathFinder.cpp
  Walker/PathNode.cpp
  Walker/WalkData.cpp
  Walker/Walker.cpp
)

set(crypt_src
  Crypt/GameCrypt.cpp
  Crypt/LoginCrypt.cpp
  Crypt/md5.cpp
  Crypt/twofish.cpp
  Crypt/CryptEntry.cpp
)

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
  WeatherEffeect.cpp            # FIXME: filename

  FileSystem.cpp
  Sockets.cpp
  OrionMain.cpp
  Stubs.cpp                     # FIXME: temporary stubbed stuff
)

if(ORION_WINDOWS)
  set(orion_src ${orion_src}
    VMQuery.cpp                 # FIXME: windows only, rap, coredump
    StackWalker.cpp             # FIXME: windows only, wrap, use backtrace
    ExceptionFilter.cpp         # FIXME: exception handler, wrap, use signals
    OrionUO.rc
  )
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

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  #target_link_libraries(wisp PUBLIC dl rt)
  add_definitions(-DNULL=nullptr)
endif()

macro(orion_library TARGET SRCS)
  string(REPLACE ".cpp" ".h" headers "${SRCS}")
  string(REPLACE "GUI/GUIVirtireGump.h" "GUI/GUIVirtureGump.h" headers "${headers}")
  string(REPLACE "Crypt/twofish.h" "" headers "${headers}")
  add_library(${TARGET} ${SRCS} ${headers})
  set_target_properties(${TARGET} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "stdafx.h")
  cotire(${TARGET})
endmacro()

if(ORION_BUILD_MODULES)
  orion_library(wisp "${wisp_src}")
  orion_library(gameobjects "${gameobjects_src}")
  orion_library(glengine "${glengine_src}")
  orion_library(gui "${gui_src}")
  orion_library(gumps "${gumps_src}")
  orion_library(managers "${managers_src}")
  orion_library(network "${network_src}")
  orion_library(screenstages "${screenstages_src}")
  orion_library(textengine "${textengine_src}")
  orion_library(utility "${utility_src}")
  orion_library(walker "${walker_src}")
  orion_library(crypt "${crypt_src}")

  string(REPLACE ".cpp" ".h" orion_hdr "${orion_src}")
  string(REPLACE "WeatherEffeect.h" "WeatherEffect.h" orion_hdr "${orion_hdr}")
  add_executable(OnionUO ${orion_src} ${orion_hdr})
  target_link_libraries(screenstages PRIVATE gumps)
  target_link_libraries(gumps PRIVATE managers)
  target_link_libraries(OnionUO PUBLIC managers network gumps screenstages gameobjects glengine gui textengine utility walker crypt wisp)
else()
  set(SRCS ${orion_src} ${gameobjects_src} ${glengine_src} ${gui_src} ${gumps_src} ${managers_src} ${network_src} ${screenstages_src} ${textengine_src} ${utility_src} ${walker_src} ${crypt_src} ${wisp_src})
  string(REPLACE ".cpp" ".h" headers "${SRCS}")
  string(REPLACE "WeatherEffeect.h" "WeatherEffect.h" headers "${headers}")
  string(REPLACE "GUI/GUIVirtireGump.h" "GUI/GUIVirtureGump.h" headers "${headers}")
  string(REPLACE "Crypt/twofish.h" "" headers "${headers}")
  source_group(TREE "${PROJECT_SOURCE_DIR}/OrionUO/" FILES ${SRCS} ${headers})
  add_executable(OnionUO ${SRCS} ${headers})
endif()

target_link_libraries(OnionUO PUBLIC ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${ZLIB_LIBRARIES} ${OPENGL_LIBRARIES} ${FREEIMAGE_LIBRARIES} ${GLEW_LIBRARIES} ${THREAD})

set_target_properties(OnionUO PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "stdafx.h")
cotire(OnionUO)

if(ORION_WINDOWS)
  set_target_properties(OnionUO PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
  set_target_properties(OnionUO_unity PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
  if(ORION_64BITS)
    add_custom_command(TARGET OnionUO POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/test_bin64" $<TARGET_FILE_DIR:OnionUO>)
    add_custom_command(TARGET OnionUO_unity POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/test_bin64" $<TARGET_FILE_DIR:OnionUO_unity>)
  else()
    add_custom_command(TARGET OnionUO POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/bin" $<TARGET_FILE_DIR:OnionUO>)
    add_custom_command(TARGET OnionUO_unity POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/bin" $<TARGET_FILE_DIR:OnionUO_unity>)
  endif()
endif()
