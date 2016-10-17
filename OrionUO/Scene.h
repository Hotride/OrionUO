/***********************************************************************************
**
** Scene.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef SCENE_H
#define SCENE_H
//----------------------------------------------------------------------------------
#include "Globals.h"
#include "EnumList.h"
//----------------------------------------------------------------------------------
class CScene
{
protected:
	//!Индекс действия, которое необходимо совершить после окончания плавного перехода затемненного состояния экрана
	uchar m_SmoothScreenAction;

public:
	CScene() : m_SmoothScreenAction(0) {}
	virtual ~CScene() {}

	/*!
	Инициализация
	@return 
	*/
	virtual void Init(const GAME_STATE &state);

	/*!
	Инициализация всплывающих подсказок
	@return 
	*/
	virtual void InitToolTip();

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
	virtual void ProcessSmoothAction(uchar action = 0xFF);

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



	/*!
	Нажатие левой кнопки мыши
	@return 
	*/
	virtual void OnLeftMouseButtonDown();

	/*!
	Отпускание левой кнопки мыши
	@return 
	*/
	virtual void OnLeftMouseButtonUp();

	/*!
	Нажатие правой кнопки мыши
	@return 
	*/
	virtual void OnRightMouseButtonDown();

	/*!
	Отпускание правой кнопки мыши
	@return 
	*/
	virtual void OnRightMouseButtonUp();

	/*!
	Двойной клик левой кнопкой мыши
	@return true при успешной обработке
	*/
	virtual bool OnLeftMouseButtonDoubleClick();

	/*!
	Двойной клик правой кнопкой мыши
	@return true при успешной обработке
	*/
	virtual bool OnRightMouseButtonDoubleClick();

	/*!
	Обработка средней кнопки (колесика) мыши
	@param [__in] state Состояние колесика
	@return 
	*/
	virtual void OnMouseWheel(const MOUSE_WHEEL_STATE &state);

	virtual bool OnMouseWheelDoubleClick();

	virtual void OnDragging();

	virtual void OnDragDrop();

	/*!
	Обработка нажатия клавиши
	@param [__in] wparam не подписанный параметр
	@param [__in] lparam не подписанный параметр
	@return 
	*/
	virtual void OnCharPress(const WPARAM &wparam, const LPARAM &lparam);

	/*!
	Обработка нажатия клавиши
	@param [__in] wparam не подписанный параметр
	@param [__in] lparam не подписанный параметр
	@return
	*/
	virtual void OnKeyPress(const WPARAM &wparam, const LPARAM &lparam);
};
//----------------------------------------------------------------------------------
extern CScene g_Scene;
//----------------------------------------------------------------------------------
#endif
