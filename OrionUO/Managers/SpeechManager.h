/***********************************************************************************
**
** SpeechManager.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef SPEECHMANAGER_H
#define SPEECHMANAGER_H
//----------------------------------------------------------------------------------
class CLangCode
{
public:
    int Code = 0;

    //! Строковая, уникальная абревиатура локали.
    string Abbreviature = "";

    //! Числовая абревиатура группы? локали.
    uint Unknown = 0;

    //! Название языка.
    string Language = "";

    //! Название страны.
    string Country = "";

    CLangCode() {}
    CLangCode(const string &abbreviature, int code, const string &language, const string &country)
        : Abbreviature(abbreviature)
        , Code(code)
        , Language(language)
        , Country(country)
    {
    }
    virtual ~CLangCode() {}
};
//----------------------------------------------------------------------------------
class CSpeechItem
{
public:
    //! Код посылаемый клиенту.
    ushort Code = 0;

    //! Строка с ключевым словом для поиска в вводе.
    wstring Data = L"";

    bool CheckStart = false;
    bool CheckEnd = false;

    CSpeechItem() {}
    CSpeechItem(ushort code, const wstring &data);
    virtual ~CSpeechItem() {}
};
//----------------------------------------------------------------------------------
//!Класс менеджера cпича
class CSpeechManager
{
    CLangCode *CurrentLanguage = NULL;

private:
    vector<CSpeechItem> m_SpeechEntries;
    vector<CLangCode> m_LangCodes;
    bool m_Loaded{ false };

public:
    CSpeechManager();
    ~CSpeechManager();

    /*!
	Загрузка cпич данных
	@return true при успешной загрузке
	*/
    bool LoadSpeech();

    /*!
	Загрузка данных о локалях
	@return true при успешной загрузке
	*/
    bool LoadLangCodes();

    //Достаём вектор с кодами словосочетаний найденных в вводе
    void GetKeywords(const wchar_t *text, UINT_LIST &codes);
};
//----------------------------------------------------------------------------------
//!Ссылка на менеджер cпича
extern CSpeechManager g_SpeechManager;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
