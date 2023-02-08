//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyDevice/FDRRec/MyFDRHeadRec.cpp
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#include "MyBase.h"
#include "MyDevice.h"
#include "MyJARVis.h"

//--------------------------------------------------------------------------------------------------
// クラス初期メソッド
//--------------------------------------------------------------------------------------------------
_MyFDRHeadRec::_MyFDRHeadRec()
{
}
/**/
//--------------------------------------------------------------------------------------------------
// クラス終了メソッド
//--------------------------------------------------------------------------------------------------
_MyFDRHeadRec::~_MyFDRHeadRec()
{
}
/**/
//--------------------------------------------------------------------------------------------------
// BME280センサー関連
//	GetWeatherTemperature		: 気温（℃）を取得
//	GetWeatherPressure			: 気圧（hPa）を取得
//	GetWeatherApproxAltitude	: 近似高度（m）を取得
//	GetWeatherHumidity			: 湿度（%）を取得
//--------------------------------------------------------------------------------------------------
VOID			_MyFDRHeadRec::SetWeatherData(
FLOAT			fTemperature,
FLOAT			fPressure,
FLOAT			fApproxAltitude,
FLOAT			fHumidity)
{
	this->fWeatherTemperature		= fTemperature;				// From BME280	気温（℃）
	this->fWeatherPressure			= fPressure;				//				気圧（hPa）
	this->fWeatherApproxAltitude	= fApproxAltitude;			//				近似高度（ｍ）
	this->fWeatherHumidity			= fHumidity;				//				湿度（％）
}
FLOAT			_MyFDRHeadRec::GetWeatherTemperature(VOID)		{	return	this->fWeatherTemperature;		}
FLOAT			_MyFDRHeadRec::GetWeatherPressure(VOID)			{	return	this->fWeatherPressure;			}
FLOAT			_MyFDRHeadRec::GetWeatherApproxAltitude(VOID)	{	return	this->fWeatherApproxAltitude;	}
FLOAT			_MyFDRHeadRec::GetWeatherHumidity(VOID)			{	return	this->fWeatherHumidity;			}
/**/
#ifdef	MY_TPMSBLE_USE_BUILD
//--------------------------------------------------------------------------------------------------
// TPMS(BLE)センサー関連
//	SetTPMSInfoFront			: タイヤ気圧情報（前輪）を保存
//	SetTPMSInfoRear				: タイヤ気圧情報（後輪）を保存
//	GetTPMSInfoFront			: タイヤ気圧情報（前輪）を取得
//	GetTPMSInfoRear				: タイヤ気圧情報（後輪）を取得
//--------------------------------------------------------------------------------------------------
VOID			_MyFDRHeadRec::SetTPMSInfoFront(_MyTPMSInfo *pMyTPMSInfo)	{	::memcpy(&this->MyTPMSInfoFront, pMyTPMSInfo, sizeof(_MyTPMSInfo));	}
VOID			_MyFDRHeadRec::SetTPMSInfoRear(_MyTPMSInfo *pMyTPMSInfo)	{	::memcpy(&this->MyTPMSInfoRear,	 pMyTPMSInfo, sizeof(_MyTPMSInfo));	}
_MyTPMSInfo		_MyFDRHeadRec::GetTPMSInfoFront(VOID)			{	return	this->MyTPMSInfoFront;			}
_MyTPMSInfo		_MyFDRHeadRec::GetTPMSInfoRear(VOID)			{	return	this->MyTPMSInfoRear;			}
#endif
/**/
//--------------------------------------------------------------------------------------------------
// FDRRec情報をSDデバイスに書き込み様にString変換を行う(_MyFDRHeadRec情報)
//--------------------------------------------------------------------------------------------------
VOID			_MyFDRHeadRec::ConvFDRStreamBuff(
DateTime		dtAiMaiNow,
CHAR			*pszStreamBuff)
{
	//----------------------------------------------------------------------------------------------
	// ログファイル記録用文字列を組み立てる
	//----------------------------------------------------------------------------------------------
	INT		iCount;
	iCount = sprintf(pszStreamBuff,
					"FDRRec CPUTime:%8ld,RTC:%04d/%02d/%02d-%02d:%02d:%02d",
					millis(),										// millis()
					dtAiMaiNow.year(),								// システム 補正日時（年）
					dtAiMaiNow.month(),								//					（月）
					dtAiMaiNow.day(),								//					（日）
					dtAiMaiNow.hour(),								//					（時）
					dtAiMaiNow.minute(),							//					（分）
					dtAiMaiNow.second());							//					（秒）
	//
#ifdef	MY_TPMSBLE_USE_BUILD
	iCount += sprintf(&pszStreamBuff[iCount],
					",TPMS(%d,%d,%d/%d,%d,%d)",
					(INT)this->MyTPMSInfoFront.fAirPressure,		// From TPMS(F)	内圧（KPa）
					(INT)this->MyTPMSInfoFront.fTemperature,		//				温度（℃）
					(INT)this->MyTPMSInfoFront.cBattery,			//				電池残（％）
					(INT)this->MyTPMSInfoFront.fAirPressure,		// From TPMS(R)	内圧（KPa）
					(INT)this->MyTPMSInfoFront.fTemperature,		//				温度（℃）
					(INT)this->MyTPMSInfoFront.cBattery);			//				電池残（％）
#endif
	//
	iCount += sprintf(&pszStreamBuff[iCount],
					",Weather(%5.2f,%7.2f,%7.2f,%5.1f)\n",
					this->fWeatherTemperature,						// From BME280	気温（℃）
					this->fWeatherPressure,							//				気圧（hPa）
					this->fWeatherApproxAltitude,					//				近似高度（ｍ）
					this->fWeatherHumidity);						//				湿度（％）
}
/*
*/
#ifdef	MY_DEBUG_PRINT
//--------------------------------------------------------------------------------------------------
// 開発用のデータダンプ
//--------------------------------------------------------------------------------------------------
VOID			_MyFDRHeadRec::MyDump(VOID)
{
	MY_TRACE("---MyFDRHeadRec---");
	MY_LOG("Weather Temp[%5.2f]\n", this->fWeatherTemperature);			// From BME280	気温（℃）
	MY_LOG("        Pres[%7.2f]\n", this->fWeatherPressure);			//				気圧（hPa）
	MY_LOG("        Alti[%7.2f]\n", this->fWeatherApproxAltitude);		//				近似高度（ｍ）
	MY_LOG("        Humi[%5.1f]\n", this->fWeatherHumidity);			//				湿度（％）
	//---
	MY_LOG("TPMS Front KPa[%5.2f]/Temp[%4.2f]/Bat[%d]\n",
									this->MyTPMSInfoFront.fAirPressure,	// タイヤ内圧　（KPa）
									this->MyTPMSInfoFront.fTemperature,	// タイヤ内温度（℃
									this->MyTPMSInfoFront.cBattery); 	// 内蔵バッテリィ残割合（％）
	MY_LOG("TPMS Rear  KPa[%5.2f]/Temp[%4.2f]/Bat[%d]\n",
									this->MyTPMSInfoRear.fAirPressure,	// タイヤ内圧　（KPa）
									this->MyTPMSInfoRear.fTemperature,	// タイヤ内温度（℃
									this->MyTPMSInfoRear.cBattery); 	// 内蔵バッテリィ残割合（％）
}
#endif
/*-- End Of File --*/
