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
#include "stdafx.h"
//---------------------------------------------------------------------------
TTextFileParser::TTextFileParser(const char *fileName, const char *delimiters, const char *comentaries, const char *quotes)
: m_Ptr(NULL), m_EOL(NULL)
{
	memset(&m_File, 0, sizeof(m_File));

	//Загружаем файл в память
	FileManager.LoadFileToMemory(m_File, fileName);

	//Инициализация разделителей
	memset(&m_Delimiters[0], 0, sizeof(m_Delimiters));
	m_DelimitersSize = strlen(delimiters);

	if (m_DelimitersSize)
		memcpy(&m_Delimiters[0], &delimiters[0], m_DelimitersSize);

	//Инициализация комментариев
	memset(&m_Comentaries[0], 0, sizeof(m_Comentaries));
	m_ComentariesSize = strlen(comentaries);

	if (m_ComentariesSize)
		memcpy(&m_Comentaries[0], &comentaries[0], m_ComentariesSize);

	//Инициализация кавычек
	memset(&m_Quotes[0], 0, sizeof(m_Quotes));
	m_QuotesSize = strlen(quotes);

	if (m_QuotesSize)
		memcpy(&m_Quotes[0], &quotes[0], m_QuotesSize);

	//Вычисляем конец файла
	m_End = ((PBYTE)m_File.Address + m_File.Size);

	if (m_File.Address == NULL)
		memset(&m_File, 0, sizeof(m_File));
}
//---------------------------------------------------------------------------
TTextFileParser::~TTextFileParser()
{
	//Выгружаем файл из памяти
	FileManager.UnloadFileFromMemory(m_File);
}
//---------------------------------------------------------------------------
//Проверка на конец файла
bool TTextFileParser::IsEOF()
{
	return (m_File.Ptr >= m_End);
}
//---------------------------------------------------------------------------
//Получить конец строки
void TTextFileParser::GetEOL()
{
	//Конец строки равен текущему адресу
	m_EOL = m_File.Ptr;

	//Если файл прочитан не до конца
	if (!IsEOF())
	{
		//Ищем конец строки
		while (m_EOL < m_End && *m_EOL)
		{
			if (*m_EOL == '\n')
				break;

			m_EOL++;
		}
	}
}
//---------------------------------------------------------------------------
//Проверка на разделитель
bool TTextFileParser::IsDelimiter()
{
	bool result = false;

	//Проход по всем разделителям
	for (int i = 0; i < m_DelimitersSize&& !result; i++)
		result = (*m_Ptr == m_Delimiters[i]);

	return result;
}
//---------------------------------------------------------------------------
//Пропустить все до данных
void TTextFileParser::SkipToData()
{
	//Если символ - разделитель, то проход по всем разделителям и смещение указателя
	while (m_Ptr < m_EOL && *m_Ptr && IsDelimiter())
		m_Ptr++;
}
//---------------------------------------------------------------------------
//Проверка на комментарий
bool TTextFileParser::IsComment()
{
	bool result = (*m_Ptr == '\n');

	//Проход по всем комментариям
	for (int i = 0; i < m_ComentariesSize && !result; i++)
	{
		result = (*m_Ptr == m_Comentaries[i]);

		if (result && i + 1 < m_ComentariesSize && m_Comentaries[i] == m_Comentaries[i + 1] && m_Ptr + 1 < m_EOL)
		{
			result = (m_Ptr[0] == m_Ptr[1]);
			i++;
		}
	}

	return result;
}
//---------------------------------------------------------------------------
//Получить следующий токен
string TTextFileParser::ObtainData()
{
	bool exit = false;
	string result = "";

	//Пока разделитель валиден - записываем данные и смещаем указатель
	while (m_Ptr < m_End && *m_Ptr && *m_Ptr != '\n' && !exit)
	{
		//Проверка на разделитель
		if (IsDelimiter())
		{
			exit = true;
			break;
		}
		//Проверка на комментарий
		else if (IsComment())
		{
			exit = true;
			m_Ptr = m_EOL;
			break;
		}

		//Если это валидный символ
		if (!exit)
		{
			//Проверяем на перенос строки, при необходимости обрезаем пробелы
			if (*m_Ptr != '\r' && (!m_Trim || (*m_Ptr != ' ' && *m_Ptr != '\t')))
				result.push_back(*m_Ptr);

			m_Ptr++;
		}
	}

	return result;
}
//---------------------------------------------------------------------------
//Получить следующий фрагмент строки или токен (если кавычки не обнаружены)
string TTextFileParser::ObtainQuotedData()
{
	bool exit = false;
	string result = "";

	//Пройдемся по кавычкам, формат кавычек в наборе: openQuote, closeQuote (могут быть одинаковыми)
	for (int i = 0; i < m_QuotesSize; i += 2)
	{
		//Если кавычка нашлась
		if (*m_Ptr == m_Quotes[i])
		{
			//Запомним закрывающую кавычку
			char endQuote = m_Quotes[i + 1];
			exit = true;

			//Смещаем указатель, т.к. кавычку писать не нужно
			m_Ptr++;
			PBYTE ptr = m_Ptr;

			//Пропустим все до конца строки или до закрывающей кавычки
			while (ptr < m_EOL && *ptr && *ptr != '\n' && *ptr != endQuote)
				ptr++;

			//Размер фрагмента
			int size = ptr - m_Ptr;

			if (size > 0)
			{
				//Выделяем память под фрагмент
				result.resize(size + 1);

				//Копируем фрагмент
				memcpy(&result[0], &m_Ptr[0], size);
				result[size] = 0;

				//Пройдемся по фрагменту с конца и затрем лишнее
				for (int j = size - 1; j >= 0 && result[j] == '\r'; j--)
					result[j] = 0;

				//Указатель на конец фрагмена
				m_Ptr = ptr;

				if (m_Ptr < m_EOL && *m_Ptr == endQuote)
					m_Ptr++;
			}

			break;
		}
	}

	//Если это не кавычки - вычленим слово
	if (!exit)
		result = ObtainData();

	return result;
}
//---------------------------------------------------------------------------
//Прочитать токены из файла
vector<string> TTextFileParser::ReadTokens(bool trim)
{
	m_Trim = trim;
	vector<string> result;

	//Если не достигли конца файла
	if (!IsEOF())
	{
		//Инициализация указателей
		m_Ptr = m_File.Ptr;
		GetEOL();

		//Проход до конца строки
		while (m_Ptr < m_EOL)
		{
			//Пропустить разделители
			SkipToData();

			//Если это комментарий - выходим
			if (IsComment())
				break;

			//Получаем токен
			string buf = ObtainQuotedData();

			//Если токен не пуст - запишем его в стек
			if (buf.length())
				result.push_back(buf);
		}

		m_File.Ptr = m_EOL + 1;
	}

	return result;
}
//---------------------------------------------------------------------------
//Прочитать токены из строки
vector<string> TTextFileParser::GetTokens(const char *str, bool trim)
{
	m_Trim = trim;
	vector<string> result;

	//Сохраним предыдущее значения конца данных
	PBYTE oldEnd = m_End;

	m_Ptr = (PBYTE)str;
	m_End = (PBYTE)str + strlen(str);
	m_EOL = m_End;

	//Пройдемся по строке
	while (m_Ptr < m_EOL)
	{
		//Пропустить разделители
		SkipToData();

		//Если это комментарий - выходим
		if (IsComment())
			break;

		//Получаем токен
		string buf = ObtainQuotedData();

		//Если токен не пуст - запишем его в стек
		if (buf.length())
			result.push_back(buf);
	}

	//Восстановим предыдущее значения конца данных
	m_End = oldEnd;

	return result;
}
//---------------------------------------------------------------------------