//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : include/MyDef/MyDefTypedef.h
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#ifndef MYDEFTYPEDEF_H
#define MYDEFTYPEDEF_H

//--------------------------------------------------------------------------------------------------
// 嫌悪感を件を間を抱く人が多いと思うが、無関係にいつもの感じで型宣言を大文字化します。
//--------------------------------------------------------------------------------------------------
typedef void						VOID;
typedef bool						BOOL;
typedef const char					CONST_CHAR;
typedef char						CHAR;
typedef unsigned char				UCHAR;
typedef short						SHORT;
typedef unsigned short				USHORT;
typedef int							INT;
typedef unsigned int				UINT;
typedef long						LONG;
typedef unsigned long				ULONG;
typedef float						FLOAT;
typedef double						DOUBLE;
//
typedef	String						STRING;
typedef	const String				CONST_STRING;
//
typedef const __FlashStringHelper   F_STRING;
//
#define MY_PRIVATE					static
#define MY_FALSE					false
#define MY_TRUE						true
//
#define MY_VIRTUAL					virtual
#define INLINE						inline
//
//--------------------------------------------------------------------------------------------------
// ＵＴＣから時差（＋９のマクロ定義はどこに保存するか悩むけどね）
//--------------------------------------------------------------------------------------------------
#define MY_TIMEZONE 				+9

#endif
/*-- End Of File --*/
