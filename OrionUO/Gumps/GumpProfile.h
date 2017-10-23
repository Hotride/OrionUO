/***********************************************************************************
**
** GumpProfile.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPPROFILE_H
#define GUMPPROFILE_H
//----------------------------------------------------------------------------------
class CGumpProfile : public CGumpBaseScroll
{
private:
	static const int ID_GP_APPLY = 1;
	static const int ID_GP_TEXT_FIELD = 2;
	
	CGUITextEntry *m_Entry{ NULL };
	CGUIHitBox *m_HitBox{ NULL };
	CBaseGUI *m_BottomData[4];

	void RecalculateHeight();

public:
	CGumpProfile(uint serial, short x, short y, wstring topText, wstring bottomText, wstring dataText);
	virtual ~CGumpProfile();

	GUMP_BUTTON_EVENT_H;

	virtual bool OnLeftMouseButtonDoubleClick();
	virtual void OnCharPress(const WPARAM &wParam, const LPARAM &lParam);
	virtual void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
