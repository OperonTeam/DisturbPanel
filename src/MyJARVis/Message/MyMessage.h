//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyJARVis/Message/MyMessage.h
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#ifndef MYMESSAGE_H
#define MYMESSAGE_H

//--------------------------------------------------------------------------------------------------
// メッセージＩＤ、文字列定義
//--------------------------------------------------------------------------------------------------
typedef	enum {
	MyMsgID_INIT					=	0,
	MyMsgID_OPEN					=	1,
	MyMsgID_SETUP					=	2,
	MyMsgID_FAIL					=	3,
	MyMsgID_LOST					=	4,
	//---
	MyMsgID_GPS_SETUP				=	5,
	MyMsgID_RTC_SETUP				=	6,
	MyMsgID_IMU_SETUP				=	7,
	MyMsgID_GPS_FAIL				=	8,
	MyMsgID_FDR_FAIL				=	9,
	MyMsgID_GPS_LOST				=	10,
	MyMsgID_FDR_LOST				=	11,
	//---
	MyMsgID_OPERONOPERON			=	12,
	MyMsgID_TAKEBREAK				=	13,
	MyMsgID_TOPSPEED				=	14,
	MyMsgID_AVESPEED				=	15,
	//---
	MyMsgID_COUNT					=	16
} _MyMsgID;
//
//--------------------------------------------------------------------------------------------------
// 執事のメッセージ用クラス
//--------------------------------------------------------------------------------------------------
class   _MyMessage {
	private:
		String			strMsgTbl[_MyMsgID::MyMsgID_COUNT];

		//------------------------------------------------------------------------------------------
		// メッセージ定義
		//------------------------------------------------------------------------------------------
		struct _MyDevMsgTable {
			String		strDevName;
			_MyMsgID	MyMsgID[_MyDEV_STAT::MY_DEVSTAT_COUNT];
		};
		_MyDevMsgTable	MyMsgTable[_MyDEV_ID::MY_DEVID_COUNT];

	public:
		_MyMessage();
		~_MyMessage();

		//------------------------------------------------------------------------------------------
		// 執事のメッセージ情報・アクセスメソッド
		//------------------------------------------------------------------------------------------
		String				GetStringMsg(_MyMsgID MyMsgID);
		_MyDEV_STAT			GetDeviceInfoMsg(_MyDevice *pMyDevice, _MyDEV_ID MyDevID, String *pstrMsgString);
		// 
		String				GetStringDateTimeMDHM(DateTime dtNow);
};
#endif
/*-- End Of File --*/
