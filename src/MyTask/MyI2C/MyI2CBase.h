//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyTask/MyI2C/MyI2CBase.h
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#ifndef MYI2CBASE_H
#define MYI2CBASE_H

//--------------------------------------------------------------------------------------------------
// Ｉ２Ｃデバイス用基底クラス
//--------------------------------------------------------------------------------------------------
class   _MyI2CBase {
	protected:
		//------------------------------------------------------------------------------------------
		// Ｉ２Ｃセンサー用共通情報
		//------------------------------------------------------------------------------------------
		_MyDEV_ID				MyDevIdI2C;							// Ｉ２ＣセンサーＩＤ
		TwoWire					*pI2CTwoWire;						// Ｉ２Ｃバス
		UCHAR					ucI2CAddress;						// Ｉ２Ｃアドレス
		//---
		ULONG					ulChipRunningCount;					// Chip処理回数
		ULONG					ulTotalMillisTime;					// Updateセンサー総処理時間(millis)
		ULONG					ulLastProcessingTime;				// 前回処理を行った時間(millis) 
		//---
		ULONG					ulThisUpdateStartTime;				// 処理開始時の開始時間(millis)

	public:
		_MyI2CBase();
		~_MyI2CBase();
		//---
		//------------------------------------------------------------------------------------------
		// Ｉ２Ｃセンサー用基底メソッド
		//	BeginBaseInit					センサー情報更新基底処理：初期処理を行う
		//	UpdateBaseStart					センサー情報更新基底処理：事前処理を行う
		//	UpdateBaseExit					センサー情報更新基底処理：事後処理を行う
		//	MyDump							センサー情報のダンプを行う
		//------------------------------------------------------------------------------------------
		VOID					BeginBaseInit(_MyDEV_ID MyDevID, UCHAR ucAddress, TwoWire *pTwoWire);
		VOID					UpdateBaseStart(VOID);
		VOID					UpdateBaseExit(VOID);
		VOID					MyDump(VOID);
};
#endif
/*-- End Of File --*/
