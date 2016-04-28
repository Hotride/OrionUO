/****************************************************************************
**
** TextureObject.h
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
#ifndef TextureObjectH
#define TextureObjectH
//---------------------------------------------------------------------------
//Включение/выключение сохранения данных в текстуру (прирост скорости обработки в ущерб занимаемой приложением памяти или наоборот)
#define UO_ENABLE_TEXTURE_DATA_SAVING 0
//---------------------------------------------------------------------------
//Класс для работы с текстурой
class TTextureObject
{
protected:
	//Габариты текстуры
	int m_Width;
	int m_Height;
public:
	TTextureObject();
	virtual ~TTextureObject();

	SETGET(int, Width);
	SETGET(int, Height);

	//Индекс текстуры
	GLuint Texture;

#if UO_ENABLE_TEXTURE_DATA_SAVING == 1
	//Данные текстуры (0/1)
	PBYTE Data;
#endif
};
//---------------------------------------------------------------------------
//Класс для работы с текстурой круга прозрачности
class TCircleOfTransparencyTexture : public TTextureObject
{
protected:
	//Радиус круга
	int m_Radius;

	//Координаты отрисовки круга
	int m_X;
	int m_Y;
public:
	TCircleOfTransparencyTexture();
	virtual ~TCircleOfTransparencyTexture();
	
	SETGET(int, Radius);
	SETGET(int, X);
	SETGET(int, Y);

	//Создать текстуру
	bool Create(int radius);

	//Отрисовать текстуру
	void Draw(int x, int y);

	//Отрисовать текстуру
	void Redraw();

	//Данные текстуры
	PBYTE PixelData;
};
//---------------------------------------------------------------------------
//Класс для работы с текстурой кадра анимации
class TTextureAnimationFrame : public TBaseQueueItem
{
private:
	//Индекс кадра
	BYTE m_Frame;

	//Габариты текстуры
	short m_Width;
	short m_Height;

	//Координаты центра текстуры относительно начала текстуры
	short m_CenterX;
	short m_CenterY;
public:
	TTextureAnimationFrame(const int &frame);
	virtual ~TTextureAnimationFrame();

	SETGET(BYTE, Frame);
	SETGET(short, Width);
	SETGET(short, Height);
	SETGET(short, CenterX);
	SETGET(short, CenterY);

	//Индекс текстуры
	GLuint Texture;
};
//---------------------------------------------------------------------------
//Класс для работы с направлением анимации
class TTextureAnimationDirection : public TBaseQueueItem
{
private:
	//Направление
	BYTE m_Direction;

	//Количество кадров
	BYTE m_FrameCount;

	//Адрес в памяти
	DWORD m_Address;

	//Время последнего доступа
	DWORD m_LastAccessTime;
public:
	TTextureAnimationDirection(int direction);
	virtual ~TTextureAnimationDirection();

	SETGET(BYTE, Direction);
	SETGET(BYTE, FrameCount);
	SETGET(DWORD, Address);
	SETGET(DWORD, LastAccessTime);

	//Получить ссылку на фрэйм
	TTextureAnimationFrame *GetFrame(const BYTE &frame);

	//Найти ссылку на фрэйм
	TTextureAnimationFrame *FindFrame(const BYTE &frame);
};
//---------------------------------------------------------------------------
//Класс для работы с группой анимаций
class TTextureAnimationGroup : public TBaseQueueItem
{
private:
	//Индекс группы
	BYTE m_Index;
public:
	TTextureAnimationGroup(int index);
	virtual ~TTextureAnimationGroup();

	SETGET(BYTE, Index);

	//Получить ссылку на направление
	TTextureAnimationDirection *GetDirection(const BYTE &direction);
};
//---------------------------------------------------------------------------
//Класс для работы с группами текстур анимаций
class TTextureAnimation : public TBaseQueue
{
private:
public:
	TTextureAnimation();
	virtual ~TTextureAnimation();

	//Получить ссылку на группу
	TTextureAnimationGroup *GetGroup(const BYTE &index);
};
//---------------------------------------------------------------------------
//Структура данных для хранения информации о веб-ссылке
struct WEB_LINK_RECT
{
	//Индекс ссылки
	WORD LinkID;

	//Начало ссылки относительно начальных координат текста
	int StartX;
	int StartY;

	//Конец ссылки относительно начальных координат текста
	int EndX;
	int EndY;
};
//---------------------------------------------------------------------------
//Класс для работы с текстурами текста
class TTextTexture
{
private:
	//Габариты
	int m_Width;
	int m_Height;

	//Количество строк
	int m_Lines;
public:
	TTextTexture();
	virtual ~TTextTexture();

	//Данные текстуры (0/1)
	PBYTE Data;

	SETGET(int, Width);
	SETGET(int, Height);
	SETGETEX(int, Lines);

	//Текстура текста
	GLuint Texture;

	//Создана ли текстура
	bool Empty() {return (Texture == 0);}

	//Инициализация
	void Init();

	//Очистка текстуры
	void Clear();

	//Отрисовать текстуру
	void Draw(const int &x, const int &y);

	//Проверка на наличие текстуры под мышкой
	bool UnderMouse(int x, int y);

	//Очистка веб-ссылки
	virtual void ClearWebLink() {}

	//Добавиление веб-ссылки
	virtual void AddWebLink(WEB_LINK_RECT &wl) {}

	//Проверка веб-ссылки под мышкой
	virtual WORD WebLinkUnderMouse(int x, int y) {return 0;}
};
//---------------------------------------------------------------------------
//Класс для работы HTML текстурами текста
class THTMLTextTexture : public TTextTexture
{
private:
public:
	THTMLTextTexture();
	virtual ~THTMLTextTexture();

	//Список ссылок
	std::deque<WEB_LINK_RECT> WebLinkRect;

	//Очистка веб-ссылки
	void ClearWebLink() {WebLinkRect.clear();}

	//Добавиление веб-ссылки
	void AddWebLink(WEB_LINK_RECT &wl) {WebLinkRect.push_back(wl);}

	//Проверка веб-ссылки под мышкой
	WORD WebLinkUnderMouse(int x, int y);
};
//---------------------------------------------------------------------------
extern TCircleOfTransparencyTexture g_CircleOfTransparency;
//---------------------------------------------------------------------------
#endif