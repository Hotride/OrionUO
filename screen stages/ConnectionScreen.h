/****************************************************************************
**
** ConnectionScreen.h
**
** Copyright (C) September 2015 Hotride
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
*****************************************************************************
*/
//---------------------------------------------------------------------------
#ifndef ConnectionScreenH
#define ConnectionScreenH
//---------------------------------------------------------------------------
class TConnectionScreen : public TBaseScreen
{
private:
	bool m_ConnectionFailed;
	bool m_Connected;
	bool m_Completed;
	int m_ErrorCode;
	CONNECTION_SCREEN_TYPE m_Type;
	
	TTextTexture m_Text[32];
	
	static const int ID_CS_OK = 1;
	static const int ID_CS_CANCEL = 2;

public:
	TConnectionScreen();
	~TConnectionScreen();
	
	SETGET(bool, ConnectionFailed)
	SETGET(bool, Connected)
	SETGET(bool, Completed)
	SETGET(int, ErrorCode)
	SETGET(CONNECTION_SCREEN_TYPE, Type)

	void Init();

	int Render(bool mode);
	
	void OnLeftMouseDown();
	void OnLeftMouseUp();
	void OnKeyPress(WPARAM wparam, LPARAM lparam);
};

extern TConnectionScreen *ConnectionScreen;
//---------------------------------------------------------------------------
#endif