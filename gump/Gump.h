/****************************************************************************
**
** Gump.h
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
#ifndef GumpH
#define GumpH
//---------------------------------------------------------------------------
class TGump : public IGump, public TRenderObject
{
//private:
protected:
	GUMP_TYPE m_GumpType;
	DWORD m_ID;

	int m_MinimizedX;
	int m_MinimizedY;

	bool m_NoClose;
	bool m_NoMove;
	bool m_Minimized;
	bool m_FrameCreated;
	bool m_FrameRedraw;
	bool m_Blocked;
	bool m_LockMoving;

	virtual void CalculateGumpState();
public:
	TGump(GUMP_TYPE type, DWORD serial, short x, short y);
	virtual ~TGump();

	virtual TTextRenderer *GetTextRenderer() {return NULL;}

	SETGET(GUMP_TYPE, GumpType);
	SETGET(DWORD, ID);
	SETGETEX(int, MinimizedX);
	SETGETEX(int, MinimizedY);
	SETGET(bool, NoMove);
	SETGET(bool, NoClose);
	SETGET(bool, Minimized);
	SETGET(bool, FrameCreated);
	SETGET(bool, FrameRedraw);
	SETGET(bool, Blocked);
	SETGET(bool, LockMoving);

	virtual void GetBaseProperties(DWORD &serial, WORD &graphic, WORD &color, WORD &x, WORD &y);

	virtual void ChangeHeight() {}

	virtual void UpdateFrame();
	virtual void GenerateFrame(int posX, int posY) {}

	virtual bool CanBeDisplayed() {return true;}
	bool CanBeMoved();

	void DrawLocker();

	virtual int Draw(bool &mode) {return 0;}

	virtual void OnLeftMouseDown() {}
	virtual void OnLeftMouseUp() {}
	virtual void OnRightMouseDown() {}
	virtual void OnRightMouseUp() {}
	virtual bool OnLeftMouseDoubleClick() {return false;}
	virtual bool OnRightMouseDoubleClick() {return false;}
	virtual void OnMouseWheel(MOUSE_WHEEL_STATE &state) {}
	virtual void OnCharPress(WPARAM &wparam, LPARAM &lparam) {}
	virtual void OnKeyPress(WPARAM &wparam, LPARAM &lparam) {}

	virtual bool EntryPointerHere() {return false;}

	virtual int CalculateScrollerAndTextPosition(int &currentLine, int &visibleLines, int &maxY, int currentY);
	virtual int CalculateScrollerY(int &currentLine, int &visibleLines, int &maxY);

	void SendGumpResponse(int index);
	void SendMenuResponse(int index);
	void SendTradingResponse(int code);
	void SendTextEntryDialogResponse(bool mode);
	void SendRenameRequest();
	void SendSellList();
	void SendBuyList();
	void SendTipRequest(BYTE flag);
};
//---------------------------------------------------------------------------
extern TGump *g_ResizedGump;
//---------------------------------------------------------------------------
#endif