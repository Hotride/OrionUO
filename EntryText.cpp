/****************************************************************************
**
** EntryText.cpp
**
** Copyright (C) September 2015 Hotride
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

TEntryText *EntryPointer = NULL;
//---------------------------------------------------------------------------
TEntryText::TEntryText(int maxLength, int width, int maxWidth, bool numberOnly)
: m_MaxLength(maxLength), m_Position(0), m_Changed(true), m_DrawOffset(0),
m_Width(width), m_MaxWidth(maxWidth), m_Color(0), m_NumberOnly(numberOnly)
{
	m_Text = L"";
	m_CText = "";
	memset(&m_CaretPos, 0, sizeof(m_CaretPos));
}
//---------------------------------------------------------------------------
TEntryText::~TEntryText()
{
	//Если удаляемое поле - текущее то выставим консоль согласно конфигу
	if (g_GameState >= GS_GAME && EntryPointer == this)
	{
		if (ConfigManager.GetConsoleNeedEnter())
			EntryPointer = NULL;
		else
			EntryPointer = GameConsole;
	}

	//Очистим данные
	Clear();
}
//---------------------------------------------------------------------------
//Вызывается при клике на текстовое поле (если это предусмотрено в обработчике)
const char *TEntryText::c_str()
{
	m_CText = ToString(m_Text);
	return m_CText.c_str();
}
//---------------------------------------------------------------------------
//Вызывается при клике на текстовое поле (если это предусмотрено в обработчике)
void TEntryText::OnClick(TGump *gump, BYTE font, bool unicode, int x, int y, TEXT_ALIGN_TYPE align, WORD flags)
{
	//Текстовое поле - текущее поле ввода
	if (EntryPointer == this)
	{
		//Запомним позицию
		int oldPos = m_Position;

		//Вычислим новую позицию согласно типа шрифта
		if (unicode)
			m_Position = FontManager->CalculateCaretPosW(font, m_Text.c_str(), x, y, m_Width, align, flags);
		else
			m_Position = FontManager->CalculateCaretPosA(font, c_str(), x, y, m_Width, align, flags);

		//Если позиция изменилась - укажем что текст изменен
		if (oldPos != m_Position)
			m_Changed = true;
	}
	else //Иначе зададим текущее поле как поле для ввода
	{
		//Получим гамп с предыдущим указателем ввода
		TGump *gumpEntry = GumpManager->GetTextEntryOwner();

		//Если гамп найден - обновим окно гампа
		if (gumpEntry != NULL)
			gumpEntry->FrameCreated = false;

		//Изменим указатель
		EntryPointer = this;
	}

	//Если это ентри гампа - обновим его
	if (gump != NULL)
		gump->FrameCreated = false;
}
//---------------------------------------------------------------------------
//Вызывается при обработке нажатия клавиши
void TEntryText::OnKey(TGump *gump, WPARAM wParam)
{
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
//---------------------------------------------------------------------------
//Получить количество строк
int TEntryText::GetLinesCountA(BYTE font, TEXT_ALIGN_TYPE align, WORD flags, int width)
{
	if (!width)
		width = m_Width;

	MULTILINES_FONT_INFO *info = FontManager->GetInfoA(font, c_str(), Length(), align, flags, width);

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
//---------------------------------------------------------------------------
//Получить количество строк
int TEntryText::GetLinesCountW(BYTE font, TEXT_ALIGN_TYPE align, WORD flags, int width)
{
	if (!width)
		width = m_Width;

	MULTILINES_FONT_INFO *info = FontManager->GetInfoW(font, Data(), Length(), align, flags, width);

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
//---------------------------------------------------------------------------
//Вставить символ относительно m_Position
bool TEntryText::Insert(wchar_t ch, TGump *gump)
{
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
			string str = ToString(wstr);

			if (std::atoi(str.c_str()) >= m_MaxLength) //Сверим максимальное значение
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
//---------------------------------------------------------------------------
//Удалить символ относительно m_Position (true - слева, false - справа)
void TEntryText::Remove(bool left, TGump *gump)
{
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
//---------------------------------------------------------------------------
/*!
Очистить данные
@return 
*/
void TEntryText::Clear()
{
	//Очистка
	m_Text = L"";
	m_CText = "";
	m_Position = 0;
	m_Changed = true;
	m_DrawOffset = 0;
	m_Texture.Clear();
	memset(&m_CaretPos, 0, sizeof(m_CaretPos));
}
//---------------------------------------------------------------------------
//Изменение позиции m_Position
void TEntryText::AddPos(int val, TGump *gump)
{
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
//---------------------------------------------------------------------------
//Изменение позиции m_Position
void TEntryText::SetPos(int val, TGump *gump)
{
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
//---------------------------------------------------------------------------
//Изменение текста
void TEntryText::SetText(const string &text)
{
	//Перевод ASCII в юникод строку
	wstring wtext = ToWString(text);

	//Стандартная процедура изменения текста
	SetText(wtext);
}
//---------------------------------------------------------------------------
//Изменение текста
void TEntryText::SetText(const wstring &text)
{
	//Очистка перед изменением
	Clear();

	//Изменим текст и выставим указатель в конец текста
	m_Text = text;
	m_Position = m_Text.length() - 1;

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
	if (GumpManager != NULL)
	{
		TGump *gump = GumpManager->GetTextEntryOwner();

		if (gump != NULL)
			gump->FrameCreated = false;
	}
}
//---------------------------------------------------------------------------
//Проверка ширины текста и корректировка входящей строки (при необходимости)
string TEntryText::CheckMaxWidthA(BYTE font, string str)
{
	//Если задана максимальная ширина
	if (m_MaxWidth > 0)
	{
		//Вычислим текущую ширину
		int width = FontManager->GetWidthA(font, str.c_str(), 0);
		int len = str.length();

		//И пока строка не будет соответствовать указанным параметрам - урезаем ее
		while (m_MaxWidth < width && len > 0)
		{
			str.erase(str.begin() + len);
			len--;
			width = FontManager->GetWidthA(font, str.c_str(), 0);
		}
	}

	return str;
}
//---------------------------------------------------------------------------
wstring TEntryText::CheckMaxWidthW(BYTE font, wstring str)
{
	//Если задана максимальная ширина
	if (m_MaxWidth > 0)
	{
		//Вычислим текущую ширину
		int width = FontManager->GetWidthW(font, str.c_str(), 0);
		int len = str.length();

		//И пока строка не будет соответствовать указанным параметрам - урезаем ее
		while (m_MaxWidth < width && len > 0)
		{
			str.erase(str.begin() + len);
			len--;
			width = FontManager->GetWidthW(font, str.c_str(), 0);
		}
	}

	return str;
}
//---------------------------------------------------------------------------
void TEntryText::FixMaxWidthA(BYTE font)
{
	c_str();

	//Если задана максимальная ширина
	if (m_MaxWidth > 0)
	{
		//Вычислим текущую ширину
		int width = FontManager->GetWidthA(font, m_CText.c_str(), 0);
		int len = m_CText.length();

		//И пока строка не будет соответствовать указанным параметрам - урезаем ее
		while (m_MaxWidth < width && len > 0)
		{
			Remove((m_Position > 0), NULL);
			len--;
			width = FontManager->GetWidthA(font, c_str(), 0);
		}
	}
}
//---------------------------------------------------------------------------
void TEntryText::FixMaxWidthW(BYTE font)
{
	//Если задана максимальная ширина
	if (m_MaxWidth > 0)
	{
		//Вычислим текущую ширину
		int width = FontManager->GetWidthW(font, m_Text.c_str(), 0);
		int len = m_Text.length();

		//И пока строка не будет соответствовать указанным параметрам - урезаем ее
		while (m_MaxWidth < width && len > 0)
		{
			Remove((m_Position > 0), NULL);
			len--;
			width = FontManager->GetWidthW(font, m_Text.c_str(), 0);
		}
	}
}
//---------------------------------------------------------------------------
void TEntryText::CreateTextureA(BYTE font, string str, WORD color, int width, TEXT_ALIGN_TYPE align, WORD flags)
{
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
			m_CaretPos = FontManager->GetCaretPosA(font, str.c_str(), m_Position, width, align, flags);

			if (flags & UOFONT_FIXED)
			{
				if (m_DrawOffset)
				{
					if (m_CaretPos.x + m_DrawOffset < 0)
						m_DrawOffset = -m_CaretPos.x;
					else if (m_Width + -m_DrawOffset < m_CaretPos.x)
						m_DrawOffset = m_Width - m_CaretPos.x;
				}
				else if (m_Width + m_DrawOffset < m_CaretPos.x)
					m_DrawOffset = m_Width - m_CaretPos.x;
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
		FontManager->GenerateA(font, m_Texture, str.c_str(), color, m_Width + abs(m_DrawOffset), align, flags);
	}
}
//---------------------------------------------------------------------------
void TEntryText::CreateTextureW(BYTE font, wstring str, WORD color, int width, TEXT_ALIGN_TYPE align, WORD flags)
{
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
			m_CaretPos = FontManager->GetCaretPosW(font, str.c_str(), m_Position, width, align, flags);

			if (flags & UOFONT_FIXED)
			{
				if (m_DrawOffset)
				{
					if (m_CaretPos.x + m_DrawOffset < 0)
						m_DrawOffset = -m_CaretPos.x;
					else if (m_Width + -m_DrawOffset < m_CaretPos.x)
						m_DrawOffset = m_Width - m_CaretPos.x;
				}
				else if (m_Width + m_DrawOffset < m_CaretPos.x)
					m_DrawOffset = m_Width - m_CaretPos.x;
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
		FontManager->GenerateW(font, m_Texture, str.c_str(), color, 30, m_Width, align, flags);
	}
}
//---------------------------------------------------------------------------
void TEntryText::DrawA(BYTE font, WORD color, int x, int y, TEXT_ALIGN_TYPE align, WORD flags)
{
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
	}

	//Отрисовка текстуры
	m_Texture.Draw(x + m_DrawOffset, y);

	//Если это поле для ввода - отобразим каретку
	if (this == EntryPointer)
	{
		//Таблица смещений по оси Y
		const int offsetTable[] = {1, 2, 1, 1, 1, 2, 1, 1, 2, 2};
		int offsY = offsetTable[font % 10];

		//Отрисуем каретку
		FontManager->DrawA(font, "_", color, x + m_DrawOffset + m_CaretPos.x, y + offsY + m_CaretPos.y);
	}
}
//---------------------------------------------------------------------------
void TEntryText::DrawW(BYTE font, WORD color, int x, int y, TEXT_ALIGN_TYPE align, WORD flags)
{
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
	}

	//Отрисовка текстуры
	m_Texture.Draw(x + m_DrawOffset, y);

	//Если это поле для ввода - отобразим каретку
	if (this == EntryPointer)
		FontManager->DrawW(font, L"_", color, x + m_DrawOffset + m_CaretPos.x, y + m_CaretPos.y, 30);
}
//---------------------------------------------------------------------------
void TEntryText::DrawMaskA(BYTE font, WORD color, int x, int y, TEXT_ALIGN_TYPE align, WORD flags)
{
	//Накладываем на текст маску из "*"
	int len = Length();
	string str = "";
	IFOR(i, 0, len)
		str += "*";

	//Если текст есть - отрисуем его
	if (len)
		FontManager->DrawA(font, str.c_str(), color, x + m_DrawOffset, y);

	//Если это поле для ввода - отобразим каретку
	if (this == EntryPointer)
	{
		//Таблица смещений по оси Y
		const int offsetTable[] = {1, 2, 1, 1, 1, 2, 1, 1, 2, 2};
		int offsY = offsetTable[font % 10];

		//Отрегулируем смещение
		if (m_Position)
			x += FontManager->GetWidthA(font, str.c_str(), m_Position);

		//Отрисуем каретку
		FontManager->DrawA(font, "_", color, x + m_DrawOffset, y + offsY);
	}
}
//---------------------------------------------------------------------------
void TEntryText::DrawMaskW(BYTE font, WORD color, int x, int y, TEXT_ALIGN_TYPE align, WORD flags)
{
	//Накладываем на текст маску из "*"
	int len = Length();
	wstring str = L"";
	IFOR(i, 0, len)
		str += L"*";

	//Если текст есть - отрисуем его
	if (len)
		FontManager->DrawW(font, str.c_str(), color, x + m_DrawOffset, y, 30, 0, TS_LEFT, flags);

	//Если это поле для ввода - отобразим каретку
	if (this == EntryPointer)
	{
		//Отрегулируем смещение
		if (m_Position)
			x += FontManager->GetWidthW(font, str.c_str(), m_Position);

		//Отрисуем каретку
		FontManager->DrawW(font, L"_", color, x + m_DrawOffset, y, 30, 0, TS_LEFT, flags);
	}
}
//---------------------------------------------------------------------------