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
#include "Globals.h"
//----------------------------------------------------------------------------------
class CRenderTextObject;
//----------------------------------------------------------------------------------
class CImageBounds
{
	SETGET(int, X);
	SETGET(int, Y);
	SETGET(int, Width);
	SETGET(int, Height);

public:
	CImageBounds(const int &x, const int &y, const int &width, const int &height);
	virtual ~CImageBounds();

	bool InRect(const CImageBounds &ib);
};
//----------------------------------------------------------------------------------
class CTextImageBounds : public CImageBounds
{
public:
	CTextImageBounds(const int &x, const int &y, const int &width, const int &height, CRenderTextObject *text);
	virtual ~CTextImageBounds();

	CRenderTextObject *m_Text;
};
//----------------------------------------------------------------------------------
extern CImageBounds g_PlayerRect;
//----------------------------------------------------------------------------------
#endif
