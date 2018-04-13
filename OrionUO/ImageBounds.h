/***********************************************************************************
**
** ImageBounds.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef IMAGEBOUNDS_H
#define IMAGEBOUNDS_H
//----------------------------------------------------------------------------------
class CRenderTextObject;
//----------------------------------------------------------------------------------
class CImageBounds
{
public:
	int X = 0;
	int Y = 0;
	int Width = 0;
	int Height = 0;

	CImageBounds(const int &x, const int &y, const int &width, const int &height);
	virtual ~CImageBounds();

	bool InRect(const CImageBounds &ib);
};
//----------------------------------------------------------------------------------
class CTextImageBounds : public CImageBounds
{
public:
	CTextImageBounds(const int &x, const int &y, const int &width, const int &height, CRenderTextObject *text);
	CTextImageBounds(class CTextData *text);
	virtual ~CTextImageBounds();

	CRenderTextObject *Text{ NULL };
};
//----------------------------------------------------------------------------------
extern CImageBounds g_PlayerRect;
//----------------------------------------------------------------------------------
#endif
