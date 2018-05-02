/***********************************************************************************
**
** GLFrameBuffer.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GLFRAMEBUFFER_H
#define GLFRAMEBUFFER_H
//----------------------------------------------------------------------------------
//!Класс для работы с фрэймбуфером
class CGLFrameBuffer
{
private:
    //!Создан буфер и готов к использованию или нет
    bool m_Ready{ false };

    //!Указатель на предыдущий фрэймбуфер
    GLint m_OldFrameBuffer{ 0 };

    //!Указатель на фрэймбуфер
    GLuint m_FrameBuffer{ 0 };

public:
    CGLFrameBuffer();
    ~CGLFrameBuffer();

    //!Указатель на текстуру фрэймбуфера
    CGLTexture Texture{ CGLTexture() };

    /*!
	Инициализациия буфера
	@param [__in] width Ширина буфера
	@param [__in] height Высота буфера
	@return true в случае успеха
	*/
    bool Init(int width, int height);

    bool Init(const WISP_GEOMETRY::CSize &size) { return Init(size.Width, size.Height); }

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
    bool Ready(int width, int height);

    bool Ready(const WISP_GEOMETRY::CSize &size) { return Ready(size.Width, size.Height); }

    bool ReadyMinSize(int width, int height);

    bool ReadyMinSize(const WISP_GEOMETRY::CSize &size) { return Ready(size.Width, size.Height); }

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
    void Draw(int x, int y);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
