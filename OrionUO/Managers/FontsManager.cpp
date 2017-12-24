// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** FontsManager.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CFontsManager g_FontManager;
//----------------------------------------------------------------------------------
CFontsManager::CFontsManager()
{
	WISPFUN_DEBUG("c143_f1");
	memset(m_UnicodeFontAddress, 0, sizeof(m_UnicodeFontAddress));
	memset(m_UnicodeFontSize, 0, sizeof(m_UnicodeFontSize));
}
//----------------------------------------------------------------------------------
CFontsManager::~CFontsManager()
{
	WISPFUN_DEBUG("c143_f2");
	delete []m_Font;
	m_FontCount = 0;
	m_WebLink.clear();
}
//----------------------------------------------------------------------------------
/*!
Загрузка ASCII шрифтов
@return true при успешной загрузке
*/
bool CFontsManager::LoadFonts()
{
	WISPFUN_DEBUG("c143_f3");

	WISP_FILE::CMappedFile fontFile;

	if (!fontFile.Load(g_App.UOFilesPath("fonts.mul")))
		return false;

	m_FontCount = 0;

	while (!fontFile.IsEOF())
	{
		bool exit = false;
		fontFile.Move(1);

		IFOR(i, 0, 224)
		{
			PFONT_HEADER fh = (PFONT_HEADER)fontFile.Ptr;
			fontFile.Move(sizeof(FONT_HEADER));

			int bcount = fh->Width * fh->Height * 2;

			if (fontFile.Ptr + bcount > fontFile.End) //Bad font file...
			{
				exit = true;

				break;
			}

			fontFile.Move(bcount);
		}

		if (exit)
			break;

		m_FontCount++;
	}

	if (m_FontCount < 1)
	{
		m_Font = NULL;
		m_FontCount = 0;

		return false;
	}

	m_Font = new FONT_DATA[m_FontCount];

	fontFile.ResetPtr();

	IFOR(i, 0, m_FontCount)
	{
		FONT_DATA &fd = m_Font[i];
		fd.Header = fontFile.ReadUInt8();

		IFOR(j, 0, 224)
		{
			FONT_CHARACTER_DATA &fcd = fd.Chars[j];
			fcd.Width = fontFile.ReadUInt8();
			fcd.Height = fontFile.ReadUInt8();
			fontFile.Move(1);

			int dataSize = fcd.Width * fcd.Height;
			fcd.Data.resize(dataSize);
			fontFile.ReadDataLE((puchar)&fcd.Data[0], dataSize * 2);
		}
	}

	IFOR(i, 0, 20)
	{
		m_UnicodeFontAddress[i] = (size_t)g_FileManager.m_UnifontMul[i].Start;
		m_UnicodeFontSize[i] = (uint)g_FileManager.m_UnifontMul[i].Size;
	}

	if (m_UnicodeFontAddress[1] == 0)
	{
		m_UnicodeFontAddress[1] = m_UnicodeFontAddress[0];
		m_UnicodeFontSize[1] = m_UnicodeFontSize[0];
	}

	IFOR(i, 0, 256)
	{
		if (m_FontIndex[i] >= 0xE0)
			m_FontIndex[i] = m_FontIndex[' '];
	}

	return true;
}
//----------------------------------------------------------------------------------
bool CFontsManager::UnicodeFontExists(const uchar &font)
{
	WISPFUN_DEBUG("c143_f4");
	if (font >= 20 || m_UnicodeFontAddress[font] == NULL)
		return false;

	return true;
}
//----------------------------------------------------------------------------------
/*!
Переход по ссылке по индексу
@param [__in] link Индекс ссылки
@return 
*/
void CFontsManager::GoToWebLink(ushort link)
{
	WISPFUN_DEBUG("c143_f5");
	WEBLINK_MAP::iterator it = m_WebLink.find(link);

	if (it != m_WebLink.end())
	{
		it->second.Visited = true;
		g_Orion.GoToWebLink(it->second.WebLink);
	}
}
//----------------------------------------------------------------------------------
//----------------------------------------ASCII-------------------------------------
//----------------------------------------------------------------------------------
/*!
Получить смещение символа ширифта
@param [__in] font Шрифт
@param [__in] index Индекс символа
@return Смещение в пикселях
*/
int CFontsManager::GetFontOffsetY(const uchar &font, const uchar &index)
{
	WISPFUN_DEBUG("c143_f6");

	if (index == 0xB8)
		return 1;
	else if (!(index >= 0x41 && index <= 0x5A) && !(index >= 0xC0 && index <= 0xDF) && index != 0xA8)
	{
		if (font < 10)
		{
			if (index >= 0x61 && index <= 0x7A)
			{
				const int offsetCharTable[] = { 2, 0, 2, 2, 0, 0, 2, 2, 0, 0 };
				return offsetCharTable[font];
			}
			else
			{
				const int offsetSymbolTable[] = { 1, 0, 1, 1, -1, 0, 1, 1, 0, 0 };
				return offsetSymbolTable[font];
			}
		}
		else
			return 2;
	}

	return 0;
}
//----------------------------------------------------------------------------------
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
WISP_GEOMETRY::CPoint2Di CFontsManager::GetCaretPosA(const uchar &font, const string &str, int pos, int width, const TEXT_ALIGN_TYPE &align, const ushort &flags)
{
	WISPFUN_DEBUG("c143_f7");
	WISP_GEOMETRY::CPoint2Di p;

	if (font >= m_FontCount || pos < 1 || str.empty())
		return p;

	FONT_DATA &fd = m_Font[font];

	if (!width)
		width = GetWidthA(font, str);

	PMULTILINES_FONT_INFO info = GetInfoA(font, str.c_str(), (int)str.length(), align, flags, width);
	if (info == NULL)
		return p;

	int height = 0;
	PMULTILINES_FONT_INFO ptr = info;

	while (ptr != NULL)
	{
		info = ptr;

		if (pos > 0)
		{
			int len = ptr->CharCount;

			if (len && pos - len < 1)
			{
				IFOR(i, 0, len)
				{
					const uchar &index = m_FontIndex[(uchar)ptr->Data[i].item];

					p.X += fd.Chars[index].Width;
					pos--;

					if (pos < 1)
						break;
				}
			}

			pos -= len;

			if (pos <= 1)
			{
				p.Y = height;

				if (pos == 1)
					p.Y += ptr->MaxHeight;

				break;
			}
			else
				height += ptr->MaxHeight;

			pos--;
		}

		ptr = ptr->m_Next;

		info->Data.clear();
		delete info;
	}

	return p;
}
//----------------------------------------------------------------------------------
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
int CFontsManager::CalculateCaretPosA(const uchar &font, const string &str, const int &x, const int &y, int width, const TEXT_ALIGN_TYPE &align, const ushort &flags)
{
	WISPFUN_DEBUG("c143_f8");
	if (font >= m_FontCount || x < 0 || y < 0 || str.empty())
		return 0;

	FONT_DATA &fd = m_Font[font];

	if (!width)
		width = GetWidthA(font, str);

	if (x >= width)
		return (int)str.length();

	PMULTILINES_FONT_INFO info = GetInfoA(font, str.c_str(), (int)str.length(), align, flags, width);
	if (info == NULL)
		return 0;

	int height = GetHeightA(info);

	PMULTILINES_FONT_INFO ptr = info;

	if (y >= height)
	{
		while (ptr != NULL)
		{
			info = ptr;
			ptr = ptr->m_Next;

			info->Data.clear();
			delete info;
		}

		return 0;
	}

	height = 0;

	int pos = 0;
	bool found = false;

	while (ptr != NULL)
	{
		info = ptr;

		height += ptr->MaxHeight;
		width = 0;

		if (!found)
		{
			if (y < height)
			{
				int len = ptr->CharCount;

				IFOR(i, 0, len)
				{
					const uchar &index = m_FontIndex[(uchar)ptr->Data[i].item];

					width += fd.Chars[index].Width;

					if (width > x)
						break;

					pos++;
				}

				found = true;
			}
			else
			{
				pos += ptr->CharCount;
				pos++;
			}
		}

		ptr = ptr->m_Next;

		info->Data.clear();
		delete info;
	}

	return pos;
}
//----------------------------------------------------------------------------------
/*!
Получить ширину текста
@param [__in] font Шрифт
@param [__in] str Текст
@param [__in_opt] len Длина текста
@return Ширина текста в пикселях
*/
int CFontsManager::GetWidthA(const uchar &font, const string &str)
{
	WISPFUN_DEBUG("c143_f9");
	if (font >= m_FontCount || str.empty())
		return 0;

	FONT_DATA &fd = m_Font[font];
	int textLength = 0;

	for (const char &c : str)
	{
		textLength += fd.Chars[m_FontIndex[(uchar)c]].Width;
	}

	return textLength;
}
//----------------------------------------------------------------------------------
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
int CFontsManager::GetWidthExA(const uchar &font, const string &str, const int &maxWidth, const TEXT_ALIGN_TYPE &align, const ushort &flags)
{
	WISPFUN_DEBUG("c143_f10");
	if (font >= m_FontCount || str.empty())
		return 0;

	PMULTILINES_FONT_INFO info = GetInfoA(font, str.c_str(), (int)str.length(), align, flags, maxWidth);

	int textWidth = 0;

	while (info != NULL)
	{
		if (info->Width > textWidth)
			textWidth = info->Width;

		PMULTILINES_FONT_INFO ptr = info;
		info = info->m_Next;

		ptr->Data.clear();
		delete ptr;
	}

	return textWidth;
}
//----------------------------------------------------------------------------------
/*!
Получить высоту текста
@param [__in] font Шрифт
@param [__in] str Текст
@param [__in_opt] width Ширина текстуры
@param [__in_opt] align Расположение текста
@param [__in_opt] flags Эффекты текста
@return Высота текста в пикселях
*/
int CFontsManager::GetHeightA(const uchar &font, const string &str, int width, const TEXT_ALIGN_TYPE &align, const ushort &flags)
{
	WISPFUN_DEBUG("c143_f11");
	if (font >= m_FontCount || str.empty())
		return 0;

	if (!width)
		width = GetWidthA(font, str);

	PMULTILINES_FONT_INFO info = GetInfoA(font, str.c_str(), (int)str.length(), align, flags, width);

	int textHeight = 0;

	while (info != NULL)
	{
		textHeight += info->MaxHeight;

		PMULTILINES_FONT_INFO ptr = info;
		info = info->m_Next;

		ptr->Data.clear();
		delete ptr;
	}

	return textHeight;
}
//----------------------------------------------------------------------------------
/*!
Получить высоту текста по списку строк
@param [__in] info Ссылка на мультистрочный текст
@return Высота текста в пикселях
*/
int CFontsManager::GetHeightA(PMULTILINES_FONT_INFO info)
{
	WISPFUN_DEBUG("c143_f12");
	int textHeight = 0;

	while (info != NULL)
	{
		textHeight += info->MaxHeight;

		info = info->m_Next;
	}

	return textHeight;
}
//----------------------------------------------------------------------------------
/*!
Получить текст указанной ширины
@param [__in] font Шрифт
@param [__in] str Текст
@param [__in] len Длина текста
@param [__in] width Ширина текстуры
@param [__in] IsCropped Ограниченный текст, вышедшая за доступные пределы часть обрезается и заменяется на многоточие
@return Результирующий текст
*/
string CFontsManager::GetTextByWidthA(const uchar &font, const string &str, int width, const bool &isCropped)
{
	WISPFUN_DEBUG("c143_f13");
	if (font >= m_FontCount || str.empty())
		return string("");

	FONT_DATA &fd = m_Font[font];

	if (isCropped)
	{
		width -= fd.Chars[m_FontIndex[(uchar)'.']].Width * 3;
	}

	int textLength = 0;
	string result = "";

	for (const char &c : str)
	{
		textLength += fd.Chars[m_FontIndex[(uchar)c]].Width;

		if (textLength > width)
			break;

		result += c;

	}

	if (isCropped)
		result += "...";

	return result;
}
//----------------------------------------------------------------------------------
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
PMULTILINES_FONT_INFO CFontsManager::GetInfoA(uchar font, const char *str, int len, TEXT_ALIGN_TYPE align, ushort flags, int width)
{
	WISPFUN_DEBUG("c143_f14");
	if (font >= m_FontCount)
		return NULL;

	FONT_DATA &fd = m_Font[font];

	PMULTILINES_FONT_INFO info = new MULTILINES_FONT_INFO();
	info->Reset();
	info->Align = align;

	PMULTILINES_FONT_INFO ptr = info;

	int indentionOffset = 0;
	ptr->IndentionOffset = 0;

	bool isFixed = (flags & UOFONT_FIXED);
	bool isCropped = (flags & UOFONT_CROPPED);

	int charCount = 0;
	int lastSpace = 0;
	int readWidth = 0;

	IFOR(i, 0, len)
	{
		char si = str[i];

		if (si == '\r' || si == '\n')
		{
			if (si == '\r' || isFixed || isCropped)
				continue;
			else
				si = '\n';
		}

		if (si == ' ')
		{
			lastSpace = (int)i;
			ptr->Width += readWidth;
			readWidth = 0;
			ptr->CharCount += charCount;
			charCount = 0;
		}

		FONT_CHARACTER_DATA &fcd = fd.Chars[m_FontIndex[(uchar)si]];

		if (si == '\n' || ptr->Width + readWidth + fcd.Width > width)
		{
			if (lastSpace == ptr->CharStart && !lastSpace && si != '\n')
				ptr->CharStart = 1;

			if (si == '\n')
			{
				ptr->Width += readWidth;
				ptr->CharCount += charCount;

				lastSpace = (int)i;

				if (!ptr->Width)
					ptr->Width = 1;

				if (!ptr->MaxHeight)
					ptr->MaxHeight = 14;

				ptr->Data.resize(ptr->CharCount);

				PMULTILINES_FONT_INFO newptr = new MULTILINES_FONT_INFO();
				newptr->Reset();

				ptr->m_Next = newptr;

				ptr = newptr;

				ptr->Align = align;
				ptr->CharStart = (int)i + 1;

				readWidth = 0;
				charCount = 0;
				indentionOffset = 0;

				ptr->IndentionOffset = 0;

				continue;
			}
			else if (lastSpace + 1 == ptr->CharStart && !isFixed && !isCropped)
			{
				ptr->Width += readWidth;
				ptr->CharCount += charCount;

				if (!ptr->Width)
					ptr->Width = 1;

				if (!ptr->MaxHeight)
					ptr->MaxHeight = 14;

				PMULTILINES_FONT_INFO newptr = new MULTILINES_FONT_INFO();
				newptr->Reset();

				ptr->m_Next = newptr;

				ptr = newptr;

				ptr->Align = align;
				ptr->CharStart = (int)i;
				lastSpace = (int)i - 1;
				charCount = 0;

				if (ptr->Align == TS_LEFT && (flags & UOFONT_INDENTION))
					indentionOffset = 14;

				ptr->IndentionOffset = indentionOffset;

				readWidth = indentionOffset;
			}
			else
			{
				if (isFixed)
				{
					MULTILINES_FONT_DATA mfd = {si, flags, font, 0, 0xFFFFFFFF};
					ptr->Data.push_back(mfd);

					readWidth += fcd.Width;

					if (fcd.Height > ptr->MaxHeight)
						ptr->MaxHeight = fcd.Height;

					charCount++;

					ptr->Width += readWidth;
					ptr->CharCount += charCount;
				}

				i = lastSpace + 1;
				si = str[i];

				if (!ptr->Width)
					ptr->Width = 1;

				if (!ptr->MaxHeight)
					ptr->MaxHeight = 14;

				ptr->Data.resize(ptr->CharCount);
				charCount = 0;

				if (isFixed || isCropped)
					break;

				PMULTILINES_FONT_INFO newptr = new MULTILINES_FONT_INFO();
				newptr->Reset();

				ptr->m_Next = newptr;

				ptr = newptr;

				ptr->Align = align;
				ptr->CharStart = (int)i;

				if (ptr->Align == TS_LEFT && (flags & UOFONT_INDENTION))
					indentionOffset = 14;

				ptr->IndentionOffset = indentionOffset;

				readWidth = indentionOffset;
			}
		}

		MULTILINES_FONT_DATA mfd = {si, flags, font, 0, 0xFFFFFFFF};
		ptr->Data.push_back(mfd);

		readWidth += fcd.Width;

		if (fcd.Height > ptr->MaxHeight)
			ptr->MaxHeight = fcd.Height;

		charCount++;
	}

	ptr->Width += readWidth;
	ptr->CharCount += charCount;

	if (!readWidth && len && (str[len - 1] == '\n' || str[len - 1] == '\r'))
	{
		ptr->Width = 1;
		ptr->MaxHeight = 14;
	}

	if (font == 4)
	{
		ptr = info;

		while (ptr != NULL)
		{
			if (ptr->Width > 1)
				ptr->MaxHeight = ptr->MaxHeight + 2;
			else
				ptr->MaxHeight = ptr->MaxHeight + 6;

			ptr = ptr->m_Next;
		}
	}

	return info;
}
//----------------------------------------------------------------------------------
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
bool CFontsManager::GenerateA(const uchar &font, CGLTextTexture &th, const string &str, const ushort &color, const int &width, const TEXT_ALIGN_TYPE &align, const ushort &flags)
{
	WISPFUN_DEBUG("c143_f15");
	if ((flags & UOFONT_FIXED) || (flags & UOFONT_CROPPED))
	{
		th.Clear();

		if (!width || str.empty())
			return false;

		int realWidth = GetWidthA(font, str);

		if (realWidth > width)
		{
			string newstr = GetTextByWidthA(font, str, width, (flags & UOFONT_CROPPED));
			return GenerateABase(font, th, newstr, color, width, align, flags);
		}
	}

	return GenerateABase(font, th, str, color, width, align, flags);
}
//----------------------------------------------------------------------------------
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
UINT_LIST CFontsManager::GeneratePixelsA(const uchar &font, CGLTextTexture &th, const char *str, const ushort &color, int width, const TEXT_ALIGN_TYPE &align, const ushort &flags)
{
	WISPFUN_DEBUG("c143_f16");
	UINT_LIST pData;

	th.Clear();

	if (font >= m_FontCount)
		return pData;

	int len = (int)strlen(str);
	if (!len)
		return pData;

	FONT_DATA &fd = m_Font[font];

	if (!width)
		width = GetWidthA(font, str);

	if (!width)
		return pData;

	PMULTILINES_FONT_INFO info = GetInfoA(font, str, len, align, flags, width);
	if (info == NULL)
		return pData;

	width += 4;

	int height = GetHeightA(info);

	if (!height)
	{
		PMULTILINES_FONT_INFO ptr = info;

		while (ptr != NULL)
		{
			info = ptr;

			ptr = ptr->m_Next;

			info->Data.clear();
			delete info;
		}

		return pData;
	}
	
	int blocksize = height * width;

	pData.resize(blocksize, 0);

	int lineOffsY = 0;
	PMULTILINES_FONT_INFO ptr = info;

	bool partialHue = (font != 5 && font != 8) && !m_UnusePartialHue;
	int font6OffsetY = (int)(font == 6) * 7;

	while (ptr != NULL)
	{
		info = ptr;

		th.LinesCount = th.LinesCount + 1;
		
		int w = 0;
		if (ptr->Align == TS_CENTER)
		{
			w = (((width - 10) - ptr->Width) / 2);
			if (w < 0)
				w = 0;
		}
		else if (ptr->Align == TS_RIGHT)
		{
			w = ((width - 10) - ptr->Width);
			if (w < 0)
				w = 0;
		}
		else if (ptr->Align == TS_LEFT && (flags & UOFONT_INDENTION))
			w = ptr->IndentionOffset;

		int count = (int)ptr->Data.size();

		IFOR(i, 0, count)
		{
			uchar index = (uchar)ptr->Data[i].item;
			int offsY = GetFontOffsetY(font, index);

			FONT_CHARACTER_DATA &fcd = fd.Chars[m_FontIndex[index]];

			int dw = fcd.Width;
			int dh = fcd.Height;

			ushort charColor = color;

			IFOR(y, 0, dh)
			{
				int testY = (int)y + lineOffsY + offsY;

				if (testY >= height)
					break;

				IFOR(x, 0, dw)
				{
					if (((int)x + w) >= width)
						break;

					ushort pic = fcd.Data[(y * dw) + x];

					if (pic)
					{
						uint pcl = 0;

						if (partialHue)
							pcl = g_ColorManager.GetPartialHueColor(pic, charColor);
						else
							pcl = g_ColorManager.GetColor(pic, charColor);

						int block = (testY * width) + ((int)x + w);

						pData[block] = pcl << 8 | 0xFF; // (0xFF << 24) | (GetBValue(pcl) << 16) | (GetGValue(pcl) << 8) | GetRValue(pcl);
					}
				}
			}

			w += dw;
		}

		lineOffsY += (ptr->MaxHeight - font6OffsetY);

		ptr = ptr->m_Next;

		info->Data.clear();
		delete info;
	}

	th.Width = width;
	th.Height = height;

	return pData;
}
//----------------------------------------------------------------------------------
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
bool CFontsManager::GenerateABase(const uchar &font, CGLTextTexture &th, const string &str, const ushort &color, const int &width, const TEXT_ALIGN_TYPE &align, const ushort &flags)
{
	WISPFUN_DEBUG("c143_f17");
	UINT_LIST pixels = GeneratePixelsA(font, th, str.c_str(), color, width, align, flags);
	bool result = false;

	if (pixels.size())
	{
		g_GL_BindTexture32(th, th.Width, th.Height, &pixels[0]);

		result = true;
	}

	return true;
}
//----------------------------------------------------------------------------------
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
void CFontsManager::DrawA(const uchar &font, const string &str, const ushort &color, const int &x, const int &y, const int &width, const TEXT_ALIGN_TYPE &align, const ushort &flags)
{
	WISPFUN_DEBUG("c143_f18");
	CGLTextTexture th;

	if (GenerateA(font, th, str, color, width, align, flags))
		th.Draw(x, y);
}









