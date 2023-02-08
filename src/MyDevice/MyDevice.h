//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyDevice/MyDevice.h
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#ifndef MYDEVICE_H
#define MYDEVICE_H

//--------------------------------------------------------------------------------------------------
// 外部参照・依存関係インクルード・ヘッダ情報
//--------------------------------------------------------------------------------------------------
#include <CircularBuffer.h>													// メモリバッファ管理
//---
#include "Button/MyButton.h"												// Button Operation
#include "FDRRec/MyFDRHeadRec.h"											// FDRRec Head
#include "FDRRec/MyFDRBodyRec.h"											// FDRRec Body
#include "Thermo/MyThermoMatrix.h"											// Thermo Pixcel
//---
#include "MyTask.h"
#include "MyTask/MyGPS/MyGPS.h"
#include "MyTask/MyI2C/MyI2C.h"
#include "MyTask/MyNET/MyNET.h"
#include "MyTask/MyBLE/MyBLE.h"

//
//--------------------------------------------------------------------------------------------------
// 周辺デバイス・クラス定義（作業領域をこのクラスに集約させようと考えていました）
//--------------------------------------------------------------------------------------------------
class   _MyDevice {
	private:
		//------------------------------------------------------------------------------------------
		// 接続センサーデバイス・クラス
		//	MyRTCDevice					: 内蔵RTCデバイス・クラス
		//  MyButton					; 物理ボタンデバイス・クラス
		//---
		//	MyPanelMain					: メインパネル描画クラス
		//	MyPanelSide					: 	MyPanelSide;									// サイドパネル描画クラス
		//------------------------------------------------------------------------------------------
		RTC_SAMD51			MyRTCDevice;									// 内蔵RTC
		_MyButton			MyButton;										// ボタン操作クラス
		//---
		//------------------------------------------------------------------------------------------
		// RTOSタスク・クラス
		//  MyTFT						: TFT液晶パネル・クラス
		//  MyGPS						; シリアル接続の激安GPSセンサー
		//  MyI2C						: I2C接続の各種センサー
		//  MyNET						: NET(WiFi TCP/IP)接続クラス
		//  MyBLE						: Bluetooth接続のセンサー（ライブラリが謎、、、）
		//------------------------------------------------------------------------------------------
		_MyGPS				MyGPS;											// GPSデバイス
		_MyI2C				MyI2C;											// I2Cデバイス
		_MyNET				MyNET;											// NETデバイス
		_MyBLE				MyBLE;											// BLEデバイス
//---
		//------------------------------------------------------------------------------------------
		// デバイスステータス・管理情報
		//------------------------------------------------------------------------------------------
		_MyDEV_STAT			MyFDELogID[_MyDEV_ID::MY_DEVID_COUNT];
//---
		//------------------------------------------------------------------------------------------
		// ＳＤ書込み用のバッファ：リアルタイムデータ（10Hz）保存領域
		//------------------------------------------------------------------------------------------
		// MyRingBuff.available()	Allocate Free Space Num
		// MyRingBuff.capacity		Allocate Element Num
		// MyRingBuff.size()		Store Data Num
		//------------------------------------------------------------------------------------------
		CircularBuffer<_MyFDRBodyRec, MY_FDRREC_RINGBUFF_NUM>MyFDRRingBuff;	// Ringバッファメモリ領域
		ULONG				ulRecordSeq;									// FDRRecシーケンス番号
		INT					iUnConfirmedNum;								// 書込み未完了レコード件数
//---
	public:
		_MyDevice();
		~_MyDevice();
//---
		//------------------------------------------------------------------------------------------
		// 隠匿デバイスクラスへの公開アクセスメソッド
		//	RTC		: RTCAdjust			: RTC日時調整メソッド
		//	Button	: SetButtonInfo		: 手動操作情報の設定
		//			: GetButtonInfo		: 手動操作情報の取得
		//	GPS		: GPSCourseTo		: 指定座標までのコース
		//			: GPSDistance		: 指定座標までの距離
		//	Task	: TaskCreateStart	: 全タスク生成・開始メソッド
		//------------------------------------------------------------------------------------------
		VOID				RTCAdjust(DateTime dtAdjust);					// RTC日時調整メソッド
		VOID				SetButtonInfo(UINT uiGesture);
		UINT				GetButtonInfo(UINT uiMaskInfo = MY_KEYPAD_ALLMASK);
		FLOAT				GPSCourseTo(_MyGPSPoint MyGPSPoint);			// 指定座標までのコース
		FLOAT				GPSDistance(_MyGPSPoint MyGPSPoint);			// 指定座標までの距離
		//
		VOID				Initialize(_MyJARVis *pMyJARVis);				// デバイスクラスの初期化
		//---
		INLINE	VOID		TaskCreateI2C(USHORT usStackSize, ULONG ulUrgency, INT iFrequency) {
								this->MyI2C.MyTaskCreate("I2C", usStackSize, ulUrgency, iFrequency, &(this->MyI2C));
		};
		INLINE	VOID		TaskCreateGPS(USHORT usStackSize, ULONG ulUrgency, INT iFrequency) {
								this->MyGPS.MyTaskCreate("GPS", usStackSize, ulUrgency, iFrequency, &(this->MyGPS));
		};
		INLINE	VOID		TaskCreateNET(USHORT usStackSize, ULONG ulUrgency, INT iFrequency) {
								this->MyNET.MyTaskCreate("NET", usStackSize, ulUrgency, iFrequency, &(this->MyNET));
		};
		INLINE	VOID		TaskCreateBLE(USHORT usStackSize, ULONG ulUrgency, INT iFrequency) {
								this->MyNET.MyTaskCreate("BLE", usStackSize, ulUrgency, iFrequency, &(this->MyBLE));
		};
//---
		//------------------------------------------------------------------------------------------
		// デバイスステータスへの公開アクセスメソッド
		//	SetDeviceInfo				: 指定したデバイスの状態を更新する
		//	GetDeviceInfo				: 指定したデバイスの状態を取得する
		//	IsAllDeviceNormal			: 全てのデバイス状態が正常値か確認する
		//------------------------------------------------------------------------------------------
		VOID 				SetDeviceInfo(_MyDEV_ID MyDevID, _MyDEV_STAT _MyDevStat);
		_MyDEV_STAT			GetDeviceInfo(_MyDEV_ID MyDevID);
		BOOL				IsAllDeviceNormal(VOID);
//---
		//------------------------------------------------------------------------------------------
		// ＳＤ書込み用のバッファ：リアルタイムデータ（10Hz）保存領域・アクセスメソッド
		//	FDRRingBuffInit				: Ringバッファを初期化する
		//	FDRRingBuffPush				: Ringバッファ新規挿入
		//	FDRRingBuffGetFirst			: Ringバッファに保存されているSD未書込みのメモリを取得する
		//	FDRRingBuffSize				: Ringバッファに保存されているSD未書込みのデータ件数を取得する
		//	FDRRingBuffShift			: SD書込みが完了したデータ件数を更新する
		//------------------------------------------------------------------------------------------
		VOID				FDRRingBuffInit(VOID);
		BOOL				FDRRingBuffPush(VOID);							// PUSH(this->MyFDRBodyRecNow)
		BOOL				FDRRingBuffGetFirst(_MyFDRBodyRec *pMyFDRBodyRec);
		INT					FDRRingBuffSize(VOID);
		VOID				FDRRingBuffShift(VOID);
//---
		//------------------------------------------------------------------------------------------
		// FDRRec情報
		//  接続しているセンサーが取得した最新データを保存する領域・アクセスクラス
		//------------------------------------------------------------------------------------------
		_MyFDRHeadRec		MyFDRHeadRecNow;								// 最新FDRHeadRecクラス情報
		_MyFDRBodyRec		MyFDRBodyRecNow;								// 最新FDRBodyRecクラス情報 (10Hz)
//---
		//------------------------------------------------------------------------------------------
		// Thermoセンサー分析情報
		//  センサー分解能をソフト的に向上させたデータを保存する領域・アクセスクラス  
		//------------------------------------------------------------------------------------------
		_MyThermoMatrix		MyThermoMatrix;									// Thermoセンサー分析情報
//---
		//------------------------------------------------------------------------------------------
		// 開発用
		//------------------------------------------------------------------------------------------
		VOID				DebugTaskInfo(CHAR *pszMsg = "");
};
#endif
/*-- End Of File --*/
