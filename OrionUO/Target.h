/***********************************************************************************
**
** Target.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef TARGET_H
#define TARGET_H
//----------------------------------------------------------------------------------
//Класс для работы с таргетом
class CTarget
{
public:
	//Тип объекта прицела
	uchar Type = 0;
	//Тип прицела
	uchar CursorType = 0;
	//Мульти на таргете
	ushort MultiGraphic = 0;
	//Мульти на таргете X
	ushort MultiX = 0;
	//Мульти на таргете Y
	ushort MultiY = 0;
	//Серийник объекта, к которому привязан прицел
	uint CursorID = 0;
	//Флаг состояния прицела
	bool Targeting = false;

private:
	//Текущие и последний данные прицела
	uchar m_Data[19];
	uchar m_LastData[19];

	//Список объектов мульти на таргете
	CMulti *m_Multi{ NULL };

	//Добавить мульти-объект
	void AddMultiObject(CMultiObject *obj);

public:
	CTarget();
	~CTarget() {}

	//Послать таргет на объект
	void SetLastTargetObject(const uint &serial);

	//Установить данные прицела
	void SetData(WISP_DATASTREAM::CDataReader &reader);

	//Установить данные мульти-таргета
	void SetMultiData(WISP_DATASTREAM::CDataReader &reader);

	bool IsTargeting() const { return Targeting; }

	//Очистить таргет
	void Reset();

	void RequestFromCustomHouse();

	//Послать таргет на объект
	void SendTargetObject(const uint &Serial);

	//Послать таргет на тайл
	void SendTargetTile(const ushort &tileID, const short &x, const short &Y, char z);

	//Послать отмену таргета
	void SendCancelTarget();

	//Послать таргет на объект
	void Plugin_SendTargetObject(const uint &Serial);

	//Послать таргет на тайл
	void Plugin_SendTargetTile(const ushort &tileID, const short &x, const short &Y, char z);

	//Послать отмену таргета
	void Plugin_SendCancelTarget();

	//Послать таргет
	void Plugin_SendTarget();

	//Послать таргет на последнюю цель
	void SendLastTarget();

	//Послать таргет
	void SendTarget();

	//Загрузить мульти-объект
	void LoadMulti(const int &offsetX, const int &offsetY, const char &offsetZ);

	//Выгрузить мульти-объект
	void UnloadMulti();

	//Получить объект мульти в координатах
	CMulti *GetMultiAtXY(const short &x, const short &y);
};
//----------------------------------------------------------------------------------
extern CTarget g_Target;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
