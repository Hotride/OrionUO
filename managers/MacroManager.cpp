/****************************************************************************
**
** MacroManager.cpp
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
#include "stdafx.h"

TMacroManager *MacroManager = NULL;
//---------------------------------------------------------------------------
BYTE TMacroManager::m_SkillIndexTable[24] =
{
	1, 2, 35, 4, 6, 12, 14, 15, 16, 19,
	21, 0xFF /*imbuing*/, 23, 3, 46, 9, 30, 22, 48, 32,
	33, 47, 36, 38
};
//---------------------------------------------------------------------------
TMacroManager::TMacroManager()
: TBaseQueue(), m_NextTimer(0), m_WaitingBandageTarget(false),
m_WaitForTargetTimer(0)
{
}
//---------------------------------------------------------------------------
TMacroManager::~TMacroManager()
{
}
//---------------------------------------------------------------------------
/*!
Конвертирование строки в виртуальный код клавиши
@param [__in] strings Исходные строки, при склейке получим входную строку
@return Ключ
*/
WORD TMacroManager::ConvertStringToKeyCode(__in const vector<string> &strings)
{
	string str = strings[0];

	for (int i = 1; i < (int)strings.size() - 3; i++)
		str += " " + strings[i];

	WORD key = 0;

	if (str.length() == 1)
		key = *str.c_str();
	else if (str.length())
	{
		str = ToUpperA(str);

		if (str == "ESC")
			key = VK_ESCAPE;
		else if (str == "BACKSPACE")
			key = VK_BACK;
		else if (str == "TAB")
			key = VK_TAB;
		else if (str == "ENTER")
			key = VK_RETURN;
		else if (str == "CTRL")
			key = VK_CONTROL;
		else if (str == "ALT")
			key = VK_MENU;
		else if (str == "SHIFT")
			key = VK_SHIFT;
		else if (str == "SPACE")
			key = VK_SPACE;
		else if (str == "CAPS LOCK")
			key = VK_CAPITAL;
		else if (str == "F1")
			key = VK_F1;
		else if (str == "F2")
			key = VK_F2;
		else if (str == "F3")
			key = VK_F3;
		else if (str == "F4")
			key = VK_F4;
		else if (str == "F5")
			key = VK_F5;
		else if (str == "F6")
			key = VK_F6;
		else if (str == "F7")
			key = VK_F7;
		else if (str == "F8")
			key = VK_F8;
		else if (str == "F9")
			key = VK_F9;
		else if (str == "F10")
			key = VK_F10;
		else if (str == "F11")
			key = VK_F11;
		else if (str == "F12")
			key = VK_F12;
		else if (str == "PAUSE")
			key = VK_PAUSE;
		else if (str == "SCROLL LOCK")
			key = VK_SCROLL;
		else if (str == "NUM 0")
			key = VK_NUMPAD0;
		else if (str == "NUM 1")
			key = VK_NUMPAD1;
		else if (str == "NUM 2")
			key = VK_NUMPAD2;
		else if (str == "NUM 3")
			key = VK_NUMPAD3;
		else if (str == "NUM 4")
			key = VK_NUMPAD4;
		else if (str == "NUM 5")
			key = VK_NUMPAD5;
		else if (str == "NUM 6")
			key = VK_NUMPAD6;
		else if (str == "NUM 7")
			key = VK_NUMPAD7;
		else if (str == "NUM 8")
			key = VK_NUMPAD8;
		else if (str == "NUM 9")
			key = VK_NUMPAD9;
		else if (str == "NUM *")
			key = 42;
		else if (str == "NUM -")
			key = 45;
		else if (str == "NUM +")
			key = 43;
		else if (str == "NUM DEL")
			key = 46;
	}

	return key;
}
//---------------------------------------------------------------------------
/*!
Сконвертировать файл макросов оригинального клиента
@param [__in] path Путь к файлу с макросами
@return true при успешном конвертировании
*/
bool TMacroManager::Convert(__in string path)
{
	TTextFileParser file(path.c_str(), " ", "", "");

	//Позиции доп. кнопок в списке, индыксация с конца, т.е. strings.size() - position
	const int MACRO_POSITION_ALT = 2;
	const int MACRO_POSITION_CTRL = 3;
	const int MACRO_POSITION_SHIFT = 1;

	while (!file.IsEOF())
	{
		vector<string> strings = file.ReadTokens();
		int size = strings.size();

		if (!size)
			continue;

		if (size != 4)
		{
			EPRINT("Macros converter. Unwaited start args count = %i!!!\n", size);
			continue;
		}

		//TPRINT("Key: %s [alt=%i ctrl=%i shift=%i]\n", strings[0].c_str(), atoi(strings[MACRO_ALT_POSITION].c_str()), atoi(strings[MACRO_CTRL_POSITION].c_str()), atoi(strings[MACRO_SHIFT_POSITION].c_str()));
		bool macroAdded = false;

		TMacro *macro = new TMacro(ConvertStringToKeyCode(strings), atoi(strings[size - MACRO_POSITION_ALT].c_str()), atoi(strings[size - MACRO_POSITION_CTRL].c_str()), atoi(strings[size - MACRO_POSITION_SHIFT].c_str()));

		while (!file.IsEOF())
		{
			PBYTE filePtr = file.GetFilePtr();
			vector<string> data = file.ReadTokens();

			if (!data.size())
				continue;

			//Конец секции макросов
			if (*data[0].c_str() == '#')
			{
				macroAdded = true;

				Add(macro);

				break;
			}

			string upData = ToUpperA(data[0]);
			MACRO_CODE code = MC_NONE;

			IFOR(i, 0, TMacro::MACRO_ACTION_NAME_COUNT)
			{
				if (upData == ToUpperA(TMacro::m_MacroActionName[i]))
				{
					code = (MACRO_CODE)i;

					//TPRINT("Action found (%i): %s\n", i, TMacro::m_MacroActionName[i]);

					break;
				}
			}
			
			if (code != MC_NONE)
			{
				TMacroObject *obj = TMacro::CreateMacro(code, false);

				if (obj->GetType() == 2) //Аргументы - строка
				{
					filePtr += data[0].length() + 1;

					PBYTE end = filePtr;

					while (*end && *end != '\n' && *end != '\r')
						end++;

					if (end - filePtr > 0)
					{
						string args((char*)filePtr, end - filePtr);
						//TPRINT("\tSub action string is: %s\n", args.c_str());

						((TMacroObjectString*)obj)->String = args;
					}
				}
				else if (data.size() > 1) //Аргументы - код (значение), либо просто код макроса
				{
					upData = data[1];

					IFOR(i, 2, (int)data.size())
						upData += " " + data[i];

					upData = ToUpperA(upData);

					IFOR(i, 0, TMacro::MACRO_ACTION_COUNT)
					{
						if (upData == ToUpperA(TMacro::m_MacroAction[i]))
						{
							obj->SubCode = (MACRO_SUB_CODE)i;

							//TPRINT("\tSub action found (%i): %s\n", i, TMacro::m_MacroAction[i]);

							break;
						}
					}
				}
			}
		}

		//TPRINT("Cycle ends with add: %i\n", macroAdded);

		if (!macroAdded)
			Add(macro);
	}

	return PathFileExistsA(path.c_str());
}
//---------------------------------------------------------------------------
/*!
Загрузить макросы из конфига
@param [__in] path Путь к файлу конфига
@return 
*/
bool TMacroManager::Load(__in string path, __in string originalPath)
{
	bool result = false;
	Clear();

	TMappedHeader file;
	memset(&file, 0, sizeof(file));

	FileManager.LoadFileToMemory(file, path.c_str());

	if (file.Size)
	{
		BYTE version = file.ReadByte();

		short count = file.ReadShort();

		IFOR(i, 0, count)
			Add(TMacro::Load(file));

		FileManager.UnloadFileFromMemory(file);

		result = true;
	}
	else
		result = Convert(originalPath);

	return result;
}
//---------------------------------------------------------------------------
/*!
Сохранить макросы в конфиг
@param [__in] path Путь к файлу конфига
@return 
*/
void TMacroManager::Save( __in string path)
{
	TFileWriter *writer = new TFileWriter(path, true);

	writer->WriteByte(0); //verison

	short count = GetItemsCount();

	writer->WriteShort(count);
	writer->WriteBuffer();

	TMacro *obj = (TMacro*)m_Items;

	while (obj != NULL)
	{
		obj->Save(writer);

		obj = (TMacro*)obj->m_Next;
	}

	writer->WriteDWord(0); //EOF
	writer->WriteBuffer();

	delete writer;
}
//---------------------------------------------------------------------------
/*!
Поиск макроса
@param [__in] key Индекс кнопки
@param [__in] alt Зажатый альт
@param [__in] ctrl Зажатый контрол
@param [__in] shift Зажатый шифт
@return Ссылку на макрос или NULL
*/
TMacro *TMacroManager::FindMacro(__in WORD key, __in bool alt, __in bool ctrl, __in bool shift)
{
	TMacro *obj = (TMacro*)m_Items;

	while (obj != NULL)
	{
		if (obj->Key == key && obj->Alt == alt && obj->Ctrl == ctrl && obj->Shift == shift)
			break;

		obj = (TMacro*)obj->m_Next;
	}

	return obj;
}
//---------------------------------------------------------------------------
/*!
Загрузить макросы из опций
@return 
*/
void TMacroManager::LoadFromOptions()
{
	Clear();
	MacroPointer = NULL;

	TMacro *obj = (TMacro*)OptionsMacroManager->m_Items;

	while (obj != NULL)
	{
		Add(obj->GetCopy());

		obj = (TMacro*)obj->m_Next;
	}
}
//---------------------------------------------------------------------------
/*!
Начать выполнение макроса
@return 
*/
void TMacroManager::Execute()
{
	while (MacroPointer != NULL)
	{
		switch (MacroManager->Process())
		{
			case MRC_STOP:
			{
				MacroPointer = NULL;
				return;
			}
			case MRC_BREAK_PARSER:
				return;
			case MRC_PARSE_NEXT:
			{
				MacroPointer = (TMacroObject*)MacroPointer->m_Next;
				break;
			}
			default:
				break;
		}
	}
}
//---------------------------------------------------------------------------
/*!
Выполнить команды подменю
@return 
*/
void TMacroManager::ProcessSubMenu()
{
	switch (MacroPointer->Code)
	{
		case MC_OPEN:
		{
			switch (MacroPointer->SubCode)
			{
				case MSC_G2_CONFIGURATION:
				{
					Orion->OpenConfiguration();

					break;
				}
				case MSC_G2_PAPERDOLL:
				{
					Orion->PaperdollReq(g_PlayerSerial);

					break;
				}
				case MSC_G2_STATUS:
				{
					Orion->OpenStatus(g_PlayerSerial);

					break;
				}
				case MSC_G2_JOURNAL:
				{
					Orion->OpenJournal();

					break;
				}
				case MSC_G2_SKILLS:
				{
					Orion->OpenSkills();

					break;
				}
				case MSC_G2_MAGE_SPELLBOOK:
				case MSC_G2_NECRO_SPELLBOOK:
				case MSC_G2_PALADIN_SPELLBOOK:
				case MSC_G2_BUSHIDO_SPELLBOOK:
				case MSC_G2_NINJITSU_SPELLBOOK:
				case MSC_G2_SPELL_WEAVING_SPELLBOOK:
				case MSC_G2_MYSTICISM_SPELLBOOK:
				{
					SPELLBOOK_TYPE type = ST_MAGE;

					switch (MacroPointer->SubCode)
					{
						case MSC_G2_NECRO_SPELLBOOK:
						{
							type = ST_NECRO;
							break;
						}
						case MSC_G2_PALADIN_SPELLBOOK:
						{
							type = ST_PALADIN;
							break;
						}
						case MSC_G2_BUSHIDO_SPELLBOOK:
						{
							type = ST_BUSHIDO;
							break;
						}
						case MSC_G2_NINJITSU_SPELLBOOK:
						{
							type = ST_NINJITSU;
							break;
						}
						case MSC_G2_SPELL_WEAVING_SPELLBOOK:
						{
							type = ST_SPELL_WEAVING;
							break;
						}
						case MSC_G2_MYSTICISM_SPELLBOOK:
						{
							type = ST_MYSTICISM;
							break;
						}
						default:
							break;
					}

					TPacketOpenSpellbook packet(type);
					packet.Send();

					break;
				}
				case MSC_G2_CHAT:
				{
					break;
				}
				case MSC_G2_BACKPACK:
				{
					TGameItem *backpack = g_Player->FindLayer(OL_BACKPACK);

					if (backpack != NULL)
						Orion->DoubleClick(backpack->Serial);

					break;
				}
				case MSC_G2_OWERVIEW:
				{
					Orion->OpenMinimap();

					break;
				}
				case MSC_G2_WORLD_MAP:
				{
					Orion->OpenWorldMap();

					break;
				}
				case MSC_G2_MAIL:
				case MSC_G2_PARTY_MANIFEST:
				{
					Orion->OpenPartyManifest();

					break;
				}
				case MSC_G2_GUILD:
				{
					TPacketOpenGuildGump packet;
					packet.Send();

					break;
				}
				case MSC_G2_QUEST_LOG:
				{
					TPacketOpenQuestGump packet;
					packet.Send();

					break;
				}
				case MSC_G2_PARTY_CHAT:
				case MSC_G2_COMBAT_BOOK:
				case MSC_G2_RACIAL_ABILITIES_BOOK:
				case MSC_G2_BARD_SPELLBOOK:
				{
					break;
				}
				default:
					break;
			}

			break;
		}
		case MC_CLOSE:
		case MC_MINIMIZE:
		case MC_MAXIMIZE:
		{
			TGump *gump = NULL;

			switch (MacroPointer->SubCode)
			{
				case MSC_G2_CONFIGURATION:
				{
					gump = GumpManager->GetGump(g_PlayerSerial, 0, GT_OPTIONS);

					OptionsMacroManager->Clear();

					break;
				}
				case MSC_G2_PAPERDOLL:
				{
					gump = GumpManager->GetGump(g_PlayerSerial, 0, GT_PAPERDOLL);

					break;
				}
				case MSC_G2_STATUS:
				{
					gump = GumpManager->GetGump(g_PlayerSerial, 0, GT_STATUSBAR);

					break;
				}
				case MSC_G2_JOURNAL:
				{
					gump = GumpManager->GetGump(g_PlayerSerial, 0, GT_JOURNAL);

					break;
				}
				case MSC_G2_SKILLS:
				{
					gump = GumpManager->GetGump(g_PlayerSerial, 0, GT_SKILLS);

					break;
				}
				case MSC_G2_MAGE_SPELLBOOK:
				{
					gump = GumpManager->GetGump(0, 0, GT_SPELLBOOK);

					break;
				}
				case MSC_G2_CHAT:
				{
					gump = GumpManager->GetGump(0, 0, GT_CHAT);

					break;
				}
				case MSC_G2_BACKPACK:
				{
					TGameItem *backpack = g_Player->FindLayer(OL_BACKPACK);

					if (backpack != NULL)
						GumpManager->CloseGump(backpack->Serial, 0, GT_SPELLBOOK);

					break;
				}
				case MSC_G2_OWERVIEW:
				{
					gump = GumpManager->GetGump(g_PlayerSerial, 0, GT_MINIMAP);

					break;
				}
				case MSC_G2_MAIL:
				{
					gump = GumpManager->GetGump(0, 0, GT_MAIL);

					break;
				}
				case MSC_G2_PARTY_MANIFEST:
				{
					gump = GumpManager->GetGump(0, 0, GT_PARTY_MANIFEST);

					break;
				}
				case MSC_G2_PARTY_CHAT:
				case MSC_G2_NECRO_SPELLBOOK:
				case MSC_G2_PALADIN_SPELLBOOK:
				case MSC_G2_COMBAT_BOOK:
				case MSC_G2_BUSHIDO_SPELLBOOK:
				case MSC_G2_NINJITSU_SPELLBOOK:
				case MSC_G2_GUILD:
				case MSC_G2_SPELL_WEAVING_SPELLBOOK:
				case MSC_G2_QUEST_LOG:
				case MSC_G2_MYSTICISM_SPELLBOOK:
				case MSC_G2_RACIAL_ABILITIES_BOOK:
				case MSC_G2_BARD_SPELLBOOK:
				{
					break;
				}
				default:
					break;
			}

			if (gump != NULL)
			{
				if (MacroPointer->Code == MC_CLOSE)
					GumpManager->RemoveGump(gump);
				else
				{
					gump->Minimized = (MacroPointer->Code == MC_MINIMIZE);

					gump->FrameCreated = false;
				}
			}

			break;
		}
		default:
			break;
	}
}
//---------------------------------------------------------------------------
/*!
Выполнить действие макроса (или набор действий)
@return Код выполнения
*/
MACRO_RETURN_CODE TMacroManager::Process()
{
	MACRO_RETURN_CODE result = MRC_PARSE_NEXT;
	static DWORD itemInHand[2] = { 0, 0 };

	if (MacroPointer == NULL)
		result = MRC_STOP;
	else if (m_NextTimer <= GetTickCount())
	{
		switch (MacroPointer->Code)
		{
			case MC_SAY:
			case MC_EMOTE:
			case MC_WHISPER:
			case MC_YELL:
			{
				TMacroObjectString *mos = (TMacroObjectString*)MacroPointer;

				if (mos->GetString().length())
				{
					SPEECH_TYPE st = ST_NORMAL;

					switch (MacroPointer->Code)
					{
						case MC_EMOTE:
						{
							st = ST_EMOTE;
							break;
						}
						case MC_WHISPER:
						{
							st = ST_WHISPER;
							break;
						}
						case MC_YELL:
						{
							st = ST_YELL;
							break;
						}
						default:
							break;
					}

					TPacketASCIISpeechRequest packet(mos->GetString().c_str(), st, 3, ConfigManager.SpeechColor);
					packet.Send();
					packet.Free();
				}

				break;
			}
			case MC_WALK:
			{
				BYTE dt = (BYTE)DT_NW;
				if (MacroPointer->SubCode != MSC_G1_NW)
				{
					dt = (BYTE)(MacroPointer->Code - 2);

					if (dt > 7)
						dt = 0;
				}

				if (!PathFinder->AutoWalking)
					PathFinder->Walk(false, dt);

				break;
			}
			case MC_WAR_PEACE:
			{
				Orion->ChangeWarmode();
				
				break;
			}
			case MC_PASTE:
			{
				if (EntryPointer != NULL)
				{
					if (OpenClipboard(g_hWnd))
					{
						HANDLE cb = GetClipboardData(CF_TEXT);
						
						if (cb != NULL)
						{
							char* chBuffer = (char*)GlobalLock(cb);

							if (strlen(chBuffer))
							{
								wstring str = EntryPointer->Data() + ToWString(chBuffer);
								EntryPointer->SetText(str);
							}

							GlobalUnlock(cb);
						}

						CloseClipboard();
					}
				}

				break;
			}
			case MC_OPEN:
			case MC_CLOSE:
			case MC_MINIMIZE:
			case MC_MAXIMIZE:
			{
				ProcessSubMenu();

				break;
			}
			case MC_OPEN_DOOR:
			{
				Orion->OpenDoor();

				break;
			}
			case MC_USE_SKILL:
			{
				int skill = (MacroPointer->SubCode - MSC_G3_ANATOMY);

				if (skill >= 0 && skill < 24)
				{
					skill = m_SkillIndexTable[skill];
					
					if (skill != 0xFF)
						Orion->UseSkill(skill);
				}

				break;
			}
			case MC_LAST_SKILL:
			{
				Orion->UseSkill(g_LastSkillIndex);

				break;
			}
			case MC_CAST_SPELL:
			{
				int spell = (MacroPointer->SubCode - MSC_G6_CLUMSY + 1);

				if (spell > 0 && spell <= 143)
					Orion->CastSpell(spell);

				break;
			}
			case MC_LAST_SPELL:
			{
				Orion->CastSpell(g_LastSpellIndex);

				break;
			}
			case MC_LAST_OBJECT:
			{
				if (World->FindWorldObject(g_LastUseObject))
					Orion->DoubleClick(g_LastUseObject);

				break;
			}
			case MC_BOW:
			case MC_SALUTE:
			{
				const char *emote[2] = {"bow", "salute"};

				int index = MacroPointer->Code - MC_BOW;

				Orion->EmoteAction(emote[index]);

				break;
			}
			case MC_QUIT_GAME:
			{
				Orion->OpenLogOut();

				break;
			}
			case MC_ALL_NAMES:
			{
				Orion->AllNames();

				break;
			}
			case MC_LAST_TARGET:
			{
				if (m_WaitForTargetTimer == 0)
					m_WaitForTargetTimer = GetTickCount() + 1000;

				if (Target.IsTargeting())
				{
					Target.SendLastTarget();

					m_WaitForTargetTimer = 0;
				}
				else if (m_WaitForTargetTimer < GetTickCount())
					m_WaitForTargetTimer = 0;
				else
					result = MRC_BREAK_PARSER;

				break;
			}
			case MC_TARGET_SELF:
			{
				if (m_WaitForTargetTimer == 0)
					m_WaitForTargetTimer = GetTickCount() + 1000;

				if (Target.IsTargeting())
				{
					Target.SendTargetObject(g_PlayerSerial);

					m_WaitForTargetTimer = 0;
				}
				else if (m_WaitForTargetTimer < GetTickCount())
					m_WaitForTargetTimer = 0;
				else
					result = MRC_BREAK_PARSER;

				break;
			}
			case MC_ARM_DISARM:
			{
				int handIndex = 1 - (MacroPointer->SubCode - MSC_G4_LEFT_HAND);

				if (handIndex < 0 || handIndex > 1 || ObjectInHand != NULL)
					break;

				if (itemInHand[handIndex])
				{
					TGameItem *objHand = World->FindWorldItem(itemInHand[handIndex]);

					if (objHand != NULL)
					{
						Orion->PickupItem(objHand, 1, false);
						Orion->EquipItem(g_PlayerSerial);

						if (ObjectInHand != NULL)
						{
							delete ObjectInHand;
							ObjectInHand = NULL;
						}
					}

					itemInHand[handIndex] = 0;
				}
				else
				{
					DWORD backpack = 0;
					TGameItem *objBackpack = g_Player->FindLayer(OL_BACKPACK);

					if (objBackpack != NULL)
						backpack = objBackpack->Serial;

					if (!backpack)
						break;

					TGameItem *objHand = g_Player->FindLayer(OL_1_HAND + handIndex);

					if (objHand != NULL)
					{
						itemInHand[handIndex] = objHand->Serial;

						Orion->PickupItem(objHand, 1, false);
						Orion->DropItem(backpack, 0xFFFF, 0xFFFF, 0);

						if (ObjectInHand != NULL)
						{
							delete ObjectInHand;
							ObjectInHand = NULL;
						}

						GumpManager->UpdateGump(g_PlayerSerial, 0, GT_PAPERDOLL);
					}
				}

				break;
			}
			case MC_WAIT_FOR_TARGET:
			{
				if (m_WaitForTargetTimer == 0)
					m_WaitForTargetTimer = GetTickCount() + 1000;

				if (Target.IsTargeting() || m_WaitForTargetTimer < GetTickCount())
					m_WaitForTargetTimer = 0;
				else
					result = MRC_BREAK_PARSER;

				break;
			}
			case MC_TARGET_NEXT:
			{
				TGameObject *obj = World->SearchWorldObject(g_LastTargetObject, 30, STO_MOBILES, SMO_NEXT);

				if (obj != NULL)
				{
					g_LastTargetObject = obj->Serial;
					g_LastAttackObject = obj->Serial;
				}

				/*TGameObject *start = World->FindWorldObject(g_LastTargetObject);

				int count = 2;
				int startI = 0;

				if (start == NULL || start->m_Next == NULL)
				{
					start = World->m_Items;
					startI = 1;
				}
				else
					start = (TGameObject*)start->m_Next;

				if (start != NULL)
				{
					TGameObject *obj = start;
					bool found = false;

					IFOR(i, startI, count)
					{
						if (i)
							obj = World->m_Items;

						while (obj != NULL)
						{
							if (obj->NPC && !obj->IsPlayer() && obj->Serial != g_LastTargetObject)
							{
								g_LastTargetObject = obj->Serial;

								found = true;

								break;
							}

							obj = (TGameObject*)obj->m_Next;
						}

						if (found)
							break;
					}
				}*/

				break;
			}
			case MC_ATTACK_LAST:
			{
				Orion->Attack(g_LastAttackObject);

				break;
			}
			case MC_DELAY:
			{
				TMacroObjectString *mos = (TMacroObjectString*)MacroPointer;

				string str = mos->GetString();
				
				if (str.length())
					m_NextTimer = GetTickCount() + std::atoi(str.c_str());

				break;
			}
			case MC_CIRCLE_TRANS:
			{
				ConfigManager.UseCircleTrans = !ConfigManager.UseCircleTrans;

				break;
			}
			case MC_CLOSE_GUMPS:
			{
				TGump *gump = (TGump*)GumpManager->m_Items;

				while (gump != NULL)
				{
					TGump *next = (TGump*)gump->m_Next;

					if (gump->GumpType == GT_OPTIONS)
						OptionsMacroManager->Clear();

					if (gump->GumpType != GT_MENUBAR && gump->GumpType != GT_BUFF && (gump->GumpType != GT_GENERIC && !gump->NoClose))
						GumpManager->RemoveGump(gump);

					gump = next;
				}

				break;
			}
			case MC_ALWAYS_RUN:
			{
				ConfigManager.AlwaysRun = !ConfigManager.AlwaysRun;

				break;
			}
			case MC_SAVE_DESKTOP:
			{
				Orion->SaveLocalConfig();

				break;
			}
			case MC_ENABLE_RANGE_COLOR:
			{
				ConfigManager.GrayOutOfRangeObjects = true;

				break;
			}
			case MC_DISABLE_RANGE_COLOR:
			{
				ConfigManager.GrayOutOfRangeObjects = false;

				break;
			}
			case MC_TOGGLE_RANGE_COLOR:
			{
				ConfigManager.GrayOutOfRangeObjects = !ConfigManager.GrayOutOfRangeObjects;

				break;
			}
			case MC_ATTACK_SELECTED_TARGET:
			{
				if (!ConfigManager.DisableNewTargetSystem && NewTargetSystem.Serial && NewTargetSystem.Serial < 0x40000000)
					Orion->Attack(NewTargetSystem.Serial);

				break;
			}
			case MC_USE_SELECTED_TARGET:
			{
				if (!ConfigManager.DisableNewTargetSystem && NewTargetSystem.Serial)
					Orion->DoubleClick(NewTargetSystem.Serial);

				break;
			}
			case MC_CURRENT_TARGET:
			{
				if (!ConfigManager.DisableNewTargetSystem && NewTargetSystem.Serial)
				{
					if (m_WaitForTargetTimer == 0)
						m_WaitForTargetTimer = GetTickCount() + 1000;

					if (Target.IsTargeting())
					{
						Target.SendTargetObject(NewTargetSystem.Serial);

						m_WaitForTargetTimer = 0;
					}
					else if (m_WaitForTargetTimer < GetTickCount())
						m_WaitForTargetTimer = 0;
					else
						result = MRC_BREAK_PARSER;
				}

				break;
			}
			case MC_TARGET_SYSTEM_ON_OFF:
			{
				ConfigManager.DisableNewTargetSystem = !ConfigManager.DisableNewTargetSystem;
				
				break;
			}
			case MC_BANDAGE_SELF:
			case MC_BANDAGE_TARGET:
			{
				if (m_WaitingBandageTarget)
				{
					if (m_WaitForTargetTimer == 0)
						m_WaitForTargetTimer = GetTickCount() + 500;

					if (Target.IsTargeting())
					{
						if (MacroPointer->Code == MC_BANDAGE_SELF)
							Target.SendTargetObject(g_PlayerSerial);
						else if (!ConfigManager.DisableNewTargetSystem && NewTargetSystem.Serial)
							Target.SendTargetObject(NewTargetSystem.Serial);

						m_WaitingBandageTarget = false;
						m_WaitForTargetTimer = 0;
					}
					else if (m_WaitForTargetTimer < GetTickCount())
					{
						m_WaitingBandageTarget = false;
						m_WaitForTargetTimer = 0;
					}
					else
						result = MRC_BREAK_PARSER;
				}
				else
				{
					TGameItem *bandage = g_Player->FindBandage();

					if (bandage != NULL)
					{
						m_WaitingBandageTarget = true;
						Orion->DoubleClick(bandage->Serial);

						result = MRC_BREAK_PARSER;
					}
				}

				break;
			}
			case MC_SET_UPDATE_RANGE:
			case MC_MODIFY_UPDATE_RANGE:
			{
				TMacroObjectString *mos = (TMacroObjectString*)MacroPointer;

				string str = mos->GetString();

				if (str.length())
				{
					g_UpdateRange = std::atoi(str.c_str());

					if (g_UpdateRange < 5)
						g_UpdateRange = 5;
					else if (g_UpdateRange > 18)
						g_UpdateRange = 18;
				}

				break;
			}
			case MC_INCREASE_UPDATE_RANGE:
			{
				g_UpdateRange++;

				if (g_UpdateRange > 18)
					g_UpdateRange = 18;

				break;
			}
			case MC_DECREASE_UPDATE_RANGE:
			{
				g_UpdateRange--;

				if (g_UpdateRange < 5)
					g_UpdateRange = 5;

				break;
			}
			case MC_MAX_UPDATE_RANGE:
			{
				g_UpdateRange = 18;

				break;
			}
			case MC_MIN_UPDATE_RANGE:
			{
				g_UpdateRange = 5;

				break;
			}
			case MC_DEFAULT_UPDATE_RANGE:
			{
				g_UpdateRange = 18;

				break;
			}
			case MC_UPDATE_RANGE_INFO:
			{
				Orion->CreateTextMessageF(3, 0, "Current update range is %i", g_UpdateRange);

				break;
			}
			case MC_SELECT_NEXT:
			case MC_SELECT_PREVEOUS:
			case MC_SELECT_NEAREST:
			{
				if (ConfigManager.DisableNewTargetSystem)
					break;
				
				SCAN_TYPE_OBJECT scanType = (SCAN_TYPE_OBJECT)(MacroPointer->SubCode - MSC_G7_HOSTLE);

				TGameObject *obj = World->SearchWorldObject(NewTargetSystem.Serial, 10, scanType, (SCAN_MODE_OBJECT)(MacroPointer->Code - MC_SELECT_NEXT));

				if (obj != NULL)
				{
					GumpManager->CloseGump(NewTargetSystem.Serial, 0, GT_TARGET_SYSTEM);

					NewTargetSystem.Serial = obj->Serial;

					if (GumpManager->GetGump(NewTargetSystem.Serial, 0, GT_TARGET_SYSTEM) == NULL)
					{
						if (NewTargetSystem.Serial < 0x40000000)
						{
							TPacketStatusRequest packet(NewTargetSystem.Serial);
							packet.Send();
						}

						GumpManager->AddGump(new TGumpTargetSystem(NewTargetSystem.Serial, NewTargetSystem.GumpX, NewTargetSystem.GumpY));
					}
				}
				else
				{
					const char *resultNames[5] = {"Hostles", "Party Members", "Followers", "Objects", "Mobiles"};

					Orion->CreateUnicodeTextMessageF(0, 0x038A, "There are no %s on the screen to select.", resultNames[scanType]);
				}

				break;
			}
			case MC_TOGGLE_BUICON_WINDOW:
			{
				ConfigManager.ToggleBufficonWindow = !ConfigManager.ToggleBufficonWindow;

				break;
			}
			case MC_INVOKE_VIRTURE:
			{
				BYTE id = MacroPointer->SubCode - MSC_G5_HONOR + 31;

				TPacketInvokeVirtureRequest packet(id);
				packet.Send();

				break;
			}
			case MC_KILL_GUMP_OPEN:
			case MC_PRIMARY_ABILITY:
			case MC_SECONDARY_ABILITY:
			case MC_EQUIP_LAST_WEAPON:
			case MC_TOGGLE_GARGOYLE_FLYING:
			{
				Orion->CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0x77, "That macro is not work now");

				break;
			}
			default:
				break;
		}
	}
	else
		result = MRC_BREAK_PARSER;

	return result;
}
//---------------------------------------------------------------------------