//----------------------------------------------------------------------------------
//--------------------------------------Unicode-------------------------------------
//----------------------------------------------------------------------------------
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
WISP_GEOMETRY::CPoint2Di CFontsManager::GetCaretPosW(const uchar &font, const wstring &str, int pos, int width, const TEXT_ALIGN_TYPE &align, const ushort &flags)
{
	WISPFUN_DEBUG("c143_f19");
	WISP_GEOMETRY::CPoint2Di p;

	if (pos < 1 || font >= 20 || !m_UnicodeFontAddress[font] || str.empty())
		return p;

	if (!width)
		width = GetWidthW(font, str);

	PMULTILINES_FONT_INFO info = GetInfoW(font, str.c_str(), (int)str.length(), align, flags, width);
	if (info == NULL)
		return p;
	
	puint table = (puint)m_UnicodeFontAddress[font];
	int height = 0;

	while (info != NULL)
	{
		if (pos > 0)
		{
			int len = info->CharCount;

			if (len && pos - len < 1)
			{
				IFOR(i, 0, len)
				{
					const wchar_t &ch = info->Data[i].item;
					int cwidth = 0;
					uint offset = table[ch];

					if (offset && offset != 0xFFFFFFFF)
					{
						puchar cptr = (puchar)((size_t)table + offset);
						p.X += ((char)cptr[0] + (char)cptr[2] + 1);
					}
					else if (ch == L' ')
						p.X += UNICODE_SPACE_WIDTH;
					else
						pos++;

					pos--;

					if (pos < 1)
						break;
				}
			}

			pos -= len;

			if (pos <= 1)
			{
				p.Y = height;

				if (pos == 1)
					p.Y += info->MaxHeight;

				break;
			}
			else
				height += info->MaxHeight;

			pos--;
		}

		PMULTILINES_FONT_INFO ptr = info;

		info = info->m_Next;

		ptr->Data.clear();
		delete ptr;
	}

	return p;
}
//----------------------------------------------------------------------------------
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
int CFontsManager::CalculateCaretPosW(const uchar &font, const wstring &str, const int &x, const int &y, int width, const TEXT_ALIGN_TYPE &align, const ushort &flags)
{
	WISPFUN_DEBUG("c143_f20");
	if (x < 0 || y < 0 || font >= 20 || !m_UnicodeFontAddress[font] || str.empty())
		return 0;

	if (!width)
		width = GetWidthW(font, str);

	if (x >= width)
		return (int)str.length();

	PMULTILINES_FONT_INFO info = GetInfoW(font, str.c_str(), (int)str.length(), align, flags, width);
	if (info == NULL)
		return 0;

	int height = GetHeightW(info);

	if (y >= height)
	{
		while (info != NULL)
		{
			PMULTILINES_FONT_INFO ptr = info;
			info = info->m_Next;

			ptr->Data.clear();
			delete ptr;
		}

		return 0;
	}

	height = 0;

	puint table = (puint)m_UnicodeFontAddress[font];
	int pos = 0;
	bool found = false;

	while (info != NULL)
	{
		height += info->MaxHeight;
		width = 0;

		if (!found)
		{
			if (y < height)
			{
				int len = info->CharCount;

				IFOR(i, 0, len)
				{
					const wchar_t &ch = info->Data[i].item;
					int offset = table[ch];

					if (offset && offset != 0xFFFFFFFF)
					{
						puchar cptr = (puchar)((size_t)table + offset);
						width += ((char)cptr[0] + (char)cptr[2] + 1);
					}
					else if (ch == L' ')
						width += UNICODE_SPACE_WIDTH;

					if (width > x)
						break;

					pos++;
				}

				found = true;
			}
			else
			{
				pos += info->CharCount;
				pos++;
			}
		}

		PMULTILINES_FONT_INFO ptr = info;

		info = info->m_Next;

		ptr->Data.clear();
		delete ptr;
	}

	return pos;
}
//----------------------------------------------------------------------------------
/*!
Получить ширину текста
@param [__in] font Шрифт
@param [__in] str Текст
@param [__in_opt] len Длина текста
@return Ширина текста в пикселях
*/
int CFontsManager::GetWidthW(const uchar &font, const wstring &str)
{
	WISPFUN_DEBUG("c143_f21");
	if (font >= 20 || !m_UnicodeFontAddress[font] || str.empty())
		return 0;

	puint table = (puint)m_UnicodeFontAddress[font];
	int textLength = 0;
	int maxTextLength = 0;

	for (const wchar_t &c : str)
	{
		uint &offset = table[c];

		if (offset && offset != 0xFFFFFFFF)
		{
			puchar ptr = (puchar)((size_t)table + offset);
			textLength += ((char)ptr[0] + (char)ptr[2] + 1);
		}
		else if (c == L' ')
			textLength += UNICODE_SPACE_WIDTH;
		else if (c == L'\n' || c == L'\r')
		{
			maxTextLength = max(maxTextLength, textLength);
			textLength = 0;
		}
	}

	return max(maxTextLength, textLength);
}
//----------------------------------------------------------------------------------
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
int CFontsManager::GetWidthExW(const uchar &font, const wstring &str, const int &maxWidth, const TEXT_ALIGN_TYPE &align, const ushort &flags)
{
	WISPFUN_DEBUG("c143_f22");
	if (font >= 20 || !m_UnicodeFontAddress[font] || str.empty())
		return 0;

	PMULTILINES_FONT_INFO info = GetInfoW(font, str.c_str(), (int)str.length(), align, flags, maxWidth);

	int textWidth = 0;

	while (info != NULL)
	{
		if (info->Width > textWidth)
			textWidth = info->Width;

		PMULTILINES_FONT_INFO ptr = info;

		info = info->m_Next;

		ptr->Data.clear();
		delete ptr;
	}

	return textWidth;
}
//----------------------------------------------------------------------------------
/*!
Получить высоту текста
@param [__in] font Шрифт
@param [__in] str Текст
@param [__in_opt] width Ширина текстуры
@param [__in_opt] align Расположение текста
@param [__in_opt] flags Эффекты текста
@return Высота текста в пикселях
*/
int CFontsManager::GetHeightW(const uchar &font, const wstring &str, int width, const TEXT_ALIGN_TYPE &align, const ushort &flags)
{
	WISPFUN_DEBUG("c143_f23");
	if (font >= 20 || !m_UnicodeFontAddress[font] || str.empty())
		return 0;

	if (!width)
		width = GetWidthW(font, str);

	PMULTILINES_FONT_INFO info = GetInfoW(font, str.c_str(), (int)str.length(), align, flags, width);

	int textHeight = 0;

	while (info != NULL)
	{
		if (m_UseHTML)
			textHeight += MAX_HTML_TEXT_HEIGHT;
		else
			textHeight += info->MaxHeight;

		PMULTILINES_FONT_INFO ptr = info;

		info = info->m_Next;

		ptr->Data.clear();
		delete ptr;
	}

	return textHeight;
}
//----------------------------------------------------------------------------------
/*!
Получить высоту текста по списку строк
@param [__in] info Ссылка на мультистрочный текст
@return Высота текста в пикселях
*/
int CFontsManager::GetHeightW(PMULTILINES_FONT_INFO info)
{
	WISPFUN_DEBUG("c143_f24");

	int textHeight = 0;

	for (; info != NULL; info = info->m_Next)
	{
		if (m_UseHTML)
			textHeight += MAX_HTML_TEXT_HEIGHT;
		else
			textHeight += info->MaxHeight;
	}

	return textHeight;
}
//----------------------------------------------------------------------------------
/*!
Получить текст указанной ширины
@param [__in] font Шрифт
@param [__in] str Текст
@param [__in] len Длина текста
@param [__in] width Ширина текстуры
@param [__in] IsCropped Ограниченный текст, вышедшая за доступные пределы часть обрезается и заменяется на многоточие
@return Результирующий текст
*/
wstring CFontsManager::GetTextByWidthW(const uchar &font, const wstring &str, int width, const bool &isCropped)
{
	WISPFUN_DEBUG("c143_f25");
	if (font >= 20 || !m_UnicodeFontAddress[font] || str.empty())
		return wstring(L"");

	puint table = (puint)m_UnicodeFontAddress[font];

	if (isCropped)
	{
		uint offset = table[L'.'];

		if (offset && offset != 0xFFFFFFFF)
			width -= (*((puchar)((size_t)table + offset + 2)) * 3);
	}

	int textLength = 0;
	wstring result = L"";

	for (const wchar_t &c : str)
	{
		uint offset = table[c];
		char charWidth = 0;

		if (offset && offset != 0xFFFFFFFF)
		{
			puchar ptr = (puchar)((size_t)table + offset);
			charWidth = ((char)ptr[0] + (char)ptr[2] + 1);
		}
		else if (c == L' ')
			charWidth = UNICODE_SPACE_WIDTH;

		if (charWidth)
		{
			textLength += charWidth;

			if (textLength > width)
				break;

			result += c;
		}
	}

	if (isCropped)
		result += L"...";

	return result;
}
//----------------------------------------------------------------------------------
ushort CFontsManager::GetWebLinkID(const wstring &link, uint &color)
{
	WISPFUN_DEBUG("c143_f26");
	return GetWebLinkID(ToString(link), color);
}
//----------------------------------------------------------------------------------
/*!
Получить индекс ссылки
@param [__in] link Ссылка
@param [__out] color Цвет ссылки
@return Индекс ссылки
*/
ushort CFontsManager::GetWebLinkID(const string &link, uint &color)
{
	WISPFUN_DEBUG("c143_f27");
	ushort linkID = 0;

	WEBLINK_MAP::iterator it = m_WebLink.begin();

	for (; it != m_WebLink.end(); ++it)
	{
		if ((*it).second.WebLink == link)
			break;
	}

	if (it == m_WebLink.end())
	{
		linkID = (ushort)m_WebLink.size() + 1;
		WEB_LINK wl = { false, link };
		m_WebLink[linkID] = wl;
	}
	else
	{
		if ((*it).second.Visited)
			color = m_VisitedWebLinkColor;

		linkID = (*it).first;
	}

	return linkID;
}
//----------------------------------------------------------------------------------
HTMLCHAR_LIST CFontsManager::GetHTMLData(uchar font, const wchar_t *str, int &len, TEXT_ALIGN_TYPE align, ushort flags)
{
	WISPFUN_DEBUG("c143_f28");
	HTMLCHAR_LIST data;

	if (len < 1)
		return data;

	data.resize(len);
	int newlen = 0;

	HTML_DATA_INFO info = { HTT_NONE, align, flags, font, m_HTMLColor, 0 };

	vector<HTML_DATA_INFO> stack;
	stack.push_back(info);

	HTML_DATA_INFO currentInfo = info;

	IFOR(i, 0, len)
	{
		wchar_t si = str[i];

		if (si == L'<')
		{
			bool endTag = false;
			HTML_DATA_INFO newInfo = { HTT_NONE, TS_LEFT, 0, 0xFF, 0, 0 };

			HTML_TAG_TYPE tag = ParseHTMLTag(str, len, i, endTag, newInfo);

			if (tag == HTT_NONE)
				continue;

			if (!endTag)
			{
				if (newInfo.Font == 0xFF)
					newInfo.Font = stack.back().Font;

				if (tag != HTT_BODY)
					stack.push_back(newInfo);
				else
				{
					stack.clear();
					newlen = 0;

					if (newInfo.Color)
						info.Color = newInfo.Color;

					stack.push_back(info);
				}
			}
			else if (stack.size() > 1)
			{
				//stack.pop_back();

				int index = -1;

				DFOR(j, (int)stack.size() - 1, 1)
				{
					if (stack[j].Tag == tag)
					{
						stack.erase(stack.begin() + j);
						break;
					}
				}
			}

			currentInfo = GetCurrentHTMLInfo(stack);

			switch (tag)
			{
				case HTT_LEFT:
				case HTT_CENTER:
				case HTT_RIGHT:
				{
					if (newlen)
						endTag = true;
				}
				case HTT_P:
				{
					if (endTag)
						si = L'\n';
					else
						si = 0;

					break;
				}
				case HTT_BR:
				case HTT_BQ:
				{
					si = L'\n';

					break;
				}
				default:
				{
					si = 0;

					break;
				}
			}
		}

		if (si)
		{
			data[newlen].Char = si;
			data[newlen].Font = currentInfo.Font;
			data[newlen].Align = currentInfo.Align;
			data[newlen].Flags = currentInfo.Flags;
			data[newlen].Color = currentInfo.Color;
			data[newlen].LinkID = currentInfo.Link;

			newlen++;
		}
	}

	data.resize(newlen);
	len = newlen;

	return data;
}
//----------------------------------------------------------------------------------
HTML_DATA_INFO CFontsManager::GetHTMLInfoFromTag(const HTML_TAG_TYPE &tag)
{
	WISPFUN_DEBUG("c143_f29");
	HTML_DATA_INFO info = { tag, TS_LEFT, 0, 0xFF, 0, 0 };

	switch (tag)
	{
		case HTT_B:
			info.Flags = UOFONT_SOLID;
			break;
		case HTT_I:
			info.Flags = UOFONT_ITALIC;
			break;
		case HTT_U:
			info.Flags = UOFONT_UNDERLINE;
			break;
		case HTT_P:
			info.Flags = UOFONT_INDENTION;
			break;
		case HTT_BIG:
			info.Font = 0;
			break;
		case HTT_SMALL:
			info.Font = 2;
			break;
		case HTT_H1:
			info.Flags = UOFONT_SOLID | UOFONT_UNDERLINE;
			info.Font = 0;
			break;
		case HTT_H2:
			info.Flags = UOFONT_SOLID;
			info.Font = 0;
			break;
		case HTT_H3:
			info.Font = 0;
			break;
		case HTT_H4:
			info.Flags = UOFONT_SOLID;
			info.Font = 2;
			break;
		case HTT_H5:
			info.Flags = UOFONT_ITALIC;
			info.Font = 2;
			break;
		case HTT_H6:
			info.Font = 2;
			break;
		case HTT_BQ:
			info.Flags = UOFONT_BQ;
			info.Color = 0x008000FF;
			break;
		case HTT_LEFT:
			info.Align = TS_LEFT;
			break;
		case HTT_CENTER:
			info.Align = TS_CENTER;
			break;
		case HTT_RIGHT:
			info.Align = TS_RIGHT;
			break;
		default:
			break;
	}

	return info;
}
//----------------------------------------------------------------------------------
HTML_DATA_INFO CFontsManager::GetCurrentHTMLInfo(const HTMLINFO_LIST &list)
{
	WISPFUN_DEBUG("c143_f30");
	HTML_DATA_INFO info = { HTT_NONE, TS_LEFT, 0, 0xFF, 0, 0 };

	IFOR(i, 0, (int)list.size())
	{
		const HTML_DATA_INFO &current = list[i];

		switch (current.Tag)
		{
			case HTT_NONE:
				info = current;
				break;
			case HTT_B:
			case HTT_I:
			case HTT_U:
			case HTT_P:
				info.Flags |= current.Flags;
				break;
			case HTT_A:
			{
				info.Flags |= current.Flags;
				info.Color = current.Color;
				info.Link = current.Link;

				break;
			}
			case HTT_BIG:
			case HTT_SMALL:
				if (current.Font != 0xFF && m_UnicodeFontAddress[current.Font])
					info.Font = current.Font;
				break;
			case HTT_BASEFONT:
			{
				if (current.Font != 0xFF && m_UnicodeFontAddress[current.Font])
					info.Font = current.Font;

				if (current.Color != 0)
					info.Color = current.Color;

				break;
			}
			case HTT_H1:
			case HTT_H2:
			case HTT_H4:
			case HTT_H5:
				info.Flags |= current.Flags;
			case HTT_H3:
			case HTT_H6:
				if (current.Font != 0xFF && m_UnicodeFontAddress[current.Font])
					info.Font = current.Font;
				break;
			case HTT_BQ:
				info.Color = current.Color;
				info.Flags |= current.Flags;
				break;
			case HTT_LEFT:
			case HTT_CENTER:
			case HTT_RIGHT:
				info.Align = current.Align;
				break;
			case HTT_DIV:
				info.Align = current.Align;
				break;
			default:
				break;
		}
	}

	return info;
}
//----------------------------------------------------------------------------------
void CFontsManager::TrimHTMLString(string &str)
{
	WISPFUN_DEBUG("c143_f31");
	if (str.length() >= 2 && str[0] == '"' && str[str.length() - 1] == '"')
	{
		str.resize(str.length() - 1);
		str.erase(str.begin());
	}
}
//----------------------------------------------------------------------------------
uint CFontsManager::GetHTMLColorFromText(string &str)
{
	WISPFUN_DEBUG("c143_f32");
	uint color = 0;

	if (str.length() > 1)
	{
		if (str[0] == '#')
		{
			char *end;
			color = strtoul(str.c_str() + 1, &end, 16);

			puchar clrBuf = (puchar)&color;
			color = (clrBuf[0] << 24) | (clrBuf[1] << 16) | (clrBuf[2] << 8) | 0xFF;
		}
		else
		{
			str = ToLowerA(str);

			if (str == "red")
				color = 0x0000FFFF;
			else if (str == "cyan")
				color = 0xFFFF00FF;
			else if (str == "blue")
				color = 0xFF0000FF;
			else if (str == "darkblue")
				color = 0xA00000FF;
			else if (str == "lightblue")
				color = 0xE6D8ADFF;
			else if (str == "purple")
				color = 0x800080FF;
			else if (str == "yellow")
				color = 0x00FFFFFF;
			else if (str == "lime")
				color = 0x00FF00FF;
			else if (str == "magenta")
				color = 0xFF00FFFF;
			else if (str == "white")
				color = 0xFFFEFEFF;
			else if (str == "silver")
				color = 0xC0C0C0FF;
			else if (str == "gray" || str == "grey")
				color = 0x808080FF;
			else if (str == "black")
				color = 0x010101FF;
			else if (str == "orange")
				color = 0x00A5FFFF;
			else if (str == "brown")
				color = 0x2A2AA5FF;
			else if (str == "maroon")
				color = 0x000080FF;
			else if (str == "green")
				color = 0x008000FF;
			else if (str == "olive")
				color = 0x008080FF;
		}
	}

	return color;
}
//----------------------------------------------------------------------------------
void CFontsManager::GetHTMLInfoFromContent(HTML_DATA_INFO &info, const string &content)
{
	WISPFUN_DEBUG("c143_f33");
	WISP_FILE::CTextFileParser parser("", " =", "", "\"\"");

	STRING_LIST strings = parser.GetTokens(content.c_str());
	int size = (int)strings.size();

	for (int i = 0; i < size; i += 2)
	{
		if (i + 1 >= size)
			break;

		string str = ToLowerA(strings[i]);
		string &value = strings[i + 1];
		TrimHTMLString(value);

		if (!value.length())
			continue;

		switch (info.Tag)
		{
			case HTT_BODY:
			{
				if (str == "text")
					info.Color = GetHTMLColorFromText(value);
				else if (m_HTMLBackgroundCanBeColored && str == "bgcolor")
					m_BackgroundColor = GetHTMLColorFromText(value);
				else if (str == "link")
					m_WebLinkColor = GetHTMLColorFromText(value);
				else if (str == "vlink")
					m_VisitedWebLinkColor = GetHTMLColorFromText(value);
				else if (str == "leftmargin")
					m_LeftMargin = atoi(value.c_str());
				else if (str == "topmargin")
					m_TopMargin = atoi(value.c_str());
				else if (str == "rightmargin")
					m_RightMargin = atoi(value.c_str());
				else if (str == "bottommargin")
					m_BottomMargin = atoi(value.c_str());

				break;
			}
			case HTT_BASEFONT:
			{
				if (str == "color")
					info.Color = GetHTMLColorFromText(value);
				else if (str == "size")
				{
					uchar font = atoi(value.c_str());

					if (font && font <= 3)
						info.Font = 2;
					else
						info.Font = 0;
				}

				break;
			}
			case HTT_A:
			{
				if (str == "href")
				{
					info.Flags = UOFONT_UNDERLINE;
					info.Color = m_WebLinkColor;
					info.Link = GetWebLinkID(value, info.Color);
				}

				break;
			}
			case HTT_DIV:
			{
				if (str == "align")
				{
					str = ToLowerA(value);

					if (str == "left")
						info.Align = TS_LEFT;
					else if (str == "center")
						info.Align = TS_CENTER;
					else if (str == "right")
						info.Align = TS_RIGHT;
				}

				break;
			}
			default:
				break;
		}
	}
}
//----------------------------------------------------------------------------------
HTML_TAG_TYPE CFontsManager::ParseHTMLTag(const wchar_t *str, const int &len, intptr_t &i, bool &endTag, HTML_DATA_INFO &info)
{
	WISPFUN_DEBUG("c143_f34");
	HTML_TAG_TYPE tag = HTT_NONE;

	i++;

	if (i < len && str[i] == L'/')
	{
		endTag = true;

		i++;
	}

	while (str[i] == L' ' && i < len)
		i++;

	int j = (int)i;

	for (; i < len; i++)
	{
		if (str[i] == L' ' || str[i] == L'>')
			break;
	}

	if (j != (int)i && i < len)
	{
		int cmdLen = (int)i - j;
		//LOG("cmdLen = %i\n", cmdLen);
		wstring cmd;
		cmd.resize(cmdLen);
		memcpy(&cmd[0], &str[j], cmdLen * 2);
		//LOG(L"cmd[%s] = %s\n", (endTag ? L"end" : L"start"), cmd.c_str());
		cmd = ToLowerW(cmd);

		j = (int)i;
			
		while (str[i] != L'>' && i < len)
			i++;

		if (cmd == L"b")
			tag = HTT_B;
		else if (cmd == L"i")
			tag = HTT_I;
		else if (cmd == L"a")
			tag = HTT_A;
		else if (cmd == L"u")
			tag = HTT_U;
		else if (cmd == L"p")
			tag = HTT_P;
		else if (cmd == L"big")
			tag = HTT_BIG;
		else if (cmd == L"small")
			tag = HTT_SMALL;
		else if (cmd == L"body")
			tag = HTT_BODY;
		else if (cmd == L"basefont")
			tag = HTT_BASEFONT;
		else if (cmd == L"h1")
			tag = HTT_H1;
		else if (cmd == L"h2")
			tag = HTT_H2;
		else if (cmd == L"h3")
			tag = HTT_H3;
		else if (cmd == L"h4")
			tag = HTT_H4;
		else if (cmd == L"h5")
			tag = HTT_H5;
		else if (cmd == L"h6")
			tag = HTT_H6;
		else if (cmd == L"br")
			tag = HTT_BR;
		else if (cmd == L"bq")
			tag = HTT_BQ;
		else if (cmd == L"left")
			tag = HTT_LEFT;
		else if (cmd == L"center")
			tag = HTT_CENTER;
		else if (cmd == L"right")
			tag = HTT_RIGHT;
		else if (cmd == L"div")
			tag = HTT_DIV;

		if (!endTag)
		{
			info = GetHTMLInfoFromTag(tag);

			if (i < len && j != (int)i)
			{
				switch (tag)
				{
					case HTT_BODY:
					case HTT_BASEFONT:
					case HTT_A:
					case HTT_DIV:
					{
						wstring content = L"";
						cmdLen = (int)i - j;
						//LOG("contentCmdLen = %i\n", cmdLen);
						content.resize(cmdLen);
						memcpy(&content[0], &str[j], cmdLen * 2);
						//LOG(L"contentCmd = %s\n", content.c_str());

						if (content.size())
							GetHTMLInfoFromContent(info, ToString(content));

						break;
					}
					default:
						break;
				}
			}
		}
	}

	return tag;
}
//----------------------------------------------------------------------------------
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
PMULTILINES_FONT_INFO CFontsManager::GetInfoHTML(uchar font, const wchar_t *str, int len, TEXT_ALIGN_TYPE align, ushort flags, int width)
{
	WISPFUN_DEBUG("c143_f35");
	HTMLCHAR_LIST htmlData = GetHTMLData(font, str, len, align, flags);

	if (!htmlData.size())
		return NULL;

	PMULTILINES_FONT_INFO info = new MULTILINES_FONT_INFO();
	info->Reset();
	info->Align = align;

	PMULTILINES_FONT_INFO ptr = info;

	int indentionOffset = 0;

	ptr->IndentionOffset = indentionOffset;

	int charCount = 0;
	int lastSpace = 0;
	int readWidth = 0;

	bool isFixed = ((flags & UOFONT_FIXED) != 0);
	bool isCropped = ((flags & UOFONT_CROPPED) != 0);

	if (len)
		ptr->Align = htmlData[0].Align;

	IFOR(i, 0, len)
	{
		wchar_t si = htmlData[i].Char;
		puint table = (puint)m_UnicodeFontAddress[htmlData[i].Font];

		if (si == 0x000D || si == L'\n')
		{
			if (si == 0x000D || isFixed || isCropped)
				si = 0;
			else
				si = L'\n';
		}

		if ((!table[si] || table[si] == 0xFFFFFFFF) && si != L' ' && si != L'\n')
			continue;

		puchar data = (puchar)((size_t)table + table[si]);

		if (si == L' ')
		{
			lastSpace = (int)i;
			ptr->Width += readWidth;
			readWidth = 0;
			ptr->CharCount += charCount;
			charCount = 0;
		}

		int solidWidth = (int)(htmlData[i].Flags & UOFONT_SOLID);

		if (ptr->Width + readWidth + ((char)data[0] + (char)data[2]) + solidWidth > width || si == L'\n')
		{
			if (lastSpace == ptr->CharStart && !lastSpace && si != L'\n')
				ptr->CharStart = 1;

			if (si == L'\n')
			{
				ptr->Width += readWidth;
				ptr->CharCount += charCount;

				lastSpace = (int)i;

				if (!ptr->Width)
					ptr->Width = 1;

				// (!ptr->MaxHeight)
					//ptr->MaxHeight = 14;
				ptr->MaxHeight = MAX_HTML_TEXT_HEIGHT;

				ptr->Data.resize(ptr->CharCount);

				PMULTILINES_FONT_INFO newptr = new MULTILINES_FONT_INFO();
				newptr->Reset();

				ptr->m_Next = newptr;

				ptr = newptr;

				ptr->Align = htmlData[i].Align;
				ptr->CharStart = (int)i + 1;

				readWidth = 0;
				charCount = 0;
				indentionOffset = 0;

				ptr->IndentionOffset = indentionOffset;

				continue;
			}
			else if (lastSpace + 1 == ptr->CharStart && !isFixed && !isCropped)
			{
				ptr->Width += readWidth;
				ptr->CharCount += charCount;

				if (!ptr->Width)
					ptr->Width = 1;

				//if (!ptr->MaxHeight)
					//ptr->MaxHeight = 10;
				ptr->MaxHeight = MAX_HTML_TEXT_HEIGHT;

				PMULTILINES_FONT_INFO newptr = new MULTILINES_FONT_INFO();
				newptr->Reset();

				ptr->m_Next = newptr;

				ptr = newptr;

				ptr->Align = htmlData[i].Align;
				ptr->CharStart = (int)i;
				lastSpace = (int)i - 1;
				charCount = 0;

				if (ptr->Align == TS_LEFT && (htmlData[i].Flags & UOFONT_INDENTION))
					indentionOffset = 14;
				
				ptr->IndentionOffset = indentionOffset;

				readWidth = (int)indentionOffset;
			}
			else
			{
				if (isFixed)
				{
					MULTILINES_FONT_DATA mfd = {si, htmlData[i].Flags, htmlData[i].Font, htmlData[i].LinkID, htmlData[i].Color};
					ptr->Data.push_back(mfd);

					readWidth += ((char)data[0] + (char)data[2] + 1);

					//if (((char)data[1] + (char)data[3]) > ptr->MaxHeight)
						//ptr->MaxHeight = ((char)data[1] + (char)data[3]);
					ptr->MaxHeight = MAX_HTML_TEXT_HEIGHT;
				
					charCount++;

					ptr->Width += readWidth;
					ptr->CharCount += charCount;
				}

				i = lastSpace + 1;
				
				si = htmlData[i].Char;
				solidWidth = (int)(htmlData[i].Flags & UOFONT_SOLID);

				if (!ptr->Width)
					ptr->Width = 1;

				//if (!ptr->MaxHeight)
					//ptr->MaxHeight = 10;
				ptr->MaxHeight = MAX_HTML_TEXT_HEIGHT;

				ptr->Data.resize(ptr->CharCount);
				charCount = 0;

				if (isFixed || isCropped)
					break;

				PMULTILINES_FONT_INFO newptr = new MULTILINES_FONT_INFO();
				newptr->Reset();

				ptr->m_Next = newptr;

				ptr = newptr;

				ptr->Align = htmlData[i].Align;
				ptr->CharStart = (int)i;

				if (ptr->Align == TS_LEFT && (htmlData[i].Flags & UOFONT_INDENTION))
					indentionOffset = 14;

				ptr->IndentionOffset = indentionOffset;

				readWidth = indentionOffset;
			}
		}
				
		MULTILINES_FONT_DATA mfd = {si, htmlData[i].Flags, htmlData[i].Font, htmlData[i].LinkID, htmlData[i].Color};
		ptr->Data.push_back(mfd);

		if (si == L' ')
		{
			readWidth += UNICODE_SPACE_WIDTH;

			//if (!ptr->MaxHeight)
				//ptr->MaxHeight = 5;
		}
		else
		{
			readWidth += ((char)data[0] + (char)data[2] + 1) + solidWidth;

			//if (((char)data[1] + (char)data[3]) > ptr->MaxHeight)
				//->MaxHeight = ((char)data[1] + (char)data[3]);
		}

		charCount++;
	}

	ptr->Width += readWidth;
	ptr->CharCount += charCount;
	ptr->MaxHeight = MAX_HTML_TEXT_HEIGHT;

	return info;
}
//----------------------------------------------------------------------------------
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
PMULTILINES_FONT_INFO CFontsManager::GetInfoW(uchar font, const wchar_t *str, int len, TEXT_ALIGN_TYPE align, ushort flags, int width)
{
	WISPFUN_DEBUG("c143_f36");
	m_WebLinkColor = 0xFF0000FF;
	m_VisitedWebLinkColor = 0x0000FFFF;
	m_BackgroundColor = 0;
	m_LeftMargin = 0;
	m_TopMargin = 0;
	m_RightMargin = 0;
	m_BottomMargin = 0;

	if (font >= 20 || !m_UnicodeFontAddress[font])
		return NULL;

	if (m_UseHTML)
		return GetInfoHTML(font, str, len, align, flags, width);

	puint table = (puint)m_UnicodeFontAddress[font];

	PMULTILINES_FONT_INFO info = new MULTILINES_FONT_INFO();
	info->Reset();
	info->Align = align;

	PMULTILINES_FONT_INFO ptr = info;

	int indentionOffset = 0;

	ptr->IndentionOffset = 0;

	int charCount = 0;
	int lastSpace = 0;
	int readWidth = 0;

	bool isFixed = ((flags & UOFONT_FIXED) != 0);
	bool isCropped = ((flags & UOFONT_CROPPED) != 0);

	TEXT_ALIGN_TYPE current_align = align;
	ushort current_flags = flags;
	uchar current_font = font;
	uint charcolor = 0xFFFFFFFF;
	uint current_charcolor = 0xFFFFFFFF;
	uint lastspace_charcolor = 0xFFFFFFFF;
	uint lastspace_current_charcolor = 0xFFFFFFFF;

	IFOR(i, 0, len)
	{
		wchar_t si = str[i];

		if (si == L'\r' || si == L'\n')
		{
			if (isFixed || isCropped)
				si = 0;
			else
				si = L'\n';
		}

		if ((!table[si] || table[si] == 0xFFFFFFFF) && si != L' ' && si != L'\n')
			continue;

		puchar data = (puchar)((size_t)table + table[si]);

		if (si == L' ')
		{
			lastSpace = (int)i;
			ptr->Width += readWidth;
			readWidth = 0;
			ptr->CharCount += charCount;
			charCount = 0;
			lastspace_charcolor = charcolor;
			lastspace_current_charcolor = current_charcolor;
		}

		if (ptr->Width + readWidth + ((char)data[0] + (char)data[2]) > width || si == L'\n')
		{
			if (lastSpace == ptr->CharStart && !lastSpace && si != L'\n')
				ptr->CharStart = 1;

			if (si == L'\n')
			{
				ptr->Width += readWidth;
				ptr->CharCount += charCount;

				lastSpace = (int)i;

				if (!ptr->Width)
					ptr->Width = 1;

				if (!ptr->MaxHeight)
					ptr->MaxHeight = 14;

				ptr->Data.resize(ptr->CharCount);

				PMULTILINES_FONT_INFO newptr = new MULTILINES_FONT_INFO();
				newptr->Reset();

				ptr->m_Next = newptr;

				ptr = newptr;

				ptr->Align = current_align;
				ptr->CharStart = (int)i + 1;

				readWidth = 0;
				charCount = 0;
				indentionOffset = 0;

				ptr->IndentionOffset = 0;

				continue;
			}
			else if (lastSpace + 1 == ptr->CharStart && !isFixed && !isCropped)
			{
				ptr->Width += readWidth;
				ptr->CharCount += charCount;

				if (!ptr->Width)
					ptr->Width = 1;

				if (!ptr->MaxHeight)
					ptr->MaxHeight = 14;

				PMULTILINES_FONT_INFO newptr = new MULTILINES_FONT_INFO();
				newptr->Reset();

				ptr->m_Next = newptr;

				ptr = newptr;

				ptr->Align = current_align;
				ptr->CharStart = (int)i;
				lastSpace = (int)i - 1;
				charCount = 0;

				if (ptr->Align == TS_LEFT && (current_flags & UOFONT_INDENTION))
					indentionOffset = 14;

				ptr->IndentionOffset = indentionOffset;

				readWidth = indentionOffset;
			}
			else
			{
				if (isFixed)
				{
					MULTILINES_FONT_DATA mfd = {si, current_flags, current_font, 0, current_charcolor};
					ptr->Data.push_back(mfd);

					readWidth += ((char)data[0] + (char)data[2] + 1);

					if (((char)data[1] + (char)data[3]) > ptr->MaxHeight)
						ptr->MaxHeight = ((char)data[1] + (char)data[3]);

					charCount++;

					ptr->Width += readWidth;
					ptr->CharCount += charCount;
				}

				i = lastSpace + 1;

				charcolor = lastspace_charcolor;
				current_charcolor = lastspace_current_charcolor;
				si = str[i];

				if (!ptr->Width)
					ptr->Width = 1;

				if (!ptr->MaxHeight)
					ptr->MaxHeight = 14;

				ptr->Data.resize(ptr->CharCount);

				if (isFixed || isCropped)
					break;

				PMULTILINES_FONT_INFO newptr = new MULTILINES_FONT_INFO();
				newptr->Reset();

				ptr->m_Next = newptr;

				ptr = newptr;

				ptr->Align = current_align;
				ptr->CharStart = (int)i;
				charCount = 0;

				if (ptr->Align == TS_LEFT && (current_flags & UOFONT_INDENTION))
					indentionOffset = 14;

				ptr->IndentionOffset = indentionOffset;

				readWidth = indentionOffset;
			}
		}

		MULTILINES_FONT_DATA mfd = {si, current_flags, current_font, 0, current_charcolor};
		ptr->Data.push_back(mfd);

		if (si == L' ')
		{
			readWidth += UNICODE_SPACE_WIDTH;

			if (!ptr->MaxHeight)
				ptr->MaxHeight = 5;
		}
		else
		{
			readWidth += ((char)data[0] + (char)data[2] + 1);

			if (((char)data[1] + (char)data[3]) > ptr->MaxHeight)
				ptr->MaxHeight = ((char)data[1] + (char)data[3]);
		}

		charCount++;
	}

	ptr->Width += readWidth;
	ptr->CharCount += charCount;

	if (!readWidth && len && (str[len - 1] == L'\n' || str[len - 1] == L'\r'))
	{
		ptr->Width = 1;
		ptr->MaxHeight = 14;
	}

	return info;
}
//----------------------------------------------------------------------------------
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
bool CFontsManager::GenerateW(const uchar &font, CGLTextTexture &th, const wstring &str, const ushort &color, const uchar &cell, const int &width, const TEXT_ALIGN_TYPE &align, const ushort &flags)
{
	WISPFUN_DEBUG("c143_f37");
	if ((flags & UOFONT_FIXED) || (flags & UOFONT_CROPPED))
	{
		th.Clear();

		if (!width || str.empty())
			return false;

		int realWidth = GetWidthW(font, str);

		if (realWidth > width)
		{
			wstring newstr = GetTextByWidthW(font, str, width, (flags & UOFONT_CROPPED));
			return GenerateWBase(font, th, newstr, color, cell, width, align, flags);
		}
	}

	return GenerateWBase(font, th, str, color, cell, width, align, flags);
}
//----------------------------------------------------------------------------------
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
UINT_LIST CFontsManager::GeneratePixelsW(const uchar &font, CGLTextTexture &th, const wchar_t *str, const ushort &color, const uchar &cell, int width, const TEXT_ALIGN_TYPE &align, const ushort &flags)
{
	WISPFUN_DEBUG("c143_f38");
	UINT_LIST pData;

	if (font >= 20 || !m_UnicodeFontAddress[font])
		return pData;

	int len = lstrlenW(str);

	if (!len)
		return pData;

	int oldWidth = width;

	if (!width)
	{
		width = GetWidthW(font, str);

		if (!width)
			return pData;
	}

	PMULTILINES_FONT_INFO info = GetInfoW(font, str, len, align, flags, width);

	if (info == NULL)
		return pData;

	if (m_UseHTML && (m_LeftMargin || m_RightMargin))
	{
		while (info != NULL)
		{
			PMULTILINES_FONT_INFO ptr = info->m_Next;

			info->Data.clear();
			delete info;

			info = ptr;
		}

		int newWidth = width - (m_LeftMargin + m_RightMargin);

		if (newWidth < 10)
			newWidth = 10;

		info = GetInfoW(font, str, len, align, flags, newWidth);

		if (info == NULL)
			return pData;
	}

	if (!oldWidth && m_RecalculateWidthByInfo)
	{
		PMULTILINES_FONT_INFO ptr = info;
		width = 0;

		while (ptr != NULL)
		{
			if (ptr->Width > width)
				width = ptr->Width;

			ptr = ptr->m_Next;
		}
	}

	width += 4;

	int height = GetHeightW(info);

	if (!height)
	{
		while (info != NULL)
		{
			PMULTILINES_FONT_INFO ptr = info;

			info = info->m_Next;

			ptr->Data.clear();
			delete ptr;
		}

		return pData;
	}

	height += m_TopMargin + m_BottomMargin + 4;

	int blocksize = height * width;

	pData.resize(blocksize, 0);

	puint table = (puint)m_UnicodeFontAddress[font];

	int lineOffsY = 1 + m_TopMargin;

	PMULTILINES_FONT_INFO ptr = info;

	uint datacolor = 0;

	if (/*m_UseHTML &&*/ color == 0xFFFF)
		datacolor = 0xFFFFFFFE;
	else
		datacolor = g_ColorManager.GetPolygoneColor(cell, color) << 8 | 0xFF;

	bool isItalic = (flags & UOFONT_ITALIC);
	bool isSolid = (flags & UOFONT_SOLID);
	bool isBlackBorder = (flags & UOFONT_BLACK_BORDER);
	bool isUnderline = (flags & UOFONT_UNDERLINE);
	uint blackColor = 0x010101FF;

	bool isLink = false;
	int linkStartX = 0;
	int linkStartY = 0;

	while (ptr != NULL)
	{
		info = ptr;

		th.LinesCount = th.LinesCount + 1;

		int w = m_LeftMargin;

		if (ptr->Align == TS_CENTER)
		{
			w += (((width - 10) - ptr->Width) / 2);
			if (w < 0)
				w = 0;
		}
		else if (ptr->Align == TS_RIGHT)
		{
			w += ((width - 10) - ptr->Width);

			if (w < 0)
				w = 0;
		}
		else if (ptr->Align == TS_LEFT && (flags & UOFONT_INDENTION))
			w += ptr->IndentionOffset;

		ushort oldLink = 0;

		int dataSize = (int)ptr->Data.size();
		IFOR(i, 0, dataSize)
		{
			const MULTILINES_FONT_DATA &dataPtr = ptr->Data[i];
			const wchar_t &si = dataPtr.item;

			table = (puint)m_UnicodeFontAddress[dataPtr.font];

			if (!isLink)
			{
				oldLink = dataPtr.linkID;

				if (oldLink)
				{
					isLink = true;
					linkStartX = w;
					linkStartY = lineOffsY + 3;
				}
			}
			else if (!dataPtr.linkID || i + 1 == dataSize)
			{
				isLink = false;
				int linkHeight = lineOffsY - linkStartY;
				if (linkHeight < 14)
					linkHeight = 14;

				int ofsX = 0;

				if (si == L' ')
					ofsX = UNICODE_SPACE_WIDTH;
				else if ((!table[si] || table[si] == 0xFFFFFFFF) && si != L' ')
				{
				}
				else
				{
					puchar xData = (puchar)((size_t)table + table[si]);
					ofsX = (char)xData[2];
				}

				WEB_LINK_RECT wlr = { oldLink, linkStartX, linkStartY, w - ofsX, linkHeight };
				th.AddWebLink(wlr);
				oldLink = 0;
			}

			/*if (m_UseHTML)
			{
			if (i >= ptr->Data.size()) break;
			si = ptr->Data[i].item;
			}*/

			if ((!table[si] || table[si] == 0xFFFFFFFF) && si != L' ')
				continue;

			puchar data = (puchar)((size_t)table + table[si]);

			int offsX = 0;
			int offsY = 0;
			int dw = 0;
			int dh = 0;

			if (si == L' ')
			{
				offsX = 0;
				dw = UNICODE_SPACE_WIDTH;
			}
			else
			{
				offsX = (char)data[0] + 1;
				offsY = (char)data[1];
				dw = (char)data[2];
				dh = (char)data[3];

				data += 4;
			}

			int tmpW = w;
			uint charcolor = datacolor;
			bool isBlackPixel = (((charcolor >> 24) & 0xFF) <= 8 && ((charcolor >> 16) & 0xFF) <= 8 && ((charcolor >> 8) & 0xFF) <= 8);

			if (si != L' ')
			{
				if (m_UseHTML && i < (int)ptr->Data.size())
				{
					isItalic = (dataPtr.flags & UOFONT_ITALIC);
					isSolid = (dataPtr.flags & UOFONT_SOLID);
					isBlackBorder = (dataPtr.flags & UOFONT_BLACK_BORDER);
					isUnderline = (dataPtr.flags & UOFONT_UNDERLINE);

					if (dataPtr.color != 0xFFFFFFFF)
					{
						charcolor = dataPtr.color;
						isBlackPixel = (((charcolor >> 24) & 0xFF) <= 8 && ((charcolor >> 16) & 0xFF) <= 8 && ((charcolor >> 8) & 0xFF) <= 8);
					}
				}

				int scanlineCount = (int)((dw - 1) / 8) + 1;

				IFOR(y, 0, dh)
				{
					int testY = offsY + lineOffsY + (int)y;

					if (testY >= height)
						break;

					puchar scanlines = data;
					data += scanlineCount;

					int italicOffset = 0;
					if (isItalic)
						italicOffset = (int)((dh - (int)y) / ITALIC_FONT_KOEFFICIENT);

					int testX = w + offsX + italicOffset + (int)isSolid;

					IFOR(c, 0, scanlineCount)
					{
						IFOR(j, 0, 8)
						{
							int x = ((int)c * 8) + (int)j;

							if (x >= dw)
								break;

							int nowX = testX + (int)x;

							if (nowX >= width)
								break;

							uchar cl = scanlines[c] & (1 << (7 - j));

							int block = (testY * width) + nowX;

							if (cl)
								pData[block] = charcolor;
						}
					}
				}

				if (isSolid)
				{
					uint solidColor = blackColor;

					if (solidColor == charcolor)
						solidColor++;

					int minXOk = ((w + offsX) > 0) ? -1 : 0;
					int maxXOk = ((w + offsX + dw) < width) ? 1 : 0;

					maxXOk += dw;

					IFOR(cy, 0, dh)
					{
						int testY = offsY + lineOffsY + (int)cy;

						if (testY >= height)
							break;

						int italicOffset = 0;
						if (isItalic && cy < dh)
							italicOffset = (int)((dh - (int)cy) / ITALIC_FONT_KOEFFICIENT);

						IFOR(cx, minXOk, maxXOk)
						{
							int testX = (int)cx + w + offsX + italicOffset;

							if (testX >= width/* + italicOffset*/)
								break;

							int block = (testY * width) + testX;

							if (!pData[block] && pData[block] != solidColor)
							{
								int endX = (cx < dw) ? 2 : 1;

								if (endX == 2 && (testX + 1) >= width)
									endX--;

								IFOR(x, 0, endX)
								{
									int nowX = testX + (int)x;

									int testBlock = (testY * width) + nowX;

									if (pData[testBlock] && pData[testBlock] != solidColor)
									{
										pData[block] = solidColor;
										break;
									}
								}
							}
						}
					}

					IFOR(cy, 0, dh)
					{
						int testY = offsY + lineOffsY + (int)cy;

						if (testY >= height)
							break;

						int italicOffset = 0;
						if (isItalic)
							italicOffset = (int)((dh - cy) / ITALIC_FONT_KOEFFICIENT);

						IFOR(cx, 0, dw)
						{
							int testX = (int)cx + w + offsX + italicOffset;

							if (testX >= width/* + italicOffset*/)
								break;

							int block = (testY * width) + testX;

							if (pData[block] == solidColor)
								pData[block] = charcolor;
						}
					}
				}

				if (isBlackBorder && !isBlackPixel)
				{
					int minXOk = (w + offsX > 0) ? -1 : 0;
					int minYOk = (offsY + lineOffsY > 0) ? -1 : 0;
					int maxXOk = (w + offsX + dw < width) ? 1 : 0;
					int maxYOk = (offsY + lineOffsY + dh < height) ? 1 : 0;

					maxXOk += dw;
					maxYOk += dh;

					IFOR(cy, minYOk, maxYOk)
					{
						int testY = offsY + lineOffsY + (int)cy;

						if (testY >= height)
							break;

						int italicOffset = 0;
						if (isItalic && cy >= 0 && cy < dh)
							italicOffset = (int)((dh - (int)cy) / ITALIC_FONT_KOEFFICIENT);

						IFOR(cx, minXOk, maxXOk)
						{
							int testX = (int)cx + w + offsX + italicOffset;

							if (testX >= width/* + italicOffset*/)
								break;

							int block = (testY * width) + testX;

							if (!pData[block] && pData[block] != blackColor)
							{
								int startX = (cx > 0) ? -1 : 0;
								int startY = (cy > 0) ? -1 : 0;
								int endX = (cx < dw - 1) ? 2 : 1;
								int endY = (cy < dh - 1) ? 2 : 1;

								if (endX == 2 && (testX + 1) >= width)
									endX--;

								bool passed = false;

								IFOR(x, startX, endX)
								{
									int nowX = testX + (int)x;

									IFOR(y, startY, endY)
									{
										int testBlock = ((testY + (int)y) * width) + nowX;

										if (pData[testBlock] && pData[testBlock] != blackColor)
										{
											pData[block] = blackColor;

											passed = true;

											break;
										}
									}

									if (passed)
										break;
								}
							}
						}
					}
				}

				w += (dw + offsX + (int)isSolid);
			}
			else if (si == L' ')
			{
				w += UNICODE_SPACE_WIDTH;

				if (m_UseHTML)
				{
					isUnderline = (dataPtr.flags & UOFONT_UNDERLINE);

					if (dataPtr.color != 0xFFFFFFFF)
					{
						charcolor = dataPtr.color;
						isBlackPixel = (((charcolor >> 24) & 0xFF) <= 8 && ((charcolor >> 16) & 0xFF) <= 8 && ((charcolor >> 8) & 0xFF) <= 8);
					}
				}
			}

			if (isUnderline)
			{
				int minXOk = ((tmpW + offsX) > 0) ? -1 : 0;
				int maxXOk = ((w + offsX + dw) < width) ? 1 : 0;

				puchar aData = (puchar)((size_t)table + table[L'a']);

				int testY = lineOffsY + (char)aData[1] + (char)aData[3];

				if (testY >= height)
					break;

				IFOR(cx, minXOk, dw + maxXOk)
				{
					int testX = ((int)cx + tmpW + offsX + (int)isSolid);

					if (testX >= width)
						break;

					int block = (testY * width) + testX;

					pData[block] = charcolor;
				}
			}
		}

		lineOffsY += ptr->MaxHeight;

		ptr = ptr->m_Next;

		info->Data.clear();
		delete info;
	}

	if (m_UseHTML && m_HTMLBackgroundCanBeColored && m_BackgroundColor)
	{
		m_BackgroundColor |= 0xFF;

		IFOR(y, 0, height)
		{
			int yPos = ((int)y * width);

			IFOR(x, 0, width)
			{
				uint &p = pData[yPos + x];

				if (!p)
					p = m_BackgroundColor;
			}
		}
	}

	th.Width = width;
	th.Height = height;

	return pData;
}
//----------------------------------------------------------------------------------
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
bool CFontsManager::GenerateWBase(const uchar &font, CGLTextTexture &th, const wstring &str, const ushort &color, const uchar &cell, const int &width, const TEXT_ALIGN_TYPE &align, const ushort &flags)
{
	WISPFUN_DEBUG("c143_f39");
	UINT_LIST pixels = GeneratePixelsW(font, th, str.c_str(), color, cell, width, align, flags);
	bool result = false;

	if (pixels.size())
	{
		g_GL_BindTexture32(th, th.Width, th.Height, &pixels[0]);

		result = true;
	}

	return result;
}
//----------------------------------------------------------------------------------
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
void CFontsManager::DrawW(const uchar &font, const wstring &str, const ushort &color, const int &x, const int &y, const uchar &cell, const int &width, const TEXT_ALIGN_TYPE &align, const ushort &flags)
{
	WISPFUN_DEBUG("c143_f40");
	CGLTextTexture th;

	if (GenerateW(font, th, str, color, cell, width, align, flags))
		th.Draw(x, y);
}
//----------------------------------------------------------------------------------
//!Таблица ассоциации ASCII шрифтов
uchar CFontsManager::m_FontIndex[256] =
{
	0xFF, //0
	0xFF, //1
	0xFF, //2
	0xFF, //3
	0xFF, //4
	0xFF, //5
	0xFF, //6
	0xFF, //7
	0xFF, //8
	0xFF, //9
	0xFF, //10
	0xFF, //11
	0xFF, //12
	0xFF, //13
	0xFF, //14
	0xFF, //15
	0xFF, //16
	0xFF, //17
	0xFF, //18
	0xFF, //19
	0xFF, //20
	0xFF, //21
	0xFF, //22
	0xFF, //23
	0xFF, //24
	0xFF, //25
	0xFF, //26
	0xFF, //27
	0xFF, //28
	0xFF, //29
	0xFF, //30
	0xFF, //31
	0,    //32
	1,    //33
	2,    //34
	3,    //35
	4,    //36
	5,    //37
	6,    //38
	7,    //39
	8,    //40
	9,    //41
	10,   //42
	11,   //43
	12,   //44
	13,   //45
	14,   //46
	15,   //47
	16,   //48
	17,   //49
	18,   //50
	19,   //51
	20,   //52
	21,   //53
	22,   //54
	23,   //55
	24,   //56
	25,   //57
	26,   //58
	27,   //59
	28,   //60
	29,   //61
	30,   //62
	31,   //63
	32,   //64
	33,   //65
	34,   //66
	35,   //67
	36,   //68
	37,   //69
	38,   //70
	39,   //71
	40,   //72
	41,   //73
	42,   //74
	43,   //75
	44,   //76
	45,   //77
	46,   //78
	47,   //79
	48,   //80
	49,   //81
	50,   //82
	51,   //83
	52,   //84
	53,   //85
	54,   //86
	55,   //87
	56,   //88
	57,   //89
	58,   //90
	59,   //91
	60,   //92
	61,   //93
	62,   //94
	63,   //95
	64,   //96
	65,   //97
	66,   //98
	67,   //99
	68,   //100
	69,   //101
	70,   //102
	71,   //103
	72,   //104
	73,   //105
	74,   //106
	75,   //107
	76,   //108
	77,   //109
	78,   //110
	79,   //111
	80,   //112
	81,   //113
	82,   //114
	83,   //115
	84,   //116
	85,   //117
	86,   //118
	87,   //119
	88,   //120
	89,   //121
	90,   //122
	0xFF, //123
	0xFF, //124
	0xFF, //125
	0xFF, //126
	0xFF, //127
	0xFF, //128
	0xFF, //129
	0xFF, //130
	0xFF, //131
	0xFF, //132
	0xFF, //133
	0xFF, //134
	0xFF, //135
	0xFF, //136
	0xFF, //137
	0xFF, //138
	0xFF, //139
	0xFF, //140
	0xFF, //141
	0xFF, //142
	0xFF, //143
	0xFF, //144
	0xFF, //145
	0xFF, //146
	0xFF, //147
	0xFF, //148
	0xFF, //149
	0xFF, //150
	0xFF, //151
	0xFF, //152
	0xFF, //153
	0xFF, //154
	0xFF, //155
	0xFF, //156
	0xFF, //157
	0xFF, //158
	0xFF, //159
	0xFF, //160
	0xFF, //161
	0xFF, //162
	0xFF, //163
	0xFF, //164
	0xFF, //165
	0xFF, //166
	0xFF, //167
	136,  //168
	0xFF, //169
	0xFF, //170
	0xFF, //171
	0xFF, //172
	0xFF, //173
	0xFF, //174
	0xFF, //175
	0xFF, //176
	0xFF, //177
	0xFF, //178
	0xFF, //179
	0xFF, //180
	0xFF, //181
	0xFF, //182
	0xFF, //183
	152,  //184
	0xFF, //185
	0xFF, //186
	0xFF, //187
	0xFF, //188
	0xFF, //189
	0xFF, //190
	0xFF, //191
	160,  //192
	161,  //193
	162,  //194
	163,  //195
	164,  //196
	165,  //197
	166,  //198
	167,  //199
	168,  //200
	169,  //201
	170,  //202
	171,  //203
	172,  //204
	173,  //205
	174,  //206
	175,  //207
	176,  //208
	177,  //209
	178,  //210
	179,  //211
	180,  //212
	181,  //213
	182,  //214
	183,  //215
	184,  //216
	185,  //217
	186,  //218
	187,  //219
	188,  //220
	189,  //221
	190,  //222
	191,  //223
	192,  //224
	193,  //225
	194,  //226
	195,  //227
	196,  //228
	197,  //229
	198,  //230
	199,  //231
	200,  //232
	201,  //233
	202,  //234
	203,  //235
	204,  //236
	205,  //237
	206,  //238
	207,  //239
	208,  //240
	209,  //241
	210,  //242
	211,  //243
	212,  //244
	213,  //245
	214,  //246
	215,  //247
	216,  //248
	217,  //249
	218,  //250
	219,  //251
	220,  //252
	221,  //253
	222,  //254
	223   //255
};
//----------------------------------------------------------------------------------
