/***********************************************************************************
**
** GumpWorldMap.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPWORLDMAP_H
#define GUMPWORLDMAP_H
//----------------------------------------------------------------------------------
#include "Gump.h"
//----------------------------------------------------------------------------------
class CGumpWorldMap : public CGump
{
	SETGET(int, Width);
	SETGET(int, Height);
	SETGET(int, Scale);
	SETGET(int, Map);
	SETGET(bool, LinkWithPlayer);
	SETGET(int, OffsetX);
	SETGET(int, OffsetY);
	SETGET(bool, Called);

private:
	static const int ID_GWM_MINIMIZE = 1;
	static const int ID_GWM_RESIZE = 2;
	static const int ID_GWM_MAP = 3;
	static const int ID_GWM_LINK_WITH_PLAYER = 4;
	static const int ID_GWM_MAP_LIST = 10;
	static const int ID_GWM_SCALE_LIST = 20;

	static const int MIN_WORLD_MAP_HEIGHT = 300;
	static const int MIN_WORLD_MAP_WIDTH = 400;

	int m_StartResizeWidth;
	int m_StartResizeHeight;

	bool m_MapMoving;

	void FixOffsets(int &offsetX, int &offsetY, int &width, int &height);
	void GetScaledDimensions(int &width, int &height, int &playerX, int &playerY);
	void GetCurrentCenter(int &x, int &y, int &mouseX, int &mouseY);
	void ScaleOffsets(int newScale, int mouseX, int mouseY);
	void LoadMap(int map);

	void UpdateSize();

public:
	CGumpWorldMap(uint serial, short x, short y);
	virtual ~CGumpWorldMap();

	GUMP_BUTTON_EVENT_H;
	GUMP_CHECKBOX_EVENT_H;
	GUMP_COMBOBOX_SELECTION_EVENT_H;
	GUMP_RESIZE_START_EVENT_H;
	GUMP_RESIZE_EVENT_H;
	GUMP_RESIZE_END_EVENT_H;

	void OnLeftMouseButtonDown();
	bool OnLeftMouseButtonDoubleClick();
	void OnMidMouseButtonScroll(const bool &up);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
