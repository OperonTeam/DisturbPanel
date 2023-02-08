//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyPanel/PanelSide/MyPanelSide.h
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#ifndef MYPANELSIDE_H
#define MYPANELSIDE_H

//--------------------------------------------------------------------------------------------------
// 外部参照・依存関係インクルード・ヘッダ情報
//--------------------------------------------------------------------------------------------------
#include "Utility/MyDrawScroll.h"											// Utility(Scroll)
#include "TopArea/MyPanelSideTop.h"											// Side Top
#include "LowArea/MyPanelSideLowMove.h"										// Side Low
#include "LowArea/MyPanelSideLowStop.h"										// Side Low

//--------------------------------------------------------------------------------------------------
// 情報表示（Ｓｉｄｅ）用ＬＣＤクラス
//--------------------------------------------------------------------------------------------------
class   _MyPanelSide {
	private:
		//------------------------------------------------------------------------------------------
		// ＬＧＦＸパネル・オブジェクト
		//-----------------------------------------------------------------------------------------
		_MyPanelSideTop		MyPanelSideTop;									// サイドパネル（上部）描画クラス
		_MyPanelSideLowMove	MyPanelSideLowMove;								// サイドパネル（下部）描画クラス
		_MyPanelSideLowStop	MyPanelSideLowStop;								// サイドパネル（下部）描画クラス
		//---
		CHAR				cDrivingCondition;								// 停止状態フラグ
		_MyComTimer			MyComTimer;										// 簡易タイマー

	public:
		_MyPanelSide();
		~_MyPanelSide();
		//
		//------------------------------------------------------------------------------------------
		// ＬＧＦＸの初期化
		//------------------------------------------------------------------------------------------
//		INLINE	VOID			MyLGFXSetup(_MyLGFX::_PanelType PanelType, INT iRotateMode) {
//									this->MyLGFX.LcdSetup(PanelType, iRotateMode);
//		};
//		INLINE	VOID			MyLGFXClear(VOID) {
//									this->MyLGFX.LcdClear();
//		};
		//---
		VOID					DrawTopSystemMarker(BOOL bDrawInitFlag, _MyLGFX *pMyLGFX, _MyDevice *pMyDevice);
		BOOL					DrawTopSystemConsole(BOOL bDrawInitFlag, _MyLGFX *pMyLGFX, _MyDevice *pMyDevice, _MyJARVis *pMyJARVis);
		VOID					DrawLowSystemArea(BOOL bDrawInitFlag, _MyLGFX *pMyLGFX, _MyDevice *pMyDevice, _MyJARVis *pMyJARVis);
};
#endif
/*-- End Of File --*/
