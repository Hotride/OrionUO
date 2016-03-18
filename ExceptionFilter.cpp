//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
LONG __stdcall MyUnhandledExceptionFilter(struct _EXCEPTION_POINTERS *ExceptionInfo)
{
	static int TotalErrorCount = 0;
	static int ErrorCount = 0;
	static DWORD LastErrorTime = GetTickCount();

	ErrorCount++;
	TotalErrorCount++;
	if(ExceptionInfo->ExceptionRecord)
	{
		EPRINT("Unhandled exception%3d: 0x%08X at %08X\n", TotalErrorCount, ExceptionInfo->ExceptionRecord->ExceptionCode, ExceptionInfo->ExceptionRecord->ExceptionAddress);

		if (g_Hinstance && ErrorCount > 100 && (GetTickCount() - LastErrorTime) < 5000)
		// crash when too many errors happen too often
		{
			//DumpErrorInformation(ExceptionInfo->ContextRecord);
			if (MessageBoxA(0, "UO client performed an unrecoverable invalid operation.\nTerminate?", 0, MB_ICONSTOP | MB_YESNO) == IDYES) ExitProcess(1);
		}

		if (g_Hinstance && (GetTickCount() - LastErrorTime) > 5000) // reset error count every 5 sec
		{
			ErrorCount = 0;
			LastErrorTime = GetTickCount();
		}

		/*int B = 255 & PEEKB(ExceptionInfo->ContextRecord->Eip);	// this can cause another crash...

		if (B >= 0xD9 && B <= 0xDF)	// For some hell float exceptions are unmasked
		{	// - ignore all floating point exceptions and clear coprocessor err state
			// these constants are coprocessor opcodes
			ExceptionInfo->ContextRecord->FloatSave.ControlWord = 0x27f;
			ExceptionInfo->ContextRecord->FloatSave.StatusWord &= ~0xff;
			return EXCEPTION_CONTINUE_EXECUTION;
		}*/

		if (TotalErrorCount < 3)
		{
			// Write error information to the log file
			// I think that only 2 first errors can be interesting

			//DumpErrorInformation(ExceptionInfo->ContextRecord);
		}

		// Determine the length of a faulting instruction and step over it
		DWORD Len = 0;
		//disasm((BYTE*)ExceptionInfo->ContextRecord->Eip,&Len);

		ExceptionInfo->ContextRecord->Eip += Len;

		if (Len == 0)
			return EXCEPTION_CONTINUE_SEARCH;
	}

	return EXCEPTION_CONTINUE_EXECUTION;
}
//---------------------------------------------------------------------------