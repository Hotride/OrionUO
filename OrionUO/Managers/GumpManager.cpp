/***********************************************************************************
**
** GumpManager.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GumpManager.h"
#include "../SelectedObject.h"
#include "../PressedObject.h"
#include "../Party.h"
#include "../OrionUO.h"
#include "../Container.h"
#include "../TextEngine/GameConsole.h"
#include "../Game objects/GameWorld.h"
#include "../Game objects/GamePlayer.h"
#include "../Game objects/ObjectOnCursor.h"
#include "MouseManager.h"
#include "ConfigManager.h"
#include "OptionsMacroManager.h"
#include "PacketManager.h"
#include "../Wisp/WispMappedFile.h"
#include "../Wisp/WispBinaryFileWritter.h"
#include "../Container.h"
#include "../Gumps/GumpConsoleType.h"
#include "../Gumps/GumpSpell.h"
#include "../Gumps/GumpMenubar.h"
#include "../Gumps/GumpPaperdoll.h"
#include "../Gumps/GumpMinimap.h"
#include "../Gumps/GumpPopupMenu.h"
#include "../Gumps/GumpStatusbar.h"
#include "../Gumps/GumpMenu.h"
#include "../Gumps/GumpGeneric.h"
#include "../Gumps/GumpBuff.h"
#include "../Gumps/GumpJournal.h"
#include "../Gumps/GumpOptions.h"
#include "../Gumps/GumpSkills.h"
#include "../Gumps/GumpBook.h"
#include "../Gumps/GumpSecureTrading.h"
#include "../Gumps/GumpWorldMap.h"
#include "../Gumps/GumpSkill.h"
//----------------------------------------------------------------------------------
CGumpManager g_GumpManager;
//----------------------------------------------------------------------------------
/*!
Вычислить количество статусбаров без пати
@return Количество non-party статусбыров
*/
int CGumpManager::GetNonpartyStatusbarsCount()
{
	int count = 0;

	QFOR(gump, m_Items, CGump*)
	{
		if (gump->GumpType == GT_STATUSBAR && gump->Serial != g_PlayerSerial && !g_Party.Contains(gump->Serial))
			count++;
	}

	return count;
}
//----------------------------------------------------------------------------------
/*!
Добавить гамп
@param [__in] obj Ссылка на гамп
@return 
*/
void CGumpManager::AddGump(CGump *obj)
{
	if (m_Items == NULL)
	{
		m_Items = obj;
		obj->m_Next = NULL;
		obj->m_Prev = NULL;
	}
	else
	{
		bool canCheck = (obj->GumpType != GT_GENERIC && obj->GumpType != GT_MENU && obj->GumpType != GT_TRADE && obj->GumpType != GT_BULLETIN_BOARD_ITEM);

		QFOR(gump, m_Items, CGump*)
		{
			GUMP_TYPE gumpType = gump->GumpType;

			if (gumpType == obj->GumpType && canCheck)
			{
				if (gump->Serial == obj->Serial)
				{
					if (gump->CanBeMoved())
					{
						if (gumpType == GT_DRAG)
						{
							gump->X = obj->X;
							gump->Y = obj->Y;
						}
						else if (gumpType == GT_STATUSBAR)
						{
							CGumpStatusbar *sb = (CGumpStatusbar*)gump;

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
						else if (gumpType == GT_SPELL)
						{
							CGumpSpell *spell = (CGumpSpell*)gump;

							int gx = obj->X;
							int gy = obj->Y;

							if (spell->InGroup())
								spell->UpdateGroup(-(gump->X - gx), -(gump->Y - gy));

							gump->X = gx;
							gump->Y = gy;
						}
					}

					obj->m_Next = NULL;
					delete obj;

					if (gumpType == GT_WORLD_MAP && !((CGumpWorldMap*)gump)->Called)
						((CGumpWorldMap*)gump)->Called = ((CGumpWorldMap*)obj)->Called;
					else if (gump->GumpType == GT_POPUP_MENU)
						g_PopupMenu = (CGumpPopupMenu*)gump;
					else if (gumpType == GT_CONTAINER || gumpType == GT_JOURNAL || gumpType == GT_SKILLS)
					{
						gump->Minimized = false;
						gump->Page = 2;
					}
					else if (gumpType == GT_MINIMAP)
					{
						gump->Minimized = !gump->Minimized;
						((CGumpMinimap*)gump)->LastX = 0;
					}
					else if (gumpType == GT_SPELLBOOK && gump->Minimized)
					{
						gump->Minimized = false;
						gump->WantUpdateContent = true;
					}

					MoveToBack(gump);

					gump->FrameCreated = false;

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
		}
	}

	if (obj->GumpType == GT_STATUSBAR && !g_Party.Contains(obj->Serial) && GetNonpartyStatusbarsCount() > 10)
	{
		//RemoveGump(obj);

		QFOR(gump, m_Items, CGump*)
		{
			if (gump->GumpType == GT_STATUSBAR && gump->Serial != g_PlayerSerial && !g_Party.Contains(gump->Serial))
			{
				RemoveGump(gump);

				g_Orion.CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "You have too many non-party status bars");

				break;
			}
		}

		obj->PrepareTextures();
	}
	else
	{
		switch (obj->GumpType)
		{
			case GT_CONTAINER:
			{
				ushort sound = g_ContainerOffset[obj->Graphic].OpenSound;

				if (sound)
					g_Orion.PlaySoundEffect(sound);

				break;
			}
			case GT_SPELLBOOK:
			{
				//UO->PlaySoundEffect(0x????);
				break;
			}
			case GT_SKILLS:
			{
				((CGumpSkills*)obj)->Init();
				break;
			}
			case GT_OPTIONS:
			{
				((CGumpOptions*)obj)->Init();
				break;
			}
			default:
				break;
		}

		obj->PrepareTextures();
	}
}
//----------------------------------------------------------------------------------
/*!
Получить гамп-владелец текущей активной TEntryText
@return Ссылку на гамп или NULL
*/
CGump *CGumpManager::GetTextEntryOwner()
{
	QFOR(gump, m_Items, CGump*)
	{
		if (gump->EntryPointerHere())
			return gump;
	}

	return NULL;
}
//----------------------------------------------------------------------------------
/*!
Проверить, существует ли гамп
@param [__in] gumpID ID гампа (в памяти)
@return
*/
CGump *CGumpManager::GumpExists(uint gumpID)
{
	QFOR(gump, m_Items, CGump*)
	{
		if (gumpID == (uint)gump)
			return gump;
	}

	return NULL;
}
//----------------------------------------------------------------------------------
/*!
Обновить содержимое гампа
@param [__in] serial Серийник гампа
@param [__in] ID ID гампа
@param [__in] Type Тип гампа
@return Ссылку на обновленный гамп или NULL
*/
CGump *CGumpManager::UpdateContent(uint serial, uint id, GUMP_TYPE type)
{
	CGump *gump = GetGump(serial, id, type);

	if (gump != NULL)
		gump->WantUpdateContent = true;

	return gump;
}
//----------------------------------------------------------------------------------
/*!
Обновить гамп
@param [__in] serial Серийник гампа
@param [__in] ID ID гампа
@param [__in] Type Тип гампа
@return Ссылку на обновленный гамп или NULL
*/
CGump *CGumpManager::UpdateGump(uint serial, uint id, GUMP_TYPE type)
{
	CGump *gump = GetGump(serial, id, type);

	if (gump != NULL)
		gump->WantRedraw = true;

	return gump;
}
//----------------------------------------------------------------------------------
/*!
Найти гамп
@param [__in] serial Серийник гампа
@param [__in] ID ID гампа
@param [__in] Type Тип гампа
@return Ссылку на гамп или NULL
*/
CGump *CGumpManager::GetGump(uint serial, uint id, GUMP_TYPE type)
{
	CGump *gump = (CGump*)m_Items;

	while (gump != NULL)
	{
		if (gump->GumpType == type)
		{
			if (type == GT_SHOP || type == GT_TARGET_SYSTEM)
				break;
			else if (type == GT_TRADE)
			{
				if (gump->Serial == serial)
					break;
				else if (!serial && (gump->ID == id || ((CGumpSecureTrading*)gump)->ID2 == id))
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

		gump = (CGump*)gump->m_Next;
	}

	return gump;
}
//----------------------------------------------------------------------------------
/*!
Закрыть все гампы с указанными параметрами
@param [__in] serial Серийник гампа
@param [__in] ID ID гампа
@param [__in] Type Тип гампа
@return
*/
void CGumpManager::CloseGump(uint serial, uint id, GUMP_TYPE type)
{
	for (CGump *gump = (CGump*)m_Items; gump != NULL; )
	{
		CGump *next = (CGump*)gump->m_Next;

		if (gump->GumpType == type)
		{
			if (type == GT_TARGET_SYSTEM)
				RemoveGump(gump);
			else if (type == GT_GENERIC && gump->Serial == serial && gump->ID == id)
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
//----------------------------------------------------------------------------------
/*!
Удалить гамп
@param [__in] obj Ссылка на гамп
@return
*/
void CGumpManager::RemoveGump(CGump *obj)
{
	Unlink(obj);

	if (g_World != NULL)
	{
		CGameItem *selobj = g_World->FindWorldItem(obj->Serial);

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
//----------------------------------------------------------------------------------
/*!
Событие удаления менеджера (вызывается перед удалением)
@return 
*/
void CGumpManager::OnDelete()
{
	CGump *gump = (CGump*)m_Items;

	while (gump != NULL)
	{
		CGump *tmp = (CGump*)gump->m_Next;

		if (gump->GumpType == GT_STATUSBAR)
			RemoveGump(gump);

		gump = tmp;
	}
}
//----------------------------------------------------------------------------------
/*!
Удалить гампы, которые не могут быть досягаемы из-за изменения дистанции до объекта
@return 
*/
void CGumpManager::RemoveRangedGumps()
{
	if (g_World != NULL)
	{
		CGump *gump = (CGump*)m_Items;

		while (gump != NULL)
		{
			CGump *tmp = (CGump*)gump->m_Next;

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
					if (g_World->FindWorldObject(gump->Serial) == NULL)
						RemoveGump(gump); //Или CloseGump() ?
					break;
				}
				case GT_CONTAINER:
				{
					if (GetTopObjDistance(g_Player, g_World->FindWorldObject(gump->Serial)) > REMOVE_CONTAINER_GUMP_RANGE)
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
//----------------------------------------------------------------------------------
void CGumpManager::PrepareContent()
{
	for (CGump* gump = (CGump*)m_Items; gump != NULL;)
	{
		CGump* next = (CGump*)gump->m_Next;

		if (!gump->RemoveMark)
			gump->PrepareContent();

		if (gump->RemoveMark)
			RemoveGump(gump);

		gump = next;
	}
}
//----------------------------------------------------------------------------------
void CGumpManager::RemoveMarked()
{
	for (CGump* gump = (CGump*)m_Items; gump != NULL;)
	{
		CGump* next = (CGump*)gump->m_Next;

		if (gump->RemoveMark)
			RemoveGump(gump);

		gump = next;
	}
}
//----------------------------------------------------------------------------------
/*!
Подготовка текстур
@return 
*/
void CGumpManager::PrepareTextures()
{
	QFOR(gump, m_Items, CGump*)
		gump->PrepareTextures();
}
//----------------------------------------------------------------------------------
void CGumpManager::Draw(const bool &blocked)
{
	CGump *gump = (CGump*)m_Items;
	CGump *menuBarGump = NULL;

	while (gump != NULL)
	{
		CGump *next = (CGump*)gump->m_Next;

		if (blocked == gump->Blocked)
		{
			if (gump->CanBeDisplayed())
				gump->Draw();

			if (gump->GumpType == GT_MENUBAR)
				menuBarGump = gump;
		}

		gump = next;
	}

	if (menuBarGump != NULL)
		menuBarGump->Draw();
}
//----------------------------------------------------------------------------------
void CGumpManager::Select(const bool &blocked)
{
	CGump *gump = (CGump*)m_Items;
	CGump *menuBarGump = NULL;

	while (gump != NULL)
	{
		CGump *next = (CGump*)gump->m_Next;

		if (blocked == gump->Blocked)
		{
			if (gump->CanBeDisplayed())
				gump->Select();

			if (gump->GumpType == GT_MENUBAR)
				menuBarGump = gump;
		}

		gump = next;
	}

	if (menuBarGump != NULL && blocked == menuBarGump->Blocked)
		menuBarGump->Select();
}
//----------------------------------------------------------------------------------
void CGumpManager::InitToolTip()
{
	CGump *gump = g_SelectedObject.Gump();

	if (gump != NULL)
	{
		if (!g_ConfigManager.UseToolTips)
		{
			if (g_PacketManager.ClientVersion < CV_308Z)
				return;
			else if (gump->GumpType != GT_CONTAINER && gump->GumpType != GT_PAPERDOLL && gump->GumpType != GT_TRADE)
				return;
		}

		gump->InitToolTip();
	}
}
//----------------------------------------------------------------------------------
/*!
Перерисовать все гампы
@return 
*/
void CGumpManager::RedrawAll()
{
	QFOR(gump, m_Items, CGump*)
		gump->WantRedraw = true;
}
//----------------------------------------------------------------------------------
/*!
Нажатие левой кнопки мыши
@param [__in] blocked Состояние экрана
@return 
*/
void CGumpManager::OnLeftMouseButtonDown(const bool &blocked)
{
	if (g_SelectedObject.Object() != NULL && g_SelectedObject.Object()->IsText())
		return;

	QFOR(gump, m_Items, CGump*)
	{
		if (g_SelectedObject.Gump() == gump && !gump->NoProcess)
		{
			if (gump->GumpType == GT_STATUSBAR && ((CGumpStatusbar*)gump)->InGroup())
				((CGumpStatusbar*)gump)->UpdateGroup(0, 0);
			else if (gump->GumpType == GT_SPELL && ((CGumpSpell*)gump)->InGroup())
				((CGumpSpell*)gump)->UpdateGroup(0, 0);
			else
				MoveToBack(gump);

			gump->OnLeftMouseButtonDown();

			break;
		}
	}

	RemoveMarked();
}
//----------------------------------------------------------------------------------
/*!
Отпускание левой кнопки мыши
@param [__in] blocked Состояние экрана
@return 
*/
bool CGumpManager::OnLeftMouseButtonUp(const bool &blocked)
{
	if (g_SelectedObject.Object() != NULL && g_SelectedObject.Object()->IsText())
		return false;

	QFOR(gump, m_Items, CGump*)
	{
		if (g_PressedObject.LeftGump() == gump && !gump->NoProcess)
		{
			bool canMove = false;

			if (g_PressedObject.LeftObject() != NULL)
			{
				if (!g_PressedObject.LeftObject()->IsText())
				{
					if (!g_PressedObject.LeftObject()->Serial)
						canMove = true;
					else if (g_PressedObject.LeftObject()->IsGUI() && ((CBaseGUI*)g_PressedObject.LeftObject())->MoveOnDrag)
						canMove = true;
				}
			}
			else
				canMove = true;

			if (canMove && gump->CanBeMoved() && !gump->NoMove && g_ObjectInHand == NULL)
			{
				WISP_GEOMETRY::CPoint2Di offset = g_MouseManager.LeftDroppedOffset();

				if (gump->GumpType == GT_STATUSBAR)
				{
					CGumpStatusbar *sb = (CGumpStatusbar*)gump;

					if (!gump->Minimized)
					{
						sb->RemoveFromGroup();
						gump->X = gump->X + offset.X;
						gump->Y = gump->Y + offset.Y;
					}
					else
					{
						gump->MinimizedX = gump->MinimizedX + offset.X;
						gump->MinimizedY = gump->MinimizedY + offset.Y;

						if (sb->InGroup())
							sb->UpdateGroup(offset.X, offset.Y);
						else
						{
							int testX = g_MouseManager.Position.X;
							int testY = g_MouseManager.Position.Y;

							CGumpStatusbar *nearBar = sb->GetNearStatusbar(testX, testY);

							if (nearBar != NULL)
							{
								gump->MinimizedX = testX;
								gump->MinimizedY = testY;

								nearBar->AddStatusbar(sb);
							}
						}
					}
				}
				else if (gump->GumpType == GT_SPELL)
				{
					CGumpSpell *spell = (CGumpSpell*)gump;

					gump->X = gump->X + offset.X;
					gump->Y = gump->Y + offset.Y;

					if (spell->InGroup())
						spell->UpdateGroup(offset.X, offset.Y);
					else
					{
						int testX = g_MouseManager.Position.X;
						int testY = g_MouseManager.Position.Y;

						CGumpSpell *nearSpell = spell->GetNearSpell(testX, testY);

						if (nearSpell != NULL)
						{
							gump->X = testX;
							gump->Y = testY;

							nearSpell->AddSpell(spell);
						}
					}
				}
				else if (gump->Minimized && gump->GumpType != GT_MINIMAP)
				{
					gump->MinimizedX = gump->MinimizedX + offset.X;
					gump->MinimizedY = gump->MinimizedY + offset.Y;
				}
				else
				{
					gump->X = gump->X + offset.X;
					gump->Y = gump->Y + offset.Y;
				}

				g_MouseManager.CancelDoubleClick = true;

				//gump->FrameCreated = false;
			}
			
			if (g_ObjectInHand != NULL)
			{
				if (g_SelectedObject.Gump() == gump)
					gump->OnLeftMouseButtonUp();
			}
			else
				gump->OnLeftMouseButtonUp();


			RemoveMarked();
			//MoveGumpToTop(gump);
			return true;
		}
		else if (g_SelectedObject.Gump() == gump && g_ObjectInHand != NULL && (gump->GumpType == GT_PAPERDOLL || gump->GumpType == GT_CONTAINER || gump->GumpType == GT_TRADE))
		{
			gump->OnLeftMouseButtonUp();

			RemoveMarked();
			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------------
/*!
Двойной клик левой кнопкой мыши
@param [__in] blocked Состояние экрана
@return true при успешной обработке
*/
bool CGumpManager::OnLeftMouseButtonDoubleClick(const bool &blocked)
{
	bool result = false;

	if (g_SelectedObject.Object() != NULL && g_SelectedObject.Object()->IsText())
		return result;

	QFOR(gump, m_Items, CGump*)
	{
		if (g_SelectedObject.Gump() == gump && !gump->NoProcess)
		{
			if (gump->OnLeftMouseButtonDoubleClick())
				result = true;

			//MoveGumpToTop(gump);
			break;
		}
	}

	RemoveMarked();

	return result;
}
//----------------------------------------------------------------------------------
/*!
Нажатие правой кнопки мыши
@param [__in] blocked Состояние экрана
@return 
*/
void CGumpManager::OnRightMouseButtonDown(const bool &blocked)
{
	if (g_SelectedObject.Object() != NULL && g_SelectedObject.Object()->IsText())
		return;

	QFOR(gump, m_Items, CGump*)
	{
		if (g_SelectedObject.Gump() == gump && !gump->NoProcess)
		{
			if (gump->GumpType == GT_STATUSBAR && ((CGumpStatusbar*)gump)->InGroup())
				((CGumpStatusbar*)gump)->UpdateGroup(0, 0);
			else if (gump->GumpType == GT_SPELL && ((CGumpSpell*)gump)->InGroup())
				((CGumpSpell*)gump)->UpdateGroup(0, 0);
			else
				MoveToBack(gump);

			g_MouseManager.CancelDoubleClick = true;

			break;
		}
	}

	RemoveMarked();
}
//----------------------------------------------------------------------------------
/*!
Отпускание правой кнопки мыши
@param [__in] blocked Состояние экрана
@return 
*/
void CGumpManager::OnRightMouseButtonUp(const bool &blocked)
{
	if (g_SelectedObject.Object() != NULL && g_SelectedObject.Object()->IsText())
		return;

	QFOR(gump, m_Items, CGump*)
	{
		if (g_PressedObject.RightGump() == gump && !gump->NoProcess && !gump->NoClose && (gump->CanBeMoved() || gump->GumpType == GT_GENERIC))
		{
			//gump->OnClose();
			switch (gump->GumpType)
			{
				case GT_CONTAINER:
				{
					CGameItem *obj = g_World->FindWorldItem(gump->Serial);

					if (obj != NULL)
					{
						if (obj->IsCorpse())
						{
							for (CGameItem *gi = (CGameItem*)obj->m_Items; gi != NULL; )
							{
								CGameItem *next = (CGameItem*)gi->m_Next;

								if (gi->Layer == OL_NONE)
									g_World->RemoveObject(gi);

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
					gump->FrameCreated = false;

					break;
				}
				case GT_BUFF:
				{
					//gump->X = g_GameWindowPosX;
					//gump->Y = g_GameWindowPosY + g_GameWindowHeight;
					gump->FrameCreated = false;
					g_ConfigManager.ToggleBufficonWindow = false;

					break;
				}
				case GT_MENU:
				{
					((CGumpMenu*)gump)->SendMenuResponse(0);

					break;
				}
				case GT_TRADE:
				{
					((CGumpSecureTrading*)gump)->SendTradingResponse(1);

					break;
				}
				case GT_GENERIC:
				{
					((CGumpGeneric*)gump)->SendGumpResponse(0);

					break;
				}
				case GT_OPTIONS:
				{
					g_OptionsMacroManager.Clear();

					CloseGump(gump->Serial, gump->ID, gump->GumpType);

					break;
				}
				case GT_STATUSBAR:
				{
					CGumpStatusbar *sb = ((CGumpStatusbar*)gump)->GetTopStatusbar();

					if (sb != NULL)
					{
						while (sb != NULL)
						{
							CGumpStatusbar *next = sb->m_GroupNext;

							sb->RemoveFromGroup();
							RemoveGump(sb);

							sb = next;
						}
					}
					else
						RemoveGump(gump);

					break;
				}
				/*case GT_SPELL:
				{
					CGumpSpell *spell = ((CGumpSpell*)gump)->GetTopSpell();

					if (spell != NULL)
					{
						while (spell != NULL)
						{
							CGumpSpell *next = spell->m_GroupNext;

							spell->RemoveFromGroup();
							RemoveGump(spell);

							spell = next;
						}
					}
					else
						RemoveGump(gump);

					break;
				}*/
				case GT_SPELLBOOK:
				{
					g_Orion.PlaySoundEffect(0x0055);
					CloseGump(gump->Serial, gump->ID, gump->GumpType);

					break;
				}
				case GT_BOOK:
				{
					((CGumpBook*)gump)->ChangePage(0);
					CloseGump(gump->Serial, gump->ID, gump->GumpType);

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
	}

	RemoveMarked();
}
//----------------------------------------------------------------------------------
/*!
Обработка средней кнопки (колесика) мыши
@param [__in] state Состояние колесика
@param [__in] blocked Состояние экрана
@return 
*/
void CGumpManager::OnMidMouseButtonScroll(const bool &up, const bool &blocked)
{
	QFOR(gump, m_Items, CGump*)
	{
		if (g_SelectedObject.Gump() == gump && !gump->NoProcess)
		{
			gump->OnMidMouseButtonScroll(up);

			break;
		}
	}
}
//----------------------------------------------------------------------------------
void CGumpManager::OnDragging(const bool &blocked)
{
	QFOR(gump, m_Items, CGump*)
	{
		if (g_PressedObject.LeftGump() == gump && !gump->NoProcess)
		{
			gump->OnDragging();

			break;
		}
	}
}
//----------------------------------------------------------------------------------
/*!
Обработка нажатия клавиши
@param [__in] wparam не подписанный параметр
@param [__in] lparam не подписанный параметр
@param [__in] blocked Состояние экрана
@return true при успешной обработке
*/
bool CGumpManager::OnCharPress(const WPARAM &wParam, const LPARAM &lParam, const bool &blocked)
{
	CGump *gump = GetTextEntryOwner();
	bool result = false;

	if (gump != NULL && !gump->NoProcess)
	{
		if (g_GameState == GS_GAME_BLOCKED)
		{
			if (gump->GumpType == GT_TEXT_ENTRY_DIALOG)
			{
				gump->OnCharPress(wParam, lParam);

				result = true;
			}
		}
		else
		{
			gump->OnCharPress(wParam, lParam);

			result = true;
		}
	}

	RemoveMarked();

	return result;
}
//----------------------------------------------------------------------------------
/*!
Обработка нажатия клавиши
@param [__in] wparam не подписанный параметр
@param [__in] lparam не подписанный параметр
@param [__in] blocked Состояние экрана
@return true при успешной обработке
*/
bool CGumpManager::OnKeyDown(const WPARAM &wParam, const LPARAM &lParam, const bool &blocked)
{
	bool result = false;

	if (g_EntryPointer != NULL && g_EntryPointer != &g_GameConsole)
	{
		CGump *gump = GetTextEntryOwner();

		if (gump != NULL && !gump->NoProcess)
		{
			switch (gump->GumpType)
			{
				case GT_STATUSBAR:
				case GT_DRAG:
				case GT_OPTIONS:
				case GT_SKILLS:
				case GT_PROFILE:
				case GT_BULLETIN_BOARD_ITEM:
				case GT_GENERIC:
				case GT_BOOK:
				{
					gump->OnKeyDown(wParam, lParam);

					result = true;

					break;
				}
			}
		}
		else if (wParam == VK_DELETE)
		{
			gump = GetGump(g_PlayerSerial, 0, GT_SKILLS);
			if (gump != NULL && !gump->NoProcess)
				gump->OnKeyDown(wParam, lParam);
		}
	}
	else if (wParam == VK_DELETE)
	{
		CGump *gump = GetGump(g_PlayerSerial, 0, GT_SKILLS);

		if (gump != NULL && !gump->NoProcess)
			gump->OnKeyDown(wParam, lParam);
	}

	RemoveMarked();

	return result;
}
//----------------------------------------------------------------------------------
/*!
Загрузка гампов из конфига
@param [__in] path Путь к файлу конфига
@return 
*/
void CGumpManager::Load(const string &path)
{
	WISP_FILE::CMappedFile file;

	bool paperdollRequested = false;
	bool menubarFound = false;
	bool bufficonWindowFound = false;
	bool minimizedConsoleType = false;
	bool showFullTextConsoleType = false;

	if (file.Load(path) && file.Size)
	{
		uchar version = file.ReadUInt8();

		puchar oldPtr = file.Ptr;

		short count = 0;
		short spellGroupsCount = 0;

		if (version)
		{
			file.Ptr = (puchar)file.Start + (file.Size - 8);
			spellGroupsCount = file.ReadInt16LE();
			count = file.ReadInt16LE();
		}
		else
		{
			file.Ptr = (puchar)file.Start + (file.Size - 6);
			count = file.ReadInt16LE();
		}

		file.Ptr = oldPtr;
		bool menubarLoaded = false;

		IFOR(i, 0, count)
		{
			CGump *gump = NULL;

			puchar next = file.Ptr;
			uchar size = file.ReadUInt8();
			next += size;

			GUMP_TYPE gumpType = (GUMP_TYPE)file.ReadUInt8();
			ushort gumpX = file.ReadUInt16LE();
			ushort gumpY = file.ReadUInt16LE();
			uchar gumpMinimized = file.ReadUInt8();
			ushort gumpMinimizedX = file.ReadUInt16LE();
			ushort gumpMinimizedY = file.ReadUInt16LE();
			uchar gumpLockMoving = file.ReadUInt8();

			switch ((GUMP_TYPE)gumpType)
			{
				case GT_PAPERDOLL:
				{
					gump = new CGumpPaperdoll(g_PlayerSerial, gumpX, gumpY, gumpMinimized);
							
					g_Orion.PaperdollReq(g_PlayerSerial);
					paperdollRequested = true;

					break;
				}
				case GT_STATUSBAR:
				{
					gump = new CGumpStatusbar(g_PlayerSerial, gumpX, gumpY, gumpMinimized);
					break;
				}
				case GT_MINIMAP:
				{
					gump = new CGumpMinimap(g_PlayerSerial, gumpX, gumpY, gumpMinimized);
					break;
				}
				case GT_SKILLS:
				{
					gump = new CGumpSkills(g_PlayerSerial, gumpX, gumpY, gumpMinimized, file.ReadInt16LE());

					break;
				}
				case GT_JOURNAL:
				{
					gump = new CGumpJournal(g_PlayerSerial, gumpX, gumpY, gumpMinimized, file.ReadInt16LE());
					
					break;
				}
				case GT_WORLD_MAP:
				{
					CGumpWorldMap *wmg = new CGumpWorldMap(g_PlayerSerial, gumpX, gumpY);
					gump = wmg;

					wmg->Minimized = gumpMinimized;

					if (gumpMinimized)
						wmg->Page = 1;
					else
						wmg->Page = 2;

					wmg->Map = file.ReadUInt8();
					wmg->Scale = file.ReadUInt8();
					wmg->LinkWithPlayer = file.ReadUInt8();

					wmg->Width = file.ReadInt16LE();
					wmg->Height = file.ReadInt16LE();

					wmg->OffsetX = file.ReadInt16LE();
					wmg->OffsetY = file.ReadInt16LE();

					wmg->UpdateSize();

					break;
				}
				case GT_MENUBAR:
				case GT_BUFF:
				{
					if (gumpType == GT_BUFF)
					{
						bufficonWindowFound = true;
						gump = new CGumpBuff(g_PlayerSerial, gumpX, gumpY);
						gump->Graphic = file.ReadUInt16LE();
					}
					else if (!g_ConfigManager.DisableMenubar)
					{
						menubarFound = true;
						gump = new CGumpMenubar(g_PlayerSerial, gumpX, gumpY);
						((CGumpMenubar*)gump)->Opened = gumpMinimized;
					}

					break;
				}
				case GT_CONTAINER:
				case GT_SPELLBOOK:
				case GT_SPELL:
				{
					uint serial = file.ReadUInt32LE();

					if (gumpType == GT_SPELL)
					{
						ushort graphic = file.ReadUInt16LE();

						SPELLBOOK_TYPE spellType = ST_MAGE;

						if (size > 18)
							spellType = (SPELLBOOK_TYPE)file.ReadUInt8();
					
						gump = new CGumpSpell(serial, gumpX, gumpY, graphic, spellType);
					}
					else
					{
						g_ContainerStack.push_back(CContainerStackItem(serial, gumpX, gumpY, gumpMinimizedX, gumpMinimizedY, gumpMinimized, gumpLockMoving));

						g_UseItemActions.Add(serial);
						//g_Orion.DoubleClick(serial);
					}

					break;
				}
				case GT_CONSOLE_TYPE:
				{
					minimizedConsoleType = gumpMinimized;
					showFullTextConsoleType = file.ReadUInt8();
					break;
				}
				case GT_SKILL:
				{
					uint serial = file.ReadUInt32LE();

					gump = new CGumpSkill(serial, gumpX, gumpY);

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

				AddGump(gump);

				gump->WantUpdateContent = true;
			}

			file.Ptr = next;
		}

		IFOR(i, 0, spellGroupsCount)
		{
			CGumpSpell *topSpell = NULL;

			ushort spellsCount = file.ReadUInt16LE();

			IFOR(j, 0, spellsCount)
			{
				puchar next = file.Ptr;
				uchar size = file.ReadUInt8();
				next += size;

				GUMP_TYPE gumpType = (GUMP_TYPE)file.ReadUInt8();
				ushort gumpX = file.ReadUInt16LE();
				ushort gumpY = file.ReadUInt16LE();
				file.Move(5); //Minimized state, x, y
				uchar gumpLockMoving = file.ReadUInt8();

				uint serial = file.ReadUInt32LE();
				ushort graphic = file.ReadUInt16LE();

				SPELLBOOK_TYPE spellType = ST_MAGE;

				if (size > 18)
					spellType = (SPELLBOOK_TYPE)file.ReadUInt8();

				CGumpSpell *spell = new CGumpSpell(serial, gumpX, gumpY, graphic, spellType);
				spell->LockMoving = gumpLockMoving;

				AddGump(spell);

				if (topSpell == NULL)
					topSpell = spell;
				else
					topSpell->AddSpell(spell);

				spell->WantUpdateContent = true;

				file.Ptr = next;
			}
		}

		file.Unload();
	}
	else
	{
		if (!g_ConfigManager.DisableMenubar && !g_ConfigManager.GameWindowX && !g_ConfigManager.GameWindowY)
			g_ConfigManager.GameWindowY = 40;

		AddGump(new CGumpPaperdoll(g_PlayerSerial, g_ConfigManager.GameWindowX + g_ConfigManager.GameWindowWidth, g_ConfigManager.GameWindowY, false));
		AddGump(new CGumpStatusbar(g_PlayerSerial, g_ConfigManager.GameWindowX + g_ConfigManager.GameWindowWidth, g_ConfigManager.GameWindowY + g_ConfigManager.GameWindowHeight - 50, false));
		AddGump(new CGumpMinimap(g_PlayerSerial, g_ConfigManager.GameWindowX, g_ConfigManager.GameWindowY, true));

		if (g_Player != NULL)
		{
			CGameItem *backpack = g_Player->FindLayer(OL_BACKPACK);

			if (backpack != NULL)
			{
				g_ContainerStack.push_back(CContainerStackItem(backpack->Serial, g_ConfigManager.GameWindowX, g_ConfigManager.GameWindowY, g_ConfigManager.GameWindowX, g_ConfigManager.GameWindowY, false, false));

				g_UseItemActions.Add(backpack->Serial);
				//g_Orion.DoubleClick(backpack->Serial);
			}
		}
	}

	if (!g_ConfigManager.DisableMenubar && !menubarFound)
	{
		CGumpMenubar *mbg = new CGumpMenubar(g_PlayerSerial, 0, 0);
		mbg->Opened = true;
		AddGump(mbg);
	}

	if (!bufficonWindowFound)
		AddGump(new CGumpBuff(g_PlayerSerial, g_ConfigManager.GameWindowX + (int)(g_ConfigManager.GameWindowWidth * 0.7f), g_ConfigManager.GameWindowY + g_ConfigManager.GameWindowHeight));

	if (!paperdollRequested)
		g_Orion.PaperdollReq(g_PlayerSerial);

	AddGump(new CGumpConsoleType(minimizedConsoleType, showFullTextConsoleType));
}
//----------------------------------------------------------------------------------
void CGumpManager::SaveDefaultGumpProperties(WISP_FILE::CBinaryFileWritter &writer, CGump *gump, const int &size)
{
	writer.WriteInt8(size);
	writer.WriteInt8(gump->GumpType);
	writer.WriteUInt16LE(gump->X);
	writer.WriteUInt16LE(gump->Y);

	if (gump->GumpType == GT_MENUBAR)
		writer.WriteInt8(((CGumpMenubar*)gump)->Opened);
	else //buff
		writer.WriteInt8(gump->Minimized);

	writer.WriteUInt16LE(gump->MinimizedX);
	writer.WriteUInt16LE(gump->MinimizedY);
	writer.WriteInt8(gump->LockMoving);
};
//----------------------------------------------------------------------------------
/*!
Сохранить гампы в конфиг
@param [__in] path Путь к файлу кофнига
@return 
*/
void CGumpManager::Save(const string &path)
{
	WISP_FILE::CBinaryFileWritter writter;

	writter.Open(path);

	writter.WriteInt8(1); //version
	writter.WriteBuffer();

	short count = 0;

	vector<CGump*> containerList;
	vector<CGump*> spellInGroupList;

	QFOR(gump, m_Items, CGump*)
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
				if (gump->Serial != g_PlayerSerial)
					break;
				
				uchar size = 12;
				
				if (gump->GumpType == GT_JOURNAL)
					size += 2;
				else if (gump->GumpType == GT_SKILLS)
					size += 2;
				else if (gump->GumpType == GT_WORLD_MAP)
					size += 11;

				SaveDefaultGumpProperties(writter, gump, size);

				if (gump->GumpType == GT_JOURNAL)
					writter.WriteUInt16LE(((CGumpJournal*)gump)->Height);
				else if (gump->GumpType == GT_SKILLS)
					writter.WriteUInt16LE(((CGumpSkills*)gump)->Height);
				else if (gump->GumpType == GT_WORLD_MAP)
				{
					CGumpWorldMap *wmg = (CGumpWorldMap*)gump;
					
					writter.WriteUInt8(wmg->Map);
					writter.WriteUInt8(wmg->Scale);
					writter.WriteUInt8(wmg->LinkWithPlayer);

					writter.WriteInt16LE(wmg->Width);
					writter.WriteInt16LE(wmg->Height);

					writter.WriteInt16LE(wmg->OffsetX);
					writter.WriteInt16LE(wmg->OffsetY);
				}

				writter.WriteBuffer();
				count++;

				break;
			}
			case GT_MENUBAR:
			{
				SaveDefaultGumpProperties(writter, gump, 12);

				writter.WriteBuffer();
				count++;

				break;
			}
			case GT_BUFF:
			{
				SaveDefaultGumpProperties(writter, gump, 14);

				writter.WriteUInt16LE(gump->Graphic);

				writter.WriteBuffer();
				count++;

				break;
			}
			case GT_CONTAINER:
			case GT_SPELLBOOK:
			case GT_SPELL:
			{
				if (gump->GumpType != GT_SPELL)
				{
					CGameObject *topobj = g_World->FindWorldObject(gump->Serial);

					if (topobj == NULL || ((CGameItem*)topobj)->Layer == OL_BANK)
						break;

					topobj = topobj->GetTopObject();

					if (topobj->Serial != g_PlayerSerial)
						break;

					containerList.push_back(gump);
					break;
				}
				else if (((CGumpSpell*)gump)->InGroup())
				{
					spellInGroupList.push_back(gump);
					break;
				}

				SaveDefaultGumpProperties(writter, gump, 19);

				writter.WriteUInt32LE(gump->Serial);
				writter.WriteUInt16LE(gump->Graphic);
				writter.WriteUInt8(((CGumpSpell*)gump)->SpellType);
				writter.WriteBuffer();

				count++;

				break;
			}
			case GT_CONSOLE_TYPE:
			{
				SaveDefaultGumpProperties(writter, gump, 13);

				writter.WriteUInt8(((CGumpConsoleType*)gump)->ShowFullText);

				writter.WriteBuffer();
				count++;
			}
			case GT_SKILL:
			{
				SaveDefaultGumpProperties(writter, gump, 16);

				writter.WriteUInt32LE(gump->Serial);
				writter.WriteBuffer();

				count++;

				break;
			}
			default:
				break;
		}
	}

	UINT_LIST playerContainers;
	playerContainers.push_back(g_PlayerSerial);

	while (!playerContainers.empty() && !containerList.empty())
	{
		uint containerSerial = playerContainers.front();
		playerContainers.erase(playerContainers.begin());

		for (vector<CGump*>::iterator it = containerList.begin(); it != containerList.end();)
		{
			CGump *gump = *it;

			if (gump->Serial == containerSerial)
			{
				SaveDefaultGumpProperties(writter, gump, 16);

				writter.WriteUInt32LE(gump->Serial);
				writter.WriteBuffer();

				count++;

				it = containerList.erase(it);

				break;
			}
			else
				it++;
		}

		CGameObject *owner = g_World->FindWorldObject(containerSerial);

		if (owner != NULL)
		{
			QFOR(item, owner->m_Items, CGameItem*)
			{
				if (item->Opened)
					playerContainers.push_back(item->Serial);
			}
		}
	}

	int spellGroupsCount = 0;

	if (spellInGroupList.size())
	{
		vector<CGump*> spellGroups;

		while (spellInGroupList.size())
		{
			CGumpSpell *spell = (CGumpSpell*)spellInGroupList[0];
			CGumpSpell *topSpell = spell->GetTopSpell();
			spellGroups.push_back(topSpell);
			spellGroupsCount++;

			for (spell = topSpell; spell != NULL; spell = spell->m_GroupNext)
			{
				for (vector<CGump*>::iterator it = spellInGroupList.begin(); it != spellInGroupList.end(); it++)
				{
					if (*it == spell)
					{
						spellInGroupList.erase(it);
						break;
					}
				}
			}
		}

		IFOR(i, 0, spellGroupsCount)
		{
			CGumpSpell *spell = (CGumpSpell*)spellGroups[i];
			int spellsCount = 0;

			for (CGumpSpell *spell = (CGumpSpell*)spellGroups[i]; spell != NULL; spell = spell->m_GroupNext)
				spellsCount++;

			writter.WriteInt16LE(spellsCount);

			for (CGumpSpell *spell = (CGumpSpell*)spellGroups[i]; spell != NULL; spell = spell->m_GroupNext)
			{
				SaveDefaultGumpProperties(writter, spell, 19);

				writter.WriteUInt32LE(spell->Serial);
				writter.WriteUInt16LE(spell->Graphic);
				writter.WriteUInt8(spell->SpellType);
				writter.WriteBuffer();
			}
		}
	}
	
	writter.WriteInt16LE(spellGroupsCount);
	writter.WriteInt16LE(count);
	writter.WriteUInt32LE(0); //EOF
	writter.WriteBuffer();

	writter.Close();
}
//----------------------------------------------------------------------------------
