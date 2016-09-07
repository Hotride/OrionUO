/***********************************************************************************
**
** GumpSkills.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GumpSkills.h"
#include "../SkillGroup.h"
#include "../Managers/SkillGroupManager.h"
#include "../Managers/ConfigManager.h"
#include "../Managers/MouseManager.h"
#include "../TextEngine/GameConsole.h"
//----------------------------------------------------------------------------------
CGumpSkills::CGumpSkills(uint serial, short x, short y, bool minimized, int height)
: CGumpBaseScroll(GT_SKILLS, serial, 0x1F40, height, x, y, true, 0, true, 15),
m_ShowReal(false), m_ShowCap(false)
{
	m_Locker.Serial = ID_GS_LOCK_MOVING;

	if (minimized)
	{
		m_Page = 1;
		m_Minimized = minimized;
		m_MinimizedX = x;
		m_MinimizedY = y;
	}
	else
		m_Page = 2;

	Add(new CGUIPage(1));
	Add(new CGUIGumppic(0x0839, 0, 0));

	Add(new CGUIPage(2));
	Add(new CGUIGumppic(0x0834, 82, 34)); //Skills text gump

	CGUICheckbox *checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GS_SHOW_REAL, 0x0938, 0x0939, 0x0938, 226, 34));
	checkbox->Checked = m_ShowReal;
	checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GS_SHOW_CAP, 0x0938, 0x0939, 0x0938, 280, 34));
	checkbox->Checked = m_ShowCap;

	CGUIText *text = (CGUIText*)Add(new CGUIText(0x0386, 180, 33));
	text->CreateTextureA(1, "Show:   Real    Cap");

	Add(new CGUIGumppic(0x082B, 30, 60)); //Top line
	m_BottomLine = (CGUIGumppic*)Add(new CGUIGumppic(0x082B, 31, m_Height - 48)); //Bottom line
	m_Comment = (CGUIGumppic*)Add(new CGUIGumppic(0x0836, 30, m_Height - 35));//Skills comment gump
	m_CreateGroup = (CGUIButton*)Add(new CGUIButton(ID_GS_BUTTON_NEW_GROUP, 0x083A, 0x083A, 0x083A, 60, m_Height - 3)); //New Group gump

	m_SkillSum = (CGUIText*)Add(new CGUIText(0x0065, 235, m_Height - 6));
	UpdateSkillsSum();

	m_DataBox = (CGUIDataBox*)m_HTMLGump->Add(new CGUIDataBox());
}
//----------------------------------------------------------------------------------
CGumpSkills::~CGumpSkills()
{
}
//----------------------------------------------------------------------------------
void CGumpSkills::UpdateHeight()
{
	CGumpBaseScroll::UpdateHeight();

	m_BottomLine->Y = m_Height - 48; //Bottom line
	m_Comment->Y = m_Height - 35;//Skills comment gump
	m_CreateGroup->Y = m_Height - 3; //New Group gump
	m_SkillSum->Y = m_Height - 6;
}
//----------------------------------------------------------------------------------
void CGumpSkills::UpdateSkillsSum()
{
	char str[20] = { 0 };
	sprintf(str, "%.1f", g_SkillsTotal);
	m_SkillSum->CreateTextureA(3, str);
}
//----------------------------------------------------------------------------------
void CGumpSkills::Init()
{
	//Свернем все доступные группы
	QFOR(group, g_SkillGroupManager.m_Groups, CSkillGroupObject*)
		group->Maximized = false;
}
//----------------------------------------------------------------------------------
void CGumpSkills::PrepareContent()
{
}
//----------------------------------------------------------------------------------
void CGumpSkills::UpdateContent()
{
	m_DataBox->Clear();

	/*//Индекс строки для начала отображения
	int startIndex = m_CurrentLine;
	int currentIndex = 0; //Буффер индексов

	int drawX = 30;
	int drawY = 72;

	//Стандартный цвет выделенной группы/скилла
	DWORD selectedPolygoneColor = 0x007F7F7F;

	//Установим видимую область
	g_GL.Scissor((int)g_GumpTranslateX + drawX, (int)g_GumpTranslateY + drawY, 264, height - 74);

	//Расстояние между строками
	const int drawStep = 17;

	//Стартовые координаты начала отображения
	drawX -= 2;
	drawY += 3;

	//Максимальная координата отображения по оси Y
	int boundsY = height;

	int si = -1; //Индекс выбранного скилла

	CSkillGroupObject *selectedGroup = NULL; //Указатель на группу, в которой выбран скилл
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
						g_GL.DrawPolygone(drawX + 16, drawY, 200, 14);

						//Текущее название
						TextEntry->DrawA(6, 0, drawX + 16, drawY - 5);
					}
					else //Другое
					{
						//Необходимо ли подсвечивать фон (для перетаскиваемого скилла)
						bool hightlightGroup = false;
						if (group == selectedGroup && !group->Maximized && si != -1)
							hightlightGroup = true;

						//Если выбрана группа или перетаскиваем скилл на текущую группу - подсветим ее
						if (selname == 1)
							g_GL.DrawPolygone(drawX + 16, drawY, th.Width - 10, 14);
						else if (hightlightGroup)
							g_GL.DrawPolygone(drawX + 16, drawY, 200, 14);

						//Отобразим имя группы (из заранее заготовленной текстуры)
						th.Draw(drawX + 16, drawY - 5);

						//И "косу" до конца строки
						int fw = 11 + th.Width;
						int lw = 215 - fw;
						if (lw > 0)
							Orion->DrawGump(0x0835, 0, drawX + fw, drawY + 5, lw, 0);
					}
				}
			}

			if (group->GetMaximized()) //Если группа развернута - отображаем её содержимое
			{
				if (canDraw) //Можем рисовать
				{
					//Если в группе что-то есть - отобразим стрелку
					if (group->GetCount())
						Orion->DrawGump(0x0826, 0, drawX, drawY);

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
									if (g_GumpSelectElement == ID_GS_SKILL_BUTTON + idx)
										gumpID = 0x0838; //Selected button

									Orion->DrawGump(gumpID, 0, drawX + 8, drawY); //Button
								}

								TTextTexture &th = Skills[idx].m_Texture; //Name
								if (!th.Empty())
								{
									if (si == idx)
										g_GL.DrawPolygone(drawX + 20, drawY - 1, 250, 14);

									th.Draw(drawX + 22, drawY - 1);
								}

								BYTE status = g_Player->GetSkillStatus(idx); //Статус (вниз/вверх/замок)

								gumpID = 0x0984; //Up
								if (status == 1)
									gumpID = 0x0986; //Down
								else if (status == 2)
									gumpID = 0x082C; //Lock
								Orion->DrawGump(gumpID, 0, drawX + 251, drawY - 1);

								//Значение скилла (учитывая выбранный флаг отображения)
								float val = g_Player->GetSkillBaseValue(idx);
								if (m_ShowReal)
									val = g_Player->GetSkillValue(idx);
								else if (m_ShowCap)
									val = g_Player->GetSkillCap(idx);

								char sbf[20] = { 0 };
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
					Orion->DrawGump(0x0827, 0, drawX, drawY);

				drawY += drawStep; //Переходим на следующую позицию координат
			}

			group = group->m_Next; //Следующая кгруппа
		}
	}

	//Если есть необходимость удалить элемент - удаляем
	if (removeIndexOnExit)
		selectedGroup->Remove(si);*/
}
//----------------------------------------------------------------------------------
/*int TGumpSkills::Draw(bool &mode)
{
	//Индекс гампа
	DWORD index = (DWORD)this;

	//Если гамп свернут - берем соответствующие координаты
	if (m_Minimized)
		m_LastResizeY = 0; //Выставим значение изменения высоты в нуль

	CalculateGumpState();

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
			m_FrameCreated = false;
	}

	//Для вычисления задержек между скроллингом
	DWORD ticks = g_Ticks;

	//Проверим, вдруг необходимо изменить положение отображаемого элемента
	if (g_GumpPressedScroller && m_LastChangedLineTime < ticks)
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
			if (g_MouseY < ((int)g_GumpTranslateY + 72)) //Скроллинг вверх
			{
				if (m_CurrentLine > 0)
					m_CurrentLine--;
				else
					m_CurrentLine = 0;

				m_LastChangedLineTime = ticks + (SCROLL_LISTING_DELAY / 3);
			}
			else if (g_MouseY > ((int)g_GumpTranslateY + height)) //Скроллинг вниз
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

	if (!m_Minimized) //Только для оригинального гампа
	{
		//Количество видимых линий
		int visibleLines = SkillGroupManager.GetVisibleLinesCount();

		//Максимальная позиция скроллера
		int maxScrollerY = height - 104;

		//Скроллер захвачен
		if (g_GumpPressedScroller && g_LastObjectLeftMouseDown == ID_GS_SCROLLER) //Scroller pressed
		{
			//Текущая позиция скроллера
			int currentY = (g_MouseY - 10) - (int)g_GumpTranslateY - 72; //Scroller position

			scrollerY = CalculateScrollerAndTextPosition(m_CurrentLine, visibleLines, maxScrollerY, currentY);

			if (mode)
				m_FrameCreated = false;
		}
		else if (m_CurrentLine) //Или же, скроллер смещен, но не захвачеен
		{
			scrollerY = CalculateScrollerY(m_CurrentLine, visibleLines, maxScrollerY);

			//Если режим рисования - перерисуем фрэйм
			if (mode)
				m_FrameCreated = false;
		}
	}

	if (mode) //Отрисовка
	{
		//Если фрэйм не был создан - создаем
		if (!m_FrameCreated || g_GumpSelectElement || g_GumpMovingOffsetX || g_GumpMovingOffsetY) // || g_GumpPressed
			GenerateFrame();
		else if (m_FrameRedraw)
		{
			GenerateFrame();
			m_FrameRedraw = false;
		}

		glTranslatef(g_GumpTranslateX, g_GumpTranslateY, 0.0f);

		//Отобразим фрэйм
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

		if (m_Minimized) //Если гамп свернут
		{
			if (Orion->GumpPixelsInXY(0x0839, 0, 0))
			{
				g_LastSelectedObject = 0;
				g_LastSelectedGump = index;
			}

			g_MouseX = oldMouseX;
			g_MouseY = oldMouseY;

			return 0;
		}

		//Стандартный гамп
		//Ниже те же действия, что и в функции создания фрэйма, только без самого рисования, комменты можно посмотреть там

		int LSG = 0;

		if (Orion->GumpPixelsInXY(0x082D, 167, 0)) //Minimize
		{
			LSG = ID_GS_BUTTON_MINIMIZE;
			g_LastSelectedGump = index;
		}
		else if (Orion->GumpPixelsInXY(0x1F40, 0, 23)) //Top scroll
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}
		
		if (g_LastSelectedGump != index)
		{
			//Skills body
			int curposY = 59;

			while (true)
			{
				int deltaHeight = height - (curposY - 36);

				if (deltaHeight  < 70)
				{
					if (deltaHeight > 0)
					{
						if (Orion->GumpPixelsInXY(0x1F41, 21, curposY, 0, deltaHeight))
						{
							g_LastSelectedObject = 0;
							g_LastSelectedGump = index;
						}
					}

					break;
				}
				else if (Orion->GumpPixelsInXY(0x1F41, 21, curposY))
				{
					g_LastSelectedObject = 0;
					g_LastSelectedGump = index;

					break;
				}

				curposY += 70;

				deltaHeight = height - (curposY - 36);

				if (deltaHeight < 70)
				{
					if (deltaHeight > 0)
					{
						if (Orion->GumpPixelsInXY(0x1F42, 21, curposY, 0, deltaHeight))
						{
							g_LastSelectedObject = 0;
							g_LastSelectedGump = index;
						}
					}

					break;
				}
				else if (Orion->GumpPixelsInXY(0x1F42, 21, curposY))
				{
					g_LastSelectedObject = 0;
					g_LastSelectedGump = index;

					break;
				}

				curposY += 70;
			}
		}
		
		if (Orion->GumpPixelsInXY(0x1F43, 21, height + 34)) //Bottom scroll
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}
		
		if (Orion->GumpPixelsInXY(0x082E, 167, height + 66)) //Resize
		{
			LSG = ID_GS_BUTTON_RESIZE;
			g_LastSelectedGump = index;
		}
		else if (Orion->GumpPixelsInXY(0x0938, 226, 34)) //Real
			LSG = ID_GS_SHOW_REAL;
		else if (Orion->GumpPixelsInXY(0x0938, 280, 34)) //Cap
			LSG = ID_GS_SHOW_CAP;
		else if (Orion->GumpPixelsInXY(0x0824, 294, 56)) //^ button
			LSG = ID_GS_BUTTON_UP;
		else if (Orion->GumpPixelsInXY(0x0825, 294, height + 3)) //v button
			LSG = ID_GS_BUTTON_DOWN;
		else if (Orion->GumpPixelsInXY(0x0828, 296, 72 + scrollerY)) //Scroller
			LSG = ID_GS_SCROLLER;
		else if (Orion->PolygonePixelsInXY(60, height + 44, 80, 14))
			LSG = ID_GS_BUTTON_NEW_GROUP;
		
		int startIndex = m_CurrentLine;
		int currentIndex = 0;
		
		int drawX = 28;
		int drawY = 75;

		const int drawStep = 17;
		
		int boundsY = height;

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

					if (th.Width && Orion->PolygonePixelsInXY(drawX + 16, drawY, th.Width - 10, 14))
					{
						LSG = ID_GS_GROUP + groupIndex;
						break;
					}
				}

				if (group->GetMaximized())
				{
					if (canDraw)
					{
						if (group->GetCount() && Orion->PolygonePixelsInXY(drawX, drawY, 14, 14))
						{
							LSG = ID_GS_GROUP_MINIMIZE + groupIndex;

							break;
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
								if (Skills[idx].m_Button && Orion->GumpPixelsInXY(0x0837, drawX + 8, drawY))
								{
									LSG = ID_GS_SKILL_BUTTON + idx; //Button
									completedSearch = true;

									break;
								}

								TTextTexture &th = Skills[idx].m_Texture;
								int width = 150;

								if (th.Width > 150)
									width = th.Width;

								if (Orion->PolygonePixelsInXY(drawX + 22, drawY - 1, width, 14))
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

								if (Orion->GumpPixelsInXY(gumpID, drawX + 251, drawY - 1))
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
					if (group->GetCount() && Orion->PolygonePixelsInXY(drawX, drawY, 14, 14))
					{
						LSG = ID_GS_GROUP_MINIMIZE + groupIndex;

						break;
					}

					drawY += drawStep;
				}

				groupIndex++;
				group = group->m_Next;
			}
		}

		if (g_ShowGumpLocker && Orion->PolygonePixelsInXY(0, 0, 10, 14))
			g_LastSelectedObject = ID_GS_LOCK_MOVING;

		g_MouseX = oldMouseX;
		g_MouseY = oldMouseY;

		return LSG;
	}

	return 0;
}*/
//----------------------------------------------------------------------------------
CSkillGroupObject *CGumpSkills::GetGroupUnderCursor(int startIndex)
{
	//Получить группу под курсором
	WISP_GEOMETRY::CPoint2Di mouse = g_MouseManager.Position;

	//Если вышли за пределы гампа по оси X
	if (mouse.X < (m_X + 30) || mouse.X > (m_X + 250))
		return NULL; //Exit from bounds on X

	//Если назодимся в пределах гампа по оси Y
	if (mouse.Y > (m_Y + 72) && mouse.Y < (m_Y + m_Height)) //Bounds of Y
	{
		int currentIndex = 0; //Указатель на текущий индекс

		//Начало рисования
		int drawY = m_Y + 75;

		//Шаг рисования (в пикселях)
		const int drawStep = 17;

		//Максимальная координата Y для отображения
		int boundsY = m_Y + m_Height;

		//Указатель на группы
		CSkillGroupObject *group = g_SkillGroupManager.m_Groups;

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
			if (group->Maximized)
			{
				if (canDraw) //Если пригодно для отображения
				{
					drawY += (drawStep + 2); //Смещаем координаты рисования

					if (mouse.Y < drawY)
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
						if (mouse.Y < drawY)
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
				if (mouse.Y < drawY)
					return group;
			}

			group = group->m_Next; //Следующая группа
		}
	}

	//Ничего не нашлось
	return NULL;
}
//----------------------------------------------------------------------------------
/*void TGumpSkills::OnLeftMouseDown()
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
}*/
//----------------------------------------------------------------------------------
/*void TGumpSkills::OnLeftMouseUp()
{
	if (m_Minimized)
		return; //Не обрабатываем при свернутом гампе
	else if (g_LastObjectLeftMouseDown != g_LastSelectedObject)
	{
		//Если скилл перетаскивается - можно выполнить действие
		if (g_LastObjectLeftMouseDown < ID_GS_SKILL || g_LastObjectLeftMouseDown >= ID_GS_SKILL_STATE)
			return;
	}

	//Текущее количество тиков
	DWORD ticks = g_Ticks;

	if (g_LastObjectLeftMouseDown == ID_GS_BUTTON_MINIMIZE) //Сворачиваем гамп
	{
		m_Minimized = true;
		m_FrameCreated = false;
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
		m_FrameCreated = false;
	}
	else if (g_LastObjectLeftMouseDown == ID_GS_BUTTON_DOWN && m_LastChangedLineTime < ticks) //Скроллинг вниз
	{
		int visibleLines = SkillGroupManager.GetVisibleLinesCount();

		if (m_CurrentLine < visibleLines)
			m_CurrentLine++;
		else
			m_CurrentLine = visibleLines;

		m_LastChangedLineTime = ticks + SCROLL_LISTING_DELAY;
		m_FrameCreated = false;
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

			Orion->SkillStatusChange(idx, status);
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

			Orion->UseSkill(idx);
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
//----------------------------------------------------------------------------------
bool TGumpSkills::OnLeftMouseDoubleClick()
{
	if (m_Minimized) //При даблклике по мини-гампу - раскрываем его
	{
		m_Minimized = false;
		m_FrameCreated = false;

		return true;
	}

	return false;
}*/
//----------------------------------------------------------------------------------
void CGumpSkills::OnCharPress(const WPARAM &wParam, const LPARAM &lParam)
{
	g_EntryPointer->Insert(wParam);

	int val = g_FontManager.GetWidthA(6, g_EntryPointer->c_str(), g_EntryPointer->Length());

	if (val > 170)
		g_EntryPointer->Remove(true);
	else
		m_WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGumpSkills::OnKeyDown(const WPARAM &wParam, const LPARAM &lParam)
{
	/*if (g_EntryPointer != TextEntry)
	{
		if (wParam == VK_DELETE)
		{
			CSkillGroupObject *item = g_SkillGroupManager.m_Groups;

			while (item != NULL)
			{
				if (item->Selected == 1)
				{
					g_SkillGroupManager.Remove(item);
					m_FrameCreated = false;

					break;
				}

				item = item->m_Next;
			}
		}

		return;
	}

	//Обработчик нажатия клавишь
	switch (wParam)
	{
		case VK_RETURN:
		{
			CSkillGroupObject *item = g_SkillGroupManager.m_Groups;

			while (item != NULL)
			{
				if (item->Selected == 2)
				{
					if (TextEntry->Length() > 0)
						item->Name = TextEntry->c_str();
				}

				item->Selected = 0;

				if (!item->Name.length())
					item->Name = "NoNameGroup";

				item = item->m_Next;
			}

			if (g_ConfigManager.ConsoleNeedEnter)
				g_EntryPointer = NULL;
			else
				g_EntryPointer = &g_GameConsole;

			m_WantRedraw = true;

			break;
		}
		case VK_HOME:
		case VK_END:
		case VK_LEFT:
		case VK_RIGHT:
		case VK_BACK:
		case VK_DELETE:
		{
			g_EntryPointer->OnKey(this, wParam);

			break;
		}
		default:
			break;
	}*/
}
//----------------------------------------------------------------------------------
