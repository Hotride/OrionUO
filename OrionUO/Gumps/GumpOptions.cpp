/***********************************************************************************
**
** GumpOptions.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
const ushort g_OptionsTextColor = 0;
//----------------------------------------------------------------------------------
CGumpOptions::CGumpOptions(short x, short y)
: CGump(GT_OPTIONS, 0, x, y)
{
	m_Page = 1;
}
//----------------------------------------------------------------------------------
CGumpOptions::~CGumpOptions()
{
}
//----------------------------------------------------------------------------------
void CGumpOptions::CalculateGumpState()
{
	WISPFUN_DEBUG("c104_f1");
	CGump::CalculateGumpState();

	if (g_GumpPressed)
	{
		if (g_PressedObject.LeftObject != NULL && ((CBaseGUI*)g_PressedObject.LeftObject)->Type == GOT_COMBOBOX)
		{
			g_GumpMovingOffset.Reset();

			if (m_Minimized)
			{
				g_GumpTranslate.X = (float)m_MinimizedX;
				g_GumpTranslate.Y = (float)m_MinimizedY;
			}
			else
			{
				g_GumpTranslate.X = (float)m_X;
				g_GumpTranslate.Y = (float)m_Y;
			}
		}
		else
			m_WantRedraw = true;
	}
}
//----------------------------------------------------------------------------
void CGumpOptions::PrepareContent()
{
	WISPFUN_DEBUG("c104_f2");
	if (m_WantRedrawMacroData)
	{
		RedrawMacroData();
		m_WantRedraw = true;
	}
}
//----------------------------------------------------------------------------
void CGumpOptions::UpdateContent()
{
	WISPFUN_DEBUG("c104_f3");
	Clear();

	//Body
	Add(new CGUIResizepic(0, 0x0A28, 40, 0, 550, 450));



	//Left page buttons

	//Sound and Music
	CGUIButton *button = (CGUIButton*)Add(new CGUIButton(ID_GO_PAGE_1, 0x00DA, 0x00DA, 0x00DA, 0, 45));
	button->ToPage = 1;
	//Orion's configuration
	button = (CGUIButton*)Add(new CGUIButton(ID_GO_PAGE_2, 0x00DC, 0x00DC, 0x00DC, 0, 111));
	button->ToPage = 2;
	//Language
	button = (CGUIButton*)Add(new CGUIButton(ID_GO_PAGE_3, 0x00DE, 0x00DE, 0x00DE, 0, 177));
	button->ToPage = 3;
	//Chat
	button = (CGUIButton*)Add(new CGUIButton(ID_GO_PAGE_4, 0x00E0, 0x00E0, 0x00E0, 0, 243));
	button->ToPage = 4;
	//Macro Options
	button = (CGUIButton*)Add(new CGUIButton(ID_GO_PAGE_5, 0x00ED, 0x00ED, 0x00ED, 0, 309));
	button->ToPage = 5;



	//Right page buttons

	//Interface
	button = (CGUIButton*)Add(new CGUIButton(ID_GO_PAGE_6, 0x00E2, 0x00E2, 0x00E2, 576, 45));
	button->ToPage = 6;
	//Display
	button = (CGUIButton*)Add(new CGUIButton(ID_GO_PAGE_7, 0x00E4, 0x00E4, 0x00E4, 576, 111));
	button->ToPage = 7;
	//Reputation System
	button = (CGUIButton*)Add(new CGUIButton(ID_GO_PAGE_8, 0x00E6, 0x00E6, 0x00E6, 576, 177));
	button->ToPage = 8;
	//Miscellaneous
	button = (CGUIButton*)Add(new CGUIButton(ID_GO_PAGE_9, 0x00E8, 0x00E8, 0x00E8, 576, 243));
	button->ToPage = 9;
	//Filter Options
	button = (CGUIButton*)Add(new CGUIButton(ID_GO_PAGE_10, 0x00EB, 0x00EB, 0x00EB, 576, 309));
	button->ToPage = 10;



	Add(new CGUIButton(ID_GO_CANCEL, 0x00F3, 0x00F2, 0x00F1, 154, 405));
	Add(new CGUIButton(ID_GO_APPLY, 0x00EF, 0x00EE, 0x00F0, 248, 405));
	Add(new CGUIButton(ID_GO_DEFAULT, 0x00F6, 0x00F5, 0x00F4, 346, 405));
	Add(new CGUIButton(ID_GO_OKAY, 0x00F9, 0x00F7, 0x00F8, 443, 405));



	DrawPage1(); //Sound and Music
	DrawPage2(); //Orion's configuration
	DrawPage3(); //Language
	DrawPage4(); //Chat
	DrawPage5(); //Macro Options
	DrawPage6(); //Interface
	DrawPage7(); //Display
	DrawPage8(); //Reputation System
	DrawPage9(); //Miscellaneous
	DrawPage10(); //Filter Options

	RedrawMacroData();
}
//----------------------------------------------------------------------------
void CGumpOptions::Init()
{
	WISPFUN_DEBUG("c104_f4");
	g_OptionsMacroManager.LoadFromMacro();
	g_OptionsDeveloperMode = g_DeveloperMode;

	m_MacroPointer = (CMacro*)g_OptionsMacroManager.m_Items;
	m_MacroObjectPointer = (CMacroObject*)m_MacroPointer->m_Items;

	m_WantUpdateContent = true;
}
//---------------------------------------------------------------------------
void CGumpOptions::InitToolTip()
{
	WISPFUN_DEBUG("c104_f5");
	uint id = g_SelectedObject.Serial;

		switch (id)
		{
			case ID_GO_PAGE_1:
			{
				g_ToolTip.Set(L"See the 'Sound and Music' configuration page");
				break;
			}
			case ID_GO_PAGE_2:
			{
				g_ToolTip.Set(L"See the 'Orion' configuration page");
				break;
			}
			case ID_GO_PAGE_3:
			{
				g_ToolTip.Set(L"See the 'Language' configuration page");
				break;
			}
			case ID_GO_PAGE_4:
			{
				g_ToolTip.Set(L"See the 'Chat' configuration page");
				break;
			}
			case ID_GO_PAGE_5:
			{
				g_ToolTip.Set(L"See the 'Macro' configuration page");
				break;
			}
			case ID_GO_PAGE_6:
			{
				g_ToolTip.Set(L"See the 'Interface' configuration page");
				break;
			}
			case ID_GO_PAGE_7:
			{
				g_ToolTip.Set(L"See the 'Display' configuration page");
				break;
			}
			case ID_GO_PAGE_8:
			{
				g_ToolTip.Set(L"See the 'Reputation System' configuration page");
				break;
			}
			case ID_GO_PAGE_9:
			{
				g_ToolTip.Set(L"See the 'Miscellaneous' configuration page");
				break;
			}
			case ID_GO_PAGE_10:
			{
				g_ToolTip.Set(L"See the 'Filter' configuration page");
				break;
			}
			case ID_GO_CANCEL:
			{
				g_ToolTip.Set(L"Cancel changes and close the configuration gump");
				break;
			}
			case ID_GO_APPLY:
			{
				g_ToolTip.Set(L"Apply changes on current page");
				break;
			}
			case ID_GO_DEFAULT:
			{
				g_ToolTip.Set(L"Set default values on current page");
				break;
			}
			case ID_GO_OKAY:
			{
				g_ToolTip.Set(L"Apply changes on current page and close the configuration gump");
				break;
			}
			case ID_GO_P1_SOUND_ON_OFF:
			{
				g_ToolTip.Set(L"On/off sound effects");
				break;
			}
			case ID_GO_P1_MUSIC_ON_OFF:
			{
				g_ToolTip.Set(L"On/off music");
				break;
			}
			case ID_GO_P1_PLAY_FOOTSTEP_SOUNDS:
			{
				g_ToolTip.Set(L"On/off footstep sound effects");
				break;
			}
			case ID_GO_P1_PLAY_COMBAT_MUSIC:
			{
				g_ToolTip.Set(L"On/off combat music in war mode");
				break;
			}
			case ID_GO_P1_SOUND_VOLUME:
			{
				g_ToolTip.Set(L"Sound effects volume value");
				break;
			}
			case ID_GO_P1_MUSIC_VOLUME:
			{
				g_ToolTip.Set(L"Music volume value");
				break;
			}
			case ID_GO_P2_CLIENT_FPS:
			{
				g_ToolTip.Set(L"Orion's FPS value");
				break;
			}
			case ID_GO_P2_REDUCE_FPS_UNACTIVE_WINDOW:
			{
				g_ToolTip.Set(L"Reduce FPS when Orion's window is unactive");
				break;
			}
			case ID_GO_P2_CHARACTERS_ANIMATION_DELAY:
			{
				g_ToolTip.Set(L"Original characters animation frame rate");
				break;
			}
			case ID_GO_P2_ITEMS_ANIMATION_DELAY:
			{
				g_ToolTip.Set(L"Original items animation frame rate");
				break;
			}
			case ID_GO_P2_ENABLE_SCALING:
			{
				g_ToolTip.Set(L"Use middle mouse for scaling the game window");
				break;
			}
			case ID_GO_P2_REMOVE_TEXT_WITH_BLENDING:
			{
				g_ToolTip.Set(L"World/container's text removing with using alpha-blending");
				break;
			}
			case ID_GO_P2_NO_DRAW_CHARACTERS_STATUS:
			{
				g_ToolTip.Set(L"No draw character's status in the game window");
				break;
			}
			case ID_GO_P2_DRAW_CHARACTERS_STATUS_TOP:
			{
				g_ToolTip.Set(L"Draw character's status (text) in the game window above characters");
				break;
			}
			case ID_GO_P2_DRAW_CHARACTERS_STATUS_BOTTOM:
			{
				g_ToolTip.Set(L"Draw character's status (line) in the game window under characters");
				break;
			}
			case ID_GO_P2_DRAW_CHARACTER_BARS_ALWAYS:
			{
				g_ToolTip.Set(L"Always draw character's status in the game window");
				break;
			}
			case ID_GO_P2_DRAW_CHARACTER_BARS_NOT_MAX:
			{
				g_ToolTip.Set(L"Draw character's status in the game window if hits is not equals max hits");
				break;
			}
			case ID_GO_P2_DRAW_CHARACTER_BARS_LOWER:
			{
				g_ToolTip.Set(L"Draw character's status in the game window if hits lower value");
				break;
			}
			case ID_GO_P2_DRAW_STUMPS:
			{
				g_ToolTip.Set(L"Change trees to stumps and hide leaves");
				break;
			}
			case ID_GO_P2_MARKING_CAVES:
			{
				g_ToolTip.Set(L"Marking cave tiles to grid");
				break;
			}
			case ID_GO_P2_NO_VEGETATION:
			{
				g_ToolTip.Set(L"Hide vegetation static objects");
				break;
			}
			case ID_GO_P2_HIDDEN_CHARACTES_MODE_1:
			{
				g_ToolTip.Set(L"Default hidden characters");
				break;
			}
			case ID_GO_P2_HIDDEN_CHARACTES_MODE_2:
			{
				g_ToolTip.Set(L"Hidden characters drawn with alpha-blending");
				break;
			}
			case ID_GO_P2_HIDDEN_CHARACTES_MODE_3:
			{
				g_ToolTip.Set(L"Hidden characters drawn with spectral color");
				break;
			}
			case ID_GO_P2_HIDDEN_CHARACTES_MODE_4:
			{
				g_ToolTip.Set(L"Hidden characters drawn with special spectral color");
				break;
			}
			case ID_GO_P2_HIDDEN_ALPHA:
			{
				g_ToolTip.Set(L"Value of alpha channel for using in hidden mode");
				break;
			}
			case ID_GO_P2_USE_HIDDEN_MODE_ONLY_FOR_SELF:
			{
				g_ToolTip.Set(L"Change hidden characters mode only for your person");
				break;
			}
			case ID_GO_P2_NO_ANIMATE_FIELDS:
			{
				g_ToolTip.Set(L"Disable the field animation");
				break;
			}
			case ID_GO_P2_LOCK_GUMP_MOVING:
			{
				g_ToolTip.Set(L"Enable gump's locking");
				break;
			}
			case ID_GO_P2_TRANSPARENT_SPELL_ICONS:
			{
				g_ToolTip.Set(L"Transparent spell icons");
				break;
			}
			case ID_GO_P2_OLD_STYLE_STATUSBAR:
			{
				g_ToolTip.Set(L"Draw your maximized statusbar gump in old-style (low info gump, for clients >= 3.0.8d)", 160);
				break;
			}
			case ID_GO_P2_ORIGINAL_PARTY_STATUSBAR:
			{
				g_ToolTip.Set(L"Draw original minimized statusbar gump instead a party statusbar gump", 160);
				break;
			}
			case ID_GO_P2_APPLY_STATE_COLOR_ON_CHARACTERS:
			{
				g_ToolTip.Set(L"Colorize poisoned or paralyzed characters in the world", 160);
				break;
			}
			case ID_GO_P2_SPELL_ICONS_ALPHA:
			{
				g_ToolTip.Set(L"Value of alpha channel for spell icons");
				break;
			}
			case ID_GO_P2_CONSOLE_ENTER:
			{
				g_ToolTip.Set(L"Activate chat after 'Enter' pressing");
				break;
			}
			case ID_GO_P2_CHANGE_FIELDS_GRAPHIC:
			{
				g_ToolTip.Set(L"Replace animated fields images to tiles");
				break;
			}
			case ID_GO_P2_PAPERDOLL_SLOTS:
			{
				g_ToolTip.Set(L"Add slots in paperdoll gump for:\nHelmet\nEarrings\nNecklace\nRing\nBracelet\nBody Sash");
				break;
			}
			case ID_GO_P2_REMOVE_STATUSBARS_WITHOUT_OBJECTS:
			{
				g_ToolTip.Set(L"Remove statusbars, whose objects have been removed");
				break;
			}
			case ID_GO_P2_SHOW_CONSOLE_ENTRY_MODE:
			{
				g_ToolTip.Set(L"Show console entry mode under game window.");
				break;
			}
			case ID_GO_P2_DRAW_AURA_NEVER:
			{
				g_ToolTip.Set(L"Disable the aura under characters");
				break;
			}
			case ID_GO_P2_DRAW_AURA_IN_WARMODE:
			{
				g_ToolTip.Set(L"Draw aura under characters only if your person in war mode");
				break;
			}
			case ID_GO_P2_DRAW_AURA_ALWAYS:
			{
				g_ToolTip.Set(L"Always draw aura under characters");
				break;
			}
			case ID_GO_P2_DRAW_AURA_WITH_CTRL_PRESSED:
			{
				g_ToolTip.Set(L"Draw aura under characters only if Ctrl pressed");
				break;
			}
			case ID_GO_P2_SCREENSHOT_FORMAT_BMP:
			{
				g_ToolTip.Set(L"Save screen shots in BMP format");
				break;
			}
			case ID_GO_P2_SCREENSHOT_FORMAT_PNG:
			{
				g_ToolTip.Set(L"Save screen shots in PNG format");
				break;
			}
			case ID_GO_P2_SCREENSHOT_FORMAT_TIFF:
			{
				g_ToolTip.Set(L"Save screen shots in TIFF format");
				break;
			}
			case ID_GO_P2_SCREENSHOT_FORMAT_JPEG:
			{
				g_ToolTip.Set(L"Save screen shots in JPEG format (maybe crashes)");
				break;
			}
			case ID_GO_P2_SCALE_IMAGES_IN_PAPERDOLL_SLOTS:
			{
				g_ToolTip.Set(L"Scale images in paperdoll slots");
				break;
			}
			case ID_GO_P2_REMOVE_OR_CREATE_OBJECTS_WITH_BLENDING:
			{
				g_ToolTip.Set(L"Remove or new draw objects (static/game) displaying with blending");
				break;
			}
			case ID_GO_P2_DRAW_HELMETS_ON_SHROUD:
			{
				g_ToolTip.Set(L"Draw helmets on shrouds in the world");
				break;
			}
			case ID_GO_P2_USE_GLOBAL_MAP_LAYER:
			{
				g_ToolTip.Set(L"Draw world map before all gumps");
				break;
			}
			case ID_GO_P2_NO_DRAW_ROOFS:
			{
				g_ToolTip.Set(L"No draw roofs");
				break;
			}
			case ID_GO_P2_HIGHLIGHT_TARGET_BY_TYPE:
			{
				g_ToolTip.Set(L"Highlight target by type (netural, harmful, helpful)");
				break;
			}
			case ID_GO_P2_AUTO_DISPLAY_WORLD_MAP:
			{
				g_ToolTip.Set(L"Display a world map immediately after entering the world");
				break;
			}
			case ID_GO_P2_USE_GL_LISTS_FOR_INTERFACE:
			{
				g_ToolTip.Set(L"Use GL lists for draw interface gumps");
				break;
			}
			case ID_GO_P2_DEV_MODE_1:
			{
				g_ToolTip.Set(L"Original client work");
				break;
			}
			case ID_GO_P2_DEV_MODE_2:
			{
				g_ToolTip.Set(L"Show only FPS and game window scaling on screen");
				break;
			}
			case ID_GO_P2_DEV_MODE_3:
			{
				g_ToolTip.Set(L"Show debugging information in game window and highlight the land/static tiles");
				break;
			}
			case ID_GO_P3_USE_TOOLTIP:
			{
				g_ToolTip.Set(L"Use the tooltips");
				break;
			}
			case ID_GO_P3_TEXT_COLOR:
			{
				g_ToolTip.Set(L"Tooltips text color");
				break;
			}
			case ID_GO_P3_TEXT_FONT:
			{
				g_ToolTip.Set(L"Open the selection font gump for select tooltips font");
				break;
			}
			case ID_GO_P3_DELAY_BEFORE_TOOLTIP:
			{
				g_ToolTip.Set(L"Delay before tooltip shown");
				break;
			}
			case ID_GO_P4_TEXT_FONT:
			{
				g_ToolTip.Set(L"Open the selection font gump for select chat font");
				break;
			}
			case ID_GO_P5_BUTTON_ADD:
			{
				g_ToolTip.Set(L"Add a new macro");
				break;
			}
			case ID_GO_P5_BUTTON_DELETE:
			{
				g_ToolTip.Set(L"Delete current macro");
				break;
			}
			case ID_GO_P5_BUTTON_PREVEOUS:
			{
				g_ToolTip.Set(L"Seek to previous macro");
				break;
			}
			case ID_GO_P5_BUTTON_NEXT:
			{
				g_ToolTip.Set(L"Seek to next macro");
				break;
			}
			case ID_GO_P5_KEY_BOX:
			{
				g_ToolTip.Set(L"Key for this macro");
				break;
			}
			case ID_GO_P5_BUTTON_SHIFT:
			{
				g_ToolTip.Set(L"Macro running if only shift key pressed");
				break;
			}
			case ID_GO_P5_BUTTON_ALT:
			{
				g_ToolTip.Set(L"Macro running if only alt key pressed");
				break;
			}
			case ID_GO_P5_BUTTON_CTRL:
			{
				g_ToolTip.Set(L"Macro running if only ctrl key pressed");
				break;
			}
			case ID_GO_P5_BUTTON_UP:
			{
				g_ToolTip.Set(L"Up in macro actions list");
				break;
			}
			case ID_GO_P5_BUTTON_DOWN:
			{
				g_ToolTip.Set(L"Down in macro actions list");
				break;
			}
			case ID_GO_P6_ENABLE_PATHFINDING:
			{
				g_ToolTip.Set(L"Enable pathfinding in client");
				break;
			}
			case ID_GO_P6_HOLD_TAB_FOR_COMBAT:
			{
				g_ToolTip.Set(L"Hold the tab for war mode");
				break;
			}
			case ID_GO_P6_OFFSET_INTERFACE_WINDOWS:
			{
				g_ToolTip.Set(L"Any opened gumps position calculated from previous gump or opened in right top angle of screen");
				break;
			}
			case ID_GO_P6_AUTO_ARRANGE_MINIMIZED_WINDOWS:
			{
				g_ToolTip.Set(L"Auto arrange the gumps");
				break;
			}
			case ID_GO_P6_ALWAYS_RUN:
			{
				g_ToolTip.Set(L"Your character is always running");
				break;
			}
			case ID_GO_P6_DISABLE_MENUBAR:
			{
				g_ToolTip.Set(L"Hide/show the menubar gump");
				break;
			}
			case ID_GO_P6_GRAY_OUT_OF_RANGE_OBJECTS:
			{
				g_ToolTip.Set(L"Objects out of client's viewrane is grayed");
				break;
			}
			case ID_GO_P6_DISABLE_NEW_TARGET_SYSTEM:
			{
				g_ToolTip.Set(L"Disable/enable the new target system");
				break;
			}
			case ID_GO_P6_OBJECT_HANDLES:
			{
				g_ToolTip.Set(L"Use objects handles in game window (pressing Ctrl + Shift)");
				break;
			}
			case ID_GO_P6_DISPLAY_ITEM_PROPERTIES_ICON:
			{
				g_ToolTip.Set(L"???");
				break;
			}
			case ID_GO_P6_HOLD_SHIFT_FOR_CONTEXT_MENUS:
			{
				g_ToolTip.Set(L"Shift + mouse click on characters for call context menu");
				break;
			}
			case ID_GO_P6_HOLD_SHIFT_FOR_ENABLE_PATHFINDING:
			{
				g_ToolTip.Set(L"Shift + rights mouse click in game window for call pathfinding");
				break;
			}
			case ID_GO_P7_SCALE_SPEECH_DURATION:
			{
				g_ToolTip.Set(L"Use timer scaling based on text lines count");
				break;
			}
			case ID_GO_P7_SPEECH_COLOR:
			{
				g_ToolTip.Set(L"Speech text color");
				break;
			}
			case ID_GO_P7_EMOTE_COLOR:
			{
				g_ToolTip.Set(L"Emote text color");
				break;
			}
			case ID_GO_P7_PARTY_MESSAGE_COLOR:
			{
				g_ToolTip.Set(L"Party messages text color");
				break;
			}
			case ID_GO_P7_GUILD_MESSAGE_COLOR:
			{
				g_ToolTip.Set(L"Guild messages text color");
				break;
			}
			case ID_GO_P7_ALLIANCE_MESSAGE_COLOR:
			{
				g_ToolTip.Set(L"Alliance messages text color");
				break;
			}
			case ID_GO_P7_IGNORE_GUILD_MESSAGE:
			{
				g_ToolTip.Set(L"Ignore the guild messages");
				break;
			}
			case ID_GO_P7_IGNORE_ALLIANCE_MESSAGE:
			{
				g_ToolTip.Set(L"Ignore the alliance messages");
				break;
			}
			case ID_GO_P7_DARK_NIGHTS:
			{
				g_ToolTip.Set(L"Nights is very dark");
				break;
			}
			case ID_GO_P7_COLORED_LIGHTING:
			{
				g_ToolTip.Set(L"Light sources has exude colored lights");
				break;
			}
			case ID_GO_P7_AJUST_LONG_SPEECH:
			{
				g_ToolTip.Set(L"Scale for text timer");
				break;
			}
			case ID_GO_P7_GAME_WINDOW_WIDTH:
			{
				g_ToolTip.Set(L"Width for game window");
				break;
			}
			case ID_GO_P7_GAME_WINDOW_HEIGHT:
			{
				g_ToolTip.Set(L"Height for game window");
				break;
			}
			case ID_GO_P7_LOCK_GAME_WINDOW_RESIZING:
			{
				g_ToolTip.Set(L"Disable/enable real-time window moving and resizing");
				break;
			}
			case ID_GO_P8_INNOCENT_COLOR:
			{
				g_ToolTip.Set(L"Innocent characters color");
				break;
			}
			case ID_GO_P8_FRIENDLY_COLOR:
			{
				g_ToolTip.Set(L"Friendly characters color");
				break;
			}
			case ID_GO_P8_SOMEONE_COLOR:
			{
				g_ToolTip.Set(L"Someone characters color");
				break;
			}
			case ID_GO_P8_CRIMINAL_COLOR:
			{
				g_ToolTip.Set(L"Criminal characters color");
				break;
			}
			case ID_GO_P8_ENEMY_COLOR:
			{
				g_ToolTip.Set(L"Enemy characters color");
				break;
			}
			case ID_GO_P8_MURDERER_COLOR:
			{
				g_ToolTip.Set(L"Murderer characters color");
				break;
			}
			case ID_GO_P8_QUERY_CRIMINAL_ACTIONS:
			{
				g_ToolTip.Set(L"Query a criminal actions");
				break;
			}
			case ID_GO_P9_SHOW_APPROACHING_NAMES:
			{
				g_ToolTip.Set(L"Show names of approaching players and new corpses");
				break;
			}
			case ID_GO_P9_USE_CIRCLE_OF_TRANSPARENCY:
			{
				g_ToolTip.Set(L"Use circle of transparency");
				break;
			}
			case ID_GO_P9_INFORM_STATS:
			{
				g_ToolTip.Set(L"Stats increase/decrease information");
				break;
			}
			case ID_GO_P9_SPEECH_FONT:
			{
				g_ToolTip.Set(L"Speech font");
				break;
			}
			case ID_GO_P9_TRANSPARENCY_RADIUS:
			{
				g_ToolTip.Set(L"Value for circle of transparency");
				break;
			}
			case ID_GO_P9_INFORM_SKILLS:
			{
				g_ToolTip.Set(L"Skills increase/decrease information");
				break;
			}
			default:
				break;
		}
}
//----------------------------------------------------------------------------
void CGumpOptions::DrawPage1()
{
	WISPFUN_DEBUG("c104_f6");
	//Sound and Music
	Add(new CGUIPage(1));

	Add(new CGUIGumppic(0x00D9, 0, 45));

	CGUIText *text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 84, 22));
	text->CreateTextureW(0, L"Sound and Music", 30, 460, TS_CENTER);

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 64, 44));
	text->CreateTextureW(0, L"These settings affect the sound and music you will hear while playing Ultima Online.", 30, 500);

	CGUICheckbox *checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P1_SOUND_ON_OFF, 0x00D2, 0x00D3, 0x00D2, 64, 90));
	checkbox->Checked = g_OptionsConfig.Sound;
	checkbox->SetTextParameters(0, L"Sound on/off", g_OptionsTextColor);

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 64, 112));
	text->CreateTextureW(0, L"Sound volume");

	m_SliderSound = (CGUISlider*)Add(new CGUISlider(ID_GO_P1_SOUND_VOLUME, 0x00D8, 0x00D8, 0x00D8, 0x00D5, true, false, 64, 133, 90, 0, 255, g_OptionsConfig.SoundVolume));
	m_SliderSound->SetTextParameters(true, STP_RIGHT, 0, g_OptionsTextColor, true);



	checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P1_MUSIC_ON_OFF, 0x00D2, 0x00D3, 0x00D2, 64, 151));
	checkbox->Checked = g_OptionsConfig.Music;
	checkbox->SetTextParameters(0, L"Music on/off", g_OptionsTextColor);

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 64, 173));
	text->CreateTextureW(0, L"Music volume");

	m_SliderMusic = (CGUISlider*)Add(new CGUISlider(ID_GO_P1_MUSIC_VOLUME, 0x00D8, 0x00D8, 0x00D8, 0x00D5, true, false, 64, 194, 90, 0, 255, g_OptionsConfig.MusicVolume));
	m_SliderMusic->SetTextParameters(true, STP_RIGHT, 0, g_OptionsTextColor, true);



	checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P1_PLAY_FOOTSTEP_SOUNDS, 0x00D2, 0x00D3, 0x00D2, 64, 212));
	checkbox->Checked = g_OptionsConfig.FootstepsSound;
	checkbox->SetTextParameters(0, L"Play footstep sounds", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P1_PLAY_COMBAT_MUSIC, 0x00D2, 0x00D3, 0x00D2, 64, 232));
	checkbox->Checked = g_OptionsConfig.CombatMusic;
	checkbox->SetTextParameters(0, L"Play combat music", g_OptionsTextColor);
}
//----------------------------------------------------------------------------
void CGumpOptions::DrawPage2()
{
	WISPFUN_DEBUG("c104_f7");
	//Orion's configuration
	Add(new CGUIPage(2));

	Add(new CGUIGumppic(0x00DB, 0, 111));

	CGUIText *text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 84, 22));
	text->CreateTextureW(0, L"Orion's configuration", 30, 460, TS_CENTER);

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 64, 44));
	text->CreateTextureW(0, L"These settings configure the Orion UO Client.");



	CGUIRadio *devRadio = (CGUIRadio*)Add(new CGUIRadio(ID_GO_P2_DEV_MODE_1, 0x00D0, 0x00D1, 0x00D2, 64, 64));
	devRadio->Checked = (g_DeveloperMode == DM_NO_DEBUG);
	devRadio->SetTextParameters(0, L"No debug", g_OptionsTextColor);

	devRadio = (CGUIRadio*)Add(new CGUIRadio(ID_GO_P2_DEV_MODE_2, 0x00D0, 0x00D1, 0x00D2, 164, 64));
	devRadio->Checked = (g_DeveloperMode == DM_SHOW_FPS_ONLY);
	devRadio->SetTextParameters(0, L"FPS only", g_OptionsTextColor);

	devRadio = (CGUIRadio*)Add(new CGUIRadio(ID_GO_P2_DEV_MODE_3, 0x00D0, 0x00D1, 0x00D2, 264, 64));
	devRadio->Checked = (g_DeveloperMode == DM_DEBUGGING);
	devRadio->SetTextParameters(0, L"Debugging", g_OptionsTextColor);



	CGUIHTMLGump *html = (CGUIHTMLGump*)Add(new CGUIHTMLGump(0xFFFFFFFF, 0x0BB8, 64, 90, 500, 300, false, true));

	text = (CGUIText*)html->Add(new CGUIText(g_OptionsTextColor, 0, 0));
	text->CreateTextureW(0, L"FPS rate:");

	m_SliderClientFPS = (CGUISlider*)html->Add(new CGUISlider(ID_GO_P2_CLIENT_FPS, 0x00D8, 0x00D8, 0x00D8, 0x00D5, true, false, 0, 21, 90, 16, 64, g_OptionsConfig.ClientFPS));
	m_SliderClientFPS->SetTextParameters(true, STP_RIGHT, 0, g_OptionsTextColor, true);


	CGUICheckbox *checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P2_REDUCE_FPS_UNACTIVE_WINDOW, 0x00D2, 0x00D3, 0x00D2, 140, 16));
	checkbox->Checked = g_OptionsConfig.ReduceFPSUnactiveWindow;
	checkbox->SetTextParameters(0, L"Reduce FPS when UO window is unactive", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P2_CHARACTERS_ANIMATION_DELAY, 0x00D2, 0x00D3, 0x00D2, 0, 40));
	checkbox->Checked = g_OptionsConfig.StandartCharactersAnimationDelay;
	checkbox->SetTextParameters(0, L"Standard characters animation delay", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P2_ITEMS_ANIMATION_DELAY, 0x00D2, 0x00D3, 0x00D2, 0, 60));
	checkbox->Checked = g_OptionsConfig.StandartItemsAnimationDelay;
	checkbox->SetTextParameters(0, L"Standard items animation delay", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P2_ENABLE_SCALING, 0x00D2, 0x00D3, 0x00D2, 0, 80));
	checkbox->Checked = g_OptionsConfig.UseScaling;
	checkbox->SetTextParameters(0, L"Use scaling in game window (BETA VERSION!!!)", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P2_REMOVE_TEXT_WITH_BLENDING, 0x00D2, 0x00D3, 0x00D2, 0, 100));
	checkbox->Checked = g_OptionsConfig.RemoveTextWithBlending;
	checkbox->SetTextParameters(0, L"Remove object's text with alpha-blending", g_OptionsTextColor);

	text = (CGUIText*)html->Add(new CGUIText(g_OptionsTextColor, 0, 120));
	text->CreateTextureW(0, L"Draw character's status in game window");

	html->Add(new CGUIGroup(1));
	CGUIRadio *radio = (CGUIRadio*)html->Add(new CGUIRadio(ID_GO_P2_NO_DRAW_CHARACTERS_STATUS, 0x00D0, 0x00D1, 0x00D2, 10, 140));
	radio->Checked = (g_OptionsConfig.DrawStatusState == DCSS_NO_DRAW);
	radio->SetTextParameters(0, L"No draw", g_OptionsTextColor);

	radio = (CGUIRadio*)html->Add(new CGUIRadio(ID_GO_P2_DRAW_CHARACTERS_STATUS_TOP, 0x00D0, 0x00D1, 0x00D2, 10, 160));
	radio->Checked = (g_OptionsConfig.DrawStatusState == DCSS_ABOVE);
	radio->SetTextParameters(0, L"Above character (Text)", g_OptionsTextColor);

	radio = (CGUIRadio*)html->Add(new CGUIRadio(ID_GO_P2_DRAW_CHARACTERS_STATUS_BOTTOM, 0x00D0, 0x00D1, 0x00D2, 10, 180));
	radio->Checked = (g_OptionsConfig.DrawStatusState == DCSS_UNDER);
	radio->SetTextParameters(0, L"Under character (Line)", g_OptionsTextColor);

	html->Add(new CGUIGroup(2));
	radio = (CGUIRadio*)html->Add(new CGUIRadio(ID_GO_P2_DRAW_CHARACTER_BARS_ALWAYS, 0x00D0, 0x00D1, 0x00D2, 230, 140));
	radio->Checked = (g_OptionsConfig.DrawStatusConditionState == DCSCS_ALWAYS);
	radio->SetTextParameters(0, L"Always", g_OptionsTextColor);

	radio = (CGUIRadio*)html->Add(new CGUIRadio(ID_GO_P2_DRAW_CHARACTER_BARS_NOT_MAX, 0x00D0, 0x00D1, 0x00D2, 230, 160));
	radio->Checked = (g_OptionsConfig.DrawStatusConditionState == DCSCS_NOT_MAX);
	radio->SetTextParameters(0, L"HP <> MaxHP", g_OptionsTextColor);

	radio = (CGUIRadio*)html->Add(new CGUIRadio(ID_GO_P2_DRAW_CHARACTER_BARS_LOWER, 0x00D0, 0x00D1, 0x00D2, 230, 180));
	radio->Checked = (g_OptionsConfig.DrawStatusConditionState == DCSCS_LOWER);
	radio->SetTextParameters(0, L"HP lower %", g_OptionsTextColor);

	m_SliderDrawStatusConditionValue = (CGUISlider*)html->Add(new CGUISlider(ID_GO_P2_DRAW_CHARACTER_BARS_LOWER_VALUE, 0x00D8, 0x00D8, 0x00D8, 0x00D5, true, false, 342, 182, 90, 10, 100, g_OptionsConfig.DrawStatusConditionValue));
	m_SliderDrawStatusConditionValue->SetTextParameters(true, STP_RIGHT_CENTER, 0, g_OptionsTextColor, true);
	
	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P2_DRAW_STUMPS, 0x00D2, 0x00D3, 0x00D2, 0, 205));
	checkbox->Checked = g_OptionsConfig.DrawStumps;
	checkbox->SetTextParameters(0, L"Change trees to stumps", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P2_MARKING_CAVES, 0x00D2, 0x00D3, 0x00D2, 0, 225));
	checkbox->Checked = g_OptionsConfig.MarkingCaves;
	checkbox->SetTextParameters(0, L"Marking cave tiles", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P2_NO_VEGETATION, 0x00D2, 0x00D3, 0x00D2, 0, 245));
	checkbox->Checked = g_OptionsConfig.NoVegetation;
	checkbox->SetTextParameters(0, L"Hide vegetation", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P2_NO_ANIMATE_FIELDS, 0x00D2, 0x00D3, 0x00D2, 0, 265));
	checkbox->Checked = g_OptionsConfig.NoAnimateFields;
	checkbox->SetTextParameters(0, L"No fields animation", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P2_LOCK_GUMP_MOVING, 0x00D2, 0x00D3, 0x00D2, 0, 285));
	checkbox->Checked = g_OptionsConfig.LockGumpsMoving;
	checkbox->SetTextParameters(0, L"Lock gumps moving", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P2_CONSOLE_ENTER, 0x00D2, 0x00D3, 0x00D2, 0, 305));
	checkbox->Checked = g_OptionsConfig.ConsoleNeedEnter;
	checkbox->SetTextParameters(0, L"Chat need press 'Enter' to activate it.", g_OptionsTextColor);

	text = (CGUIText*)html->Add(new CGUIText(g_OptionsTextColor, 0, 325));
	text->CreateTextureW(0, L"Hidden characters display mode:");

	html->Add(new CGUIGroup(3));
	radio = (CGUIRadio*)html->Add(new CGUIRadio(ID_GO_P2_HIDDEN_CHARACTES_MODE_1, 0x00D0, 0x00D1, 0x00D2, 10, 345));
	radio->Checked = (g_OptionsConfig.HiddenCharactersRenderMode == HCRM_ORIGINAL);
	radio->SetTextParameters(0, L"Original", g_OptionsTextColor);

	radio = (CGUIRadio*)html->Add(new CGUIRadio(ID_GO_P2_HIDDEN_CHARACTES_MODE_2, 0x00D0, 0x00D1, 0x00D2, 10, 365));
	radio->Checked = (g_OptionsConfig.HiddenCharactersRenderMode == HCRM_ALPHA_BLENDING);
	radio->SetTextParameters(0, L"With alpha-blending, alpha:", g_OptionsTextColor);

	radio = (CGUIRadio*)html->Add(new CGUIRadio(ID_GO_P2_HIDDEN_CHARACTES_MODE_3, 0x00D0, 0x00D1, 0x00D2, 10, 385));
	radio->Checked = (g_OptionsConfig.HiddenCharactersRenderMode == HCRM_SPECTRAL_COLOR);
	radio->SetTextParameters(0, L"With spectral color", g_OptionsTextColor);

	radio = (CGUIRadio*)html->Add(new CGUIRadio(ID_GO_P2_HIDDEN_CHARACTES_MODE_4, 0x00D0, 0x00D1, 0x00D2, 10, 405));
	radio->Checked = (g_OptionsConfig.HiddenCharactersRenderMode == HCRM_SPECIAL_SPECTRAL_COLOR);
	radio->SetTextParameters(0, L"With special spectral color", g_OptionsTextColor);

	m_SliderHiddenAlpha = (CGUISlider*)html->Add(new CGUISlider(ID_GO_P2_HIDDEN_ALPHA, 0x00D8, 0x00D8, 0x00D8, 0x00D5, true, false, 225, 369, 90, 20, 255, g_OptionsConfig.HiddenAlpha));
	m_SliderHiddenAlpha->SetTextParameters(true, STP_RIGHT, 0, g_OptionsTextColor, true);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P2_USE_HIDDEN_MODE_ONLY_FOR_SELF, 0x00D2, 0x00D3, 0x00D2, 0, 430));
	checkbox->Checked = g_OptionsConfig.UseHiddenModeOnlyForSelf;
	checkbox->SetTextParameters(0, L"Change hidden characters mode only for your person", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P2_TRANSPARENT_SPELL_ICONS, 0x00D2, 0x00D3, 0x00D2, 0, 450));
	checkbox->Checked = g_OptionsConfig.TransparentSpellIcons;
	checkbox->SetTextParameters(0, L"Transparent spell icons, alpha:", g_OptionsTextColor);

	m_SliderSpellIconsAlpha = (CGUISlider*)html->Add(new CGUISlider(ID_GO_P2_SPELL_ICONS_ALPHA, 0x00D8, 0x00D8, 0x00D8, 0x00D5, true, false, 232, 454, 90, 30, 255, g_OptionsConfig.SpellIconAlpha));
	m_SliderSpellIconsAlpha->SetTextParameters(true, STP_RIGHT, 0, g_OptionsTextColor, true);
	
	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P2_OLD_STYLE_STATUSBAR, 0x00D2, 0x00D3, 0x00D2, 0, 470));
	checkbox->Checked = g_OptionsConfig.OldStyleStatusbar;
	checkbox->SetTextParameters(0, L"Old style maximized statusbar", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P2_ORIGINAL_PARTY_STATUSBAR, 0x00D2, 0x00D3, 0x00D2, 0, 490));
	checkbox->Checked = g_OptionsConfig.OriginalPartyStatusbar;
	checkbox->SetTextParameters(0, L"Original party statusbar gump", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P2_APPLY_STATE_COLOR_ON_CHARACTERS, 0x00D2, 0x00D3, 0x00D2, 0, 510));
	checkbox->Checked = g_OptionsConfig.ApplyStateColorOnCharacters;
	checkbox->SetTextParameters(0, L"Colorize characters by state", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P2_CHANGE_FIELDS_GRAPHIC, 0x00D2, 0x00D3, 0x00D2, 0, 530));
	checkbox->Checked = g_OptionsConfig.ChangeFieldsGraphic;
	checkbox->SetTextParameters(0, L"Change animated fields to tiles", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P2_PAPERDOLL_SLOTS, 0x00D2, 0x00D3, 0x00D2, 0, 550));
	checkbox->Checked = g_OptionsConfig.PaperdollSlots;
	checkbox->SetTextParameters(0, L"Add paperdoll slots", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P2_SCALE_IMAGES_IN_PAPERDOLL_SLOTS, 0x00D2, 0x00D3, 0x00D2, 200, 550));
	checkbox->Checked = g_OptionsConfig.ScaleImagesInPaperdollSlots;
	checkbox->SetTextParameters(0, L"Scale images in slots", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P2_REMOVE_STATUSBARS_WITHOUT_OBJECTS, 0x00D2, 0x00D3, 0x00D2, 0, 570));
	checkbox->Checked = g_OptionsConfig.RemoveStatusbarsWithoutObjects;
	checkbox->SetTextParameters(0, L"Remove statusbars without objects", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P2_SHOW_CONSOLE_ENTRY_MODE, 0x00D2, 0x00D3, 0x00D2, 0, 590));
	checkbox->Checked = g_OptionsConfig.ShowDefaultConsoleEntryMode;
	checkbox->SetTextParameters(0, L"Show console entry mode under game window", g_OptionsTextColor);

	text = (CGUIText*)html->Add(new CGUIText(g_OptionsTextColor, 0, 610));
	text->CreateTextureW(0, L"Draw aura under characters mode:");

	html->Add(new CGUIGroup(4));

	radio = (CGUIRadio*)html->Add(new CGUIRadio(ID_GO_P2_DRAW_AURA_NEVER, 0x00D0, 0x00D1, 0x00D2, 10, 630));
	radio->Checked = (g_OptionsConfig.DrawAuraState == DAS_NEVER);
	radio->SetTextParameters(0, L"Never", g_OptionsTextColor);

	radio = (CGUIRadio*)html->Add(new CGUIRadio(ID_GO_P2_DRAW_AURA_IN_WARMODE, 0x00D0, 0x00D1, 0x00D2, 10, 650));
	radio->Checked = (g_OptionsConfig.DrawAuraState == DAS_IN_WARMODE);
	radio->SetTextParameters(0, L"Only in war mode", g_OptionsTextColor);

	radio = (CGUIRadio*)html->Add(new CGUIRadio(ID_GO_P2_DRAW_AURA_ALWAYS, 0x00D0, 0x00D1, 0x00D2, 10, 670));
	radio->Checked = (g_OptionsConfig.DrawAuraState == DAS_ALWAYS);
	radio->SetTextParameters(0, L"Always", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P2_DRAW_AURA_WITH_CTRL_PRESSED, 0x00D2, 0x00D3, 0x00D2, 0, 690));
	checkbox->Checked = g_OptionsConfig.DrawAuraWithCtrlPressed;
	checkbox->SetTextParameters(0, L"Draw aura only if Ctrl pressed", g_OptionsTextColor);

	text = (CGUIText*)html->Add(new CGUIText(g_OptionsTextColor, 0, 710));
	text->CreateTextureW(0, L"Screenshots format:");

	html->Add(new CGUIGroup(5));

	radio = (CGUIRadio*)html->Add(new CGUIRadio(ID_GO_P2_SCREENSHOT_FORMAT_BMP, 0x00D0, 0x00D1, 0x00D2, 10, 730));
	radio->Checked = (g_OptionsConfig.ScreenshotFormat == SF_BMP);
	radio->SetTextParameters(0, L"BMP", g_OptionsTextColor);

	radio = (CGUIRadio*)html->Add(new CGUIRadio(ID_GO_P2_SCREENSHOT_FORMAT_PNG, 0x00D0, 0x00D1, 0x00D2, 100, 730));
	radio->Checked = (g_OptionsConfig.ScreenshotFormat == SF_PNG);
	radio->SetTextParameters(0, L"PNG", g_OptionsTextColor);

	radio = (CGUIRadio*)html->Add(new CGUIRadio(ID_GO_P2_SCREENSHOT_FORMAT_TIFF, 0x00D0, 0x00D1, 0x00D2, 190, 730));
	radio->Checked = (g_OptionsConfig.ScreenshotFormat == SF_TIFF);
	radio->SetTextParameters(0, L"TIFF", g_OptionsTextColor);

	radio = (CGUIRadio*)html->Add(new CGUIRadio(ID_GO_P2_SCREENSHOT_FORMAT_JPEG, 0x00D0, 0x00D1, 0x00D2, 280, 730));
	radio->Checked = (g_OptionsConfig.ScreenshotFormat == SF_JPEG);
	radio->SetTextParameters(0, L"JPEG (maybe crashes)", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P2_REMOVE_OR_CREATE_OBJECTS_WITH_BLENDING, 0x00D2, 0x00D3, 0x00D2, 0, 760));
	checkbox->Checked = g_OptionsConfig.RemoveOrCreateObjectsWithBlending;
	checkbox->SetTextParameters(0, L"Remove or new draw objects displaying with use blending", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P2_DRAW_HELMETS_ON_SHROUD, 0x00D2, 0x00D3, 0x00D2, 0, 780));
	checkbox->Checked = g_OptionsConfig.DrawHelmetsOnShroud;
	checkbox->SetTextParameters(0, L"Draw helmets on shroud in the world", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P2_USE_GLOBAL_MAP_LAYER, 0x00D2, 0x00D3, 0x00D2, 0, 800));
	checkbox->Checked = g_OptionsConfig.UseGlobalMapLayer;
	checkbox->SetTextParameters(0, L"Draw world map before all gumps", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P2_NO_DRAW_ROOFS, 0x00D2, 0x00D3, 0x00D2, 0, 820));
	checkbox->Checked = g_OptionsConfig.NoDrawRoofs;
	checkbox->SetTextParameters(0, L"No draw roofs", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P2_HIGHLIGHT_TARGET_BY_TYPE, 0x00D2, 0x00D3, 0x00D2, 0, 840));
	checkbox->Checked = g_OptionsConfig.HighlightTargetByType;
	checkbox->SetTextParameters(0, L"Highlight target by type (netural, harmful, helpful)", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P2_AUTO_DISPLAY_WORLD_MAP, 0x00D2, 0x00D3, 0x00D2, 0, 860));
	checkbox->Checked = g_OptionsConfig.AutoDisplayWorldMap;
	checkbox->SetTextParameters(0, L"Display a world map immediately after entering the world", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P2_USE_GL_LISTS_FOR_INTERFACE, 0x00D2, 0x00D3, 0x00D2, 0, 880));
	checkbox->Checked = g_OptionsConfig.UseGLListsForInterface;
	checkbox->SetTextParameters(0, L"Use GL lists for draw interface gumps", g_OptionsTextColor);

	html->CalculateDataSize();
}
//----------------------------------------------------------------------------
void CGumpOptions::DrawPage3()
{
	WISPFUN_DEBUG("c104_f8");
	//Language
	Add(new CGUIPage(3));

	Add(new CGUIGumppic(0x00DD, 0, 177));

	CGUIText *text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 84, 22));
	text->CreateTextureW(0, L"Language", 30, 460, TS_CENTER);

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 64, 44));
	text->CreateTextureW(0, L"The language you use when playing UO is obtained from your Operating System settings.", 30, 480);

	CGUICheckbox *checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P3_USE_TOOLTIP, 0x00D2, 0x00D3, 0x00D2, 64, 90));
	checkbox->Checked = g_OptionsConfig.UseToolTips;
	checkbox->SetTextParameters(0, L"Use Tool-tips", g_OptionsTextColor);

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 64, 112));
	text->CreateTextureW(0, L"Delay befor Tool-tip appears");

	m_SliderTooltipDelay = (CGUISlider*)Add(new CGUISlider(ID_GO_P3_DELAY_BEFORE_TOOLTIP, 0x00D8, 0x00D8, 0x00D8, 0x00D5, true, false, 64, 133, 90, 0, 5000, g_OptionsConfig.ToolTipsDelay));
	m_SliderTooltipDelay->SetTextParameters(true, STP_RIGHT, 0, g_OptionsTextColor, true);

	Add(new CGUIButton(ID_GO_P3_TEXT_COLOR, 0x00D4, 0x00D4, 0x00D4, 64, 151));

	uint color = 0xFF7F7F7F;

	if (g_OptionsConfig.ToolTipsTextColor != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.ToolTipsTextColor);

	m_ColorTooltipText = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P3_TEXT_COLOR, g_OptionsConfig.ToolTipsTextColor, 67, 154, 13, 14, color));
	m_ColorTooltipText->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 88, 151));
	text->CreateTextureW(0, L"Color of Tool-tip text");

	Add(new CGUIButton(ID_GO_P3_TEXT_FONT, 0x00D0, 0x00D0, 0x00D0, 64, 173));

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 88, 173));
	text->CreateTextureW(0, L"Font for Tool-tips");
}
//----------------------------------------------------------------------------
void CGumpOptions::DrawPage4()
{
	WISPFUN_DEBUG("c104_f9");
	//Chat
	Add(new CGUIPage(4));

	Add(new CGUIGumppic(0x00DF, 0, 243));

	CGUIText *text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 84, 22));
	text->CreateTextureW(0, L"Chat", 30, 460, TS_CENTER);

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 64, 44));
	text->CreateTextureW(0, L"These settings affect the interface display for the chat system.");


	Add(new CGUIButton(ID_GO_P4_TEXT_COLOR, 0x00D4, 0x00D4, 0x00D4, 64, 90));

	uint color = 0xFF7F7F7F;

	if (g_OptionsConfig.ChatColorInputText != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.ChatColorInputText);

	m_ColorInputText = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P4_TEXT_COLOR, g_OptionsConfig.ChatColorInputText, 67, 93, 13, 14, color));
	m_ColorInputText->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 88, 90));
	text->CreateTextureW(0, L"Input text color");

	Add(new CGUIButton(ID_GO_P4_TEXT_COLOR + 1, 0x00D4, 0x00D4, 0x00D4, 64, 109));

	if (g_OptionsConfig.ChatColorInputText != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.ChatColorInputText);
	else
		color = 0xFF7F7F7F;

	m_ColorMenuOption = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P4_TEXT_COLOR + 1, g_OptionsConfig.ChatColorInputText, 67, 112, 13, 14, color));
	m_ColorMenuOption->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 88, 109));
	text->CreateTextureW(0, L"Menu option color");

	Add(new CGUIButton(ID_GO_P4_TEXT_COLOR + 2, 0x00D4, 0x00D4, 0x00D4, 64, 128));

	if (g_OptionsConfig.ChatColorInputText != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.ChatColorInputText);
	else
		color = 0xFF7F7F7F;

	m_ColorPlayerColorInMemberList = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P4_TEXT_COLOR + 2, g_OptionsConfig.ChatColorInputText, 67, 131, 13, 14, color));
	m_ColorPlayerColorInMemberList->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 88, 128));
	text->CreateTextureW(0, L"Player color in member list");

	Add(new CGUIButton(ID_GO_P4_TEXT_COLOR + 3, 0x00D4, 0x00D4, 0x00D4, 64, 147));

	if (g_OptionsConfig.ChatColorInputText != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.ChatColorInputText);
	else
		color = 0xFF7F7F7F;

	m_ColorChatText = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P4_TEXT_COLOR + 3, g_OptionsConfig.ChatColorInputText, 67, 150, 13, 14, color));
	m_ColorChatText->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 88, 147));
	text->CreateTextureW(0, L"Chat text color");

	Add(new CGUIButton(ID_GO_P4_TEXT_COLOR + 4, 0x00D4, 0x00D4, 0x00D4, 64, 166));

	if (g_OptionsConfig.ChatColorInputText != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.ChatColorInputText);
	else
		color = 0xFF7F7F7F;

	m_ColorPlayerNameWithoutSpeakingPrivileges = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P4_TEXT_COLOR + 4, g_OptionsConfig.ChatColorInputText, 67, 169, 13, 14, color));
	m_ColorPlayerNameWithoutSpeakingPrivileges->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 88, 166));
	text->CreateTextureW(0, L"Player name without speaking privileges", 30, 160);

	Add(new CGUIButton(ID_GO_P4_TEXT_COLOR + 5, 0x00D4, 0x00D4, 0x00D4, 64, 201));

	if (g_OptionsConfig.ChatColorInputText != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.ChatColorInputText);
	else
		color = 0xFF7F7F7F;

	m_ColorMutedText = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P4_TEXT_COLOR + 5, g_OptionsConfig.ChatColorInputText, 67, 204, 13, 14, color));
	m_ColorMutedText->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 88, 201));
	text->CreateTextureW(0, L"Muted text color");

	Add(new CGUIButton(ID_GO_P4_TEXT_COLOR + 6, 0x00D4, 0x00D4, 0x00D4, 64, 220));

	if (g_OptionsConfig.ChatColorInputText != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.ChatColorInputText);
	else
		color = 0xFF7F7F7F;

	m_ColorChannelModeratorName = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P4_TEXT_COLOR + 6, g_OptionsConfig.ChatColorInputText, 67, 223, 13, 14, color));
	m_ColorChannelModeratorName->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 88, 220));
	text->CreateTextureW(0, L"Channel moderator name");

	Add(new CGUIButton(ID_GO_P4_TEXT_COLOR + 7, 0x00D4, 0x00D4, 0x00D4, 64, 239));

	if (g_OptionsConfig.ChatColorInputText != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.ChatColorInputText);
	else
		color = 0xFF7F7F7F;

	m_ColorChannelModeratorText = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P4_TEXT_COLOR + 7, g_OptionsConfig.ChatColorInputText, 67, 242, 13, 14, color));
	m_ColorChannelModeratorText->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 88, 239));
	text->CreateTextureW(0, L"Channel moderator text");

	Add(new CGUIButton(ID_GO_P4_TEXT_COLOR + 8, 0x00D4, 0x00D4, 0x00D4, 64, 258));

	if (g_OptionsConfig.ChatColorInputText != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.ChatColorInputText);
	else
		color = 0xFF7F7F7F;

	m_ColorMyName = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P4_TEXT_COLOR + 8, g_OptionsConfig.ChatColorInputText, 67, 261, 13, 14, color));
	m_ColorMyName->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 88, 258));
	text->CreateTextureW(0, L"My name's color");

	Add(new CGUIButton(ID_GO_P4_TEXT_COLOR + 9, 0x00D4, 0x00D4, 0x00D4, 64, 277));

	if (g_OptionsConfig.ChatColorInputText != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.ChatColorInputText);
	else
		color = 0xFF7F7F7F;

	m_ColorMyText = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P4_TEXT_COLOR + 9, g_OptionsConfig.ChatColorInputText, 67, 280, 13, 14, color));
	m_ColorMyText->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 88, 277));
	text->CreateTextureW(0, L"My text color");

	Add(new CGUIButton(ID_GO_P4_TEXT_COLOR + 10, 0x00D4, 0x00D4, 0x00D4, 64, 296));

	if (g_OptionsConfig.ChatColorInputText != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.ChatColorInputText);
	else
		color = 0xFF7F7F7F;

	m_ColorSystemMessage = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P4_TEXT_COLOR + 10, g_OptionsConfig.ChatColorInputText, 67, 299, 13, 14, color));
	m_ColorSystemMessage->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 88, 296));
	text->CreateTextureW(0, L"System message color");



	Add(new CGUIButton(ID_GO_P4_TEXT_COLOR + 11, 0x00D4, 0x00D4, 0x00D4, 300, 90));

	if (g_OptionsConfig.ChatColorInputText != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.ChatColorInputText);
	else
		color = 0xFF7F7F7F;

	m_ColorTextOutputBackground = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P4_TEXT_COLOR + 11, g_OptionsConfig.ChatColorInputText, 303, 93, 13, 14, color));
	m_ColorTextOutputBackground->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 324, 90));
	text->CreateTextureW(0, L"Text Output Background Color");

	Add(new CGUIButton(ID_GO_P4_TEXT_COLOR + 12, 0x00D4, 0x00D4, 0x00D4, 300, 109));

	if (g_OptionsConfig.ChatColorInputText != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.ChatColorInputText);
	else
		color = 0xFF7F7F7F;

	m_ColorTextInputBackground = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P4_TEXT_COLOR + 12, g_OptionsConfig.ChatColorInputText, 303, 112, 13, 14, color));
	m_ColorTextInputBackground->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 324, 109));
	text->CreateTextureW(0, L"Text Input Background Color");

	Add(new CGUIButton(ID_GO_P4_TEXT_COLOR + 13, 0x00D4, 0x00D4, 0x00D4, 300, 128));

	if (g_OptionsConfig.ChatColorInputText != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.ChatColorInputText);
	else
		color = 0xFF7F7F7F;

	m_ColorUserListBackground = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P4_TEXT_COLOR + 13, g_OptionsConfig.ChatColorInputText, 303, 131, 13, 14, color));
	m_ColorUserListBackground->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 324, 128));
	text->CreateTextureW(0, L"User List Background Color");

	Add(new CGUIButton(ID_GO_P4_TEXT_COLOR + 14, 0x00D4, 0x00D4, 0x00D4, 300, 147));

	if (g_OptionsConfig.ChatColorInputText != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.ChatColorInputText);
	else
		color = 0xFF7F7F7F;

	m_ColorConferenceListBackground = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P4_TEXT_COLOR + 14, g_OptionsConfig.ChatColorInputText, 303, 150, 13, 14, color));
	m_ColorConferenceListBackground->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 324, 147));
	text->CreateTextureW(0, L"Conference List Background Color");

	Add(new CGUIButton(ID_GO_P4_TEXT_COLOR + 15, 0x00D4, 0x00D4, 0x00D4, 300, 166));

	if (g_OptionsConfig.ChatColorInputText != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.ChatColorInputText);
	else
		color = 0xFF7F7F7F;

	m_ColorCommandListBackground = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P4_TEXT_COLOR + 15, g_OptionsConfig.ChatColorInputText, 303, 169, 13, 14, color));
	m_ColorCommandListBackground->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 324, 166));
	text->CreateTextureW(0, L"Command List Background Color");



	Add(new CGUIButton(ID_GO_P4_TEXT_FONT, 0x00D0, 0x00D0, 0x00D0, 300, 192));

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 324, 192));
	text->CreateTextureW(0, L"Chat font");
}
//----------------------------------------------------------------------------
void CGumpOptions::RedrawMacroData()
{
	WISPFUN_DEBUG("c104_f10");
	m_WantRedrawMacroData = false;
	m_MacroDataBox->Clear();

	bool alt = false;
	bool ctrl = false;
	bool shift = false;
	ushort key = 0;

	if (m_MacroPointer != NULL)
	{
		alt = m_MacroPointer->Alt;
		ctrl = m_MacroPointer->Ctrl;
		shift = m_MacroPointer->Shift;
		key = m_MacroPointer->Key;
	}

	m_MacroCheckboxShift->Checked = shift;
	m_MacroCheckboxAlt->Checked = alt;
	m_MacroCheckboxCtrl->Checked = ctrl;

	m_MacroKey->m_Entry.SetText(m_HotkeyText[key & 0xFF]);

	CMacroObject *obj = m_MacroObjectPointer;

	if (obj != NULL)
	{
		if (obj->m_Prev != NULL)
			m_MacroDataBox->Add(new CGUIButton(ID_GO_P5_BUTTON_UP, 0x0983, 0x0984, 0x0984, 456, 173));

		const int maxMacroDraw = 7;
		int macroCount = 0;

		int x = 164;
		int y = 187;

		while (obj != NULL && macroCount < maxMacroDraw)
		{
			CGUIComboBox *combobox = (CGUIComboBox*)m_MacroDataBox->Add(new CGUIComboBox(ID_GO_P5_MACRO_SELECTION + (macroCount * 1000), 0x098D, true, 0x09B5, 168, y, 0, 20, true));
			combobox->SelectedIndex = obj->Code;

			IFOR(i, 0, CMacro::MACRO_ACTION_NAME_COUNT)
				combobox->Add(new CGUIComboboxText(0x0386, 1, CMacro::GetActionName(i)));

			if (obj->HasSubMenu == 1)
			{
				int macroListOffset = 0;
				int macroListCount = 0;
				CMacro::GetBoundByCode(obj->Code, macroListCount, macroListOffset);

				combobox = (CGUIComboBox*)m_MacroDataBox->Add(new CGUIComboBox(ID_GO_P5_ACTION_SELECTION + (macroCount * 1000), 0x098E, true, 0x09B5, 286, y, 0, 20, true));
				combobox->SelectedIndex = obj->SubCode - macroListOffset;

				IFOR(i, 0, macroListCount)
					combobox->Add(new CGUIComboboxText(0x0386, 1, CMacro::GetAction(macroListOffset + i), 150, TS_LEFT, UOFONT_FIXED));
			}
			else if (obj->HasSubMenu == 2)
			{
				m_MacroDataBox->Add(new CGUIGumppic(0x098E, 286, y));

				m_MacroDataBox->Add(new CGUIScissor(true, 0, 0, 292, y + 5, 150, 20));

				m_MacroDataBox->Add(new CGUIHitBox(ID_GO_P5_ACTION_SELECTION + (macroCount * 1000), 292, y + 5, 150, 20));

				CGUITextEntry *entry = (CGUITextEntry*)m_MacroDataBox->Add(new CGUITextEntry(ID_GO_P5_ACTION_SELECTION + (macroCount * 1000), 0x0386, 0x0386, 0x0386, 292, y + 5, 0, false, 1));
				entry->CheckOnSerial = true;
				entry->m_Entry.SetText(((CMacroObjectString*)obj)->String);

				m_MacroDataBox->Add(new CGUIScissor(false));
			}

			macroCount++;
			y += 26;

			obj = (CMacroObject*)obj->m_Next;
		}

		if (macroCount >= maxMacroDraw)
			m_MacroDataBox->Add(new CGUIButton(ID_GO_P5_BUTTON_DOWN, 0x0985, 0x0986, 0x0986, 456, y /*295 /*269*/));
	}
}
//----------------------------------------------------------------------------
void CGumpOptions::DrawPage5()
{
	WISPFUN_DEBUG("c104_f11");
	Add(new CGUIPage(5));

	Add(new CGUIGumppic(0x00EC, 0, 309));

	CGUIText *text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 84, 22));
	text->CreateTextureW(0, L"Macro Options", 30, 460, TS_CENTER);

	Add(new CGUIButton(ID_GO_P5_BUTTON_ADD, 0x099C, 0x099E, 0x099D, 152, 60));
	CGUIButton *button = (CGUIButton*)Add(new CGUIButton(ID_GO_P5_BUTTON_DELETE, 0x099F, 0x09A1, 0x09A0, 205, 60));
	button->ProcessPressedState = true;
	button = (CGUIButton*)Add(new CGUIButton(ID_GO_P5_BUTTON_PREVEOUS, 0x09A2, 0x09A4, 0x09A3, 273, 60));
	button->ProcessPressedState = true;
	button = (CGUIButton*)Add(new CGUIButton(ID_GO_P5_BUTTON_NEXT, 0x09A5, 0x09A7, 0x09A6, 357, 60));
	button->ProcessPressedState = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 134, 82));
	text->CreateTextureW(0, L"Keystroke");

	//KeyBox
	Add(new CGUIGumppic(0x098B, 133, 112));
	Add(new CGUIHitBox(ID_GO_P5_KEY_BOX, 133, 112, 63, 23));
	m_MacroKey = (CGUITextEntry*)Add(new CGUITextEntry(ID_GO_P5_KEY_BOX, 0x0386, 0x0386, 0x0386, 138, 117, 56, false, 1, TS_LEFT, UOFONT_FIXED));
	m_MacroKey->CheckOnSerial = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 200, 111));
	text->CreateTextureW(0, L"Key");

	//Shift checkbox
	m_MacroCheckboxShift = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P5_BUTTON_SHIFT, 0x0867, 0x0869, 0x0867, 248, 79));
	m_MacroCheckboxShift->GraphicSelected = 0x0868;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 280, 82));
	text->CreateTextureW(0, L"Shift");

	//Alt checkbox
	m_MacroCheckboxAlt = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P5_BUTTON_ALT, 0x0867, 0x0869, 0x0867, 248, 107));
	m_MacroCheckboxAlt->GraphicSelected = 0x0868;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 280, 111));
	text->CreateTextureW(0, L"Alt");

	//Ctrl checkbox
	m_MacroCheckboxCtrl = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P5_BUTTON_CTRL, 0x0867, 0x0869, 0x0867, 248, 135));
	m_MacroCheckboxCtrl->GraphicSelected = 0x0868;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 280, 140));
	text->CreateTextureW(0, L"Ctrl");

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 134, 163));
	text->CreateTextureW(0, L"Actions");

	m_MacroDataBox = (CGUIDataBox*)Add(new CGUIDataBox());
}
//----------------------------------------------------------------------------
void CGumpOptions::DrawPage6()
{
	WISPFUN_DEBUG("c104_f12");
	//Interface
	Add(new CGUIPage(6));

	Add(new CGUIGumppic(0x00E1, 576, 45));

	CGUIText *text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 84, 22));
	text->CreateTextureW(0, L"Interface", 30, 460, TS_CENTER);

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 64, 44));
	text->CreateTextureW(0, L"These options affect your interface.");

	CGUIHTMLGump *html = (CGUIHTMLGump*)Add(new CGUIHTMLGump(0xFFFFFFFF, 0x0BB8, 64, 90, 500, 300, false, true));

	CGUICheckbox *checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P6_ENABLE_PATHFINDING, 0x00D2, 0x00D3, 0x00D2, 0, 0));
	checkbox->Checked = g_OptionsConfig.EnablePathfind;
	checkbox->SetTextParameters(0, L"Enable pathfinding with double-right-click", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P6_HOLD_TAB_FOR_COMBAT, 0x00D2, 0x00D3, 0x00D2, 0, 20));
	checkbox->Checked = g_OptionsConfig.HoldTabForCombat;
	checkbox->SetTextParameters(0, L"Hold down TAB key for combat, instead of tapping it to toggle combat mode", g_OptionsTextColor, STP_RIGHT, 450);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P6_OFFSET_INTERFACE_WINDOWS, 0x00D2, 0x00D3, 0x00D2, 0, 56));
	checkbox->Checked = g_OptionsConfig.OffsetInterfaceWindows;
	checkbox->SetTextParameters(0, L"Offset interface windows rather than perfectly stacking them", g_OptionsTextColor);

	text = (CGUIText*)html->Add(new CGUIText(g_OptionsTextColor, 0, 76));
	text->CreateTextureW(0, L"Default interface windows offset x:");

	html->Add(new CGUIResizepic(ID_GO_P6_CONTAINER_OFFSET_X, 0x0BB8, 250, 76, 60, 22));
	m_ContainerOffsetX = (CGUITextEntry*)html->Add(new CGUITextEntry(ID_GO_P6_CONTAINER_OFFSET_X, g_OptionsTextColor, g_OptionsTextColor, g_OptionsTextColor, 254, 78));
	m_ContainerOffsetX->CheckOnSerial = true;
	m_ContainerOffsetX->m_Entry.MaxLength = GetSystemMetrics(SM_CXSCREEN) - 20;
	m_ContainerOffsetX->m_Entry.NumberOnly = true;
	m_ContainerOffsetX->m_Entry.SetText(std::to_wstring(g_ContainerRect.DefaultX));

	text = (CGUIText*)html->Add(new CGUIText(g_OptionsTextColor, 312, 76));
	text->CreateTextureW(0, L"y:");

	html->Add(new CGUIResizepic(ID_GO_P6_CONTAINER_OFFSET_Y, 0x0BB8, 329, 76, 60, 22));
	m_ContainerOffsetY = (CGUITextEntry*)html->Add(new CGUITextEntry(ID_GO_P6_CONTAINER_OFFSET_Y, g_OptionsTextColor, g_OptionsTextColor, g_OptionsTextColor, 333, 77));
	m_ContainerOffsetY->CheckOnSerial = true;
	m_ContainerOffsetY->m_Entry.MaxLength = GetSystemMetrics(SM_CYSCREEN) - 60;
	m_ContainerOffsetY->m_Entry.NumberOnly = true;
	m_ContainerOffsetY->m_Entry.SetText(std::to_wstring(g_ContainerRect.DefaultY));


	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P6_AUTO_ARRANGE_MINIMIZED_WINDOWS, 0x00D2, 0x00D3, 0x00D2, 0, 96));
	checkbox->Checked = g_OptionsConfig.AutoArrange;
	checkbox->SetTextParameters(0, L"Automatically arrange minimized windows", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P6_ALWAYS_RUN, 0x00D2, 0x00D3, 0x00D2, 0, 116));
	checkbox->Checked = g_OptionsConfig.AlwaysRun;
	checkbox->SetTextParameters(0, L"Your character will always run if this is checked", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P6_DISABLE_MENUBAR, 0x00D2, 0x00D3, 0x00D2, 0, 136));
	checkbox->Checked = g_OptionsConfig.DisableMenubar;
	checkbox->SetTextParameters(0, L"Disable the Menu Bar", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P6_GRAY_OUT_OF_RANGE_OBJECTS, 0x00D2, 0x00D3, 0x00D2, 0, 156));
	checkbox->Checked = g_OptionsConfig.GrayOutOfRangeObjects;
	checkbox->SetTextParameters(0, L"Gray out of range objects", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P6_DISABLE_NEW_TARGET_SYSTEM, 0x00D2, 0x00D3, 0x00D2, 0, 176));
	checkbox->Checked = g_OptionsConfig.DisableNewTargetSystem;
	checkbox->SetTextParameters(0, L"Disable New Targeting System", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P6_OBJECT_HANDLES, 0x00D2, 0x00D3, 0x00D2, 0, 196));
	checkbox->Checked = g_OptionsConfig.ObjectHandles;
	checkbox->SetTextParameters(0, L"Object Handles", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P6_DISPLAY_ITEM_PROPERTIES_ICON, 0x00D2, 0x00D3, 0x00D2, 0, 216));
	checkbox->Checked = g_OptionsConfig.ItemPropertiesIcon;
	checkbox->SetTextParameters(0, L"Display Item Properties Icon", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P6_HOLD_SHIFT_FOR_CONTEXT_MENUS, 0x00D2, 0x00D3, 0x00D2, 0, 236));
	checkbox->Checked = g_OptionsConfig.HoldShiftForContextMenus;
	checkbox->SetTextParameters(0, L"Hold Shift For Context Menus", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)html->Add(new CGUICheckbox(ID_GO_P6_HOLD_SHIFT_FOR_ENABLE_PATHFINDING, 0x00D2, 0x00D3, 0x00D2, 0, 256));
	checkbox->Checked = g_OptionsConfig.HoldShiftForEnablePathfind;
	checkbox->SetTextParameters(0, L"Hold Shift For Enable Pathfinding", g_OptionsTextColor);

	html->CalculateDataSize();
}
//----------------------------------------------------------------------------
void CGumpOptions::DrawPage7()
{
	WISPFUN_DEBUG("c104_f13");
	//Display
	Add(new CGUIPage(7));

	Add(new CGUIGumppic(0x00E3, 576, 111));

	CGUIText *text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 84, 22));
	text->CreateTextureW(0, L"Display", 30, 460, TS_CENTER);

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 64, 44));
	text->CreateTextureW(0, L"These options affect your display, and adjusting some of them may improve your graphics performance.", 30, 500);



	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 64, 90));
	text->CreateTextureW(0, L"Game Play Window Size");

	Add(new CGUIResizepic(ID_GO_P7_GAME_WINDOW_WIDTH, 0x0BB8, 64, 112, 60, 22));
	m_GameWindowWidth = (CGUITextEntry*)Add(new CGUITextEntry(ID_GO_P7_GAME_WINDOW_WIDTH, g_OptionsTextColor, g_OptionsTextColor, g_OptionsTextColor, 68, 114));
	m_GameWindowWidth->CheckOnSerial = true;
	m_GameWindowWidth->m_Entry.MaxLength = GetSystemMetrics(SM_CXSCREEN) - 20;
	m_GameWindowWidth->m_Entry.NumberOnly = true;
	m_GameWindowWidth->m_Entry.SetText(std::to_wstring(g_OptionsConfig.GameWindowWidth));

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 126, 112));
	text->CreateTextureW(0, L"x");

	Add(new CGUIResizepic(ID_GO_P7_GAME_WINDOW_HEIGHT, 0x0BB8, 139, 112, 60, 22));
	m_GameWindowHeight = (CGUITextEntry*)Add(new CGUITextEntry(ID_GO_P7_GAME_WINDOW_HEIGHT, g_OptionsTextColor, g_OptionsTextColor, g_OptionsTextColor, 143, 114));
	m_GameWindowHeight->CheckOnSerial = true;
	m_GameWindowHeight->m_Entry.MaxLength = GetSystemMetrics(SM_CYSCREEN) - 60;
	m_GameWindowHeight->m_Entry.NumberOnly = true;
	m_GameWindowHeight->m_Entry.SetText(std::to_wstring(g_OptionsConfig.GameWindowHeight));

	CGUICheckbox *checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P7_LOCK_GAME_WINDOW_RESIZING, 0x00D2, 0x00D3, 0x00D2, 230, 114));
	checkbox->Checked = g_OptionsConfig.LockResizingGameWindow;
	checkbox->SetTextParameters(0, L"Lock game window moving and resizing", g_OptionsTextColor);



	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 64, 140));
	text->CreateTextureW(0, L"Adjust how long speech remains on screen");

	m_SliderSpeechDuration = (CGUISlider*)Add(new CGUISlider(ID_GO_P7_AJUST_LONG_SPEECH, 0x00D8, 0x00D8, 0x00D8, 0x00D5, true, false, 64, 160, 90, 0, 999, g_OptionsConfig.SpeechDelay));
	m_SliderSpeechDuration->SetTextParameters(true, STP_RIGHT, 0, g_OptionsTextColor, true);



	checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P7_SCALE_SPEECH_DURATION, 0x00D2, 0x00D3, 0x00D2, 64, 182));
	checkbox->Checked = g_OptionsConfig.ScaleSpeechDelay;
	checkbox->SetTextParameters(0, L"Scale speech duration based on length", g_OptionsTextColor);



	Add(new CGUIButton(ID_GO_P7_SPEECH_COLOR, 0x00D4, 0x00D4, 0x00D4, 64, 204));

	uint color = 0xFF7F7F7F;

	if (g_OptionsConfig.SpeechColor != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.SpeechColor);

	m_ColorSpeech = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P7_SPEECH_COLOR, g_OptionsConfig.SpeechColor, 67, 207, 13, 14, color));
	m_ColorSpeech->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 204));
	text->CreateTextureW(0, L"Speech Color");

	Add(new CGUIButton(ID_GO_P7_EMOTE_COLOR, 0x00D4, 0x00D4, 0x00D4, 64, 223));

	if (g_OptionsConfig.EmoteColor != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.EmoteColor);
	else
		color = 0xFF7F7F7F;

	m_ColorEmote = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P7_EMOTE_COLOR, g_OptionsConfig.EmoteColor, 67, 226, 13, 14, color));
	m_ColorEmote->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 223));
	text->CreateTextureW(0, L"Emote Color");

	Add(new CGUIButton(ID_GO_P7_PARTY_MESSAGE_COLOR, 0x00D4, 0x00D4, 0x00D4, 64, 242));

	if (g_OptionsConfig.PartyMessageColor != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.PartyMessageColor);
	else
		color = 0xFF7F7F7F;

	m_ColorPartyMessage = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P7_PARTY_MESSAGE_COLOR, g_OptionsConfig.PartyMessageColor, 67, 245, 13, 14, color));
	m_ColorPartyMessage->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 242));
	text->CreateTextureW(0, L"Party Message Color");



	checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P7_DARK_NIGHTS, 0x00D2, 0x00D3, 0x00D2, 64, 264));
	checkbox->Checked = g_OptionsConfig.DarkNights;
	checkbox->SetTextParameters(0, L"Dark Nights", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P7_COLORED_LIGHTING, 0x00D2, 0x00D3, 0x00D2, 64, 284));
	checkbox->Checked = g_OptionsConfig.ColoredLighting;
	checkbox->SetTextParameters(0, L"Colored Lighting", g_OptionsTextColor);

	if (g_PacketManager.ClientVersion >= CV_6000)
	{
		Add(new CGUIButton(ID_GO_P7_GUILD_MESSAGE_COLOR, 0x00D4, 0x00D4, 0x00D4, 354, 204));

		if (g_OptionsConfig.GuildMessageColor != 0xFFFF)
			color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.GuildMessageColor);
		else
			color = 0xFF7F7F7F;

		m_ColorGuildMessage = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P7_GUILD_MESSAGE_COLOR, g_OptionsConfig.GuildMessageColor, 357, 207, 13, 14, color));
		m_ColorGuildMessage->CallOnMouseUp = true;

		text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 376, 204));
		text->CreateTextureW(0, L"Guild Message Color");

		Add(new CGUIButton(ID_GO_P7_ALLIANCE_MESSAGE_COLOR, 0x00D4, 0x00D4, 0x00D4, 354, 223));

		if (g_OptionsConfig.AllianceMessageColor != 0xFFFF)
			color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.AllianceMessageColor);
		else
			color = 0xFF7F7F7F;

		m_ColorAllianceMessage = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P7_ALLIANCE_MESSAGE_COLOR, g_OptionsConfig.AllianceMessageColor, 357, 226, 13, 14, color));
		m_ColorAllianceMessage->CallOnMouseUp = true;

		text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 376, 223));
		text->CreateTextureW(0, L"Alliance Message Color");

		checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P7_IGNORE_GUILD_MESSAGE, 0x00D2, 0x00D3, 0x00D2, 354, 245));
		checkbox->Checked = g_OptionsConfig.IgnoreGuildMessage;
		checkbox->SetTextParameters(0, L"Ignore Guild Messages", g_OptionsTextColor);

		checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P7_IGNORE_ALLIANCE_MESSAGE, 0x00D2, 0x00D3, 0x00D2, 354, 265));
		checkbox->Checked = g_OptionsConfig.IgnoreAllianceMessage;
		checkbox->SetTextParameters(0, L"Ignore Alliance Messages", g_OptionsTextColor);
	}
}
//----------------------------------------------------------------------------
void CGumpOptions::DrawPage8()
{
	WISPFUN_DEBUG("c104_f14");
	//Reputation System
	Add(new CGUIPage(8));

	Add(new CGUIGumppic(0x00E5, 576, 177));

	CGUIText *text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 84, 22));
	text->CreateTextureW(0, L"Reputation System", 30, 460, TS_CENTER);

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 64, 44));
	text->CreateTextureW(0, L"These settings affect the reputation system, which is Ultima Online's system for controlling antisocial behavior.", 30, 500);



	Add(new CGUIButton(ID_GO_P8_INNOCENT_COLOR, 0x00D4, 0x00D4, 0x00D4, 64, 90));

	uint color = 0xFF7F7F7F;

	if (g_OptionsConfig.InnocentColor != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.InnocentColor);

	m_ColorInnocent = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P8_INNOCENT_COLOR, g_OptionsConfig.InnocentColor, 67, 93, 13, 14, color));
	m_ColorInnocent->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 90));
	text->CreateTextureW(0, L"Innocent highlight color");

	Add(new CGUIButton(ID_GO_P8_FRIENDLY_COLOR, 0x00D4, 0x00D4, 0x00D4, 64, 109));

	if (g_OptionsConfig.FriendlyColor != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.FriendlyColor);
	else
		color = 0xFF7F7F7F;

	m_ColorFriendly = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P8_FRIENDLY_COLOR, g_OptionsConfig.FriendlyColor, 67, 112, 13, 14, color));
	m_ColorFriendly->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 109));
	text->CreateTextureW(0, L"Friendly guilds highlight color");

	Add(new CGUIButton(ID_GO_P8_SOMEONE_COLOR, 0x00D4, 0x00D4, 0x00D4, 64, 128));

	if (g_OptionsConfig.SomeoneColor != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.SomeoneColor);
	else
		color = 0xFF7F7F7F;

	m_ColorSomeone = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P8_SOMEONE_COLOR, g_OptionsConfig.SomeoneColor, 67, 131, 13, 14, color));
	m_ColorSomeone->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 128));
	text->CreateTextureW(0, L"Someone that can be attacked color");

	Add(new CGUIButton(ID_GO_P8_CRIMINAL_COLOR, 0x00D4, 0x00D4, 0x00D4, 64, 147));

	if (g_OptionsConfig.CriminalColor != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.CriminalColor);
	else
		color = 0xFF7F7F7F;

	m_ColorCriminal = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P8_CRIMINAL_COLOR, g_OptionsConfig.CriminalColor, 67, 150, 13, 14, color));
	m_ColorCriminal->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 147));
	text->CreateTextureW(0, L"Criminal highlight color");

	Add(new CGUIButton(ID_GO_P8_ENEMY_COLOR, 0x00D4, 0x00D4, 0x00D4, 64, 166));

	if (g_OptionsConfig.EnemyColor != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.EnemyColor);
	else
		color = 0xFF7F7F7F;

	m_ColorEnemy = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P8_ENEMY_COLOR, g_OptionsConfig.EnemyColor, 67, 169, 13, 14, color));
	m_ColorEnemy->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 166));
	text->CreateTextureW(0, L"Enemy guildmembers color");

	Add(new CGUIButton(ID_GO_P8_MURDERER_COLOR, 0x00D4, 0x00D4, 0x00D4, 64, 185));

	if (g_OptionsConfig.MurdererColor != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.MurdererColor);
	else
		color = 0xFF7F7F7F;

	m_ColorMurderer = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P8_MURDERER_COLOR, g_OptionsConfig.MurdererColor, 67, 188, 13, 14, color));
	m_ColorMurderer->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 185));
	text->CreateTextureW(0, L"Murderer highlight color");



	CGUICheckbox *checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P8_QUERY_CRIMINAL_ACTIONS, 0x00D2, 0x00D3, 0x00D2, 64, 204));
	checkbox->Checked = g_OptionsConfig.CriminalActionsQuery;
	checkbox->SetTextParameters(0, L"Query before performing criminal actions", g_OptionsTextColor);
}
//----------------------------------------------------------------------------
void CGumpOptions::DrawPage9()
{
	WISPFUN_DEBUG("c104_f15");
	//Miscellaneous
	Add(new CGUIPage(9));

	Add(new CGUIGumppic(0x00E7, 576, 243));

	CGUIText *text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 84, 22));
	text->CreateTextureW(0, L"Miscellaneous", 30, 460, TS_CENTER);

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 64, 44));
	text->CreateTextureW(0, L"Miscellaneous options.", 30, 500);

	CGUICheckbox *checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P9_SHOW_APPROACHING_NAMES, 0x00D2, 0x00D3, 0x00D2, 64, 90));
	checkbox->Checked = g_OptionsConfig.ShowIncomingNames;
	checkbox->SetTextParameters(0, L"Show Names of Approaching Players", g_OptionsTextColor);

	checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P9_USE_CIRCLE_OF_TRANSPARENCY, 0x00D2, 0x00D3, 0x00D2, 64, 110));
	checkbox->Checked = g_OptionsConfig.UseCircleTrans;
	checkbox->SetTextParameters(0, L"Use circle of transparency", g_OptionsTextColor);



	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 64, 132));
	text->CreateTextureW(0, L"Set radius of transparency circle");

	m_SliderCircleTransparency = (CGUISlider*)Add(new CGUISlider(ID_GO_P9_TRANSPARENCY_RADIUS, 0x00D8, 0x00D8, 0x00D8, 0x00D5, true, false, 64, 153, 90, 0, 200, g_OptionsConfig.CircleTransRadius));
	m_SliderCircleTransparency->SetTextParameters(true, STP_RIGHT, 0, g_OptionsTextColor, true);



	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 64, 175));
	text->CreateTextureW(0, L"Inform me when my skills improve this much (in tenths)");

	m_SliderInformSkills = (CGUISlider*)Add(new CGUISlider(ID_GO_P9_INFORM_SKILLS, 0x00D8, 0x00D8, 0x00D8, 0x00D5, true, false, 64, 196, 90, 0, 100, g_OptionsConfig.SkillReport));
	m_SliderInformSkills->SetTextParameters(true, STP_RIGHT, 0, g_OptionsTextColor, true);



	checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P9_INFORM_STATS, 0x00D2, 0x00D3, 0x00D2, 64, 216));
	checkbox->Checked = g_OptionsConfig.StatReport;
	checkbox->SetTextParameters(0, L"Inform me of increases in strength, dexterity, and intelligence.", g_OptionsTextColor);

	Add(new CGUIButton(ID_GO_P9_SPEECH_FONT, 0x00D0, 0x00D0, 0x00D0, 64, 243));

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 243));
	text->CreateTextureW(0, L"Set the font for speech");
}
//----------------------------------------------------------------------------
void CGumpOptions::DrawPage10()
{
	WISPFUN_DEBUG("c104_f16");
	//Filter Options
	Add(new CGUIPage(10));

	Add(new CGUIGumppic(0x00EA, 576, 309));
}
//----------------------------------------------------------------------------
void CGumpOptions::UpdateColor(const SELECT_COLOR_GUMP_STATE &state, const ushort &color)
{
	WISPFUN_DEBUG("c104_f17");
	switch (state)
	{
		case SCGS_OPT_TOOLTIP_TEXT:
		{
			g_OptionsConfig.ToolTipsTextColor = color;
			m_ColorTooltipText->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_CHAT_INPUT_TEXT:
		{
			g_OptionsConfig.ChatColorInputText = color;
			m_ColorInputText->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_CHAT_MENU_OPTION:
		{
			g_OptionsConfig.ChatColorMenuOption = color;
			m_ColorMenuOption->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_CHAT_PLAYER_IN_MEMBER_LIST:
		{
			g_OptionsConfig.ChatColorPlayerInMemberList = color;
			m_ColorPlayerColorInMemberList->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_CHAT_TEXT:
		{
			g_OptionsConfig.ChatColorText = color;
			m_ColorChatText->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_CHAT_PLAYER_WITHOUT_PRIV:
		{
			g_OptionsConfig.ChatColorPlayerNameWithout = color;
			m_ColorPlayerNameWithoutSpeakingPrivileges->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_CHAT_MUTED_TEXT:
		{
			g_OptionsConfig.ChatColorMuted = color;
			m_ColorMutedText->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_CHAT_CHAN_MODER_NAME:
		{
			g_OptionsConfig.ChatColorChannelModeratorName = color;
			m_ColorChannelModeratorName->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_CHAT_CHAN_MODER_TEXT:
		{
			g_OptionsConfig.ChatColorChannelModeratorText = color;
			m_ColorChannelModeratorText->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_CHAT_MY_NAME:
		{
			g_OptionsConfig.ChatColorMyName = color;
			m_ColorMyName->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_CHAT_MY_TEXT:
		{
			g_OptionsConfig.ChatColorMyText = color;
			m_ColorMyText->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_CHAT_SYSTEM_MESSAGE:
		{
			g_OptionsConfig.ChatColorSystemMessage = color;
			m_ColorSystemMessage->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_CHAT_BG_OUTPUT:
		{
			g_OptionsConfig.ChatColorBGOutputText = color;
			m_ColorTextOutputBackground->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_CHAT_BG_INPUT:
		{
			g_OptionsConfig.ChatColorBGInputText = color;
			m_ColorTextInputBackground->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_CHAT_BG_USER_LIST:
		{
			g_OptionsConfig.ChatColorBGUserList = color;
			m_ColorUserListBackground->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_CHAT_BG_CONF_LIST:
		{
			g_OptionsConfig.ChatColorBGCommandList = color;
			m_ColorConferenceListBackground->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_CHAT_BG_COMMAND_LIST:
		{
			g_OptionsConfig.ChatColorBGCommandList = color;
			m_ColorCommandListBackground->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_DISPLAY_SPEECH:
		{
			g_OptionsConfig.SpeechColor = color;
			m_ColorSpeech->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_DISPLAY_EMOTE:
		{
			g_OptionsConfig.EmoteColor = color;
			m_ColorEmote->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_DISPLAY_PARTY_MESSAGE:
		{
			g_OptionsConfig.PartyMessageColor = color;
			m_ColorPartyMessage->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_DISPLAY_GUILD_MESSAGE:
		{
			g_OptionsConfig.GuildMessageColor = color;
			m_ColorGuildMessage->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_DISPLAY_ALLIANCE_MESSAGE:
		{
			g_OptionsConfig.AllianceMessageColor = color;
			m_ColorAllianceMessage->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_REPSYS_INNOCENT:
		{
			g_OptionsConfig.InnocentColor = color;
			m_ColorInnocent->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_REPSYS_FRIENDLY:
		{
			g_OptionsConfig.FriendlyColor = color;
			m_ColorFriendly->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_REPSYS_SOMEONE:
		{
			g_OptionsConfig.SomeoneColor = color;
			m_ColorSomeone->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_REPSYS_CRIMINAL:
		{
			g_OptionsConfig.CriminalColor = color;
			m_ColorCriminal->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_REPSYS_ENEMY:
		{
			g_OptionsConfig.EnemyColor = color;
			m_ColorEnemy->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_REPSYS_MURDERER:
		{
			g_OptionsConfig.MurdererColor = color;
			m_ColorMurderer->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------
void CGumpOptions::GUMP_BUTTON_EVENT_C
{
	WISPFUN_DEBUG("c104_f18");
	if (serial == ID_GO_PAGE_6)
	{
		m_ContainerOffsetX->m_Entry.SetText(std::to_string(g_ContainerRect.DefaultX));
		m_ContainerOffsetY->m_Entry.SetText(std::to_string(g_ContainerRect.DefaultY));
	}
	else if (serial == ID_GO_PAGE_7)
	{
		m_GameWindowWidth->m_Entry.SetText(std::to_string(g_OptionsConfig.GameWindowWidth));
		m_GameWindowHeight->m_Entry.SetText(std::to_string(g_OptionsConfig.GameWindowHeight));
	}
	else if (serial == ID_GO_CANCEL) //Cancel
	{
		g_OptionsMacroManager.Clear();
		m_RemoveMark = true;
	}
	else if (serial == ID_GO_APPLY) //Apply
		ApplyPageChanges();
	else if (serial == ID_GO_DEFAULT) //Default
	{
		switch (m_Page)
		{
			case 1:
			{
				g_OptionsConfig.DefaultPage1();
				break;
			}
			case 2:
			{
				g_OptionsConfig.DefaultPage2();
				break;
			}
			case 3:
			{
				g_OptionsConfig.DefaultPage3();
				break;
			}
			case 4:
			{
				g_OptionsConfig.DefaultPage4();
				break;
			}
			case 6:
			{
				g_OptionsConfig.DefaultPage6();

				m_ContainerOffsetX->m_Entry.SetText(std::to_string(g_ContainerRect.DefaultX));
				m_ContainerOffsetY->m_Entry.SetText(std::to_string(g_ContainerRect.DefaultY));

				break;
			}
			case 7:
			{
				g_OptionsConfig.DefaultPage7();

				m_GameWindowWidth->m_Entry.SetText(std::to_string(g_OptionsConfig.GameWindowWidth));
				m_GameWindowHeight->m_Entry.SetText(std::to_string(g_OptionsConfig.GameWindowHeight));

				break;
			}
			case 8:
			{
				g_OptionsConfig.DefaultPage8();
				break;
			}
			case 9:
			{
				g_OptionsConfig.DefaultPage9();
				break;
			}
			default:
				break;
		}

		m_WantUpdateContent = true;
	}
	else if (serial == ID_GO_OKAY) //Okay
	{
		/*IFOR(i, 0, 10)
		{
			m_Page = i;
			ApplyPageChanges();
		}*/

		ApplyPageChanges();
		g_OptionsMacroManager.Clear();

		m_RemoveMark = true;
	}
	else
	{
		switch (m_Page)
		{
			case 1: //Sound and Music
			{
				break;
			}
			case 2: //Orion's configuration
			{
				break;
			}
			case 3: //Language
			{
				if (serial == ID_GO_P3_TEXT_COLOR) //Color of Tool-tip text
					g_GumpManager.AddGump(new CGumpSelectColor(0, 100, 100, SCGS_OPT_TOOLTIP_TEXT));
				else if (serial == ID_GO_P3_TEXT_FONT) //Font
					g_GumpManager.AddGump(new CGumpSelectFont(0, 320, 240, SFGS_OPT_TOOLTIP));

				break;
			}
			case 4: //Chat
			{
				if (serial >= ID_GO_P4_TEXT_COLOR) //Input text color
					g_GumpManager.AddGump(new CGumpSelectColor(0, 100, 100, (SELECT_COLOR_GUMP_STATE)(SCGS_OPT_CHAT_INPUT_TEXT + (serial - ID_GO_P4_TEXT_COLOR))));
				else if (serial == ID_GO_P4_TEXT_FONT) //Font
					g_GumpManager.AddGump(new CGumpSelectFont(0, 320, 240, SFGS_OPT_CHAT));

				break;
			}
			case 5: //Macro Options
			{
				if (serial == ID_GO_P5_BUTTON_ADD) //Add button
				{
					m_MacroPointer = CMacro::CreateBlankMacro();
					m_MacroObjectPointer = (CMacroObject*)m_MacroPointer->m_Items;
					g_OptionsMacroManager.Add(m_MacroPointer);

					m_MacroKey->m_Entry.SetText(m_HotkeyText[m_MacroPointer->Key & 0xFF]);

					RedrawMacroData();
				}
				else if (serial == ID_GO_P5_BUTTON_DELETE) //Delete button
				{
					if (m_LastChangeMacroTime < g_Ticks)
					{
						CMacro *newpointer = (CMacro*)m_MacroPointer->m_Next;

						if (newpointer == NULL)
							newpointer = (CMacro*)m_MacroPointer->m_Prev;

						g_OptionsMacroManager.Delete(m_MacroPointer);

						if (newpointer == NULL)
						{
							newpointer = CMacro::CreateBlankMacro();
							g_OptionsMacroManager.Add(newpointer);
						}

						m_MacroPointer = newpointer;
						m_MacroObjectPointer = (CMacroObject*)m_MacroPointer->m_Items;

						m_MacroKey->m_Entry.SetText(m_HotkeyText[m_MacroPointer->Key & 0xFF]);

						RedrawMacroData();

						m_LastChangeMacroTime = g_Ticks + CHANGE_MACRO_DELAY;
					}
				}
				else if (serial == ID_GO_P5_BUTTON_PREVEOUS) //Preveous button
				{
					if (m_MacroPointer->m_Prev != NULL && m_LastChangeMacroTime < g_Ticks)
					{
						m_MacroPointer = (CMacro*)m_MacroPointer->m_Prev;
						m_MacroObjectPointer = (CMacroObject*)m_MacroPointer->m_Items;

						RedrawMacroData();

						m_LastChangeMacroTime = g_Ticks + CHANGE_MACRO_DELAY;
					}
				}
				else if (serial == ID_GO_P5_BUTTON_NEXT && m_LastChangeMacroTime < g_Ticks) //Next button
				{
					if (m_MacroPointer->m_Next != NULL)
					{
						m_MacroPointer = (CMacro*)m_MacroPointer->m_Next;
						m_MacroObjectPointer = (CMacroObject*)m_MacroPointer->m_Items;

						RedrawMacroData();

						m_LastChangeMacroTime = g_Ticks + CHANGE_MACRO_DELAY;
					}
				}
				else if (serial == ID_GO_P5_BUTTON_UP) //Up button
				{
					if (m_MacroObjectPointer->m_Prev != NULL)
					{
						m_MacroObjectPointer = (CMacroObject*)m_MacroObjectPointer->m_Prev;
						RedrawMacroData();
					}
				}
				else if (serial == ID_GO_P5_BUTTON_DOWN) //Down button
				{
					if (m_MacroObjectPointer->m_Next != NULL)
					{
						m_MacroObjectPointer = (CMacroObject*)m_MacroObjectPointer->m_Next;
						RedrawMacroData();
					}
				}

				break;
			}
			case 6: //Interface
			{
				break;
			}
			case 7: //Display
			{
				if (serial >= ID_GO_P7_SPEECH_COLOR && serial <= ID_GO_P7_ALLIANCE_MESSAGE_COLOR) //Speech color
					g_GumpManager.AddGump(new CGumpSelectColor(0, 100, 100, (SELECT_COLOR_GUMP_STATE)(SCGS_OPT_DISPLAY_SPEECH + (serial - ID_GO_P7_SPEECH_COLOR))));

				break;
			}
			case 8: //Reputation System
			{
				if (serial >= ID_GO_P8_INNOCENT_COLOR && serial <= ID_GO_P8_MURDERER_COLOR) //Innocent highlight color
					g_GumpManager.AddGump(new CGumpSelectColor(0, 100, 100, (SELECT_COLOR_GUMP_STATE)(SCGS_OPT_REPSYS_INNOCENT + (serial - ID_GO_P8_INNOCENT_COLOR))));

				break;
			}
			case 9: //Miscellaneous
			{
				if (serial == ID_GO_P9_SPEECH_FONT) //Speech Font
					g_GumpManager.AddGump(new CGumpSelectFont(0, 320, 240, SFGS_OPT_MISCELLANEOUS));

				break;
			}
			case 10: //Filter Options
			{
				break;
			}
			default:
				break;
		}
	}
}
//----------------------------------------------------------------------------
void CGumpOptions::GUMP_CHECKBOX_EVENT_C
{
	WISPFUN_DEBUG("c104_f19");
	switch (m_Page)
	{
		case 1: //Sound and Music
		{
			if (serial == ID_GO_P1_SOUND_ON_OFF) //Sound on/off
				g_OptionsConfig.Sound = state;
			else if (serial == ID_GO_P1_MUSIC_ON_OFF) //Music on/off
				g_OptionsConfig.Music = state;
			else if (serial == ID_GO_P1_PLAY_FOOTSTEP_SOUNDS) //Play footstep sounds
				g_OptionsConfig.FootstepsSound = state;
			else if (serial == ID_GO_P1_PLAY_COMBAT_MUSIC) //Play combat music
				g_OptionsConfig.CombatMusic = state;

			break;
		}
		case 2: //Orion's configuration
		{
			if (serial == ID_GO_P2_ENABLE_SCALING) //Use scaling in game window
				g_OptionsConfig.UseScaling = state;
			else if (serial == ID_GO_P2_REMOVE_TEXT_WITH_BLENDING) //Remove object's text with alpha-blending
				g_OptionsConfig.RemoveTextWithBlending = state;
			else if (serial == ID_GO_P2_DRAW_STUMPS)
				g_OptionsConfig.DrawStumps = state;
			else if (serial == ID_GO_P2_MARKING_CAVES)
				g_OptionsConfig.MarkingCaves = state;
			else if (serial == ID_GO_P2_NO_VEGETATION)
				g_OptionsConfig.NoVegetation = state;
			else if (serial == ID_GO_P2_NO_ANIMATE_FIELDS)
				g_OptionsConfig.NoAnimateFields = state;
			else if (serial == ID_GO_P2_REDUCE_FPS_UNACTIVE_WINDOW) //Reduce FPS when Window is Unactive
				g_OptionsConfig.ReduceFPSUnactiveWindow = state;
			else if (serial == ID_GO_P2_CHARACTERS_ANIMATION_DELAY) //Standard characters animation delay
				g_OptionsConfig.StandartCharactersAnimationDelay = state;
			else if (serial == ID_GO_P2_ITEMS_ANIMATION_DELAY) //Standard items animation delay
				g_OptionsConfig.StandartItemsAnimationDelay = state;
			else if (serial == ID_GO_P2_LOCK_GUMP_MOVING) //Lock gump moving
				g_OptionsConfig.LockGumpsMoving = state;
			else if (serial == ID_GO_P2_CONSOLE_ENTER) //Console need press 'Enter' to activate it.
				g_OptionsConfig.ConsoleNeedEnter = state;
			else if (serial == ID_GO_P2_USE_HIDDEN_MODE_ONLY_FOR_SELF)
				g_OptionsConfig.UseHiddenModeOnlyForSelf = state;
			else if (serial == ID_GO_P2_TRANSPARENT_SPELL_ICONS)
				g_OptionsConfig.TransparentSpellIcons = state;
			else if (serial == ID_GO_P2_OLD_STYLE_STATUSBAR)
				g_OptionsConfig.OldStyleStatusbar = state;
			else if (serial == ID_GO_P2_ORIGINAL_PARTY_STATUSBAR)
				g_OptionsConfig.OriginalPartyStatusbar = state;
			else if (serial == ID_GO_P2_APPLY_STATE_COLOR_ON_CHARACTERS)
				g_OptionsConfig.ApplyStateColorOnCharacters = state;
			else if (serial == ID_GO_P2_CHANGE_FIELDS_GRAPHIC)
				g_OptionsConfig.ChangeFieldsGraphic = state;
			else if (serial == ID_GO_P2_PAPERDOLL_SLOTS)
				g_OptionsConfig.PaperdollSlots = state;
			else if (serial == ID_GO_P2_REMOVE_STATUSBARS_WITHOUT_OBJECTS)
				g_OptionsConfig.RemoveStatusbarsWithoutObjects = state;
			else if (serial == ID_GO_P2_SHOW_CONSOLE_ENTRY_MODE)
				g_OptionsConfig.ShowDefaultConsoleEntryMode = state;
			else if (serial == ID_GO_P2_DRAW_AURA_WITH_CTRL_PRESSED)
				g_OptionsConfig.DrawAuraWithCtrlPressed = state;
			else if (serial == ID_GO_P2_SCALE_IMAGES_IN_PAPERDOLL_SLOTS)
				g_OptionsConfig.ScaleImagesInPaperdollSlots = state;
			else if (serial == ID_GO_P2_REMOVE_OR_CREATE_OBJECTS_WITH_BLENDING)
				g_OptionsConfig.RemoveOrCreateObjectsWithBlending = state;
			else if (serial == ID_GO_P2_DRAW_HELMETS_ON_SHROUD)
				g_OptionsConfig.DrawHelmetsOnShroud = state;
			else if (serial == ID_GO_P2_USE_GLOBAL_MAP_LAYER)
				g_OptionsConfig.UseGlobalMapLayer = state;
			else if (serial == ID_GO_P2_NO_DRAW_ROOFS)
				g_OptionsConfig.NoDrawRoofs = state;
			else if (serial == ID_GO_P2_HIGHLIGHT_TARGET_BY_TYPE)
				g_OptionsConfig.HighlightTargetByType = state;
			else if (serial == ID_GO_P2_AUTO_DISPLAY_WORLD_MAP)
				g_OptionsConfig.AutoDisplayWorldMap = state;
			else if (serial == ID_GO_P2_USE_GL_LISTS_FOR_INTERFACE)
				g_OptionsConfig.UseGLListsForInterface = state;
			else if (serial == ID_GO_P2_DEV_MODE_1)
				g_OptionsDeveloperMode = DM_NO_DEBUG;
			else if (serial == ID_GO_P2_DEV_MODE_2)
				g_OptionsDeveloperMode = DM_SHOW_FPS_ONLY;
			else if (serial == ID_GO_P2_DEV_MODE_3)
				g_OptionsDeveloperMode = DM_DEBUGGING;
			

			break;
		}
		case 3: //Language
		{
			if (serial == ID_GO_P3_USE_TOOLTIP) //Use Tool-tips
				g_OptionsConfig.UseToolTips = state;

			break;
		}
		case 4: //Chat
		{

			break;
		}
		case 5: //Macro Options
		{
			if (serial == ID_GO_P5_BUTTON_SHIFT) //Shift checkbox
				m_MacroPointer->Shift = state;
			else if (serial == ID_GO_P5_BUTTON_ALT) //Alt checkbox
				m_MacroPointer->Alt = state;
			else if (serial == ID_GO_P5_BUTTON_CTRL) //Ctrl checkbox
				m_MacroPointer->Ctrl = state;

			break;
		}
		case 6: //Interface
		{
			if (serial == ID_GO_P6_ENABLE_PATHFINDING) //Enable pathfinding with double-right-click
				g_OptionsConfig.EnablePathfind = state;
			else if (serial == ID_GO_P6_HOLD_TAB_FOR_COMBAT) //Hold down TAB key for combat, instead of tapping it to toggle combat mode
				g_OptionsConfig.HoldTabForCombat = state;
			else if (serial == ID_GO_P6_OFFSET_INTERFACE_WINDOWS) //Offset interface windows rather than perfectly stacking them
				g_OptionsConfig.OffsetInterfaceWindows = state;
			else if (serial == ID_GO_P6_AUTO_ARRANGE_MINIMIZED_WINDOWS) //Automatically arrange minimized windows
				g_OptionsConfig.AutoArrange = state;
			else if (serial == ID_GO_P6_ALWAYS_RUN) //Your character will always run if this is checked
				g_OptionsConfig.AlwaysRun = state;
			else if (serial == ID_GO_P6_DISABLE_MENUBAR) //Disable the Menu Bar
				g_OptionsConfig.DisableMenubar = state;
			else if (serial == ID_GO_P6_GRAY_OUT_OF_RANGE_OBJECTS) //Gray out of range objects
				g_OptionsConfig.GrayOutOfRangeObjects = state;
			else if (serial == ID_GO_P6_DISABLE_NEW_TARGET_SYSTEM) //Disable New Target System
				g_OptionsConfig.DisableNewTargetSystem = state;
			else if (serial == ID_GO_P6_OBJECT_HANDLES) //Object Handles
				g_OptionsConfig.ObjectHandles = state;
			else if (serial == ID_GO_P6_DISPLAY_ITEM_PROPERTIES_ICON) //Display Item Properties Icon
				g_OptionsConfig.ItemPropertiesIcon = state;
			else if (serial == ID_GO_P6_HOLD_SHIFT_FOR_CONTEXT_MENUS) //Hold Shift For Context Menus
				g_OptionsConfig.HoldShiftForContextMenus = state;
			else if (serial == ID_GO_P6_HOLD_SHIFT_FOR_ENABLE_PATHFINDING) //Hold Shift For Enable Pathfinding
				g_OptionsConfig.HoldShiftForEnablePathfind = state;

			break;
		}
		case 7: //Display
		{
			if (serial == ID_GO_P7_SCALE_SPEECH_DURATION) //Scale speech duration based on length
				g_OptionsConfig.ScaleSpeechDelay = state;
			else if (serial == ID_GO_P7_IGNORE_GUILD_MESSAGE) //Ignore Guild Messages
				g_OptionsConfig.IgnoreGuildMessage = state;
			else if (serial == ID_GO_P7_IGNORE_ALLIANCE_MESSAGE) //Ignore Alliance Messages
				g_OptionsConfig.IgnoreAllianceMessage = state;
			else if (serial == ID_GO_P7_DARK_NIGHTS) //Dark Nights
				g_OptionsConfig.DarkNights = state;
			else if (serial == ID_GO_P7_COLORED_LIGHTING) //Colored Lighting
				g_OptionsConfig.ColoredLighting = state;
			else if (serial == ID_GO_P7_LOCK_GAME_WINDOW_RESIZING) //Lock game window resizing
				g_OptionsConfig.LockResizingGameWindow = state;

			break;
		}
		case 8: //Reputation System
		{
			if (serial == ID_GO_P8_QUERY_CRIMINAL_ACTIONS) //Query before performing criminal actions
				g_OptionsConfig.CriminalActionsQuery = state;

			break;
		}
		case 9: //Miscellaneous
		{
			if (serial == ID_GO_P9_SHOW_APPROACHING_NAMES) //Show Names of Approaching Players
				g_OptionsConfig.ShowIncomingNames = state;
			else if (serial == ID_GO_P9_USE_CIRCLE_OF_TRANSPARENCY) //Use circle of transparency
				g_OptionsConfig.UseCircleTrans = state;
			else if (serial == ID_GO_P9_INFORM_STATS) //Inform me of increases in strength, dexterity, and intelligence.
				g_OptionsConfig.StatReport = state;

			break;
		}
		case 10: //Filter Options
		{
			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------
void CGumpOptions::GUMP_RADIO_EVENT_C
{
	WISPFUN_DEBUG("c104_f20");
	if (!state)
		return;

	switch (m_Page)
	{
		case 1: //Sound and Music
		{
			break;
		}
		case 2: //Orion's configuration
		{
			if (serial == ID_GO_P2_NO_DRAW_CHARACTERS_STATUS) //No draw
				g_OptionsConfig.DrawStatusState = DCSS_NO_DRAW;
			else if (serial == ID_GO_P2_DRAW_CHARACTERS_STATUS_TOP) //Above character
				g_OptionsConfig.DrawStatusState = DCSS_ABOVE;
			else if (serial == ID_GO_P2_DRAW_CHARACTERS_STATUS_BOTTOM) //Under character
				g_OptionsConfig.DrawStatusState = DCSS_UNDER;
			else if (serial == ID_GO_P2_HIDDEN_CHARACTES_MODE_1)
				g_OptionsConfig.HiddenCharactersRenderMode = HCRM_ORIGINAL;
			else if (serial == ID_GO_P2_HIDDEN_CHARACTES_MODE_2)
				g_OptionsConfig.HiddenCharactersRenderMode = HCRM_ALPHA_BLENDING;
			else if (serial == ID_GO_P2_HIDDEN_CHARACTES_MODE_3)
				g_OptionsConfig.HiddenCharactersRenderMode = HCRM_SPECTRAL_COLOR;
			else if (serial == ID_GO_P2_HIDDEN_CHARACTES_MODE_4)
				g_OptionsConfig.HiddenCharactersRenderMode = HCRM_SPECIAL_SPECTRAL_COLOR;
			else if (serial == ID_GO_P2_DRAW_CHARACTER_BARS_ALWAYS)
				g_OptionsConfig.DrawStatusConditionState = DCSCS_ALWAYS;
			else if (serial == ID_GO_P2_DRAW_CHARACTER_BARS_NOT_MAX)
				g_OptionsConfig.DrawStatusConditionState = DCSCS_NOT_MAX;
			else if (serial == ID_GO_P2_DRAW_CHARACTER_BARS_LOWER)
				g_OptionsConfig.DrawStatusConditionState = DCSCS_LOWER;
			else if (serial == ID_GO_P2_DRAW_AURA_NEVER)
				g_OptionsConfig.DrawAuraState = DAS_NEVER;
			else if (serial == ID_GO_P2_DRAW_AURA_IN_WARMODE)
				g_OptionsConfig.DrawAuraState = DAS_IN_WARMODE;
			else if (serial == ID_GO_P2_DRAW_AURA_ALWAYS)
				g_OptionsConfig.DrawAuraState = DAS_ALWAYS;
			else if (serial == ID_GO_P2_SCREENSHOT_FORMAT_BMP)
				g_OptionsConfig.ScreenshotFormat = SF_BMP;
			else if (serial == ID_GO_P2_SCREENSHOT_FORMAT_PNG)
				g_OptionsConfig.ScreenshotFormat = SF_PNG;
			else if (serial == ID_GO_P2_SCREENSHOT_FORMAT_TIFF)
				g_OptionsConfig.ScreenshotFormat = SF_TIFF;
			else if (serial == ID_GO_P2_SCREENSHOT_FORMAT_JPEG)
				g_OptionsConfig.ScreenshotFormat = SF_JPEG;
			else if (serial == ID_GO_P2_DEV_MODE_1)
				g_OptionsDeveloperMode = DM_NO_DEBUG;
			else if (serial == ID_GO_P2_DEV_MODE_2)
				g_OptionsDeveloperMode = DM_SHOW_FPS_ONLY;
			else if (serial == ID_GO_P2_DEV_MODE_3)
				g_OptionsDeveloperMode = DM_DEBUGGING;

			break;
		}
		case 3: //Language
		{
			break;
		}
		case 4: //Chat
		{

			break;
		}
		case 5: //Macro Options
		{
			break;
		}
		case 6: //Interface
		{
			break;
		}
		case 7: //Display
		{
			break;
		}
		case 8: //Reputation System
		{
			break;
		}
		case 9: //Miscellaneous
		{
			break;
		}
		case 10: //Filter Options
		{
			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------
void CGumpOptions::GUMP_SLIDER_CLICK_EVENT_C
{
	WISPFUN_DEBUG("c104_f21");
	OnSliderMove(serial);
}
//----------------------------------------------------------------------------
void CGumpOptions::GUMP_SLIDER_MOVE_EVENT_C
{
	WISPFUN_DEBUG("c104_f22");
	switch (m_Page)
	{
		case 1: //Sound and Music
		{
			if (serial == ID_GO_P1_SOUND_VOLUME)
				g_OptionsConfig.SoundVolume = m_SliderSound->Value;
			else if (serial == ID_GO_P1_MUSIC_VOLUME)
				g_OptionsConfig.MusicVolume = m_SliderMusic->Value;

			break;
		}
		case 2: //Orion's configuration
		{
			if (serial == ID_GO_P2_CLIENT_FPS)
				g_OptionsConfig.ClientFPS = m_SliderClientFPS->Value;
			else if (serial == ID_GO_P2_HIDDEN_ALPHA)
				g_OptionsConfig.HiddenAlpha = m_SliderHiddenAlpha->Value;
			else if (serial == ID_GO_P2_SPELL_ICONS_ALPHA)
				g_OptionsConfig.SpellIconAlpha = m_SliderSpellIconsAlpha->Value;
			else if (serial == ID_GO_P2_DRAW_CHARACTER_BARS_LOWER_VALUE)
				g_OptionsConfig.DrawStatusConditionValue = m_SliderDrawStatusConditionValue->Value;

			break;
		}
		case 3: //Language
		{
			if (serial == ID_GO_P3_DELAY_BEFORE_TOOLTIP)
				g_OptionsConfig.ToolTipsDelay = m_SliderTooltipDelay->Value;

			break;
		}
		case 4: //Chat
		{
			break;
		}
		case 5: //Macro Options
		{
			break;
		}
		case 6: //Interface
		{
			break;
		}
		case 7: //Display
		{
			if (serial == ID_GO_P7_AJUST_LONG_SPEECH)
				g_OptionsConfig.SpeechDelay = m_SliderSpeechDuration->Value;
					
			break;
		}
		case 8: //Reputation System
		{
			break;
		}
		case 9: //Miscellaneous
		{
			if (serial == ID_GO_P9_TRANSPARENCY_RADIUS)
				g_OptionsConfig.CircleTransRadius = m_SliderCircleTransparency->Value;
			else if (serial == ID_GO_P9_INFORM_SKILLS)
				g_OptionsConfig.SkillReport = m_SliderInformSkills->Value;
			break;
		}
		case 10: //Filter Options
		{
			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------
void CGumpOptions::GUMP_COMBOBOX_SELECTION_EVENT_C
{
	WISPFUN_DEBUG("c104_f23");
	bool isAction = false;
	int index = serial - ID_GO_P5_MACRO_SELECTION;

	if (serial >= ID_GO_P5_ACTION_SELECTION)
	{
		isAction = true;
		index = serial - ID_GO_P5_ACTION_SELECTION;
	}

	int macroIndex = 0;

	while (index >= 1000)
	{
		index -= 1000;
		macroIndex++;
	}

	CMacroObject *obj = m_MacroObjectPointer;

	if (obj != NULL)
	{
		const int maxMacroDraw = 7;
		int macroCount = 0;

		while (obj != NULL && macroCount < maxMacroDraw)
		{
			if (macroCount == macroIndex)
				break;

			macroCount++;
			obj = (CMacroObject*)obj->m_Next;
		}
	}

	if (obj != NULL)
	{
		if (!isAction)
		{
			MACRO_CODE code = (MACRO_CODE)index;

			if (obj->Code != code)
			{
				CMacroObject *newobj = CMacro::CreateMacro(code);

				if (obj == m_MacroObjectPointer)
					m_MacroObjectPointer = newobj;

				m_MacroPointer->ChangeObject(obj, newobj);
				obj = newobj;
			}

			if (code != MC_NONE)
			{
				if (obj->m_Next == NULL)
					m_MacroPointer->Add(new CMacroObject(MC_NONE, MSC_NONE));
			}
			else if (obj->m_Next != NULL && obj->m_Next->m_Next == NULL && ((CMacroObject*)obj->m_Next)->Code == MC_NONE)
			{
				if (obj == m_MacroObjectPointer)
					m_MacroObjectPointer = (CMacroObject*)obj->m_Next;

				m_MacroPointer->Delete(obj);
			}
		}
		else
		{
			int macroListOffset = 0;
			int macroListCount = 0;
			CMacro::GetBoundByCode(obj->Code, macroListCount, macroListOffset);

			obj->SubCode = (MACRO_SUB_CODE)(macroListOffset + index);
		}

		m_WantRedrawMacroData = true;
	}
}
//----------------------------------------------------------------------------
void CGumpOptions::OnCharPress(const WPARAM &wParam, const LPARAM &lParam)
{
	WISPFUN_DEBUG("c104_f24");
	if (g_EntryPointer == &m_GameWindowWidth->m_Entry || g_EntryPointer == &m_GameWindowHeight->m_Entry || g_EntryPointer == &m_ContainerOffsetX->m_Entry || g_EntryPointer == &m_ContainerOffsetY->m_Entry)
	{
		if (wParam >= '0' && wParam <= '9')
		{
			g_EntryPointer->Insert(wParam);

			int val = atoi(g_EntryPointer->c_str());

			if (val > 2000)
				g_EntryPointer->Remove(true);
			else
				m_WantRedraw = true;
		}
	}
	else if (g_EntryPointer != &m_MacroKey->m_Entry)
	{
		CMacroObject *obj = m_MacroObjectPointer;

		if (obj != NULL)
		{
			CGUITextEntry *entry = NULL;

			QFOR(item, m_MacroDataBox->m_Items, CBaseGUI*)
			{
				if (item->Type == GOT_TEXTENTRY && g_EntryPointer == &((CGUITextEntry*)item)->m_Entry)
				{
					entry = (CGUITextEntry*)item;
					break;
				}
			}

			if (entry != NULL)
			{
				const int maxMacroDraw = 7;
				int macroCount = 0;

				while (obj != NULL && macroCount < maxMacroDraw)
				{
					if (obj->HasSubMenu == 2 && entry->Serial == ID_GO_P5_ACTION_SELECTION + (macroCount * 1000))
						break;

					macroCount++;
					obj = (CMacroObject*)obj->m_Next;
				}
			}
		}

		if (obj != NULL)
		{
			bool canAdd = false;

			switch (obj->Code)
			{
				case MC_DELAY:
				case MC_SET_UPDATE_RANGE:
				case MC_MODIFY_UPDATE_RANGE:
				{
					if (wParam >= '0' && wParam <= '9')
						canAdd = true;

					break;
				}
				case MC_SAY:
				case MC_EMOTE:
				case MC_WHISPER:
				case MC_YELL:
				{
					canAdd = true;

					break;
				}
				default:
					break;
			}

			if (canAdd)
			{
				g_EntryPointer->Insert(wParam);
				((CMacroObjectString*)obj)->String = g_EntryPointer->c_str();
				m_WantRedraw = true;
			}
		}
	}
}
//----------------------------------------------------------------------------
void CGumpOptions::OnKeyDown(const WPARAM &wParam, const LPARAM &lParam)
{
	WISPFUN_DEBUG("c104_f25");
	if (g_EntryPointer == &m_MacroKey->m_Entry)
	{
		m_MacroPointer->Key = wParam & 0xFF;

		m_MacroKey->m_Entry.SetText(m_HotkeyText[wParam & 0xFF]);

		m_WantRedraw = true;
	}
	else
	{
		if (wParam == VK_RETURN)
		{
			if (g_ConfigManager.ConsoleNeedEnter)
				g_EntryPointer = NULL;
			else
				g_EntryPointer = &g_GameConsole;

			m_WantRedraw = true;
		}
		else
		{
			g_EntryPointer->OnKey(this, wParam);

			if (g_EntryPointer != &m_GameWindowWidth->m_Entry && g_EntryPointer != &m_GameWindowHeight->m_Entry && g_EntryPointer != &m_ContainerOffsetX->m_Entry && g_EntryPointer != &m_ContainerOffsetY->m_Entry)
			{
				CMacroObject *obj = m_MacroObjectPointer;

				if (obj != NULL)
				{
					CGUITextEntry *entry = NULL;

					QFOR(item, m_MacroDataBox->m_Items, CBaseGUI*)
					{
						if (item->Type == GOT_TEXTENTRY && g_EntryPointer == &((CGUITextEntry*)item)->m_Entry)
						{
							entry = (CGUITextEntry*)item;
							break;
						}
					}

					if (entry != NULL)
					{
						const int maxMacroDraw = 7;
						int macroCount = 0;

						while (obj != NULL && macroCount < maxMacroDraw)
						{
							if (obj->HasSubMenu == 2 && entry->Serial == ID_GO_P5_ACTION_SELECTION + (macroCount * 1000))
								break;

							macroCount++;
							obj = (CMacroObject*)obj->m_Next;
						}
					}
				}

				if (obj != NULL)
					((CMacroObjectString*)obj)->String = g_EntryPointer->c_str();
			}
		}
	}
}
//----------------------------------------------------------------------------
void CGumpOptions::ApplyPageChanges()
{
	WISPFUN_DEBUG("c104_f26");
	switch (m_Page)
	{
		case 1: //Sound and Music
		{		
			//Меняем громкость звука эффектам и текущим эффектам
			g_ConfigManager.SoundVolume = g_OptionsConfig.SoundVolume;

			//Меняем громкость звука музыке и текущей музыке
			g_ConfigManager.MusicVolume = g_OptionsConfig.MusicVolume;
			
		    //Выключаем звук эффектов.
			g_ConfigManager.Sound = g_OptionsConfig.Sound;

			//Выключаем звук музыки.
			g_ConfigManager.Music = g_OptionsConfig.Music;

			g_ConfigManager.FootstepsSound = g_OptionsConfig.FootstepsSound;
			g_ConfigManager.CombatMusic = g_OptionsConfig.CombatMusic;

			break;
		}
		case 2: //Orion's configuration
		{
			g_ConfigManager.ClientFPS = g_OptionsConfig.ClientFPS;
			g_ConfigManager.UseScaling = g_OptionsConfig.UseScaling;
			g_ConfigManager.RemoveTextWithBlending = g_OptionsConfig.RemoveTextWithBlending;
			g_ConfigManager.DrawStatusState = g_OptionsConfig.DrawStatusState;
			g_ConfigManager.DrawStumps = g_OptionsConfig.DrawStumps;
			g_ConfigManager.MarkingCaves = g_OptionsConfig.MarkingCaves;
			g_ConfigManager.NoVegetation = g_OptionsConfig.NoVegetation;
			g_ConfigManager.NoAnimateFields = g_OptionsConfig.NoAnimateFields;
			g_ConfigManager.ConsoleNeedEnter = g_OptionsConfig.ConsoleNeedEnter;
			g_ConfigManager.ReduceFPSUnactiveWindow = g_OptionsConfig.ReduceFPSUnactiveWindow;
			g_ConfigManager.StandartCharactersAnimationDelay = g_OptionsConfig.StandartCharactersAnimationDelay;
			g_ConfigManager.StandartItemsAnimationDelay = g_OptionsConfig.StandartItemsAnimationDelay;
			g_ConfigManager.LockGumpsMoving = g_OptionsConfig.LockGumpsMoving;
			g_ConfigManager.HiddenCharactersRenderMode = g_OptionsConfig.HiddenCharactersRenderMode;
			g_ConfigManager.HiddenAlpha = g_OptionsConfig.HiddenAlpha;
			g_ConfigManager.UseHiddenModeOnlyForSelf = g_OptionsConfig.UseHiddenModeOnlyForSelf;
			g_ConfigManager.TransparentSpellIcons = g_OptionsConfig.TransparentSpellIcons;
			g_ConfigManager.SpellIconAlpha = g_OptionsConfig.SpellIconAlpha;
			g_ConfigManager.OldStyleStatusbar = g_OptionsConfig.OldStyleStatusbar;
			g_ConfigManager.OriginalPartyStatusbar = g_OptionsConfig.OriginalPartyStatusbar;
			g_ConfigManager.ApplyStateColorOnCharacters = g_OptionsConfig.ApplyStateColorOnCharacters;
			g_ConfigManager.ChangeFieldsGraphic = g_OptionsConfig.ChangeFieldsGraphic;
			g_ConfigManager.PaperdollSlots = g_OptionsConfig.PaperdollSlots;
			g_ConfigManager.DrawStatusConditionState = g_OptionsConfig.DrawStatusConditionState;
			g_ConfigManager.DrawStatusConditionValue = g_OptionsConfig.DrawStatusConditionValue;
			g_ConfigManager.RemoveStatusbarsWithoutObjects = g_OptionsConfig.RemoveStatusbarsWithoutObjects;
			g_ConfigManager.ShowDefaultConsoleEntryMode = g_OptionsConfig.ShowDefaultConsoleEntryMode;
			g_ConfigManager.DrawAuraState = g_OptionsConfig.DrawAuraState;
			g_ConfigManager.DrawAuraWithCtrlPressed = g_OptionsConfig.DrawAuraWithCtrlPressed;
			g_ConfigManager.ScreenshotFormat = g_OptionsConfig.ScreenshotFormat;
			g_ConfigManager.ScaleImagesInPaperdollSlots = g_OptionsConfig.ScaleImagesInPaperdollSlots;
			g_ConfigManager.RemoveOrCreateObjectsWithBlending = g_OptionsConfig.RemoveOrCreateObjectsWithBlending;
			g_ConfigManager.DrawHelmetsOnShroud = g_OptionsConfig.DrawHelmetsOnShroud;
			g_ConfigManager.UseGlobalMapLayer = g_OptionsConfig.UseGlobalMapLayer;
			g_ConfigManager.NoDrawRoofs = g_OptionsConfig.NoDrawRoofs;
			g_ConfigManager.HighlightTargetByType = g_OptionsConfig.HighlightTargetByType;
			g_ConfigManager.AutoDisplayWorldMap = g_OptionsConfig.AutoDisplayWorldMap;
			g_ConfigManager.UseGLListsForInterface = g_OptionsConfig.UseGLListsForInterface;
			g_DeveloperMode = g_OptionsDeveloperMode;

			break;
		}
		case 3: //Language
		{
			g_ConfigManager.UseToolTips = g_OptionsConfig.UseToolTips;
			g_ConfigManager.ToolTipsTextColor = g_OptionsConfig.ToolTipsTextColor;
			g_ConfigManager.ToolTipsTextFont = g_OptionsConfig.ToolTipsTextFont;
			g_ConfigManager.ToolTipsDelay = g_OptionsConfig.ToolTipsDelay;

			break;
		}
		case 4: //Chat
		{
			g_ConfigManager.ChatColorInputText = g_OptionsConfig.ChatColorInputText;
			g_ConfigManager.ChatColorMenuOption = g_OptionsConfig.ChatColorMenuOption;
			g_ConfigManager.ChatColorPlayerInMemberList = g_OptionsConfig.ChatColorPlayerInMemberList;
			g_ConfigManager.ChatColorText = g_OptionsConfig.ChatColorText;
			g_ConfigManager.ChatColorPlayerNameWithout = g_OptionsConfig.ChatColorPlayerNameWithout;
			g_ConfigManager.ChatColorMuted = g_OptionsConfig.ChatColorMuted;
			g_ConfigManager.ChatColorChannelModeratorName = g_OptionsConfig.ChatColorChannelModeratorName;
			g_ConfigManager.ChatColorChannelModeratorText = g_OptionsConfig.ChatColorChannelModeratorText;
			g_ConfigManager.ChatColorMyName = g_OptionsConfig.ChatColorMyName;
			g_ConfigManager.ChatColorMyText = g_OptionsConfig.ChatColorMyText;
			g_ConfigManager.ChatColorSystemMessage = g_OptionsConfig.ChatColorSystemMessage;
			g_ConfigManager.ChatColorBGOutputText = g_OptionsConfig.ChatColorBGOutputText;
			g_ConfigManager.ChatColorBGInputText = g_OptionsConfig.ChatColorBGInputText;
			g_ConfigManager.ChatColorBGUserList = g_OptionsConfig.ChatColorBGUserList;
			g_ConfigManager.ChatColorBGConfList = g_OptionsConfig.ChatColorBGConfList;
			g_ConfigManager.ChatColorBGCommandList = g_OptionsConfig.ChatColorBGCommandList;
			g_ConfigManager.ChatFont = g_OptionsConfig.ChatFont;

			break;
		}
		case 5: //Macro Options
		{
			g_MacroManager.LoadFromOptions();
			break;
		}
		case 6: //Interface
		{
			g_ConfigManager.EnablePathfind = g_OptionsConfig.EnablePathfind;
			g_ConfigManager.HoldTabForCombat = g_OptionsConfig.HoldTabForCombat;

			if (g_ConfigManager.OffsetInterfaceWindows != g_OptionsConfig.OffsetInterfaceWindows && g_OptionsConfig.OffsetInterfaceWindows)
				g_ContainerRect.MakeDefault();

			g_ConfigManager.OffsetInterfaceWindows = g_OptionsConfig.OffsetInterfaceWindows;
			g_ConfigManager.AutoArrange = g_OptionsConfig.AutoArrange;
			g_ConfigManager.AlwaysRun = g_OptionsConfig.AlwaysRun;
			g_ConfigManager.DisableMenubar = g_OptionsConfig.DisableMenubar;
			g_ConfigManager.GrayOutOfRangeObjects = g_OptionsConfig.GrayOutOfRangeObjects;
			g_ConfigManager.DisableNewTargetSystem = g_OptionsConfig.DisableNewTargetSystem;
			g_ConfigManager.ObjectHandles = g_OptionsConfig.ObjectHandles;
			g_ConfigManager.ItemPropertiesIcon = g_OptionsConfig.ItemPropertiesIcon;
			g_ConfigManager.HoldShiftForContextMenus = g_OptionsConfig.HoldShiftForContextMenus;
			g_ConfigManager.HoldShiftForEnablePathfind = g_OptionsConfig.HoldShiftForEnablePathfind;

			int curX = g_ContainerRect.DefaultX;

			if (m_ContainerOffsetX->m_Entry.Length())
				curX = atoi(m_ContainerOffsetX->m_Entry.c_str());

			int curY = g_ContainerRect.DefaultY;

			if (m_ContainerOffsetY->m_Entry.Length())
				curY = atoi(m_ContainerOffsetY->m_Entry.c_str());

			g_ContainerRect.DefaultX = curX;
			g_ContainerRect.DefaultY = curY;

			if (g_OptionsConfig.DisableMenubar)
				g_GumpManager.CloseGump(0, 0, GT_MENUBAR);
			else
				g_GumpManager.AddGump(new CGumpMenubar(0, 0));

			break;
		}
		case 7: //Display
		{
			int curX = 0;

			if (m_GameWindowWidth->m_Entry.Length())
				curX = atoi(m_GameWindowWidth->m_Entry.c_str());

			if (curX < 640)
				curX = 640;
			else if (curX > (g_OrionWindow.Size.Width - 20))
				curX = g_OrionWindow.Size.Width - 20;

			g_OptionsConfig.GameWindowWidth = curX;
			g_ConfigManager.GameWindowWidth = curX;
			
			int curY = 0;
			
			if (m_GameWindowHeight->m_Entry.Length())
				curY = atoi(m_GameWindowHeight->m_Entry.c_str());

			if (curY < 480)
				curY = 480;
			else if (curY > (g_OrionWindow.Size.Height - 40))
				curY = (g_OrionWindow.Size.Height - 40);

			g_OptionsConfig.GameWindowHeight = curY;
			g_ConfigManager.GameWindowHeight = curY;

			if (g_PacketManager.GetClientVersion() >= CV_200)
				CPacketGameWindowSize().Send();

			g_ConfigManager.SpeechDelay = g_OptionsConfig.SpeechDelay;
			g_ConfigManager.ScaleSpeechDelay = g_OptionsConfig.ScaleSpeechDelay;
			g_ConfigManager.SpeechColor = g_OptionsConfig.SpeechColor;
			g_ConfigManager.EmoteColor = g_OptionsConfig.EmoteColor;
			g_ConfigManager.PartyMessageColor = g_OptionsConfig.PartyMessageColor;
			g_ConfigManager.GuildMessageColor = g_OptionsConfig.GuildMessageColor;
			g_ConfigManager.AllianceMessageColor = g_OptionsConfig.AllianceMessageColor;
			g_ConfigManager.IgnoreGuildMessage = g_OptionsConfig.IgnoreGuildMessage;
			g_ConfigManager.IgnoreAllianceMessage = g_OptionsConfig.IgnoreAllianceMessage;
			g_ConfigManager.DarkNights = g_OptionsConfig.DarkNights;
			g_ConfigManager.ColoredLighting = g_OptionsConfig.ColoredLighting;
			g_ConfigManager.LockResizingGameWindow = g_OptionsConfig.LockResizingGameWindow;
			
			break;
		}
		case 8: //Reputation System
		{
			g_ConfigManager.InnocentColor = g_OptionsConfig.InnocentColor;
			g_ConfigManager.FriendlyColor = g_OptionsConfig.FriendlyColor;
			g_ConfigManager.SomeoneColor = g_OptionsConfig.SomeoneColor;
			g_ConfigManager.CriminalColor = g_OptionsConfig.CriminalColor;
			g_ConfigManager.EnemyColor = g_OptionsConfig.EnemyColor;
			g_ConfigManager.MurdererColor = g_OptionsConfig.MurdererColor;
			g_ConfigManager.CriminalActionsQuery = g_OptionsConfig.CriminalActionsQuery;

			break;
		}
		case 9: //Miscellaneous
		{
			g_ConfigManager.ShowIncomingNames = g_OptionsConfig.ShowIncomingNames;
			g_ConfigManager.UseCircleTrans = g_OptionsConfig.UseCircleTrans;
			g_ConfigManager.StatReport = g_OptionsConfig.StatReport;
			g_ConfigManager.CircleTransRadius = g_OptionsConfig.CircleTransRadius;
			g_ConfigManager.SkillReport = g_OptionsConfig.SkillReport;
			g_ConfigManager.SpeechFont = g_OptionsConfig.SpeechFont;

			g_CircleOfTransparency.Create(g_ConfigManager.CircleTransRadius);

			break;
		}
		case 10: //Filter Options
		{
			break;
		}
		default:
			break;
	}
}
//---------------------------------------------------------------------------
const char *CGumpOptions::m_HotkeyText[0x100] =
{
	"",         //0x00
	"Left Mouse",   //0x01
	"Right Mouse",  //0x02
	"Ctrl-Break",   //0x03
	"Middle Mouse", //0x04
	"0x05",         //0x05
	"0x06",         //0x06
	"0x07",         //0x07
	"Backspace",    //0x08
	"Tab",          //0x09
	"0x0a",         //0x0a
	"0x0b",         //0x0b
	"Clear",        //0x0c
	"Enter",        //0x0d
	"0x0e",         //0x0e
	"0x0f",         //0x0f
	"Shift",        //0x10
	"Ctrl",         //0x11
	"Alt",          //0x12
	"Pause",        //0x13
	"Caps Lock",    //0x14
	"0x15",         //0x15
	"0x16",         //0x16
	"0x17",         //0x17
	"0x18",         //0x18
	"0x19",         //0x19
	"0x1a",         //0x1a
	"Esc",          //0x1b
	"0x1c",         //0x1c
	"0x1d",         //0x1d
	"0x1e",         //0x1e
	"0x1f",         //0x1f
	"Space",        //0x20
	"Page Up",      //0x21
	"Page Down",    //0x22
	"End",          //0x23
	"Home",         //0x24
	"Left Arrow",   //0x25
	"Up Arrow",     //0x26
	"Right Arrow",  //0x27
	"Down Arrow",   //0x28
	"Select",       //0x29
	"OEM 2A",       //0x2a
	"Execute",      //0x2b
	"Print Screen", //0x2c
	"Ins",          //0x2d
	"Del",          //0x2e
	"Help",         //0x2f
	"0",            //0x30
	"1",            //0x31
	"2",            //0x32
	"3",            //0x33
	"4",            //0x34
	"5",            //0x35
	"6",            //0x36
	"7",            //0x37
	"8",            //0x38
	"9",            //0x39
	"0x3a",         //0x3a
	"0x3b",         //0x3b
	"0x3c",         //0x3c
	"0x3d",         //0x3d
	"0x3e",         //0x3e
	"0x3f",         //0x3f
	"0x40",         //0x40
	"A",            //0x41
	"B",            //0x42
	"C",            //0x43
	"D",            //0x44
	"E",            //0x45
	"F",            //0x46
	"G",            //0x47
	"H",            //0x48
	"I",            //0x49
	"J",            //0x4a
	"K",            //0x4b
	"L",            //0x4c
	"M",            //0x4d
	"N",            //0x4e
	"O",            //0x4f
	"P",            //0x50
	"Q",            //0x51
	"R",            //0x52
	"S",            //0x53
	"T",            //0x54
	"U",            //0x55
	"V",            //0x56
	"W",            //0x57
	"X",            //0x58
	"Y",            //0x59
	"Z",            //0x5a
	"Left Win",     //0x5b
	"Right Win",    //0x5c
	"Apps",         //0x5d
	"0x5e",         //0x5e
	"0x5f",         //0x5f
	"Num 0",        //0x60
	"Num 1",        //0x61
	"Num 2",        //0x62
	"Num 3",        //0x63
	"Num 4",        //0x64
	"Num 5",        //0x65
	"Num 6",        //0x66
	"Num 7",        //0x67
	"Num 8",        //0x68
	"Num 9",        //0x69
	"Num *",        //0x6a
	"Num +",        //0x6b
	"Separator",    //0x6c
	"Num -",        //0x6d
	"Num .",        //0x6e
	"Num /",        //0x6f
	"F1",           //0x70
	"F2",           //0x71
	"F3",           //0x72
	"F4",           //0x73
	"F5",           //0x74
	"F6",           //0x75
	"F7",           //0x76
	"F8",           //0x77
	"F9",           //0x78
	"F10",          //0x79
	"F11",          //0x7a
	"F12",          //0x7b
	"Wheel Click",	//"F13",	//0x7c
	"Wheel Up",		//"F14",	//0x7d
	"Wheel Down",	//"F15",	//0x7e
	"Mouse+ Forward",//"F16",	//0x7f
	"Mouse+ Back",	//"F17",	//0x80
	"F18",          //0x81
	"F19",          //0x82
	"F20",          //0x83
	"F21",          //0x84
	"F22",          //0x85
	"F23",          //0x86
	"F24",          //0x87
	"0x88",         //0x88
	"0x89",         //0x89
	"0x8a",         //0x8a
	"0x8b",         //0x8b
	"0x8c",         //0x8c
	"0x8d",         //0x8d
	"0x8e",         //0x8e
	"0x8f",         //0x8f
	"Num Lock",      //0x90
	"Scroll Lock",   //0x91
	"0x92",         //0x92
	"0x93",         //0x93
	"0x94",         //0x94
	"0x95",         //0x95
	"0x96",         //0x96
	"0x97",         //0x97
	"0x98",         //0x98
	"0x99",         //0x99
	"0x9a",         //0x9a
	"0x9b",         //0x9b
	"0x9c",         //0x9c
	"0x9d",         //0x9d
	"0x9e",         //0x9e
	"0x9f",         //0x9f
	"0xa0",         //0xa0
	"0xa1",         //0xa1
	"0xa2",         //0xa2
	"0xa3",         //0xa3
	"0xa4",         //0xa4
	"0xa5",         //0xa5
	"0xa6",         //0xa6
	"0xa7",         //0xa7
	"0xa8",         //0xa8
	"0xa9",         //0xa9
	"0xaa",         //0xaa
	"0xab",         //0xab
	"0xac",         //0xac
	"0xad",         //0xad
	"0xae",         //0xae
	"0xaf",         //0xaf
	"0xb0",         //0xb0
	"0xb1",         //0xb1
	"0xb2",         //0xb2
	"0xb3",         //0xb3
	"0xb4",         //0xb4
	"0xb5",         //0xb5
	"0xb6",         //0xb6
	"0xb7",         //0xb7
	"0xb8",         //0xb8
	"0xb9",         //0xb9
	";",            //0xba
	"=",            //0xbb
	",",            //0xbc
	"-",            //0xbd
	".",            //0xbe
	"/",            //0xbf
	"`",            //0xc0
	"0xc1",         //0xc1
	"0xc2",         //0xc2
	"0xc3",         //0xc3
	"0xc4",         //0xc4
	"0xc5",         //0xc5
	"0xc6",         //0xc6
	"0xc7",         //0xc7
	"0xc8",         //0xc8
	"0xc9",         //0xc9
	"0xca",         //0xca
	"0xcb",         //0xcb
	"0xcc",         //0xcc
	"0xcd",         //0xcd
	"0xce",         //0xce
	"0xcf",         //0xcf
	"0xd0",         //0xd0
	"0xd1",         //0xd1
	"0xd2",         //0xd2
	"0xd3",         //0xd3
	"0xd4",         //0xd4
	"0xd5",         //0xd5
	"0xd6",         //0xd6
	"0xd7",         //0xd7
	"0xd8",         //0xd8
	"0xd9",         //0xd9
	"0xda",         //0xda
	"[",            //0xdb
	"\\",            //0xdc
	"]",            //0xdd
	"'",            //0xde
	"OEM DF",       //0xdf
	"OEM E0",       //0xe0
	"OEM E1",       //0xe1
	"OEM E2",       //0x32
	"OEM E3",       //0xe3
	"OEM E4",       //0xe4
	"0xe5",         //0xe5
	"OEM E6",       //0xe6
	"0xe7",         //0xe7
	"0xe8",         //0xe8
	"OEM E9",       //0xe9
	"OEM EA",       //0xea
	"OEM EB",       //0xeb
	"OEM EC",       //0xec
	"OEM ED",       //0xed
	"OEM EE",       //0xee
	"OEM EF",       //0xef
	"OEM F0",       //0xf0
	"OEM F1",       //0xf1
	"OEM F2",       //0xf2
	"OEM F3",       //0xf3
	"OEM F4",       //0xf4
	"OEM F5",       //0xf5
	"Attn",         //0xf6
	"Crsel",        //0xf7
	"Exsel",        //0xf8
	"Erase EOF",    //0xf9
	"Play",         //0xfa
	"Zoom",         //0xfb
	"0xfc",         //0xfc
	"PA1",          //0xfd
	"Clear",        //0xfe
	"0xff"          //0xff
};
//----------------------------------------------------------------------------