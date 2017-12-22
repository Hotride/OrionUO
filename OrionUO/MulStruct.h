/***********************************************************************************
**
** MulStruct.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef MULSTRUCT_H
#define MULSTRUCT_H
//----------------------------------------------------------------------------------
#pragma pack (push,1)

typedef struct VERDATA_HEADER
{
	unsigned int FileID;
	unsigned int BlockID;
	unsigned int Position;
	unsigned int Size;
	unsigned int GumpData;
} *PVERDATA_HEADER;
//----------------------------------------------------------------------------------
typedef struct BASE_IDX_BLOCK
{
	unsigned int Position;
	unsigned int Size;
} *PBASE_IDX_BLOCK;
//----------------------------------------------------------------------------------
typedef struct MAP_CELLS_EX
{
	unsigned short TileID;
	unsigned short Color;
	char Z;
} *PMAP_CELLS_EX;
//----------------------------------------------------------------------------------
typedef struct MAP_CELLS
{
	unsigned short TileID;
	char Z;
} *PMAP_CELLS;
//----------------------------------------------------------------------------------
typedef struct MAP_BLOCK
{
	unsigned int Header;
	MAP_CELLS Cells[64];
} *PMAP_BLOCK;
//----------------------------------------------------------------------------------
struct RADAR_MAP_CELLS
{
	unsigned short Graphic;
	char Z;
	char IsLand;
};
//----------------------------------------------------------------------------------
struct RADAR_MAP_BLOCK
{
	RADAR_MAP_CELLS Cells[8][8];
};
//----------------------------------------------------------------------------------
typedef struct STAIDX_BLOCK : public BASE_IDX_BLOCK
{
	unsigned int Unknown;
} *PSTAIDX_BLOCK;
//----------------------------------------------------------------------------------
typedef struct STATICS_BLOCK
{
	unsigned short Color;
	unsigned char X;
	unsigned char Y;
	char Z;
	unsigned short Hue;
} *PSTATICS_BLOCK;
//----------------------------------------------------------------------------------
struct LAND_TILES_OLD
{
	unsigned int Flags;
	unsigned short TexID;
	char Name[20];
};
//----------------------------------------------------------------------------------
typedef struct LAND_GROUP_OLD
{
	unsigned int Unknown;
	LAND_TILES_OLD Tiles[32];
} *PLAND_GROUP_OLD;
//----------------------------------------------------------------------------------
struct STATIC_TILES_OLD
{
	unsigned int Flags;
	unsigned char Weight;
	unsigned char Layer;
	unsigned int Count;
	unsigned short AnimID;
	unsigned short Hue;
	unsigned short LightIndex;
	unsigned char Height;
	char Name[20];
};
//----------------------------------------------------------------------------------
typedef struct STATIC_GROUP_OLD
{
	unsigned int Unk;
	STATIC_TILES_OLD Tiles[32];
} *PSTATIC_GROUP_OLD;
//----------------------------------------------------------------------------------
struct LAND_TILES_NEW
{
	unsigned __int64 Flags;
	unsigned short TexID;
	char Name[20];
};
//----------------------------------------------------------------------------------
typedef struct LAND_GROUP_NEW
{
	unsigned int Unknown;
	LAND_TILES_NEW Tiles[32];
} *PLAND_GROUP_NEW;
//----------------------------------------------------------------------------------
struct STATIC_TILES_NEW
{
	unsigned __int64 Flags;
	unsigned char Weight;
	unsigned char Layer;
	unsigned int Count;
	unsigned short AnimID;
	unsigned short Hue;
	unsigned short LightIndex;
	unsigned char Height;
	char Name[20];
};
//----------------------------------------------------------------------------------
typedef struct STATIC_GROUP_NEW
{
	unsigned int Unk;
	STATIC_TILES_NEW Tiles[32];
} *PSTATIC_GROUP_NEW;
//----------------------------------------------------------------------------------
struct LAND_TILES
{
	unsigned __int64 Flags;
	unsigned short TexID;
	string Name;
};
//----------------------------------------------------------------------------------
typedef struct LAND_GROUP
{
	unsigned int Unknown;
	LAND_TILES Tiles[32];
} *PLAND_GROUP;
//----------------------------------------------------------------------------------
struct STATIC_TILES
{
	unsigned __int64 Flags;
	unsigned char Weight;
	unsigned char Layer;
	unsigned int Count;
	unsigned short AnimID;
	unsigned short Hue;
	unsigned short LightIndex;
	unsigned char Height;
	string Name;
};
//----------------------------------------------------------------------------------
typedef struct STATIC_GROUP
{
	unsigned int Unk;
	STATIC_TILES Tiles[32];
} *PSTATIC_GROUP;
//----------------------------------------------------------------------------------
typedef struct MULTI_IDX_BLOCK : public BASE_IDX_BLOCK
{
	unsigned int Unknown;
} *PMULTI_IDX_BLOCK;
//----------------------------------------------------------------------------------
typedef struct MULTI_BLOCK
{
	unsigned short ID;
	short X;
	short Y;
	short Z;
	unsigned int Flags;
} *PMULTI_BLOCK;
//----------------------------------------------------------------------------------
typedef struct MULTI_BLOCK_NEW
{
	unsigned short ID;
	short X;
	short Y;
	short Z;
	unsigned int Flags;
	int Unknown;
} *PMULTI_BLOCK_NEW;
//----------------------------------------------------------------------------------
typedef struct HUES_BLOCK
{
	unsigned short ColorTable[32];
	unsigned short TableStart;
	unsigned short TableEnd;
	char Name[20];
} *PHUES_BLOCK;
//----------------------------------------------------------------------------------
typedef struct HUES_GROUP
{
	unsigned int Header;
	HUES_BLOCK Entries[8];
} *PHUES_GROUP;
//----------------------------------------------------------------------------------
typedef struct VERDATA_HUES_BLOCK
{
	unsigned short ColorTable[32];
	unsigned short TableStart;
	unsigned short TableEnd;
	char Name[20];
	unsigned short Unk[32];
} *PVERDATA_HUES_BLOCK;
//----------------------------------------------------------------------------------
typedef struct VERDATA_HUES_GROUP
{
	unsigned int Header;
	VERDATA_HUES_BLOCK Entries[8];
} *PVERDATA_HUES_GROUP;
//----------------------------------------------------------------------------------
typedef struct GUMP_IDX_BLOCK : public BASE_IDX_BLOCK
{
	unsigned short Height;
	unsigned short Width;
} *PGUMP_IDX_BLOCK;
//----------------------------------------------------------------------------------
typedef struct GUMP_BLOCK
{
	unsigned short Value;
	unsigned short Run;
} *PGUMP_BLOCK;
//----------------------------------------------------------------------------------
typedef struct SKILLS_IDX_BLOCK : public BASE_IDX_BLOCK
{
	unsigned int Unknown;
} *PSKILLS_IDX_BLOCK;
//----------------------------------------------------------------------------------
typedef struct SKILLS_BLOCK
{
	unsigned char Button;
	char *SkillName;
} *PSKILLS_BLOCK;
//----------------------------------------------------------------------------------
typedef struct BODYCONV_DATA
{
	short Anim2;
	short Anim3;
	short Anim4;
	short Anim5;
	
	void Reset()
	{
		Anim2 = -1;
		Anim3 = -1;
		Anim4 = -1;
		Anim5 = -1;
	}
} *PBODYCONV_DATA;
//----------------------------------------------------------------------------------
typedef struct ANIM_DATA
{
	char FrameData[64];
	unsigned char Unknown;
	unsigned char FrameCount;
	unsigned char FrameInterval;
	unsigned char FrameStart;
} *PANIM_DATA;
//----------------------------------------------------------------------------------
typedef struct ART_IDX_BLOCK : public BASE_IDX_BLOCK
{
	unsigned int Unknown;
} *PART_IDX_BLOCK;
//----------------------------------------------------------------------------------
typedef struct LIGHT_IDX_BLOCK : public BASE_IDX_BLOCK
{
	unsigned short Height;
	unsigned short Width;
} *PLIGHT_IDX_BLOCK;
//----------------------------------------------------------------------------------
typedef struct ANIM_IDX_BLOCK : public BASE_IDX_BLOCK
{
	unsigned int Unknown;
} *PANIM_IDX_BLOCK;
//----------------------------------------------------------------------------------
typedef struct TEXTURE_IDX_BLOCK : public BASE_IDX_BLOCK
{
	unsigned int Unknown;
} *PTEXTURE_IDX_BLOCK;
//----------------------------------------------------------------------------------
typedef struct PALETTE_BLOCK
{
	unsigned char R;
	unsigned char G;
	unsigned char B;
} *PPALETTE_BLOCK;
//----------------------------------------------------------------------------------
typedef struct FONT_HEADER
{
	unsigned char Width;
	unsigned char Height;
	unsigned char Unknown;
} *PFONT_HEADER;
//----------------------------------------------------------------------------------
typedef struct FONT_CHARACTER
{
	unsigned char Width;
	unsigned char Height;
	unsigned char Unknown;
} *PFONT_CHARACTER;
//----------------------------------------------------------------------------------
typedef struct FONT_CHARACTER_DATA
{
	unsigned char Width;
	unsigned char Height;
	USHORT_LIST Data;
} *PFONT_CHARACTER_DATA;
//----------------------------------------------------------------------------------
typedef struct FONT_DATA
{
	unsigned char Header;
	FONT_CHARACTER_DATA Chars[224];
} *PFONT_DATA;
//----------------------------------------------------------------------------------
struct FONT_OBJECT
{
	unsigned char Width;
	unsigned char Height;
	GLuint Texture;
	unsigned char *Data;
};
//----------------------------------------------------------------------------------
typedef struct UNICODE_FONT_DATA
{
	unsigned char OffsetX;
	unsigned char OffsetY;
	unsigned char Width;
	unsigned char Height;
} *PUNICODE_FONT_DATA;
//----------------------------------------------------------------------------------
typedef struct SOUND_IDX_BLOCK : public BASE_IDX_BLOCK
{
	unsigned short Index;
	unsigned short Reserved;
} *PSOUND_IDX_BLOCK;
//----------------------------------------------------------------------------------
typedef struct SOUND_BLOCK
{
	char Name[16];
	unsigned int Unknown1;
	unsigned int Unknown2;
	unsigned int Unknown3;
	unsigned int Unknown4;
	//Data;
} *PSOUND_BLOCK;

#pragma pack (pop)
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
