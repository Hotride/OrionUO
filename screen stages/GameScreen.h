/****************************************************************************
**
** GameScreen.h
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
#ifndef GameScreenH
#define GameScreenH
//---------------------------------------------------------------------------
const int RENDER_LIST_STACK_SIZE = 1000;
const int MAX_LIGHT_SOURCES = 100;
//---------------------------------------------------------------------------
struct RENDER_VARIABLES_FOR_GAME_WINDOW
{
	int GameWindowPosX;
	int GameWindowPosY;

	int GameWindowSizeX;
	int GameWindowSizeY;

	int GameWindowCenterX;
	int GameWindowCenterY;

	int RealMinRangeX;
	int RealMaxRangeX;
	int RealMinRangeY;
	int RealMaxRangeY;

	int MinBlockX;
	int MinBlockY;
	int MaxBlockX;
	int MaxBlockY;

	int MinPixelsX;
	int MaxPixelsX;
	int MinPixelsY;
	int MaxPixelsY;
};
//---------------------------------------------------------------------------
struct LIGHT_DATA
{
	BYTE ID;
	WORD Color;
	WORD X;
	WORD Y;
	char Z;
	int DrawX;
	int DrawY;
};
//---------------------------------------------------------------------------
struct RENDER_LIST_DATA
{
	TRenderWorldObject *obj;
	short DrawX;
	short DrawY;
};
//---------------------------------------------------------------------------
class TGameScreen : public TBaseScreen
{
private:
	RENDER_VARIABLES_FOR_GAME_WINDOW m_RenderBounds;
	bool m_GameWindowMoving;
	bool m_GameWindowResizing;

	LIGHT_DATA m_Light[MAX_LIGHT_SOURCES];
	int m_LightCount;
	TFrameBuffer m_LightBuffer;

	char m_StartDrawZ;
	int m_ListWidth;
	int m_ListHeight;
	RENDER_LIST_DATA m_List[200][200];
	RENDER_LIST_DATA m_CheckList[200][200];

	void CreateRenderList();

	void RemoveLight(WORD x, WORD y, char z);

	int GetMaxDrawZ(bool &noDrawRoof, int &maxGroundZ);
	void CheckMouseEvents(bool &charSelected);
public:
	TGameScreen();
	virtual ~TGameScreen();

	void CalculateGameWindow();

	void Init();

	void InitTooltip();
	
	void AddLight(LIGHT_DATA &light);

	int Render(bool mode);

	void OnLeftMouseDown();
	void OnLeftMouseUp();
	void OnRightMouseDown();
	void OnRightMouseUp();
	bool OnLeftMouseDoubleClick();
	bool OnRightMouseDoubleClick();
	void OnMouseWheel(MOUSE_WHEEL_STATE state);
	void OnCharPress(WPARAM wparam, LPARAM lparam);
	void OnKeyPress(WPARAM wparam, LPARAM lparam);
};

extern TGameScreen *GameScreen;
//---------------------------------------------------------------------------
#endif