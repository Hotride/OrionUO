/***********************************************************************************
**
** GUITilepic.h
**
** Компонента для отображения картинки статики
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUITILEPIC_H
#define GUITILEPIC_H
//----------------------------------------------------------------------------------
class CGUITilepic : public CGUIDrawObject
{
public:
    CGUITilepic(ushort graphic, ushort color, int x, int y);
    virtual ~CGUITilepic();

    virtual WISP_GEOMETRY::CSize GetSize();

    virtual void PrepareTextures();

    virtual void Draw(bool checktrans = false);
    virtual bool Select();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
