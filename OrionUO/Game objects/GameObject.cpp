/***********************************************************************************
**
** GameObject.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GameObject.h"
#include "GameWorld.h"
#include "GameEffect.h"
#include "../Managers/ClilocManager.h"
#include "../Managers/MouseManager.h"
#include "../Managers/FontsManager.h"
#include "../Managers/ConfigManager.h"
#include "../Managers/ColorManager.h"
#include "../Managers/PacketManager.h"
#include "../Managers/GumpManager.h"
#include "../Managers/PluginManager.h"
#include "../SelectedObject.h"
#include "../OrionUO.h"
#include "../ServerList.h"
#include "../OrionWindow.h"
//----------------------------------------------------------------------------------
CGameObject::CGameObject(const uint &serial)
: CRenderStaticObject(ROT_GAME_OBJECT, serial, 0, 0, 0, 0, 0), m_Container(0xFFFFFFFF),
m_MapIndex(0), m_Count(0), m_Flags(0), m_Name(""), m_NPC(false), m_Clicked(false),
m_Effects(NULL), m_AnimIndex(0), m_YouSeeJournalPrefix(false),
m_LastAnimationChangeTime(GetTickCount()), m_ClilocMessage(L"")
{
	memset(&m_FrameInfo, 0, sizeof(DRAW_FRAME_INFORMATION));

#if UO_DEBUG_INFO!=0
	g_GameObjectsCount++;
#endif //UO_DEBUG_INFO!=0
}
//----------------------------------------------------------------------------------
CGameObject::~CGameObject()
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
//----------------------------------------------------------------------------------
void CGameObject::OnChangedFlags(const uchar &val)
{
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
	if (IsPlayer() && m_Name != newName)
	{
		if (g_GameState >= GS_GAME)
		{
			char buf[256] = { 0 };
			sprintf_s(buf, "Ultima Online - %s (%s)", newName.c_str(), g_ServerList.GetSelectedServer()->Name.c_str());

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
void CGameObject::DrawObjectHandlesTexture(const int &x, const int &y)
{
	if (m_TextureObjectHalndes.Texture == NULL)
	{
		if (m_NPC || IsCorpse())
			GenerateObjectHandlesTexture(ToWString(m_Name));
		else
			GenerateObjectHandlesTexture(g_ClilocManager.Cliloc(g_Language)->GetW(102000 + m_Graphic, m_Name));
	}

	m_TextureObjectHalndes.Draw(x, y);
}
//----------------------------------------------------------------------------------
/*!
Выбрать текстуру опции Object Handles
@param [__in] x Экранная координата X
@param [__in] y Экранная координата Y
@return
*/
void CGameObject::SelectObjectHandlesTexture(const int &x, const int &y)
{
	if (m_TextureObjectHalndes.Texture != NULL)
	{
		int testX = g_MouseManager.Position.X - x;

		if (testX < 0 || testX >= g_ObjectHandlesWidth)
			return;

		int testY = g_MouseManager.Position.Y - y;

		if (testY < 0 || testY >= g_ObjectHandlesHeight)
			return;

		if (g_ObjectHandlesBackgroundPixels[(testY * g_ObjectHandlesWidth) + testX] != 0)
			g_SelectedObject.Init(this);
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

	m_TextureObjectHalndes.Width = g_ObjectHandlesWidth;
	m_TextureObjectHalndes.Height = g_ObjectHandlesHeight;

	g_GL.BindTexture16(m_TextureObjectHalndes.Texture, g_ObjectHandlesWidth, g_ObjectHandlesHeight, pixels);
}
//----------------------------------------------------------------------------------
/*!
Добавить текст в контейнер
@param [__in] td Ссылка на объект текста
@return
*/
void CGameObject::AddText(CTextData *msg)
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

	g_Orion.AddJournalMessage(msg, msgname);
}
//----------------------------------------------------------------------------------
/*!
Получить индекс анимации
@return Индекс анимации
*/
ushort CGameObject::GetMountAnimation()
{
	return m_Graphic; // + UO->GetStaticPointer(m_Graphic)->Increment;
}
//----------------------------------------------------------------------------------
/*!
Очистить контейнер
@return
*/
void CGameObject::Clear()
{
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
bool CGameObject::Poisoned()
{
	if (g_PacketManager.ClientVersion >= CV_7000)
		return (m_Flags & 0x20);
	else
		return (m_Flags & 0x04);
}
//----------------------------------------------------------------------------------
bool CGameObject::Flying()
{
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
int CGameObject::IsGold()
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
//----------------------------------------------------------------------------------
/*!
Получить индекс картинки для рисования
@param [__out] doubleDraw Двойная отрисовка объекта
@return Индекс картинки
*/
ushort CGameObject::GetDrawGraphic(bool &doubleDraw)
{
	int index = IsGold();
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
	if (m_NPC)
	{
		CGameCharacter *gc = GameCharacterPtr();

		x += gc->OffsetX;
		y += gc->OffsetY;
	}

	QFOR(effect, m_Effects, CGameEffect*)
	{
		effect->ApplyRenderMode();

		if (effect->EffectType == EF_LIGHTING)
		{
			ushort graphic = 0x4E20 + effect->AnimIndex;

			WISP_GEOMETRY::CSize size = g_Orion.GetGumpDimension(graphic);

			g_Orion.DrawGump(graphic, effect->Color, x - (size.Width / 2), y - (size.Height + (m_Z * 4)));
		}
		else
			g_Orion.DrawStaticArt(effect->GetCurrentGraphic(), effect->Color, x, y, m_Z);

		effect->RemoveRenderMode();
	}
}
//----------------------------------------------------------------------------------
void CGameObject::UpdateEffects()
{
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
	CGameObject *obj = this;

	while (obj->Container != 0xFFFFFFFF)
		obj = g_World->FindWorldObject(obj->Container);

	return obj;
}
//----------------------------------------------------------------------------------
CGameItem *CGameObject::FindLayer(const int &layer)
{
	QFOR(obj, m_Items, CGameItem*)
	{
		if (obj->Layer == layer)
			return obj;
	}

	return NULL;
}
//----------------------------------------------------------------------------------
CGameItem *CGameObject::FindSecureTradeBox()
{
	QFOR(obj, m_Items, CGameItem*)
	{
		if (!obj->Layer && obj->Graphic == 0x1E5E)
			return obj;
	}

	return NULL;
}
//----------------------------------------------------------------------------------
