//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyPanel/PanelSide/TopArea/MyPanelSideTop.h
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#ifndef MYPANELSIDETOP_H
#define MYPANELSIDETOP_H

//--------------------------------------------------------------------------------------------------
// 外部参照・依存関係インクルード・ヘッダ情報
//--------------------------------------------------------------------------------------------------
#include "MyDevice.h"														// Device
#include "MyJARVis.h"														// JARVis
#include "LGFX/MyLGFX.h"													// LovyanGFX Wrapper
#include "Utility/MyDrawScroll.h"											// Scroll

//--------------------------------------------------------------------------------------------------
// 情報表示（サイドパネル）上層レイアウト・描画クラス
//--------------------------------------------------------------------------------------------------
class   _MyPanelSideTop {
	private:
		//------------------------------------------------------------------------------------------
		// 描画用オブジェクト
		//------------------------------------------------------------------------------------------
		BOOL					bSystemInfoStatus;							// 
		DateTime				dtSystemMsgDateTime;						//
		ULONG					ulUpptimeTotalSecond;						//
		//---
		//
		//---
		_MyDrawScroll			MyDrawScroll;
		//
		_MyComTimer				MyComTimer;									//
		SHORT					GetDevStatusColor(_MyDEV_STAT MyFDELogID);
//
	public:
		//------------------------------------------------------------------------------------------
		// メインどころのメソッド
		//------------------------------------------------------------------------------------------
		_MyPanelSideTop();
		~_MyPanelSideTop();
//---

		//------------------------------------------------------------------------------------------
		//	DrawSystemTime			: 現在システム時刻描画
		//	DrawSystemUptime		: 現在システム起動時間描画
		//------------------------------------------------------------------------------------------
		VOID					DrawSystemTime(_MyLGFX *pMyLGFX, BOOL bDrawFlag, _MyDevice *pMyDevice);
		VOID					DrawSystemUptime(_MyLGFX *pMyLGFX, BOOL bDrawFlag, _MyJARVis *pMyJARVis);

		//------------------------------------------------------------------------------------------
		// TopArea :
		//	DrawSystemDevStatus		: システムデバイス・マーカーの描画
		//	DrawThermoMap			: Thermoイメージを描画
		//	DrawSystemInfoMessage	: システム情報メッセージを描画
		//------------------------------------------------------------------------------------------
		BOOL					DrawSystemDevStatus(_MyLGFX *pMyLGFX, BOOL bDrawFlag, _MyDevice *pMyDevice);
		VOID					DrawThermoMap(_MyLGFX *pMyLGFX, BOOL bDrawFlag, _MyDevice *pMyDevice);
		VOID					DrawSystemInfoMessage(_MyLGFX *pMyLGFX, BOOL bDrawFlag, _MyDevice *pMyDevice, _MyJARVis *pMyJARVis);
};
#endif
/*-- End Of File --*/
