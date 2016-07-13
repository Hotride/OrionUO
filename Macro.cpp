/****************************************************************************
**
** Macro.cpp
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

TMacroObject *MacroPointer = NULL;
//---------------------------------------------------------------------------
//--------------------------------TMacroObject-------------------------------
//---------------------------------------------------------------------------
TMacroObject::TMacroObject(MACRO_CODE code, MACRO_SUB_CODE subCode)
: TBaseQueueItem(), m_Code(code), m_SubCode(subCode)
{
	switch (code)
	{
		//With sub menu
		case MC_WALK:
		case MC_OPEN:
		case MC_CLOSE:
		case MC_MINIMIZE:
		case MC_MAXIMIZE:
		case MC_USE_SKILL:
		case MC_ARM_DISARM:
		case MC_INVOKE_VIRTURE:
		case MC_CAST_SPELL:
		case MC_SELECT_NEXT:
		case MC_SELECT_PREVEOUS:
		case MC_SELECT_NEAREST:
		{
			if (subCode == MSC_NONE)
			{
				int count = 0;
				int offset = 0;
				TMacro::GetBoundByCode(code, count, offset);
				m_SubCode = (MACRO_SUB_CODE)offset;
			}

			m_HasSubMenu = 1;

			break;
		}
		//With entry text
		case MC_SAY:
		case MC_EMOTE:
		case MC_WHISPER:
		case MC_YELL:
		case MC_DELAY:
		case MC_SET_UPDATE_RANGE:
		case MC_MODIFY_UPDATE_RANGE:
		{
			m_HasSubMenu = 2;

			break;
		}
		default:
		{
			m_HasSubMenu = 0;

			break;
		}
	}
}
//---------------------------------------------------------------------------
TMacroObject::~TMacroObject()
{
}
//---------------------------------------------------------------------------
//------------------------------TMacroObjectString---------------------------
//---------------------------------------------------------------------------
TMacroObjectString::TMacroObjectString(MACRO_CODE code, MACRO_SUB_CODE subCode, string str)
: TMacroObject(code, subCode), m_String(str)
{
}
//---------------------------------------------------------------------------
TMacroObjectString::~TMacroObjectString()
{
}
//---------------------------------------------------------------------------
//-------------------------------TMacroObjectEntry---------------------------
//---------------------------------------------------------------------------
TMacroObjectEntry::TMacroObjectEntry(MACRO_CODE code, MACRO_SUB_CODE subCode, string str)
: TMacroObject(code, subCode)
{
	TextEntry = new TEntryText(0, 150);

	TextEntry->SetText(str);
}
//---------------------------------------------------------------------------
TMacroObjectEntry::~TMacroObjectEntry()
{
	if (TextEntry != NULL)
	{
		delete TextEntry;
		TextEntry = NULL;
	}
}
//---------------------------------------------------------------------------
//-----------------------------------TMacro----------------------------------
//---------------------------------------------------------------------------
TMacro::TMacro(WORD key, bool alt, bool ctrl, bool shift)
: TBaseQueueItem(), m_Key(key), m_Alt(alt), m_Ctrl(ctrl), m_Shift(shift)
{
}
//---------------------------------------------------------------------------
TMacro::~TMacro()
{
}
//---------------------------------------------------------------------------
TMacro *TMacro::CreateBlankMacro()
{
	TMacro *obj = new TMacro(0, false, false, false);

	obj->Add(new TMacroObject(MC_NONE, MSC_NONE));

	return obj;
}
//---------------------------------------------------------------------------
TMacroObject *TMacro::CreateMacro(MACRO_CODE code, bool forOptionsGump)
{
	TMacroObject *obj = NULL;
	
	switch (code)
	{
		//With entry text
		case MC_SAY:
		case MC_EMOTE:
		case MC_WHISPER:
		case MC_YELL:
		case MC_DELAY:
		case MC_SET_UPDATE_RANGE:
		case MC_MODIFY_UPDATE_RANGE:
		{
			if (forOptionsGump)
				obj = new TMacroObjectEntry(code, MSC_NONE, "");
			else
				obj = new TMacroObjectString(code, MSC_NONE, "");
					
			break;
		}
		default:
		{
			obj = new TMacroObject(code, MSC_NONE);

			break;
		}
	}

	return obj;
}
//---------------------------------------------------------------------------
void TMacro::ChangeObject(TMacroObject *source, TMacroObject *obj)
{
	obj->m_Prev = source->m_Prev;
	obj->m_Next = source->m_Next;

	if (source->m_Prev == NULL)
		m_Items = obj;
	else
		source->m_Prev->m_Next = obj;

	if (source->m_Next != NULL)
		source->m_Next->m_Prev = obj;
	
	source->m_Prev = NULL;
	source->m_Next = NULL;
	delete source;
}
//---------------------------------------------------------------------------
TMacro *TMacro::Load(TMappedHeader &file)
{
	PBYTE next = file.Ptr;
	short size = file.ReadShort();
	next += size;

	WORD key = file.ReadWord();

	bool alt = false;

	if (key & MODKEY_ALT)
	{
		key -= MODKEY_ALT;
		alt = true;
	}

	bool ctrl = false;

	if (key & MODKEY_CTRL)
	{
		key -= MODKEY_CTRL;
		ctrl = true;
	}

	bool shift = false;

	if (key & MODKEY_SHIFT)
	{
		key -= MODKEY_SHIFT;
		shift = true;
	}

	WORD count = file.ReadShort();
	
	TMacro *macro = new TMacro(key, alt, ctrl, shift);

	IFOR(i, 0, count)
	{
		BYTE type = file.ReadByte();

		MACRO_CODE code = (MACRO_CODE)file.ReadWord();

		MACRO_SUB_CODE subCode = (MACRO_SUB_CODE)file.ReadWord();

		TMacroObject *obj = NULL;

		switch (type)
		{
			case 0: //original
			{
				obj = new TMacroObject(code, subCode);
				break;
			}
			case 2: //with string
			{
				short len = file.ReadShort();

				string str = file.ReadString(len);

				obj = new TMacroObjectString(code, subCode, str);

				break;
			}
			default:
				break;
		}

		if (obj != NULL)
			macro->Add(obj);
	}

	file.Ptr = next;

	return macro;
}
//---------------------------------------------------------------------------
void TMacro::Save(TFileWriter *writer)
{
	short size = 10;
	short count = 0;

	TMacroObject *obj = (TMacroObject*)m_Items;

	while (obj != NULL)
	{
		BYTE type = (BYTE)obj->GetType();

		if (type < 3)
		{
			size += 5;
			count++;

			if (type == 2) //with string
			{
				string str = ((TMacroObjectString*)obj)->GetString();
				size += str.length() + 3;
			}
		}

		obj = (TMacroObject*)obj->m_Next;
	}

	writer->WriteWord(size);

	WORD key = m_Key;

	if (m_Alt)
		key += MODKEY_ALT;

	if (m_Ctrl)
		key += MODKEY_CTRL;

	if (m_Shift)
		key += MODKEY_SHIFT;

	writer->WriteWord(key);

	writer->WriteWord(count);

	obj = (TMacroObject*)m_Items;

	while (obj != NULL)
	{
		BYTE type = (BYTE)obj->GetType();

		if (type < 3)
		{
			writer->WriteByte(type);
			writer->WriteWord(obj->Code);
			writer->WriteWord(obj->SubCode);

			if (type == 2) //with string
			{
				string str = ((TMacroObjectString*)obj)->GetString();
				int len = (int)str.length();

				writer->WriteShort(len + 1);

				if (len > 0)
					writer->WriteAsciiString(str);
			}

			writer->WriteBuffer();
		}

		obj = (TMacroObject*)obj->m_Next;
	}
	
	writer->WriteDWord(0); //EOM
	writer->WriteBuffer();
}
//---------------------------------------------------------------------------
TMacro *TMacro::GetOptionsCopy()
{
	TMacro *macro = new TMacro(m_Key, m_Alt, m_Ctrl, m_Shift);

	TMacroObject *obj = (TMacroObject*)m_Items;

	while (obj != NULL)
	{
		if (obj->GetType() == 2) //With text
		{
			/*case MC_SAY:
			case MC_EMOTE:
			case MC_WHISPER:
			case MC_YELL:
			case MC_DELAY:
			case MC_SET_UPDATE_RANGE:
			case MC_MODIFY_UPDATE_RANGE:*/

			macro->Add(new TMacroObjectEntry(obj->Code, obj->SubCode, ((TMacroObjectString*)obj)->GetString()));
		}
		else
			macro->Add(new TMacroObject(obj->Code, obj->SubCode));

		obj = (TMacroObject*)obj->m_Next;
	}

	return macro;
}
//---------------------------------------------------------------------------
TMacro *TMacro::GetCopy()
{
	TMacro *macro = new TMacro(m_Key, m_Alt, m_Ctrl, m_Shift);

	TMacroObject *obj = (TMacroObject*)m_Items;
	
	while (obj != NULL)
	{
		if (obj->GetType() == 3) //With entry text
		{
			/*case MC_SAY:
			case MC_EMOTE:
			case MC_WHISPER:
			case MC_YELL:
			case MC_DELAY:
			case MC_SET_UPDATE_RANGE:
			case MC_MODIFY_UPDATE_RANGE:*/

			TEntryText *entry = ((TMacroObjectEntry*)obj)->TextEntry;
			string str(entry->Length() ? entry->c_str() : "");

			macro->Add(new TMacroObjectString(obj->Code, obj->SubCode, str));
		}
		else
			macro->Add(new TMacroObject(obj->Code, obj->SubCode));

		obj = (TMacroObject*)obj->m_Next;
	}

	return macro;
}
//---------------------------------------------------------------------------
TMacroObject *TMacro::EntryPointerHere()
{
	TMacroObject *obj = (TMacroObject*)m_Items;

	while (obj != NULL)
	{
		if (obj->HasSubMenu == 2)
		{
			TEntryText *entry = ((TMacroObjectEntry*)obj)->TextEntry;

			if (entry == EntryPointer)
				return obj;
		}

		obj = (TMacroObject*)obj->m_Next;
	}

	return NULL;
}
//---------------------------------------------------------------------------
void TMacro::GetBoundByCode(MACRO_CODE code, int &count, int &offset)
{
	switch (code)
	{
		case MC_WALK:
		{
			offset = 1;
			count = 8;
			break;
		}
		case MC_OPEN:
		case MC_CLOSE:
		case MC_MINIMIZE:
		case MC_MAXIMIZE:
		{
			offset = 9;
			count = 24;
			break;
		}
		case MC_USE_SKILL:
		{
			offset = 33;
			count = 24;
			break;
		}
		case MC_ARM_DISARM:
		{
			offset = 57;
			count = 2;
			break;
		}
		case MC_INVOKE_VIRTURE:
		{
			offset = 59;
			count = 3;
			break;
		}
		case MC_CAST_SPELL:
		{
			offset = 62;
			count = 143;
			break;
		}
		case MC_SELECT_NEXT:
		case MC_SELECT_PREVEOUS:
		case MC_SELECT_NEAREST:
		{
			offset = 205;
			count = 5;
			break;
		}
		default:
			break;
	}

}
//---------------------------------------------------------------------------
const char *TMacro::m_MacroActionName[MACRO_ACTION_NAME_COUNT] =
{
	"(NONE)",
	"Say",
	"Emote",
	"Whisper",
	"Yell",
	"Walk",
	"War/Peace",
	"Paste",
	"Open",
	"Close",
	"Minimize",
	"Maximize",
	"OpenDoor",
	"UseSkill",
	"LastSkill",
	"CastSpell",
	"LastSpell",
	"LastObject",
	"Bow",
	"Salute",
	"QuitGame",
	"AllNames",
	"LastTarget",
	"TargetSelf",
	"Arm/Disarm",
	"WaitForTarget",
	"TargetNext",
	"AttackLast",
	"Delay",
	"CircleTrans",
	"CloseGumps",
	"AlwaysRun",
	"SaveDesktop",
	"KillGumpOpen",
	"PrimaryAbility",
	"SecondaryAbility",
	"EquipLastWeapon",
	"SetUpdateRange",
	"ModifyUpdateRange",
	"IncreaseUpdateRange",
	"DecreaseUpdateRange",
	"MaxUpdateRange",
	"MinUpdateRange",
	"DefaultUpdateRange",
	"UpdateRangeInfo",
	"EnableRangeColor",
	"DisableRangeColor",
	"ToggleRangeColor",
	"InvoreVirture",
	"SelectNext",
	"SelectPreveous",
	"SelectNearest",
	"AttackSelectedTarget",
	"UseSelectedTarget",
	"CurrentTarget",
	"TargetSystemOn/Off",
	"ToggleBuficonWindow",
	"BandageSelf",
	"BandageTarget",
	"ToggleGargoyleFlying"
};
//---------------------------------------------------------------------------
const char *TMacro::m_MacroAction[MACRO_ACTION_COUNT] =
{
	"?",
	"NW (top)", //Walk group
	"N (topright)",
	"NE (right)",
	"E (bottonright)",
	"SE (bottom)",
	"S (bottomleft)",
	"SW (left)",
	"W (topleft)",
	"Configuration", //Open/Close/Minimize/Maximize group
	"Paperdoll",
	"Status",
	"Journal",
	"Skills",
	"Mage Spellbook",
	"Chat",
	"Backpack",
	"Overview",
	"World Map",
	"Mail",
	"Party Manifest",
	"Party Chat",
	"Necro Spellbook",
	"Paladin Spellbook",
	"Combat Book",
	"Bushido Spellbook",
	"Ninjitsu Spellbook",
	"Guild",
	"Spell Weaving Spellbook",
	"Quest Log",
	"Mysticism Spellbook",
	"Racial Abilities Book",
	"Bard Spellbook",
	"Anatomy", //Skills group
	"Animal Lore",
	"Animal Taming",
	"Arms Lore",
	"Begging",
	"Cartograpy",
	"Detecting Hidden",
	"Enticement",
	"Evaluating Intelligence",
	"Forensic Evaluation",
	"Hiding",
	"Imbuing",
	"Inscription",
	"Item Identification",
	"Meditation",
	"Peacemaking",
	"Poisoning",
	"Provocation",
	"Remove Trap",
	"Spirit Speak",
	"Stealing",
	"Stealth",
	"Taste Identification",
	"Tracking",
	"Left Hand", ///Arm/Disarm group
	"Right Hand",
	"Honor", //Invoke Virture group
	"Sacrifice",
	"Valor",
	"Clumsy", //Cast Spell group
	"Create Food",
	"Feeblemind",
	"Heal",
	"Magic Arrow",
	"Night Sight",
	"Reactive Armor",
	"Weaken",
	"Agility",
	"Cunning",
	"Cure",
	"Harm",
	"Magic Trap",
	"Magic Untrap",
	"Protection",
	"Strength",
	"Bless",
	"Fireball",
	"Magic Lock",
	"Poison",
	"Telekinesis",
	"Teleport",
	"Unlock",
	"Wall Of Stone",
	"Arch Cure",
	"Arch Protection",
	"Curse",
	"Fire Field",
	"Greater Heal",
	"Lightning",
	"Mana Drain",
	"Recall",
	"Blade Spirits",
	"Dispell Field",
	"Incognito",
	"Magic Reflection",
	"Mind Blast",
	"Paralyze",
	"Poison Field",
	"Summon Creature",
	"Dispel",
	"Energy Bolt",
	"Explosion",
	"Invisibility",
	"Mark",
	"Mass Curse",
	"Paralyze Field",
	"Reveal",
	"Chain Lightning",
	"Energy Field",
	"Flame Strike",
	"Gate Travel",
	"Mana Vampire",
	"Mass Dispel",
	"Meteor Swarm",
	"Polymorph",
	"Earthquake",
	"Energy Vortex",
	"Resurrection",
	"Air Elemental",
	"Summon Daemon",
	"Earth Elemental",
	"Fire Elemental",
	"Water Elemental",
	"Animate Dead",
	"Blood Oath",
	"Corpse Skin",
	"Curse Weapon",
	"Evil Omen",
	"Horrific Beast",
	"Lich Form",
	"Mind Rot",
	"Pain Spike",
	"Poison Strike",
	"Strangle",
	"Summon Familar",
	"Vampiric Embrace",
	"Vengeful Spirit",
	"Wither",
	"Wraith Form",
	"Exorcism",
	"Cleance By Fire",
	"Close Wounds",
	"Concentrate Weapon",
	"Dispel Evil",
	"Divine Fury",
	"Enemy Of One",
	"Holy Light",
	"Noble Sacrifice",
	"Remove Curse",
	"Sacred Journey",
	"Honorable Execution",
	"Confidence",
	"Evasion",
	"Counter Attack",
	"Lightning Strike",
	"Momentum Strike",
	"Focus Attack",
	"Death Strike",
	"Animal Form",
	"Ki Attack",
	"Surprice Attack",
	"Backstab",
	"Shadowjump",
	"Mirror Image",
	"Arcane Circle",
	"Gift Of Reneval",
	"Immolating Weapon",
	"Attunement",
	"Thunderstorm",
	"Natures Fury",
	"Summon Fey",
	"Summon Fiend",
	"Reaper Form",
	"Wildfire",
	"Essence Of Wind",
	"Dryad Allure",
	"Ethereal Voyage",
	"Word Of Death",
	"Gift Of Life",
	"Arcane Empowermen",
	"Nether Bolt",
	"Healing Stone",
	"Purge Magic",
	"Enchant",
	"Sleep",
	"Eagle Strike",
	"Animated Weapon",
	"Stone Form",
	"Spell Ttrigger",
	"Mass Sleep",
	"Cleansing Winds",
	"Bombard",
	"Spell Plague",
	"Hail Storm",
	"Nether Cyclone",
	"Rising Colossus",
	"Inspire",
	"Invirorate",
	"Resilience",
	"Perseverance",
	"Tribulation",
	"Despair",
	"Hostle", //Select Next/Preveous/Nearest group
	"Party",
	"Follower",
	"Object",
	"Mobile"
};
//---------------------------------------------------------------------------