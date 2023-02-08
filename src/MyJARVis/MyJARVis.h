//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyJARVis/MyJARVis.h
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#ifndef MYJARVIS_H
#define MYJARVIS_H

//--------------------------------------------------------------------------------------------------
// 外部参照・依存関係インクルード・ヘッダ情報
//--------------------------------------------------------------------------------------------------
#include <CircularBuffer.h>										// メモリバッファ管理
//---
//--------------------------------------------------------------------------------------------------
// 子クラス・インクルード
//--------------------------------------------------------------------------------------------------
#include "Geolocation/MyGeolocation.h"
#include "Analyze/MyJARVisAnalyze.h"
#include "PastRec/MyPastRec.h"
#include "Message/MyMessage.h"
//
//--------------------------------------------------------------------------------------------------
// 構造体関係の定義
//--------------------------------------------------------------------------------------------------
struct _MyShortTermRec {														// For IMU Point (By FLOAT)
	FLOAT				fGPSSpeed;												// For GPS			  				RMC	移動速度（km/h）
	_My3DFIMU			My3DFIMUACCEL;											// For MPU9250	IMU (By FLOAT)
};	
//
//--------------------------------------------------------------------------------------------------
// 執事管理用クラス
//--------------------------------------------------------------------------------------------------
class   _MyJARVis {
	private:
		//------------------------------------------------------------------------------------------
		// メッセージ・クラス
		//------------------------------------------------------------------------------------------
		_MyMessage			MyMessage;											// メッセージ文字列
		//---
		//------------------------------------------------------------------------------------------
		// 簡易統計情報・分析更新処理
		//	MakeShortTermBuff				: 短期系情報を生成・挿入する
		//	MakeLongTermBuff				: 長期系情報を生成・挿入する
		//------------------------------------------------------------------------------------------
		VOID				MakeShortTermBuff(_MyFDRBodyRec *pMyFDRBodyRec, _MyDevice *pMyDevice);
		VOID				MakeLongTermBuff(_MyDevice *pMyDevice);
//---
	public:
		_MyJARVis();
		~_MyJARVis();
//
		//------------------------------------------------------------------------------------------
		// 超簡易統計情報分析・クラス
		//------------------------------------------------------------------------------------------
		_MyJARVisAnalyze	MyJARVisAnalyze;									// JARVis 分析クラス
//---
		//------------------------------------------------------------------------------------------
		// パネル描画用のバッファ（一定時間毎の平均値を保存）
		//	・短期系情報を描画する為のバッファ@10Hz（60個）	-> 6秒
		//	・長期系情報を描画する為のバッファ（250個）		-> 250Pixcel分
		//		1 Pixcel / 1 Sec --->   250 Sec :  4 Min 10 Sec
		//		1 Pixcel / 2 Sec --->   500 Sec :  8 Min 20 Sec
		//		1 Pixcel / 3 Sec --->   750 Sec : 12 Min 30 Sec
		//		1 Pixcel / 5 Sec --->  1250 Sec : 20 Min 50 Sec
		//		1 Pixcel / 6 Sec --->  1500 Sec : 25 Min 00 Sec
		//------------------------------------------------------------------------------------------
		// 描画用短期系バッファ領域
		//------------------------------------------------------------------------------------------
#define	MY_JARVIS_DRAWBUFF_SHORT_NUM	60
		CircularBuffer<_MyShortTermRec, MY_JARVIS_DRAWBUFF_SHORT_NUM>	MyCircularShortBuff;
		//---
		//------------------------------------------------------------------------------------------
		// 描画用長期系バッファ領域
		//------------------------------------------------------------------------------------------
#define	MY_JARVIS_DRAWBUFF_LONG_NUM		250
		CircularBuffer<SHORT,			MY_JARVIS_DRAWBUFF_LONG_NUM>	MyCircularLongBuff;
		DateTime			dtLongBuffLastUpdate;								// 描画用長期系バッファ最終更新日時
//---
		//------------------------------------------------------------------------------------------
		// 簡易統計情報・分析更新処理
		//	Initialize					: 統計情報を初期化する
		//	AnalyzeFDRRec				: 統計分析を更新する
		//	QueJARVisMessage			: アナウンス情報を取得する
		//------------------------------------------------------------------------------------------
		VOID				Initialize(_MyDevice *pMyDevice);
		BOOL				AnalyzeFDRRec(_MyFDRBodyRec *pMyFDRBodyRec, _MyDevice *pMyDevice);
		BOOL				QueJARVisMessage(_MyDevice *pMyDevice, String *pstrMessage);
};
#endif
/*-- End Of File --*/