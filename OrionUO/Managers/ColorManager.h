/***********************************************************************************
**
** ColorManager.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef COLORMANAGER_H
#define COLORMANAGER_H
//----------------------------------------------------------------------------------
//!Структура цветов для отправки в шейдер
struct FLOAT_HUES
{
	//!Палитра цветов, 32 штуки по 3 цвета
	float Palette[32 * 3];
};
//----------------------------------------------------------------------------------
//!Кдасс менеджера цветов
class CColorManager
{
private:
	//!Указатель на палитру
	vector<HUES_GROUP> m_HuesRange;

	//!Указатель на список цветов для шейдера
	vector<FLOAT_HUES> m_HuesFloat;

	//!Количество цветов
	int m_HuesCount{ 0 };

	//!Данные для радара
	USHORT_LIST m_Radarcol;

	//!Данные из palette.mul
	PALETTE_BLOCK m_Palette[256];

public:
	CColorManager();
	~CColorManager();

	void Init();

	/*!
	Получить указатель на начало цветов
	@return Ссылка на цвета
	*/
	PHUES_GROUP GetHuesRangePointer() { return &m_HuesRange[0]; }

	/*!
	Получить количество цветов
	@return Количество цветов
	*/
	int GetHuesCount() const { return m_HuesCount; }

	/*!
	Патч блока цветов из вердаты
	@param [__in] index Индекс в списке
	@param [__in] group Указатель на цвета патча
	@return 
	*/
	void SetHuesBlock(int index, PVERDATA_HUES_GROUP group);

	/*!
	Создание палитры цветов для шейдера
	@return 
	*/
	void CreateHuesPalette();

	/*!
	Отправка цветов в шейдер
	@param [__in] color Индекс цвета
	@return 
	*/
	void SendColorsToShader(ushort color);



	/*!
	Конвертирование цвета из 32 бит в 16 бит
	@param [__in] c 32-битный цвет
	@return 16-битный цвет
	*/
	ushort Color32To16(const uint &c);

	/*!
	Конвертирование цвета из 16 бит в 32 бит
	@param [__in] c 16-битный цвет
	@return 32-битный цвет
	*/
	uint Color16To32(const ushort &c);

	/*!
	Перевод в серый
	@param [__in] c 16-битный цвет
	@return 16-битный цвет
	*/
	ushort ConvertToGray(const ushort &c);



	/*!
	Получить 16-битный цвет
	@param [__in] c Исходный цвет
	@param [__in] color Индекс цвета в палитре
	@return 16-битный цвет
	*/
	ushort GetColor16(const ushort &c, ushort color);

	/*!
	Получить 16-битный цвет для радара
	@param [__in] c Исходный 16-битный цвет
	@return 16-битный цвет
	*/
	ushort GetRadarColorData(const uint &c);

	/*!
	Получить 32-битный цвет без конвертирования входящего цвета
	@param [__in] c Исходный 16-битный цвет
	@param [__in] color Индекс цвета в палитре
	@return 32-битный цвет
	*/
	uint GetPolygoneColor(ushort c, ushort color);

	/*!
	Получить 32-битный цвет для Unicode-шрифтов
	@param [__in] c Исходный 16-битный цвет
	@param [__in] color Индекс цвета в палитре
	@return 32-битный цвет
	*/
	uint GetUnicodeFontColor(ushort &c, ushort color);

	/*!
	Получить 32-битный цвет
	@param [__in] c Исходный 16-битный цвет
	@param [__in] color Индекс цвета в палитре
	@return 32-битный цвет
	*/
	uint GetColor(ushort &c, ushort color);

	/*!
	Получить 32-битный цвет с учетом оттенков серого
	@param [__in] c Исходный 16-битный цвет
	@param [__in] color Индекс цвета в палитре
	@return 32-битный цвет
	*/
	uint GetPartialHueColor(ushort &c, ushort color);

	ushort FixColor(const ushort &color, const ushort &defaultColor = 0);
 };
 //----------------------------------------------------------------------------------
//!Ссылка на менеджер цветов
 extern CColorManager g_ColorManager;
 //----------------------------------------------------------------------------------
#endif
