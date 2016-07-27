/****************************************************************************
**
** AnimationManager.h
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
#ifndef AnimationManagerH
#define AnimationManagerH
//---------------------------------------------------------------------------
struct FRAME_OUTPUT_INFO
{
	int StartX;
	int StartY;
	int EndX;
	int EndY;
};
//---------------------------------------------------------------------------
struct ANIMATION_DIMENSIONS
{
	int Width;
	int Height;
	int CenterX;
	int CenterY;
};
//---------------------------------------------------------------------------
//!Структура с информацией о тени
struct SHADOW_DATA
{
	//!Индекс текстуры
	GLuint Texture;

	//!Экранные координаты объекта
	int DrawX;
	int DrawY;

	//!Значение Z-buffer'а
	int ZBuffer;

	//!Габариты изображения
	int Width;
	int Height;

	//!Перевернутая картинка или нет
	bool Mirror;
};
//---------------------------------------------------------------------------
//!Класс менеджера анимаций
class TAnimationManager
{
private:
	//!Адреса файлов в памяти
	DWORD m_AddressIdx[6];
	DWORD m_AddressMul[6];
	DWORD m_SizeIdx[6];

	//!Цвет текущего кадра
	WORD m_Color;

	const float UPPER_BODY_RATIO = 0.35f;
	const float MID_BODY_RATIO = 0.60f;
	const float LOWER_BODY_RATIO = 0.94f;

	//!Высота текстуры персонажа.
	int m_CharacterFrameHeight;

	//!Начало текстуры персонажа ( экранная координата )
	int m_CharacterFrameStartY;

	//!Начало пояса в текстуре персонажа ( сверху-вниз )
	int m_StartCharacterWaistY;

	//!Начало колен в текстуре персонажа ( сверху-вниз )
	int m_StartCharacterKneesY;

	//!Начало стоп в текстуре персонажа ( сверху-вниз )
	int m_StartCharacterFeetY;

	//!Группа анимаций текущего кадра
	int m_AnimGroup;

	//!Направление текущего кадра
	BYTE m_Direction;

	//!Текущий кадр сидит
	int m_Sitting;

	//!Список теней
	SHADOW_DATA m_ShadowList[MAX_SHADOWS_COUNT];

	//!Количество теней
	int m_ShadowCount;

	//!Упорядоченный список слоев для корректного порядка прорисовки для всех направлений персонажа
	static const int USED_LAYER_COUNT = 19;
	static int m_UsedLayers[8][USED_LAYER_COUNT];

	//!Данные анимаций
	TIndexAnimation m_DataIndex[MAX_ANIMATIONS_DATA_INDEX_COUNT];

	//!Список замен для трупов
	WORD m_CorpseReplaces[MAX_ANIMATIONS_DATA_INDEX_COUNT];

	//!Список используемых анимаций
	TLinkedList *m_UsedAnimList;

	//Добавление тени в список
	void AddShadow(GLuint texture, int drawX, int drawY, int zBuffer, int width, int height, bool mirror);

	//Проверка пикселей картинки в указанных координатах
	bool TestImagePixels(TTextureAnimationDirection *direction, BYTE &frame, WORD &id, int &checkX, int &checkY);
	bool TestPixels(TGameObject *obj, int x, int y, bool &mirror, BYTE &frameIndex, WORD id = 0x0000);

	//Корректировка направления сидячего персонажа, согласно тому. на чем он сидит
	void FixSittingDirection(BYTE &layerDirection, bool &mirror, int &x, int &y);

	//Обобщенная фукнция рисования
	void Draw(TGameObject *obj, int x, int y, bool &mirror, BYTE &frameIndex, int id = 0);

	void DrawIntoFrameBuffer(__in TGameCharacter *obj, __in int x, __in int y);

	//!Рисуем слои одетой одежды на чаре, вычисляем исключающие друг-друга слои.
	void DrawEquippedLayers(TGameCharacter* obj, int drawX, int drawY, bool mirror, BYTE layerDir, BYTE animIndex, int lightOffset);

	//!Находится-ли данный слой под робой
	bool IsUnderRobe(int layer);

	bool m_Transform;

public:
	TAnimationManager();
	~TAnimationManager();

	SETGET(WORD, Color);
	SETGET(int, AnimGroup);
	SETGET(BYTE, Direction);
	SETGET(int, ShadowCount);

	/*!
	Инициализация адресов
	@param [__in] idx Индекс файла
	@param [__in] AddressIdx Адрес файла с таблицей адресов
	@param [__in] AddressMul Адрес файла с данными
	@param [__in] SizeIdx Размер файла с таблицей адресов
	@return 
	*/
	void Init( __in int idx, __in DWORD AddressIdx, __in DWORD AddressMul, __in DWORD SizeIdx)
	{
		m_AddressIdx[idx] = AddressIdx;
		m_AddressMul[idx] = AddressMul;
		m_SizeIdx[idx] = SizeIdx;
	}

	/*!
	Загрузка файла корректора индексов картинок анимаций
	@param [__in] verdata Ссылка на адрес в памяти файла патчей (verdata.mul)
	@return
	*/
	void InitIndexReplaces(__in PDWORD verdata);

	/*!
	Загрузка данных
	@param [__in] verdata Ссылка на адрес в памяти файла патчей (verdata.mul)
	@return 
	*/
	void Load(__in PDWORD verdata);

	/*!
	Получение ссылки на указанный фрэйм
	@param [__in] obj Игровой объект
	@param [__inout] frameIndex Индекс кадра
	@param [__in_opt] id Индекс картинки
	@return Ссылка на кадр анимации
	*/
	TTextureAnimationFrame *GetFrame(__in TGameObject *obj, __inout BYTE &frameIndex, __in_opt WORD id = 0x0000);

	/*!
	Очистка неиспользуемых текстур
	@param [__in] ticks Таймер удаления
	@return 
	*/
	void ClearUnusedTextures(__in DWORD ticks);

	/*!
	Загрузка картинок для указанного направления персонажа
	@param [__in] direction Ссылка на направление анимации
	@param [__in] id Индекс картинки
	@param [__in] offset Смещение относительно начала анимаций
	@return true в случае успешной загрузки
	*/
	bool ExecuteDirectionGroup(__in TTextureAnimationDirection *direction, __in WORD &id, __in int &offset);

	/*!
	Коррекция направления и режима зеркального отображения
	@param [__inout] dir Направление
	@param [__inout] mirror Зеркальное отображение
	@return 
	*/
	void GetAnimDirection(__inout BYTE &dir, __inout bool &mirror);

	/*!
	Коррекция направления и режима зеркального отображения для сидячего персонажа
	@param [__inout] dir Направление
	@param [__inout] mirror Зеркальное отображение
	@param [__inout] x Координата X
	@param [__inout] y Координата Y
	@return 
	*/
	void GetSittingAnimDirection(__inout BYTE &dir, __inout bool &mirror, __inout int &x, __inout int &y);

	/*!
	Получить ссылку на данные анимации
	@param [__in] id Индекс картинки
	@return Ссылка на анимацию
	*/
	TTextureAnimation *GetAnimation(__in WORD id);

	/*!
	Отрисовать персонажа
	@param [__in] obj Ссылка на персонажа
	@param [__in] x Экранная координата X
	@param [__in] y Экранная координата Y
	@param [__in] z Координата Z
	@return 
	*/
	void DrawCharacter(__in TGameCharacter *obj, __in int x, __in int y, __in int z);

	void DrawCharacterAAA(__in TGameCharacter *obj, __in int x, __in int y, __in int z);

	/*!
	Проверить наличие пикселя персонажа в указанных координатах
	@param [__in] obj Ссылка на персонажа
	@param [__in] x Экранная координата X
	@param [__in] y Экранная координата Y
	@param [__in] z Координата Z
	@return true в случае, если хоть 1 пиксель находится под мышкой
	*/
	bool CharacterPixelsInXY(__in TGameCharacter *obj, __in int x, __in int y, __in int z);

	/*!
	Отрисовать труп
	@param [__in] obj Ссылка на труп
	@param [__in] x Экранная координата X
	@param [__in] y Экранная координата Y
	@param [__in] z Координата Z
	@return 
	*/
	void DrawCorpse(__in TGameItem *obj, __in int x, __in int y, __in int z);

	/*!
	Проверить наличие пикселя трупа в указанных координатах
	@param [__in] obj Ссылка на труп
	@param [__in] x Экранная координата X
	@param [__in] y Экранная координата Y
	@param [__in] z Координата Z
	@return 
	*/
	bool CorpsePixelsInXY(__in TGameItem *obj, __in int x, __in int y, __in int z);

	/*!
	Получить индекс группы смерти анимации
	@param [__in] id Byltrc rfhnbyrb
	@param [__in] second Группа смерти номер 2
	@return Индекс группы анимации
	*/
	BYTE GetDieGroupIndex(__in WORD id, __in bool second);

	/*!
	Получить индекс группы по индексу картинки
	@param [__in] id Индекс картинки
	@return Группа анимаций
	*/
	ANIMATION_GROUPS GetGroupIndex(__in WORD id);

	/*!
	Отрисовать тени
	@return 
	*/
	void DrawShadows();

	/*!
	Существует ли анимация в файле
	@param [__in] graphic Индекс картинки
	@param [__in] group Группа анимации
	@return true в случае успеха
	*/
	bool AnimationExists(__in WORD &graphic, __in BYTE group);

	/*!
	Получить индекс картинки трупа
	@param [__inout] graphic Индекс картинки
	@return 
	*/
	void GetCorpseGraphic(__inout WORD &graphic);

	/*!
	Получить индекс анимации
	@param [__inout] graphic Индекс каритнки
	@return 
	*/
	void GetBodyGraphic(__inout WORD &graphic);

	ANIMATION_DIMENSIONS GetAnimationDimensions(TGameObject *obj, BYTE frameIndex = 0xFF, const BYTE &defaultDirection = 0, const BYTE &defaultGroup = 0);

	void CalculateFrameInformation(FRAME_OUTPUT_INFO &info, TGameObject *obj, const bool &mirror, const BYTE &animIndex);

	DRAW_FRAME_INFORMATION CollectFrameInformation(TGameObject *gameObject, const bool &checkLayers = true);
 };
//---------------------------------------------------------------------------
 //!Ссылка на менеджер анимаций
extern TAnimationManager *AnimationManager;
//---------------------------------------------------------------------------
#endif