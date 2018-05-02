/***********************************************************************************
**
** GumpQuestion.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GumpQuestionH
#define GumpQuestionH
//----------------------------------------------------------------------------------
class CGumpQuestion : public CGump
{
    uchar Variant = 0;

private:
    static const int ID_GQ_BUTTON_CANCEL = 1;
    static const int ID_GQ_BUTTON_OKAY = 2;

public:
    CGumpQuestion(uint serial, short x, short y, uchar variant);
    virtual ~CGumpQuestion();

    static const int ID_GQ_STATE_QUIT = 1;
    static const int ID_GQ_STATE_ATTACK_REQUEST = 2;

    GUMP_BUTTON_EVENT_H;
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
