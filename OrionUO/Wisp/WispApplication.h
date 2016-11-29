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
	SETGET(HINSTANCE, Hinstance);
	SETGET(string, ExePathA);
	SETGET(wstring, ExePathW);

protected:
	virtual void OnIDLELoop() {}

public:
	CApplication();
	virtual ~CApplication();

	int Run(HINSTANCE hinstance);

	string FilePath(const char *str, ...);
	wstring FilePath(const wchar_t *str, ...);
};
//----------------------------------------------------------------------------------
extern CApplication *g_WispApplication;
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
#endif

