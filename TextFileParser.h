/****************************************************************************
**
** TextFileParser.h
**
** Copyright (C) Marth 2016 Hotride
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
*****************************************************************************
*/
//---------------------------------------------------------------------------
#ifndef TextFileParserH
#define TextFileParserH
//---------------------------------------------------------------------------
//Класс для работы с текстовыми файлами конфига
class TTextFileParser
{
private:
	//Указатель на адрес в памяти, куда загружен файл
	TMappedHeader m_File;

	//Указатель на текущий обрабатываемый символ
	PBYTE m_Ptr;

	//Адрес конца строка
	PBYTE m_EOL;

	//Адрес конца файла
	PBYTE m_End;

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

	//Получить следующий токен
	string ObtainData();

	//Получить следующий фрагмент строки или токен (если кавычки не обнаружены)
	string ObtainQuotedData();
public:
	TTextFileParser(const char *fileName, const char *delimiters, const char *comentaries, const char *quotes);
	~TTextFileParser();

	//Проверка на конец файла
	bool IsEOF();

	//Прочитать токены из файла
	vector<string> ReadTokens(bool trim = true);

	//Прочитать токены из строки
	vector<string> GetTokens(const char *str, bool trim = true);
};
//---------------------------------------------------------------------------
#endif