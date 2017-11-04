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
CDataWritter::CDataWritter(const int &size, const bool &autoResize)
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
void CDataWritter::Resize(const int &newSize, const bool &resetPtr)
{
	WISPFUN_DEBUG("c4_f3");
	m_Data.resize(newSize, 0);

	if (resetPtr)
		m_Ptr = &m_Data[0];
}
//----------------------------------------------------------------------------------
void CDataWritter::Move(const int &offset)
{
	WISPFUN_DEBUG("c4_f4");
	if (m_AutoResize)
	{
		for (int i = 0; i < offset; i++)
			m_Data.push_back(0);
	}
	else if (m_Ptr != NULL)
		m_Ptr += offset;
}
//----------------------------------------------------------------------------------
void CDataWritter::WriteDataBE(const puchar data, const int &size, const int offset)
{
	WISPFUN_DEBUG("c4_f5");
	if (m_AutoResize)
	{
		for (int i = size - 1; i >= 0; i--)
			m_Data.push_back(data[i]);
	}
	else if (m_Ptr != NULL)
	{
		puchar ptr = m_Ptr + offset + size - 1;

		for (int i = size - 1; i >= 0; i--)
			*(ptr - i) = data[i];

		m_Ptr += size;
	}
}
//----------------------------------------------------------------------------------
void CDataWritter::WriteDataLE(const puchar data, const int &size, const int offset)
{
	WISPFUN_DEBUG("c4_f6");
	if (m_AutoResize)
	{
		for (int i = 0; i < size; i++)
			m_Data.push_back(data[i]);
	}
	else if (m_Ptr != NULL)
	{
		puchar ptr = m_Ptr + offset;

		for (int i = 0; i < size; i++)
			ptr[i] = data[i];

		m_Ptr += size;
	}
}
//----------------------------------------------------------------------------------
void CDataWritter::WriteString(const string &val, int length, const bool &nullTerminated, const int &offset)
{
	WISPFUN_DEBUG("c4_f7");
	if (!length)
		length = val.length();

	if (length <= (int)val.length())
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
void CDataWritter::WriteWString(const wstring &val, int length, const bool &bigEndian, const bool &nullTerminated, const int &offset)
{
	WISPFUN_DEBUG("c4_f8");
	if (!length)
		length = val.length();

	m_Ptr += offset;
	int size = (int)val.length();

	if (length <= size)
	{
		if (bigEndian)
		{
			for (int i = 0; i < length; i++)
				WriteInt16BE(val[i]);
		}
		else
		{
			for (int i = 0; i < length; i++)
				WriteInt16LE(val[i]);
		}
	}
	else
	{
		if (bigEndian)
		{
			for (int i = 0; i < size; i++)
				WriteInt16BE(val[i]);
		}
		else
		{
			for (int i = 0; i < size; i++)
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
CDataReader::CDataReader(puchar start, const int &size)
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
void CDataReader::SetData(puchar start, const int &size, const int &offset)
{
	WISPFUN_DEBUG("c5_f3");
	m_Start = start;
	m_Size = size;
	m_End = m_Start + size;
	m_Ptr = m_Start + offset;
}
//----------------------------------------------------------------------------------
void CDataReader::ReadDataBE(puchar data, const int &size, const int offset)
{
	WISPFUN_DEBUG("c5_f4");
	if (m_Ptr != NULL)
	{
		puchar ptr = m_Ptr + offset + size - 1;

		if (ptr >= m_Start && ptr <= m_End)
		{
			for (int i = 0; i < size; i++)
				data[i] = *(ptr - i);

			m_Ptr += size;
		}
	}
}
//----------------------------------------------------------------------------------
void CDataReader::ReadDataLE(puchar data, const int &size, const int offset)
{
	WISPFUN_DEBUG("c5_f5");
	if (m_Ptr != NULL)
	{
		puchar ptr = m_Ptr + offset;

		if (ptr >= m_Start && ptr + size <= m_End)
		{
			for (int i = 0; i < size; i++)
				data[i] = ptr[i];

			m_Ptr += size;
		}
	}
}
//----------------------------------------------------------------------------------
string CDataReader::ReadString(int size, const int &offset)
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
wstring CDataReader::ReadWString(int size, const bool &bigEndian, const int &offset)
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
			for (int i = 0; i < size; i++)
				result[i] = ReadInt16BE(offset);
		}
		else
		{
			for (int i = 0; i < size; i++)
				result[i] = ReadInt16LE(offset);
		}
	}

	return result.c_str();
}
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
