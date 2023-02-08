//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyDevice/FDRRec/MyFDRBodyRec.cpp
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#include "MyBase.h"
#include "MyDevice.h"
#include "MyJARVis.h"

//--------------------------------------------------------------------------------------------------
// クラス初期メソッド
//--------------------------------------------------------------------------------------------------
_MyFDRBodyRec::_MyFDRBodyRec()
{
}
/**/
//--------------------------------------------------------------------------------------------------
// クラス終了メソッド
//--------------------------------------------------------------------------------------------------
_MyFDRBodyRec::~_MyFDRBodyRec()
{
}
/**/
//--------------------------------------------------------------------------------------------------
//	SetFDRRecordSeq					: FDRRec 管理レコード番号
//--------------------------------------------------------------------------------------------------
VOID			_MyFDRBodyRec::SetRecordSeq(ULONG _ulRecordSeq)
{
	this->ulRecordSeq = _ulRecordSeq;							// RTC情報をFDRRec領域に保存する
}
/**/
//--------------------------------------------------------------------------------------------------
// GPSセンサー取得データ(GGA)を退避する
//--------------------------------------------------------------------------------------------------
VOID			_MyFDRBodyRec::SetGPSGGAData(
CHAR			cOnline,
FLOAT			fAltitude)
{
	//----------------------------------------------------------------------------------------------
	// 自クラス内で管理するデータを退避
	//----------------------------------------------------------------------------------------------
	this->cGPSOnlineNum		= cOnline;							// From GPS		オンライン衛星数
	this->fGPSAltitude		= fAltitude;						// 				高度（m）
}
/**/
//--------------------------------------------------------------------------------------------------
// GPSセンサー取得データ(RMC)を退避する
//--------------------------------------------------------------------------------------------------
VOID			_MyFDRBodyRec::SetGPSRMCData(
DateTime		dtDateTime,
UCHAR			ucCentiSecond,
_MyGPSPoint		MyPoint,
FLOAT			fSpeed,
FLOAT			fCourse)
{
	//----------------------------------------------------------------------------------------------
	// 自クラス内で管理するデータを退避
	//----------------------------------------------------------------------------------------------
	this->dtGPSDateTime		= dtDateTime;						// From GPS		日時（LocalTime）
	this->ucGPSCentiSecond	= ucCentiSecond;					//				1/10秒
	this->MyGPSPoint		= MyPoint;							// 				座標 （緯度,経度）
	this->fGPSSpeed			= fSpeed;							// 				移動速度（km/h）
	this->fGPSCourse		= fCourse;							// 				進行方向
}
/**/
//--------------------------------------------------------------------------------------------------
// Thermoセンサー取得データを退避する
// 実際の生データは、AMG8833の作業領域として直接Privateメンバに保存されている
//--------------------------------------------------------------------------------------------------
VOID			_MyFDRBodyRec::SetVehicieDataTempAve(
FLOAT			fThermoAve)
{
	//----------------------------------------------------------------------------------------------
	// FDRRecにデータを退避
	//----------------------------------------------------------------------------------------------
	this->sVehicieTireTempAve = fThermoAve;
}
/**/
//--------------------------------------------------------------------------------------------------
// IMUセンサー取得データを退避する
//--------------------------------------------------------------------------------------------------
VOID			_MyFDRBodyRec::SetIMUACCELData(
FLOAT			_fRoll,
FLOAT			_fPitch,
FLOAT			_fYaw)
{
	this->My3DFACCEL.fRoll	= _fRoll;
	this->My3DFACCEL.fPitch	= _fPitch;
	this->My3DFACCEL.fYaw	= _fYaw;
}
/**/
//--------------------------------------------------------------------------------------------------
// IMUセンサー取得データを退避する
//--------------------------------------------------------------------------------------------------
VOID			_MyFDRBodyRec::SetIMUAHRSData(
FLOAT			_fRoll,
FLOAT			_fPitch,
FLOAT			_fYaw)
{
	this->My3DFAHRS.fRoll	= _fRoll;
	this->My3DFAHRS.fPitch	= _fPitch;
	this->My3DFAHRS.fYaw	= _fYaw;
}
/**/
//--------------------------------------------------------------------------------------------------
// アクセスメソッド
//--------------------------------------------------------------------------------------------------
ULONG			_MyFDRBodyRec::GetRecordSeq(VOID)			{	return  this->ulRecordSeq;			}	// FDRRec情報からレコード通番情報を取得する
CHAR			_MyFDRBodyRec::GetGPSOnlineNum(VOID)		{	return  this->cGPSOnlineNum;		}	// FDRRec情報からGPS情報（オンライン衛星数）を取得する
DateTime		_MyFDRBodyRec::GetGPSDateTime(VOID)			{	return  this->dtGPSDateTime;		}	// FDRRec情報からGPS情報（システム日時）を取得する
UCHAR			_MyFDRBodyRec::GetGPSCentiSecond(VOID)		{	return	this->ucGPSCentiSecond;		}	// FDRRec情報からGPS情報（システム日時1/10秒）を取得する
_MyGPSPoint		_MyFDRBodyRec::GetGPSPoint(VOID)			{	return  this->MyGPSPoint;			}	// FDRRec情報からGPS情報（ＧＰＳ座標）を取得する
FLOAT			_MyFDRBodyRec::GetGPSSpeed(VOID)			{	return  this->fGPSSpeed;			}	// FDRRec情報からGPS情報（速度）を取得する
FLOAT			_MyFDRBodyRec::GetGPSAltitude(VOID)			{	return	this->fGPSAltitude;			}	// FDRRec情報からGPS情報（高度（m））を取得する
FLOAT			_MyFDRBodyRec::GetGPSCourse(VOID)			{	return  this->fGPSCourse;			}	// FDRRec情報からGPS情報（進行方向）を取得する
SHORT			_MyFDRBodyRec::GetVehicieTireTempAve(VOID)	{	return  this->sVehicieTireTempAve;	}	// FDRRec情報からVehicie情報（タイヤ・全領域平均温度）を取得する
_My3DFIMU		_MyFDRBodyRec::GetIMUAHRSData(VOID)			{	return	this->My3DFAHRS;			}	// From MPU9250 IMU 推定姿勢情報（3D）
_My3DFIMU		_MyFDRBodyRec::GetIMUACCELData(VOID)		{	return	this->My3DFACCEL;			}	// From MPU9250 IMU 加速度情報（3D）
/**/
//--------------------------------------------------------------------------------------------------
// FDRRec情報をSDデバイスに書き込み様にString変換を行う(_MyFDRRec情報)
//--------------------------------------------------------------------------------------------------
VOID			_MyFDRBodyRec::ConvFDRStreamBuff(
CHAR			*pszStreamBuff)
{
	//----------------------------------------------------------------------------------------------
	// ログファイル記録用文字列を組み立てる
	// システムコール:sprintf()の呼び出し回数は少ない方が良いのは事実。
	//----------------------------------------------------------------------------------------------
	INT		iCount = 0;
	iCount = sprintf(pszStreamBuff,
					"%06lu,%02d:%02d:%02d.%03d",
					this->ulRecordSeq,								// ログ記録レコードカウンタ
					this->dtGPSDateTime.hour(),						// ＧＰＳ日時（時）
					this->dtGPSDateTime.minute(),					// ＧＰＳ日時（分）
					this->dtGPSDateTime.second(),					// ＧＰＳ日時（秒）
					this->ucGPSCentiSecond);						// ＧＰＳ日時（センチ秒）
	//
	iCount += sprintf(&pszStreamBuff[iCount],
					",GPS:%02d,(%10.6lf,%10.6lf)",
					this->cGPSOnlineNum,							// ＧＰＳオンライン衛星数
					this->MyGPSPoint.dLat,							// ＧＰＳ座標値（ＬＡＴ）
					this->MyGPSPoint.dLng);							// ＧＰＳ座標値（ＬＮＧ）
	//
	iCount += sprintf(&pszStreamBuff[iCount],
					",Alt:%6.1f,Speed:%05.1f",
					this->fGPSAltitude,								// ＧＰＳ高度
					this->fGPSSpeed);								// ＧＰＳ速度
	//
	iCount += sprintf(&pszStreamBuff[iCount],
					",IMU(%9.4f,%9.4f,%9.4f),Tire:%03d\n",
					this->My3DFAHRS.fRoll,							// ＩＭＵ姿勢Ｒｏｌｌ
					this->My3DFAHRS.fPitch,							// ＩＭＵ姿勢Ｐｉｔｃｈ
					this->My3DFAHRS.fYaw,							// ＩＭＵ姿勢Ｙａｗ
					//---
					this->sVehicieTireTempAve);						// タイヤ表面温度・領域平均（℃）
}
/*
*/
#ifdef	MY_DEBUG_PRINT
//--------------------------------------------------------------------------------------------------
// 開発用のデータダンプ
//--------------------------------------------------------------------------------------------------
VOID		_MyFDRBodyRec::MyDump(VOID)
{
	MY_TRACE("---MyFDRBodyRec---");
	MY_LOG("  ulRecordSeq:[%ld]\n", this->ulRecordSeq);			// For FDR		Record Sequence Number
	MY_LOG("  dtGPSDatTime[%04d/%02d/%02d %02d:%02d:%02d.%03d\n",
			this->dtGPSDateTime.year(),	this->dtGPSDateTime.month(), this->dtGPSDateTime.day(),
			this->dtGPSDateTime.hour(), this->dtGPSDateTime.minute(),this->dtGPSDateTime.second(),
			this->ucGPSCentiSecond);
	MY_LOG("  GPS Online:[%02d]\n", this->cGPSOnlineNum);		// 				オンライン衛星数
	MY_LOG("  GPS Point:[%10.6lf, %10.6lf]\n",
			this->MyGPSPoint.dLat, this->MyGPSPoint.dLng);	 	// 				座標 （緯度,経度） 123.123456
	MY_LOG("  GPS Speed:[%05.1lf]\n", this->fGPSSpeed);			// 				移動速度（km/h）
	MY_LOG("  GPS Course:[%06.2f]\n", this->fGPSCourse);		// 				進行方向
	MY_LOG("  GPS Altitude:[%06.2f]\n", this->fGPSAltitude);	// 				高度（m）
	MY_LOG("  IMU Roll:[%9.4f] / Pitch:[%9.4f] / Yaw:[%9.4f]\n",
			this->fAHRSRoll, this->fAHRSPitch, this->fAHRSYaw);	// For IMU		Roll/Pitch/Yaw
	MY_LOG("  AMG Temp:[%03d]\n", this->sVehicieTireTempAve);	// For AMG8833	タイヤ表面温度　領域平均（℃）
}
#endif
/*-- End Of File --*/
