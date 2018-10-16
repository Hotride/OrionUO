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
		//Указатель на адрес в памяти, куда загружен файл // Pointer to the address where the file is loaded
		CMappedFile m_File;

		//Указатель на текущий обрабатываемый символ // Pointer to the character being processed
		puchar m_Ptr{ 0 };

		//Адрес конца строка // End of line address
		puchar m_EOL{ 0 };

		//Адрес конца файла // End of file address
		puchar m_End{ 0 };

		//Разделители	// Delimiters
		char m_Delimiters[50];

		//Количество разделителей // Number of delimiters
		int m_DelimitersSize{ 0 };

		//Комментарии // Comments
		char m_Comentaries[50];

		//Количество коментариев // Comments size
		int m_ComentariesSize{ 0 };

		//Набор кавычек, формат кавычек в наборе: openQuote, closeQuote (могут быть одинаковыми) // Set of quotes, format of quotes in the set: openQuote, closeQuote (may be the same)
		char m_Quotes[50];

		//Количество элементов в наборе кавычек // Number of elements in the quotes set
		int m_QuotesSize{ 0 };

		//Обрезать строки на выходе // Trim output lines
		bool m_Trim{ false };

		//Получить конец строки //Get end of line
		void GetEOL();

		//Проверка на разделитель //Check for delimiter
		bool IsDelimiter();

		//Пропустить все до данных //Skip all to data
		void SkipToData();

		//Проверка на комментарий //Check for comment
		bool IsComment();

		//Проверка на кавычку // Check for quotes
		bool IsQuote();

		//Проверка на закрывающую кавычку // Check for closing quotation
		bool IsSecondQuote();

		//Получить следующий токен //Get the next token
		string ObtainData();

		//Получить следующий фрагмент строки или токен (если кавычки не обнаружены) // Get the next fragment of a string or token (if no quotes are found)
		string ObtainQuotedData();

		void StartupInitalize(const char *delimiters, const char *comentaries, const char *quotes);

		void SaveRawLine();

	public:
		CTextFileParser(
			const os_path &path,
			const char *delimiters = "",
			const char *comentaries = "",
			const char *quotes = "");
		~CTextFileParser();

		//Сбросить указатель на старт //Restart pointer
		void Restart();

		//Проверка на конец файла //Checks for end of file
		bool IsEOF();

		//Прочитать токены из файла //Reads token from file
		STRING_LIST ReadTokens(bool trim = true);

		//Прочитать токены из строки //Read tokens from the string
		STRING_LIST GetTokens(const char *str, bool trim = true);
	};
	//----------------------------------------------------------------------------------
	class CTextFileWritter
	{
	private:
		//Указатель на адрес в памяти, куда загружен файл //Pointer to the address in memory where the file is loaded
		FILE * m_File{ nullptr };

	public:
		CTextFileWritter(const os_path &path);
		~CTextFileWritter();

		bool Opened() { return (m_File != nullptr); };

		void Close();

		void WriteString(const string &key, const string &value);

		void WriteInt(const string &key, int value);

		void WriteBool(const string &key, bool value);
	};
	//----------------------------------------------------------------------------------
}; // namespace WISP_FILE
   //----------------------------------------------------------------------------------
#endif