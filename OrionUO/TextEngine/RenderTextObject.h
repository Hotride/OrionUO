/***********************************************************************************
**
** RenderTextObject.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef RENDERTEXTOBJECT_H
#define RENDERTEXTOBJECT_H
//----------------------------------------------------------------------------------
//Класс текста объекта мира
class CRenderTextObject : public CRenderObject
{
public:
	bool Transparent = false;

	CRenderTextObject();
	virtual ~CRenderTextObject();

	CRenderTextObject *m_NextDraw{ NULL };
	CRenderTextObject *m_PrevDraw{ NULL };

	//Вытащить из очереди
	void UnlinkDraw();

	//Наверх очереди
	void ToTop();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
