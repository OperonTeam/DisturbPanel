//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyPanel/PanelMain/MyPanelMain.cpp
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#include "MyBase.h"
#include "MyDevice.h"
#include "MyJARVis.h"
#include "MyPanelMain.h"

//--------------------------------------------------------------------------------------------------
// コンパス・オブジェクトの座標定義
//--------------------------------------------------------------------------------------------------
#define	MY_COMPASS_CIRCLE_X				159					// コンパスオブジェクトの中心座標Ｘ
#define	MY_COMPASS_CIRCLE_Y				179					// コンパスオブジェクトの中心座標Ｙ
#define	MY_COMPASS_R					158					// コンパスオブジェクト楕円の半径Ｘ
#define	MY_COMPASS_RY					40					// コンパスオブジェクト楕円の半径Ｙ
//
#define	MY_GPSCHANGED_DIFF_DEG			(SHORT)2			// 表示許容範囲（角度）
//
//--------------------------------------------------------------------------------------------------
// クラスインスタンスの初期処理
//--------------------------------------------------------------------------------------------------
_MyPanelMain::_MyPanelMain()
{
	this->cCompassGPSOnlineNum 		= (CHAR)0;				// GPSオンライン衛星数
	this->fCompassGPSSpeed			= (FLOAT)-1.0;			// GPS移動速度
	this->sCompassGPSPointCourse	= (SHORT)-1;			// GPS移動方位（北方向）を示す角度
	this->sCompassIMUGeomagnetism	= (SHORT)361;			// IMU地磁気センサー北方向を示す角度
	this->iTFTGeoOffset				= (SHORT)0;				// コンパス描画用オフセット角度
	this->sCompassIMUAngle			= (SHORT)0;				// 現在傾斜角度
}
/**/
//--------------------------------------------------------------------------------------------------
// クラスインスタンスの終了処理
//--------------------------------------------------------------------------------------------------
_MyPanelMain::~_MyPanelMain()
{
}
/**/
//--------------------------------------------------------------------------------------------------
// （ＴＦＴ表示領域全体）情報表示スクリーン処理
//--------------------------------------------------------------------------------------------------
VOID				_MyPanelMain::MyUpdateAll(
BOOL				bDrawFlag,
_MyLGFX				*pMyLGFX,
_MyDevice			*pMyDevice,
_MyJARVis			*pMyJARVis)
{
	//----------------------------------------------------------------------------------------------
	// コンパス・オブジェクトのオフスクリーン描画（動的）情報を生成する
	//----------------------------------------------------------------------------------------------
	_My2DSPoint	My2DSPointSRC[4];
	_My2DSPoint	My2DSPointDST[4];
	_My2DSPoint	My2DSPointCenter;
	_My2DSPoint	My2DSRet;
	My2DSPointCenter.sX = MY_COMPASS_CIRCLE_X;
	My2DSPointCenter.sY = MY_COMPASS_CIRCLE_Y;

	//----------------------------------------------------------------------------------------------
	// IMU地磁気センサーが北方向を示す角度（方向）のオフセット値
	//----------------------------------------------------------------------------------------------
	this->iTFTGeoOffset = MyGlobalTool.MyConfigFile.iTFTGeoOffset;				// ＴＦＴ・磁気方位描画オフセット値

	//----------------------------------------------------------------------------------------------
	// 現在描画中の情報とセンサー情報を比較する（つまり再描画が必要か否か？）
	// ＧＰＳオンライン衛星数
	// ＧＰＳ移動速度
	// 現在座標値とPoint座標との距離(m)
	// 進行方向に対して北方向を示す角度
	// 現在座標値とPoint座標を示す角度
	// 現在傾斜角度
	// 最大左傾斜角度
	// 最大右傾斜角度
	//----------------------------------------------------------------------------------------------
	if (pMyDevice->MyFDRBodyRecNow.GetGPSOnlineNum() > 0)	{	if (this->cCompassGPSOnlineNum < (CHAR)1 )	bDrawFlag = MY_TRUE;	}			// ＯＦＦ　－＞　ＯＮ
	else													{	if (this->cCompassGPSOnlineNum > (CHAR)0 )	bDrawFlag = MY_TRUE;	}			// ＯＮ　－＞　ＯＦＦ
	this->cCompassGPSOnlineNum = pMyDevice->MyFDRBodyRecNow.GetGPSOnlineNum();
//---
	bDrawFlag += MyGlobalTool.IfSetDifferent(pMyDevice->MyFDRBodyRecNow.GetGPSSpeed(),					&this->fCompassGPSSpeed);
	bDrawFlag += MyGlobalTool.IfSetDifferent((SHORT)pMyDevice->MyFDRBodyRecNow.GetGPSCourse(),			&this->sCompassGPSPointCourse,	MY_GPSCHANGED_DIFF_DEG);	// Course (North) Angle
	bDrawFlag += MyGlobalTool.IfSetDifferent((SHORT)pMyDevice->MyFDRBodyRecNow.GetIMUAHRSData().fYaw,	&this->sCompassIMUGeomagnetism,	MY_GPSCHANGED_DIFF_DEG);	// North Angle
	bDrawFlag += MyGlobalTool.IfSetDifferent((SHORT)pMyDevice->MyFDRBodyRecNow.GetIMUAHRSData().fRoll,	&this->sCompassIMUAngle,		MY_GPSCHANGED_DIFF_DEG);	// Angle
//---
	//----------------------------------------------------------------------------------------------
	// 描画更新が必要な場合は、以下の処理順に描画処理を行う
	// ・オフスクリーン描画用の領域を生成
	// ・静的描画情報を生成
	// ・動的描画情報を生成
	// ・オフスクリーン領域をＬＣＤパネルに（描画）書き込む
	//----------------------------------------------------------------------------------------------
	if (bDrawFlag) {
		//------------------------------------------------------------------------------------------
		// オフスクリーン領域を生成する
		//------------------------------------------------------------------------------------------
		this->MakeSpriteCompass(pMyLGFX);

		//------------------------------------------------------------------------------------------
		// ＧＰＳオンライン衛星数が、存在しない場合
		//------------------------------------------------------------------------------------------
		if (this->cCompassGPSOnlineNum < (CHAR)1 ) {
			//--------------------------------------------------------------------------------------
			// ＧＰＳ信号喪失の場合は、真っ赤なエラーメッセージを表示する
			//--------------------------------------------------------------------------------------
			pMyLGFX->SpriteDrawStringFormat(F("GPS"),			160,  46,  MY_LCDFONT_eSPI4, MY_PALETTE_RED, 2.0, 2.0, textdatum_t::top_center);
			pMyLGFX->SpriteDrawStringFormat(F("Signal Lost"),	160,  84,  MY_LCDFONT_eSPI4, MY_PALETTE_RED, 2.0, 2.0, textdatum_t::top_center);

			//------------------------------------------------------------------------------------------
			// ＩＭＵの磁気情報【現在進行方向に対して北方向を示す角度】を、内部楕円コンパス内に描画する。
			//------------------------------------------------------------------------------------------
			SHORT	sAngle = MyGlobalTool.IMUMag2Angle(this->sCompassIMUGeomagnetism, this->iTFTGeoOffset);
			My2DSRet = MyGlobalTool.EllipseAngleContact(My2DSPointCenter, MY_COMPASS_R - 6, MY_COMPASS_RY - 6, sAngle);
			pMyLGFX->SpriteFillCircle(My2DSRet.sX, My2DSRet.sY, 8, MY_PALETTE_RED);
		} else {
			//--------------------------------------------------------------------------------------
			// 【ＧＰＳ移動速度】情報を描画する
			//--------------------------------------------------------------------------------------
			pMyLGFX->SpriteSetConfig(MY_LCDFONT_eSPI7, MY_PALETTE_YELLOW, MY_PALETTE_BLACK, 2.0, 2.0, textdatum_t::bottom_right);
			pMyLGFX->SpriteDrawNumber((LONG)this->fCompassGPSSpeed, 240, 138);					// 整数部
			//
			LONG	lA = ((DOUBLE)(this->fCompassGPSSpeed - (LONG)this->fCompassGPSSpeed)*10);
			pMyLGFX->SpriteSetConfig(MY_LCDFONT_eSPI7, MY_PALETTE_YELLOW, MY_PALETTE_BLACK, 0.8, 0.8, textdatum_t::bottom_left);
			pMyLGFX->SpriteDrawNumber(lA, 250, 112);												// 少数部
			pMyLGFX->SpriteDrawStringFormat(F("Km/h"),	240, 138, MY_LCDFONT_eSPI4, MY_PALETTE_YELLOW, 1.0, 1.0, textdatum_t::bottom_left);
//---
			//--------------------------------------------------------------------------------------
			// 【ＧＰＳ進行方向】情報から、北方向の方位情報を描画する
			//	（描画パネルは進行方向を向いている前提で）北方向方位を描画する場所は、以下の通りです。
			//--------------------------------------------------------------------------------------
			//		GPS進行方向が地磁気で北方位(=  0)の場合、パネルの上部
			//		GPS進行方向が地磁気で東方位(= 90)の場合、パネルの左部
			//		GPS進行方向が地磁気で南方位(=180)の場合、パネルの下部
			//		GPS進行方向が地磁気で西方位(=270)の場合、パネルの右部
			//--------------------------------------------------------------------------------------
			SHORT	sGPSNorth = MyGlobalTool.GPSCourse2Angle(this->sCompassGPSPointCourse);
			My2DSRet = MyGlobalTool.EllipseAngleContact(My2DSPointCenter, MY_COMPASS_R - 6, MY_COMPASS_RY - 6, sGPSNorth);
			pMyLGFX->SpriteFillCircle(My2DSRet.sX, My2DSRet.sY, 8, MY_PALETTE_RED);
			//
			//--------------------------------------------------------------------------------------
			// 【現在座標値とPoint座標との距離】を描画する
			//	表示する距離が1Km以下の場合は、メートル単位の表示を行う
			//--------------------------------------------------------------------------------------
			FLOAT	fDistance = pMyJARVis->MyJARVisAnalyze.QueJARVisDistanceTo(pMyDevice);
			pMyLGFX->SpriteSetConfig(MY_LCDFONT_eSPI7, MY_PALETTE_YELLOW, MY_PALETTE_BLACK, 0.5, 1.0, textdatum_t::middle_right);
			if (fDistance < (FLOAT)1000)	pMyLGFX->SpriteDrawFloat(fDistance/1000, 3, MY_COMPASS_CIRCLE_X + 120, MY_COMPASS_CIRCLE_Y);
			else							pMyLGFX->SpriteDrawFloat(fDistance/1000, 1, MY_COMPASS_CIRCLE_X + 120, MY_COMPASS_CIRCLE_Y);
			//
			//--------------------------------------------------------------------------------------
			// 【現在座標値とPoint座標を示す方位】を描画する
			//	（描画パネルは進行方向を向いている前提で）指定座標方向を描画する場所は、以下の通りです。
			//--------------------------------------------------------------------------------------
			//	指定座標への方位         ：北方位(=  0)	 |東方位(= 90)  |南方位(=180)  |西方位(=270)  |
			//	GPS進行方向が北方位(=  0)：  0:パネル上部| 90:パネル右部|180:パネル下部|270:パネル右部|	
			//	GPS進行方向が東方位(= 90)：270:パネル左部|  0:パネル上部| 90:パネル右部|180:パネル下部|
			//	GPS進行方向が南方位(=180)：180:パネル下部|270:パネル左部|  0:パネル上部| 90:パネル右部|
			//	GPS進行方向が西方位(=270)：270:パネル左部|180:パネル下部|270:パネル左部|  0:パネル上部|
			//--------------------------------------------------------------------------------------
			SHORT	sCourseTo = pMyJARVis->MyJARVisAnalyze.QueJARVisCourseTo(pMyDevice);
			SHORT	sAngle = MyGlobalTool.GPSCourseTo2Angle(sCourseTo, this->sCompassGPSPointCourse);
			//---
			My2DSRet = MyGlobalTool.EllipseAngleContact(My2DSPointCenter, MY_COMPASS_R - 8, MY_COMPASS_RY - 8, sAngle);
			pMyLGFX->SpriteDrawLine(MY_COMPASS_CIRCLE_X, MY_COMPASS_CIRCLE_Y, My2DSRet.sX, My2DSRet.sY, MY_PALETTE_YELLOW);
			for (INT i = -4; i < 1; i += 1) {
				pMyLGFX->SpriteDrawLine(My2DSRet.sX - i, My2DSRet.sY - 14, My2DSRet.sX - i, My2DSRet.sY, MY_PALETTE_YELLOW);
			}
		}

		//------------------------------------------------------------------------------------------
		// コンパスの中心に円と垂直線を描画する
		//------------------------------------------------------------------------------------------
		pMyLGFX->SpriteDrawFastVLine(MY_COMPASS_CIRCLE_X, MY_COMPASS_CIRCLE_Y - 16, 32, MY_PALETTE_DARKGREEN);
		pMyLGFX->SpriteFillCircle(MY_COMPASS_CIRCLE_X, MY_COMPASS_CIRCLE_Y, 5, MY_PALETTE_DARKGREEN);

//--------------------------------------------------------------------------------------------------
// 最大傾斜角度
//--------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------
		// 最大傾斜角度（左右最大）を求める
		//------------------------------------------------------------------------------------------
		if (abs(this->sCompassIMUAngle) < 50) {
			if (this->sCompassIMUAngle > 0) {
				this->sCompassIMUAngleRight = max(this->sCompassIMUAngleRight, this->sCompassIMUAngle);	// 最大・右傾斜角度
			} else {
				this->sCompassIMUAngleLeft  = min(this->sCompassIMUAngleLeft,  this->sCompassIMUAngle);	// 最大・左傾斜角度
			}
		}
		//------------------------------------------------------------------------------------------
		// 傾斜角度（右最大）を描画する
		//------------------------------------------------------------------------------------------
		My2DSPointSRC[0].sX = MY_COMPASS_CIRCLE_X;			My2DSPointSRC[0].sY = MY_COMPASS_CIRCLE_Y - MY_COMPASS_R;
		My2DSPointSRC[1].sX = MY_COMPASS_CIRCLE_X - 6;		My2DSPointSRC[1].sY = MY_COMPASS_CIRCLE_Y - MY_COMPASS_R - 16;
		My2DSPointSRC[2].sX = MY_COMPASS_CIRCLE_X + 6;		My2DSPointSRC[2].sY = MY_COMPASS_CIRCLE_Y - MY_COMPASS_R - 16;
		MyGlobalTool.CompRotPoint(My2DSPointCenter, this->sCompassIMUAngleRight, My2DSPointSRC, 3, My2DSPointDST);
		pMyLGFX->SpriteFillTriangle(My2DSPointDST[0].sX, My2DSPointDST[0].sY,
								My2DSPointDST[1].sX, My2DSPointDST[1].sY,
								My2DSPointDST[2].sX, My2DSPointDST[2].sY, MY_PALETTE_RED);
		//
		//------------------------------------------------------------------------------------------
		// 傾斜角度（左最大）を描画する
		//------------------------------------------------------------------------------------------
		My2DSPointSRC[0].sX = MY_COMPASS_CIRCLE_X;			My2DSPointSRC[0].sY = MY_COMPASS_CIRCLE_Y - MY_COMPASS_R;
		My2DSPointSRC[1].sX = MY_COMPASS_CIRCLE_X - 6;		My2DSPointSRC[1].sY = MY_COMPASS_CIRCLE_Y - MY_COMPASS_R - 16;
		My2DSPointSRC[2].sX = MY_COMPASS_CIRCLE_X + 6;		My2DSPointSRC[2].sY = MY_COMPASS_CIRCLE_Y - MY_COMPASS_R - 16;
		MyGlobalTool.CompRotPoint(My2DSPointCenter, this->sCompassIMUAngleLeft, My2DSPointSRC, 3, My2DSPointDST);
		pMyLGFX->SpriteFillTriangle(My2DSPointDST[0].sX, My2DSPointDST[0].sY,
								My2DSPointDST[1].sX, My2DSPointDST[1].sY,
								My2DSPointDST[2].sX, My2DSPointDST[2].sY, MY_PALETTE_RED);
		//
		//------------------------------------------------------------------------------------------
		// 現在の車体傾斜角度を描画する座標情報を算出する
		//------------------------------------------------------------------------------------------
		My2DSPointSRC[0].sX = MY_COMPASS_CIRCLE_X;			My2DSPointSRC[0].sY = MY_COMPASS_CIRCLE_Y - MY_COMPASS_R;
		My2DSPointSRC[1].sX = MY_COMPASS_CIRCLE_X - 6;		My2DSPointSRC[1].sY = MY_COMPASS_CIRCLE_Y - MY_COMPASS_R - 16;
		My2DSPointSRC[2].sX = MY_COMPASS_CIRCLE_X + 6;		My2DSPointSRC[2].sY = MY_COMPASS_CIRCLE_Y - MY_COMPASS_R - 16;
		MyGlobalTool.CompRotPoint(My2DSPointCenter, this->sCompassIMUAngle, My2DSPointSRC, 3, My2DSPointDST);
		pMyLGFX->SpriteFillTriangle(My2DSPointDST[0].sX, My2DSPointDST[0].sY,
								My2DSPointDST[1].sX, My2DSPointDST[1].sY,
								My2DSPointDST[2].sX, My2DSPointDST[2].sY, MY_PALETTE_YELLOW);

		//------------------------------------------------------------------------------------------
		// オフスクリーンバッファに生成した領域を、ＬＣＤパネルに出力（描画）する
		//	メインパネルの描画領域は、320x240pixcel(ColorDepth:2)あり、オフスクリーン描画を行っていますが、
		//	実パネルに全領域を描画する（SpritePush）のには、およそ36mmSec必要です。
		//------------------------------------------------------------------------------------------
		pMyLGFX->SpritePush(0, 0, MY_TRUE);
	}
}
/**/
//--------------------------------------------------------------------------------------------------
// コンパスオブジェクトの背景（静的）オブジェクトをオフスクリーン領域に生成する
//--------------------------------------------------------------------------------------------------
VOID				_MyPanelMain::MakeSpriteCompass(_MyLGFX *pMyLGFX)
{
	//----------------------------------------------------------------------------------------------
	// オフスクリーン・バッファ領域を作成する
	//----------------------------------------------------------------------------------------------
	pMyLGFX->SpriteCreate(MY_PANEL_MAX_WIDTH, MY_PANEL_MAX_HEIGHT, 2);

	//----------------------------------------------------------------------------------------------
	// 内周（楕）円（TFT_GREEN）を描画する（枠線が1pixelでは細過ぎる）
	//----------------------------------------------------------------------------------------------
	pMyLGFX->SpriteClearDisplay(MY_PALETTE_BLACK);									// TFT_BLACK
	pMyLGFX->SpriteDrawEllipse(MY_COMPASS_CIRCLE_X, MY_COMPASS_CIRCLE_Y - 1, MY_COMPASS_R, MY_COMPASS_RY, MY_PALETTE_YELLOW);
	for (int i = 0; i < 18; i += 1) {
		pMyLGFX->SpriteDrawEllipse(MY_COMPASS_CIRCLE_X, MY_COMPASS_CIRCLE_Y + i, MY_COMPASS_R, MY_COMPASS_RY, MY_PALETTE_DARKGREEN);
	}
	pMyLGFX->SpriteFillEllipse(MY_COMPASS_CIRCLE_X + 1, MY_COMPASS_CIRCLE_Y + 1, MY_COMPASS_R - 1, MY_COMPASS_RY - 1, MY_PALETTE_BLACK);
//
	//----------------------------------------------------------------------------------------------
	// コンパス外周枠オブジェクトを生成する（枠線が1pixelでは細過ぎる）
	//----------------------------------------------------------------------------------------------
	for (int i = 0; i < 2; i++) {
		pMyLGFX->SpriteDrawArc(MY_COMPASS_CIRCLE_X, MY_COMPASS_CIRCLE_Y, MY_COMPASS_R - i, MY_COMPASS_R - i,	180, 0, MY_PALETTE_DARKGREEN);
	}
	//----------------------------------------------------------------------------------------------
	// コンパス外周枠にマーカーを描画する
	//----------------------------------------------------------------------------------------------
	_My2DSPoint	My2DSPointSRC[2];
	_My2DSPoint	My2DSPointDST[2];
	_My2DSPoint	My2DSPointCenter;
	My2DSPointCenter.sX = MY_COMPASS_CIRCLE_X;			My2DSPointCenter.sY = MY_COMPASS_CIRCLE_Y;
	My2DSPointSRC[0].sX = MY_COMPASS_CIRCLE_X;			My2DSPointSRC[0].sY = MY_COMPASS_CIRCLE_Y - MY_COMPASS_R - 5;
//
	MyGlobalTool.CompRotPoint(My2DSPointCenter,  10, My2DSPointSRC, 1, My2DSPointDST);
	pMyLGFX->SpriteFillCircle(My2DSPointDST[0].sX, My2DSPointDST[0].sY, 3, MY_PALETTE_DARKGREEN);

	MyGlobalTool.CompRotPoint(My2DSPointCenter,  20, My2DSPointSRC, 1, My2DSPointDST);
	pMyLGFX->SpriteFillCircle(My2DSPointDST[0].sX, My2DSPointDST[0].sY, 3, MY_PALETTE_DARKGREEN);

	MyGlobalTool.CompRotPoint(My2DSPointCenter,  30, My2DSPointSRC, 1, My2DSPointDST);
	pMyLGFX->SpriteFillCircle(My2DSPointDST[0].sX, My2DSPointDST[0].sY, 3, MY_PALETTE_DARKGREEN);

	MyGlobalTool.CompRotPoint(My2DSPointCenter,  45, My2DSPointSRC, 1, My2DSPointDST);
	pMyLGFX->SpriteFillCircle(My2DSPointDST[0].sX, My2DSPointDST[0].sY, 8, MY_PALETTE_DARKGREEN);
//
	MyGlobalTool.CompRotPoint(My2DSPointCenter, -10, My2DSPointSRC, 1, My2DSPointDST);
	pMyLGFX->SpriteFillCircle(My2DSPointDST[0].sX, My2DSPointDST[0].sY, 3, MY_PALETTE_DARKGREEN);

	MyGlobalTool.CompRotPoint(My2DSPointCenter, -20, My2DSPointSRC, 1, My2DSPointDST);
	pMyLGFX->SpriteFillCircle(My2DSPointDST[0].sX, My2DSPointDST[0].sY, 3, MY_PALETTE_DARKGREEN);

	MyGlobalTool.CompRotPoint(My2DSPointCenter, -30, My2DSPointSRC, 1, My2DSPointDST);
	pMyLGFX->SpriteFillCircle(My2DSPointDST[0].sX, My2DSPointDST[0].sY, 3, MY_PALETTE_DARKGREEN);

	MyGlobalTool.CompRotPoint(My2DSPointCenter, -45, My2DSPointSRC, 2, My2DSPointDST);
	pMyLGFX->SpriteFillCircle(My2DSPointDST[0].sX, My2DSPointDST[0].sY, 8, MY_PALETTE_DARKGREEN);

	//----------------------------------------------------------------------------------------------
	// オブジェクトの外周枠を生成する
	//----------------------------------------------------------------------------------------------
	pMyLGFX->SpriteDrawRect(0, 0, MY_PANEL_MAX_WIDTH, MY_PANEL_MAX_HEIGHT, MY_PALETTE_DARKGREEN);
}
/**/
/*-- End Of File --*/
