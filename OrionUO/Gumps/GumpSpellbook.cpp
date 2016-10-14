/***********************************************************************************
**
** GumpSpellbook.cpp
**
** Copyright (C) September 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GumpSpellbook.h"
#include "GumpSpell.h"
#include "../Game objects/GameWorld.h"
#include "../PressedObject.h"
#include "../ClickObject.h"
#include "../OrionUO.h"
#include "../Managers/MouseManager.h"
#include "../Managers/GumpManager.h"
#include "../Managers/ClilocManager.h"
#include "../OrionWindow.h"
#include "../ToolTip.h"
//----------------------------------------------------------------------------------
ushort CGumpSpellbook::m_ReagentsIndex[8] = 
{
	0x0F7A, 0x0F7B, 0x0F84, 0x0F85,
	0x0F86, 0x0F88, 0x0F8C, 0x0F8D
};
//----------------------------------------------------------------------------------
CGumpSpellbook::CGumpSpellbook(uint serial, int x, int y)
: CGump(GT_SPELLBOOK, serial, x, y), m_PageCount(8), m_SpellCount(0), m_Body(NULL),
m_PrevPage(NULL), m_NextPage(NULL), m_LastSpellPointer(NULL), m_LastSpellBookmark(NULL),
m_BookType(ST_NONE)
{
	m_Draw2Page = true;
	memset(&m_Spells[0], 0, sizeof(m_Spells));
}
//----------------------------------------------------------------------------------
CGumpSpellbook::~CGumpSpellbook()
{
}
//----------------------------------------------------------------------------------
void CGumpSpellbook::UpdateGraphic(const ushort &parentGraphic)
{
	SPELLBOOK_TYPE bookType = m_BookType;

	switch (parentGraphic)
	{
		case 0x0EFA:
		{
			m_BookType = ST_MAGE;
			break;
		}
		case 0x2253:
		{
			m_BookType = ST_NECRO;
			break;
		}
		case 0x2252:
		{
			m_BookType = ST_PALADIN;
			break;
		}
		case 0x238C:
		{
			m_BookType = ST_BUSHIDO;
			break;
		}
		case 0x23A0:
		{
			m_BookType = ST_NINJITSU;
			break;
		}
		case 0x2D50:
		{
			m_BookType = ST_SPELL_WEAVING;
			break;
		}
		/*case 0x????:
		{
			m_BookType = ST_MYSTICISM;
			break;
		}*/
		default:
		{
			m_BookType = ST_NONE;
			break;
		}
	}

	if (bookType != m_BookType)
		m_WantUpdateContent = true;
}
//----------------------------------------------------------------------------------
int CGumpSpellbook::GetReagentIndex(const ushort &id)
{
	switch (id)
	{
		case 0x0F7A:
			return 0;
		case 0x0F7B:
			return 1;
		case 0x0F84:
			return 2;
		case 0x0F85:
			return 3;
		case 0x0F86:
			return 4;
		case 0x0F88:
			return 5;
		case 0x0F8C:
			return 6;
		case 0x0F8D:
			return 7;
		default:
			break;
	}

	return 0;
}
//----------------------------------------------------------------------------------
void CGumpSpellbook::InitToolTip()
{
	if (m_Minimized)
	{
		g_ToolTip.Set(L"Double click to maximize book gump", g_SelectedObject.Object());
		return;
	}

	switch (m_BookType)
	{
		case ST_MAGE:
		{
			if (m_Page >= 8)
			{
				uint serial = g_SelectedObject.Serial;

				if (serial >= ID_GSB_SPELL_ICON_LEFT)
				{
					if (serial >= ID_GSB_SPELL_ICON_RIGHT)
						serial -= ID_GSB_SPELL_ICON_RIGHT;
					else
						serial -= ID_GSB_SPELL_ICON_LEFT;

					g_ToolTip.Set(g_ClilocManager.Cliloc(g_Language)->GetW(1061290 + serial), g_SelectedObject.Object(), 150);
				}
			}

			break;
		}
		case ST_NECRO:
		{
			break;
		}
		case ST_PALADIN:
		{
			break;
		}
		case ST_BUSHIDO:
		{
			break;
		}
		case ST_NINJITSU:
		{
			break;
		}
		case ST_SPELL_WEAVING:
		{
			break;
		}
		case ST_MYSTICISM:
		{
			break;
		}
	}
}
//----------------------------------------------------------------------------
void CGumpSpellbook::PrepareContent()
{
	switch (m_BookType)
	{
		case ST_MAGE:
		{
			if (g_PressedObject.LeftGump() == this && m_Page >= 8 && g_PressedObject.LeftSerial >= ID_GSB_SPELL_ICON_LEFT)
			{
				WISP_GEOMETRY::CPoint2Di offset = g_MouseManager.LeftDroppedOffset();

				if ((abs(offset.X) >= DRAG_PIXEL_RANGE || abs(offset.Y) >= DRAG_PIXEL_RANGE) || (g_MouseManager.LastLeftButtonClickTimer + g_MouseManager.DoubleClickDelay < g_Ticks))
				{
					int index = g_PressedObject.LeftSerial;

					if (index < ID_GSB_SPELL_ICON_RIGHT)
						index -= ID_GSB_SPELL_ICON_LEFT;
					else
						index -= ID_GSB_SPELL_ICON_RIGHT;

					g_GumpManager.AddGump(new CGumpSpell(index + 1, g_MouseManager.Position.X - 20, g_MouseManager.Position.Y - 20, 0x08C0 + index));

					g_OrionWindow.EmulateOnLeftMouseButtonDown();
				}
			}

			if (!m_Minimized && m_LastSpellPointer != NULL)
			{
				bool wantVisible = false;
				ushort graphicBookmark = 0x08AD;
				ushort graphicPointer = 0x08AF;
				int wantX = m_LastSpellPointer->X;
				int wantY = m_LastSpellPointer->Y;

				if (m_Page < 8)
				{
					IFOR(j, 0, 2)
					{
						int y = 0;

						IFOR(i, 0, 8)
						{
							int offs = i + (8 * (m_Page + j));

							if (m_Spells[offs])
							{
								if (offs == g_LastSpellIndex - 1)
								{
									wantVisible = true;
									wantY = 52 + y;

									if ((m_Page + j) % 2)
									{
										graphicBookmark = 0x08AE;
										graphicPointer = 0x08B0;
										wantX = 203;
									}
									else
										wantX = 184;

									break;
								}

								y += 15;
							}
						}
					}
				}
				else
				{
					int page = 8;

					IFOR(i, 0, 64)
					{
						if (!m_Spells[i])
							continue;

						if (page == m_Page || page == (m_Page + 1))
						{
							if (i == g_LastSpellIndex - 1)
							{
								wantVisible = true;
								wantY = 40;

								if (page % 2)
								{
									graphicBookmark = 0x08AE;
									graphicPointer = 0x08B0;
									wantX = 203;
								}
								else
									wantX = 184;

								break;
							}
						}

						page++;
					}
				}

				if (wantVisible != m_LastSpellPointer->Visible || wantX != m_LastSpellPointer->X || wantY != m_LastSpellPointer->Y)
				{
					m_LastSpellPointer->Visible = wantVisible;
					m_LastSpellPointer->Graphic = graphicPointer;
					m_LastSpellPointer->X = wantX;
					m_LastSpellPointer->Y = wantY;

					m_LastSpellBookmark->Visible = wantVisible;
					m_LastSpellBookmark->Graphic = graphicBookmark;
					m_LastSpellBookmark->X = wantX;

					m_WantRedraw = true;
				}
			}

			break;
		}
		case ST_NECRO:
		{
			break;
		}
		case ST_PALADIN:
		{
			break;
		}
		case ST_BUSHIDO:
		{
			break;
		}
		case ST_NINJITSU:
		{
			break;
		}
		case ST_SPELL_WEAVING:
		{
			break;
		}
		case ST_MYSTICISM:
		{
			break;
		}
	}
}
//----------------------------------------------------------------------------
void CGumpSpellbook::UpdateContentMage()
{
	if (m_Minimized)
	{
		m_Body->Graphic = 0x08BA;

		return;
	}
	else
		m_Body->Graphic = 0x08AC;

	CGameItem *spellbook = g_World->FindWorldItem(m_Serial);

	if (spellbook == NULL)
		return;

	m_SpellCount = 0;
	memset(&m_Spells[0], 0, sizeof(m_Spells));

	QFOR(item, spellbook->m_Items, CGameItem*)
	{
		int currentCount = item->Count;

		if (currentCount > 0 && currentCount <= 64)
		{
			m_Spells[currentCount - 1] = 1;
			m_SpellCount++;
		}
	}

	m_PageCount = 8 + m_SpellCount;

	//if (m_SpellCount % 2)
	//	m_PageCount--;

	Add(new CGUIButton(ID_GSB_BUTTON_CIRCLE_1_2, 0x08B1, 0x08B1, 0x08B1, 58, 175));
	Add(new CGUIButton(ID_GSB_BUTTON_CIRCLE_1_2, 0x08B2, 0x08B2, 0x08B2, 93, 175));
	Add(new CGUIButton(ID_GSB_BUTTON_CIRCLE_3_4, 0x08B3, 0x08B3, 0x08B3, 130, 175));
	Add(new CGUIButton(ID_GSB_BUTTON_CIRCLE_3_4, 0x08B4, 0x08B4, 0x08B4, 164, 175));

	Add(new CGUIButton(ID_GSB_BUTTON_CIRCLE_5_6, 0x08B5, 0x08B5, 0x08B5, 227, 175));
	Add(new CGUIButton(ID_GSB_BUTTON_CIRCLE_5_6, 0x08B6, 0x08B6, 0x08B6, 260, 175));
	Add(new CGUIButton(ID_GSB_BUTTON_CIRCLE_7_8, 0x08B7, 0x08B7, 0x08B7, 297, 175));
	Add(new CGUIButton(ID_GSB_BUTTON_CIRCLE_7_8, 0x08B8, 0x08B8, 0x08B8, 332, 175));

	IFOR(page, 0, 8)
	{
		Add(new CGUIPage(page));

		int indexX = 106;
		int dataX = 62;
		int y = 0;
		uint spellSerial = ID_GSB_SPELL_ICON_LEFT;

		if (page % 2)
		{
			indexX = 269;
			dataX = 225;
			spellSerial = ID_GSB_SPELL_ICON_RIGHT;
		}

		CGUIText *text = (CGUIText*)Add(new CGUIText(0x0288, indexX, 10));
		text->CreateTextureA(6, "INDEX");

		text = (CGUIText*)Add(new CGUIText(0x0288, dataX, 30));
		text->CreateTextureA(6, SPELL_CIRCLES_NAMES[page]);

		IFOR(i, 0, 8)
		{
			int offs = i + (8 * page);

			if (m_Spells[offs])
			{
				ushort id = 0x2080 + offs;

				CGUITextEntry *entry = (CGUITextEntry*)Add(new CGUITextEntry(spellSerial + offs, 0x0288, 0, 0, dataX, 52 + y, 0, false, 9));
				entry->m_Entry.SetText(g_Orion.m_StaticData[id / 32].Tiles[id % 32].Name);
				entry->CheckOnSerial = true;
				entry->ReadOnly = true;

				CGUIHitBox *box = (CGUIHitBox*)Add(new CGUIHitBox(spellSerial + offs, dataX, 52 + y, 100, 16, true));
				box->MoveOnDrag = true;

				y += 15;
			}
		}
	}

	int page = 8;

	IFOR(i, 0, 64)
	{
		if (!m_Spells[i])
			continue;

		int iconX = 62;
		int textCircleX = 87;
		int textNameX = 112;
		int abbreviatureX = 112;
		int lineX = 62;
		int reagentsX = 62;
		uint iconSerial = ID_GSB_SPELL_ICON_LEFT + i;

		if (page % 2)
		{
			iconX = 225;
			textCircleX = 244;
			textNameX = 275;
			abbreviatureX = 275;
			lineX = 225;
			reagentsX = 225;
			iconSerial = ID_GSB_SPELL_ICON_RIGHT + i;
		}

		Add(new CGUIPage(page));
		page++;

		CGUIText *text = (CGUIText*)Add(new CGUIText(0x0288, textCircleX, 10));
		text->CreateTextureA(6, SPELL_CIRCLES_NAMES[i / 8]);

		CGUIGumppic *icon = (CGUIGumppic*)Add(new CGUIGumppic(0x08C0 + i, iconX, 40));
		icon->Serial = iconSerial;

		ushort id = 0x2080 + i;

		text = (CGUIText*)Add(new CGUIText(0x0288, textNameX, 34));
		text->CreateTextureA(6, g_Orion.m_StaticData[id / 32].Tiles[id % 32].Name, 80);

		int abbreviatureY = 26;

		if (text->m_Texture.Height < 24)
			abbreviatureY = 31;

		abbreviatureY += text->m_Texture.Height;

		text = (CGUIText*)Add(new CGUIText(0x0288, abbreviatureX, abbreviatureY));
		text->CreateTextureA(8, SPELL_CAST_ABBREVIATURE[i]);

		Add(new CGUIGumppicTiled(0x0835, lineX, 88, 120, 0));

		text = (CGUIText*)Add(new CGUIText(0x0288, reagentsX, 92));
		text->CreateTextureA(6, "Reagents:");

		IFOR(j, 0, 4)
		{
			id = SPELL_REAGENTS[i][j];

			if (!id)
				break;

			text = (CGUIText*)Add(new CGUIText(0x0288, reagentsX, 114 + (j * 14)));
			text->CreateTextureA(6, GetReagentName(m_ReagentsIndex[GetReagentIndex(id)]));
		}
	}
}
//----------------------------------------------------------------------------
void CGumpSpellbook::UpdateContentNecro()
{
	if (m_Minimized)
	{
		m_Body->Graphic = 0x2B03;

		return;
	}
	else
		m_Body->Graphic = 0x2B00;

	CGameItem *spellbook = g_World->FindWorldItem(m_Serial);

	if (spellbook == NULL)
		return;
}
//----------------------------------------------------------------------------
void CGumpSpellbook::UpdateContentPaladin()
{
	if (m_Minimized)
	{
		m_Body->Graphic = 0x2B04;

		return;
	}
	else
		m_Body->Graphic = 0x2B01;

	CGameItem *spellbook = g_World->FindWorldItem(m_Serial);

	if (spellbook == NULL)
		return;
}
//----------------------------------------------------------------------------
void CGumpSpellbook::UpdateContentBushido()
{
	if (m_Minimized)
	{
		m_Body->Graphic = 0x2B09;

		return;
	}
	else
		m_Body->Graphic = 0x2B07;

	CGameItem *spellbook = g_World->FindWorldItem(m_Serial);

	if (spellbook == NULL)
		return;
}
//----------------------------------------------------------------------------
void CGumpSpellbook::UpdateContentNinjitsu()
{
	if (m_Minimized)
	{
		m_Body->Graphic = 0x2B08;

		return;
	}
	else
		m_Body->Graphic = 0x2B06;

	CGameItem *spellbook = g_World->FindWorldItem(m_Serial);

	if (spellbook == NULL)
		return;
}
//----------------------------------------------------------------------------
void CGumpSpellbook::UpdateContentSpellWeaving()
{
	if (m_Minimized)
	{
		m_Body->Graphic = 0x2B2D;

		return;
	}
	else
		m_Body->Graphic = 0x2B2F;

	CGameItem *spellbook = g_World->FindWorldItem(m_Serial);

	if (spellbook == NULL)
		return;
}
//----------------------------------------------------------------------------
void CGumpSpellbook::UpdateContentMysticism()
{
}
//----------------------------------------------------------------------------
void CGumpSpellbook::UpdateContent()
{
	m_Body = NULL;
	m_PrevPage = NULL;
	m_NextPage = NULL;
	m_LastSpellPointer = NULL;
	m_LastSpellBookmark = NULL;

	Clear();

	Add(new CGUIPage(-1));

	if (m_Minimized)
		m_Body = (CGUIGumppic*)Add(new CGUIGumppic(0x08BA, 0, 0));
	else
	{
		m_Body = (CGUIGumppic*)Add(new CGUIGumppic(0x08AC, 0, 0));

		Add(new CGUIHitBox(ID_GSB_BUTTON_MINIMIZE, 6, 100, 16, 16, true));
	}

	switch (m_BookType)
	{
		case ST_MAGE:
		{
			UpdateContentMage();
			break;
		}
		case ST_NECRO:
		{
			UpdateContentNecro();
			break;
		}
		case ST_PALADIN:
		{
			UpdateContentPaladin();
			break;
		}
		case ST_BUSHIDO:
		{
			UpdateContentBushido();
			break;
		}
		case ST_NINJITSU:
		{
			UpdateContentNinjitsu();
			break;
		}
		case ST_SPELL_WEAVING:
		{
			UpdateContentSpellWeaving();
			break;
		}
		case ST_MYSTICISM:
		{
			UpdateContentMysticism();
			break;
		}
	}

	if (!m_Minimized)
	{
		Add(new CGUIPage(-1));

		m_PrevPage = (CGUIButton*)Add(new CGUIButton(ID_GSB_BUTTON_PREV, 0x08BB, 0x08BB, 0x08BB, 50, 8));
		m_PrevPage->Visible = (m_Page != 0);
		m_NextPage = (CGUIButton*)Add(new CGUIButton(ID_GSB_BUTTON_NEXT, 0x08BC, 0x08BC, 0x08BC, 321, 8));
		m_NextPage->Visible = (m_Page + 2 < m_PageCount);

		m_LastSpellBookmark = (CGUIGumppic*)Add(new CGUIGumppic(0x08AD, 184, 2));
		m_LastSpellPointer = (CGUIGumppic*)Add(new CGUIGumppic(0x08AF, 184, 52));
	}
}
//----------------------------------------------------------------------------
void CGumpSpellbook::GUMP_BUTTON_EVENT_C
{
	int newPage = -1;

	if (serial == ID_GSB_BUTTON_PREV)
	{
		if (m_Page > 0)
		{
			newPage = m_Page - 2;

			if (newPage < 0)
				newPage = 0;
		}
	}
	else if (serial == ID_GSB_BUTTON_NEXT)
	{
		if (m_Page < m_PageCount)
		{
			newPage = m_Page + 2;

			if (newPage >= m_PageCount)
				newPage = m_PageCount - 1;
		}
	}
	else if (serial == ID_GSB_BUTTON_CIRCLE_1_2)
		ChangePage(0);
	else if (serial == ID_GSB_BUTTON_CIRCLE_3_4)
		ChangePage(2);
	else if (serial == ID_GSB_BUTTON_CIRCLE_5_6)
		ChangePage(4);
	else if (serial == ID_GSB_BUTTON_CIRCLE_7_8)
		ChangePage(6);
	else if (serial == ID_GSB_BUTTON_MINIMIZE)
	{
		m_Minimized = true;
		m_WantUpdateContent = true;
	}
	else if (serial == ID_GSB_LOCK_MOVING)
		m_LockMoving = !m_LockMoving;
	else if (serial >= ID_GSB_SPELL_ICON_LEFT)
	{
		switch (m_BookType)
		{
			case ST_MAGE:
			{
				if (m_Page < 8) //List of spells
				{
					IFOR(j, 0, 2)
					{
						IFOR(i, 0, 8)
						{
							int offs = i + (8 * (m_Page + j));

							if (m_Spells[offs])
							{
								if (serial == offs + ID_GSB_SPELL_ICON_LEFT || serial == offs + ID_GSB_SPELL_ICON_RIGHT)
								{
									int c = 8;

									IFOR(j, 0, 64)
									{
										if (m_Spells[j])
										{
											if (j == offs)
												break;

											c++;
										}
									}

									newPage = c;

									break;
								}
							}
						}
					}
				}

				break;
			}
			case ST_NECRO:
			{
				break;
			}
			case ST_PALADIN:
			{
				break;
			}
			case ST_BUSHIDO:
			{
				break;
			}
			case ST_NINJITSU:
			{
				break;
			}
			case ST_SPELL_WEAVING:
			{
				break;
			}
			case ST_MYSTICISM:
			{
				break;
			}
		}
	}

	if (newPage > -1 && !g_ClickObject.Enabled)
	{
		if (newPage % 2)
			newPage--;

		g_ClickObject.Init(g_PressedObject.LeftObject(), this);
		g_ClickObject.Timer = g_Ticks + g_MouseManager.DoubleClickDelay;
		g_ClickObject.Page = newPage;
	}
}
//----------------------------------------------------------------------------
bool CGumpSpellbook::OnLeftMouseButtonDoubleClick()
{
	bool result = false;

	if (m_Minimized)
	{
		m_Minimized = false;
		m_WantUpdateContent = true;

		result = true;
	}
	else
	{
		if (g_PressedObject.LeftSerial == ID_GSB_BUTTON_PREV)
		{
			ChangePage(0);

			m_WantRedraw = true;

			result = true;
		}
		else if (g_PressedObject.LeftSerial == ID_GSB_BUTTON_NEXT)
		{
			int newPage = m_PageCount - 1;

			if (newPage % 2)
				newPage--;

			ChangePage(newPage);

			m_WantRedraw = true;

			result = true;
		}
		else
		{
			switch (m_BookType)
			{
				case ST_MAGE:
				{
					if (g_PressedObject.LeftSerial >= ID_GSB_SPELL_ICON_LEFT)
					{
						//Было использовано заклинание
						if (g_PressedObject.LeftSerial < ID_GSB_SPELL_ICON_RIGHT)
							g_Orion.CastSpellFromBook(g_PressedObject.LeftSerial - ID_GSB_SPELL_ICON_LEFT + 1, m_Serial);
						else
							g_Orion.CastSpellFromBook(g_PressedObject.LeftSerial - ID_GSB_SPELL_ICON_RIGHT + 1, m_Serial);

						//Сворачиваем книгу
						m_Minimized = true;
						m_WantUpdateContent = true;

						result = true;
					}

					break;
				}
				case ST_NECRO:
				{
					break;
				}
				case ST_PALADIN:
				{
					break;
				}
				case ST_BUSHIDO:
				{
					break;
				}
				case ST_NINJITSU:
				{
					break;
				}
				case ST_SPELL_WEAVING:
				{
					break;
				}
				case ST_MYSTICISM:
				{
					break;
				}
			}
		}
	}

	return result;
}
//----------------------------------------------------------------------------------
void CGumpSpellbook::DelayedClick(CRenderObject *obj)
{
	if (obj != NULL)
	{
		ChangePage(g_ClickObject.Page);
		m_WantRedraw = true;
	}
}
//----------------------------------------------------------------------------------
void CGumpSpellbook::ChangePage(int newPage)
{
	m_Page = newPage;

	m_PrevPage->Visible = (m_Page != 0);
	m_NextPage->Visible = (m_Page + 2 < m_PageCount);

	g_Orion.PlaySoundEffect(0x0055);
}
//----------------------------------------------------------------------------