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
void TMacroManager::Load(string path)
{
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
	}
}
//---------------------------------------------------------------------------
void TMacroManager::Save(string path)
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
TMacro *TMacroManager::FindMacro(WORD key, bool alt, bool ctrl, bool shift)
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
					UO->OpenConfiguration();

					break;
				}
				case MSC_G2_PAPERDOLL:
				{
					UO->PaperdollReq(g_PlayerSerial);

					break;
				}
				case MSC_G2_STATUS:
				{
					UO->OpenStatus(g_PlayerSerial);

					break;
				}
				case MSC_G2_JOURNAL:
				{
					UO->OpenJournal();

					break;
				}
				case MSC_G2_SKILLS:
				{
					UO->OpenSkills();

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
						UO->DoubleClick(backpack->Serial);

					break;
				}
				case MSC_G2_OWERVIEW:
				{
					UO->OpenMinimap();

					break;
				}
				case MSC_G2_WORLD_MAP:
				{
					UO->OpenWorldMap();

					break;
				}
				case MSC_G2_MAIL:
				case MSC_G2_PARTY_MANIFEST:
				{
					UO->OpenPartyManifest();

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

					gump->UpdateFrame();
				}
			}

			break;
		}
		default:
			break;
	}
}
//---------------------------------------------------------------------------
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
				UO->ChangeWarmode();
				
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
				UO->OpenDoor();

				break;
			}
			case MC_USE_SKILL:
			{
				int skill = (MacroPointer->SubCode - MSC_G3_ANATOMY);

				if (skill >= 0 && skill < 24)
				{
					skill = m_SkillIndexTable[skill];
					
					if (skill != 0xFF)
						UO->UseSkill(skill);
				}

				break;
			}
			case MC_LAST_SKILL:
			{
				UO->UseSkill(g_LastSkillIndex);

				break;
			}
			case MC_CAST_SPELL:
			{
				int spell = (MacroPointer->SubCode - MSC_G6_CLUMSY + 1);

				if (spell > 0 && spell <= 143)
					UO->CastSpell(spell);

				break;
			}
			case MC_LAST_SPELL:
			{
				UO->CastSpell(g_LastSpellIndex);

				break;
			}
			case MC_LAST_OBJECT:
			{
				if (World->FindWorldObject(g_LastUseObject))
					UO->DoubleClick(g_LastUseObject);

				break;
			}
			case MC_BOW:
			case MC_SALUTE:
			{
				const char *emote[2] = {"bow", "salute"};

				int index = MacroPointer->Code - MC_BOW;

				UO->EmoteAction(emote[index]);

				break;
			}
			case MC_QUIT_GAME:
			{
				UO->OpenLogOut();

				break;
			}
			case MC_ALL_NAMES:
			{
				UO->AllNames();

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

				if (handIndex < 0 || handIndex > 1)
					break;

				if (itemInHand[handIndex])
				{
					TGameItem *objHand = World->FindWorldItem(itemInHand[handIndex]);

					if (objHand != NULL)
					{
						UO->PickupItem(objHand, 1, false);
						UO->EquipItem(g_PlayerSerial);
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

						UO->PickupItem(objHand, 1, false);
						UO->DropItem(backpack, 0xFFFF, 0xFFFF, 0);

						TGump *paperdoll = GumpManager->GetGump(g_PlayerSerial, 0, GT_PAPERDOLL);

						if (paperdoll != NULL)
							paperdoll->FrameCreated = false;
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
					g_LastTargetObject = obj->Serial;

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
				UO->Attack(g_LastAttackObject);

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
					{
						OptionsMacroManager->Clear();
					}

					if (gump->GumpType != GT_MENUBAR)
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
				UO->SaveLocalConfig();

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
				if (!ConfigManager.DisableNewTargetSystem && NewTargetSystem.Serial && NewTargetSystem.Serial >= 0x40000000)
					UO->Attack(NewTargetSystem.Serial);

				break;
			}
			case MC_USE_SELECTED_TARGET:
			{
				if (!ConfigManager.DisableNewTargetSystem && NewTargetSystem.Serial)
					UO->DoubleClick(NewTargetSystem.Serial);

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
						UO->DoubleClick(bandage->Serial);

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
				UO->CreateTextMessageF(3, 0, "Current update range is %i", g_UpdateRange);

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
						GumpManager->AddGump(new TGumpTargetSystem(NewTargetSystem.Serial, NewTargetSystem.GumpX, NewTargetSystem.GumpY));
				}
				else
				{
					const char *resultNames[5] = {"Hostles", "Party Members", "Followers", "Objects", "Mobiles"};

					UO->CreateUnicodeTextMessageF(0, 0x038A, "There are no %s on the screen to select.", resultNames[scanType]);
				}

				break;
			}
			case MC_TOGGLE_BUICON_WINDOW:
			{
				TGump *gump = GumpManager->GetGump(0, 0, GT_BUFF);

				if (gump != NULL)
					GumpManager->CloseGump(0, 0, GT_BUFF);
				else
				{
					//GumpManager->AddGump(new TGumpBuff());
				}

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
				UO->CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0x77, "That macro is not work now");

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