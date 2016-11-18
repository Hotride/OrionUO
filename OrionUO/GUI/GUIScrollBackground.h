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
#include "BaseGUI.h"
//----------------------------------------------------------------------------------
class CGUIScrollBackground : public CBaseGUI
{
	//!Высота
	SETGET(int, Height);

	//!Смещение по оси X
	SETGET(int, OffsetX);

	//!Смещение нижней части по оси X
	SETGET(int, BottomOffsetX);

	//!Ширина
	SETGET(int, Width);

	//!Рабочая область
	SETGET(WISP_GEOMETRY::CRect, WorkSpace);

public:
	CGUIScrollBackground(const uint &serial, const ushort &graphic, const int &x, const int &y, const int &height);
	virtual ~CGUIScrollBackground();

	//!Обновить высоту
	void UpdateHeight(const int &height);

	virtual WISP_GEOMETRY::CSize GetSize() { return WISP_GEOMETRY::CSize(m_Width, m_Height); }

	virtual void PrepareTextures();

	virtual void Draw(const bool &checktrans = false);
	virtual bool Select();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
