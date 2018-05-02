/***********************************************************************************
**
** GUIHTMLSlider.h
**
** Компонента скроллер для HTMLGump'а
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIHTMLSLIDER_H
#define GUIHTMLSLIDER_H
//----------------------------------------------------------------------------------
class CGUIHTMLSlider : public CGUISlider
{
private:
    //!Гамп-родитель
    class CGUIHTMLGump *m_HTMLGump{ NULL };

public:
    CGUIHTMLSlider(
        class CGUIHTMLGump *htmlGump,
        int serial,
        ushort graphic,
        ushort graphicSelected,
        ushort graphicPressed,
        ushort backgroundGraphic,
        bool compositeBackground,
        bool vertical,
        int x,
        int y,
        int lenght,
        int minValue,
        int maxValue,
        int value);
    virtual ~CGUIHTMLSlider();

    //!Пересчитать смещения
    virtual void CalculateOffset();

    virtual bool IsControlHTML() { return true; }
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
