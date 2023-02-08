//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyTask/MyI2C/MPU9250/MyMPU9250.cpp
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#include "MyBase.h"
#include "MyDevice.h"

//--------------------------------------------------------------------------------------------------
// クラス初期メソッド
//--------------------------------------------------------------------------------------------------
_MyMPU9250::_MyMPU9250()
{
}
/**/
//--------------------------------------------------------------------------------------------------
// クラス終了メソッド
//--------------------------------------------------------------------------------------------------
_MyMPU9250::~_MyMPU9250()
{
}
/**/
//--------------------------------------------------------------------------------------------------
// ＩＭＵデバイスを初期化する（＆センサーデータのオフセット値を取得する）
//--------------------------------------------------------------------------------------------------
VOID			_MyMPU9250::Begin(
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
// ＩＭＵセンサー値から現在の姿勢状態を取得する
//--------------------------------------------------------------------------------------------------
VOID			_MyMPU9250::Update(
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
VOID			_MyMPU9250::MySensorBegin(
_MyDevice		*pMyDevice)
{
	//----------------------------------------------------------------------------------------------
	// ＩＭＵセンサー／６軸＋磁気センサー(MPU9250+6500)（外部）を初期化する
	// 「伏角（ふっかく）」と「偏角（へんかく）
	//----------------------------------------------------------------------------------------------
	this->MySensor.verbose(MY_FALSE);											// Vervose：ON
//
	// 以下の設定がデフォルト値
	this->MyMPU9250Setting.accel_fs_sel = ACCEL_FS_SEL::A16G;					//
	this->MyMPU9250Setting.gyro_fs_sel = GYRO_FS_SEL::G2000DPS;					//
	this->MyMPU9250Setting.mag_output_bits = MAG_OUTPUT_BITS::M16BITS;			// M14BITS
	this->MyMPU9250Setting.fifo_sample_rate = FIFO_SAMPLE_RATE::SMPL_200HZ;		//
	this->MyMPU9250Setting.gyro_fchoice = {0x03};								//
	this->MyMPU9250Setting.gyro_dlpf_cfg = GYRO_DLPF_CFG::DLPF_41HZ;			// ジャイロセンサのハイパスフィルタ周波数
	this->MyMPU9250Setting.accel_fchoice = {0x01};								//
	this->MyMPU9250Setting.accel_dlpf_cfg = ACCEL_DLPF_CFG::DLPF_45HZ;			// 加速度センサのローパスフィルタ周波数
	//---
	//--- Overwrite Parameter
	//---
	this->MyMPU9250Setting.accel_fs_sel = ACCEL_FS_SEL::A2G;					//
	this->MyMPU9250Setting.gyro_fs_sel = GYRO_FS_SEL::G500DPS;					//
	this->MyMPU9250Setting.gyro_dlpf_cfg = GYRO_DLPF_CFG::DLPF_10HZ;			// ジャイロセンサのハイパスフィルタ周波数
	this->MyMPU9250Setting.accel_dlpf_cfg = ACCEL_DLPF_CFG::DLPF_10HZ;			//
//
	if (this->MySensor.setup(this->ucI2CAddress, this->MyMPU9250Setting, *(this->pI2CTwoWire))) {
		//------------------------------------------------------------------------------------------
		// フィルタを設定する
		//------------------------------------------------------------------------------------------
//		this->MySensor.selectFilter(QuatFilterSel::MADGWICK);					// 姿勢推定法：Madgwick Default
		this->MySensor.selectFilter(QuatFilterSel::MAHONY);						// 姿勢推定法：Mahony
//		this->MySensor.selectFilter(QuatFilterSel::NONE);						// 姿勢推定法：None
		this->MySensor.setFilterIterations(20);									// フィルター計算の反復数 Default is 1
		//
		//------------------------------------------------------------------------------------------
		// デバイス管理クラスにタスク状態を通知する(MY_DEVSTAT_SETUP)
		//------------------------------------------------------------------------------------------
		pMyDevice->SetDeviceInfo(this->MyDevIdI2C, _MyDEV_STAT::MY_DEVSTAT_SETUP);
	}
	this->MySensor.verbose(MY_FALSE);											// Vervose：OFF
}
/**/
VOID			_MyMPU9250::MySensorUpdate(
_MyDevice		*pMyDevice)
{
	//--------------------------------------------------------------------------------------
	// まぁこんな感じで、IMUデータを取得
	//--------------------------------------------------------------------------------------
	if (this->MySensor.update()) {
		//----------------------------------------------------------------------------------
		// センサー最新情報を退避領域に保存する
		// 進行方向に対して90度向きが異る為、センサ取得座標を入れ替えます。
		//----------------------------------------------------------------------------------
		pMyDevice->MyFDRBodyRecNow.SetIMUAHRSData(
							this->MySensor.getPitch(),							// 本来は、Roll
							this->MySensor.getRoll(),							// 本来は、Pitch
							this->MySensor.getYaw());							// Yawは入れ替えない（地磁方位）
		pMyDevice->MyFDRBodyRecNow.SetIMUACCELData(
							this->MySensor.getAccX(),							// 本来は、Roll
							this->MySensor.getAccY(),							// 本来は、Pitch
							this->MySensor.getAccZ());							// Yawは入れ替えない（地磁方位）
//
#ifdef	MY_DEBUG_I2C_MPU9250_DUMP
		MY_LOG("MyMPU9259::MySensorUpdate X,Y,Z = [%8.3f, %8.3f, %8.3f]\n", this->MySensor.getPitch(), this->MySensor.getRoll(), this->MySensor.getYaw()); 
#endif
	} else {
		MY_LOG("IMU Update Error\n");
	}
}
/**/
VOID			_MyMPU9250::MySensorSetup(
_MyDevice		*pMyDevice)
{
	//----------------------------------------------------------------------------------------------
	// デバイス管理クラスにタスク状態を通知する(MY_DEVSTAT_SETUP)
	//----------------------------------------------------------------------------------------------
	pMyDevice->SetDeviceInfo(this->MyDevIdI2C, _MyDEV_STAT::MY_DEVSTAT_SETUP);

	//----------------------------------------------------------------------------------------------
	// ＩＭＵセンサーの設定情報を取得する
	//----------------------------------------------------------------------------------------------
	INT			iDebug = 0;
	INT			i8Figure = 0;
	FLOAT		fMagDecl = (FLOAT)0.0;
	_My3DFIMU	My3DFBias;
	_My3DFIMU	My3DFScale;

//	My3DFBias	= MyConfig.Get3DF(MYSETP_IMU_MAGBIAS);							// 磁気情報傾向
//	My3DFScale	= MyConfig.Get3DF(MYSETP_IMU_MAGSCALE);							// 磁気情報縮尺
//---
	if (MyGlobalTool.MyConfigFile.bIMU_Debug) {									// 開発用モード使用状態(1:使用/0:未使用)
		//------------------------------------------------------------------------------------------
		// ＩＭＵ設定値を出力する
		//------------------------------------------------------------------------------------------
		this->MyDump("IMU Setting Before\n");
	}
//---
	//----------------------------------------------------------------------------------------------
	// ＩＭＵ６軸（加速度、ジャイロ）センサーをキャリブレーション（補正値を設定）する
	//----------------------------------------------------------------------------------------------
	this->MySensor.calibrateAccelGyro();
//---
	//----------------------------------------------------------------------------------------------
	// 地磁気センサーは利用場所によって補正値が確定している(Decl)
	//----------------------------------------------------------------------------------------------
	this->MySensor.setMagneticDeclination(MyGlobalTool.MyConfigFile.fIMU_MagDecl);	// 磁気情報赤緯（地理的なオフセット）
//
	//----------------------------------------------------------------------------------------------
	// ＩＭＵ地磁気センサーをキャリブレーション（補正値を設定）する
	//----------------------------------------------------------------------------------------------
	if (MyGlobalTool.MyConfigFile.bIMU_8Figure) {								// 磁気８の字キャリブレーション(1:使用/0:未使用)
		//------------------------------------------------------------------------------------------
		// キャリブレーションには、センサーを物理的に８の字に回転させる必要がある。
		//------------------------------------------------------------------------------------------
		this->MySensor.calibrateMag();
	} else {
		//------------------------------------------------------------------------------------------
		// 設定情報から値を設定する(Bias)
		//------------------------------------------------------------------------------------------
		this->MySensor.setMagBias(
							MyGlobalTool.MyConfigFile.My3DFMagBias.fRoll,
							MyGlobalTool.MyConfigFile.My3DFMagBias.fPitch,
							MyGlobalTool.MyConfigFile.My3DFMagBias.fYaw);

		//------------------------------------------------------------------------------------------
		// 設定情報から値を設定する(Scale)
		//------------------------------------------------------------------------------------------
		this->MySensor.setMagScale(
							MyGlobalTool.MyConfigFile.My3DFMagScale.fRoll,
							MyGlobalTool.MyConfigFile.My3DFMagScale.fPitch,
							MyGlobalTool.MyConfigFile.My3DFMagScale.fYaw);
	}
//---
	if (MyGlobalTool.MyConfigFile.bIMU_Debug) {									// 開発用モード使用状態(1:使用/0:未使用)
		//------------------------------------------------------------------------------------------
		// ＩＭＵ設定値を出力する
		//------------------------------------------------------------------------------------------
		this->MyDump("IMU Setting After\n");
	}
//---
	pMyDevice->SetDeviceInfo(this->MyDevIdI2C, _MyDEV_STAT::MY_DEVSTAT_OPEN);
}
/**/
VOID			_MyMPU9250::MyDump(CHAR *pszMsg)
{
	MY_LOG(pszMsg);
	MY_LOG("Accel Bias : X,Y,Z = [%8.3f, %8.3f, %8.3f]\n", this->MySensor.getAccBiasX(),  this->MySensor.getAccBiasY(),  this->MySensor.getAccBiasZ());
	MY_LOG("Gyro  Bias : X,Y,Z = [%8.3f, %8.3f, %8.3f]\n", this->MySensor.getGyroBiasX(), this->MySensor.getGyroBiasY(), this->MySensor.getGyroBiasZ());
	MY_LOG("Mag   Bias : X,Y,Z = [%8.3f, %8.3f, %8.3f]\n", this->MySensor.getMagBiasX(),  this->MySensor.getMagBiasY(),  this->MySensor.getMagBiasZ());
	MY_LOG("Mag  Scale : X,Y,Z = [%8.3f, %8.3f, %8.3f]\n", this->MySensor.getMagScaleX(), this->MySensor.getMagScaleY(), this->MySensor.getMagScaleZ());
}
/*-- End Of File --*/
