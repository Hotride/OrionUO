/***********************************************************************************
**
** ConnectionScreen.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef CONNECTIONSCREEN_H
#define CONNECTIONSCREEN_H
//----------------------------------------------------------------------------------
class CConnectionScreen : public CBaseScreen
{
protected:
    bool m_ConnectionFailed = false;

public:
    bool GetConnectionFailed() { return m_ConnectionFailed; };
    void SetConnectionFailed(bool val);

protected:
    bool m_Connected = false;

public:
    bool GetConnected() { return m_Connected; };
    void SetConnected(bool val);

protected:
    bool m_Completed = false;

public:
    bool GetCompleted() { return m_Completed; };
    void SetCompleted(bool val);

protected:
    int m_ErrorCode = 0;

public:
    int GetErrorCode() { return m_ErrorCode; };
    void SetErrorCode(int val);

protected:
    CONNECTION_SCREEN_TYPE m_Type = CST_LOGIN;

public:
    CONNECTION_SCREEN_TYPE GetType() { return m_Type; };
    void SetType(CONNECTION_SCREEN_TYPE val);

protected:
    string m_Text = "";

public:
    string GetText() { return m_Text; };
    void SetText(const string &val);

private:
    CGumpScreenConnection m_ConnectionGump;

public:
    CConnectionScreen();
    ~CConnectionScreen();

    //!Идентификаторы событий для плавного перехода
    static const uchar ID_SMOOTH_CS_GO_SCREEN_MAIN = 1;
    static const uchar ID_SMOOTH_CS_GO_SCREEN_CHARACTER = 2;
    static const uchar ID_SMOOTH_CS_GO_SCREEN_PROFESSION = 3;
    static const uchar ID_SMOOTH_CS_SEND_DELETE = 4;

    /*!
	Инициализация
	@return 
	*/
    void Init();

    /*!
	Обработка события после плавного затемнения экрана
	@param [__in_opt] action Идентификатор действия
	@return 
	*/
    void ProcessSmoothAction(uchar action = 0xFF);

    virtual void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam);
};
//----------------------------------------------------------------------------------
extern CConnectionScreen g_ConnectionScreen;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
