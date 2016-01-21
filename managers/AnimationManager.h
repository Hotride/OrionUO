/****************************************************************************
**
** AnimationManager.h
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
#ifndef AnimationManagerH
#define AnimationManagerH
//---------------------------------------------------------------------------
class TAnimationManager
{
private:
	DWORD m_AddressIdx[6];
	DWORD m_AddressMul[6];
	DWORD m_SizeIdx[6];

	WORD m_Color;
	int m_AnimGroup;
	BYTE m_Direction;
	bool m_Grayed;
	
	static const int FRAME_BUFFER_WIDTH = 300;
	static const int FRAME_BUFFER_HEIGHT = 300;
	TFrameBuffer m_FrameBuffer;

	static const int m_LayerCount = 19;
	static int m_UsedLayers[8][m_LayerCount];

	TIndexAnimation m_DataIndex[0x0800];

	TLinkedList *m_UsedAnimList;

    float LUMA_THRESHOLD;
    float ALPHA_SCALE;
    int ALPHA_BITS;
    int BIT_STEP;

	bool TestImagePixels(TTextureAnimationDirection *direction, BYTE &frame, WORD &id, int &checkX, int &checkY);
	bool TestPixels(TGameObject *obj, int x, int y, bool &mirror, BYTE &frameIndex, WORD id = 0x0000);

	float getLuma(unsigned char&, unsigned char&, unsigned char&);
	void setAlphaAt(std::vector<bool>&, PDWORD, short&, short&, int&, int&, float&, float&);

	void Draw(TGameObject *obj, int x, int y, bool &mirror, BYTE &frameIndex, WORD id = 0x0000);
public:
	TAnimationManager();
	~TAnimationManager();
	
	void Init(int idx, DWORD AddressIdx, DWORD AddressMul, DWORD SizeIdx)
	{
		m_AddressIdx[idx] = AddressIdx;
		m_AddressMul[idx] = AddressMul;
		m_SizeIdx[idx] = SizeIdx;
	}

	SETGET(WORD, Color)
	SETGET(int, AnimGroup)
	SETGET(BYTE, Direction)


	void EstimateImageCornerAlpha(PDWORD pixels, short &width, short &height, float alpha_scale = 5.0f, float luma_threshold = 18.0f);

	void InitBodyconv(PDWORD verdata, string fName);
	void Load(PDWORD verdata);

	TTextureAnimationFrame *GetFrame(TGameObject *obj, BYTE &frameIndex, WORD id = 0x0000);

	void ClearUnusedTextures(DWORD ticks);

	bool ExecuteDirectionGroup(TTextureAnimationDirection *direction, WORD &id, int &offset);

	void GetAnimDirection(BYTE &dir, bool &mirror);

	TTextureAnimation *GetAnimation(WORD id);
	
	void DrawCharacter(TGameCharacter *obj, int x, int y, int z);
	bool CharacterPixelsInXY(TGameCharacter *obj, int X, int Y, int Z);

	void DrawCorpse(TGameItem *obj, int x, int y, int z);
	bool CorpsePixelsInXY(TGameItem *obj, int x, int y, int z);

	BYTE GetDieGroupIndex(WORD id, bool second);
	ANIMATION_GROUPS GetGroupIndex(WORD id);
};

extern TAnimationManager *AnimationManager;
//---------------------------------------------------------------------------
#endif