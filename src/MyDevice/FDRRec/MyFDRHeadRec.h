//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyDevice/FDRRec/MyFDRHeadRec.h
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#ifndef MYFDRHEADREC_H
#define MYFDRHEADREC_H

//--------------------------------------------------------------------------------------------------
// FDRHeadRec管理用クラス
//	1分毎に1レコード生成される、FDRの管理報
//--------------------------------------------------------------------------------------------------
class   _MyFDRHeadRec {
	private:
		//------------------------------------------------------------------------------------------
		// FDRHeadRec情報
		//------------------------------------------------------------------------------------------
		FLOAT				fWeatherTemperature;							// From BME280	気温（℃）
		FLOAT				fWeatherPressure;								//				気圧（hPa）
		FLOAT				fWeatherApproxAltitude;							//				近似高度（ｍ）
		FLOAT				fWeatherHumidity;								//				湿度（％）
#ifdef	MY_TPMSBLE_USE_BUILD
		_MyTPMSInfo			MyTPMSInfoFront;								// For TPMS		タイヤ情報（前輪）
		_MyTPMSInfo			MyTPMSInfoRear;									// For TPMS		タイヤ情報（後輪）
#endif
//---
	public:
		_MyFDRHeadRec();
		~_MyFDRHeadRec();

		//------------------------------------------------------------------------------------------
		// BME(Weaher)センサー関連
		//------------------------------------------------------------------------------------------
		VOID				SetWeatherData(FLOAT fTemperature, FLOAT fPressure, FLOAT fApproxAltitude, FLOAT fHumidity);
		//---
		FLOAT				GetWeatherTemperature(VOID);					// From BME280	気温（℃）の取得
		FLOAT				GetWeatherPressure(VOID);						//				気圧（hPa）の取得
		FLOAT				GetWeatherApproxAltitude(VOID);					//				近似高度（m）の取得
		FLOAT				GetWeatherHumidity(VOID);						//				湿度（%）の取得

		//------------------------------------------------------------------------------------------
		// TPMS(BLE)センサー関連
		//------------------------------------------------------------------------------------------
#ifdef	MY_TPMSBLE_USE_BUILD
		VOID				SetTPMSInfoFront(_MyTPMSInfo *pMyTPMSInfo);		// From TPMS	フロントTPMSの設定
		VOID				SetTPMSInfoRear(_MyTPMSInfo *pMyTPMSInfo);		//				リアTPMSの設定
		//---
		_MyTPMSInfo			GetTPMSInfoFront(VOID);							//				フロントTPMSの取得
		_MyTPMSInfo			GetTPMSInfoRear(VOID);							//				リアTPMSの取得
#endif
		//------------------------------------------------------------------------------------------
		// アクセスメソッド（FDRRec SD記録用データ変換）
		//------------------------------------------------------------------------------------------
		VOID				ConvFDRStreamBuff(DateTime dtAiMaiNow, CHAR *pszStreamBuff);
//---
		VOID				MyDump(VOID);
};
#endif
/*-- End Of File --*/
