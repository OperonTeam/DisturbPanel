//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyTool/MyTool.cpp
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#include "MyBase.h"

//--------------------------------------------------------------------------------------------------
// クラスインスタンスの初期処理
//--------------------------------------------------------------------------------------------------
_MyTool::_MyTool()
{
}
/**/
//--------------------------------------------------------------------------------------------------
// クラスインスタンスの終了処理
//--------------------------------------------------------------------------------------------------
_MyTool::~_MyTool()
{
}
/**/
//--------------------------------------------------------------------------------------------------
// 変換座標用パラメータ設定メソッド処理（角度の指定は、時計回りに角度(0->360)を指定）
//--------------------------------------------------------------------------------------------------
_My2DSPoint	_MyTool::CompRotPoint(
_My2DSPoint	My2DSPointCenter,
SHORT		sAngle,
_My2DSPoint	*pMy2DSPointInp,
INT			iDataNum,
_My2DSPoint	*pMy2DSPointOut)
{
	//----------------------------------------------------------------------------------------------
	// 変換座標用パラメータ設定メソッド処理（角度の指定は、時計回りに角度(0->360)を指定）
	//----------------------------------------------------------------------------------------------
	if ((this->My2DSPointCenter.sX	!= My2DSPointCenter.sX)||
		(this->My2DSPointCenter.sY	!= My2DSPointCenter.sY)||
		(this->sAngle				!= sAngle))
	{
		//------------------------------------------------------------------------------------------
		// 指定座標が異なっていたら、作業情報を再計算する
		//------------------------------------------------------------------------------------------
		this->My2DSPointCenter	= My2DSPointCenter;
		this->sAngle			= sAngle;
		//
		DOUBLE	dAngle;
		dAngle = (DOUBLE)((DOUBLE)-sAngle/180.0) * PI;						// 3.14159265358979323846;
		this->dSinAngle = sin(dAngle);										// 予め演算しておく
		this->dCosAngle = cos(dAngle);										// 予め演算しておく
	}
	//
	//----------------------------------------------------------------------------------------------
	// 入力データ個数の数だけループ
	//----------------------------------------------------------------------------------------------
	_My2DSPoint	My2DSPoint;
	My2DSPointCenter.sY = My2DSPointCenter.sY * -1.0;						// 数学座標と同じ様にするためにy座標値を反転
	//
	for (INT iIndex = 0; iIndex < iDataNum; iIndex++) {
		//------------------------------------------------------------------------------------------
		// 座標(My2DSPoint) を，(My2DSPointCenter)を中心に、thetaラジアン回転した座標をそれぞれ，*xp, *yp に返す
		//------------------------------------------------------------------------------------------
		DOUBLE	dX = (DOUBLE)pMy2DSPointInp[iIndex].sX;						// 
		DOUBLE	dY = (DOUBLE)pMy2DSPointInp[iIndex].sY * -1.0;				// 数学座標と同じ様にするためにy座標値を反転
		//
		//------------------------------------------------------------------------------------------
		// 三角関数の値を算出
		//------------------------------------------------------------------------------------------
		My2DSPoint.sX =	 (dX - My2DSPointCenter.sX) * this->dCosAngle - (dY - My2DSPointCenter.sY) * this->dSinAngle + My2DSPointCenter.sX;
		My2DSPoint.sY = ((dX - My2DSPointCenter.sX) * this->dSinAngle + (dY - My2DSPointCenter.sY) * this->dCosAngle + My2DSPointCenter.sY) * -1.0;
		//
		//------------------------------------------------------------------------------------------
		// 算出データを退避する
		//------------------------------------------------------------------------------------------
		if (pMy2DSPointOut) {
			pMy2DSPointOut[iIndex] = My2DSPoint;
		}
	}
	return	My2DSPoint;
}
/*
*/
/**/
//--------------------------------------------------------------------------------------------------
// DateTime => HH:MM
//--------------------------------------------------------------------------------------------------
String		_MyTool::ConvDateTime2StringHHMM(DateTime dtDateTime)
{
	String	strRet = "";
	CHAR	szBuff[12];
	sprintf(szBuff, "%02d:%02d", dtDateTime.hour(), dtDateTime.minute());
	strRet.concat(szBuff);
	return	strRet;
}
/**/
//--------------------------------------------------------------------------------------------------
// DateTime => :SS
//--------------------------------------------------------------------------------------------------
String		_MyTool::ConvDateTime2StringSS(DateTime dtDateTime)
{
	String	strRet = "";
	CHAR	szBuff[12];
	sprintf(szBuff, ":%02d", dtDateTime.second());
	strRet.concat(szBuff);
	return	strRet;
}
/**/
String		_MyTool::ConvTimeSpan2StringMMM(TimeSpan tsUptime)
{
	String	strRet = "";
	CHAR	szBuff[12];
	sprintf(szBuff, "%3d", tsUptime.minutes() + (tsUptime.hours() * 60));
	strRet.concat(szBuff);
	return	strRet;
}
/**/
String		_MyTool::ConvTimeSpan2StringSS(TimeSpan tsUptime)
{
	String	strRet = "";
	CHAR	szBuff[12];
	CHAR	cSecond = tsUptime.seconds();
	if (cSecond % 2)	sprintf(szBuff, ":%02d", cSecond);
	else				sprintf(szBuff, " %02d", cSecond);
	strRet.concat(szBuff);
	return	strRet;
}
/**/
//--------------------------------------------------------------------------------------------------
// RadをDeg（単位を通常の角度情報）に変換する
//--------------------------------------------------------------------------------------------------
FLOAT		_MyTool::Rad2Deg(FLOAT fRad)
{
	FLOAT	fDeg = fRad * (180.0 / PI);
	return	fDeg;
}
//
//--------------------------------------------------------------------------------------------------
// 楕円の中心角度から指定された角度の接点座標を取得する
// 指定角度：	0度 -> 12時方向の座標
// 			   90度 ->  3時方向の座標
// 			  180度 ->  6時方向の座標
// 			  270度 ->  9時方向の座標
//--------------------------------------------------------------------------------------------------
_My2DSPoint	_MyTool::EllipseAngleContact(_My2DSPoint My2DSPoint, INT iRX, INT iRY, INT iAngle)
{
	//----------------------------------------------------------------------------------------------
	// 指定角度が  0度の場合、中心座標に対して12時方向の座標が通知される様に、指定角度を変換する。
	//----------------------------------------------------------------------------------------------
	iAngle -= 90;
	if (iAngle < 0)	iAngle += 360;

	//----------------------------------------------------------------------------------------------
	// 楕円の中心座標から指定角度への直線と、楕円との交点座標を算出する
	//----------------------------------------------------------------------------------------------
	_My2DSPoint	My2DSPointReturn;
	My2DSPointReturn.sX = (iRX * cos(iAngle * PI/180)) + My2DSPoint.sX;
	My2DSPointReturn.sY = (iRY * sin(iAngle * PI/180)) + My2DSPoint.sY;
	return	My2DSPointReturn;
}
//
//--------------------------------------------------------------------------------------------------
// GPS進行方向（方位）から、地磁気（北の方位）を示す角度を求める
//	入力値：GPSから得られる進行方向情報		北:0, 東:90, 南:180, 西:270
//  出力値：地磁気で北を示す角度情報		 　0,    270,   180,     90
//--------------------------------------------------------------------------------------------------
SHORT		_MyTool::GPSCourse2Angle(SHORT sGPSCourse)
{
	return	abs(360 - sGPSCourse);
}
//
//--------------------------------------------------------------------------------------------------
// GPS進行方向（方位）から、地磁気（北の方位）を示す角度を求める
//	入力値：GPSから得られる進行方向情報		北:0, 東:90, 南:180, 西:270
//			進行方向オフセット情報			北:0, 東:90, 南:180, 西:270
//  出力値：地磁気で北を示す角度情報		 　0,    270,   180,     90
//--------------------------------------------------------------------------------------------------
SHORT		_MyTool::GPSCourseTo2Angle(SHORT sGPSCourseTo, SHORT sGPSCourse)
{
	SHORT	sAngle = sGPSCourseTo - sGPSCourse;
	if (sAngle > 0)			return	sAngle;
	else					return	360 + sAngle;
}
//
//--------------------------------------------------------------------------------------------------
// IMU地磁気（方位）から、地磁気（北の方位）を示す角度を求める
//	入力値：IMUから得られる進行方向は、0 -> -180 +180 -> 0までの値
//  出力値：地磁気で北を示す角度情報		 　0,    270,   180,     90
//--------------------------------------------------------------------------------------------------
SHORT		_MyTool::IMUMag2Angle(SHORT sIMUMagYaw, SHORT sIMUOffset)
{
	//----------------------------------------------------------------------------------------------
	// IMU Yaw(今回は方位)値は、0 -> -180 180 -> 0 の値を示すので、0 -> 360の連続した値に変換する
	//----------------------------------------------------------------------------------------------
	if (sIMUMagYaw > 0)		sAngle = 360 - sIMUMagYaw;
	else					sAngle = sIMUMagYaw * -1;
//
	//----------------------------------------------------------------------------------------------
	// 進行方向に対して、IMU センサーの取付オフセットを演算する
	//----------------------------------------------------------------------------------------------
	sAngle += sIMUOffset;
	if (sAngle > 360)	sAngle -= 360;
	if (sAngle < 0)		sAngle += 360;

	MY_LOG("North %5d / %5d / %5d\n", sIMUMagYaw, sIMUOffset, sAngle);
//N -134
//E -116
//S -170
//W -165
	return	sAngle;
}
/**/
//--------------------------------------------------------------------------------------------------
// (DateTime)パラメータは妥当な値か？
//--------------------------------------------------------------------------------------------------
BOOL			_MyTool::IfNormalValue(
DateTime		MyDateTime)
{
	if ((MyDateTime.year() > 2020)&&(MyDateTime.year() < 2100))		return	MY_TRUE;
	else															return	MY_FALSE;
}
/**/
//--------------------------------------------------------------------------------------------------
// 指示されたキーワード・ラベルに記載されている情報を取得する
//--------------------------------------------------------------------------------------------------
LONG			_MyTool::QueKeyValue(
String			*pstrSource,
String			strKeyword)
{
	//----------------------------------------------------------------------------------------------
	// 指示されたキーワード・ラベル文字列を検索する：キーワードが探知出来ない場合はエラー
	//----------------------------------------------------------------------------------------------
	INT	iFindIndex = pstrSource->indexOf(strKeyword);
	if (iFindIndex < 0)						return	0;

	//----------------------------------------------------------------------------------------------
	// 目的の情報位置を算出して、一般的なデリミッタまでの情報を整数数値変換して呼び出し元に通知する
	// .toIntは便利かも、、、、。
	//----------------------------------------------------------------------------------------------
	iFindIndex += strKeyword.length() - 1;
	return	pstrSource->substring(iFindIndex).toInt();
}
/*
*/
//--------------------------------------------------------------------------------------------------
// パラメータ値の差が、指定された許容内であるか判断する
//	<SHORT>
//--------------------------------------------------------------------------------------------------
BOOL			_MyTool::IfSetDifferent(
SHORT			sSource,
SHORT			*psDestination,
SHORT			sCriteria)
{
	if (abs(sSource - *psDestination) > sCriteria) {
		*psDestination = sSource;
		return	MY_TRUE;
	}
	return	MY_FALSE;
}
/**/
template<typename T_n>
BOOL			_MyTool::IfSetDifferent(
T_n				objSource,
T_n				*pobjDestination)
{
	if (objSource != *pobjDestination) {
		*pobjDestination = objSource;
		return	MY_TRUE;
	}
	return	MY_FALSE;
}
/**/
template<>
BOOL			_MyTool::IfSetDifferent(
FLOAT			fSource,
FLOAT			*pfDestination)
{
	if (fSource != *pfDestination) {
		*pfDestination = fSource;
		return	MY_TRUE;
	}
	return	MY_FALSE;
}
/**/
template<>
BOOL			_MyTool::IfSetDifferent(
DateTime		dtSource,
DateTime		*pdtDestination)
{
	if (dtSource != *pdtDestination) {
		*pdtDestination = dtSource;
		return	MY_TRUE;
	}
	return	MY_FALSE;
}
/**/
template<>
BOOL			_MyTool::IfSetDifferent(
TimeSpan		tsSource,
TimeSpan		*ptsDestination)
{
	if (tsSource.totalseconds() != ptsDestination->totalseconds()) {
		*ptsDestination = tsSource;
		return	MY_TRUE;
	}
	return	MY_FALSE;
}
/**/
template<>
BOOL			_MyTool::IfSetDifferent(
CHAR			cSource,
CHAR			*pcDestination)
{
	if (cSource != *pcDestination) {
		*pcDestination = cSource;
		return	MY_TRUE;
	}
	return	MY_FALSE;
}
/**/
template<>
BOOL			_MyTool::IfSetDifferent(
BOOL			bSource,
BOOL			*pbDestination)
{
	if (bSource != *pbDestination) {
		*pbDestination = bSource;
		return	MY_TRUE;
	}
	return	MY_FALSE;
}
/**/
template<>
BOOL			_MyTool::IfSetDifferent(
ULONG			ulSource,
ULONG			*pulDestination)
{
	if (ulSource != *pulDestination) {
		*pulDestination = ulSource;
		return	MY_TRUE;
	}
	return	MY_FALSE;
}
/**/
TimeSpan		_MyTool::MAX(
TimeSpan		tsCompA,
TimeSpan		tsCompB)
{
	if (tsCompA.totalseconds() > tsCompB.totalseconds())	return	tsCompA;
	else													return	tsCompB;
}
/**/
//--------------------------------------------------------------------------------------------------
// このタイミングで確保可能な動的メモリの最大値を取得する
// 同時実行中の他タスクに大きく影響するため、本関数実行後の正常動作は期待しては駄目
//--------------------------------------------------------------------------------------------------
LONG			_MyTool::MemoryTest(String strFunc)
{
	LONG	lMemSum = (LONG)0;
	CHAR	*pMyTBL[100];
	for (LONG lMainIndex = 0; lMainIndex < 50; lMainIndex++) {
		pMyTBL[lMainIndex] = NULL;
		for (LONG lSideIndex = 1; lSideIndex < 1000000; lSideIndex++) {
			CHAR	*pPntDST = (CHAR *)realloc(pMyTBL[lMainIndex], sizeof(CHAR) * lSideIndex);
			if (!pPntDST) {
				if (lSideIndex == 1)	break;
				lMemSum += lSideIndex;
				MY_LOG("Memory[%ld]=(%ld) %ld\n", lMainIndex, lMemSum, lSideIndex);
				delay(1);
				break;
			}
			pMyTBL[lMainIndex] = pPntDST;
		}
	}
//---
	for (LONG lMainIndex = 0; lMainIndex < 50; lMainIndex++) {
		if (pMyTBL[lMainIndex]) free(pMyTBL[lMainIndex]);
	}
	return	lMemSum;
}
/*-- End Of File --*/
