/***********************************************************************************
**
** GUISkillItem.h
**
** Компонента для отображения навыка
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUISKILLITEM_H
#define GUISKILLITEM_H
//----------------------------------------------------------------------------------
class CGUISkillItem : public CBaseGUI
{
public:
	//!Индекс навыка
	int Index = 0;

	//!Статус навыка
	SETGETE(uchar, Status, 0);

private:
	//!Текустуры текста
	CGLTextTexture m_NameText{ CGLTextTexture() };
	CGLTextTexture m_ValueText{ CGLTextTexture() };

	//!Получить ИД картинки кнопки статуса
	ushort GetStatusButtonGraphic();

public:
	CGUISkillItem(const uint &serial, const uint &useSerial, const uint &statusSerial, const int &index, const int &x, const int &y);
	virtual ~CGUISkillItem();

	//!Компоненты управления
	CGUIButton *m_ButtonUse{ NULL };
	CGUIButton *m_ButtonStatus{ NULL };

	//!Создать текстуру значения навыка
	void CreateValueText(const bool &showReal = false, const bool &showCap = false);

	virtual WISP_GEOMETRY::CSize GetSize() { return WISP_GEOMETRY::CSize(255, 17); }

	virtual void PrepareTextures();

	virtual void Draw(const bool &checktrans = false);
	virtual bool Select();

	//!Получить ссылку на выбранную компоненту или на себя
	CBaseGUI *SelectedItem();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
