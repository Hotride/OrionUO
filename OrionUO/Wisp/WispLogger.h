//----------------------------------------------------------------------------------
#ifndef WISPLOGGER_H
#define WISPLOGGER_H
//----------------------------------------------------------------------------------
#include "WispGlobal.h"
//----------------------------------------------------------------------------------
namespace WISP_LOGGER
{
//----------------------------------------------------------------------------------
#define CWISPLOGGER 1

#if CWISPLOGGER != 0
	#define INITLOGGER(path) WISP_LOGGER::g_WispLogger.Init(path);
	#define LOG WISP_LOGGER::g_WispLogger.Print
	#if CWISPLOGGER == 2
		#define LOG_DUMP(...)
	#else //CWISPLOGGER != 2
		#define LOG_DUMP WISP_LOGGER::g_WispLogger.Dump
	#endif //CWISPLOGGER == 2
#else //CWISPLOGGER == 0
	#define INITLOGGER(path)
#define LOG(...)
#define LOG_DUMP(...)
#endif //CWISPLOGGER!=0
//----------------------------------------------------------------------------------
class CLogger
{
protected:
	FILE *m_File;

public:
	CLogger();
	~CLogger();

	bool Ready() const { return m_File != NULL; }

	void Init(const string &filePath);
	void Init(const wstring &filePath);

	void Print(const char *format, ...);
	void VPrint(const char *format, va_list ap);
	void Print(const wchar_t *format, ...);
	void VPrint(const wchar_t *format, va_list ap);
	void Dump(uchar *buf, int size);
};
//----------------------------------------------------------------------------------
extern CLogger g_WispLogger;
}
//----------------------------------------------------------------------------------
#endif //WISPLOGGER_H
//----------------------------------------------------------------------------------
