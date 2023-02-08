//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyJARVis/Message/MyMessage.cpp
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#include "MyBase.h"
#include "MyDevice.h"
#include "MyJARVis.h"

//--------------------------------------------------------------------------------------------------
// クラス初期メソッド
//--------------------------------------------------------------------------------------------------
_MyMessage::_MyMessage()
{
	//----------------------------------------------------------------------------------------------
	// メッセージを定義
	//----------------------------------------------------------------------------------------------
	//--- 共通メッセージ
	this->strMsgTbl[_MyMsgID::MyMsgID_INIT]			= F("Sensor Disconnect");					// センサ未接続
	this->strMsgTbl[_MyMsgID::MyMsgID_OPEN]			= F("Sensor Connected");					// センサ接続済み
	this->strMsgTbl[_MyMsgID::MyMsgID_SETUP]		= F("Setup Now");							// セットアップ中
	this->strMsgTbl[_MyMsgID::MyMsgID_FAIL]			= F("Sensor Abort");						// センサ異常
	this->strMsgTbl[_MyMsgID::MyMsgID_LOST]			= F("Data Lost");							// データ喪失
	
	//--- 個別メッセージ
	this->strMsgTbl[_MyMsgID::MyMsgID_GPS_SETUP]	= F("Sending Setup Command");				// 設定コマンド送信中
	this->strMsgTbl[_MyMsgID::MyMsgID_RTC_SETUP]	= F("Adjust TIME Now");						// 時刻補正中
	this->strMsgTbl[_MyMsgID::MyMsgID_IMU_SETUP]	= F("IMU Setup");							// 軸補正中
	this->strMsgTbl[_MyMsgID::MyMsgID_GPS_FAIL]		= F("GPS Sensor Disconnect");				// センサ通信切断
	this->strMsgTbl[_MyMsgID::MyMsgID_FDR_FAIL]		= F("Abort FDR Recording");					// 記録処理異常
	this->strMsgTbl[_MyMsgID::MyMsgID_GPS_LOST]		= F("Search GPS Satellite");				// 衛星探索中
	this->strMsgTbl[_MyMsgID::MyMsgID_FDR_LOST]		= F("Lost FDR Recording Info");				// 記録情報喪失

	//--- JARVisメッセージ
	this->strMsgTbl[_MyMsgID::MyMsgID_TAKEBREAK]	= F("You need a Break!");					// 休憩が必要です
	this->strMsgTbl[_MyMsgID::MyMsgID_TOPSPEED]		= F("Maximux Speed:");						// 最高速度:
	this->strMsgTbl[_MyMsgID::MyMsgID_AVESPEED]		= F("Average Speed:");						// 平均速度:

	//----------------------------------------------------------------------------------------------
	// デバイス・ステータスメッセージを定義					+---------------------------+-----------------------+-------------------------------+---------------------------+---------------------------+
	//------------------------------------------------------|INITIALIZE					|OPEN					|SETUP							|FAIL						|LOST						|
	this->MyMsgTable[_MyDEV_ID::MY_DEVID_GPS] = {F("GPS"),	{_MyMsgID::MyMsgID_INIT,	_MyMsgID::MyMsgID_OPEN,	_MyMsgID::MyMsgID_GPS_SETUP,	_MyMsgID::MyMsgID_GPS_FAIL,	_MyMsgID::MyMsgID_GPS_LOST	}};
	this->MyMsgTable[_MyDEV_ID::MY_DEVID_NET] = {F("NET"),	{_MyMsgID::MyMsgID_INIT,	_MyMsgID::MyMsgID_OPEN,	_MyMsgID::MyMsgID_SETUP,		_MyMsgID::MyMsgID_FAIL,		_MyMsgID::MyMsgID_LOST		}};
	this->MyMsgTable[_MyDEV_ID::MY_DEVID_RTC] = {F("RTC"),	{_MyMsgID::MyMsgID_INIT,	_MyMsgID::MyMsgID_OPEN,	_MyMsgID::MyMsgID_RTC_SETUP,	_MyMsgID::MyMsgID_FAIL,		_MyMsgID::MyMsgID_LOST		}};
	this->MyMsgTable[_MyDEV_ID::MY_DEVID_IMU] = {F("IMU"),	{_MyMsgID::MyMsgID_INIT,	_MyMsgID::MyMsgID_OPEN,	_MyMsgID::MyMsgID_IMU_SETUP,	_MyMsgID::MyMsgID_FAIL,		_MyMsgID::MyMsgID_LOST		}};
	this->MyMsgTable[_MyDEV_ID::MY_DEVID_BME] = {F("BME"),	{_MyMsgID::MyMsgID_INIT,	_MyMsgID::MyMsgID_OPEN,	_MyMsgID::MyMsgID_SETUP,		_MyMsgID::MyMsgID_FAIL,		_MyMsgID::MyMsgID_LOST		}};
	this->MyMsgTable[_MyDEV_ID::MY_DEVID_AMG] = {F("AMG"),	{_MyMsgID::MyMsgID_INIT,	_MyMsgID::MyMsgID_OPEN,	_MyMsgID::MyMsgID_SETUP,		_MyMsgID::MyMsgID_FAIL,		_MyMsgID::MyMsgID_LOST		}};
	this->MyMsgTable[_MyDEV_ID::MY_DEVID_FDR] = {F("FDR"),	{_MyMsgID::MyMsgID_INIT,	_MyMsgID::MyMsgID_OPEN,	_MyMsgID::MyMsgID_SETUP,		_MyMsgID::MyMsgID_FDR_FAIL,	_MyMsgID::MyMsgID_FDR_LOST	}};
	this->MyMsgTable[_MyDEV_ID::MY_DEVID_PAJ] = {F("PAJ"),	{_MyMsgID::MyMsgID_INIT,	_MyMsgID::MyMsgID_OPEN,	_MyMsgID::MyMsgID_SETUP,		_MyMsgID::MyMsgID_FAIL,		_MyMsgID::MyMsgID_LOST		}};
	this->MyMsgTable[_MyDEV_ID::MY_DEVID_HAL] = {F("HAL"),	{_MyMsgID::MyMsgID_INIT,	_MyMsgID::MyMsgID_OPEN,	_MyMsgID::MyMsgID_SETUP,		_MyMsgID::MyMsgID_FAIL,		_MyMsgID::MyMsgID_LOST		}};
}
/**/
//--------------------------------------------------------------------------------------------------
// クラス終了メソッド
//--------------------------------------------------------------------------------------------------
_MyMessage::~_MyMessage()
{
}
/**/
String			_MyMessage::GetStringMsg(
_MyMsgID		MyMsgID)
{
	return	this->strMsgTbl[MyMsgID];
}
/**/
_MyDEV_STAT		_MyMessage::GetDeviceInfoMsg(
_MyDevice		*pMyDevice,
_MyDEV_ID		MyDevID,
String			*pstrMsgString)
{
	//----------------------------------------------------------------------------------------------
	// 入力されたデバイスＩＤの【状態】を取得する
	//----------------------------------------------------------------------------------------------
	_MyDEV_STAT	MyDEVStat = pMyDevice->GetDeviceInfo(MyDevID);

	//----------------------------------------------------------------------------------------------
	// 取得した入力されたデバイスＩＤ【状態】の【デバイス名称】と【メッセージＩＤ】を取得する
	//----------------------------------------------------------------------------------------------
	String		strWork = MyMsgTable[MyDevID].strDevName;
	_MyMsgID	MyMsgID = MyMsgTable[MyDevID].MyMsgID[MyDEVStat];

	//----------------------------------------------------------------------------------------------
	// 【メッセージＩＤ】から【メッセージ文字列】を取得して、文字列を結合する
	//----------------------------------------------------------------------------------------------
	*pstrMsgString = strWork + this->strMsgTbl[MyMsgID];

	return	MyDEVStat;
}
/**/
String		_MyMessage::GetStringDateTimeMDHM(DateTime dtNow)
{
	//----------------------------------------------------------------------------------------------
	// 経過時間（秒）を文字列に編集する
	//----------------------------------------------------------------------------------------------
	CHAR	szMsgBuff[32];
	sprintf(szMsgBuff, "DateTime:%02d/%02d %02d:%02d",
									dtNow.month(), dtNow.day(),
									dtNow.hour(), dtNow.minute());
	return	String(szMsgBuff);
}
/*-- End Of File --*/
