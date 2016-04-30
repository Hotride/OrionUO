/****************************************************************************
**
** ClientUO.h
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
#ifndef ClientUOH
#define ClientUOH
//---------------------------------------------------------------------------
#pragma once
#include "resource.h"
//---------------------------------------------------------------------------
#define IDT_UPDATE_MOUSE_TIMER		666
//---------------------------------------------------------------------------
//Основной класс
class TUltimaOnline
{
private:
	//CRC32 таблица
	DWORD m_CRC_Table[256];

	//Списки информации о данных текстур, звуков и т.п.
	TIndexObject m_LandDataIndex[MAX_LAND_DATA_INDEX_COUNT];
	TIndexObjectStatic m_StaticDataIndex[MAX_STATIC_DATA_INDEX_COUNT];
	TIndexObject m_GumpDataIndex[MAX_GUMP_DATA_INDEX_COUNT];
	TIndexObject m_TextureDataIndex[MAX_LAND_TEXTURES_DATA_INDEX_COUNT];
	TIndexSound m_SoundDataIndex[MAX_SOUND_DATA_INDEX_COUNT];
	TMP3Struct m_MP3Data[MAX_MP3_COUNT];
	TIndexMulti m_MultiDataIndex[0x2000];
	TIndexObject m_LightDataIndex[MAX_LIGHTS_DATA_INDEX_COUNT];

	//Список анимированных картинок
	TLinkedList *m_StaticAnimList;

	//Списки загруженных объектов
	TLinkedList *m_UsedLandList;
	TLinkedList *m_UsedStaticList;
	TLinkedList *m_UsedGumpList;
	TLinkedList *m_UsedTextureList;
	TLinkedList *m_UsedSoundList;
	TLinkedList *m_UsedLightList;

	//Данные из анимдаты
	PBYTE m_AnimData;

	//Загрузка стартового конфига
	void LoadClientStartupConfig();

	//Загрузки индексных файлов
	void LoadIndexFiles();

	//Выгрузка индексных файлов
	void UnloadIndexFiles();

	//Создание списка анимированных предметов
	void InitStaticAnimList();

	//Очистка неиспользуемых текстур
	void ClearUnusedTextures();

	//Загрузка данных из тайлдаты
	void LoadTiledata();

	//Патч файлов
	void PatchFiles();

	//Подмена индексов (согласно текстовым файлам)
	void IndexReplaces();

	//Загрузка данных о навыках
	bool LoadSkills();

	//Выгрузка данных о навыках
	void UnloadSkills();

	//Загрузка логинсервера
	void LoadLogin();

	//Загрузка шейдеров
	void LoadShaders();

	//Загрузка конфигурации клиента
	void LoadClientConfig();

	//Загрузка имен для автовхода
	void LoadAutoLoginNames();

	//Вычисление цвета света
	WORD CalculateLightColor(WORD id);

	//Обработка стека шагов
	void SendWalkStack();

	void CreateObjectHandlesBackground();

public:
	TUltimaOnline();
	~TUltimaOnline();

	//Инициализация
	bool Install();

	//Загрузка стартового конфига
	void LoadStartupConfig();

	//Загрузка плагинов
	void LoadPluginConfig();

	//Загрузка конфига персонажа
	void LoadLocalConfig();

	//Сохранение конфига персонажа
	void SaveLocalConfig();

	//Получить хэш-код файла
	DWORD GetFileHashCode(DWORD address, DWORD size);

	//Проверка наличия имени в списке автовхода
	bool AutoLoginNameExists(string name);

	//Обработка списка анимированных предметов
	void ProcessStaticAnimList();



	//Загрузка текстуры гампа
	TTextureObject *ExecuteGump(const WORD &id, const bool &partialHue = true);

	//Загрузка текстуры ландшафта из art.mul
	TTextureObject *ExecuteLandArt(const WORD &id);

	//Загрузка текстуры статики
	TTextureObject *ExecuteStaticArt(const WORD &id);

	//Загрузка текстуры анимированной статики
	TTextureObject *ExecuteStaticArtAnimated(const WORD &id);

	//Загрузка текстуры ландшафта (из texmaps.mul)
	TTextureObject *ExecuteTexture(WORD id);

	//Загрузка текстуры света
	TTextureObject *ExecuteLight(BYTE &id);



	//Загрузка элементов гампов
	bool ExecuteGumpPart(const WORD &id, const int &count);

	//Загрузка элементов фона гампа
	bool ExecuteResizepic(const WORD &id) { return ExecuteGumpPart(id, 9); }

	//Загрузка элементов кнопки гампа
	bool ExecuteButton(const WORD &id) { return ExecuteGumpPart(id, 3); }



	//Отрисовка гампа-ползунка (составной гамп)
	void DrawSphereGump(const int &value, const float &maxValue, const int &x, const int &y, const int &width = 95);



	//Нарисовать гамп
	void DrawGump(const WORD &id, const WORD &color, const int &x, const int &y, const bool &partialHue = false);

	//Нарисовать замощенный гамп
	void DrawGump(const WORD &id, const WORD &color, const int &x, const int &y, int width, int height, const bool &partialHue = false);

	//Нарисовать фон гампа
	void DrawResizepicGump(const WORD &id, const int &x, const int &y, const int &width, const int &height);

	//Нарисовать ландшафт (из texmaps.mul)
	void DrawLandTexture(const WORD &id, WORD color, const int &x, const int &y, RECT &rc, TVector *normals);

	//Нарисовать ландшафт (из art.mul)
	void DrawLandArt(const WORD &id, WORD color, const int &x, const int &y, const int &z);

	//Нарисовать статику
	void DrawStaticArt(const WORD &id, WORD color, const int &x, const int &y, const int &z, const bool &selection = false);

	//Нарисовать анимированную статику
	void DrawStaticArtAnimated(const WORD &id, const WORD &color, const int &x, const int &y, const int &z, const bool &selection = false);

	//Нарисовать повернутую статику
	void DrawStaticArtRotated(const WORD &id, WORD color, const int &x, const int &y, const int &z, const float &angle);

	//Нарисовать повернутую анимированную статику
	void DrawStaticArtAnimatedRotated(const WORD &id, const WORD &color, const int &x, const int &y, const int &z, const float &angle);

	//Нарисовать прозрачную статику
	void DrawStaticArtTransparent(const WORD &id, WORD color, int x, int y, const int &z, const bool &selection = false);

	//Нарисовать прозрачную анимированную статику
	void DrawStaticArtAnimatedTransparent(const WORD &id, const WORD &color, const int &x, const int &y, const int &z, const bool &selection = false);

	//Нарисовать статику в контейнере
	void DrawStaticArtInContainer(const WORD &id, WORD color, int x, int y, const bool &selection = false, const bool &onMouse = false);

	//Нарисовать свет
	void DrawLight(BYTE id, const WORD &color, const int &x, const int &y);
	


	//Проверка прямоугольника под курсором мышки
	bool PolygonePixelsInXY(int x, int y, const int &width, const int &height);

	//Проверка гампа под курсором мышки
	bool GumpPixelsInXY(const WORD &id, int x, int y, const bool &noSubMouse = false);

	//Проверка замощенного гампа под курсором мышки
	bool GumpPixelsInXY(const WORD &id, int x, int y, int width, int height, const bool &noSubMouse = false);

	//Проверка фона под курсором мышки
	bool ResizepicPixelsInXY(const WORD &id, int x, int y, const int &width, const int &height);

	//Проверка статики под курсором мышки
	bool StaticPixelsInXY(const WORD &id, int x, int y, const int &z);

	//Проверка анимированной статики под курсором мышки
	bool StaticPixelsInXYAnimated(const WORD &id, const int &x, const int &y, const int &z);

	//Проверка круга прозрачности под курсором мышки
	bool CircleTransPixelsInXY();

	//Проверка статики в контейнере под курсором мышки
	bool StaticPixelsInXYInContainer(const WORD &id, int x, int y);

	//Проверка ландшафта (из art.mul) под курсором мышки
	bool LandPixelsInXY(const WORD &id, int x, int y, const int &z);

	//Проверка растянутого ландшафта (из texmaps.mul) под курсором мышки
	bool LandTexturePixelsInXY(int x, int  y, RECT &r);



	//Получить флаги ландшафта
	DWORD GetLandFlags(const WORD &id);

	//Получить флаги статики
	DWORD GetStaticFlags(const WORD &id);



	//Получить габариты картинки статики
	void GetArtDimension(const WORD &id, POINT &p);

	//Получить габариты картинки статики (реальные пиксельные границы)
	void GetStaticArtRealPixelDimension(const WORD &id, RECT &r);

	//Получить габариты картинки гампа
	void GetGumpDimension(const WORD &id, POINT &p);


	
	//Получить указатель на индексник гампа
	TIndexObjectStatic *GetStaticPointer(const WORD &id);

	//Получить указатель на индексник гампа
	TIndexObject *GetGumpPointer(const WORD &id);

	//Получить указатель на индексник мульи
	TIndexMulti *GetMultiPointer(const WORD &id);



	//Получить цвет света
	WORD GetLightColor(const WORD &id) { return m_StaticDataIndex[id].LightColor; }

	int GetConfigValue(const char *option, int value = -1);



	//Проиграть звуковой эффект
	void PlaySoundEffect(const WORD &index, int volume = -1);

	//Проиграть музыку mp3 либо midi
	void PlayMusic(int index);

	//Удалить неиспользуемые звуковые эффекты
	void ResetSoundEffects(DWORD ticks);



	//Данные из тайлдаты по ландшафту
	LAND_GROUP m_LandData[512];

	//Данные из тайлдаты по статике
	STATIC_GROUP m_StaticData[512];



	//Отправить пакет на сервер
	int Send(PBYTE buf, int size);

	//Инициализация экрана
	void InitScreen(GAME_STATE state);

	//Обработка мэйнлупа
	void Process();



	//Создать текстовое сообщение с форматированным вводом
	void CreateTextMessageF(BYTE font, WORD color, const char *format, ...);

	//Создать текстовое сообщение с форматированным вводом (Unicode)
	void CreateUnicodeTextMessageF(BYTE font, WORD color, const char *format, ...);

	//Создать текстовое сообщение (ASCII)
	void CreateTextMessage(TEXT_TYPE type, DWORD serial, WORD font, WORD color, string text);

	//Создать текстовое сообщение (Unicode)
	void CreateUnicodeTextMessage(TEXT_TYPE type, DWORD serial, WORD font, WORD color, wstring text);

	//Добавить сообщение в систем чат
	void AddSystemMessage(TTextData *msg);

	//Добавить сообщение в журнал
	void AddJournalMessage(TTextData *msg, string name);



	//Изменение индекса карты
	void ChangeMap(BYTE newmap);



	//Подключиться к логин сокету
	void Connect();

	//Оборвать соединение
	void Disconnect();

	//Выбор сервера
	void ServerSelection(int pos);

	//Подключение к игровому сокету
	void RelayServer(const char *ip, int port, PBYTE gameSeed);

	//Выбор персонажа
	void CharacterSelection(int pos);

	//Подтверждение о завершении логина (приходит от сервера)
	void LoginComplete();



	//Отправить промпт
	void ConsolePromptSend();

	//Отправить отмену промпта
	void ConsolePromptCancel();



	//Поднять предмет
	void PickupItem(TGameItem *obj, int count = 0, bool isGameFigure = false);

	//Бросить предмет
	void DropItem(DWORD container, WORD x, WORD y, char z);

	//Одеть предмет
	void EquipItem(DWORD container = 0);



	//Изменить вармод
	void ChangeWarmode(BYTE status = 0xFF);

	//Запрос хэлп-меню
	void HelpRequest();

	//Запрос статуса
	void StatusReq(DWORD serial);

	//Запрос навыков
	void SkillsReq(DWORD serial);

	//Изменения состояния навыка
	void SkillStatusChange(BYTE skill, BYTE state);

	//Кликнуть по объекту
	void Click(DWORD serial);

	//Запрос имени объекта (клик)
	void NameReq(DWORD serial) {Click(serial);};

	//Двойное нажатие на объект
	void DoubleClick(DWORD serial);

	//Запрос папердолла
	void PaperdollReq(DWORD serial);

	//Обработка атаки в клиенте
	void Attack(DWORD serial);

	//Отправка пакета на сервер
	void AttackReq(DWORD serial);

	//Отправка ASCII текста
	void SendASCIIText(const char *str, SPEECH_TYPE type);

	//Начать каст заклинания
	void CastSpell(int index);

	//Начать каст заклинания с книги
	void CastSpellFromBook(int index, DWORD serial);

	//Использовать навык
	void UseSkill(int index);

	//Открыть дверь
	void OpenDoor();

	//Запрос анимации эмоции
	void EmoteAction(const char *action);

	//Клик на всех персонажей в зоне видимости
	void AllNames();

	//Переход по веб-ссылке
	void GoToWebLink(string url);



	//Удаление объектов, вышедших за пределы g_UpdateRange
	void RemoveRangedObjects();



	//Выход из игры на основной экран
	void LogOut();



	//--------------------------------------------
	//---Обработка кнопок папердолла и менюбара---
	//--------------------------------------------

	//Открыть гамп папердолла
	void OpenPaperdoll();

	//Открыть гамп статуса
	void OpenStatus(DWORD serial);

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
	void OpenProfile(DWORD serial = 0);

	//Запросить гамп гильдии
	void RequestGuildGump();

	//Запросить гамп квестов
	void RequestQuestGump();
};
//---------------------------------------------------------------------------
extern TUltimaOnline *UO;
//---------------------------------------------------------------------------
#endif