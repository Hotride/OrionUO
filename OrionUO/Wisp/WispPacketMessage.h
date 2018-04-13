//----------------------------------------------------------------------------------
#ifndef WISPPACKETMESSAGE_H
#define WISPPACKETMESSAGE_H
//----------------------------------------------------------------------------------
namespace WISP_NETWORK
{
//----------------------------------------------------------------------------------
class CPacketMessage
{
	bool BigEndian = false;

protected:
	UCHAR_LIST m_Data;

public:
	CPacketMessage(const bool &bigEndian = true);
	CPacketMessage(puchar data, const int &dataSizeconst, const bool &bigEndian = true);
	CPacketMessage(const UCHAR_LIST &dataconst, const bool &bigEndian = true);
	virtual ~CPacketMessage();

	void Append(puchar data, const int &dataSize);
	void Append(const UCHAR_LIST &data);

	void Clear() { m_Data.clear(); }

	UCHAR_LIST Read(class CPacketReader *reader, int &dataOffset);
};
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
#endif

