// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** Macro.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CMacroObject *g_MacroPointer = NULL;
//----------------------------------------------------------------------------------
//-----------------------------------CMacroObject-----------------------------------
//----------------------------------------------------------------------------------
CMacroObject::CMacroObject(const MACRO_CODE &code, const MACRO_SUB_CODE &subCode)
: CBaseQueueItem(), m_Code(code), m_SubCode(subCode)
{
	WISPFUN_DEBUG("c190_f1");
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
				CMacro::GetBoundByCode(code, count, offset);
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
//----------------------------------------------------------------------------------
CMacroObject::~CMacroObject()
{
}
//----------------------------------------------------------------------------------
//----------------------------------CMacroObjectString------------------------------
//----------------------------------------------------------------------------------
CMacroObjectString::CMacroObjectString(const MACRO_CODE &code, const MACRO_SUB_CODE &subCode, const string &str)
: CMacroObject(code, subCode), m_String(str)
{
}
//----------------------------------------------------------------------------------
CMacroObjectString::~CMacroObjectString()
{
}
//----------------------------------------------------------------------------------
//----------------------------------------CMacro------------------------------------
//----------------------------------------------------------------------------------
CMacro::CMacro(const ushort &key, const bool &alt, const bool &ctrl, const bool &shift)
: CBaseQueueItem(), m_Key(key), m_Alt(alt), m_Ctrl(ctrl), m_Shift(shift)
{
}
//----------------------------------------------------------------------------------
CMacro::~CMacro()
{
}
//----------------------------------------------------------------------------------
CMacro *CMacro::CreateBlankMacro()
{
	WISPFUN_DEBUG("c191_f1");
	CMacro *obj = new CMacro(0, false, false, false);

	obj->Add(new CMacroObject(MC_NONE, MSC_NONE));

	return obj;
}
//----------------------------------------------------------------------------------
CMacroObject *CMacro::CreateMacro(const MACRO_CODE &code)
{
	WISPFUN_DEBUG("c191_f2");
	CMacroObject *obj = NULL;
	
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
			obj = new CMacroObjectString(code, MSC_NONE, "");
					
			break;
		}
		default:
		{
			obj = new CMacroObject(code, MSC_NONE);

			break;
		}
	}

	return obj;
}
//----------------------------------------------------------------------------------
void CMacro::ChangeObject(CMacroObject *source, CMacroObject *obj)
{
	WISPFUN_DEBUG("c191_f3");
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
//----------------------------------------------------------------------------------
CMacro *CMacro::Load(WISP_FILE::CMappedFile &file)
{
	WISPFUN_DEBUG("c191_f4");
	puchar next = file.Ptr;
	short size = file.ReadInt16LE();
	next += size;

	ushort key = file.ReadUInt16LE();

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

	int count = file.ReadInt16LE();
	
	CMacro *macro = new CMacro(key, alt, ctrl, shift);

	IFOR(i, 0, count)
	{
		BYTE type = file.ReadUInt8();

		MACRO_CODE code = (MACRO_CODE)file.ReadUInt16LE();

		MACRO_SUB_CODE subCode = (MACRO_SUB_CODE)file.ReadUInt16LE();

		CMacroObject *obj = NULL;

		switch (type)
		{
			case 0: //original
			{
				obj = new CMacroObject(code, subCode);
				break;
			}
			case 2: //with string
			{
				short len = file.ReadUInt16LE();

				string str = file.ReadString(len);

				obj = new CMacroObjectString(code, subCode, str);

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
//----------------------------------------------------------------------------------
void CMacro::Save(WISP_FILE::CBinaryFileWritter &writter)
{
	WISPFUN_DEBUG("c191_f5");
	short size = 10;
	short count = 0;

	QFOR(obj, m_Items, CMacroObject*)
	{
		size += 5;
		count++;

		if (obj->HaveString()) //with string
		{
			string str = ((CMacroObjectString*)obj)->String;
			size += (short)str.length() + 3;
		}
	}

	writter.WriteUInt16LE(size);

	ushort key = m_Key;

	if (m_Alt)
		key += MODKEY_ALT;

	if (m_Ctrl)
		key += MODKEY_CTRL;

	if (m_Shift)
		key += MODKEY_SHIFT;

	writter.WriteUInt16LE(key);

	writter.WriteUInt16LE(count);

	QFOR(obj, m_Items, CMacroObject*)
	{
		uchar type = 0;

		if (obj->HaveString())
			type = 2;

		writter.WriteUInt8(type);
		writter.WriteUInt16LE(obj->Code);
		writter.WriteUInt16LE(obj->SubCode);

		if (type == 2) //with string
		{
			string str = ((CMacroObjectString*)obj)->String;
			int len = (int)str.length();

			writter.WriteInt16LE(len + 1);
			writter.WriteString(str);
		}

		writter.WriteBuffer();
	}
	
	writter.WriteUInt32LE(0); //EOM
	writter.WriteBuffer();
}
//----------------------------------------------------------------------------------
CMacro *CMacro::GetCopy()
{
	WISPFUN_DEBUG("c191_f6");
	CMacro *macro = new CMacro(m_Key, m_Alt, m_Ctrl, m_Shift);
	MACRO_CODE oldCode = MC_NONE;

	QFOR(obj, m_Items, CMacroObject*)
	{
		if (obj->HaveString())
			macro->Add(new CMacroObjectString(obj->Code, obj->SubCode, ((CMacroObjectString*)obj)->String));
		else
			macro->Add(new CMacroObject(obj->Code, obj->SubCode));

		oldCode = obj->Code;
	}

	if (oldCode != MC_NONE)
		macro->Add(new CMacroObject(MC_NONE, MSC_NONE));

	return macro;
}
//----------------------------------------------------------------------------------
void CMacro::GetBoundByCode(const MACRO_CODE &code, int &count, int &offset)
{
	WISPFUN_DEBUG("c191_f7");
	switch (code)
	{
		case MC_WALK:
		{
			offset = MSC_G1_NW;
			count = MSC_G2_CONFIGURATION - MSC_G1_NW;
			break;
		}
		case MC_OPEN:
		case MC_CLOSE:
		case MC_MINIMIZE:
		case MC_MAXIMIZE:
		{
			offset = MSC_G2_CONFIGURATION;
			count = MSC_G3_ANATOMY - MSC_G2_CONFIGURATION;
			break;
		}
		case MC_USE_SKILL:
		{
			offset = MSC_G3_ANATOMY;
			count = MSC_G4_LEFT_HAND - MSC_G3_ANATOMY;
			break;
		}
		case MC_ARM_DISARM:
		{
			offset = MSC_G4_LEFT_HAND;
			count = MSC_G5_HONOR - MSC_G4_LEFT_HAND;
			break;
		}
		case MC_INVOKE_VIRTURE:
		{
			offset = MSC_G5_HONOR;
			count = MSC_G6_CLUMSY - MSC_G5_HONOR;
			break;
		}
		case MC_CAST_SPELL:
		{
			offset = MSC_G6_CLUMSY;
			count = MSC_G7_HOSTILE - MSC_G6_CLUMSY;
			break;
		}
		case MC_SELECT_NEXT:
		case MC_SELECT_PREVEOUS:
		case MC_SELECT_NEAREST:
		{
			offset = MSC_G7_HOSTILE;
			count = MSC_TOTAL_COUNT - MSC_G7_HOSTILE;
			break;
		}
		default:
			break;
	}

}
//----------------------------------------------------------------------------------
const char *CMacro::m_MacroActionName[MACRO_ACTION_NAME_COUNT] =
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
//----------------------------------------------------------------------------------
const char *CMacro::m_MacroAction[MACRO_ACTION_COUNT] =
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
	"Cleanse By Fire",
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
	"Arcane Empowerment",
	"Nether Bolt",
	"Healing Stone",
	"Purge Magic",
	"Enchant",
	"Sleep",
	"Eagle Strike",
	"Animated Weapon",
	"Stone Form",
	"Spell Trigger",
	"Mass Sleep",
	"Cleansing Winds",
	"Bombard",
	"Spell Plague",
	"Hail Storm",
	"Nether Cyclone",
	"Rising Colossus",
	"Inspire",
	"Invigorate",
	"Resilience",
	"Perseverance",
	"Tribulation",
	"Despair",
	"Hostile", //Select Next/Previous/Nearest group
	"Party",
	"Follower",
	"Object",
	"Mobile"
};
//----------------------------------------------------------------------------------
