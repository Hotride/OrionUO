/***********************************************************************************
**
** GumpStatusbar.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GumpStatusbar.h"
#include "../Game objects/GameWorld.h"
#include "../Game objects/GamePlayer.h"
#include "../Managers/ConnectionManager.h"
#include "../Managers/GumpManager.h"
#include "../Managers/MouseManager.h"
#include "../Managers/ConfigManager.h"
#include "../Managers/FontsManager.h"
#include "../PressedObject.h"
#include "../SelectedObject.h"
#include "../Network/Packets.h"
#include "../Target.h"
#include "../TargetGump.h"
#include "../Party.h"
#include "../OrionUO.h"
#include "../TextEngine/GameConsole.h"
//----------------------------------------------------------------------------------
CGumpStatusbar::CGumpStatusbar(uint serial, short x, short y, bool minimized)
: CGump(GT_STATUSBAR, serial, x, y), m_GroupNext(NULL), m_GroupPrev(NULL),
m_WantFullUpdateContent(false), m_StatusbarUnlocker(NULL), m_Body(NULL),
m_HitsBody(NULL), m_Entry(NULL), m_Name("")
{
	if (minimized)
	{
		m_Minimized = true;
		m_MinimizedX = x;
		m_MinimizedY = y;
	}

	if (!g_ConfigManager.DisableNewTargetSystem && m_Serial == g_NewTargetSystem.Serial)
		g_GumpManager.UpdateGump(m_Serial, 0, GT_TARGET_SYSTEM);

	m_Locker.Serial = ID_GSB_LOCK_MOVING;
}
//----------------------------------------------------------------------------------
CGumpStatusbar::~CGumpStatusbar()
{
	if (g_ConnectionManager.Connected() && g_World != NULL && g_World->FindWorldObject(m_Serial) != NULL)
		CPacketCloseStatusbarGump(m_Serial).Send();

	RemoveFromGroup();
}
//----------------------------------------------------------------------------------
CGumpStatusbar *CGumpStatusbar::GetTopStatusbar()
{
	if (!InGroup())
		return NULL;

	if (m_GroupPrev == NULL)
		return this;

	CGumpStatusbar *gump = m_GroupPrev;

	while (gump != NULL && gump->m_GroupPrev != NULL)
		gump = gump->m_GroupPrev;

	return gump;
}
//----------------------------------------------------------------------------------
CGumpStatusbar *CGumpStatusbar::GetNearStatusbar(int &x, int &y)
{
	if (InGroup() || !m_Minimized)
		return NULL;

	//154x59 mini-gump
	
	const int rangeX = 77;
	const int rangeY = 29;
	const int rangeOffsetX = 60;
	const int rangeOffsetY = 24;

	CGump *gump = (CGump*)g_GumpManager.m_Items;

	while (gump != NULL)
	{
		if (gump != this && gump->GumpType == GT_STATUSBAR && gump->Minimized)
		{
			int gumpX = gump->MinimizedX;
			int offsetX = abs(x - gumpX);
			int passed = 0;
			
			if (x >= gumpX && x <= (gumpX + 154))
				passed = 2;
			else if (offsetX < rangeOffsetX) //left part of gump
				passed = 1;
			else
			{
				offsetX = abs(x - (gumpX + 154));

				if (offsetX < rangeOffsetX) //right part of gump
					passed = -1;
				else if (x >= (gumpX - rangeX) && x <= (gumpX + 154 + rangeX))
					passed = 2;
			}

			int gumpY = gump->MinimizedY;

			if (abs(passed) == 1)
			{
				if (y < (gumpY - rangeY) || y > (gumpY + 59 + rangeY))
					passed = 0;
			}
			else if (passed == 2) //in gump range X
			{
				int offsetY = abs(y - gumpY);

				if (offsetY < rangeOffsetY) //top part of gump
					passed = 2;
				else
				{
					offsetY = abs(y - (gumpY + 59));

					if (offsetY < rangeOffsetY) //bottom part of gump
						passed = -2;
					else
						passed = 0;
				}
			}

			if (passed)
			{
				int testX = gumpX;
				int testY = gumpY;

				switch (passed)
				{
					case -2: //gump bottom
					{
						testY += 59;
						break;
					}
					case -1: //gump right
					{
						testX += 154;
						break;
					}
					case 1: //gump left
					{
						testX -= 154;
						break;
					}
					case 2: //gump top
					{
						testY -= 59;
						break;
					}
					default:
						break;
				}

				CGump *testGump = (CGump*)g_GumpManager.m_Items;

				while (testGump != NULL)
				{
					if (testGump != this && testGump->GumpType == GT_STATUSBAR && testGump->Minimized)
					{
						if (testGump->MinimizedX == testX && testGump->MinimizedY == testY)
							break;
					}

					testGump = (CGump*)testGump->m_Next;
				}

				if (testGump == NULL)
				{
					x = testX;
					y = testY;

					break;
				}
			}
		}

		gump = (CGump*)gump->m_Next;
	}

	return (CGumpStatusbar*)gump;
}
//----------------------------------------------------------------------------------
bool CGumpStatusbar::GetStatusbarGroupOffset(int &x, int &y)
{
	if (InGroup() && m_Minimized && g_MouseManager.LeftButtonPressed && g_PressedObject.LeftObject() != NULL && g_PressedObject.LeftObject()->IsGUI() && ((CBaseGUI*)g_PressedObject.LeftObject())->MoveOnDrag)
	{
		CGumpStatusbar *gump = GetTopStatusbar();

		while (gump != NULL)
		{
			//Если гамп захватили и (может быть) двигают
			if (gump != this && g_PressedObject.LeftGump() == gump && gump->CanBeMoved())
			{
				WISP_GEOMETRY::CPoint2Di offset = g_MouseManager.LeftDroppedOffset();

				x += offset.X;
				y += offset.Y;

				return true;
			}

			gump = gump->m_GroupNext;
		}
	}

	return false;
}
//----------------------------------------------------------------------------------
void CGumpStatusbar::UpdateGroup(int x, int y)
{
	if (!InGroup())
		return;

	CGumpStatusbar *gump = GetTopStatusbar();

	while (gump != NULL)
	{
		if (gump != this)
		{
			gump->MinimizedX += x;
			gump->MinimizedY += y;

			g_GumpManager.MoveToBack(gump);
			gump->WantRedraw = true;
		}

		gump = gump->m_GroupNext;
	}

	g_GumpManager.MoveToBack(this);
}
//----------------------------------------------------------------------------------
void CGumpStatusbar::AddStatusbar(CGumpStatusbar *bar)
{
	if (m_GroupNext == NULL)
	{
		m_GroupNext = bar;
		bar->m_GroupPrev = this;
		bar->m_GroupNext = NULL;
	}
	else
	{
		CGumpStatusbar *gump = m_GroupNext;

		while (gump != NULL && gump->m_GroupNext != NULL)
			gump = gump->m_GroupNext;

		gump->m_GroupNext = bar;
		bar->m_GroupPrev = gump;
		bar->m_GroupNext = NULL;
	}

	if (bar->m_StatusbarUnlocker != NULL)
	{
		bar->m_StatusbarUnlocker->Visible = bar->InGroup();
		bar->WantRedraw = true;
	}

	if (m_StatusbarUnlocker != NULL)
	{
		m_StatusbarUnlocker->Visible = InGroup();

		m_WantRedraw = true;
	}
}
//----------------------------------------------------------------------------------
void CGumpStatusbar::RemoveFromGroup()
{
	if (m_GroupNext != NULL)
	{
		m_GroupNext->WantRedraw = true;
		m_GroupNext->m_GroupPrev = m_GroupPrev;

		if (m_GroupNext->m_StatusbarUnlocker != NULL)
			m_GroupNext->m_StatusbarUnlocker->Visible = m_GroupNext->InGroup();
	}
	
	if (m_GroupPrev != NULL)
	{
		m_GroupPrev->WantRedraw = true;
		m_GroupPrev->m_GroupNext = m_GroupNext;

		if (m_GroupPrev->m_StatusbarUnlocker != NULL)
			m_GroupPrev->m_StatusbarUnlocker->Visible = m_GroupPrev->InGroup();
	}

	m_GroupNext = NULL;
	m_GroupPrev = NULL;

	if (m_StatusbarUnlocker != NULL)
	{
		m_StatusbarUnlocker->Visible = InGroup();
		m_WantRedraw = true;
	}
}
//----------------------------------------------------------------------------------
void CGumpStatusbar::CalculateGumpState()
{
	CGump::CalculateGumpState();

	//Если гамп захватили и (может быть) двигают
	if (g_GumpMovingOffset.X || g_GumpMovingOffset.Y)
	{
		if (g_Target.IsTargeting())
			g_GeneratedMouseDown = true;

		if (!InGroup())
		{
			int testX = g_MouseManager.Position.X;
			int testY = g_MouseManager.Position.Y;

			if (GetNearStatusbar(testX, testY) != NULL)
			{
				g_GumpTranslate.X = (float)testX;
				g_GumpTranslate.Y = (float)testY;
			}
		}
	}
	else if (InGroup())
	{
		int x = (int)g_GumpTranslate.X;
		int y = (int)g_GumpTranslate.Y;

		GetStatusbarGroupOffset(x, y);

		g_GumpTranslate.X = (float)x;
		g_GumpTranslate.Y = (float)y;
	}
}
//----------------------------------------------------------------------------------
void CGumpStatusbar::PrepareContent()
{
	if (m_HitsBody != NULL)
	{
		CGameCharacter *obj = g_World->FindWorldCharacter(m_Serial);

		if (obj == NULL && m_HitsBody->Color != 0x0386)
		{
			m_Body->Color = 0x0386;
			m_HitsBody->Color = 0x0386;
			m_Entry->Color = 0x0386;
		}
		else if (obj != NULL && m_HitsBody->Color == 0x0386)
		{
			m_Body->Color = g_ConfigManager.GetColorByNotoriety(obj->Notoriety);
			m_HitsBody->Color = 0;

			if (obj->CanChangeName)
				m_Entry->Color = 0x000E;
			else
				m_Entry->Color = 0x0386;
		}
	}
}
//----------------------------------------------------------------------------------
void CGumpStatusbar::UpdateContent()
{
	Clear();
	m_StatusbarUnlocker = NULL;
	m_Body = NULL;
	m_HitsBody = NULL;
	m_Entry = NULL;

	CGUIText *text = NULL;

	if (m_Serial == g_PlayerSerial) //Если это статусбар игрока
	{
		if (!m_Minimized) //Если это "полная" версия статусбара
		{
			POINT p = { 0, 0 };

			if (g_ConnectionManager.ClientVersion >= CV_308D)
				Add(new CGUIGumppic(0x2A6C, 0, 0));
			else
			{
				p.x = 244;
				p.y = 112;

				Add(new CGUIGumppic(0x0802, 0, 0));
			}

			//Отрисовка набора характеристик, расположение в зависимости от версии протокола, комментировать не буду...
			if (g_ConnectionManager.ClientVersion >= CV_308Z)
			{
				p.x = 389;
				p.y = 152;

				//Отрисуем имя игрока
				if (g_Player->Name.length())
				{
					text = (CGUIText*)Add(new CGUIText(0x0386, 58, 50));
					text->CreateTextureA(1, g_Player->Name, 320, TS_CENTER);
				}

				if (g_ConnectionManager.ClientVersion >= CV_5020)
				{
					//Кнопка вызова гампа бафов
					Add(new CGUIButton(ID_GSB_BUFF_GUMP, 0x7538, 0x7538, 0x7538, 40, 50));

					//Кнопочки для изменения роста/лока статов
					if (g_ConnectionManager.ClientVersion >= CV_60142)
					{
						//Str
						uchar status = g_Player->LockStr; //Статус (вниз/вверх/замок)

						ushort gumpID = 0x0984; //Up
						if (status == 1)
							gumpID = 0x0986; //Down
						else if (status == 2)
							gumpID = 0x082C; //Lock

						Add(new CGUIButton(ID_GSB_BUFF_LOCKER_STR, gumpID, gumpID, gumpID, 40, 76));


						//Dex
						status = g_Player->LockDex; //Статус (вниз/вверх/замок)

						gumpID = 0x0984; //Up
						if (status == 1)
							gumpID = 0x0986; //Down
						else if (status == 2)
							gumpID = 0x082C; //Lock

						Add(new CGUIButton(ID_GSB_BUFF_LOCKER_STR, gumpID, gumpID, gumpID, 40, 102));



						//Int
						status = g_Player->LockInt; //Статус (вниз/вверх/замок)

						gumpID = 0x0984; //Up
						if (status == 1)
							gumpID = 0x0986; //Down
						else if (status == 2)
							gumpID = 0x082C; //Lock

						Add(new CGUIButton(ID_GSB_BUFF_LOCKER_STR, gumpID, gumpID, gumpID, 40, 132));
					}
				}



				text = (CGUIText*)Add(new CGUIText(0x0386, 88, 77));
				text->CreateTextureA(1, std::to_string(g_Player->Str));

				text = (CGUIText*)Add(new CGUIText(0x0386, 88, 105));
				text->CreateTextureA(1, std::to_string(g_Player->Dex));

				text = (CGUIText*)Add(new CGUIText(0x0386, 88, 133));
				text->CreateTextureA(1, std::to_string(g_Player->Int));

				Add(new CGUIHitBox(ID_GSB_TEXT_STR, 58, 70, 59, 24));
				Add(new CGUIHitBox(ID_GSB_TEXT_DEX, 58, 98, 59, 24));
				Add(new CGUIHitBox(ID_GSB_TEXT_INT, 58, 126, 59, 24));



				int textWidth = 40;

				//Hits
				text = (CGUIText*)Add(new CGUIText(0x0386, 146, 70));
				text->CreateTextureA(1, std::to_string(g_Player->Hits), textWidth, TS_CENTER);

				Add(new CGUILine(146, 82, 180, 82, 0xFF383838));

				text = (CGUIText*)Add(new CGUIText(0x0386, 146, 83));
				text->CreateTextureA(1, std::to_string(g_Player->MaxHits), textWidth, TS_CENTER);

				//Stam
				text = (CGUIText*)Add(new CGUIText(0x0386, 146, 98));
				text->CreateTextureA(1, std::to_string(g_Player->Stam), textWidth, TS_CENTER);

				Add(new CGUILine(146, 110, 180, 110, 0xFF383838));

				text = (CGUIText*)Add(new CGUIText(0x0386, 146, 111));
				text->CreateTextureA(1, std::to_string(g_Player->MaxStam), textWidth, TS_CENTER);

				//Mana
				text = (CGUIText*)Add(new CGUIText(0x0386, 146, 126));
				text->CreateTextureA(1, std::to_string(g_Player->Mana), textWidth, TS_CENTER);

				Add(new CGUILine(146, 138, 180, 138, 0xFF383838));

				text = (CGUIText*)Add(new CGUIText(0x0386, 146, 139));
				text->CreateTextureA(1, std::to_string(g_Player->MaxMana), textWidth, TS_CENTER);

				Add(new CGUIHitBox(ID_GSB_TEXT_HITS, 124, 70, 59, 24));
				Add(new CGUIHitBox(ID_GSB_TEXT_STAM, 124, 98, 59, 24));
				Add(new CGUIHitBox(ID_GSB_TEXT_MANA, 124, 126, 59, 24));



				text = (CGUIText*)Add(new CGUIText(0x0386, 220, 77));
				text->CreateTextureA(1, std::to_string(g_Player->StatsCap));

				text = (CGUIText*)Add(new CGUIText(0x0386, 220, 105));
				text->CreateTextureA(1, std::to_string(g_Player->Luck));

				//Weights
				text = (CGUIText*)Add(new CGUIText(0x0386, 216, 126));
				text->CreateTextureA(1, std::to_string(g_Player->Weight), textWidth, TS_CENTER);

				Add(new CGUILine(216, 138, 250, 138, 0xFF383838));

				text = (CGUIText*)Add(new CGUIText(0x0386, 216, 139));
				text->CreateTextureA(1, std::to_string(g_Player->MaxWeight), textWidth, TS_CENTER);

				Add(new CGUIHitBox(ID_GSB_TEXT_TITHING_POINTS, 188, 70, 65, 24));
				Add(new CGUIHitBox(ID_GSB_TEXT_LUCK, 188, 98, 65, 24));
				Add(new CGUIHitBox(ID_GSB_TEXT_WEIGHT, 188, 126, 65, 24));



				text = (CGUIText*)Add(new CGUIText(0x0386, 280, 77));
				text->CreateTextureA(1, std::to_string(g_Player->MinDamage) + "-" + std::to_string(g_Player->MaxDamage));

				text = (CGUIText*)Add(new CGUIText(0x0386, 280, 105));
				text->CreateTextureA(1, std::to_string(g_Player->Gold));

				text = (CGUIText*)Add(new CGUIText(0x0386, 285, 133));
				text->CreateTextureA(1, std::to_string(g_Player->Followers) + "/" + std::to_string(g_Player->MaxFollowers));

				Add(new CGUIHitBox(ID_GSB_TEXT_DAMAGE, 260, 70, 69, 24));
				Add(new CGUIHitBox(ID_GSB_TEXT_GOLD, 260, 98, 69, 24));
				Add(new CGUIHitBox(ID_GSB_TEXT_FOLLOWERS, 260, 126, 69, 24));



				text = (CGUIText*)Add(new CGUIText(0x0386, 354, 76));
				text->CreateTextureA(1, std::to_string(g_Player->Armor));

				text = (CGUIText*)Add(new CGUIText(0x0386, 354, 92));
				text->CreateTextureA(1, std::to_string(g_Player->FireResistance));

				text = (CGUIText*)Add(new CGUIText(0x0386, 354, 106));
				text->CreateTextureA(1, std::to_string(g_Player->ColdResistance));

				text = (CGUIText*)Add(new CGUIText(0x0386, 354, 120));
				text->CreateTextureA(1, std::to_string(g_Player->PoisonResistance));

				text = (CGUIText*)Add(new CGUIText(0x0386, 354, 134));
				text->CreateTextureA(1, std::to_string(g_Player->EnergyResistance));

				Add(new CGUIHitBox(ID_GSB_TEXT_RESISTANCE_PHYSICAL, 334, 76, 40, 14));
				Add(new CGUIHitBox(ID_GSB_TEXT_RESISTANCE_FIRE, 334, 92, 40, 14));
				Add(new CGUIHitBox(ID_GSB_TEXT_RESISTANCE_COLD, 334, 106, 40, 14));
				Add(new CGUIHitBox(ID_GSB_TEXT_RESISTANCE_POISON, 334, 120, 40, 14));
				Add(new CGUIHitBox(ID_GSB_TEXT_RESISTANCE_ENERGY, 334, 134, 40, 14));
			}
			else
			{
				if (!p.x)
				{
					p.x = 243;
					p.y = 150;
				}

				//Отрисуем имя игрока
				if (g_Player->Name.length())
				{
					text = (CGUIText*)Add(new CGUIText(0x0386, 86, 42));
					text->CreateTextureA(1, g_Player->Name);
				}

				text = (CGUIText*)Add(new CGUIText(0x0386, 86, 61));
				text->CreateTextureA(1, std::to_string(g_Player->Str));

				text = (CGUIText*)Add(new CGUIText(0x0386, 86, 73));
				text->CreateTextureA(1, std::to_string(g_Player->Dex));

				text = (CGUIText*)Add(new CGUIText(0x0386, 86, 85));
				text->CreateTextureA(1, std::to_string(g_Player->Int));

				text = (CGUIText*)Add(new CGUIText(0x0386, 86, 97));
				text->CreateTextureA(1, (g_Player->Sex ? "F" : "M"));

				text = (CGUIText*)Add(new CGUIText(0x0386, 86, 109));
				text->CreateTextureA(1, std::to_string(g_Player->Armor));

				text = (CGUIText*)Add(new CGUIText(0x0386, 171, 61));
				text->CreateTextureA(1, std::to_string(g_Player->Hits) + "/" + std::to_string(g_Player->MaxHits));

				text = (CGUIText*)Add(new CGUIText(0x0386, 171, 73));
				text->CreateTextureA(1, std::to_string(g_Player->Mana) + "/" + std::to_string(g_Player->MaxMana));

				text = (CGUIText*)Add(new CGUIText(0x0386, 171, 85));
				text->CreateTextureA(1, std::to_string(g_Player->Stam) + "/" + std::to_string(g_Player->MaxStam));

				text = (CGUIText*)Add(new CGUIText(0x0386, 171, 97));
				text->CreateTextureA(1, std::to_string(g_Player->Gold));

				text = (CGUIText*)Add(new CGUIText(0x0386, 171, 109));
				text->CreateTextureA(1, std::to_string(g_Player->Weight));

				Add(new CGUIHitBox(ID_GSB_TEXT_STR, 86, 61, 34, 12));
				Add(new CGUIHitBox(ID_GSB_TEXT_DEX, 86, 73, 34, 12));
				Add(new CGUIHitBox(ID_GSB_TEXT_INT, 86, 85, 34, 12));
				Add(new CGUIHitBox(ID_GSB_TEXT_SEX, 86, 97, 34, 12));
				Add(new CGUIHitBox(ID_GSB_TEXT_ARMOR, 86, 109, 34, 12));

				Add(new CGUIHitBox(ID_GSB_TEXT_HITS, 171, 61, 66, 12));
				Add(new CGUIHitBox(ID_GSB_TEXT_MANA, 171, 73, 66, 12));
				Add(new CGUIHitBox(ID_GSB_TEXT_STAM, 171, 85, 66, 12));
				Add(new CGUIHitBox(ID_GSB_TEXT_GOLD, 171, 97, 66, 12));
				Add(new CGUIHitBox(ID_GSB_TEXT_WEIGHT, 171, 109, 66, 12));

				if (g_ConnectionManager.ClientVersion == CV_308D)
				{
					text = (CGUIText*)Add(new CGUIText(0x0386, 171, 124));
					text->CreateTextureA(1, std::to_string(g_Player->StatsCap));

					Add(new CGUIHitBox(ID_GSB_TEXT_MAX_STATS, 171, 124, 34, 12));
				}
				else if (g_ConnectionManager.ClientVersion == CV_308J)
				{
					text = (CGUIText*)Add(new CGUIText(0x0386, 180, 131));
					text->CreateTextureA(1, std::to_string(g_Player->StatsCap));

					text = (CGUIText*)Add(new CGUIText(0x0386, 180, 144));
					text->CreateTextureA(1, std::to_string(g_Player->Followers) + "/" + std::to_string(g_Player->MaxFollowers));

					Add(new CGUIHitBox(ID_GSB_TEXT_MAX_STATS, 180, 131, 34, 12));
					Add(new CGUIHitBox(ID_GSB_TEXT_FOLLOWERS, 171, 144, 34, 12));
				}
			}

			Add(new CGUIHitBox(ID_GSB_MINIMIZE, p.x, p.y, 16, 16, true));
		}
		else //Это уменьшенная врсия статусбара (с полосками)
		{
			if (g_Party.Leader != 0) //inParty
			{
				text = (CGUIText*)Add(new CGUIText(0x0386, 16, -2));
				text->CreateTextureA(3, "[* SELF *]");

				Add(new CGUIButton(ID_GSB_BUTTON_HEAL_1, 0x0938, 0x093A, 0x093A, 16, 20));
				Add(new CGUIButton(ID_GSB_BUTTON_HEAL_2, 0x0939, 0x093A, 0x093A, 16, 33));

				//Hits
				Add(new CGUIGumppic(0x0028, 34, 20));

				int per = CalculatePercents(g_Player->MaxHits, g_Player->Hits, 96);

				if (per > 0)
					Add(new CGUIGumppicTiled(0x0029, 34, 20, per, 0));

				//Mana
				Add(new CGUIGumppic(0x0028, 34, 33));

				per = CalculatePercents(g_Player->MaxMana, g_Player->Mana, 96);

				Add(new CGUIShader(g_ColorizerShader, true));

				if (per > 0)
				{
					CGUIGumppic *gumppic = (CGUIGumppic*)Add(new CGUIGumppicTiled(0x0029, 34, 33, per, 0)); //0x0170 green //0x0035 yellow
					gumppic->Color = 0x0482;
				}

				//Stam
				Add(new CGUIGumppic(0x0028, 34, 45));

				per = CalculatePercents(g_Player->MaxStam, g_Player->Stam, 96);

				if (per > 0)
				{
					CGUIGumppic *gumppic = (CGUIGumppic*)Add(new CGUIGumppicTiled(0x0029, 34, 45, per, 0)); //0x0170 green //0x0035 yellow
					gumppic->Color = 0x0075;
				}

				Add(new CGUIShader(g_ColorizerShader, false));
			}
			else
			{
				if (g_Player->Warmode)
					Add(new CGUIGumppic(0x0807, 0, 0));//Версия с включенным вармодом
				else
					Add(new CGUIGumppic(0x0803, 0, 0)); //Гамп статусбара

				//Hits
				Add(new CGUIGumppic(0x0805, 34, 12));

				int per = CalculatePercents(g_Player->MaxHits, g_Player->Hits, 109);

				if (per > 0)
				{
					ushort gumpid = 0x0806; //Character status line (blue)
					if (g_Player->Poisoned())
						gumpid = 0x0808; //Character status line (green)
					else if (g_Player->YellowHits())
						gumpid = 0x0809; //Character status line (yellow)

					Add(new CGUIGumppicTiled(gumpid, 34, 12, per, 0));
				}

				//Mana
				Add(new CGUIGumppic(0x0805, 34, 25));

				per = CalculatePercents(g_Player->MaxMana, g_Player->Mana, 109);

				if (per > 0)
					Add(new CGUIGumppicTiled(0x0806, 34, 25, per, 0));

				//Stam
				Add(new CGUIGumppic(0x0805, 34, 38));

				per = CalculatePercents(g_Player->MaxStam, g_Player->Stam, 109);

				if (per > 0)
					Add(new CGUIGumppicTiled(0x0806, 34, 38, per, 0));
			}

			m_StatusbarUnlocker = (CGUIButton*)Add(new CGUIButton(ID_GSB_BUTTON_REMOVE_FROM_GROUP, 0x082C, 0x082C, 0x082C, 136, 24));
			m_StatusbarUnlocker->CheckPolygone = true;
			m_StatusbarUnlocker->Visible = InGroup();
		}
	}
	else //Чужой статусбар
	{
		if (g_Party.Contains(m_Serial))
		{
			IFOR(i, 0, 10)
			{
				if (g_Party.Member[i].Serial == m_Serial)
				{
					CPartyObject &member = g_Party.Member[i];

					string memberName = member.GetName(i);

					ushort textColor = 0x0386;

					if (member.CanChangeName)
						textColor = 0x000E;

					CGUITextEntry *entry = (CGUITextEntry*)Add(new CGUITextEntry(ID_GSB_TEXT_FIELD, textColor, textColor, textColor, 16, -2, 150, false, 1, TS_LEFT, UOFONT_FIXED));
					entry->m_Entry.SetText(memberName);
					entry->CheckOnSerial = true;

					if (member.CanChangeName)
					{
						entry->ReadOnly = false;
						Add(new CGUIHitBox(ID_GSB_TEXT_FIELD, 16, -2, 109, 16));
					}
					else
						entry->ReadOnly = true;

					Add(new CGUIButton(ID_GSB_BUTTON_HEAL_1, 0x0938, 0x093A, 0x093A, 16, 20));
					Add(new CGUIButton(ID_GSB_BUTTON_HEAL_2, 0x0939, 0x093A, 0x093A, 16, 33));

					//Hits
					Add(new CGUIGumppic(0x0028, 34, 20));

					int per = CalculatePercents(g_Player->MaxHits, g_Player->Hits, 96);

					if (per > 0)
						Add(new CGUIGumppicTiled(0x0029, 34, 20, per, 0));

					//Mana
					Add(new CGUIGumppic(0x0028, 34, 33));

					per = CalculatePercents(g_Player->MaxMana, g_Player->Mana, 96);

					Add(new CGUIShader(g_ColorizerShader, true));

					if (per > 0)
					{
						CGUIGumppic *gumppic = (CGUIGumppic*)Add(new CGUIGumppicTiled(0x0029, 34, 33, per, 0)); //0x0170 green //0x0035 yellow
						gumppic->Color = 0x0482;
					}

					//Stam
					Add(new CGUIGumppic(0x0028, 34, 45));

					per = CalculatePercents(g_Player->MaxStam, g_Player->Stam, 96);

					if (per > 0)
					{
						CGUIGumppic *gumppic = (CGUIGumppic*)Add(new CGUIGumppicTiled(0x0029, 34, 45, per, 0)); //0x0170 green //0x0035 yellow
						gumppic->Color = 0x0075;
					}

					Add(new CGUIShader(g_ColorizerShader, false));

					break;
				}
			}
		}
		else
		{
			Add(new CGUIShader(g_ColorizerShader, true));

			ushort color = 0;
			ushort hitsColor = 0x0386;
			ushort textColor = 0x0386;
			CGameCharacter *obj = g_World->FindWorldCharacter(m_Serial);
			string objName = "";
			bool canChangeName = false;

			if (obj != NULL)
			{
				hitsColor = 0;
				//Вычисляем цвет статусбара
				color = g_ConfigManager.GetColorByNotoriety(obj->Notoriety);

				if (obj->Notoriety == NT_CRIMINAL || obj->Notoriety == NT_SOMEONE_GRAY)
					color = 0;

				objName = obj->Name;

				if (obj->CanChangeName)
				{
					textColor = 0x000E;
					canChangeName = true;
				}
			}

			m_Body = (CGUIGumppic*)Add(new CGUIGumppic(0x0804, 0, 0));
			m_Body->Color = color;

			m_HitsBody = (CGUIGumppic*)Add(new CGUIGumppic(0x0805, 34, 38));
			m_Body->Color = hitsColor;

			Add(new CGUIShader(g_ColorizerShader, false));

			if (obj != NULL && obj->MaxHits > 0)
			{
				int per = CalculatePercents(obj->MaxHits, obj->Hits, 109);

				if (per > 0)
				{
					ushort gumpid = 0x0806; //Character status line (blue)
					if (obj->Poisoned())
						gumpid = 0x0808; //Character status line (green)
					else if (obj->YellowHits())
						gumpid = 0x0809; //Character status line (yellow)

					Add(new CGUIGumppicTiled(gumpid, 34, 38, per, 0));
				}
			}

			m_Entry = (CGUITextEntry*)Add(new CGUITextEntry(ID_GSB_TEXT_FIELD, textColor, textColor, textColor, 16, 14, 150, false, 1, TS_LEFT, UOFONT_FIXED));
			m_Entry->m_Entry.SetText(objName);
			m_Entry->CheckOnSerial = true;

			if (canChangeName)
			{
				m_Entry->ReadOnly = false;

				Add(new CGUIHitBox(ID_GSB_TEXT_FIELD, 16, 14, 109, 16));
			}
			else
				m_Entry->ReadOnly = true;
		}

		m_StatusbarUnlocker = (CGUIButton*)Add(new CGUIButton(ID_GSB_BUTTON_REMOVE_FROM_GROUP, 0x082C, 0x082C, 0x082C, 136, 24));
		m_StatusbarUnlocker->CheckPolygone = true;
		m_StatusbarUnlocker->Visible = InGroup();
	}

	QFOR(item, m_Items, CBaseGUI*)
	{
		if (item->Serial != ID_GSB_TEXT_FIELD && item->Serial != ID_GSB_MINIMIZE)
			item->MoveOnDrag = true;
	}
}
//----------------------------------------------------------------------------------
void CGumpStatusbar::OnLeftMouseButtonDown()
{
	if (g_GeneratedMouseDown)
		return;

	if (!g_PressedObject.LeftSerial || g_PressedObject.LeftSerial > ID_GSB_LOCK_MOVING)
	{
		//Проверим, может быть есть таргет, который нужно повесить на данного чара
		if (g_Target.IsTargeting())
		{
			g_Target.SendTargetObject(m_Serial);
			g_MouseManager.CancelDoubleClick = true;
		}
	}
}
//----------------------------------------------------------------------------------
void CGumpStatusbar::GUMP_BUTTON_EVENT_C
{
	if (g_GeneratedMouseDown)
		return;

	if (serial == ID_GSB_MINIMIZE && m_Serial == g_PlayerSerial)
	{
		//Кнопка минимизации на полной версии гампа
		m_Minimized = true;
		m_WantUpdateContent = true;
	}
	else if (serial == ID_GSB_LOCK_MOVING)
		m_LockMoving = !m_LockMoving;
	else if (serial == ID_GSB_BUTTON_HEAL_1)
	{
		g_Orion.CastSpell(29);
		g_PartyHelperTimer = g_Ticks + 500;
		g_PartyHelperTarget = m_Serial;
	}
	else if (serial == ID_GSB_BUTTON_HEAL_2)
	{
		g_Orion.CastSpell(11);
		g_PartyHelperTimer = g_Ticks + 500;
		g_PartyHelperTarget = m_Serial;
	}
	else if (serial == ID_GSB_BUTTON_REMOVE_FROM_GROUP)
	{
		CGumpStatusbar *oldGroup = m_GroupNext;

		if (oldGroup == NULL)
			oldGroup = m_GroupPrev;

		RemoveFromGroup();

		if (oldGroup != NULL)
		{
			oldGroup->UpdateGroup(0, 0);
			oldGroup->WantRedraw = true;
		}
	}
	else if (serial == ID_GSB_BUFF_GUMP)
		g_ConfigManager.ToggleBufficonWindow = true;
	else if (serial == ID_GSB_BUFF_LOCKER_STR)
	{
		uchar state = (g_Player->LockStr + 1) % 3;

		CPacketChangeStatLockStateRequest(0, state).Send();
	}
	else if (serial == ID_GSB_BUFF_LOCKER_DEX)
	{
		uchar state = (g_Player->LockDex + 1) % 3;

		CPacketChangeStatLockStateRequest(1, state).Send();
	}
	else if (serial == ID_GSB_BUFF_LOCKER_INT)
	{
		uchar state = (g_Player->LockInt + 1) % 3;

		CPacketChangeStatLockStateRequest(2, state).Send();
	}
}
//----------------------------------------------------------------------------------
bool CGumpStatusbar::OnLeftMouseButtonDoubleClick()
{
	if (g_GeneratedMouseDown)
		return false;

	if (!g_PressedObject.LeftSerial && m_Serial == g_PlayerSerial && m_Minimized)
	{
		//Если это статусбар игрока (с полосками) то развернем его до полной версии
		m_Minimized = false;

		if (InGroup())
		{
			CGumpStatusbar *oldGroup = m_GroupNext;

			if (oldGroup == NULL)
				oldGroup = m_GroupPrev;

			RemoveFromGroup();

			if (oldGroup != NULL)
			{
				oldGroup->UpdateGroup(0, 0);
				oldGroup->WantRedraw = true;
			}
		}

		m_WantUpdateContent = true;

		return true;
	}
	else if (m_Serial != g_PlayerSerial)
	{
		if (g_Player->Warmode)
			g_Orion.Attack(m_Serial); //Если в вармоде - атакуем
		else
			g_Orion.DoubleClick(m_Serial); //Или используем предмет

		return true;
	}
	else if (!m_Minimized)
	{
		//По даблклику по полной версии статусбара теперь открывается папердолл
		g_Orion.PaperdollReq(m_Serial);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------------
void CGumpStatusbar::OnCharPress(const WPARAM &wParam, const LPARAM &lParam)
{
	//Изменение имени существа
	if (Serial != g_PlayerSerial) //Только чужие статусбары
	{
		if ((g_EntryPointer->Length() <= 15) && g_FontManager.GetWidthA(1, g_EntryPointer->c_str(), g_EntryPointer->Pos()) <= 100 && ((wParam >= 'a' && wParam <= 'z') || (wParam >= 'A' && wParam <= 'Z')))
		{
			g_EntryPointer->Insert(wParam);
			m_WantRedraw = true;
		}
	}
}
//----------------------------------------------------------------------------------
void CGumpStatusbar::OnKeyUp(const WPARAM &wParam, const LPARAM &lParam)
{
	switch (wParam)
	{
		case VK_RETURN:
		{
			//Если всё впорядке - изменяем имя
			if (g_EntryPointer->Length())
				SendRenameRequest();
			else //Нельзя изменить имя на пустое
			{
				CGameObject *obj = g_World->FindWorldObject(m_Serial);

				if (obj != NULL)
					g_EntryPointer->SetText(obj->Name);
			}

			if (g_ConfigManager.ConsoleNeedEnter)
				g_EntryPointer = NULL;
			else
				g_EntryPointer = &g_GameConsole;

			m_WantRedraw = true; //Перерисуем

			break;
		}
		case VK_HOME:
		case VK_LEFT:
		case VK_RIGHT:
		case VK_BACK:
		case VK_DELETE:
		case VK_END:
		{
			g_EntryPointer->OnKey(this, wParam);
			m_WantRedraw = true;

			break;
		}
		case VK_ESCAPE:
		{
			//По тыку на Esc можно выйти из редактирования имени существа

			CGameObject *obj = g_World->FindWorldObject(m_Serial);
			if (obj != NULL)
				g_EntryPointer->SetText(obj->Name);

			if (g_ConfigManager.GetConsoleNeedEnter())
				g_EntryPointer = NULL;
			else
				g_EntryPointer = &g_GameConsole;

			m_WantRedraw = true; //Перерисуем

			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------------
void CGumpStatusbar::SendRenameRequest()
{
	QFOR(item, m_Items, CBaseGUI*)
	{
		if (item->Type != GOT_TEXTENTRY)
			continue;

		CEntryText *entry = &((CGUITextEntry*)item)->m_Entry;

		if (entry->Length()) //Если в поле для ввода текста что-то есть
		{
			//Отправляем запрос на изменение имени
			CPacketRenameRequest(m_Serial, entry->c_str()).Send();
		}
	}
}
//----------------------------------------------------------------------------------
