//----------------------------------------------------------------------------------
#include "ExceptionFilter.h"
#include "OrionWindow.h"
#include "OrionUO.h"
#include "Wisp/WispMappedFile.h"
#include <psapi.h>
#include <tlhelp32.h>
#include "VMQuery.h"

#pragma comment(lib, "Psapi.lib")
//----------------------------------------------------------------------------------
string GetMemStorageText(const uint &storage)
{
	switch (storage)
	{
		case MEM_FREE:
			return "Free";
		case MEM_RESERVE:
			return "Reserve";
		case MEM_IMAGE:
			return "Image";
		case MEM_MAPPED:
			return "Mapped";
		case MEM_PRIVATE:
			return "Private";
		default:
			break;
	}

	return "Unknown";
}
//----------------------------------------------------------------------------------
inline BOOL ModuleFind(const HANDLE &snapshot, PVOID pvBaseAddr, PMODULEENTRY32 pme)
{
	for (BOOL ok = Module32First(snapshot, pme); ok; ok = Module32Next(snapshot, pme))
	{
		if (pme->modBaseAddr == pvBaseAddr)
			return TRUE;
	}

	return FALSE;
}
//----------------------------------------------------------------------------------
void DumpRegionInfo(const HANDLE &snapshot, HANDLE hProcess, VMQUERY &vmq)
{
	if (vmq.dwRgnStorage == MEM_FREE)
		return;

	string filePath = "";

	if (vmq.pvRgnBaseAddress != NULL)
	{
		MODULEENTRY32 me = { sizeof(me) };
		if (ModuleFind(snapshot, vmq.pvRgnBaseAddress, &me))
			filePath = ToString(me.szExePath);
		else
		{
			wchar_t filename[MAX_PATH + 1];
			DWORD d = GetMappedFileName(hProcess, vmq.pvRgnBaseAddress, filename, MAX_PATH);

			if (d)
				filePath = ToString(filename);
		}
	}

	if (vmq.fRgnIsAStack)
		filePath = "Thread Stack";

	CRASHLOG("Address: 0x%08X => 0x%08X Storage: '%s' size: %i blocks: %i path: %s\n", vmq.pvBlkBaseAddress, (puchar)vmq.pvBlkBaseAddress + vmq.RgnSize, GetMemStorageText(vmq.dwBlkStorage).c_str(), vmq.RgnSize, vmq.dwRgnBlocks, filePath.c_str());
}
//----------------------------------------------------------------------------------
void DumpLibraryInformation()
{
	CRASHLOG("Library information:\n");

	DWORD processId = 0;
	GetWindowThreadProcessId(g_OrionWindow.Handle, &processId);

	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

	if (process == NULL)
	{
		CRASHLOG("::OpenProcess failed!\n");
		return;
	}

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, processId);

	if (snapshot == NULL)
	{
		CRASHLOG("::CreateToolhelp32Snapshot failed!\n");
		CloseHandle(process);
		return;
	}

	PVOID address = NULL;

	while (true)
	{
		VMQUERY vmq = { 0 };

		if (!VMQuery(process, address, &vmq))
			break;

		DumpRegionInfo(snapshot, process, vmq);

		address = ((puchar)vmq.pvRgnBaseAddress + vmq.RgnSize);
	}

	CloseHandle(snapshot);
	CloseHandle(process);
}
//----------------------------------------------------------------------------------
void DumpCurrentRegistersInformation(CONTEXT* CR)
{
	CRASHLOG("EAX=0x%08X, EBX=0x%08X, ECX=0x%08X, EDX=0x%08X\n", CR->Eax, CR->Ebx, CR->Ecx, CR->Edx);
	CRASHLOG("ESI=0x%08X, EDI=0x%08X, ESP=0x%08X, EBP=0x%08X\n", CR->Esi, CR->Edi, CR->Esp, CR->Ebp);
	CRASHLOG("EIP=0x%08X, EFLAGS=0x%08X\n\n", CR->Eip, CR->EFlags);

	CRASHLOG("Bytes at EIP:\n");
	CRASHLOG_DUMP((puchar)CR->Eip, 16);

	CRASHLOG("Bytes at ESP:\n");
	CRASHLOG_DUMP((puchar)CR->Esp, 64);
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
			if (MessageBoxA(0, "Orion client performed an unrecoverable invalid operation.\nTerminate?", 0, MB_ICONSTOP | MB_YESNO) == IDYES)
			{
				DumpLibraryInformation();

				DumpCurrentRegistersInformation(exceptionInfo->ContextRecord);

				WISP_FILE::CMappedFile file;

				wchar_t fileName[MAX_PATH] = { 0 };
				GetModuleFileName(0, fileName, MAX_PATH);

				if (file.Load(fileName))
				{
					UCHAR_LIST pattern;
					puchar eipBytes = (puchar)exceptionInfo->ContextRecord->Eip;

					IFOR(i, 0, 16)
						pattern.push_back(eipBytes[i]);

					UINT_LIST list = COrion::FindPattern(file.Start, file.Size, pattern);

					for (const int &item : list)
						CRASHLOG("Address in exe (by EIP): 0x%08X\n", item);

					file.Unload();
				}

				ExitProcess(1);
			}
		}

		if (ticks - lastErrorTime > 5000)
		{
			errorCount = 0;
			lastErrorTime = ticks;
		}

		if (errorCount == 1)
			DumpCurrentRegistersInformation(exceptionInfo->ContextRecord);
	}

	//	return EXCEPTION_CONTINUE_SEARCH;
	return EXCEPTION_CONTINUE_EXECUTION;
}
//----------------------------------------------------------------------------------
