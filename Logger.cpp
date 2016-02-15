/****************************************************************************
**
** Logger.cpp
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
TLogger *g_Logger = NULL;
//---------------------------------------------------------------------------
TLogger::TLogger()
: m_file(NULL)
{
}
//---------------------------------------------------------------------------
TLogger::~TLogger()
{
	if (m_file != NULL)
	{
		TPRINT("Log closed.\n");
		fclose(m_file);
	}
}
//---------------------------------------------------------------------------
void TLogger::Init(const char *fName)
{
	if (m_file != NULL)
		fclose(m_file);

	m_file = fopen(fName, "w");
	g_Logger = this;
}
//---------------------------------------------------------------------------
void TLogger::Print(const char *format, ...)
{
	if (m_file == NULL)
		return;

	va_list arg;
	va_start(arg, format);
	vfprintf(m_file, format, arg);
	va_end(arg);
	fflush(m_file);
}
//---------------------------------------------------------------------------
void TLogger::VPrint(const char *format, va_list ap)
{
	if (m_file == NULL)
		return;

	vfprintf(m_file, format, ap);
	fflush(m_file);
}
//---------------------------------------------------------------------------
void TLogger::Dump(PBYTE buf, int len)
{
	if (m_file == NULL)
		return;
	int num_lines = len / 16;
	if (len % 16 != 0)
		num_lines++;

	for (int line = 0; line < num_lines; line++)
	{
		int row = 0;
		fprintf(m_file, "%04x: ", line * 16);

		for (row = 0; row < 16; row++)
		{
			if (line * 16 + row < len)
				fprintf(m_file, "%02x ", buf[line * 16 + row]);
			else
				fprintf(m_file, "-- ");
		}

		fprintf(m_file, ": ");

		for (row = 0; row < 16; row++)
		{
			if (line * 16 + row < len)
				fputc(isprint(buf[line * 16 + row]) ? buf[line * 16 + row] : '.', m_file);
		}

		fputc('\n', m_file);
	}

	fflush(m_file);
}
//---------------------------------------------------------------------------