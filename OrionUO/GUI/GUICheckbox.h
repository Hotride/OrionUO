/***********************************************************************************
**
** GUICheckbox.h
**
** Компонента для отображения чекбокса
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUICHECKBOX_H
#define GUICHECKBOX_H
//----------------------------------------------------------------------------------
class CGUICheckbox : public CGUIDrawObject
{
public:
    //!ИД картинки в нажатом состоянии
    ushort GraphicChecked = 0;

    //!ИД картинки в выбранном состоянии
    ushort GraphicSelected = 0;

    //!ИД картинки в отключенном состоянии
    ushort GraphicDisabled = 0;

    //!Состояние компоненты
    bool Checked = false;

    //!Позиция текста
    SLIDER_TEXT_POSITION TextPosition = STP_RIGHT;

    //!Координата текста по оси X
    int TextX = 0;

    //!Координата текста по оси Y
    int TextY = 0;

    //!Стандартное смещение текста
    int DefaultTextOffset = 2;

protected:
    CGLTextTexture Text{ CGLTextTexture() };

    void UpdateTextPosition();

public:
    CGUICheckbox(
        int serial, ushort graphic, ushort graphicChecked, ushort graphicDisabled, int x, int y);
    virtual ~CGUICheckbox();

    //!Установить текст
    void SetTextParameters(
        uchar font,
        const wstring &text,
        ushort color,
        SLIDER_TEXT_POSITION textPosition = STP_RIGHT,
        int textWidth = 0,
        TEXT_ALIGN_TYPE align = TS_LEFT,
        ushort textFlags = 0);
    void SetTextParameters(
        uchar font,
        const string &text,
        ushort color,
        SLIDER_TEXT_POSITION textPosition = STP_RIGHT,
        int textWidth = 0,
        TEXT_ALIGN_TYPE align = TS_LEFT,
        ushort textFlags = 0);

    virtual void PrepareTextures();
    virtual ushort GetDrawGraphic();

    virtual void Draw(bool checktrans = false);
    virtual bool Select();

    virtual void OnMouseEnter();
    virtual void OnMouseExit();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
