//----------------------------------------------------------------------------------
#include "WispBinaryFileWritter.h"
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

namespace WISP_FILE
{
//----------------------------------------------------------------------------------
CBinaryFileWritter::CBinaryFileWritter()
: WISP_DATASTREAM::CDataWritter(), m_File(NULL)
{
}
//----------------------------------------------------------------------------------
CBinaryFileWritter::~CBinaryFileWritter()
{
	Close();
}
//----------------------------------------------------------------------------------
bool CBinaryFileWritter::Open(const string &path)
{
	bool result = false;

	//if (PathFileExistsA(path.c_str()))
	{
		fopen_s(&m_File, path.c_str(), "wb");
		result = (m_File != NULL);

	}

	return result;
}
//----------------------------------------------------------------------------------
bool CBinaryFileWritter::Open(const wstring &path)
{
	bool result = false;

	//if (PathFileExistsW(path.c_str()))
	{
		_wfopen_s(&m_File, path.c_str(), L"wb");
		result = (m_File != NULL);
	}

	return result;
}
//----------------------------------------------------------------------------------
void CBinaryFileWritter::Close()
{
	WriteBuffer();

	if (m_File != NULL)
	{
		fclose(m_File);
		m_File = NULL;
	}
}
//----------------------------------------------------------------------------------
void CBinaryFileWritter::WriteBuffer()
{
	if (m_File != NULL && m_Data.size() > 0)
	{
		fwrite(&m_Data[0], m_Data.size(), 1, m_File);
		m_Data.clear();
	}
}
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
