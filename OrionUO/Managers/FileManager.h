/***********************************************************************************
**
** FileManager.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef FILEMANAGER_H
#define FILEMANAGER_H
//----------------------------------------------------------------------------------
class CUopBlockHeader
{
	SETGET(uint64, Offset, 0);
	//SETGET(uint, HeaderSize, 0);
	SETGET(uint, CompressedSize, 0);
	SETGET(uint, DecompressedSize, 0);
	//SETGET(uint64, Hash, 0);
	//SETGET(uint, Unknown, 0);
	//SETGET(ushort, Flags, 0);

public:
	CUopBlockHeader() {}
	CUopBlockHeader(const uint64 &offset, const uint &headerSize, const uint &compresseSize, const uint &decompressedSize, const uint64 &Hash, const uint &unknown, const ushort &flags)
	: m_Offset(offset), m_CompressedSize(compresseSize), m_DecompressedSize(decompressedSize) {}
	~CUopBlockHeader() {}
};
//----------------------------------------------------------------------------------
class CUopMappedFile : public WISP_FILE::CMappedFile
{
private:
public:
	std::unordered_map<uint64, CUopBlockHeader> m_Map;

public:
	CUopMappedFile();
	virtual ~CUopMappedFile();

	void Add(const uint64 &hash, const CUopBlockHeader &item);

	CUopBlockHeader *GetBlock(const uint64 &hash);

	UCHAR_LIST GetData(const CUopBlockHeader &block);
};
//----------------------------------------------------------------------------------
class CFileManager : public WISP_DATASTREAM::CDataReader
{
	SETGET(bool, UseVerdata, false);
	SETGET(bool, UseUOPGumps, false);
	SETGET(int, UnicodeFontsCount, 0);

public:
	CFileManager();
	virtual ~CFileManager();

	AutoResetEvent m_AutoResetEvent;

	//!Адреса файлов в памяти
	WISP_FILE::CMappedFile m_AnimIdx[6];
	WISP_FILE::CMappedFile m_ArtIdx;
	WISP_FILE::CMappedFile m_GumpIdx;
	WISP_FILE::CMappedFile m_LightIdx;
	WISP_FILE::CMappedFile m_MultiIdx;
	WISP_FILE::CMappedFile m_SkillsIdx;
	WISP_FILE::CMappedFile m_SoundIdx;
	WISP_FILE::CMappedFile m_StaticIdx[6];
	WISP_FILE::CMappedFile m_TextureIdx;

	WISP_FILE::CMappedFile m_AnimMul[6];
	WISP_FILE::CMappedFile m_AnimdataMul;
	WISP_FILE::CMappedFile m_ArtMul;
	WISP_FILE::CMappedFile m_HuesMul;
	WISP_FILE::CMappedFile m_FontsMul;
	WISP_FILE::CMappedFile m_GumpMul;
	WISP_FILE::CMappedFile m_LightMul;
	WISP_FILE::CMappedFile m_MapMul[6];
	WISP_FILE::CMappedFile m_MultiMul;
	WISP_FILE::CMappedFile m_RadarcolMul;
	WISP_FILE::CMappedFile m_SkillsMul;
	WISP_FILE::CMappedFile m_SoundMul;
	WISP_FILE::CMappedFile m_StaticMul[6];
	WISP_FILE::CMappedFile m_TextureMul;
	WISP_FILE::CMappedFile m_TiledataMul;
	WISP_FILE::CMappedFile m_UnifontMul[20];
	WISP_FILE::CMappedFile m_VerdataMul;
	WISP_FILE::CMappedFile m_FacetMul[6];

	WISP_FILE::CMappedFile m_MultiMap;
	WISP_FILE::CMappedFile m_SpeechMul;
	WISP_FILE::CMappedFile m_LangcodeIff;

	//UOP
	CUopMappedFile m_ArtLegacyMUL;
	CUopMappedFile m_GumpartLegacyMUL;
	CUopMappedFile m_SoundLegacyMUL;
	CUopMappedFile m_Tileart;
	CUopMappedFile m_MainMisc;
	CUopMappedFile m_MapUOP[6];
	CUopMappedFile m_MapXUOP[6];
	CUopMappedFile m_AnimationSequence;
	CUopMappedFile m_MultiCollection;

	//Map patches
	WISP_FILE::CMappedFile m_MapDifl[6];
	WISP_FILE::CMappedFile m_MapDif[6];

	WISP_FILE::CMappedFile m_StaDifl[6];
	WISP_FILE::CMappedFile m_StaDifi[6];
	WISP_FILE::CMappedFile m_StaDif[6];

	bool Load();
	bool LoadWithUOP();
	void Unload();
	void TryReadUOPAnimations();

	//Чтение сжатых данных с кадрами из УОП аним файла.
	static char *ReadUOPDataFromFileStream(UOPAnimationData &animData);

	//Разжатие данных с кадрами из УОП аним файла.
	static bool DecompressUOPFileData(UOPAnimationData &animData, UCHAR_LIST &decLayoutData, char *buf);

private:
	void ReadTask();
	static bool FileExists(const std::string& filename);

	bool LoadUOPFile(CUopMappedFile &file, const char *fileName);
};
//---------------------------------------------------------------------------
extern CFileManager g_FileManager;
//---------------------------------------------------------------------------
#endif