/****************************************************************************
**
** FileManager.h
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
#ifndef FileManagerH
#define FileManagerH
//---------------------------------------------------------------------------
class TMappedHeader
{
private:
public:
	TMappedHeader();
	~TMappedHeader() {}

	HANDLE File;
	DWORD Size;
	HANDLE Map;
	PVOID Address;

	PBYTE Ptr;

	inline void Move(int offset) {Ptr += offset;}

	BYTE ReadByte();
	WORD ReadWord();
	DWORD ReadDWord();
	char ReadChar();
	short ReadShort();
	int ReadInt();
	string ReadString(int size);
};
//---------------------------------------------------------------------------
class TFileManager
{
private:
	bool m_UseVerdata;
	int m_UnicodeFontCount;

public:
	TFileManager() :m_UseVerdata(true), m_UnicodeFontCount(0) {}
	~TFileManager() {}
	
	TMappedHeader AnimIdx[6];
	TMappedHeader ArtIdx;
	TMappedHeader GumpIdx;
	TMappedHeader LightIdx;
	TMappedHeader MultiIdx;
	TMappedHeader SkillsIdx;
	TMappedHeader SoundIdx;
	TMappedHeader StaticIdx[6];
	TMappedHeader TextureIdx;
	
	TMappedHeader AnimMul[6];
	TMappedHeader AnimdataMul;
	TMappedHeader ArtMul;
	TMappedHeader HuesMul;
	TMappedHeader FontsMul;
	TMappedHeader GumpMul;
	TMappedHeader LightMul;
	TMappedHeader MapMul[6];
	TMappedHeader MultiMul;
	TMappedHeader PaletteMul;
	TMappedHeader RadarcolMul;
	TMappedHeader SkillsMul;
	TMappedHeader SoundMul;
	TMappedHeader StaticMul[6];
	TMappedHeader TextureMul;
	TMappedHeader TiledataMul;
	TMappedHeader UnifontMul[20];
	TMappedHeader VerdataMul;
	
	TMappedHeader MultiMap;

	SETGET(bool, UseVerdata);
	SETGET(int, UnicodeFontCount);

	bool Load();
	void Unload();

	bool LoadFileToMemory(TMappedHeader &object, const char *fName);
	void UnloadFileFromMemory(TMappedHeader &object);
};
//---------------------------------------------------------------------------
class TFileWriter
{
private:
	FILE *m_File;
	string m_FilePath;
	bool m_UseBuffer;
	bool m_Ready;

	int m_BufferCount;
	BYTE m_Buffer[MAX_FILE_BUFFER_SIZE];
public:
	TFileWriter(string path, bool useBuffer);
	~TFileWriter();
	
	PBYTE Ptr;

	SETGET(string, FilePath);
	SETGET(bool, Ready);
	SETGET(bool, UseBuffer);
	SETGET(int, BufferCount);

	void Move(int offset);
	
	void WriteByte(BYTE val);
	void WriteChar(char val);
	void WriteWord(WORD val);
	void WriteShort(short val);
	void WriteDWord(DWORD val);
	void WriteInt(int val);
	void WriteAsciiString(string val);
	void WriteUnicodeString(wstring val);

	void WriteBuffer();
};
//---------------------------------------------------------------------------
extern TFileManager FileManager;
//---------------------------------------------------------------------------
#endif