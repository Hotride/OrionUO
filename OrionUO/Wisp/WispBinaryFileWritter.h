//----------------------------------------------------------------------------------
#ifndef WISPBINARYFILEWRITTER_H
#define WISPBINARYFILEWRITTER_H
//----------------------------------------------------------------------------------
#include "FileSystem.h"
//----------------------------------------------------------------------------------
namespace WISP_FILE
{
//----------------------------------------------------------------------------------
class CBinaryFileWritter : public WISP_DATASTREAM::CDataWritter
{
private:
	FILE *m_File{ nullptr };

public:
	CBinaryFileWritter();

	virtual ~CBinaryFileWritter();

	bool Open(const os_path &path);

	void Close();

	void WriteBuffer();
};
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
#endif

