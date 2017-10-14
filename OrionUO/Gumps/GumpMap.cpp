/***********************************************************************************
**
** GumpMap.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGumpMap::CGumpMap(uint serial, ushort graphic, int startX, int startY, int endX, int endY, int width, int height)
: CGump(GT_MAP, serial, 0, 0), m_StartX(startX), m_StartY(startY),
m_EndX(endX), m_EndY(endY), m_Width(width), m_Height(height)
{
	WISPFUN_DEBUG("c99_f1");
	m_Graphic = graphic;

	Add(new CGUIResizepic(0, 0x1432, 0, 0, m_Width + 44, m_Height + 61)); //Map Gump

	m_PlotCourse = (CGUIButton*)Add(new CGUIButton(ID_GM_PLOT_COURSE, 0x1398, 0x1398, 0x1398, (m_Width - 100) / 2, 5)); //Plot Course
	m_StopPlotting = (CGUIButton*)Add(new CGUIButton(ID_GM_STOP_PLOTTING, 0x1399, 0x1399, 0x1399, (m_Width - 70) / 2, 5)); //Stop Plotting
	m_ClearCourse = (CGUIButton*)Add(new CGUIButton(ID_GM_CLEAR_COURSE, 0x139A, 0x139A, 0x139A, (m_Width - 66) / 2, m_Height + 37)); //Clear Course

	m_PlotCourse->Visible = (m_PlotState == 0);
	m_StopPlotting->Visible = (m_PlotState == 1);
	m_ClearCourse->Visible = (m_PlotState == 1);

	m_Texture = (CGUIExternalTexture*)Add(new CGUIExternalTexture(new CGLTexture(), true, 24, 31));

	Add(new CGUIGumppic(0x139D, m_Width - 20, m_Height - 20)); //N

	m_DataBox = (CGUIDataBox*)Add(new CGUIDataBox());
	m_DataBox->Visible = false;
}
//----------------------------------------------------------------------------------
CGumpMap::~CGumpMap()
{
	for (CGUIText *text : m_Labels)
	{
		delete text;
	}

	m_Labels.clear();
}
//----------------------------------------------------------------------------------
void CGumpMap::OnChangePlotState(const int &val)
{
	WISPFUN_DEBUG("c99_f2");
	m_PlotCourse->Visible = (val == 0);
	m_StopPlotting->Visible = (val == 1);
	m_ClearCourse->Visible = (val == 1);

	m_WantRedraw = true;
}
//----------------------------------------------------------------------------------
int CGumpMap::LineUnderMouse(int &x1, int &y1, int x2, int y2)
{
	WISPFUN_DEBUG("c99_f3");
	int tempX = x2 - x1;
	int tempY = y2 - y1;

	float testOfsX = (float)tempX;

	if (testOfsX == 0.0f)
		testOfsX = 1.0f;

	float pi = (float)M_PI;

	float a = -(atan(tempY / testOfsX) * 180.0f / pi);

	bool inverseCheck = false;

	if (x1 >= x2 && y1 <= y2)
		inverseCheck = true;
	else if (x1 >= x2 && y1 >= y2)
		inverseCheck = true;

	float sinA = sin(a * pi / 180.f);
	float cosA = cos(a * pi / 180.f);

	int offsX = (int)((tempX * cosA) - (tempY * sinA));
	int offsY = (int)((tempX * sinA) + (tempY * cosA));

	int endX2 = x1 + offsX;
	int endY2 = y1 + offsY;

	tempX = g_MouseManager.Position.X - x1;
	tempY = g_MouseManager.Position.Y - y1;

	offsX = (int)((tempX * cosA) - (tempY * sinA));
	offsY = (int)((tempX * sinA) + (tempY * cosA));

	POINT mousePoint = {x1 + offsX, y1 + offsY};

	const int polyOffset = 5;

	int result = 0;

	if (!inverseCheck)
	{
		RECT lineRect = {x1 - polyOffset, y1 - polyOffset, endX2 + polyOffset, endY2 + polyOffset};

		if (PtInRect(&lineRect, mousePoint))
		{
			x1 = x1 + ((x2 - x1) / 2);
			y1 = y1 + ((y2 - y1) / 2);

			result = 1;
		}
	}
	else
	{		
		RECT lineRect = {endX2 - polyOffset, endY2 - polyOffset, x1 + polyOffset, y1 + polyOffset};

		if (PtInRect(&lineRect, mousePoint))
		{
			x1 = x2 + ((x1 - x2) / 2);
			y1 = y2 + ((y1 - y2) / 2);

			result = 2;
		}
	}

	return result;
}
//----------------------------------------------------------------------------------
void CGumpMap::PrepareContent()
{
	WISPFUN_DEBUG("c99_f4");
	if (m_DataBox != NULL)
	{
		int serial = 1;

		QFOR(item, m_DataBox->m_Items, CBaseGUI*)
		{
			item->Serial = serial;

			serial++;
		}

		//Если окошко захвачено для перемещения - вычислим оффсеты
		if (g_PressedObject.LeftGump == this && g_PressedObject.LeftObject != NULL && m_PlotState)
		{
			if (m_PinOnCursor == NULL)
			{
				WISP_GEOMETRY::CPoint2Di offset = g_MouseManager.LeftDroppedOffset();

				if ((offset.X || offset.Y) && g_PressedObject.LeftSerial > ID_GM_PIN_LIST && g_PressedObject.LeftSerial < ID_GM_PIN_LIST_INSERT && m_PinTimer > g_Ticks)
					m_PinOnCursor = (CBaseGUI*)g_PressedObject.LeftObject;
			}

			if (m_PinOnCursor != NULL)
			{
				int newX = g_MouseManager.Position.X - (m_X + 20);
				int newY = g_MouseManager.Position.Y - (m_Y + 30);

				m_WantRedraw = (m_PinOnCursor->X != newX || m_PinOnCursor->Y != newY);

				m_PinOnCursor->X = newX;
				m_PinOnCursor->Y = newY;
			}
		}

		m_NoMove = (m_PinOnCursor != NULL);
	}
}
//----------------------------------------------------------------------------------
void CGumpMap::GenerateFrame()
{
	WISPFUN_DEBUG("c99_f5");

	//m_Labels

	if (m_DataBox != NULL)
	{
		int idx = 0;

		QFOR(item, m_DataBox->m_Items, CBaseGUI*)
		{
			int drawX = item->X + 18;
			int drawY = item->Y + 21;

			if (item != m_PinOnCursor)
			{
				CGUIText *text = NULL;

				if (idx >= (int)m_Labels.size())
				{
					text = new CGUIText(0, drawX - 10, drawY - 12);
					m_Labels.push_back(text);
				}
				else
					text = m_Labels[idx];

				idx++;
				text->CreateTextureA(0, std::to_string(idx));
			}
		}
	}

	CGump::GenerateFrame();

	if (m_DataBox != NULL)
	{
		int idx = 1;

		QFOR(item, m_DataBox->m_Items, CBaseGUI*)
		{
			int drawX = item->X + 18;
			int drawY = item->Y + 21;

			CBaseGUI *next = (CBaseGUI*)item->m_Next;

			if (next != NULL)
			{
				int nextDrawX = next->X + 20;
				int nextDrawY = next->Y + 30;

				if (next == m_PinOnCursor || item == m_PinOnCursor)
					glColor4f(0.87f, 0.87f, 0.87f, 1.0f);
				else
					glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

				g_GL.DrawLine(drawX + 2, drawY + 8, nextDrawX, nextDrawY);

				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

				if (m_PinOnCursor == NULL && g_SelectedObject.Serial >= ID_GM_PIN_LIST_INSERT && (g_SelectedObject.Serial - ID_GM_PIN_LIST_INSERT) == idx)
				{
					int checkX = drawX + 2;
					int checkY = drawY + 8;

					if (LineUnderMouse(checkX, checkY, nextDrawX, nextDrawY))
						g_Orion.DrawGump(0x139B, 0, checkX - 2, checkY - 8);
				}
			}

			if (item != m_PinOnCursor)
			{
				g_Orion.DrawGump(0x139B, 0, drawX, drawY);

				if (idx - 1 < (int)m_Labels.size())
				{
					CGUIText *text = m_Labels[idx - 1];
					text->Draw();
				}

				idx++;
			}
		}
	}
}
//----------------------------------------------------------------------------------
CRenderObject *CGumpMap::Select()
{
	WISPFUN_DEBUG("c99_f6");
	CRenderObject *selected = CGump::Select();

	if (m_DataBox != NULL)
	{
		WISP_GEOMETRY::CPoint2Di oldPos = g_MouseManager.Position;
		g_MouseManager.Position = WISP_GEOMETRY::CPoint2Di(oldPos.X - (int)g_GumpTranslate.X, oldPos.Y - (int)g_GumpTranslate.Y);

		QFOR(item, m_DataBox->m_Items, CBaseGUI*)
		{
			int drawX = item->X + 18;
			int drawY = item->Y + 21;

			CBaseGUI *next = (CBaseGUI*)item->m_Next;

			if (next != NULL)
			{
				int nextDrawX = next->X + 20;
				int nextDrawY = next->Y + 30;

				int checkX = drawX + 2;
				int checkY = drawY + 8;

				if (LineUnderMouse(checkX, checkY, nextDrawX, nextDrawY))
				{
					g_SelectedObject.Init(item, this);
					g_SelectedObject.Serial = item->Serial + ID_GM_PIN_LIST_INSERT;
				}
			}

			if (g_Orion.PolygonePixelsInXY(drawX, drawY, 10, 10))
			{
				g_SelectedObject.Init(item, this);
				g_SelectedObject.Serial = item->Serial + ID_GM_PIN_LIST;
			}
		}

		g_MouseManager.Position = oldPos;
	}

	return selected;
}
//----------------------------------------------------------------------------
void CGumpMap::GUMP_BUTTON_EVENT_C
{
	WISPFUN_DEBUG("c99_f7");
	if (serial == ID_GM_PLOT_COURSE || serial == ID_GM_STOP_PLOTTING) //Plot Course /Stop Plotting
	{
		CPacketMapMessage(m_Serial, MM_EDIT, m_PlotState).Send();
		PlotState = !m_PlotState;

		m_WantRedraw = true;
	}
	else if (serial == ID_GM_CLEAR_COURSE) //Clear Course
	{
		CPacketMapMessage(m_Serial, MM_CLEAR).Send();

		m_DataBox->Clear();

		m_WantRedraw = true;
	}
}
//----------------------------------------------------------------------------
void CGumpMap::OnLeftMouseButtonDown()
{
	WISPFUN_DEBUG("c99_f8");
	CGump::OnLeftMouseButtonDown();

	m_PinTimer = g_Ticks + 300;
}
//----------------------------------------------------------------------------
void CGumpMap::OnLeftMouseButtonUp()
{
	WISPFUN_DEBUG("c99_f9");
	CGump::OnLeftMouseButtonUp();

	if (m_DataBox != NULL && g_PressedObject.LeftObject != NULL)
	{
		if (m_PlotState && m_PinOnCursor == NULL && m_PinTimer > g_Ticks)
		{
			if (g_PressedObject.LeftSerial >= ID_GM_PIN_LIST_INSERT)
			{
				int idx = g_PressedObject.LeftSerial - ID_GM_PIN_LIST_INSERT - 1;

				CBaseGUI *first = (CBaseGUI*)m_DataBox->Get(idx);

				if (first != NULL)
				{
					int x = m_X;
					int y = m_Y;

					int drawX = x + first->X + 18;
					int drawY = y + first->Y + 21;

					CBaseGUI *next = (CBaseGUI*)first->m_Next;

					if (next != NULL)
					{
						int nextDrawX = x + next->X + 20;
						int nextDrawY = y + next->Y + 30;

						int checkX = drawX + 2;
						int checkY = drawY + 8;

						if (LineUnderMouse(checkX, checkY, nextDrawX, nextDrawY))
						{
							checkX = checkX - (x + 20);
							checkY = checkY - (y + 29);

							CPacketMapMessage(m_Serial, MM_INSERT, idx + 1, checkX, checkY).Send();

							m_DataBox->Insert(first, new CGUIGumppic(0x139B, checkX, checkY));
							m_WantRedraw = true;
						}
					}
				}
			}
			else
			{
				int x = m_X + 24;
				int y = m_Y + 32;

				if (g_Orion.PolygonePixelsInXY(x, y, m_Width, m_Height))
				{
					x = g_MouseManager.Position.X - x - 4;
					y = g_MouseManager.Position.Y - y - 2;

					CPacketMapMessage(m_Serial, MM_ADD, 0, x, y).Send();

					m_DataBox->Add(new CGUIGumppic(0x139B, x, y));
					m_WantRedraw = true;
				}
			}
		}
	}

	if (m_PinOnCursor != NULL && m_DataBox != NULL)
	{
		int x = m_X + 24;
		int y = m_Y + 32;

		int idx = g_PressedObject.LeftSerial - ID_GM_PIN_LIST - 1;

		if (g_Orion.PolygonePixelsInXY(x, y, m_Width, m_Height))
		{
			x = g_MouseManager.Position.X - (x - 4);
			y = g_MouseManager.Position.Y - (y - 2);

			m_PinOnCursor->X = x;
			m_PinOnCursor->Y = y;

			CPacketMapMessage(Serial, MM_MOVE, idx, x, y).Send();
		}
		else
		{
			CPacketMapMessage(Serial, MM_REMOVE, idx).Send();

			m_DataBox->Delete(m_PinOnCursor);
		}

		m_WantRedraw = true;
		m_PinOnCursor = NULL;
	}
}
//----------------------------------------------------------------------------