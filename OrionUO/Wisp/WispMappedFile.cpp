// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//----------------------------------------------------------------------------------
#include "stdafx.h"

namespace WISP_FILE
{
//----------------------------------------------------------------------------------
string g_WispMappedFileError = "";
//----------------------------------------------------------------------------------
CMappedFile::CMappedFile()
: WISP_DATASTREAM::CDataReader()
{
}
//----------------------------------------------------------------------------------
CMappedFile::~CMappedFile()
{
	Unload();
}
//----------------------------------------------------------------------------------
bool CMappedFile::Load()
{
	WISPFUN_DEBUG("c7_f1");
	bool result = false;

	Size = GetFileSize(m_File, NULL);

	if (Size > 0)
	{
		m_Map = CreateFileMapping(m_File, NULL, 2, 0, NULL, NULL);

		if (m_Map != NULL)
		{
			Start = (puchar)MapViewOfFile(m_Map, FILE_MAP_READ, 0, 0, Size);

			result = (Start != NULL);

			if (!result)
			{
				CloseHandle(m_Map);
				CloseHandle(m_File);
				m_Map = NULL;
				m_File = INVALID_HANDLE_VALUE;
			}
			else
				SetData(Start, Size);
		}
		else
		{
			CloseHandle(m_File);
			m_File = INVALID_HANDLE_VALUE;
		}
	}
	else
	{
		CloseHandle(m_File);
		m_File = INVALID_HANDLE_VALUE;
	}

	return result;
}
//----------------------------------------------------------------------------------
bool CMappedFile::Load(const string &path)
{
	WISPFUN_DEBUG("c7_f2");
	LOG("Mmaping  %s\n", path.c_str());
	bool result = false;

	if (PathFileExistsA(path.c_str()))
	{
		Unload();

		m_File = CreateFileA(path.c_str(), GENERIC_READ, 1, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (m_File != INVALID_HANDLE_VALUE)
			result = Load();
		else
			LOG("INVALID_HANDLE_VALUE for CreateFileA  %s\n", path.c_str());
	}
	else
		LOG("File not found %s\n", path.c_str());

	if (!result)
	{
		DWORD errorCode = GetLastError();
		LOG("Failed to memory map, error code: %i\n", errorCode);
		g_WispMappedFileError = path;
	}
		

	return result;
}
//----------------------------------------------------------------------------------
bool CMappedFile::Load(const wstring &path)
{
	WISPFUN_DEBUG("c7_f3");
	bool result = false;

	if (PathFileExistsW(path.c_str()))
	{
		Unload();

		m_File = CreateFileW(path.c_str(), GENERIC_READ, 1, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (m_File != INVALID_HANDLE_VALUE)
			result = Load();
	}

	if (!result)
		g_WispMappedFileError = ToString(path);

	return result;
}
//----------------------------------------------------------------------------------
void CMappedFile::Unload()
{
	WISPFUN_DEBUG("c7_f4");
	if (Start != NULL)
		UnmapViewOfFile(Start);

	if (m_Map != NULL)
	{
		CloseHandle(m_Map);
		m_Map = 0;
	}

	if (m_File != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_File);
		m_File = INVALID_HANDLE_VALUE;
	}

	SetData(NULL, 0);
}
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
