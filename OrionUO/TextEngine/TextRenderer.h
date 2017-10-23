/***********************************************************************************
**
** TextRenderer.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H
//----------------------------------------------------------------------------------
//Класс для обработки вывода текста на экран
class CTextRenderer : public CRenderTextObject
{
private:
	//Список координат, занятых текстом
	deque<CTextImageBounds> m_TextRect;

	bool ProcessTextRemoveBlending(CTextData &text);

public:
	CTextRenderer();
	virtual ~CTextRenderer();

	//Список текста
	CRenderTextObject *m_TextItems{ NULL };

	CRenderTextObject *m_DrawPointer{ NULL };

	//Добавить текст
	CRenderTextObject *AddText(CRenderTextObject *obj);

	//Переместить текст наверх очереди
	void ToTop(CRenderTextObject *obj);

	//Добавить координаты текста
	void AddRect(const CTextImageBounds &rect) { m_TextRect.push_back(rect); }

	//Очистить список координат текста
	void ClearRect() { m_TextRect.clear(); }

	//Проверка, есть ли текст в координатах
	bool InRect(CTextData *text, CRenderWorldObject *rwo);

	bool CalculatePositions(const bool &noCalculate);

	void Draw();

	void Select(class CGump *gump);

	bool CalculateWorldPositions(const bool &noCalculate);

	void WorldDraw();
};
//----------------------------------------------------------------------------------
extern CTextRenderer g_WorldTextRenderer;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
