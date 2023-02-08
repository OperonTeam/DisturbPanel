//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyTool/Com/MyComTimer.h
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#ifndef MYCOMTIMER_H
#define MYCOMTIMER_H

//--------------------------------------------------------------------------------------------------
// 簡易タイマー
//--------------------------------------------------------------------------------------------------
class	_MyComTimer {
	private:
		//------------------------------------------------------------------------------------------
		// 作業領域
		//------------------------------------------------------------------------------------------
		SHORT			sEventNum;											// 管理するイベント数
		ULONG			ulIntervalTimer;									// タイマ値（millis）
		//---
		ULONG			ulStartTime;										// タイマ開始（millis）
		SHORT			sEventIndex;										// 現状のイベント番号
	public:
		_MyComTimer();
		~_MyComTimer();
		VOID			InitTimer(SHORT sEventNum, ULONG ulTimer);			// 簡易タイマーの初期化
		SHORT			QueTimer(BOOL *pbChangeFlag = NULL);				// タイマーイベントの取得
};
#endif
/*-- End Of File --*/
