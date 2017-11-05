/***********************************************************************************
**
** GumpPropertyIcon.h
**
** Copyright (C) November 2017 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPPROPERTYICON_H
#define GUMPPROPERTYICON_H
//----------------------------------------------------------------------------------
class CGumpPropertyIcon : public CGump
{
	SETGETE(wstring, Text, L"No Data");
	SETGETP(CRenderObject*, Object, NULL);

private:
	static const int ID_GPI_LOCK_MOVING = 1;
	static const int ID_GPI_MINIMIZE = 2;

	CGLTextTexture m_Texture;

public:
	CGumpPropertyIcon(const int &x, const int &y);
	virtual ~CGumpPropertyIcon();

	virtual void PrepareContent();

	virtual void UpdateContent();

	GUMP_BUTTON_EVENT_H;

	virtual bool OnLeftMouseButtonDoubleClick();
};
//----------------------------------------------------------------------------------
#endif //GUMPPROPERTYICON_H
//----------------------------------------------------------------------------------
