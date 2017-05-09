/***********************************************************************************
**
** GUIShopItem.h
**
** Компонента для отображения объекта магазина
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
	//!Количество
	SETGET(uint, Count);

	//!Стоимость
	SETGET(uint, Price);

	//!Название товара
	SETGET(string, Name);

	//!Флаг выбора
	SETGET(bool, Selected);

	//!Имя создано из клилока
	SETGET(bool, NameFromCliloc);

private:
	//!Текстуры для текста
	CGLTextTexture m_NameText;
	CGLTextTexture m_CountText;

	//!Смещение картинки
	int m_ImageOffset;

	//!Смещение текста
	int m_TextOffset;

	//!Максимальное смещение
	int m_MaxOffset;

public:
	CGUIShopItem(const uint &serial, const ushort &graphic, const ushort &color, const uint &count, const uint &price, const string &name, const int &x, const int &y);
	virtual ~CGUIShopItem();

	//!Нажатие на компоненту
	void OnClick();

	//!Создать текстуру названия
	void CreateNameText();

	void UpdateOffsets();

	//!Создать текстуру количества
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
