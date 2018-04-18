/***********************************************************************************
**
** GUIScrollBackground.h
**
** Компонента для отображения фона для свиткоподобных гампов
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUISCROLLBACKGROUND_H
#define GUISCROLLBACKGROUND_H
//----------------------------------------------------------------------------------
class CGUIScrollBackground : public CBaseGUI
{
public:
	//!Высота
	int Height = 0;

	//!Смещение по оси X
	int OffsetX = 0;

	//!Смещение нижней части по оси X
	int BottomOffsetX = 0;

	//!Ширина
	int Width = 0;

	//!Рабочая область
	WISP_GEOMETRY::CRect WorkSpace = WISP_GEOMETRY::CRect();

	CGUIScrollBackground(int serial, ushort graphic, int x, int y, int height);
	virtual ~CGUIScrollBackground();

	//!Обновить высоту
	void UpdateHeight(int height);

	virtual WISP_GEOMETRY::CSize GetSize() { return WISP_GEOMETRY::CSize(Width, Height); }

	virtual void PrepareTextures();

	virtual void Draw(bool checktrans = false);
	virtual bool Select();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
