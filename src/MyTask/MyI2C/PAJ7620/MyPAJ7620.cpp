//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyTask/MyI2C/PAJ7620/MyPAJ7620.cpp
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#include "MyBase.h"
#include "MyDevice.h"

//
//--------------------------------------------------------------------------------------------------
// クラス初期メソッド
//--------------------------------------------------------------------------------------------------
_MyPAJ7620::_MyPAJ7620()
{
}
/**/
//--------------------------------------------------------------------------------------------------
// クラス終了メソッド
//--------------------------------------------------------------------------------------------------
_MyPAJ7620::~_MyPAJ7620()
{
}
/**/
//--------------------------------------------------------------------------------------------------
// センサーを初期化する
//--------------------------------------------------------------------------------------------------
VOID			_MyPAJ7620::Begin(
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
// センサー情報を更新する
//--------------------------------------------------------------------------------------------------
VOID			_MyPAJ7620::Update(
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
VOID			_MyPAJ7620::MySensorBegin(
_MyDevice		*pMyDevice)
{
	//----------------------------------------------------------------------------------------------
	// Initialize PAJ7620U2 (configure I2C and initial values)
	// ライブラリ側で利用している、I2CアドレスとWireは固定。（ライブラリに手を入れるのは相当先、、、）
	//----------------------------------------------------------------------------------------------
	if (this->MySensor.begin() == 0) {
		//------------------------------------------------------------------------------------------
		// 初期動作パラメータを設定する
		//------------------------------------------------------------------------------------------
		this->MySensor.setGestureHighRate(MY_TRUE);						// Ｈｉゲイン

		//------------------------------------------------------------------------------------------
		// デバイス管理クラスにタスク状態を通知する(MY_DEVSTAT_OPEN)
		//------------------------------------------------------------------------------------------
		pMyDevice->SetDeviceInfo(this->MyDevIdI2C, _MyDEV_STAT::MY_DEVSTAT_OPEN);
	}
}
/**/
VOID			_MyPAJ7620::MySensorUpdate(
_MyDevice		*pMyDevice)
{
	//----------------------------------------------------------------------------------------------
	// ジェスチャー情報を取得する
	//----------------------------------------------------------------------------------------------
	USHORT		usGesture = MY_KEYPAD_EVENT_NOTNOTIFY;
	DFRobot_PAJ7620U2::eGesture_t PAJGesture = this->MySensor.getGesture();
	if (PAJGesture != this->MySensor.eGestureNone) {
		//------------------------------------------------------------------------------------------
		// デバイス管理クラスにタスク状態を通知する(MY_DEVSTAT_LOST)
		//------------------------------------------------------------------------------------------
		pMyDevice->SetDeviceInfo(this->MyDevIdI2C,_MyDEV_STAT::MY_DEVSTAT_LOST);

		//------------------------------------------------------------------------------------------
		// ジェスチャー結果を取得して退避します。
		// －＞センサーを実車両に取付後、ジェスチャー方向を定めます
		//------------------------------------------------------------------------------------------
		switch(PAJGesture) {
		//--case	DFRobot_PAJ7620U2::eGestureNone:						usGesture = MY_KEYPAD_EVENT_NOTNOTIFY;	break;	// "None"
			case	DFRobot_PAJ7620U2::eGestureRight:						usGesture = MY_KEYPAD_STICK_RIGHT;		break;	// "Right"
			case	DFRobot_PAJ7620U2::eGestureLeft:						usGesture = MY_KEYPAD_STICK_LEFT;		break;	// "Left"
			case	DFRobot_PAJ7620U2::eGestureUp:							usGesture = MY_KEYPAD_STICK_UP;			break;	// "Up"
			case	DFRobot_PAJ7620U2::eGestureDown:						usGesture = MY_KEYPAD_STICK_DOWN;		break;	// "Down"
			case	DFRobot_PAJ7620U2::eGestureForward:						usGesture = MY_KEYPAD_STICK_PUSH;		break;	// "Forward"
			case	DFRobot_PAJ7620U2::eGestureBackward:					usGesture = MY_KEYPAD_STICK_PULL;		break;	// "Backward"
			case	DFRobot_PAJ7620U2::eGestureClockwise:					usGesture = MY_KEYPAD_BTN_A_CLICK;		break;	// "Clockwise"
			case	DFRobot_PAJ7620U2::eGestureAntiClockwise:				usGesture = MY_KEYPAD_BTN_B_CLICK;		break;	// "Anti-Clockwise"
			case	DFRobot_PAJ7620U2::eGestureWave:						usGesture = MY_KEYPAD_BTN_C_CLICK;		break;	// "Wave"
			case	DFRobot_PAJ7620U2::eGestureWaveSlowlyDisorder:															// "WaveSlowlyDisorder"
			case	DFRobot_PAJ7620U2::eGestureWaveSlowlyLeftRight:															// "WaveSlowlyLeftRight"
			case	DFRobot_PAJ7620U2::eGestureWaveSlowlyUpDown:															// "WaveSlowlyUpDown"
			case	DFRobot_PAJ7620U2::eGestureWaveSlowlyForwardBackward:													// "WaveSlowlyForwardBackward"
			default:
				break;
		}
		//------------------------------------------------------------------------------------------
		// センサー情報を読み取る前に、セットしたイベント情報を初期化する
		// デバイス管理クラスにタスク状態を通知する(MY_DEVSTAT_OPEN)
		//------------------------------------------------------------------------------------------
		pMyDevice->SetDeviceInfo(this->MyDevIdI2C,_MyDEV_STAT::MY_DEVSTAT_OPEN);

		//------------------------------------------------------------------------------------------
		// 最新ジェスチャーセンサーの取得値を（ＯＲ）積算して保存する
		//------------------------------------------------------------------------------------------
		pMyDevice->SetButtonInfo(usGesture);
	}
}
/**/
VOID			_MyPAJ7620::MySensorSetup(
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
