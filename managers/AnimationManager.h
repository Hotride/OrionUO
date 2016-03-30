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
//Структура с информацией о тени
struct SHADOW_DATA
{
	//Индекс текстуры
	GLuint Texture;

	//Экранные координаты объекта
	int DrawX;
	int DrawY;

	//Значение Z-buffer'а
	int ZBuffer;

	//Габариты изображения
	int Width;
	int Height;

	//Перевернутая картинка или нет
	bool Mirror;
};
//---------------------------------------------------------------------------
//Менеджер анимаций
class TAnimationManager
{
private:
	//Адреса файлов в памяти
	DWORD m_AddressIdx[6];
	DWORD m_AddressMul[6];
	DWORD m_SizeIdx[6];

	//Информация о текущем кадре
	WORD m_Color;
	int m_AnimGroup;
	BYTE m_Direction;
	int m_Sitting;

	//Данные о тенях
	SHADOW_DATA m_ShadowList[MAX_SHADOWS_COUNT];
	int m_ShadowCount;

	//Упорядоченный список слоев для корректного порядка прорисовки для всех направлений персонажа
	static const int USED_LAYER_COLUT = 19;
	static int m_UsedLayers[8][USED_LAYER_COLUT];

	//Данные анимаций
	TIndexAnimation m_DataIndex[MAX_ANIMATIONS_DATA_INDEX_COUNT];
	WORD m_CorpseReplaces[MAX_ANIMATIONS_DATA_INDEX_COUNT];

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
public:
	TAnimationManager();
	~TAnimationManager();

	//Инициализация адресов
	void Init(int idx, DWORD AddressIdx, DWORD AddressMul, DWORD SizeIdx)
	{
		m_AddressIdx[idx] = AddressIdx;
		m_AddressMul[idx] = AddressMul;
		m_SizeIdx[idx] = SizeIdx;
	}

	SETGET(WORD, Color);
	SETGET(int, AnimGroup);
	SETGET(BYTE, Direction);
	SETGET(int, ShadowCount);

	//Загрузка файла корректора индексов картинок анимаций
	void InitIndexReplaces(PDWORD verdata);

	//Загрузка данных
	void Load(PDWORD verdata);

	//Получение ссылки на указанный фрэйм
	TTextureAnimationFrame *GetFrame(TGameObject *obj, BYTE &frameIndex, WORD id = 0x0000);

	//Очистка неиспользуемых текстур
	void ClearUnusedTextures(DWORD ticks);

	//Загрузка картинок для указанного направления персонажа
	bool ExecuteDirectionGroup(TTextureAnimationDirection *direction, WORD &id, int &offset);

	//Коррекция направления и режима зеркального отображения
	void GetAnimDirection(BYTE &dir, bool &mirror);
	
	//Коррекция направления и режима зеркального отображения для сидячего персонажа
	void GetSittingAnimDirection(BYTE &dir, bool &mirror, int &x, int &y);

	//Получить ссылку на данные анимации
	TTextureAnimation *GetAnimation(WORD id);

	//Отрисовать персонажа
	void DrawCharacter(TGameCharacter *obj, int x, int y, int z);

	//Проверить наличие пикселя персонажа в указанных координатах
	bool CharacterPixelsInXY(TGameCharacter *obj, int X, int Y, int Z);

	//Отрисовать труп
	void DrawCorpse(TGameItem *obj, int x, int y, int z);

	//Проверить наличие пикселя трупа в указанных координатах
	bool CorpsePixelsInXY(TGameItem *obj, int x, int y, int z);

	//Получить индекс группы смерти анимации
	BYTE GetDieGroupIndex(WORD id, bool second);

	//Получить индекс группы по индексу картинки
	ANIMATION_GROUPS GetGroupIndex(WORD id);

	//Отрисовать тени
	void DrawShadows();

	bool AnimationExists(WORD &graphic, BYTE group);

	void GetCorpseGraphic(WORD &graphic);
	void GetBodyGraphic(WORD &graphic);
};
//---------------------------------------------------------------------------
extern TAnimationManager *AnimationManager;
//---------------------------------------------------------------------------
#endif