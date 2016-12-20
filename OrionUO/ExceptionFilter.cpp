//----------------------------------------------------------------------------------
#include "ExceptionFilter.h"
//----------------------------------------------------------------------------------
void DumpCurrentRegistersInformation(CONTEXT* CR)
{
	CRASHLOG("EAX=%08X, EBX=%08X, ECX=%08X, EDX=%08X\n",
		CR->Eax, CR->Ebx, CR->Ecx, CR->Edx);
	CRASHLOG("ESI=%08X, EDI=%08X, ESP=%08X, EBP=%08X\n",
		CR->Esi, CR->Edi, CR->Esp, CR->Ebp);
	CRASHLOG("EIP=%08X, EFLAGS=%08X\n\n",
		CR->Eip, CR->EFlags);

	CRASHLOG("Bytes at EIP:\n");
	CRASHLOG_DUMP((unsigned char*)CR->Eip, 16);
	CRASHLOG("Bytes at ESP:\n");
	CRASHLOG_DUMP((unsigned char*)CR->Esp, 64);
}
//----------------------------------------------------------------------------------
LONG __stdcall OrionUnhandledExceptionFilter(struct _EXCEPTION_POINTERS *exceptionInfo)
{
	static int errorCount = 0;
	static uint lastErrorTime = 0;
	
	uint ticks = GetTickCount();

	errorCount++;

	if (exceptionInfo->ExceptionRecord)
	{
		CRASHLOG("Unhandled exception #%i: 0x%08X at %08X\n", errorCount, exceptionInfo->ExceptionRecord->ExceptionCode, exceptionInfo->ExceptionRecord->ExceptionAddress);

		if (errorCount > 100 && (ticks - lastErrorTime) < 5000)
		{
			if (MessageBoxA(0, "UO client performed an unrecoverable invalid operation.\nTerminate?", 0, MB_ICONSTOP | MB_YESNO) == IDYES)
			{
				DumpCurrentRegistersInformation(exceptionInfo->ContextRecord);

				ExitProcess(1);
			}
		}

		if (ticks - lastErrorTime > 5000)
		{
			errorCount = 0;
			lastErrorTime = ticks;
		}

		if (errorCount < 2)
			DumpCurrentRegistersInformation(exceptionInfo->ContextRecord);
	}

	//	return EXCEPTION_CONTINUE_SEARCH;
	return EXCEPTION_CONTINUE_EXECUTION;
}
//----------------------------------------------------------------------------------
