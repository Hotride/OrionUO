//----------------------------------------------------------------------------------
#ifndef WISPAPPLICATION_H
#define WISPAPPLICATION_H
//----------------------------------------------------------------------------------
namespace WISP_APPLICATION
{
//----------------------------------------------------------------------------------
class CApplication
{
public:
	HINSTANCE Hinstance = 0;
	string ExePathA = "";
	wstring ExePathW = L"";
	string UOFilesPathA = "";
	wstring UOFilesPathW = L"";

protected:
	virtual void OnMainLoop() {}

public:
	CApplication();
	virtual ~CApplication();

	int Run(HINSTANCE hinstance);

	string GetFileVersion(uint *numericVerion = NULL);

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

