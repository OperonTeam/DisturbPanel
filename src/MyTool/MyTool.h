//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyTool/MyTool.h
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#ifndef MYTOOL_H
#define MYTOOL_H

//--------------------------------------------------------------------------------------------------
// 従属クラスのインクルード
//--------------------------------------------------------------------------------------------------
#include "Log/MyLog.h"
#include "Config/MyConfigFile.h"
#include "Com/MyComTimer.h"
#include "GPSAnalyze/MyGPSAnalyze.h"

//--------------------------------------------------------------------------------------------------
// その他メソッドを集約した
//--------------------------------------------------------------------------------------------------
class   _MyTool {
	private:
		//------------------------------------------------------------------------------------------
		// 座標変換用・作業領域
		//------------------------------------------------------------------------------------------
		_My2DSPoint		My2DSPointCenter;									// 中心座標点(X,Y)
		SHORT			sAngle;												// 回転角度
		DOUBLE			dSinAngle;											// 演算済みsin(回転角)情報
		DOUBLE			dCosAngle;											// 演算済みcos(回転角)情報
		//---
		DOUBLE			dcXCenter;											// 中心座標点(X)
		DOUBLE			dcYCenter;											// 中心座標点(Y)
		//---
		DOUBLE			dcAngle;											// 回転角
		DOUBLE			dcSinAngle;											// sin(回転角)
		DOUBLE			dcCosAngle;											// cos(回転角)

	public:
		_MyTool();
		~_MyTool();

		//--------------------------------------------------------------------------------------------------
		// 環境・動作定義用ファイル・アクセスクラス
		//--------------------------------------------------------------------------------------------------
		_MyConfigFile	MyConfigFile;

		//--------------------------------------------------------------------------------------------------
		// 環境・動作定義用ファイル・アクセスクラス
		//--------------------------------------------------------------------------------------------------
		_MyGPSAnalyze	MyGPSAnalyze;

		//------------------------------------------------------------------------------------------
		// ＬＣＤ描画ツールメソッド関係（今後、描画系のクラスに分割するか微妙、、、、、）
		//------------------------------------------------------------------------------------------
		_My2DSPoint		CompRotPoint(_My2DSPoint My2DSPointCenter, SHORT sAngle, _My2DSPoint *pMy2DSPointInp, INT iDataNum = 1, _My2DSPoint *pMy2DSPointOut = NULL);
		SHORT			ComputeAngle(FLOAT fIMUAccel);
		VOID			ToStringHHMMTime(CHAR *pszBuff, SHORT sSec);
		FLOAT			Rad2Deg(FLOAT rad);
		INLINE	INT		Deg2Deg(INT iDeg) {
							if (iDeg < 0)		iDeg += 360;
							if (iDeg >= 360)	iDeg -= 360;
							return	iDeg;
		};
//---
		_My2DSPoint		EllipseAngleContact(_My2DSPoint My2DSPoint, INT iRX, INT iRY, INT iAngle);
		SHORT			GPSCourse2Angle(SHORT sGPSCourse);
		SHORT			GPSCourseTo2Angle(SHORT sGPSCourse, SHORT sGPSOffset = 0);
		SHORT			IMUMag2Angle(SHORT sIMUMagYaw, SHORT sIMUOffset);

		//------------------------------------------------------------------------------------------
		// 文字列操作用メソッド
		//	ConvDateTime2StringHHMM	:	日時情報変換
		//	ConvDateTime2StringSS	:	日時情報変換
		//	ConvTimeSpan2StringMMM	:	経過時間変換
		//	ConvTimeSpan2StringSS	:	経過時間変換
		//------------------------------------------------------------------------------------------
		String			ConvDateTime2StringHHMM(DateTime dtDateTime);
		String			ConvDateTime2StringSS(DateTime dtDateTime);
		//---
		String			ConvTimeSpan2StringMMM(TimeSpan tsUptime);
		String			ConvTimeSpan2StringSS(TimeSpan tsUptime);
//---
		//------------------------------------------------------------------------------------------
		// 文字列操作用メソッド
		//------------------------------------------------------------------------------------------
		LONG			QueKeyValue(String *pstrSource, String strKeyword);
//
		//------------------------------------------------------------------------------------------
		// 入力パラメータを比較する。　基準値以上の差がある場合はTRUEを返す。
		// 本来はマクロ化した方が早いんだけど。。。
		//------------------------------------------------------------------------------------------
		BOOL			IfSetDifferent(SHORT  sSource, SHORT  *psDestination, SHORT  sCriteria = (SHORT)0);
		//---
		template<typename T_n>
			BOOL		IfSetDifferent(T_n objSource, T_n *pobjDestination);
		//
		TimeSpan		MAX(TimeSpan tsCompA, TimeSpan tsCompB);

		//------------------------------------------------------------------------------------------
		// 入力情報の妥当性を確認にする
		//------------------------------------------------------------------------------------------
		BOOL			IfNormalValue(DateTime MyDateTime);
		VOID			HexDump(VOID *pvPnt, size_t dumpSize, CONST_CHAR *pszTitle);
		LONG			MemoryTest(String strFunc);
};
/**/
//--------------------------------------------------------------------------------------------------
// グローバル用定義
//--------------------------------------------------------------------------------------------------
extern	_MyTool			MyGlobalTool;

#endif
/*-- End Of File --*/
