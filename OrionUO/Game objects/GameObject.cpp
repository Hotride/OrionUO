/***********************************************************************************
**
** GameObject.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGameObject::CGameObject(const uint &serial)
: CRenderStaticObject(ROT_GAME_OBJECT, serial, 0, 0, 0, 0, 0),
m_LastAnimationChangeTime(GetTickCount())
{
	WISPFUN_DEBUG("c20_f1");
	memset(&m_FrameInfo, 0, sizeof(DRAW_FRAME_INFORMATION));

#if UO_DEBUG_INFO!=0
	g_GameObjectsCount++;
#endif //UO_DEBUG_INFO!=0
}
//----------------------------------------------------------------------------------
CGameObject::~CGameObject()
{
	WISPFUN_DEBUG("c20_f2");
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
//----------------------------------------------------------------------------------
void CGameObject::OnChangedFlags(const uchar &val)
{
	WISPFUN_DEBUG("c20_f3");
	bool poisoned = Poisoned();
	bool yellowHits = YellowHits();

	m_Flags = val;

	if (poisoned != Poisoned() || yellowHits != YellowHits())
	{
		g_GumpManager.UpdateContent(m_Serial, 0, GT_STATUSBAR);
		g_GumpManager.UpdateContent(m_Serial, 0, GT_TARGET_SYSTEM);
	}
}
//----------------------------------------------------------------------------------
void CGameObject::OnChangeName(const string &newName)
{
	WISPFUN_DEBUG("c20_f4");
	if (IsPlayer() && m_Name != newName)
	{
		if (g_GameState >= GS_GAME)
		{
			char buf[256] = { 0 };
			CServer *server = g_ServerList.GetSelectedServer();
			if (server != NULL)
			{
				sprintf_s(buf, "Ultima Online - %s (%s)", newName.c_str(), server->Name.c_str());
			}
			else
			{
				sprintf_s(buf, "Ultima Online - %s", newName.c_str());
			}
				

			g_OrionWindow.SetTitle(buf);
		}

		g_PluginManager.WindowProc(g_OrionWindow.Handle, UOMSG_SET_PLAYER_NAME, (WPARAM)newName.c_str(), 0);
	}
}
//----------------------------------------------------------------------------------
/*!
Отрисовать текстуру опции Object Handles (автоматически создает текстуру, если она не была создана)
@param [__in] x Экранная координата X
@param [__in] y Экранная координата Y
@return 
*/
void CGameObject::DrawObjectHandlesTexture()
{
	WISPFUN_DEBUG("c20_f5");
	if (m_TextureObjectHalndes.Texture == NULL)
	{
		if (m_NPC || IsCorpse())
			GenerateObjectHandlesTexture(ToWString(m_Name));
		else
		{
			wstring name = ToWString(m_Name);
			if (!name.length())
				name = g_ClilocManager.Cliloc(g_Language)->GetW(1020000 + m_Graphic, true, g_Orion.m_StaticData[m_Graphic].Name);
			GenerateObjectHandlesTexture(name);
		}
	}

	int x = m_DrawX - g_ObjectHandlesWidthOffset;
	int y = m_DrawY;

	if (m_NPC)
	{
		CGameCharacter *gc = (CGameCharacter*)this;

		ANIMATION_DIMENSIONS dims = g_AnimationManager.GetAnimationDimensions(this);

		x += gc->OffsetX;
		y += gc->OffsetY - (gc->OffsetZ + dims.Height + dims.CenterY + 8);
	}
	else
		y -= g_Orion.GetArtDimension(m_Graphic, true).Height;

	m_TextureObjectHalndes.Draw(x, y);
}
//----------------------------------------------------------------------------------
/*!
Выбрать текстуру опции Object Handles
@param [__in] x Экранная координата X
@param [__in] y Экранная координата Y
@return
*/
void CGameObject::SelectObjectHandlesTexture()
{
	WISPFUN_DEBUG("c20_f6");
	if (m_TextureObjectHalndes.Texture != NULL)
	{
		int x = m_DrawX - g_ObjectHandlesWidthOffset;
		int y = m_DrawY;

		if (m_NPC)
		{
			CGameCharacter *gc = (CGameCharacter*)this;

			ANIMATION_DIMENSIONS dims = g_AnimationManager.GetAnimationDimensions(this);

			x += gc->OffsetX;
			y += gc->OffsetY - (gc->OffsetZ + dims.Height + dims.CenterY + 8);
		}
		else
			y -= g_Orion.GetArtDimension(m_Graphic, true).Height;

		x = g_MouseManager.Position.X - x;
		y = g_MouseManager.Position.Y - y;

		if (x < 0 || x >= g_ObjectHandlesWidth || y < 0 || y >= g_ObjectHandlesHeight)
			return;

		if (g_ObjectHandlesBackgroundPixels[(y * g_ObjectHandlesWidth) + x] != 0)
		{
			g_SelectedObject.Init(this);
			g_SelectedGameObjectHandle = m_Serial;
		}
	}
}
//----------------------------------------------------------------------------------
/*!
Создать текстуру для опции Object Handles
@param [__in] text Текст названия объекта
@return 
*/
void CGameObject::GenerateObjectHandlesTexture(wstring text)
{
	WISPFUN_DEBUG("c20_f7");
	if (m_TextureObjectHalndes.Texture != NULL)
	{
		glDeleteTextures(1, &m_TextureObjectHalndes.Texture);
		m_TextureObjectHalndes.Texture = NULL;
	}

	int width = g_ObjectHandlesWidth - 20;

	uchar font = 1;
	CGLTextTexture textTexture;
	ushort color = 0xFFFF;
	uchar cell = 30;
	TEXT_ALIGN_TYPE tat = TS_CENTER;
	ushort flags = 0;

	if (g_FontManager.GetWidthW(font, text.c_str(), text.length()) > width)
		text = g_FontManager.GetTextByWidthW(font, text.c_str(), text.length(), width - 6, true);

	UINT_LIST textData = g_FontManager.GeneratePixelsW(font, textTexture, text.c_str(), color, cell, width, tat, flags);

	if (!textData.size())
		return;

	static const int size = g_ObjectHandlesWidth * g_ObjectHandlesHeight;
	ushort pixels[size] = { 0 };

	memcpy(&pixels[0], &g_ObjectHandlesBackgroundPixels[0], size * 2);

	color = 0;

	if (m_NPC)
	{
		if (IsPlayer())
			color = 0x0386;
		else
			color = g_ConfigManager.GetColorByNotoriety(GameCharacterPtr()->Notoriety);

		if (color)
		{
			IFOR(x, 0, g_ObjectHandlesWidth)
			{
				IFOR(y, 0, g_ObjectHandlesHeight)
				{
					ushort &pixel = pixels[(y * g_ObjectHandlesWidth) + x];

					if (pixel)
					{
						uchar r = (pixel & 0x1F);
						uchar g = ((pixel >> 5) & 0x1F);
						uchar b = ((pixel >> 10) & 0x1F);

						if (r == g && r == b)
							pixel = g_ColorManager.GetColor16(pixel, color) | 0x8000;
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

			uint &pixel = textData[(y * textTexture.Width) + x];

			if (pixel)
			{
				puchar bytes = (PBYTE)&pixel;
				uchar buf = bytes[0];
				bytes[0] = bytes[3];
				bytes[3] = buf;
				buf = bytes[1];
				bytes[1] = bytes[2];
				bytes[2] = buf;
				pixels[(gumpDataY * g_ObjectHandlesWidth) + gumpDataX] = g_ColorManager.Color32To16(pixel) | 0x8000;
			}
		}
	}

	g_GL_BindTexture16(m_TextureObjectHalndes, g_ObjectHandlesWidth, g_ObjectHandlesHeight, pixels);
}
//----------------------------------------------------------------------------------
/*!
Добавить текст в контейнер
@param [__in] td Ссылка на объект текста
@return
*/
void CGameObject::AddText(CTextData *msg)
{
	WISPFUN_DEBUG("c20_f8");

	msg->Owner = this;
	m_TextControl->Add(msg);
	UpdateTextCoordinates();

	string msgname = "You see: ";

	if (!m_YouSeeJournalPrefix)
		msgname = m_Name + ": ";

	/*if (m_Clicked)
	{
		m_Clicked = false;

		if (IsPlayer()) //(m_NPC)
			msgname = m_Name + ": ";
	}*/

	g_Orion.AddJournalMessage(msg, msgname);
}
//----------------------------------------------------------------------------------
/*!
Получить индекс анимации
@return Индекс анимации
*/
ushort CGameObject::GetMountAnimation()
{
	WISPFUN_DEBUG("c20_f9");
	return m_Graphic; // + UO->GetStaticPointer(m_Graphic)->Increment;
}
//----------------------------------------------------------------------------------
/*!
Очистить контейнер
@return
*/
void CGameObject::Clear()
{
	WISPFUN_DEBUG("c20_f10");
	if (!Empty())
	{
		CGameObject *obj = (CGameObject*)m_Items;

		while (obj != NULL)
		{
			CGameObject *next = (CGameObject*)obj->m_Next;

			g_World->RemoveObject(obj);

			obj = next;
		}

		m_Items = NULL;
	}
}
//----------------------------------------------------------------------------------
void CGameObject::ClearUnequipped()
{
	WISPFUN_DEBUG("c20_f10_1");
	if (!Empty())
	{
		CGameObject *newFirstItem = NULL;
		CGameObject *obj = (CGameObject*)m_Items;

		while (obj != NULL)
		{
			CGameObject *next = (CGameObject*)obj->m_Next;

			if (((CGameItem*)obj)->Layer != OL_NONE)
			{
				if (newFirstItem == NULL)
					newFirstItem = obj;
			}
			else
				g_World->RemoveObject(obj);

			obj = next;
		}

		m_Items = newFirstItem;
	}
}
//----------------------------------------------------------------------------------
void CGameObject::ClearNotOpenedItems()
{
	WISPFUN_DEBUG("c20_f11");
	if (!Empty())
	{
		CGameObject *obj = (CGameObject*)m_Items;

		while (obj != NULL)
		{
			CGameObject *next = (CGameObject*)obj->m_Next;

			if (!obj->NPC && !((CGameItem*)obj)->Opened && ((CGameItem*)obj)->Layer != OL_BACKPACK)
				g_World->RemoveObject(obj);

			obj = next;
		}
	}
}
//----------------------------------------------------------------------------------
bool CGameObject::Poisoned()
{
	WISPFUN_DEBUG("c20_f12");
	if (g_PacketManager.ClientVersion >= CV_7000)
		return SA_Poisoned;
	else
		return (m_Flags & 0x04);
}
//----------------------------------------------------------------------------------
bool CGameObject::Flying()
{
	WISPFUN_DEBUG("c20_f13");
	if (g_PacketManager.ClientVersion >= CV_7000)
		return (m_Flags & 0x04);
	else
		return false;
}
//----------------------------------------------------------------------------------
/*!
Золото ли это
@return Индекс в таблице золота
*/
int CGameObject::IsGold(const ushort &graphic)
{
	WISPFUN_DEBUG("c20_f14");
	switch (graphic)
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
//----------------------------------------------------------------------------------
/*!
Получить индекс картинки для рисования
@param [__out] doubleDraw Двойная отрисовка объекта
@return Индекс картинки
*/
ushort CGameObject::GetDrawGraphic(bool &doubleDraw)
{
	WISPFUN_DEBUG("c20_f15");
	int index = IsGold(m_Graphic);
	ushort result = m_Graphic;

	const ushort graphicAssociateTable[3][3] =
	{
		{0x0EED, 0x0EEE, 0x0EEF},
		{0x0EEA, 0x0EEB, 0x0EEC},
		{0x0EF0, 0x0EF1, 0x0EF2}
	};

	if (index)
	{
		int graphicIndex = (int)(m_Count > 1) + (int)(m_Count > 5);
		result = graphicAssociateTable[index - 1][graphicIndex];
	}
	else
		doubleDraw = IsStackable() && (m_Count > 1);

	return result;
}
//----------------------------------------------------------------------------------
/*!
Отрисовать эффект
@param [__in] drawX Экранная координата X
@param [__in] drawY Экранная координата Y
@param [__in] ticks Таймер рендера
@return 
*/
void CGameObject::DrawEffects(int x, int y)
{
	WISPFUN_DEBUG("c20_f16");
	if (m_NPC)
	{
		CGameCharacter *gc = GameCharacterPtr();

		x += gc->OffsetX;
		y += gc->OffsetY - (int)gc->OffsetZ - 3;
	}

	QFOR(effect, m_Effects, CGameEffect*)
	{
		effect->ApplyRenderMode();

		if (effect->EffectType == EF_LIGHTING)
		{
			ushort graphic = 0x4E20 + effect->AnimIndex;

			WISP_GEOMETRY::CSize size = g_Orion.GetGumpDimension(graphic);

			g_Orion.DrawGump(graphic, effect->Color, x - (size.Width / 2), y - size.Height);
		}
		else
			g_Orion.DrawStaticArt(effect->GetCurrentGraphic(), effect->Color, x, y);

		effect->RemoveRenderMode();
	}
}
//----------------------------------------------------------------------------------
void CGameObject::UpdateEffects()
{
	WISPFUN_DEBUG("c20_f17");
	CGameEffect *effect = m_Effects;

	while (effect != NULL)
	{
		CGameEffect *next = (CGameEffect*)effect->m_Next;

		effect->Update(this);

		effect = next;
	}
}
//----------------------------------------------------------------------------------
/*!
Добавить эффект
@param [__in] effect Ссылка на эффект
@return 
*/
void CGameObject::AddEffect(CGameEffect *effect)
{
	WISPFUN_DEBUG("c20_f18");
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
//----------------------------------------------------------------------------------
/*!
Удалить эффект
@param [__in] effect Ссылка на эффект
@return 
*/
void CGameObject::RemoveEffect(CGameEffect *effect)
{
	WISPFUN_DEBUG("c20_f19");
	if (effect->m_Prev == NULL)
	{
		m_Effects = (CGameEffect*)effect->m_Next;

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
//----------------------------------------------------------------------------------
/*!
Добавить объект в список объектов текущего контейнера
@param [__in] obj Ссылка на объект
@return 
*/
void CGameObject::AddObject(CGameObject *obj)
{
	WISPFUN_DEBUG("c20_f20");
	g_World->RemoveFromContainer(obj);

	if (m_Next == NULL)
	{
		m_Next = obj;
		m_Next->m_Prev = this;
		m_Next->m_Next = NULL;

		((CGameObject*)m_Next)->Container = m_Container;
	}
	else
	{
		CGameObject *item = (CGameObject*)m_Next;

		while (item->m_Next != NULL)
			item = (CGameObject*)item->m_Next;

		item->m_Next = obj;
		obj->m_Next = NULL;
		obj->m_Prev = item;

		obj->Container = m_Container;
	}
}
//----------------------------------------------------------------------------------
/*!
Добавить объект в контейнер (this - контейнер)
@param [__in] obj Ссылка на объект
@return 
*/
void CGameObject::AddItem(CGameObject *obj)
{
	WISPFUN_DEBUG("c20_f21");
	if (obj->Container != 0xFFFFFFFF)
		return;

	g_World->RemoveFromContainer(obj);

	if (m_Items != NULL)
	{
		CGameObject *item = (CGameObject*)Last();

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
//----------------------------------------------------------------------------------
/*!
Изъять объект из контейнера
@param [__in] obj Ссылка на объект
@return 
*/
void CGameObject::Reject(CGameObject *obj)
{
	WISPFUN_DEBUG("c20_f22");
	if (obj->Container != m_Serial)
		return;

	if (m_Items != NULL)
	{
		if (((CGameObject*)m_Items)->Serial == obj->Serial)
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
//----------------------------------------------------------------------------------
/*!
Найти объект в мире, в котором содержится контейнер
@return Ссылка на объект в мире
*/
CGameObject *CGameObject::GetTopObject()
{
	WISPFUN_DEBUG("c20_f23");
	CGameObject *obj = this;

	while (obj->Container != 0xFFFFFFFF)
		obj = g_World->FindWorldObject(obj->Container);

	return obj;
}
//----------------------------------------------------------------------------------
CGameItem *CGameObject::FindLayer(const int &layer)
{
	WISPFUN_DEBUG("c20_f24");
	QFOR(obj, m_Items, CGameItem*)
	{
		if (obj->Layer == layer)
			return obj;
	}

	return NULL;
}
//----------------------------------------------------------------------------------
