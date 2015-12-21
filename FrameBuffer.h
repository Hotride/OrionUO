/****************************************************************************
**
** FrameBuffer.h
**
** Copyright (C) December 2015 Hotride
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
#ifndef FrameBufferH
#define FrameBufferH
//---------------------------------------------------------------------------
class TFrameBuffer
{
private:
	int m_Width;
	int m_Height;

	bool m_Ready;
	
	GLint m_OldFrameBuffer;
	GLuint m_FrameBuffer;
	GLuint m_Texture;
public:
	TFrameBuffer();
	~TFrameBuffer();
	
	bool Init(int width, int height);
	void Free();
	void Release();
	
	bool Ready(int &width, int &height);
	bool Use();
	void Draw(float X, float Y);
};
//---------------------------------------------------------------------------
#endif