/***********************************************************************************
**
** GumpPaperdoll.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPPAPERDOLL_H
#define GUMPPAPERDOLL_H
//----------------------------------------------------------------------------------
class CGumpPaperdoll : public CGump
{
	SETGET(bool, CanLift, false);

protected:
	static const int ID_GP_BUTTON_HELP = 1;
	static const int ID_GP_BUTTON_OPTIONS = 2;
	static const int ID_GP_BUTTON_LOGOUT = 3;
	static const int ID_GP_BUTTON_JOURNAL_OR_QUESTS = 4;
	static const int ID_GP_BUTTON_SKILLS = 5;
	static const int ID_GP_BUTTON_CHAT_OR_GUILD = 6;
	static const int ID_GP_BUTTON_WARMODE = 7;
	static const int ID_GP_BUTTON_STATUS = 8;
	static const int ID_GP_BUTTON_MINIMIZE = 9;
	static const int ID_GP_PROFILE_SCROLL = 10;
	static const int ID_GP_PARTY_MANIFEST_SCROLL = 11;
	static const int ID_GP_BUTTON_VIRTURE = 12;
	static const int ID_GP_COMBAT_BOOK = 13;
	static const int ID_GP_RACIAL_ABILITIES_BOOK = 14;
	static const int ID_GP_LOCK_MOVING = 15;

	CTextRenderer m_TextRenderer{ CTextRenderer() };
	
	static const int m_LayerCount = 23;
	static int UsedLayers[m_LayerCount];

	CGUIDataBox *m_DataBox{ NULL };
	CGUIText *m_Description{ NULL };

	bool m_WantTransparentContent{ false };

	virtual void CalculateGumpState();

public:
	CGumpPaperdoll(uint serial, short x, short y, bool minimized);
	virtual ~CGumpPaperdoll();

	CGUIButton *m_ButtonWarmode{ NULL };

	CTextContainer m_TextContainer{ CTextContainer(10) };

	void UpdateDescription(const string &text);

	CTextRenderer *GetTextRenderer() {return &m_TextRenderer;}

	static const int ID_GP_ITEMS = 20;

	virtual void DelayedClick(CRenderObject *obj);

	virtual void PrepareContent();

	virtual void UpdateContent();

	virtual void InitToolTip();

	virtual void Draw();
	virtual CRenderObject *Select();

	GUMP_BUTTON_EVENT_H;
	virtual void OnLeftMouseButtonUp();
	virtual bool OnLeftMouseButtonDoubleClick();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
