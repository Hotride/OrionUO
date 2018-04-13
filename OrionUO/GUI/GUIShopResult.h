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
class CGUIShopResult : public CBaseGUI
{
public:
	//!Стоимость
	uint Price = 0;

	//!Название
	string Name = "";

private:
	//!Текстура названия
	CGLTextTexture m_NameText{ CGLTextTexture() };

public:
	CGUIShopResult(class CGUIShopItem *shopItem, const int &x, const int &y);
	virtual ~CGUIShopResult();

	//!Компонента кнопок MinMax
	class CGUIMinMaxButtons *m_MinMaxButtons{ NULL };

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
