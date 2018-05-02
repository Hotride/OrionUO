/***********************************************************************************
**
** GumpScreenGame.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPSCREENGAME_H
#define GUMPSCREENGAME_H
//----------------------------------------------------------------------------------
class CGumpScreenGame : public CGump
{
private:
    //!Идентификаторы кнопок
    static const int ID_GS_RESIZE = 1;

public:
    CGumpScreenGame();
    virtual ~CGumpScreenGame();

    virtual void UpdateContent();

    virtual void InitToolTip();

    virtual void Draw();

    virtual CRenderObject *Select();

    virtual void OnLeftMouseButtonDown();

    virtual void OnLeftMouseButtonUp();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
