/***********************************************************************************
**
** EntryText.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CEntryText *g_EntryPointer = NULL;
//----------------------------------------------------------------------------------
CEntryText::CEntryText(int maxLength, int width, int maxWidth, bool numberOnly)
: m_MaxLength(maxLength), m_Width(width), m_MaxWidth(maxWidth), m_NumberOnly(numberOnly)
{
	WISPFUN_DEBUG("c169_f1");
}
//----------------------------------------------------------------------------------
CEntryText::~CEntryText()
{
	WISPFUN_DEBUG("c169_f2");
	//Если удаляемое поле - текущее то выставим консоль согласно конфигу
	if (g_EntryPointer == this)
	{
		if (g_ConfigManager.GetConsoleNeedEnter())
			g_EntryPointer = NULL;
		else
		{
			if (g_GameState >= GS_GAME)
				g_EntryPointer = &g_GameConsole;
			else
				g_EntryPointer = NULL;
		}
	}

	//Очистим данные
	Clear();
}
//----------------------------------------------------------------------------------
//Вызывается при клике на текстовое поле (если это предусмотрено в обработчике)
const char *CEntryText::c_str()
{
	WISPFUN_DEBUG("c169_f3");
	m_CText = ToString(m_Text);
	return m_CText.c_str();
}
//----------------------------------------------------------------------------------
//Вызывается при клике на текстовое поле (если это предусмотрено в обработчике)
void CEntryText::OnClick(CGump *gump, uchar font, bool unicode, int x, int y, TEXT_ALIGN_TYPE align, ushort flags)
{
	WISPFUN_DEBUG("c169_f4");
	//Текстовое поле - текущее поле ввода
	if (g_EntryPointer == this)
	{
		//Запомним позицию
		int oldPos = m_Position;

		//Вычислим новую позицию согласно типа шрифта
		if (unicode)
			m_Position = g_FontManager.CalculateCaretPosW(font, m_Text.c_str(), x, y, m_Width, align, flags);
		else
			m_Position = g_FontManager.CalculateCaretPosA(font, c_str(), x, y, m_Width, align, flags);

		//Если позиция изменилась - укажем что текст изменен
		if (oldPos != m_Position)
			m_Changed = true;
	}
	else //Иначе зададим текущее поле как поле для ввода
	{
		//Получим гамп с предыдущим указателем ввода
		CGump *gumpEntry = g_GumpManager.GetTextEntryOwner();

		//Если гамп найден - обновим окно гампа
		if (gumpEntry != NULL)
			gumpEntry->FrameCreated = false;

		//Изменим указатель
		g_EntryPointer = this;
		m_Changed = true;
	}

	//Если это ентри гампа - обновим его
	if (gump != NULL)
		gump->FrameCreated = false;
}
//----------------------------------------------------------------------------------
//Вызывается при обработке нажатия клавиши
void CEntryText::OnKey(CGump *gump, WPARAM wParam)
{
	WISPFUN_DEBUG("c169_f5");
	switch (wParam)
	{
		case VK_HOME:
		{
			SetPos(0, gump);

			break;
		}
		case VK_END:
		{
			SetPos(Length(), gump);

			break;
		}
		case VK_LEFT:
		{
			AddPos(-1, gump);

			break;
		}
		case VK_RIGHT:
		{
			AddPos(1, gump);

			break;
		}
		case VK_BACK:
		{
			Remove(true, gump);

			break;
		}
		case VK_DELETE:
		{
			Remove(false, gump);

			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------------
//Получить количество строк
int CEntryText::GetLinesCountA(uchar font, TEXT_ALIGN_TYPE align, ushort flags, int width)
{
	WISPFUN_DEBUG("c169_f6");
	if (!width)
		width = m_Width;

	MULTILINES_FONT_INFO *info = g_FontManager.GetInfoA(font, c_str(), Length(), align, flags, width);

	int count = 0;

	while (info != NULL)
	{
		MULTILINES_FONT_INFO *next = info->m_Next;
		delete info;
		info = next;
		count++;
	}

	return count;
}
//----------------------------------------------------------------------------------
//Получить количество строк
int CEntryText::GetLinesCountW(uchar font, TEXT_ALIGN_TYPE align, ushort flags, int width)
{
	WISPFUN_DEBUG("c169_f7");
	if (!width)
		width = m_Width;

	MULTILINES_FONT_INFO *info = g_FontManager.GetInfoW(font, Data(), Length(), align, flags, width);

	int count = 0;

	while (info != NULL)
	{
		MULTILINES_FONT_INFO *next = info->m_Next;
		delete info;
		info = next;
		count++;
	}

	return count;
}
//----------------------------------------------------------------------------------
//Вставить символ относительно m_Position
bool CEntryText::Insert(wchar_t ch, CGump *gump)
{
	WISPFUN_DEBUG("c169_f8");
	//Коррекция позиции
	if (m_Position < 0)
		m_Position = 0;

	//Коррекция позиции
	if (m_Position > (int)m_Text.length())
		m_Position = m_Text.length();

	//Если максимальная ширина задана
	if (m_MaxLength > 0)
	{
		//Если это числовое поле для ввода
		if (m_NumberOnly)
		{
			wstring wstr = m_Text;
			wstr.insert(wstr.begin() + m_Position, ch);

			if (_wtoi(wstr.c_str()) >= m_MaxLength) //Сверим максимальное значение
				return false;
		}
		else if ((int)m_Text.length() >= m_MaxLength) //Иначе - сверим длину с максимальной
			return false;
	}

	//После прохождения всех проверок и коррекций - добавляем символ в указанную позицию
	m_Text.insert(m_Text.begin() + m_Position, ch);

	//Увеличиваем индекс позиции
	m_Position++;

	//Изменения внесены
	m_Changed = true;

	//Обновляем гамп (если есть)
	if (gump != NULL)
		gump->FrameCreated = false;

	return true;
}
//----------------------------------------------------------------------------------
//Удалить символ относительно m_Position (true - слева, false - справа)
void CEntryText::Remove(bool left, CGump *gump)
{
	WISPFUN_DEBUG("c169_f9");
	//Ткнули Backspace
	if (left)
	{
		//Удалять нечего
		if (m_Position < 1)
			return;

		m_Position--;
	}
	else //Или ткнули Delete
	{
		//Уже в конце строки
		if (m_Position >= (int)m_Text.length())
			return;
	}

	//Удаляем указанный символ
	m_Text.erase(m_Text.begin() + m_Position);

	//Регистрируем изменения
	m_Changed = true;

	//Обновляем гамп (если есть)
	if (gump != NULL)
		gump->FrameCreated = false;
}
//----------------------------------------------------------------------------------
/*!
Очистить данные
@return 
*/
void CEntryText::Clear()
{
	WISPFUN_DEBUG("c169_f10");
	//Очистка
	m_Text = L"";
	m_CText = "";
	m_Position = 0;
	m_Changed = true;
	m_DrawOffset = 0;
	m_Texture.Clear();
	memset(&m_CaretPos, 0, sizeof(m_CaretPos));
}
//----------------------------------------------------------------------------------
void CEntryText::Paste()
{
	WISPFUN_DEBUG("c169_f11");
	if (OpenClipboard(g_OrionWindow.Handle))
	{
		HANDLE hData = GetClipboardData(CF_UNICODETEXT);

		if (hData != NULL)
		{
			wstring text((wchar_t*)GlobalLock(hData));

			IFOR(i, 0, (int)text.length())
				Insert(text[i]);

			GlobalUnlock(hData);
		}

		CloseClipboard();
	}
}
//----------------------------------------------------------------------------------
//Изменение позиции m_Position
void CEntryText::AddPos(int val, CGump *gump)
{
	WISPFUN_DEBUG("c169_f12");
	//Добавляем (вычитаем) позицию
	m_Position += val;

	//Корректировка
	if (m_Position < 0)
		m_Position = 0;

	//Корректировка
	if (m_Position > (int)m_Text.length())
		m_Position = m_Text.length();
	
	//Регистрируем изменения
	m_Changed = true;

	//Обновляем гамп (если есть)
	if (gump != NULL)
		gump->FrameCreated = false;
}
//----------------------------------------------------------------------------------
//Изменение позиции m_Position
void CEntryText::SetPos(int val, CGump *gump)
{
	WISPFUN_DEBUG("c169_f13");
	//Выставляем указанную позицию
	m_Position = val;

	//Корректировка
	if (m_Position < 0)
		m_Position = 0;

	//Корректировка
	if (m_Position > (int)m_Text.length())
		m_Position = m_Text.length();
	
	//Регистрируем изменения
	m_Changed = true;

	//Обновляем гамп (если есть)
	if (gump != NULL)
		gump->FrameCreated = false;
}
//----------------------------------------------------------------------------------
//Изменение текста
void CEntryText::SetText(const string &text)
{
	WISPFUN_DEBUG("c169_f14");
	//Перевод ASCII в юникод строку
	wstring wtext = ToWString(text);

	//Стандартная процедура изменения текста
	SetText(wtext);
}
//----------------------------------------------------------------------------------
//Изменение текста
void CEntryText::SetText(const wstring &text)
{
	WISPFUN_DEBUG("c169_f15");
	//Очистка перед изменением
	Clear();

	//Изменим текст и выставим указатель в конец текста
	m_Text = text;
	m_Position = m_Text.length();

	if (m_Position < 0)
		m_Position = 0;

	//Если указана максимальная длина
	if (m_MaxLength > 0)
	{
		//Если это числовое поле для ввода
		if (m_NumberOnly)
		{
			string str = ToString(m_Text);

			//Пока строка не пустая и значение больше допустимого - удаляем по 1 символу сзади
			while (true)
			{
				int len = str.length();

				if (std::atoi(str.c_str()) >= m_MaxLength && len > 0)
					str.resize(len - 1);
				else
					break;
			}
		}
		else if ((int)m_Text.length() >= m_MaxLength) //Иначе - сверим длину
			m_Text.resize(m_MaxLength);
	}

	//Обновим гамп (если есть)
	CGump *gump = g_GumpManager.GetTextEntryOwner();

	if (gump != NULL)
		gump->FrameCreated = false;
}
//----------------------------------------------------------------------------------
//Проверка ширины текста и корректировка входящей строки (при необходимости)
string CEntryText::CheckMaxWidthA(uchar font, string str)
{
	WISPFUN_DEBUG("c169_f16");
	//Если задана максимальная ширина
	if (m_MaxWidth > 0)
	{
		//Вычислим текущую ширину
		int width = g_FontManager.GetWidthA(font, str.c_str(), 0);
		int len = str.length();

		//И пока строка не будет соответствовать указанным параметрам - урезаем ее
		while (m_MaxWidth < width && len > 0)
		{
			str.erase(str.begin() + len);
			len--;
			width = g_FontManager.GetWidthA(font, str.c_str(), 0);
		}
	}

	return str;
}
//----------------------------------------------------------------------------------
wstring CEntryText::CheckMaxWidthW(uchar font, wstring str)
{
	WISPFUN_DEBUG("c169_f17");
	//Если задана максимальная ширина
	if (m_MaxWidth > 0)
	{
		//Вычислим текущую ширину
		int width = g_FontManager.GetWidthW(font, str);
		int len = str.length();

		//И пока строка не будет соответствовать указанным параметрам - урезаем ее
		while (m_MaxWidth < width && len > 0)
		{
			str.erase(str.begin() + len);
			len--;
			width = g_FontManager.GetWidthW(font, str);
		}
	}

	return str;
}
//----------------------------------------------------------------------------------
void CEntryText::FixMaxWidthA(uchar font)
{
	WISPFUN_DEBUG("c169_f18");
	c_str();

	//Если задана максимальная ширина
	if (m_MaxWidth > 0)
	{
		//Вычислим текущую ширину
		int width = g_FontManager.GetWidthA(font, m_CText.c_str(), 0);
		int len = m_CText.length();

		//И пока строка не будет соответствовать указанным параметрам - урезаем ее
		while (m_MaxWidth < width && len > 0)
		{
			Remove((m_Position > 0), NULL);
			len--;
			width = g_FontManager.GetWidthA(font, c_str(), 0);
		}
	}
}
//----------------------------------------------------------------------------------
void CEntryText::FixMaxWidthW(uchar font)
{
	WISPFUN_DEBUG("c169_f19");
	//Если задана максимальная ширина
	if (m_MaxWidth > 0)
	{
		//Вычислим текущую ширину
		int width = g_FontManager.GetWidthW(font, m_Text);
		int len = m_Text.length();

		//И пока строка не будет соответствовать указанным параметрам - урезаем ее
		while (m_MaxWidth < width && len > 0)
		{
			Remove((m_Position > 0), NULL);
			len--;
			width = g_FontManager.GetWidthW(font, m_Text);
		}
	}
}
//----------------------------------------------------------------------------------
void CEntryText::CreateTextureA(uchar font, string str, ushort color, int width, TEXT_ALIGN_TYPE align, ushort flags)
{
	WISPFUN_DEBUG("c169_f20");
	//Если это пустая строка - очищаем все
	if (!str.length())
		Clear();
	else
	{
		//Чистим текстуру
		m_Texture.Clear();

		//Вычисляем позицию каретки
		if (m_Position)
		{
			m_CaretPos = g_FontManager.GetCaretPosA(font, str.c_str(), m_Position, width, align, flags);

			if (flags & UOFONT_FIXED)
			{
				if (m_DrawOffset)
				{
					if (m_CaretPos.X + m_DrawOffset < 0)
						m_DrawOffset = -m_CaretPos.X;
					else if (m_Width + -m_DrawOffset < m_CaretPos.X)
						m_DrawOffset = m_Width - m_CaretPos.X;
				}
				else if (m_Width + m_DrawOffset < m_CaretPos.X)
					m_DrawOffset = m_Width - m_CaretPos.X;
				else
					m_DrawOffset = 0;

				/*if (m_Width + m_DrawOffset < m_CaretPos.x)
					m_DrawOffset = m_Width - m_CaretPos.x;
				else
					m_DrawOffset = 0;*/
			}
		}
		else //Либо обнуляем ее
		{
			memset(&m_CaretPos, 0, sizeof(m_CaretPos));
			m_DrawOffset = 0;
		}

		//Генерируем текстуру
		g_FontManager.GenerateA(font, m_Texture, str.c_str(), color, m_Width + abs(m_DrawOffset), align, flags);
	}
}
//----------------------------------------------------------------------------------
void CEntryText::CreateTextureW(uchar font, wstring str, ushort color, int width, TEXT_ALIGN_TYPE align, ushort flags)
{
	WISPFUN_DEBUG("c169_f21");
	//Если это пустая строка - очищаем все
	if (!str.length())
		Clear();
	else
	{
		//Чистим текстуру
		m_Texture.Clear();

		//Вычисляем позицию каретки
		if (m_Position)
		{
			m_CaretPos = g_FontManager.GetCaretPosW(font, str, m_Position, width, align, flags);

			if (flags & UOFONT_FIXED)
			{
				if (m_DrawOffset)
				{
					if (m_CaretPos.X + m_DrawOffset < 0)
						m_DrawOffset = -m_CaretPos.X;
					else if (m_Width + -m_DrawOffset < m_CaretPos.X)
						m_DrawOffset = m_Width - m_CaretPos.X;
				}
				else if (m_Width + m_DrawOffset < m_CaretPos.X)
					m_DrawOffset = m_Width - m_CaretPos.X;
				else
					m_DrawOffset = 0;
			}
		}
		else //Либо обнуляем ее
		{
			memset(&m_CaretPos, 0, sizeof(m_CaretPos));
			m_DrawOffset = 0;
		}

		//Генерируем текстуру
		g_FontManager.GenerateW(font, m_Texture, str.c_str(), color, 30, m_Width, align, flags);
	}
}
//----------------------------------------------------------------------------------
void CEntryText::PrepareToDrawA(uchar font, ushort color, TEXT_ALIGN_TYPE align, ushort flags)
{
	WISPFUN_DEBUG("c169_f22");
	//Если изменился текст или цвет
	if (m_Changed || m_Color != color)
	{
		//Урезаем данные до максимальных габаритов
		FixMaxWidthA(font);

		//Создаем текстуру
		CreateTextureA(font, m_CText, color, /*m_MaxWidth*/m_Width, align, flags);

		//Регистрируем изменения
		m_Changed = false;
		m_Color = color;

		//Если это поле для ввода - сгенерируем каретку
		if (this == g_EntryPointer)
			g_FontManager.GenerateA(font, m_CaretTexture, "_", color);
	}
}
//----------------------------------------------------------------------------------
void CEntryText::PrepareToDrawW(uchar font, ushort color, TEXT_ALIGN_TYPE align, ushort flags)
{
	WISPFUN_DEBUG("c169_f23");
	//Если изменился текст или цвет
	if (m_Changed || m_Color != color)
	{
		//Урезаем данные до максимальных габаритов
		FixMaxWidthW(font);

		//Создаем текстуру
		CreateTextureW(font, m_Text, color, m_Width, align, flags);

		//Регистрируем изменения
		m_Changed = false;
		m_Color = color;

		//Если это поле для ввода - сгенерируем каретку
		if (this == g_EntryPointer)
			g_FontManager.GenerateW(font, m_CaretTexture, L"_", color, 30);
	}
}
//----------------------------------------------------------------------------------
void CEntryText::DrawA(uchar font, ushort color, int x, int y, TEXT_ALIGN_TYPE align, ushort flags)
{
	WISPFUN_DEBUG("c169_f24");
	PrepareToDrawA(font, color, align, flags);

	//Отрисовка текстуры
	m_Texture.Draw(x + m_DrawOffset, y);

	//Если это поле для ввода - отобразим каретку
	if (this == g_EntryPointer)
	{
		//Таблица смещений по оси Y
		const int offsetTable[] = {1, 2, 1, 1, 1, 2, 1, 1, 2, 2};
		int offsY = offsetTable[font % 10];

		//Отрисуем каретку
		m_CaretTexture.Draw(x + m_DrawOffset + m_CaretPos.X, y + offsY + m_CaretPos.Y);
	}
}
//----------------------------------------------------------------------------------
void CEntryText::DrawW(uchar font, ushort color, int x, int y, TEXT_ALIGN_TYPE align, ushort flags)
{
	WISPFUN_DEBUG("c169_f25");
	PrepareToDrawW(font, color, align, flags);

	//Отрисовка текстуры
	m_Texture.Draw(x + m_DrawOffset, y);

	//Если это поле для ввода - отобразим каретку
	if (this == g_EntryPointer)
		m_CaretTexture.Draw(x + m_DrawOffset + m_CaretPos.X, y + m_CaretPos.Y);
}
//----------------------------------------------------------------------------------
void CEntryText::DrawMaskA(uchar font, ushort color, int x, int y, TEXT_ALIGN_TYPE align, ushort flags)
{
	WISPFUN_DEBUG("c169_f26");
	//Накладываем на текст маску из "*"
	int len = Length();
	string str = "";
	IFOR(i, 0, len)
		str += "*";

	//Если текст есть - отрисуем его
	if (len)
		g_FontManager.DrawA(font, str.c_str(), color, x + m_DrawOffset, y);

	//Если это поле для ввода - отобразим каретку
	if (this == g_EntryPointer)
	{
		//Таблица смещений по оси Y
		const int offsetTable[] = {1, 2, 1, 1, 1, 2, 1, 1, 2, 2};
		int offsY = offsetTable[font % 10];

		//Отрегулируем смещение
		if (m_Position)
			x += g_FontManager.GetWidthA(font, str.c_str(), m_Position);

		//Отрисуем каретку
		g_FontManager.DrawA(font, "_", color, x + m_DrawOffset, y + offsY);
	}
}
//----------------------------------------------------------------------------------
void CEntryText::DrawMaskW(uchar font, ushort color, int x, int y, TEXT_ALIGN_TYPE align, ushort flags)
{
	WISPFUN_DEBUG("c169_f27");
	//Накладываем на текст маску из "*"
	int len = Length();
	wstring str = L"";
	IFOR(i, 0, len)
		str += L"*";

	//Если текст есть - отрисуем его
	if (len)
		g_FontManager.DrawW(font, str.c_str(), color, x + m_DrawOffset, y, 30, 0, TS_LEFT, flags);

	//Если это поле для ввода - отобразим каретку
	if (this == g_EntryPointer)
	{
		//Отрегулируем смещение
		if (m_Position)
		{
			str.resize(m_Position);
			x += g_FontManager.GetWidthW(font, str);
		}

		//Отрисуем каретку
		g_FontManager.DrawW(font, L"_", color, x + m_DrawOffset, y, 30, 0, TS_LEFT, flags);
	}
}
//----------------------------------------------------------------------------------
