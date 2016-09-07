//----------------------------------------------------------------------------------
#include "WispMappedFile.h"
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

namespace WISP_FILE
{
//----------------------------------------------------------------------------------
string g_WispMappedFileError = "";
//----------------------------------------------------------------------------------
CMappedFile::CMappedFile()
: WISP_DATASTREAM::CDataReader(), m_File(INVALID_HANDLE_VALUE), m_Map(0)
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
	bool result = false;

	m_Size = GetFileSize(m_File, NULL);

	if (m_Size > 0)
	{
		m_Map = CreateFileMapping(m_File, NULL, 2, 0, NULL, NULL);

		if (m_Map != NULL)
		{
			m_Start = (puchar)MapViewOfFile(m_Map, FILE_MAP_READ, 0, 0, m_Size);

			result = (m_Start != NULL);

			if (!result)
			{
				CloseHandle(m_Map);
				CloseHandle(m_File);
				m_File = INVALID_HANDLE_VALUE;
			}
			else
				SetData(m_Start, m_Size);
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
	bool result = false;

	if (PathFileExistsA(path.c_str()))
	{
		Unload();

		m_File = CreateFileA(path.c_str(), GENERIC_READ, 1, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (m_File != INVALID_HANDLE_VALUE)
			result = Load();
	}

	if (!result)
		g_WispMappedFileError = path;

	return result;
}
//----------------------------------------------------------------------------------
bool CMappedFile::Load(const wstring &path)
{
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
	if (m_Start != NULL)
		UnmapViewOfFile(m_Start);

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
