//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyPanel/PanelSide/LowArea/MyPanelSideLowMove.h
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#ifndef MYPANELSIDELOWMOVE_H
#define MYPANELSIDELOWMOVE_H

//--------------------------------------------------------------------------------------------------
// 外部参照・依存関係インクルード・ヘッダ情報
//--------------------------------------------------------------------------------------------------
#include "MyDevice.h"														// Device
#include "MyJARVis.h"														// JARVis
#include "LGFX/MyLGFX.h"													// LovyanGFX Wrapper

//--------------------------------------------------------------------------------------------------
// 情報表示（サイドパネル）用ＬＣＤクラス
//--------------------------------------------------------------------------------------------------
class   _MyPanelSideLowMove {
	private:
		//------------------------------------------------------------------------------------------
		// 描画用オブジェクト
		//------------------------------------------------------------------------------------------
		DateTime				dtLongBuffLastUpdate;						// 描画用長期系バッファ最終更新日時
		SHORT					sLongBuffHistoryMin;						// 描画用長期系最低速度
		SHORT					sLongBuffHistoryMax;						// 描画用長期系最高速度
		//---
		SHORT					sSpeedHistoryMin;							// 速度経緯オブジェクトの最低速度
		SHORT					sSpeedHistoryMax;							// 速度経緯オブジェクトの最高速度

		//------------------------------------------------------------------------------------------
		//	DrawLogSpeedInfoHistory	: 移動速度遷移描画（長期系）
		//	DrawLogSpeedInfoAccell	: 加減速度描画（短期系）
		//	GetSpeedColor			: 移動速度描画表示色
		//------------------------------------------------------------------------------------------
		VOID					DrawLogSpeedInfoHistory(_MyLGFX *pMyLGFX, BOOL bDrawFlag, _MyDevice *pMyDevice, _MyJARVis *pMyJARVis);
		VOID					DrawLogSpeedInfoAccell(_MyLGFX *pMyLGFX, BOOL bDrawFlag, _MyDevice *pMyDevice, _MyJARVis *pMyJARVis);
		INT						GetSpeedColor(LONG lSpeed);
//---
	public:
		//------------------------------------------------------------------------------------------
		// メインどころのメソッド
		//------------------------------------------------------------------------------------------
		_MyPanelSideLowMove();
		~_MyPanelSideLowMove();
//
		//------------------------------------------------------------------------------------------
		// LowArea :
		//	DrawLogSpeedInfo		: 移動速度推移描画
		//------------------------------------------------------------------------------------------
		VOID					DrawLogSpeedInfo(_MyLGFX *pMyLGFX, BOOL bDrawFlag, _MyDevice *pMyDevice, _MyJARVis *pMyJARVis);
};
#endif
/*-- End Of File --*/
