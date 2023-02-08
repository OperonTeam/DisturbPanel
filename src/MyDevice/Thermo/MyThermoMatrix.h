//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyDevice/Thermo/MyThermoMatrix.h
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#ifndef MYTHERMOMATRIX_H
#define MYTHERMOMATRIX_H

//--------------------------------------------------------------------------------------------------
// サーモ取得情報、ソフト演算座標定数 
//--------------------------------------------------------------------------------------------------
#define IAUTOMEMN				10
//
#define THERMOMAP_RAWX			8
#define THERMOMAP_RAWY			8
//
#define IGRIDMAX				56
#define IGRIDMAX1				2
#define IGRIDMAX2				54
#define IDIVMATRIX				7
#define fDIVMATRIX				(FLOAT)IDIVMATRIX
//
#define	MY_THERMO_COLOR_RANGE	(FLOAT)16.0							// サーモヒートマップ色深度
#define	AMG8833_SENSOR_MIN_VAL	-20									// サーモセンサー取得最低値（温度）
#define	AMG8833_SENSOR_MAX_VAL	80									// サーモセンサー取得最高値（温度）
//
//--------------------------------------------------------------------------------------------------
// Ｉ２Ｃデバイス用クラス
//--------------------------------------------------------------------------------------------------
class   _MyThermoMatrix {
	private:
		//------------------------------------------------------------------------------------------
		// サーモ情報マトリクス情報の疑似拡張用(8x8 -> 52x52)
		//------------------------------------------------------------------------------------------
		INT						autoptr;							// 過去情報保存用インデックス
		INT						automin[IAUTOMEMN];					// 過去Thermoマトリクス情報の最低値
		INT						automax[IAUTOMEMN];					// 過去Thermoマトリクス情報の最高値
		FLOAT					sensData[THERMOMAP_RAWX+1][THERMOMAP_RAWY+1];			//
		CHAR					cMap[IGRIDMAX+1][IGRIDMAX+1];		//	最終的に演算した結果（ヒートマップ情報）

	public:
		_MyThermoMatrix();
		~_MyThermoMatrix();
		//---
		//------------------------------------------------------------------------------------------
		// センサー情報の取り扱いメソッド
		//	MakeExtendMatrixMap			: AMG8833 センサー情報情報の分解能を向上させる
		//	GetThermoHeatMap			: ソフト演算したヒートマップ情報を取得する
		//------------------------------------------------------------------------------------------
		FLOAT					MakeExtendMatrixMap(FLOAT *pfPnt, BOOL bFaceFlag);
		INLINE	CHAR			GetThermoHeatMap(INT iXs, INT iYs) {
									return	this->cMap[iXs][iYs];
		};
};
#endif
/*-- End Of File --*/
