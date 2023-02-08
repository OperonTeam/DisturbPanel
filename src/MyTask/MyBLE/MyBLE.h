//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyTask/MyBLE/MyBLE.h
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#ifndef MYBLE_H
#define MYBLE_H

//--------------------------------------------------------------------------------------------------
// 外部参照・依存関係インクルード・ヘッダ情報
//--------------------------------------------------------------------------------------------------
#include "rpcBLEDevice.h"

//--------------------------------------------------------------------------------------------------
// ＣＯＭデバイス用クラス
//--------------------------------------------------------------------------------------------------
class   _MyBLE	: public _MyTask {
	protected:
		BLEScan				*pBLEScan;										// BLE Scan
//		BLEAdvertisedDevice	BLEDev;											// BLE Device 
		String				strBleTMPSUUID;									// 目的のＵＵＩＤ
//---
		ULONG				ulReceiveByte;									// センサー情報受信バイト数
		ULONG				ulLoopCount;									// センサー情報ループカウンタ
//
		//------------------------------------------------------------------------------------------
		// 仮想メソッド
		//------------------------------------------------------------------------------------------
		_MyTask::_MyTaskRet	MyBegin(_MyDevice *pMyDevice, _MyJARVis *pMyJARVis)		override;
		_MyTask::_MyTaskRet	MyUpdate(_MyDevice *pMyDevice, _MyJARVis *pMyJARVis)	override;
		_MyTask::_MyTaskRet	MySetup(_MyDevice *pMyDevice, _MyJARVis *pMyJARVis)		override;

		//------------------------------------------------------------------------------------------
		//	AnalyzTPMSInfo		ＴＰＭＳ受信情報を分析する
		//------------------------------------------------------------------------------------------
public:
		typedef	enum {
			MY_TPMS_NO		= -1,										// 目的のTPMSでは無い	
			MY_TPMS_FRONT	= 0,										// フロントタイヤ用
			MY_TPMS_REAR	= 1,										// リアタイヤ用
			MY_TPMS_NUM		= 2											// 取扱うTPMSの数量
		} _MyTPMSType;
		_MyTPMSType			AnalyzTPMSInfo(BLEAdvertisedDevice *pBLEDev, _MyTPMSInfo *pMyTPMSInfo);
		_MyTPMSType			DecodeTPMSInfo(CHAR *pszManufacturerData, _MyTPMSInfo *pMyTPMSInfo);
		//--
//	public:
		_MyBLE();
		~_MyBLE();
//---
		VOID				MyDump(VOID);
};
#endif
/*-- End Of File --*/
