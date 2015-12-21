/****************************************************************************
**
** GumpSkills.cpp
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
//----------------------------------------------------------------------------
TGumpSkills::TGumpSkills(DWORD serial, short x, short y, bool minimized)
: TGump(GT_SKILLS, serial, x, y), m_Height(200), m_LastResizeY(0),
m_ShowReal(false), m_ShowCap(false), m_LastChangedLineTime(0), m_CurrentLine(0)
{
	//Создание указателя для ввода текста
	TextEntry = new TEntryText();

	//Заготовка константной текстуры
	FontManager->GenerateA(1, m_Text, "Show:   Real    Cap", 0x0386);

	if (minimized)
	{
		Minimized = minimized;
		MinimizedX = x;
		MinimizedY = y;
	}
}
//----------------------------------------------------------------------------
TGumpSkills::~TGumpSkills()
{
	//При закрытии гампа проверим указатель на ввод текста
	if (TextEntry == EntryPointer)
	{
		//Если указатель в гампе - изменим его
		if (ConfigManager.GetConsoleNeedEnter())
			EntryPointer = NULL;
		else
			EntryPointer = GameConsole;
	}

	//Удаляем указатель на ввод текста
	if (TextEntry != NULL)
	{
		delete TextEntry;
		TextEntry = NULL;
	}

	//Почистим текстуру
	m_Text.Clear();
}
//----------------------------------------------------------------------------
void TGumpSkills::Init()
{
	//Свернем все доступные группы
	TSkillGroupObject *group = SkillGroupManager.m_Groups;
	while (group != NULL)
	{
		group->Maximized = false;

		group = group->m_Next;
	}
}
//----------------------------------------------------------------------------
void TGumpSkills::SetHeight(short val)
{
	//Установка и корректировка значения высоты гампа
	if (val < 120)
		val = 120;

	int buf = (GetSystemMetrics(SM_CYSCREEN) - 50);
	if (val >= buf)
		val = buf;

	m_Height = val;
}
//----------------------------------------------------------------------------
void TGumpSkills::ChangeHeight()
{
	//Событие изменения высоты гампа с вложенной корректировкой
	if (m_LastResizeY)
		SetHeight(m_Height + (g_MouseY - m_LastResizeY));

	m_LastResizeY = 0;
	g_ResizedGump = NULL;
}
//---------------------------------------------------------------------------
void TGumpSkills::PrepareTextures()
{
	UO->ExecuteGump(0x1F40); //Top scroll
	UO->ExecuteGump(0x1F43); //Bottom scroll
	UO->ExecuteGumpPart(0x0938, 2); //Real/Cap buttons
	UO->ExecuteGumpPart(0x1F41, 2); //Body
	UO->ExecuteGump(0x0984);
	UO->ExecuteGump(0x0986);
	UO->ExecuteGumpPart(0x0824, 12);
	UO->ExecuteGumpPart(0x0834, 7);
}
//---------------------------------------------------------------------------
void TGumpSkills::GenerateFrame(int posX, int posY)
{
	if (!g_DrawMode)
	{
		FrameRedraw = false;
		FrameCreated = false;

		return;
	}

	//Индекс гампа
	DWORD index = (DWORD)this;

	//Нажата ли кнопка в окне?
	bool IsScrollerPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index);

	//Нажата ли кнопка в окне?
	bool IsPressed = (IsScrollerPressed && g_LastSelectedGump == index);

	//Может ли быть подсвечен элемент?
	int CanSelectedButton = ((g_LastSelectedGump == index) ? g_LastSelectedObject : 0);

	//Может ли быть нажат элемент?
	int CanPressedButton = 0;
	if (IsPressed && g_LastObjectLeftMouseDown == g_LastSelectedObject)
		CanPressedButton = g_LastObjectLeftMouseDown;
	
	int height = m_Height; //Высота для быстрого доступа и безобезненной временной корректировки

	if (m_LastResizeY) //Если активировано изменение высоты гампа
	{
		//Подкорректируем временное значение высоты
		height += (g_MouseY - m_LastResizeY);
		if (height < 120)
			height = 120;

		int bh = (GetSystemMetrics(SM_CYSCREEN) - 50);
		if (height >= bh)
			height = bh;
	}

	//Стандартная позиция скроллера (posY не нужно учитывать)
	int scrollerY = 0;

	//Если гамп не свернут (впринципе проверка выше это уже делает) то расчитаем позицию скроллера
	if (!Minimized)
	{
		//Максимальное количество визимых строк
		int visibleLines = SkillGroupManager.GetVisibleLinesCount();

		//Максимально возможное значение позиции скроллера
		int maxScrollerY = height - 104;

		//Если скроллер захвачен мышкой
		if (IsScrollerPressed && g_LastObjectLeftMouseDown == ID_GS_SCROLLER)
		{
			//Вычисляем текущую позицию скроллера
			int currentY = (g_MouseY - 10) - posY - 72;

			scrollerY = CalculateScrollerAndTextPosition(m_CurrentLine, visibleLines, maxScrollerY, currentY);
		}
		else if (m_CurrentLine) //Или же, стартовая позиция видимых линий смещена
			scrollerY = CalculateScrollerY(m_CurrentLine, visibleLines, maxScrollerY);
	}

	//Создание дисплей листа
	glNewList((GLuint)index, GL_COMPILE);

		if (Minimized) //Если это свернутый гамп
		{
			//Мини-гамп скиллов
			UO->DrawGump(0x0839, 0, posX, posY);

			//Завершим создание листа и выйдем из процедуры
			glEndList();

			FrameRedraw = true;
			FrameCreated = true;

			return;
		}

		//Нормальное состояние гампа

		UO->DrawGump(0x082D, 0, posX + 167, posY); //Minimize
		
		UO->DrawGump(0x1F40, 0, posX, posY + 23); //Top scroll
		UO->DrawGump(0x0834, 0, posX + 82, posY + 34); //Skills text gump
		UO->DrawGump(0x0938 + (int)m_ShowReal, 0, posX + 226, posY + 34); //Real
		UO->DrawGump(0x0938 + (int)m_ShowCap, 0, posX + 280, posY + 34); //Cap
		//UO->DrawFont(1, "Show:   Real    Cap", 0x0386, posX + 180, posY + 33);
		m_Text.Draw(posX + 180, posY + 33);

		//Skills body
		int curposY = posY + 59;

		//Ниже вычисление высоты гамп-блоков и отрисовка тела гампа
		while (true)
		{
			int deltaHeight = (height + posY) - (curposY - 36);

			if (deltaHeight  < 70)
			{
				if (deltaHeight > 0)
					UO->DrawGump(0x1F41, 0, posX + 21, curposY, 0, deltaHeight);

				break;
			}
			else
				UO->DrawGump(0x1F41, 0, posX + 21, curposY);

			curposY += 70;

			deltaHeight = (height + posY) - (curposY - 36);

			if (deltaHeight < 70)
			{
				if (deltaHeight > 0)
					UO->DrawGump(0x1F42, 0, posX + 21, curposY, 0, deltaHeight);

				break;
			}
			else
				UO->DrawGump(0x1F42, 0, posX + 21, curposY);

			curposY += 70;
		}

		//Up
		//UO->DrawGump(0x0828, 0, posX + 296, posY + 72); //Scroller
		//Down
		//UO->DrawGump(0x0828, 0, posX + 296, posY + height - 32); //Scroller

		UO->DrawGump(0x0828, 0, posX + 296, posY + 72 + scrollerY); //Scroller

		//Индекс строки для начала отображения
		int startIndex = m_CurrentLine;
		int currentIndex = 0; //Буффер индексов
		
		int drawX = posX + 30;
		int drawY = posY + 72;

		//Стандартный цвет выделенной группы/скилла
		DWORD selectedPolygoneColor = 0x007F7F7F;

		//g_GL.DrawPolygone(0x007F7F7F, drawX, drawY, 260, height - 74);

		//Установим видимую область
		g_GL.ViewPort(drawX, drawY, 264, height - 74);

		//Расстояние между строками
		const int drawStep = 17;

		//Стартовые координаты начала отображения
		drawX -= 2;
		drawY += 3;

		//Максимальная координата отображения по оси Y
		int boundsY = posY + height;
		
		int si = -1; //Индекс выбранного скилла

		TSkillGroupObject *selectedGroup = NULL; //Указатель на группу, в которой выбран скилл
		bool removeIndexOnExit = false; //Флаг удаления скилла при выходе из процедуры

		//Если захвачен скилл
		if (g_LastObjectLeftMouseDown >= ID_GS_SKILL && g_LastObjectLeftMouseDown < 14000)
		{
			//Вычислим индекс скилла
			si = g_LastObjectLeftMouseDown - ID_GS_SKILL;

			//Получим выбранную группу
			selectedGroup = GetGroupUnderCursor(m_CurrentLine);

			//Если группа нашлась и в ней нет текущего выбранного скилла
			//то добавим его туда (отсортировав) и установим флаг удаления при выходе
			if (selectedGroup != NULL && !selectedGroup->Contains(si))
			{
				selectedGroup->AddSorted(si);
				removeIndexOnExit = true;
			}
		}

		//Если игрок присутствует
		if (g_Player != NULL)
		{
			PSKILLS_DATA Skills = g_Skills; //Получаем указатель на доступные навыки

			TSkillGroupObject *group = SkillGroupManager.m_Groups; //Указатель на группы

			while (group != NULL)
			{
				if (drawY + 10 >= boundsY)
					break; //Если вышле за возможные пределы отрисовки

				//Возможно ли отрисовать элемент?
				bool canDraw = (currentIndex >= startIndex);
				currentIndex++;

				if (canDraw) //Разрешено рисовать
				{
					//Получаем указатель на текстуру названия группы
					TTextTexture &th = group->m_Texture;

					//Текстура есть
					if (!th.Empty())
					{
						//Стэйт выбора имени группы
						BYTE selname = group->GetSelected();

						if (selname == 2) //Изменение имени группы
						{
							//Отобразим цветной фон
							g_GL.DrawPolygone(selectedPolygoneColor, (float)drawX + 16.0f, (float)drawY, 200.0f, 14.0f);

							//Текущее название
							TextEntry->DrawA(6, 0, drawX + 16, drawY - 5);
						}
						else //Другое
						{
							//Необходимо ли подсвечивать фон (для перетаскиваемого скилла)
							bool hightlightGroup = false;
							if (group == selectedGroup && !group->GetMaximized() && si != -1)
								hightlightGroup = true;

							//Если выбрана группа или перетаскиваем скилл на текущую группу - подсветим ее
							if (selname == 1)
								g_GL.DrawPolygone(selectedPolygoneColor, drawX + 16.0f, (float)drawY, th.Width - 10.0f, 14.0f);
							else if (hightlightGroup)
								g_GL.DrawPolygone(selectedPolygoneColor, drawX + 16.0f, (float)drawY, 200.0f, 14.0f);

							//Отобразим имя группы (из заранее заготовленной текстуры)
							th.Draw(drawX + 16, drawY - 5);

							//И "косу" до конца строки
							int fw = 11 + th.Width;
							int lw = 215 - fw;
							if (lw > 0)
								UO->DrawGump(0x0835, 0, drawX + fw, drawY + 5, lw, 0);
						}
					}
				}

				if (group->GetMaximized()) //Если группа развернута - отображаем её содержимое
				{
					if (canDraw) //Можем рисовать
					{
						//Если в группе что-то есть - отобразим стрелку
						if (group->GetCount())
							UO->DrawGump(0x0826, 0, drawX, drawY);

						//Смещаем указатель на стандартный шаг + 2 (только для группы)
						drawY += (drawStep + 2);
					}

					//Если это группа, содержащая изначально перетаскиваемый скилл
					//и скилл этот отображается в другой группе на данный момент
					//то не отображаем его здесь
					bool dontDrawSI = false;
					if (group != selectedGroup && removeIndexOnExit)
						dontDrawSI = true;

					int cnt = group->GetCount();

					IFOR(i, 0, cnt) //Цикл по все элементам группы
					{
						if (drawY + 10 >= boundsY)
							break; //Вышли за пределы области

						//Доступно для отображения?
						canDraw = (currentIndex >= startIndex);
						currentIndex++;

						if (canDraw) //Можно рисовать
						{
							BYTE idx = group->GetItem(i); //Получаем индекс скилла по порядковому номеру
							if (idx < g_SkillsCount) //Он валиден
							{
								//Если это перетаскиваемый скилл и его не нужно отображать - ничего не делаем
								if (idx == si && dontDrawSI)
								{
								}
								else //В ином случае - рисуем
								{
									WORD gumpID = 0x0837; //Button
									if (Skills[idx].m_Button) //Если у скилла есть кнопка - отобразим её
									{
										if (CanSelectedButton == 12000 + idx) 
											gumpID = 0x0838; //Selected button

										UO->DrawGump(gumpID, 0, drawX + 8, drawY); //Button
									}

									TTextTexture &th = Skills[idx].m_Texture; //Name
									if (!th.Empty())
									{
										if (si == idx)
											g_GL.DrawPolygone(selectedPolygoneColor, drawX + 20.0f, drawY - 1.0f, 250.0f, 14.0f);

										th.Draw(drawX + 22, drawY - 1);
									}

									BYTE status = g_Player->GetSkillStatus(idx); //Статус (вниз/вверх/замок)

									gumpID = 0x0984; //Up
									if (status == 1)
										gumpID = 0x0986; //Down
									else if (status == 2)
										gumpID = 0x082C; //Lock
									UO->DrawGump(gumpID, 0, drawX + 251, drawY - 1);

									//Значение скилла (учитывая выбранный флаг отображения)
									float val = g_Player->GetSkillBaseValue(idx);
									if (m_ShowReal)
										val = g_Player->GetSkillValue(idx);
									else if (m_ShowCap)
										val = g_Player->GetSkillCap(idx);

									char sbf[20] = {0};
									sprintf(sbf, "%.1f", val);

									//Выравниваем его к правому краю
									int fw = FontManager->GetWidthA(9, sbf, strlen(sbf));
									FontManager->DrawA(9, sbf, 0x0288, drawX + 250 - fw, drawY - 1); //Value

									drawY += drawStep; //Переходим на следующую позицию по оси Y
								}
							}
						}
					}
				}
				else if (canDraw) //Группа свернута
				{
					//Если в группе что-то есть - нарисуем стрелку
					if (group->GetCount())
						UO->DrawGump(0x0827, 0, drawX, drawY);
					//g_GL.DrawPolygone(0x007F7F7F, drawX, drawY, 14, 14);

					drawY += drawStep; //Переходим на следующую позицию координат
				}

				group = group->m_Next; //Следующая кгруппа
			}
		}

		//Если есть необходимость удалить элемент - удаляем
		if (removeIndexOnExit)
			selectedGroup->Remove(si);

		//Восстанавливаем размер рабочей области
		g_GL.RestorePort();

		UO->DrawGump(0x082B, 0, posX + 30, posY + 60); //Top line
		UO->DrawGump(0x0824, 0, posX + 294, posY + 56); //^ button
		
		UO->DrawGump(0x082B, 0, posX + 31, posY + height - 1); //Bottom line
		UO->DrawGump(0x0836, 0, posX + 30, posY + height + 13); //Skills comment gump
		UO->DrawGump(0x0825, 0, posX + 294, posY + height + 3); //v button

		UO->DrawGump(0x1F43, 0, posX + 21, posY + height + 34); //Bottom scroll
		UO->DrawGump(0x083A, 0, posX + 60, posY + height + 44); //New Group gump

		//Общая сумма скиллов
		char totalSkills[20] = {0};
		sprintf(totalSkills, "%.1f", g_SkillsTotal);
		FontManager->DrawA(3, totalSkills, 0x0065, posX + 235, posY + height + 41);
		
		WORD gumpID = 0x082E;
		if (CanSelectedButton == ID_GS_BUTTON_RESIZE)
			gumpID = 0x082F;
		UO->DrawGump(gumpID, 0, posX + 167, posY + height + 66); //Resize

	//Завершаем запись дисплей листа
	glEndList();

	FrameRedraw = true;
	FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpSkills::Draw(bool &mode)
{
	//Индекс гампа
	DWORD index = (DWORD)this;

	//Координаты, для быстрого доступа
	int posX = X;
	int posY = Y;

	//Если гамп свернут - берем соответствующие координаты
	if (Minimized)
	{
		posX = MinimizedX;
		posY = MinimizedY;

		m_LastResizeY = 0; //Выставим значение изменения высоты в нуль
	}
	
	//Нажата ли кнопка в окне?
	bool IsScrollerPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index);

	//Нажата ли кнопка в окне?
	bool IsPressed = (IsScrollerPressed && g_LastSelectedGump == index);

	//Может ли быть подсвечен элемент?
	int CanSelectedButton = ((g_LastSelectedGump == index) ? g_LastSelectedObject : 0);

	//Может ли быть нажат элемент?
	int CanPressedButton = 0;
	if (IsPressed && g_LastObjectLeftMouseDown == g_LastSelectedObject)
		CanPressedButton = g_LastObjectLeftMouseDown;

	//Для безопасного и быстрого доступа к высоте гампа
	int height = m_Height;

	//У гампа активирован статус изменения высоты
	if (m_LastResizeY)
	{
		//Корректируем текущую высоту
		height += (g_MouseY - m_LastResizeY);
		if (height < 120)
			height = 120;

		int bh = (GetSystemMetrics(SM_CYSCREEN) - 50);
		if (height >= bh)
			height = bh;

		//Если это режим рисования - слепим фрэйм гампа
		if (mode)
			GenerateFrame(posX, posY);
	}

	//Для вычисления задержек между скроллингом
	DWORD ticks = GetTickCount();

	//Проверим, вдруг необходимо изменить положение отображаемого элемента
	if (IsScrollerPressed && m_LastChangedLineTime < ticks)
	{
		if (g_LastObjectLeftMouseDown == ID_GS_BUTTON_UP) //Скроллинг вверх (гампом-стрелкой)
		{
			if (m_CurrentLine > 0)
				m_CurrentLine--;
			else
				m_CurrentLine = 0;

			m_LastChangedLineTime = ticks + SCROLL_LISTING_DELAY;
		}
		else if (g_LastObjectLeftMouseDown == ID_GS_BUTTON_DOWN) //Скроллинг вниз (гампом-стрелкой)
		{
			int visibleLines = SkillGroupManager.GetVisibleLinesCount();

			if (m_CurrentLine < visibleLines)
				m_CurrentLine++;
			else
				m_CurrentLine = visibleLines;

			m_LastChangedLineTime = ticks + SCROLL_LISTING_DELAY;
		}
		else if (g_LastObjectLeftMouseDown >= ID_GS_SKILL && g_LastObjectLeftMouseDown < 14000) //Skill name pressed
		{
			//Скроллинг перетаскиванием скилла
			if (g_MouseY < (posY + 72)) //Скроллинг вверх
			{
				if (m_CurrentLine > 0)
					m_CurrentLine--;
				else
					m_CurrentLine = 0;

				m_LastChangedLineTime = ticks + (SCROLL_LISTING_DELAY / 3);
			}
			else if (g_MouseY > (posY + height)) //Скроллинг вниз
			{
				int visibleLines = SkillGroupManager.GetVisibleLinesCount();

				if (m_CurrentLine < visibleLines)
					m_CurrentLine++;
				else
					m_CurrentLine = visibleLines;
				
				m_LastChangedLineTime = ticks + (SCROLL_LISTING_DELAY / 3);
			}
		}
	}

	//Вычисляем позицию скролл бара
	int scrollerY = 0;
	bool noRedraw = false;

	if (!Minimized) //Только для оригинального гампа
	{
		//Количество видимых линий
		int visibleLines = SkillGroupManager.GetVisibleLinesCount();

		//Максимальная позиция скроллера
		int maxScrollerY = height - 104;

		//Скроллер захвачен
		if (IsScrollerPressed && g_LastObjectLeftMouseDown == ID_GS_SCROLLER) //Scroller pressed
		{
			//Текущая позиция скроллера
			int currentY = (g_MouseY - 10) - posY - 72; //Scroller position

			scrollerY = CalculateScrollerAndTextPosition(m_CurrentLine, visibleLines, maxScrollerY, currentY);

			if (mode)
				GenerateFrame(posX, posY);

			noRedraw = true;
		}
		else if (m_CurrentLine) //Или же, скроллер смещен, но не захвачеен
		{
			scrollerY = CalculateScrollerY(m_CurrentLine, visibleLines, maxScrollerY);

			//Если режим рисования - перерисуем фрэйм
			if (mode)
				GenerateFrame(posX, posY);

			noRedraw = true;
		}
	}

	//Если гамп перетаскивают
	if (CanBeMoved() && g_LeftMouseDown && g_LastGumpLeftMouseDown == index && !g_LastObjectLeftMouseDown)
	{
		posX += g_MouseX - g_DroppedLeftMouseX;
		posY += g_MouseY - g_DroppedLeftMouseY;

		//А так же, если это режим рисования - перерисуем фрэйм
		if (mode && !m_LastResizeY)
			GenerateFrame(posX, posY);
	}
	else if (mode && !noRedraw) //Перерисовка фрэйма, при соблюдении необходимых условий
	{
		if (CanSelectedButton && !m_LastResizeY)
			GenerateFrame(posX, posY);
		else if (FrameRedraw)
		{
			GenerateFrame(posX, posY);
			FrameRedraw = false;
		}
	}
	
	if (mode) //Отрисовка
	{
		//Если фрэйм не был создан - создаем
		if (!FrameCreated)
			GenerateFrame(posX, posY);

		//Отобразим фрэйм
		glCallList((GLuint)index);

		if (g_ShowGumpLocker)
			g_GL.Draw(g_TextureGumpState[LockMoving], (GLfloat)posX, (GLfloat)posY, 10.0f, 14.0f);
	}
	else //Выбор объектов
	{
		if (Minimized) //Если гамп свернут
		{
			if (UO->GumpPixelsInXY(0x0839, posX, posY))
			{
				g_LastSelectedObject = 0;
				g_LastSelectedGump = index;
			}

			return 0;
		}

		//Стандартный гамп
		//Ниже те же действия, что и в функции создания фрэйма, только без самого рисования, комменты можно посмотреть там

		int LSG = 0;

		if (UO->GumpPixelsInXY(0x082D, posX + 167, posY)) //Minimize
		{
			LSG = ID_GS_BUTTON_MINIMIZE;
			g_LastSelectedGump = index;
		}
		else if (UO->GumpPixelsInXY(0x1F40, posX, posY + 23)) //Top scroll
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}
		
		if (g_LastSelectedGump != index)
		{
			//Skills body
			int curposY = posY + 59;
			while (true)
			{
				int deltaHeight = (height + posY) - (curposY - 36);

				if (deltaHeight  < 70)
				{
					if (deltaHeight > 0)
					{
						if (UO->GumpPixelsInXY(0x1F41, posX + 21, curposY, 0, deltaHeight))
						{
							g_LastSelectedObject = 0;
							g_LastSelectedGump = index;
						}
					}

					break;
				}
				else if (UO->GumpPixelsInXY(0x1F41, posX + 21, curposY))
				{
					g_LastSelectedObject = 0;
					g_LastSelectedGump = index;

					break;
				}

				curposY += 70;

				deltaHeight = (height + posY) - (curposY - 36);

				if (deltaHeight < 70)
				{
					if (deltaHeight > 0)
					{
						if (UO->GumpPixelsInXY(0x1F42, posX + 21, curposY, 0, deltaHeight))
						{
							g_LastSelectedObject = 0;
							g_LastSelectedGump = index;
						}
					}

					break;
				}
				else if (UO->GumpPixelsInXY(0x1F42, posX + 21, curposY))
				{
					g_LastSelectedObject = 0;
					g_LastSelectedGump = index;

					break;
				}

				curposY += 70;
			}
		}
		
		if (UO->GumpPixelsInXY(0x1F43, posX + 21, posY + height + 34)) //Bottom scroll
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}
		
		if (UO->GumpPixelsInXY(0x082E, posX + 167, posY + height + 66)) //Resize
		{
			LSG = ID_GS_BUTTON_RESIZE;
			g_LastSelectedGump = index;
		}
		else if (UO->GumpPixelsInXY(0x0938, posX + 226, posY + 34)) //Real
			LSG = ID_GS_SHOW_REAL;
		else if (UO->GumpPixelsInXY(0x0938, posX + 280, posY + 34)) //Cap
			LSG = ID_GS_SHOW_CAP;
		else if (UO->GumpPixelsInXY(0x0824, posX + 294, posY + 56)) //^ button
			LSG = ID_GS_BUTTON_UP;
		else if (UO->GumpPixelsInXY(0x0825, posX + 294, posY + height + 3)) //v button
			LSG = ID_GS_BUTTON_DOWN;
		else if (UO->GumpPixelsInXY(0x0828, posX + 296, posY + 72 + scrollerY)) //Scroller
			LSG = ID_GS_SCROLLER;
		else
		{
			RECT rc = {0, 0, 80, 14};
			POINT p = {g_MouseX - (posX + 60), g_MouseY - (posY + height + 44)};

			if (PtInRect(&rc, p))
				LSG = ID_GS_BUTTON_NEW_GROUP;
		}
		
		if (g_LastSelectedGump == index && !LSG)
		{
			int startIndex = m_CurrentLine;
			int currentIndex = 0;
		
			int drawX = posX + 28;
			int drawY = posY + 75;

			const int drawStep = 17;
		
			int boundsY = posY + height;

			int groupIndex = 0;
		
			if (g_Player != NULL)
			{
				PSKILLS_DATA Skills = g_Skills;

				TSkillGroupObject *group = SkillGroupManager.m_Groups;
				while (group != NULL)
				{
					if (drawY + 10 >= boundsY)
						break;

					bool canDraw = (currentIndex >= startIndex);
					currentIndex++;

					if (canDraw)
					{
						TTextTexture &th = group->m_Texture;
						if (th.Width)
						{
							RECT rc = {0, 0, th.Width - 10, 14};
							POINT p = {g_MouseX - (drawX + 16), g_MouseY - drawY};
							if (PtInRect(&rc, p))
							{
								LSG = ID_GS_GROUP + groupIndex;

								break;
							}
						}
					}

					if (group->GetMaximized())
					{
						if (canDraw)
						{
							if (group->GetCount())
							{
								RECT rc = {0, 0, 14, 14};
								POINT p = {g_MouseX - drawX, g_MouseY - drawY};
								if (PtInRect(&rc, p))
								{
									LSG = ID_GS_GROUP_MINIMIZE + groupIndex;

									break;
								}
							}

							drawY += (drawStep + 2);
						}

						bool completedSearch = false;

						int cnt = group->GetCount();
						IFOR(i, 0, cnt)
						{
							if (completedSearch)
								break;

							if (drawY + 10 >= boundsY)
								break;

							canDraw = (currentIndex >= startIndex);
							currentIndex++;

							if (canDraw)
							{
								BYTE idx = group->GetItem(i);
								if (idx < g_SkillsCount)
								{
									if (Skills[idx].m_Button && UO->GumpPixelsInXY(0x0837, drawX + 8, drawY))
									{
										LSG = ID_GS_SKILL_BUTTON + idx; //Button
										completedSearch = true;

										break;
									}

									RECT rc = {0, 0, 150, 14};

									TTextTexture &th = Skills[idx].m_Texture;
									if (th.Width > 150)
										rc.right = th.Width;

									POINT p = {g_MouseX - (drawX + 22), g_MouseY - (drawY - 1)};
									if (PtInRect(&rc, p))
									{
										LSG = ID_GS_SKILL + idx; //Name
										completedSearch = true;

										break;
									}

									BYTE status = g_Player->GetSkillStatus(idx);

									WORD gumpID = 0x0984; //Up
									if (status == 1)
										gumpID = 0x0986; //Down
									else if (status == 2)
										gumpID = 0x082C; //Lock

									if (UO->GumpPixelsInXY(gumpID, drawX + 251, drawY - 1))
									{
										LSG = ID_GS_SKILL_STATE + idx;
										completedSearch = true;

										break;
									}
								
									drawY += drawStep;
								}
							}
						}

						if (completedSearch)
							break;
					}
					else if (canDraw)
					{
						if (group->GetCount())
						{
							RECT rc = {0, 0, 14, 14};
							POINT p = {g_MouseX - drawX, g_MouseY - drawY};
							//if (UO->GumpPixelsInXY(0x0827, drawX, drawY))
							if (PtInRect(&rc, p))
							{
								LSG = ID_GS_GROUP_MINIMIZE + groupIndex;

								break;
							}
						}

						drawY += drawStep;
					}

					groupIndex++;
					group = group->m_Next;
				}
			}
		}

		if (LSG != 0)
			g_LastSelectedObject = LSG;

		if (g_ShowGumpLocker && UO->PolygonePixelsInXY(posX, posY, 10, 14))
		{
			g_LastSelectedObject = ID_GS_LOCK_MOVING;
			g_LastSelectedGump = index;
		}

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
TSkillGroupObject *TGumpSkills::GetGroupUnderCursor(int startIndex)
{
	//Получить группу под курсором

	int x = X;
	int y = Y;

	//Если вышли за пределы гампа по оси X
	if (g_MouseX < (x + 30) || g_MouseX > (x + 250))
		return NULL; //Exit from bounds on X

	//Если назодимся в пределах гампа по оси Y
	if (g_MouseY > (y + 72) && g_MouseY < (y + m_Height)) //Bounds of Y
	{
		int currentIndex = 0; //Указатель на текущий индекс

		//Начало рисования
		int drawY = y + 75;

		//Шаг рисования (в пикселях)
		const int drawStep = 17;

		//Максимальная координата Y для отображения
		int boundsY = y + m_Height;

		//Указатель на группы
		TSkillGroupObject *group = SkillGroupManager.m_Groups;

		//Пробежимся по всем группам
		while (group != NULL)
		{
			//Вышли за пределы, возвращаем нулик
			if (drawY >= boundsY)
				return NULL;

			//Можно ли отобразить?
			bool canDraw = (currentIndex >= startIndex);
			currentIndex++;

			//Если группа развернута
			if (group->GetMaximized())
			{
				if (canDraw) //Если пригодно для отображения
				{
					drawY += (drawStep + 2); //Смещаем координаты рисования

					if (g_MouseY < drawY)
						return group; //Если мышка над этим местом - возвращаем группу
				}

				int cnt = group->GetCount();

				//Пройдемся по элементам группы
				IFOR(i, 0, cnt)
				{
					if (drawY >= boundsY)
						return NULL; //Вышли за пределы, нуль

					//Видимый объект
					if (currentIndex >= startIndex)
					{
						drawY += drawStep; //Смещаемся

						//Если попали над объект - возвращаем группу
						if (g_MouseY < drawY)
							return group;
					}

					//Увеличиваем счетчик объектов
					currentIndex++;
				}
			}
			else if (canDraw) //Или если можно отобразить
			{
				drawY += drawStep; //Движемся вперед

				//Группа найдена, возвращаем её
				if (g_MouseY < drawY)
					return group;
			}

			group = group->m_Next; //Следующая группа
		}
	}

	//Ничего не нашлось
	return NULL;
}
//----------------------------------------------------------------------------
void TGumpSkills::OnLeftMouseDown()
{
	if (g_LastSelectedGump == (DWORD)this && (g_LastSelectedObject >= ID_GS_SKILL && g_LastSelectedObject < ID_GS_SKILL_STATE))
	{
		//При нажатии кнопки левой мыши в гампе по скиллу можно снять выделение с групп
		//выйти из редактирования и изменить редактируемое имя
		TSkillGroupObject *group = SkillGroupManager.m_Groups;
		while (group != NULL)
		{
			if (group->GetSelected() == 2)
			{
				if (TextEntry->Length() > 0)
					group->SetName(TextEntry->c_str());
			}

			if (!group->GetName().length())
				group->SetName("NoNameGroup");

			group->SetSelected(0);

			group = group->m_Next;
		}
	}
}
//----------------------------------------------------------------------------
void TGumpSkills::OnLeftMouseUp()
{
	if (Minimized)
		return; //Не обрабатываем при свернутом гампе
	else if (g_LastObjectLeftMouseDown != g_LastSelectedObject)
	{
		//Если скилл перетаскивается - можно выполнить действие
		if (g_LastObjectLeftMouseDown < ID_GS_SKILL || g_LastObjectLeftMouseDown >= ID_GS_SKILL_STATE)
			return;
	}

	//Текущее количество тиков
	DWORD ticks = GetTickCount();

	if (g_LastObjectLeftMouseDown == ID_GS_BUTTON_MINIMIZE) //Сворачиваем гамп
	{
		Minimized = true;
		GenerateFrame(MinimizedX, MinimizedY);
	}
	else if (g_LastObjectLeftMouseDown == ID_GS_LOCK_MOVING)
	{
		LockMoving = !LockMoving;
		g_CancelDoubleClick = true;
	}
	else if (g_LastObjectLeftMouseDown == ID_GS_BUTTON_RESIZE) //Ресайзинг
	{
		if (!m_LastResizeY)
		{
			m_LastResizeY = g_DroppedLeftMouseY;
			g_ResizedGump = this;
		}
		else
		{
			SetHeight(m_Height + (g_MouseY - m_LastResizeY));

			m_LastResizeY = 0;
			g_ResizedGump = NULL;
		}
	}
	else if (g_LastObjectLeftMouseDown == ID_GS_SHOW_REAL) //Показать реальное значение
	{
		m_ShowReal = !m_ShowReal;
		m_ShowCap = false;
	}
	else if (g_LastObjectLeftMouseDown == ID_GS_SHOW_CAP) //Показать доступный предел прокачки
	{
		m_ShowCap = !m_ShowCap;
		m_ShowReal = false;
	}
	else if (g_LastObjectLeftMouseDown == ID_GS_BUTTON_NEW_GROUP) //Создание новой группы
	{
		TSkillGroupObject *group = new TSkillGroupObject();
		group->SetName("New Group");
		SkillGroupManager.Add(group);
	}
	else if (g_LastObjectLeftMouseDown == ID_GS_BUTTON_UP && m_LastChangedLineTime < ticks) //Скроллинг вверх
	{
		if (m_CurrentLine > 0)
			m_CurrentLine--;
		else
			m_CurrentLine = 0;

		m_LastChangedLineTime = ticks + SCROLL_LISTING_DELAY;
		UpdateFrame();
	}
	else if (g_LastObjectLeftMouseDown == ID_GS_BUTTON_DOWN && m_LastChangedLineTime < ticks) //Скроллинг вниз
	{
		int visibleLines = SkillGroupManager.GetVisibleLinesCount();

		if (m_CurrentLine < visibleLines)
			m_CurrentLine++;
		else
			m_CurrentLine = visibleLines;

		m_LastChangedLineTime = ticks + SCROLL_LISTING_DELAY;
		UpdateFrame();
	}
	else if (g_LastObjectLeftMouseDown >= ID_GS_GROUP_MINIMIZE) //Операции со скиллами
	{
		if (g_LastObjectLeftMouseDown >= ID_GS_SKILL_STATE) //Изменение статуса
		{
			int idx = g_LastObjectLeftMouseDown - ID_GS_SKILL_STATE;
			if (idx < 0 || idx >= g_SkillsCount)
				return;

			if (g_Player == NULL)
				return;

			BYTE status = g_Player->GetSkillStatus(idx);
			if (status < 2)
				status++;
			else
				status = 0;

			UO->SkillStatusChange(idx, status);
			g_Player->SetSkillStatus(idx, status);
		}
		else if (g_LastObjectLeftMouseDown >= ID_GS_SKILL) //Выбор названия скилла
		{
			int idx = g_LastObjectLeftMouseDown - ID_GS_SKILL;
			if (idx < 0 || idx >= g_SkillsCount)
				return;

			TSkillGroupObject *group = GetGroupUnderCursor(m_CurrentLine);
				
			if (group != NULL)
			{
				TSkillGroupObject *groups = SkillGroupManager.m_Groups;
				while (groups != NULL)
				{
					groups->Remove(idx);
					groups = groups->m_Next;
				}

				group->AddSorted(idx);

				return;
			}
		}
		else if (g_LastObjectLeftMouseDown >= ID_GS_SKILL_BUTTON) //Выбор кнопки для использования скилла
		{
			int idx = g_LastObjectLeftMouseDown - ID_GS_SKILL_BUTTON;
			if (idx < 0 || idx >= g_SkillsCount)
				return;

			UO->UseSkill(idx);
		}
		else if (g_LastObjectLeftMouseDown >= ID_GS_GROUP) //Выбор названия группы
		{
			int idx = g_LastObjectLeftMouseDown - ID_GS_GROUP;
			int cidx = 0;

			TSkillGroupObject *group = SkillGroupManager.m_Groups;
			while (group != NULL)
			{
				if (idx == cidx)
				{
					BYTE sel = group->GetSelected();
					if (sel < 2)
						sel++;

					TSkillGroupObject *item = SkillGroupManager.m_Groups;
					while (item != NULL)
					{
						if (item->GetSelected() == 2)
						{
							if (TextEntry->Length() > 0)
								item->SetName(TextEntry->c_str());
						}

						item->SetSelected(0);
						if (!item->GetName().length())
							item->SetName("NoNameGroup");

						item = item->m_Next;
					}

					if (sel == 2)
					{
						EntryPointer = TextEntry;
						EntryPointer->SetText(group->GetName());
					}

					group->SetSelected(sel);

					break;
				}

				cidx++;
				group = group->m_Next;
			}
		}
		else if (g_LastObjectLeftMouseDown >= ID_GS_GROUP_MINIMIZE) //Скрыть/раскрыть группу
		{
			int idx = g_LastObjectLeftMouseDown - ID_GS_GROUP_MINIMIZE;
			int cidx = 0;

			TSkillGroupObject *group = SkillGroupManager.m_Groups;
			while (group != NULL)
			{
				if (idx == cidx)
				{
					group->SetMaximized(!group->GetMaximized());
					break;
				}

				cidx++;
				group = group->m_Next;
			}
		}
	}
}
//----------------------------------------------------------------------------
bool TGumpSkills::OnLeftMouseDoubleClick()
{
	if (Minimized) //При даблклике по мини-гампу - раскрываем его
	{
		Minimized = false;
		GenerateFrame(X, Y);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void TGumpSkills::OnMouseWheel(MOUSE_WHEEL_STATE &state)
{
	if (!Minimized && !g_LeftMouseDown && !g_RightMouseDown) //Доступно для скроллинга
	{
		DWORD ticks = GetTickCount();

		if (state == MWS_UP && m_LastChangedLineTime < ticks) //Скроллинг вверх
		{
			if (m_CurrentLine > 0)
				m_CurrentLine--;
			else
				m_CurrentLine = 0;

			m_LastChangedLineTime = ticks + (SCROLL_LISTING_DELAY / 5);
			UpdateFrame();
		}
		else if (state == MWS_DOWN && m_LastChangedLineTime < ticks) //Скроллинг вниз
		{
			int visibleLines = SkillGroupManager.GetVisibleLinesCount();

			if (m_CurrentLine < visibleLines)
				m_CurrentLine++;
			else
				m_CurrentLine = visibleLines;

			m_LastChangedLineTime = ticks + (SCROLL_LISTING_DELAY / 5);
			UpdateFrame();
		}
	}
}
//----------------------------------------------------------------------------
void TGumpSkills::OnCharPress(WPARAM &wparam, LPARAM &lparam)
{
	//Ввод текста
	if (wparam == VK_RETURN || wparam == VK_BACK || wparam == VK_ESCAPE || EntryPointer != TextEntry)
		return; //Ignore no print keys

	EntryPointer->Insert(wparam);

	int val = FontManager->GetWidthA(6, EntryPointer->c_str(), EntryPointer->Length());
	if (val > 170)
		EntryPointer->Remove(true);
	else
		UpdateFrame();
}
//----------------------------------------------------------------------------
void TGumpSkills::OnKeyPress(WPARAM &wparam, LPARAM &lparam)
{
	if (EntryPointer != TextEntry)
	{
		if (wparam == VK_DELETE)
		{
			TSkillGroupObject *item = SkillGroupManager.m_Groups;

			while (item != NULL)
			{
				if (item->GetSelected() == 1)
				{
					SkillGroupManager.Remove(item);
					UpdateFrame();

					break;
				}

				item = item->m_Next;
			}
		}

		return;
	}

	//Обработчик нажатия клавишь
	switch (wparam)
	{
		case VK_RETURN:
		{
			TSkillGroupObject *item = SkillGroupManager.m_Groups;
			while (item != NULL)
			{
				if (item->GetSelected() == 2)
				{
					if (TextEntry->Length() > 0)
						item->SetName(TextEntry->c_str());
				}

				item->SetSelected(0);
				if (!item->GetName().length())
					item->SetName("NoNameGroup");

				item = item->m_Next;
			}

			if (ConfigManager.GetConsoleNeedEnter())
				EntryPointer = NULL;
			else
				EntryPointer = GameConsole;

			UpdateFrame();

			break;
		}
		case VK_HOME:
		{
			EntryPointer->SetPos(0);
			UpdateFrame();

			break;
		}
		case VK_END:
		{
			EntryPointer->SetPos(EntryPointer->Length());
			UpdateFrame();

			break;
		}
		case VK_LEFT:
		{
			EntryPointer->AddPos(-1);
			UpdateFrame();

			break;
		}
		case VK_RIGHT:
		{
			EntryPointer->AddPos(1);
			UpdateFrame();

			break;
		}
		case VK_BACK:
		{
			EntryPointer->Remove(true);
			UpdateFrame();

			break;
		}
		case VK_DELETE:
		{
			EntryPointer->Remove(false);
			UpdateFrame();

			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------