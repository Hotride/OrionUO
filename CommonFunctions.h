/****************************************************************************
**
** CommonFunctions.h
**
** Copyright (C) February 2016 Hotride
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
#ifndef CommonFunctionsH
#define CommonFunctionsH
//---------------------------------------------------------------------------
const char *GetReagentName(WORD ID);
//---------------------------------------------------------------------------
int CalculateSphereOffset(int max, int current, int maxValue, float divizor = 100.0f);
int CalculatePercents(int max, int current, int maxValue);
//---------------------------------------------------------------------------
void TileOffsetOnMonitorToXY(int &ofsX, int &ofsY, int &x, int &y);
//---------------------------------------------------------------------------
void UnuseShader();
//---------------------------------------------------------------------------
string FilePath(const string &fName);
string EncodeUTF8(const string &fName);
string ToString(const wstring &wstr);
wstring ToWString(const string &str);
string ToLowerA(string str);
string ToUpperA(string str);
bool ToBool(string &str);
//---------------------------------------------------------------------------
//Функции для вычисления дистанции
class TGameObject;
int GetDistance(TGameObject *current, TGameObject *target);
int GetDistance(TGameObject *current, POINT target);
int GetMultiDistance(POINT current, TGameObject *target);
int GetDistance(POINT current, TGameObject *target);
int GetDistance(POINT current, POINT target);
int GetTopObjDistance(TGameObject *current, TGameObject *target);
//---------------------------------------------------------------------------
int gumpuucode2str(const wchar_t *wstr, int wlength, char *receiver, int maxsize);
int uucode2str(const wchar_t *wstr, int wlength, char *receiver, int maxsize);
int str2uucode(const char *str, int length, wchar_t* wreceiver, int wmaxsize);
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
/*!
Перевод радианов в градусы
@param [__in] degr Радианы
@return Градусы
*/
inline float deg2radf( __in float degr)
{
	return degr * (float)(M_PI / 180.0f);
}
//---------------------------------------------------------------------------
inline float rad2degf(float radians)
{
	return (float)(radians * 180.0f / M_PI);
}
//---------------------------------------------------------------------------
inline int RandomInt(int n)
{
	return (rand() % n);
}
//---------------------------------------------------------------------------
inline int RandomIntMinMax(int n, int m)
{
	return (rand() % (m - n) + n);
}
//---------------------------------------------------------------------------
inline DWORD unpack32(PBYTE buf)
{
	return (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
}
//---------------------------------------------------------------------------
inline WORD unpack16(PBYTE buf)
{
	return (buf[0] << 8) | buf[1];
}
//---------------------------------------------------------------------------
inline void pack32(PBYTE buf, DWORD x)
{
	buf[0] = BYTE(x >> 24);
	buf[1] = BYTE((x >> 16) & 0xff);
	buf[2] = BYTE((x >> 8) & 0xff);
	buf[3] = BYTE(x & 0xff);
}
//---------------------------------------------------------------------------
inline void pack16(PBYTE buf, WORD x)
{
	buf[0] = x >> 8;
	buf[1] = x & 0xff;
}
//---------------------------------------------------------------------------
inline bool IsBackground(DWORD Flags) { return (Flags & 0x00000001); }
inline bool IsWeapon(DWORD Flags) { return (Flags & 0x00000002); }
inline bool IsTransparent(DWORD Flags) { return (Flags & 0x00000004); }
inline bool IsTranslucent(DWORD Flags) { return (Flags & 0x00000008); }
inline bool IsWall(DWORD Flags) { return (Flags & 0x00000010); }
inline bool IsDamaging(DWORD Flags) { return (Flags & 0x00000020); }
inline bool IsImpassable(DWORD Flags) { return (Flags & 0x00000040); }
inline bool IsWet(DWORD Flags) { return (Flags & 0x00000080); }
inline bool IsUnknown(DWORD Flags) { return (Flags & 0x00000100); }
inline bool IsSurface(DWORD Flags) { return (Flags & 0x00000200); }
inline bool IsBridge(DWORD Flags) { return (Flags & 0x00000400); }
inline bool IsStackable(DWORD Flags) { return (Flags & 0x00000800); }
inline bool IsWindow(DWORD Flags) { return (Flags & 0x00001000); }
inline bool IsNoShoot(DWORD Flags) { return (Flags & 0x00002000); }
inline bool IsPrefixA(DWORD Flags) { return (Flags & 0x00004000); }
inline bool IsPrefixAn(DWORD Flags) { return (Flags & 0x00008000); }
inline bool IsInternal(DWORD Flags) { return (Flags & 0x00010000); }
inline bool IsFoliage(DWORD Flags) { return (Flags & 0x00020000); }
inline bool IsPartialHue(DWORD Flags) { return (Flags & 0x00040000); }
inline bool IsUnknown1(DWORD Flags) { return (Flags & 0x00080000); }
inline bool IsMap(DWORD Flags) { return (Flags & 0x00100000); }
inline bool IsContainer(DWORD Flags) { return (Flags & 0x00200000); }
inline bool IsWearable(DWORD Flags) { return (Flags & 0x00400000); }
inline bool IsLightSource(DWORD Flags) { return (Flags & 0x00800000); }
inline bool IsAnimated(DWORD Flags) { return (Flags & 0x01000000); }
inline bool IsNoDiagonal(DWORD Flags) { return (Flags & 0x02000000); }
inline bool IsUnknown2(DWORD Flags) { return (Flags & 0x04000000); }
inline bool IsArmor(DWORD Flags) { return (Flags & 0x08000000); }
inline bool IsRoof(DWORD Flags) { return (Flags & 0x10000000); }
inline bool IsDoor(DWORD Flags) { return (Flags & 0x20000000); }
inline bool IsStairBack(DWORD Flags) { return (Flags & 0x40000000); }
inline bool IsStairRight(DWORD Flags) { return (Flags & 0x80000000); }
//---------------------------------------------------------------------------
#endif