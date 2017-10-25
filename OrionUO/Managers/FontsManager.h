/***********************************************************************************
**
** FontsManager.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef FONTSMANAGER_H
#define FONTSMANAGER_H
//----------------------------------------------------------------------------------
typedef struct MULTILINES_FONT_DATA
{
	wchar_t item;
	ushort flags;
	uchar font;
	ushort linkID;
	uint color;

	MULTILINES_FONT_DATA *Next;
} *PMULTILINES_FONT_DATA;
//----------------------------------------------------------------------------------
typedef struct MULTILINES_FONT_INFO
{
	int Width;
	int IndentionOffset;
	int MaxHeight;
	int CharStart;
	int CharCount;
	TEXT_ALIGN_TYPE Align;
	vector<MULTILINES_FONT_DATA> Data;

	MULTILINES_FONT_INFO *m_Next;

	void Reset()
	{
		Width = 0;
		IndentionOffset = 0;
		MaxHeight = 0;
		CharStart = 0;
		CharCount = 0;
		Align = TS_LEFT;
		m_Next = NULL;
	}
} *PMULTILINES_FONT_INFO;
//----------------------------------------------------------------------------------
//!Структура с данными о ссылке
struct WEB_LINK
{
	//!Была посещена
	bool Visited;

	//!Ссылка
	string WebLink;
};
//----------------------------------------------------------------------------------
//!Структура с данными о HTML-символе
struct HTML_char
{
	//!Индекс символа
	wchar_t Char;

	//!Индекс шрифта
	uchar Font;

	//!Выравнивание
	TEXT_ALIGN_TYPE Align;

	//!Набор флагов
	ushort Flags;

	//!Цвет символа
	uint Color;

	//!Индекс ссылки
	ushort LinkID;
};
//----------------------------------------------------------------------------------
struct HTML_DATA_INFO
{
	HTML_TAG_TYPE Tag;
	TEXT_ALIGN_TYPE Align;
	ushort Flags;
	uchar Font;
	uint Color;
	ushort Link;
};
//----------------------------------------------------------------------------------
typedef map<ushort, WEB_LINK> WEBLINK_MAP;
typedef vector<HTML_char> HTMLCHAR_LIST;
typedef vector<HTML_DATA_INFO> HTMLINFO_LIST;
//----------------------------------------------------------------------------------
//!Класс менеджера шрифтов
class CFontsManager
{
	SETGET(bool, RecalculateWidthByInfo, false);
	SETGET(bool, UnusePartialHue, false);

	//!Количество ASCII ширфтов
	SETGET(int, FontCount, 0);

private:
	//!Список ASCII шрифтов
	FONT_DATA *m_Font{ NULL };

	//!Список ссылок
	WEBLINK_MAP m_WebLink;

	//!Таблица ассоциации ASCII шрифтов
	static const uchar m_FontIndex[256];

	//!Данные о юникод шрифтах: адрес, размер
	uint m_UnicodeFontAddress[20];
	uint m_UnicodeFontSize[20];

	//!Использование HTML расширений
	bool m_UseHTML{ false };

	//!Цвет HTML
	uint m_HTMLColor{ 0xFFFFFFFF };

	//!Возможность раскраски фона текста
	bool m_HTMLBackgroundCanBeColored{ false };

	//!Цвет фона текста
	uint m_BackgroundColor{ 0 };

	uint m_WebLinkColor{ 0 };
	uint m_VisitedWebLinkColor{ 0 };

	int m_LeftMargin{ 0 };
	int m_TopMargin{ 0 };
	int m_RightMargin{ 0 };
	int m_BottomMargin{ 0 };

	/*!
	Получить индекс ссылки
	@param [__in] link Ссылка
	@param [__out] color Цвет ссылки
	@return Индекс ссылки
	*/
	ushort GetWebLinkID(const string &link, uint &color);

	ushort GetWebLinkID(const wstring &link, uint &color);

	/*!
	Получение HTML данных
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in] len Длина текста
	@param [__in] align Расположение текста
	@param [__in] flags Эффекты текста
	@return Массив HTML символов
	*/
	HTMLCHAR_LIST GetHTMLData(uchar font, const wchar_t *str, int &len, TEXT_ALIGN_TYPE align, ushort flags);

	HTML_DATA_INFO GetHTMLInfoFromTag(const HTML_TAG_TYPE &tag);

	HTML_DATA_INFO GetCurrentHTMLInfo(const HTMLINFO_LIST &list);

	void GetHTMLInfoFromContent(HTML_DATA_INFO &info, const string &content);

	void TrimHTMLString(string &str);

	uint GetHTMLColorFromText(string &str);

	HTML_TAG_TYPE ParseHTMLTag(const wchar_t *str, const int &len, int &i, bool &endTag, HTML_DATA_INFO &info);

	HTMLCHAR_LIST GetHTMLDataOld(uchar font, const wchar_t *str, int &len, TEXT_ALIGN_TYPE align, ushort flags);

	/*!
	Получение данных многострочного текста HTML
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in] len Длина текста
	@param [__in] align Расположение текста
	@param [__in] flags Эффекты текста
	@param [__in] width Ширина текста
	@return Ссылка на данные
	*/
	PMULTILINES_FONT_INFO GetInfoHTML(uchar font, const wchar_t *str, int len, TEXT_ALIGN_TYPE align, ushort flags, int width);

	/*!
	Создание ASCII текстуры
	@param [__in] font Шрифт
	@param [__out] th Данные текстуры
	@param [__in] str Текст
	@param [__in] color Цвет
	@param [__in] width Ширина текстуры
	@param [__in] align Расположение текста
	@param [__in] flags Эффекты текста
	@return true при успешной генерации
	*/
	bool GenerateABase(uchar &font, CGLTextTexture &th, const char *str, ushort &color, int &width, TEXT_ALIGN_TYPE &align, ushort &flags);

	/*!
	Создание Unicode текстуры
	@param [__in] font Шрифт
	@param [__out] th Данные текстуры
	@param [__in] str Текст
	@param [__in] color Цвет
	@param [__in] cell Номер ячейки в палитре цветов
	@param [__in] width Ширина текстуры
	@param [__in] align Расположение текста
	@param [__in] flags Эффекты текста
	@return true при успешной генерации
	*/
	bool GenerateWBase(const uchar &font, CGLTextTexture &th, const wstring &str, const ushort &color, const uchar &cell, const int &width, const TEXT_ALIGN_TYPE &align, const ushort &flags);

