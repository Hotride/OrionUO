/****************************************************************************
**
** SpellbookGump.cpp
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
//----------------------------------------------------------------------------
#include "stdafx.h"

TTextTexture TGumpSpellbook::m_TextIndex;
TTextTexture TGumpSpellbook::m_TextReagents;
TTextTexture TGumpSpellbook::m_TextCircle[8];
TTextTexture TGumpSpellbook::m_TextReagent[8];
TTextTexture TGumpSpellbook::m_TextSpell[64];
//----------------------------------------------------------------------------
WORD TGumpSpellbook::m_ReagentsIndex[8] = 
{
	0x0F7A, 0x0F7B, 0x0F84, 0x0F85,
	0x0F86, 0x0F88, 0x0F8C, 0x0F8D
};
//----------------------------------------------------------------------------
TGumpSpellbook::TGumpSpellbook(DWORD serial, short x, short y)
: TGump(GT_SPELLBOOK, serial, x, y), m_Page(1)
{
}
//----------------------------------------------------------------------------
TGumpSpellbook::~TGumpSpellbook()
{
}
//----------------------------------------------------------------------------
int TGumpSpellbook::GetReagentIndex(WORD &id)
{
	switch (id)
	{
		case 0x0F7A:
			return 0;
		case 0x0F7B:
			return 1;
		case 0x0F84:
			return 2;
		case 0x0F85:
			return 3;
		case 0x0F86:
			return 4;
		case 0x0F88:
			return 5;
		case 0x0F8C:
			return 6;
		case 0x0F8D:
			return 7;
		default:
			break;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpSpellbook::InitTextTextures()
{
	FontManager->GenerateA(6, m_TextIndex, "INDEX", 0x0288);

	FontManager->GenerateA(6, m_TextReagents, "Reagents:", 0x0288);

	string str = "";

	IFOR(i, 0, 8)
	{
		FontManager->GenerateA(6, m_TextCircle[i], SPELL_CIRCLES_NAMES[i], 0x0288);
		
		str = GetReagentName(m_ReagentsIndex[i]);
		FontManager->GenerateA(9, m_TextReagent[i], str.c_str(), 0x0288);
	}

	IFOR(i, 0, 64)
	{
		WORD id = 0x2080 + i;
		str = UO->m_StaticData[id / 32].Tiles[id % 32].Name;
		FontManager->GenerateA(9, m_TextSpell[i], str.c_str(), 0x0288);
	}
}
//----------------------------------------------------------------------------
void TGumpSpellbook::ReleaseTextTextures()
{
	m_TextIndex.Clear();

	IFOR(i, 0, 8)
	{
		m_TextCircle[i].Clear();
		m_TextReagent[i].Clear();
	}

	IFOR(i, 0, 64)
		m_TextSpell[i].Clear();
}
//----------------------------------------------------------------------------
void TGumpSpellbook::PrepareTextures()
{
	UO->ExecuteGump(0x0835);
	UO->ExecuteGumpPart(0x08AC, 13);
	UO->ExecuteGumpPart(0x08B1, 8);
	UO->ExecuteGumpPart(0x08BB, 2);
	UO->ExecuteGumpPart(0x08C0, 64);
}
//----------------------------------------------------------------------------
void TGumpSpellbook::GenerateFrame()
{
	if (!g_DrawMode)
	{
		m_FrameRedraw = false;
		m_FrameCreated = false;

		return;
	}

	TGameItem *spellbook = World->FindWorldItem(m_Serial);
	if (spellbook == NULL)
		return; //Если нет спеллбуки - уходим от сюда

	CalculateGumpState();

	if (m_Minimized) //Свернута с маленькую книжечку
	{
		glNewList((GLuint)this, GL_COMPILE);

			UO->DrawGump(0x08BA, 0, 0, 0); //Мини-гамп спеллбуки

		glEndList();

		m_FrameCreated = true;

		return; //Нам больше нечего отрисовывать
	}

	int spellCount = 0;

	BYTE spells[64] = {0}; //Указатели на заклинания
	spellbook = (TGameItem*)spellbook->m_Items; //С чего начать поиск

	while (spellbook != NULL) //Пройдемся по всем элементам спеллбуки
	{
		int currentCount = spellbook->Count;

		if (currentCount > 0 && currentCount <= 64) //Валидный индекс заклинания
		{
			spells[currentCount - 1] = 1; //Обнаружено, учтем это
			spellCount++; //Увеличиваем счетчик заклинаний
		}

		spellbook = (TGameItem*)spellbook->m_Next;
	}

	int pageCount = 4 + (spellCount / 2); //Вычисляем количество страниц книги
	if (spellCount % 2)
		pageCount++; //Если нечетное - увеличиваем до четного (последняя будет пустой)

	if (m_Page < 1)
		m_Page = 1; //На всякий случай исправим текущую страницу

	glNewList((GLuint)this, GL_COMPILE);

		UO->DrawGump(0x08AC, 0, 0, 0); //Гамп книги

		if (m_Page != 1)
			UO->DrawGump(0x08BB, 0, 50, 8); //Уголок "Назад"

		if (m_Page != pageCount)
			UO->DrawGump(0x08BC, 0, 321, 8); //Уголок "Вперед"

		//Круги 1-4 (в кружочках внизу)
		UO->DrawGump(0x08B1, 0, 58, 175);
		UO->DrawGump(0x08B2, 0, 93, 175);
		UO->DrawGump(0x08B3, 0, 130, 175);
		UO->DrawGump(0x08B4, 0, 164, 175);
		
		//Круги 5-8 (в кружочках внизу)
		UO->DrawGump(0x08B5, 0, 227, 175);
		UO->DrawGump(0x08B6, 0, 260, 175);
		UO->DrawGump(0x08B7, 0, 297, 175);
		UO->DrawGump(0x08B8, 0, 332, 175);
			
		if (m_Page <= 4) //Оглавление книги
		{
			m_TextIndex.Draw(106, 10);
			m_TextIndex.Draw(269, 10);

			int pageIdx = (m_Page - 1) * 2;

			m_TextCircle[pageIdx].Draw(62, 30);
			m_TextCircle[pageIdx + 1].Draw(225, 30);

			//Разрисуем названия спеллов
			for (int i = 0, si1 = 0, si2 = 0; i < 8; i++)
			{
				int offs = i + ((8 * (m_Page - 1)) * 2);

				//Для левой страницы
				if (spells[offs])
				{
					m_TextSpell[offs].Draw(62, 52 + (si1 * 15));

					//Если совпало с последним использованным заклинанием - отобразим закладку
					if (offs == g_LastSpellIndex - 1)
					{
						UO->DrawGump(0x08AD, 0, 184, 2);
						UO->DrawGump(0x08AF, 0, 184, 52 + (si1 * 15));
					}

					si1++;
				}

				offs = 8 + i + ((8 * (m_Page - 1)) * 2);

				//Для правой страницы
				if (spells[offs])
				{
					m_TextSpell[offs].Draw(225, 52 + (si2 * 15));

					//Если совпало с последним использованным заклинанием - отобразим закладку
					if (offs == g_LastSpellIndex - 1)
					{
						UO->DrawGump(0x08AE, 0, 203, 3);
						UO->DrawGump(0x08B0, 0, 203, 52 + (si2 * 15));
					}

					si2++;
				}
			}
		}
		else //Страницы с подробностями заклинаний
		{
			//Вычислим, сколько заклинаний будет (1 или 2)
			bool oneSpell = ((m_Page == pageCount) && (spellCount % 2));

			int idx = ((m_Page - 5) * 2); //Вычисляем индекс
			if (oneSpell)
				idx++; //Если 1 спелла - фиксим индекс

			int SI = 0;
			int c = 0;

			for (SI = 0, c = 0; SI < 64; SI++)
			{
				if (spells[SI] != 0)
					c++; //Если спеллка есть - увеличиваем счетчик

				if (oneSpell && c == idx)
					break; //Если всего 1 спелл и это искомый индекс - выходим из цикла

				if (c == idx + 1)
					break; //Если счетчик установлен на позиции индекс + 1 - выходим
			}

			if (oneSpell) //Если 1 спелл
			{
				if (c != idx)
				{
					glEndList();

					m_FrameCreated = true;

					return; //Если индексы не совпадают - ошибка, выходим
				}
			}
			else if (c != idx + 1)
			{
				glEndList();

				m_FrameCreated = true;

				return; //Или же, если счетчик не равен инжекс + 1 - ошибка, выходим
			}

			if (SI == 64)
			{
				glEndList();

				m_FrameCreated = true;

				return; //Ошибка. Достигли конца списка заклинаний, выходим
			}

			//Название круга
			m_TextCircle[(SI / 8)].Draw(87, 10);

			//Иконка заклинания
			UO->DrawGump(0x08C0 + SI, 0, 62, 40);

			WORD id = (0x2080 + SI); //Для названия заклинания
			TTextTexture th;
			FontManager->GenerateA(6, th, UO->m_StaticData[id / 32].Tiles[id % 32].Name, 0x0288, 80);
			th.Draw(112, 34);

			//Аббревиатура заклинания
			int addY = 26;
			if (th.Height < 24)
				addY = 31;
			FontManager->DrawA(8, SPELL_CAST_ABBREVIATURE[SI], 0x0288, 112, addY + th.Height);
			th.Clear();

			//Полоска
			UO->DrawGump(0x0835, 0, 62, 88, 120, 0);

			m_TextReagents.Draw(62, 92);

			IFOR(i, 0, 4) //Отобразим реагенты, необходимые на каст
			{
				id = SPELL_REAGENTS[SI][i];

				if (!id)
					break; //Если рега нет - выходим, дальше ничего не будет

				m_TextReagent[GetReagentIndex(id)].Draw(62, 114 + (i * 14));
			}

			//Если это последнее скастованное заклинание - отображаем закладку
			if (SI == g_LastSpellIndex - 1)
			{
				UO->DrawGump(0x08AD, 0, 184, 2);
				UO->DrawGump(0x08AF, 0, 184, 40);
			}

			//Переходим на следующее заклинание
			SI++;

			for (; SI < 64; SI++) //Пройдемся по оставшимся заклинаниям
			{
				if (spells[SI] != 0)
					break; //Если что-то нашлось раньше окончания книги - выходим из цикла
			}

			if (SI == 64)
			{
				glEndList();

				m_FrameCreated = true;

				return; //Заклинаний больше нет, конец книги достигнут, уходим
			}

			//Название круга
			m_TextCircle[(SI / 8)].Draw(244, 10);

			//Иконка заклинания
			UO->DrawGump(0x08C0 + SI, 0, 225, 40);

			id = (0x2080 + SI); //Для названия заклинания
			FontManager->GenerateA(6, th, UO->m_StaticData[id / 32].Tiles[id % 32].Name, 0x0288, 80);
			th.Draw(275, 34);

			//Аббревиатура заклинания
			addY = 26;
			if (th.Height < 24)
				addY = 31;
			FontManager->DrawA(8, SPELL_CAST_ABBREVIATURE[SI], 0x0288, 275, addY + th.Height);
			th.Clear();

			//Полоска
			UO->DrawGump(0x0835, 0, 225, 88, 120, 0);

			m_TextReagents.Draw(225, 92);

			IFOR(i, 0, 4) //Пройдемся по всем реагентам
			{
				id = SPELL_REAGENTS[SI][i];

				if (!id)
					break; //Если рега нет - выходим, дальше ничего не будет

				m_TextReagent[GetReagentIndex(id)].Draw(225, 114 + (i * 14));
			}

			//Если это последнее скастованное заклинание - отображаем закладку
			if (SI == g_LastSpellIndex - 1)
			{
				UO->DrawGump(0x08AE, 0, 203, 3);
				UO->DrawGump(0x08B0, 0, 203, 40);
			}
		}

	glEndList();

	m_FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpSpellbook::Draw(bool &mode)
{
	TGameItem *spellbook = World->FindWorldItem(m_Serial);
	if (spellbook == NULL)
		return 0; //Если нет спеллбуки - уходим от сюда

	//ИД гампа внутри клиента
	DWORD index = (DWORD)this;

	CalculateGumpState();

	//Нужно вытащить иконку заклинания
	if (g_GumpPressed && m_Page > 4 && g_LastObjectLeftMouseDown >= ID_GSB_SPELL_ICON_LEFT && ((g_MouseX != g_DroppedLeftMouseX || g_MouseY != g_DroppedLeftMouseY) || (g_LastGumpMouseDownTime + DCLICK_DELAY < GetTickCount())))
	{
		//Создадим новый объект гампа, вычисляем его индекс заклинания
		if (g_LastObjectLeftMouseDown < ID_GSB_SPELL_ICON_RIGHT)
			g_LastObjectLeftMouseDown -= ID_GSB_SPELL_ICON_LEFT;
		else
			g_LastObjectLeftMouseDown -= ID_GSB_SPELL_ICON_RIGHT;

		TGumpSpell *newgump = new TGumpSpell(g_LastObjectLeftMouseDown + 1, g_MouseX - 20, g_MouseY - 20, 0x08C0 + g_LastObjectLeftMouseDown);

		//Гамп пора добавлять в клиент
		GumpManager->AddGump(newgump);

		//Обновим информацию о объекте под курсором и о захваченном мышкой гампе
		CurrentScreen->OnLeftMouseDown();
	}

	if (mode) //Рисование
	{
		//Если список отображения не создан
		if (!m_FrameCreated)
			GenerateFrame();

		glTranslatef(g_GumpTranslateX, g_GumpTranslateY, 0.0f);

		//Отрисуем гамп
		glCallList((GLuint)index);

		DrawLocker();

		glTranslatef(-g_GumpTranslateX, -g_GumpTranslateY, 0.0f);
	}
	else //Выбор объектов
	{
		int oldMouseX = g_MouseX;
		int oldMouseY = g_MouseY;
		g_MouseX -= (int)g_GumpTranslateX;
		g_MouseY -= (int)g_GumpTranslateY;

		if (m_Minimized) //Если это минимизированная версия гампа
		{
			if (UO->GumpPixelsInXY(0x08BA, 0, 0))
			{
				g_LastSelectedObject = 0;
				g_LastSelectedGump = index;
			}

			g_MouseX = oldMouseX;
			g_MouseY = oldMouseY;

			return 0;
		}

		int spellCount = 0;

		//Список актуальных заклинаний
		BYTE spells[64] = {0};

		//Указатель на заклинания
		spellbook = (TGameItem*)spellbook->m_Items;

		//Пройдемся по всем заклинаниям, вычислим какие есть
		while (spellbook != NULL)
		{
			int currentCount = spellbook->Count;
			if (currentCount > 0 && currentCount <= 64)
			{
				//Отметим найденное заклинание
				spells[currentCount - 1] = 1;
				spellCount++;
			}

			spellbook = (TGameItem*)spellbook->m_Next; //Следующий элемент книги
		}

		int pageCount = 4 + (spellCount / 2); //Вычисляем количество страниц книги
		if (spellCount % 2)
			pageCount++; //Фиксим количество (если нечетное)

		if (m_Page < 1)
			m_Page = 1; //Фиксим текущую страницу

		//Гамп спеллбуки
		if (UO->GumpPixelsInXY(0x08AC, 0, 0))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}

		//Мышка не над гампом, дальнейшие проверки не актуальны. Уходим от сюда
		if (g_LastSelectedGump != index)
		{
			g_MouseX = oldMouseX;
			g_MouseY = oldMouseY;

			return 0;
		}

		int LSG = 0;

		//Если имеются предыдущие страницы
		if (m_Page != 1)
		{
			if (UO->GumpPixelsInXY(0x08BB, 50, 8))
				LSG = ID_GSB_BUTTON_PREV; //Prev
		}

		//Если имеются следующие страницы
		if (m_Page != pageCount)
		{
			if (UO->GumpPixelsInXY(0x08BC, 321, 8))
				LSG = ID_GSB_BUTTON_NEXT; //Next
		}

		//Круги 1-4 (в кружочках внизу)
		if (UO->GumpPixelsInXY(0x08B1, 58, 175))
			LSG = ID_GSB_BUTTON_CIRCLE_1_2;
		else if (UO->GumpPixelsInXY(0x08B2, 93, 175))
			LSG = ID_GSB_BUTTON_CIRCLE_1_2;
		else if (UO->GumpPixelsInXY(0x08B3, 130, 175))
			LSG = ID_GSB_BUTTON_CIRCLE_3_4;
		else if (UO->GumpPixelsInXY(0x08B4, 164, 175))
			LSG = ID_GSB_BUTTON_CIRCLE_3_4;
		
		//Круги 5-8 (в кружочках внизу)
		else if (UO->GumpPixelsInXY(0x08B5, 227, 175))
			LSG = ID_GSB_BUTTON_CIRCLE_5_6;
		else if (UO->GumpPixelsInXY(0x08B6, 260, 175))
			LSG = ID_GSB_BUTTON_CIRCLE_5_6;
		else if (UO->GumpPixelsInXY(0x08B7, 297, 175))
			LSG = ID_GSB_BUTTON_CIRCLE_7_8;
		else if (UO->GumpPixelsInXY(0x08B8, 332, 175))
			LSG = ID_GSB_BUTTON_CIRCLE_7_8;

		//Кнопка минимизации гампа
		if (UO->PolygonePixelsInXY(6, 100, 16, 16))
			LSG = ID_GSB_BUTTON_MINIMIZE;

		if (m_Page <= 4) //Если это оглавление книги
		{
			RECT rc = {0, 0, 100, 16};
			for (int i = 0, si1 = 0, si2 = 0; i < 8; i++)
			{
				int offs = i + ((8 * (m_Page - 1)) * 2);

				if (spells[offs]) //Заклинание левой страницы
				{
					if (UO->PolygonePixelsInXY(62, 52 + (si1 * 15), 100, 16))
					{
						LSG = offs + ID_GSB_SPELL_ICON_LEFT;
						break;
					}

					si1++;
				}

				offs = 8 + i + ((8 * (m_Page - 1)) * 2);

				if (spells[offs]) //Заклинание правой страницы
				{
					if (UO->PolygonePixelsInXY(225, 52 + (si2 * 15), 100, 16))
					{
						LSG = offs + ID_GSB_SPELL_ICON_RIGHT;
						break;
					}

					si2++;
				}
			}
		}
		else //Страницы с деталировкой заклинаний
		{
			//Будет отображено только одно заклинание
			bool oneSpell = ((m_Page == pageCount) && (spellCount % 2));

			//Вычисляем текущий индекс заклинания
			int idx = ((m_Page - 5) * 2);
			if (oneSpell)
				idx++; //Если будет отображено одно заклинание - фиксим

			int SI = 0;
			int c = 0;

			//Пробежимся по всем заклинаниям
			for (SI = 0, c = 0; SI < 64; SI++)
			{
				if (spells[SI] != 0)
					c++; //Если оно существует - увеличиваем счетчик

				if (oneSpell && c == idx)
					break; //Если отображается одно заклинание и индекс со счетчиком совпадают - выходим из цикла

				if (c == idx + 1)
					break; //Или же, если индекс + 1 равен счетчику - выходим из цикла
			}

			if (oneSpell) //Если одно заклинание
			{
				//Если счетчик не равен индексу - ошибка, выходим с сохранением объекта под курсором
				if (c != idx)
				{
					g_MouseX = oldMouseX;
					g_MouseY = oldMouseY;

					return LSG;
				}
			}
			else if (c != idx + 1)
			{
				g_MouseX = oldMouseX;
				g_MouseY = oldMouseY;

				return LSG; //Error...
			}

			if (SI == 64)
			{
				g_MouseX = oldMouseX;
				g_MouseY = oldMouseY;

				return LSG; //Error...
			}

			//Проверяем гамп иконки заклинания
			if (UO->GumpPixelsInXY(0x08C0 + SI, 62, 40))
				LSG = ID_GSB_SPELL_ICON_LEFT + SI;

			//Увеличиваем индекс заклинания
			SI++;

			for (; SI < 64; SI++) //Пробежимся по всем заклинаниям
			{
				if (spells[SI] != 0)
					break; //Если что-то нашлось - выходим из цикла
			}

			if (SI == 64)
			{
				g_MouseX = oldMouseX;
				g_MouseY = oldMouseY;

				return LSG; //End of spellbook
			}

			//Проверим гамп иконки заклинания
			if (UO->GumpPixelsInXY(0x08C0 + SI, 225, 40))
				LSG = ID_GSB_SPELL_ICON_RIGHT + SI;
		}

		if (g_ShowGumpLocker && UO->PolygonePixelsInXY(0, 0, 10, 14))
			LSG = ID_GSB_LOCK_MOVING;

		g_MouseX = oldMouseX;
		g_MouseY = oldMouseY;

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpSpellbook::OnLeftMouseUp()
{
	//Проверим валидность переменных
	if (g_LastObjectLeftMouseDown != g_LastSelectedObject || !g_LastObjectLeftMouseDown || Minimized || !g_LastSelectedGump)
		return;

	//Сохраним текущую страницу в буффер
	int lastPage = m_Page;

	if (g_LastObjectLeftMouseDown == ID_GSB_BUTTON_PREV) //Prev
	{
		//Ткнули по уголку "назад"

		//gump->SetPage(gump->GetPage() - 1);
		if (!g_ClickObjectReq) //Если не было запроса на клик
		{
			//Активируем запрос на клик
			g_ClickObjectReq = true;

			//Инициализируем клик-буффер
			g_ClickObject.Init(COT_GUMP);
			g_ClickObject.GumpType = m_GumpType;
			g_ClickObject.Serial = m_Serial;
			g_ClickObject.GumpID = m_ID;
			g_ClickObject.GumpButtonID = m_Page - 1;

			//Задаем время до выполнения
			g_ClickObject.Timer = GetTickCount() + DCLICK_DELAY;
		}
	}
	else if (g_LastObjectLeftMouseDown == ID_GSB_BUTTON_NEXT) //Next
	{
		//Ткнули по уголку "назад"

		//gump->SetPage(gump->GetPage() + 1);
		if (!g_ClickObjectReq) //Если не было запроса на клик
		{
			//Активируем запрос на клик
			g_ClickObjectReq = true;

			//Инициализируем клик-буффер
			g_ClickObject.Init(COT_GUMP);
			g_ClickObject.GumpType = m_GumpType;
			g_ClickObject.Serial = m_Serial;
			g_ClickObject.GumpID = m_ID;
			g_ClickObject.GumpButtonID = m_Page + 1;

			//Задаем время до выполнения
			g_ClickObject.Timer = GetTickCount() + DCLICK_DELAY;
		}
	}
	else if (g_LastObjectLeftMouseDown == ID_GSB_BUTTON_CIRCLE_1_2)
		m_Page = 1; //Переход к оглавлению на круги 1-2
	else if (g_LastObjectLeftMouseDown == ID_GSB_BUTTON_CIRCLE_3_4)
		m_Page = 2; //Переход к оглавлению на круги 3-4
	else if (g_LastObjectLeftMouseDown == ID_GSB_BUTTON_CIRCLE_5_6)
		m_Page = 3; //Переход к оглавлению на круги 5-6
	else if (g_LastObjectLeftMouseDown == ID_GSB_BUTTON_CIRCLE_7_8)
		m_Page = 4; //Переход к оглавлению на круги 7-8
	else if (g_LastObjectLeftMouseDown == ID_GSB_BUTTON_MINIMIZE) //Свернуть гамп
	{
		m_Minimized = true;
		m_FrameCreated = false;
	}
	else if (g_LastObjectLeftMouseDown == ID_GSB_LOCK_MOVING)
	{
		LockMoving = !LockMoving;
		g_CancelDoubleClick = true;
	}
	else if (g_LastObjectLeftMouseDown >= ID_GSB_SPELL_ICON_LEFT)
	{
		//Было выбрано заклинание. Стандартные процедуры работы со спеллбукой, описанные выше

		TGameItem *spellbook = World->FindWorldItem(m_Serial);
		if (spellbook == NULL)
			return;

		int spellCount = 0;

		BYTE spells[64] = {0};
		spellbook = (TGameItem*)spellbook->m_Items;

		while (spellbook != NULL)
		{
			int currentCount = spellbook->Count;

			if (currentCount > 0 && currentCount <= 64)
			{
				spells[currentCount - 1] = 1;
				spellCount++;
			}

			spellbook = (TGameItem*)spellbook->m_Next;
		}

		int pageCount = 4 + (spellCount / 2);
		if (spellCount % 2)
			pageCount++;

		if (m_Page < 1)
			m_Page = 1;

		if (m_Page <= 4) //List of spells
		{
			for (int i = 0, si1 = 0, si2 = 0; i < 8; i++)
			{
				int offs = i + ((8 * (m_Page - 1)) * 2);
				
				if (spells[offs])
				{
					if (g_LastObjectLeftMouseDown == offs + ID_GSB_SPELL_ICON_LEFT)
					{
						//gump->SetPage(5);

						int c = 0;

						for (int j = 0; j < 64; j++)
						{
							if (spells[j])
							{
								c++;

								if (j == offs)
									break;
							}
						}

						if (!g_ClickObjectReq)
						{
							g_ClickObjectReq = true;

							g_ClickObject.Init(COT_GUMP);
							g_ClickObject.GumpType = m_GumpType;
							g_ClickObject.Serial = m_Serial;
							g_ClickObject.GumpID = m_ID;
							g_ClickObject.GumpButtonID = 5 + ((c - 1) / 2);
							g_ClickObject.Timer = GetTickCount() + DCLICK_DELAY;
						}

						//gump->SetPage(5 + ((c - 1) / 2));

						return;
					}
					si1++;
				}

				offs = 8 + i + ((8 * (m_Page - 1)) * 2);

				if (spells[offs])
				{
					if (g_LastObjectLeftMouseDown == offs + ID_GSB_SPELL_ICON_RIGHT)
					{
						//gump->SetPage(5);

						int c = 0;

						for (int j = 0; j < 64; j++)
						{
							if (spells[j])
							{
								c++;

								if (j == offs)
									break;
							}
						}

						if (!g_ClickObjectReq)
						{
							g_ClickObjectReq = true;

							g_ClickObject.Init(COT_GUMP);
							g_ClickObject.GumpType = m_GumpType;
							g_ClickObject.Serial = m_Serial;
							g_ClickObject.GumpID = m_ID;
							g_ClickObject.GumpButtonID = 5 + ((c - 1) / 2);
							g_ClickObject.Timer = GetTickCount() + DCLICK_DELAY;
						}

						//gump->SetPage(5 + ((c - 1) / 2));

						return;
					}
					si2++;
				}
			}
		}
	}

	//Если была изменена страница - перерисуем гамп
	if (lastPage != m_Page)
		m_FrameCreated = false;
}
//----------------------------------------------------------------------------
bool TGumpSpellbook::OnLeftMouseDoubleClick()
{
	//Если ничего выбрано не было - уходим от сюда
	if (!g_LastSelectedGump)
		return false;

	if (!g_LastObjectLeftMouseDown && m_Minimized)
	{
		//Если гамп минимизирован - восстанавливаем его нормальный вид
		m_Minimized = false;
		m_FrameCreated = false;

		return true;
	}
	else if (!m_Minimized && g_LastObjectLeftMouseDown)
	{
		//Если не минимизирован и что-то выбрано

		TGameItem *spellbook = World->FindWorldItem(m_Serial);
		if (spellbook == NULL)
			return false; //Книга отсутствует, проверять нечего, уходим

		int spellCount = 0;

		//Стандартные процедуры, описанные в функциях выше
		BYTE spells[64] = {0};
		spellbook = (TGameItem*)spellbook->m_Items;

		while (spellbook != NULL)
		{
			int currentCount = spellbook->Count;

			if (currentCount > 0 && currentCount <= 64)
			{
				spells[currentCount - 1] = 1;
				spellCount++;
			}

			spellbook = (TGameItem*)spellbook->m_Next;
		}

		int pageCount = 4 + (spellCount / 2);
		if (spellCount % 2)
			pageCount++;

		if (m_Page < 1)
			m_Page = 1;

		if (g_LastObjectLeftMouseDown == ID_GSB_BUTTON_PREV) //Prev
		{
			//Был нажат уголок "Назад", при даблклике устанавливаем 1 страницу
			m_Page = 1;

			//Перерисуем гамп
			m_FrameCreated = false;
			UO->PlaySoundEffect(0x0055);

			return true;
		}
		else if (g_LastObjectLeftMouseDown == ID_GSB_BUTTON_NEXT) //Next
		{
			//Был нажат уголок "Вперед", при даблклике устанавливаем последнюю страницу
			m_Page = pageCount;

			//Перерисуем гамп
			m_FrameCreated = false;
			UO->PlaySoundEffect(0x0055);

			return true;
		}
		else if (g_LastObjectLeftMouseDown >= ID_GSB_SPELL_ICON_LEFT)
		{
			//Было использовано заклинание
			if (g_LastObjectLeftMouseDown < ID_GSB_SPELL_ICON_RIGHT)
				UO->CastSpellFromBook(g_LastObjectLeftMouseDown - ID_GSB_SPELL_ICON_LEFT + 1, m_Serial);
			else
				UO->CastSpellFromBook(g_LastObjectLeftMouseDown - ID_GSB_SPELL_ICON_RIGHT + 1, m_Serial);

			//Сворачиваем книгу
			m_Minimized = true;
			m_FrameCreated = false;

			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------