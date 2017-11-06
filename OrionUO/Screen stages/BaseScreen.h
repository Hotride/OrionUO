/***********************************************************************************
**
** BaseScreen.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef BASESCREEN_H
#define BASESCREEN_H
//----------------------------------------------------------------------------------
//Базовый класс стадий окна клиента
class CBaseScreen : public CBaseQueue
{
	//!Индекс действия, которое необходимо совершить после окончания плавного перехода затемненного состояния экрана
	SETGET(uchar, SmoothScreenAction, 0);
	SETGET(ushort, CursorGraphic, 0x2073);

protected:
	CGump &m_Gump;

public:
	CBaseScreen(CGump &gump);
	virtual ~CBaseScreen() {}

	virtual void PrepareContent() { m_Gump.PrepareContent(); }

	virtual void UpdateContent() { m_Gump.UpdateContent(); }

	/*!
	Инициализация
	@return 
	*/
	virtual void Init() {}

	/*!
	Инициализация всплывающих подсказок
	@return 
	*/
	virtual void InitToolTip() { m_Gump.InitToolTip(); }

	/*!
	Отрисовка/выбор объектов
	@param [__in] mode true - отрисовка, false - выбор
	@return При выборе объектов - идентификатор выбранного объекта
	*/
	virtual void Render(const bool &mode);

	/*!
	Создание плавного затемнения экрана
	@param [__in] action Идентификатор действия
	@return 
	*/
	virtual void CreateSmoothAction(uchar action);

	/*!
	Обработка события после перехода
	@param [__in_opt] action Идентификатор действия
	@return 
	*/
	virtual void ProcessSmoothAction(uchar action = 0xFF) {}

	/*!
	Вычисление состояния перехода
	@return Индекс состояния
	*/
	virtual int DrawSmoothMonitor();

	/*!
	Наложение эффекта перехода
	@return 
	*/
	virtual void DrawSmoothMonitorEffect();



	virtual void OnLeftMouseButtonDown() { m_Gump.OnLeftMouseButtonDown(); }
	virtual void OnLeftMouseButtonUp() { m_Gump.OnLeftMouseButtonUp(); m_Gump.WantRedraw = true; }
	virtual bool OnLeftMouseButtonDoubleClick() { return m_Gump.OnLeftMouseButtonDoubleClick(); }
	virtual void OnRightMouseButtonDown() { m_Gump.OnRightMouseButtonDown(); }
	virtual void OnRightMouseButtonUp() { m_Gump.OnRightMouseButtonUp(); }
	virtual bool OnRightMouseButtonDoubleClick() { return m_Gump.OnRightMouseButtonDoubleClick(); }
	virtual void OnMidMouseButtonDown() { m_Gump.OnMidMouseButtonDown(); }
	virtual void OnMidMouseButtonUp() { m_Gump.OnMidMouseButtonUp(); }
	virtual bool OnMidMouseButtonDoubleClick() { return m_Gump.OnMidMouseButtonDoubleClick(); }
	virtual void OnMidMouseButtonScroll(const bool &up) { m_Gump.OnMidMouseButtonScroll(up); }
	virtual void OnDragging() { m_Gump.OnDragging(); }
	virtual void OnCharPress(const WPARAM &wParam, const LPARAM &lParam) { m_Gump.OnCharPress(wParam, lParam); }
	virtual void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam) { m_Gump.OnKeyDown(wParam, lParam); }
	virtual void OnKeyUp(const WPARAM &wParam, const LPARAM &lParam) { m_Gump.OnKeyUp(wParam, lParam); }
};
//----------------------------------------------------------------------------------
//!Указатель на текущий экран
extern CBaseScreen *g_CurrentScreen;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
