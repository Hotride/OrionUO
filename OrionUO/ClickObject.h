/***********************************************************************************
**
** ClickObject.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef CLICKOBJECT_H
#define CLICKOBJECT_H
//----------------------------------------------------------------------------------
//!Объект клика мышкой
class CClickObject
{
public:
    uint Timer = 0;
    ushort X = 0;
    ushort Y = 0;
    bool Enabled = false;
    uint Page = 0;
    CRenderObject *Object = NULL;
    CGump *Gump = NULL;
    CClickObject() {}
    ~CClickObject() {}

    void Clear(CRenderObject *obj);
    void Clear();

    void Init(CRenderObject *obj, CGump *gump = NULL);
};
//----------------------------------------------------------------------------------
extern CClickObject g_ClickObject; //Ссылка на объект клика
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
