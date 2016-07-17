/****************************************************************************
**
** GameObject.cpp
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
//---------------------------------------------------------------------------
TGameObject::TGameObject(DWORD serial)
: TRenderStaticObject(ROT_GAME_OBJECT, serial, 0, 0, 0, 0, 0), m_Container(0xFFFFFFFF),
m_MapIndex(0), m_Count(0), m_Flags(0), m_Name(""), m_NPC(false), m_Clicked(false),
m_Effects(NULL), m_AnimIndex(0), m_YouSeeJournalPrefix(false),
m_LastAnimationChangeTime(GetTickCount()), m_ClilocMessage(L"")
{
	memset(&m_TextureObjectHalndes, 0, sizeof(TTextureObject));
	memset(&m_FrameInfo, 0, sizeof(DRAW_FRAME_INFORMATION));

#if UO_DEBUG_INFO!=0
	g_GameObjectsCount++;
#endif //UO_DEBUG_INFO!=0
}
//---------------------------------------------------------------------------
TGameObject::~TGameObject()
{
	if (m_Effects != NULL)
	{
		delete m_Effects;
		m_Effects = NULL;
	}
	
	m_Next = NULL;
	m_Prev = NULL;

	if (m_TextureObjectHalndes.Texture != NULL)
	{
		glDeleteTextures(1, &m_TextureObjectHalndes.Texture);
		m_TextureObjectHalndes.Texture = NULL;
	}

	Clear();

#if UO_DEBUG_INFO!=0
	g_GameObjectsCount--;
#endif //UO_DEBUG_INFO!=0
}
//---------------------------------------------------------------------------
/*!
Отрисовать текстуру опции Object Handles (автоматически создает текстуру, если она не была создана)
@param [__in] x Экранная координата X
@param [__in] y Экранная координата Y
@return 
*/
void TGameObject::DrawObjectHandlesTexture(__in const bool &mode, __in int &x, __in int &y)
{
	if (mode)
	{
		if (m_TextureObjectHalndes.Texture == NULL)
		{
			if (m_NPC || IsCorpse())
				GenerateObjectHandlesTexture(ToWString(m_Name));
			else
				GenerateObjectHandlesTexture(ClilocManager->Cliloc(g_Language)->GetW(102000 + m_Graphic, m_Name));
		}

		if (m_TextureObjectHalndes.Texture != NULL)
			g_GL.Draw(m_TextureObjectHalndes.Texture, x, y, m_TextureObjectHalndes.Width, m_TextureObjectHalndes.Height);
	}
	else
	{
		if (m_TextureObjectHalndes.Texture != NULL)
		{
			int testX = g_MouseX - x;

			if (testX < 0 || testX >= g_ObjectHandlesWidth)
				return;

			int testY = g_MouseY - y;

			if (testY < 0 || testY >= g_ObjectHandlesHeight)
				return;

			if (g_ObjectHandlesBackgroundPixels[(testY * g_ObjectHandlesWidth) + testX] != 0)
			{
				g_LastObjectType = SOT_GAME_OBJECT;
				g_LastSelectedObject = m_Serial;
				g_SelectedObject = this;
			}
		}
	}
}
//---------------------------------------------------------------------------
/*!
Создать текстуру для опции Object Handles
@param [__in] text Текст названия объекта
@return 
*/
void TGameObject::GenerateObjectHandlesTexture( __in wstring text)
{
	if (m_TextureObjectHalndes.Texture != NULL)
	{
		glDeleteTextures(1, &m_TextureObjectHalndes.Texture);
		m_TextureObjectHalndes.Texture = NULL;
	}

	int width = g_ObjectHandlesWidth - 20;

	BYTE font = 1;
	TTextTexture textTexture;
	memset(&textTexture, 0, sizeof(TTextTexture));
	WORD color = 0xFFFF;
	BYTE cell = 30;
	TEXT_ALIGN_TYPE tat = TS_CENTER;
	WORD flags = 0;

	if (FontManager->GetWidthW(font, text.c_str(), text.length()) > width)
		text = FontManager->GetTextByWidthW(font, text.c_str(), text.length(), width - 6, true);

	PDWORD textData = FontManager->GeneratePixelsW(font, textTexture, text.c_str(), color, cell, width, tat, flags);

	if (textData == NULL)
		return;

	static const int size = g_ObjectHandlesWidth * g_ObjectHandlesHeight;
	WORD pixels[size] = { 0 };

	memcpy(&pixels[0], &g_ObjectHandlesBackgroundPixels[0], size * 2);

	color = 0;

	if (m_NPC)
	{
		if (IsPlayer())
			color = 0x0386;
		else
			color = ConfigManager.GetColorByNotoriety(this->GameCharacterPtr()->Notoriety);

		if (color)
		{
			IFOR(x, 0, g_ObjectHandlesWidth)
			{
				IFOR(y, 0, g_ObjectHandlesHeight)
				{
					WORD &pixel = pixels[(y * g_ObjectHandlesWidth) + x];

					if (pixel)
					{
						BYTE r = (pixel & 0x1F);
						BYTE g = ((pixel >> 5) & 0x1F);
						BYTE b = ((pixel >> 10) & 0x1F);

						if (r == g && r == b)
							pixel = ColorManager->GetColor16(pixel, color) | 0x8000;
					}
				}
			}
		}
	}

	int maxHeight = textTexture.Height;

	IFOR(x, 0, width)
	{
		int gumpDataX = x + 10;

		if (gumpDataX >= g_ObjectHandlesWidth)
			break;

		IFOR(y, 0, maxHeight)
		{
			int gumpDataY = y + 1;

			if (gumpDataY >= g_ObjectHandlesHeight)
				break;

			DWORD &pixel = textData[(y * textTexture.Width) + x];

			if (pixel)
			{
				PBYTE bytes = (PBYTE)&pixel;
				BYTE buf = bytes[0];
				bytes[0] = bytes[3];
				bytes[3] = buf;
				buf = bytes[1];
				bytes[1] = bytes[2];
				bytes[2] = buf;
				pixels[(gumpDataY * g_ObjectHandlesWidth) + gumpDataX] = ColorManager->Color32To16(pixel) | 0x8000;
			}
		}
	}

	m_TextureObjectHalndes.Width = g_ObjectHandlesWidth;
	m_TextureObjectHalndes.Height = g_ObjectHandlesHeight;

	g_GL.BindTexture16(m_TextureObjectHalndes.Texture, g_ObjectHandlesWidth, g_ObjectHandlesHeight, pixels);

	delete textData;
}
//---------------------------------------------------------------------------
/*!
Добавить текст в контейнер
@param [__in] td Ссылка на объект текста
@return
*/
void TGameObject::AddText(__in TTextData *msg)
{
	m_TextControl->Add(msg);

	string msgname = "You see: ";

	if (!m_YouSeeJournalPrefix)
		msgname = m_Name + ": ";

	/*if (m_Clicked)
	{
		m_Clicked = false;

		if (IsPlayer()) //(m_NPC)
			msgname = m_Name + ": ";
	}*/

	Orion->AddJournalMessage(msg, msgname);
}
//---------------------------------------------------------------------------
/*!
Получить индекс анимации
@return Индекс анимации
*/
WORD TGameObject::GetMountAnimation()
{
	return m_Graphic; // + UO->GetStaticPointer(m_Graphic)->Increment;
}
//---------------------------------------------------------------------------
/*!
Очистить контейнер
@return
*/
void TGameObject::Clear()
{
	if (!Empty())
	{
		TGameObject *obj = (TGameObject*)m_Items;

		while (obj != NULL)
		{
			TGameObject *next = (TGameObject*)obj->m_Next;

			World->RemoveObject(obj);

			obj = next;
		}

		m_Items = NULL;
	}
}
//---------------------------------------------------------------------------
/*!
Золото ли это
@return Индекс в таблице золота
*/
int TGameObject::IsGold()
{
	switch (m_Graphic)
	{
		case 0x0EED:
			return 1;
		/*case 0x0EEA:
			return 2;*/
		case 0x0EF0:
			return 3;
		default:
			break;
	}

	return 0;
}
//---------------------------------------------------------------------------
/*!
Получить индекс картинки для рисования
@param [__out] doubleDraw Двойная отрисовка объекта
@return Индекс картинки
*/
WORD TGameObject::GetDrawGraphic( __out bool &doubleDraw)
{
	int index = IsGold();
	WORD result = m_Graphic;

	const WORD graphicAccociateTable[3][3] =
	{
		{0x0EED, 0x0EEE, 0x0EEF},
		{0x0EEA, 0x0EEB, 0x0EEC},
		{0x0EF0, 0x0EF1, 0x0EF2}
	};

	if (index)
	{
		int graphicIndex = (int)(m_Count > 1) + (int)(m_Count > 5);
		result = graphicAccociateTable[index - 1][graphicIndex];
	}
	else
		doubleDraw = IsStackable() && (m_Count > 1);

	return result;
}
//---------------------------------------------------------------------------
/*!
Отрисовать эффект
@param [__in] drawX Экранная координата X
@param [__in] drawY Экранная координата Y
@param [__in] ticks Таймер рендера
@return 
*/
void TGameObject::DrawEffects( __in int &drawX, __in int &drawY, __in DWORD &ticks)
{
	TGameEffect *effect = m_Effects;
	char gZ = Z;
	char z = gZ * 4;

	int x = drawX;
	int y = drawY;

	if (m_NPC)
	{
		TGameCharacter *gc = GameCharacterPtr();

		x += gc->OffsetX;
		y += gc->OffsetY;

		//ANIMATION_DIMENSIONS dims = AnimationManager->GetAnimationDimensions(this);

		//x += dims.Width / 2;
		//y -= dims.Height / 2;
	}

	while (effect != NULL)
	{
		TGameEffect *next = (TGameEffect*)effect->m_Next;
		
		WORD eGraphic = 0;
		WORD gGraphic = 0;

		if (effect->Duration < ticks)
			RemoveEffect(effect);
		else if (effect->LastChangeFrameTime < ticks)
		{
			effect->LastChangeFrameTime = ticks + effect->Speed;

			if (effect->EffectType == EF_LIGHTING) //lighting
			{
				int animIndex = effect->AnimIndex;
				
				gGraphic = 0x4E20 + animIndex;

				animIndex++;
				if (animIndex >= 10)
					RemoveEffect(effect);
				else
					effect->AnimIndex = animIndex;
			}
			else if (effect->EffectType == EF_STAY_AT_SOURCE)
				eGraphic = effect->CalculateCurrentGraphic();
		}
		else
		{
			if (effect->EffectType == EF_LIGHTING) //lighting
				gGraphic = 0x4E20 + effect->AnimIndex;
			else if (effect->EffectType == EF_STAY_AT_SOURCE)
				eGraphic = effect->GetCurrentGraphic();
		}
		
		if (gGraphic != 0)
		{
			POINT p = {0, 0};
			Orion->GetGumpDimension(gGraphic, p);

			effect->ApplyRenderMode();

			Orion->DrawGump(gGraphic, effect->Color, x - (p.x / 2), (y - p.y) - z);

			effect->RemoveRenderMode();
		}
		else if (eGraphic != 0)
		{
			effect->ApplyRenderMode();

			Orion->DrawStaticArt(eGraphic, effect->Color, x, y, gZ);

			effect->RemoveRenderMode();
		}

		effect = next;
	}
}
//---------------------------------------------------------------------------
/*!
Добавить эффект
@param [__in] effect Ссылка на эффект
@return 
*/
void TGameObject::AddEffect( __in TGameEffect *effect)
{
	if (m_Effects == NULL)
	{
		m_Effects = effect;
		effect->m_Next = NULL;
		effect->m_Prev = NULL;
	}
	else
	{
		effect->m_Next = m_Effects;
		m_Effects->m_Prev = effect;
		effect->m_Prev = NULL;
		m_Effects = effect;
	}
}
//---------------------------------------------------------------------------
/*!
Удалить эффект
@param [__in] effect Ссылка на эффект
@return 
*/
void TGameObject::RemoveEffect( __in TGameEffect *effect)
{
	if (effect->m_Prev == NULL)
	{
		m_Effects = (TGameEffect*)effect->m_Next;

		if (m_Effects != NULL)
			m_Effects->m_Prev = NULL;
	}
	else
	{
		effect->m_Prev->m_Next = effect->m_Next;

		if (effect->m_Next != NULL)
			effect->m_Next->m_Prev = effect->m_Prev;
	}
	
	effect->m_Next = NULL;
	effect->m_Prev = NULL;
	delete effect;
}
//---------------------------------------------------------------------------
/*!
Добавить объект в список объектов текущего контейнера
@param [__in] obj Ссылка на объект
@return 
*/
void TGameObject::AddObject( __in TGameObject *obj)
{
	World->RemoveFromContainer(obj);

	if (m_Next == NULL)
	{
		m_Next = obj;
		m_Next->m_Prev = this;
		m_Next->m_Next = NULL;

		((TGameObject*)m_Next)->Container = m_Container;
	}
	else
	{
		TGameObject *item = (TGameObject*)m_Next;
		while (item->m_Next != NULL)
			item = (TGameObject*)item->m_Next;

		item->m_Next = obj;
		obj->m_Next = NULL;
		obj->m_Prev = item;

		obj->Container = m_Container;
	}
}
//---------------------------------------------------------------------------
/*!
Добавить объект в контейнер (this - контейнер)
@param [__in] obj Ссылка на объект
@return 
*/
void TGameObject::AddItem( __in TGameObject *obj)
{
	if (obj->Container != 0xFFFFFFFF)
		return;

	World->RemoveFromContainer(obj);

	if (m_Items != NULL)
	{
		TGameObject *item = (TGameObject*)m_Items;
		while (item->m_Next != NULL)
			item = (TGameObject*)item->m_Next;

		item->m_Next = obj;
		obj->m_Next = NULL;
		obj->m_Prev = item;
	}
	else
	{
		m_Items = obj;
		m_Items->m_Next = NULL;
		m_Items->m_Prev = NULL;
	}

	obj->Container = Serial;
}
//---------------------------------------------------------------------------
/*!
Изъять объект из контейнера
@param [__in] obj Ссылка на объект
@return 
*/
void TGameObject::Reject( __in TGameObject *obj)
{
	if (obj->Container != Serial)
		return;

	if (m_Items != NULL)
	{
		if (((TGameObject*)m_Items)->Serial == obj->Serial)
		{
			if (m_Items->m_Next != NULL)
			{
				m_Items = m_Items->m_Next;
				m_Items->m_Prev = NULL;
			}
			else
				m_Items = NULL;
		}
		else
		{
			if (obj->m_Next != NULL)
			{
				if (obj->m_Prev != NULL)
				{
					obj->m_Prev->m_Next = obj->m_Next;
					obj->m_Next->m_Prev = obj->m_Prev;
				}
				else //WTF???
					obj->m_Next->m_Prev = NULL;
			}
			else if (obj->m_Prev != NULL)
				obj->m_Prev->m_Next = NULL;
		}
	}

	obj->m_Next = NULL;
	obj->m_Prev = NULL;
	obj->Container = 0xFFFFFFFF;
}
//---------------------------------------------------------------------------
/*!
Найти объект в мире, в котором содержится контейнер
@return Ссылка на объект в мире
*/
TGameObject *TGameObject::GetTopObject()
{
	TGameObject *obj = this;

	while (obj->Container != 0xFFFFFFFF)
		obj = World->FindWorldObject(obj->Container);

	return obj;
}
//---------------------------------------------------------------------------