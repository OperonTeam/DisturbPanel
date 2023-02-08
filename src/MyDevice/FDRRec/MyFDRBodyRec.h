//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyDevice/FDRRec/MyFDRBodyRec.h
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#ifndef MYFDRBODYREC_H
#define MYFDRBODYREC_H

//--------------------------------------------------------------------------------------------------
// FDRBodyRec管理用クラス
//	最大毎秒10レコード生成される、FDRの明細情報
//--------------------------------------------------------------------------------------------------
class   _MyFDRBodyRec {
	private:
		//------------------------------------------------------------------------------------------
		// FDRBodyRec情報
		//------------------------------------------------------------------------------------------
		ULONG				ulRecordSeq;									// For FDR		SET	Record Sequence Number
		DateTime			dtGPSDateTime;									// For GPS		RMC	日時（LocalTime）
		UCHAR				ucGPSCentiSecond;								//				RMC	1/10秒
		CHAR				cGPSOnlineNum;									// 				GGA	オンライン衛星数
		_MyGPSPoint			MyGPSPoint;										// 				RMC	座標 （緯度,経度）
		FLOAT				fGPSSpeed;										// 				RMC	移動速度（km/h）
		FLOAT				fGPSCourse;										// 				RMC	進行方向
		FLOAT				fGPSAltitude;									// 				GGA	高度（m）
		_My3DFIMU			My3DFACCEL;										// For MPU9250	IMU	加速度（3D）
		_My3DFIMU			My3DFAHRS;										// For MPU9250	IMU	推定姿勢状態（3D）
		SHORT				sVehicieTireTempAve;							// For AMG8833	AMG	タイヤ表面温度　領域平均（℃）
//---
	public:
		_MyFDRBodyRec();
		~_MyFDRBodyRec();

		//------------------------------------------------------------------------------------------
		// シーケンス情報
		//------------------------------------------------------------------------------------------
		VOID				SetRecordSeq(ULONG ulRecordSeq);
		//---
		ULONG				GetRecordSeq(VOID);								// From FDR		Record Sequence Number
//---
		//------------------------------------------------------------------------------------------
		// GPSセンサー情報
		//------------------------------------------------------------------------------------------
		VOID				SetGPSGGAData(CHAR cOnlineNum, FLOAT fAltitude);
		VOID				SetGPSRMCData(DateTime dtNow, UCHAR ucCentiSecond, _MyGPSPoint MyPoint, FLOAT fSpeed, FLOAT fCourse);
		//---
		CHAR				GetGPSOnlineNum(VOID);							// From GPS		GGA	オンライン衛星数
		FLOAT				GetGPSAltitude(VOID);							// 				GGA	高度（m）
		DateTime			GetGPSDateTime(VOID);							// From GPS		RMC	日時（LocalTime）
		UCHAR				GetGPSCentiSecond(VOID);						//				RMC	時刻（1/10秒）
		_MyGPSPoint			GetGPSPoint(VOID);								// 				RMC	座標 （緯度,経度）
		FLOAT				GetGPSSpeed(VOID);								// 				RMC	移動速度（km/h）
		FLOAT				GetGPSCourse(VOID);								// 				RMC	進行方向
//---
		//------------------------------------------------------------------------------------------
		// IMUセンサー情報
		//------------------------------------------------------------------------------------------
		VOID				SetIMUACCELData(FLOAT fRoll, FLOAT fPitch, FLOAT fYaw);
		VOID				SetIMUAHRSData(FLOAT fRoll, FLOAT fPitch, FLOAT fYaw);
		//---
		_My3DFIMU			GetIMUACCELData(VOID);							// From MPU9250 IMU 加速度情報（Roll,Pitch,Yaw）
		_My3DFIMU			GetIMUAHRSData(VOID);							// From MPU9250 IMU 推定姿勢情報（Roll,Pitch,Yaw）
//---
		//------------------------------------------------------------------------------------------
		// Thermoセンサー情報
		//------------------------------------------------------------------------------------------
		VOID				SetVehicieDataTempAve(FLOAT fThermoAve);
		//---
		SHORT				GetVehicieTireTempAve(VOID);					// From AMG8833	AMG	タイヤ表面温度　領域平均（℃）
//---
		//------------------------------------------------------------------------------------------
		// アクセスメソッド（FDRRec SD記録用データ変換）
		//------------------------------------------------------------------------------------------
		VOID				ConvFDRStreamBuff(CHAR *pszStreamBuff);
		VOID				MyDump(VOID);
};
#endif
/*-- End Of File --*/
