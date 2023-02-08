//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyTask/MyTask.h
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#ifndef MYTASK_H
#define MYTASK_H

//--------------------------------------------------------------------------------------------------
// 前方定義(クラスの構造が悪いってこと）
//--------------------------------------------------------------------------------------------------
class	_MyDevice;
class	_MyJARVis;

//--------------------------------------------------------------------------------------------------
// タスク生成時のプロシージャ定義
//--------------------------------------------------------------------------------------------------
extern "C" {
	VOID		MyTaskMainProc(VOID	*arg);
}

//--------------------------------------------------------------------------------------------------
// アラート用タイマー定義
//--------------------------------------------------------------------------------------------------
#define	MY_TASK_USERALERT_WAIT_1SEC		1000

//--------------------------------------------------------------------------------------------------
// タスクの優先順位
//--------------------------------------------------------------------------------------------------
#define	MY_TASK_TOP_URGENCY				8
#define	MY_TASK_MID_URGENCY				7
#define	MY_TASK_LOW_URGENCY				6

//--------------------------------------------------------------------------------------------------
// 周辺デバイス管理（基底）クラス
//--------------------------------------------------------------------------------------------------
class   _MyTask {
	public:
		_MyTask();
		~_MyTask();
//
		//------------------------------------------------------------------------------------------
		// 仮想メソッド（必要に応じて継承先クラスによるオーバーライドすること）
		// デバイス処理状態定義 : _MyStatus
		//------------------------------------------------------------------------------------------
		enum _MyTaskRet {
			MY_DEVICE_STATUS_NOTOPEN		= -99,									// 未オープン（未初期化）
			MY_DEVICE_STATUS_OPEN			= 0,									// オープン済み
			MY_DEVICE_STATUS_SETUP			= 10,									// デバイス・セットアップ
			MY_DEVICE_STATUS_LONGSLEEP		= 60,									// デバイス・時間待ち
			MY_DEVICE_STATUS_CLOSE			= 90									// デバイス・終了
		};
		virtual	_MyTaskRet	MyBegin(_MyDevice *pMyDevice, _MyJARVis *pMyJARVis);	// Overwrite用の自前メソッド
		virtual	_MyTaskRet	MyUpdate(_MyDevice *pMyDevice, _MyJARVis *pMyJARVis);	// Overwrite用の自前メソッド
		virtual	_MyTaskRet	MySetup(_MyDevice *pMyDevice, _MyJARVis *pMyJARVis);	// Overwrite用の自前メソッド
		virtual	_MyTaskRet	MyQuit(_MyDevice *pMyDevice, _MyJARVis *pMyJARVis);		// Overwrite用の自前メソッド

		//------------------------------------------------------------------------------------------
		// RTOSタスク管理系メソッド
		//------------------------------------------------------------------------------------------
		BaseType_t			MyTaskCreate(CONST_CHAR *pszTaskName, USHORT usStackDepth, UBaseType_t ulPriority, INT iFrequency, _MyTask *pMyTaskChild);
		VOID				InitUserAlert(VOID);									// ユーザータイマー値を初期化する
		BOOL				IsUserAlert(ULONG ulTimer);								// 指定時間が経過したか判断する
//---
		//------------------------------------------------------------------------------------------
		// MyTaskクラスPrivateメンバへのインタフェース
		//	MyTaskMainProcInitProcedure	: Task処理関数から、_MyTaskクラスPrivateメンバを初期化する
		//	MyTaskMainProcAfterProcedure: Task処理関数から、_MyTaskクラスPrivateメンバを更新する
		//------------------------------------------------------------------------------------------
		_MyTask::_MyTaskRet	MyTaskMainProcInitProcedure(VOID);
		VOID				MyTaskMainProcAfterProcedure(_MyTask::_MyTaskRet ThisMyTaskRet, ULONG _ulMillisStart);
//---
		VOID				MyDumpBase(VOID);
//---
	private:
		//------------------------------------------------------------------------------------------
		// タスク生成時のパラメータ保存領域
		//------------------------------------------------------------------------------------------
		INT					iProcedureTime;											// 1回のloop処理に期待する処理時間(mSec)
		INT					iMinWaitTime;											// 最低保障(Delay)タイマ値(mSec)
//
		//------------------------------------------------------------------------------------------
		// RTOS タスクハンドル
		//------------------------------------------------------------------------------------------
		TaskHandle_t		HandleTask;												// TaskHandle
//		_MyTask::_MyTaskRet	MyTaskRet;												// デバイス処理状態
		_MyTaskRet			MyTaskRet;												// デバイス処理状態
		ULONG				ulMillisUserTimer;										// デバイス処理・ユーザタイマー
//
#ifdef	MY_TASK_EXECUTION_STATISTICS
		//------------------------------------------------------------------------------------------
		// 生成タスクの実行統計情報系メソッド（開発用の参照メソッド）
		//------------------------------------------------------------------------------------------
		ULONG				ulProcRunCount;											// デバイス処理・実行回数カウンタ
		ULONG				ulMillisFirst;											// デバイス処理・初回開始時刻（millis）
		ULONG				ulMillisStart;											// デバイス処理・処理開始時刻（作業用）
		ULONG				ulTotalMillisProc;										// デバイス処理・処理時間の合計
		U                                bbbbbbbb bbn     bLONG				ulTotalMillisWait;										// デバイス処理・待機間合の合計
//---  nb
		UINT				MyTaskQueStack(VOID);									// タスクスタックサイズ
		String				MyTaskQueName(VOID);									// タスク名称を取得
		ULONG				GetRunCount(VOID);										// タスク処理の実行ループ回数を取得する
		FLOAT				GetRunTimePerProc(VOID);								// タスク                             処理の平均実行時間（処理実行時間の合計／実行回数）
		FLOAT				GetRunWaitPerProc(VOID);								// タスク処理の平均待機時間（処理待機時間の合計／実行回数）
		FLOAT				GetRunProcPerTime(VOID);								// タスク処理の実行回数（１秒当たり）
		ULONG				GetExpectedTime(VOID);									// 1回のloop処理に期待する処理時間(mSec)
		ULONG				GetMinTime(VOID);										// 最低保障(Delay)タイマ値(mSec)
#endif
};
#endif
/*-- End Of File --*/
