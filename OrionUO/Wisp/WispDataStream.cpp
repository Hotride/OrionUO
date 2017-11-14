// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//----------------------------------------------------------------------------------
#include "stdafx.h"

namespace WISP_DATASTREAM
{
//----------------------------------------------------------------------------------
//------------------------------------CDataWritter----------------------------------
//----------------------------------------------------------------------------------
CDataWritter::CDataWritter()
{
}
//----------------------------------------------------------------------------------
CDataWritter::CDataWritter(const size_t &size, const bool &autoResize)
: m_AutoResize(autoResize)
{
	WISPFUN_DEBUG("c4_f1");
	m_Data.resize(size, 0);
	m_Ptr = &m_Data[0];
}
//----------------------------------------------------------------------------------
CDataWritter::~CDataWritter()
{
	WISPFUN_DEBUG("c4_f2");
	m_Data.clear();
	m_Ptr = NULL;
}
//----------------------------------------------------------------------------------
void CDataWritter::Resize(const size_t &newSize, const bool &resetPtr)
{
	WISPFUN_DEBUG("c4_f3");
	m_Data.resize(newSize, 0);

	if (resetPtr)
		m_Ptr = &m_Data[0];
}
//----------------------------------------------------------------------------------
void CDataWritter::Move(const intptr_t &offset)
{
	WISPFUN_DEBUG("c4_f4");
	if (m_AutoResize)
	{
		IFOR(i, offset, 0)
			m_Data.push_back(0);
	}
	else if (m_Ptr != NULL)
		m_Ptr += offset;
}
//----------------------------------------------------------------------------------
void CDataWritter::WriteDataBE(const puchar data, const size_t &size, const intptr_t &offset)
{
	WISPFUN_DEBUG("c4_f5");
	if (m_AutoResize)
	{
		DFOR(i, size - 1, 0)
			m_Data.push_back(data[i]);
	}
	else if (m_Ptr != NULL)
	{
		puchar ptr = m_Ptr + offset + size - 1;

		DFOR(i, size - 1, 0)
			*(ptr - i) = data[i];

		m_Ptr += size;
	}
}
//----------------------------------------------------------------------------------
void CDataWritter::WriteDataLE(const puchar data, const size_t &size, const intptr_t &offset)
{
	WISPFUN_DEBUG("c4_f6");
	if (m_AutoResize)
	{
		IFOR(i, 0, size)
			m_Data.push_back(data[i]);
	}
	else if (m_Ptr != NULL)
	{
		puchar ptr = m_Ptr + offset;

		IFOR(i, 0, size)
			ptr[i] = data[i];

		m_Ptr += size;
	}
}
//----------------------------------------------------------------------------------
void CDataWritter::WriteString(const string &val, size_t length, const bool &nullTerminated, const intptr_t &offset)
{
	WISPFUN_DEBUG("c4_f7");
	if (!length)
		length = val.length();

	if (length <= val.length())
		WriteDataLE((puchar)val.c_str(), length, offset);
	else
	{
		WriteDataLE((puchar)val.c_str(), val.length(), offset);
		Move(length - val.length());
	}

	if (nullTerminated)
	{
		uchar nullTerminator = 0;
		WriteDataBE((puchar)&nullTerminator, sizeof(uchar), offset);
	}
}
//----------------------------------------------------------------------------------
void CDataWritter::WriteWString(const wstring &val, size_t length, const bool &bigEndian, const bool &nullTerminated, const intptr_t &offset)
{
	WISPFUN_DEBUG("c4_f8");
	if (!length)
		length = val.length();

	m_Ptr += offset;
	size_t size = val.length();

	if (length <= size)
	{
		if (bigEndian)
		{
			IFOR(i, 0, length)
				WriteInt16BE(val[i]);
		}
		else
		{
			IFOR(i, 0, length)
				WriteInt16LE(val[i]);
		}
	}
	else
	{
		if (bigEndian)
		{
			IFOR(i, 0, size)
				WriteInt16BE(val[i]);
		}
		else
		{
			IFOR(i, 0, size)
				WriteInt16LE(val[i]);
		}

		Move((length - size) * 2);
	}

	if (nullTerminated)
	{
		ushort nullTerminator = 0;
		WriteDataBE((puchar)&nullTerminator, sizeof(ushort), offset);
	}

	m_Ptr -= offset;
}
//----------------------------------------------------------------------------------
//------------------------------------CDataReader-----------------------------------
//----------------------------------------------------------------------------------
CDataReader::CDataReader()
{
}
//----------------------------------------------------------------------------------
CDataReader::CDataReader(puchar start, const size_t &size)
: m_Start(start), m_Size(size), m_End(m_Start + size)
{
	WISPFUN_DEBUG("c5_f1");
	m_Ptr = m_Start;
}
//----------------------------------------------------------------------------------
CDataReader::~CDataReader()
{
	WISPFUN_DEBUG("c5_f2");
	m_Start = NULL;
	m_Size = 0;
	m_End = NULL;
	m_Ptr = NULL;
}
//----------------------------------------------------------------------------------
void CDataReader::SetData(puchar start, const size_t &size, const intptr_t &offset)
{
	WISPFUN_DEBUG("c5_f3");
	m_Start = start;
	m_Size = size;
	m_End = m_Start + size;
	m_Ptr = m_Start + offset;
}
//----------------------------------------------------------------------------------
void CDataReader::ReadDataBE(puchar data, const size_t &size, const intptr_t &offset)
{
	WISPFUN_DEBUG("c5_f4");
	if (m_Ptr != NULL)
	{
		puchar ptr = m_Ptr + offset + size - 1;

		if (ptr >= m_Start && ptr <= m_End)
		{
			IFOR(i, 0, size)
				data[i] = *(ptr - i);

			m_Ptr += size;
		}
	}
}
//----------------------------------------------------------------------------------
void CDataReader::ReadDataLE(puchar data, const size_t &size, const intptr_t &offset)
{
	WISPFUN_DEBUG("c5_f5");
	if (m_Ptr != NULL)
	{
		puchar ptr = m_Ptr + offset;

		if (ptr >= m_Start && ptr + size <= m_End)
		{
			IFOR(i, 0, size)
				data[i] = ptr[i];

			m_Ptr += size;
		}
	}
}
//----------------------------------------------------------------------------------
string CDataReader::ReadString(size_t size, const intptr_t &offset)
{
	WISPFUN_DEBUG("c5_f6");
	puchar ptr = m_Ptr + offset;

	if (!size)
	{
		if (ptr >= m_Start && ptr <= m_End)
		{
			puchar buf = ptr;

			while (buf <= m_End && *buf)
				buf++;

			size = (buf - ptr) + 1;
		}
	}

	string result = "";

	if (ptr >= m_Start && ptr + size <= m_End)
	{
		result.resize(size, 0);
		ReadDataLE((puchar)&result[0], size, offset);
	}

	return result.c_str();
}
//----------------------------------------------------------------------------------
wstring CDataReader::ReadWString(size_t size, const bool &bigEndian, const intptr_t &offset)
{
	WISPFUN_DEBUG("c5_f7");
	puchar ptr = m_Ptr + offset;

	if (!size)
	{
		if (ptr >= m_Start && ptr <= m_End)
		{
			puchar buf = ptr;

			while (buf <= m_End)
			{
				ushort val = (bigEndian ? ((buf[0] << 8) | buf[1]) : *(pushort)buf);

				buf += 2;

				if (!val)
					break;
			}

			size = ((buf - ptr) / 2);
		}
	}

	wstring result = L"";

	if (ptr >= m_Start && ptr + size <= m_End)
	{
		result.resize(size, 0);

		if (bigEndian)
		{
			IFOR(i, 0, size)
				result[i] = ReadInt16BE(offset);
		}
		else
		{
			IFOR(i, 0, size)
				result[i] = ReadInt16LE(offset);
		}
	}

	return result.c_str();
}
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
