﻿/***********************************************************************************
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
struct UOPFrameData
{
	puchar dataStart;
	short frameId;
	uint pixelDataOffset;
};
//----------------------------------------------------------------------------------
class CEquipConvData
{
public:
	ushort Graphic = 0;
	ushort Gump = 0;
	ushort Color = 0;

	CEquipConvData(ushort graphic, ushort gump, ushort color)
		: Graphic(graphic), Gump(gump), Color(color) {}
	~CEquipConvData() {}
};
//----------------------------------------------------------------------------------
typedef unordered_map<ushort, CEquipConvData> EQUIP_CONV_DATA_MAP;
typedef unordered_map<ushort, EQUIP_CONV_DATA_MAP> EQUIP_CONV_BODY_MAP;
//----------------------------------------------------------------------------------
//!Класс менеджера анимаций
class CAnimationManager : public WISP_DATASTREAM::CDataReader
{
public:
	//!Цвет текущего кадра
	ushort Color = 0;
	//!Группа анимаций текущего кадра
	uchar AnimGroup = 0;
	//!Направление текущего кадра
	uchar Direction = 0;
	//!Айдишник текущей анимации
	ushort AnimID = 0;
	//!Максимальный индекс группы анимаций
	uchar AnimGroupCount = PAG_ANIMATION_COUNT;

private:
	//!Адреса файлов в памяти
	size_t m_AddressIdx[6];
	size_t m_SizeIdx[6];

	//!Высота текстуры персонажа.
	int m_CharacterFrameHeight{ 0 };

	static void PrepareTargetAttackGump(CTargetGump &gump, int drawX, int drawY, ushort targetColor, int per, CGameCharacter &obj);

	//!Начало текстуры персонажа ( экранная координата )
	int m_CharacterFrameStartY{ 0 };

	//!Начало пояса в текстуре персонажа ( сверху-вниз )
	int m_StartCharacterWaistY{ 0 };

	//!Начало колен в текстуре персонажа ( сверху-вниз )
	int m_StartCharacterKneesY{ 0 };

	//!Начало стоп в текстуре персонажа ( сверху-вниз )
	int m_StartCharacterFeetY{ 0 };

	//!Текущий кадр сидит
	int m_Sitting{ 0 };

	bool m_UseBlending{ false };

	EQUIP_CONV_BODY_MAP m_EquipConv;

	CEquipConvData *m_EquipConvItem{ nullptr };

	//!Упорядоченный список слоев для корректного порядка прорисовки для всех направлений персонажа
	static const int USED_LAYER_COUNT = 23;
	static const int m_UsedLayers[8][USED_LAYER_COUNT];

	vector<std::pair<ushort, uchar> > m_GroupReplaces[2];

	//!Список используемых анимаций
	deque<CTextureAnimationDirection*> m_UsedAnimList;

	//Проверка пикселей картинки в указанных координатах
	bool TestPixels(class CGameObject *obj, int x, int y, bool mirror, uchar &frameIndex, ushort id = 0x0000);

	//Корректировка направления сидячего персонажа, согласно тому. на чем он сидит
	void FixSittingDirection(uchar &layerDirection, bool &mirror, int &x, int &y);

	//Обобщенная фукнция рисования
	void Draw(class CGameObject *obj, int x, int y, bool mirror, uchar &frameIndex, int id = 0);

	void DrawIntoFrameBuffer(class CGameCharacter *obj, int x, int y);

	//!Рисуем слои одетой одежды на чаре, вычисляем исключающие друг-друга слои.
	bool DrawEquippedLayers(bool selection, class CGameObject *obj, int drawX, int drawY, bool mirror, uchar layerDir, uchar animIndex, int lightOffset);

	//!Находится-ли данный слой под робой
	bool IsCovered(int layer, class CGameObject *owner);

	bool m_Transform = false;

	ushort m_CharacterLayerGraphic[25];
	ushort m_CharacterLayerAnimID[25];

	bool TryReadUOPAnimDimins(CTextureAnimationDirection &direction);

	//Прочитать и получить вектор с оффсетами пиксельных данных кадров.
	vector<UOPFrameData> ReadUOPFrameDataOffsets();

	//Чтение указанных в аргументах данных о кадре.
	void ReadUOPFrameData(short &imageCenterX, short &imageCenterY, short &imageWidth, short &imageHeight, pushort &palette, UOPFrameData &frameData);

	uchar GetObjectNewAnimationType_0(CGameCharacter *obj, ushort action, uchar mode);
	uchar GetObjectNewAnimationType_1_2(CGameCharacter *obj, ushort action, uchar mode);
	uchar GetObjectNewAnimationType_3(CGameCharacter *obj, ushort action, uchar mode);
	uchar GetObjectNewAnimationType_4(CGameCharacter *obj, ushort action, uchar mode);
	uchar GetObjectNewAnimationType_5(CGameCharacter *obj, ushort action, uchar mode);
	uchar GetObjectNewAnimationType_6_14(CGameCharacter *obj, ushort action, uchar mode);
	uchar GetObjectNewAnimationType_7(CGameCharacter *obj, ushort action, uchar mode);
	uchar GetObjectNewAnimationType_8(CGameCharacter *obj, ushort action, uchar mode);
	uchar GetObjectNewAnimationType_9_10(CGameCharacter *obj, ushort action, uchar mode);
	uchar GetObjectNewAnimationType_11(CGameCharacter *obj, ushort action, uchar mode);

	void ReadFrameDimensionData(ANIMATION_DIMENSIONS &result, uchar frameIndex, bool isCorpse);
	void ReadFramesPixelData(CTextureAnimationDirection &direction);

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
	void Init(int graphic, const size_t &addressIdx, const size_t &sizeIdx)
	{
		m_AddressIdx[graphic] = addressIdx;
		m_SizeIdx[graphic] = sizeIdx;
	}

	EQUIP_CONV_BODY_MAP &GetEquipConv() { return m_EquipConv; }

	/*!
	Загрузка файла корректора индексов картинок анимаций
	@param [__in] verdata Ссылка на адрес в памяти файла патчей (verdata.mul)
	@return
	*/
	void InitIndexReplaces(puint verdata);

	void UpdateAnimationAddressTable();

	/*!
	Загрузка данных
	@param [__in] verdata Ссылка на адрес в памяти файла патчей (verdata.mul)
	@return 
	*/
	void Load(puint verdata);

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
	void DrawCharacter(class CGameCharacter *obj, int x, int y);

	/*!
	Проверить наличие пикселя персонажа в указанных координатах
	@param [__in] obj Ссылка на персонажа
	@param [__in] x Экранная координата X
	@param [__in] y Экранная координата Y
	@param [__in] z Координата Z
	@return true в случае, если хоть 1 пиксель находится под мышкой
	*/
	bool CharacterPixelsInXY(class CGameCharacter *obj, int x, int y);

	/*!
	Отрисовать труп
	@param [__in] obj Ссылка на труп
	@param [__in] x Экранная координата X
	@param [__in] y Экранная координата Y
	@param [__in] z Координата Z
	@return 
	*/
	void DrawCorpse(class CGameItem *obj, int x, int y);

	/*!
	Проверить наличие пикселя трупа в указанных координатах
	@param [__in] obj Ссылка на труп
	@param [__in] x Экранная координата X
	@param [__in] y Экранная координата Y
	@param [__in] z Координата Z
	@return 
	*/
	bool CorpsePixelsInXY(class CGameItem *obj, int x, int y);

	/*!
	Получить индекс группы смерти анимации
	@param [__in] id Byltrc rfhnbyrb
	@param [__in] second Группа смерти номер 2
	@return Индекс группы анимации
	*/
	uchar GetDieGroupIndex(ushort id, bool second);

	/*!
	Получить индекс группы по индексу картинки
	@param [__in] id Индекс картинки
	@return Группа анимаций
	*/
	ANIMATION_GROUPS GetGroupIndex(ushort id);

	/*!
	Существует ли анимация в файле
	@param [__in] graphic Индекс картинки
	@param [__in] group Группа анимации
	@return true в случае успеха
	*/
	bool AnimationExists(ushort graphic, uchar group);

	ANIMATION_DIMENSIONS GetAnimationDimensions(uchar frameIndex, ushort id, uchar dir, uchar animGroup, bool isCorpse);

	ANIMATION_DIMENSIONS GetAnimationDimensions(class CGameObject *obj, uchar frameIndex = 0xFF, uchar defaultDirection = 0, uchar defaultGroup = 0);

	void CalculateFrameInformation(FRAME_OUTPUT_INFO &info, class CGameObject *obj, bool mirror, uchar animIndex);

	struct DRAW_FRAME_INFORMATION CollectFrameInformation(class CGameObject *gameObject, bool checkLayers = true);

	uchar GetReplacedObjectAnimation(CGameCharacter *obj, ushort index);

	uchar GetObjectNewAnimation(CGameCharacter *obj, ushort type, ushort action, uchar delay);
 };
//----------------------------------------------------------------------------------
//!Ссылка на менеджер анимаций
extern CAnimationManager g_AnimationManager;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
