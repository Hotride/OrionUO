/****************************************************************************
**
** Logger.h
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
#ifndef LoggerH
#define LoggerH
//---------------------------------------------------------------------------
class TLogger
{
private:
	FILE *m_file;
public:
	TLogger();
	~TLogger();

	bool Ready() const {return m_file != NULL;}

	void Init(const char *fName);

	void Print(const char *format, ...);
	void VPrint(const char *format, va_list ap);
	void Dump(PBYTE buf, int len);
};

extern TLogger *g_Logger;
//---------------------------------------------------------------------------
#endif