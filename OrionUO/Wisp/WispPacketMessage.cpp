// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//----------------------------------------------------------------------------------
#include "stdafx.h"

namespace WISP_NETWORK
{
//----------------------------------------------------------------------------------
CPacketMessage::CPacketMessage(const bool &bigEndian)
: m_BigEndian(bigEndian)
{
}
//----------------------------------------------------------------------------------
CPacketMessage::CPacketMessage(puchar data, const int &dataSize, const bool &bigEndian)
: m_BigEndian(bigEndian)
{
	WISPFUN_DEBUG("c9_f1");
	m_Data.resize(dataSize);
	memcpy(&m_Data[0], &data[0], dataSize);
}
//----------------------------------------------------------------------------------
CPacketMessage::CPacketMessage(const UCHAR_LIST &data, const bool &bigEndian)
: m_BigEndian(bigEndian), m_Data(data)
{
	WISPFUN_DEBUG("c9_f2");
}
//----------------------------------------------------------------------------------
CPacketMessage::~CPacketMessage()
{
	WISPFUN_DEBUG("c9_f3");
	m_Data.clear();
}
//----------------------------------------------------------------------------------
void CPacketMessage::Append(puchar data, const int &dataSize)
{
	WISPFUN_DEBUG("c9_f4");
	UCHAR_LIST buf(dataSize);
	memcpy(&buf[0], &data[0], dataSize);

	m_Data.insert(m_Data.end(), buf.begin(), buf.end());
}
//----------------------------------------------------------------------------------
void CPacketMessage::Append(const UCHAR_LIST &data)
{
	WISPFUN_DEBUG("c9_f5");
	m_Data.insert(m_Data.end(), data.begin(), data.end());
}
//----------------------------------------------------------------------------------
UCHAR_LIST CPacketMessage::Read(class CPacketReader *reader, int &dataOffset)
{
	WISPFUN_DEBUG("c9_f6");
	UCHAR_LIST result;

	if (!m_Data.size())
		return result;

	int offsetToSize = 0;
	int wantSize = reader->GetPacketSize(m_Data, offsetToSize);

	if (wantSize == PACKET_VARIABLE_SIZE)
	{
		if (m_Data.size() < 3)
			return result;

		puchar data = &m_Data[1];

		if (m_BigEndian)
			wantSize = (data[0] << 8) | data[1];
		else
			wantSize = (data[1] << 8) | data[0];

		dataOffset = offsetToSize + 2;
	}
	else
		dataOffset = offsetToSize;

	if ((int)m_Data.size() < wantSize)
		return result;

	result.insert(result.begin(), m_Data.begin(), m_Data.begin() + wantSize);

	m_Data.erase(m_Data.begin(), m_Data.begin() + wantSize);

	return result;
}
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
