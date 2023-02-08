//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyJARVis/Geolocation/MyGeolocation.cpp
//  Copyright 2019. 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#ifdef	MY_LAPTIME_PROGRAM
#include "MyBase.h"
#include "MyDevice.h"
#include "MyJARVis.h"

//--------------------------------------------------------------------------------------------------
// 拡張ライブラリ系の基本インクルード
//--------------------------------------------------------------------------------------------------
#include <ArduinoJson.h>

//--------------------------------------------------------------------------------------------------
// クラス初期メソッド
//--------------------------------------------------------------------------------------------------
_MyGeolocation::_MyGeolocation()
{
	this->sControlInfoNum		= 0;
}
/**/
//--------------------------------------------------------------------------------------------------
// クラス終了メソッド
//--------------------------------------------------------------------------------------------------
_MyGeolocation::~_MyGeolocation()
{
}
/**/
//--------------------------------------------------------------------------------------------------
// GPS移動情報は、登録済みコントロールラインと交差したか？
//--------------------------------------------------------------------------------------------------
BOOL				_MyGeolocation::IsControlLineCrossed(
_MyGPSLine			*pMyGPSMovePoint,
SHORT				*psIndex,
String				*pstrControlLineName,
_MyGPSPoint			*pMyGPSCrossPoint)
{
	_MyGPSLine		MyGPSLine;
	String			strName = "";
	BOOL			bRet = MY_FALSE;

	//--------------------------------------------------------------------------------------
	// 検索対象は、登録済みコントロールライン座標（Outer-Inner）情報
	//--------------------------------------------------------------------------------------
	for (SHORT sIndex = 0; sIndex < this->sControlInfoNum; sIndex++) {
		//----------------------------------------------------------------------------------
		// コントロールライン座標（Outer-Inner）情報と、GPS移動情報との交差を判断する
		//----------------------------------------------------------------------------------
		strName = this->GetRegistControlLine(sIndex, &MyGPSLine);
		if (MyGlobalTool.MyGPSAnalyz.IsCrossedLine(pMyGPSMovePoint, &MyGPSLine)) {
			//------------------------------------------------------------------------------
			// ２つの線分が交差している場合（≒コンロトールラインを通過した）
			//------------------------------------------------------------------------------
			*psIndex = sIndex;
			*pstrControlLineName = strName;
			MY_LOG("_MyGeolocation::Update : Crossed[%s] Raw[%11.7lf][%11.7lf] - [%11.7lf][%11.7lf]  TO Reg[%11.7lf][%11.7lf] - [%11.7lf][%11.7lf]\n",
							strName.c_str(),
							pMyGPSMovePoint->MyGPSPointA.dLat,		pMyGPSMovePoint->MyGPSPointA.dLng,
							pMyGPSMovePoint->MyGPSPointB.dLat,		pMyGPSMovePoint->MyGPSPointB.dLng,
							MyGPSLine.MyGPSPointA.dLat,				MyGPSLine.MyGPSPointA.dLng,
							MyGPSLine.MyGPSPointB.dLat,				MyGPSLine.MyGPSPointB.dLng);
			bRet = MY_TRUE;

			//------------------------------------------------------------------------------
			// 交点座標を求める
			//------------------------------------------------------------------------------
			*pMyGPSCrossPoint = MyGlobalTool.MyGPSAnalyz.QueCrossedLinePoint(pMyGPSMovePoint, &MyGPSLine);
			break;
		}
	}
	return	bRet;
}
/**/
//--------------------------------------------------------------------------------------------------
// 登録済みコントロールライン座標情報を取得する
//--------------------------------------------------------------------------------------------------
String				_MyGeolocation::GetRegistControlLine(SHORT sIndex, _MyGPSLine *pMyGPSLine)
{
	//----------------------------------------------------------------------------------------------
	// 登録済みコントロールライン座標情報を返却する
	//----------------------------------------------------------------------------------------------
	pMyGPSLine->MyGPSPointA.dLat = this->MyGPSLine[sIndex].MyGPSPointA.dLat;
	pMyGPSLine->MyGPSPointA.dLng = this->MyGPSLine[sIndex].MyGPSPointA.dLng;
	pMyGPSLine->MyGPSPointB.dLat = this->MyGPSLine[sIndex].MyGPSPointB.dLat;
	pMyGPSLine->MyGPSPointB.dLng = this->MyGPSLine[sIndex].MyGPSPointB.dLng;
	return	this->strControlTitle[sIndex];
}
/**/
//--------------------------------------------------------------------------------------------------
// コントロールライン情報ファイルを読み込む
//--------------------------------------------------------------------------------------------------
BOOL				_MyGeolocation::ReadControlLineFile(VOID)
{
	//----------------------------------------------------------------------------------------------
	// CSVファイル・フォーマット情報を設定する
	//---
	//	"ControlNum": 2,
	//		"No000": { "Title":"筑波サーキット", "GPSLine": [36.150315, 139.919500, 36.150292, 139.919680]	},
	//		"No001": { "Title":"鈴鹿サーキット", "GPSLine": [36.150315, 139.919500, 36.150292, 139.919680]	}
	//----------------------------------------------------------------------------------------------
	// 外部コントロール情報定義ファイルをオープンする
	//----------------------------------------------------------------------------------------------
	SDFile	MySDFile = (SDFile)MYDEV_SD.open(F("/AWSSetup/ControlLine.json"), FILE_READ);
	if (! MySDFile) {
		//------------------------------------------------------------------------------------------
		// CSVファイルをオープン出来なかった場合
		//------------------------------------------------------------------------------------------
		return	MY_FALSE;
	}

	//----------------------------------------------------------------------------------------------
	// 外部コントロール情報定義ファイルをオープンする
	//----------------------------------------------------------------------------------------------
	DynamicJsonDocument		MyJsonDoc(2048);								// JSON制御クラス
	String	StreamBuff = MySDFile.readString();								// ファイルを全て読み出す
	MySDFile.close();
	//---
	DeserializationError	error = deserializeJson(MyJsonDoc, StreamBuff);	// JSONを解析
	if (error) {
		//------------------------------------------------------------------------------------------
		// JSON解析が異常だった場合
		//------------------------------------------------------------------------------------------
MY_TRACE("Json devoceErr[%s]", error.c_str());
		return	MY_FALSE;
	}

	//----------------------------------------------------------------------------------------------
	// コントロールライン情報の登録数を取得する
	//----------------------------------------------------------------------------------------------
	String		strTitle;
	CHAR		szSubKey[8];
	this->sControlInfoNum = MyJsonDoc[F("ControlNum")].as<SHORT>();				// データ件数を取得
MY_LOG("ControlNum-001-[%d]\n", this->sControlInfoNum);

	//----------------------------------------------------------------------------------------------
	// コントロールライン情報の登録数を取得する
	//----------------------------------------------------------------------------------------------
	for (SHORT	sIndex = 0; sIndex < this->sControlInfoNum; sIndex++) {
		//------------------------------------------------------------------------------------------
		// JSON索引キーを作成して、JSONデータを取り出す　
		//------------------------------------------------------------------------------------------
		sprintf(szSubKey, "No%03d", sIndex);
		//---
		if (sIndex < 10) {
			this->strControlTitle[sIndex]				= MyJsonDoc[szSubKey][F("Title")].as<STRING>();
			this->MyGPSLine[sIndex].MyGPSPointA.dLat	= MyJsonDoc[szSubKey][F("GPSLine")][0].as<DOUBLE>();
			this->MyGPSLine[sIndex].MyGPSPointA.dLng	= MyJsonDoc[szSubKey][F("GPSLine")][1].as<DOUBLE>();
			this->MyGPSLine[sIndex].MyGPSPointB.dLat	= MyJsonDoc[szSubKey][F("GPSLine")][2].as<DOUBLE>();
			this->MyGPSLine[sIndex].MyGPSPointB.dLng	= MyJsonDoc[szSubKey][F("GPSLine")][3].as<DOUBLE>();
		}
		MY_LOG("No[%02d] [%s] [%f, %f] - [%f, %f]\n",
									sIndex,
									this->strControlTitle[sIndex].c_str(),
									this->MyGPSLine[sIndex].MyGPSPointA.dLat,
									this->MyGPSLine[sIndex].MyGPSPointA.dLng,
									this->MyGPSLine[sIndex].MyGPSPointB.dLat,
									this->MyGPSLine[sIndex].MyGPSPointB.dLng);
	}
	return	MY_TRUE;
}
#endif
/*-- End Of File --*/
