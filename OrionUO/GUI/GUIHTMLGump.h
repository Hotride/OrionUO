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
class CGUIHTMLGump : public CGUIPolygonal
{
public:
	//!Имеет фон
	bool HaveBackground = false;

	//!Имеет скроллер с кнопками
	bool HaveScrollbar = false;

	//!Размер данных, хранимых внутри компоненты
	WISP_GEOMETRY::CSize DataSize = WISP_GEOMETRY::CSize();

	//!Смещение к данным
	WISP_GEOMETRY::CPoint2Di DataOffset = WISP_GEOMETRY::CPoint2Di();

	//!Текущее смещение к данным
	WISP_GEOMETRY::CPoint2Di CurrentOffset = WISP_GEOMETRY::CPoint2Di();

	//!Доступное смещение для скроллинга
	WISP_GEOMETRY::CPoint2Di AvailableOffset = WISP_GEOMETRY::CPoint2Di();

private:
	void CalculateDataSize(CBaseGUI *item, int &startX, int &startY, int &endX, int &endY);

public:
	CGUIHTMLGump(const uint &serial, const ushort &graphic, const int &x, const int &y, const int &width, const int &height, const bool &haveBackground, const bool &haveScrollbar);
	virtual ~CGUIHTMLGump();

	//!Ссылки на компоненты
	CGUIHTMLResizepic *m_Background{ NULL };
	CGUIHTMLButton *m_ButtonUp{ NULL };
	CGUIHTMLButton *m_ButtonDown{ NULL };
	CGUIHTMLHitBox *m_HitBoxLeft{ NULL };
	CGUIHTMLHitBox *m_HitBoxRight{ NULL };
	CGUIHTMLSlider *m_Slider{ NULL };
	CGUIScissor *m_Scissor{ NULL };

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
