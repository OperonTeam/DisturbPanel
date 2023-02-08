//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyTool/GPSAnalyze/MyGPSAnalyze.cpp
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#include "MyBase.h"
#include "MyDevice.h"
#include "MyJARVis.h"

//--------------------------------------------------------------------------------------------------
// クラス初期メソッド
//--------------------------------------------------------------------------------------------------
_MyGPSAnalyze::_MyGPSAnalyze()
{
}
/**/
//--------------------------------------------------------------------------------------------------
// クラス終了メソッド
//--------------------------------------------------------------------------------------------------
_MyGPSAnalyze::~_MyGPSAnalyze()
{
}
/**/
//--------------------------------------------------------------------------------------------------
// 座標（MyPoinA, MyPointB）を通過する直線と、線分（MyPointC, MyPointD）の交差を判定する。
// MY_TRUE:交差する / MY_FALSE:交差しない
//--------------------------------------------------------------------------------------------------
BOOL			_MyGPSAnalyze::IsCrossedLineA(
_MyGPSLine		*pMyLineA,
_MyGPSLine		*pMyLineB)
{
	if (((pMyLineA->MyGPSPointA.dLat - pMyLineA->MyGPSPointB.dLat) * (pMyLineB->MyGPSPointA.dLng - pMyLineA->MyGPSPointA.dLng) +
		 (pMyLineA->MyGPSPointA.dLng - pMyLineA->MyGPSPointB.dLng) * (pMyLineA->MyGPSPointA.dLat - pMyLineB->MyGPSPointA.dLat))
	   *((pMyLineA->MyGPSPointA.dLat - pMyLineA->MyGPSPointB.dLat) * (pMyLineB->MyGPSPointB.dLng - pMyLineA->MyGPSPointA.dLng) +
		 (pMyLineA->MyGPSPointA.dLng - pMyLineA->MyGPSPointB.dLng) * (pMyLineA->MyGPSPointA.dLat - pMyLineB->MyGPSPointB.dLat)) < (DOUBLE)0.0)
	{
		//------------------------------------------------------------------------------------------
		// 交差する
		//------------------------------------------------------------------------------------------
		return	MY_TRUE;
	}
	return	MY_FALSE;
}
/**/
//--------------------------------------------------------------------------------------------------
// 線分A(MyGPSLineA)と、線分B(MyGPSLineB)の交点座標を求める。
//--------------------------------------------------------------------------------------------------
BOOL			_MyGPSAnalyze::IsCrossedLine(
_MyGPSLine		*pMyLineA,
_MyGPSLine		*pMyLineB)
{
	if (this->IsCrossedLineA(pMyLineA, pMyLineB) == MY_TRUE) {
		if (this->IsCrossedLineA(pMyLineB, pMyLineA) == MY_TRUE) {
			//--------------------------------------------------------------------------------------
			// 交差する
			//--------------------------------------------------------------------------------------
			return	MY_TRUE;
		}
	}
	return	MY_FALSE;
}
/**/
//--------------------------------------------------------------------------------------------------
// 線分A(PointA, PointB)と線分B(PointC, PointD)が交差している場合、その交点座標を求める。
//--------------------------------------------------------------------------------------------------
_MyGPSPoint		_MyGPSAnalyze::QueCrossedLinePoint(
_MyGPSLine		*pMyGPSLineA,
_MyGPSLine		*pMyGPSLineB)
{
	DOUBLE	a0 = (pMyGPSLineA->MyGPSPointB.dLng - pMyGPSLineA->MyGPSPointA.dLng) / (pMyGPSLineA->MyGPSPointB.dLat - pMyGPSLineA->MyGPSPointA.dLat);
	DOUBLE	a1 = (pMyGPSLineB->MyGPSPointB.dLng - pMyGPSLineB->MyGPSPointA.dLng) / (pMyGPSLineB->MyGPSPointB.dLat - pMyGPSLineB->MyGPSPointA.dLat);
//---
	_MyGPSPoint		MyGPSPoint;
	MyGPSPoint.dLat = (a0 * pMyGPSLineA->MyGPSPointA.dLat - pMyGPSLineA->MyGPSPointA.dLng - a1 * pMyGPSLineB->MyGPSPointA.dLat + pMyGPSLineB->MyGPSPointA.dLng) / (a0 - a1);
	MyGPSPoint.dLng = (pMyGPSLineA->MyGPSPointB.dLng - pMyGPSLineA->MyGPSPointA.dLng) / (pMyGPSLineA->MyGPSPointB.dLat - pMyGPSLineA->MyGPSPointA.dLat) * (MyGPSPoint.dLat - pMyGPSLineA->MyGPSPointA.dLat) + pMyGPSLineA->MyGPSPointA.dLng;
//---
	return	MyGPSPoint;
};
/*-- End Of File --*/
