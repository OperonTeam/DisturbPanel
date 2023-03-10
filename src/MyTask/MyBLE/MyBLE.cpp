//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyTask/MyBLE/MyBLE.cpp
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
//---
//--------------------------------------------------------------------------------------------------
// BLEクラスをコメントアウトしている件について　
// １，WioTerminalのBLEライブラリをぐるぐる回していると不穏な挙動をします。
// 　・残念ながら、この真の原因が判明出来ていません。
// ２，TPMSのプロトコルを解析していません。
// 　・TPMS電源オン時のデータは解析済みだが、一定時間以降、TPMSからセンサーデータが飛んでこない。
// 　　本家のアプリ（iOS）が存在するので挙動差異があるはずなのだが未検証。
//	 　この辺りは、Wireshark for BLEで、、、、。
//--------------------------------------------------------------------------------------------------
#include "MyBase.h"
#include "MyDevice.h"
#include "MyJARVis.h"

//--------------------------------------------------------------------------------------------------
//	2022/03/25(Fri)
//	Amazon検索の結果、２輪専用に販売されているTPMSは普通自動車用よりも高額で、BLE対応が無い。
//	本来であればタイヤ交換時にホイール内装式＋バランス取りを実施したかったのだが、無いんだよね。
//	そもそもメーカー推奨の空気圧が若干高めなので、対応可能なのか？も心配していたのだが、
//	なんとかAmazonで普通自動車用のTPMS(Bluetooth対応)を購入した。
//	以下、「TPMSあるある」なのだろうが、センサーの電源はどうなっているのか？につまずいた。
//	正解は判らないが、センサーを分解して電池を入れ直したらあっけなく始動したのは良いのだけれど、
//	どうやらたまたま購入したTPMSはビーコン(iBeacon)タイプでは無いらしい。
//	結局通信をリバースして判った範囲が以下の通り。
//	１、普通のBLEセンサ(iBeaconでは無い)
//	２、アクティブスキャンを実行するとLocalNameが取得可能で、フォーマットは以下の通り
//		TPMSn-XXXXXX
//			|    +------- : 同梱されてきたバーコードに記載された16進数で表現された 3 Byteの文字列
//			+------------ : TPMSセンサーに貼ってある番号(1-4)
//	３、属性を見ると接続可能、、、。--->ここは少し掘り下げないと駄目。
//	４、UUIDは固定で、[0000fbb0-0000-1000-8000-00805f9b34fb]
//	５、センサーデータは、ManufacturerData内に存在する。
//			-------------------------------------------
//		Addr:  0 1 2 3 4 5 6 7 8  9 A B C D E F | 10 11
//		Data: 000181EACA21A7DEE8-030000D5080000 | 58 00
//		      |- @FIX -||NAME||- KPa-|TempC|		|BAT|	 
//								  |	 |	|		  +------	: 58       -> 88   -> 88%   バッテリィー残量（％）
//								  |	 |	+----------------	: 00 08 D5 -> 2261 -> 22.61 温度（℃）
//								  |	 +---------------------	: 00 03 E8 -> 1000 -> 1.000 空気圧（KPa）
//								  +------------------------	: 21A7DE(上記２の16進数で表現された3 Byteの文字列)
//	６、不明点
//		電源投入後、60秒でTPMSセンサーがSLEEPする。
//		流石に初期不良ってことは無いと思うのだけれど、センサーの値が動かないからSLEEPするのかが判断付かずなので、ここは実車確認か？
//		先の話に戻るが、60秒間の間に接続しておいて、、、、だったりすると面倒。
//		いまのところ、様子を見ます。
//--------------------------------------------------------------------------------------------------
//	WioTemrinalのBLEデバイスを使うと戻り値が違ってるｗ
//	00:11:21.608 > _MyBLE::MyUpdate(002) Loop=[5/8]
//	00:11:21.608 >                            NAME:[]
//	00:11:21.608 > _MyBLE::Analyze1 ServiceUUID[0000fbb0-0000-1000-8000-00805f9b34fb]
//	00:11:21.608 > DUMP [_MyBLE::Analyze4 ManuData]/Size=16 Start
//	00:11:21.608 > 0000 : 80 EA CA 11 AE 8B E8 03 00 00 03 0B 00 00 5D 00  | @@@@@@@@@@@@@@]@
//									|ID   |  |KPa     | | TempC  |  |Bat| 
//	00:11:21.608 > DUMP END
//	00:11:21.608 > TPMS[-1]:[8BE803]/ 空気圧(KPa) = 0.000000 / 温度(℃) = 0.000000 / Batt = _MyBLE::MyUpdate(002) Loop=[6/8]
//
//--------------------------------------------------------------------------------------------------
//	実際のBLE実装は、
//		１、pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks())で、ハンドラを登録
//		２，チェック処理を実施
//		３，独自のタイミングで、BLEの実処理を実行
//	ってな感じがよろしいはず。
//--------------------------------------------------------------------------------------------------
//---
const int scanning_time = 1;        // スキャン時間（秒）
INT iGCount = 0;

