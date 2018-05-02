/***********************************************************************************
**
** SelectedObject.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef SELECTEDOBJECT_H
#define SELECTEDOBJECT_H
//----------------------------------------------------------------------------------
class CSelectedObject
{
public:
    uint Serial = 0;
    CRenderObject *Object = NULL;
    CGump *Gump = NULL;
    CSelectedObject();
    virtual ~CSelectedObject();

    void Clear();
    void Clear(CRenderObject *obj);

    void Init(CRenderObject *obj, CGump *gump = NULL);
    void Init(const CSelectedObject &obj);
};
//----------------------------------------------------------------------------------
extern CSelectedObject g_SelectedObject;
extern CSelectedObject g_LastSelectedObject;
//----------------------------------------------------------------------------------
#endif
