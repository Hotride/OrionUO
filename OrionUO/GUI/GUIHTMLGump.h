/***********************************************************************************
**
** GUIHTMLGump.h
**
** Компонента гампа с кнопками, скроллером прокрутки, фоном, служит контейнером для объектов
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIHTMLGUMP_H
#define GUIHTMLGUMP_H
//----------------------------------------------------------------------------------
#include "GUIPolygonal.h"
#include "GUIHTMLButton.h"
#include "GUIHTMLHitBox.h"
#include "GUIHTMLSlider.h"
#include "GUIHTMLResizepic.h"
#include "GUIScissor.h"
//----------------------------------------------------------------------------------
class CGUIHTMLGump : public CGUIPolygonal
{
	//!Имеет фон
	SETGET(bool, HaveBackground);

	//!Имеет скроллер с кнопками
	SETGET(bool, HaveScrollbar);

	//!Размер данных, хранимых внутри компоненты
	SETGET(WISP_GEOMETRY::CSize, DataSize);

	//!Смещение к данным
	SETGET(WISP_GEOMETRY::CPoint2Di, DataOffset);

	//!Текущее смещение к данным
	SETGET(WISP_GEOMETRY::CPoint2Di, CurrentOffset);

	//!Доступное смещение для скроллинга
	SETGET(WISP_GEOMETRY::CPoint2Di, AvailableOffset);

private:
	void CalculateDataSize(CBaseGUI *item, int &startX, int &startY, int &endX, int &endY);

public:
	CGUIHTMLGump(const uint &serial, const ushort &graphic, const int &x, const int &y, const int &width, const int &height, const bool &haveBackground, const bool &haveScrollbar);
	virtual ~CGUIHTMLGump();

	//!Ссылки на компоненты
	CGUIHTMLResizepic *m_Background;
	CGUIHTMLButton *m_ButtonUp;
	CGUIHTMLButton *m_ButtonDown;
	CGUIHTMLHitBox *m_HitBoxLeft;
	CGUIHTMLHitBox *m_HitBoxRight;
	CGUIHTMLSlider *m_Slider;
	CGUIScissor *m_Scissor;

	//!Инициализация
	//!			menu - флаг инициализации для гампа CGumpMenu
	void Initalize(bool menu = false);

	//!Обновление высота гампа
	void UpdateHeight(const int &height);

	//!Сброс смещений
	void ResetDataOffset();

	//!Пересчет смещений
	void CalculateDataSize();

	virtual void PrepareTextures();
	
	virtual bool EntryPointerHere();

	virtual bool Select();

	//!Функция прокрутки
	virtual void Scroll(const bool &up, const uint &delay);

	virtual bool IsHTMLGump() { return true; }
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
