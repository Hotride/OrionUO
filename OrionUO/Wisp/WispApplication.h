//----------------------------------------------------------------------------------
#ifndef WISPAPPLICATION_H
#define WISPAPPLICATION_H
//----------------------------------------------------------------------------------
#include "FileSystem.h"
//----------------------------------------------------------------------------------
namespace WISP_APPLICATION
{
//----------------------------------------------------------------------------------
class CApplication
{
public:
	HINSTANCE Hinstance = 0;
	os_path m_ExePath;
	os_path m_UOPath;

protected:
	virtual void OnMainLoop() {}

public:
	CApplication();
	virtual ~CApplication();

	int Run(HINSTANCE hinstance);

	string GetFileVersion(uint *numericVerion = nullptr) const;
	os_path ExeFilePath(const char *str, ...) const;
	os_path UOFilesPath(const char *str, ...) const;
	os_path UOFilesPath(const string &str, ...) const;
};
//----------------------------------------------------------------------------------
extern CApplication *g_WispApplication;
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
#endif

