//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyTool/Config/MyConfigFile.h
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#ifndef MYCONFIGFILE_H
#define MYCONFIGFILE_H

//--------------------------------------------------------------------------------------------------
// 外部参照・依存関係インクルード・ヘッダ情報
//--------------------------------------------------------------------------------------------------
#include <ArduinoJson.h>

//--------------------------------------------------------------------------------------------------
// オフセット時刻計算用
//--------------------------------------------------------------------------------------------------
//---
#define	MYSETP_FILEPATH_FORMAT		"/AWSSetup/Config%s.json"
//---
#define	ADD_ROLL					F(".Roll")
#define	ADD_PITCH					F(".Pitch")
#define	ADD_YAW						F(".Yaw")
//---
//--------------------------------------------------------------------------------------------------
// 外部ｊｓｏｎ定義ファイル名称・アクセスキー定義
//--------------------------------------------------------------------------------------------------
#define	MYCONFIG_GPS				F("GPS")									// ＧＰＳ定義ファイル
#define	MYCONFIG_GPS_HOMELAT		F("HOME_LAT")								// HOME_LAT
#define	MYCONFIG_GPS_HOMELNG		F("HOME_LNG")								// HOME_LNG
//---
#define	MYCONFIG_NET				F("NET")									// ＮＥＴ定義ファイル
#define	MYCONFIG_NET_MODULE			F("MODULE")									// ＮＥＴモジュール使用状態(1:使用/0:未使用)
#define	MYCONFIG_NET_SSID			F("SSID")									// ＮＥＴ・ＷｉＦｉＳＳＩＤ
#define	MYCONFIG_NET_PASS			F("PASS")									// ＮＥＴ・ＷｉＦｉパスフレーズ
//---
#define	MYCONFIG_AMG				F("AMG")									// ＡＭＧ８８３３定義ファイル
#define	MYCONFIG_AMG_MODULE			F("MODULE")									// ＡＭＧ・モジュール使用状態(1:使用/0:未使用)
#define	MYCONFIG_AMG_FACE			F("FACE")									// ＡＭＧ・左右逆転モード(1:逆転/0:通常)
//---
#define	MYCONFIG_IMU				F("IMU")									// ＩＭＵ９２５０定義ファイル
#define	MYCONFIG_IMU_DEBUG			F("DEBUG")									// ＩＭＵ・開発用モード使用状態(1:使用/0:未使用)
#define	MYCONFIG_IMU_8FIGURE		F("8FIGURE")								// ＩＭＵ・磁気８の字キャリブレーション(1:使用/0:未使用)
#define	MYCONFIG_IMU_MAGDECL		F("MAGDECL")								// ＩＭＵ・磁気情報赤緯（地理的なオフセット）
#define	MYCONFIG_IMU_MAGBIAS		F("MAGBIAS")								// ＩＭＵ・磁気情報傾向
#define	MYCONFIG_IMU_MAGSCALE		F("MAGSCALE")								// ＩＭＵ・磁気情報縮尺
//---
#define	MYCONFIG_BME				F("BME")									// ＢＭＥ２８０定義ファイル
#define	MYCONFIG_BME_OFFSET			F("OFFSET")									// ＢＭＥ・温度センサーの補正値
//---
#define	MYCONFIG_TFT				F("TFT")									// ＴＦＴパネル定義ファイル
#define	MYCONFIG_TFT_SLEEPTIME		F("SLEEPTIME")								// ＴＦＴ・自動消灯時間（秒）
#define	MYCONFIG_TFT_GEOOFFSET		F("GEOOFFSET")								// ＴＦＴ・地磁気マーカー描画オフセット
#define	MYCONFIG_TFT_COLOR_NUM		10											// ＴＦＴ・速度別表示・カラーデータ数
#define	MYCONFIG_TFT_COLOR_SPEED	F("COLOR_SPEED_")							// ＴＦＴ・速度別表示・移動速度
#define	MYCONFIG_TFT_COLOR_NO		F("COLORNO_")								// ＴＦＴ・速度別表示・カラー番号
//---
//---実装中---///
#define	MYCONFIG_BLE				F("BLE");									// ＢＬＥ定義ファイル
#define	MYCONFIG_BLE_TPMSUUID		F("TPMSUUID");								// ＢＬＥ・ＴＰＭＳセンサーＵＵＩＤ
#define	MYCONFIG_BLE_TPMSFrontID	F("TPMS_FRONT")								// ＢＬＥ・ＴＰＭＳフロントＩＤ
#define	MYCONFIG_BLE_TPMSRearID		F("TPMS_REAR")								// ＢＬＥ・ＴＰＭＳリアＩＤ
//---
//---
//---
//---
//---
//---
#define	MY_CONFIG_JARVIS_			F("JARVis")									// ＪＡＲＶｉｓ定義ファイル
#define	MY_CONFIG_JARVIS_SLEEP		F("SLEEP")									// パネル・ブランク開始時間（秒）
#define	MY_CONFIG_JARVIS_ELAPSED	F("ELAPSED")								// 休憩取得・警告時間（秒）
//
#define	MY_CONFIG_TFT_IMUGEOOFFSET	F("GEOOFFSET")								// 磁気情報描画オフセット
//-> For DRAW #define	MY_CONFIG_IMU_MAGSCALE		"GEOOFFSET"					// 磁気情報赤緯（描画時の北オフセット）
//---
// 現状はハードコーデイング
#define	MY_CONFIG_TFT_THERMO_NUM	16											// サーモ表示色（設定数）
#define	MY_CONFIG_TFT_THERMO_COLOR	F("THERMO_COLOR_%02d")						// サーモ表示色
//
//
//
//--------------------------------------------------------------------------------------------------
// 環境・動作定義用ファイル・アクセスクラス
//--------------------------------------------------------------------------------------------------
class   _MyConfigFile {
	private:
		//------------------------------------------------------------------------------------------
		// 内部アクセスメソッド
		//	LoadFile						: jsonファイルを読取ってjsonデコードを行う
		//---
		//	GetBool							: BOOL型のjsonデータ値を取得する
		//	GetInt							: INT型のjsonデータ値を取得する
		//	GetLong							: LONG型のjsonデータ値を取得する
		//	GetFloat						: FLOAT型のjsonデータ値を取得する
		//	GetDouble						: DOUBLE型のjsonデータ値を取得する
		//	GetString						: String型のjsonデータ値を取得する
		//	Get3DFIMU						: _My3DFIMU型のjsonデータ値を取得する
		//------------------------------------------------------------------------------------------
		BOOL			LoadFile(String strSetupFile);
		//---
		BOOL			GetBool(String strKeyWord);
		INT				GetInt(String strKeyWord);
		INT				GetInt(String strKeyWord, INT iIndex);
		LONG			GetLong(String strKeyWord);
		FLOAT			GetFloat(String strKeyWord);
		DOUBLE			GetDouble(String strKeyWord);
		String			GetString(String strKeyWord);
		_My3DFIMU		Get3DFIMU(String strKeyword);
//---
	public:
		//------------------------------------------------------------------------------------------
		// 以下の変数へのアクセスメソッドは用意されません。（変更も可能だけど、、、ね）
		//------------------------------------------------------------------------------------------
		DateTime		dtJARVisBuild;											// ＪＡＲＶｉｓビルド日時
		_MyGPSPoint		MyGPSPointHome;											// ＧＰＳ・ランドマーク座標
		BOOL			bNET_Module;											// ＮＥＴ・モジュール使用状態(1:使用/0:未使用)
		String			strNET_SSID;											// ＮＥＴ・ＷｉＦｉＳＳＩＤ
		String			strNET_PASS;											// ＮＥＴ・ＷｉＦｉパスフレーズ
		BOOL			bAMG_Module;											// ＡＭＧ・モジュール使用状態(1:使用/0:未使用)
		BOOL			bAMG_Face;												// ＡＭＧ・左右逆転モード(1:逆転/0:通常)
		BOOL			bIMU_Debug;												// ＩＭＵ・開発用モード使用状態(1:使用/0:未使用)
		BOOL			bIMU_8Figure;											// ＩＭＵ・磁気８の字キャリブレーション(1:使用/0:未使用)
		FLOAT			fIMU_MagDecl;											// ＩＭＵ・磁気情報赤緯（地理的なオフセット）
		_My3DFIMU		My3DFMagBias;											// ＩＭＵ・磁気情報バイアス
		_My3DFIMU		My3DFMagScale;											// ＩＭＵ・磁気情報スケール
		FLOAT			fBME_Offset;											// ＢＭＥ・温度センサーの補正値
		LONG			lTFTSleepTime;											// ＴＦＴ・パネル自動消灯時間（秒）
		INT				iTFTGeoOffset;											// ＴＦＴ・磁気方位描画オフセット値
		INT				iTFTColorSpeed[MYCONFIG_TFT_COLOR_NUM][2];				// ＴＦＴ・移動速度表示色テーブル
		BOOL			bBLE_Module;											// ＢＬＥ・モジュール使用状態(1:使用/0:未使用)
		String			strBLE_TPMSUUID;										// ＢＬＥ・ＴＰＭＳ・ＵＵＩＤ
		String			strBLE_TPMSFront;										// ＢＬＥ・ＴＰＭＳ・フロント
		String			strBLE_TPMSRear;										// ＢＬＥ・ＴＰＭＳ・リア
//---
		//------------------------------------------------------------------------------------------
		// 外部公開用・アクセスメソッド
		//------------------------------------------------------------------------------------------
		_MyConfigFile();														// 初期化（アクセス定義ファイルの読み込み）
		~_MyConfigFile();														// メモリ解放

		//------------------------------------------------------------------------------------------
		// Initialize				: セットアップ情報ファイルの読み込み
		// GetSysConfigSpeedColor	: 移動速度描画表示色の取得
		// DumpTest					: セットアップ情報ファイルの読み込み・反映テスト
		//------------------------------------------------------------------------------------------
		BOOL			Initialize(CONST_CHAR *pszDate, CONST_CHAR *pszTime);	// セットアップ情報ファイルの読み込み
		INT				GetSysConfigSpeedColor(LONG lSpeed);					// 移動速度表示色情報テーブル
		VOID			DumpTest(VOID);
};
#endif
/*-- End Of File --*/
