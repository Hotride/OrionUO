/****************************************************************************
**
** SelectProfessionScreen.h
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
#ifndef SelectProfessionScreenH
#define SelectProfessionScreenH
//---------------------------------------------------------------------------
class TSelectProfessionScreen : public TBaseScreen
{
private:
	//Смещение в пикселях отображаемой информации в скроллбоксе
	int m_PixelOffset;

	//Последнее время изменения положения информации в скроллбоксе
	DWORD m_LastScrollChangeTime;

	//Выбранный навык
	int m_SkillSelection;

	//Идентификаторы событий для плавного перехода
	static const BYTE ID_SMOOTH_SPS_QUIT = 1;
	static const BYTE ID_SMOOTH_SPS_GO_SCREEN_CHARACTER = 2;
	static const BYTE ID_SMOOTH_SPS_GO_SCREEN_GAME_CONNECT = 3;
	static const BYTE ID_SMOOTH_SPS_GO_SCREEN_CREATE = 4;

	//Изменение ползунков характеристик
	void ShuffleStats(int x, int maxSum, int maxVal);

	//Изменение ползунков навыков
	void ShuffleSkills(int x);

	//Пролистать список
	void ListingList(bool direction, int divizor = 1);

	//Получить высоту скроллбокса
	int GetScrollBoxHeight();

	//Объекты текста
	TTextTexture m_Text;
	TTextTexture m_TextStat[3];
	TTextTexture m_TextSkillInList[56][2];

	//Идентификаторы кнопок
	static const int ID_SPS_QUIT = 1;
	static const int ID_SPS_ARROW_PREV = 2;
	static const int ID_SPS_ARROW_NEXT = 3;
	static const int ID_SPS_ARROW_BACK_PROFESSION = 4;
	static const int ID_SPS_LABEL_BACK_PROFESSION = 5;
	static const int ID_SPS_SCROLLBAR_UP = 6;
	static const int ID_SPS_SCROLLBAR_DOWN = 7;
	static const int ID_SPS_SCROLLBAR = 8;
	static const int ID_SPS_SCROLLBAR_BACKGROUND = 9;
	static const int ID_SPS_STATS_SPHERE = 10;
	static const int ID_SPS_SKILLS_SPHERE = 13;
	static const int ID_SPS_SKILLS_FILED = 16;
	static const int ID_SPS_LABEL = 20;
	static const int ID_SPS_SKILLS_LIST = 50;
	
	int RenderOld(bool &mode);
	int RenderNew(bool &mode);
public:
	TSelectProfessionScreen();
	virtual ~TSelectProfessionScreen();

	//Инициализация
	void Init();

	//Обработка события после плавного затемнения экрана
	void ProcessSmoothAction(BYTE action = 0xFF);

	//Инициализация тултипа
	void InitPopupHelp();

	//Рисование экрана
	int Render(bool mode);

	//События
	void OnLeftMouseDown();
	void OnLeftMouseUp();
	void OnMouseWheel(MOUSE_WHEEL_STATE state);
};
//---------------------------------------------------------------------------
extern TSelectProfessionScreen *SelectProfessionScreen;
//---------------------------------------------------------------------------
#endif