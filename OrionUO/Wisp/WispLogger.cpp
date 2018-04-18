// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//----------------------------------------------------------------------------------
#include "stdafx.h"
#include "FileSystem.h"
//----------------------------------------------------------------------------------
namespace WISP_LOGGER
{
//----------------------------------------------------------------------------------
CLogger g_WispLogger;
CLogger g_WispCrashLogger;
//----------------------------------------------------------------------------------
CLogger::CLogger()
{
}
//----------------------------------------------------------------------------------
CLogger::~CLogger()
{
	Close();
}
//----------------------------------------------------------------------------------
void CLogger::Close()
{
	if (m_File != nullptr)
	{
		LOG("Log closed.\n");
		fs_close(m_File);
		m_File = nullptr;
	}
}
//----------------------------------------------------------------------------------
void CLogger::Init(const os_path &filePath)
{
	if (m_File != nullptr)
		fs_close(m_File);

	m_File = fs_open(filePath, FS_WRITE);

	if (this == &g_WispLogger)
		LOG("Log opened.\n");

	FileName = filePath;
}
//----------------------------------------------------------------------------------
void CLogger::Print(const char *format, ...)
{
	if (m_File == nullptr)
		return;

	va_list arg;
	va_start(arg, format);
	vfprintf(m_File, format, arg);
	va_end(arg);
	fflush(m_File);
}
//----------------------------------------------------------------------------------
void CLogger::VPrint(const char *format, va_list ap)
{
	if (m_File == nullptr)
		return;

	vfprintf(m_File, format, ap);
	fflush(m_File);
}
//----------------------------------------------------------------------------------
void CLogger::Print(const wchar_t *format, ...)
{
	if (m_File == nullptr)
		return;

	va_list arg;
	va_start(arg, format);
	vfwprintf(m_File, format, arg);
	va_end(arg);
	fflush(m_File);
}
//----------------------------------------------------------------------------------
void CLogger::VPrint(const wchar_t *format, va_list ap)
{
	if (m_File == nullptr)
		return;

	vfwprintf(m_File, format, ap);
	fflush(m_File);
}
//----------------------------------------------------------------------------------
void CLogger::Dump(uchar *buf, int size)
{
	if (m_File == nullptr)
		return;

	int num_lines = size / 16;

	if (size % 16 != 0)
		num_lines++;

	for (int line = 0; line < num_lines; line++)
	{
		int row = 0;
		fprintf(m_File, "%04X: ", line * 16);

		for (row = 0; row < 16; row++)
		{
			if (line * 16 + row < size)
				fprintf(m_File, "%02X ", buf[line * 16 + row]);
			else
				fprintf(m_File, "-- ");
		}

		fprintf(m_File, ": ");

		for (row = 0; row < 16; row++)
		{
			if (line * 16 + row < size)
				fputc(isprint(buf[line * 16 + row]) ? buf[line * 16 + row] : '.', m_File);
		}

		fputc('\n', m_File);
	}

	fflush(m_File);
}
//----------------------------------------------------------------------------------
};
//----------------------------------------------------------------------------------