//
//--------------------------------------------------------------------------------------------------
// クラス初期メソッド
//--------------------------------------------------------------------------------------------------
class	MyAdvertisedDeviceCallbacks	:	public BLEAdvertisedDeviceCallbacks {
	//----------------------------------------------------------------------------------------------
	//　onResult() BLE Advertised Callback
	//----------------------------------------------------------------------------------------------
	VOID	onResult(BLEAdvertisedDevice advertisedDevice) {
		MY_TRACE("Start onResult=%d", iGCount);
		if (advertisedDevice.haveServiceUUID()) {
			//--------------------------------------------------------------------------------------
			// コールバック・ダンプ（ＳｅｒｖｉｃｅＵＵＩＤ）
			//--------------------------------------------------------------------------------------
			MY_HEXDUMP((VOID *)advertisedDevice.getServiceUUID().toString().c_str(),	// Dump Data ServiceUUID
						advertisedDevice.getServiceUUID().toString().length(),			// Dump Size
						"ServiceUUID");													// Dump Title
		}
		if (advertisedDevice.haveManufacturerData()) {
			//--------------------------------------------------------------------------------------
			// コールバック・ダンプ（ＭａｎｕｆａｃｔｕｒｅＤａｔａ）
			//--------------------------------------------------------------------------------------
			MY_HEXDUMP((VOID *)advertisedDevice.getManufacturerData(),					// Dump Data Manufacture Data
						advertisedDevice.getManufacturerDataLength(),					// Dump Size
						"ManufacturerData");											// Dump Title
		}
		MY_TRACE("Exit");
	}
};

