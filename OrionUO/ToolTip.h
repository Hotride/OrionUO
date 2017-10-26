/***********************************************************************************
**
** ToolTip.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef TOOLTIP_H
#define TOOLTIP_H
//----------------------------------------------------------------------------------
//Класс для работы с тултипами
class CToolTip
{
	SETGET(uint, Timer, 0);
	SETGET(wstring, Data, L"");
	SETGET(uint, ClilocID, 0);
	SETGET(int, MaxWidth, 0);
	SETGET(WISP_GEOMETRY::CPoint2Di, Position, WISP_GEOMETRY::CPoint2Di());
	SETGET(bool, Use, false);

public:
	CToolTip();
	~CToolTip();

	void Reset();

	class CRenderObject *m_Object{ NULL };

	//Текстура тулпита
	CGLTextTexture Texture;

	//Установить тултип из строки
	void Set(const wstring &str, const int &maxWidth = 0);

	//Установить тултип из клилока
	void Set(const uint &clilocID, const string &str, const int &maxWidth = 0, const bool &toCamelCase = false);

	//Отрисовать тултип
	void Draw(const int &cursorWidth = 0, const int &cursorHeight = 0);
};
//----------------------------------------------------------------------------------
extern CToolTip g_ToolTip;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
