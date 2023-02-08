//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyTask/MyI2C/MyI2C.h
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#ifndef MYI2C_H
#define MYI2C_H

//--------------------------------------------------------------------------------------------------
// 外部参照・依存関係インクルード・ヘッダ情報
//--------------------------------------------------------------------------------------------------
#include "MyI2CBase.h"
#include "BME280/MyBME280.h"											// 気象センサー
#include "MPU9250/MyMPU9250.h"											// ９軸センサー
#include "PAJ7620/MyPAJ7620.h"											// ジェスチャーセンサー
#include "AMG8833/MyAMG8833.h"											// サーモセンサー
//
//--------------------------------------------------------------------------------------------------
// Ｉ２Ｃデバイス用クラス
//--------------------------------------------------------------------------------------------------
class   _MyI2C : public _MyTask {
	protected:
		//------------------------------------------------------------------------------------------
		// センサー・アクセスクラスの実体
		//------------------------------------------------------------------------------------------
		_MyBME280				MyBME280;							// BME280センサー
		_MyAMG8833				MyAMG8833;							// AMG8833センサー
		_MyMPU9250				MyMPU9250;							// MPU9250センサー
		_MyPAJ7620				MyPAJ7620;							// PAJ7620センサー
		//
		CHAR					cRunType;							// 更新実行センサー種別
		//------------------------------------------------------------------------------------------
		// 仮想メソッド
		//------------------------------------------------------------------------------------------
		_MyTask::_MyTaskRet	MyBegin(_MyDevice *pMyDevice, _MyJARVis *pMyJARVis)		override;
		_MyTask::_MyTaskRet	MyUpdate(_MyDevice *pMyDevice, _MyJARVis *pMyJARVis)	override;
		_MyTask::_MyTaskRet	MySetup(_MyDevice *pMyDevice, _MyJARVis *pMyJARVis)		override;

	public:
		_MyI2C();
		~_MyI2C();

		//------------------------------------------------------------------------------------------
		// DUMP
		//------------------------------------------------------------------------------------------
		VOID					MyDump(VOID);
};
#endif
/*-- End Of File --*/