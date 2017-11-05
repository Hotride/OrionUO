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
class CGUIComboBox : public CBaseGUI
{
	//!ИД картинки в развернутом состоянии
	SETGET(ushort, OpenGraphic, 0);

	//!Флаг составного фона, если включено - используются повторяющиеся картинки (5 штук) m_OpenGraphic, в выключенном состоянии выводит Resizepic из m_OpenGraphic
	SETGET(bool, CompositeBackground, false);

	//!Количество отображаемых строк в открытом списке
	SETGETE(int, ShowItemsCount, 0);

	//!Ширина в свернутом состоянии (только для отрисовки Resizepic)
	SETGET(int, Width, 0);

	//!Ширина в развернутом состоянии (только для отрисовки Resizepic)
	SETGET(int, OpenedWidth, 0);

	//!Стартовый индекс для отображения в развернутом состоянии
	SETGET(int, StartIndex, 0);

	//!Текущий выбранный индекс
	SETGET(int, SelectedIndex, -1);

	//!Направление скроллинга (вычисляется в Select())
	SETGET(int, ListingDirection, 0);

	//!Таймер скроллинга
	SETGET(uint, ListingTimer, 0);

	//!Центрировать развернутое состояние по оси Y
	SETGET(bool, ShowMaximizedCenter, false);

	//!Смещение текста для отображения
	SETGET(int, TextOffsetY, 0);

private:
	//!Получить указатель на первый объект отображения
	CBaseGUI *SkipToStart();

	//!Позиция стрелки вниз (в свернутом состоянии)
	int m_MinimizedArrowX{ 0 };

	//!Позиция стрелки вниз
	int m_ArrowX{ 0 };

	//!Смещение по оси Y
	int m_OffsetY{ 0 };

	//!Шаг по оси Y
	int m_StepY{ 0 };

	//!Рабочая ширина
	int m_WorkWidth{ 0 };

	//!Рабочая высота
	int m_WorkHeight{ 0 };

	//!Выводимый в свернутом состоянии текст
	class CGUIText *m_Text{ NULL };

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
