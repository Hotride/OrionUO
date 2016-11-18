//----------------------------------------------------------------------------------
#include "WispPacketReader.h"

namespace WISP_NETWORK
{
//----------------------------------------------------------------------------------
CPacketReader::CPacketReader()
:WISP_DATASTREAM::CDataReader()
{
}
//----------------------------------------------------------------------------------
CPacketReader::~CPacketReader()
{
}
//----------------------------------------------------------------------------------
void CPacketReader::Read(class CConnection *connection)
{
	if (connection->ReadyRead())
	{
		if (!connection->Read())
		{
			OnReadFailed();
			return;
		}

		WISP_NETWORK::CPacketMessage *parser = connection->m_MessageParser;

		while (true)
		{
			int offset = 0;
			UCHAR_LIST packet = parser->Read(this, offset);

			if (!packet.size())
				break;

			SetData((puchar)&packet[0], packet.size(), offset);
			OnPacket();
		}
	}
}
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
