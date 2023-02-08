//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyTask/MyTask.cpp
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#include "MyBase.h"
#include "MyDevice.h"
#include "MyJARVis.h"

//--------------------------------------------------------------------------------------------------
// xTaskCreateで生成されるタスク処理の割り込み先
//--------------------------------------------------------------------------------------------------
extern	_MyDevice	MyDevice;
extern	_MyJARVis	MyJARVis;

//--------------------------------------------------------------------------------------------------
// xTaskCreateで生成されるタスク処理の割り込み先（の実態を定義する関数）
// 注意：
//	同じコード・ファイル内て定義している為、ついつい_MyTaskクラス・Privateメンバに直接アクセス
//	したくなるのですが、この関数内に引き渡している_MyTaskハンドルでは、_MyTaskクラス・Privateメンバ
//	にはアクセス出来ません。
//--------------------------------------------------------------------------------------------------
VOID			MyTaskMainProc(
VOID			*arg)
{
	//----------------------------------------------------------------------------------------------
	// ユーザタスク・メインプロシージャーの初期処理を実施します。
	//----------------------------------------------------------------------------------------------
	_MyTask		*pMyTask = (_MyTask*)arg;
	_MyTask::_MyTaskRet	ThisMyTaskRet = pMyTask->MyTaskMainProcInitProcedure();

	//----------------------------------------------------------------------------------------------
	// ユーザタスク・プロシージャー処理を実行する
	//----------------------------------------------------------------------------------------------
	BOOL	bMyLoopFlag = MY_TRUE;
	while(bMyLoopFlag == MY_TRUE) {
		//------------------------------------------------------------------------------------------
		// ループの前処理を実行する
		//------------------------------------------------------------------------------------------
		ULONG	_ulMillisStart = millis();									// 処理実行開始時間を保存

		//------------------------------------------------------------------------------------------
		// 前処理で取得したタスク状態情報に従って、ユーザ(Override)定義メソッドを実行する
		//------------------------------------------------------------------------------------------
		switch(ThisMyTaskRet) {
			//--------------------------------------------------------------------------------------
			// 未オープン（未初期化）
			//--------------------------------------------------------------------------------------
			case	_MyTask::_MyTaskRet::MY_DEVICE_STATUS_NOTOPEN:
				ThisMyTaskRet = pMyTask->MyBegin(&MyDevice, &MyJARVis);		// デバイスのオープン処理
				break;
			//--------------------------------------------------------------------------------------
			// タイムアウト（オープン済みで強制的に長時間リソース開放）
			//--------------------------------------------------------------------------------------
			case	_MyTask::_MyTaskRet::MY_DEVICE_STATUS_LONGSLEEP:
				ThisMyTaskRet = _MyTask::_MyTaskRet::MY_DEVICE_STATUS_OPEN;
				// 	break;		このbreakのコメントアウトはコンパイラがエラーを吐き出すかも。
			//--------------------------------------------------------------------------------------
			// オープン済み（通常デバイス運用状態）
			//--------------------------------------------------------------------------------------
			case	_MyTask::_MyTaskRet::MY_DEVICE_STATUS_OPEN:
				ThisMyTaskRet = pMyTask->MyUpdate(&MyDevice, &MyJARVis);	// デバイスのメイン処理
				break;
			//--------------------------------------------------------------------------------------
			// デバイスセットアップ
			//--------------------------------------------------------------------------------------
			case	_MyTask::_MyTaskRet::MY_DEVICE_STATUS_SETUP:
				ThisMyTaskRet = pMyTask->MySetup(&MyDevice, &MyJARVis);		// デバイスのセットアップ処理
				break;
			//--------------------------------------------------------------------------------------
			// デバイス終了
			//--------------------------------------------------------------------------------------
			case	_MyTask::_MyTaskRet::MY_DEVICE_STATUS_CLOSE:
			default:
				ThisMyTaskRet = pMyTask->MyQuit(&MyDevice, &MyJARVis);		//　デバイスのクローズ処理
				bMyLoopFlag = MY_FALSE;
				break;
		}
		//----------------------------------------------------------------------------------------------
		//　ユーザタスク・メインプロシージャー実行後の後処理を行う（休止処理）
		//----------------------------------------------------------------------------------------------
		pMyTask->MyTaskMainProcAfterProcedure(ThisMyTaskRet, _ulMillisStart);
	}
	//----------------------------------------------------------------------------------------------
	// ユーザタスク・プロシージャー処理を終了します。（現在は未到達ロジックです）
	//----------------------------------------------------------------------------------------------
    vTaskDelete(NULL);
}
/**/
//--------------------------------------------------------------------------------------------------
// クラス初期メソッド（基底クラス）
//--------------------------------------------------------------------------------------------------
_MyTask::_MyTask()
{
	this->HandleTask = NULL;												// TASK ハンドル
	this->MyTaskRet = _MyTask::_MyTaskRet::MY_DEVICE_STATUS_NOTOPEN;		// デバイス処理状態
	this->ulMillisUserTimer	= 0;											// デバイス処理・初回開始時刻（millis）
#ifdef	MY_TASK_EXECUTION_STATISTICS
	this->ulProcRunCount = 0;												// デバイス処理・処理回数
	this->ulMillisStart		= 0;											// デバイス処理・処理開始時刻（millis）
	this->ulMillisFirst		= 0;											// デバイス処理・実行処理の開始（初回）時間
	this->ulTotalMillisProc	= 0;											// デバイス処理・総処理終了時刻（millis）
	this->ulTotalMillisWait	= 0;											// デバイス処理・総待機時間の合計（millis）
#endif
}
/**/
_MyTask::~_MyTask()
{
}
/**/
//--------------------------------------------------------------------------------------------------
// タスクの生成メソッド
//--------------------------------------------------------------------------------------------------
BaseType_t		_MyTask::MyTaskCreate(
CONST_CHAR		*_pszTaskName,												// 生成タスク名称
USHORT			_usStackDepth,												// RTOS タスクスタックサイズ
UBaseType_t		_ulPriority,												// RTOS タスク優先順位
INT				_iFrequency,												// 期待する繰り返し数（Hz）
_MyTask			*_pMyTaskChild)												// RTOS タスクハンドラ
{
	//----------------------------------------------------------------------------------------------
	// 入力パラメータ（タスク情報）を保存
	//----------------------------------------------------------------------------------------------
	this->iProcedureTime	= (1000 / _iFrequency);							// クロックから逆算（ユーザタスク時間は0換算）
	this->iMinWaitTime		= (1000 / _iFrequency);							// Delay()最低保証値
	
	//----------------------------------------------------------------------------------------------
	// タスクを生成する(FreeRTOS)
	//----------------------------------------------------------------------------------------------
	BaseType_t		MyBaseTypeRet;
	MyBaseTypeRet = xTaskCreate(MyTaskMainProc, _pszTaskName, _usStackDepth, _pMyTaskChild, tskIDLE_PRIORITY + _ulPriority, &(this->HandleTask));
	if (MyBaseTypeRet == pdPASS) {
		//------------------------------------------------------------------------------------------
		// タスク生成に成功した場合は、カレントタスクよりも優先順位の低いタスクを生成した場合を
		// 考慮して、呼び出し元タスクの待機を行います。
		//------------------------------------------------------------------------------------------
		delay(100);
	}
	return	MyBaseTypeRet;
}
/**/
//==================================================================================================
// Virtual Method
// 継承先からオーバーライドする為、基底クラスのメソッドはこんな感じで超絶省略
//==================================================================================================
_MyTask::_MyTaskRet	_MyTask::MyBegin(_MyDevice *pMyDevice, _MyJARVis *pMyJARVis)	{	return	_MyTask::_MyTaskRet::MY_DEVICE_STATUS_OPEN;	}
_MyTask::_MyTaskRet	_MyTask::MyUpdate(_MyDevice *pMyDevice, _MyJARVis *pMyJARVis)	{	return	_MyTask::_MyTaskRet::MY_DEVICE_STATUS_OPEN;	}
_MyTask::_MyTaskRet	_MyTask::MySetup(_MyDevice *pMyDevice, _MyJARVis *pMyJARVis)	{	return	_MyTask::_MyTaskRet::MY_DEVICE_STATUS_OPEN;	}
_MyTask::_MyTaskRet	_MyTask::MyQuit(_MyDevice *pMyDevice, _MyJARVis *pMyJARVis)		{	return	_MyTask::_MyTaskRet::MY_DEVICE_STATUS_OPEN; }
/**/
//--------------------------------------------------------------------------------------------------
//　ユーザタスク・メインプロシージャー実行前の初期処理を行う
//--------------------------------------------------------------------------------------------------
_MyTask::_MyTaskRet	_MyTask::MyTaskMainProcInitProcedure(VOID)
{
	//----------------------------------------------------------------------------------------------
	// _MyTaskクラス　Privateメンバーの初期化を行う
	//----------------------------------------------------------------------------------------------
#ifdef	MY_TASK_EXECUTION_STATISTICS
	this->ulMillisStart = millis();
	this->ulMillisFirst = this->ulMillisStart;
#endif
	return	this->MyTaskRet;
}
/**/
//--------------------------------------------------------------------------------------------------
//　ユーザタスク・メインプロシージャー実行後の後処理を行う
//--------------------------------------------------------------------------------------------------
VOID				_MyTask::MyTaskMainProcAfterProcedure(
_MyTask::_MyTaskRet	ThisMyTaskRet,
ULONG				_ulMillisStart)
{
	//----------------------------------------------------------------------------------------------
	// タスク保存情報を最新化します。
	//----------------------------------------------------------------------------------------------
#ifdef	MY_TASK_EXECUTION_STATISTICS
	this->ulProcRunCount += 1;												// 「実行処理回数」をカウントアップ
	this->ulTotalMillisProc	+= ulProcTime;									// 「総処理時間」に加算
#endif
	//----------------------------------------------------------------------------------------------
	// 1回のloop処理に期待する処理時間(mSec) から今回の処理時間を減算して、今回の休止時間を求める
	// 直前のタスク処理結果がLONGSLEEP（長時間待機）の場合は、無条件に1秒待機させる
	//----------------------------------------------------------------------------------------------
	LONG	lWaitTime = MY_TASK_USERALERT_WAIT_1SEC;						// 長時間待機(1000msec)
	if (ThisMyTaskRet != _MyTask::_MyTaskRet::MY_DEVICE_STATUS_LONGSLEEP) {
		//------------------------------------------------------------------------------------------
		// 通常タスク処理における休止時間を求めます。
		// →　今回のタスク実行時間　＋　タスク休止時間　＝　期待処理時間
		// 　　タスク実行時間が期待処理時間を超えている場合は、最低保障タイマ値を設定。
		//------------------------------------------------------------------------------------------
		lWaitTime = this->iProcedureTime - (millis() - _ulMillisStart);
		if (lWaitTime < 0)	lWaitTime = this->iMinWaitTime;
	}
	//----------------------------------------------------------------------------------------------
	// タスクを休止する
	//----------------------------------------------------------------------------------------------
	vTaskDelay(lWaitTime);

	//----------------------------------------------------------------------------------------------
	// タスクの実行種別を更新します
	//----------------------------------------------------------------------------------------------
	this->MyTaskRet = ThisMyTaskRet;
#ifdef	MY_TASK_EXECUTION_STATISTICS
	this->ulTotalMillisWait += lWaitTime;									// 「総待機時間」に加算
#endif
}
/**/
//--------------------------------------------------------------------------------------------------
// ユーザ・タイマー処理を開始する
//--------------------------------------------------------------------------------------------------
VOID			_MyTask::InitUserAlert(VOID)
{
	this->ulMillisUserTimer = millis();										// ユーザータイマー値を初期化する
}
/**/
//--------------------------------------------------------------------------------------------------
// ユーザ・タイマー処理から、指定時間が経過したか判断する
//--------------------------------------------------------------------------------------------------
BOOL			_MyTask::IsUserAlert(ULONG _ulTimer)
{
	if (this->ulMillisUserTimer + _ulTimer < millis())	return	MY_TRUE;
	return	MY_FALSE;
}
/**/
#ifdef	MY_TASK_EXECUTION_STATISTICS
//--------------------------------------------------------------------------------------------------
// 開発用：タスク実行統計情報系のアクセスメソッド
//	MyTaskQueStack		:	カレントタスクの空きスタックサイズを求める
//	MyTaskQueName		:	カレントタスクのタスク名称を求める
//	GetRunCount			:	タスク処理の実行ループ回数を取得する
//	GetRunTimePerProc	:	タスク処理の平均実行時間（処理実行時間の合計／実行回数）
//	GetRunWaitPerProc	:	タスク処理の平均待機時間（処理待機時間の合計／実行回数）
//	GetRunProcPerTime	:	タスク処理の実行回数（１秒当たり）
//	GetExpectedTime		:	1回のloop処理に期待する処理時間(mSec)
//	GetMinTime			:	最低保障(Delay)タイマ値(mSec)
//	MyDumpBase			:	ダンプ
//--------------------------------------------------------------------------------------------------
UINT			_MyTask::MyTaskQueStack(VOID)
{
	INT		iStackFreeByte = 0;
	if(this->HandleTask) {
		iStackFreeByte = uxTaskGetStackHighWaterMark(this->HandleTask);		// タスクの空きスタックサイズを取得
	}
	return	iStackFreeByte;
}
String			_MyTask::MyTaskQueName(VOID)		{	return	this->strTaskName;																			}
ULONG 		  	_MyTask::GetRunCount(VOID)			{	return	this->ulProcRunCount;																		}
FLOAT			_MyTask::GetRunTimePerProc(VOID)	{	return	(FLOAT) (((FLOAT)this->ulTotalMillisProc / (FLOAT)this->ulProcRunCount) / 1000);			}
FLOAT			_MyTask::GetRunWaitPerProc(VOID)	{	return	(FLOAT) (FLOAT)this->ulTotalMillisWait / (FLOAT)this->ulProcRunCount; 						}
FLOAT			_MyTask::GetRunProcPerTime(VOID)	{	FLOAT	ufTime = millis() - this->ulMillisFirst;	return	ufTime / (FLOAT)this->ulProcRunCount; 	}
ULONG			_MyTask::GetExpectedTime(VOID)		{	return	this->ulExpectedTime;																		}
ULONG			_MyTask::GetMinTime(VOID)			{	return	this->ulMinTime;																			}
/**/
#ifdef	MY_DEBUG_PRINT
//--------------------------------------------------------------------------------------------------
// 内部情報ダンプ出力
//--------------------------------------------------------------------------------------------------
VOID			_MyTask::MyDumpBase(VOID)
{
	MY_LOG("Task Name:[%8s] ",		this->strTaskName.c_str());
	MY_LOG("- Status:%3d ",			this->MyTaskRet);
	MY_LOG("/ Stack:%6u ",			this->MyTaskQueStack());
	MY_LOG("- RunCount:%6ld ",		this->GetRunCount());
	MY_LOG("/ ProcTime Ave(%7.2f) ",this->GetRunTimePerProc());
	MY_LOG("/ WaitTime Ave(%7.2f)",	this->GetRunWaitPerProc());
	MY_LOG("/ Count/Sec %7.2f",		this->GetRunProcPerTime());
	MY_LOG("/ FreeHeap %ld",		xPortGetFreeHeapSize());
	MY_LOG("/ MaxHeap %ld",			xPortGetMinimumEverFreeHeapSize());
	MY_LOG("\n");
}
#endif
#endif
/*-- End Of File --*/
