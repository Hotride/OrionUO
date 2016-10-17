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
CGumpSpellbook::CGumpSpellbook(uint serial, int x, int y)
: CGump(GT_SPELLBOOK, serial, x, y), m_PageCount(8), m_SpellCount(0), m_Body(NULL),
m_PrevPage(NULL), m_NextPage(NULL), m_LastSpellPointer(NULL), m_LastSpellBookmark(NULL),
m_BookType(ST_MAGE)
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
			m_BookType = ST_MAGE;
			break;
		}
	}

	if (bookType != m_BookType)
		m_WantUpdateContent = true;
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

					IFOR(i, 0, SPELLBOOK_1_SPELLS_COUNT)
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
void CGumpSpellbook::GetSummaryBookInfo(int &maxSpellsCount, int &dictionaryPagesCount, int &spellsOnPage, ushort &graphic, ushort &minimizedGraphic)
{
	switch (m_BookType)
	{
		case ST_MAGE:
		{
			maxSpellsCount = SPELLBOOK_1_SPELLS_COUNT;
			dictionaryPagesCount = 8;
			spellsOnPage = 8;
			graphic = 0x08AC;
			minimizedGraphic = 0x08BA;

			break;
		}
		case ST_NECRO:
		{
			maxSpellsCount = SPELLBOOK_2_SPELLS_COUNT;
			dictionaryPagesCount = 4;
			spellsOnPage = 8;
			graphic = 0x2B00;
			minimizedGraphic = 0x2B03;

			break;
		}
		case ST_PALADIN:
		{
			maxSpellsCount = SPELLBOOK_3_SPELLS_COUNT;
			dictionaryPagesCount = 2;
			spellsOnPage = 5;
			graphic = 0x2B01;
			minimizedGraphic = 0x2B04;

			break;
		}
		case ST_BUSHIDO:
		{
			maxSpellsCount = SPELLBOOK_4_SPELLS_COUNT;
			dictionaryPagesCount = 2;
			spellsOnPage = 3;
			graphic = 0x2B07;
			minimizedGraphic = 0x2B09;

			break;
		}
		case ST_NINJITSU:
		{
			maxSpellsCount = SPELLBOOK_5_SPELLS_COUNT;
			dictionaryPagesCount = 2;
			spellsOnPage = 4;
			graphic = 0x2B06;
			minimizedGraphic = 0x2B08;

			break;
		}
		case ST_SPELL_WEAVING:
		{
			maxSpellsCount = SPELLBOOK_6_SPELLS_COUNT;
			dictionaryPagesCount = 2;
			spellsOnPage = 8;
			graphic = 0x2B2F;
			minimizedGraphic = 0x2B2D;

			break;
		}
		case ST_MYSTICISM: //?
		{
			maxSpellsCount = SPELLBOOK_7_SPELLS_COUNT;
			dictionaryPagesCount = 2;
			spellsOnPage = 8;
			graphic = 0;
			minimizedGraphic = 0;

			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------
string CGumpSpellbook::GetSpellName(const int &offset)
{
	switch (m_BookType)
	{
		case ST_MAGE:
			return m_SpellName1[offset];
		case ST_NECRO:
			return m_SpellName2[offset];
		case ST_PALADIN:
			return m_SpellName3[offset];
		case ST_BUSHIDO:
			return m_SpellName4[offset];
		case ST_NINJITSU:
			return m_SpellName5[offset];
		case ST_SPELL_WEAVING:
			return m_SpellName6[offset];
		case ST_MYSTICISM:
			return m_SpellName7[offset];
		default:
			break;
	}

	return "";
}
//----------------------------------------------------------------------------
void CGumpSpellbook::UpdateContentMage(const int &maxSpellsCount, const int &dictionaryPagesCount)
{
	int page = dictionaryPagesCount;

	IFOR(i, 0, maxSpellsCount)
	{
		if (!m_Spells[i])
			continue;

		int iconX = 62;
		int textCircleX = 87;
		int textNameX = 112;
		int abbreviatureX = 112;
		uint iconSerial = ID_GSB_SPELL_ICON_LEFT + i;

		if (page % 2)
		{
			iconX = 225;
			textCircleX = 244;
			textNameX = 275;
			abbreviatureX = 275;
			iconSerial = ID_GSB_SPELL_ICON_RIGHT + i;
		}

		Add(new CGUIPage(page));
		page++;

		CGUIText *text = (CGUIText*)Add(new CGUIText(0x0288, textCircleX, 10));
		text->CreateTextureA(6, SPELL_CIRCLES_NAMES[i / 8]);

		CGUIGumppic *icon = (CGUIGumppic*)Add(new CGUIGumppic(0x08C0 + i, iconX, 40));
		icon->Serial = iconSerial;

		text = (CGUIText*)Add(new CGUIText(0x0288, textNameX, 34));
		text->CreateTextureA(6, m_SpellName1[i], 80);

		int abbreviatureY = 26;

		if (text->m_Texture.Height < 24)
			abbreviatureY = 31;

		abbreviatureY += text->m_Texture.Height;

		text = (CGUIText*)Add(new CGUIText(0x0288, abbreviatureX, abbreviatureY));
		text->CreateTextureA(8, SPELL_CAST_ABBREVIATURE[i]);

		Add(new CGUIGumppicTiled(0x0835, iconX, 88, 120, 0));

		text = (CGUIText*)Add(new CGUIText(0x0288, iconX, 92));
		text->CreateTextureA(6, "Reagents:");

		text = (CGUIText*)Add(new CGUIText(0x0288, iconX, 114));
		text->CreateTextureA(9, m_SpellReagents1[i]);
	}
}
//----------------------------------------------------------------------------
void CGumpSpellbook::UpdateContentNecro(const int &maxSpellsCount, const int &dictionaryPagesCount)
{
	int page = dictionaryPagesCount;

	IFOR(i, 0, maxSpellsCount)
	{
		if (!m_Spells[i])
			continue;

		int iconX = 62;
		int textNameX = 87;
		int abbreviatureX = 112;
		uint iconSerial = ID_GSB_SPELL_ICON_LEFT + i;

		if (page % 2)
		{
			iconX = 225;
			textNameX = 244;
			abbreviatureX = 275;
			iconSerial = ID_GSB_SPELL_ICON_RIGHT + i;
		}

		Add(new CGUIPage(page));
		page++;

		CGUIText *text = (CGUIText*)Add(new CGUIText(0x0288, textNameX, 10));
		text->CreateTextureA(6, m_SpellName2[i], 100);

		CGUIGumppic *icon = (CGUIGumppic*)Add(new CGUIGumppic(0x5000 + i, iconX, 40));
		icon->Serial = iconSerial;

		text = (CGUIText*)Add(new CGUIText(0x0288, abbreviatureX, 34));
		text->CreateTextureA(8, SPELL_CAST_ABBREVIATURE[i]);

		Add(new CGUIGumppicTiled(0x0835, iconX, 88, 120, 0));

		text = (CGUIText*)Add(new CGUIText(0x0288, iconX, 92));
		text->CreateTextureA(6, "Reagents:");

		text = (CGUIText*)Add(new CGUIText(0x0288, iconX, 114));
		text->CreateTextureA(9, m_SpellReagents1[i]);

		text = (CGUIText*)Add(new CGUIText(0x0288, iconX, 162));
		text->CreateTextureA(6, "Mana Cost: 0\nMin. Skill: 0");
	}
}
//----------------------------------------------------------------------------
void CGumpSpellbook::UpdateContentPaladin(const int &maxSpellsCount, const int &dictionaryPagesCount)
{
	int page = dictionaryPagesCount;

	IFOR(i, 0, maxSpellsCount)
	{
		if (!m_Spells[i])
			continue;

		int iconX = 62;
		int textNameX = 87;
		int abbreviatureX = 112;
		uint iconSerial = ID_GSB_SPELL_ICON_LEFT + i;

		if (page % 2)
		{
			iconX = 225;
			textNameX = 244;
			abbreviatureX = 275;
			iconSerial = ID_GSB_SPELL_ICON_RIGHT + i;
		}

		Add(new CGUIPage(page));
		page++;

		CGUIText *text = (CGUIText*)Add(new CGUIText(0x0288, textNameX, 10));
		text->CreateTextureA(6, m_SpellName3[i], 100);

		CGUIGumppic *icon = (CGUIGumppic*)Add(new CGUIGumppic(0x5100 + i, iconX, 40));
		icon->Serial = iconSerial;

		text = (CGUIText*)Add(new CGUIText(0x0288, abbreviatureX, 34));
		text->CreateTextureA(8, SPELL_CAST_ABBREVIATURE[i]);

		text = (CGUIText*)Add(new CGUIText(0x0288, iconX, 148));
		text->CreateTextureA(6, "Tithing Cost: 0\nMana Cost: 0\nMin. Skill: 0");
	}
}
//----------------------------------------------------------------------------
void CGumpSpellbook::UpdateContentBushido(const int &maxSpellsCount, const int &dictionaryPagesCount)
{
	int page = dictionaryPagesCount;

	IFOR(i, 0, maxSpellsCount)
	{
		if (!m_Spells[i])
			continue;

		int iconX = 62;
		int textNameX = 87;
		uint iconSerial = ID_GSB_SPELL_ICON_LEFT + i;

		if (page % 2)
		{
			iconX = 225;
			textNameX = 244;
			iconSerial = ID_GSB_SPELL_ICON_RIGHT + i;
		}

		Add(new CGUIPage(page));
		page++;

		CGUIText *text = (CGUIText*)Add(new CGUIText(0x0288, textNameX, 10));
		text->CreateTextureA(6, m_SpellName4[i], 100);

		CGUIGumppic *icon = (CGUIGumppic*)Add(new CGUIGumppic(0x5200 + i, iconX, 40));
		icon->Serial = iconSerial;

		text = (CGUIText*)Add(new CGUIText(0x0288, iconX, 162));
		text->CreateTextureA(6, "Mana Cost: 0\nMin. Skill: 0");
	}
}
//----------------------------------------------------------------------------
void CGumpSpellbook::UpdateContentNinjitsu(const int &maxSpellsCount, const int &dictionaryPagesCount)
{
	int page = dictionaryPagesCount;

	IFOR(i, 0, maxSpellsCount)
	{
		if (!m_Spells[i])
			continue;

		int iconX = 62;
		int textNameX = 87;
		uint iconSerial = ID_GSB_SPELL_ICON_LEFT + i;

		if (page % 2)
		{
			iconX = 225;
			textNameX = 244;
			iconSerial = ID_GSB_SPELL_ICON_RIGHT + i;
		}

		Add(new CGUIPage(page));
		page++;

		CGUIText *text = (CGUIText*)Add(new CGUIText(0x0288, textNameX, 10));
		text->CreateTextureA(6, m_SpellName5[i], 100);

		CGUIGumppic *icon = (CGUIGumppic*)Add(new CGUIGumppic(0x5300 + i, iconX, 40));
		icon->Serial = iconSerial;

		text = (CGUIText*)Add(new CGUIText(0x0288, iconX, 162));
		text->CreateTextureA(6, "Mana Cost: 0\nMin. Skill: 0");
	}
}
//----------------------------------------------------------------------------
void CGumpSpellbook::UpdateContentSpellWeaving(const int &maxSpellsCount, const int &dictionaryPagesCount)
{
	int page = dictionaryPagesCount;

	IFOR(i, 0, maxSpellsCount)
	{
		if (!m_Spells[i])
			continue;

		int iconX = 62;
		int textNameX = 87;
		uint iconSerial = ID_GSB_SPELL_ICON_LEFT + i;

		if (page % 2)
		{
			iconX = 225;
			textNameX = 244;
			iconSerial = ID_GSB_SPELL_ICON_RIGHT + i;
		}

		Add(new CGUIPage(page));
		page++;

		CGUIText *text = (CGUIText*)Add(new CGUIText(0x0288, textNameX, 10));
		text->CreateTextureA(6, m_SpellName6[i], 100);

		CGUIGumppic *icon = (CGUIGumppic*)Add(new CGUIGumppic(0x5400 + i, iconX, 40));
		icon->Serial = iconSerial;

		text = (CGUIText*)Add(new CGUIText(0x0288, iconX, 162));
		text->CreateTextureA(6, "Mana Cost: 0\nMin. Skill: 0");
	}
}
//----------------------------------------------------------------------------
void CGumpSpellbook::UpdateContentMysticism(const int &maxSpellsCount, const int &dictionaryPagesCount)
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

	ushort graphic = 0;
	ushort minimizedGraphic = 0;
	int maxSpellsCount = 0;
	int dictionaryPagesCount = 0;
	int spellsOnPage = 0;

	GetSummaryBookInfo(maxSpellsCount, dictionaryPagesCount, spellsOnPage, graphic, minimizedGraphic);

	if (m_Minimized)
	{
		m_Body = (CGUIGumppic*)Add(new CGUIGumppic(minimizedGraphic, 0, 0));

		return;
	}
	else
	{
		m_Body = (CGUIGumppic*)Add(new CGUIGumppic(graphic, 0, 0));

		Add(new CGUIHitBox(ID_GSB_BUTTON_MINIMIZE, 6, 100, 16, 16, true));
	}

	CGameItem *spellbook = g_World->FindWorldItem(m_Serial);

	if (spellbook == NULL)
		return;

	m_SpellCount = 0;
	memset(&m_Spells[0], 0, sizeof(m_Spells));

	QFOR(item, spellbook->m_Items, CGameItem*)
	{
		int currentCount = item->Count;

		if (currentCount > 0 && currentCount <= maxSpellsCount)
		{
			m_Spells[currentCount - 1] = 1;
			m_SpellCount++;
		}
	}

	m_PageCount = dictionaryPagesCount + m_SpellCount;

	//if (m_SpellCount % 2)
	//	m_PageCount--;

	int offs = 0;

	bool isMageSpellbook = false;

	if (m_BookType == ST_MAGE)
	{
		isMageSpellbook = true;

		Add(new CGUIButton(ID_GSB_BUTTON_CIRCLE_1_2, 0x08B1, 0x08B1, 0x08B1, 58, 175));
		Add(new CGUIButton(ID_GSB_BUTTON_CIRCLE_1_2, 0x08B2, 0x08B2, 0x08B2, 93, 175));
		Add(new CGUIButton(ID_GSB_BUTTON_CIRCLE_3_4, 0x08B3, 0x08B3, 0x08B3, 130, 175));
		Add(new CGUIButton(ID_GSB_BUTTON_CIRCLE_3_4, 0x08B4, 0x08B4, 0x08B4, 164, 175));

		Add(new CGUIButton(ID_GSB_BUTTON_CIRCLE_5_6, 0x08B5, 0x08B5, 0x08B5, 227, 175));
		Add(new CGUIButton(ID_GSB_BUTTON_CIRCLE_5_6, 0x08B6, 0x08B6, 0x08B6, 260, 175));
		Add(new CGUIButton(ID_GSB_BUTTON_CIRCLE_7_8, 0x08B7, 0x08B7, 0x08B7, 297, 175));
		Add(new CGUIButton(ID_GSB_BUTTON_CIRCLE_7_8, 0x08B8, 0x08B8, 0x08B8, 332, 175));
	}

	IFOR(page, 0, dictionaryPagesCount)
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

		if (isMageSpellbook)
		{
			text = (CGUIText*)Add(new CGUIText(0x0288, dataX, 30));
			text->CreateTextureA(6, SPELL_CIRCLES_NAMES[page]);
		}

		IFOR(i, 0, spellsOnPage)
		{
			if (m_Spells[offs])
			{
				CGUITextEntry *entry = (CGUITextEntry*)Add(new CGUITextEntry(spellSerial + offs, 0x0288, 0, 0, dataX, 52 + y, 0, false, 9));
				entry->m_Entry.SetText(GetSpellName(offs));
				entry->CheckOnSerial = true;
				entry->ReadOnly = true;

				CGUIHitBox *box = (CGUIHitBox*)Add(new CGUIHitBox(spellSerial + offs, dataX, 52 + y, 100, 16, true));
				box->MoveOnDrag = true;

				y += 15;
			}

			offs++;
		}
	}

	switch (m_BookType)
	{
		case ST_MAGE:
		{
			UpdateContentMage(maxSpellsCount, dictionaryPagesCount);
			break;
		}
		case ST_NECRO:
		{
			UpdateContentNecro(maxSpellsCount, dictionaryPagesCount);
			break;
		}
		case ST_PALADIN:
		{
			UpdateContentPaladin(maxSpellsCount, dictionaryPagesCount);
			break;
		}
		case ST_BUSHIDO:
		{
			UpdateContentBushido(maxSpellsCount, dictionaryPagesCount);
			break;
		}
		case ST_NINJITSU:
		{
			UpdateContentNinjitsu(maxSpellsCount, dictionaryPagesCount);
			break;
		}
		case ST_SPELL_WEAVING:
		{
			UpdateContentSpellWeaving(maxSpellsCount, dictionaryPagesCount);
			break;
		}
		case ST_MYSTICISM:
		{
			UpdateContentMysticism(maxSpellsCount, dictionaryPagesCount);
			break;
		}
	}

	Add(new CGUIPage(-1));

	m_PrevPage = (CGUIButton*)Add(new CGUIButton(ID_GSB_BUTTON_PREV, 0x08BB, 0x08BB, 0x08BB, 50, 8));
	m_PrevPage->Visible = (m_Page != 0);
	m_NextPage = (CGUIButton*)Add(new CGUIButton(ID_GSB_BUTTON_NEXT, 0x08BC, 0x08BC, 0x08BC, 321, 8));
	m_NextPage->Visible = (m_Page + 2 < m_PageCount);

	m_LastSpellBookmark = (CGUIGumppic*)Add(new CGUIGumppic(0x08AD, 184, 2));
	m_LastSpellPointer = (CGUIGumppic*)Add(new CGUIGumppic(0x08AF, 184, 52));
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
		ushort graphic = 0;
		ushort minimizedGraphic = 0;
		int maxSpellsCount = 0;
		int dictionaryPagesCount = 0;
		int spellsOnPage = 0;

		GetSummaryBookInfo(maxSpellsCount, dictionaryPagesCount, spellsOnPage, graphic, minimizedGraphic);

		if (m_Page < dictionaryPagesCount) //List of spells
		{
			IFOR(j, 0, 2)
			{
				IFOR(i, 0, spellsOnPage)
				{
					int offs = i + (spellsOnPage * (m_Page + j));

					if (!m_Spells[offs])
						continue;

					if (serial == offs + ID_GSB_SPELL_ICON_LEFT || serial == offs + ID_GSB_SPELL_ICON_RIGHT)
					{
						int c = dictionaryPagesCount;

						IFOR(k, 0, maxSpellsCount)
						{
							if (m_Spells[k])
							{
								if (k == offs)
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
const string CGumpSpellbook::m_SpellName1[SPELLBOOK_1_SPELLS_COUNT] =
{
	"Clumsy",
	"Create Food",
	"Feeblemind",
	"Heal",
	"Magic Arrow",
	"Night Sight",
	"Reactive Armor",
	"Weaken",
	"Agility",
	"Cunning",
	"Cure",
	"Harm",
	"Magic Trap",
	"Magic Untrap",
	"Protection",
	"Strength",
	"Bless",
	"Fireball",
	"Magic Lock",
	"Poison",
	"Telekinesis",
	"Teleport",
	"Unlock",
	"Wall of Stone",
	"Arch Cure",
	"Arch Protection",
	"Curse",
	"Fire Field",
	"Greater Heal",
	"Lightning",
	"Mana Drain",
	"Recall",
	"Blade Spirits",
	"Dispel Field",
	"Incognito",
	"Magic Reflection",
	"Mind Blast",
	"Paralyze",
	"Poison Field",
	"Summ. Creature",
	"Dispel",
	"Energy Bolt",
	"Explosion",
	"Invisibility",
	"Mark",
	"Mass Curse",
	"Paralyze Field",
	"Reveal",
	"Chain Lightning",
	"Energy Field",
	"Flame Strike",
	"Gate Travel",
	"Mana Vampire",
	"Mass Dispel",
	"Meteor Swarm",
	"Polymorph",
	"Earthquake",
	"Energy Vortex",
	"Resurrection",
	"Air Elemental",
	"Summon Daemon",
	"Earth Elemental",
	"Fire Elemental",
	"Water Elemental"
};
//----------------------------------------------------------------------------
const string CGumpSpellbook::m_SpellReagents1[SPELLBOOK_1_SPELLS_COUNT] =
{
	"Bloodmoss\nNightshade",
	"Garlic\nGinseng\nMandrake root",
	"Ginseng\nNightshade",
	"Garlic\nGinseng\nSpiders silk",
	"Black pearl\nNightshade",
	"Sulfurous ash\nSpiders silk",
	"Garlic\nSulfurous ash\nSpiders silk",
	"Garlic\nNightshade",
	"Bloodmoss\nMandrake root",
	"Mandrake root\nNightshade",
	"Garlic\nGinseng",
	"Nightshade\nSpiders silk",
	"Garlic\nSulfurous ash\nSpiders silk",
	"Bloodmoss\nSulfurous ash",
	"Garlic\nGinseng\nSulfurous ash",
	"Mandrake root\nNightshade",
	"Garlic\nMandrake root",
	"Black pearl\nSulfurous ash",
	"Sulfurous ash\nBloodmoss\nGarlic",
	"Nightshade",
	"Bloodmoss\nMandrake root",
	"Bloodmoss\nMandrake root",
	"Bloodmoss\nSulfurous ash",
	"Bloodmoss\nGarlic",
	"Garlic\nGinseng\nMandrake root",
	"Garlic\nGinseng\nMandrake root\nSulfurous ash",
	"Garlic\nNightshade\nSulfurous ash",
	"Black pearl\nSpiders silk\nSulfurous ash",
	"Garlic\nGinseng\nMandrake root\nSpiders silk",
	"Black pearl\nMandrake root\nSulfurous ash",
	"Black pearl\nMandrake root\nSpiders silk",
	"Black pearl\nBloodmoss\nMandrake root",
	"Black pearl\nMandrake root\nNightshade",
	"Garlic\nBlack pearl\nSpiders silk\nSulfurous ash",
	"Bloodmoss\nGarlic\nNightshade",
	"Garlic\nMandrake root\nSpiders silk",
	"Black pearl\nMandrake root\nNightshade\nSulfurous ash",
	"Garlic\nMandrake root\nSpiders silk",
	"Black pearl\nNightshade\nSpiders silk",
	"Bloodmoss\nMandrake root\nSpiders silk",
	"Garlic\nMandrake root\nSulfurous ash",
	"Black pearl\nNightshade",
	"Black pearl\nMandrake root\nSulfurous ash",
	"Bloodmoss\nNightshade",
	"Black pearl\nBloodmoss\nMandrake root",
	"Garlic\nMandrake root\nNightshade\nSulfurous ash",
	"Black pearl\nGinseng\nSpiders silk",
	"Bloodmoss\nSulfurous ash",
	"Black pearl\nMandrake root\nBloodmoss\nSulfurous ash",
	"Black pearl\nMandrake root\nSpiders silk\nSulfurous ash",
	"Spiders silk\nSulfurous ash",
	"Black pearl\nMandrake root\nSulfurous ash",
	"Black pearl\nBloodmoss\nMandrake root\nSpiders silk",
	"Black pearl\nGarlic\nMandrake root\nSulfurous ash",
	"Bloodmoss\nSpiders silk\nMandrake root\nSulfurous ash",
	"Bloodmoss\nMandrake root\nSpiders silk",
	"Bloodmoss\nGinseng\nMandrake root\nSulfurous ash",
	"Black pearl\nBloodmoss\nMandrake root\nNightshade",
	"Bloodmoss\nGarlic\nGinseng",
	"Bloodmoss\nMandrake root\nSpiders silk",
	"Bloodmoss\nMandrake root\nSpiders silk\nSulfurous ash",
	"Bloodmoss\nMandrake root\nSpiders silk",
	"Bloodmoss\nMandrake root\nSpiders silk\nSulfurous ash",
	"Bloodmoss\nMandrake root\nSpiders silk"
};
//----------------------------------------------------------------------------
const string CGumpSpellbook::m_SpellName2[SPELLBOOK_2_SPELLS_COUNT]
{
	"Animate Dead",
	"Blood Oath",
	"Corpse Skin",
	"Curse Weapon",
	"Evil Omen",
	"Horrific Beast",
	"Lich Form",
	"Mind Rot",
	"Pain Spike",
	"Poison Strike",
	"Strangle",
	"Summon Familiar",
	"Vampiric Embrace",
	"Vengeful Spirit",
	"Wither",
	"Wraith Form",
	"Exorcism"
};
//----------------------------------------------------------------------------
const string CGumpSpellbook::m_SpellName3[SPELLBOOK_3_SPELLS_COUNT]
{
	"Cleanse by Fire",
	"Close Wounds",
	"Consecrate Weapon",
	"Dispel Evil",
	"Divine Fury",
	"Enemy of One",
	"Holy Light",
	"Noble Sacrifice",
	"Remove Curse",
	"Sacred Journey"
};
//----------------------------------------------------------------------------
const string CGumpSpellbook::m_SpellName4[SPELLBOOK_4_SPELLS_COUNT]
{
	"Honorable Execution",
	"Confidence",
	"Evasion",
	"Counter Attack",
	"Lightning Strike",
	"Momentum Strike"
};
//----------------------------------------------------------------------------
const string CGumpSpellbook::m_SpellName5[SPELLBOOK_5_SPELLS_COUNT]
{
	"Focus Attack",
	"Death Strike",
	"Animal Form",
	"Ki Attack",
	"Surprise Attack",
	"Backstab",
	"Shadowjump",
	"Mirror Image"
};
//----------------------------------------------------------------------------
const string CGumpSpellbook::m_SpellName6[SPELLBOOK_6_SPELLS_COUNT]
{
	"Arcane Circle",
	"Gift of Renewal",
	"Immolating Weapon",
	"Attunement",
	"Thunderstorm",
	"Nature's Fury",
	"Summon Fey",
	"Summon Fiend",
	"Reaper Form",
	"Wildfire",
	"Essence of Wind",
	"Dryad Allure",
	"Ethereal Voyage",
	"Word of Death",
	"Gift of Life",
	"Arcane Empowerment"
};
//----------------------------------------------------------------------------
const string CGumpSpellbook::m_SpellName7[SPELLBOOK_7_SPELLS_COUNT]
{
	"Nether Bolt",
	"Healing Stone",
	"Purge Magic",
	"Enchant",
	"Sleep",
	"Eagle Strike",
	"Animated Weapon",
	"Stone Form",
	"Spell Trigger",
	"Mass Sleep",
	"Cleansing Winds",
	"Bombard",
	"Spell Plague",
	"Hail Storm",
	"Nether Cyclone",
	"Rising Colossus",
	"Inspire",
	"Invigorate",
	"Resilience",
	"Perseverance",
	"Tribulation",
	"Despair",
	"Death Ray",
	"Ethereal Burst",
	"Nether Blast",
	"Mystic Weapon",
	"Command Undead",
	"Conduit",
	"Mana Shield",
	"Summon Reaper",
	"Enchanted Summoning",
	"Anticipate Hit",
	"Warcry",
	"Intuition",
	"Rejuvenate",
	"Holy Fist",
	"Shadow",
	"White Tiger Form",
	"Thrust",
	"Pierce",
	"Stagger",
	"Toughness",
	"Onslaught",
	"Focused Eye",
	"Elemental Fury",
	"Called Shot",
	"Saving Throw",
	"Shield Bash",
	"Bodyguard",
	"Heighten Senses",
	"Tolerance",
	"Injected Strike",
	"Potency",
	"Rampage",
	"Fists of Fury",
	"Knockout",
	"Whispering",
	"Combat Training",
	"Boarding",
	"Flaming Shot",
	"Playing The Odds"
};
//----------------------------------------------------------------------------