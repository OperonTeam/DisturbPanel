//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyJARVis/Geolocation/MyGeolocation.h
//  Copyright 2019. 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#ifndef MYGEOLOCATION_H
#define MYGEOLOCATION_H

//--------------------------------------------------------------------------------------------------
// 位置情報管理用クラス
//--------------------------------------------------------------------------------------------------
class   _MyGeolocation {
	private:
		//------------------------------------------------------------------------------------------
		// コントロールラインGPS座標・アクセスメソッド　
		//	GetRegistControlLine	:	登録済みコントロールライン座標情報を取得する
		//------------------------------------------------------------------------------------------
		SHORT					sControlInfoNum;								// 登録済みコントロールライン情報の件数
		String					strControlTitle[10];							// コントロールラインの名称
		_MyGPSLine				MyGPSLine[10];									// コントロールラインの座標
		//---
		String					GetRegistControlLine(SHORT sIndex, _MyGPSLine *pMyGPSLine);
//---
	public:
		_MyGeolocation();
		~_MyGeolocation();
//---
		//------------------------------------------------------------------------------------------
		// 外部から呼び出される分析メソッド
		//	ReadControlLineFile			:	コントロールライン情報ファイルを読み込む
		//	IsControlLineCrossed		:	位置情報管理クラスの情報を更新する
		//------------------------------------------------------------------------------------------
		BOOL					ReadControlLineFile(VOID);
		BOOL					IsControlLineCrossed(_MyGPSLine *pMyGPSMovePoint, SHORT *psIndex, String *pstrControlLineName, _MyGPSPoint *pMyGPSCrossPoint);
};
#endif
/*-- End Of File --*/
