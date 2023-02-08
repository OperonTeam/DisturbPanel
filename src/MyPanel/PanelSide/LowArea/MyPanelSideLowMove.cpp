//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyPanel/PanelSide/LowArea/MyPanelSideLowMove.cpp
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#include "MyBase.h"
#include "MyDevice.h"
#include "MyJARVis.h"
#include "MyPanelSideLowMove.h"

//--------------------------------------------------------------------------------------------------
// クラスインスタンスの初期処理
//--------------------------------------------------------------------------------------------------
_MyPanelSideLowMove::_MyPanelSideLowMove()
{
	this->dtLongBuffLastUpdate			= DateTime(2000,1,1,0,0,0);
	this->sLongBuffHistoryMin			= (SHORT)9999;
	this->sLongBuffHistoryMax			= (SHORT)0;
	//---
	this->sSpeedHistoryMin				= (SHORT)9999;								// 速度経緯オブジェクトの最低速度
	this->sSpeedHistoryMax				= (SHORT)0;									// 速度経緯オブジェクトの最高速度
}
/**/
//--------------------------------------------------------------------------------------------------
// クラスインスタンスの終了処理
//--------------------------------------------------------------------------------------------------
_MyPanelSideLowMove::~_MyPanelSideLowMove()
{
}
/**/
//--------------------------------------------------------------------------------------------------
// 移動速度遷移描画（長期系）
//--------------------------------------------------------------------------------------------------
VOID				_MyPanelSideLowMove::DrawLogSpeedInfoHistory(
_MyLGFX				*pMyLGFX,
BOOL				bDrawFlag,
_MyDevice			*pMyDevice,
_MyJARVis			*pMyJARVis)
{
	//----------------------------------------------------------------------------------------------
	// 現在描画中の情報とセンサー情報を比較する（つまり再描画が必要か否か？）
	//----------------------------------------------------------------------------------------------
	bDrawFlag += MyGlobalTool.IfSetDifferent(pMyJARVis->dtLongBuffLastUpdate, &this->dtLongBuffLastUpdate);
	bDrawFlag += MyGlobalTool.IfSetDifferent(this->sSpeedHistoryMax, &this->sLongBuffHistoryMax);
	bDrawFlag += MyGlobalTool.IfSetDifferent(this->sSpeedHistoryMin, &this->sLongBuffHistoryMin);
	if (bDrawFlag) {
		//------------------------------------------------------------------------------------------
		// 長期系列の移動速度推移グラフを表示する
		//------------------------------------------------------------------------------------------
		SHORT	sSpeedHight = (SHORT)0;												// 速度描画グラフＹ軸値
		SHORT	sWorkMax	= this->sSpeedHistoryMax;								// 最高速度情報を初期化
		SHORT	sWorkMin	= this->sSpeedHistoryMin;								// 最低速度情報を初期化
		INT		iW = 252;
		INT		iH = 137;
		pMyLGFX->SpriteCreate(iW, iH, 4);
		//---
		//------------------------------------------------------------------------------------------
		// 速度遷移描画用・バッファ領域（左側・長期系列）を作成する
		//------------------------------------------------------------------------------------------
		INT	iStoreCount = pMyJARVis->MyCircularLongBuff.size();		// データの保存件数
		for (INT iX = 0; iX < iW - 2; iX++) {
			//--------------------------------------------------------------------------------------
			// 画面描画用・移動速度情報を取得する（速度を１０倍した値が保存されている）
			//--------------------------------------------------------------------------------------
			if (iX < iStoreCount) {
				SHORT	sSpeed = (SHORT)pMyJARVis->MyCircularLongBuff[iX];
				sSpeedHight = map(sSpeed, this->sSpeedHistoryMin, this->sSpeedHistoryMax, 0, (iH - 2) * 0.9);
				//---
				sWorkMax = max(sWorkMax, sSpeed);									// 最高速度を更新
				sWorkMin = min(sWorkMin, sSpeed);									// 最低速度を更新
				//---
				//----------------------------------------------------------------------------------
				// 速度遷移値を指定カラーで描画する
				//----------------------------------------------------------------------------------
				pMyLGFX->SpriteDrawFastVLine(iX + 1, iH - sSpeedHight - 1, sSpeedHight, this->GetSpeedColor((LONG)sSpeed/10));
			}
			//---
			//--------------------------------------------------------------------------------------
			// 垂直方向のスケールを描画する
			//--------------------------------------------------------------------------------------
			INT	iScaleLen = 10;
			if (!(iX % 10)) {
				if (!(iX % 100)) 	iScaleLen =  6;
				if (!(iX %  50))	iScaleLen = 18;
				pMyLGFX->SpriteDrawFastVLine(iX + 1,				  1, iScaleLen, MY_PALETTE_LIGHTGRAY);
				pMyLGFX->SpriteDrawFastVLine(iX + 1, iH - iScaleLen - 1, iScaleLen, MY_PALETTE_LIGHTGRAY);
			}
		}
		//---
		//------------------------------------------------------------------------------------------
		// 水平方向のスケールを描画する
		//------------------------------------------------------------------------------------------
		for (INT iY = 1; iY < 6; iY++) {
			pMyLGFX->SpriteDrawFastHLine(1, ((iH / 5 ) * iY) + 1, iW - 2, MY_PALETTE_DARKGRAY);
		}
		//---
		//------------------------------------------------------------------------------------------
		// 画面描画用・平均速度情報を取得する（速度を１０倍する）
		//------------------------------------------------------------------------------------------
		SHORT	sSpeed = (SHORT)ceil(pMyJARVis->MyJARVisAnalyze.QueJARVisSpeedAve() * 10.0);
		sSpeedHight = map(sSpeed, this->sSpeedHistoryMin, this->sSpeedHistoryMax, 0, (iH - 2) * 0.9);
		sWorkMax = max(sWorkMax, sSpeed);											// 最高速度を更新
		sWorkMin = min(sWorkMin, sSpeed);											// 最低速度を更新
		//
		//------------------------------------------------------------------------------------------
		// 平均速度値（水平線）を描画する
		//------------------------------------------------------------------------------------------
		pMyLGFX->SpriteDrawFastHLine(1, iH - sSpeedHight - 3, iW - 2, MY_PALETTE_YELLOW);
//		pMyLGFX->SpriteDrawFastHLine(1, iH - sSpeedHight - 2, iW - 2, MY_PALETTE_DARKGRAY);
		pMyLGFX->SpriteDrawFastHLine(1, iH - sSpeedHight - 1, iW - 2, MY_PALETTE_YELLOW);
		//
		//------------------------------------------------------------------------------------------
		// オブジェクト枠を描画する
		//------------------------------------------------------------------------------------------
		pMyLGFX->SpriteDrawRect(0, 0, iW, iH, MY_PALETTE_DARKGREEN);
		//
		//------------------------------------------------------------------------------------------
		// オフスクリーンバッファに生成した領域をＬＣＤパネルに出力（描画）する
		//------------------------------------------------------------------------------------------
		pMyLGFX->SpritePush(0, 103, MY_TRUE);
		//
		//------------------------------------------------------------------------------------------
		// 次回描画用のグラフ表示倍率を保存する
		//------------------------------------------------------------------------------------------
		this->sSpeedHistoryMax = sWorkMax;											// 最高速度を更新
		this->sSpeedHistoryMin = sWorkMin;											// 最低速度を更新
	}
}
/**/
//--------------------------------------------------------------------------------------------------
// 加減速度描画（短期系）
//--------------------------------------------------------------------------------------------------
VOID				_MyPanelSideLowMove::DrawLogSpeedInfoAccell(
_MyLGFX				*pMyLGFX,
BOOL				bDrawFlag,
_MyDevice			*pMyDevice,
_MyJARVis			*pMyJARVis)
{
	//----------------------------------------------------------------------------------------------
	// オフスクリーン・バッファ領域（右側・短期系列）を作成する
	//----------------------------------------------------------------------------------------------
	INT		iW = 68;
	INT		iH = 137;
	pMyLGFX->SpriteCreate(iW, iH, 4);
//---
	//----------------------------------------------------------------------------------------------
	// なんちゃってＧセンサー（現在加速・減速）のメモリを描画する
	//----------------------------------------------------------------------------------------------
	pMyLGFX->SpriteDrawCircle(iW / 2, iH / 2, 25, MY_PALETTE_BLUE);
	pMyLGFX->SpriteDrawCircle(iW / 2, iH / 2, 50, MY_PALETTE_BLUE);
	//---
	pMyLGFX->SpriteDrawLine(0,			iH / 2,		iW,		iH / 2, MY_PALETTE_BLUE);
	pMyLGFX->SpriteDrawLine(iW / 2,	0,			iW / 2, iH,		MY_PALETTE_BLUE);

	//----------------------------------------------------------------------------------------------
	// 現在速度値（水平線）を描画する
	//----------------------------------------------------------------------------------------------
	SHORT	sSpeed = (SHORT)(pMyJARVis->MyJARVisAnalyze.QueJARVisSpeedNow() * 10.0);			// map()の精度を高める
	SHORT	sSpeedHight = map(sSpeed, this->sSpeedHistoryMin, this->sSpeedHistoryMax, 0, (iH - 2) * 0.9);
	//---
	pMyLGFX->SpriteDrawFastHLine(2, iH - sSpeedHight - 3, iW -2, MY_PALETTE_WHITE);
//	pMyLGFX->SpriteDrawFastHLine(2, iH - sSpeedHight - 2, iW -2, MY_PALETTE_DARKGRAY);
	pMyLGFX->SpriteDrawFastHLine(2, iH - sSpeedHight - 1, iW -2, MY_PALETTE_WHITE);

	//----------------------------------------------------------------------------------------------
	// 現在加速・減速値（Ｇセンサー・ポジション）を描画する
	//----------------------------------------------------------------------------------------------
	for (INT iIndex = 0; iIndex < pMyJARVis->MyCircularShortBuff.size(); iIndex++) {
		FLOAT	fX = pMyJARVis->MyCircularShortBuff[iIndex].My3DFIMUACCEL.fRoll;
		FLOAT	fY = pMyJARVis->MyCircularShortBuff[iIndex].My3DFIMUACCEL.fPitch;
//		FLOAT	fZ = pMyJARVis->MyCircularShortBuff[iIndex].My3DFIMUACCEL.fYaw;
		INT		iX = (iW / 2) + (fX * 100.0);
		INT		iY = (iH / 2) + (fY * 100.0);
		pMyLGFX->SpriteFillCircle(iX, iY, 8, MY_PALETTE_RED);
		pMyLGFX->SpriteFillCircle(iX, iY, 5, MY_PALETTE_YELLOW);
	}

	//----------------------------------------------------------------------------------------------
	// オブジェクト枠を描画する
	//----------------------------------------------------------------------------------------------
	pMyLGFX->SpriteDrawRect(0, 0, iW, iH, MY_PALETTE_DARKGREEN);
	//
	//----------------------------------------------------------------------------------------------
	// オフスクリーンバッファに生成した領域をＬＣＤパネルに出力（描画）する
	//----------------------------------------------------------------------------------------------
	pMyLGFX->SpritePush(320 - iW, 103, MY_TRUE);

	//----------------------------------------------------------------------------------------------
	// 次回描画用のグラフ表示倍率を保存する
	//----------------------------------------------------------------------------------------------
	this->sSpeedHistoryMax = max(this->sSpeedHistoryMax, sSpeed);					// 最高速度を更新
	this->sSpeedHistoryMin = min(this->sSpeedHistoryMin, sSpeed);					// 最低速度を更新
}
/**/
//--------------------------------------------------------------------------------------------------
// 移動速度の表示色を取得する（本来は文字色も変えたいところ）
//--------------------------------------------------------------------------------------------------
INT					_MyPanelSideLowMove::GetSpeedColor(
LONG				lSpeed)
{
	INT	iColor = MyGlobalTool.MyConfigFile.GetSysConfigSpeedColor(lSpeed);
	if (iColor <= 0)		iColor = MY_PALETTE_YELLOW;
	return	iColor;
}
/**/
//--------------------------------------------------------------------------------------------------
// サイドパネル（速度遷移）・ＪＡＲＶｉｓ情報（移動速度）経緯の描画を行う
//
//	    0, 103							249,103		319, 103
//			+-------------------------------+-------+
//			| Long Term						|Short	|
//			|								|	Term|
//			|		Speed History			|		|
//			|								|G		|
//			|								| Sensor|
//	   0,239+-------------------------------+-------+
//--------------------------------------------------------------------------------------------------
VOID				_MyPanelSideLowMove::DrawLogSpeedInfo(
_MyLGFX				*pMyLGFX,
BOOL				bDrawFlag,
_MyDevice			*pMyDevice,
_MyJARVis			*pMyJARVis)
{
	//----------------------------------------------------------------------------------------------
	// 移動速度遷移描画（長期系）
	//----------------------------------------------------------------------------------------------
	DrawLogSpeedInfoHistory(pMyLGFX, bDrawFlag, pMyDevice, pMyJARVis);

	//----------------------------------------------------------------------------------------------
	// 移動速度遷移描画（短期系）
	//----------------------------------------------------------------------------------------------
	DrawLogSpeedInfoAccell(pMyLGFX, bDrawFlag, pMyDevice, pMyJARVis);
}
/*-- End Of File --*/
