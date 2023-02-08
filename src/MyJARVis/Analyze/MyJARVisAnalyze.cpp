//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyJARVis/Analyze/MyJARVisAnalyze.cpp
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#include "MyBase.h"
#include "MyDevice.h"
#include "MyJARVis.h"

//--------------------------------------------------------------------------------------------------
// クラス初期メソッド
//--------------------------------------------------------------------------------------------------
_MyJARVisAnalyze::_MyJARVisAnalyze()
{
	//----------------------------------------------------------------------------------------------
	// 初期化
	//----------------------------------------------------------------------------------------------
	this->dtJARVisUpTime			= DateTime(2000, 1, 1, 0, 0, 0);	// ＧＰＳ初回認識日時
	this->MyGPSPointStart.dLat		= (DOUBLE)0.0;						// ＧＰＳ初回認識座標（ＬＡＴ）
	this->MyGPSPointStart.dLng		= (DOUBLE)0.0;						// ＧＰＳ初回認識座標（ＬＮＧ）
	this->dtJARVisDateTimeNow		= DateTime(2000, 1, 1, 0, 0, 0);	// ＧＰＳ最新日時
	this->ucJARVisCSecondNow		= (UCHAR)0;							// ＧＰＳ最新日時（1/10秒）
//---
	this->dJARVisMovingdistance		= (DOUBLE)0.0;						// 移動距離（合計）
	this->dJARVisSpeedSum			= (DOUBLE)0.0;						// 平均速度算出用・速度合算値
	this->ulJARVisSpeedCount		= (ULONG)0;							// 平均速度算出用・合算データ数
	this->fJARVisSpeedMax			= (FLOAT)0.0;						// 最高速度
}
/**/
//--------------------------------------------------------------------------------------------------
// クラス終了メソッド
//--------------------------------------------------------------------------------------------------
_MyJARVisAnalyze::~_MyJARVisAnalyze()
{
}
/**/
//--------------------------------------------------------------------------------------------------
// 情報分析処理クラス
//	Initialize					: 統計情報の初期化
//	AnalyzeFDRRec				: 統計情報の更新（メイン処理）
//--------------------------------------------------------------------------------------------------
VOID		_MyJARVisAnalyze::Initialize(_MyDevice *pMyDevice)
{
	//----------------------------------------------------------------------------------------------
	// デバイス管理クラスにタスク状態を通知する(MY_DEVSTAT_SETUP)
	// ー＞センサーから得られる値が、妥当な値になるまでは、SETUP状態とします。w
	//----------------------------------------------------------------------------------------------
	pMyDevice->SetDeviceInfo(_MyDEV_ID::MY_DEVID_HAL, _MyDEV_STAT::MY_DEVSTAT_SETUP);
//---
	//----------------------------------------------------------------------------------------------
	// 内部利用変数を初期化します。
	//----------------------------------------------------------------------------------------------
	this->dtJARVisUpTime			= DateTime(2000, 1, 1, 0, 0, 0);	// ＧＰＳ初回認識日時
	this->MyGPSPointStart.dLat		= (DOUBLE)0.0;						// ＧＰＳ初回認識座標（ＬＡＴ）
	this->MyGPSPointStart.dLng		= (DOUBLE)0.0;						// ＧＰＳ初回認識座標（ＬＮＧ）
	this->dtJARVisDateTimeNow		= DateTime(2000, 1, 1, 0, 0, 0);	// ＧＰＳ最新日時
	this->ucJARVisCSecondNow		= (UCHAR)0;							// ＧＰＳ（センチ秒）
//---
//---
	//----------------------------------------------------------------------------------------------
	// _MyJARVisAnalyze::AnalyzeFDRRecDrivingCondition
	//----------------------------------------------------------------------------------------------
	this->cJARVisDrivingCondition		= (CHAR)_My_Driving_Condition::STATE_UNKNOWN;	// 現在の走行状態
	this->sJARVisStopCount				= (SHORT)0;								// 停止回数
	this->tsJARVisStopElapsedTimeNow	= TimeSpan((ULONG)0);					// 停止経過時間（今回）
	this->tsJARVisStopElapsedTimeMax	= TimeSpan((ULONG)0);					// 停止経過時間（最大）
	this->tsJARVisStopElapsedTimeSum	= TimeSpan((ULONG)0);					// 停止経過時間（積算）
	//---
	this->tsJARVisMoveElapsedTimeNow	= TimeSpan((ULONG)0);					// 移動経過時間（今回）
	this->tsJARVisMoveElapsedTimeMax	= TimeSpan((ULONG)0);					// 移動経過時間（最大）
	//---
	this->tsWorkStopElapsedTimeSum		= TimeSpan((ULONG)0);					// 停止経過時間（積算作業用）
	this->dtWorkStateChangeMove			= DateTime(2000,1,1,0,0,0);				// 移動速度検知日時（直前）
	this->dtWorkStateChangeStop			= DateTime(2000,1,1,0,0,0);				// 停止速度検知日時（直前）
//---
	this->dJARVisMovingdistance			= (DOUBLE)0.0;							// 移動距離（合計）
	this->dJARVisSpeedSum				= (DOUBLE)0.0;							// 平均速度算出用・速度合算値
	this->ulJARVisSpeedCount			= (ULONG)0;								// 平均速度算出用・合算データ数
	this->fJARVisSpeedMax				= (FLOAT)0.0;							// 最高速度
	this->fJARVisSpeedNow				= (FLOAT)-1.0;							// 移動速度（最新）
//---
	//----------------------------------------------------------------------------------------------
	// システム外部定義情報ファイルに記載される情報を取り出す
	//----------------------------------------------------------------------------------------------
	this->MyGPSPointTo = MyGlobalTool.MyConfigFile.MyGPSPointHome;				// 指定GPS座標（緯度経度）
}
/**/
//--------------------------------------------------------------------------------------------------
// ＧＰＳ日時情報を分析する
//--------------------------------------------------------------------------------------------------
BOOL		_MyJARVisAnalyze::AnalyzeFDRRecGPSDateTime(_MyFDRBodyRec *pMyFDRBodyRec, _MyDevice *pMyDevice)
{
	//----------------------------------------------------------------------------------------------
	// ＧＰＳ日付は妥当か？（衛星データが受信出来ない場合がある）
	//----------------------------------------------------------------------------------------------
	if (pMyFDRBodyRec->GetGPSDateTime().year() > 2000) {
		//------------------------------------------------------------------------------------------
		// 最新のＧＰＳ日時を保存する
		//------------------------------------------------------------------------------------------
		this->dtJARVisDateTimeNow	= pMyFDRBodyRec->GetGPSDateTime();			// ＧＰＳ最新日時
		this->ucJARVisCSecondNow	= pMyFDRBodyRec->GetGPSCentiSecond();		// ＧＰＳ(センチ秒)
		//
		if (this->dtJARVisUpTime.year() <= 2000) {
			//--------------------------------------------------------------------------------------
			// ＧＰＳ初回認識日時が初期値の場合は、ＧＰＳ初回認識日時を保存する
			//--------------------------------------------------------------------------------------
			this->dtJARVisUpTime		= this->dtJARVisDateTimeNow;			// ＧＰＳ初回認識日時
			//
			//--------------------------------------------------------------------------------------
			// 内蔵RTC内部日時を設定する。
			//--------------------------------------------------------------------------------------
			pMyDevice->RTCAdjust(this->dtJARVisUpTime);							// ＲＴＣ時刻合わせ
			pMyDevice->SetDeviceInfo(_MyDEV_ID::MY_DEVID_RTC, _MyDEV_STAT::MY_DEVSTAT_OPEN);
		}
	} else {
		//------------------------------------------------------------------------------------------
		// ＧＰＳセンサーから日時情報が取得出来ない
		//------------------------------------------------------------------------------------------
		return	MY_FALSE;
	}
	return	MY_TRUE;
}
/**/
//--------------------------------------------------------------------------------------------------
// ＧＰＳ座標情報を分析する
//--------------------------------------------------------------------------------------------------
BOOL		_MyJARVisAnalyze::AnalyzeFDRRecGPSPoint(_MyFDRBodyRec *pMyFDRBodyRec, _MyDevice *pMyDevice)
{
	//----------------------------------------------------------------------------------------------
	// ＧＰＳ座標が得られない場合
	//----------------------------------------------------------------------------------------------
	if (pMyFDRBodyRec->GetGPSPoint().dLat == (DOUBLE)0.0) {
		if (this->MyGPSPointStart.dLat == (DOUBLE)0.0) {
			//--------------------------------------------------------------------------------------
			// ＧＰＳセンサーから「一度も有効な」座標情報が取得されていない
			//--------------------------------------------------------------------------------------
			return	MY_FALSE;
		}
	} else {
		//------------------------------------------------------------------------------------------
		// 有効な最新のＧＰＳ座標を保存する
		//------------------------------------------------------------------------------------------
		this->MyGPSPointNow	= pMyFDRBodyRec->GetGPSPoint();						// ＧＰＳ最新座標
		//
		if (this->MyGPSPointStart.dLat == (DOUBLE)0.0) {
			//--------------------------------------------------------------------------------------
			// ＧＰＳ初回認識座標が初期値の場合は、ＧＰＳ初回認識座標を保存する
			//--------------------------------------------------------------------------------------
			this->MyGPSPointStart = this->MyGPSPointNow;						// ＧＰＳ初回認識座標

			//--------------------------------------------------------------------------------------
			// デバイス管理クラスにタスク状態を通知する(MY_DEVSTAT_OPEN)
			//--------------------------------------------------------------------------------------
			pMyDevice->SetDeviceInfo(_MyDEV_ID::MY_DEVID_HAL, _MyDEV_STAT::MY_DEVSTAT_OPEN);
		}
	}
	return	MY_TRUE;
}
/**/
//--------------------------------------------------------------------------------------------------
// ＧＰＳ移動速度情報を分析する
//--------------------------------------------------------------------------------------------------
BOOL		_MyJARVisAnalyze::AnalyzeFDRRecGPSSpeed(_MyFDRBodyRec *pMyFDRBodyRec)
{
	//----------------------------------------------------------------------------------------------
	// 【最新の移動速度】を退避して、【最高移動速度・平均移動速度】情報を更新する
	//----------------------------------------------------------------------------------------------
	FLOAT	fSpeedNow = pMyFDRBodyRec->GetGPSSpeed();
	if (fSpeedNow > (FLOAT)0.0) {
		//------------------------------------------------------------------------------------------
		// 最新の移動速度値が０以上の場合（移動している場合）
		//------------------------------------------------------------------------------------------
		this->fJARVisSpeedMax = max(fSpeedNow, this->fJARVisSpeedMax);			// 最高速度
		this->dJARVisSpeedSum += fSpeedNow;										// 平均速度算出用・長期間合算値
		this->ulJARVisSpeedCount += 1;											// 平均速度算出用・長期間合算データ数
		//
		this->fJARVisSpeedShortSum += fSpeedNow;								// 平均速度算出用・短期間合算値
		this->iJARVisSpeedShortCount += 1;										// 平均速度算出用・短期間合算データ数
	}
//---
	return	MY_TRUE;
}
/**/
//--------------------------------------------------------------------------------------------------
// 走行状態を推定・分析する
//	・速度０の状態を停止中とするのでは無く、例えば渋滞中に瞬間的に速度０の状態が発生する様な、
//	　最徐行の場合は「走行中」としたい。
//	　ー＞走行状態の「停止中」とは、継続して移動速度０が一定時間以上経過した状態。
//	・「停止中」と認識されない短い時間で、移動～停止を繰り返す可能性がある為、初回の[STOP->MOVE]時に
//	　限ったロジックが存在する。
//	・移動経過時間（積算）を求める際は、システム経過時間から、停止経過時間（積算）を減算すること。
//--------------------------------------------------------------------------------------------------
BOOL		_MyJARVisAnalyze::AnalyzeFDRRecDrivingCondition(_MyFDRBodyRec *pMyFDRBodyRec)
{
	DateTime	dtNow = pMyFDRBodyRec->GetGPSDateTime();
	FLOAT		fSpeedNow = pMyFDRBodyRec->GetGPSSpeed();
	if (this->fJARVisSpeedNow <= (FLOAT)0.0) {										// 前回の移動速度
		if (fSpeedNow <= (FLOAT)0.0) {												// 今回の移動速度
			//--------------------------------------------------------------------------------------
			// STOP -> STOP
			//--------------------------------------------------------------------------------------
			TimeSpan	tsWork = dtNow - this->dtWorkStateChangeStop;				// ???これで大丈夫???設定されていることが前提
			if (tsWork.totalseconds() > MY_JARVIS_STANDSTILL_TIMESEC) {
				if (MyGlobalTool.IfSetDifferent((CHAR)_My_Driving_Condition::STATE_STOP, &this->cJARVisDrivingCondition)) {
					this->sJARVisStopCount++;
					this->tsWorkStopElapsedTimeSum = this->tsJARVisStopElapsedTimeSum;
				}
				this->tsJARVisStopElapsedTimeNow = tsWork;
				this->tsJARVisStopElapsedTimeMax = MyGlobalTool.MAX(this->tsJARVisStopElapsedTimeMax, tsWork);
				this->tsJARVisStopElapsedTimeSum = tsWorkStopElapsedTimeSum + tsWork;
			}
		} else {
			//--------------------------------------------------------------------------------------
			// STOP -> MOVE
			//--------------------------------------------------------------------------------------
			if (MyGlobalTool.IfSetDifferent((CHAR)_My_Driving_Condition::STATE_RUNNING, &this->cJARVisDrivingCondition)) {
				this->dtWorkStateChangeMove = dtNow;
			}
		}
	} else {
		if (fSpeedNow <= (FLOAT)0.0) {
			//--------------------------------------------------------------------------------------
			// MOVE -> STOP
			//--------------------------------------------------------------------------------------
			this->dtWorkStateChangeStop = dtNow;
		} else {
			//--------------------------------------------------------------------------------------
			// MOVE -> MOVE
			//--------------------------------------------------------------------------------------
			TimeSpan	tsWork = dtNow - this->dtWorkStateChangeMove;
			this->tsJARVisMoveElapsedTimeNow = tsWork;
			this->tsJARVisMoveElapsedTimeMax = MyGlobalTool.MAX(this->tsJARVisMoveElapsedTimeMax, tsWork);
		}
	}
	this->fJARVisSpeedNow = fSpeedNow;
//---

MY_LOG("--- Driving Condition ---\n");
MY_LOG("移動速度        = %f\n",    fSpeedNow);
MY_LOG("走行状態        = %d\n",    this->QueJARVisDrivingCondition());
MY_LOG("停止回数        = %03d\n",  this->QueJARVisStopCount());
MY_LOG("停止時間（合算）= %03d:%02d\n", this->QueJARVisStopElapsedTimeSum().minutes(), this->QueJARVisStopElapsedTimeSum().seconds());
MY_LOG("停止時間（今回）= %03d:%02d\n", this->QueJARVisStopElapsedTimeNow().minutes(), this->QueJARVisStopElapsedTimeNow().seconds());
MY_LOG("停止時間（最大）= %03d:%02d\n", this->QueJARVisStopElapsedTimeMax().minutes(), this->QueJARVisStopElapsedTimeMax().seconds());
MY_LOG("走行時間（今回）= %03d:%02d\n", this->QueJARVisMoveElapsedTimeNow().minutes(), this->QueJARVisMoveElapsedTimeNow().seconds());
MY_LOG("走行時間（最大）= %03d:%02d\n", this->QueJARVisMoveElapsedTimeMax().minutes(), this->QueJARVisMoveElapsedTimeMax().seconds());

	return	MY_TRUE;
}
/**/
//--------------------------------------------------------------------------------------------------
// FDRRec情報から、JARVis内で保有する各種統計情報を更新する。
//--------------------------------------------------------------------------------------------------
BOOL		_MyJARVisAnalyze::AnalyzeFDRRec(_MyFDRBodyRec *pMyFDRBodyRecNow, _MyDevice *pMyDevice)
{
	//----------------------------------------------------------------------------------------------
	// ＧＰＳ日時情報を分析する（得られた日時が信頼出来ない場合は、以降の処理を行わない）
	//----------------------------------------------------------------------------------------------
	if (! this->AnalyzeFDRRecGPSDateTime(pMyFDRBodyRecNow, pMyDevice))	return	MY_FALSE;
//---
	//----------------------------------------------------------------------------------------------
	// ＧＰＳ座標情報を分析する（得られた座標値が信頼出来ない場合は、以降の処理を行わない）
	//----------------------------------------------------------------------------------------------
	if (! this->AnalyzeFDRRecGPSPoint(pMyFDRBodyRecNow, pMyDevice))		return	MY_FALSE;
//---
	//----------------------------------------------------------------------------------------------
	// ＧＰＳ移動速度情報を分析する
	//----------------------------------------------------------------------------------------------
	this->AnalyzeFDRRecGPSSpeed(pMyFDRBodyRecNow);
//---
	//----------------------------------------------------------------------------------------------
	// 走行状態を推定・分析する
	//----------------------------------------------------------------------------------------------
	this->AnalyzeFDRRecDrivingCondition(pMyFDRBodyRecNow);
//---
	return	MY_TRUE;
}
	//------
	//
	//------
