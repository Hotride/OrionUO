//----------------------------------------------------------------------------------
#ifndef WISPAPPLICATION_H
#define WISPAPPLICATION_H
//----------------------------------------------------------------------------------
namespace WISP_APPLICATION
{
//----------------------------------------------------------------------------------
class CApplication
{
	SETGET(HINSTANCE, Hinstance, 0);
	SETGET(string, ExePathA, "");
	SETGET(wstring, ExePathW, L"");
	SETGET(string, UOFilesPathA, "");
	SETGET(wstring, UOFilesPathW, L"");

protected:
	virtual void OnMainLoop() {}

public:
	CApplication();
	virtual ~CApplication();

	int Run(HINSTANCE hinstance);

	string GetFileVersion();

	string ExeFilePath(const char *str, ...);
	wstring ExeFilePath(const wchar_t *str, ...);

	string UOFilesPath(const char *str, ...);
	wstring UOFilesPath(const wchar_t *str, ...);;
};
//----------------------------------------------------------------------------------
extern CApplication *g_WispApplication;
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
#endif

