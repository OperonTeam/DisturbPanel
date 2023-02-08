//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyTask/MyGPS/MyGPS.h
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#ifndef MYGPS_H
#define MYGPS_H

//--------------------------------------------------------------------------------------------------
// 外部参照・依存関係インクルード・ヘッダ情報
//--------------------------------------------------------------------------------------------------
#include <TinyGPS++.h>														// GPS command decorder

//--------------------------------------------------------------------------------------------------
// ＧＰＳデバイス用クラス
//--------------------------------------------------------------------------------------------------
class   _MyGPS	: public _MyTask {
	protected:
		TinyGPSPlus			MyTinyGPS;										// The TinyGPS++ object
		//---
		ULONG				ulReceiveByte;									// センサー情報受信バイト数
		ULONG				ulLoopCount;									// センサー情報ループカウンタ
		//---
		DateTime			dtGPSDateTime;									// GPS日時(LocalTime年月日時分秒)	
		UCHAR				ucGPSCentiSecond;								// GPS(センチ秒)
		CHAR				cGPSOnlineNum;									// GPSオンライン衛星数
		_MyGPSPoint			MyGPSPoint;										// GPS座標（緯度経度）
		DOUBLE				dGPSSpeed;										// 移動速度
		DOUBLE				dGPSCourse;										// 移動進行方向
		DOUBLE				dGPSAltitude;									// 海抜高度（ｍ）
//---
		//------------------------------------------------------------------------------------------
		// 仮想メソッド
		//------------------------------------------------------------------------------------------
		_MyTask::_MyTaskRet	MyBegin(_MyDevice *pMyDevice, _MyJARVis *pMyJARVis)		override;
		_MyTask::_MyTaskRet	MyUpdate(_MyDevice *pMyDevice, _MyJARVis *pMyJARVis)	override;
		_MyTask::_MyTaskRet	MySetup(_MyDevice *pMyDevice, _MyJARVis *pMyJARVis)		override;
		//---
		VOID				SendGPSControlCommand(String strCommand);		// GPSセンサーへコマンドを送信する
#ifdef	MY_DEBUG_GPS_DUMP_DETAIL
		VOID				MyGPSHexDump(INT iReadBuff);					// MyGPSHexDump For Developer...
#endif

	public:
		_MyGPS();
		~_MyGPS();

		//------------------------------------------------------------------------------------------
		// メソッド
		//	GetUTCDate		ＧＰＳ(ＵＴＣ）日付の取得
		//	GetUTCTime		ＧＰＳ(ＵＴＣ）時刻の取得
		//	GetOnlineNum	ＧＰＳオンライン衛星数を取得
		//	GetGPSPosition	ＧＰＳ座標を取得
		//	GetAltitude		ＧＰＳ高度(海抜)を取得
		//	GetCourse		ＧＰＳ進行方向を取得（只今試験中につきSHORT化した劇落ち情報を返す）
		//	GetSpeed		ＧＰＳ進行速度(時速)を取得
		//	GetDistanceTo	ＧＰＳ現在座標と指定された座標との方向を取得
		//	GetDistance		ＧＰＳ現在座標と指定された座標間の距離(M)を取得
		//------------------------------------------------------------------------------------------
		DateTime			GetGPSDateTime(SHORT sTimeShift = -9);
		_MyGPSPoint			GetGPSPosition(VOID);
		DOUBLE				GetGPSSpeed(VOID);
		DOUBLE				GetGPSCourse(VOID);
		DOUBLE				GetGPSAltitude(VOID);
		CHAR				GetGPSOnlineNum(VOID);
//
		DOUBLE				GetGPSCourseTo(_MyGPSPoint MyGPSPointTo);
		DOUBLE				GetGPSDistance(_MyGPSPoint MyGPSPointTo);
//---
		VOID				MyDump(VOID);
};
#endif
/*-- End Of File --*/
