//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyPanel/PanelSide/LowArea/MyPanelSideLowStop.h
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#ifndef MYPANELSIDELOWSTOP_H
#define MYPANELSIDELOWSTOP_H

//--------------------------------------------------------------------------------------------------
// 外部参照・依存関係インクルード・ヘッダ情報
//--------------------------------------------------------------------------------------------------
#include "MyDevice.h"														// Device
#include "MyJARVis.h"														// JARVis
#include "LGFX/MyLGFX.h"													// LovyanGFX Wrapper

//--------------------------------------------------------------------------------------------------
// 情報表示（サイドパネル）下層レイアウト・描画クラス
//--------------------------------------------------------------------------------------------------
class   _MyPanelSideLowStop {
	private:
		//------------------------------------------------------------------------------------------
		// 描画用オブジェクト
		//------------------------------------------------------------------------------------------
		FLOAT					fWeatherTemperature;						// From BME280	気温（℃）
		FLOAT					fWeatherPressure;							//				気圧（hPa）
		FLOAT					fWeatherApproxAltitude;						//				近似高度（ｍ）
		FLOAT					fWeatherHumidity;							//				湿度（％）
		//
		FLOAT					fJARVisSpeedMax;							// From JARVis	最高速度
		FLOAT					fJARVisSpeedAve;							//				平均速度
		FLOAT					fJARVisLinearDistance;						//				初回認識地点からの直線距離
		FLOAT					fJARVisLinearSpeedAve;						//				初回認識地点からの直線移動平均速度
		//
		TimeSpan				tsJARVisMoveElapsedTimeSum;					// From JARVis	連続移動時間（積算）
		TimeSpan				tsJARVisStopElapsedTimeSum;					//				連続停車時間（積算）
		SHORT					sJARVisStopCount;							//				停車回数 

	public:
		//------------------------------------------------------------------------------------------
		// メインどころのメソッド
		//------------------------------------------------------------------------------------------
		_MyPanelSideLowStop();
		~_MyPanelSideLowStop();
//
		//------------------------------------------------------------------------------------------
		// LowArea :
		//	DrawLogWeatherInfo		: 気象情報描画
		//	DrawLogDriveInfo		: ドライブ情報描画
		//	DrawLogJARVisInfo		: JARVis情報描画
		//	DrawLogTripInfo			: Trip情報描画
		//------------------------------------------------------------------------------------------
		VOID					DrawLogWeatherInfo(_MyLGFX *pMyLGFX, BOOL bDrawFlag, _MyDevice *pMyDevice);
		VOID					DrawLogDriveInfo(_MyLGFX *pMyLGFX, BOOL bDrawFlag, _MyDevice *pMyDevice, _MyJARVis *pMyJARVis);
		VOID					DrawLogJARVisInfo(_MyLGFX *pMyLGFX, BOOL bDrawFlag, _MyDevice *pMyDevice, _MyJARVis *pMyJARVis);
		VOID					DrawLogTripInfo(_MyLGFX *pMyLGFX, BOOL bDrawFlag, _MyDevice *pMyDevice, _MyJARVis *pMyJARVis);
};
#endif
/*-- End Of File --*/
