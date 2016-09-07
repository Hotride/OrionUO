/***********************************************************************************
**
** GUIComboBox.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUICOMBOBOX_H
#define GUICOMBOBOX_H
//----------------------------------------------------------------------------------
#include "BaseGUI.h"
//----------------------------------------------------------------------------------
class CGUIComboBox : public CBaseGUI
{
	SETGET(ushort, OpenGraphic);
	SETGET(bool, CompositeBackground);
	SETGETE(int, ShowItemsCount, OnChangeShowItemsCount);
	SETGET(int, Width);
	SETGET(int, OpenedWidth);
	SETGET(int, StartIndex);
	SETGET(int, SelectedIndex);
	SETGET(int, ListingDirection);
	SETGET(uint, ListingTimer);
	SETGET(bool, ShowMaximizedCenter);

private:
	CBaseGUI *SkipToStart();
	
	int m_ArrowX;
	int m_OffsetY;
	int m_StepY;
	int m_WorkWidth;
	int m_WorkHeight;

	class CGUIText *m_Text;

public:
	CGUIComboBox(const uint &serial, const ushort &graphic, const bool &compositeBackground, const ushort &openGraphic, const int &x, const int &y, const int &width, const int &showItemsCount, const bool &showMaximizedCenter);
	virtual ~CGUIComboBox();

	void SetText(class CGUIText *text) { m_Text = text; }

	void RecalculateWidth();

	virtual WISP_GEOMETRY::CSize GetSize();

	virtual void PrepareTextures();

	virtual void Draw(const bool &checktrans = false);
	virtual bool Select();

	CBaseGUI *SelectedItem();
	int IsSelectedItem();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
