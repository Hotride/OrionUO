/***********************************************************************************
**
** GUIText.h
**
** Компонента для отображения текста
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUITEXT_H
#define GUITEXT_H
//----------------------------------------------------------------------------------
class CGUIText : public CBaseGUI
{
public:
    CGUIText(ushort color, int x, int y);
    virtual ~CGUIText();

    //!Текстура текста
    CGLTextTexture m_Texture{ CGLTextTexture() };

    virtual WISP_GEOMETRY::CSize GetSize()
    {
        return WISP_GEOMETRY::CSize(m_Texture.Width, m_Texture.Height);
    }

    //!Создать обычную текстуру текста
    void CreateTextureA(
        uchar font,
        const string &str,
        int width = 0,
        TEXT_ALIGN_TYPE align = TS_LEFT,
        ushort flags = 0);

    //!Создать юникодовскую текстуру текста
    void CreateTextureW(
        uchar font,
        const wstring &str,
        uchar cell = 30,
        int width = 0,
        TEXT_ALIGN_TYPE align = TS_LEFT,
        ushort flags = 0);

    virtual void Draw(bool checktrans = false);
    virtual bool Select();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
