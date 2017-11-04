// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//----------------------------------------------------------------------------------
#include "stdafx.h"

namespace WISP_FILE
{
//----------------------------------------------------------------------------------
CBinaryFileWritter::CBinaryFileWritter()
: WISP_DATASTREAM::CDataWritter()
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
	WISPFUN_DEBUG("c2_f1");
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
	WISPFUN_DEBUG("c2_f2");
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
	WISPFUN_DEBUG("c2_f3");
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
	WISPFUN_DEBUG("c2_f4");
	if (m_File != NULL && m_Data.size() > 0)
	{
		fwrite(&m_Data[0], m_Data.size(), 1, m_File);
		m_Data.clear();
	}
}
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
