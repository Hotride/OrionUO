/****************************************************************************
**
** GumpWorldMap.h
**
** Copyright (C) November 2015 Hotride
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
#ifndef GumpWorldMapH
#define GumpWorldMapH
//---------------------------------------------------------------------------
class TGumpWorldMap : public TGump
{
private:
	static const int ID_GWM_MINIMIZE = 1;
	static const int ID_GWM_RESIZE = 2;
	static const int ID_GWM_MAP = 3;
	static const int ID_GWM_LINK_WITH_PLAYER = 4;
	static const int ID_GWM_MAP_LIST = 10;
	static const int ID_GWM_SCALE_LIST = 20;

	static const int ID_GWM_LIST = 100;
	
	static const int MIN_WORLD_MAP_HEIGHT = 300;
	static const int MIN_WORLD_MAP_WIDTH = 400;

	static TTextTexture m_Text;
	static TTextTexture m_TextMap[7];
	static TTextTexture m_TextScale[7];

	int m_Width;
	int m_Height;
	int m_Scale;
	int m_Map;
	bool m_Resizing;
	bool m_MapMoving;
	bool m_LinkWithPlayer;
	int m_OffsetX;
	int m_OffsetY;
	BYTE m_OpenedList;
	bool m_Called;

	void FixOffsets(int &offsetX, int &offsetY, int &width, int &height);
	void GetScaledDimensions(int &width, int &height, int &playerX, int &playerY);
	void GetCurrentCenter(int &x, int &y, int &mouseX, int &mouseY);
	void ScaleOffsets(int newScale, int mouseX, int mouseY);
	void LoadMap(int map);
public:
	TGumpWorldMap(DWORD serial, short x, short y);
	virtual ~TGumpWorldMap();

	static void InitTextTextures();
	static void ReleaseTextTextures();

	SETGET(int, Width)
	SETGET(int, Height)
	SETGET(int, Scale)
	SETGET(int, Map)
	SETGET(bool, LinkWithPlayer)
	SETGET(int, OffsetX)
	SETGET(int, OffsetY)
	SETGET(bool, Called)

	void ChangeHeight();

	void PrepareTextures();

	int Draw(bool &mode);

	void GenerateFrame(int posX, int posY);
	
	void OnLeftMouseDown();
	void OnLeftMouseUp();
	bool OnLeftMouseDoubleClick();
	void OnMouseWheel(MOUSE_WHEEL_STATE &state);
};
//---------------------------------------------------------------------------
#endif