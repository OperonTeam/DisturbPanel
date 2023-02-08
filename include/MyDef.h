//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : include/MyDef.h
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#ifndef MYDEF_H
#define MYDEF_H

#include "MyDef\MyDefTypedef.h"
#include "MyDef\MyDefEnum.h"
#include "MyDef\MyDefStruct.h"
//
//--------------------------------------------------------------------------------------------------
// グローバル・デバイス名称
//--------------------------------------------------------------------------------------------------
#define MYDEV_SD					SD									// SD Device
#define MYDEV_SFUD					SFUD								// SDUF(Internal Strage)
#define MYDEV_WiFi	    			WiFi								// WiFi Device
#define	MYGPS_Serial				Serial1								// GPS GROVE Serial
#define	MYDEV_Debug					Serial								// For Debug Serial

//--------------------------------------------------------------------------------------------------
// 日本語ＦＯＮＴを実行モジュールに組み込む場合はこちら（実行モジュールサイズに注意）
//--------------------------------------------------------------------------------------------------
//#define	MY_USE_JAPANESE_FONT				1				// 日本語ＦＯＮＴを利用する

//--------------------------------------------------------------------------------------------------
// FDR情報(SDファイル)記録時フォルダのマクロ
//--------------------------------------------------------------------------------------------------
#define	MY_FDRREC_FILEPATH					"/AWSLog"		// FDR記録トップフォルダ名
#define	MY_FDRREC_MAXRECORDERNUM			100				// 一度に処理する最大レコード数
#define	MY_FDRREC_RINGBUFF_NUM				100             // RingBuffで確保するレコード数

//--------------------------------------------------------------------------------------------------
// RTOS上で実行するタスクの簡易情報出力
// 【注意】以下を利用する場合は、以下の行（のコメントを修正するの）では無く、コンパイル時のオプション
//	(-D MY_DEBUG_TASKINFO)を指定すること。 ->  platformio.ini
//--------------------------------------------------------------------------------------------------
//#define	MY_DEBUG_TASKINFO				1				// タスク情報の出力
//
//--------------------------------------------------------------------------------------------------
// ConfigFile設定内容確認用のマクロ
// SD内に記述した動作環境設定ファイルの内容をダンプします
//--------------------------------------------------------------------------------------------------
//#define MY_DEBUG_CONFIGFILEDUMP			1				// ConfigFileダンプ出力
//
//--------------------------------------------------------------------------------------------------
// GPS開発・室内確認用のマクロ
// 利用したGPSセンサーは、室内でGPS信号を受信することが出来ない為、適当なデータを返却させます。
//--------------------------------------------------------------------------------------------------
//#define MY_DEBUG_GPSONLINE				1				// GPS開発用：オンライン衛星数
//#define MY_DEBUG_GPSDATETIME				1				// GPS開発用：日時 
//#define MY_DEBUG_GPSPOSISION				1				// GPS開発用：座標 
//#define MY_DEBUG_GPSSPEED					1				// GPS開発用：移動速度
//#define MY_DEBUG_GPSALTITUDE				1				// GPS開発用：高度
//#define MY_DEBUG_GPSCOURSE				1				// GPS開発用：移動方向
//#define	MY_DEBUG_GPS_DUMP_DETAIL		1				// GPSセンサーから送信されるGPS情報データをダンプ出力する
//#define	MY_GPS_SERIAL_HISPEEDMODE		1				// GPSセンサーとの通信速度を高速化（19200bps）する。
															// 10Hzでセンサーデータを受信する場合、シリアル通信が標準
															// 速度（9600bps）のままだと取りこぼす可能性があります。
//
//--------------------------------------------------------------------------------------------------
// RTOS上で実行するタスクが、具体的にどの様に実行さえれるのか調査する際に利用した情報
// 現在でもRTOS知識は乏しく、いつか時間を見つけて学んでみようと思う。
//--------------------------------------------------------------------------------------------------
//#define MY_TASK_EXECUTION_STATISTICS		1				// ユーザタスク実行統計情報取得処理
//#define MY_DEBUG_BLE_DUMP_DETAIL			1				// BLEセンサー情報
//#define MY_DEBUG_I2C_BME280_DUMP			1				// I2C BME280センサー情報
//#define MY_DEBUG_I2C_MPU9250_DUMP			1				// I2C MPU9250センサー情報
//
//--------------------------------------------------------------------------------------------------
// 開発・メンテナンス用のマクロ定義
// グローバル・ヘッダに定義しているので変更すると、本来無関係なソースファイルもビルドされます
//--------------------------------------------------------------------------------------------------
#define	MY_SPI_PANEL_DONOT_TEST				1				// SPIパネル起動時に、機能テストを行わない。
															// 液晶パネル起動時にセルフ・チェックを行わない。
															// →あまり嬉しい情報は取得出来ない。
//---
//#define	MY_TPMSBLE_USE_BUILD				1			// BLE(TPMS)実装時のマクロ
//---
//--------------------------------------------------------------------------------------------------
// 表示（メイン・サイド）パネル情報用のマクロ定義
//--------------------------------------------------------------------------------------------------
//#define	MY_NEWGFX_INTERNAL_PANEL		1				// WioTerminal 内臓パネル（誤って壊してしまった内蔵パネル）
															// 買い替え様にも、販売金額がおかしな感じなので。
															
//--------------------------------------------------------------------------------------------------
// 物理ボタン・操作のマクロ定義
//--------------------------------------------------------------------------------------------------
#define		MY_BUTTON_INVALIDATION			1				// 配置上、操作出来ない物理ボタン処理を停止する

#endif
/*-- End Of File --*/