//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyJARVis/Analyze/MyJARVisAnalyze.h
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#ifndef MYJARVISANALYZE_H
#define MYJARVISANALYZE_H

//--------------------------------------------------------------------------------------------------
// 外部参照・依存関係インクルード・ヘッダ情報
//--------------------------------------------------------------------------------------------------
#include <CircularBuffer.h>														// メモリバッファ管理
//---
//--------------------------------------------------------------------------------------------------
// データ保存件数
//--------------------------------------------------------------------------------------------------
#define	MY_JARVIS_STANDSTILL_TIMESEC	5										// 停止状態を意図するマクロ（５秒停止）
//---
enum	_My_Driving_Condition {
	STATE_UNKNOWN	= 0,
	STATE_STOP		= 1,
	STATE_RUNNING	= 2
}; 

//--------------------------------------------------------------------------------------------------
// JARVis分析クラス
//--------------------------------------------------------------------------------------------------
class   _MyJARVisAnalyze {
	private:
		//------------------------------------------------------------------------------------------
		// 超簡易統計情報保存領域
		//------------------------------------------------------------------------------------------
		DateTime			dtJARVisUpTime;										// ＧＰＳを初回認識した日時
		DateTime			dtJARVisDateTimeNow;								// ＧＰＳの最新日時
		UCHAR				ucJARVisCSecondNow;									// ＧＰＳの最新センチ秒
		FLOAT				fJARVisSpeedNow;									// 移動速度（最新）
		//---
		CHAR				cJARVisDrivingCondition;							// 現在の走行状態
		SHORT				sJARVisStopCount;									// 停止回数
		TimeSpan			tsJARVisStopElapsedTimeNow;							// 停止経過時間（今回）
		TimeSpan			tsJARVisStopElapsedTimeMax;							// 停止経過時間（最大）
		TimeSpan			tsJARVisStopElapsedTimeSum;							// 停止経過時間（積算）
		//---
		TimeSpan			tsJARVisMoveElapsedTimeNow;							// 移動経過時間（今回）
		TimeSpan			tsJARVisMoveElapsedTimeMax;							// 移動経過時間（最大）
		//---
		TimeSpan			tsWorkStopElapsedTimeSum;							// 停止経過時間（積算作業用）
		DateTime			dtWorkStateChangeMove;								// 移動速度検知日時（直前）
		DateTime			dtWorkStateChangeStop;								// 停止速度検知日時（直前）
//---
		DOUBLE				dJARVisMovingdistance;								// 移動距離（合計）
		FLOAT				fJARVisSpeedMax;									// 最高速度
		DOUBLE				dJARVisSpeedSum;									// 平均速度算出用・長期間合算値
		ULONG				ulJARVisSpeedCount;									// 平均速度算出用・長期間合算データ数
		//---
		FLOAT				fJARVisSpeedShortSum;								// 平均速度算出用・短期間合算値
		INT					iJARVisSpeedShortCount;								// 平均速度算出用・短期間合算データ数
		//
		_MyGPSPoint			MyGPSPointStart;									// ＧＰＳを初回認識した座標	
		_MyGPSPoint			MyGPSPointNow;										// ＧＰＳの最新座標
		_MyGPSPoint			MyGPSPointTo;										// 指定GPS座標（緯度経度）：簡易ナビ用
		//---
		BOOL				AnalyzeFDRRecGPSDateTime(_MyFDRBodyRec *pMyFDRBodyRec, _MyDevice *pMyDevice);
		BOOL				AnalyzeFDRRecGPSPoint(_MyFDRBodyRec *pMyFDRBodyRec, _MyDevice *pMyDevice);
		BOOL				AnalyzeFDRRecGPSSpeed(_MyFDRBodyRec *pMyFDRBodyRec);
		BOOL				AnalyzeFDRRecDrivingConditionOld(_MyFDRBodyRec *pMyFDRBodyRec);
		BOOL				AnalyzeFDRRecDrivingCondition(_MyFDRBodyRec *pMyFDRBodyRec);

