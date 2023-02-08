//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyTask/MyNET/MyNET.h
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#ifndef MYNET_H
#define MYNET_H

//--------------------------------------------------------------------------------------------------
// 外部参照・依存関係インクルード・ヘッダ情報
//--------------------------------------------------------------------------------------------------
#include <rpcPing.h>
#include <rpcWiFi.h>														// NET(TCP/IP@WiFi)
#include <HTTPClient.h>

//--------------------------------------------------------------------------------------------------
// ＣＯＭデバイス用クラス
//--------------------------------------------------------------------------------------------------
class   _MyNET	: public _MyTask {
	protected:
		String				strTargetURL;									// アクセス先のＵＲＬ
		String				strTargetIP;									// アクセス先のＩＰアドレス

		//------------------------------------------------------------------------------------------
		// 仮想メソッド
		//------------------------------------------------------------------------------------------
		_MyTask::_MyTaskRet	MyBegin(_MyDevice *pMyDevice, _MyJARVis *pMyJARVis)		override;
		_MyTask::_MyTaskRet	MyUpdate(_MyDevice *pMyDevice, _MyJARVis *pMyJARVis)	override;
		_MyTask::_MyTaskRet	MySetup(_MyDevice *pMyDevice, _MyJARVis *pMyJARVis)		override;

	public:
		_MyNET();
		~_MyNET();
	//---
		VOID				MyDump(VOID);
};
#endif
/*-- End Of File --*/
