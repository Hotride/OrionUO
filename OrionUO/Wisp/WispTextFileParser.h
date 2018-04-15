//----------------------------------------------------------------------------------
#ifndef WISPTEXTFILEPARSER_H
#define WISPTEXTFILEPARSER_H
//----------------------------------------------------------------------------------
namespace WISP_FILE
{
//----------------------------------------------------------------------------------
class CTextFileParser
{
public:
	string RawLine = "";

private:
	//Указатель на адрес в памяти, куда загружен файл
	CMappedFile m_File;

	//Указатель на текущий обрабатываемый символ
	puchar m_Ptr{ 0 };

	//Адрес конца строка
	puchar m_EOL{ 0 };

	//Адрес конца файла
	puchar m_End{ 0 };

	//Разделители
	char m_Delimiters[50];

	//Количество разделителей
	int m_DelimitersSize{ 0 };

	//Комментарии
	char m_Comentaries[50];

	//Количество коментариев
	int m_ComentariesSize{ 0 };

	//Набор кавычек, формат кавычек в наборе: openQuote, closeQuote (могут быть одинаковыми)
	char m_Quotes[50];

	//Количество элементов в наборе кавычек
	int m_QuotesSize{ 0 };

	//Обрезать строки на выходе
	bool m_Trim{ false };

	//Получить конец строки
	void GetEOL();

	//Проверка на разделитель
	bool IsDelimiter();

	//Пропустить все до данных
	void SkipToData();

	//Проверка на комментарий
	bool IsComment();

	//Проверка на кавычку
	bool IsQuote();

	//Проверка на закрывающую кавычку
	bool IsSecondQuote();

	//Получить следующий токен
	string ObtainData();

	//Получить следующий фрагмент строки или токен (если кавычки не обнаружены)
	string ObtainQuotedData();

	void StartupInitalize(const char *delimiters, const char *comentaries, const char *quotes);

	void SaveRawLine();

public:
	CTextFileParser(const string &path, const char *delimiters = "", const char *comentaries = "", const char *quotes = "");
	CTextFileParser(const wstring &path, const char *delimiters = "", const char *comentaries = "", const char *quotes = "");
	~CTextFileParser();

	//Сбросить указатель на старт
	void Restart();

	//Проверка на конец файла
	bool IsEOF();

	//Прочитать токены из файла
	STRING_LIST ReadTokens(bool trim = true);

	//Прочитать токены из строки
	STRING_LIST GetTokens(const char *str, bool trim = true);
};
//----------------------------------------------------------------------------------
class CTextFileWritter
{
private:
	//Указатель на адрес в памяти, куда загружен файл
	FILE *m_File{ NULL };

public:
	CTextFileWritter(const string &path);
	CTextFileWritter(const wstring &path);
	~CTextFileWritter();

	bool Opened() { return (m_File != NULL); };

	void Close();

	void WriteString(const string &key, const string &value);

	void WriteInt(const string &key, int value);

	void WriteBool(const string &key, bool value);
};
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
#endif

