/***********************************************************************************
**
** GUIShopItem.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUISHOPITEM_H
#define GUISHOPITEM_H
//----------------------------------------------------------------------------------
#include "BaseGUI.h"
#include "../GLEngine/GLTextTexture.h"
//----------------------------------------------------------------------------------
class CGUIShopItem : public CBaseGUI
{
	SETGET(uint, Count);
	SETGET(uint, Price);
	SETGET(string, Name);
	SETGET(bool, Selected);

private:
	CGLTextTexture m_NameText;
	CGLTextTexture m_CountText;

	int m_ImageOffset;
	int m_TextOffset;
	int m_MaxOffset;

public:
	CGUIShopItem(const uint &serial, const ushort &graphic, const ushort &color, const uint &count, const uint &price, const string &name, const int &x, const int &y);
	virtual ~CGUIShopItem();

	void OnClick();

	void CreateNameText();
	void CreateCountText(const int &lostCount);

	virtual WISP_GEOMETRY::CSize GetSize() { return WISP_GEOMETRY::CSize(200, m_MaxOffset + 20); }

	virtual void PrepareTextures();

	virtual void SetShaderMode();

	virtual void Draw(const bool &checktrans = false);
	virtual bool Select();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
