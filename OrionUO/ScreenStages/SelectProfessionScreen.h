/***********************************************************************************
**
** SelectProfessionScreen.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef SELECTPROFESSIONSCREEN_H
#define SELECTPROFESSIONSCREEN_H
//----------------------------------------------------------------------------------
class CSelectProfessionScreen : public CBaseScreen
{
    //!Выбранный навык
protected:
    int m_SkillSelection = 0;

public:
    int GetSkillSelection() { return m_SkillSelection; };
    void SetSkillSelection(int val);

private:
    CGumpScreenSelectProfession m_SelectProfessionGump;

public:
    CSelectProfessionScreen();
    virtual ~CSelectProfessionScreen();

    //!Идентификаторы событий для плавного перехода
    static const uchar ID_SMOOTH_SPS_QUIT = 1;
    static const uchar ID_SMOOTH_SPS_GO_SCREEN_CHARACTER = 2;
    static const uchar ID_SMOOTH_SPS_GO_SCREEN_GAME_CONNECT = 3;
    static const uchar ID_SMOOTH_SPS_GO_SCREEN_CREATE = 4;

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
};
//----------------------------------------------------------------------------------
extern CSelectProfessionScreen g_SelectProfessionScreen;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
