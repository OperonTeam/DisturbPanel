//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyTool/Com/MyComTimer.cpp
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#include "MyBase.h"

//--------------------------------------------------------------------------------------------------
// クラスインスタンスの初期処理
//--------------------------------------------------------------------------------------------------
_MyComTimer::_MyComTimer()
{
	this->sEventNum			= 0;											// 管理するイベント数
	this->ulIntervalTimer	= (ULONG)0;										// タイマ値（millis）
	this->ulStartTime		= (ULONG)0;										// タイマ開始（millis）
	this->sEventIndex		= 0;											// 現状のイベント番号
}
/**/
//--------------------------------------------------------------------------------------------------
// クラスインスタンスの終了処理
//--------------------------------------------------------------------------------------------------
_MyComTimer::~_MyComTimer()
{
}
/**/
//--------------------------------------------------------------------------------------------------
// 簡易タイマーの初期化を行う
//--------------------------------------------------------------------------------------------------
VOID			_MyComTimer::InitTimer(
SHORT			_sEventNum,
ULONG			_ulTimer)
{
	if (! this->sEventNum) {
		//-------------------------------------------------------------------------------------------
		// 入力パラメータを退避して、初回タイマ値を保存
		//-------------------------------------------------------------------------------------------
		this->sEventNum			= _sEventNum;								// 管理するイベント数
		this->ulIntervalTimer	= _ulTimer;									// タイマ値（millis）
		//---
		this->ulStartTime		= millis();									// 簡易タイマ開始時間
		this->sEventIndex		= 0;										// 現状のイベント番号
	}
}
/**/
//--------------------------------------------------------------------------------------------------
// 簡易タイマーイベントの取得
//----
//	VOID	_MyComTimer::InitTimer(3, 1000);
//	SHORT	_MyComTimer::QueTimer(&bFlag);			1秒間隔で、0から3までの値が取得される
//--------------------------------------------------------------------------------------------------
SHORT			_MyComTimer::QueTimer(
BOOL			*pbChangeFlag)
{
	ULONG	ulNow		= millis();
//---
	//----------------------------------------------------------------------------------------------
	// タイマ時刻の到来を確認する
	//----------------------------------------------------------------------------------------------
	if (this->ulStartTime + this->ulIntervalTimer < ulNow) {
		//------------------------------------------------------------------------------------------
		// 現在時間が、「前回タイマ到来時間＋タイマ値」を超えた場合
		//------------------------------------------------------------------------------------------
		this->ulStartTime = ulNow;											// 前回タイマ到来時間を現在時間で更新
		this->sEventIndex += 1;												// 通知タイマ番号を加算　
		if (this->sEventIndex > this->sEventNum)	this->sEventIndex = 0;	//
		//---
		if (pbChangeFlag)	*pbChangeFlag += MY_TRUE;						// 更新イベントをオン
	}
	//----------------------------------------------------------------------------------------------
	// 現在のタイマ番号を返却する
	//----------------------------------------------------------------------------------------------
	return	this->sEventIndex - 1;
}
/*-- End Of File --*/
