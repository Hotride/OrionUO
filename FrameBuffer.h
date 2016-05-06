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
//!Класс для работы с фрэймбуфером
class TFrameBuffer
{
private:
	//!Габариты буфера
	int m_Width;
	int m_Height;

	//!Создан буфер и готов к использованию или нет
	bool m_Ready;

	//!Указатель на предыдущий фрэймбуфер
	GLint m_OldFrameBuffer;

	//!Указатель на фрэймбуфер
	GLuint m_FrameBuffer;

	//!Указатель на текстуру фрэймбуфера
	GLuint m_Texture;

public:
	TFrameBuffer();
	~TFrameBuffer();

	/*!
	Инициализациия буфера
	@param [__in] width Ширина буфера
	@param [__in] height Высота буфера
	@return true в случае успеха
	*/
	bool Init(__in int width, __in int height);

	/*!
	Очистка фрэймбуфера
	@return 
	*/
	void Free();

	/*!
	Завершение использования фрэймбуфера
	@return 
	*/
	void Release();

	/*!
	Готов или нет буфер
	@return true в случае готовности
	*/
	bool Ready() const { return m_Ready; }

	/*!
	Проверка готовности буфера с потенциальным пересозданием
	@param [__in] width Ширина буфера
	@param [__in] height Высота буфера
	@return true в случае готовности
	*/
	bool Ready(__in int &width, __in int &height);

	/*!
	Использование буфера
	@return true в случае успеха
	*/
	bool Use();

	/*!
	Отрисовать текстуру буфера
	@param [__in] x Экранная координата X
	@param [__in] y Экранная координата Y
	@return 
	*/
	void Draw(__in int x, __in int y);

	void DrawShadow(__in int x, __in int y);
};
//---------------------------------------------------------------------------
//!Буфер для освещения
extern TFrameBuffer g_LightBuffer;
extern TFrameBuffer g_CharacterBuffer;
//---------------------------------------------------------------------------
#endif