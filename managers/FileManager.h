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
//!Класс для хранения данных и работе с файлами, спроецированными в память
class TMappedHeader
{
public:
	TMappedHeader();
	~TMappedHeader() {}

	//!Хэндл файла
	HANDLE File;

	//!Размер файла
	DWORD Size;

	//!Ссылка на карту
	HANDLE Map;

	//!Адрес файла в памяти
	PVOID Address;

	//!Указатель на конец файла
	PBYTE End;

	//!Указатель на текущую позицию
	PBYTE Ptr;

	inline bool IsEof() { return (Ptr >= End); }

	/*!
	Переместить указатель
	@param [__in] offset Смещение указателя
	@return 
	*/
	inline void Move(__in int offset) { Ptr += offset; }

	/*!
	Прочитать байт (1 байт) little-endian
	@return 
	*/
	BYTE ReadByte();

	/*!
	Прочитать байт (1 байт) big-endian
	@return
	*/
	BYTE ReadByteBE();

	/*!
	Прочитать слово (2 байта) little-endian
	@return 
	*/
	WORD ReadWord();

	/*!
	Прочитать слово (2 байта) big-endian
	@return
	*/
	WORD ReadWordBE();

	/*!
	Прочитать двойное слово (4 байла) little-endian
	@return 
	*/
	DWORD ReadDWord();

	/*!
	Прочитать двойное слово (4 байла) big-endian
	@return
	*/
	DWORD ReadDWordBE();

	/*!
	Прочитать символ (1 байт) little-endian
	@return 
	*/
	char ReadChar();

	/*!
	Прочитать символ (1 байт) big-endian
	@return
	*/
	char ReadCharBE();

	/*!
	Прочитать короткое значение (2 байта) little-endian
	@return 
	*/
	short ReadShort();

	/*!
	Прочитать короткое значение (2 байта) big-endian
	@return
	*/
	short ReadShortBE();

	/*!
	Прочитать целое (4 байта) little-endian
	@return 
	*/
	int ReadInt();

	/*!
	Прочитать целое (4 байта) big-endian
	@return
	*/
	int ReadIntBE();

	/*!
	Прочитать строку
	@param [__in] size Размер строки, если 0 - читает до нуля
	@return 
	*/
	string ReadString(__in int size);

	/*!
	Прочитать строку utf-8
	@param [__in] size Размер строки
	@return
	*/
	wstring ReadUtf8String(__in int size);
};
//---------------------------------------------------------------------------
//!Класс для работы с файлами
class TFileManager
{
private:
	//!Использовать ли файл патчей (verdata.mul)
	bool m_UseVerdata;

	//!Количество загруженных файлов с Unicode-шрифтами
	int m_UnicodeFontCount;

public:
	TFileManager() :m_UseVerdata(true), m_UnicodeFontCount(0) {}
	~TFileManager() {}
	
	//!Адреса файлов в памяти
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
	TMappedHeader SpeechMul;
	TMappedHeader LangcodeIff;

	SETGET(bool, UseVerdata);
	SETGET(int, UnicodeFontCount);

	/*!
	Загрузить файлы
	@return 
	*/
	bool Load();

	/*!
	Выгрузить файлы
	@return 
	*/
	void Unload();

	/*!
	Спроецировать файл в память
	@param [__inout] object Объект для работы с файлом
	@param [__in] fName Путь к файлу
	@return 
	*/
	bool LoadFileToMemory(__inout TMappedHeader &object, __in const char *fName);

	/*!
	Выгрузить файл из памяти
	@param [__inout] object Объект для работы с файлом
	@return 
	*/
	void UnloadFileFromMemory(__inout TMappedHeader &object);

};
//---------------------------------------------------------------------------
//!Класс для записи бинарных файлоа
class TFileWriter
{
private:
	//!Хэндл файла
	FILE *m_File;

	//!Путь к файлу
	string m_FilePath;

	//!Использовать ли внутренний буфер для записи
	bool m_UseBuffer;

	//!Открыт ли файл
	bool m_Ready;

	//!Количество данных в буфере
	int m_BufferCount;

	//!Буфер
	BYTE m_Buffer[MAX_FILE_BUFFER_SIZE];

public:
	TFileWriter(string path, bool useBuffer);
	~TFileWriter();
	
	//Указатель на данные во внутреннем буфере
	PBYTE Ptr;

	SETGET(string, FilePath);
	SETGET(bool, Ready);
	SETGET(bool, UseBuffer);
	SETGET(int, BufferCount);

	/*!
	Переместить указатель внутреннего буфера
	@param [__in] offset Смещение
	@return 
	*/
	void Move(__in int offset);

	/*!
	Записать байт
	@param [__in] val Данные
	@return 
	*/
	void WriteByte(__in BYTE val);

	/*!
	Записать символ
	@param [__in] val Данные
	@return
	*/
	void WriteChar(char val);

	/*!
	Записать слово
	@param [__in] val Данные
	@return
	*/
	void WriteWord(WORD val);

	/*!
	Записать короткое значение
	@param [__in] val Данные
	@return
	*/
	void WriteShort(short val);

	/*!
	Записать двойное слово
	@param [__in] val Данные
	@return
	*/
	void WriteDWord(DWORD val);

	/*!
	Записать целое
	@param [__in] val Данные
	@return
	*/
	void WriteInt(int val);

	/*!
	Записать ASCII строку
	@param [__in] val Данные
	@return 
	*/
	void WriteAsciiString(__in string val);

	/*!
	Записать Unicode строку
	@param [__in] val Данные
	@return 
	*/
	void WriteUnicodeString(__in wstring val);

	/*!
	Записать буфер в файл
	@return 
	*/
	void WriteBuffer();
};
//---------------------------------------------------------------------------
//!Менеджер файлов
extern TFileManager FileManager;
//---------------------------------------------------------------------------
#endif