//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyPanel/PanelMain/MyPanelMain.h
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#ifndef MYPANELMAIN_H
#define MYPANELMAIN_H

//--------------------------------------------------------------------------------------------------
// 外部参照・依存関係インクルード・ヘッダ情報
//--------------------------------------------------------------------------------------------------
#include "MyDevice.h"														// Device
#include "MyJARVis.h"														// JARVis
#include "LGFX/MyLGFX.h"													// LovyanGFX Wrapper

//--------------------------------------------------------------------------------------------------
// 情報表示（Ｍａｉｎ）用ＬＣＤクラス
//--------------------------------------------------------------------------------------------------
class   _MyPanelMain {
	private:
		//------------------------------------------------------------------------------------------
		// コンパス描画用オブジェクト
		//------------------------------------------------------------------------------------------
		CHAR					cCompassGPSOnlineNum;						// 現在GPSオンライン衛星数
		FLOAT					fCompassGPSSpeed;							// 現在移動速度
		SHORT					sCompassIMUGeomagnetism;					// IMU地磁気センサーが北方向を示す角度（方向）
		SHORT					sCompassGPSPointCourse;						// GPSセンサーが現在進行方向に対して北方向を示す角度（方向）
		SHORT					sCompassIMUAngle;							// 現在傾斜角度
		SHORT					sCompassIMUAngleLeft;						// 左最大傾斜角度
		SHORT					sCompassIMUAngleRight;						// 右最大傾斜角度
		//---
		INT						iTFTGeoOffset;								// IMU地磁気センサーが北方向を示す角度（方向）のオフセット値
//--ｰ
		//------------------------------------------------------------------------------------------
		// オブジェクトの表示メソッド
		//	MakeSpriteCompass				: オフスクリーン描画オブジェクトの生成
		//------------------------------------------------------------------------------------------
		VOID					MakeSpriteCompass(_MyLGFX *pMyLGFX);

	public:
		_MyPanelMain();
		~_MyPanelMain();
		//
		//------------------------------------------------------------------------------------------
		// メインパネルの描画処理　
		//------------------------------------------------------------------------------------------
		VOID					MyUpdateAll(BOOL bPanelDrawINIT, _MyLGFX *pMyLGFX, _MyDevice *pMyDevice, _MyJARVis *pMyJARVis);
};
#endif
/*-- End Of File --*/