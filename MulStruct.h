/****************************************************************************
**
** MulStrict.h
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
#ifndef MulStrictH
#define MulStrictH
//--------------------------------------------------------------------------
#pragma pack (push,1)

typedef struct VERDATA_HEADER
{
	DWORD FileID;
	DWORD BlockID;
	DWORD Position;
	DWORD Size;
	DWORD GumpData;
} *PVERDATA_HEADER;
//--------------------------------------------------------------------------
typedef struct MAP_CELLS_EX
{
	WORD TileID;
	WORD Color;
	char Z;
} *PMAP_CELLS_EX;
//--------------------------------------------------------------------------
typedef struct MAP_CELLS
{
	WORD TileID;
	char Z;
} *PMAP_CELLS;
//--------------------------------------------------------------------------
typedef struct MAP_BLOCK
{
	DWORD Header;
	MAP_CELLS Cells[64];
} *PMAP_BLOCK;
//--------------------------------------------------------------------------
typedef struct STAIDX_BLOCK
{
	DWORD Position;
	DWORD Size;
	DWORD Unknown;
} *PSTAIDX_BLOCK;
//--------------------------------------------------------------------------
typedef struct STATICS_BLOCK
{
	WORD Color;
	BYTE X;
	BYTE Y;
	char Z;
	WORD Hue;
} *PSTATICS_BLOCK;
//--------------------------------------------------------------------------
struct LAND_TILES
{
	DWORD Flags;
	WORD TexID;
	char Name[20];
};
//--------------------------------------------------------------------------
typedef struct LAND_GROUP
{
	DWORD Unknown;
	LAND_TILES Tiles[32];
} *PLAND_GROUP;
//--------------------------------------------------------------------------
struct STATIC_TILES
{
	DWORD Flags;
	BYTE Weight;
	BYTE Quality;
	WORD Unknown;
	BYTE Unknown1;
	BYTE Quality1;
	WORD AnimID;
	BYTE Unknown2;
	BYTE Hue;
	WORD Unknown3;
	BYTE Height;
	char Name[20];
};
//--------------------------------------------------------------------------
typedef struct STATIC_GROUP
{
	DWORD Unk;
	STATIC_TILES Tiles[32];
} *PSTATIC_GROUP;
//--------------------------------------------------------------------------
typedef struct MULTI_IDX_BLOCK
{
	DWORD Position;
	DWORD Size;
	DWORD Unknown;
} *PMULTI_IDX_BLOCK;
//--------------------------------------------------------------------------
typedef struct MULTI_BLOCK
{
	WORD ID;
	short X;
	short Y;
	short Z;
	DWORD Flags;
} *PMULTI_BLOCK;
//--------------------------------------------------------------------------
typedef struct HUES_BLOCK
{
	WORD ColorTable[32];
	WORD TableStart;
	WORD TableEnd;
	char Name[20];
} *PHUES_BLOCK;
//--------------------------------------------------------------------------
typedef struct HUES_GROUP
{
	DWORD Header;
	HUES_BLOCK Entries[8];
} *PHUES_GROUP;
//--------------------------------------------------------------------------
typedef struct VERDATA_HUES_BLOCK
{
	WORD ColorTable[32];
	WORD TableStart;
	WORD TableEnd;
	char Name[20];
	WORD Unk[32];
} *PVERDATA_HUES_BLOCK;
//--------------------------------------------------------------------------
typedef struct VERDATA_HUES_GROUP
{
	DWORD Header;
	VERDATA_HUES_BLOCK Entries[8];
} *PVERDATA_HUES_GROUP;
//--------------------------------------------------------------------------
typedef struct GUMP_IDX_BLOCK
{
	DWORD Position;
	DWORD Size;
	WORD Height;
	WORD Width;
} *PGUMP_IDX_BLOCK;
//--------------------------------------------------------------------------
typedef struct GUMP_BLOCK
{
	WORD Value;
	WORD Run;
} *PGUMP_BLOCK;
//--------------------------------------------------------------------------
typedef struct SKILLS_IDX_BLOCK
{
	DWORD Position;
	DWORD Size;
	DWORD Unknown;
} *PSKILLS_IDX_BLOCK;
//--------------------------------------------------------------------------
typedef struct SKILLS_BLOCK
{
	BYTE Button;
	char *SkillName;
} *PSKILLS_BLOCK;
//--------------------------------------------------------------------------
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
//--------------------------------------------------------------------------
typedef struct ANIM_DATA
{
	char FrameData[64];
	BYTE Unknown;
	BYTE FrameCount;
	BYTE FrameInterval;
	BYTE FrameStart;
} *PANIM_DATA;
//--------------------------------------------------------------------------
typedef struct ART_IDX_BLOCK
{
	DWORD Position;
	DWORD Size;
	DWORD Unknown;
} *PART_IDX_BLOCK;
//--------------------------------------------------------------------------
typedef struct LIGHT_IDX_BLOCK
{
	DWORD Position;
	DWORD Size;
	WORD Height;
	WORD Width;
} *PLIGHT_IDX_BLOCK;
//--------------------------------------------------------------------------
typedef struct ANIM_FRAME_INFO
{
	short ImageCenterX;
	short ImageCenterY;
	WORD Width;
	WORD Height;
	BYTE FrameCount;
	PWORD Palette;
	PWORD Address;

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
//--------------------------------------------------------------------------
typedef struct ANIM_IDX_BLOCK
{
	DWORD Position;
	DWORD Size;
	DWORD Unknown;
} *PANIM_IDX_BLOCK;
//--------------------------------------------------------------------------
typedef struct ANIM_FRAME
{
	short ImageCenterX;
	short ImageCenterY;
	WORD Width;
	WORD Height;
	GLuint Texture;
	PBYTE Data;

	void Reset();
	void Clear();
} *PANIM_FRAME;
//--------------------------------------------------------------------------
typedef struct ANIM_GROUP
{
	WORD Palette[256];
	DWORD FrameCount;
	DWORD *FrameOffset;
	PANIM_FRAME Frames;
} *PANIM_GROUP;
//--------------------------------------------------------------------------
typedef struct ANIMATION_DATA
{
	DWORD FrameCount[5];
	PANIM_FRAME Frames[5];
} *PANIMATION_DATA;
//--------------------------------------------------------------------------
typedef struct TEXTURE_IDX_BLOCK
{
	DWORD Position;
	DWORD Size;
	DWORD Unknown;
} *PTEXTURE_IDX_BLOCK;
//--------------------------------------------------------------------------
typedef struct PALETTE_BLOCK
{
	BYTE R;
	BYTE G;
	BYTE B;
} *PPALETTE_BLOCK;
//--------------------------------------------------------------------------
typedef struct FONT_HEADER
{
	BYTE Width;
	BYTE Height;
	BYTE Unknown;
} *PFONT_HEADER;
//--------------------------------------------------------------------------
typedef struct FONT_CHARACTER
{
	BYTE Width;
	BYTE Height;
	BYTE Unknown;
} *PFONT_CHARACTER;
//--------------------------------------------------------------------------
typedef struct FONT_CHARACTER_DATA
{
	BYTE Width;
	BYTE Height;
	PWORD Data;
} *PFONT_CHARACTER_DATA;
//--------------------------------------------------------------------------
typedef struct FONT_DATA
{
	BYTE Header;
	FONT_CHARACTER_DATA Chars[224];
} *PFONT_DATA;
//--------------------------------------------------------------------------
struct FONT_OBJECT
{
	BYTE Width;
	BYTE Height;
	GLuint Texture;
	PBYTE Data;
};
//--------------------------------------------------------------------------
typedef struct UNICODE_FONT_DATA
{
	BYTE OffsetX;
	BYTE OffsetY;
	BYTE Width;
	BYTE Height;
} *PUNICODE_FONT_DATA;
//--------------------------------------------------------------------------
typedef struct SOUND_IDX_BLOCK
{
	DWORD Position;
	DWORD Size;
	WORD Index;
	WORD Reserved;
} *PSOUND_IDX_BLOCK;
//--------------------------------------------------------------------------
typedef struct SOUND_BLOCK
{
	char Name[16];
	DWORD Unknown1;
	DWORD Unknown2;
	DWORD Unknown3;
	DWORD Unknown4;
	//Data;
} *PSOUND_BLOCK;

#pragma pack (pop)
//--------------------------------------------------------------------------
#endif