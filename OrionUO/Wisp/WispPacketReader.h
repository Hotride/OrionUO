//----------------------------------------------------------------------------------
#ifndef WISPPACKETREADER_H
#define WISPPACKETREADER_H
#include "WispGlobal.h"
#include "WispDataStream.h"
#include "WispConnection.h"
//----------------------------------------------------------------------------------
namespace WISP_NETWORK
{
//----------------------------------------------------------------------------------
class CPacketReader : public WISP_DATASTREAM::CDataReader
{
public:
	CPacketReader();
	virtual ~CPacketReader();

	void Read(class CConnection *connection);

	virtual int GetPacketSize(const UCHAR_LIST &packet, int &offsetToSize) { return 0; }

protected:
	virtual void OnPacket() {}

	virtual void OnReadFailed() {}
};
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
#endif

