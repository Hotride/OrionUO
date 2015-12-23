/****************************************************************************
**
** FileManager.cpp
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

TFileManager FileManager;
//---------------------------------------------------------------------------
//----------------------------------TFileManager-----------------------------
//---------------------------------------------------------------------------
TMappedHeader::TMappedHeader()
: File(0), Size(0), Map(0), Address(NULL), Ptr(NULL)
{
}
//---------------------------------------------------------------------------
BYTE TMappedHeader::ReadByte()
{
	BYTE result = *Ptr;

	Ptr++;

	return result;
}
//---------------------------------------------------------------------------
WORD TMappedHeader::ReadWord()
{
	WORD result = *(PWORD)Ptr;

	Ptr += 2;

	return result;
}
//---------------------------------------------------------------------------
DWORD TMappedHeader::ReadDWord()
{
	DWORD result = *(PDWORD)Ptr;

	Ptr += 4;

	return result;
}
//---------------------------------------------------------------------------
char TMappedHeader::ReadChar()
{
	char result = (char)*Ptr;

	Ptr++;

	return result;
}
//---------------------------------------------------------------------------
short TMappedHeader::ReadShort()
{
	short result = (short)*(PWORD)Ptr;

	Ptr += 2;

	return result;
}
//---------------------------------------------------------------------------
int TMappedHeader::ReadInt()
{
	int result = (int)*(PDWORD)Ptr;

	Ptr += 4;

	return result;
}
//---------------------------------------------------------------------------
string TMappedHeader:: ReadString(int size)
{
	if (!size)
	{
		PBYTE buf = Ptr;

		while (*buf)
			buf++;

		size = (buf - Ptr) + 1;
	}

	char *str = new char[size + 1];
	memcpy(&str[0], &Ptr[0], size);
	str[size] = 0;

	Ptr += size;

	string result(str);
	delete str;

	return result;
}
//---------------------------------------------------------------------------
//----------------------------------TFileManager-----------------------------
//---------------------------------------------------------------------------
bool TFileManager::Load()
{
	memset(AnimIdx, 0, sizeof(AnimIdx));
	memset(&ArtIdx, 0, sizeof(TMappedHeader));
	memset(&GumpIdx, 0, sizeof(TMappedHeader));
	memset(&LightIdx, 0, sizeof(TMappedHeader));
	memset(&MultiIdx, 0, sizeof(TMappedHeader));
	memset(&SkillsIdx, 0, sizeof(TMappedHeader));
	memset(&SoundIdx, 0, sizeof(TMappedHeader));
	memset(StaticIdx, 0, sizeof(StaticIdx));
	memset(&TextureIdx, 0, sizeof(TMappedHeader));
	
	memset(AnimMul, 0, sizeof(AnimMul));
	memset(&AnimdataMul, 0, sizeof(TMappedHeader));
	memset(&ArtMul, 0, sizeof(TMappedHeader));
	memset(&HuesMul, 0, sizeof(TMappedHeader));
	memset(&FontsMul, 0, sizeof(TMappedHeader));
	memset(&GumpMul, 0, sizeof(TMappedHeader));
	memset(&LightMul, 0, sizeof(TMappedHeader));
	memset(MapMul, 0, sizeof(MapMul));
	memset(&MultiMul, 0, sizeof(TMappedHeader));
	memset(&PaletteMul, 0, sizeof(TMappedHeader));
	memset(&RadarcolMul, 0, sizeof(TMappedHeader));
	memset(&SkillsMul, 0, sizeof(TMappedHeader));
	memset(&SoundMul, 0, sizeof(TMappedHeader));
	memset(StaticMul, 0, sizeof(StaticMul));
	memset(&TextureMul, 0, sizeof(TMappedHeader));
	memset(&TiledataMul, 0, sizeof(TMappedHeader));
	memset(UnifontMul, 0, sizeof(UnifontMul));
	memset(&VerdataMul, 0, sizeof(TMappedHeader));
	
	memset(&MultiMap, 0, sizeof(TMappedHeader));

	if (!LoadFileToMemory(AnimIdx[0], FilePath("anim.idx").c_str()))
		return false;

	IFOR(i, 1, 6)
	{
		char buf[50] = {0};
		sprintf(buf, "anim%d.idx", i);
		LoadFileToMemory(AnimIdx[i], FilePath(buf).c_str());
	}

	if (!LoadFileToMemory(ArtIdx, FilePath("artidx.mul").c_str()))
		return false;
	if (!LoadFileToMemory(GumpIdx, FilePath("gumpidx.mul").c_str()))
		return false;
	if (!LoadFileToMemory(LightIdx, FilePath("lightidx.mul").c_str()))
		return false;
	if (!LoadFileToMemory(MultiIdx, FilePath("multi.idx").c_str()))
		return false;
	if (!LoadFileToMemory(SkillsIdx, FilePath("skills.idx").c_str()))
		return false;
	if (!LoadFileToMemory(SoundIdx, FilePath("soundidx.mul").c_str()))
		return false;

	if (!LoadFileToMemory(MultiMap, FilePath("multimap.rle").c_str()))
		return false;

	IFOR(i, 0, 6)
	{
		char buf[50] = {0};
		sprintf(buf, "staidx%d.mul", i);
		LoadFileToMemory(StaticIdx[i], FilePath(buf).c_str());
	}

	if (!LoadFileToMemory(TextureIdx, FilePath("texidx.mul").c_str()))
		return false;



	if (!LoadFileToMemory(AnimMul[0], FilePath("anim.mul").c_str()))
		return false;

	IFOR(i, 1, 6)
	{
		char buf[50] = {0};
		sprintf(buf, "anim%d.mul", i);
		LoadFileToMemory(AnimMul[i], FilePath(buf).c_str());
	}

	if (!LoadFileToMemory(AnimdataMul, FilePath("animdata.mul").c_str()))
		return false;
	if (!LoadFileToMemory(ArtMul, FilePath("art.mul").c_str()))
		return false;
	if (!LoadFileToMemory(HuesMul, FilePath("hues.mul").c_str()))
		return false;
	if (!LoadFileToMemory(FontsMul, FilePath("fonts.mul").c_str()))
		return false;
	if (!LoadFileToMemory(GumpMul, FilePath("gumpart.mul").c_str()))
		return false;
	if (!LoadFileToMemory(LightMul, FilePath("light.mul").c_str()))
		return false;

	IFOR(i, 0, 6)
	{
		char buf[50] = {0};
		sprintf(buf, "map%d.mul", i);
		LoadFileToMemory(MapMul[i], FilePath(buf).c_str());
	}

	if (!LoadFileToMemory(MultiMul, FilePath("multi.mul").c_str()))
		return false;
	if (!LoadFileToMemory(PaletteMul, FilePath("palette.mul").c_str()))
		return false;
	if (!LoadFileToMemory(RadarcolMul, FilePath("radarcol.mul").c_str()))
		return false;
	if (!LoadFileToMemory(SkillsMul, FilePath("skills.mul").c_str()))
		return false;
	if (!LoadFileToMemory(SoundMul, FilePath("sound.mul").c_str()))
		return false;

	IFOR(i, 0, 6)
	{
		char buf[50] = {0};
		sprintf(buf, "statics%d.mul", i);
		LoadFileToMemory(StaticMul[i], FilePath(buf).c_str());
	}

	if (!LoadFileToMemory(TextureMul, FilePath("texmaps.mul").c_str()))
		return false;
	if (!LoadFileToMemory(TiledataMul, FilePath("tiledata.mul").c_str()))
		return false;

	IFOR(i, 0, 20)
	{
		char buf[50] = {0};
		if (i)
			sprintf(buf, "unifont%d.mul", i);
		else
			sprintf(buf, "unifont.mul");

		if (!LoadFileToMemory(UnifontMul[i], FilePath(buf).c_str()))
			break;

		m_UnicodeFontCount++;
	}

	if (m_UseVerdata && !LoadFileToMemory(VerdataMul, FilePath("verdata.mul").c_str()))
		m_UseVerdata = false;

	return true;
}
//---------------------------------------------------------------------------
void TFileManager::Unload()
{
	IFOR(i, 0, 6)
		UnloadFileFromMemory(AnimIdx[i]);
	UnloadFileFromMemory(ArtIdx);
	UnloadFileFromMemory(GumpIdx);
	UnloadFileFromMemory(LightIdx);
	UnloadFileFromMemory(MultiIdx);
	UnloadFileFromMemory(SkillsIdx);
	UnloadFileFromMemory(SoundIdx);
	IFOR(i, 0, 6)
		UnloadFileFromMemory(StaticIdx[i]);
	UnloadFileFromMemory(TextureIdx);



	IFOR(i, 0, 6)
		UnloadFileFromMemory(AnimMul[i]);
	UnloadFileFromMemory(AnimdataMul);
	UnloadFileFromMemory(ArtMul);
	UnloadFileFromMemory(HuesMul);
	UnloadFileFromMemory(FontsMul);
	UnloadFileFromMemory(GumpMul);
	UnloadFileFromMemory(LightMul);
	IFOR(i, 0, 6)
		UnloadFileFromMemory(MapMul[i]);
	UnloadFileFromMemory(MultiMul);
	UnloadFileFromMemory(PaletteMul);
	UnloadFileFromMemory(RadarcolMul);
	UnloadFileFromMemory(SkillsMul);
	UnloadFileFromMemory(SoundMul);
	IFOR(i, 0, 6)
		UnloadFileFromMemory(StaticMul[i]);
	UnloadFileFromMemory(TextureMul);
	UnloadFileFromMemory(TiledataMul);
	IFOR(i, 0, UnicodeFontCount)
		UnloadFileFromMemory(UnifontMul[i]);
	UnloadFileFromMemory(VerdataMul);
}
//---------------------------------------------------------------------------
bool TFileManager::LoadFileToMemory(TMappedHeader &object, const char *fName)
{
	FILE *fl = fopen(fName, "rb");

	if (fl == NULL)
	{
		g_LastLoadFileError = fName;

		return false;
	}

	fclose(fl);

	UnloadFileFromMemory(object);

	object.File = CreateFileA(fName, GENERIC_READ, 1, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (object.File == INVALID_HANDLE_VALUE)
	{
		g_LastLoadFileError = fName;

		return false;
	}

	object.Size = GetFileSize(object.File, NULL);

	object.Map = CreateFileMapping(object.File, NULL, 2, 0, NULL, NULL);

	if (object.Map == NULL)
	{
		CloseHandle(object.File);
		g_LastLoadFileError = fName;

		return false;
	}

	object.Address = MapViewOfFile(object.Map, FILE_MAP_READ, 0, 0, object.Size);

	if (object.Address == NULL)
	{
		CloseHandle(object.Map);
		CloseHandle(object.File);
		g_LastLoadFileError = fName;

		return false;
	}

	object.Ptr = (PBYTE)object.Address;

	return true;
}
//---------------------------------------------------------------------------
void TFileManager::UnloadFileFromMemory(TMappedHeader &object)
{
	if (object.Address != NULL)
		UnmapViewOfFile(object.Address);

	if (object.Map != NULL)
		CloseHandle(object.Map);

	if (object.File != INVALID_HANDLE_VALUE)
		CloseHandle(object.File);

	memset(&object, 0, sizeof(object));
}
//---------------------------------------------------------------------------
IMappedHeader *TFileManager::GetMappedFile(int index)
{
	IMappedHeader *result = 0;

	/*switch (index)
	{
		//index files
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			result = &AnimIdx[index - 1];
			break;
		case 7:
			result = &ArtIdx;
			break;
		case 8:
			result = &GumpIdx;
			break;
		case 9:
			result = &LightIdx;
			break;
		case 10:
			result = &MultiIdx;
			break;
		case 11:
			result = &SkillsIdx;
			break;
		case 12:
			result = &SoundIdx;
			break;
		case 13:
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
			result = &StaticIdx[index - 13];
			break;
		case 19:
			result = &TextureIdx;
			break;

		//data files
		case 20:
		case 21:
		case 22:
		case 23:
		case 24:
		case 25:
			result = &AnimMul[index - 20];
			break;
		case 26:
			result = &AnimdataMul;
			break;
		case 27:
			result = &ArtMul;
			break;
		case 28:
			result = &HuesMul;
			break;
		case 29:
			result = &FontsMul;
			break;
		case 30:
			result = &GumpMul;
			break;
		case 31:
			result = &LightMul;
			break;
		case 32:
		case 33:
		case 34:
		case 35:
		case 36:
		case 37:
			result = &MapMul[index - 32];
			break;
		case 38:
			result = &MultiMul;
			break;
		case 39:
			result = &PaletteMul;
			break;
		case 40:
			result = &RadarcolMul;
			break;
		case 41:
			result = &SkillsMul;
			break;
		case 42:
			result = &SoundMul;
			break;
		case 43:
		case 44:
		case 45:
		case 46:
		case 47:
		case 48:
			result = &StaticMul[index - 43];
			break;
		case 49:
			result = &TextureMul;
			break;
		case 50:
			result = &TiledataMul;
			break;
		case 51:
			result = &VerdataMul;
			break;
		case 52:
			result = &MultiMap;
			break;
	}

	if (result == NULL && index >= 53 && index < 73)
		result = &UnifontMul[index - 53];*/

	return result;
}
//---------------------------------------------------------------------------
//--------------------------------TFileWriter-------------------------------
//---------------------------------------------------------------------------
TFileWriter::TFileWriter(string path, bool useBuffer)
: m_FilePath(path), m_UseBuffer(useBuffer), m_BufferCount(0)
{
	memset(&m_Buffer[0], 0, MAX_FILE_BUFFER_SIZE);
	Ptr = m_Buffer;

	m_File = fopen(path.c_str(), "wb");
	m_Ready = (m_File != NULL);
}
//---------------------------------------------------------------------------
TFileWriter::~TFileWriter()
{
	if (m_File != NULL)
	{
		if (m_UseBuffer && m_BufferCount > 0)
		{
			fwrite(&m_Buffer[0], m_BufferCount, 1, m_File);
			m_BufferCount = 0;
		}

		fclose(m_File);
		m_File = NULL;
	}
}
//---------------------------------------------------------------------------
void TFileWriter::Move(int offset)
{
	if (m_File != NULL)
	{
		if (m_UseBuffer)
		{
			if (m_BufferCount + offset >= MAX_FILE_BUFFER_SIZE)
			{
				fwrite(&m_Buffer[0], m_BufferCount, 1, m_File);
				m_BufferCount = 0;
				Ptr = m_Buffer;
			}

			Ptr += offset;
			m_BufferCount += offset;
		}
		else
		{
			fwrite(NULL, offset, 1, m_File);
		}
	}
}
//----------------------------------------------------------------------------
//Get ordinary class property customized
#define FILE_WRITE_BODY(type, name) \
void TFileWriter::Write##name(type val) \
{ \
	if (m_File != NULL) \
	{ \
		int size = sizeof(type); \
		if (m_UseBuffer) \
		{ \
			if (m_BufferCount + size >= MAX_FILE_BUFFER_SIZE) \
			{ \
				fwrite(&m_Buffer[0], m_BufferCount, 1, m_File); \
				m_BufferCount = 0; \
				Ptr = m_Buffer; \
			} \
			type *obj = (type*)Ptr; \
			*obj = val; \
			Ptr += size; \
			m_BufferCount += size; \
		} \
		else \
			fwrite(&val, size, 1, m_File); \
	} \
}
//---------------------------------------------------------------------------
FILE_WRITE_BODY(BYTE, Byte)
FILE_WRITE_BODY(char, Char)
FILE_WRITE_BODY(WORD, Word)
FILE_WRITE_BODY(short, Short)
FILE_WRITE_BODY(DWORD, DWord)
FILE_WRITE_BODY(int, Int)
//---------------------------------------------------------------------------
void TFileWriter::WriteAsciiString(string val)
{
	if (m_File != NULL)
	{
		int size = val.length();
		const char *data = val.c_str();

		if (m_UseBuffer)
		{
			if (m_BufferCount + size + 1 >= MAX_FILE_BUFFER_SIZE)
			{
				fwrite(&m_Buffer[0], m_BufferCount, 1, m_File);
				m_BufferCount = 0;
				Ptr = m_Buffer;
			}

			memcpy(&Ptr[0], &data[0], size);
			Ptr += size;
			*Ptr++ = 0;
			m_BufferCount += (size + 1);
		}
		else
		{
			fwrite(&data[0], size, 1, m_File);
			BYTE nullTerm = 0;
			fwrite(&nullTerm, 1, 1, m_File);
		}
	}
}
//---------------------------------------------------------------------------
void TFileWriter::WriteUnicodeString(wstring val)
{
	if (m_File != NULL)
	{
		int size = val.length() * 2;
		const wchar_t *data = val.c_str();

		if (m_UseBuffer)
		{
			if (m_BufferCount + size + 2 >= MAX_FILE_BUFFER_SIZE)
			{
				fwrite(&m_Buffer[0], m_BufferCount, 1, m_File);
				m_BufferCount = 0;
				Ptr = m_Buffer;
			}

			memcpy(&Ptr[0], &data[0], size);
			Ptr += size;
			*Ptr++ = 0;
			*Ptr++ = 0;
			m_BufferCount += (size + 2);
		}
		else
		{
			fwrite(&data[0], size, 1, m_File);
			WORD nullTerm = 0;
			fwrite(&nullTerm, 2, 1, m_File);
		}
	}
}
//---------------------------------------------------------------------------
void TFileWriter::WriteBuffer()
{
	if (m_File != NULL && m_UseBuffer)
	{
		if (m_BufferCount > 0)
		{
			fwrite(&m_Buffer[0], m_BufferCount, 1, m_File);
			m_BufferCount = 0;
			Ptr = m_Buffer;
		}
	}
}
//---------------------------------------------------------------------------