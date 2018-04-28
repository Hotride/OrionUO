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
CDataWritter::CDataWritter(size_t size, bool autoResize)
: AutoResize(autoResize)
{
	WISPFUN_DEBUG("c4_f1");
	m_Data.resize(size, 0);
	Ptr = &m_Data[0];
}
//----------------------------------------------------------------------------------
CDataWritter::~CDataWritter()
{
	WISPFUN_DEBUG("c4_f2");
	m_Data.clear();
	Ptr = NULL;
}
//----------------------------------------------------------------------------------
void CDataWritter::Resize(size_t newSize, bool resetPtr)
{
	WISPFUN_DEBUG("c4_f3");
	m_Data.resize(newSize, 0);

	if (resetPtr)
		Ptr = &m_Data[0];
}
//----------------------------------------------------------------------------------
void CDataWritter::Move(const intptr_t &offset)
{
	WISPFUN_DEBUG("c4_f4");
	if (AutoResize)
	{
		IFOR(i, offset, 0)
			m_Data.push_back(0);
	}
	else if (Ptr != NULL)
		Ptr += offset;
}
//----------------------------------------------------------------------------------
void CDataWritter::WriteDataBE(const puchar data, size_t size, const intptr_t &offset)
{
	WISPFUN_DEBUG("c4_f5");
	if (AutoResize)
	{
		DFOR(i, size - 1, 0)
			m_Data.push_back(data[i]);
	}
	else if (Ptr != NULL)
	{
		puchar ptr = Ptr + offset + size - 1;

		DFOR(i, size - 1, 0)
			*(ptr - i) = data[i];

		Ptr += size;
	}
}
//----------------------------------------------------------------------------------
void CDataWritter::WriteDataLE(const puchar data, size_t size, const intptr_t &offset)
{
	WISPFUN_DEBUG("c4_f6");
	if (AutoResize)
	{
		IFOR(i, 0, size)
			m_Data.push_back(data[i]);
	}
	else if (Ptr != NULL)
	{
		puchar ptr = Ptr + offset;

		IFOR(i, 0, size)
			ptr[i] = data[i];

		Ptr += size;
	}
}
//----------------------------------------------------------------------------------
void CDataWritter::WriteString(const string &val, size_t length, bool nullTerminated, const intptr_t &offset)
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
void CDataWritter::WriteWString(const wstring &val, size_t length, bool bigEndian, bool nullTerminated, const intptr_t &offset)
{
	WISPFUN_DEBUG("c4_f8");
	if (!length)
		length = val.length();

	Ptr += offset;
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

	Ptr -= offset;
}
//----------------------------------------------------------------------------------
//------------------------------------CDataReader-----------------------------------
//----------------------------------------------------------------------------------
CDataReader::CDataReader()
{
}
//----------------------------------------------------------------------------------
CDataReader::CDataReader(puchar start, size_t size)
: Start(start), Size(size), End(Start + size)
{
	WISPFUN_DEBUG("c5_f1");
	Ptr = Start;
}
//----------------------------------------------------------------------------------
CDataReader::~CDataReader()
{
	WISPFUN_DEBUG("c5_f2");
	Start = NULL;
	Size = 0;
	End = NULL;
	Ptr = NULL;
}
//----------------------------------------------------------------------------------
void CDataReader::SetData(puchar start, size_t size, const intptr_t &offset)
{
	WISPFUN_DEBUG("c5_f3");
	Start = start;
	Size = size;
	End = Start + size;
	Ptr = Start + offset;
}
//----------------------------------------------------------------------------------
void CDataReader::ReadDataBE(puchar data, size_t size, const intptr_t &offset)
{
	WISPFUN_DEBUG("c5_f4");
	if (Ptr != NULL)
	{
		puchar ptr = Ptr + offset + size - 1;

		if (ptr >= Start && ptr <= End)
		{
			IFOR(i, 0, size)
				data[i] = *(ptr - i);

			Ptr += size;
		}
	}
}
//----------------------------------------------------------------------------------
void CDataReader::ReadDataLE(puchar data, size_t size, const intptr_t &offset)
{
	WISPFUN_DEBUG("c5_f5");
	if (Ptr != NULL)
	{
		puchar ptr = Ptr + offset;

		if (ptr >= Start && ptr + size <= End)
		{
			IFOR(i, 0, size)
				data[i] = ptr[i];

			Ptr += size;
		}
	}
}
//----------------------------------------------------------------------------------
string CDataReader::ReadString(size_t size, const intptr_t &offset)
{
	WISPFUN_DEBUG("c5_f6");
	puchar ptr = Ptr + offset;

	if (!size)
	{
		if (ptr >= Start && ptr <= End)
		{
			puchar buf = ptr;

			while (buf <= End && *buf)
				buf++;

			size = (buf - ptr) + 1;
		}
	}

	string result = "";

	if (ptr >= Start && ptr + size <= End)
	{
		result.resize(size, 0);
		ReadDataLE((puchar)&result[0], size, offset);
	}

	return result.c_str();
}
//----------------------------------------------------------------------------------
wstring CDataReader::ReadWString(size_t size, bool bigEndian, const intptr_t &offset)
{
	WISPFUN_DEBUG("c5_f7");
	puchar ptr = Ptr + offset;

	if (!size)
	{
		if (ptr >= Start && ptr <= End)
		{
			puchar buf = ptr;

			while (buf <= End)
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

	if (ptr >= Start && ptr + size <= End)
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
