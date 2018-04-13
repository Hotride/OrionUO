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
public:
	uint Timer = 0;
	wstring Data = L"";
	uint ClilocID = 0;
	int MaxWidth = 0;
	WISP_GEOMETRY::CPoint2Di Position = WISP_GEOMETRY::CPoint2Di();
	bool Use = false;

	CToolTip();
	~CToolTip();

	void Reset();

	class CRenderObject *m_Object{ NULL };

	//Текстура тулпита
	CGLTextTexture Texture;

	void CreateTextTexture(CGLTextTexture &texture, const wstring &str, int &width, const int &minWidth);

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
