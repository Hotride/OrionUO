//----------------------------------------------------------------------------------
#ifndef WISPMAPPEDFILE_H
#define WISPMAPPEDFILE_H
#include "WispDataStream.h"
//----------------------------------------------------------------------------------
namespace WISP_FILE
{
//----------------------------------------------------------------------------------
class CMappedFile : public WISP_DATASTREAM::CDataReader
{
	HANDLE m_File;
	HANDLE m_Map;

	bool Load();

public:
	CMappedFile();

	virtual ~CMappedFile();

	bool Load(const string &path);
	bool Load(const wstring &path);

	void Unload();
};
//----------------------------------------------------------------------------------
extern string g_WispMappedFileError;
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
#endif

