/****************************************************************************
**
** GumpManager.cpp
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
#include "stdafx.h"

TGumpManager *GumpManager = NULL;
//---------------------------------------------------------------------------
void TGumpManager::PrepareTextures()
{
	TGump *gump = (TGump*)m_Items;

	while (gump != NULL)
	{
		gump->PrepareTextures();

		gump = (TGump*)gump->m_Next;
	}
}
//---------------------------------------------------------------------------
void TGumpManager::Draw(bool &mode, bool blocked)
{
	TGump *gump = (TGump*)m_Items;
	TGump *menuBarGump = NULL;

	while (gump != NULL)
	{
		TGump *next = (TGump*)gump->m_Next;

		if (gump->GumpType == GT_MENUBAR)
			menuBarGump = gump;
		else
		{
			if (mode)
			{
				if (blocked == gump->Blocked && gump->CanBeDisplayed())
					gump->Draw(mode);
			}
			else
			{
				if (blocked == gump->Blocked && gump->CanBeDisplayed())
				{
					if (gump->Draw(mode))
						g_LastSelectedGump = (DWORD)gump;
				}
			}
		}

		gump = next;
	}

	if (menuBarGump != NULL && menuBarGump->Draw(mode))
	{
		if (!mode)
			g_LastSelectedGump = (DWORD)menuBarGump;
	}
}
//---------------------------------------------------------------------------
void TGumpManager::RedrawAll()
{
	for (TGump *gump = (TGump*)m_Items; gump != NULL; gump = (TGump*)gump->m_Next)
		gump->FrameCreated = false;
}
//---------------------------------------------------------------------------
void TGumpManager::OnLeftMouseDown(bool blocked)
{
	TGump *gump = (TGump*)m_Items;

	if (g_LastObjectType == SOT_TEXT_OBJECT)
		gump = NULL;

	while (gump != NULL)
	{
		if (g_LastSelectedGump == (DWORD)gump)
		{
			if (gump->GumpType == GT_STATUSBAR && ((TGumpStatusbar*)gump)->InGroup())
				((TGumpStatusbar*)gump)->UpdateGroup(0, 0);
			else
				MoveToBack(gump);

			gump->OnLeftMouseDown();

			break;
		}

		gump = (TGump*)gump->m_Next;
	}
}
//---------------------------------------------------------------------------
bool TGumpManager::OnLeftMouseUp(bool blocked)
{
	TGump *gump = (TGump*)m_Items;

	if (g_LastObjectType == SOT_TEXT_OBJECT)
		gump = NULL;

	while (gump != NULL)
	{
		if (g_LastGumpLeftMouseDown == (DWORD)gump)
		{
			int LOLMD = g_LastObjectLeftMouseDown;
			
			switch (gump->GumpType)
			{
				case GT_PAPERDOLL:
				{
					if (g_LastObjectLeftMouseDown == TGumpPaperdoll::ID_GP_ITEMS + OL_BACKPACK)
						g_LastObjectLeftMouseDown = 0;
					break;
				}
				case GT_GENERIC:
				{
					if (g_LastObjectLeftMouseDown >= TGumpGeneric::ID_GG_HTML_TEXT)
						g_LastObjectLeftMouseDown = 0;
					break;
				}
				case GT_BULLETIN_BOARD_ITEM:
				{
					if (g_LastObjectLeftMouseDown == TGumpBulletinBoardItem::ID_GBBI_TEXT_FIELD)
						g_LastObjectLeftMouseDown = 0;
					break;
				}
				case GT_PROFILE:
				{
					if (g_LastObjectLeftMouseDown == TGumpProfile::ID_GP_TEXT_FIELD)
						g_LastObjectLeftMouseDown = 0;
					break;
				}
				default:
					break;
			}

			if (gump->CanBeMoved() && gump->GumpType == GT_MENU)
			{
				if (!g_LastObjectLeftMouseDown || g_LastObjectLeftMouseDown > 2)
				{
					int GX = gump->X + (g_MouseX - g_DroppedLeftMouseX);
					int GY = gump->Y + (g_MouseY - g_DroppedLeftMouseY);
					gump->X = GX;
					gump->Y = GY;
					gump->GenerateFrame(gump->X, gump->Y);
				}
			}
			else if (!g_LastObjectLeftMouseDown && gump->CanBeMoved() && !gump->NoMove && ObjectInHand == NULL)
			{
				int GX = (g_MouseX - g_DroppedLeftMouseX);
				int GY = (g_MouseY - g_DroppedLeftMouseY);

				if (gump->GumpType == GT_STATUSBAR)
				{
					TGumpStatusbar *sb = (TGumpStatusbar*)gump;

					if (!gump->Minimized)
					{
						sb->RemoveFromGroup();
						gump->AddX(GX);
						gump->AddY(GY);
					}
					else
					{
						gump->AddMinimizedX(GX);
						gump->AddMinimizedY(GY);

						if (sb->InGroup())
							sb->UpdateGroup(g_MouseX - g_DroppedLeftMouseX, g_MouseY - g_DroppedLeftMouseY);
						else
						{
							int testX = g_MouseX;
							int testY = g_MouseY;

							TGumpStatusbar *nearBar = sb->GetNearStatusbar(testX, testY);

							if (nearBar != NULL)
							{
								GX = testX;
								GY = testY;

								gump->MinimizedX = GX;
								gump->MinimizedY = GY;

								nearBar->AddStatusbar(sb);
							}
						}
					}
				}
				else if (gump->Minimized && gump->GumpType != GT_MINIMAP)
				{
					gump->AddMinimizedX(GX);
					gump->AddMinimizedY(GY);
				}
				else
				{
					gump->AddX(GX);
					gump->AddY(GY);
				}

				gump->UpdateFrame();
			}
			
			g_LastObjectLeftMouseDown = LOLMD;
			if (ObjectInHand != NULL)
			{
				if (g_LastSelectedGump == (DWORD)gump)
					gump->OnLeftMouseUp();
			}
			else
				gump->OnLeftMouseUp();

			//MoveGumpToTop(gump);
			return true;
		}
		else if (g_LastSelectedGump == (DWORD)gump && ObjectInHand != NULL && (gump->GumpType == GT_PAPERDOLL || gump->GumpType == GT_CONTAINER || gump->GumpType == GT_TRADE))
		{
			gump->OnLeftMouseUp();
			return true;
		}

		gump = (TGump*)gump->m_Next;
	}

	return false;
}
//---------------------------------------------------------------------------
void TGumpManager::OnRightMouseDown(bool blocked)
{
	TGump *gump = (TGump*)m_Items;
	
	if (g_LastObjectType == SOT_TEXT_OBJECT)
		gump = NULL;

	while (gump != NULL)
	{
		if (g_LastSelectedGump == (DWORD)gump)
		{
			g_LastSelectedGump = (DWORD)gump;

			if (gump->GumpType == GT_STATUSBAR && ((TGumpStatusbar*)gump)->InGroup())
				((TGumpStatusbar*)gump)->UpdateGroup(0, 0);
			else
				MoveToBack(gump);

			break;
		}

		gump = (TGump*)gump->m_Next;
	}
}
//---------------------------------------------------------------------------
void TGumpManager::OnRightMouseUp(bool blocked)
{
	TGump *gump = (TGump*)m_Items;
	
	if (g_LastObjectType == SOT_TEXT_OBJECT)
		gump = NULL;

	while (gump != NULL)
	{
		if (g_LastGumpRightMouseDown == (DWORD)gump && !gump->NoClose && gump->CanBeMoved())
		{
			//gump->OnClose();

			switch (gump->GumpType)
			{
				case GT_CONTAINER:
				{
					TGameItem *obj = World->FindWorldItem(gump->Serial);
					if (obj != NULL)
					{
						if (obj->IsCorpse())
						{
							for (TGameItem *gi = (TGameItem*)obj->m_Items; gi != NULL; )
							{
								TGameItem *next = (TGameItem*)gi->m_Next;

								if (gi->Layer == OL_NONE)
									World->RemoveObject(gi);

								gi = next;
							}
						}
						else
							obj->Clear();

						obj->Opened = false;
						CloseGump(gump->Serial, 0, GT_CONTAINER);
					}

					break;
				}
				case GT_MENUBAR:
				{
					gump->X = 0;
					gump->Y = 0;
					gump->GenerateFrame(0, 0);

					break;
				}
				case GT_MENU:
				{
					gump->SendMenuResponse(0);

					break;
				}
				case GT_TRADE:
				{
					gump->SendTradingResponse(1);

					break;
				}
				case GT_GENERIC:
				{
					gump->SendGumpResponse(0);

					break;
				}
				case GT_OPTIONS:
				{
					OptionsMacroManager->Clear();

					CloseGump(gump->Serial, gump->ID, gump->GumpType);

					break;
				}
				case GT_STATUSBAR:
				{
					TGumpStatusbar *sb = ((TGumpStatusbar*)gump)->GetTopStatusbar();

					if (sb != NULL)
					{
						while (sb != NULL)
						{
							TGumpStatusbar *next = sb->m_GroupNext;

							sb->RemoveFromGroup();
							RemoveGump(sb);

							sb = next;
						}
					}
					else
						RemoveGump(gump);

					break;
				}
				default:
				{
					CloseGump(gump->Serial, gump->ID, gump->GumpType);

					break;
				}
			}

			break;
		}

		gump = (TGump*)gump->m_Next;
	}
}
//---------------------------------------------------------------------------
bool TGumpManager::OnLeftMouseDoubleClick(bool blocked)
{
	bool result = false;
	TGump *gump = (TGump*)m_Items;
	
	if (g_LastObjectType == SOT_TEXT_OBJECT)
		gump = NULL;

	while (gump != NULL)
	{
		if (g_LastSelectedGump == (DWORD)gump)
		{
			g_LastObjectLeftMouseDown = g_LastSelectedObject;

			if (gump->OnLeftMouseDoubleClick())
				result = true;

			//MoveGumpToTop(gump);
			break;
		}

		gump = (TGump*)gump->m_Next;
	}

	return result;
}
//---------------------------------------------------------------------------
bool TGumpManager::OnRightMouseDoubleClick(bool blocked)
{
	return false;
}
//---------------------------------------------------------------------------
void TGumpManager::OnMouseWheel(MOUSE_WHEEL_STATE state, bool blocked)
{
	TGump *gump = (TGump*)m_Items;

	while (gump != NULL)
	{
		if (g_LastSelectedGump == (DWORD)gump)
		{
			gump->OnMouseWheel(state);

			break;
		}

		gump = (TGump*)gump->m_Next;
	}
}
//---------------------------------------------------------------------------
bool TGumpManager::OnCharPress(WPARAM wparam, LPARAM lparam, bool blocked)
{
	TGump *gump = GetTextEntryOwner();
	bool result = false;

	if (gump != NULL)
	{
		if (g_GameState == GS_GAME_BLOCKED)
		{
			if (gump->GumpType == GT_TEXT_ENTRY_DIALOG)
			{
				gump->OnCharPress(wparam, lparam);

				result = true;
			}
		}
		else
		{
			gump->OnCharPress(wparam, lparam);

			result = true;
		}
	}

	return result;
}
//---------------------------------------------------------------------------
bool TGumpManager::OnKeyPress(WPARAM wparam, LPARAM lparam, bool blocked)
{
	bool result = false;

	if (EntryPointer != NULL && EntryPointer != GameConsole)
	{
		TGump *gump = GetTextEntryOwner();

		if (gump != NULL)
		{
			switch (gump->GumpType)
			{
				case GT_STATUSBAR:
				case GT_DRAG:
				case GT_OPTIONS:
				case GT_SKILLS:
				case GT_PROFILE:
				case GT_BULLETIN_BOARD_ITEM:
				{
					gump->OnKeyPress(wparam, lparam);

					result = true;

					break;
				}
			}
		}
		else if (wparam == VK_DELETE)
		{
			gump = GetGump(g_PlayerSerial, 0, GT_SKILLS);
			if (gump != NULL)
				gump->OnKeyPress(wparam, lparam);
		}
	}
	else if (wparam == VK_DELETE)
	{
		TGump *gump = GetGump(g_PlayerSerial, 0, GT_SKILLS);
		if (gump != NULL)
			gump->OnKeyPress(wparam, lparam);
	}

	return result;
}
//---------------------------------------------------------------------------
int TGumpManager::GetNonpartyStatusbarsCount()
{
	TGump *gump = (TGump*)m_Items;
	int count = 0;

	while (gump != NULL)
	{
		if (gump->GumpType == GT_STATUSBAR && gump->Serial != g_PlayerSerial && !Party.Contains(gump->Serial))
			count++;

		gump = (TGump*)gump->m_Next;
	}

	return count;
}
//---------------------------------------------------------------------------
void TGumpManager::AddGump(TGump *obj)
{
	if (m_Items == NULL)
	{
		m_Items = obj;
		obj->m_Next = NULL;
		obj->m_Prev = NULL;
	}
	else
	{
		TGump *gump = (TGump*)m_Items;

		bool canCheck = (obj->GumpType != GT_GENERIC && obj->GumpType != GT_MENU && obj->GumpType != GT_TRADE && obj->GumpType != GT_BULLETIN_BOARD_ITEM);

		while (gump != NULL)
		{
			GUMP_TYPE gumpType = gump->GumpType;

			if (gumpType == obj->GumpType && canCheck)
			{
				if (gump->Serial == obj->Serial)
				{
					if (gump->CanBeMoved())
					{
						if (gumpType == GT_SPELL || gumpType == GT_DRAG)
						{
							gump->X = obj->X;
							gump->Y = obj->Y;
						}
						else if (gumpType == GT_STATUSBAR)
						{
							TGumpStatusbar *sb = (TGumpStatusbar*)gump;

							int gx = obj->X;
							int gy = obj->Y;
						
							if (obj->Minimized)
							{
								gx = obj->MinimizedX;
								gy = obj->MinimizedY;
							}
						
							if (sb->InGroup())
								sb->UpdateGroup(-(gump->MinimizedX - gx), -(gump->MinimizedY - gy));

							if (gump->Minimized)
							{
								gump->MinimizedX = gx;
								gump->MinimizedY = gy;
							}
							else
							{
								gump->X = gx;
								gump->Y = gy;
							}
						}
					}

					if (gumpType == GT_WORLD_MAP && !((TGumpWorldMap*)gump)->Called)
						((TGumpWorldMap*)gump)->Called = ((TGumpWorldMap*)obj)->Called;

					obj->m_Next = NULL;
					delete obj;

					if (gumpType == GT_CONTAINER || gumpType == GT_JOURNAL || gumpType == GT_SKILLS)
						gump->Minimized = false;
					else if (gumpType == GT_MINIMAP)
					{
						gump->Minimized = !gump->Minimized;
						((TGumpMinimap*)gump)->LastX = 0;
					}

					MoveToBack(gump);

					gump->UpdateFrame();

					return;
				}
			}

			if (gump->m_Next == NULL)
			{
				gump->m_Next = obj;
				obj->m_Prev = gump;
				obj->m_Next = NULL;

				break;
			}

			gump = (TGump*)gump->m_Next;
		}
	}

	if (obj->GumpType == GT_STATUSBAR && !Party.Contains(obj->Serial) && GetNonpartyStatusbarsCount() > 10)
	{
		//RemoveGump(obj);

		TGump *gump = (TGump*)m_Items;

		while (gump != NULL)
		{
			if (gump->GumpType == GT_STATUSBAR && gump->Serial != g_PlayerSerial && !Party.Contains(gump->Serial))
			{
				RemoveGump(gump);

				UO->CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "You have too many non-party status bars");

				break;
			}

			gump = (TGump*)gump->m_Next;
		}

		obj->PrepareTextures();
	}
	else
	{
		switch (obj->GumpType)
		{
			case GT_CONTAINER:
			{
				WORD sound = g_ContainerOffset[obj->Graphic].OpenSound;

				if (sound)
					UO->PlaySoundEffect(sound);

				break;
			}
			case GT_SPELLBOOK:
			{
				//UO->PlaySoundEffect(0x????);
				break;
			}
			case GT_SKILLS:
			{
				((TGumpSkills*)obj)->Init();
				break;
			}
			case GT_OPTIONS:
			{
				((TGumpOptions*)obj)->Init();
				break;
			}
			default:
				break;
		}

		obj->PrepareTextures();
	}
}
//---------------------------------------------------------------------------
TGump *TGumpManager::GetTextEntryOwner()
{
	TGump *gump = (TGump*)m_Items;

	while (gump != NULL)
	{
		if (gump->EntryPointerHere())
			break;

		gump = (TGump*)gump->m_Next;
	}

	return gump;
}
//---------------------------------------------------------------------------
TGump *TGumpManager::GumpExists(DWORD gumpID)
{
	TGump *gump = (TGump*)m_Items;

	while (gump != NULL)
	{
		if (gumpID == (DWORD)gump)
			break;

		gump = (TGump*)gump->m_Next;
	}

	return gump;
}
//---------------------------------------------------------------------------
TGump *TGumpManager::GetGump(DWORD serial, DWORD id, GUMP_TYPE type)
{
	TGump *gump = (TGump*)m_Items;

	while (gump != NULL)
	{
		if (gump->GumpType == type)
		{
			if (type == GT_SHOP)
				break;
			else if (type == GT_TRADE)
			{
				if (gump->Serial == serial)
					break;
				else if (!serial && (gump->ID == id || ((TGumpSecureTrading*)gump)->ID2 == id))
					break;
			}
			else if (gump->Serial == serial)
			{
				if (type == GT_CONTAINER || type == GT_SPELLBOOK || type == GT_BULLETIN_BOARD_ITEM)
					break;
				else if (gump->ID == id)
					break;
			}
		}

		gump = (TGump*)gump->m_Next;
	}

	return gump;
}
//---------------------------------------------------------------------------
void TGumpManager::CloseGump(DWORD serial, DWORD id, GUMP_TYPE type)
{
	for (TGump *gump = (TGump*)m_Items; gump != NULL; )
	{
		TGump *next = (TGump*)gump->m_Next;

		if (gump->GumpType == type)
		{
			if (type == GT_GENERIC && gump->Serial == serial && gump->ID == id)
				RemoveGump(gump);
			else if (type == GT_TRADE && gump->ID == serial)
				RemoveGump(gump);
			else if (type == GT_BULLETIN_BOARD_ITEM && gump->ID == id)
			{
				if (serial == 0xFFFFFFFF || gump->Serial == serial)
					RemoveGump(gump);
			}
			else if (type == GT_SHOP || type == GT_SPELLBOOK)
			{
				if (serial)
				{
					if (serial == gump->Serial)
						RemoveGump(gump);
				}
				else
					RemoveGump(gump);
			}
			else if (gump->Serial == serial)
				RemoveGump(gump);
		}

		gump = next;
	}
}
//---------------------------------------------------------------------------
void TGumpManager::RemoveGump(TGump *obj)
{
	Unlink(obj);

	if (World != NULL)
	{
		TGameItem *selobj = World->FindWorldItem(obj->Serial);
		if (selobj != NULL)
		{
			selobj->Dragged = false;
			selobj->Opened = false;
		}
	}

	obj->m_Next = NULL;
	obj->m_Prev = NULL;
	delete obj;
}
//--------------------------------------------------------------------------
void TGumpManager::OnDelete()
{
	TGump *gump = (TGump*)m_Items;

	while (gump != NULL)
	{
		TGump *tmp = (TGump*)gump->m_Next;

		switch (gump->GumpType)
		{
			case GT_STATUSBAR:
			{
				RemoveGump(gump);

				break;
			}
			default:
				break;
		}

		gump = tmp;
	}
}
//--------------------------------------------------------------------------
void TGumpManager::RemoveRangedGumps()
{
	if (World != NULL)
	{
		TGump *gump = (TGump*)m_Items;

		while (gump != NULL)
		{
			TGump *tmp = (TGump*)gump->m_Next;

			switch (gump->GumpType)
			{
				//case GT_STATUSBAR:
				case GT_PAPERDOLL:
				case GT_MAP:
				case GT_SPELLBOOK:
				case GT_DRAG:
				case GT_TRADE:
				case GT_SHOP:
				{
					if (World->FindWorldObject(gump->Serial) == NULL)
						RemoveGump(gump); //Èëè CloseGump() ?
					break;
				}
				case GT_CONTAINER:
				{
					if (GetTopObjDistance(g_Player, World->FindWorldObject(gump->Serial)) > REMOVE_CONTAINER_GUMP_RANGE)
						RemoveGump(gump);
					break;
				}
				default:
					break;
			}

			gump = tmp;
		}
	}
}
//--------------------------------------------------------------------------
void TGumpManager::Load(string path)
{
	TMappedHeader file;
	memset(&file, 0, sizeof(TMappedHeader));

	FileManager.LoadFileToMemory(file, path.c_str());
	
	bool paperdollRequested = false;
	bool menubarFound = false;

	if (file.Size)
	{
		BYTE version = file.ReadByte();

		PBYTE oldPtr = file.Ptr;
		file.Ptr = (PBYTE)file.Address + (file.Size - 6);
		short count = file.ReadShort();
		file.Ptr = oldPtr;
		bool menubarLoaded = false;

		IFOR(i, 0, count)
		{
			TGump *gump = NULL;

			PBYTE next = file.Ptr;
			BYTE size = file.ReadByte();
			next += size;

			GUMP_TYPE gumpType = (GUMP_TYPE)file.ReadByte();
			WORD gumpX = file.ReadWord();
			WORD gumpY = file.ReadWord();
			BYTE gumpMinimized = file.ReadByte();
			WORD gumpMinimizedX = file.ReadWord();
			WORD gumpMinimizedY = file.ReadWord();
			BYTE gumpLockMoving = file.ReadByte();

			switch ((GUMP_TYPE)gumpType)
			{
				case GT_PAPERDOLL:
				{
					gump = new TGumpPaperdoll(g_PlayerSerial, gumpX, gumpY, gumpMinimized);
							
					UO->PaperdollReq(g_PlayerSerial);
					paperdollRequested = true;

					break;
				}
				case GT_STATUSBAR:
				{
					gump = new TGumpStatusbar(g_PlayerSerial, gumpX, gumpY, gumpMinimized);
					break;
				}
				case GT_MINIMAP:
				{
					gump = new TGumpMinimap(g_PlayerSerial, gumpX, gumpY, gumpMinimized);
					break;
				}
				case GT_SKILLS:
				{
					gump = new TGumpSkills(g_PlayerSerial, gumpX, gumpY, gumpMinimized);

					short height = file.ReadShort();
					((TGumpSkills*)gump)->SetHeight(height);

					break;
				}
				case GT_JOURNAL:
				{
					gump = new TGumpJournal(g_PlayerSerial, gumpX, gumpY, gumpMinimized);
					
					short height = file.ReadShort();
					((TGumpJournal*)gump)->SetHeight(height);

					break;
				}
				case GT_WORLD_MAP:
				{
					TGumpWorldMap *wmg = new TGumpWorldMap(g_PlayerSerial, gumpX, gumpY);
					gump = wmg;

					wmg->Minimized = gumpMinimized;

					wmg->Map = file.ReadByte();
					wmg->Scale = file.ReadByte();
					wmg->LinkWithPlayer = file.ReadByte();

					wmg->Width = file.ReadShort();
					wmg->Height = file.ReadShort();

					wmg->OffsetX = file.ReadShort();
					wmg->OffsetY = file.ReadShort();

					break;
				}
				case GT_MENUBAR:
				case GT_BUFF:
				{
					if (gumpType == GT_BUFF)
					{
						//gump = new TGumpBuff(g_PlayerSerial, gumpX, gumpY);
					}
					else if (!ConfigManager.DisableMenubar)
					{
						menubarFound = true;
						gump = new TGumpMenubar(g_PlayerSerial, gumpX, gumpY);
						((TGumpMenubar*)gump)->Opened = gumpMinimized;
					}

					break;
				}
				case GT_CONTAINER:
				case GT_SPELLBOOK:
				case GT_SPELL:
				{
					DWORD serial = file.ReadDWord();

					if (gumpType == GT_SPELL)
					{
						WORD graphic = file.ReadWord();
					
						gump = new TGumpSpell(serial, gumpX, gumpY, graphic);
					}
					else
					{
						TContainerStack *cs = new TContainerStack(serial, gumpX, gumpY, gumpMinimizedX, gumpMinimizedY, gumpMinimized, gumpLockMoving);

						if (ContainerStack != NULL)
							ContainerStack->m_Prev = cs;

						cs->m_Next = ContainerStack;
						cs->m_Prev = NULL;

						ContainerStack = cs;

						UO->DoubleClick(serial);
					}

					break;
				}
				default:
					break;
			}

			if (gump != NULL)
			{
				if (gumpType == GT_MENUBAR)
				{
					if (menubarLoaded)
					{
						delete gump;
						continue;
					}

					menubarLoaded = true;
				}

				gump->MinimizedX = gumpMinimizedX;
				gump->MinimizedY = gumpMinimizedY;
				gump->LockMoving = gumpLockMoving;

				GumpManager->AddGump(gump);

				gump->UpdateFrame();
			}

			file.Ptr = next;
		}

		FileManager.UnloadFileFromMemory(file);
	}

	if (!ConfigManager.DisableMenubar && !menubarFound)
	{
		TGumpMenubar *mbg = new TGumpMenubar(g_PlayerSerial, 0, 0);
		mbg->Opened = true;
		GumpManager->AddGump(mbg);
	}

	if (!paperdollRequested)
		UO->PaperdollReq(g_PlayerSerial);
}
//--------------------------------------------------------------------------
void TGumpManager::Save(string path)
{
	TFileWriter *writer = new TFileWriter(path, true);

	writer->WriteByte(0); //version
	writer->WriteBuffer();

	short count = 0;

	TGump *gump = (TGump*)m_Items;

	while (gump != NULL)
	{
		switch (gump->GumpType)
		{
			case GT_PAPERDOLL:
			case GT_STATUSBAR:
			case GT_MINIMAP:
			case GT_SKILLS:
			case GT_JOURNAL:
			case GT_WORLD_MAP:
			{
				DWORD serial = gump->Serial;

				if (serial != g_PlayerSerial)
					break;
				
				BYTE size = 12;
				
				if (gump->GumpType == GT_JOURNAL)
					size += 2;
				else if (gump->GumpType == GT_SKILLS)
					size += 2;
				else if (gump->GumpType == GT_WORLD_MAP)
					size += 11;

				writer->WriteByte(size);
				writer->WriteByte(gump->GumpType);
				writer->WriteWord(gump->X);
				writer->WriteWord(gump->Y);
				writer->WriteByte(gump->Minimized);
				writer->WriteWord(gump->MinimizedX);
				writer->WriteWord(gump->MinimizedY);
				writer->WriteByte(gump->LockMoving);

				if (gump->GumpType == GT_JOURNAL)
					writer->WriteWord(((TGumpJournal*)gump)->GetHeight());
				else if (gump->GumpType == GT_SKILLS)
					writer->WriteWord(((TGumpSkills*)gump)->GetHeight());
				else if (gump->GumpType == GT_WORLD_MAP)
				{
					TGumpWorldMap *wmg = (TGumpWorldMap*)gump;
					
					writer->WriteByte(wmg->Map);
					writer->WriteByte(wmg->Scale);
					writer->WriteByte(wmg->LinkWithPlayer);

					writer->WriteShort(wmg->Width);
					writer->WriteShort(wmg->Height);

					writer->WriteShort(wmg->OffsetX);
					writer->WriteShort(wmg->OffsetY);
				}

				writer->WriteBuffer();
				count++;

				break;
			}
			case GT_MENUBAR:
			case GT_BUFF:
			{
				writer->WriteByte(12); //size
				writer->WriteByte(gump->GumpType);
				writer->WriteWord(gump->X);
				writer->WriteWord(gump->Y);

				if (gump->GumpType == GT_MENUBAR)
					writer->WriteByte(((TGumpMenubar*)gump)->Opened);
				else //buff
					writer->WriteByte(gump->Minimized);
				
				writer->WriteWord(gump->MinimizedX);
				writer->WriteWord(gump->MinimizedY);
				writer->WriteByte(gump->LockMoving);

				writer->WriteBuffer();
				count++;

				break;
			}
			case GT_CONTAINER:
			case GT_SPELLBOOK:
			case GT_SPELL:
			{
				int size = 16;

				if (gump->GumpType != GT_SPELL)
				{
					TGameObject *topobj = World->FindWorldObject(gump->Serial);
					if (topobj == NULL)
						break;

					topobj = topobj->GetTopObject();
					if (topobj->Serial != g_PlayerSerial)
						break;
				}
				else
					size += 2;
				
				writer->WriteByte(size); //size
				writer->WriteByte(gump->GumpType);
				writer->WriteWord(gump->X);
				writer->WriteWord(gump->Y);
				writer->WriteByte(gump->Minimized);
				writer->WriteWord(gump->MinimizedX);
				writer->WriteWord(gump->MinimizedY);
				writer->WriteByte(gump->LockMoving);
				writer->WriteDWord(gump->Serial);
				
				if (gump->GumpType == GT_SPELL)
					writer->WriteWord(gump->Graphic);

				writer->WriteBuffer();
				count++;

				break;
			}
			default:
				break;
		}

		gump = (TGump*)gump->m_Next;
	}
	
	writer->WriteShort(count); //EOF
	writer->WriteDWord(0); //EOF
	writer->WriteBuffer();

	delete writer;
}
//---------------------------------------------------------------------------