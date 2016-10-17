//----------------------------------------------------------------------------------
#ifndef WISPBINARYFILEWRITTER_H
#define WISPBINARYFILEWRITTER_H
#include "WispDataStream.h"
//----------------------------------------------------------------------------------
namespace WISP_FILE
{
//----------------------------------------------------------------------------------
class CBinaryFileWritter : public WISP_DATASTREAM::CDataWritter
{
private:
	FILE *m_File;

public:
	CBinaryFileWritter();

	virtual ~CBinaryFileWritter();

	bool Open(const string &path);
	bool Open(const wstring &path);

	void Close();

	void WriteBuffer();
};
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
#endif

