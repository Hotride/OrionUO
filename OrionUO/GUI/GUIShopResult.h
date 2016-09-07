/***********************************************************************************
**
** GUIShopResult.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUISHOPRESULT_H
#define GUISHOPRESULT_H
//----------------------------------------------------------------------------------
#include "BaseGUI.h"
#include "../GLEngine/GLTextTexture.h"
//----------------------------------------------------------------------------------
class CGUIShopResult : public CBaseGUI
{
	SETGET(uint, Price);
	SETGET(string, Name);

private:
	CGLTextTexture m_NameText;

public:
	CGUIShopResult(class CGUIShopItem *shopItem, const int &x, const int &y);
	virtual ~CGUIShopResult();

	class CGUIMinMaxButtons *m_MinMaxButtons;

	CBaseGUI *SelectedItem();

	virtual WISP_GEOMETRY::CSize GetSize() { return WISP_GEOMETRY::CSize(200, m_NameText.Height); }

	virtual void PrepareTextures();

	virtual void Draw(const bool &checktrans = false);
	virtual bool Select();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
