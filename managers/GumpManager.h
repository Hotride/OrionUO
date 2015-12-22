/****************************************************************************
**
** GumpManager.h
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
#ifndef GumpManagerH
#define GumpManagerH
//--------------------------------------------------------------------------
class TGumpManager : public IGumpManager, public TBaseQueue
{
private:
	int GetNonpartyStatusbarsCount();
public:
	TGumpManager() : TBaseQueue() {}
	virtual ~TGumpManager() {}

	void AddGump(TGump *obj);
	TGump *GetGump(DWORD serial, DWORD ID, GUMP_TYPE Type);
	TGump *GetTextEntryOwner();
	TGump *GumpExists(DWORD gumpID);
	void CloseGump(DWORD serial, DWORD ID, GUMP_TYPE Type);
	void RemoveGump(TGump *obj);

	void RedrawAll();

	void PrepareTextures();
	
	void Draw(bool &mode, bool blocked);
	
	void OnLeftMouseDown(bool blocked);
	bool OnLeftMouseUp(bool blocked);
	void OnRightMouseDown(bool blocked);
	void OnRightMouseUp(bool blocked);
	bool OnLeftMouseDoubleClick(bool blocked);
	bool OnRightMouseDoubleClick(bool blocked);
	void OnMouseWheel(MOUSE_WHEEL_STATE state, bool blocked);
	bool OnCharPress(WPARAM wparam, LPARAM lparam, bool blocked);
	bool OnKeyPress(WPARAM wparam, LPARAM lparam, bool blocked);

	void OnDelete();

	void RemoveRangedGumps();
	
	void Load(string path);
	void Save(string path);
};

extern TGumpManager *GumpManager;
//--------------------------------------------------------------------------
#endif