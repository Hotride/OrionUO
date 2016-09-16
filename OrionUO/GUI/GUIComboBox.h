/***********************************************************************************
**
** GUIComboBox.h
**
** Компонента для отображения выпадающего списка
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
	//!ИД картинки в развернутом состоянии
	SETGET(ushort, OpenGraphic);

	//!Флаг составного фона, если включено - используются повторяющиеся картинки (5 штук) m_OpenGraphic, в выключенном состоянии выводит Resizepic из m_OpenGraphic
	SETGET(bool, CompositeBackground);

	//!Количество отображаемых строк в открытом списке
	SETGETE(int, ShowItemsCount, OnChangeShowItemsCount);

	//!Ширина в свернутом состоянии (только для отрисовки Resizepic)
	SETGET(int, Width);

	//!Ширина в развернутом состоянии (только для отрисовки Resizepic)
	SETGET(int, OpenedWidth);

	//!Стартовый индекс для отображения в развернутом состоянии
	SETGET(int, StartIndex);

	//!Текущий выбранный индекс
	SETGET(int, SelectedIndex);

	//!Направление скроллинга (вычисляется в Select())
	SETGET(int, ListingDirection);

	//!Таймер скроллинга
	SETGET(uint, ListingTimer);

	//!Центрировать развернутое состояние по оси Y
	SETGET(bool, ShowMaximizedCenter);

	//!Смещение текста для отображения
	SETGET(int, TextOffsetY);

private:
	//!Получить указатель на первый объект отображения
	CBaseGUI *SkipToStart();
	
	//!Позиция стрелки вниз
	int m_ArrowX;

	//!Смещение по оси Y
	int m_OffsetY;

	//!Шаг по оси Y
	int m_StepY;

	//!Рабочая ширина
	int m_WorkWidth;

	//!Рабочая высота
	int m_WorkHeight;

	//!Выводимый в свернутом состоянии текст
	class CGUIText *m_Text;

public:
	CGUIComboBox(const uint &serial, const ushort &graphic, const bool &compositeBackground, const ushort &openGraphic, const int &x, const int &y, const int &width, const int &showItemsCount, const bool &showMaximizedCenter);
	virtual ~CGUIComboBox();

	//!Установить текст
	void SetText(class CGUIText *text) { m_Text = text; }

	//!Пересчитать ширину
	void RecalculateWidth();

	virtual WISP_GEOMETRY::CSize GetSize();

	virtual void PrepareTextures();

	virtual void Draw(const bool &checktrans = false);
	virtual bool Select();

	//!Получить ссылку на выбранный элемент списка (или на самого себя)
	CBaseGUI *SelectedItem();

	//!Получить индекс выбранного элемента, -1 если ничего не выбрано
	int IsSelectedItem();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
