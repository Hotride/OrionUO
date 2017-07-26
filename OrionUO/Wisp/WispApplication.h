//----------------------------------------------------------------------------------
#ifndef WISPAPPLICATION_H
#define WISPAPPLICATION_H
//----------------------------------------------------------------------------------
#include "WispGlobal.h"
//----------------------------------------------------------------------------------
namespace WISP_APPLICATION
{
//----------------------------------------------------------------------------------
class CApplication
{
	SETGET(HINSTANCE, Hinstance, 0);
	SETGET(string, ExePathA, "");
	SETGET(wstring, ExePathW, L"");

protected:
	virtual void OnIDLELoop() {}

public:
	CApplication();
	virtual ~CApplication();

	int Run(HINSTANCE hinstance);

	string GetFileVersion();

	string FilePath(const char *str, ...);
	wstring FilePath(const wchar_t *str, ...);
};
//----------------------------------------------------------------------------------
extern CApplication *g_WispApplication;
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
#endif

