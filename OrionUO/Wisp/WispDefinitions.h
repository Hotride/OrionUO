//----------------------------------------------------------------------------------
#ifndef WISPDEFINITIONS_H
#define WISPDEFINITIONS_H
//----------------------------------------------------------------------------------
#define WISP_ONE_NAME 1

#if WISP_ONE_NAME == 1
#define WISP_APPLICATION Wisp
#define WISP_WINDOW Wisp
#define WISP_MOUSE Wisp
#define WISP_GEOMETRY Wisp
#define WISP_THREAD Wisp
#define WISP_THREADED_TIMER Wisp
#define WISP_DATASTREAM Wisp
#define WISP_NETWORK Wisp
#define WISP_FILE Wisp
#define WISP_LOGGER Wisp
#else
#define WISP_APPLICATION WispApplication
#define WISP_WINDOW WispWindow
#define WISP_MOUSE WispMouse
#define WISP_GEOMETRY WispGeometry
#define WISP_THREAD WispThread
#define WISP_THREADED_TIMER WispThreadedTimer
#define WISP_DATASTREAM WispDataStream
#define WISP_NETWORK WispNetwork
#define WISP_FILE WispFile
#define WISP_LOGGER WispLogger
#endif
//----------------------------------------------------------------------------------
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned __int64 uint64;
typedef uchar *puchar;
typedef ushort *pushort;
typedef uint *puint;
typedef uint64 *puint64;
typedef char *pchar;
typedef short *pshort;
typedef int *pint;
typedef __int64 *pint64;
//----------------------------------------------------------------------------------
#define DEBUGGIND_OUTPUT 1

#if DEBUGGIND_OUTPUT == 1
void DebugMsg(const char *format, ...);
void DebugMsg(const wchar_t *format, ...);
void DebugDump(puchar data, const int &size);
#else
#define DebugMsg(...)
#define DebugDump(buf, size)
#endif
//----------------------------------------------------------------------------------
#define RELEASE_POINTER(ptr) \
if (ptr != NULL) \
{ \
	delete ptr; \
	ptr = NULL; \
}
//----------------------------------------------------------------------------------
//!Incremented ordinary for
#define IFOR(var, start, stop) for (int var = start; var < stop; var ++)
//!Decremented ordinary for
#define DFOR(var, start, stop) for (int var = start; var >= stop; var --)
//----------------------------------------------------------------------------------
#define IN_RANGE(name, id1, id2) (name >= id1 && name <= id2)
#define OUT_RANGE(name, id1, id2) (name < id1 || name > id2)
//----------------------------------------------------------------------------------
//!Set/Get ordinary class property
#define SETGET(type, name) \
	protected: \
	type m_##name; \
	public: \
	inline void __fastcall Set##name(const type &val) { m_##name = val; } \
	inline type Get##name() const { return m_##name; } \
	__declspec(property(get = Get##name, put = Set##name)) type name
//----------------------------------------------------------------------------------
//!Set/Get ordinary class property with event
#define SETGETE(type, name, event) \
	protected: \
	type m_##name; \
	public: \
	void event(const type &val); \
	inline void __fastcall Set##name(const type &val) { event(val); m_##name = val; } \
	inline type Get##name() const { return m_##name; } \
	__declspec(property(get = Get##name, put = Set##name)) type name
//----------------------------------------------------------------------------------
#endif
