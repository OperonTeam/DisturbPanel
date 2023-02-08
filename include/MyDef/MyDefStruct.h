//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : include/MyDef/MyDefStruct.h
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#ifndef MYDEFSTRUCT_H
#define MYDEFSTRUCT_H

//--------------------------------------------------------------------------------------------------
// 構造体関係の定義
//--------------------------------------------------------------------------------------------------
struct _My3DFIMU {											// For IMU Point (By FLOAT)
	FLOAT			fRoll;
	FLOAT			fPitch;
	FLOAT			fYaw;
};
//
struct _My2DSPoint {										// For 2D Point
	SHORT			sX;										// LCDモニタ描画時の座標値で利用
	SHORT			sY;
};
//
struct _MyGPSPoint {										// For GPS
	DOUBLE			dLat;									// 緯度：LatitudePoint
	DOUBLE			dLng;									// 経度：LongitudePoint
};
//
struct _MyGPSLine {
	_MyGPSPoint		MyGPSPointA;							// GPS座標A（From）
	_MyGPSPoint		MyGPSPointB;							// GPS座標B（To）
};
//
struct _MyTPMSInfo {										// For TPMS センサー値
	FLOAT			fAirPressure;							// タイヤ内圧　（KPa）
	FLOAT			fTemperature;							// タイヤ内温度（℃
	CHAR			cBattery;								// 内蔵バッテリィ残割合（％）
};
//
#endif
/*-- End Of File --*/