//--------------------------------------------------------------------------------------------------
// クラス初期メソッド
//--------------------------------------------------------------------------------------------------
_MyBLE::_MyBLE()
{
	this->ulReceiveByte		= (ULONG)0;									// センサー受信データバイト数
	this->ulLoopCount		= (ULONG)0;									// センサー情報ループカウンタ
}
/**/
//--------------------------------------------------------------------------------------------------
// クラス終了メソッド
//--------------------------------------------------------------------------------------------------
_MyBLE::~_MyBLE()
{
}
/**/
//--------------------------------------------------------------------------------------------------
// COMデバイスを初期化する
//--------------------------------------------------------------------------------------------------
_MyTask::_MyTaskRet	_MyBLE::MyBegin(
_MyDevice			*pMyDevice,
_MyJARVis			*pMyJARVis)
{
	//----------------------------------------------------------------------------------------------
	// ＢＬＥタスクの利用可否を確認します
	//----------------------------------------------------------------------------------------------
	_MyTaskRet	MyTaskRet = _MyTaskRet::MY_DEVICE_STATUS_OPEN;				// 正常終了
	if (! MyGlobalTool.MyConfigFile.bBLE_Module) {
		//------------------------------------------------------------------------------------------
		// 自タスクの終了をセットする　
		//------------------------------------------------------------------------------------------
		MyTaskRet = _MyTaskRet::MY_DEVICE_STATUS_CLOSE;						// 自タスクの終了
	} else {
		//------------------------------------------------------------------------------------------
		// ＢＬＥデバイスの初期化を行います。
		//------------------------------------------------------------------------------------------
		BLEDevice::init("");												// ＢＬＥデバイスを初期化
		this->pBLEScan = BLEDevice::getScan();								// スキャンオブジェクトを取得
		this->pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
		this->pBLEScan->setActiveScan(false);								// false:パッシブスキャン設定 / true:アクティブ
		this->pBLEScan->setInterval(500);									//
		this->pBLEScan->setWindow(500);										// less or equal setInterval value
	}
	//----------------------------------------------------------------------------------------------
	// デバイス管理クラスにタスク状態を通知する(MY_DEVSTAT_OPEN)
	//----------------------------------------------------------------------------------------------
	pMyDevice->SetDeviceInfo(_MyDEV_ID::MY_DEVID_BLE, _MyDEV_STAT::MY_DEVSTAT_OPEN);

	//----------------------------------------------------------------------------------------------
	// 基底（タスク）管理クラスに（自タスク終了／正常終了）返却する
	//----------------------------------------------------------------------------------------------
	return	MyTaskRet;
}
/**/
//--------------------------------------------------------------------------------------------------
// ＣＯＭセンサーの情報を更新する（センサーからの受信情報を解析する）
//--------------------------------------------------------------------------------------------------
_MyTask::_MyTaskRet	_MyBLE::MyUpdate(
_MyDevice			*pMyDevice,
_MyJARVis			*pMyJARVis)
{
	//----------------------------------------------------------------------------------------------
	// ＢＬＥスキャンを実行して目的のＵＵＩＤ（アドバタイズ）を取得する
	//----------------------------------------------------------------------------------------------
	MY_TRACE("Start LoopCount:[%d] / pBLEScan=%x", iGCount++, this->pBLEScan);
	BLEScanResults foundDevices = this->pBLEScan->start(scanning_time, false);
	this->pBLEScan->clearResults();										// delete results fromBLEScan buffer to release memory
	delay(1000);
//	this->pBLEScan->_scanProcessing;

#ifdef	_OLD_
	for (INT iCount = 0; iCount < foundDevices.getCount(); iCount++) {
		MY_LOG("BLEScan Loop(%d/%d)\n", iCount, foundDevices.getCount());
		//------------------------------------------------------------------------------------------
		// 取り出したアドバタイズ・デバイスの全件数を処理対象とする
		//------------------------------------------------------------------------------------------
		BLEAdvertisedDevice	BLEDev = foundDevices.getDevice(iCount);

		//------------------------------------------------------------------------------------------
		// 取得したアドバタイズデータからＴＰＭＳセンサー情報を分析する
		//------------------------------------------------------------------------------------------
		_MyTPMSInfo		MyTPMSInfo;
		_MyBLE::_MyTPMSType	MyTPMSType = this->AnalyzTPMSInfo(&BLEDev, &MyTPMSInfo);
		switch(MyTPMSType) {
			//--------------------------------------------------------------------------------------
			// 取得したアドバタイズデータからＴＰＭＳセンサー情報を分析する
			//--------------------------------------------------------------------------------------
			case	_MyBLE::MY_TPMS_FRONT:								// フロントタイヤ用
				pMyDevice->MyFDRHeadRec.SetTPMSInfoFront(&MyTPMSInfo);
				break;
			case	_MyBLE::MY_TPMS_REAR:								// リアタイヤ用
				pMyDevice->MyFDRHeadRec.SetTPMSInfoRear(&MyTPMSInfo);
				break;
			case	_MyBLE::MY_TPMS_NO:									// 目的のTPMSでは無い	
			default:
				break;
		}
	}
//---
	//----------------------------------------------------------------------------------------------
	// 基底（タスク）管理クラスに正常終了を返却する
	//----------------------------------------------------------------------------------------------
	this->pBLEScan->stop();													// scan stop
	this->pBLEScan->clearResults();   										// delete results fromBLEScan buffer to release memory

#endif

	MY_TRACE("Exit");
	return	_MyTask::_MyTaskRet::MY_DEVICE_STATUS_OPEN;
}
/**/
//--------------------------------------------------------------------------------------------------
// ＣＯＭセンサーをセットアップする
//--------------------------------------------------------------------------------------------------
_MyTask::_MyTaskRet	_MyBLE::MySetup(
_MyDevice			*pMyDevice,
_MyJARVis			*pMyJARVis)
{
	//----------------------------------------------------------------------------------------------
	// デバイス管理クラスにタスク状態を通知する(MY_FDELOGID_SETUP)
	//----------------------------------------------------------------------------------------------
	pMyDevice->SetDeviceInfo(_MyDEV_ID::MY_DEVID_BLE, _MyDEV_STAT::MY_DEVSTAT_SETUP);
//---
	delay(500);
//---
	//----------------------------------------------------------------------------------------------
	// デバイス管理クラスにタスク状態を通知する(MY_DEVSTAT_OPEN)
	//----------------------------------------------------------------------------------------------
	pMyDevice->SetDeviceInfo(_MyDEV_ID::MY_DEVID_BLE, _MyDEV_STAT::MY_DEVSTAT_OPEN);

	//----------------------------------------------------------------------------------------------
	// タスク状態を通知する
	//----------------------------------------------------------------------------------------------
	return	_MyTask::_MyTaskRet::MY_DEVICE_STATUS_OPEN;
}
/**/
//--------------------------------------------------------------------------------------------------
// ＴＰＭＳで取得したセンサーデータ値をデコードする
//--------------------------------------------------------------------------------------------------
_MyBLE::_MyTPMSType	_MyBLE::AnalyzTPMSInfo(
BLEAdvertisedDevice	*pBLEDev,
_MyTPMSInfo			*pMyTPMSInfo)
{
	//----------------------------------------------------------------------------------------------
	// スキャンしたＵＵＩＤ情報を取得して、目的のＴＰＭＳデータであるか判断する
	//----------------------------------------------------------------------------------------------
	_MyBLE::_MyTPMSType	MyTPMSType = _MyBLE::MY_TPMS_NO;				// 目的のTPMSでは無い

#ifdef	MY_DEBUG_BLE_DUMP_DETAIL
//---
if(pBLEDev->getName().length() > 1) {
	MY_HEXDUMP((VOID*)pBLEDev->getName().c_str(), pBLEDev->getName().length(), "getName");
}
if(pBLEDev->haveServiceUUID()) {
	MY_HEXDUMP((VOID*)pBLEDev->getServiceUUID().toString().c_str(), pBLEDev->getServiceUUID().toString().length(), "ServiceUUID");
}
if(pBLEDev->haveServiceData()) {
	MY_HEXDUMP((VOID*)pBLEDev->getServiceData(), pBLEDev->getServiceDataLength(), "ServiceData");
}
if(pBLEDev->haveManufacturerData()) {
	MY_HEXDUMP((VOID*)pBLEDev->getManufacturerData(), pBLEDev->getManufacturerDataLength(), "ManufacturerData");
}
#endif

//---
//---
//---
	if ((pBLEDev->haveServiceUUID())&&
		(!strncmp(MyGlobalTool.MyConfigFile.strBLE_TPMSUUID.c_str(), pBLEDev->getServiceUUID().toString().c_str(), 8))) {
		//------------------------------------------------------------------------------------------
		// ＴＰＭＳ ManufacturerDataから目的のセンサーデータを取得する。
		//------------------------------------------------------------------------------------------
		std::string	strManuData = (CHAR *)pBLEDev->getManufacturerData();

		//------------------------------------------------------------------------------------------
		// TPMSの内部IDを取得する(3バイト)
		//------------------------------------------------------------------------------------------
		CHAR	szWork[8];
		sprintf(szWork, "%02X%02X%02X", strManuData[3], strManuData[4], strManuData[5]);
		MY_LOG("szWork[%s]\n", szWork);
		//---
		if (!strcmp(szWork, MyGlobalTool.MyConfigFile.strBLE_TPMSFront.c_str()))	MyTPMSType = _MyBLE::MY_TPMS_FRONT;
		if (!strcmp(szWork, MyGlobalTool.MyConfigFile.strBLE_TPMSRear.c_str()))		MyTPMSType = _MyBLE::MY_TPMS_FRONT;
		//---
		//------------------------------------------------------------------------------------------
		// TPMS空気圧を取得する(KPa) リトルエンディアン変換を行う
		//------------------------------------------------------------------------------------------
		MY_LOG("[%02X%02X%02X]\n", strManuData[6],strManuData[7],strManuData[8]);
		pMyTPMSInfo->fAirPressure = (FLOAT)(strManuData[8] << 16 | strManuData[7]  << 8 | strManuData[6])  / 1000.00;

		//------------------------------------------------------------------------------------------
		// TPMS温度を取得する(℃) リトルエンディアン変換を行う
		//------------------------------------------------------------------------------------------
		MY_LOG("[%02X%02X%02X]\n", strManuData[9],strManuData[10],strManuData[11]);
		pMyTPMSInfo->fTemperature = (FLOAT)(strManuData[11] << 16 | strManuData[10] << 8 | strManuData[9]) / 100.00;

		//------------------------------------------------------------------------------------------
		// TPMSの内蔵バッテリィ残率を取得する(%) 
		//------------------------------------------------------------------------------------------
		pMyTPMSInfo->cBattery = strManuData[14];
		MY_LOG("TPMS[%d] / 空気圧(KPa) = %f / 温度(℃) = %f / Batt = %d\n", MyTPMSType, pMyTPMSInfo->fAirPressure, pMyTPMSInfo->fTemperature, (INT)pMyTPMSInfo->cBattery);
	}
	return	MyTPMSType;
}
/**/
//--------------------------------------------------------------------------------------------------
// ＢＬＥセンサーの状態を出力する
//--------------------------------------------------------------------------------------------------
VOID				_MyBLE::MyDump(VOID)
{
	//----------------------------------------------------------------------------------------------
	// ＢＬＥタスクの基本情報をダンプ出力する
	//----------------------------------------------------------------------------------------------
	MY_TRACE("Start");
	this->MyDumpBase();
//
	//----------------------------------------------------------------------------------------------
	// ＢＬＥスキャン・ダンプを実行する
	//----------------------------------------------------------------------------------------------
	pBLEScan->setActiveScan(true);										// アクティブスキャンに設定変更
	BLEScanResults foundDevices = pBLEScan->start(scanning_time);
	for (INT iCount = 0; iCount < foundDevices.getCount(); iCount++) {
		//------------------------------------------------------------------------------------------
		// 取得したイニシエータ情報をダンプ出力する
		//------------------------------------------------------------------------------------------
		BLEAdvertisedDevice dev = foundDevices.getDevice(iCount);
		MY_LOG("---[%02d/%02d]---\n", iCount, foundDevices.getCount());
		MY_LOG(" Name:[%s] UUID:[%s]\n", dev.getName().c_str(), dev.getServiceUUID(0).toString().c_str());
		MyGlobalTool.HexDump((VOID *)dev.getManufacturerData(), dev.getServiceDataLength(), "ManufacturerData");
	}
	MY_TRACE("Exit");
}
/*-- End Of File --*/