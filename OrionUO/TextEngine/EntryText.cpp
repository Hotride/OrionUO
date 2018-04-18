﻿// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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
: MaxLength(maxLength), Width(width), MaxWidth(maxWidth), NumberOnly(numberOnly)
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
	m_CText = ToString(Text);
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
			m_Position = g_FontManager.CalculateCaretPosW(font, Text.c_str(), x, y, Width, align, flags);
		else
			m_Position = g_FontManager.CalculateCaretPosA(font, c_str(), x, y, Width, align, flags);

		//Если позиция изменилась - укажем что текст изменен
		if (oldPos != m_Position)
			Changed = true;
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
		Changed = true;
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
			SetPos((int)Length(), gump);

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
		width = Width;

	MULTILINES_FONT_INFO *info = g_FontManager.GetInfoA(font, c_str(), (int)Length(), align, flags, width);

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
		width = Width;

	MULTILINES_FONT_INFO *info = g_FontManager.GetInfoW(font, Data(), (int)Length(), align, flags, width);

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
	if (m_Position > (int)Text.length())
		m_Position = (int)Text.length();

	//Если максимальная ширина задана
	if (MaxLength > 0)
	{
		//Если это числовое поле для ввода
		if (NumberOnly)
		{
			wstring wstr = Text;
			wstr.insert(wstr.begin() + m_Position, ch);

			if (_wtoi(wstr.c_str()) >= MaxLength) //Сверим максимальное значение
				return false;
		}
		else if ((int)Text.length() >= MaxLength) //Иначе - сверим длину с максимальной
			return false;
	}

	//После прохождения всех проверок и коррекций - добавляем символ в указанную позицию
	Text.insert(Text.begin() + m_Position, ch);

	//Увеличиваем индекс позиции
	m_Position++;

	//Изменения внесены
	Changed = true;

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
		if (m_Position >= (int)Text.length())
			return;
	}

	//Удаляем указанный символ
	if (m_Position < Text.length())
		Text.erase(Text.begin() + m_Position);
	else
		Text.erase(Text.length() - 1);

	//Регистрируем изменения
	Changed = true;

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
	Text = L"";
	m_CText = "";
	m_Position = 0;
	Changed = true;
	DrawOffset = 0;
	m_Texture.Clear();
	CaretPos.Reset();
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
			CGump *gump = g_GumpManager.GetTextEntryOwner();
			if (gump != NULL && gump->GumpType == GT_BOOK)
				gump->PasteClipboardData(text);
			else
			{
				IFOR(i, 0, (int)text.length())
					Insert(text[i]);
			}

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
	if (m_Position > (int)Text.length())
		m_Position = (int)Text.length();
	
	//Регистрируем изменения
	Changed = true;

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
	if (m_Position > (int)Text.length())
		m_Position = (int)Text.length();
	
	//Регистрируем изменения
	Changed = true;

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
	Text = text;
	m_Position = (int)Text.length();

	if (m_Position < 0)
		m_Position = 0;

	//Если указана максимальная длина
	if (MaxLength > 0)
	{
		//Если это числовое поле для ввода
		if (NumberOnly)
		{
			string str = ToString(Text);

			//Пока строка не пустая и значение больше допустимого - удаляем по 1 символу сзади
			while (true)
			{
				size_t len = str.length();

				if (std::atoi(str.c_str()) >= MaxLength && len > 0)
					str.resize(len - 1);
				else
					break;
			}
		}
		else if ((int)Text.length() >= MaxLength) //Иначе - сверим длину
			Text.resize(MaxLength);
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
	if (MaxWidth > 0)
	{
		//Вычислим текущую ширину
		int width = g_FontManager.GetWidthA(font, str);
		size_t len = str.length();

		//И пока строка не будет соответствовать указанным параметрам - урезаем ее
		while (MaxWidth < width && len > 0)
		{
			str.erase(str.begin() + len);
			len--;
			width = g_FontManager.GetWidthA(font, str);
		}
	}

	return str;
}
//----------------------------------------------------------------------------------
wstring CEntryText::CheckMaxWidthW(uchar font, wstring str)
{
	WISPFUN_DEBUG("c169_f17");
	//Если задана максимальная ширина
	if (MaxWidth > 0)
	{
		//Вычислим текущую ширину
		int width = g_FontManager.GetWidthW(font, str);
		size_t len = str.length();

		//И пока строка не будет соответствовать указанным параметрам - урезаем ее
		while (MaxWidth < width && len > 0)
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
	if (MaxWidth > 0)
	{
		//Вычислим текущую ширину
		int width = g_FontManager.GetWidthA(font, m_CText);
		size_t len = m_CText.length();

		//И пока строка не будет соответствовать указанным параметрам - урезаем ее
		while (MaxWidth < width && len > 0)
		{
			Remove((m_Position > 0), NULL);
			len--;
			width = g_FontManager.GetWidthA(font, c_str());
		}
	}
}
//----------------------------------------------------------------------------------
void CEntryText::FixMaxWidthW(uchar font)
{
	WISPFUN_DEBUG("c169_f19");
	//Если задана максимальная ширина
	if (MaxWidth > 0)
	{
		//Вычислим текущую ширину
		int width = g_FontManager.GetWidthW(font, Text);
		size_t len = Text.length();

		//И пока строка не будет соответствовать указанным параметрам - урезаем ее
		while (MaxWidth < width && len > 0)
		{
			Remove((m_Position > 0), NULL);
			len--;
			width = g_FontManager.GetWidthW(font, Text);
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
			CaretPos = g_FontManager.GetCaretPosA(font, str.c_str(), m_Position, width, align, flags);

			if (flags & UOFONT_FIXED)
			{
				if (DrawOffset)
				{
					if (CaretPos.X + DrawOffset < 0)
						DrawOffset = -CaretPos.X;
					else if (Width + -DrawOffset < CaretPos.X)
						DrawOffset = Width - CaretPos.X;
				}
				else if (Width + DrawOffset < CaretPos.X)
					DrawOffset = Width - CaretPos.X;
				else
					DrawOffset = 0;

				/*if (Width + DrawOffset < CaretPos.x)
					DrawOffset = Width - CaretPos.x;
				else
					DrawOffset = 0;*/
			}
		}
		else //Либо обнуляем ее
		{
			CaretPos.Reset();
			DrawOffset = 0;
		}

		//Генерируем текстуру
		g_FontManager.GenerateA(font, m_Texture, str, color, Width + abs(DrawOffset), align, flags);
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
			CaretPos = g_FontManager.GetCaretPosW(font, str, m_Position, width, align, flags);

			if (flags & UOFONT_FIXED)
			{
				if (DrawOffset)
				{
					if (CaretPos.X + DrawOffset < 0)
						DrawOffset = -CaretPos.X;
					else if (Width + -DrawOffset < CaretPos.X)
						DrawOffset = Width - CaretPos.X;
				}
				else if (Width + DrawOffset < CaretPos.X)
					DrawOffset = Width - CaretPos.X;
				else
					DrawOffset = 0;
			}
		}
		else //Либо обнуляем ее
		{
			CaretPos.Reset();
			DrawOffset = 0;
		}

		//Генерируем текстуру
		g_FontManager.GenerateW(font, m_Texture, str, color, 30, Width, align, flags);
	}
}
//----------------------------------------------------------------------------------
void CEntryText::PrepareToDrawA(uchar font, ushort color, TEXT_ALIGN_TYPE align, ushort flags)
{
	WISPFUN_DEBUG("c169_f22");
	//Если изменился текст или цвет
	if (Changed || Color != color)
	{
		//Урезаем данные до максимальных габаритов
		FixMaxWidthA(font);

		//Создаем текстуру
		CreateTextureA(font, m_CText, color, /*MaxWidth*/Width, align, flags);

		//Регистрируем изменения
		Changed = false;
		Color = color;

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
	if (Changed || Color != color)
	{
		//Урезаем данные до максимальных габаритов
		FixMaxWidthW(font);

		//Создаем текстуру
		CreateTextureW(font, Text, color, Width, align, flags);

		//Регистрируем изменения
		Changed = false;
		Color = color;

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
	m_Texture.Draw(x + DrawOffset, y);

	//Если это поле для ввода - отобразим каретку
	if (this == g_EntryPointer)
	{
		//Таблица смещений по оси Y
		const int offsetTable[] = {1, 2, 1, 1, 1, 2, 1, 1, 2, 2};
		int offsY = offsetTable[font % 10];

		//Отрисуем каретку
		m_CaretTexture.Draw(x + DrawOffset + CaretPos.X, y + offsY + CaretPos.Y);
	}
}
//----------------------------------------------------------------------------------
void CEntryText::DrawW(uchar font, ushort color, int x, int y, TEXT_ALIGN_TYPE align, ushort flags)
{
	WISPFUN_DEBUG("c169_f25");
	PrepareToDrawW(font, color, align, flags);

	//Отрисовка текстуры
	m_Texture.Draw(x + DrawOffset, y);

	//Если это поле для ввода - отобразим каретку
	if (this == g_EntryPointer)
		m_CaretTexture.Draw(x + DrawOffset + CaretPos.X, y + CaretPos.Y);
}
//----------------------------------------------------------------------------------
void CEntryText::DrawMaskA(uchar font, ushort color, int x, int y, TEXT_ALIGN_TYPE align, ushort flags)
{
	WISPFUN_DEBUG("c169_f26");
	//Накладываем на текст маску из "*"
	size_t len = Length();
	string str = "";
	IFOR(i, 0, len)
		str += "*";

	//Если текст есть - отрисуем его
	if (len)
		g_FontManager.DrawA(font, str, color, x + DrawOffset, y);

	//Если это поле для ввода - отобразим каретку
	if (this == g_EntryPointer)
	{
		//Таблица смещений по оси Y
		const int offsetTable[] = {1, 2, 1, 1, 1, 2, 1, 1, 2, 2};
		int offsY = offsetTable[font % 10];

		//Отрегулируем смещение
		if (m_Position)
		{
			str.resize(m_Position);
			x += g_FontManager.GetWidthA(font, str.c_str());
		}

		//Отрисуем каретку
		g_FontManager.DrawA(font, "_", color, x + DrawOffset, y + offsY);
	}
}
//----------------------------------------------------------------------------------
void CEntryText::DrawMaskW(uchar font, ushort color, int x, int y, TEXT_ALIGN_TYPE align, ushort flags)
{
	WISPFUN_DEBUG("c169_f27");
	//Накладываем на текст маску из "*"
	size_t len = Length();
	wstring str = L"";
	IFOR(i, 0, len)
		str += L"*";

	//Если текст есть - отрисуем его
	if (len)
		g_FontManager.DrawW(font, str, color, x + DrawOffset, y, 30, 0, TS_LEFT, flags);

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
		g_FontManager.DrawW(font, L"_", color, x + DrawOffset, y, 30, 0, TS_LEFT, flags);
	}
}
//----------------------------------------------------------------------------------
WPARAM CEntryText::GetLastChar()
{
	if (Text.length() == 0) return 0;
	return Text[Text.length() - 1];
}
//----------------------------------------------------------------------------------
void CEntryText::RemoveSequence(int startPos, int length)
{
	Text.erase(startPos, length);
}
//----------------------------------------------------------------------------------
string CEntryText::GetTextA() const
{
	return m_CText;
}
//----------------------------------------------------------------------------------
wstring CEntryText::GetTextW() const
{
	return Text;
}
//----------------------------------------------------------------------------------