public:
	CFontsManager();
	~CFontsManager();

	/*!
	Установить использование HTML-тэгов
	@param [__in] val Состояние использования
	@param [__in_opt] htmlStartColor Начальный цвет
	@return
	*/
	void SetUseHTML(const bool &val, const uint &htmlStartColor = 0xFFFFFFFF, const bool backgroundCanBeColored = false) { m_UseHTML = val; m_HTMLColor = htmlStartColor; m_HTMLBackgroundCanBeColored = backgroundCanBeColored; }
	bool GetUseHTML() const { return m_UseHTML; }
	__declspec(property(get = GetUseHTML)) bool UseHTML;

	/*!
	Загрузка ASCII шрифтов
	@return true при успешной загрузке
	*/
	bool LoadFonts();

	uchar UnicodeFontExists(const uchar &font);

	/*!
	Переход по ссылке по индексу
	@param [__in] link Индекс ссылки
	@return 
	*/
	void GoToWebLink(ushort link);



	//-----------------------------
	//---Работа с ASCII шрифтами---
	//-----------------------------

	/*!
	Проверка, печатаемый ли символ
	@param [__in] index Индекс символа
	@return true если это печатаемый символ
	*/
	bool IsPrintASCII(uchar index) { return (m_FontIndex[index] != 0xFF); }

	/*!
	Получить смещение символа ширифта
	@param [__in] font Шрифт
	@param [__in] index Индекс символа
	@return Смещение в пикселях
	*/
	int GetFontOffsetY(uchar font, uchar index);

	/*!
	Получить позицию каретки в тексте
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in] pos Текущая позиция в тексте
	@param [__in] width Ширина текстуры
	@param [__in] align Расположение текста
	@param [__in] flags Эффекты текста
	@return Координаты каретки
	*/
	WISP_GEOMETRY::CPoint2Di GetCaretPosA(uchar font, const char *str, int pos, int width, TEXT_ALIGN_TYPE align, ushort flags);

	/*!
	Вычислить положение каретки
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in] x Координата каретки по оси X
	@param [__in] y Координата каретки по оси Y
	@param [__in] width Ширина текстуры
	@param [__in] align Расположение текста
	@param [__in] flags Эффекты текста
	@return Позиция каретки в строке
	*/
	int CalculateCaretPosA(uchar font, const char *str, int x, int y, int width, TEXT_ALIGN_TYPE align, ushort flags);

	/*!
	Получить ширину текста
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in_opt] len Длина текста
	@return Ширина текста в пикселях
	*/
	int GetWidthA(uchar font, const char *str, int len = 0);

	/*!
	Получить ширину текста (с учетом параметров отрисовки)
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in] len Длина текста
	@param [__in] maxWidth Максимальная ширина текстуры
	@param [__in] align Расположение текста
	@param [__in] flags Эффекты текста
	@return Ширина текста в пикселях
	*/
	int GetWidthExA(uchar font, const char *str, int len, int maxWidth, TEXT_ALIGN_TYPE align, ushort flags);


	/*!
	Получить высоту текста
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in_opt] width Ширина текстуры
	@param [__in_opt] align Расположение текста
	@param [__in_opt] flags Эффекты текста
	@return Высота текста в пикселях
	*/
	int GetHeightA(uchar font, const char *str, int width = 0, TEXT_ALIGN_TYPE align = TS_LEFT, ushort flags = 0);

	/*!
	Получить высоту текста по списку строк
	@param [__in] info Ссылка на мультистрочный текст
	@return Высота текста в пикселях
	*/
	int GetHeightA(PMULTILINES_FONT_INFO info);

	/*!
	Получить текст указанной ширины
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in] len Длина текста
	@param [__in] width Ширина текстуры
	@param [__in] IsCropped Ограниченный текст, вышедшая за доступные пределы часть обрезается и заменяется на многоточие
	@return Результирующий текст
	*/
	string GetTextByWidthA(uchar font, const char *str, int len, int width, bool IsCropped);

	/*!
	Получить информацию о тексте (в мультистрочном виде)
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in] len Длина текста
	@param [__in] align Расположение текста
	@param [__in] flags Эффекты текста
	@param [__in] width Ширина текстуры
	@return Ссылка на мультистрочный текст или NULL
	*/
	PMULTILINES_FONT_INFO GetInfoA(uchar font, const char *str, int len, TEXT_ALIGN_TYPE align, ushort flags, int width);

	/*!
	Сгенерировать пиксели текстуры текста
	@param [__in] font Шрифт
	@param [__inout] th Данные о текстуре текста
	@param [__in] str Текст
	@param [__in] color Цвет текста
	@param [__in] width Ширина текстуры
	@param [__in] align Расположение текста
	@param [__in] flags Эффекты текста
	@return Ссылка на массив пикселей
	*/
	UINT_LIST GeneratePixelsA(uchar &font, CGLTextTexture &th, const char *str, ushort &color, int &width, TEXT_ALIGN_TYPE &align, ushort &flags);

	/*!
	Сгенерировать текстуру текста
	@param [__in] font Шрифт
	@param [__inout] th Данные о текстуре текста
	@param [__in] str Текст
	@param [__in_opt] color Цвет
	@param [__in_opt] width Ширина текстуры
	@param [__in_opt] align Расположение текста
	@param [__in_opt] flags Эффекты текста
	@return true при успешной генерации
	*/
	bool GenerateA(uchar font, CGLTextTexture &th, const char *str, ushort color = 0, int width = 0, TEXT_ALIGN_TYPE align = TS_LEFT, ushort flags = 0);

	/*!
	Отрисовать текст
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in] color Цвет
	@param [__in] x Экранная координата X
	@param [__in] y Экранная координата Y
	@param [__in_opt] width Ширина текстуры
	@param [__in_opt] align Расположение текста
	@param [__in_opt] flags Эффекты текста
	@return 
	*/
	void DrawA(uchar font, const char *str, ushort color, int x, int y, int width = 0, TEXT_ALIGN_TYPE align = TS_LEFT, ushort flags = 0);



	//-------------------------------
	//---Работа с Unicode шрифтами---
	//-------------------------------

	/*!
	Получить позицию каретки в тексте
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in] pos Текущая позиция в тексте
	@param [__in] width Ширина текстуры
	@param [__in] align Расположение текста
	@param [__in] flags Эффекты текста
	@return Координаты каретки
	*/
	WISP_GEOMETRY::CPoint2Di GetCaretPosW(const uchar &font, const wstring &str, int pos, int width, const TEXT_ALIGN_TYPE &align, const ushort &flags);

	/*!
	Вычислить положение каретки
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in] x Координата каретки по оси X
	@param [__in] y Координата каретки по оси Y
	@param [__in] width Ширина текстуры
	@param [__in] align Расположение текста
	@param [__in] flags Эффекты текста
	@return Позиция каретки в строке
	*/
	int CalculateCaretPosW(const uchar &font, const wstring &str, const int &x, const int &y, int width, const TEXT_ALIGN_TYPE &align, const ushort &flags);

	/*!
	Получить ширину текста
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in_opt] len Длина текста
	@return Ширина текста в пикселях
	*/
	int GetWidthW(const uchar &font, const wstring &str);

	/*!
	Получить ширину текста (с учетом параметров отрисовки)
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in] len Длина текста
	@param [__in] maxWidth Максимальная ширина текстуры
	@param [__in] align Расположение текста
	@param [__in] flags Эффекты текста
	@return Ширина текста в пикселях
	*/
	int GetWidthExW(const uchar &font, const wstring &str, const int &maxWidth, const TEXT_ALIGN_TYPE &align, const ushort &flags);

	/*!
	Получить высоту текста
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in_opt] width Ширина текстуры
	@param [__in_opt] align Расположение текста
	@param [__in_opt] flags Эффекты текста
	@return Высота текста в пикселях
	*/
	int GetHeightW(const uchar &font, const wstring &str, int width = 0, const TEXT_ALIGN_TYPE &align = TS_LEFT, const ushort &flags = 0);

	/*!
	Получить высоту текста по списку строк
	@param [__in] info Ссылка на мультистрочный текст
	@return Высота текста в пикселях
	*/
	int GetHeightW(PMULTILINES_FONT_INFO info);

	/*!
	Получить текст указанной ширины
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in] len Длина текста
	@param [__in] width Ширина текстуры
	@param [__in] IsCropped Ограниченный текст, вышедшая за доступные пределы часть обрезается и заменяется на многоточие
	@return Результирующий текст
	*/
	wstring GetTextByWidthW(const uchar &font, const wstring &str, int width, const bool &isCropped);

	/*!
	Получить информацию о тексте (в мультистрочном виде)
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in] len Длина текста
	@param [__in] align Расположение текста
	@param [__in] flags Эффекты текста
	@param [__in] width Ширина текстуры
	@return Ссылка на мультистрочный текст или NULL
	*/
	PMULTILINES_FONT_INFO GetInfoW(uchar font, const wchar_t *str, int len, TEXT_ALIGN_TYPE align, ushort flags, int width);

	/*!
	Сгенерировать пиксели текстуры текста
	@param [__in] font Шрифт
	@param [__inout] th Данные о текстуре текста
	@param [__in] str Текст
	@param [__in] color Цвет текста
	@param [__in] cell Ячейка в палитре цветов
	@param [__in] width Ширина текстуры
	@param [__in] align Расположение текста
	@param [__in] flags Эффекты текста
	@return Ссылка на массив пикселей
	*/
	UINT_LIST GeneratePixelsW(const uchar &font, CGLTextTexture &th, const wchar_t *str, const ushort &color, const uchar &cell, int width, const TEXT_ALIGN_TYPE &align, const ushort &flags);

	/*!
	Сгенерировать текстуру текста
	@param [__in] font Шрифт
	@param [__inout] th Данные о текстуре текста
	@param [__in] str Текст
	@param [__in_opt] color Цвет
	@param [__in_opt] cell Ячейка в палитре цветов
	@param [__in_opt] width Ширина текстуры
	@param [__in_opt] align Расположение текста
	@param [__in_opt] flags Эффекты текста
	@return true при успешной генерации
	*/
	bool GenerateW(const uchar &font, CGLTextTexture &th, const wstring &str, const ushort &color = 0, const uchar &cell = 30, const int &width = 0, const TEXT_ALIGN_TYPE &align = TS_LEFT, const ushort &flags = 0);

	/*!
	Отрисовать текст
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in] color Цвет
	@param [__in] x Экранная координата X
	@param [__in] y Экранная координата Y
	@param [__in_opt] cell Ячейка в палитре цветов
	@param [__in_opt] width Ширина текстуры
	@param [__in_opt] align Расположение текста
	@param [__in_opt] flags Эффекты текста
	@return
	*/
	void DrawW(uchar font, const wchar_t *str, ushort color, int x, int y, uchar cell = 30, int width = 0, TEXT_ALIGN_TYPE align = TS_LEFT, ushort flags = 0);
};
//--------------------------------------------------------------------------
//!Ссылка на менеджер шрифтов
extern CFontsManager g_FontManager;
//--------------------------------------------------------------------------
#endif