/***********************************************************************************
**
** GUISkillItem.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GUISkillItem.h"
#include "../OrionUO.h"
#include "../Managers/MouseManager.h"
#include "../SelectedObject.h"
#include "../PressedObject.h"
#include "../Skills.h"
#include "../Game objects/GamePlayer.h"
//----------------------------------------------------------------------------------
CGUISkillItem::CGUISkillItem(const uint &serial, const uint &useSerial, const uint &statusSerial, const int &index, const int &x, const int &y)
: CBaseGUI(GOT_SKILLITEM, serial, 0, 0, x, y), m_Index(index)
{
	if (g_Skills[m_Index].Button != 0)
		m_ButtonUse = new CGUIButton(useSerial, 0x0837, 0x0838, 0x0838, 8, 0);
	else
		m_ButtonUse = NULL;

	m_Status = g_Player->GetSkillStatus(m_Index);

	ushort graphic = GetStatusButtonGraphic();
	m_ButtonStatus = new CGUIButton(statusSerial, graphic, graphic, graphic, 251, 0);
	m_ButtonStatus->CheckPolygone = true;

	g_FontManager.GenerateA(9, m_NameText, g_Skills[m_Index].Name.c_str(), 0x0288);

	CreateValueText();
}
//----------------------------------------------------------------------------------
CGUISkillItem::~CGUISkillItem()
{
	m_NameText.Clear();
	m_ValueText.Clear();

	RELEASE_POINTER(m_ButtonUse);
	RELEASE_POINTER(m_ButtonStatus);
}
//----------------------------------------------------------------------------------
void CGUISkillItem::OnChangeStatus(const uchar &val)
{
	m_Status = val;

	ushort graphic = GetStatusButtonGraphic();

	m_ButtonStatus->Graphic = graphic;
	m_ButtonStatus->GraphicSelected = graphic;
	m_ButtonStatus->GraphicPressed = graphic;
}
//----------------------------------------------------------------------------------
ushort CGUISkillItem::GetStatusButtonGraphic()
{
	ushort graphic = 0x0984; //Up

	if (m_Status == 1)
		graphic = 0x0986; //Down
	else if (m_Status == 2)
		graphic = 0x082C; //Lock

	return graphic;
}
//----------------------------------------------------------------------------------
void CGUISkillItem::CreateValueText(const bool &showReal, const bool &showCap)
{
	//Значение скилла (учитывая выбранный флаг отображения)
	float val = g_Player->GetSkillBaseValue(m_Index);
	if (showReal)
		val = g_Player->GetSkillValue(m_Index);
	else if (showCap)
		val = g_Player->GetSkillCap(m_Index);

	char sbf[10] = { 0 };
	sprintf(sbf, "%.1f", val);

	g_FontManager.GenerateA(9, m_ValueText, sbf, 0x0288);
}
//----------------------------------------------------------------------------------
void CGUISkillItem::PrepareTextures()
{
	if (m_ButtonUse != NULL)
		m_ButtonUse->PrepareTextures();

	m_ButtonStatus->PrepareTextures();
}
//----------------------------------------------------------------------------------
CBaseGUI *CGUISkillItem::SelectedItem()
{
	CBaseGUI *selected = this;

	if (g_Orion.PolygonePixelsInXY(m_X + m_ButtonStatus->X, m_Y + m_ButtonStatus->Y, 14, 14))
		selected = m_ButtonStatus;
	else if (m_ButtonUse != NULL)
	{
		if (g_Orion.GumpPixelsInXY(m_ButtonUse->Graphic, m_X + m_ButtonUse->X, m_Y + m_ButtonUse->Y))
			selected = m_ButtonUse;
	}

	return selected;
}
//----------------------------------------------------------------------------------
void CGUISkillItem::Draw(const bool &checktrans)
{
	glTranslatef((GLfloat)m_X, (GLfloat)m_Y, 0.0f);

	if (m_ButtonUse != NULL)
		m_ButtonUse->Draw(checktrans);

	if (g_PressedObject.LeftObject() == this)
		g_GL.DrawPolygone(20, 0, 250, 14);

	m_NameText.Draw(22, 0, checktrans);

	m_ButtonStatus->Draw(checktrans);

	m_ValueText.Draw(250 - m_ValueText.Width, 0);

	glTranslatef((GLfloat)-m_X, (GLfloat)-m_Y, 0.0f);
}
//----------------------------------------------------------------------------------
bool CGUISkillItem::Select()
{
	int x = g_MouseManager.Position.X - m_X;
	int y = g_MouseManager.Position.Y - m_Y;

	return (x >= 0 && y >= 0 && x < 255 && y < 17);
}
//----------------------------------------------------------------------------------
