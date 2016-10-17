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
	SETGETE(int, Scale, OnChangeScale);
	SETGETE(int, Map, OnChangeMap);
	SETGETE(bool, LinkWithPlayer, OnChangeLinkWithPlayer);
	SETGET(int, OffsetX);
	SETGET(int, OffsetY);
	SETGET(bool, Called);
	SETGET(int, CurrentOffsetX);
	SETGET(int, CurrentOffsetY);

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

	int GetCurrentMap();

	CGUIButton *m_Minimizer;
	CGUIResizepic *m_Background;
	CGUIResizeButton *m_Resizer;
	CGUIText *m_Text;
	CGUICheckbox *m_Checkbox;
	CGUIScissor *m_Scissor;
	CGUIWorldMapTexture *m_MapData;
	CGUIComboBox *m_ComboboxScale;
	CGUIComboBox *m_ComboboxMap;

protected:
	virtual void CalculateGumpState();

public:
	CGumpWorldMap(uint serial, short x, short y);
	virtual ~CGumpWorldMap();

	void UpdateSize();

	virtual bool CanBeDisplayed() { return m_Called; }

	virtual void GenerateFrame(bool stop);

	virtual void PrepareContent();

	GUMP_BUTTON_EVENT_H;
	GUMP_CHECKBOX_EVENT_H;
	GUMP_COMBOBOX_SELECTION_EVENT_H;
	GUMP_RESIZE_START_EVENT_H;
	GUMP_RESIZE_EVENT_H;
	GUMP_RESIZE_END_EVENT_H;

	virtual void OnLeftMouseButtonDown();
	virtual void OnLeftMouseButtonUp();
	virtual bool OnLeftMouseButtonDoubleClick();
	virtual void OnMidMouseButtonScroll(const bool &up);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
