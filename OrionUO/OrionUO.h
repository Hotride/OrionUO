﻿/***********************************************************************************
**
** OrionUO.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef ORIONUO_H
#define ORIONUO_H
//----------------------------------------------------------------------------------
class COrion
{
public:
	string ClientVersionText = "2.0.3";
	int TexturesDataCount = 0;
	string DefaultLogin = "";
	int DefaultPort = 0;
	uint OrionVersionNumeric = 0;

private:
	uint m_CRC_Table[256];

	uchar m_StaticTilesFilterFlags[0x10000];

	USHORT_LIST m_StumpTiles;
	USHORT_LIST m_CaveTiles;

	deque<CIndexObjectStatic*> m_StaticAnimList;

	deque<CIndexObject*> m_UsedLandList;
	deque<CIndexObject*> m_UsedStaticList;
	deque<CIndexObject*> m_UsedGumpList;
	deque<CIndexObject*> m_UsedTextureList;
	deque<CIndexSound*> m_UsedSoundList;
	deque<CIndexObject*> m_UsedLightList;

	UCHAR_LIST m_AnimData;

	string m_GameServerIP = "";

	void LoadClientConfig();
	void LoadAutoLoginNames();

	void LoadTiledata(int landSize, int staticsSize);

	void LoadIndexFiles();

	//Выгрузка индексных файлов
	void UnloadIndexFiles();

	//Создание списка анимированных предметов
	void InitStaticAnimList();

	//Вычисление цвета света
	ushort CalculateLightColor(ushort id);

	//Обработка списка анимированных предметов
	void ProcessStaticAnimList();

	//Патч файлов
	void PatchFiles();

	//Подмена индексов (согласно текстовым файлам)
	void IndexReplaces();

	//Загрузка стартового конфига
	void LoadClientStartupConfig();

	//Загрузка шейдеров
	void LoadShaders();

	void CreateAuraTexture();

	void CreateObjectHandlesBackground();

	//Очистка неиспользуемых текстур
	void ClearUnusedTextures();

	void ReadMulIndexFile(size_t indexMaxCount, std::function<CIndexObject*(int index)> getIdxObj, const size_t &address, PBASE_IDX_BLOCK ptr, std::function<PBASE_IDX_BLOCK()> getNewPtrValue);
	void ReadUOPIndexFile(size_t indexMaxCount, std::function<CIndexObject*(int index)> getIdxObj, const char *uopFileName, int padding, const char *extesion, CUopMappedFile &uopFile, int startIndex = 0);

	void GetCurrentLocale();

	ushort TextToGraphic(const char *text);

	void CheckStaticTileFilterFiles();

	string DecodeArgumentString(const char *text, int length);

	void ParseCommandLine();

	void LoadPlugin(const string &libpath, const string &function, int flags);

	void LoadContaierOffsets();

public:
	COrion();
	~COrion();

	static unsigned long long CreateHash(string s);

	//Данные из тайлдаты по ландшафту
	vector<LAND_TILES> m_LandData;

	//Данные из тайлдаты по статике
	vector<STATIC_TILES> m_StaticData;

	static UINT_LIST FindPattern(puchar ptr, int size, const UCHAR_LIST &pattern);

	bool Install();
	void Uninstall();

	//Инициализация экрана
	void InitScreen(GAME_STATE state);

	//Обработка кликов мышки по истечении таймера ожидания даблклика
	void ProcessDelayedClicks();

	void Process(bool rendering = false);

	//Загрузка стартового конфига
	void LoadStartupConfig(int serial);

	//Загрузка плагинов
	void LoadPluginConfig();

	//Загрузка конфига персонажа
	void LoadLocalConfig(int serial);

	//Сохранение конфига персонажа
	void SaveLocalConfig(int serial);

	CIndexObjectLand m_LandDataIndex[MAX_LAND_DATA_INDEX_COUNT];
	CIndexObjectStatic m_StaticDataIndex[MAX_STATIC_DATA_INDEX_COUNT];
	CIndexGump m_GumpDataIndex[MAX_GUMP_DATA_INDEX_COUNT];
	CIndexObject m_TextureDataIndex[MAX_LAND_TEXTURES_DATA_INDEX_COUNT];
	CIndexSound m_SoundDataIndex[MAX_SOUND_DATA_INDEX_COUNT];
	CIndexMusic m_MP3Data[MAX_MUSIC_DATA_INDEX_COUNT];
	CIndexMulti m_MultiDataIndex[MAX_MULTI_DATA_INDEX_COUNT];
	CIndexLight m_LightDataIndex[MAX_LIGHTS_DATA_INDEX_COUNT];

	ushort m_WinterTile[MAX_LAND_DATA_INDEX_COUNT];

	vector<std::pair<ushort, ushort>> m_IgnoreInFilterTiles;

	bool InTileFilter(ushort graphic);

	static string FixServerName(string name);


	
	//Подключиться к логин сокету
	void Connect();

	//Оборвать соединение
	void Disconnect();

	//Отправить пакет на сервер
	int Send(puchar buf, int size);

	//Отправить пакет на сервер
	int Send(const UCHAR_LIST &buf) { return Send((puchar)&buf[0], (int)buf.size()); }

	//Выбор сервера
	void ServerSelection(int pos);

	//Подключение к игровому сокету
	void RelayServer(const char *ip, int port, puchar gameSeed);

	//Выбор персонажа
	void CharacterSelection(int pos);

	//Подтверждение о завершении логина (приходит от сервера)
	void LoginComplete(bool reload);

	//!Изменить сезон
	void ChangeSeason(const SEASON_TYPE &season, int music);

	ushort GetLandSeasonGraphic(ushort graphic);
	ushort GetSeasonGraphic(ushort graphic);
	ushort GetSpringGraphic(ushort graphic);
	ushort GetSummerGraphic(ushort graphic);
	ushort GetFallGraphic(ushort graphic);
	ushort GetWinterGraphic(ushort graphic);
	ushort GetDesolationGraphic(ushort graphic);

	int ValueInt(const VALUE_KEY_INT &key, int value = -1);
	string ValueString(const VALUE_KEY_STRING &key, string value = "");

	void ClearRemovedStaticsTextures();

	void ClearTreesTextures();

	bool IsTreeTile(ushort graphic, int &index);

	void ClearCaveTextures();

	bool IsCaveTile(ushort graphic);

	bool IsVegetation(ushort graphic);



	//Получить флаги ландшафта
	uint64 GetLandFlags(ushort id);

	//Получить флаги статики
	uint64 GetStaticFlags(ushort id);

	//Получить цвет света
	ushort GetLightColor(ushort id) { return m_StaticDataIndex[id].LightColor; }



	//Получить габариты картинки статики
	WISP_GEOMETRY::CSize GetStaticArtDimension(ushort id);

	//Получить габариты картинки гампа
	WISP_GEOMETRY::CSize GetGumpDimension(ushort id);



	//Загрузка текстуры гампа
	CGLTexture *ExecuteGump(ushort id);

	//Загрузка текстуры ландшафта из art.mul
	CGLTexture *ExecuteLandArt(ushort id);

	//Загрузка текстуры статики
	CGLTexture *ExecuteStaticArt(ushort id);

	//Загрузка текстуры анимированной статики
	CGLTexture *ExecuteStaticArtAnimated(ushort id);

	//Загрузка текстуры ландшафта (из texmaps.mul)
	CGLTexture *ExecuteTexture(ushort id);

	//Загрузка текстуры света
	CGLTexture *ExecuteLight(uchar &id);



	//Загрузка элементов гампов
	bool ExecuteGumpPart(ushort id, int count);

	//Загрузка элементов фона гампа
	bool ExecuteResizepic(ushort id) { return ExecuteGumpPart(id, 9); }

	//Загрузка элементов кнопки гампа
	bool ExecuteButton(ushort id) { return ExecuteGumpPart(id, 3); }



	//Нарисовать гамп
	void DrawGump(ushort id, ushort color, int x, int y, bool partialHue = false);

	//Нарисовать замощенный гамп
	void DrawGump(ushort id, ushort color, int x, int y, int width, int height, bool partialHue = false);

	//Нарисовать фон гампа
	void DrawResizepicGump(ushort id, int x, int y, int width, int height);

	//Нарисовать ландшафт (из texmaps.mul)
	void DrawLandTexture(class CLandObject *land, ushort color, int x, int y);

	//Нарисовать ландшафт (из art.mul)
	void DrawLandArt(ushort id, ushort color, int x, int y);

	//Нарисовать статику
	void DrawStaticArt(ushort id, ushort color, int x, int y, bool selection = false);

	//Нарисовать анимированную статику
	void DrawStaticArtAnimated(ushort id, ushort color, int x, int y, bool selection = false);

	//Нарисовать повернутую статику
	void DrawStaticArtRotated(ushort id, ushort color, int x, int y, float angle);

	//Нарисовать повернутую анимированную статику
	void DrawStaticArtAnimatedRotated(ushort id, ushort color, int x, int y, float angle);

	//Нарисовать прозрачную статику
	void DrawStaticArtTransparent(ushort id, ushort color, int x, int y, bool selection = false);

	//Нарисовать прозрачную анимированную статику
	void DrawStaticArtAnimatedTransparent(ushort id, ushort color, int x, int y, bool selection = false);

	//Нарисовать статику в контейнере
	void DrawStaticArtInContainer(ushort id, ushort color, int x, int y, bool selection = false, bool onMouse = false);

	//Нарисовать свет
	void DrawLight(struct LIGHT_DATA &light);



	//Проверка прямоугольника под курсором мышки
	bool PolygonePixelsInXY(int x, int y, int width, int height);

	//Проверка гампа под курсором мышки
	bool GumpPixelsInXY(ushort id, int x, int y);

	//Проверка замощенного гампа под курсором мышки
	bool GumpPixelsInXY(ushort id, int x, int y, int width, int height);

	//Проверка фона под курсором мышки
	bool ResizepicPixelsInXY(ushort id, int x, int y, int width, int height);

	//Проверка статики под курсором мышки
	bool StaticPixelsInXY(ushort id, int x, int y);

	//Проверка анимированной статики под курсором мышки
	bool StaticPixelsInXYAnimated(ushort id, int x, int y);

	//Проверка статики в контейнере под курсором мышки
	bool StaticPixelsInXYInContainer(ushort id, int x, int y);

	//Проверка ландшафта (из art.mul) под курсором мышки
	bool LandPixelsInXY(ushort id, int x, int y);

	//Проверка растянутого ландшафта (из texmaps.mul) под курсором мышки
	bool LandTexturePixelsInXY(int x, int  y, RECT &r);



	//Создать текстовое сообщение с форматированным вводом
	void CreateTextMessageF(uchar font, ushort color, const char *format, ...);

	//Создать текстовое сообщение с форматированным вводом (Unicode)
	void CreateUnicodeTextMessageF(uchar font, ushort color, const char *format, ...);

	//Создать текстовое сообщение (ASCII)
	void CreateTextMessage(const TEXT_TYPE &type, int serial, uchar font, ushort color, const string &text);

	//Создать текстовое сообщение (Unicode)
	void CreateUnicodeTextMessage(const TEXT_TYPE &type, int serial, uchar font, ushort color, const wstring &text);

	//Добавить сообщение в систем чат
	void AddSystemMessage(class CTextData *msg);

	//Добавить сообщение в журнал
	void AddJournalMessage(class CTextData *msg, const string &name);



	//Изменение индекса карты
	void ChangeMap(uchar newmap);



	//Поднять предмет
	void PickupItem(class CGameItem *obj, int count = 0, bool isGameFigure = false);

	//Бросить предмет
	void DropItem(int container, ushort x, ushort y, char z);

	//Одеть предмет
	void EquipItem(uint container = 0);



	//Изменить вармод
	void ChangeWarmode(uchar status = 0xFF);

	//Кликнуть по объекту
	void Click(uint serial);

	//Двойное нажатие на объект
	void DoubleClick(uint serial);

	//Запрос папердолла
	void PaperdollReq(uint serial);

	//Обработка атаки в клиенте
	void Attack(uint serial);

	//Отправка пакета на сервер
	void AttackReq(uint serial);

	//Отправка ASCII текста
	void SendASCIIText(const char *str, SPEECH_TYPE type);

	//Начать каст заклинания
	void CastSpell(int index);

	//Начать каст заклинания с книги
	void CastSpellFromBook(int index, uint serial);

	//Использовать навык
	void UseSkill(int index);

	//Открыть дверь
	void OpenDoor();

	//Запрос анимации эмоции
	void EmoteAction(const char *action);

	//Клик на всех персонажей в зоне видимости
	void AllNames();

	//Получить хэш-код файла
	uint GetFileHashCode(puchar ptr, size_t size);

	//Загрузка логинсервера
	void LoadLogin(string &login, int &port);

	//Переход по веб-ссылке
	void GoToWebLink(const string &url);

	void ResumeSound();

	void PauseSound();

	//Проиграть музыку mp3 либо midi
	void PlayMusic(int index, bool warmode = false);

	//Проиграть звуковой эффект
	void PlaySoundEffect(ushort index, float volume = -1);

	//Удалить неиспользуемые звуковые эффекты
	void AdjustSoundEffects(int ticks, float volume = -1);



	//Удаление объектов, вышедших за пределы g_UpdateRange
	void RemoveRangedObjects();

	//Очистить мир (удалить все объекты)
	void ClearWorld();

	//Выход из игры на основной экран
	void LogOut();



	//Отправить промпт
	void ConsolePromptSend();

	//Отправить отмену промпта
	void ConsolePromptCancel();



	//--------------------------------------------
	//---Обработка кнопок папердолла и менюбара---
	//--------------------------------------------

	//Открыть гамп статуса
	void OpenStatus(uint serial);

	//Открыть или переместить гамп статуса
	void DisplayStatusbarGump(int serial, int x, int y);

	//Открыть гамп мини-карты
	void OpenMinimap();

	//Открыть гамп карты мира
	void OpenWorldMap();

	//Открыть гамп журнала
	void OpenJournal();

	//Открыть гамп навыков
	void OpenSkills();

	//Открыть рюкзак
	void OpenBackpack();

	//Открыть гамп логаута
	void OpenLogOut();

	//Открыть гамп чата
	void OpenChat();

	//Открыть гамп опций
	void OpenConfiguration();

	//Открыть гамп почти
	void OpenMail();

	//Открыть гамп пати-манифеста
	void OpenPartyManifest();

	//Открыть гамп профиля
	void OpenProfile(uint serial = 0);

	void DisconnectGump();

	void OpenCombatBookGump();

	void OpenRacialAbilitiesBookGump();

	void StartReconnect();
};
//----------------------------------------------------------------------------------
extern COrion g_Orion;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------

