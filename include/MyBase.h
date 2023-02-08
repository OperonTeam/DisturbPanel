//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : include/MyBase.h
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#ifndef MYBASE_H
#define MYBASE_H

//--------------------------------------------------------------------------------------------------
// vtaskList()を利用する場合のマクロ定義
//--------------------------------------------------------------------------------------------------
#ifdef  MY_DEBUG_TASKINFO
#define configUSE_TRACE_FACILITY						1
#define configUSE_STATS_FORMATTING_FUNCTIONS			1
#define INCLUDE_uxTaskGetStackHighWaterMark				1
#define INCLUDE_uxTaskGetStackHighWaterMark2			1
#endif
//
//--------------------------------------------------------------------------------------------------
// Arduino(SEEED社Wioterminal)の動作環境系
//--------------------------------------------------------------------------------------------------
#include <Arduino.h>											// Arduino
#include <Seeed_Arduino_FreeRTOS.h>								// FreeRTOS(Seeed)
#include <Wire.h>												// Wire
#include <SPI.h>												// SPI
#include <Seeed_FS.h>											// FileSystem
#include <SD/Seeed_SD.h>										// 	+ Extenral SD
#include <Seeed_SFUD.h>						        			//	+ Internal FLASH（現在利用せず）
#include <RTC_SAMD51.h>											// RTC

//--------------------------------------------------------------------------------------------------
// 懐かしい開発言語系
//--------------------------------------------------------------------------------------------------
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

//--------------------------------------------------------------------------------------------------
// 自前定義情報をインクルード
//--------------------------------------------------------------------------------------------------
#include "MyDef.h"
#include "MyConnectModule.h"
#include "MyTool.h"

#endif
/*-- End Of File --*/
