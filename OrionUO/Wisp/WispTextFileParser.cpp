// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//----------------------------------------------------------------------------------
#include "stdafx.h"

namespace WISP_FILE
{
//----------------------------------------------------------------------------------
CTextFileParser::CTextFileParser(const string &path, const char *delimiters, const char *comentaries, const char *quotes)
{
	WISPFUN_DEBUG("c11_f1");
	m_File.Load(path);
	
	StartupInitalize(delimiters, comentaries, quotes);
}
//----------------------------------------------------------------------------------
CTextFileParser::CTextFileParser(const wstring &path, const char *delimiters, const char *comentaries, const char *quotes)
{
	WISPFUN_DEBUG("c11_f2");
	m_File.Load(path);
	
	StartupInitalize(delimiters, comentaries, quotes);
}
//----------------------------------------------------------------------------------
CTextFileParser::~CTextFileParser()
{
	WISPFUN_DEBUG("c11_f3");
	//Выгружаем файл из памяти
	m_File.Unload();
}
//----------------------------------------------------------------------------------
//Проверка на конец файла
void CTextFileParser::StartupInitalize(const char *delimiters, const char *comentaries, const char *quotes)
{
	WISPFUN_DEBUG("c11_f4");
	//Инициализация разделителей
	memset(&m_Delimiters[0], 0, sizeof(m_Delimiters));
	m_DelimitersSize = (int)strlen(delimiters);

	if (m_DelimitersSize)
		memcpy(&m_Delimiters[0], &delimiters[0], m_DelimitersSize);

	//Инициализация комментариев
	memset(&m_Comentaries[0], 0, sizeof(m_Comentaries));
	m_ComentariesSize = (int)strlen(comentaries);

	if (m_ComentariesSize)
		memcpy(&m_Comentaries[0], &comentaries[0], m_ComentariesSize);

	//Инициализация кавычек
	memset(&m_Quotes[0], 0, sizeof(m_Quotes));
	m_QuotesSize = (int)strlen(quotes);

	if (m_QuotesSize)
		memcpy(&m_Quotes[0], &quotes[0], m_QuotesSize);

	//Вычисляем конец файла
	m_End = m_File.End;
}
//----------------------------------------------------------------------------------
//Проверка на конец файла
void CTextFileParser::Restart()
{
	WISPFUN_DEBUG("c11_f5");
	m_File.ResetPtr();
}
//----------------------------------------------------------------------------------
//Проверка на конец файла
bool CTextFileParser::IsEOF()
{
	WISPFUN_DEBUG("c11_f6");
	return (m_File.Ptr >= m_End);
}
//----------------------------------------------------------------------------------
//Получить конец строки
void CTextFileParser::GetEOL()
{
	WISPFUN_DEBUG("c11_f7");
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
//----------------------------------------------------------------------------------
//Проверка на разделитель
bool CTextFileParser::IsDelimiter()
{
	WISPFUN_DEBUG("c11_f8");
	bool result = false;

	//Проход по всем разделителям
	for (int i = 0; i < m_DelimitersSize&& !result; i++)
		result = (*m_Ptr == m_Delimiters[i]);

	return result;
}
//----------------------------------------------------------------------------------
//Пропустить все до данных
void CTextFileParser::SkipToData()
{
	WISPFUN_DEBUG("c11_f9");
	//Если символ - разделитель, то проход по всем разделителям и смещение указателя
	while (m_Ptr < m_EOL && *m_Ptr && IsDelimiter())
		m_Ptr++;
}
//----------------------------------------------------------------------------------
//Проверка на комментарий
bool CTextFileParser::IsComment()
{
	WISPFUN_DEBUG("c11_f10");
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
//----------------------------------------------------------------------------------
//Проверка на кавычку
bool CTextFileParser::IsQuote()
{
	bool result = (*m_Ptr == '\n');

	//Пройдемся по кавычкам, формат кавычек в наборе: openQuote, closeQuote (могут быть одинаковыми)
	for (int i = 0; i < m_QuotesSize; i += 2)
	{
		//Если кавычка нашлась
		if (*m_Ptr == m_Quotes[i] || *m_Ptr == m_Quotes[i + 1])
		{
			result = true;
			break;
		}
	}

	return result;
}
//----------------------------------------------------------------------------------
//Проверка на закрывающую кавычку
bool CTextFileParser::IsSecondQuote()
{
	bool result = (*m_Ptr == '\n');

	//Пройдемся по кавычкам, формат кавычек в наборе: openQuote, closeQuote (могут быть одинаковыми)
	for (int i = 0; i < m_QuotesSize; i += 2)
	{
		//Если кавычка нашлась
		if (*m_Ptr == m_Quotes[i + 1])
		{
			result = true;
			break;
		}
	}

	return result;
}
//----------------------------------------------------------------------------------
//Получить следующий токен
string CTextFileParser::ObtainData()
{
	WISPFUN_DEBUG("c11_f11");
	string result = "";

	//Пока разделитель валиден - записываем данные и смещаем указатель
	while (m_Ptr < m_End && *m_Ptr && *m_Ptr != '\n')
	{
		//Проверка на разделитель
		if (IsDelimiter())
			break;
		//Проверка на комментарий
		else if (IsComment())
		{
			m_Ptr = m_EOL;
			break;
		}

		//Проверяем на перенос строки, при необходимости обрезаем пробелы
		if (*m_Ptr != '\r' && (!m_Trim || (*m_Ptr != ' ' && *m_Ptr != '\t')))
			result.push_back(*m_Ptr);

		m_Ptr++;
	}

	return result;
}
//----------------------------------------------------------------------------------
//Получить следующий фрагмент строки или токен (если кавычки не обнаружены)
string CTextFileParser::ObtainQuotedData()
{
	WISPFUN_DEBUG("c11_f12");
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
			puchar ptr = m_Ptr;

			//Пропустим все до конца строки или до закрывающей кавычки
			while (ptr < m_EOL && *ptr && *ptr != '\n' && *ptr != endQuote)
				ptr++;

			//Размер фрагмента
			size_t size = ptr - m_Ptr;

			if (size > 0)
			{
				//Выделяем память под фрагмент
				result.resize(size + 1);

				//Копируем фрагмент
				memcpy(&result[0], &m_Ptr[0], size);
				result[size] = 0;

				//Пройдемся по фрагменту с конца и затрем лишнее
				for (int j = (int)size - 1; j >= 0 && result[j] == '\r'; j--)
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
//----------------------------------------------------------------------------------
//Прочитать токены из файла
void CTextFileParser::SaveRawLine()
{
	WISPFUN_DEBUG("c11_f13");
	size_t size = m_EOL - m_Ptr;

	if (size > 0)
	{
		RawLine.resize(size, 0);
		memcpy(&RawLine[0], &m_Ptr[0], size);

		while (RawLine.length() && (RawLine[size - 1] == '\r' || RawLine[size - 1] == '\n'))
			RawLine.resize(RawLine.length() - 1);
	}
	else
		RawLine = "";
}
//----------------------------------------------------------------------------------
//Прочитать токены из файла
STRING_LIST CTextFileParser::ReadTokens(bool trim)
{
	WISPFUN_DEBUG("c11_f14");
	m_Trim = trim;
	STRING_LIST result;

	//Если не достигли конца файла
	if (!IsEOF())
	{
		//Инициализация указателей
		m_Ptr = m_File.Ptr;
		GetEOL();

		SaveRawLine();

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
			else if (IsSecondQuote())
				m_Ptr++;
		}

		m_File.Ptr = m_EOL + 1;
	}

	return result;
}
//----------------------------------------------------------------------------------
//Прочитать токены из строки
STRING_LIST CTextFileParser::GetTokens(const char *str, bool trim)
{
	WISPFUN_DEBUG("c11_f15");
	m_Trim = trim;
	STRING_LIST result;

	//Сохраним предыдущее значения конца данных
	puchar oldEnd = m_End;

	m_Ptr = (puchar)str;
	m_End = (puchar)str + strlen(str);
	m_EOL = m_End;

	SaveRawLine();

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
//----------------------------------------------------------------------------------
CTextFileWritter::CTextFileWritter(const string &path)
{
	fopen_s(&m_File, path.c_str(), "w");
}
//----------------------------------------------------------------------------------
CTextFileWritter::CTextFileWritter(const wstring &path)
{
	_wfopen_s(&m_File, path.c_str(), L"w");
}
//----------------------------------------------------------------------------------
CTextFileWritter::~CTextFileWritter()
{
	Close();
}
//----------------------------------------------------------------------------------
void CTextFileWritter::Close()
{
	if (m_File != NULL)
	{
		fclose(m_File);
		m_File = NULL;
	}
}
//----------------------------------------------------------------------------------
void CTextFileWritter::WriteString(const string &key, const string &value)
{
	if (m_File != NULL)
		fputs(string(key + "=" + value + "\n").c_str(), m_File);
}
//----------------------------------------------------------------------------------
void CTextFileWritter::WriteInt(const string &key, int value)
{
	if (m_File != NULL)
		fputs(string(key + "=" + std::to_string(value) + "\n").c_str(), m_File);
}
//----------------------------------------------------------------------------------
void CTextFileWritter::WriteBool(const string &key, bool value)
{
	if (m_File != NULL)
		fputs(string(key + "=" + (value ? "yes" : "no") + "\n").c_str(), m_File);
}
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