//	MY_LOG("JARVis : [%lf,%lf] - [%lf,%lf] - Dist[%lf]\n", this->MyGPSPointStart.dLat, this->MyGPSPointStart.dLng, MyGPSPoint.dLat, MyGPSPoint.dLng, pMyDevice->MyGPS.GetGPSDistance(this->MyGPSPointStart));
/**/
//--------------------------------------------------------------------------------------------------
// 情報分析処理
//	GetElapsedTime					: 起動経過時間の取得
//---
//	QueJARVisCourseTo				: 指定座標に対する方位の取得
//	QueJARVisDistanceTo				: 指定座標に対する距離の取得
//	QueJARVisLinearDistance			: 初回認識座標から現在座標の直線距離を取得
//	QueJARVisLinearSpeedAve			: 初回認識座標から現在座標までの直線距離に対する移動平均速度
//---
//	QueJARVisMovingdistance			: 移動距離の取得（合計）
//	QueJARVisSpeedNow				: 移動速度の取得（最新）
//	QueJARVisSpeedMax				: 移動最高速度の取得
//	QueJARVisSpeedAve				: 移動平均速度の取得
//---
//	QueJARVisDrivingCondition		: 現在の走行状態
//	QueJARVisStopCount				: 停止回数
//	QueJARVisStopElapsedTimeNpw		: 停止経過時間（今回）
//	QueJARVisStopElapsedTimeMax		: 停止経過時間（最大）
//	QueJARVisStopElapsedTimeSum		: 停止経過時間（積算）
//	QueJARVisMoveElapsedTimeNow		: 移動経過時間（今回）
//	QueJARVisMoveElapsedTimeMax		: 移動経過時間（最大）
//	QueJARVisMoveElapsedTimeSum		: 移動経過時間（積算）
//--------------------------------------------------------------------------------------------------
TimeSpan	_MyJARVisAnalyze::GetElapsedTime(VOID)							{	return	this->dtJARVisDateTimeNow - this->dtJARVisUpTime;		}
/**/
SHORT		_MyJARVisAnalyze::QueJARVisCourseTo(_MyDevice *pMyDevice)		{	return	pMyDevice->GPSCourseTo(this->MyGPSPointTo);				}
FLOAT		_MyJARVisAnalyze::QueJARVisDistanceTo(_MyDevice *pMyDevice)		{	return	pMyDevice->GPSDistance(this->MyGPSPointTo);				}
FLOAT		_MyJARVisAnalyze::QueJARVisLinearDistance(_MyDevice *pMyDevice)	{	return	pMyDevice->GPSDistance(this->MyGPSPointStart);			}
//---
FLOAT		_MyJARVisAnalyze::QueJARVisLinearSpeedAve(_MyDevice *pMyDevice)	{
	return	(60 * 60) / this->GetElapsedTime().totalseconds() * this->QueJARVisLinearDistance(pMyDevice) / 1000;
}
//---
DOUBLE		_MyJARVisAnalyze::QueJARVisMovingdistance(VOID)					{	return	this->dJARVisMovingdistance;							}
FLOAT		_MyJARVisAnalyze::QueJARVisSpeedNow(VOID)						{	return	this->fJARVisSpeedNow;									}
FLOAT		_MyJARVisAnalyze::QueJARVisSpeedMax(VOID)						{	return	this->fJARVisSpeedMax;									}
FLOAT		_MyJARVisAnalyze::QueJARVisSpeedAve(VOID)						{	return	(this->dJARVisSpeedSum / this->ulJARVisSpeedCount);		}
//---
CHAR		_MyJARVisAnalyze::QueJARVisDrivingCondition(VOID)				{	return	this->cJARVisDrivingCondition;							}
SHORT		_MyJARVisAnalyze::QueJARVisStopCount(VOID)						{	return	this->sJARVisStopCount;									}
TimeSpan	_MyJARVisAnalyze::QueJARVisStopElapsedTimeNow(VOID)				{	return	this->tsJARVisStopElapsedTimeNow;						}
TimeSpan	_MyJARVisAnalyze::QueJARVisStopElapsedTimeMax(VOID)				{	return	this->tsJARVisStopElapsedTimeMax;						}
TimeSpan	_MyJARVisAnalyze::QueJARVisStopElapsedTimeSum(VOID)				{	return	this->tsJARVisStopElapsedTimeSum;						}
TimeSpan	_MyJARVisAnalyze::QueJARVisMoveElapsedTimeNow(VOID)				{	return	this->tsJARVisMoveElapsedTimeNow;						}
TimeSpan	_MyJARVisAnalyze::QueJARVisMoveElapsedTimeMax(VOID)				{	return	this->tsJARVisMoveElapsedTimeMax;						}
TimeSpan	_MyJARVisAnalyze::QueJARVisMoveElapsedTimeSum(VOID)				{	return	this->GetElapsedTime() - this->tsJARVisStopElapsedTimeSum;	}
/**/
//--------------------------------------------------------------------------------------------------
// 開発用のデータダンプ
//--------------------------------------------------------------------------------------------------
#ifdef	MY_DEBUG_PRINT
VOID		 	_MyJARVisAnalyze::MyDump(VOID)
{
	MY_TRACE("--- Dump Start ---  ");
	MY_TRACE("--- Dump End ---");
}
#endif
/*-- End Of File --*/
