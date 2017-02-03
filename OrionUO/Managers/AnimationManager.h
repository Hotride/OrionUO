/***********************************************************************************
**
** AnimationManager.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H
//----------------------------------------------------------------------------------
#include "../IndexObject.h"
#include "../TextureObject.h"
#include "../Wisp/WispDataStream.h"
//----------------------------------------------------------------------------------
static const float UPPER_BODY_RATIO = 0.35f;
static const float MID_BODY_RATIO = 0.60f;
static const float LOWER_BODY_RATIO = 0.94f;
//----------------------------------------------------------------------------------
struct FRAME_OUTPUT_INFO
{
	int StartX;
	int StartY;
	int EndX;
	int EndY;
};
//----------------------------------------------------------------------------------
struct ANIMATION_DIMENSIONS
{
	int Width;
	int Height;
	int CenterX;
	int CenterY;
};
//----------------------------------------------------------------------------------
//!Класс менеджера анимаций
class CAnimationManager : public WISP_DATASTREAM::CDataReader
{
	//!Цвет текущего кадра
	SETGET(ushort, Color);
	//!Группа анимаций текущего кадра
	SETGET(int, AnimGroup);
	//!Направление текущего кадра
	SETGET(uchar, Direction);

private:
	//!Адреса файлов в памяти
	uint m_AddressIdx[6];
	uint m_AddressMul[6];
	uint m_SizeIdx[6];

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

	//!Текущий кадр сидит
	int m_Sitting;

	bool m_UseBlending;

	//!Упорядоченный список слоев для корректного порядка прорисовки для всех направлений персонажа
	static const int USED_LAYER_COUNT = 23;
	static const int m_UsedLayers[8][USED_LAYER_COUNT];

	//!Список используемых анимаций
	deque<CTextureAnimationDirection*> m_UsedAnimList;

	//Проверка пикселей картинки в указанных координатах
	bool TestImagePixels(CTextureAnimationDirection &direction, const uchar &frame, const int &checkX, const int &checkY);

	bool TestPixels(class CGameObject *obj, int x, int y, const bool &mirror, uchar &frameIndex, ushort id = 0x0000);

	//Корректировка направления сидячего персонажа, согласно тому. на чем он сидит
	void FixSittingDirection(uchar &layerDirection, bool &mirror, int &x, int &y);

	//Обобщенная фукнция рисования
	void Draw(class CGameObject *obj, int x, int y, const bool &mirror, uchar &frameIndex, int id = 0);

	void DrawIntoFrameBuffer(class CGameCharacter *obj, int x, int y);

	//!Рисуем слои одетой одежды на чаре, вычисляем исключающие друг-друга слои.
	bool DrawEquippedLayers(const bool &selection, class CGameObject *obj, const int &drawX, const int &drawY, const bool &mirror, const uchar &layerDir, uchar animIndex, const int &lightOffset);

	//!Находится-ли данный слой под робой
	bool IsCovered(const int &layer, class CGameObject *owner);

	bool m_Transform;

	ushort m_CharacterLayerGraphic[25];
	ushort m_CharacterLayerAnimID[25];

public:
	CAnimationManager();
	~CAnimationManager();

	//!Данные анимаций
	CIndexAnimation m_DataIndex[MAX_ANIMATIONS_DATA_INDEX_COUNT];

	/*!
	Инициализация адресов
	@param [__in] idx Индекс файла
	@param [__in] AddressIdx Адрес файла с таблицей адресов
	@param [__in] AddressMul Адрес файла с данными
	@param [__in] SizeIdx Размер файла с таблицей адресов
	@return 
	*/
	void Init(const int &graphic, const uint &addressIdx, const uint &addressMul, const uint &sizeIdx)
	{
		m_AddressIdx[graphic] = addressIdx;
		m_AddressMul[graphic] = addressMul;
		m_SizeIdx[graphic] = sizeIdx;
	}

	/*!
	Загрузка файла корректора индексов картинок анимаций
	@param [__in] verdata Ссылка на адрес в памяти файла патчей (verdata.mul)
	@return
	*/
	void InitIndexReplaces(puint verdata);

	/*!
	Загрузка данных
	@param [__in] verdata Ссылка на адрес в памяти файла патчей (verdata.mul)
	@return 
	*/
	void Load(puint verdata);

	/*!
	Получение ссылки на указанный фрэйм
	@param [__in] obj Игровой объект
	@param [__inout] frameIndex Индекс кадра
	@param [__in_opt] id Индекс картинки
	@return Ссылка на кадр анимации
	*/
	CTextureAnimationFrame *GetFrame(class CGameObject *obj, uchar frameIndex, ushort graphic = 0);

	/*!
	Очистка неиспользуемых текстур
	@param [__in] ticks Таймер удаления
	@return 
	*/
	void ClearUnusedTextures(uint ticks);

	/*!
	Загрузка картинок для указанного направления персонажа
	@param [__in] direction Ссылка на направление анимации
	@return true в случае успешной загрузки
	*/
	bool LoadDirectionGroup(CTextureAnimationDirection &direction);

	/*!
	Коррекция направления и режима зеркального отображения
	@param [__inout] dir Направление
	@param [__inout] mirror Зеркальное отображение
	@return 
	*/
	void GetAnimDirection(uchar &dir, bool &mirror);

	/*!
	Коррекция направления и режима зеркального отображения для сидячего персонажа
	@param [__inout] dir Направление
	@param [__inout] mirror Зеркальное отображение
	@param [__inout] x Координата X
	@param [__inout] y Координата Y
	@return 
	*/
	void GetSittingAnimDirection(uchar &dir, bool &mirror, int &x, int &y);

	/*!
	Отрисовать персонажа
	@param [__in] obj Ссылка на персонажа
	@param [__in] x Экранная координата X
	@param [__in] y Экранная координата Y
	@param [__in] z Координата Z
	@return 
	*/
	void DrawCharacter(class CGameCharacter *obj, int x, int y, int z);

	/*!
	Проверить наличие пикселя персонажа в указанных координатах
	@param [__in] obj Ссылка на персонажа
	@param [__in] x Экранная координата X
	@param [__in] y Экранная координата Y
	@param [__in] z Координата Z
	@return true в случае, если хоть 1 пиксель находится под мышкой
	*/
	bool CharacterPixelsInXY(class CGameCharacter *obj, int x, int y, int z);

	/*!
	Отрисовать труп
	@param [__in] obj Ссылка на труп
	@param [__in] x Экранная координата X
	@param [__in] y Экранная координата Y
	@param [__in] z Координата Z
	@return 
	*/
	void DrawCorpse(class CGameItem *obj, const int &x, const int &y);

	/*!
	Проверить наличие пикселя трупа в указанных координатах
	@param [__in] obj Ссылка на труп
	@param [__in] x Экранная координата X
	@param [__in] y Экранная координата Y
	@param [__in] z Координата Z
	@return 
	*/
	bool CorpsePixelsInXY(class CGameItem *obj, const int &x, const int &y);

	/*!
	Получить индекс группы смерти анимации
	@param [__in] id Byltrc rfhnbyrb
	@param [__in] second Группа смерти номер 2
	@return Индекс группы анимации
	*/
	uchar GetDieGroupIndex(ushort id, const bool &second);

	/*!
	Получить индекс группы по индексу картинки
	@param [__in] id Индекс картинки
	@return Группа анимаций
	*/
	ANIMATION_GROUPS GetGroupIndex(const ushort &id);

	/*!
	Существует ли анимация в файле
	@param [__in] graphic Индекс картинки
	@param [__in] group Группа анимации
	@return true в случае успеха
	*/
	bool AnimationExists(const ushort &graphic, uchar group);

	/*!
	Получить индекс анимации
	@param [__inout] graphic Индекс каритнки
	@return 
	*/
	void GetBodyGraphic(ushort &graphic);

	ANIMATION_DIMENSIONS GetAnimationDimensions(class CGameObject *obj, uchar frameIndex = 0xFF, const uchar &defaultDirection = 0, const uchar &defaultGroup = 0);

	void CalculateFrameInformation(FRAME_OUTPUT_INFO &info, class CGameObject *obj, const bool &mirror, const uchar &animIndex);

	struct DRAW_FRAME_INFORMATION CollectFrameInformation(class CGameObject *gameObject, const bool &checkLayers = true);
 };
//----------------------------------------------------------------------------------
//!Ссылка на менеджер анимаций
extern CAnimationManager g_AnimationManager;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
