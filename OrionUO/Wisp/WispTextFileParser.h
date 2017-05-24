//----------------------------------------------------------------------------------
#ifndef WISPTEXTFILEPARSER_H
#define WISPTEXTFILEPARSER_H
//----------------------------------------------------------------------------------
#include "WispMappedFile.h"
//----------------------------------------------------------------------------------
namespace WISP_FILE
{
//----------------------------------------------------------------------------------
class CTextFileParser
{
	SETGET(string, RawLine);

private:
	//Указатель на адрес в памяти, куда загружен файл
	CMappedFile m_File;

	//Указатель на текущий обрабатываемый символ
	puchar m_Ptr;

	//Адрес конца строка
	puchar m_EOL;

	//Адрес конца файла
	puchar m_End;

	//Разделители
	char m_Delimiters[50];

	//Количество разделителей
	int m_DelimitersSize;

	//Комментарии
	char m_Comentaries[50];

	//Количество коментариев
	int m_ComentariesSize;

	//Набор кавычек, формат кавычек в наборе: openQuote, closeQuote (могут быть одинаковыми)
	char m_Quotes[50];

	//Количество элементов в наборе кавычек
	int m_QuotesSize;

	//Обрезать строки на выходе
	bool m_Trim;

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
}; //namespace
//----------------------------------------------------------------------------------
#endif

