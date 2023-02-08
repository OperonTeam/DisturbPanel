//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyTool/Log/MyLog.h
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#ifndef MYTRACE_H
#define MYTRACE_H

//--------------------------------------------------------------------------------------------------
// リリースビルド用マクロ定義
//--------------------------------------------------------------------------------------------------
//#define	MYLOG_ENABLE_LOG
//#define	MYLOG_ENABLE_TRACE
//#define	MYLOG_ENABLE_HEXDUMP

//--------------------------------------------------------------------------------------------------
// Prototype for C function
//--------------------------------------------------------------------------------------------------
extern "C" {
	VOID		MyLogBegin(CONST_CHAR *pszDev);
	VOID		MyLog(CONST_CHAR *pszFMT, ...);
	VOID		MyHexDump(VOID *pvPnt, size_t dumpSize, CONST_CHAR *pszTitle);
//---
	BOOL		IsDebugTarget(CONST_CHAR *pszFile);
}
//---
//--------------------------------------------------------------------------------------------------
// MY_BEGIN
//--------------------------------------------------------------------------------------------------
#if	defined(MYLOG_ENABLE_LOG) || defined(MYLOG_ENABLE_TRACE) || defined(MYLOG_ENABLE_HEXDUMP)
#define	MY_LOGBEGIN(...)													\
	{																		\
		MyLogBegin(__VA_ARGS__);											\
	}
#else
#define	MY_LOGBEGIN(...)
#endif

//--------------------------------------------------------------------------------------------------
// MY_TRACE
//--------------------------------------------------------------------------------------------------
#ifdef MYLOG_ENABLE_TRACE
#define MY_TRACE(...)														\
	{																		\
		if (IsDebugTarget(__FILE__)) {										\
			MyLog("%s:%d (%s) :", __FILE__, __LINE__, __PRETTY_FUNCTION__);	\
			MyLog(__VA_ARGS__);												\
			MyLog("\n");													\
		}																	\
	}
#else
#define MY_TRACE(...)
#endif
//---
//--------------------------------------------------------------------------------------------------
// MY_LOG
//--------------------------------------------------------------------------------------------------
#ifdef MYLOG_ENABLE_LOG
#define MY_LOG(...)															\
	{																		\
		if (IsDebugTarget(__FILE__)) {										\
			MyLog(__VA_ARGS__);												\
		}																	\
	}
#else
#define MY_LOG(...)
#endif
//---
//--------------------------------------------------------------------------------------------------
// MY_HEXDUMP
//--------------------------------------------------------------------------------------------------
#ifdef MYLOG_ENABLE_HEXDUMP
#define MY_HEXDUMP(...)														\
	{																		\
		if (IsDebugTarget(__FILE__)) {										\
			MyHexDump(__VA_ARGS__);											\
		}																	\
	}
#else
#define MY_HEXDUMP(...)
#endif

#endif
/*-- End Of File --*/
