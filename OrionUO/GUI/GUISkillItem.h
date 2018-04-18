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
protected:
    uchar m_Status = 0;
public:
    uchar GetStatus() { return m_Status; };
    void SetStatus(uchar val);

private:
	//!Текустуры текста
	CGLTextTexture m_NameText{ CGLTextTexture() };
	CGLTextTexture m_ValueText{ CGLTextTexture() };

	//!Получить ИД картинки кнопки статуса
	ushort GetStatusButtonGraphic();

public:
	CGUISkillItem(int serial, int useSerial, int statusSerial, int index, int x, int y);
	virtual ~CGUISkillItem();

	//!Компоненты управления
	CGUIButton *m_ButtonUse{ NULL };
	CGUIButton *m_ButtonStatus{ NULL };

	//!Создать текстуру значения навыка
	void CreateValueText(bool showReal = false, bool showCap = false);

	virtual WISP_GEOMETRY::CSize GetSize() { return WISP_GEOMETRY::CSize(255, 17); }

	virtual void PrepareTextures();

	virtual void Draw(bool checktrans = false);
	virtual bool Select();

	//!Получить ссылку на выбранную компоненту или на себя
	CBaseGUI *SelectedItem();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
