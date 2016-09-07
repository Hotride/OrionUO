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
#include "Globals.h"
//----------------------------------------------------------------------------------
#pragma pack (push,1)

typedef struct VERDATA_HEADER
{
	uint FileID;
	uint BlockID;
	uint Position;
	uint Size;
	uint GumpData;
} *PVERDATA_HEADER;
//----------------------------------------------------------------------------------
typedef struct MAP_CELLS_EX
{
	ushort TileID;
	ushort Color;
	char Z;
} *PMAP_CELLS_EX;
//----------------------------------------------------------------------------------
typedef struct MAP_CELLS
{
	ushort TileID;
	char Z;
} *PMAP_CELLS;
//----------------------------------------------------------------------------------
typedef struct MAP_BLOCK
{
	uint Header;
	MAP_CELLS Cells[64];
} *PMAP_BLOCK;
//----------------------------------------------------------------------------------
typedef struct STAIDX_BLOCK
{
	uint Position;
	uint Size;
	uint Unknown;
} *PSTAIDX_BLOCK;
//----------------------------------------------------------------------------------
typedef struct STATICS_BLOCK
{
	ushort Color;
	uchar X;
	uchar Y;
	char Z;
	ushort Hue;
} *PSTATICS_BLOCK;
//----------------------------------------------------------------------------------
struct LAND_TILES
{
	uint Flags;
	ushort TexID;
	char Name[20];
};
//----------------------------------------------------------------------------------
typedef struct LAND_GROUP
{
	uint Unknown;
	LAND_TILES Tiles[32];
} *PLAND_GROUP;
//----------------------------------------------------------------------------------
struct STATIC_TILES
{
	uint Flags;
	uchar Weight;
	uchar Quality;
	ushort Unknown;
	uchar Unknown1;
	uchar Quality1;
	ushort AnimID;
	uchar Unknown2;
	uchar Hue;
	uchar SittingOffset;
	uchar Unknown3;
	uchar Height;
	char Name[20];
};
//----------------------------------------------------------------------------------
typedef struct STATIC_GROUP
{
	uint Unk;
	STATIC_TILES Tiles[32];
} *PSTATIC_GROUP;
//----------------------------------------------------------------------------------
typedef struct MULTI_IDX_BLOCK
{
	uint Position;
	uint Size;
	uint Unknown;
} *PMULTI_IDX_BLOCK;
//----------------------------------------------------------------------------------
typedef struct MULTI_BLOCK
{
	ushort ID;
	short X;
	short Y;
	short Z;
	uint Flags;
} *PMULTI_BLOCK;
//----------------------------------------------------------------------------------
typedef struct HUES_BLOCK
{
	ushort ColorTable[32];
	ushort TableStart;
	ushort TableEnd;
	char Name[20];
} *PHUES_BLOCK;
//----------------------------------------------------------------------------------
typedef struct HUES_GROUP
{
	uint Header;
	HUES_BLOCK Entries[8];
} *PHUES_GROUP;
//----------------------------------------------------------------------------------
typedef struct VERDATA_HUES_BLOCK
{
	ushort ColorTable[32];
	ushort TableStart;
	ushort TableEnd;
	char Name[20];
	ushort Unk[32];
} *PVERDATA_HUES_BLOCK;
//----------------------------------------------------------------------------------
typedef struct VERDATA_HUES_GROUP
{
	uint Header;
	VERDATA_HUES_BLOCK Entries[8];
} *PVERDATA_HUES_GROUP;
//----------------------------------------------------------------------------------
typedef struct GUMP_IDX_BLOCK
{
	uint Position;
	uint Size;
	ushort Height;
	ushort Width;
} *PGUMP_IDX_BLOCK;
//----------------------------------------------------------------------------------
typedef struct GUMP_BLOCK
{
	ushort Value;
	ushort Run;
} *PGUMP_BLOCK;
//----------------------------------------------------------------------------------
typedef struct SKILLS_IDX_BLOCK
{
	uint Position;
	uint Size;
	uint Unknown;
} *PSKILLS_IDX_BLOCK;
//----------------------------------------------------------------------------------
typedef struct SKILLS_BLOCK
{
	uchar Button;
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
	uchar Unknown;
	uchar FrameCount;
	uchar FrameInterval;
	uchar FrameStart;
} *PANIM_DATA;
//----------------------------------------------------------------------------------
typedef struct ART_IDX_BLOCK
{
	uint Position;
	uint Size;
	uint Unknown;
} *PART_IDX_BLOCK;
//----------------------------------------------------------------------------------
typedef struct LIGHT_IDX_BLOCK
{
	uint Position;
	uint Size;
	ushort Height;
	ushort Width;
} *PLIGHT_IDX_BLOCK;
//----------------------------------------------------------------------------------
typedef struct ANIM_FRAME_INFO
{
	short ImageCenterX;
	short ImageCenterY;
	ushort Width;
	ushort Height;
	uchar FrameCount;
	pushort Palette;
	pushort Address;

	void Reset()
	{
		ImageCenterX = 0;
		ImageCenterY = 0;
		Width = 0;
		Height = 0;
		FrameCount = 0;
		Palette = NULL;
		Address = NULL;
	}
} *PANIM_FRAME_INFO;
//----------------------------------------------------------------------------------
typedef struct ANIM_IDX_BLOCK
{
	uint Position;
	uint Size;
	uint Unknown;
} *PANIM_IDX_BLOCK;
//----------------------------------------------------------------------------------
typedef struct ANIM_FRAME
{
	short ImageCenterX;
	short ImageCenterY;
	ushort Width;
	ushort Height;
	GLuint Texture;
	puchar Data;

	void Reset();
	void Clear();
} *PANIM_FRAME;
//----------------------------------------------------------------------------------
typedef struct ANIM_GROUP
{
	ushort Palette[256];
	uint FrameCount;
	uint *FrameOffset;
	PANIM_FRAME Frames;
} *PANIM_GROUP;
//----------------------------------------------------------------------------------
typedef struct ANIMATION_DATA
{
	uint FrameCount[5];
	PANIM_FRAME Frames[5];
} *PANIMATION_DATA;
//----------------------------------------------------------------------------------
typedef struct TEXTURE_IDX_BLOCK
{
	uint Position;
	uint Size;
	uint Unknown;
} *PTEXTURE_IDX_BLOCK;
//----------------------------------------------------------------------------------
typedef struct PALETTE_BLOCK
{
	uchar R;
	uchar G;
	uchar B;
} *PPALETTE_BLOCK;
//----------------------------------------------------------------------------------
typedef struct FONT_HEADER
{
	uchar Width;
	uchar Height;
	uchar Unknown;
} *PFONT_HEADER;
//----------------------------------------------------------------------------------
typedef struct FONT_CHARACTER
{
	uchar Width;
	uchar Height;
	uchar Unknown;
} *PFONT_CHARACTER;
//----------------------------------------------------------------------------------
typedef struct FONT_CHARACTER_DATA
{
	uchar Width;
	uchar Height;
	pushort Data;
} *PFONT_CHARACTER_DATA;
//----------------------------------------------------------------------------------
typedef struct FONT_DATA
{
	uchar Header;
	FONT_CHARACTER_DATA Chars[224];
} *PFONT_DATA;
//----------------------------------------------------------------------------------
struct FONT_OBJECT
{
	uchar Width;
	uchar Height;
	GLuint Texture;
	puchar Data;
};
//----------------------------------------------------------------------------------
typedef struct UNICODE_FONT_DATA
{
	uchar OffsetX;
	uchar OffsetY;
	uchar Width;
	uchar Height;
} *PUNICODE_FONT_DATA;
//----------------------------------------------------------------------------------
typedef struct SOUND_IDX_BLOCK
{
	uint Position;
	uint Size;
	ushort Index;
	ushort Reserved;
} *PSOUND_IDX_BLOCK;
//----------------------------------------------------------------------------------
typedef struct SOUND_BLOCK
{
	char Name[16];
	uint Unknown1;
	uint Unknown2;
	uint Unknown3;
	uint Unknown4;
	//Data;
} *PSOUND_BLOCK;

#pragma pack (pop)
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
