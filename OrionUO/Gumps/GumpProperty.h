/***********************************************************************************
**
** GumpProperty.h
**
** Copyright (C) November 2017 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPPROPERTY_H
#define GUMPPROPERTY_H
//----------------------------------------------------------------------------------
class CGumpProperty : public CGump
{
	SETGET(uint, Timer, 0);

private:
	CGLTextTexture m_Texture;

public:
	CGumpProperty(const wstring &text);
	virtual ~CGumpProperty();

	virtual void PrepareContent();

	virtual class CRenderObject *Select() { return NULL; }
};
//----------------------------------------------------------------------------------
#endif //GUMPPROPERTY_H
//----------------------------------------------------------------------------------
