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
//Класс для работы с фрэймбуфером
class TFrameBuffer
{
private:
	//Габариты буфера
	int m_Width;
	int m_Height;

	//Создан буфер и готов к использованию или нет
	bool m_Ready;

	//Указатель на предыдущий фрэймбуфер
	GLint m_OldFrameBuffer;

	//Указатель на фрэймбуфер
	GLuint m_FrameBuffer;

	//Указатель на текстуру фрэймбуфера
	GLuint m_Texture;
public:
	TFrameBuffer();
	~TFrameBuffer();

	//Инициализациия
	bool Init(int width, int height);

	//Очистка фрэймбуфера
	void Free();

	//Завершение использования фрэймбуфера
	void Release();

	//Готов или нет буфер
	bool Ready() const { return m_Ready; }

	//Проверка готовности буфера с потенциальным пересозданием
	bool Ready(int &width, int &height);

	//Использование буфера
	bool Use();

	//Отрисовать текстуру буфера
	void Draw(int x, int y);
};
//---------------------------------------------------------------------------
extern TFrameBuffer g_LightBuffer;
//---------------------------------------------------------------------------
#endif