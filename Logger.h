//---------------------------------------------------------------------------
#ifndef LOGGER_H_FILE
#define LOGGER_H_FILE

#include <stdio.h>
//---------------------------------------------------------------------------
class CLogger
{
private:
	//Хэндл файла
	FILE *m_file;

public:
	CLogger();
	~CLogger();

	//Инициализирован ли логгер
	bool Ready() const {return m_file != NULL;}

	//Инициализация логгера
	void Init(const char *fName);

	//Вывод информации в лог
	void Print(const char *format, ...);
	void VPrint(const char *format, va_list ap);
	void Dump(unsigned char *buf, int len);
};
//---------------------------------------------------------------------------
//Включить/выключить логгер.
//Логгер будет работать в любом случае, но если эта опция выключена
//то он не будет писать в лог дампы пакетов и всякую отладочкую информацию
//Если значение равно 2 - будет писать отладочную инфу, но без дампов пакетов
//Без пакетов от нее толку мало...

#define CLOGGER 1

#if CLOGGER != 0
	#define EPRINT error_printf
	#define WPRINT warning_printf
	#define TPRINT trace_printf
	#define T_TPRINT trace_printf
	#if CLOGGER == 2
		#define TDUMP ;/##/
	#else //CLOGGER != 2
		#define TDUMP trace_dump
	#endif //CLOGGER == 2
#else //CLOGGER == 0
	#define EPRINT ;/##/
	#define WPRINT ;/##/
	#define TPRINT ;/##/
	#define TDUMP ;/##/
#endif //CLOGGER!=0
//---------------------------------------------------------------------------
// GCC_NORETURN means the function never returns
// GCC_PRINTF means the function has printf-style arguments
#ifdef __GNUC__
#   define GCC_NORETURN __attribute__((noreturn))
#   define GCC_PRINTF(n,m) __attribute__((format (printf, n, m)))
#else
#   define GCC_NORETURN
#   define GCC_PRINTF(n,m)
#endif
//---------------------------------------------------------------------------
void error_printf(const char * format, ...) GCC_PRINTF(1, 2);
void warning_printf(const char * format, ...) GCC_PRINTF(1, 2);
void trace_printf(const char * format, ...) GCC_PRINTF(1, 2);
void trace_dump(unsigned char *buf, int length);
//---------------------------------------------------------------------------
extern CLogger g_Logger;
//---------------------------------------------------------------------------
#endif //LOGGER_H_FILE
