/***********************************************************************************
**
** ProfessionManager.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef PROFESSIONMANAGER_H
#define PROFESSIONMANAGER_H
//----------------------------------------------------------------------------------
//!Класс менеджера профессий
class CProfessionManager : public CBaseQueue
{
private:
    //!Количество ключей конфига
    static const int m_KeyCount = 18;

    //!Ключи конфига
    static const string m_Keys[m_KeyCount];

    enum
    {
        PM_CODE_BEGIN = 1,
        PM_CODE_NAME,
        PM_CODE_TRUENAME,
        PM_CODE_DESC,
        PM_CODE_TOPLEVEL,
        PM_CODE_GUMP,
        PM_CODE_TYPE,
        PM_CODE_CHILDREN,
        PM_CODE_SKILL,
        PM_CODE_STAT,
        PM_CODE_STR,
        PM_CODE_INT,
        PM_CODE_DEX,
        PM_CODE_END,
        PM_CODE_TRUE,
        PM_CODE_CATEGORY,
        PM_CODE_NAME_CLILOC_ID,
        PM_CODE_DESCRIPTION_CLILOC_ID
    };

    /*!
	Получить код кофига по строке
	@param [__in] key Строка из файла
	@return Код конфига
	*/
    int GetKeyCode(const string &key);

    /*!
	Обработать часть файла (загрузка профессии)
	@param [__in] file Указатель на файл
	@return true при успешной обработке
	*/
    bool ParseFilePart(WISP_FILE::CTextFileParser &file);

    /*!
	Добавить ребенка в профессию
	@param [__in] parent Ссылка на родителя
	@param [__in] child Ссылка на детишку
	@return true в случае успешного добавления
	*/
    bool AddChild(CBaseProfession *parent, CBaseProfession *child);

    /*!
	Загрузка описания профессий
	@return 
	*/
    void LoadProfessionDescription();

public:
    CProfessionManager();
    virtual ~CProfessionManager();

    //!Получить указатель на выбранную профессию
    CBaseProfession *Selected;

    /*!
	Получить родителя профессии
	@param [__in] obj Ссылка на объект профессии
	@param [__in_opt] check Ссылка на проверочный объект профессии
	@return Ссылка на найденного родителя объекта профессии
	*/
    CBaseProfession *GetParent(CBaseProfession *obj, CBaseProfession *check = NULL);

    /*!
	Загрузка профессий из файла
	@return true в случае успешной загрузки
	*/
    bool Load();
};
//---------------------------------------------------------------------------
//!Ссылка на менеджер профессий
extern CProfessionManager g_ProfessionManager;
//---------------------------------------------------------------------------
#endif