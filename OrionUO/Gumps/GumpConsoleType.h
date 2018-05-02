/***********************************************************************************
**
** GumpConsoleType.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPCONSOLETYPE_H
#define GUMPCONSOLETYPE_H
//----------------------------------------------------------------------------------
class CGumpConsoleType : public CGump
{
protected:
    bool m_ShowFullText = false;

public:
    bool GetShowFullText() { return m_ShowFullText; };
    void SetShowFullText(bool val);

protected:
    int m_SelectedType{ 0 };

    static const int ID_GCT_NORMAL = 1;
    static const int ID_GCT_YELL = 2;
    static const int ID_GCT_WHISPER = 3;
    static const int ID_GCT_EMOTE = 4;
    static const int ID_GCT_COMMAND = 5;
    static const int ID_GCT_BROADCAST = 6;
    static const int ID_GCT_PARTY = 7;
    static const int ID_GCT_MINIMIZE = 8;
    static const int ID_GCT_SHOW_FULL_TEXT = 9;

    virtual void CalculateGumpState();

public:
    CGumpConsoleType(bool minimized, bool showFullText);
    virtual ~CGumpConsoleType();

    virtual bool CanBeDisplayed();

    virtual void UpdateContent();

    virtual void InitToolTip();

    bool ConsoleIsEmpty();

    void DeleteConsolePrefix();
    void SetConsolePrefix();

    GUMP_BUTTON_EVENT_H;
    GUMP_CHECKBOX_EVENT_H;
    GUMP_TEXT_ENTRY_EVENT_H;
};
//----------------------------------------------------------------------------------
extern CGumpConsoleType *g_GumpConsoleType;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
