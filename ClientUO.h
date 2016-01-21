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
class TUltimaOnline : public IUltimaOnline
{
private:
	DWORD m_CRC_Table[256];

	TIndexObject m_LandDataIndex[0x4000];
	TIndexObjectStatic m_StaticDataIndex[0x4000];
	TIndexObject m_GumpDataIndex[0x10000];
	TIndexObject m_TextureDataIndex[0x1000];
	TIndexSound m_SoundDataIndex[0x0800];
	TIndexMulti m_MultiDataIndex[0x2000];
	TIndexObject m_LightDataIndex[100];

	TLinkedList *m_StaticAnimList;
	
	TLinkedList *m_UsedLandList;
	TLinkedList *m_UsedStaticList;
	TLinkedList *m_UsedGumpList;
	TLinkedList *m_UsedTextureList;
	TLinkedList *m_UsedSoundList;
	TLinkedList *m_UsedLightList;

	PBYTE m_AnimData;

	void LoadClientStartupConfig();
	
	void LoadIndexFiles();
	void UnloadIndexFiles();
	void InitStaticAnimList();
	void ClearUnusedTextures();
	void LoadTiledata();
	void PatchFiles();
	bool LoadSkills();
	void UnloadSkills();
	void LoadProfession();
	void LoadProfessionDescription();
	void LoadLogin();
	void LoadShaders();

	void LoadClientConfig();
	void LoadAutoLoginNames();

	WORD CalculateLightColor(WORD id);

	void SendWalkStack();
public:
	TUltimaOnline();
	~TUltimaOnline();

	bool Install();
	
	void LoadStartupConfig();
	void LoadPluginConfig();

	void LoadLocalConfig();
	void SaveLocalConfig();
	
	DWORD GetFileHashCode(DWORD address, DWORD size);

	bool AutoLoginNameExists(string name);
	
	void ProcessStaticAnimList();

	TTextureObject *ExecuteGump(WORD id, bool partialHue = true);
	TTextureObject *ExecuteLandArt(WORD id);
	TTextureObject *ExecuteStaticArt(WORD id);
	TTextureObject *ExecuteTexture(WORD id);
	TTextureObject *ExecuteLight(BYTE &id);
	
	bool ExecuteGumpPart(WORD id, int count);
	bool ExecuteResizepic(WORD id) {return ExecuteGumpPart(id, 9);}
	bool ExecuteButton(WORD id) {return ExecuteGumpPart(id, 3);}

	void DrawSphereGump(int value, float maxValue, int x, int y, int width = 95);

	void DrawGump(WORD id, WORD color, int x, int y, bool partialHue = false);
	void DrawGump(WORD id, WORD color, int x, int y, int width, int height, bool partialHue = false);
	void DrawResizepicGump(WORD id, int x, int y, int width, int height);
	void DrawLandTexture(WORD id, WORD color, int x, int y, RECT rc, TVector *normals);
	void DrawLandArt(WORD id, WORD color, int x, int y, int z);
	void DrawStaticArt(WORD id, WORD color, int x, int y, int z, bool selection = false);
	void DrawStaticArtAnimated(WORD id, WORD color, int x, int y, int z, bool selection = false);
	void DrawStaticArtInContainer(WORD id, WORD color, int x, int y, bool selection = false, bool onMouse = false);
	void DrawLight(BYTE id, WORD color, int x, int y);
	
	bool PolygonePixelsInXY(int x, int y, int width, int height);
	bool GumpPixelsInXY(WORD id, int x, int y, bool noSubMouse = false);
	bool GumpPixelsInXY(WORD id, int x, int y, int width, int height, bool noSubMouse = false);
	bool ResizepicPixelsInXY(WORD id, int x, int y, int width, int height);
	bool StaticPixelsInXY(WORD id, int x, int y, int z);
	bool StaticPixelsInXYAnimated(WORD id, int x, int y, int z);
	bool StaticPixelsInXYInContainer(WORD id, int x, int y);
	bool LandPixelsInXY(WORD id, int x, int y, int z);

	DWORD GetLandFlags(WORD id);
	DWORD GetStaticFlags(WORD id);
	
	void GetArtDimension(WORD id, POINT &p);
	void GetGumpDimension(WORD id, POINT &p);
	
	TIndexMulti *GetMultiPointer(WORD id);
	WORD GetLightColor(WORD id) {return m_StaticDataIndex[id].LightColor;}

	void PlaySoundEffect(WORD index, int volume = -1);
	void ResetSoundEffects(DWORD ticks);

	LAND_GROUP m_LandData[512];
	PSTATIC_GROUP m_StaticData;
	int m_StaticDataCount;

	int Send(PBYTE buf, int size);

	void InitScreen(GAME_STATE state);
	void Process();
	
	void CreateTextMessage(TEXT_TYPE type, DWORD serial, WORD font, WORD color, string text);
	void CreateUnicodeTextMessage(TEXT_TYPE type, DWORD serial, WORD font, WORD color, wstring text);

	void AddSystemMessage(TTextData *msg);
	void AddJournalMessage(TTextData *msg, string name);

	void ChangeMap(BYTE newmap);
	
	void Connect();
	void Disconnect();
	void ServerSelection(int pos);
	void RelayServer(const char *ip, int port, PBYTE gameSeed);
	void CharacterSelection(int pos);
	void LoginComplete();

	void ConsolePromptSend();
	void ConsolePromptCancel();
	
	void PickupItem(TGameItem *obj, int count = 0, bool isGameFigure = false);
	void DropItem(DWORD container, WORD x, WORD y, char z);
	void EquipItem(DWORD container = 0);

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