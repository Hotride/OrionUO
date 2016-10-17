//----------------------------------------------------------------------------------
#include "WispTextFileParser.h"

namespace WISP_FILE
{
//----------------------------------------------------------------------------------
CTextFileParser::CTextFileParser(const string &path, const char *delimiters, const char *comentaries, const char *quotes)
: m_Ptr(NULL), m_EOL(NULL), m_RawLine("")
{
	m_File.Load(path);
	
	StartupInitalize(delimiters, comentaries, quotes);
}
//----------------------------------------------------------------------------------
CTextFileParser::CTextFileParser(const wstring &path, const char *delimiters, const char *comentaries, const char *quotes)
: m_Ptr(NULL), m_EOL(NULL), m_RawLine("")
{
	m_File.Load(path);
	
	StartupInitalize(delimiters, comentaries, quotes);
}
//----------------------------------------------------------------------------------
CTextFileParser::~CTextFileParser()
{
	//Выгружаем файл из памяти
	m_File.Unload();
}
//----------------------------------------------------------------------------------
//Проверка на конец файла
void CTextFileParser::StartupInitalize(const char *delimiters, const char *comentaries, const char *quotes)
{
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
	m_End = m_File.End;
}
//----------------------------------------------------------------------------------
//Проверка на конец файла
void CTextFileParser::Restart()
{
	m_File.ResetPtr();
}
//----------------------------------------------------------------------------------
//Проверка на конец файла
bool CTextFileParser::IsEOF()
{
	return (m_File.Ptr >= m_End);
}
//----------------------------------------------------------------------------------
//Получить конец строки
void CTextFileParser::GetEOL()
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
//----------------------------------------------------------------------------------
//Проверка на разделитель
bool CTextFileParser::IsDelimiter()
{
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
	//Если символ - разделитель, то проход по всем разделителям и смещение указателя
	while (m_Ptr < m_EOL && *m_Ptr && IsDelimiter())
		m_Ptr++;
}
//----------------------------------------------------------------------------------
//Проверка на комментарий
bool CTextFileParser::IsComment()
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
//----------------------------------------------------------------------------------
//Получить следующий токен
string CTextFileParser::ObtainData()
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
//----------------------------------------------------------------------------------
//Получить следующий фрагмент строки или токен (если кавычки не обнаружены)
string CTextFileParser::ObtainQuotedData()
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
			puchar ptr = m_Ptr;

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
//----------------------------------------------------------------------------------
//Прочитать токены из файла
void CTextFileParser::SaveRawLine()
{
	int size = m_EOL - m_Ptr;

	if (size > 0)
	{
		m_RawLine.resize(size, 0);
		memcpy(&m_RawLine[0], &m_Ptr[0], size);

		while (m_RawLine.length() && (m_RawLine[size - 1] == '\r' || m_RawLine[size - 1] == '\n'))
			m_RawLine.resize(m_RawLine.length() - 1);
	}
	else
		m_RawLine = "";
}
//----------------------------------------------------------------------------------
//Прочитать токены из файла
STRING_LIST CTextFileParser::ReadTokens(bool trim)
{
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
		}

		m_File.Ptr = m_EOL + 1;
	}

	return result;
}
//----------------------------------------------------------------------------------
//Прочитать токены из строки
STRING_LIST CTextFileParser::GetTokens(const char *str, bool trim)
{
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
}; //namespace
//----------------------------------------------------------------------------------
