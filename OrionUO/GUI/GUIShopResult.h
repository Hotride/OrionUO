/***********************************************************************************
**
** GUIShopResult.h
**
** Компонента для отображения выбранного товара
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
	//!Стоимость
	SETGET(uint, Price);

	//!Название
	SETGET(string, Name);

private:
	//!Текстура названия
	CGLTextTexture m_NameText;

public:
	CGUIShopResult(class CGUIShopItem *shopItem, const int &x, const int &y);
	virtual ~CGUIShopResult();

	//!Компонента кнопок MinMax
	class CGUIMinMaxButtons *m_MinMaxButtons;

	//!Получить ссылку на выбранную компоненту или ссылку на себя
	CBaseGUI *SelectedItem();

	virtual WISP_GEOMETRY::CSize GetSize() { return WISP_GEOMETRY::CSize(200, m_NameText.Height); }

	virtual void PrepareTextures();

	virtual void Draw(const bool &checktrans = false);
	virtual bool Select();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
