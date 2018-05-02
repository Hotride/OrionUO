/***********************************************************************************
**
** GUIMinMaxButtons.h
**
** Компонента для отображения блока кнопок MinMax
**
** Copyright (C) September 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIMINMAXBUTTONS_H
#define GUIMINMAXBUTTONS_H
//----------------------------------------------------------------------------------
class CGUIMinMaxButtons : public CBaseGUI
{
public:
    //!Минимальное значение
    int MinValue = 0;

    //!Максимальное значение
    int MaxValue = 0;

    //!Текущее значение
    int Value = 0;

    //!Имеет текст
    bool HaveText = false;

    //!Позиция текста
    SLIDER_TEXT_POSITION TextPosition = STP_RIGHT;

    //!Шрифт текста
    uchar Font = 0;

    //!Цвет текста
    ushort TextColor = 0;

    //!Юникод текст
    bool Unicode = true;

    //!Ширина текста
    int TextWidth = 0;

    //!Ориентация текста
    TEXT_ALIGN_TYPE Align = TS_LEFT;

    //!Флаги текста
    ushort TextFlags = 0;

    //!Координата текста по оси X
    int TextX = 0;

    //!Координата текста по оси Y
    int TextY = 0;

    //!Базовый шаг смещения
    int BaseScrollStep = 1;

    //!Текущий шаг смещения
    int ScrollStep = 1;

    //!Время последней итерации прокрутки
    uint LastScrollTime = 0;

    //!Стандартное смещение текста
    int DefaultTextOffset = 2;

private:
    //!Текстура текста
    CGLTextTexture Text{ CGLTextTexture() };

    //!Метод прокрутки (NoScroll/Max/Min)
    int m_ScrollMode{ 0 };

public:
    CGUIMinMaxButtons(
        int serial, ushort graphic, int x, int y, int minValue, int maxValue, int value);
    virtual ~CGUIMinMaxButtons();

    virtual bool IsPressedOuthit() { return true; }

    virtual WISP_GEOMETRY::CSize GetSize() { return WISP_GEOMETRY::CSize(36, 18); }

    //Прокрутка
    virtual void Scroll(int delay);

    //Нажатие
    virtual void OnClick();

    //!Обновить текст
    void UpdateText();

    //!Установить параметры текста
    void SetTextParameters(
        bool haveText,
        SLIDER_TEXT_POSITION textPosition,
        uchar font,
        ushort color,
        bool unicode,
        int textWidth = 0,
        TEXT_ALIGN_TYPE align = TS_LEFT,
        ushort textFlags = 0);

    virtual void PrepareTextures();

    virtual void Draw(bool checktrans = false);
    virtual bool Select();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
