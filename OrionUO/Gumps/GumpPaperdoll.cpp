/***********************************************************************************
**
** GumpPaperdoll.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
int CGumpPaperdoll::UsedLayers[m_LayerCount] =
{
	OL_CLOAK,
	OL_RING,
	OL_SHIRT,
	OL_ARMS,
	OL_PANTS,
	OL_SHOES,
	OL_LEGS,
	OL_TORSO,
	OL_BRACELET,
	OL_15,
	OL_GLOVES,
	OL_TUNIC,
	OL_SKIRT,
	OL_NECKLACE,
	OL_HAIR,
	OL_ROBE,
	OL_EARRINGS,
	OL_BEARD,
	OL_HELMET,
	OL_WAIST,
	OL_1_HAND,
	OL_2_HAND,
	OL_TALISMAN
};
//----------------------------------------------------------------------------------
CGumpPaperdoll::CGumpPaperdoll(uint serial, short x, short y, bool minimized)
: CGump(GT_PAPERDOLL, serial, x, y)
{
	WISPFUN_DEBUG("c105_f1");
	m_Locker.Serial = ID_GP_LOCK_MOVING;

	if (minimized)
	{
		m_Minimized = true;
		m_MinimizedX = x;
		m_MinimizedY = y;
		m_Page = 1;
	}
	else
		m_Page = 2;

	int partyManifestX = 37;
	int profileX = 23;
	static const int SCROLLS_STEP = 12;

	Add(new CGUIPage(1));
	Add(new CGUIGumppic(0x07EE, 0, 0));

	Add(new CGUIPage(2));

	if (m_Serial == g_PlayerSerial)
	{
		Add(new CGUIGumppic(0x07D0, 0, 0));

		Add(new CGUIButton(ID_GP_BUTTON_HELP, 0x07EF, 0x07F1, 0x07F0, 185, 44));
		Add(new CGUIButton(ID_GP_BUTTON_OPTIONS, 0x07D6, 0x07D8, 0x07D7, 185, 71));
		Add(new CGUIButton(ID_GP_BUTTON_LOGOUT, 0x07D9, 0x07DB, 0x07DA, 185, 98));

		if (g_PacketManager.ClientVersion >= CV_500A)
			Add(new CGUIButton(ID_GP_BUTTON_JOURNAL_OR_QUESTS, 0x57B5, 0x57B6, 0x57B7, 185, 125));
		else
			Add(new CGUIButton(ID_GP_BUTTON_JOURNAL_OR_QUESTS, 0x07DC, 0x07DE, 0x07DD, 185, 125));

		Add(new CGUIButton(ID_GP_BUTTON_SKILLS, 0x07DF, 0x07E1, 0x07E0, 185, 152));

		if (g_PacketManager.ClientVersion >= CV_500A)
			Add(new CGUIButton(ID_GP_BUTTON_CHAT_OR_GUILD, 0x57B2, 0x57B3, 0x57B4, 185, 179));
		else
		{
			if (!g_ChatEnabled)
			{
				Add(new CGUIShader(&g_DeathShader, true));
				Add(new CGUIButton(ID_GP_BUTTON_CHAT_OR_GUILD, 0x07E2, 0x07E2, 0x07E2, 185, 179));
				Add(new CGUIShader(&g_DeathShader, false));
			}
			else
				Add(new CGUIButton(ID_GP_BUTTON_CHAT_OR_GUILD, 0x07E2, 0x07E4, 0x07E3, 185, 179));
		}

		if (g_Player->Warmode)
			m_ButtonWarmode = (CGUIButton*)Add(new CGUIButton(ID_GP_BUTTON_WARMODE, 0x07E8, 0x07EA, 0x07E9, 185, 206));
		else
			m_ButtonWarmode = (CGUIButton*)Add(new CGUIButton(ID_GP_BUTTON_WARMODE, 0x07E5, 0x07E7, 0x07E6, 185, 206));

		//UO->DrawGump(0x0FA1, 0, posX + 80, posY + 4); //Paperdoll mail bag
		Add(new CGUIButton(ID_GP_BUTTON_VIRTURE, 0x0071, 0x0071, 0x0071, 80, 4));

		if (g_PaperdollBooks)
		{
			Add(new CGUIButton(ID_GP_COMBAT_BOOK, 0x2B34, 0x2B34, 0x2B34, 156, 200));

			if (g_PacketManager.ClientVersion >= CV_7000)
			{
				Add(new CGUIButton(ID_GP_RACIAL_ABILITIES_BOOK, 0x2B28, 0x2B28, 0x2B28, 23, 200));
				profileX += SCROLLS_STEP;
				partyManifestX += SCROLLS_STEP;
			}
		}

		Add(new CGUIButton(ID_GP_PARTY_MANIFEST_SCROLL, 0x07D2, 0x07D2, 0x07D2, partyManifestX, 196));

		CGUIHitBox *minimizeButton = (CGUIHitBox*)Add(new CGUIHitBox(ID_GP_BUTTON_MINIMIZE, 226, 258, 16, 16, true));
		minimizeButton->ToPage = 1;
	}
	else
	{
		m_ButtonWarmode = NULL;
		Add(new CGUIGumppic(0x07D1, 0, 0));
	}

	Add(new CGUIButton(ID_GP_BUTTON_STATUS, 0x07EB, 0x07ED, 0x07EC, 185, 233));

	Add(new CGUIButton(ID_GP_PROFILE_SCROLL, 0x07D2, 0x07D2, 0x07D2, profileX, 196));

	//if (ConnectionManager.ClientVersion >= CV_60142)
	//	UO->DrawGump(0x2B34, 0, 156, 200);

	m_DataBox = (CGUIDataBox*)Add(new CGUIDataBox());
}
//----------------------------------------------------------------------------------
CGumpPaperdoll::~CGumpPaperdoll()
{
}
//----------------------------------------------------------------------------------
void CGumpPaperdoll::CalculateGumpState()
{
	WISPFUN_DEBUG("c105_f2");
	CGump::CalculateGumpState();

	if (g_GumpPressed && g_PressedObject.LeftObject != NULL && g_PressedObject.LeftObject->IsText())
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
}
//----------------------------------------------------------------------------------
void CGumpPaperdoll::InitToolTip()
{
	WISPFUN_DEBUG("c105_f3");
	uint id = g_SelectedObject.Serial;

	if (!m_Minimized)
	{
		if (!g_ConfigManager.UseToolTips)
			id = 0;

		switch (id)
		{
			case ID_GP_BUTTON_HELP:
			{
				g_ToolTip.Set(L"Open server's help menu gump");
				break;
			}
			case ID_GP_BUTTON_OPTIONS:
			{
				g_ToolTip.Set(L"Open the configuration gump");
				break;
			}
			case ID_GP_BUTTON_LOGOUT:
			{
				g_ToolTip.Set(L"Open the logout gump");
				break;
			}
			case ID_GP_BUTTON_JOURNAL_OR_QUESTS:
			{
				if (g_PacketManager.ClientVersion >= CV_500A)
					g_ToolTip.Set(L"Open the quests gump");
				else
					g_ToolTip.Set(L"Open the journal gump");

				break;
			}
			case ID_GP_BUTTON_SKILLS:
			{
				g_ToolTip.Set(L"Open the skills gump");
				break;
			}
			case ID_GP_BUTTON_CHAT_OR_GUILD:
			{
				if (g_PacketManager.ClientVersion >= CV_500A)
					g_ToolTip.Set(L"Open the guild gump");
				else
					g_ToolTip.Set(L"Open the chat gump");

				break;
			}
			case ID_GP_BUTTON_WARMODE:
			{
				g_ToolTip.Set(L"Change player's warmode");
				break;
			}
			case ID_GP_BUTTON_STATUS:
			{
				g_ToolTip.Set(L"Open player's statusbar gump");
				break;
			}
			case ID_GP_BUTTON_MINIMIZE:
			{
				g_ToolTip.Set(L"Minimize the paperdoll gump");
				break;
			}
			case ID_GP_PROFILE_SCROLL:
			{
				g_ToolTip.Set(L"Double click for open profile gump");
				break;
			}
			case ID_GP_PARTY_MANIFEST_SCROLL:
			{
				g_ToolTip.Set(L"Double click for open party manifest gump");
				break;
			}
			case ID_GP_BUTTON_VIRTURE:
			{
				g_ToolTip.Set(L"Open server's virture(?) gump");
				break;
			}
			case ID_GP_COMBAT_BOOK:
			{
				g_ToolTip.Set(L"Open combat book");
				break;
			}
			case ID_GP_RACIAL_ABILITIES_BOOK:
			{
				g_ToolTip.Set(L"Open racial abilities book");
				break;
			}
			case ID_GP_LOCK_MOVING:
			{
				g_ToolTip.Set(L"Lock moving/closing the paperdoll gump");
				break;
			}
			default:
			{
				if (g_PacketManager.ClientVersion >= CV_308Z)
					id = g_SelectedObject.Serial;

				if (id >= ID_GP_ITEMS)
				{
					CGameCharacter *character = g_World->FindWorldCharacter(m_Serial);

					if (character != NULL)
					{
						CGameObject *obj = character->FindLayer(id - ID_GP_ITEMS);

						if (obj != NULL)
							g_ObjectPropertiesManager.Display(obj->Serial);
					}
				}

				break;
			}
		}
	}
	else if (g_ConfigManager.UseToolTips)
		g_ToolTip.Set(L"Double click to maximize paperdoll gump");
}
//----------------------------------------------------------------------------------
void CGumpPaperdoll::DelayedClick(CRenderObject *obj)
{
	WISPFUN_DEBUG("c105_f4");
	if (obj != NULL)
	{
		CTextData *td = new CTextData();
		td->Unicode = false;
		td->Font = 3;
		td->Serial = 0;
		td->Color = 0x038F;
		td->Timer = g_Ticks;
		td->Type = TT_CLIENT;
		td->X = g_MouseManager.Position.X - m_X;
		td->Y = g_MouseManager.Position.Y - m_Y;

		string text = "Party Manifest";

		if (obj->Serial == ID_GP_PROFILE_SCROLL)
			text = "Character Profile";

		td->SetText(text);

		int width = g_FontManager.GetWidthA(3, text);

		if (width > TEXT_MESSAGE_MAX_WIDTH)
			td->GenerateTexture(TEXT_MESSAGE_MAX_WIDTH, 0, TS_CENTER);
		else
			td->GenerateTexture(0, 0, TS_CENTER);

		m_TextContainer.Add(td);
		m_TextRenderer.AddText(td);

		g_Orion.AddJournalMessage(td, "");
	}
}
//----------------------------------------------------------------------------------
void CGumpPaperdoll::PrepareContent()
{
	WISPFUN_DEBUG("c105_f5");
	CGameCharacter *obj = g_World->FindWorldCharacter(m_Serial);

	if (obj == NULL)
		return;

	if (!g_Player->Dead() && (m_CanLift || m_Serial == g_PlayerSerial) && g_PressedObject.LeftGump == this && !g_ObjectInHand.Enabled && g_PressedObject.LeftSerial != 0xFFFFFFFF && g_MouseManager.LastLeftButtonClickTimer < g_Ticks)
	{
		WISP_GEOMETRY::CPoint2Di offset = g_MouseManager.LeftDroppedOffset();

		if (CanBeDraggedByOffset(offset) || (g_MouseManager.LastLeftButtonClickTimer + g_MouseManager.DoubleClickDelay < g_Ticks))
		{
			int layer = g_PressedObject.LeftSerial - ID_GP_ITEMS;

			if (layer != OL_HAIR && layer != OL_BEARD && layer != OL_BACKPACK)
			{
				CGameItem *equipment = obj->FindLayer(layer);

				if (equipment != NULL)
				{
					//if (g_Target.IsTargeting())
					//	g_Target.SendCancelTarget();

					g_Orion.PickupItem(equipment);
					//g_LastGumpLeftMouseDown = 0;
					g_PressedObject.ClearLeft();
					g_MouseManager.LeftDropPosition = g_MouseManager.Position;

					if (layer == OL_1_HAND || layer == OL_2_HAND)
						g_Player->UpdateAbilities();
				}
			}
		}
	}

	bool wantTransparent = false;

	if (g_SelectedObject.Gump == this && g_ObjectInHand.Enabled && g_ObjectInHand.TiledataPtr->AnimID)
	{
		if (obj->FindLayer(g_ObjectInHand.TiledataPtr->Layer) == NULL)
		{
			if (!m_WantTransparentContent)
			{
				m_WantUpdateContent = true;
				m_WantTransparentContent = true;
			}

			wantTransparent = true;
		}
	}

	if (m_WantTransparentContent && !wantTransparent)
	{
		m_WantUpdateContent = true;
		m_WantTransparentContent = false;
	}

	if (!m_Minimized && m_TextRenderer.CalculatePositions(false))
		m_WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGumpPaperdoll::UpdateContent()
{
	WISPFUN_DEBUG("c105_f6");
	//Clear();

	CGameCharacter *obj = g_World->FindWorldCharacter(m_Serial);

	if (obj == NULL)
		return;

	m_DataBox->Clear();

	m_Description = (CGUIText*)m_DataBox->Add(new CGUIText(0x0386, 39, 262));
	m_Description->CreateTextureA(1, obj->PaperdollText, 185);

	m_DataBox->Add(new CGUIShader(&g_ColorizerShader, true));

	CGUIGumppic *bodyGumppic = NULL;

	ushort color = obj->Color & 0x3FFF;

	if (obj->Graphic == 0x0191 || obj->Graphic == 0x0193)
	{
		bodyGumppic = (CGUIGumppic*)m_DataBox->Add(new CGUIGumppic(0x000D, 8, 19)); //Female gump
		bodyGumppic->PartialHue = true;
		bodyGumppic->Color = color;
	}
	else if (obj->Graphic == 0x025D)
	{
		bodyGumppic = (CGUIGumppic*)m_DataBox->Add(new CGUIGumppic(0x000E, 8, 19)); //Elf Male gump
		bodyGumppic->PartialHue = true;
		bodyGumppic->Color = color;
	}
	else if (obj->Graphic == 0x025E)
	{
		bodyGumppic = (CGUIGumppic*)m_DataBox->Add(new CGUIGumppic(0x000F, 8, 19)); //Elf Female gump
		bodyGumppic->PartialHue = true;
		bodyGumppic->Color = color;
	}
	else if (obj->Graphic == 0x029A)
	{
		bodyGumppic = (CGUIGumppic*)m_DataBox->Add(new CGUIGumppic(0x29A, 8, 19)); //Gargoly Male gump
		bodyGumppic->PartialHue = true;
		bodyGumppic->Color = color;
	}
	else if (obj->Graphic == 0x029B)
	{
		bodyGumppic = (CGUIGumppic*)m_DataBox->Add(new CGUIGumppic(0x299, 8, 19)); //Gargoly Female gump
		bodyGumppic->PartialHue = true;
		bodyGumppic->Color = color;
	}
	else if (obj->Graphic == 0x03DB)
	{
		bodyGumppic = (CGUIGumppic*)m_DataBox->Add(new CGUIGumppic(0x000C, 8, 19)); //Male gump
		bodyGumppic->Color = 0x03EA;
		bodyGumppic->PartialHue = true;

		bodyGumppic = (CGUIGumppic*)m_DataBox->Add(new CGUIGumppic(0xC72B, 8, 19)); //GM robe gump
		bodyGumppic->Color = color;
	}
	else //if (obj->Graphic == 0x0190 || obj->Graphic == 0x0192)
	{
		bodyGumppic = (CGUIGumppic*)m_DataBox->Add(new CGUIGumppic(0x000C, 8, 19)); //Male gump
		bodyGumppic->PartialHue = true;
		bodyGumppic->Color = color;
	}

	int gumpOffset = (obj->Female ? FEMALE_GUMP_OFFSET : MALE_GUMP_OFFSET);

	//Draw equipment & backpack
	CGameItem *equipment = NULL;

	EQUIP_CONV_BODY_MAP &equipConv = g_AnimationManager.GetEquipConv();

	EQUIP_CONV_BODY_MAP::iterator bodyIter = equipConv.find(obj->Graphic);

	g_ColorizerShader.Use();

	//if (obj->IsHuman())
	{
		bool useSlots = g_ConfigManager.PaperdollSlots;

		static const int maxPaperdollSlots = 6;
		CGameItem *slotObjects[maxPaperdollSlots] = { 0 };

		IFOR(i, 0, m_LayerCount)
		{
			equipment = obj->FindLayer(UsedLayers[i]);

			if (equipment != NULL)
			{
				if (useSlots)
				{
					switch (UsedLayers[i])
					{
						case OL_HELMET:
						{
							slotObjects[0] = equipment;
							break;
						}
						case OL_EARRINGS:
						{
							slotObjects[1] = equipment;
							break;
						}
						case OL_NECKLACE:
						{
							slotObjects[2] = equipment;
							break;
						}
						case OL_RING:
						{
							slotObjects[3] = equipment;
							break;
						}
						case OL_BRACELET:
						{
							slotObjects[4] = equipment;
							break;
						}
						case OL_TUNIC:
						{
							slotObjects[5] = equipment;
							break;
						}
						default:
							break;
					}
				}

				ushort id = equipment->AnimID;

				if (bodyIter != equipConv.end())
				{
					EQUIP_CONV_DATA_MAP::iterator dataIter = bodyIter->second.find(id);

					if (dataIter != bodyIter->second.end())
						id = dataIter->second.Gump;
				}

				if (id)
				{
					int cOfs = gumpOffset;

					if (obj->Female && g_Orion.ExecuteGump(id + cOfs) == NULL)
						cOfs = MALE_GUMP_OFFSET;

					bodyGumppic = (CGUIGumppic*)m_DataBox->Add(new CGUIGumppic(id + cOfs, 8, 19));
					bodyGumppic->Color = equipment->Color & 0x3FFF;
					bodyGumppic->PartialHue = equipment->IsPartialHue();
					bodyGumppic->Serial = ID_GP_ITEMS + UsedLayers[i];
				}
			}
			else if (m_WantTransparentContent && g_ObjectInHand.Enabled && UsedLayers[i] == g_ObjectInHand.TiledataPtr->Layer && g_ObjectInHand.TiledataPtr->AnimID)
			{
				equipment = obj->FindLayer(g_ObjectInHand.TiledataPtr->Layer);

				if (equipment == NULL)
				{
					ushort id = g_ObjectInHand.TiledataPtr->AnimID;

					if (bodyIter != equipConv.end())
					{
						EQUIP_CONV_DATA_MAP::iterator dataIter = bodyIter->second.find(id);

						if (dataIter != bodyIter->second.end())
							id = dataIter->second.Gump;
					}

					int cOfs = gumpOffset;

					if (obj->Female && !g_Orion.ExecuteGump(id + cOfs))
						cOfs = MALE_GUMP_OFFSET;

					m_DataBox->Add(new CGUIAlphaBlending(true, 0.7f));

					bodyGumppic = (CGUIGumppic*)m_DataBox->Add(new CGUIGumppic(id + cOfs, 8, 19));
					bodyGumppic->Color = g_ObjectInHand.Color & 0x3FFF;
					bodyGumppic->PartialHue = (g_ObjectInHand.Color & 0x8000);

					m_DataBox->Add(new CGUIAlphaBlending(false, 0.0f));
				}
			}
		}

		if (useSlots)
		{
			int yPtr = 75;

			bool scaleImages = g_ConfigManager.ScaleImagesInPaperdollSlots;

			IFOR(i, 0, maxPaperdollSlots)
			{
				CGUIGumppicTiled *backgroundSlot = (CGUIGumppicTiled*)m_DataBox->Add(new CGUIGumppicTiled(0x243A, 1, yPtr, 19, 20));
				CGUIGumppic *scopeSlot = (CGUIGumppic*)m_DataBox->Add(new CGUIGumppic(0x2344, 1, yPtr));

				CGameItem *equipment = slotObjects[i];

				if (equipment != NULL)
				{
					uint slotSerial = ID_GP_ITEMS + equipment->Layer;

					WISP_GEOMETRY::CRect rect = g_Orion.GetStaticArtRealPixelDimension(equipment->Graphic);

					int tileOffsetX = (13 - rect.Size.Width) / 2;
					int tileOffsetY = (14 - rect.Size.Height) / 2;

					int tileX = 4;
					int tileY = 3 + yPtr;

					bool drawed = false;

					if (scaleImages && (tileOffsetX < 0 || tileOffsetY < 0))
					{
						CGLTexture *th = g_Orion.ExecuteStaticArt(m_Graphic);

						if (th != NULL)
						{
							int width = th->Width;
							int height = th->Height;

							if (tileOffsetX < 0 && width)
							{
								float percents = (rect.Size.Width / 13.0f) / (float)(rect.Size.Width / (float)width) / 2.0f;

								tileX -= (int)(rect.Position.X / percents);
								width = (int)(width / percents);
							}
							else
								tileX -= rect.Position.X + tileOffsetX;

							if (tileOffsetY < 0 && height)
							{
								float percents = (rect.Size.Height / 14.0f) / (float)(rect.Size.Height / (float)height) / 2.0f;

								tileY -= (int)(rect.Position.Y / percents);
								height = (int)(height / percents);
							}
							else
								tileY -= rect.Position.Y + tileOffsetY;

							m_DataBox->Add(new CGUITilepicScaled(equipment->Graphic, equipment->Color & 0x3FFF, tileX, tileY, width, height));

							drawed = true;
						}
					}
					
					if (!drawed)
					{
						tileX -= rect.Position.X - tileOffsetX;
						tileY -= rect.Position.Y - tileOffsetY;
						CGUITilepic *pic = new CGUITilepic(equipment->Graphic, equipment->Color & 0x3FFF, tileX, tileY);
						pic->PartialHue = equipment->IsPartialHue();
						m_DataBox->Add(pic);
						pic->Serial = slotSerial;
					}

					backgroundSlot->Serial = slotSerial;
					scopeSlot->Serial = slotSerial;
				}

				yPtr += 21;
			}
		}
	}

	equipment = obj->FindLayer(OL_BACKPACK);

	if (equipment != NULL && equipment->AnimID != 0)
	{
		int bpX = 8;

		if (g_PaperdollBooks)
			bpX = 2;

		bodyGumppic = (CGUIGumppic*)m_DataBox->Add(new CGUIGumppic(equipment->AnimID + 50000, bpX, 19));
		bodyGumppic->Color = equipment->Color & 0x3FFF;
		bodyGumppic->PartialHue = equipment->IsPartialHue();
		bodyGumppic->Serial = ID_GP_ITEMS + OL_BACKPACK;
		bodyGumppic->MoveOnDrag = true;
	}

	m_DataBox->Add(new CGUIShader(&g_ColorizerShader, false));
}
//----------------------------------------------------------------------------------
void CGumpPaperdoll::UpdateDescription(const string &text)
{
	WISPFUN_DEBUG("c105_f7");
	if (m_Description != NULL)
	{
		m_Description->CreateTextureA(1, text, 185);

		m_WantRedraw = true;
	}
}
//----------------------------------------------------------------------------------
void CGumpPaperdoll::Draw()
{
	WISPFUN_DEBUG("c105_f8");
	CGameCharacter *obj = g_World->FindWorldCharacter(m_Serial);

	if (obj == NULL)
		return;

	//if (g_LastObjectType == SOT_TEXT_OBJECT)
	//	g_GumpPressedElement = false;

	CGump::Draw();

	if (!m_Minimized)
	{
		glTranslatef(g_GumpTranslate.X, g_GumpTranslate.Y, 0.0f);

		g_FontColorizerShader.Use();

		m_TextRenderer.Draw();

		UnuseShader();

		glTranslatef(-g_GumpTranslate.X, -g_GumpTranslate.Y, 0.0f);
	}
}
//----------------------------------------------------------------------------------
CRenderObject *CGumpPaperdoll::Select()
{
	WISPFUN_DEBUG("c105_f9");
	CGameCharacter *obj = g_World->FindWorldCharacter(m_Serial);

	if (obj == NULL)
		return NULL;

	CRenderObject *selected = CGump::Select();

	if (!m_Minimized)
	{
		WISP_GEOMETRY::CPoint2Di oldPos = g_MouseManager.Position;
		g_MouseManager.Position = WISP_GEOMETRY::CPoint2Di(oldPos.X - (int)g_GumpTranslate.X, oldPos.Y - (int)g_GumpTranslate.Y);

		m_TextRenderer.Select(this);

		g_MouseManager.Position = oldPos;
	}

	return selected;
}
//----------------------------------------------------------------------------------
void CGumpPaperdoll::GUMP_BUTTON_EVENT_C
{
	WISPFUN_DEBUG("c105_f10");
	switch (serial)
	{
		case ID_GP_BUTTON_HELP: //Paperdoll button Help
		{
			CPacketHelpRequest().Send();
			break;
		}
		case ID_GP_BUTTON_OPTIONS: //Paperdoll button Options
		{
			g_Orion.OpenConfiguration();
			break;
		}
		case ID_GP_BUTTON_LOGOUT: //Paperdoll button Log Out
		{
			g_Orion.OpenLogOut();
			break;
		}
		case ID_GP_BUTTON_JOURNAL_OR_QUESTS: //Paperdoll button Journal
		{
			if (g_PacketManager.ClientVersion >= CV_500A)
				CPacketQuestMenuRequest().Send();
			else
				g_Orion.OpenJournal();
			break;
		}
		case ID_GP_BUTTON_SKILLS: //Paperdoll button Skills
		{
			g_Orion.OpenSkills();
			break;
		}
		case ID_GP_BUTTON_CHAT_OR_GUILD: //Paperdoll button Chat
		{
			if (g_PacketManager.ClientVersion >= CV_500A)
				CPacketGuildMenuRequest().Send();
			else
				g_Orion.OpenChat();
			break;
		}
		case ID_GP_BUTTON_WARMODE: //Paperdoll button Peace/War
		{
			g_Orion.ChangeWarmode();
			break;
		}
		case ID_GP_BUTTON_STATUS: //Paperdoll button Status
		{
			g_Orion.OpenStatus(m_Serial);
			g_MouseManager.LastLeftButtonClickTimer = 0;
			break;
		}
		case ID_GP_BUTTON_MINIMIZE: //Paperdoll button Minimize
		{
			m_Minimized = true;
			break;
		}
		case ID_GP_LOCK_MOVING:
		{
			m_LockMoving = !m_LockMoving;
			g_MouseManager.CancelDoubleClick = true;
			break;
		}
		case ID_GP_PROFILE_SCROLL:
		case ID_GP_PARTY_MANIFEST_SCROLL:
		{
			if (!g_ClickObject.Enabled)
			{
				g_ClickObject.Init(g_PressedObject.LeftObject, this);
				g_ClickObject.Timer = g_Ticks + g_MouseManager.DoubleClickDelay;
			}

			break;
		}
		case ID_GP_COMBAT_BOOK:
		case ID_GP_RACIAL_ABILITIES_BOOK:
		{
			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------------
void CGumpPaperdoll::OnLeftMouseButtonUp()
{
	WISPFUN_DEBUG("c105_f11");
	CGump::OnLeftMouseButtonUp();

	uint serial = g_SelectedObject.Serial;

	CGameCharacter *container = g_World->FindWorldCharacter(m_Serial);

	if (container == NULL && serial >= ID_GP_ITEMS)
		return;

	//Что-то в руке
	if ((!serial || serial >= ID_GP_ITEMS) && g_ObjectInHand.Enabled)
	{
		int layer = serial - ID_GP_ITEMS;
		bool canWear = true;

		if (layer != OL_BACKPACK && m_Serial != g_PlayerSerial && GetDistance(g_Player, container) >= DRAG_ITEMS_DISTANCE)
			canWear = false;

		if (canWear && container != NULL)
		{
			if (layer == OL_BACKPACK) //Ткнули на пак
			{
				CGameItem *equipment = container->FindLayer(layer);

				if (equipment != NULL)
				{
					if (m_Serial != g_PlayerSerial)
						g_Orion.DropItem(container->Serial, 0xFFFF, 0xFFFF, 0);
					else
						g_Orion.DropItem(equipment->Serial, 0xFFFF, 0xFFFF, 0);

					g_MouseManager.LeftDropPosition = g_MouseManager.Position;
					g_MouseManager.CancelDoubleClick = true;

					m_FrameCreated = false;

					return;
				}
			}
			else if (IsWearable(g_ObjectInHand.TiledataPtr->Flags)) //Можно одевать
			{
				CGameItem *equipment = container->FindLayer(g_ObjectInHand.TiledataPtr->Layer);

				if (equipment == NULL) //На этом слое ничего нет
				{
					if (m_Serial != g_PlayerSerial)
						g_Orion.EquipItem(container->Serial);
					else
						g_Orion.EquipItem();

					g_MouseManager.LeftDropPosition = g_MouseManager.Position;
					g_MouseManager.CancelDoubleClick = true;

					m_FrameCreated = false;

					return;
				}
			}
		}
		else
			g_Orion.PlaySoundEffect(0x0051);
	}
	
	if (g_PressedObject.LeftSerial == serial && serial >= ID_GP_ITEMS && !g_ObjectInHand.Enabled)
	{
		int layer = serial - ID_GP_ITEMS;
		CGameItem *equipment = container->FindLayer(layer);

		if (equipment != NULL)
		{
			if (g_Target.IsTargeting())
			{
				g_Target.SendTargetObject(equipment->Serial);
				g_MouseManager.CancelDoubleClick = true;
			}
			else //Click on object
			{
				if (!g_ClickObject.Enabled)
				{
					g_ClickObject.Init(equipment);
					g_ClickObject.Timer = g_Ticks + g_MouseManager.DoubleClickDelay;
					g_ClickObject.X = g_MouseManager.Position.X - X;
					g_ClickObject.Y = g_MouseManager.Position.Y - Y;
				}
			}
		}
	}
}
//----------------------------------------------------------------------------------
bool CGumpPaperdoll::OnLeftMouseButtonDoubleClick()
{
	WISPFUN_DEBUG("c105_f12");
	if (m_Minimized)
	{
		m_Minimized = false;
		m_Page = 2;
		m_WantRedraw = true;

		return true;
	}

	bool result = false;
	uint serial = g_PressedObject.LeftSerial;

	if (serial < ID_GP_ITEMS)
	{
		if (serial == ID_GP_PROFILE_SCROLL)
		{
			g_Orion.OpenProfile(m_Serial);

			result = true;
		}
		else if (serial == ID_GP_PARTY_MANIFEST_SCROLL)
		{
			g_Orion.OpenPartyManifest();

			result = true;
		}
		else if (serial == ID_GP_BUTTON_VIRTURE)
		{
			CPacketVirtureRequest(1).Send();

			result = true;
		}
		else if (serial == ID_GP_COMBAT_BOOK)
		{
			g_Orion.OpenCombatBookGump();

			result = true;
		}
		else if (serial == ID_GP_RACIAL_ABILITIES_BOOK)
		{
			g_Orion.OpenRacialAbilitiesBookGump();

			result = true;
		}
	}
	else
	{
		CGameCharacter *container = g_World->FindWorldCharacter(m_Serial);

		if (container != NULL)
		{
			int layer = serial - ID_GP_ITEMS;

			CGameItem *equipment = container->FindLayer(layer);

			if (equipment != NULL)
			{
				g_Orion.DoubleClick(equipment->Serial);

				result = true;
			}
		}
	}

	return result;
}
//----------------------------------------------------------------------------------
