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
class CGUIShopItem : public CBaseGUI
{
public:
	//!Количество
	uint Count = 0;

	//!Стоимость
	uint Price = 0;

	//!Название товара
	string Name = "";

	//!Флаг выбора
	bool Selected = false;

	//!Имя создано из клилока
	bool NameFromCliloc = false;

private:
	//!Текстуры для текста
	CGLTextTexture m_NameText{ CGLTextTexture() };
	CGLTextTexture m_CountText{ CGLTextTexture() };

	//!Смещение картинки
	int m_ImageOffset{ 0 };

	//!Смещение текста
	int m_TextOffset{ 0 };

	//!Максимальное смещение
	int m_MaxOffset{ 0 };

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
