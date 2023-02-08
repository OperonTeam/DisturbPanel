//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyPanel/PanelSide/LowArea/MyPanelSideLowStop.cpp
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#include "MyBase.h"
#include "MyDevice.h"
#include "MyJARVis.h"
#include "MyPanelSideLowStop.h"

//--------------------------------------------------------------------------------------------------
// クラスインスタンスの初期処理
//--------------------------------------------------------------------------------------------------
_MyPanelSideLowStop::_MyPanelSideLowStop()
{
	this->fWeatherTemperature			= (FLOAT)-1.0;		// For BME280	気温（℃）
	this->fWeatherPressure				= (FLOAT)-1.0;		//				気圧（hPa）
	this->fWeatherApproxAltitude		= (FLOAT)-1.0;		//				近似高度（ｍ）
	this->fWeatherHumidity				= (FLOAT)-1.0;		//				湿度（％）
//---
	this->fJARVisSpeedMax				= (FLOAT)0.0;		// From JARVis	最高速度
	this->fJARVisSpeedAve				= (FLOAT)0.0;		//				平均速度
//---
	this->tsJARVisMoveElapsedTimeSum	= TimeSpan(0);		// From JARVis	連続移動時間（積算）
	this->tsJARVisStopElapsedTimeSum	= TimeSpan(0);		//				連続停車時間（積算）
	this->sJARVisStopCount				= (SHORT)-1;		//				停車回数 
}
/**/
//--------------------------------------------------------------------------------------------------
// クラスインスタンスの終了処理
//--------------------------------------------------------------------------------------------------
_MyPanelSideLowStop::~_MyPanelSideLowStop()
{
}
/**/
//--------------------------------------------------------------------------------------------------
// 気象情報の描画
//--------------------------------------------------------------------------------------------------
VOID				_MyPanelSideLowStop::DrawLogWeatherInfo(
_MyLGFX				*pMyLGFX,
BOOL				bDrawFlag,
_MyDevice			*pMyDevice)
{
	//----------------------------------------------------------------------------------------------
	// 現在描画中の情報とセンサー情報を比較する（つまり再描画が必要か否か？）
	// For BME280	気温（℃）
	// 				気圧（hPa）
	// 				近似高度（ｍ）
	// 				湿度（％）
	//----------------------------------------------------------------------------------------------
	bDrawFlag += MyGlobalTool.IfSetDifferent(pMyDevice->MyFDRHeadRecNow.GetWeatherTemperature(),	&this->fWeatherTemperature);
	bDrawFlag += MyGlobalTool.IfSetDifferent(pMyDevice->MyFDRHeadRecNow.GetWeatherPressure(),		&this->fWeatherPressure);
	bDrawFlag += MyGlobalTool.IfSetDifferent(pMyDevice->MyFDRHeadRecNow.GetWeatherApproxAltitude(),	&this->fWeatherApproxAltitude);
	bDrawFlag += MyGlobalTool.IfSetDifferent(pMyDevice->MyFDRHeadRecNow.GetWeatherHumidity(),		&this->fWeatherHumidity);
	if (bDrawFlag) {
		//------------------------------------------------------------------------------------------
		// オフスクリーン・バッファ領域を作成する
		//------------------------------------------------------------------------------------------
		INT	iW = 320;
		INT	iH = 136;
		pMyLGFX->SpriteCreate(iW, iH, 4);
		//---
		//------------------------------------------------------------------------------------------
		// 静的オブジェクトを生成する
		//------------------------------------------------------------------------------------------
		pMyLGFX->SpriteDrawStringFormat(F("Temperature@C"),   2,            2, MY_LCDFONT_eSPI4, MY_PALETTE_DARKGREEN, 0.8, 0.8, textdatum_t::top_left);
		pMyLGFX->SpriteDrawStringFormat(F("Altitude@m"),	 162,            2, MY_LCDFONT_eSPI4, MY_PALETTE_DARKGREEN, 0.8, 0.8, textdatum_t::top_left);
		pMyLGFX->SpriteDrawStringFormat(F("Pressure@%"),	   2, (iH / 2) + 2, MY_LCDFONT_eSPI4, MY_PALETTE_DARKGREEN, 0.8, 0.8, textdatum_t::top_left);
		pMyLGFX->SpriteDrawStringFormat(F("Humidity@pHa"),	 162, (iH / 2) + 2, MY_LCDFONT_eSPI4, MY_PALETTE_DARKGREEN, 0.8, 0.8, textdatum_t::top_left);
		//---
		pMyLGFX->SpriteDrawRect(  0,      0, 159,  (iH / 2) -  2, MY_PALETTE_DARKGREEN);
		pMyLGFX->SpriteDrawRect(161,      0, 159,  (iH / 2) -  2, MY_PALETTE_DARKGREEN);
		pMyLGFX->SpriteDrawRect(  0, iH / 2, 159,  (iH / 2) -  2, MY_PALETTE_DARKGREEN);
		pMyLGFX->SpriteDrawRect(161, iH / 2, 159,  (iH / 2) -  2, MY_PALETTE_DARKGREEN);
		//---
		//------------------------------------------------------------------------------------------
		// 動的オブジェクトを生成する（各種センサー情報）
		//------------------------------------------------------------------------------------------
		pMyLGFX->SpriteSetConfig(MY_LCDFONT_eSPI7, MY_PALETTE_YELLOW, MY_PALETTE_BLACK, 0.9, 0.9, textdatum_t::bottom_right);
		pMyLGFX->SpriteDrawFloat(this->fWeatherTemperature,		1,   157, (iH / 2) - 3);	// 気象情報（気温）
		pMyLGFX->SpriteDrawFloat(this->fWeatherApproxAltitude,  1,   317, (iH / 2) - 3);	// 気象情報（近似高度）
		pMyLGFX->SpriteDrawFloat(this->fWeatherPressure,		1,   157, iH - 3);			// 気象情報（気圧）
		pMyLGFX->SpriteDrawFloat(this->fWeatherHumidity,		1,   317, iH - 3);			// 気象情報（湿度）

		//----------------------------------------------------------------------------------------------
		// オフスクリーンバッファに生成した領域をＬＣＤパネルに出力（描画）する
		//----------------------------------------------------------------------------------------------
		pMyLGFX->SpritePush(0, 103, MY_TRUE);
	}
}
/**/
//--------------------------------------------------------------------------------------------------
// ドライブ情報の描画
//--------------------------------------------------------------------------------------------------
VOID				_MyPanelSideLowStop::DrawLogDriveInfo(
_MyLGFX				*pMyLGFX,
BOOL				bDrawFlag,
_MyDevice			*pMyDevice,
_MyJARVis			*pMyJARVis)
{
	//----------------------------------------------------------------------------------------------
	// 現在描画中の情報とセンサー情報を比較する（つまり再描画が必要か否か？）
	// For JARVis	最高速度
	// 				平均速度
	// 				初回認識座標との直線距離
	//----------------------------------------------------------------------------------------------
	bDrawFlag += MyGlobalTool.IfSetDifferent(pMyJARVis->MyJARVisAnalyze.QueJARVisSpeedMax(),				&this->fJARVisSpeedMax);
	bDrawFlag += MyGlobalTool.IfSetDifferent(pMyJARVis->MyJARVisAnalyze.QueJARVisSpeedAve(),				&this->fJARVisSpeedAve);
	bDrawFlag += MyGlobalTool.IfSetDifferent(pMyJARVis->MyJARVisAnalyze.QueJARVisLinearDistance(pMyDevice),	&this->fJARVisLinearDistance);
	bDrawFlag += MyGlobalTool.IfSetDifferent(pMyJARVis->MyJARVisAnalyze.QueJARVisLinearSpeedAve(pMyDevice),	&this->fJARVisLinearSpeedAve);
	if (bDrawFlag) {
		//------------------------------------------------------------------------------------------
		// オフスクリーン・バッファ領域を作成する
		//------------------------------------------------------------------------------------------
		INT	iW = 320;
		INT	iH = 136;
		pMyLGFX->SpriteCreate(iW, iH, 4);

		//------------------------------------------------------------------------------------------
		// 静的オブジェクトを生成する
		//------------------------------------------------------------------------------------------
		pMyLGFX->SpriteDrawStringFormat(F("Max Speed"),  2,            2, MY_LCDFONT_eSPI4, MY_PALETTE_DARKGREEN, 0.8, 0.8, textdatum_t::top_left);
		pMyLGFX->SpriteDrawStringFormat(F("Ave Speed"),162,            2, MY_LCDFONT_eSPI4, MY_PALETTE_DARKGREEN, 0.8, 0.8, textdatum_t::top_left);
		pMyLGFX->SpriteDrawStringFormat(F("Linear"),	  2, (iH / 2) + 2, MY_LCDFONT_eSPI4, MY_PALETTE_DARKGREEN, 0.8, 0.8, textdatum_t::top_left);
		pMyLGFX->SpriteDrawStringFormat(F("LinearAve"),162, (iH / 2) + 2, MY_LCDFONT_eSPI4, MY_PALETTE_DARKGREEN, 0.8, 0.8, textdatum_t::top_left);
		//---
		pMyLGFX->SpriteDrawRect(  0,      0, 159,  (iH / 2) -  2, MY_PALETTE_DARKGREEN);
		pMyLGFX->SpriteDrawRect(161,      0, 159,  (iH / 2) -  2, MY_PALETTE_DARKGREEN);
		pMyLGFX->SpriteDrawRect(  0, iH / 2, 159,  (iH / 2) -  2, MY_PALETTE_DARKGREEN);
		pMyLGFX->SpriteDrawRect(161, iH / 2, 159,  (iH / 2) -  2, MY_PALETTE_DARKGREEN);
		//---
		//------------------------------------------------------------------------------------------
		// 動的オブジェクトを生成する（各種センサー情報）
		//------------------------------------------------------------------------------------------
		pMyLGFX->SpriteSetConfig(MY_LCDFONT_eSPI7, MY_PALETTE_YELLOW, MY_PALETTE_BLACK, 0.9, 0.9, textdatum_t::bottom_right);
		pMyLGFX->SpriteDrawFloat(this->fJARVisSpeedMax,				 1,    157, (iH / 2) - 3);		// 最高速度
		pMyLGFX->SpriteDrawFloat(this->fJARVisSpeedAve,				 1,    317, (iH / 2) - 3);		// 平均速度
		pMyLGFX->SpriteDrawFloat(this->fJARVisLinearDistance / 1000, 1,    157,	iH - 3);			// 直線移動距離
		pMyLGFX->SpriteDrawFloat(this->fJARVisLinearSpeedAve / 1000, 1,    317,	iH - 3);			// 直線移動移動速度

		//----------------------------------------------------------------------------------------------
		// オフスクリーンバッファに生成した領域をＬＣＤパネルに出力（描画）する
		//----------------------------------------------------------------------------------------------
		pMyLGFX->SpritePush(0, 103, MY_TRUE);
	}
}
/**/
//--------------------------------------------------------------------------------------------------
// ＪＡＲＶｉｓ情報の描画
//--------------------------------------------------------------------------------------------------
VOID				_MyPanelSideLowStop::DrawLogJARVisInfo(
_MyLGFX				*pMyLGFX,
BOOL				bDrawFlag,
_MyDevice			*pMyDevice,
_MyJARVis			*pMyJARVis)
{
	//----------------------------------------------------------------------------------------------
	// 現在描画中の情報とセンサー情報を比較する（つまり再描画が必要か否か？）
	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	// 現在描画中の情報とセンサー情報を比較する（つまり再描画が必要か否か？）
	// For JARVis	連続移動時間（最大）
	// 				連続停車時間（最大）
	// 				停車回数
	//----------------------------------------------------------------------------------------------
	bDrawFlag += MyGlobalTool.IfSetDifferent(pMyJARVis->MyJARVisAnalyze.QueJARVisMoveElapsedTimeSum(),	&this->tsJARVisMoveElapsedTimeSum);
	bDrawFlag += MyGlobalTool.IfSetDifferent(pMyJARVis->MyJARVisAnalyze.QueJARVisStopElapsedTimeSum(),	&this->tsJARVisStopElapsedTimeSum);
	bDrawFlag += MyGlobalTool.IfSetDifferent(pMyJARVis->MyJARVisAnalyze.QueJARVisStopCount(),			&this->sJARVisStopCount);
	if (bDrawFlag) {
		//------------------------------------------------------------------------------------------
		// オフスクリーン・バッファ領域を作成する
		//------------------------------------------------------------------------------------------
		INT	iW = 320;
		INT	iH = 136;
		pMyLGFX->SpriteCreate(iW, iH, 4);

		//------------------------------------------------------------------------------------------
		// 静的オブジェクトを生成する
		//------------------------------------------------------------------------------------------
		pMyLGFX->SpriteDrawStringFormat(F("Drive"),     2,            2, MY_LCDFONT_eSPI4, MY_PALETTE_DARKGREEN, 0.8, 0.8, textdatum_t::top_left);
		pMyLGFX->SpriteDrawStringFormat(F("Still"),   162,            2, MY_LCDFONT_eSPI4, MY_PALETTE_DARKGREEN, 0.8, 0.8, textdatum_t::top_left);
		pMyLGFX->SpriteDrawStringFormat(F("Stop Num"),  2, (iH / 2) + 2, MY_LCDFONT_eSPI4, MY_PALETTE_DARKGREEN, 0.8, 0.8, textdatum_t::top_left);
//		pMyLGFX->SpriteDrawStringFormat(F("LinearAve"),162, (iH / 2) + 2, MY_LCDFONT_eSPI4, MY_PALETTE_DARKGREEN, 0.8, 0.8, textdatum_t::top_left);
		//---
		pMyLGFX->SpriteDrawRect(  0,      0, 159,  (iH / 2) -  2, MY_PALETTE_DARKGREEN);
		pMyLGFX->SpriteDrawRect(161,      0, 159,  (iH / 2) -  2, MY_PALETTE_DARKGREEN);
		pMyLGFX->SpriteDrawRect(  0, iH / 2, 159,  (iH / 2) -  2, MY_PALETTE_DARKGREEN);
//		pMyLGFX->SpriteDrawRect(161, iH / 2, 159,  (iH / 2) -  2, MY_PALETTE_DARKGREEN);
		//---
		//------------------------------------------------------------------------------------------
		// 描画用の文字列を編集する（MMM:SS）
		//------------------------------------------------------------------------------------------
		String	strMoveTime = MyGlobalTool.ConvTimeSpan2StringMMM(this->tsJARVisMoveElapsedTimeSum)
							+ MyGlobalTool.ConvTimeSpan2StringSS(this->tsJARVisMoveElapsedTimeSum);	// 連続移動時間（最大）
		String	strStopTime = MyGlobalTool.ConvTimeSpan2StringMMM(this->tsJARVisStopElapsedTimeSum)
							+ MyGlobalTool.ConvTimeSpan2StringSS(this->tsJARVisStopElapsedTimeSum);	// 連続停車時間（最大）
		//---
		//------------------------------------------------------------------------------------------
		// 動的オブジェクトを生成する（各種センサー情報）
		//------------------------------------------------------------------------------------------
		pMyLGFX->SpriteSetConfig(MY_LCDFONT_eSPI7, MY_PALETTE_YELLOW, MY_PALETTE_BLACK, 0.9, 0.9, textdatum_t::bottom_right);
		pMyLGFX->SpriteDrawStringFormat(strMoveTime,		1,    157, MY_LCDFONT_eSPI7, MY_PALETTE_YELLOW, 0.9, 0.9,  textdatum_t::bottom_right);	// 移動時間（積算）
		pMyLGFX->SpriteDrawStringFormat(strStopTime,		1,    317, MY_LCDFONT_eSPI7, MY_PALETTE_YELLOW, 0.9, 0.9,  textdatum_t::bottom_right);	// 停車時間（積算）
		pMyLGFX->SpriteDrawNumber(this->sJARVisStopCount,	1,    157);																				// 停車回数
//		pMyLGFX->SpriteDrawFloat(this->fJARVisLinearSpeedAve / 1000,	 1,    317,	iH - 3);			// 直線移動移動速度
		//
		// オフスクリーンバッファに生成した領域をＬＣＤパネルに出力（描画）する
		//----------------------------------------------------------------------------------------------
		pMyLGFX->SpritePush(0, 103, MY_TRUE);
	}
}
/**/
//--------------------------------------------------------------------------------------------------
// Ｔｒｉｐ情報の描画
//--------------------------------------------------------------------------------------------------
VOID				_MyPanelSideLowStop::DrawLogTripInfo(
_MyLGFX				*pMyLGFX,
BOOL				bDrawFlag,
_MyDevice			*pMyDevice,
_MyJARVis			*pMyJARVis)
{
	//----------------------------------------------------------------------------------------------
	// 現在描画中の情報とセンサー情報を比較する（つまり再描画が必要か否か？）
	//----------------------------------------------------------------------------------------------
	bDrawFlag = MY_TRUE;
	if (bDrawFlag) {
		//------------------------------------------------------------------------------------------
		// オフスクリーン・バッファ領域を作成する
		//------------------------------------------------------------------------------------------
		INT	iW = 320;
		INT	iH = 136;
		pMyLGFX->SpriteCreate(iW, iH, 2);
//---
		//------------------------------------------------------------------------------------------
		// 静的オブジェクトを生成する
		//------------------------------------------------------------------------------------------
//---
		//------------------------------------------------------------------------------------------
		// 動的オブジェクトを生成する（各種センサー情報）
		//------------------------------------------------------------------------------------------
//---
		//----------------------------------------------------------------------------------------------
		// オフスクリーンバッファに生成した領域をＬＣＤパネルに出力（描画）する
		//----------------------------------------------------------------------------------------------
		pMyLGFX->SpritePush(0, 103, MY_TRUE);
	}
}
/*-- End Of File --*/
