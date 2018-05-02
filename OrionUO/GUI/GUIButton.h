/***********************************************************************************
**
** GUIButton.h
**
** Компонента для отображения кнопки
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIBUTTON_H
#define GUIBUTTON_H
//----------------------------------------------------------------------------------
class CGUIButton : public CGUIDrawObject
{
public:
    //!ИД картинки для состояния, когда мышка находится над кнопкой
    ushort GraphicSelected = 0;

    //!ИД картинки для зажатого состояния
    ushort GraphicPressed = 0;

    //!Индекс страницы, если указан -1 то используется событие OnButton для гампа с идентификатором кнопки, если отличное значение - переход на страницу без вызова события
    int ToPage = -1;

    //!Флаг обработки зажатого состояния кнопки каждую итерацию рендера
    bool ProcessPressedState = false;

    CGUIButton(
        int serial, ushort graphic, ushort graphicSelected, ushort graphicPressed, int x, int y);
    virtual ~CGUIButton();

    virtual void PrepareTextures();
    virtual ushort GetDrawGraphic();

    virtual void OnMouseEnter();
    virtual void OnMouseExit();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
