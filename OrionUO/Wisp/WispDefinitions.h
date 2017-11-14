//----------------------------------------------------------------------------------
#ifndef WISPDEFINITIONS_H
#define WISPDEFINITIONS_H
//----------------------------------------------------------------------------------
//0 - disable debugging
//1 - debugging with queue (very slow run mode)
//2 - debugging with function name (normal run mode)
#define USE_WISP_DEBUG_FUNCTION_NAMES 0

#if USE_WISP_DEBUG_FUNCTION_NAMES == 1
#define WISPFUN_DEBUG(name) \
	CWispFunDebug wispfunctiondebugname("w_" __FUNCTION__); \
	(void)wispfunctiondebugname
	//static const std::string wispfunctiondebugname("w_" name);
#elif USE_WISP_DEBUG_FUNCTION_NAMES == 2
	extern char *g_WispCurrentFunctionName;
#define WISPFUN_DEBUG(name) \
	g_WispCurrentFunctionName = "w_" __FUNCTION__
#else
	#define WISPFUN_DEBUG(name)
#endif
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
#define IFOR(var, start, stop) for (intptr_t var = start; var < (intptr_t)stop; var ++)
//!Decremented ordinary for
#define DFOR(var, start, stop) for (intptr_t var = start; var >= (intptr_t)stop; var --)
//----------------------------------------------------------------------------------
#define IN_RANGE(name, id1, id2) ((name) >= (id1) && (name) <= (id2))
#define OUT_RANGE(name, id1, id2) ((name) < (id1) || (name) > (id2))
//----------------------------------------------------------------------------------
//!Set/Get ordinary class property
#define SETGET(type, name, defaultValue) \
	protected: \
	type m_##name = defaultValue; \
	public: \
	inline void __fastcall Set##name(const type &val) { m_##name = val; } \
	inline type Get##name() const { return m_##name; } \
	__declspec(property(get = Get##name, put = Set##name)) type name
//----------------------------------------------------------------------------------
//!Set/Get ordinary class property with event
#define SETGETE(type, name, defaultValue) \
	protected: \
	type m_##name = defaultValue; \
	public: \
	void Set##name(const type &val); \
	inline type Get##name() const { return m_##name; } \
	__declspec(property(get = Get##name, put = Set##name)) type name
//----------------------------------------------------------------------------------
//!Set/Get ordinary class property for pointers
#define SETGETP(type, name, defaultValue) \
	protected: \
	type m_##name = defaultValue; \
	public: \
	inline void __fastcall Set##name(type val) { m_##name = val; } \
	inline type Get##name() const { return m_##name; } \
	__declspec(property(get = Get##name, put = Set##name)) type name
//----------------------------------------------------------------------------------
#endif
