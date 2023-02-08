//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : include/MyDef/MyDefEnum.h
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#ifndef MYDEFENUM_H
#define MYDEFENUM_H

//--------------------------------------------------------------------------------------------------
// 発生デバイスＩＤ一覧
//--------------------------------------------------------------------------------------------------
enum _MyDEV_ID {				// Device Type	|NONE		|OPEN		|SETUP		|FAIL		|LOST		|
								//------+-------+-----------+-----------+-----------+-----------+-----------|
	MY_DEVID_GPS		= 0,	// GPS	|GPSTask|Initial	|Running	|SensorSetup|Dis-Connect|Signal Lost|
	MY_DEVID_RTC		= 1,	// RTC	|N/A	|Initial	|Running	|Adjust Time|N/A		|N/A		|
	MY_DEVID_NET		= 2,	// NET	|NETTask|Initial	|Running	|Ping		|WiFi		|Dis-Connect|
	MY_DEVID_FDR		= 3,	// FDR	|N/A	|Initial	|Running	|SDFileOpen	|SD Open	|Write Error|
	MY_DEVID_IMU		= 4,	// IMU	|I2CTask|Initial	|Running	|SensorSetup|N/A		|N/A		|
	MY_DEVID_BME		= 5,	// BME	|I2CTask|Initial	|Running	|N/A		|N/A		|N/A		|
	MY_DEVID_AMG		= 6,	// AMG	|I2CTask|Initial	|Running	|N/A		|N/A		|N/A		|
	MY_DEVID_PAJ		= 7,	// PAJ	|I2CTask|Initial	|Running	|ReadGesture|N/A		|N/A		|
	MY_DEVID_HAL		= 8,	// HAL	|N/A	|Initial	|Running	|			|N/A		|N/A		| 	<--	JARVis
	MY_DEVID_BLE		= 9,	// BLE	|BLETask|			|			|			|			|			|
	MY_DEVID_COUNT		= 10	// Num	+-------+-----------+-----------+-----------+-----------+-----------+
};
//
//--------------------------------------------------------------------------------------------------
// 発生イベントＩＤ一覧
//--------------------------------------------------------------------------------------------------
enum	_MyDEV_STAT {			// ステータス	|ＧＰＳ	|ＲＴＣ	|ＮＥＴ	|ＦＤＲ	|ＩＭＵ	|ＢＭＥ	|ＡＭＧ	|ＰＡＪ	|ＨＡＬ	|
								//--------------+-------+-------+-------+-------+-------+-------+-------+-------+-------|
	MY_DEVSTAT_NONE		= 0,	// 初期値		|	〇	|	〇	|	〇	|	〇	|	〇	|	〇	|	〇	|	〇	|	〇	|	デバイス未接続（初期値）
	MY_DEVSTAT_OPEN		= 1,	// オープン		|	〇	|	〇	|	〇	|	〇	|	〇	|	〇	|	〇	|	〇	|	〇	|	デバイス接続済（運航状態）
	MY_DEVSTAT_SETUP	= 2,	// セットアップ	|	〇	|		|		|		|		|		|		|		|		|	GPSセンサーコマンド送信中
								//				|		|	〇	|		|		|		|		|		|		|		|	RTC時刻補正待ち
								//				|		|		|		|	〇	|		|		|		|		|		|	FDR記録ファイル切り替え中
								//				|		|		|		|		|	〇	|		|		|		|		|	IMUキャリブレーション中
								//				|		|		|		|		|		|		|		|	〇	|		|	PAJ割り込み中
	MY_DEVSTAT_FAIL		= 3,	// 接続異常		|	〇	|		|		|		|		|		|		|		|		|	GPSセンサー通信途絶
								//				|		|		|	〇	|		|		|		|		|		|		|	NET(WiFi)接続中
								//				|		|		|		|	〇	|		|		|		|		|		|	FDR記録デバイス初期化中
	MY_DEVSTAT_LOST		= 4,	// データ欠落	|	〇	|		|		|		|		|		|		|		|		|	GPSオンライン衛星喪失
								//				|		|		|	〇	|		|		|		|		|		|		|	NET(対向ホスト)接続処理中
								//				|		|		|		|	〇	|		|		|		|		|		|	FDR記録エラー処理中
	MY_DEVSTAT_COUNT	= 5
};
#endif
/*-- End Of File --*/
