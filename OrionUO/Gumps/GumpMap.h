/***********************************************************************************
**
** GumpMap.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPMAP_H
#define GUMPMAP_H
//----------------------------------------------------------------------------------
#include "Gump.h"
//----------------------------------------------------------------------------------
class CGumpMap : public CGump
{
	SETGETE(int, PlotState, OnChangePlotState);
	SETGET(int, StartX);
	SETGET(int, StartY);
	SETGET(int, EndX);
	SETGET(int, EndY);
	SETGET(int, Width);
	SETGET(int, Height);

private:
	static const int ID_GM_PLOT_COURSE = 1;
	static const int ID_GM_STOP_PLOTTING = 2;
	static const int ID_GM_CLEAR_COURSE = 3;
	static const int ID_GM_PIN_LIST = 10;
	static const int ID_GM_PIN_LIST_INSERT = 1000;

	uint m_PinTimer;

	CBaseGUI *m_PinOnCursor;
	CBaseGUI *m_FakeInsertionPin;

	CGUIButton *m_PlotCourse;
	CGUIButton *m_StopPlotting;
	CGUIButton *m_ClearCourse;

	int LineUnderMouse(int &x1, int &y1, int x2, int y2);

public:
	CGumpMap(uint serial, ushort graphic, int startX, int startY, int endX, int endY, int width, int height);
	virtual ~CGumpMap();

	CGUIExternalTexture *m_Texture;
	CGUIDataBox *m_DataBox;

	virtual void PrepareContent();

	virtual void GenerateFrame(bool stop);

	virtual CRenderObject *Select();

	GUMP_BUTTON_EVENT_H;
	
	void OnLeftMouseButtonDown();
	void OnLeftMouseButtonUp();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
