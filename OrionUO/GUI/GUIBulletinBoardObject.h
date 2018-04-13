/***********************************************************************************
**
** GUIBulletinBoardObject.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIBULLETINBOARDOBJECT_H
#define GUIBULLETINBOARDOBJECT_H
//----------------------------------------------------------------------------------
class CGUIBulletinBoardObject : public CBaseGUI
{
	wstring Text = L"";

private:
	CGLTextTexture m_Texture;

public:
	CGUIBulletinBoardObject(const uint &serial, const int &x, const int &y, const wstring &text);
	virtual ~CGUIBulletinBoardObject();

	virtual void PrepareTextures();

	virtual void Draw(const bool &checktrans = false);
	virtual bool Select();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
