//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyTask/MyI2C/BME280/MyBME280.cpp
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#include "MyBase.h"
#include "MyDevice.h"

//--
// 変換用のマクロ
//--
#define SEALEVELPRESSURE_HPA	(FLOAT)1013.25

//--------------------------------------------------------------------------------------------------
// クラス初期メソッド
//--------------------------------------------------------------------------------------------------
_MyBME280::_MyBME280()
{
}
/**/
//--------------------------------------------------------------------------------------------------
// クラス終了メソッド
//--------------------------------------------------------------------------------------------------
_MyBME280::~_MyBME280()
{
}
/**/
//--------------------------------------------------------------------------------------------------
// 気象情報（湿度・気圧センサー）(BME280)を初期化する
//--------------------------------------------------------------------------------------------------
VOID			_MyBME280::Begin(
_MyDEV_ID		MyDevID,
UCHAR			ucAddress,
TwoWire			*pTwoWire,
_MyDevice		*pMyDevice)
{
	//----------------------------------------------------------------------------------------------
	// センサー情報更新基底処理：初期処理を行う
	//----------------------------------------------------------------------------------------------
	this->BeginBaseInit(MyDevID, ucAddress, pTwoWire);

	//----------------------------------------------------------------------------------------------
	// デバイス管理クラスにタスク状態を通知する(MY_DEVSTAT_NONE)
	//----------------------------------------------------------------------------------------------
	pMyDevice->SetDeviceInfo(this->MyDevIdI2C, _MyDEV_STAT::MY_DEVSTAT_NONE);
}
/**/
//--------------------------------------------------------------------------------------------------
// 気象情報（湿度・気圧センサー）(BME280)を取得する
//--------------------------------------------------------------------------------------------------
VOID			_MyBME280::Update(
_MyDevice		*pMyDevice)
{
	//----------------------------------------------------------------------------------------------
	// センサー情報更新基底処理：事前処理を行う
	//----------------------------------------------------------------------------------------------
	this->UpdateBaseStart();

	//----------------------------------------------------------------------------------------------
	// センサーの現在状態を取得し、【センサー状態】に応じた処理を行う
	//----------------------------------------------------------------------------------------------
	switch(pMyDevice->GetDeviceInfo(this->MyDevIdI2C)) {
		case	_MyDEV_STAT::MY_DEVSTAT_NONE:									// デバイス未接続
			this->MySensorBegin(pMyDevice);
			break;
		case	_MyDEV_STAT::MY_DEVSTAT_OPEN:									// デバイス接続済み
			this->MySensorUpdate(pMyDevice);
			break;
		case	_MyDEV_STAT::MY_DEVSTAT_SETUP:									// セットアップ
			this->MySensorSetup(pMyDevice);
			break;
		case	_MyDEV_STAT::MY_DEVSTAT_FAIL:									// 接続異常
		case	_MyDEV_STAT::MY_DEVSTAT_LOST:									// データ欠落
		default:
			break;
	}

	//----------------------------------------------------------------------------------------------
	// センサー情報更新基底処理：事後処理を行う
	//----------------------------------------------------------------------------------------------
	this->UpdateBaseExit();
}
/**/
//--------------------------------------------------------------------------------------------------
// センサー関係の処理
//	MySensorBegin					センサーを初期化する
//	MySensorUpdate					センサーを更新（情報取得）する
//	MySensorSetup					センサーをセットアップする
//--------------------------------------------------------------------------------------------------
VOID			_MyBME280::MySensorBegin(
_MyDevice		*pMyDevice)
{
	//----------------------------------------------------------------------------------------------
	// センサーを初期化する
	//----------------------------------------------------------------------------------------------
	if (this->MySensor.begin(this->ucI2CAddress, this->pI2CTwoWire) == true) {
		//------------------------------------------------------------------------------------------
		// 温度補正値を設定する（＝＞詳しくはBME280ライブラリを参照）
		//------------------------------------------------------------------------------------------
		this->MySensor.setTemperatureCompensation(MyGlobalTool.MyConfigFile.fBME_Offset);

		//------------------------------------------------------------------------------------------
		// デバイス管理クラスにタスク状態を通知する(MY_DEVSTAT_OPEN)
		//------------------------------------------------------------------------------------------
		pMyDevice->SetDeviceInfo(this->MyDevIdI2C, _MyDEV_STAT::MY_DEVSTAT_OPEN);
	}
}
/**/
VOID			_MyBME280::MySensorUpdate(
_MyDevice		*pMyDevice)
{
	//----------------------------------------------------------------------------------------------
	// センサー情報を更新して、最新情報を退避領域に保存する
	//----------------------------------------------------------------------------------------------
	pMyDevice->MyFDRHeadRecNow.SetWeatherData(
							this->MySensor.readTemperature(),					// 気温（℃）の取得
							this->MySensor.readPressure() / 100.0F,				// 気圧（hPa）の取得
							this->MySensor.readAltitude(SEALEVELPRESSURE_HPA),	// 近似高度(m)の取得
							this->MySensor.readHumidity());						// 湿度（%）の取得
}
/**/
VOID			_MyBME280::MySensorSetup(
_MyDevice		*pMyDevice)
{
	//----------------------------------------------------------------------------------------------
	// デバイス管理クラスにタスク状態を通知する(MY_FDELOGID_SETUP)
	//----------------------------------------------------------------------------------------------
	pMyDevice->SetDeviceInfo(this->MyDevIdI2C, _MyDEV_STAT::MY_DEVSTAT_SETUP);
//---
	delay(500);
//---
	//----------------------------------------------------------------------------------------------
	// デバイス管理クラスにタスク状態を通知する(MY_DEVSTAT_OPEN)
	//----------------------------------------------------------------------------------------------
	pMyDevice->SetDeviceInfo(this->MyDevIdI2C, _MyDEV_STAT::MY_DEVSTAT_OPEN);
}
/*-- End Of File --*/
