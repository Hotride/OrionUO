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
//Основной класс
class TUltimaOnline : public IUltimaOnline
{
private:
	//CRC32 таблица
	DWORD m_CRC_Table[256];

	//Списки информации о данных текстур, звуков и т.п.
	TIndexObject m_LandDataIndex[0x4000];
	TIndexObjectStatic m_StaticDataIndex[0x4000];
	TIndexObject m_GumpDataIndex[0x10000];
	TIndexObject m_TextureDataIndex[0x1000];
	TIndexSound m_SoundDataIndex[0x0800];
	TIndexMulti m_MultiDataIndex[0x2000];
	TIndexObject m_LightDataIndex[100];

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

	//Загрузка данных о навыках
	bool LoadSkills();

	//Выгрузка данных о навыках
	void UnloadSkills();

	//Загрузка профессий
	void LoadProfession();

	//Загрузка описания профессий
	void LoadProfessionDescription();

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
	TTextureObject *ExecuteGump(WORD id, bool partialHue = true);

	//Загрузка текстуры ландшафта из art.mul
	TTextureObject *ExecuteLandArt(WORD id);

	//Загрузка текстуры статики
	TTextureObject *ExecuteStaticArt(WORD id);

	//Загрузка текстуры анимированной статики
	TTextureObject *ExecuteStaticArtAnimated(WORD id);

	//Загрузка текстуры ландшафта (из texmaps.mul)
	TTextureObject *ExecuteTexture(WORD id);

	//Загрузка текстуры света
	TTextureObject *ExecuteLight(BYTE &id);



	//Загрузка элементов гампов
	bool ExecuteGumpPart(WORD id, int count);

	//Загрузка элементов фона гампа
	bool ExecuteResizepic(WORD id) {return ExecuteGumpPart(id, 9);}

	//Загрузка элементов кнопки гампа
	bool ExecuteButton(WORD id) {return ExecuteGumpPart(id, 3);}



	//Отрисовка гампа-ползунка (составной гамп)
	void DrawSphereGump(int value, float maxValue, int x, int y, int width = 95);



	//Нарисовать гамп
	void DrawGump(WORD id, WORD color, int x, int y, bool partialHue = false);

	//Нарисовать замощенный гамп
	void DrawGump(WORD id, WORD color, int x, int y, int width, int height, bool partialHue = false);

	//Нарисовать фон гампа
	void DrawResizepicGump(WORD id, int x, int y, int width, int height);

	//Нарисовать ландшафт (из texmaps.mul)
	void DrawLandTexture(WORD id, WORD color, int x, int y, RECT rc, TVector *normals);

	//Нарисовать ландшафт (из art.mul)
	void DrawLandArt(WORD id, WORD color, int x, int y, int z);

	//Нарисовать статику
	void DrawStaticArt(WORD id, WORD color, int x, int y, int z, bool selection = false);

	//Нарисовать анимированную статику
	void DrawStaticArtAnimated(WORD id, WORD color, int x, int y, int z, bool selection = false);

	//Нарисовать прозрачную статику
	void DrawStaticArtTransparent(WORD id, WORD color, int x, int y, int z, bool selection = false);

	//Нарисовать прозрачную анимированную статику
	void DrawStaticArtAnimatedTransparent(WORD id, WORD color, int x, int y, int z, bool selection = false);

	//Нарисовать статику в контейнере
	void DrawStaticArtInContainer(WORD id, WORD color, int x, int y, bool selection = false, bool onMouse = false);

	//Нарисовать свет
	void DrawLight(BYTE id, WORD color, int x, int y);
	


	//Проверка прямоугольника под курсором мышки
	bool PolygonePixelsInXY(int x, int y, int width, int height);

	//Проверка гампа под курсором мышки
	bool GumpPixelsInXY(WORD id, int x, int y, bool noSubMouse = false);

	//Проверка замощенного гампа под курсором мышки
	bool GumpPixelsInXY(WORD id, int x, int y, int width, int height, bool noSubMouse = false);

	//Проверка фона под курсором мышки
	bool ResizepicPixelsInXY(WORD id, int x, int y, int width, int height);

	//Проверка статики под курсором мышки
	bool StaticPixelsInXY(WORD id, int x, int y, int z);

	//Проверка анимированной статики под курсором мышки
	bool StaticPixelsInXYAnimated(WORD id, int x, int y, int z);

	//Проверка круга прозрачности под курсором мышки
	bool CircleTransPixelsInXY();

	//Проверка статики в контейнере под курсором мышки
	bool StaticPixelsInXYInContainer(WORD id, int x, int y);

	//Проверка ландшафта (из art.mul) под курсором мышки
	bool LandPixelsInXY(WORD id, int x, int y, int z);

	//Проверка растянутого ландшафта (из texmaps.mul) под курсором мышки
	bool LandTexturePixelsInXY(int x, int  y, RECT &r);



	//Получить флаги ландшафта
	DWORD GetLandFlags(WORD id);

	//Получить флаги статики
	DWORD GetStaticFlags(WORD id);



	//Получить габариты картинки статики
	void GetArtDimension(WORD id, POINT &p);

	//Получить габариты картинки гампа
	void GetGumpDimension(WORD id, POINT &p);



	//Получить указатель на индексник гампа
	TIndexObject *GetGumpPointer(WORD id);

	//Получить указатель на индексник мульи
	TIndexMulti *GetMultiPointer(WORD id);



	//Получить цвет света
	WORD GetLightColor(WORD id) {return m_StaticDataIndex[id].LightColor;}



	//Проиграть звуковой эффект
	void PlaySoundEffect(WORD index, int volume = -1);

	//Удалить неиспользуемые звуковые эффекты
	void ResetSoundEffects(DWORD ticks);



	//Данные из тайлдаты по ландшафту
	LAND_GROUP m_LandData[512];

	//Данные из тайлдаты по статике
	PSTATIC_GROUP m_StaticData;

	//Количество данных по статике
	int m_StaticDataCount;



	//Отправить пакет на сервер
	int Send(PBYTE buf, int size);

	//Инициализация экрана
	void InitScreen(GAME_STATE state);

	//Обработка мэйнлупа
	void Process();



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
	void HelpRequest();
	void StatusReq(DWORD serial);
	void SkillsReq(DWORD serial);
	void SkillStatusChange(BYTE skill, BYTE state);
	void Click(DWORD serial);
	void NameReq(DWORD serial) {Click(serial);};
	void DoubleClick(DWORD serial);
	void PaperdollReq(DWORD serial);
	void Attack(DWORD serial); //Обработка атаки в клиенте
	void AttackReq(DWORD serial); //Отправка пакета на сервер
	void SendASCIIText(const char *str, SPEECH_TYPE type);
	void CastSpell(int index);
	void CastSpellFromBook(int index, DWORD serial);
	void UseSkill(int index);
	void OpenDoor();
	void EmoteAction(const char *action);
	void AllNames();
	void GoToWebLink(string url);



	void RemoveRangedObjects();



	void LogOut();



	void OpenPaperdoll();
	void OpenStatus(DWORD serial);
	void OpenMinimap();
	void OpenWorldMap();
	void OpenJournal();
	void OpenSkills();
	void OpenBackpack();
	void OpenSpellbook();
	void OpenLogOut();
	void OpenChat();
	void OpenConfiguration();
	void OpenMail();
	void OpenPartyManifest();
	void OpenPartyChat();
	void OpenProfile(DWORD serial = 0);
};
//---------------------------------------------------------------------------
extern TUltimaOnline *UO;
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------