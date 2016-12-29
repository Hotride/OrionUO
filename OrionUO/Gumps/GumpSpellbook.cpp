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
#include "../Game objects/GamePlayer.h"
#include "../PressedObject.h"
#include "../ClickObject.h"
#include "../OrionUO.h"
#include "../Managers/MouseManager.h"
#include "../Managers/GumpManager.h"
#include "../Managers/ClilocManager.h"
#include "../Managers/PacketManager.h"
#include "../OrionWindow.h"
#include "../ToolTip.h"
//----------------------------------------------------------------------------------
//!Названия кругов заклинаний
const string CGumpSpellbook::m_SpellCircleName[8] =
{
	"First Circle",
	"Second Circle",
	"Third Circle",
	"Fourth Circle",
	"Fifth Circle",
	"Sixth Circle",
	"Seventh Circle",
	"Eighth Circle"
};
//----------------------------------------------------------------------------------
CGumpSpellbook::CGumpSpellbook(uint serial, int x, int y)
: CGump(GT_SPELLBOOK, serial, x, y), m_PageCount(8), m_SpellCount(0), m_Body(NULL),
m_PrevPage(NULL), m_NextPage(NULL), m_LastSpellPointer(NULL), m_LastSpellBookmark(NULL),
m_BookType(ST_MAGE), m_TithingPointsText(NULL)
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

	int dictionaryPagesCount = 0;
	int tooltipOffset = 0;

	GetTooltipBookInfo(dictionaryPagesCount, tooltipOffset);

	if (m_Page >= dictionaryPagesCount)
	{
		uint serial = g_SelectedObject.Serial;

		if (serial >= ID_GSB_SPELL_ICON_LEFT)
		{
			if (serial >= ID_GSB_SPELL_ICON_RIGHT)
				serial -= ID_GSB_SPELL_ICON_RIGHT;
			else
				serial -= ID_GSB_SPELL_ICON_LEFT;

			g_ToolTip.Set(g_ClilocManager.Cliloc(g_Language)->GetW(tooltipOffset + serial), g_SelectedObject.Object(), 150);
		}
	}
}
//----------------------------------------------------------------------------
void CGumpSpellbook::PrepareContent()
{
	int maxSpellsCount = 0;
	int spellsOnPage = 0;
	int dictionaryPagesCount = 0;
	int spellIndexOffset = 0;
	ushort graphic = 0;
	ushort minimizedGraphic = 0;
	ushort iconStartGraphic = 0;

	GetSummaryBookInfo(maxSpellsCount, dictionaryPagesCount, spellsOnPage, spellIndexOffset, graphic, minimizedGraphic, iconStartGraphic);

	if (g_PressedObject.LeftGump() == this && m_Page >= dictionaryPagesCount && g_PressedObject.LeftSerial >= ID_GSB_SPELL_ICON_LEFT)
	{
		WISP_GEOMETRY::CPoint2Di offset = g_MouseManager.LeftDroppedOffset();

		if ((abs(offset.X) >= DRAG_PIXEL_RANGE || abs(offset.Y) >= DRAG_PIXEL_RANGE) || (g_MouseManager.LastLeftButtonClickTimer + g_MouseManager.DoubleClickDelay < g_Ticks))
		{
			int index = g_PressedObject.LeftSerial;

			if (index < ID_GSB_SPELL_ICON_RIGHT)
				index -= ID_GSB_SPELL_ICON_LEFT;
			else
				index -= ID_GSB_SPELL_ICON_RIGHT;

			if (iconStartGraphic >= 0x5300 && iconStartGraphic < 0x5500)
				iconStartGraphic += 0x0020;

			g_GumpManager.AddGump(new CGumpSpell(index + spellIndexOffset + 1, g_MouseManager.Position.X - 20, g_MouseManager.Position.Y - 20, iconStartGraphic + index, m_BookType));

			g_OrionWindow.EmulateOnLeftMouseButtonDown();
		}
	}

	if (!m_Minimized && m_LastSpellPointer != NULL)
	{
		spellIndexOffset = (int)m_BookType * 100;

		bool wantVisible = false;
		ushort graphicBookmark = 0x08AD;
		ushort graphicPointer = 0x08AF;
		int wantX = m_LastSpellPointer->X;
		int wantY = m_LastSpellPointer->Y;

		if (m_Page < dictionaryPagesCount)
		{
			int offs = (spellsOnPage * m_Page);

			IFOR(j, 0, 2)
			{
				int y = 0;

				IFOR(i, 0, spellsOnPage)
				{
					if (m_Spells[offs])
					{
						if (offs + spellIndexOffset == g_LastSpellIndex - 1)
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

							offs++;
							break;
						}

						y += 15;
					}

					offs++;
				}
			}
		}
		else
		{
			int page = dictionaryPagesCount;

			IFOR(i, 0, maxSpellsCount)
			{
				if (!m_Spells[i])
					continue;

				if (page == m_Page || page == (m_Page + 1))
				{
					if (i + spellIndexOffset == g_LastSpellIndex - 1)
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
}
//----------------------------------------------------------------------------
void CGumpSpellbook::GetTooltipBookInfo(int &dictionaryPagesCount, int &tooltipOffset)
{
	int maxSpellsCount = 0;

	switch (m_BookType)
	{
		case ST_MAGE:
		{
			maxSpellsCount = SPELLBOOK_1_SPELLS_COUNT;
			tooltipOffset = 1061290;

			break;
		}
		case ST_NECRO:
		{
			maxSpellsCount = SPELLBOOK_2_SPELLS_COUNT;
			tooltipOffset = 1061390;

			break;
		}
		case ST_PALADIN:
		{
			maxSpellsCount = SPELLBOOK_3_SPELLS_COUNT;
			tooltipOffset = 1061490;

			break;
		}
		case ST_BUSHIDO:
		{
			maxSpellsCount = SPELLBOOK_4_SPELLS_COUNT;
			tooltipOffset = 1063263;

			break;
		}
		case ST_NINJITSU:
		{
			maxSpellsCount = SPELLBOOK_5_SPELLS_COUNT;
			tooltipOffset = 1063279;

			break;
		}
		case ST_SPELL_WEAVING:
		{
			maxSpellsCount = SPELLBOOK_6_SPELLS_COUNT;
			tooltipOffset = 1072042;

			break;
		}
		case ST_MYSTICISM: //?
		{
			maxSpellsCount = SPELLBOOK_7_SPELLS_COUNT;
			tooltipOffset = 0;

			break;
		}
		default:
			break;
	}

	dictionaryPagesCount = (int)ceilf(maxSpellsCount / 8.0f);

	if (dictionaryPagesCount % 2)
		dictionaryPagesCount++;
}
//----------------------------------------------------------------------------
void CGumpSpellbook::GetSummaryBookInfo(int &maxSpellsCount, int &dictionaryPagesCount, int &spellsOnPage, int &spellIndexOffset, ushort &graphic, ushort &minimizedGraphic, ushort &iconStartGraphic)
{
	switch (m_BookType)
	{
		case ST_MAGE:
		{
			maxSpellsCount = SPELLBOOK_1_SPELLS_COUNT;
			graphic = 0x08AC;
			minimizedGraphic = 0x08BA;
			iconStartGraphic = 0x08C0;
			spellIndexOffset = 0;

			break;
		}
		case ST_NECRO:
		{
			maxSpellsCount = SPELLBOOK_2_SPELLS_COUNT;
			graphic = 0x2B00;
			minimizedGraphic = 0x2B03;
			iconStartGraphic = 0x5000;
			spellIndexOffset = 64;

			break;
		}
		case ST_PALADIN:
		{
			maxSpellsCount = SPELLBOOK_3_SPELLS_COUNT;
			graphic = 0x2B01;
			minimizedGraphic = 0x2B04;
			iconStartGraphic = 0x5100;
			spellIndexOffset = 81;

			break;
		}
		case ST_BUSHIDO:
		{
			maxSpellsCount = SPELLBOOK_4_SPELLS_COUNT;
			graphic = 0x2B07;
			minimizedGraphic = 0x2B09;
			iconStartGraphic = 0x5400;
			spellIndexOffset = 91;

			break;
		}
		case ST_NINJITSU:
		{
			maxSpellsCount = SPELLBOOK_5_SPELLS_COUNT;
			graphic = 0x2B06;
			minimizedGraphic = 0x2B08;
			iconStartGraphic = 0x5300;
			spellIndexOffset = 97;

			break;
		}
		case ST_SPELL_WEAVING:
		{
			maxSpellsCount = SPELLBOOK_6_SPELLS_COUNT;
			graphic = 0x2B2F;
			minimizedGraphic = 0x2B2D;
			iconStartGraphic = 0x59D8;
			spellIndexOffset = 105;

			break;
		}
		case ST_MYSTICISM: //?
		{
			maxSpellsCount = SPELLBOOK_7_SPELLS_COUNT;
			graphic = 0;
			minimizedGraphic = 0;
			iconStartGraphic = 0;
			spellIndexOffset = 0;

			break;
		}
		default:
			break;
	}

	spellsOnPage = min(maxSpellsCount / 2, 8);

	dictionaryPagesCount = (int)ceilf(maxSpellsCount / 8.0f);

	if (dictionaryPagesCount % 2)
		dictionaryPagesCount++;
}
//----------------------------------------------------------------------------
string CGumpSpellbook::GetSpellName(const int &offset, string &abbreviature, string &reagents)
{
	switch (m_BookType)
	{
		case ST_MAGE:
			abbreviature = m_SpellName1[offset][1];
			reagents = m_SpellReagents1[offset];
			return m_SpellName1[offset][0];
		case ST_NECRO:
			abbreviature = m_SpellName2[offset][1];
			reagents = m_SpellReagents2[offset];
			return m_SpellName2[offset][0];
		case ST_PALADIN:
			abbreviature = m_SpellName3[offset][1];
			return m_SpellName3[offset][0];
		case ST_BUSHIDO:
			return m_SpellName4[offset];
		case ST_NINJITSU:
			return m_SpellName5[offset];
		case ST_SPELL_WEAVING:
			abbreviature = m_SpellName6[offset][1];
			return m_SpellName6[offset][0];
		case ST_MYSTICISM:
			abbreviature = m_SpellName7[offset][1];
			return m_SpellName7[offset][2];
		default:
			break;
	}

	return "";
}
//----------------------------------------------------------------------------
string CGumpSpellbook::GetSpellRequries(const int &offset, int &y)
{
	char buf[100] = { 0 };
	y = 162;

	int manaCost = 0;
	int minSkill = 0;

	switch (m_BookType)
	{
		case ST_NECRO:
		{
			const SPELL_REQURIES &req = m_SpellRequries2[offset];
			manaCost = req.ManaCost;
			minSkill = req.MinSkill;

			break;
		}
		case ST_PALADIN:
		{
			y = 148;

			const PALADIN_SPELL_REQURIES &req = m_SpellRequries3[offset];

			sprintf_s(buf, "Tithing Cost: %i\nMana Cost: %i\nMin. Skill: %i", req.TithingPoints, req.ManaCost, req.MinSkill);

			return buf;
		}
		case ST_BUSHIDO:
		{
			const SPELL_REQURIES &req = m_SpellRequries4[offset];
			manaCost = req.ManaCost;
			minSkill = req.MinSkill;

			break;
		}
		case ST_NINJITSU:
		{
			const SPELL_REQURIES &req = m_SpellRequries5[offset];
			manaCost = req.ManaCost;
			minSkill = req.MinSkill;

			break;
		}
		case ST_SPELL_WEAVING:
		{
			const SPELL_REQURIES &req = m_SpellRequries6[offset];
			manaCost = req.ManaCost;
			minSkill = req.MinSkill;

			break;
		}
		default:
			break;
	}

	sprintf_s(buf, "Mana Cost: %i\nMin. Skill: %i", manaCost, minSkill);

	return buf;
}
//----------------------------------------------------------------------------
void CGumpSpellbook::UpdateContent()
{
	m_Body = NULL;
	m_PrevPage = NULL;
	m_NextPage = NULL;
	m_LastSpellPointer = NULL;
	m_LastSpellBookmark = NULL;
	m_TithingPointsText = NULL;

	Clear();

	Add(new CGUIPage(-1));

	int maxSpellsCount = 0;
	int spellsOnPage = 0;
	int dictionaryPagesCount = 0;
	int spellIndexOffset = 0;
	ushort graphic = 0;
	ushort minimizedGraphic = 0;
	ushort iconStartGraphic = 0;

	GetSummaryBookInfo(maxSpellsCount, dictionaryPagesCount, spellsOnPage, spellIndexOffset, graphic, minimizedGraphic, iconStartGraphic);

	if (m_Minimized)
	{
		m_Body = (CGUIGumppic*)Add(new CGUIGumppic(minimizedGraphic, 0, 0));

		return;
	}

	m_Body = (CGUIGumppic*)Add(new CGUIGumppic(graphic, 0, 0));

	Add(new CGUIHitBox(ID_GSB_BUTTON_MINIMIZE, 6, 100, 16, 16, true));

	m_SpellCount = 0;
	memset(&m_Spells[0], 0, sizeof(m_Spells));

	CGameItem *spellbook = g_World->FindWorldItem(m_Serial);

	if (spellbook == NULL)
		return;

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

		if (!page && m_BookType == ST_PALADIN)
		{
			m_TithingPointsText = (CGUIText*)Add(new CGUIText(0x0288, 62, 162));
			string textData = "Tithing points\nAvailable: " + std::to_string(g_Player->TithingPoints);
			m_TithingPointsText->CreateTextureA(6, textData);
		}

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
			text->CreateTextureA(6, m_SpellCircleName[page]);
		}

		IFOR(i, 0, spellsOnPage)
		{
			if (m_Spells[offs])
			{
				CGUITextEntry *entry = (CGUITextEntry*)Add(new CGUITextEntry(spellSerial + offs, 0x0288, 0, 0, dataX, 52 + y, 0, false, 9));
				string abbreviature;
				string reagents;
				entry->m_Entry.SetText(GetSpellName(offs, abbreviature, reagents));
				entry->CheckOnSerial = true;
				entry->ReadOnly = true;

				CGUIHitBox *box = (CGUIHitBox*)Add(new CGUIHitBox(spellSerial + offs, dataX, 52 + y, 100, 16, true));
				box->MoveOnDrag = true;

				y += 15;
			}

			offs++;
		}
	}

	int page = dictionaryPagesCount;

	int topTextY = (m_BookType == ST_MAGE ? 10 : 6);
	bool haveReagents = (m_BookType <= ST_NECRO);
	bool haveAbbreviature = (m_BookType != ST_BUSHIDO && m_BookType != ST_NINJITSU);

	IFOR(i, 0, maxSpellsCount)
	{
		if (!m_Spells[i])
			continue;

		int iconX = 62;
		int topTextX = 87;
		int iconTextX = 112;
		uint iconSerial = ID_GSB_SPELL_ICON_LEFT + i;

		if (page % 2)
		{
			iconX = 225;
			topTextX = 244;
			iconTextX = 275;
			iconSerial = ID_GSB_SPELL_ICON_RIGHT + i;
		}

		Add(new CGUIPage(page));
		page++;

		string spellAbbreviature = "";
		string reagents = "";
		string spellName = GetSpellName(i, spellAbbreviature, reagents);

		CGUIText *text = (CGUIText*)Add(new CGUIText(0x0288, topTextX, topTextY));

		if (isMageSpellbook)
		{
			text->CreateTextureA(6, m_SpellCircleName[i / 8]);

			text = (CGUIText*)Add(new CGUIText(0x0288, iconTextX, 34));
			text->CreateTextureA(6, spellName, 80);

			int abbreviatureY = 26;

			if (text->m_Texture.Height < 24)
				abbreviatureY = 31;

			abbreviatureY += text->m_Texture.Height;

			text = (CGUIText*)Add(new CGUIText(0x0288, iconTextX, abbreviatureY));
			text->CreateTextureA(8, spellAbbreviature);
		}
		else
		{
			text->CreateTextureA(6, spellName);

			if (haveAbbreviature)
			{
				text = (CGUIText*)Add(new CGUIText(0x0288, iconTextX, 34));
				text->CreateTextureA(6, spellAbbreviature, 80);
			}
		}

		CGUIGumppic *icon = (CGUIGumppic*)Add(new CGUIGumppic(iconStartGraphic + i, iconX, 40));
		icon->Serial = iconSerial;

		if (haveReagents)
		{
			Add(new CGUIGumppicTiled(0x0835, iconX, 88, 120, 0));

			text = (CGUIText*)Add(new CGUIText(0x0288, iconX, 92));
			text->CreateTextureA(6, "Reagents:");

			text = (CGUIText*)Add(new CGUIText(0x0288, iconX, 114));
			text->CreateTextureA(9, reagents);
		}

		if (!isMageSpellbook)
		{
			int requriesY = 0;
			string requries = GetSpellRequries(i, requriesY);

			text = (CGUIText*)Add(new CGUIText(0x0288, iconX, requriesY));
			text->CreateTextureA(6, requries);
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
		int maxSpellsCount = 0;
		int dictionaryPagesCount = 0;
		int spellsOnPage = 0;
		int spellIndexOffset = 0;
		ushort graphic = 0;
		ushort minimizedGraphic = 0;
		ushort iconStartGraphic = 0;

		GetSummaryBookInfo(maxSpellsCount, dictionaryPagesCount, spellsOnPage, spellIndexOffset, graphic, minimizedGraphic, iconStartGraphic);

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
						int spellIndex = g_PressedObject.LeftSerial - ID_GSB_SPELL_ICON_RIGHT + 1;

						//Было использовано заклинание
						if (g_PressedObject.LeftSerial < ID_GSB_SPELL_ICON_RIGHT)
							spellIndex = g_PressedObject.LeftSerial - ID_GSB_SPELL_ICON_LEFT + 1;

						spellIndex += ((int)m_BookType * 100);

						if (g_PacketManager.ClientVersion < CV_308Z)
							g_Orion.CastSpellFromBook(spellIndex, m_Serial);
						else
							g_Orion.CastSpell(spellIndex);

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
//----------------------------------------------------------------------------------
const string CGumpSpellbook::m_SpellName1[SPELLBOOK_1_SPELLS_COUNT][2] =
{
	{ "Clumsy", "U J" },
	{ "Create Food", "I M Y" },
	{ "Feeblemind", "R W" },
	{ "Heal", "I M" },
	{ "Magic Arrow", "I P Y" },
	{ "Night Sight", "I L" },
	{ "Reactive Armor", "F S" },
	{ "Weaken", "D M" },
	{ "Agility", "E U" },
	{ "Cunning", "U W" },
	{ "Cure", "A N" },
	{ "Harm", "A M" },
	{ "Magic Trap", "I J" },
	{ "Magic Untrap", "A J" },
	{ "Protection", "U S" },
	{ "Strength", "U M" },
	{ "Bless", "R S" },
	{ "Fireball", "V F" },
	{ "Magic Lock", "A P" },
	{ "Poison", "I N" },
	{ "Telekinesis", "O P Y" },
	{ "Teleport", "R P" },
	{ "Unlock", "E P" },
	{ "Wall of Stone", "I S Y" },
	{ "Arch Cure", "V A N" },
	{ "Arch Protection", "V U S" },
	{ "Curse", "D S" },
	{ "Fire Field", "I F G" },
	{ "Greater Heal", "I V M" },
	{ "Lightning", "P O G" },
	{ "Mana Drain", "O R" },
	{ "Recall", "K O P" },
	{ "Blade Spirits", "I H J Y" },
	{ "Dispel Field", "A G" },
	{ "Incognito", "K I E" },
	{ "Magic Reflection", "I J S" },
	{ "Mind Blast", "P C W" },
	{ "Paralyze", "A E P" },
	{ "Poison Field", "I N G" },
	{ "Summ. Creature", "K X" },
	{ "Dispel", "A O" },
	{ "Energy Bolt", "C P" },
	{ "Explosion", "V O F" },
	{ "Invisibility", "A L X" },
	{ "Mark", "K P Y" },
	{ "Mass Curse", "V D S" },
	{ "Paralyze Field", "I E G" },
	{ "Reveal", "W Q" },
	{ "Chain Lightning", "V O G" },
	{ "Energy Field", "I S G" },
	{ "Flame Strike", "K V F" },
	{ "Gate Travel", "V R P" },
	{ "Mana Vampire", "O S" },
	{ "Mass Dispel", "V A O" },
	{ "Meteor Swarm", "F K D Y" },
	{ "Polymorph", "V Y R" },
	{ "Earthquake", "I V P" },
	{ "Energy Vortex", "V C P" },
	{ "Resurrection", "A C" },
	{ "Air Elemental", "K V X H" },
	{ "Summon Daemon", "K V X C" },
	{ "Earth Elemental", "K V X Y" },
	{ "Fire Elemental", "K V X F" },
	{ "Water Elemental", "K V X A" }
};
//----------------------------------------------------------------------------------
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
//----------------------------------------------------------------------------------
const string CGumpSpellbook::m_SpellName2[SPELLBOOK_2_SPELLS_COUNT][2]
{
	{ "Animate Dead", "Uus Corp" },
	{ "Blood Oath", "In Jux Mani Xen" },
	{ "Corpse Skin", "In Aglo Corp Ylem" },
	{ "Curse Weapon", "An Sanct Gra Char" },
	{ "Evil Omen", "Pas Tym An Sanct" },
	{ "Horrific Beast", "Rel Xen Vas Bal" },
	{ "Lich Form", "Rel Xen Corp Ort" },
	{ "Mind Rot", "Wis An Ben" },
	{ "Pain Spike", "In Sar" },
	{ "Poison Strike", "In Vas Nox" },
	{ "Strangle", "In Bal Nox" },
	{ "Summon Familiar", "Kal Xen Bal" },
	{ "Vampiric Embrace", "Rel Xen An Sanct" },
	{ "Vengeful Spirit", "Kal Xen Bal Beh" },
	{ "Wither", "Kal Vas An Flam" },
	{ "Wraith Form", "Rel Xen Um" },
	{ "Exorcism", "Ort Corp Grav" }
};
//----------------------------------------------------------------------------------
const string CGumpSpellbook::m_SpellReagents2[SPELLBOOK_2_SPELLS_COUNT] =
{
	"Daemon Blood\nGrave Dust",
	"Daemon Blood",
	"Batwing\nGrave Dust",
	"Pig Iron",
	"Batwing\nNox Crystal",
	"Batwing\nDaemon Blood",
	"Nox Crystal\nDaemon Blood\nGrave Dust",
	"Batwing\nDaemon Blood\nPig Iron",
	"Grave Dust\nPig Iron",
	"Nox Crystal",
	"Nox Crystal\nDaemon Blood",
	"Batwing\nGrave Dust\nDaemon Blood",
	"Batwing\nNox Crystal\nPig Iron",
	"Batwing\nGrave Dust\nPig Iron",
	"Nox Crystal\nGrave Dust\nPig Iron",
	"Nox Crystal\nPig Iron",
	"Nox Crystal\nGrave Dust"
};
//----------------------------------------------------------------------------------
const string CGumpSpellbook::m_SpellName3[SPELLBOOK_3_SPELLS_COUNT][2]
{
	{ "Cleanse by Fire", "Expor Flamus" },
	{ "Close Wounds", "Obsu Vulni" },
	{ "Consecrate Weapon", "Consecrus Arma" },
	{ "Dispel Evil", "Dispiro Malas" },
	{ "Divine Fury", "Divinum Furis" },
	{ "Enemy of One", "Forul Solum" },
	{ "Holy Light", "Augus Luminos" },
	{ "Noble Sacrifice", "Dium Prostra" },
	{ "Remove Curse", "Extermo Vomica" },
	{ "Sacred Journey", "Sanctum Viatas" }
};
//----------------------------------------------------------------------------------
const string CGumpSpellbook::m_SpellName4[SPELLBOOK_4_SPELLS_COUNT]
{
	"Honorable Execution",
	"Confidence",
	"Evasion",
	"Counter Attack",
	"Lightning Strike",
	"Momentum Strike"
};
//----------------------------------------------------------------------------------
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
//----------------------------------------------------------------------------------
const string CGumpSpellbook::m_SpellName6[SPELLBOOK_6_SPELLS_COUNT][2]
{
	{ "Arcane Circle", "Myrshalee" },
	{ "Gift of Renewal", "Olorisstra" },
	{ "Immolating Weapon", "Thalshara" },
	{ "Attunement", "Haeldril" },
	{ "Thunderstorm", "Erelonia" },
	{ "Nature's Fury", "Rauvvrae" },
	{ "Summon Fey", "Alalithra" },
	{ "Summon Fiend", "Nylisstra" },
	{ "Reaper Form", "Tarisstree" },
	{ "Wildfire", "Haelyn" },
	{ "Essence of Wind", "Anathrae" },
	{ "Dryad Allure", "Rathril" },
	{ "Ethereal Voyage", "Orlavdra" },
	{ "Word of Death", "Nyraxle" },
	{ "Gift of Life", "Illorae" },
	{ "Arcane Empowerment", "Aslavdra" }
};
//----------------------------------------------------------------------------------
const string CGumpSpellbook::m_SpellName7[SPELLBOOK_7_SPELLS_COUNT][2]
{
	{ "Nether Bolt", "In Corp Ylem" },
	{ "Healing Stone", "Kal In Mani" },
	{ "Purge Magic", "An Ort Sanct" },
	{ "Enchant", "In Ort Ylem" },
	{ "Sleep", "In Zu" },
	{ "Eagle Strike", "Kal Por Xen" },
	{ "Animated Weapon", "In Jux Por Ylem" },
	{ "Stone Form", "In Rel Ylem" },
	{ "Spell Trigger", "In Vas Ort Ex" },
	{ "Mass Sleep", "Vas Zu" },
	{ "Cleansing Winds", "In Vas Mani Hur" },
	{ "Bombard", "Corp Por Ylem" },
	{ "Spell Plague", "Vas Rel Jux Ort" },
	{ "Hail Storm", "Kal Des Ylem" },
	{ "Nether Cyclone", "Grav Hur" },
	{ "Rising Colossus", "Kal Vas Xen Corp Ylem" },
	{ "Inspire", "Uus Por" },
	{ "Invigorate", "An Zu" },
	{ "Resilience", "Kal Mani Tym" },
	{ "Perseverance", "Uus Jux Sanct" },
	{ "Tribulation", "In Jux Hur Rel" },
	{ "Despair", "Kal Des Mani Tym" },
	{ "Death Ray", "In Grav Corp" },
	{ "Ethereal Burst", "Uus Ort Grav" },
	{ "Nether Blast", "In Vas Xen Por" },
	{ "Mystic Weapon", "Vas Ylem Wis" },
	{ "Command Undead", "In Corp Xen Por" },
	{ "Conduit", "Uus Corp Grav" },
	{ "Mana Shield", "Faerkulggen" },
	{ "Summon Reaper", "Lartarisstree" }
};
//----------------------------------------------------------------------------------
const SPELL_REQURIES CGumpSpellbook::m_SpellRequries2[SPELLBOOK_2_SPELLS_COUNT] =
{
	{ 23, 40 },
	{ 13, 20 },
	{ 11, 20 },
	{ 7, 0 },
	{ 11, 20 },
	{ 11, 40 },
	{ 23, 70 },
	{ 17, 30 },
	{ 5, 20 },
	{ 17, 50 },
	{ 29, 65 },
	{ 17, 30 },
	{ 23, 99 },
	{ 41, 80 },
	{ 23, 60 },
	{ 17, 20 },
	{ 40, 80 }
};
//----------------------------------------------------------------------------------
const PALADIN_SPELL_REQURIES CGumpSpellbook::m_SpellRequries3[SPELLBOOK_3_SPELLS_COUNT] =
{
	{ 10, 5, 10 },
	{ 10, 0, 10 },
	{ 10, 15, 10 },
	{ 10, 35, 10 },
	{ 15, 25, 10 },
	{ 20, 45, 10 },
	{ 10, 55, 10 },
	{ 20, 65, 30 },
	{ 20, 5, 10 },
	{ 10, 15, 15 }
};
//----------------------------------------------------------------------------------
const SPELL_REQURIES CGumpSpellbook::m_SpellRequries4[SPELLBOOK_4_SPELLS_COUNT] =
{
	{ 0, 25 },
	{ 10, 25 },
	{ 10, 60 },
	{ 5, 40 },
	{ 10, 50 },
	{ 10, 70 }
};
//----------------------------------------------------------------------------------
const SPELL_REQURIES CGumpSpellbook::m_SpellRequries5[SPELLBOOK_5_SPELLS_COUNT] =
{
	{ 10, 30 },
	{ 30, 85 },
	{ 10, 0 },
	{ 25, 80 },
	{ 20, 60 },
	{ 30, 40 },
	{ 15, 50 },
	{ 10, 20 }
};
//----------------------------------------------------------------------------------
const SPELL_REQURIES CGumpSpellbook::m_SpellRequries6[SPELLBOOK_6_SPELLS_COUNT] =
{
	{ 24, 0 },
	{ 24, 0 },
	{ 32, 10 },
	{ 24, 0 },
	{ 32, 10 },
	{ 24, 0 },
	{ 10, 38 },
	{ 10, 38 },
	{ 34, 24 },
	{ 50, 66 },
	{ 40, 52 },
	{ 40, 52 },
	{ 32, 24 },
	{ 50, 80 },
	{ 70, 38 },
	{ 50, 24 }
};
//----------------------------------------------------------------------------------
//?!?!?
/*"Enchanted Summoning",
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
"Playing The Odds"*/
//----------------------------------------------------------------------------------
