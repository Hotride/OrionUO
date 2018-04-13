/***********************************************************************************
**
** Gump.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMP_H
#define GUMP_H
//----------------------------------------------------------------------------------
class CBaseGUI;
//----------------------------------------------------------------------------------
#define GUMP_BUTTON_EVENT_C OnButton(const uint &serial)
#define GUMP_CHECKBOX_EVENT_C OnCheckbox(const uint &serial, const bool &state)
#define GUMP_RADIO_EVENT_C OnRadio(const uint &serial, const bool &state)
#define GUMP_TEXT_ENTRY_EVENT_C OnTextEntry(const uint &serial)
#define GUMP_SLIDER_CLICK_EVENT_C OnSliderClick(const uint &serial)
#define GUMP_SLIDER_MOVE_EVENT_C OnSliderMove(const uint &serial)
#define GUMP_RESIZE_START_EVENT_C OnResizeStart(const uint &serial)
#define GUMP_RESIZE_EVENT_C OnResize(const uint &serial)
#define GUMP_RESIZE_END_EVENT_C OnResizeEnd(const uint &serial)
#define GUMP_DIRECT_HTML_LINK_EVENT_C OnDirectHTMLLink(const ushort &link)
#define GUMP_COMBOBOX_SELECTION_EVENT_C OnComboboxSelection(const uint &serial)
#define GUMP_SCROLL_BUTTON_EVENT_C OnScrollButton()
//----------------------------------------------------------------------------------
#define GUMP_BUTTON_EVENT_H virtual void GUMP_BUTTON_EVENT_C
#define GUMP_CHECKBOX_EVENT_H virtual void GUMP_CHECKBOX_EVENT_C
#define GUMP_RADIO_EVENT_H virtual void GUMP_RADIO_EVENT_C
#define GUMP_TEXT_ENTRY_EVENT_H virtual void GUMP_TEXT_ENTRY_EVENT_C
#define GUMP_SLIDER_CLICK_EVENT_H virtual void GUMP_SLIDER_CLICK_EVENT_C
#define GUMP_SLIDER_MOVE_EVENT_H virtual void GUMP_SLIDER_MOVE_EVENT_C
#define GUMP_RESIZE_START_EVENT_H virtual void GUMP_RESIZE_START_EVENT_C
#define GUMP_RESIZE_EVENT_H virtual void GUMP_RESIZE_EVENT_C
#define GUMP_RESIZE_END_EVENT_H virtual void GUMP_RESIZE_END_EVENT_C
#define GUMP_DIRECT_HTML_LINK_EVENT_H virtual void GUMP_DIRECT_HTML_LINK_EVENT_C
#define GUMP_COMBOBOX_SELECTION_EVENT_H virtual void GUMP_COMBOBOX_SELECTION_EVENT_C
#define GUMP_SCROLL_BUTTON_EVENT_H virtual void GUMP_SCROLL_BUTTON_EVENT_C
//----------------------------------------------------------------------------------
//!Базовый класс гампов
class CGump : public CRenderObject
{
public:
	GUMP_TYPE GumpType = GT_NONE;
	uint ID = 0;
	int MinimizedX = 0;
	int MinimizedY = 0;
	bool NoMove = false;
	bool NoClose = false;
	bool Minimized = false;
	bool FrameCreated = false;
	bool WantRedraw = false;
	bool WantUpdateContent = true;
	bool Blocked = false;
	bool LockMoving = false;
	int Page = 0;
	int Draw2Page = 0;
	bool Transparent = false;
	bool RemoveMark = false;
	bool NoProcess = false;
	bool Visible = true;
	WISP_GEOMETRY::CRect GumpRect = WISP_GEOMETRY::CRect();

protected:
	CGUIButton m_Locker{ CGUIButton(0, 0, 0, 0, 0, 0) };

	CGLFrameBuffer m_FrameBuffer{ CGLFrameBuffer() };

	virtual void CalculateGumpState();

	virtual void RecalculateSize();

public:
	CGump();
	CGump(GUMP_TYPE type, uint serial, int x, int y);
	virtual ~CGump();

	virtual void PasteClipboardData(wstring &data);

	static void ProcessListing();

	static bool ApplyTransparent(CBaseGUI *item, int page, const int &currentPage, const int draw2Page = 0);

	static void DrawItems(CBaseGUI *start, const int &currentPage, const int &draw2Page = 0);

	static class CRenderObject *SelectItems(CBaseGUI *start, const int &currentPage, const int &draw2Page = 0);

	static void GetItemsSize(CGump *gump, CBaseGUI *start, WISP_GEOMETRY::CPoint2Di &minPosition, WISP_GEOMETRY::CPoint2Di &maxPosition, WISP_GEOMETRY::CPoint2Di &offset, int count, const int &currentPage, const int &draw2Page);
	static void TestItemsLeftMouseDown(CGump *gump, CBaseGUI *start, const int &currentPage, const int &draw2Page = 0, int count = -1);
	static void TestItemsLeftMouseUp(CGump *gump, CBaseGUI *start, const int &currentPage, const int &draw2Page = 0);
	static void TestItemsDragging(CGump *gump, CBaseGUI *start, const int &currentPage, const int &draw2Page = 0, int count = -1);
	static void TestItemsScrolling(CGump *gump, CBaseGUI *start, const bool &up, const int &currentPage, const int &draw2Page = 0);

	virtual void DelayedClick(class CRenderObject *obj) {}

	virtual void PrepareContent() {}

	virtual void UpdateContent() {}

	virtual class CTextRenderer *GetTextRenderer() { return NULL; }

	virtual void PrepareTextures();

	virtual void GenerateFrame(const bool &stop);

	virtual bool CanBeDisplayed() { return true; }

	void FixCoordinates();

	bool CanBeMoved();

	void DrawLocker();

	bool SelectLocker();

	bool TestLockerClick();

	virtual bool EntryPointerHere();

	virtual void Draw();

	virtual class CRenderObject *Select();

	virtual void InitToolTip() {}



	GUMP_BUTTON_EVENT_H { DebugMsg("OnButton(%i)\n", serial); }
	GUMP_CHECKBOX_EVENT_H { DebugMsg("OnCheckbox(%i, %i)\n", serial, state); }
	GUMP_RADIO_EVENT_H { DebugMsg("OnRadio(%i, %i)\n", serial, state); }
	GUMP_TEXT_ENTRY_EVENT_H { DebugMsg("OnTextEntry(%i)\n", serial); }
	GUMP_SLIDER_CLICK_EVENT_H { DebugMsg("OnSliderClick(%i)\n", serial); }
	GUMP_SLIDER_MOVE_EVENT_H { DebugMsg("OnSliderMove(%i)\n", serial); }
	GUMP_RESIZE_START_EVENT_H { DebugMsg("OnResizeStart(%i)\n", serial); }
	GUMP_RESIZE_EVENT_H { DebugMsg("OnResize(%i)\n", serial); }
	GUMP_RESIZE_END_EVENT_H { DebugMsg("OnResizeEnd(%i)\n", serial); }
	GUMP_DIRECT_HTML_LINK_EVENT_H;
	GUMP_COMBOBOX_SELECTION_EVENT_H { DebugMsg("OnComboboxSelection(%i)\n", serial); }
	GUMP_SCROLL_BUTTON_EVENT_H{ DebugMsg("OnScrollButton()\n"); }



	virtual void OnLeftMouseButtonDown();
	virtual void OnLeftMouseButtonUp();
	virtual bool OnLeftMouseButtonDoubleClick() { return false; }
	virtual void OnRightMouseButtonDown() {}
	virtual void OnRightMouseButtonUp() {}
	virtual bool OnRightMouseButtonDoubleClick() { return false; }
	virtual void OnMidMouseButtonDown() {}
	virtual void OnMidMouseButtonUp() {}
	virtual bool OnMidMouseButtonDoubleClick() { return false; }
	virtual void OnMidMouseButtonScroll(const bool &up);
	virtual void OnDragging();
	virtual void OnCharPress(const WPARAM &wParam, const LPARAM &lParam) {}
	virtual void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam) {}
	virtual void OnKeyUp(const WPARAM &wParam, const LPARAM &lParam) {}
 };
 //----------------------------------------------------------------------------------
//!Ссылка на гамп, который в данный момент изменяет высоту
 extern CGump *g_ResizedGump;
 extern CGump *g_CurrentCheckGump;
 //----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
