/***********************************************************************************
**
** GUIDrawObject.h
**
** Компонента для отображения рисуемых гампов (общая реализация).
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIDRAWOBJECT_H
#define GUIDRAWOBJECT_H
//----------------------------------------------------------------------------------
class CGUIDrawObject : public CBaseGUI
{
public:
    CGUIDrawObject(GUMP_OBJECT_TYPE type, int serial, ushort graphic, ushort color, int x, int y);
    virtual ~CGUIDrawObject();

    virtual WISP_GEOMETRY::CSize GetSize();

    virtual void SetShaderMode();
    virtual void PrepareTextures();

    virtual void Draw(bool checktrans = false);
    virtual bool Select();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
