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
class CGumpWorldMap : public CGump
{
public:
	int Width = MIN_WORLD_MAP_WIDTH;
	int Height = MIN_WORLD_MAP_HEIGHT;
	SETGETE(int, Scale, 2);
	SETGETE(int, Map, 0);
	SETGETE(bool, LinkWithPlayer, true);
	int OffsetX = 0;
	int OffsetY = 0;
	bool Called = false;
	int CurrentOffsetX = 0;
	int CurrentOffsetY = 0;

private:
	static const int ID_GWM_MINIMIZE = 1;
	static const int ID_GWM_RESIZE = 2;
	static const int ID_GWM_MAP = 3;
	static const int ID_GWM_LINK_WITH_PLAYER = 4;
	static const int ID_GWM_MAP_LIST = 10;
	static const int ID_GWM_SCALE_LIST = 20;

	static const int MIN_WORLD_MAP_HEIGHT = 300;
	static const int MIN_WORLD_MAP_WIDTH = 400;

	int m_StartResizeWidth{ 0 };
	int m_StartResizeHeight{ 0 };

	bool m_MapMoving{ false };

	void FixOffsets(int &offsetX, int &offsetY, int &width, int &height);
	void GetScaledDimensions(int &width, int &height, int &playerX, int &playerY);
	void GetCurrentCenter(int &x, int &y, int &mouseX, int &mouseY);
	void ScaleOffsets(int newScale, int mouseX, int mouseY);
	void LoadMap(const int &map);

	int GetCurrentMap();

	CGUIButton *m_Minimizer{ NULL };
	CGUIResizepic *m_Background{ NULL };
	CGUIResizeButton *m_Resizer{ NULL };
	CGUIText *Text{ NULL };
	CGUICheckbox *m_Checkbox{ NULL };
	CGUIScissor *m_Scissor{ NULL };
	CGUIWorldMapTexture *m_MapData{ NULL };
	CGUIComboBox *m_ComboboxScale{ NULL };
	CGUIComboBox *m_ComboboxMap{ NULL };

protected:
	virtual void CalculateGumpState();

public:
	CGumpWorldMap(short x, short y);
	virtual ~CGumpWorldMap();

	void UpdateSize();

	virtual bool CanBeDisplayed() { return Called; }

	virtual void GenerateFrame(const bool &stop);

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
