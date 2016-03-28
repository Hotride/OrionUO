//---------------------------------------------------------------------------
#include "stdafx.h"
#include "Logger.h"

#include <stdarg.h>
#include <locale>
//---------------------------------------------------------------------------
CLogger *g_Logger = NULL;
//---------------------------------------------------------------------------
CLogger::CLogger()
: m_file(NULL)
{
}
//---------------------------------------------------------------------------
CLogger::~CLogger()
{
	if (m_file != NULL)
	{
        TPRINT("Log closed.\n");
		fclose(m_file);
	}
}
//---------------------------------------------------------------------------
void CLogger::Init(const char *fName)
{
	if (m_file != NULL)
		fclose(m_file);

	fopen_s(&m_file, fName, "w");
	TPRINT("Log opened.\n");
}
//---------------------------------------------------------------------------
void CLogger::Print(const char *format, ...)
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
void CLogger::VPrint(const char *format, va_list ap)
{
	if (m_file == NULL)
		return;

	vfprintf(m_file, format, ap);
	fflush(m_file);
}
//---------------------------------------------------------------------------
void CLogger::Dump(unsigned char *buf, int len)
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
void error_printf(const char * format, ...)
{
	va_list arg;
	va_start(arg, format);
	g_Logger->Print("***Error: ");
	g_Logger->VPrint(format, arg);
	va_end(arg);
}
//---------------------------------------------------------------------------
void warning_printf(const char * format, ...)
{
	va_list arg;
	va_start(arg, format);
	g_Logger->Print("**Warning: ");
	g_Logger->VPrint(format, arg);
	va_end(arg);
}
//---------------------------------------------------------------------------
void trace_printf(const char * format, ...)
{
	va_list arg;
	va_start(arg, format);
	g_Logger->VPrint(format, arg);
	va_end(arg);
}
//---------------------------------------------------------------------------
void trace_dump(unsigned char * buf, int length)
{
	g_Logger->Dump(buf, length);
}
//---------------------------------------------------------------------------