	public:
		_MyJARVisAnalyze();
		~_MyJARVisAnalyze();
//---
		//------------------------------------------------------------------------------------------
		// 簡易統計情報・分析更新処理
		//	Initialize					: 統計情報を初期化する
		//	AnalyzeFDRRec				: 統計分析を更新する
		//------------------------------------------------------------------------------------------
		VOID				Initialize(_MyDevice *pMyDevice);
		BOOL				AnalyzeFDRRec(_MyFDRBodyRec *pMyFDRBodyRec, _MyDevice *pMyDevice);
		//---
		//------------------------------------------------------------------------------------------
		// 簡易統計情報・アクセスメソッド
		//	QueJARVisElapsedTime			: 状態が変化した時点から経過した時間
		//	GetElapsedTime					: 起動後の時間を取得する
		//	QueJARVisCourseTo				: 指定座標に対する方位の取得
		//	QueJARVisDistanceTo				: 指定座標に対する距離の取得
		//	QueJARVisMovingdistance			: 移動距離（合計）の取得
		//	QueJARVisSpeedNow				: 現在移動速度の取得
		//	QueJARVisSpeedMax				: 移動最高速度の取得
		//	QueJARVisSpeedAve				: 移動平均速度の取得
		//---
		//	QueJARVisStopCount				: 停車回数 
		//	QueJARVisLinearDistance			: 初回認識座標から現在座標の直線距離を取得
		//	QueJARVisLinearSpeedAve			: 初回認識座標から現在座標までの直線距離に対する移動平均速度
		//	QueJARVisDrivingCondition		: 現在の走行状態
		//	QueJARVisStopCount				: 停止回数
		//	QueJARVisStopElapsedTimeNpw		: 停止経過時間（今回）
		//	QueJARVisStopElapsedTimeMax		: 停止経過時間（最大）
		//	QueJARVisStopElapsedTimeSum		: 停止経過時間（積算）
		//	QueJARVisMoveElapsedTimeNow		: 移動経過時間（今回）
		//	QueJARVisMoveElapsedTimeMax		: 移動経過時間（最大）
		//	QueJARVisMoveElapsedTimeSum		: 移動経過時間（積算）
		//------------------------------------------------------------------------------------------
		TimeSpan			GetElapsedTime(VOID);
		SHORT				QueJARVisCourseTo(_MyDevice *pMyDevice);			// 
		FLOAT				QueJARVisDistanceTo(_MyDevice *pMyDevice);			// 
		FLOAT				QueJARVisLinearDistance(_MyDevice *pMyDevice);		//	
		FLOAT				QueJARVisLinearSpeedAve(_MyDevice *pMyDevice);		//	
		DOUBLE				QueJARVisMovingdistance(VOID);						// 移動距離（合計）
		FLOAT				QueJARVisSpeedNow(VOID);							// 現在速度
		FLOAT				QueJARVisSpeedMax(VOID);							// 最高速度
		FLOAT				QueJARVisSpeedAve(VOID);							// 平均速度
		//---
		CHAR				QueJARVisDrivingCondition(VOID);					// 現在の走行状態
		SHORT				QueJARVisStopCount(VOID);							// 停車回数 
		TimeSpan			QueJARVisStopElapsedTimeNow(VOID);					// 
		TimeSpan			QueJARVisStopElapsedTimeMax(VOID);
		TimeSpan			QueJARVisStopElapsedTimeSum(VOID);
		//---
		TimeSpan			QueJARVisMoveElapsedTimeNow(VOID);
		TimeSpan			QueJARVisMoveElapsedTimeMax(VOID);
		TimeSpan			QueJARVisMoveElapsedTimeSum(VOID);
		//---
		VOID				MyDump(VOID);
};
#endif
/*-- End Of File --*/
