/****************************************************************************
**
** GameObject.h
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
#ifndef GameCharacterH
#define GameCharacterH
//--------------------------------------------------------------------------
class TGameCharacter: public TGameObject
{
//private:
protected:
	short m_Hits; //Текущее значение ХП
	short m_MaxHits; //Максимальное значение ХП
	short m_Mana; //Текущее значение маны
	short m_MaxMana; //Максимальное значение маны
	short m_Stam; //Текущее значение стамины
	short m_MaxStam; //Максимальное значение стамины
	bool m_Sex; //Пол
	BYTE m_Race; //Расса
	BYTE m_Direction; //Направление
	BYTE m_Notoriety; //Злобность
	bool m_CanChangeName; //Возможность изменить имя в статус баре
	BYTE m_AnimationGroup; //Группа анимации
	BYTE m_AnimationInterval; //Задержка смены кадров
	BYTE m_AnimationFrameCount; //Количество кадров
	BYTE m_AnimationRepeatMode; //Метод воспроизведения анимации
	bool m_AnimationRepeat; //Повторять анимацию?
	bool m_AnimationDirection; //Направление проигрывания анимации
	bool m_AnimationFromServer; //Если анимация пришла с сервера
	DWORD m_LastStepSoundTime; //Время последнего проигрывания звука для хотьбы
	DWORD m_TimeToRandomFidget; //Время для установки случайной анимации
	DWORD m_CorpseLink; //Ссылка на труп (для анимации трупа)
	
	char m_OffsetX; //Смещение картинки по оси X
	char m_OffsetY; //Смещение картинки по оси Y
	char m_OffsetZ; //Смещение картинки по оси Z
	DWORD m_LastStepTime; //Время последнего шага
	short m_AfterStepDelay; //Задержка после шага (для корректного отображения анимации хотьбы)
	BYTE m_StepSoundOffset; //Смещение для звука шага

	string m_PaperdollText; //Текст титула в папердолле

	bool IsCorrectStep(WORD cx, WORD cy, WORD &x, WORD &y, BYTE &dir);
public:
	TGameCharacter(DWORD serial = 0);
	virtual ~TGameCharacter();
	
	TWalkStack m_WalkStack; //Ссылка на счетчик шагов
	
	SETGETEX(short, Hits);
	SETGETEX(short, MaxHits);
	SETGETEX(short, MaxMana);
	SETGETEX(short, MaxStam);
	SETGETEX(short, Mana);
	SETGETEX(short, Stam);
	SETGET(bool, Sex);
	SETGET(BYTE, Race);
	SETGET(BYTE, Direction);
	SETGET(BYTE, Notoriety);
	SETGET(bool, CanChangeName);
	SETGET(short, AfterStepDelay);
	SETGET(BYTE, AnimationInterval);
	SETGET(BYTE, AnimationFrameCount);
	SETGET(BYTE, AnimationRepeatMode);
	SETGET(bool, AnimationRepeat);
	SETGET(bool, AnimationDirection);
	SETGET(bool, AnimationFromServer);
	SETGET(DWORD, LastStepSoundTime);
	SETGET(DWORD, TimeToRandomFidget);
	SETGET(BYTE, StepSoundOffset);
	SETGET(DWORD, CorpseLink);

	SETGETEX(char, OffsetX);
	SETGETEX(char, OffsetY);
	SETGETEX(char, OffsetZ);
	SETGET(DWORD, LastStepTime);

	virtual int Draw(bool &mode, int &drawX, int &drawY, DWORD &ticks);

	void OnGraphicChange(int direction = 0); //Обновить информацию о поле персонажа

	void SetPaperdollText(string val); //Сгенерировать текстуру текста папердолла
	void SetAnimationGroup(BYTE val); //Установка группы анимации
	void SetRandomFidgetAnimation(); //Установка случайной анимации (при длительном простое)
	//Установка анимации
	void SetAnimation(BYTE id, BYTE interval = 0, BYTE frameCount = 0, BYTE repeatCount = 0, bool repeat = false, bool frameDirection = false);

	WORD GetMountAnimation(); //Получить индекс картинки для вычисления картинки анимации

	TTextTexture m_PaperdollTextTexture; //Текстура текста папердолла

	string GetPaperdollText() const {return m_PaperdollText;}
	BYTE GetAnimationGroup(WORD graphic = 0); //Получить текущую группу анимации
	void GetAnimationGroup(ANIMATION_GROUPS group, BYTE &animation); //Скорректировать отношение анимаций
	bool Staying() {return m_AnimationGroup == 0xFF && m_WalkStack.Empty();} //Если персонаж не движется
	bool Walking() {return (m_LastStepTime > (DWORD)(GetTickCount() - m_AfterStepDelay));} //Если персонаж идет (или только что закончил передвигаться)
	void UpdateAnimationInfo(BYTE &dir, bool canChange = false); //Обновить характеристики анимации персонажа
	bool IsTeleportAction(WORD &x, WORD &y, BYTE &dir);

	//Человек ли это
	bool IsHuman() {return (((Graphic >= 0x0190) && (Graphic <= 0x0193)) || (Graphic == 0x03DB));}
	bool Dead() {return ((Graphic == 0x0192) || (Graphic == 0x0193));} //Объект мертв
	
	TGameItem *FindLayer(int layer); //Найти объект в контейнере на указанном слое
};
//---------------------------------------------------------------------------
#endif