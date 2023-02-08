//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyTool/GPSAnalyze/MyGPSAnalyze.h
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#ifndef MYGPSANALYZ_H
#define MYGPSANALYZ_H

//--------------------------------------------------------------------------------------------------
// ＧＰＳ座標解析用クラス
//--------------------------------------------------------------------------------------------------
class	_MyGPSAnalyze {
	private:
	public:
		_MyGPSAnalyze();
		~_MyGPSAnalyze();

		//------------------------------------------------------------------------------------------
		// 分析・解析用インタフェース
		//	IsCrossedLineA				２つの線分が接触しているか否か？
		//	IsCrossedLine				２つの線分が接触しているか否か？
		//	QueCrossedLinePoint			２つの線分が交差している場合に、交点座標を求める
		//------------------------------------------------------------------------------------------
		BOOL					IsCrossedLineA(_MyGPSLine *pMyLineA, _MyGPSLine *pMyLineB);
		BOOL					IsCrossedLine(_MyGPSLine *pMyLineA, _MyGPSLine *pMyLineB);
		_MyGPSPoint				QueCrossedLinePoint(_MyGPSLine *pMyGPSLineA, _MyGPSLine *pMyGPSLineB);
};
#endif
/*-- End Of File --*/
